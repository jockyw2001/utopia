//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halTSP.c
// @brief  Transport Stream Processer (TSP) HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "halTSP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define TSP_HAL_REG_SAFE_MODE       1                                   // Register protection access between 1 task and 1+ ISR

//[HWBUG]
#define MULTI_ACCESS_SW_PATCH       1                                   // It's still risk becuase some registers like readaddr will

// @FIXME: remove the test later
#define LINUX_TEST                  0
                                                                        // cause overflow before patching to correct value.
#define MIU_BUS                     4

#define VQ_PACKET_UNIT_LEN          208

#define SWPATCH_FOR_READ_PIDFLT     1

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl*    _TspCtrl  = NULL;
static REG_Ctrl2*   _TspCtrl2 = NULL;
static REG_Ctrl3*   _TspCtrl3 = NULL;
static REG_Ctrl4*   _TspCtrl4 = NULL;
static REG_Ctrl5*   _TspCtrl5 = NULL;

static REG_TS_Sample* _TspSample = NULL;

static MS_U32       _u32RegBase = 0;
static MS_U32       _u32PMRegBase = 0;


extern MS_BOOL      _bIsHK;
static MS_S32       _s32HALTSPMutexId = -1;

static MS_BOOL      _bTsPadUsed[3] = {FALSE, FALSE, FALSE};
static MS_U16       _16MobfKey = 0;

//--------------------------------------------------------------------------------------------------
//  SW patch for HW can not read #64~#71 PID filters
//  Record PID filter value to SW table for using
//--------------------------------------------------------------------------------------------------
#if SWPATCH_FOR_READ_PIDFLT
static MS_U32                       _u32PidFlt_L[TSP_PIDFLT_EXT_NUM] = {[0 ... (TSP_PIDFLT_EXT_NUM-1)] = 0x1FFF};
static MS_U32                       _u32PidFlt_H[TSP_PIDFLT_EXT_NUM] = {[0 ... (TSP_PIDFLT_EXT_NUM-1)] = 0};
#endif

//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFF);                          \
                                    (reg)->H = ((value) >> 16); } while(0)

#define _HAL_REG32L_W(reg, value)   (reg)->data = ((value) & 0x0000FFFF);

#define _HAL_REG16_W(reg, value)    (reg)->u16data = (value);


#define _HAL_HALTSP_ENTRY()         do{ \
                                        if(_s32HALTSPMutexId == -1)         \
                                            _s32HALTSPMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "HALTSP_Mutex", MSOS_PROCESS_SHARED); \
                                    }while(0)
#define _HAL_HALTSP_EXIT()          do{  \
                                        if(_s32HALTSPMutexId != -1)                                     \
                                        {   \
                                            MsOS_DeleteMutex(_s32HALTSPMutexId);    _s32HALTSPMutexId = -1; \
                                        }   \
                                    }while(0)

#define _HAL_HALTSP_LOCK()              (MsOS_ObtainMutex(_s32HALTSPMutexId, MSOS_WAIT_FOREVER))
#define _HAL_HALTSP_UNLOCK()            (MsOS_ReleaseMutex(_s32HALTSPMutexId))

#define _HAL_TSP_PIDFLT(EngId, FltId)     (&(_TspPid[EngId].Flt[FltId]))
#define _HAL_TSP_PIDFLT_H(EngId, FltId)   (&(_TspPid_H[EngId].Flt[FltId]))

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))

//#define MASK(x)                 (((1<<(x##_BITS))-1) << x##_SHIFT)
//#define BIT(x)                  (1<<(x))
//#define BMASK(bits)                 (BIT(((1)?bits)+1)-BIT(((0)?bits)))
// #define BMASK_L(bits)               (BMASK(bits)&0xFFFF)
// #define BMASK_H(bits)               (BMASK(bits)>>16)
//#define BITS(bits,value)            ((BIT(((1)?bits)+1)-BIT(((0)?bits))) & (value<<((0)?bits)))

#define PMTOP_REG(addr)             (*((volatile MS_U16*)(_u32PMRegBase + 0x3c00 + ((addr)<<2))))
    #define REG_PMTOP_CHIPID                            0x00
        #define REG_PMTOP_CHIPID_MASK                   0xFFFF
    #define REG_PMTOP_CHIPVERSION                       0x01
        #define REG_PMTOP_CHIP_VERSION_MASK             0x00FF
        #define REG_PMTOP_CHIP_REVISION_MASK            0xFF00
        #define REG_PMTOP_CHIP_REVISION_SHIFT           8

#define TSP_INT_REG(addr)           (*((volatile MS_U16*)(_u32RegBase + 0x3200 + ((addr)<<2))))

#define TSP_CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_u32RegBase + 0x1600 + ((addr)<<2))))
    #define REG_CLKGEN0_DC0_SYTNTH              0x05
        #define REG_CLKGEN0_STC_CW_SEL          0x0002
        #define REG_CLKGEN0_STC_CW_EN           0x0004
    #define REG_CLKGEN0_DC0_STC_CW_L            0x06
    #define REG_CLKGEN0_DC0_STC_CW_H            0x07

    #define REG_CLKGEN0_TSN_CLK                 0x28
        #define REG_CLKGEN0_TSN_CLK_MASK        0x1F
        #define REG_CLKGEN0_TSN_CLK_DEMOD       0x1C
        #define REG_CLKGEN0_TSN_CLK_TS0_SHIFT   0
        #define REG_CLKGEN0_TSN_CLK_TS1_SHIFT   8
        //#define TSP_CLK_TS0                        0
        //#define TSP_CLK_TS1                        1
        //#define TSP_CLK_TS2                        2
        //#define TSP_CLK_TSO_OUT                6
        //#define TSP_CLK_INDEMOD                 7
        // bit[4:0] -> ts0  -> 0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 0: TS0
        //                                     1: TS1
        //                                     2: TS2
        //                                     6: TSOOUT
        //                                     7: Internal Demmod
        // bit[12:8] -> ts1 -> 0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 0: TS0
        //                                     1: TS1
        //                                     2: TS2
        //                                     6: TSOOUT
        //                                     7: Internal Demmod
    #define REG_CLKGEN0_TSN_CLKTS4              0x29    //clk of S2p
        #define REG_CLKGEN0_TSN_CLK_TS4_SHIFT   8
        // bit[4:0] -> ts2  -> 0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 0: TS0
        //                                     1: TS1
        //                                     2: TS2
        //                                     6: TSOOUT
        //                                     7: Internal Demmod
#define TSP_CLKGEN1_REG(addr)       (*((volatile MS_U16*)(_u32RegBase + 0x6600 + ((addr)<<2))))
    #define REG_CLKGEN1_TSN_CLKFI               0x26
        #define REG_CLKGEN1_TSN_CLK_TSFI_SHIFT  8
        // bit[12:8] -> tsfi  -> 0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 0: TS0
        //                                     1: TS1
        //                                     2: TS2
        //                                     6: TSOOUT
        //                                     7: Internal Demmod

#define TSP_TOP_REG(addr)           (*((volatile MS_U16*)(_u32RegBase + 0x3c00 + ((addr)<<2))))

    #define REG_TOP_TS_OUT_MODE                 0x51    //For ts1 out configure
        #define REG_TOP_TS_OUT_MODE_MASK                0x0100
            #define REG_TOP_TS1_OUT_MODE_TSO            0x0100

    //#define REG_TOP_TS_TS0TS1_CFG_MASK                0x07
    #define REG_TOP_TSCONFIG                0x51
        #define REG_TOP_TS0CFG_SHIFT                    9
        #define REG_TOP_TS_TS0_CFG_MASK                 0x03
        #define REG_TOP_TS_TS0_PARALL_IN                1
        #define REG_TOP_TS_TS0_SERIAL_IN                2

        #define REG_TOP_TS1CFG_SHIFT                    11
        #define REG_TOP_TS_TS1_CFG_MASK                 0x07
            #define REG_TOP_TS_TS1_PARALL_IN                1
            #define REG_TOP_TS_TS1_PARALL_OUT               2
            #define REG_TOP_TS_TS1_SERIAL_IN                3

    #define REG_TOP_TS2CONFIG                0x54
        #define REG_TOP_TS2CFG_SHIFT                    4
        #define REG_TOP_TS_TS2_CFG_MASK                 0x03
        #define REG_TOP_TS_TS2_PARALL_IN                1
        #define REG_TOP_TS_TS2_SERIAL_IN                2
        #define REG_TOP_TSCFG_DISABLE_PAD               0


#define TSP_TSO0_REG(addr)           (*((volatile MS_U16*)(_u32RegBase + 0x27400UL + ((addr)<<2UL))))
    #define REG_TSO0_CFG0                       0x1C
        #define REG_TSO0_CFG0_S2PCFG_MASK                   0x001F
        #define REG_TSO0_CFG0_S2PCFG_S2P_EN                 0x0001
        #define REG_TSO0_CFG0_S2PCFG_S2P_TSSIN_C0           0x0002
        #define REG_TSO0_CFG0_S2PCFG_S2P_TSSIN_C1           0x0004
        #define REG_TSO0_CFG0_S2PCFG_S2P_3WIRE_MODE         0x0008
        #define REG_TSO0_CFG0_S2PCFG_S2P_BYPASS             0x0010
        #define REG_TSO0_CFG0_S2P0_CFG_SHIFT                0


#define TSP_TS_SAMPLE_REG(addr)           (*((volatile MS_U16*)(_u32RegBase + 0x21600 + ((addr)<<2))))
    #define REG_TSO_OUT_CLK_SEL                 0x30
    #define REG_TSO_OUT_CLK_SEL_MASK            1
        #define REG_TSO_OUT_TSO                             0x0000
        #define REG_TSO_OUT_S2P                             0x0001


#define ABS_DIFF(x1, x2)            (((x1) > (x2))? ((x1) - (x2)) : ((x2) - (x1)))
#define IsCover(_start1, _end1, _start2, _end2) (ABS_DIFF(_end1, _end2) < (((_end1) > (_end2))?((_end1)-(_start1)):((_end2)-(_start2))) )?TRUE:FALSE
//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------
static void _HAL_TSP_FW_load(
    MS_U32                     u32FwAddrPhys,
    MS_U32                     u32FwSize,
    MS_BOOL                    bFwDMA,
    MS_BOOL                    bIQmem,
    MS_BOOL                    bDQmem);

static void _HAL_TSP_tsif_select(MS_U8 u8_tsif);
// static void _HAL_TSP_SelPad(MS_U32 u32EngId, MS_U32 PadId);

//--------------------------------------------------------------------------------------------------
//  Implementation
//--------------------------------------------------------------------------------------------------
static void _delay(void)
{
    volatile MS_U32 i;
    for (i = 0; i< 0xFFFF; i++);
}

static MS_U32 _HAL_REG32_R(REG32 *reg)
{
    MS_U32     value = 0;
    value  = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

static MS_U16 _HAL_REG32L_R(REG32_L *reg)
{
    MS_U16     value;
    value = (reg)->data;
    return value;
}

static MS_U16 _HAL_REG16_R(REG16 *reg)
{
    MS_U16     value;
    value = (reg)->u16data;
    return value;
}

static REG_SecFlt* _HAL_TSP_SECFLT(MS_U32 u32EngId, MS_U32 u32FltId)
{
    return (&(_TspSec1[u32EngId].Flt[u32FltId]));
}

static void _HAL_TSP_tsif_select(MS_U8 u8_tsif)
{
    switch(u8_tsif)
    {
        default:
        case 0:
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF0_ENABLE));
            break;
        case 1:
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
            break;
        case 3: //TS_FI
            _HAL_REG16_W(&_TspCtrl5[0].Ts_If_Fi_Cfg,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].Ts_If_Fi_Cfg), TSP_FIIF_EN|TSP_FIIF_MUX_LIVE_PATH));
            break;
    }
}

#define REG16_T(addr)           (*((volatile MS_U16*)(addr)))
#define ADDR_INDR_CTRL          (_u32RegBase+ 0x2b20)
#define ADDR_INDR_ADDR0         (_u32RegBase+ 0x2b24)
#define ADDR_INDR_ADDR1         (_u32RegBase+ 0x2b28)
#define ADDR_INDR_WRITE0        (_u32RegBase+ 0x2b2c)
#define ADDR_INDR_WRITE1        (_u32RegBase+ 0x2b30)
#define ADDR_INDR_READ0         (_u32RegBase+ 0x2b34)
#define ADDR_INDR_READ1         (_u32RegBase+ 0x2b38)

#define ADDR_MOBF_FILEIN        (_u32RegBase+ 0x2a2cUL)

#if 0
#define XBYTE_1591              (_u32RegBase+ 0x2a0c)  // TsRec_Head21_Mid20
#define XBYTE_15A4              (_u32RegBase+ 0x2a10)  // TsRec_Mid21_Tail20
#define XBYTE_15A6              (_u32RegBase+ 0x2b48)  // TsRec_Mid

void HAL_TSP_HW_Lock_Init(void)
{
    REG16_T(XBYTE_1591) = 0;
    REG16_T(XBYTE_15A4) = 0;
    REG16_T(XBYTE_15A6) = 0;
}

void _HAL_TSP_HW_Lock(void)
{
#ifdef MCU_HK
    REG16_T(XBYTE_1591) = 0xFF;
    REG16_T(XBYTE_15A4) = 0xFF;
    while (REG16_T(XBYTE_15A4) && REG16_T(XBYTE_15A6));
    REG16_T(XBYTE_1591) = 0xFF;
#else // MIPS HK
    REG16_T(XBYTE_15A6) = 0xFF;
    REG16_T(XBYTE_15A4) = 0x00;
    while (REG16_T(XBYTE_1591) && (REG16_T(XBYTE_15A4)==0));
#endif
}

void _HAL_TSP_HW_Unlock(void)
{
#ifdef MCU_HK
    REG16_T(XBYTE_1591) = 0x00;
#else
    REG16_T(XBYTE_15A6) = 0x00;
#endif
}

#undef XBYTE_1591
#undef XBYTE_15A4
#undef XBYTE_15A6

#else

#define TSP_SEM_AEON              (_u32RegBase+ 0xC1480)  //sw_mail_box0
#define TSP_SEM_ORDER             (_u32RegBase+ 0xC1484)  // sw_mail_box1
#define TSP_SEM_MIPS              (_u32RegBase+ 0xC1488)  // sw_mail_box2

void HAL_TSP_HW_Lock_Init(void)
{
    REG16_T(TSP_SEM_AEON) = 0;
    REG16_T(TSP_SEM_MIPS) = 0;
    REG16_T(TSP_SEM_ORDER) = 0;

    _HAL_HALTSP_ENTRY();
}

static MS_BOOL _HAL_TSP_HW_TryLock(MS_BOOL bInit)
{
    if (_bIsHK)
    {
        if (bInit)
        {
            REG16_T(TSP_SEM_AEON) = 0xFFFF;
            REG16_T(TSP_SEM_ORDER) = 0xFFFF;
        }
        if (REG16_T(TSP_SEM_ORDER) && REG16_T(TSP_SEM_MIPS))
        {
            // REG16_T(TSP_SEM_AEON) = 0x0000;
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        if (bInit)
        {
            REG16_T(TSP_SEM_MIPS) = 0xFFFF;
            REG16_T(TSP_SEM_ORDER) = 0x00;
        }
        if ((REG16_T(TSP_SEM_ORDER) ==0) && (REG16_T(TSP_SEM_AEON)))
        {
            // REG16_T(TSP_SEM_MIPS) = 0x0000;
            return FALSE;
        }
        return TRUE;
    }
}

static void _HAL_TSP_HW_Lock(void)
{
    if (FALSE == _HAL_TSP_HW_TryLock(TRUE))
    {
        while (FALSE == _HAL_TSP_HW_TryLock(FALSE));
    }
}

static void _HAL_TSP_HW_Unlock(void)
{
    if (_bIsHK)
    {
        REG16_T(TSP_SEM_AEON) = 0x00;
    }
    else
    {
        REG16_T(TSP_SEM_MIPS) = 0x00;
    }
}

void HAL_TSP_HW_Lock_Release(void)
{
    REG16_T(TSP_SEM_AEON) = 0x00;
    REG16_T(TSP_SEM_MIPS) = 0x00;

    _HAL_HALTSP_EXIT();


}

MS_BOOL HAL_TSP_TTX_IsAccess(MS_U32 u32Try)
{
    if(u32Try == 0)
        return FALSE;

    if (_bIsHK)
    {
        if ( REG16_T(TSP_SEM_AEON))
        {
            return FALSE;
        }
    }
    else
    {
        if (REG16_T(TSP_SEM_MIPS))
        {
            return FALSE;
        }
    }

    if (_HAL_TSP_HW_TryLock(TRUE))
    {
        return TRUE;
    }

    _HAL_TSP_HW_Unlock();
    return FALSE;
}

void HAL_TSP_TTX_UnlockAccess(void)
{
    _HAL_TSP_HW_Unlock();
}

#undef TSP_SEM_AEON
#undef TSP_SEM_MIPS
#undef TSP_SEM_ORDER

#endif

MS_U32 HAL_REG32_IndR(REG32 *reg)
{
    MS_U32 u32Ret;

    _HAL_TSP_HW_Lock();
    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   ((((MS_U32)reg)>> 1) & 0xFFFF);
    REG16_T(ADDR_INDR_ADDR1)=   ((((MS_U32)reg)>> 17) & 0xFFFF);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);

    // get read value
    u32Ret = ((MS_U32)(REG16_T(ADDR_INDR_READ0))| ((MS_U32)(REG16_T(ADDR_INDR_READ1)<< 16)));

    _HAL_HALTSP_UNLOCK();
    _HAL_TSP_HW_Unlock();

    return u32Ret;
}

MS_U32 HAL_REG32_IndR_tmp(REG32 *reg)
{
    MS_U32 u32Ret;

    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   ((((MS_U32)reg)>> 1) & 0xFFFF);
    REG16_T(ADDR_INDR_ADDR1)=   ((((MS_U32)reg)>> 17) & 0xFFFF);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);

    // get read value
    u32Ret = ((MS_U32)(REG16_T(ADDR_INDR_READ0))| ((MS_U32)(REG16_T(ADDR_INDR_READ1)<< 16)));

    _HAL_HALTSP_UNLOCK();

    return u32Ret;
}
void HAL_REG32_IndW_tmp(REG32 *reg, MS_U32 value)
{
    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   ((((MS_U32)reg)>> 1) & 0xFFFF);
    REG16_T(ADDR_INDR_ADDR1)=   ((((MS_U32)reg)>> 17) & 0xFFFF);

    // set write value
    REG16_T(ADDR_INDR_WRITE0)=  (value & 0xFFFF);
    REG16_T(ADDR_INDR_WRITE1)=  ((value >> 16) & 0xFFFF);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);

    _HAL_HALTSP_UNLOCK();
}

void HAL_REG32_IndW(REG32 *reg, MS_U32 value)
{
    _HAL_TSP_HW_Lock();

    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   ((((MS_U32)reg)>> 1) & 0xFFFF);
    REG16_T(ADDR_INDR_ADDR1)=   ((((MS_U32)reg)>> 17) & 0xFFFF);

    // set write value
    REG16_T(ADDR_INDR_WRITE0)=  (value & 0xFFFF);
    REG16_T(ADDR_INDR_WRITE1)=  ((value >> 16) & 0xFFFF);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);

    _HAL_HALTSP_UNLOCK();

    _HAL_TSP_HW_Unlock();
}

#define ADDR_HWINT2         (_u32RegBase+ 0x2db0)
static void _HAL_TSP_HwInt2_BitClr(MS_U16 u16ClrBit)
{
    REG16_T(ADDR_HWINT2) = (REG16_T(ADDR_HWINT2) | 0xFF00) & ~u16ClrBit;
}

static void _HAL_TSP_HwInt2_BitSet(MS_U16 u16Bit)
{
    REG16_T(ADDR_HWINT2) = (REG16_T(ADDR_HWINT2) | 0xFF00) | u16Bit;
}
#undef ADDR_HWINT2

#if (TSP_HWPCR_BY_HK == 0 && defined(HWPCR_ENABLE))
static void _HAL_TSP_CMD_Write_HWPCR_Reg(MS_U32 u32mask, MS_U32 u32data)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, u32mask);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data1, u32data);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_HWPCR_REG_SET);

    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
    {
        _delay();
    }
}
#endif

static MS_U16 u16LastAddr0, u16LastAddr1, u16LastWrite0, u16LastWrite1, u16LastRead0, u16LastRead1;

static MS_U32                       _u32PidFltBuf[((TSP_PIDFLT_NUM_ALL) * 2 * sizeof(REG_PidFlt))>> 3];
static MS_U32                       _u32SecFltBuf[TSP_SECFLT_NUM*((sizeof(REG_SecFlt)-sizeof((((REG_SecFlt*)0)->_x50)))>> 3)];

#ifdef HWPCR_ENABLE
static MS_U32                       _u32PcrFltBuf[2];
#endif

//[LEGACY] //[OBSOLETE]
MS_BOOL                             _bIsHK = TRUE;
//[LEGACY] //[OBSOLETE]

void HAL_TSP_SaveFltState(void)
{
    MS_U32 u32EngId;
    MS_U32 i, j;
    MS_U32 u32SecEnd = ((size_t)&(((REG_SecFlt*)0)->_x50))/sizeof(REG32);
    REG_PidFlt *pPidFilter;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (i = 0; i < TSP_PIDFLT_NUM_ALL; i++)
        {
            j = i << 1;

            if(i < TSP_PIDFLT_NUM)
            {
                pPidFilter = _HAL_TSP_PIDFLT(u32EngId, i);
                _u32PidFltBuf[j] = HAL_REG32_IndR(pPidFilter);
                pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, i);
                _u32PidFltBuf[j + 1] = HAL_REG32_IndR(pPidFilter);
            }
            else
            {
                #if (SWPATCH_FOR_READ_PIDFLT == 0)
                pPidFilter = _HAL_TSP_PIDFLT(u32EngId, i);
                _u32PidFltBuf[j] = HAL_REG32_IndR(pPidFilter);
                pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, i);
                _u32PidFltBuf[j + 1] = HAL_REG32_IndR(pPidFilter);
                #else
                _u32PidFltBuf[j] = _u32PidFlt_L[i - TSP_PIDFLT_NUM];
                _u32PidFltBuf[j + 1] = _u32PidFlt_H[i - TSP_PIDFLT_NUM];
                #endif
            }
        }
#ifdef HWPCR_ENABLE
        _u32PcrFltBuf[0] = _HAL_REG32_R(&(_TspCtrl3[0].PIDFLR_PCR[0]));
        _u32PcrFltBuf[1] = _HAL_REG32_R(&(_TspCtrl3[0].PIDFLR_PCR[1]));
#endif
        j = 0;
        for (i = 0; i < TSP_SECFLT_NUM; i++)
        {
            REG32* pRegStart = (REG32*)_HAL_TSP_SECFLT(u32EngId, i);
            REG32* pRegEnd = pRegStart + u32SecEnd;
            REG32* pReg = pRegStart;
            while (pReg < pRegEnd)
            {
                _u32SecFltBuf[j] = HAL_REG32_IndR(pReg);
                j++;
                pReg++;
            }
        }
    }
}

void HAL_TSP_RestoreFltState(void)
{
    MS_U32 u32EngId;
    MS_U32 i, j;
    MS_U32 u32SecEnd = ((size_t)&(((REG_SecFlt*)0)->_x50))/sizeof(REG32);
    REG_PidFlt *pPidFilter;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (i = 0; i < TSP_PIDFLT_NUM_ALL; i++)
        {
            j = i << 1;
            pPidFilter = _HAL_TSP_PIDFLT(u32EngId, i);
            HAL_REG32_IndW(pPidFilter, _u32PidFltBuf[j]);
            pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, i);
            HAL_REG32_IndW(pPidFilter, _u32PidFltBuf[j + 1]);
        }
#ifdef HWPCR_ENABLE
        _HAL_REG32_W(&(_TspCtrl3[0].PIDFLR_PCR[0]), _u32PcrFltBuf[0]);
        _HAL_REG32_W(&(_TspCtrl3[0].PIDFLR_PCR[1]), _u32PcrFltBuf[1]);
#endif
        j = 0;
        for (i = 0; i < TSP_SECFLT_NUM; i++)
        {
            REG32* pRegStart = (REG32*) _HAL_TSP_SECFLT(u32EngId, i);
            REG32* pRegEnd = pRegStart + u32SecEnd;
            REG32* pReg = pRegStart;
            while (pReg < pRegEnd)
            {
                HAL_REG32_IndW(pReg, _u32SecFltBuf[j]);
                j++;
                pReg++;
            }
        }
    }
}

void HAL_TSP_ISR_SAVE_ALL(void)
{
    // save address
    u16LastAddr0=               (MS_U16)REG16_T(ADDR_INDR_ADDR0);
    u16LastAddr1=               (MS_U16)REG16_T(ADDR_INDR_ADDR1);

    // save write
    u16LastWrite0=              (MS_U16)REG16_T(ADDR_INDR_WRITE0);
    u16LastWrite1=              (MS_U16)REG16_T(ADDR_INDR_WRITE1);

    // save read
    u16LastRead0=               (MS_U16)REG16_T(ADDR_INDR_READ0);
    u16LastRead1=               (MS_U16)REG16_T(ADDR_INDR_READ1);
}

void HAL_TSP_ISR_RESTORE_ALL(void)
{
    // restore read
    REG16_T(ADDR_INDR_READ0)=   u16LastRead0;
    REG16_T(ADDR_INDR_READ1)=   u16LastRead1;

    // restore write
    REG16_T(ADDR_INDR_WRITE0)=  u16LastWrite0;
    REG16_T(ADDR_INDR_WRITE1)=  u16LastWrite1;

    // restore addr
    REG16_T(ADDR_INDR_ADDR0)=   u16LastAddr0;
    REG16_T(ADDR_INDR_ADDR1)=   u16LastAddr1;
}
#undef ADDR_INDR_CTRL
#undef ADDR_INDR_ADDR0
#undef ADDR_INDR_ADDR1
#undef ADDR_INDR_WRITE0
#undef ADDR_INDR_WRITE1
#undef ADDR_INDR_READ0
#undef ADDR_INDR_READ1

//--------------------------------------------------------------------------------------------------
// For MISC part
//--------------------------------------------------------------------------------------------------
void HAL_TSP_ORAcess_Optimize(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_OPT_ORACESS_TIMING));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_OPT_ORACESS_TIMING));
    }
}

void HAL_TSP_CSA_Set_ScrmPath(MS_U8 u8EngId, MS_U32 u32ScrmPath)
{
    //printf("[%s] u8EngId %d, u32ScrmPath %lx\n", __FUNCTION__, (int)u8EngId, u32ScrmPath);
    switch(u8EngId)
    {
        case 0:
            _HAL_REG32_W(&_TspCtrl[0].CA_CTRL, (_HAL_REG32_R(&_TspCtrl[0].CA_CTRL) & ~TSP_CA0_CTRL_MASK) | (u32ScrmPath & TSP_CA0_CTRL_MASK));
            break;
        case 1:
            _HAL_REG32_W(&_TspCtrl[0].CA_CTRL, (_HAL_REG32_R(&_TspCtrl[0].CA_CTRL) & ~TSP_CA1_CTRL_MASK) | (u32ScrmPath & TSP_CA1_CTRL_MASK));
            break;
        default:
            break;
    }
}

MS_U32 HAL_TSP_CSA_Get_ScrmPath(MS_U8 u8EngId)
{
    switch(u8EngId)
    {
        case 0:
            return (_HAL_REG32_R(&_TspCtrl[0].CA_CTRL) & TSP_CA0_CTRL_MASK);
        case 1:
            return (_HAL_REG32_R(&_TspCtrl[0].CA_CTRL) & TSP_CA1_CTRL_MASK);
            break;
        default:
            break;
    }

    return 0;
}

MS_BOOL HAL_TSP_CSA_Set_CACtrl(MS_U8 u8EngId, MS_U8 u8SrcTSIF, MS_U32 u32Dst)
{
    MS_U32 u32CAInTS0Live[2]    = {TSP_CA0_INPUT_TSIF0_LIVEIN,TSP_CA1_INPUT_TSIF0_LIVEIN};
    MS_U32 u32CAInTS0file[2]    = {TSP_CA0_INPUT_TSIF0_FILEIN,TSP_CA1_INPUT_TSIF0_FILEIN};
    MS_U32 u32CAInTS1[2]        = {TSP_CA0_INPUT_TSIF1,TSP_CA1_INPUT_TSIF1};
    MS_U32 u32CAOutPktDmx0L[2]  = {TSP_CA0_OUTPUT_PKTDMX0_LIVE,TSP_CA1_OUTPUT_PKTDMX0_LIVE};
    MS_U32 u32CAOutPktDmx0F[2]  = {TSP_CA0_OUTPUT_PKTDMX0_FILE,TSP_CA1_OUTPUT_PKTDMX0_FILE};
    MS_U32 u32CAOutPktDmx1[2]   = {TSP_CA0_OUTPUT_PKTDMX1,TSP_CA1_OUTPUT_PKTDMX1};
    MS_U32 u32CACtrl = 0;

    if(u8EngId >= TSP_CA_ENGINE_NUM)
    {
        return FALSE;
    }

    switch(u8SrcTSIF)
    {
        case TSP_SRC_FROM_TSIF0_LIVE:
            u32CACtrl = u32CAInTS0Live[u8EngId];
            break;
        case TSP_SRC_FROM_TSIF0_FILE:
            u32CACtrl = u32CAInTS0file[u8EngId];
            break;
        case TSP_SRC_FROM_TSIF1:
            u32CACtrl = u32CAInTS1[u8EngId];
            break;
        default:
            return FALSE;
    }
    switch(u32Dst)
    {
        case TSP_PKTDMX0_LIVE:
            u32CACtrl |= u32CAOutPktDmx0L[u8EngId];
            break;
        case TSP_PKTDMX0_FILE:
            u32CACtrl |= u32CAOutPktDmx0F[u8EngId];
            break;
        case TSP_PKTDMX1:
            u32CACtrl |= u32CAOutPktDmx1[u8EngId];
            break;
        default:
            return FALSE;
    }

    HAL_TSP_CSA_Set_ScrmPath(u8EngId, u32CACtrl);

    return TRUE;
}

MS_BOOL HAL_TSP_CSA_Get_CACtrl(MS_U8 u8EngId, MS_U8* pu8SrcTSIF, MS_U32* pu32Dst)
{
    MS_U32 u32CAInTS0Live[2]    = {TSP_CA0_INPUT_TSIF0_LIVEIN,TSP_CA1_INPUT_TSIF0_LIVEIN};
    MS_U32 u32CAInTS0file[2]    = {TSP_CA0_INPUT_TSIF0_FILEIN,TSP_CA1_INPUT_TSIF0_FILEIN};
    MS_U32 u32CAInTS1[2]        = {TSP_CA0_INPUT_TSIF1,TSP_CA1_INPUT_TSIF1};
    MS_U32 u32CAOutPktDmx0L[2]  = {TSP_CA0_OUTPUT_PKTDMX0_LIVE,TSP_CA1_OUTPUT_PKTDMX0_LIVE};
    MS_U32 u32CAOutPktDmx0F[2]  = {TSP_CA0_OUTPUT_PKTDMX0_FILE,TSP_CA1_OUTPUT_PKTDMX0_FILE};
    MS_U32 u32CAOutPktDmx1[2]   = {TSP_CA0_OUTPUT_PKTDMX1,TSP_CA1_OUTPUT_PKTDMX1};
    MS_U32 u32ScmbPath = 0;

    *pu8SrcTSIF = 0;
    *pu32Dst = 0;

    if(u8EngId >= TSP_CA_ENGINE_NUM)
    {
        return FALSE;
    }

    u32ScmbPath = HAL_TSP_CSA_Get_ScrmPath(u8EngId);

    if(u32ScmbPath & u32CAInTS0Live[u8EngId])
    {
        *pu8SrcTSIF = TSP_SRC_FROM_TSIF0_LIVE;
    }
    else if(u32ScmbPath & u32CAInTS0file[u8EngId])
    {
        *pu8SrcTSIF = TSP_SRC_FROM_TSIF0_FILE;
    }
    else if(u32ScmbPath & u32CAInTS1[u8EngId])
    {
        *pu8SrcTSIF = TSP_SRC_FROM_TSIF1;
    }

    if(u32ScmbPath & u32CAOutPktDmx0L[u8EngId])
    {
        *pu32Dst = TSP_PKTDMX0_LIVE;
    }
    else if(u32ScmbPath & u32CAOutPktDmx0F[u8EngId])
    {
        *pu32Dst = TSP_PKTDMX0_FILE;
    }
    else if(u32ScmbPath & u32CAOutPktDmx1[u8EngId])
    {
        *pu32Dst = TSP_PKTDMX1;
    }

    return TRUE;
}

//--------------------------------------------------------------------------------------------------
// For PID filter part
//--------------------------------------------------------------------------------------------------
MS_U32 HAL_TSP_PidFlt_GetFltOutput(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);
        return (HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_OUT_MASK);
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);
        return (HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_OUT_MASK);
        #else
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        return (_u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] & TSP_PIDFLT_OUT_MASK);
        #endif
    }
}

MS_U32 HAL_TSP_PidFlt_GetPid(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    MS_U32 u32PID;

    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);
        u32PID = (HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_PID_MASK) >> TSP_PIDFLT_PID_SHFT;
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);
        u32PID = (HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_PID_MASK) >> TSP_PIDFLT_PID_SHFT;
        #else
        u32PID = (_u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] & TSP_PIDFLT_PID_MASK) >> TSP_PIDFLT_PID_SHFT;
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, u32PID %lx PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, u32PID, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        #endif
    }

    return u32PID;
}

void HAL_TSP_PidFlt_SetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32PID)
{
    REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);

    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_PID_MASK) | ((u32PID << TSP_PIDFLT_PID_SHFT) & TSP_PIDFLT_PID_MASK));
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_PID_MASK) | ((u32PID << TSP_PIDFLT_PID_SHFT) & TSP_PIDFLT_PID_MASK));
        #else
        _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] = (_u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] & ~TSP_PIDFLT_PID_MASK) | ((u32PID << TSP_PIDFLT_PID_SHFT) & TSP_PIDFLT_PID_MASK);
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, u32PID %lx PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, u32PID, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        HAL_REG32_IndW((REG32 *)pPidFilter, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        #endif
    }
}

void HAL_TSP_PidFlt_SelFltOutput(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32FltOutput)
{
    REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);

    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_OUT_MASK) | (u32FltOutput & TSP_PIDFLT_OUT_MASK));
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_OUT_MASK) | (u32FltOutput & TSP_PIDFLT_OUT_MASK));
        #else
        _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] = (_u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] & ~TSP_PIDFLT_OUT_MASK) | (u32FltOutput & TSP_PIDFLT_OUT_MASK);
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, u32FltOutput %lx PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, u32FltOutput, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        HAL_REG32_IndW((REG32 *)pPidFilter, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        #endif
    }
}

void HAL_TSP_PidFlt_SelSecFlt(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32SecFltId)
{
    REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, u32PidFltId);

    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        HAL_REG32_IndW((REG32 *)pPidFilter,
            (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_SECFLT_MASK) | ((u32SecFltId << TSP_PIDFLT_SECFLT_SHFT) & TSP_PIDFLT_SECFLT_MASK));
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        HAL_REG32_IndW((REG32 *)pPidFilter,
            (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_SECFLT_MASK) | ((u32SecFltId << TSP_PIDFLT_SECFLT_SHFT) & TSP_PIDFLT_SECFLT_MASK));
        #else
        _u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM] = (_u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM] & ~TSP_PIDFLT_SECFLT_MASK) | ((u32SecFltId << TSP_PIDFLT_SECFLT_SHFT) & TSP_PIDFLT_SECFLT_MASK);
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, u32SecFltId %lx PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, u32SecFltId, _u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM]);
        HAL_REG32_IndW((REG32 *)pPidFilter, _u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM]);
        #endif
    }
}

MS_U32 HAL_TSP_PidFlt_GetSecFlt(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, u32PidFltId);
        return ((HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_SECFLT_MASK) >> TSP_PIDFLT_SECFLT_SHFT);
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, u32PidFltId);
        return ((HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_SECFLT_MASK) >> TSP_PIDFLT_SECFLT_SHFT);
        #else
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, _u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM]);
        return ((_u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM] & TSP_PIDFLT_SECFLT_MASK) >> TSP_PIDFLT_SECFLT_SHFT);
        #endif
    }
}

void HAL_TSP_PidFlt_SelFltSource(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32FltSource)
{
    REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT(u32EngId, u32PidFltId);

    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_IN_MASK) | (u32FltSource & TSP_PIDFLT_IN_MASK));
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_IN_MASK) | (u32FltSource & TSP_PIDFLT_IN_MASK));
        #else
        _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] = ((_u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM] & ~TSP_PIDFLT_IN_MASK) | (u32FltSource & TSP_PIDFLT_IN_MASK));
        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, u32FltSource %lx PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, u32FltSource, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        HAL_REG32_IndW((REG32 *)pPidFilter, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        #endif
    }
}

void HAL_TSP_PidFlt_SetFltSrcStreamID(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32SrcStrId)
{
    REG_PidFlt* pPidFilter = _HAL_TSP_PIDFLT_H(u32EngId, u32PidFltId);

    if(u32PidFltId < TSP_PIDFLT_NUM)
    {
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_IN_SRC_MASK) | ((u32SrcStrId << TSP_PIDFLT_IN_SRC_SHFT) & TSP_PIDFLT_IN_SRC_MASK));
    }
    else
    {
        #if (SWPATCH_FOR_READ_PIDFLT == 0)
        HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_IN_SRC_MASK) | ((u32SrcStrId << TSP_PIDFLT_IN_SRC_SHFT) & TSP_PIDFLT_IN_SRC_MASK));
        #else
        _u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM] = (_u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM] & ~TSP_PIDFLT_IN_SRC_MASK) | ((u32SrcStrId << TSP_PIDFLT_IN_SRC_SHFT) & TSP_PIDFLT_IN_SRC_MASK);

        //printf("[%s][%d] u32EngId %ld, u32PidFltId %lx, u32SrcStrId %lx PIDFLT %lx\n", __FUNCTION__, __LINE__, u32EngId, u32PidFltId, u32SrcStrId, _u32PidFlt_L[u32PidFltId-TSP_PIDFLT_NUM]);
        HAL_REG32_IndW((REG32 *)pPidFilter, _u32PidFlt_H[u32PidFltId-TSP_PIDFLT_NUM]);
        #endif
    }
}

void HAL_TSP_PidFlt_SetHWPcrPid(MS_U32 u32EngId, MS_U32 u32PID)
{
    _HAL_REG32_W(&(_TspCtrl3[0].PIDFLR_PCR[u32EngId]), (_HAL_REG32_R(&(_TspCtrl3[0].PIDFLR_PCR[u32EngId])) & ~TSP_PIDFLT_PCR_PID_MASK) | u32PID);
}

MS_U32  HAL_TSP_PidFlt_GetHWPcrPid(MS_U32 u32EngId)
{
    return (_HAL_REG32_R(&_TspCtrl3[0].PIDFLR_PCR[u32EngId]) & TSP_PIDFLT_PCR_PID_MASK);
}

void HAL_TSP_PidFlt_HWPcrFlt_Enable(MS_U32 u32EngId, MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG32_W(&(_TspCtrl3[0].PIDFLR_PCR[u32EngId]),
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl3[0].PIDFLR_PCR[u32EngId]), TSP_PIDFLT_PCR_EN));
    }
    else
    {
        _HAL_REG32_W(&(_TspCtrl3[0].PIDFLR_PCR[u32EngId]),
                RESET_FLAG1(_HAL_REG32_R(&(_TspCtrl3[0].PIDFLR_PCR[u32EngId])), TSP_PIDFLT_PCR_EN));
    }
}

//--------------------------------------------------------------------------------------------------
// For section filter part
//--------------------------------------------------------------------------------------------------
void HAL_TSP_SecFlt_SetType(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32FltType)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, (HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & ~TSP_SECFLT_TYPE_MASK) | (u32FltType << TSP_SECFLT_TYPE_SHFT));
}

void HAL_TSP_SecFlt_ResetState(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & ~(TSP_SECFLT_STATE_MASK));
}

void HAL_TSP_SecFlt_SetRmnCount(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32RmnCount)
{
    REG_SecFlt* pSecFilter = (REG_SecFlt *)_HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    HAL_REG32_IndW((REG32 *)&pSecFilter->RmnReqCnt, (HAL_REG32_IndR((REG32 *)&pSecFilter->RmnReqCnt) & ~TSP_SECFLT_RMNCNT_MASK) |
                                    ((u32RmnCount << TSP_SECFLT_RMNCNT_SHFT) & TSP_SECFLT_RMNCNT_MASK));
}

void HAL_TSP_SecFlt_ClrCtrl(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, 0);
}

#define ADDR_SWINT2_L           (_u32RegBase+ 0x2db4)
#define ADDR_SWINT2_H           (_u32RegBase+ 0x2db8)
MS_U32 HAL_TSP_SW_INT_STATUS(void)
{
    if (_bIsHK)
    {
        return _HAL_REG32_R(&_TspCtrl[0].SwInt_Stat);
    }
    else
    {
        MS_U32 u32SwIntStatus = (MS_U32)(REG16_T(ADDR_SWINT2_L) & 0xFFFF);
        u32SwIntStatus |= (((MS_U32)(REG16_T(ADDR_SWINT2_H) & 0xFFFF)) << 16);
        return u32SwIntStatus;
    }
}
#undef ADDR_SWINT2_L
#undef ADDR_SWINT2_H

// match mask --> 0 will compare
void HAL_TSP_SecFlt_SetMask(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Mask)
{
    MS_U32 i;
    MS_U32 u32Temp;
    MS_U32 j;
    REG_SecFlt* pSecFilter = (REG_SecFlt *)_HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp =  (pu8Mask[j]) | (pu8Mask[j+ 1] << 8  ) | (pu8Mask[j+ 2] << 16  )| (pu8Mask[j+ 3] << 24);
        HAL_REG32_IndW((REG32 *)&pSecFilter->Mask[i], u32Temp);
    }
}

// not match mask --> 1 will compare
void HAL_TSP_SecFlt_SetNMask(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8NMask)
{
    MS_U32 u32Temp;

    // fix using #17 section filter, fw als using filter #17 for NMask pattern writing
    REG_SecFlt* ptempSecFlt = _HAL_TSP_SECFLT(u32EngId, TSP_NMATCH_FLTID);

    u32Temp =  (pu8NMask[0x0]) | (pu8NMask[0x1] << 8  ) | (pu8NMask[0x2] << 16  )| (pu8NMask[0x3] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[0]), u32Temp);

    u32Temp =  (pu8NMask[0x4]) | (pu8NMask[0x5] << 8  ) | (pu8NMask[0x6] << 16  )| (pu8NMask[0x7] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[1]), u32Temp);

    u32Temp =  (pu8NMask[0x8]) | (pu8NMask[0x9] << 8  ) | (pu8NMask[0xa] << 16  )| (pu8NMask[0xb] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[2]), u32Temp);

    u32Temp =  (pu8NMask[0xc]) | (pu8NMask[0xd] << 8  ) | (pu8NMask[0xe] << 16  )| (pu8NMask[0xf] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[3]), u32Temp);

    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_NMATCH | u32SecFltId);

    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
}

void HAL_TSP_SecFlt_SetMatch(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Match)
{
    MS_U32 i;
    MS_U32 u32Temp;
    MS_U32 j;

    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp =  (pu8Match[j]) | (pu8Match[j+ 1] << 8  ) | (pu8Match[j+ 2] << 16  )| (pu8Match[j+ 3] << 24);
        HAL_REG32_IndW((REG32 *)&pSecFilter->Match[i], u32Temp);
    }
}

void HAL_TSP_SecFlt_SetReqCount(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32ReqCount)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);

    HAL_REG32_IndW((REG32 *)&pSecFilter->RmnReqCnt, (HAL_REG32_IndR((REG32 *)&pSecFilter->RmnReqCnt) & ~TSP_SECFLT_REQCNT_MASK) |
                                    ((u32ReqCount << TSP_SECFLT_REQCNT_SHFT) & TSP_SECFLT_REQCNT_MASK));
}

void HAL_TSP_SecFlt_SetMode(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32SecFltMode)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, (HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & ~TSP_SECFLT_MODE_MASK) | ((u32SecFltMode << TSP_SECFLT_MODE_SHFT) & TSP_SECFLT_MODE_MASK));
}

MS_U32 HAL_TSP_SecFlt_GetCRC32(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    return HAL_REG32_IndR((REG32 *)&pSecFilter->CRC32);
}

MS_U32 HAL_TSP_SecFlt_GetState(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    return ((HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & TSP_SECFLT_STATE_MASK) >> TSP_SECFLT_STATE_SHFT);
}

MS_U32 HAL_TSP_SecFlt_GetMode(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    return ((HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & TSP_SECFLT_MODE_MASK) >> TSP_SECFLT_MODE_SHFT);
}

void HAL_TSP_SecFlt_PcrReset(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) | TSP_SECFLT_PCRRST);
}

void HAL_TSP_SecFlt_VerReset(MS_U32 u32SecFltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_VER_RESET | u32SecFltId);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
}

void HAL_TSP_SecFlt_SetDataAddr(MS_U32 u32DataAddr)
{
    MS_U32 u32cmd = TSP_MCU_CMD_MEM_HIGH_ADDR | ((u32DataAddr & 0xFFFF0000) >> 16);

    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32cmd);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));

    u32cmd = TSP_MCU_CMD_MEM_LOW_ADDR | (u32DataAddr & 0xFFFF);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32cmd);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
}


//--------------------------------------------------------------------------------------------------
//  For section buffer part
//--------------------------------------------------------------------------------------------------
// To avoid SW read hidden HW byte enable information.
#define _TSP_SEC_BUF_ADDR_START(pSecFilter)     (TSP_SECFLT_BUFSTART_MASK & HAL_REG32_IndR((REG32 *)&((pSecFilter)->BufStart)))

void HAL_TSP_SecBuf_SetBuffer(MS_U32 u32EngId, MS_U32 u32SecBufId, MS_U32 u32StartAddr, MS_U32 u32BufSize)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufStart, u32StartAddr);
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufEnd, u32StartAddr + u32BufSize);
}

void HAL_TSP_SecBuf_SetBufRead(MS_U32 u32EngId, MS_U32 u32SecBufId, MS_U32 u32ReadAddr)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufRead, u32ReadAddr);
}

MS_U32 HAL_TSP_SecBuf_GetBufStart(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    return _TSP_SEC_BUF_ADDR_START(pSecBuf);
}

MS_U32 HAL_TSP_SecBuf_GetBufEnd(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    return HAL_REG32_IndR((REG32 *)&pSecBuf->BufEnd);
}

MS_U32 HAL_TSP_SecBuf_GetBufCur(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    return HAL_REG32_IndR((REG32 *)&pSecBuf->BufCur);
}

void HAL_TSP_SecBuf_ResetBuffer(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);

    HAL_REG32_IndW((REG32 *)&pSecBuf->BufCur, _TSP_SEC_BUF_ADDR_START(pSecBuf));
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufRead, _TSP_SEC_BUF_ADDR_START(pSecBuf));
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufWrite, _TSP_SEC_BUF_ADDR_START(pSecBuf));
}

MS_U32  HAL_TSP_SecBuf_GetBufRead(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    return HAL_REG32_IndR((REG32 *)&pSecBuf->BufRead);
}

MS_U32  HAL_TSP_SecBuf_GetBufWrite(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    return HAL_REG32_IndR((REG32 *)&pSecBuf->BufWrite);
}

#undef _TSP_SEC_BUF_ADDR_START

void HAL_TSP_SecBuf_SetBufRead_tmp(MS_U32 u32EngId, MS_U32 u32SecBufId, MS_U32 u32ReadAddr)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    HAL_REG32_IndW_tmp((REG32 *)&pSecBuf->BufRead, u32ReadAddr);
}

MS_U32  HAL_TSP_SecBuf_GetBufWrite_tmp(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt* pSecBuf = _HAL_TSP_SECFLT(u32EngId, u32SecBufId);
    return HAL_REG32_IndR_tmp((REG32 *)&pSecBuf->BufWrite);
}

//--------------------------------------------------------------------------------------------------
// For DMA part
//--------------------------------------------------------------------------------------------------
//[HW LIMIT][HW TODO] TsDma pause can not be access by TSP CPU
//[HW LIMIT][HW TODO] TsDma pause it hard to control because read/write in different register
//[HW LIMIT][HW TODO] When setting TsDma it should be disable interrupt
void HAL_TSP_TsDma_SetDelay(MS_U32 u32Delay)
{
    // Richard: the file in timer in Uranus is 24 bits.
    //          to simplify the process, writing 32 bits directly.
    //          HW will truncate the high 8 bits out, and use low 24 bits only (from Albert Lin)
    if(u32Delay == 0)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_FILEIN_BYTETIMER_ENABLE));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].TsFileIn_Timer, (u32Delay & TSP_FILE_TIMER_MASK));
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_FILEIN_BYTETIMER_ENABLE));
    }

}

void HAL_TSP_CmdQ_TsDma_SetAddr(MS_U32 u32StreamAddr)
{
    _HAL_REG32_W(&_TspCtrl[0].TsDma_Addr, u32StreamAddr);
}

MS_BOOL HAL_TSP_CmdQ_TsDma_SetSize(MS_U32 u32StreamSize)
{
    _HAL_REG32_W(&_TspCtrl[0].TsDma_Size, u32StreamSize);
    return TRUE;
}

void HAL_TSP_CmdQ_TsDma_Start(MS_U32 u32TsDmaCtrl)
{
    // enable filein byte timer
    //_HAL_REG32_W(&_TspCtrl[0].reg15b4,
    //        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_FILEIN_BYTETIMER_ENABLE));
    REG16_T(ADDR_MOBF_FILEIN) = _16MobfKey;
    _HAL_REG32_W(&_TspCtrl[0].TsDma_Ctrl_CmdQ, TSP_TSDMA_CTRL_START);
}

void HAL_TSP_TsDma_Pause(void)
{
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_PAUSE));
}

void HAL_TSP_TsDma_Resume(void)
{
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_PAUSE));
}

MS_U32 HAL_TSP_CmdQ_TsDma_GetState(void)
{
    return (HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ), TSP_TSDMA_CTRL_START) |
            (MS_U32)HAS_FLAG(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_PAUSE));
}

MS_U32 HAL_TSP_CmdQ_EmptyCount(void)
{
    return (TSP_CMDQ_SIZE - ((_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_CMDQ_CNT_MASK)>>TSP_CMDQ_CNT_SHFT));
}

void HAL_TSP_SetCtrlMode(MS_U32 u32EngId, MS_U32 u32Mode, MS_U32 u32TsIfId)
{
    // Control bits:
    // TSP_CTRL_CPU_EN
    // TSP_CTRL_SW_RST
    // TSP_CTRL_MEM_DMA_EN

    // for file in related setting
    if(u32Mode == 0)
    {
        _HAL_REG32_W(&_TspCtrl[u32EngId].TSP_Ctrl,
                (_HAL_REG32_R(&_TspCtrl[u32EngId].TSP_Ctrl) & ~(TSP_CTRL_CPU_EN   |
                                                                TSP_CTRL_SW_RST   |
                                                                TSP_CTRL_TSFILE_EN)));
        HAL_TSP_filein_enable(FALSE);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[u32EngId].TSP_Ctrl,
                (_HAL_REG32_R(&_TspCtrl[u32EngId].TSP_Ctrl) & ~(TSP_CTRL_CPU_EN   |
                                                                TSP_CTRL_SW_RST   |
                                                                //TSP_CTRL_TSFILE_EN |
//[URANUS]                                                      TSP_CTRL_CLK_GATING_DISABLE |
// @FIXME: Richard ignore this at this stage
                                                                0                  )) | u32Mode);
        if(HAS_FLAG(u32Mode, TSP_CTRL_TSFILE_EN))
            HAL_TSP_filein_enable(TRUE);
    }

    if (TSP_IF_NUM > u32TsIfId)
    {
        _HAL_TSP_tsif_select(HAS_FLAG(u32Mode, (MS_U8)(u32TsIfId & 0xFF)));
    }
}

//--------------------------------------------------------------------------------------------------
//  For PVR part
//  0: PVR1      1: PVR2      2: PVR_CB
//--------------------------------------------------------------------------------------------------
void HAL_TSP_PVR_SetBuffer(MS_U8 u8PVRId, MS_U32 u32BufStart0, MS_U32 u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    MS_U32  u32BufEnd = u32BufStart0+ u32BufSize0;

    switch(u8PVRId)
    {
        case 0:
        default:
            _HAL_REG32_W(&_TspCtrl[0].TsRec_Head, (u32BufStart0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            _HAL_REG32_W(&_TspCtrl[0].TsRec_Tail, (u32BufEnd>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            u32BufEnd = u32BufStart1+ u32BufSize1;

#define ADDR_PVR_HEAD20         (_u32RegBase+ 0x2a04)
#define ADDR_PVR_HEAD21         (_u32RegBase+ 0x2a08)
#define ADDR_PVR_MID20          (_u32RegBase+ 0x2a0c)
#define ADDR_PVR_MID21          (_u32RegBase+ 0x2a10)
#define ADDR_PVR_TAIL20         (_u32RegBase+ 0x2a14)
#define ADDR_PVR_TAIL21         (_u32RegBase+ 0x2a18)
    REG16_T(ADDR_PVR_HEAD20)=   (u32BufStart1>> MIU_BUS) & (TSP_HW_PVR_BUF_HEAD20_MASK >> TSP_HW_PVR_BUF_HEAD20_SHFT);
    REG16_T(ADDR_PVR_HEAD21)=   (u32BufStart1>> (MIU_BUS+ 16)) & TSP_HW_PVR_BUF_HEAD21_MASK;
    REG16_T(ADDR_PVR_TAIL20)=   (u32BufEnd>> MIU_BUS) & (TSP_HW_PVR_BUF_TAIL20_MASK >> TSP_HW_PVR_BUF_TAIL20_SHFT);
    REG16_T(ADDR_PVR_TAIL21)=   (u32BufEnd>> (MIU_BUS+ 16)) & TSP_HW_PVR_BUF_TAIL21_MASK;
#undef ADDR_PVR_HEAD20
#undef ADDR_PVR_HEAD21
#undef ADDR_PVR_MID20
#undef ADDR_PVR_MID21
#undef ADDR_PVR_TAIL20
#undef ADDR_PVR_TAIL21

            _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_PVR1_PINGPONG));
            break;
        case 1:
            _HAL_REG32_W(&_TspCtrl[0].Str2mi_head1_pvr2, (u32BufStart0>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            _HAL_REG32_W(&_TspCtrl[0].Str2mi_tail1_pvr2, (u32BufEnd>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            _HAL_REG32_W(&_TspCtrl[0].Str2mi_head2_pvr2, (u32BufStart1>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);
            u32BufEnd = u32BufStart1+ u32BufSize1;
            _HAL_REG32_W(&_TspCtrl[0].Str2mi_tail2_pvr2, (u32BufEnd>> MIU_BUS) & TSP_STR2MI2_ADDR_MASK);

            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_REG_PINGPONG_EN));
            break;

    }

    // flush PVR buffer
    HAL_TSP_PVR_WaitFlush(u8PVRId);

}

void HAL_TSP_PVR_Enable(MS_U8 u8PVRId, MS_BOOL bEnable)
{
    REG32 *pRegPVREn = 0;
    REG32 *pRegTSIFEn = 0;
    REG32 *pRegBurstLen = 0;
    MS_U32 u32PVRFlag = 0, u32TSIFFlag = 0, u32BurstLen = 0, u32BurstMask = 0;

    //set burst len = 1
    switch(u8PVRId)
    {
        case 0:
        default:
            pRegBurstLen = &_TspCtrl[0].reg15b4;
            u32BurstLen  = TSP_BURST_LEN_4;
            u32BurstMask = TSP_BURST_LEN_MASK;
            pRegPVREn = &_TspCtrl[0].Hw_Config4;
            u32PVRFlag = TSP_HW_CFG4_PVR_ENABLE;
            pRegTSIFEn = &_TspCtrl[0].Hw_Config4;
            u32TSIFFlag = TSP_HW_CFG4_TSIF1_ENABLE;
            break;
        case 1:
            pRegBurstLen = &_TspCtrl[0].PVR2_Config;
            u32BurstLen  = TSP_PVR2_BURST_LEN_4;
            u32BurstMask = TSP_PVR2_BURST_LEN_MASK;
            pRegPVREn = &_TspCtrl[0].PVR2_Config;
            u32PVRFlag = TSP_PVR2_STR2MIU_EN;
            pRegTSIFEn = &_TspCtrl[0].PVR2_Config;
            u32TSIFFlag = TSP_TSIF2_ENABLE;
            break;
    }

    if (bEnable)
    {
        _HAL_REG32_W(pRegBurstLen, (_HAL_REG32_R(pRegBurstLen) & ~u32BurstMask) | u32BurstLen);
        _HAL_REG32_W(pRegPVREn, SET_FLAG1(_HAL_REG32_R(pRegPVREn), u32PVRFlag));
        _HAL_REG32_W(pRegTSIFEn, SET_FLAG1(_HAL_REG32_R(pRegTSIFEn), u32TSIFFlag));
    }
    else
    {
        _HAL_REG32_W(pRegPVREn, RESET_FLAG1(_HAL_REG32_R(pRegPVREn), u32PVRFlag));
        //_HAL_REG32_W(pRegTSIFEn, RESET_FLAG1(_HAL_REG32_R(pRegTSIFEn), u32TSIFFlag));
    }
}

void HAL_TSP_PVR_Reset(MS_U8 u8PVRIndex)
{
    // Richard: @FIXME:
    // Don't know PVR "reset" definition. call flush instead.
    HAL_TSP_PVR_WaitFlush(u8PVRIndex);
}

//Only PVR1 support Old record all mode, and must disable remove packet demux bit
//0: PVR1   1: PVR2     3: PVRCB
void HAL_TSP_PVR_All(MS_U8 u8PVRId, MS_BOOL bPvrAll, MS_BOOL bWithNull, MS_BOOL bOldMode)
{
    REG32 *pReg = 0;
    MS_U32 u32flag = 0;
    REG32 *pRegPidBypass = 0;
    MS_U32 u32PidBypassFlag = 0;

    switch(u8PVRId)
    {
        case 0:
        default:
            pRegPidBypass = &_TspCtrl[0].reg15b4;
            u32PidBypassFlag = TSP_PVR_PID_BYPASS;
            pReg = &_TspCtrl[0].HW2_Config3;
            if(bOldMode)
                u32flag = TSP_REC_ALL_OLD;
            else
            {
                u32flag = TSP_PVR1_REC_ALL_EN;
                if(bWithNull)
                    u32flag |= TSP_REC_NULL;
            }
            break;
        case 1:
            pRegPidBypass = &_TspCtrl[0].reg15b4;
            u32PidBypassFlag = TSP_PVR_PID_BYPASS2;
            pReg = &_TspCtrl[0].HW2_Config3;
            u32flag = TSP_PVR2_REC_ALL_EN;
            if(bWithNull)
                u32flag |= TSP_REC_NULL;
            break;
    }

    _HAL_REG32_W(pRegPidBypass, SET_FLAG1(_HAL_REG32_R(pRegPidBypass), u32PidBypassFlag));

    if (bPvrAll)
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    }
    else
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    }
}

void HAL_TSP_PVR_BypassHeader_En(MS_U8 u8PVRId, MS_BOOL bBypassHD)
{
    MS_U32 u32flag = 0;

    switch(u8PVRId)
    {
        case 0:
            u32flag = TSP_PVR_PID_BYPASS;
            break;
        case 1:
            u32flag = TSP_PVR_PID_BYPASS2;
            break;
        default:
            return;
    }

    if(bBypassHD)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), u32flag));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), u32flag));
    }
}

void HAL_TSP_SetPKTSize(MS_U32 u32PKTSize)
{
    if(u32PKTSize == 0x82) // RVU
    {
        _HAL_REG32_W(&_TspCtrl[0].reg163C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg163C), (TSP_PKT130_EN | TSP_PKT130_TEI_EN)));

    }
    else if(u32PKTSize == 0x86) // RVU with timestamp
    {
        _HAL_REG32_W(&_TspCtrl[0].reg163C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg163C), (TSP_PKT130_EN | TSP_PKT130_TEI_EN)));

        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_FILEIN192_EN));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_ALT_TS_SIZE));
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, (_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein)&~TSP_PKT_SIZE_MASK)|(TSP_PKT_SIZE_MASK&u32PKTSize));
    }
}

// Set 1 to disable file-in timestamp block scheme, bypass timestamp
void HAL_TSP_FileIn_192BlockScheme_En(MS_BOOL bEnable)
{
    if (!bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | TSP_PKT192_BLK_DIS_FIN);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & ~TSP_PKT192_BLK_DIS_FIN);
    }
}

void HAL_TSP_STC64_Mode_En(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | TSP_SYSTIME_MODE_STC64);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & ~TSP_SYSTIME_MODE_STC64);
    }
}

// For MIPS highway issue (last_done_Z), HW update PVR write pointer only when DMA done,
// So buffer start address will not update to write pointer at first time.
MS_U32  HAL_TSP_PVR_GetBufWrite(MS_U8 u8PVRId)
{
    MS_U32 u32value = 0;

    _HAL_REG32_W(&_TspCtrl[0].CH_BW_CTRL,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].CH_BW_CTRL), TSP_CH_BW_WP_LD));
    switch(u8PVRId)
    {
        case 0:
            u32value = _HAL_REG32_R(&_TspCtrl[0].TsRec_Mid_PVR1_WPTR);
            break;
        case 1:
            u32value = _HAL_REG32_R(&_TspCtrl[0].Str2mi_mid1_wptr_pvr2);
            break;
    }
    _HAL_REG32_W(&_TspCtrl[0].CH_BW_CTRL,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].CH_BW_CTRL), TSP_CH_BW_WP_LD));

    return (u32value << MIU_BUS);

}

void HAL_TSP_PVR_WaitFlush(MS_U8 u8PVRId)
{
    REG32 *pReg = 0;
    MS_U32 u32flag = 0;

    switch(u8PVRId)
    {
        default:
        case 0:
            pReg = &_TspCtrl[0].Hw_Config4;
            u32flag = TSP_HW_CFG4_PVR_FLUSH;
            break;
        case 1:
            pReg = &_TspCtrl[0].PVR2_Config;
            u32flag = TSP_PVR2_STR2MIU_RST_WADR;
            break;
    }

    _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32flag));
}

void HAL_TSP_PVR_Src_Select(MS_U8 u8PVRId, MS_U32 u32Src)
{
#if (TSP_HWPCR_BY_HK == 1 || !defined(HWPCR_ENABLE))
        switch(u8PVRId)
        {
            case 0:
                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_PVR1_SRC_MASK)| (u32Src << TSP_PVR1_SRC_SHIFT));
                break;
            case 1:
                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_PVR2_SRC_MASK)| (u32Src << TSP_PVR2_SRC_SHIFT));
                break;
            default:
                return;
        }
#else
        switch(u8PVRId)
        {
            case 0:
                _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_PVR1_SRC_MASK, (u32Src << TSP_PVR1_SRC_SHIFT));
                break;
            case 1:
                _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_PVR2_SRC_MASK, (u32Src << TSP_PVR2_SRC_SHIFT));
                break;
            default:
                return;
        }
#endif

}

MS_U32 HAL_TSP_PVR_StartingEngs_Get(MS_U32 u32PktDmxSrc)
{
    MS_U32 u32Flag = 0;
    MS_U32 u32Src;

    u32Src = 1 << ((_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & TSP_PVR1_SRC_MASK) >> TSP_PVR1_SRC_SHIFT);
    if(u32PktDmxSrc & u32Src)
    {
        if(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PVR_ENABLE)
            u32Flag = 1;
    }
    u32Src = 1 << ((_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & TSP_PVR2_SRC_MASK) >> TSP_PVR2_SRC_SHIFT);
    if(u32PktDmxSrc & u32Src)
    {
        if(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config) & TSP_PVR2_STR2MIU_EN)
            u32Flag |= 2;
    }

    return u32Flag;
}

MS_BOOL HAL_TSP_PVR_IsEnabled(MS_U32 u32EngId)
{
    if(u32EngId == 0)
    {
        return ((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PVR_ENABLE) > 0);
    }
    else if(u32EngId == 1)
    {
        return ((_HAL_REG32_R(&_TspCtrl[0].PVR2_Config) & TSP_PVR2_STR2MIU_EN) > 0);
    }
    else
    {
        return FALSE;
    }

}

static MS_U32 _u32FlowPadMap[TSP_IF_NUM] = { 0x0, 0x0, 0x0}; //TS0, TS1, TSFI
//--------------------------------------------------------------------------------------------------
// For pad select part
//--------------------------------------------------------------------------------------------------
MS_BOOL HAL_TSP_TsOutPadCfg(MS_U32 u32OutPad, MS_U32 u32OutPadMode, MS_U32 u32InPad, MS_BOOL bInParallel)
{
    MS_U16 u16Data = 0;
    MS_U16 u16S2pCfg = 0, u16clk = 0;

    //printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    //printf("[%s\[%d] u32OutPad %ld u32InPad %ld bInParallel %d\n", __FUNCTION__, __LINE__, u32OutPad, u32InPad, (int)bInParallel);

    if(u32OutPad != TSP_MUX_TS1)
    {
        return FALSE;
    }

    // S2P setting
    if((u32InPad != TSP_MUX_INDEMOD) && (u32InPad != TSP_MUX_TSO))
    {
        u16S2pCfg = TSP_TSO0_REG(REG_TSO0_CFG0) | ((REG_TSO0_CFG0_S2PCFG_S2P_EN|REG_TSO0_CFG0_S2PCFG_S2P_TSSIN_C0) << REG_TSO0_CFG0_S2P0_CFG_SHIFT);

        if(u32InPad == TSP_MUX_NONE)
        {
            TSP_TSO0_REG(REG_TSO0_CFG0) &= ~(REG_TSO0_CFG0_S2PCFG_S2P_EN << REG_TSO0_CFG0_S2P0_CFG_SHIFT);
            return TRUE;
        }

        //BYPASS_S2P setting select
        if(bInParallel)
        {
            u16S2pCfg |= REG_TSO0_CFG0_S2PCFG_S2P_BYPASS;
        }
        else
        {
            u16S2pCfg &= ~REG_TSO0_CFG0_S2PCFG_S2P_BYPASS;
        }

        //S2p input pad select
        switch(u32InPad)
        {
            case TSP_MUX_TS0:
                u16Data = TSP_MUX_TS0;
                u16clk = TSP_CLK_TS0;
                break;
            case TSP_MUX_TS1:
                u16Data = TSP_MUX_TS1;
                u16clk = TSP_CLK_TS1;
                break;
            case TSP_MUX_TS2:
                u16Data = TSP_MUX_TS2;
                u16clk = TSP_CLK_TS2;
                break;
            default:
                return FALSE;
        }

        _HAL_REG16_W(&(_TspCtrl5[0].TS_MUX_CFG_S2P), ((_HAL_REG16_R(&(_TspCtrl5[0].TS_MUX_CFG_S2P)) & ~TS_MUX_CFG_S2P0_MUX_MASK) | u16Data));
        TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLKTS4) =
            (TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLKTS4) & ~(REG_CLKGEN0_TSN_CLK_MASK<<REG_CLKGEN0_TSN_CLK_TS4_SHIFT)) | (u16clk<<REG_CLKGEN0_TSN_CLK_TS4_SHIFT);
        TSP_TSO0_REG(REG_TSO0_CFG0) = u16S2pCfg;
        TSP_TS_SAMPLE_REG(REG_TSO_OUT_CLK_SEL) = ((TSP_TS_SAMPLE_REG(REG_TSO_OUT_CLK_SEL) & ~REG_TSO_OUT_CLK_SEL_MASK) | REG_TSO_OUT_S2P);  //s2p out
    }

    if(u32InPad == TSP_MUX_INDEMOD)
    { // Internal Demod out
        TSP_TOP_REG(REG_TOP_TS_OUT_MODE) = TSP_TOP_REG(REG_TOP_TS_OUT_MODE) & ~REG_TOP_TS_OUT_MODE_MASK;
        TSP_TOP_REG(REG_TOP_TSCONFIG) = (TSP_TOP_REG(REG_TOP_TSCONFIG) & ~(REG_TOP_TS_TS1_CFG_MASK << REG_TOP_TS1CFG_SHIFT)) | (REG_TOP_TS_TS1_PARALL_OUT << REG_TOP_TS1CFG_SHIFT);
    }
    else
    {
        TSP_TOP_REG(REG_TOP_TS_OUT_MODE) = (TSP_TOP_REG(REG_TOP_TS_OUT_MODE) & ~REG_TOP_TS_OUT_MODE_MASK) | REG_TOP_TS1_OUT_MODE_TSO;
        TSP_TOP_REG(REG_TOP_TSCONFIG) = TSP_TOP_REG(REG_TOP_TSCONFIG) & ~(REG_TOP_TS_TS1_CFG_MASK << REG_TOP_TS1CFG_SHIFT);
    }

    u16Data = 0;
    switch(u32InPad)
    {
        case TSP_MUX_TS0:
        case TSP_MUX_TS2:
            u16Data = TS_MUX_CFG_TSOOUT_FROM_S2P;
            break;
        case TSP_MUX_TSO:
            u16Data = TS_MUX_CFG_TSOOUT_FROM_TSO;
            break;
        default:
            break;
    }
    _HAL_REG16_W(&(_TspCtrl5[0].TS_MUX_CFG_TSOOUT), ((_HAL_REG16_R(&(_TspCtrl5[0].TS_MUX_CFG_TSOOUT)) & ~TS_MUX_CFG_TSOOUT_MASK) | u16Data));

    return TRUE;
}

void HAL_TSP_SelPad(MS_U32 u32EngId, MS_U32 u32Flow, MS_U32 u32Pad, MS_BOOL bParl)
{
    MS_U16 u16data = 0;
    MS_U16 u16Shift = 0;
    MS_U16 u16padsel = 0;
    MS_U16 u16Reg = 0;
    MS_U16 u16Mask = 0;
    MS_U16 u16ShiftSet[TSP_IF_NUM] = {TS_MUX_CFG_TS0_MUX_SHIFT,TS_MUX_CFG_TS1_MUX_SHIFT,TS_MUX_CFG_TSFI_MUX_SHIFT};

    //printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    //printf("[%s\[%d] u32Flow %ld u32Pad %ld bParl %d\n", __FUNCTION__, __LINE__, u32Flow, u32Pad, (int)bParl);

    MS_ASSERT(9 > u32Pad);

    if(u32Flow == 0x80) //E_DRVTSP_IF_FI
    {
        u32Flow = TSP_IF_NUM - 1;
    }
    else if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    if((u32Pad == TSP_MUX_TS0) && (_bTsPadUsed[0] == FALSE))
    {
        //TSP_TOP_REG(REG_TOP_TS0_PE) = TSP_TOP_REG(REG_TOP_TS0_PE)| REG_TOP_TS0_PE_MASK;
        _bTsPadUsed[0] = TRUE;
    }
    if((u32Pad == TSP_MUX_TS1) && (_bTsPadUsed[1] == FALSE))
    {
        //TSP_TOP_REG(REG_TOP_TS1_PE) = TSP_TOP_REG(REG_TOP_TS1_PE) | REG_TOP_TS1_PE_MASK;
        _bTsPadUsed[1] = TRUE;
    }
    if((u32Pad == TSP_MUX_TS2) && (_bTsPadUsed[2] == FALSE))
    {
        //TSP_TOP_REG(REG_TOP_TS2_PE) = TSP_TOP_REG(REG_TOP_TS2_PE)| REG_TOP_TS2_PE_MASK;
        _bTsPadUsed[2] = TRUE;
    }

    _u32FlowPadMap[u32Flow] = u32Pad;
    u16padsel = (MS_U16)u32Pad;
    u16Shift = u16ShiftSet[u32Flow];

    u16data = (_HAL_REG16_R(&(_TspCtrl5[0].TS_MUX_CFG0)) & ~(TS_MUX_CFG_TS0_MUX_MASK << u16Shift)) | (u16padsel<< u16Shift);
    _HAL_REG16_W(&(_TspCtrl5[0].TS_MUX_CFG0), u16data);


    u16Shift = 0;
    switch(u16padsel)
    {
        case TSP_MUX_TS0:
            u16Reg = REG_TOP_TSCONFIG;
            u16Shift = REG_TOP_TS0CFG_SHIFT;
            u16Mask = REG_TOP_TS_TS0_CFG_MASK;
            break;
        case TSP_MUX_TS1:
            TSP_TOP_REG(REG_TOP_TS_OUT_MODE) = TSP_TOP_REG(REG_TOP_TS_OUT_MODE) & ~REG_TOP_TS_OUT_MODE_MASK; //disable ts1 out mode
            u16Reg = REG_TOP_TSCONFIG;
            u16Shift = REG_TOP_TS1CFG_SHIFT;
            u16Mask = REG_TOP_TS_TS1_CFG_MASK;
            break;
        case TSP_MUX_TS2:
            u16Reg = REG_TOP_TS2CONFIG;
            u16Shift = REG_TOP_TS2CFG_SHIFT;
            u16Mask = REG_TOP_TS_TS2_CFG_MASK;
            break;
        default:
            break;
    }

    if(bParl == FALSE)
    {
        switch(u16padsel)
        {
            case TSP_MUX_TS0:
                u16data = REG_TOP_TS_TS0_SERIAL_IN;
                break;
            case TSP_MUX_TS2:
                u16data = REG_TOP_TS_TS2_SERIAL_IN;
                break;
            case TSP_MUX_TS1:
                u16data = REG_TOP_TS_TS1_SERIAL_IN;
                break;
            default:
                break;
        }
    }
    else
    {
        if( (u16padsel == TSP_MUX_TS0) || (u16padsel == TSP_MUX_TS1) || (u16padsel == TSP_MUX_TS2) )
        {
            u16data = 1;
        }
    }

    //printf("[%s\[%d] u16Reg %x u16Mask %x u16Shift %x\n", __FUNCTION__, __LINE__, u16Reg, u16Mask, u16Shift);
    TSP_TOP_REG(u16Reg) = (TSP_TOP_REG(u16Reg) & ~(u16Mask<<u16Shift)) | (u16data << u16Shift);

}

void HAL_TSP_SelPad_ClkInv(MS_U32 u32EngId, MS_U32 u32Flow, MS_BOOL bClkInv)
{
    MS_U32 u32Clk = 0;
    MS_U32 u32data = 0;

    if(u32Flow == 0x80) //E_DRVTSP_IF_FI
    {
        u32Flow = TSP_IF_NUM - 1;
    }
    else if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    switch(_u32FlowPadMap[u32Flow])
    {
        case TSP_MUX_INDEMOD:
            u32Clk = TSP_CLK_INDEMOD;
            break;
        case TSP_MUX_TS0:
            u32Clk = TSP_CLK_TS0;
            break;
        case TSP_MUX_TS1:
            u32Clk = TSP_CLK_TS1;
            break;
        case TSP_MUX_TS2:
            u32Clk = TSP_CLK_TS2;
            break;
        case TSP_MUX_TSO:
            u32Clk = TSP_CLK_TSOOUT;
            break;
        default:
            return;
    }

    if (bClkInv)
        u32Clk |= TSP_CLK_INVERSE;

    switch(u32Flow)
    {
        case 0:
            u32data = TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) & ~(REG_CLKGEN0_TSN_CLK_MASK<< REG_CLKGEN0_TSN_CLK_TS0_SHIFT);
            u32data |= (u32Clk<< REG_CLKGEN0_TSN_CLK_TS0_SHIFT);
            TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) = u32data;
            break;
        case 1:
            u32data = TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) & ~(REG_CLKGEN0_TSN_CLK_MASK<< REG_CLKGEN0_TSN_CLK_TS1_SHIFT);
            u32data |= (u32Clk<< REG_CLKGEN0_TSN_CLK_TS1_SHIFT);
            TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) = u32data;
            break;
        case 2:
            u32data = TSP_CLKGEN1_REG(REG_CLKGEN1_TSN_CLKFI) & ~(REG_CLKGEN0_TSN_CLK_MASK<< REG_CLKGEN1_TSN_CLK_TSFI_SHIFT);
            u32data |= (u32Clk<< REG_CLKGEN1_TSN_CLK_TSFI_SHIFT);
            TSP_CLKGEN1_REG(REG_CLKGEN1_TSN_CLKFI) = u32data;
            break;
        default:
            return;
    }

}

void HAL_INT_Force(MS_U16 u16value)
{
    TSP_INT_REG(0x31) = TSP_INT_REG(0x31) | u16value;
    printf("HAL_INT_Force 0x%x\n", (int)(TSP_INT_REG(0x31)))  ;

}

void HAL_TSP_SelPad_ExtSync(MS_U32 u32EngId, MS_BOOL bExtSync, MS_U32 u32Flow)
{
    REG32* pReg = NULL;
    MS_U32 u32ExtSync = 0;

    if(u32Flow == 0x80) //E_DRVTSP_IF_FI
    {
        u32Flow = TSP_IF_NUM - 1;
    }
    else if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    switch(u32Flow)
    {
        case 0:
            pReg = &(_TspCtrl[0].Hw_Config0);
            u32ExtSync = TSP_HW_CFG0_TSIF0_EXTSYNC;
            break;
        case 1:
            pReg = &(_TspCtrl[0].Hw_Config2);
            u32ExtSync = TSP_HW_CFG2_TSIF1_EXTSYNC;
            break;
        case 2:
            if (bExtSync)
            {
                _HAL_REG16_W(&(_TspCtrl5[0].Ts_If_Fi_Cfg), SET_FLAG1(_HAL_REG16_R(&(_TspCtrl5[0].Ts_If_Fi_Cfg)), TSP_FIIF_EXT_SYNC_SEL));
            }
            else
            {
                _HAL_REG16_W(&(_TspCtrl5[0].Ts_If_Fi_Cfg), RESET_FLAG1(_HAL_REG16_R(&(_TspCtrl5[0].Ts_If_Fi_Cfg)), TSP_FIIF_EXT_SYNC_SEL));
            }
            return;
        default:
            return;
    }

    if (bExtSync)
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32ExtSync));
    }
    else
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32ExtSync));
    }
}

void HAL_TSP_SelPad_Parl(MS_U32 u32EngId, MS_BOOL bParl, MS_U32 u32Flow)
{
    REG32* pReg = NULL;
    MS_U32 u32Parl = 0;

    if(u32Flow == 0x80) //E_DRVTSP_IF_FI
    {
        u32Flow = TSP_IF_NUM - 1;
    }
    else if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    switch(u32Flow)
    {
        case 0:
            pReg = &(_TspCtrl[0].Hw_Config0);
            u32Parl = TSP_HW_CFG0_TSIF0_PARL;
            break;
        case 1:
            pReg = &(_TspCtrl[0].Hw_Config2);
            u32Parl = TSP_HW_CFG2_TSIF1_PARL;
            break;
        case 2:
            if (bParl)
            {
                _HAL_REG16_W(&(_TspCtrl5[0].Ts_If_Fi_Cfg), SET_FLAG1(_HAL_REG16_R(&(_TspCtrl5[0].Ts_If_Fi_Cfg)), TSP_FIIF_P_SEL));
            }
            else
            {
                _HAL_REG16_W(&(_TspCtrl5[0].Ts_If_Fi_Cfg), RESET_FLAG1(_HAL_REG16_R(&(_TspCtrl5[0].Ts_If_Fi_Cfg)), TSP_FIIF_P_SEL));
            }
            return;
        default:
            return;
    }

    if (bParl)  // parallel
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32Parl));
    }
    else        // serial
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32Parl));
    }
}

MS_BOOL HAL_TSP_TsOuOutClockPhase(MS_U16 u16OutPad, MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt)
{
    if(bEnable == FALSE)
    {
        _HAL_REG16_W(&_TspSample[0].S2P_Out_Clk_Sample,
            RESET_FLAG1(_HAL_REG16_R(&_TspSample[0].S2P_Out_Clk_Sample), S2P_PHASE_ADJUST_EN));
    }
    else
    {
        _HAL_REG16_W(&_TspSample[0].S2P_Out_Clk_Sample,
            (_HAL_REG16_R(&_TspSample[0].S2P_Out_Clk_Sample) & ~S2P_PHASE_ADJUST_COUNT_MASK) | (u16Val & S2P_PHASE_ADJUST_COUNT_MASK) | S2P_PHASE_ADJUST_EN);
    }

    // Set S2P clk invert config
    if(u32S2pOpt & HAL_S2P_CLK_OPT_INVERT)
    {
        _HAL_REG16_W(&_TspSample[0].S2P_Out_Clk_Sample,
            SET_FLAG1(_HAL_REG16_R(&_TspSample[0].S2P_Out_Clk_Sample), S2P_CLK_INVERT));
    }
    if(u32S2pOpt & HAL_S2P_CLK_OPT_NON_INVERT)
    {
        _HAL_REG16_W(&_TspSample[0].S2P_Out_Clk_Sample,
            RESET_FLAG1(_HAL_REG16_R(&_TspSample[0].S2P_Out_Clk_Sample), S2P_CLK_INVERT));
    }

    return TRUE;
}

void HAL_TSP_TSOut_En(MS_BOOL bEnable)
{
    return;
}

void HAL_TSP_Parl_BitOrderSwap(MS_U32 u32EngId, MS_U32 u32Flow, MS_BOOL bInvert)
{
    REG32* pReg = &(_TspCtrl[0].Hw_Config4);
    MS_U32 u32Invert = 0;

    if(u32Flow == 0x80) //E_DRVTSP_IF_FI
    {
        u32Flow = TSP_IF_NUM-1;
    }
    else if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    switch(u32Flow)
    {
        case 0:
            pReg = &(_TspCtrl[0].Hw_Config4);
            u32Invert = TSP_HW_CFG4_TS_DATA0_SWAP;
            break;
        case 1:
            pReg = &(_TspCtrl[0].Hw_Config4);
            u32Invert = TSP_HW_CFG4_TS_DATA1_SWAP;
            break;
        case 2:
            if (bInvert)
            {
                _HAL_REG16_W(&(_TspCtrl5[0].Ts_If_Fi_Cfg), SET_FLAG1(_HAL_REG16_R(&(_TspCtrl5[0].Ts_If_Fi_Cfg)), TSP_FIIF_DATA_SWAP));
            }
            else
            {
                _HAL_REG16_W(&(_TspCtrl5[0].Ts_If_Fi_Cfg), RESET_FLAG1(_HAL_REG16_R(&(_TspCtrl5[0].Ts_If_Fi_Cfg)), TSP_FIIF_DATA_SWAP));
            }
            return;
        default:
            return;
    }

    if(bInvert)
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32Invert));
    }
    else
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32Invert));
    }
}

MS_BOOL HAL_TSP_GetCap(MS_U32 u32Cap, void* pData)
{
    MS_BOOL bRet = TRUE;

    switch (u32Cap)
    {
    case HAL_TSP_CAP_PID_FILTER_NUM:
        *((MS_U32*)pData) = TSP_PIDFLT_NUM_ALL;
        break;
    case HAL_TSP_CAP_PVR_FILTER_NUM:
    case HAL_TSP_CAP_PVR1_FILTER_NUM:
        *((MS_U32*)pData) = TSP_PIDFLT_NUM_ALL;
        break;
    case HAL_TSP_CAP_SEC_FILTER_NUM:
        *((MS_U32*)pData) = TSP_SECFLT_NUM;
        break;
    case HAL_TSP_CAP_SEC_BUF_NUM:
        *((MS_U32*)pData) = TSP_SECBUF_NUM;
        break;
    case HAL_TSP_CAP_PVR_ENG_NUM:
        *((MS_U32*)pData) = TSP_PVR_IF_NUM;
        break;
    case HAL_TSP_CAP_MMFI0_FILTER_NUM:
        *((MS_U32*)pData) = TSP_MMFI0_FILTER_NUM;
        break;
    case HAL_TSP_CAP_MMFI1_FILTER_NUM:
        *((MS_U32*)pData) = TSP_MMFI1_FILTER_NUM;
        break;
    case HAL_TSP_CAP_TSIF_NUM:
        *((MS_U32*)pData) = TSP_IF_NUM;
        break;
    case HAL_TSP_CAP_DEMOD_NUM:
        *((MS_U32*)pData) = TSP_DEMOD_NUM;
        break;
    case HAL_TSP_CAP_VFIFO_NUM:
        *((MS_U32*)pData) = TSP_VFIFO_NUM;
        break;
    case HAL_TSP_CAP_AFIFO_NUM:
        *((MS_U32*)pData) = TSP_AFIFO_NUM;
        break;
    case HAL_TSP_CAP_TS_PAD_NUM:
        *((MS_U32*)pData) = TSP_TS_PAD_NUM;
        break;
    case HAL_TSP_CAP_VQ_NUM:
        *((MS_U32*)pData) = TSP_VQ_NUM;
        break;
    case HAL_TSP_CAP_CA_FLT_NUM:
        *((MS_U32*)pData) = TSP_CA_FLT_NUM;
        break;
    case HAL_TSP_CAP_CA_KEY_NUM:
        *((MS_U32*)pData) = TSP_CA_KEY_NUM;
        break;
    case HAL_TSP_CAP_FW_ALIGN:
        *((MS_U32*)pData) = (1L << (MIU_BUS+TSP_DNLD_ADDR_ALI_SHIFT));
        break;
    case HAL_TSP_CAP_VQ_ALIGN:
    case HAL_TSP_CAP_SEC_BUF_ALIGN:
    case HAL_TSP_CAP_PVR_ALIGN:
        *((MS_U32*)pData) = (1L << MIU_BUS);
        break;
    case HAL_TSP_CAP_VQ_PITCH:
        *((MS_U32*)pData) = VQ_PACKET_UNIT_LEN;
        break;
    case HAL_TSP_CAP_PVRCA_PATH_NUM:
        *((MS_U32*)pData) = TSP_CA_ENGINE_NUM;
        break;
    case HAL_TSP_CAP_SHAREKEY_FLT_RANGE:
        *((MS_U32*)pData) = DSCMB_FLT_SHAREKEY_START_ID;
        *((MS_U32*)pData + 1) = DSCMB_FLT_SHAREKEY_END_ID;
        break;
    case HAL_TSP_CAP_CA0_FLT_RANGE:
        *((MS_U32*)pData) = DSCMB_FLT_START_ID;
        *((MS_U32*)pData + 1) = DSCMB_FLT_END_ID;
        break;
    case HAL_TSP_CAP_CA1_FLT_RANGE:
        *((MS_U32*)pData) = DSCMB1_FLT_START_ID;
        *((MS_U32*)pData + 1) = DSCMB1_FLT_END_ID;
        break;
    case HAL_TSP_CAP_SHAREKEY_FLT1_RANGE:
        *((MS_U32*)pData) = DSCMB_FLT_SHAREKEY1_START_ID;
        *((MS_U32*)pData + 1) = DSCMB_FLT_SHAREKEY1_END_ID;
        break;
    case HAL_TSP_CAP_HW_TYPE:
        *((MS_U32*)pData) = 0x00001006;
        break;
    case HAL_TSP_CAP_HWPCR_SUPPORT:
        *((MS_U32*)pData) = TSP_HWPCR_BY_HK;
        break;
    case HAL_TSP_CAP_PCRFLT_START_IDX:
        *((MS_U32*)pData) = TSP_PIDFLT_NUM;
        break;
    case HAL_TSP_CAP_HWWP_SET_NUM:
        *((MS_U32*)pData) = TSP_WP_SET_NUM;
        break;
    case HAL_TSP_CAP_DSCMB_ENG_NUM:
        *((MS_U32*)pData) = TSP_CA_ENGINE_NUM;
        break;
    case HAL_TSP_CAP_MERGESTR_NUM:
        *((MS_U32*)pData) = TSP_MERGESTR_MUM;
        break;
    case HAL_TSP_CAP_MAX_SEC_FLT_DEPTH:
        *((MS_U32*)pData) = TSP_SEC_FLT_DEPTH;
        break;
    case HAL_TSP_CAP_FW_BUF_SIZE:
        *((MS_U32*)pData) = TSP_FW_BUF_SIZE;
        break;
    case HAL_TSP_CAP_FW_BUF_RANGE:
        *((MS_U32*)pData) = TSP_FW_BUF_LOW_BUD;
        *((MS_U32*)pData+1) = TSP_FW_BUF_UP_BUD;
        break;
    case HAL_TSP_CAP_VQ_BUF_RANGE:
        *((MS_U32*)pData) = TSP_VQ_BUF_LOW_BUD;
        *((MS_U32*)pData+1) = TSP_VQ_BUF_UP_BUD;
        break;
    case HAL_TSP_CAP_SEC_BUF_RANGE:
        *((MS_U32*)pData) = TSP_SEC_BUF_LOW_BUD;
        *((MS_U32*)pData+1) = TSP_SEC_BUF_UP_BUD;
        break;
    case HAL_TSP_CAP_FIQ_NUM:
        *((MS_U32*)pData) = TSP_FIQ_NUM;
        break;
    default:
        *((MS_U32*)pData) = 0xFFFFFFFF;
        bRet = FALSE;
        break;
    }
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void _HAL_TSP_FW_load(
    MS_U32                     u32FwAddrPhys,
    MS_U32                     u32FwSize,
    MS_BOOL                    bFwDMA,
    MS_BOOL                    bIQmem,
    MS_BOOL                    bDQmem)
{
    MS_U32 u32Value = 0x0;
    // bDQmem is always true
    MS_ASSERT(bDQmem);

    // @FIXME: Richard: Only allow TSP FW running in DRAM at this first stage.
    // improve this afterward.
     _HAL_REG32_W(&_TspCtrl[0].Cpu_Base, 0/*u32FwAddrPhys >> 3*/); // 16 bytes address unit

    if (bFwDMA)
    {
        MS_U32              u32DnldCtrl  = 0;
        MS_U32              u32DnldCtrl1 = 0;
        u32DnldCtrl  =      ((u32FwAddrPhys >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT) & TSP_DNLD_ADDR_MASK;
        u32DnldCtrl1 =      (((u32FwAddrPhys >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT) >> 16) & TSP_DMA_RADDR_MSB_MASK;
        printf("firmware 111 0x%08lx 0x%08lx 0x%08lx\n", u32FwAddrPhys, u32DnldCtrl1, u32DnldCtrl);

        u32DnldCtrl |=      (_TSP_QMEM_SIZE << TSP_DNLD_NUM_SHFT);

        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl, u32DnldCtrl);
        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2, (_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2) & ~TSP_DMA_RADDR_MSB_MASK) | u32DnldCtrl1);
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_START| TSP_CTRL_DNLD_DONE));
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_START));
        while (!HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_DONE));//printf(".");
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_START| TSP_CTRL_DNLD_DONE));
    }

    u32Value = _HAL_REG32_R(&_TspCtrl[0].Qmem_Imask) | _TSP_QMEM_I_MASK;
    _HAL_REG32_W(&_TspCtrl[0].Qmem_Imask, u32Value);
    if (bIQmem)
    {
        _HAL_REG32_W(&_TspCtrl[0].Qmem_Ibase,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Qmem_Ibase), _TSP_QMEM_I_ADDR_HIT));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Qmem_Ibase, _TSP_QMEM_I_ADDR_MISS);
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_ICACHE_EN));
    }

    u32Value = _HAL_REG32_R(&_TspCtrl[0].Qmem_Dmask) | _TSP_QMEM_D_MASK;
    _HAL_REG32_W(&_TspCtrl[0].Qmem_Dmask,u32Value);

    u32Value = _HAL_REG32_R(&_TspCtrl[0].Qmem_Dbase) | _TSP_QMEM_D_ADDR_HIT;
    _HAL_REG32_W(&_TspCtrl[0].Qmem_Dbase,u32Value);

}

void HAL_TSP_filein_enable(MS_BOOL b_enable)
{
    // Richard: enable/disable file in timer as well
    //          file in could only walk through pid filter set 0.
    if (b_enable)
    {
        // Set Data port enable for audio bypass
        //_HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        //        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
        //_HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        //        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_ENABLE| TSP_CTRL1_FILEIN_TIMER_ENABLE));

        _HAL_REG16_W(&_TspCtrl5[0].Ts_If_Fi_Cfg,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].Ts_If_Fi_Cfg), TSP_FIIF_MUX_LIVE_PATH));
        _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_PVR_CMD_QUEUE_ENABLE));
    }
    else
    {
        //_HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        //        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
        //_HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        //        RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_ENABLE| TSP_CTRL1_FILEIN_TIMER_ENABLE));

        _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
                RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_PVR_CMD_QUEUE_ENABLE));
    }
    //_HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                //RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
}

void HAL_TSP_PS_Path_Disable(void)
{
    // set PS VID/AUD enable while video/audio/audio2 bypass mode
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN)));
    _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), (TSP_PS_VID3D_EN)));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
}

void HAL_TSP_PS_Path_Enable(MS_U32 u32TsDmaCtrl)
{
    switch (u32TsDmaCtrl)
    {
        case TSP_TSDMA_CTRL_VPES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PS_VID_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN)));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), (TSP_PS_VID3D_EN)));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_VID_EN));

                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_VID_SRC_MASK)| (TSP_SRC_FROM_PKTDMXFL << TSP_VID_SRC_SHIFT));

                // File in PS mode, fifo block mode enable
                HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_VD, FALSE);
            }

            break;
        case TSP_TSDMA_CTRL_APES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PS_AUD_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD2_EN)));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), (TSP_PS_VID3D_EN)));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_AUD_EN));

                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_AUD_SRC_MASK)| (TSP_SRC_FROM_PKTDMXFL << TSP_AUD_SRC_SHIFT));

                // File in PS mode, fifo block mode enable
                HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_AU, FALSE);
            }

            break;
        case TSP_TSDMA_CTRL_A2PES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PS_AUD2_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN)));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), (TSP_PS_VID3D_EN )));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_AUD2_EN));

                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_AUDB_SRC_MASK)| (TSP_SRC_FROM_PKTDMXFL << TSP_AUDB_SRC_SHIFT));

                // File in PS mode, fifo block mode enable
                HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_AUB, FALSE);
            }

            break;
        case TSP_TSDMA_CTRL_V3DPES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2) & TSP_PS_VID3D_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN)));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));

                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_VID3D_SRC_MASK)| (TSP_SRC_FROM_PKTDMXFL << TSP_VID3D_SRC_SHIFT));

                // File in PS mode, fifo block mode enable
                HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_V3D, FALSE);

            }

            break;
        default:
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN)));
            _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), (TSP_PS_VID3D_EN)));
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));

            break;
    }
}

MS_U32 HAL_TSP_GetCtrlMode(MS_U32 u32EngId)
{
    return (_HAL_REG32_R(&_TspCtrl[u32EngId].TSP_Ctrl));
}

void HAL_TSP_Flush_AV_FIFO(MS_U32 u32StreamId, MS_BOOL bFlush)
{
    MS_U32 u32Flag;
    REG32* pReg = NULL;

    switch(u32StreamId)
    {
        default:
        case 0:
            pReg = &_TspCtrl[0].reg160C;
            u32Flag = TSP_RESET_VFIFO;
            break;
        case 1:
            pReg = &_TspCtrl[0].reg160C;
            u32Flag = TSP_RESET_AFIFO;
            break;
        case 2:
            pReg = &_TspCtrl[0].reg160C;
            u32Flag = TSP_RESET_AFIFO2;
            break;
        case 3:
            pReg = &_TspCtrl[0].reg160C;
            u32Flag = TSP_RESET_VFIFO3D;
            break;
    }

    if (bFlush)
    {
        _HAL_REG32_W(pReg,
                SET_FLAG1(_HAL_REG32_R(pReg), u32Flag));
    }
    else
    {
        _HAL_REG32_W(pReg,
                RESET_FLAG1(_HAL_REG32_R(pReg), u32Flag));
    }
}

MS_U32 HAL_TSP_Get_AVFifoLevel(MS_U32 u32StreamId)
{
    switch (u32StreamId)
    {
        case 0: // return VFifo status
            return ((_HAL_REG32_R(&_TspCtrl[0].Pkt_Info2) & TSP_VFIFO_STATUS) >> TSP_VFIFO_STATUS_SHFT);
        case 1: // return AFifo 0 status
            return ((_HAL_REG32_R(&_TspCtrl[0].Pkt_Info2) & TSP_AFIFO_STATUS) >> TSP_AFIFO_STATUS_SHFT);
        case 2: // return AFifo 1 status
            return ((_HAL_REG32_R(&_TspCtrl[0].Pkt_Info2) & TSP_AFIFOB_STATUS) >> TSP_AFIFOB_STATUS_SHFT);
        case 3: // return V3D Fifo status
            return ((_HAL_REG32_R(&_TspCtrl[0].Pkt_Info2) & TSP_VFIFO3D_STATUS) >> TSP_VFIFO3D_STATUS_SHFT);
        default:
            return -1;
    }
}

void HAL_TSP_AVFIFO_Src_Select(MS_U32 u32Fifo, MS_U32 u32Src)
{
#if (TSP_HWPCR_BY_HK == 1 || !defined(HWPCR_ENABLE))

        switch(u32Fifo)
        {
            case TSP_FIFO_AU:
                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_AUD_SRC_MASK)| (u32Src << TSP_AUD_SRC_SHIFT));
                break;
            case TSP_FIFO_AUB:
                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_AUDB_SRC_MASK)| (u32Src << TSP_AUDB_SRC_SHIFT));
                break;
            case TSP_FIFO_VD:
                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_VID_SRC_MASK)| (u32Src << TSP_VID_SRC_SHIFT));
                break;
            case TSP_FIFO_V3D:
                _HAL_REG32_W(&_TspCtrl[0].FIFO_Src, (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_VID3D_SRC_MASK)| (u32Src << TSP_VID3D_SRC_SHIFT));
                break;
            default:
                return;
        }

#else
        switch(u32Fifo)
        {
            case TSP_FIFO_AU:
                _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_AUD_SRC_MASK, (u32Src << TSP_AUD_SRC_SHIFT));
                break;
            case TSP_FIFO_AUB:
                _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_AUDB_SRC_MASK, (u32Src << TSP_AUDB_SRC_SHIFT));
                break;
            case TSP_FIFO_VD:
                _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_VID_SRC_MASK, (u32Src << TSP_VID_SRC_SHIFT));
                break;
            case TSP_FIFO_V3D:
                _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_VID3D_SRC_MASK, (u32Src << TSP_VID3D_SRC_SHIFT));
                break;
            default:
                return;
        }
#endif

}

void HAL_TSP_AVFIFO_Block_Disable(MS_U32 u32Fifo, MS_BOOL bDisable)
{
    if(bDisable)
    {
        switch(u32Fifo)
        {
            case TSP_FIFO_AU:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_AUD_BLOCK_DIS));
                break;
            case TSP_FIFO_AUB:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_AUDB_BLOCK_DIS));
                break;
            case TSP_FIFO_VD:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_V_BLOCK_DIS));
                break;
            case TSP_FIFO_V3D:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_V3D_BLOCK_DIS));
                break;
        }
        return;
    }

    switch(u32Fifo)
    {
        case TSP_FIFO_AU:
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_AUD_BLOCK_DIS));
            break;
        case TSP_FIFO_AUB:
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_AUDB_BLOCK_DIS));
            break;
        case TSP_FIFO_VD:
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_V_BLOCK_DIS));
            break;
        case TSP_FIFO_V3D:
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_V3D_BLOCK_DIS));
            break;
    }
}

MS_BOOL HAL_TSP_TSIF_Enable(MS_U8 u8_tsif, MS_BOOL bEnable)
{
    if(bEnable)
        _HAL_TSP_tsif_select(u8_tsif);
    else
    {
        switch(u8_tsif)
        {
            default:
            case 0:
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF0_ENABLE));
                break;
            case 1:
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
                break;
            case 3: //file_FI
                _HAL_REG16_W(&_TspCtrl5[0].Ts_If_Fi_Cfg,
                    RESET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].Ts_If_Fi_Cfg), TSP_FIIF_EN));
                break;
        }
    }

    return TRUE;
}

void HAL_TSP_SelMatchPidSrc(MS_U32 u32Src)
{
    _HAL_REG32_W(&_TspCtrl[0].reg15b4, (_HAL_REG32_R(&_TspCtrl[0].reg15b4) & ~TSP_MATCH_PID_SRC_MASK)| (u32Src << TSP_MATCH_PID_SRC_SHIFT));
}

//Select TS1/TS2 PID filter source from TS1/TS2 or MMFI0/MMFI1
MS_BOOL HAL_TSP_PidFlt_Src_Select(MS_U32 u32Src)
{
    switch(u32Src)
    {
        case TSP_PIDFLT1_USE_TSIF1:
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config2, (_HAL_REG32_R(&_TspCtrl[0].Hw_Config2) & ~(TSP_HW_CFG2_PIDFLT1_SOURCE_TSIF_MMFI0)));
            break;
        case TSP_PIDFLT1_USE_TSIF_MMFI0:
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config2, (_HAL_REG32_R(&_TspCtrl[0].Hw_Config2) | (TSP_HW_CFG2_PIDFLT1_SOURCE_TSIF_MMFI0)));
            break;
        default:
            break;
    }
    return TRUE;
}

void HAL_TSP_Ind_Enable(void)
{
    _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_SW_RST));
}

MS_U32 HAL_TSP_HW_INT_STATUS(void)
{
    return (MS_U32)(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat) & TSP_HWINT_STATUS_MASK);
}

MS_U32 HAL_TSP_HW_INT2_STATUS(void)
{
    return (MS_U32)(_HAL_REG16_R(&_TspCtrl[0].SwInt_Stat1_L) & TSP_HWINT2_STATUS_MASK);
}

void HAL_TSP_SetBank(MS_U32 u32BankAddr, MS_U32 u32PMBankAddr)
{
    _u32RegBase = u32BankAddr;
    _u32PMRegBase = u32PMBankAddr;
    _TspCtrl   = (REG_Ctrl*)(_u32RegBase+ REG_CTRL_BASE);
    _TspCtrl2  = (REG_Ctrl2*)(_u32RegBase + REG_CTRL_MMFIBASE);
    _TspCtrl3  = (REG_Ctrl3*)(_u32RegBase + REG_CTRL_TSP3);
    _TspCtrl4  = (REG_Ctrl4*)(_u32RegBase + REG_CTRL_TSP4);
    _TspCtrl5  = (REG_Ctrl5*)(_u32RegBase + REG_CTRL_TSP5);
    _TspSample = (REG_TS_Sample*)(_u32RegBase + REG_CTRL_TS_SAMPLE);
}

void HAL_TSP_Reset(MS_U32 u32EngId)
{
    _HAL_REG32_W(&_TspCtrl[u32EngId].TSP_Ctrl, 0);
}

void HAL_TSP_HwPatch(void)
{
    _HAL_REG16_W(&_TspCtrl3[0].HWeco0,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl3[0].HWeco0), HW_ECO_RVU|HW_ECO_NEW_SYNCP_IN_ECO|HW_ECO_FIX_SEC_NULLPKT_ERR));

    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_BYTE_ADDR_DMA|TSP_HW_CFG4_ALT_TS_SIZE|TSP_HW_CFG4_ISYNC_PATCH_EN));

    // Bad initial value of TSP_CTRL1
    // Suppose Standby mode for TSP should NOT be enabled.
    // Enabling TSP standby mode cause TSP section registers (SRAM in AEON) malfunction.
    // Disable it by SW at this stage.
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_STANDBY));

    //enable PVR record to bypass header
    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            _HAL_REG32_R(&_TspCtrl[0].reg15b4)|(TSP_PVR_PID_BYPASS|TSP_PVR_PID_BYPASS2));

    //_HAL_REG32_W(&_TspCtrl[0].reg163C,
    //        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg163C), TSP_ALL_VALID_EN));
    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), (/*TSP_VQ2PINGPONG_EN |*/ TSP_PVR1_ALIGN_EN|TSP_RM_PKT_DEMUX_PIPE)));

    //Disable all live pathes block mechanism
    _HAL_REG32_W(&_TspCtrl[0].reg160C, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), (TSP_RM_DMA_GLITCH|TSP_VQTX0_BLOCK_DIS|TSP_VQTX2_BLOCK_DIS)));

    //enable ECO bit for section DMA burst mode
    _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
        _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | TSP_SEC_DMA_BURST_EN | TSP_REMOVE_DUP_AV_PKT | TSP_HW_STANDBY_MODE);

    //Disable pvr1 & pvr2 block mechanism
    //DisableAV FIFO block mechanism for live path
    _HAL_REG32_W(&_TspCtrl[0].PVR2_Config,
        _HAL_REG32_R(&_TspCtrl[0].PVR2_Config)|(TSP_PVR2_PVR_ALIGN_EN|TSP_PVR1_BLOCK_DIS|TSP_PVR2_BLOCK_DIS|TSP_V_BLOCK_DIS|TSP_AUD_BLOCK_DIS|TSP_AUDB_BLOCK_DIS|TSP_V3D_BLOCK_DIS));

    // Set filein segment bit to 0
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_PVR_CMD_QUEUE_ENABLE));

    _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | (TSP_SYSTIME_MODE_STC64));

    _HAL_REG16_W(&_TspCtrl3[0].HW3_Cfg0, _HAL_REG16_R(&_TspCtrl3[0].HW3_Cfg0) | (PREVENT_SRAM_COLLISION | PUSI_THREE_BYTE_MODE));

    //sync byte
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif0[0]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif0[1]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif0[2]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif0[3]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_file[0]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_file[1]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_file[2]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_file[3]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif1[0]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif1[1]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif1[2]), 0x4747);
    _HAL_REG16_W(&(_TspCtrl3[0].SyncByte_tsif1[3]), 0x4747);

    //source id
    _HAL_REG16_W(&(_TspCtrl3[0].SourceId_tsif0[0]), 0x3210);
    _HAL_REG16_W(&(_TspCtrl3[0].SourceId_tsif0[1]), 0x7654);
    _HAL_REG16_W(&(_TspCtrl3[0].SourceId_file[0]), 0x3210);
    _HAL_REG16_W(&(_TspCtrl3[0].SourceId_file[1]), 0x7654);
    _HAL_REG16_W(&(_TspCtrl3[0].SourceId_tsif1[0]), 0x3210);
    _HAL_REG16_W(&(_TspCtrl3[0].SourceId_tsif1[1]), 0x7654);

    //drop scmb packet
    _HAL_REG16_W(&_TspCtrl3[0].HW3_Cfg1, _HAL_REG16_R(&_TspCtrl3[0].HW3_Cfg1) | (MASK_SCR_VID_EN|MASK_SCR_VID_3D_EN|MASK_SCR_AUD_EN|MASK_SCR_AUD_B_EN));

    //ENBLE to not check
     _HAL_REG32_W(&_TspCtrl[0].DMAW_ERR_WADDR_SRC_SEL, _HAL_REG32_R(&_TspCtrl[0].DMAW_ERR_WADDR_SRC_SEL) | (TSP_BLK_AF_SCRMB_BIT));

    //Enable eco bit: 192 file-in timer 0 fail
    _HAL_REG16_W(&_TspCtrl5[0].TSP5_Eco, _HAL_REG16_R(&_TspCtrl5[0].TSP5_Eco) | (TSP_192_TIMER_0_EN));

    //VQ parameters
    _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL,   (_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL)  & ~TSP_VQ0_FORCE_FIRE_CNT_1K_MASK) | (0x0C << TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT));
    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config, (_HAL_REG32_R(&_TspCtrl[0].VQ1_Config) & ~TSP_VQ1_FORCEFIRE_CNT_1K_MASK) | (0x0C << TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT));
    _HAL_REG32_W(&_TspCtrl[0].VQ2_Config, (_HAL_REG32_R(&_TspCtrl[0].VQ2_Config) & ~TSP_VQ2_FORCEFIRE_CNT_1K_MASK) | (0x0C << TSP_VQ2_FORCEFIRE_CNT_1K_SHIFT));

}

//  Default value of low bound is 0, default value of up bound is 0xFFFFFFFF, means no protection
//  If set both low bound and up bound to be 0, means protection all
//  The range can be written:  u32StartAddr <= x < u32EndAddr
//  Protection range: x >= u32EndAddr && x < u32StartAddr
void HAL_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_U32 u32StartAddr, MS_U32 u32EndAddr)
{
    MS_U32  lbnd, ubnd;

    if (bEnable)
    {
        if(u32StartAddr == u32EndAddr)
            u32StartAddr += (1 << MIU_BUS);

        lbnd  = (u32StartAddr >>  MIU_BUS) & TSP_ORZ_DMAW_LBND_MASK;
        ubnd  = (u32EndAddr >>  MIU_BUS) & TSP_ORZ_DMAW_UBND_MASK;
        _HAL_REG32_W(&_TspCtrl[0].ORZ_DMAW_LBND, lbnd);
        _HAL_REG32_W(&_TspCtrl[0].ORZ_DMAW_UBND, ubnd);
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) | TSP_ORZ_DMAW_PROT_EN);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) & ~TSP_ORZ_DMAW_PROT_EN);
    }
}

void HAL_TSP_RemoveDupAVPkt(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | TSP_REMOVE_DUP_AV_PKT);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & ~TSP_REMOVE_DUP_AV_PKT);
    }
}

void HAL_TSP_RemoveDupAVFifoPkt(MS_U32 u32StreamId, MS_BOOL bEnable)
{
    MS_U32 u32Flag[6] = {TSP_REMOVE_DUP_VIDEO_PKT, TSP_REMOVE_DUP_AUDIO_PKT, TSP_REMOVE_DUP_AUDIOB_PKT, TSP_REMOVE_DUP_VIDEO3D_PKT};

    if(bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
            _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | u32Flag[u32StreamId]);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
            _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & ~u32Flag[u32StreamId]);
    }
}

void HAL_TSP_TEI_RemoveErrorPkt(MS_U32 u32PktType, MS_BOOL bEnable)
{
    // don't implement yet
}

MS_BOOL HAL_TSP_GetTSIF_Status(MS_U8 u8TsIfId, MS_U16* pu16Pad, MS_U16* pu16Clk, MS_BOOL* pbExtSync, MS_BOOL* pbParl)
{
    MS_U16 u16dta;
    MS_U32 u32data;

    *pu16Pad = 0xFFFF;
    *pu16Clk = TSP_CLK_DISABLE;
    *pbExtSync = FALSE;
    *pbParl = FALSE;

    if(u8TsIfId == 0x80)
    {
        *pu16Pad = (_HAL_REG16_R(&(_TspCtrl5[0].TS_MUX_CFG0)) >> TS_MUX_CFG_TSFI_MUX_SHIFT) & TS_MUX_CFG_TS0_MUX_MASK;
        *pu16Clk = (TSP_CLKGEN1_REG(REG_CLKGEN1_TSN_CLKFI) >> REG_CLKGEN1_TSN_CLK_TSFI_SHIFT) & REG_CLKGEN0_TSN_CLK_MASK;
        u16dta = _HAL_REG16_R(&_TspCtrl5[0].Ts_If_Fi_Cfg);
        *pbExtSync = (MS_BOOL)(u16dta & TSP_FIIF_EXT_SYNC_SEL);
        *pbParl = (MS_BOOL)(u16dta & TSP_FIIF_P_SEL);
        return TRUE;
    }
    else if(u8TsIfId >= TSP_IF_NUM)
    {
        return FALSE;
    }

    u16dta = _HAL_REG16_R(&(_TspCtrl5[0].TS_MUX_CFG0));

    switch(u8TsIfId)
    {
        case 0:
        default:
            u16dta >>= TS_MUX_CFG_TS0_MUX_SHIFT;
            *pu16Clk = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) >> REG_CLKGEN0_TSN_CLK_TS0_SHIFT) & REG_CLKGEN0_TSN_CLK_MASK;
            u32data = _HAL_REG32_R(&_TspCtrl[0].Hw_Config0);
            *pbExtSync = ((u32data & TSP_HW_CFG0_TSIF0_EXTSYNC) == TSP_HW_CFG0_TSIF0_EXTSYNC);
            *pbParl = ((u32data & TSP_HW_CFG0_TSIF0_PARL) == TSP_HW_CFG0_TSIF0_PARL);
            break;
        case 1:
            u16dta >>= TS_MUX_CFG_TS1_MUX_SHIFT;
            *pu16Clk = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) >> REG_CLKGEN0_TSN_CLK_TS1_SHIFT) & REG_CLKGEN0_TSN_CLK_MASK;
            u32data = _HAL_REG32_R(&_TspCtrl[0].Hw_Config2);
            *pbExtSync = ((u32data & TSP_HW_CFG2_TSIF1_EXTSYNC) == TSP_HW_CFG2_TSIF1_EXTSYNC);
            *pbParl = ((u32data & TSP_HW_CFG2_TSIF1_PARL) == TSP_HW_CFG2_TSIF1_PARL);
            break;
    }

    *pu16Pad = u16dta & TS_MUX_CFG_TS0_MUX_MASK;

    return TRUE;
}

MS_BOOL HAL_TSP_Check_FIFO_Overflow(MS_U32 u32StreamId)
{
    MS_U32 u32data = _HAL_REG32_R(&_TspCtrl[0].Idr_Read1);

    switch (u32StreamId)
    {
        case 0: // return VFifo status
            return ((u32data & TSP_VD_FIFO_OVERFLOW) == TSP_VD_FIFO_OVERFLOW);
        case 1: // return AFifo 0 status
            return ((u32data & TSP_AU_FIFO_OVERFLOW) == TSP_AU_FIFO_OVERFLOW);
        case 2: // return AFifo 1 status
            return ((u32data & TSP_AUB_FIFO_OVERFLOW) == TSP_AUB_FIFO_OVERFLOW);
        case 3: // return V3D Fifo status
            return ((u32data & TSP_V3D_FIFO_OVERFLOW) == TSP_V3D_FIFO_OVERFLOW);
        default:
            return FALSE;
    }
}

void HAL_TSP_HWPcr_SetSrcId(MS_U32 u32EngId, MS_U32 u32SrcId)
{
    _HAL_REG32_W(&_TspCtrl3[0].PIDFLR_PCR[u32EngId],
                (_HAL_REG32_R(&_TspCtrl3[0].PIDFLR_PCR[u32EngId]) & ~TSP_PIDFLT_PCR_SOURCE_MASK) | (u32SrcId << TSP_PIDFLT_PCR_SOURCE_SHIFT));
}

void HAL_TSP_HWPcr_SelSrc(MS_U32 u32EngId, MS_U32 u32Src)
{
#if (TSP_HWPCR_BY_HK == 1 || !defined(HWPCR_ENABLE))

    _HAL_REG32_W(&_TspCtrl[0].FIFO_Src,
                (_HAL_REG32_R(&_TspCtrl[0].FIFO_Src) & ~TSP_PCR0_SRC_MASK) | (u32Src << TSP_PCR0_SRC_SHIFT));
#else
    _HAL_TSP_CMD_Write_HWPCR_Reg(TSP_PCR0_SRC_MASK, (u32Src << TSP_PCR0_SRC_SHIFT));
#endif

}

void HAL_TSP_HWPcr_Reset(MS_U32 u32EngId, MS_BOOL bReset)
{
    MS_U32 u32value = TSP_PCR0_RESET;

#if (TSP_HWPCR_BY_HK == 1 || !defined(HWPCR_ENABLE))

    if(bReset)
    {
        _HAL_REG32_W(&_TspCtrl[0].FIFO_Src,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].FIFO_Src), u32value));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].FIFO_Src,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].FIFO_Src), u32value));
    }
#else
    if(bReset)
    {
        _HAL_TSP_CMD_Write_HWPCR_Reg(u32value, 0);
    }
    else
    {
        _HAL_TSP_CMD_Write_HWPCR_Reg(u32value, 1);
    }
#endif

}

void HAL_TSP_HWPcr_Read(MS_U32 u32EngId, MS_U32 *pu32Pcr, MS_U32 *pu32Pcr_32)
{
    MS_U32 u32Mask = TSP_PCR0_READ;
    MS_U16 u16value = TSP_HWINT2_PCR0_UPDATE_END;

    _HAL_REG32_W(&_TspCtrl[0].FIFO_Src,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].FIFO_Src), u32Mask));

    *pu32Pcr = _HAL_REG32_R(&_TspCtrl[0].HWPCR0_L);
    *pu32Pcr_32 = _HAL_REG32_R(&_TspCtrl[0].HWPCR0_H) & 0x00000001;

    _HAL_TSP_HwInt2_BitClr(u16value);

    _HAL_REG32_W(&_TspCtrl[0].FIFO_Src,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].FIFO_Src), u32Mask));
}

void HAL_TSP_HWPcr_Int_Enable(MS_U32 u32EngId, MS_BOOL bEnable)
{
    MS_U16 u16Mask = TSP_HWINT2_PCR0_UPDATE_END;

    if(bEnable)
    {
        _HAL_TSP_HwInt2_BitSet(u16Mask);
    }
    else
    {
        _HAL_TSP_HwInt2_BitClr(u16Mask);
    }
}

//--------------------------------------------------------------------------------------------------
// For STC part
//--------------------------------------------------------------------------------------------------
void HAL_TSP_Stc_ctrl(MS_U32 u32EngId, MS_U32 u32Sync)
{
    MS_U32 u32value = 0;

    /////////////Set STC control by HK////////////////
    // select synth from chip top : bit 1 -> 0 -> controlled by HK
    u32value = REG_CLKGEN0_STC_CW_SEL;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~u32value;

    // set HK STC synth CW
     //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    u32value = REG_CLKGEN0_DC0_STC_CW_L;
    TSP_CLKGEN0_REG(u32value) = (MS_U16)(u32Sync & 0xFFFF);
    u32value = REG_CLKGEN0_DC0_STC_CW_H;
    TSP_CLKGEN0_REG(u32value) = (MS_U16)(u32Sync >> 16);

    // set STC synth
    u32value = REG_CLKGEN0_STC_CW_EN;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~u32value;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= u32value;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~u32value;

    /////////////Set STC control by TSP////////////////
    // select synth from TSP      : bit 1 -> 1 -> controlled by TSP
    u32value = REG_CLKGEN0_STC_CW_SEL;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= u32value;

    // set TSP STC synth CW
    //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    u32value = 0x0021024c;
    HAL_REG32_IndW((REG32 *)(u32value<<1), u32Sync);

    // t2 , t3 had no 0x0021025c, it was add after t4, eanble synthesizer
    u32value = 0x01;
    HAL_REG32_IndW((REG32 *)(0x0021025c<<1), HAL_REG32_IndR((REG32 *)(0x0021025c<<1))|u32value);
    HAL_REG32_IndW((REG32 *)(0x0021025c<<1), HAL_REG32_IndR((REG32 *)(0x0021025c<<1))& ~u32value);
}

// GET MCU STC synth CW
MS_U32 HAL_TSP_GetSTCSynth(MS_U32 u32EngId)
{
    MS_U32 u32value = 0, u32sync = 0;

    u32value = REG_CLKGEN0_DC0_STC_CW_L;
    u32sync = (MS_U32)TSP_CLKGEN0_REG(u32value);
    u32value = REG_CLKGEN0_DC0_STC_CW_H;
    u32sync |= (((MS_U32)TSP_CLKGEN0_REG(u32value)) << 16);

    return u32sync;
}

void HAL_TSP_STC_Update_Disable(MS_U32 u32EngId, MS_BOOL bDisable)
{
    if(bDisable)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_cnt_33b_ld));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_cnt_33b_ld));
    }
}

MS_U32 HAL_TSP_GetSTC(MS_U32 u32EngId)
{
    if(HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_SYSTIME_MODE_STC64))
    {
        MS_U32 u32temp;

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].TsRec_Tail2_Pcr1) & TSP_PCR64_L16_MASK) >> TSP_PCR64_L16_SHFT;
        u32temp |= ((_HAL_REG32_R(&_TspCtrl[0].Pcr1) & 0xFFFF) << 16);
        return u32temp ;
    }
    else
    {
        return HAL_REG32_IndR((REG32 *)(0x00210244<< 1));
    }

    return 0;
}

MS_U32 HAL_TSP_GetSTC_32(MS_U32 u32EngId)
{
    if(HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_SYSTIME_MODE_STC64))
    {
        MS_U32 u32temp;

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr1) >> 16)& 0xFFFF;
        u32temp |= (((_HAL_REG32_R(&_TspCtrl[0].Pcr64_H) & TSP_PCR64_H16_MASK) & 0xFFFF) << 16);
        return u32temp ;
    }
    else
    {
        return (HAL_REG32_IndR((REG32 *)(0x00210248<< 1)) & 0x01);
    }
}

void HAL_TSP_SetSTC(MS_U32 u32EngId, MS_U32 u32STC, MS_U32 u32STC_32)
{
    if(HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_SYSTIME_MODE_STC64))
    {
        MS_U32 u32temp;

        u32temp = ((u32STC & 0xFFFF) << TSP_PCR64_L16_SHFT) |
                (_HAL_REG32_R(&_TspCtrl[0].TsRec_Tail2_Pcr1) & ~TSP_PCR64_L16_MASK);
        _HAL_REG32_W(&_TspCtrl[0].TsRec_Tail2_Pcr1, u32temp);

        u32temp = ((u32STC >> 16) & 0xFFFF) | ((u32STC_32 & 0xFFFF) << 16);
        _HAL_REG32_W(&_TspCtrl[0].Pcr1, u32temp);

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr64_H) & ~TSP_PCR64_H16_MASK) | ((u32STC_32 >> 16) & TSP_PCR64_H16_MASK);
        _HAL_REG32_W(&_TspCtrl[0].Pcr64_H, u32temp);
    }
    else
    {
        HAL_REG32_IndW((REG32 *)(0x00210244<< 1), u32STC);
        HAL_REG32_IndW((REG32 *)(0x00210248<< 1), u32STC_32  & 0x01);
    }
}

MS_BOOL HAL_TSP_SelectSTCEng(MS_U32 u32FltSrc, MS_U32 u32Eng)
{
    MS_U32 u32cmd = TSP_MCU_CMD_SEL_STC_ENG|((u32FltSrc >> TSP_PIDFLT_IN_SHIFT) << TSP_MCU_CMD_SEL_STC_ENG_FLTSRC_SHIFT)|u32Eng;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32cmd);

    while(_HAL_REG32_R(&_TspCtrl[0].MCU_Cmd) != 0);

    return TRUE;
}

#if 0
void HAL_TSP_SetSTC_32(MS_U32 u32EngId, MS_U32 u32STC_32)
{
    if(u32EngId == 1)
    {
        _HAL_REG32_W(&_TspCtrl[0].PCR64_2_H, u32STC_32);
        return;
    }

    if(HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_SYSTIME_MODE_STC64))
    {
        MS_U32 u32temp;

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr1) & ~ 0xFFFF0000) | ((u32STC_32 & 0xFFFF) << 16);
        _HAL_REG32_W(&_TspCtrl[0].Pcr1, u32temp);
        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr64_H) & ~TSP_PCR64_H16_MASK) | ((u32STC_32 >> 16) & TSP_PCR64_H16_MASK);
        _HAL_REG32_W(&_TspCtrl[0].Pcr64_H, u32temp);
    }
    else
    {
        HAL_REG32_IndW((REG32 *)(0x00210248<< 1), u32STC_32  & 0x01);
    }
}
#endif

void HAL_TSP_CmdQ_SetSTC(MS_U32 u32EngId, MS_U32 u32STC)
{
    _HAL_REG32_W(&_TspCtrl[0].Pcr.ML, u32STC);
}

void HAL_TSP_CmdQ_SetSTC_32(MS_U32 u32EngId, MS_U32 u32STC_32)
{
    _HAL_REG32L_W(&_TspCtrl[0].Pcr.H32, u32STC_32 & 0x01);
}

MS_U32 HAL_TSP_CmdQ_GetSTC(MS_U32 u32EngId)
{
    return (_HAL_REG32_R(&_TspCtrl[0].Pcr.ML));
}

MS_U32  HAL_TSP_CmdQ_GetSTC_32(MS_U32 u32EngId)
{
    return (_HAL_REG32L_R(&_TspCtrl[0].Pcr.H32) & 0x01);
}

MS_BOOL HAL_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Opt)
{
    MS_U32 i = 0;
    MS_U32 u32Enable = 0;
    MS_U32 u32Cmd = 0;

    if(u8Opt & HAL_TSP_STC_UPDATE_HK)
    {
        u32Enable = 1;
    }
    if(u8Opt & HAL_TSP_STC_UPDATE_UPDATEONCE)
    {
        u32Cmd = TSP_MCU_CMD_CTRL_STC_UPDATE_ONCE;
    }

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, u32Enable);

    u32Cmd |= TSP_MCU_CMD_CTRL_STC_UPDATE;
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32Cmd);

    while (i< 4)
    {
        if (0 == _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
        {
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
            return TRUE;
        }
        i++;
        _delay();
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, 0);
    return FALSE;
}

MS_BOOL HAL_TSP_GetPcr(MS_U32 u32EngId, MS_U32 *pu32Pcr_32, MS_U32 *pu32Pcr)
{
    MS_U32 i = 0;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data1, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_PCR_GET | (u32EngId << TSP_MCU_CMD_NMATCH_FLT_SHFT));
    while (i< 4)
    {
        if (0 == _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
        {
            *pu32Pcr = _HAL_REG32_R(&_TspCtrl[0].MCU_Data0);
            *pu32Pcr_32 = _HAL_REG32_R(&_TspCtrl[0].MCU_Data1);
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data1, 0);
            return ((0!= *pu32Pcr) || (0!= *pu32Pcr_32))? TRUE: FALSE;
        }
        i++;
        _delay();
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, 0);
    return FALSE;
}

MS_BOOL HAL_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd)
{
    MS_U32 u32opt = ((MS_U32)bAdd & 0xFF) << TSP_MCU_CMD_SET_STC_OFFSET_OPTION_SHIFT;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, u32Offset);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_SET_STC_OFFSET | u32opt | u32EngId);

    while(_HAL_REG32_R(&_TspCtrl[0].MCU_Cmd) != 0);

    return TRUE;
}


MS_BOOL HAL_TSP_CmdQ_IsEmpty(void)
{
    if (_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_CMDQ_EMPTY)
    {
        return TRUE;
    }
    return FALSE;
}

void HAL_TSP_Int_Disable(MS_U32 u32Mask)
{
    _HAL_REG16_W(&_TspCtrl[0].HwInt_Stat,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat)|0xFF00, (MS_U16)(u32Mask>>8)));
}

void HAL_TSP_Int2_Disable(MS_U32 u32Mask)
{
    _HAL_TSP_HwInt2_BitClr((MS_U16)(u32Mask >> 8));
}

void HAL_TSP_Int_Enable(MS_U32 u32Mask)
{
    _HAL_REG16_W(&_TspCtrl[0].HwInt_Stat,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat)|0xFF00, (MS_U16)(u32Mask>>8)));
}

void HAL_TSP_Int2_Enable(MS_U32 u32Mask)
{
    _HAL_TSP_HwInt2_BitSet((MS_U16)(u32Mask>>8));
}

#define ADDR_SWINT2_L           (_u32RegBase+ 0x2db4)
#define ADDR_SWINT2_H           (_u32RegBase+ 0x2db8)
void HAL_TSP_Int_ClearSw(void)
{
    if (_bIsHK)
    {
        _HAL_REG32_W(&_TspCtrl[0].SwInt_Stat, 0);
    }
    else
    {
        REG16_T(ADDR_SWINT2_L) = 0;
        REG16_T(ADDR_SWINT2_H) = 0;
    }
}
#undef ADDR_SWINT2_L
#undef ADDR_SWINT2_H

void HAL_TSP_Int_ClearHw(MS_U32 u32Mask)
{
    _HAL_REG16_W(&_TspCtrl[0].HwInt_Stat,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat)|0xff00, (MS_U16)u32Mask));
}

void HAL_TSP_Int_ClearHw2(MS_U32 u32Mask)
{
    _HAL_TSP_HwInt2_BitClr((MS_U16)u32Mask);
}

MS_U32 HAL_TSP_CmdQ_CmdCount(void)
{
    return (((_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_CMDQ_CNT_MASK)>>TSP_CMDQ_CNT_SHFT));
}

void HAL_TSP_CmdQ_TsDma_Reset(void)
{
    _HAL_REG32_W(&_TspCtrl[0].TsDma_Ctrl_CmdQ, 0);
}

MS_BOOL HAL_TSP_CmdQ_Reset(void)
{
    int ii = 0;
    _HAL_TSP_HW_Lock();
    _HAL_HALTSP_LOCK();
    //_HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
    //    SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FORCE_XIU_WRDY));
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_CMDQ_RESET));
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_CMDQ_RESET));
    //_HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
    //    RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FORCE_XIU_WRDY));

    _HAL_TSP_HW_Unlock();
    _HAL_HALTSP_UNLOCK();

    //reset the last data that hw is excuting --> HW new design
    _HAL_REG16_W(&_TspCtrl5[0].S2PCfg1,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].S2PCfg1), TSP_S2PCFG1_WB_FSM_RST));

    for(ii = 0; ii < 100; ii++)
    {
        if(_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_TSDMA_CTRL_DONE)
        {
            break;
        }
        MsOS_DelayTask(1);
    }
    _HAL_REG16_W(&_TspCtrl5[0].S2PCfg1,
                RESET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].S2PCfg1), TSP_S2PCFG1_WB_FSM_RST));

    if(ii == 100)
    {
        return FALSE;
    }

    //rst_ts_fin
    _HAL_REG32_W(&_TspCtrl[0].reg160C, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_TIMESTAMP_RESET));
    _HAL_REG32_W(&_TspCtrl[0].reg160C, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_TIMESTAMP_RESET));

    return TRUE;

}

MS_U8 HAL_TSP_Get_CmdQFifoLevel(void)
{
    return (MS_U8)((_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_CMDQ_WR_LEVEL_MASK) >> TSP_CMDQ_WR_LEVEL_SHFT);
}

void HAL_TSP_WbDmaEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_WB_DMA_RESET));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_WB_DMA_RESET));
    }
}

// u32TSSrc: 0 -> TS0, 1 -> File, 2 -> TS1
// u32GroupId: 0 -> filter0~filter31, 1 -> filter32~filter63
MS_U32 HAL_TSP_Scmb_Status(MS_U32 u32TSSrc, MS_U32 u32GroupId, MS_U32 u32PidFltId)
{
    MS_U32              u32PIDFltMask = u32PidFltId;
    MS_U32              u32ScmbSts = 0;

    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
        (_HAL_REG32_R(&_TspCtrl[0].reg15b4) & ~TSP_MATCH_PID_SRC_MASK) | (u32TSSrc << TSP_MATCH_PID_SRC_SHIFT));

    if(u32PidFltId != 0xFFFFFFFF)
    {
        u32PIDFltMask = (1 << (u32PidFltId & 0x1F));
    }

    _HAL_REG16_W(&_TspCtrl5[0].S2PCfg,
        (_HAL_REG16_R(&_TspCtrl5[0].S2PCfg) & ~TSP_MATCH_PID_SEL_MASK) | ((MS_U16)u32GroupId << TSP_MATCH_PID_SEL_SHIFT));

    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_MATCH_PID_LD));

    u32ScmbSts = HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].TsPidScmbStatTsin), u32PIDFltMask);

    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_MATCH_PID_LD));

    if(u32PIDFltMask != 0xFFFFFFFF)
    {
        u32ScmbSts = ((u32ScmbSts > 0) ? 1: 0);
    }

    return u32ScmbSts;
}


#if 0
void HAL_TSP_CPU_SetBase(MS_U32 u32Addr, MS_U32 u32Size)
{
#if (!LINUX_TEST)
    // TSP FW running in QMEM
    _HAL_TSP_FW_load(u32Addr, u32Size, TRUE, TRUE, TRUE);
#else
    // only for linux
    // @FIXME: abstract this later
    void* pBuf = NULL;
    MS_U32 u32PhysAddr = 0;

#if 0
    if (NULL == (pBuf = MsOS_AllocateMemory (u32Size, gs32NonCachedPoolID)))
    {
        MS_ASSERT(0);
    }

    memcpy(pBuf, (void*)u32Addr, u32Size);
    u32PhysAddr = (MS_U32)VA2PA(pBuf);
    printf("firmware 0x%08x 0x%08x\n", (MS_U32)pBuf, u32Addr);
    _HAL_TSP_FW_load(u32PhysAddr, u32Size, TRUE, TRUE, TRUE);
    MsOS_FreeMemory(pBuf, gs32NonCachedPoolID);
#else
    if (NULL == (pBuf = MsOS_AllocateMemory (72*1024*1024, gs32NonCachedPoolID)))
    {
        MS_ASSERT(0);
    }
    u32PhysAddr = 60*1024*1024;
    memcpy(PA2KSEG1(u32PhysAddr), (void*)u32Addr, u32Size);
    printf("firmware 0x%08x 0x%08x\n", (MS_U32)PA2KSEG1(u32PhysAddr), u32PhysAddr);
    _HAL_TSP_FW_load(u32PhysAddr, u32Size, TRUE, TRUE, TRUE);
    MsOS_FreeMemory(pBuf, gs32NonCachedPoolID);
#endif
#endif
}
#else
void HAL_TSP_CPU_SetBase(MS_U32 u32Addr, MS_U32 u32Size)
{
    printf("[%s][%d] load firmware (address, size) = (0x%08x, 0x%08x)\n", __FUNCTION__, __LINE__, (int)u32Addr, (int)u32Size);
    _HAL_TSP_FW_load(u32Addr, u32Size, TRUE, TRUE, TRUE);
}

#endif // #if 0

MS_BOOL HAL_TSP_Alive(void)
{
    MS_U32 i = 0;
    MS_U32 u32Data;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_ALIVE);
    while (i< 4)
    {
        if (0 == _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
        {
            u32Data = _HAL_REG32_R(&_TspCtrl[0].MCU_Data0);
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
            return (TSP_MCU_DATA_ALIVE == u32Data)? TRUE: FALSE;
        }
        i++;
        _delay();
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, 0);
    return FALSE;
}

void HAL_TSP_SetOwner(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_BOOL bOwner)
{
    MS_U32 u32HkId;
    REG_SecFlt* pSecFilter = _HAL_TSP_SECFLT(u32EngId, u32SecFltId);

    if (_bIsHK)
    {
        u32HkId = (bOwner)? 0: 1;
    }
    else
    {
        u32HkId = (bOwner)? 1: 0;
    }
    HAL_REG32_IndW((REG32 *)&pSecFilter->RmnReqCnt, (HAL_REG32_IndR((REG32 *)&pSecFilter->RmnReqCnt) & ~TSP_SECFLT_OWNER_MASK) |
                                    ((u32HkId << TSP_SECFLT_OWNER_SHFT) & TSP_SECFLT_OWNER_MASK));
}

void HAL_TSP_FileIn_Set(MS_BOOL bset)
{
    if (bset)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_FILEIN192_EN));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_FILEIN192_EN));
    }
}

//Reset file-in timestamp
void HAL_TSP_ResetTimeStamp(void)
{
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_TIMESTAMP_RESET));
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_TIMESTAMP_RESET));
}

MS_U32  HAL_TSP_GetPVRTimeStamp(MS_U8 u8PVRId)
{
    MS_U32 u32lpcr = 0;

    switch(u8PVRId)
    {
        case 0:
        default:
            _HAL_REG32_W(&_TspCtrl[0].reg160C,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_PVR1_LPCR1_RLD));
            u32lpcr = _HAL_REG32_R(&_TspCtrl[0].PVR1_LPcr1);
            _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_PVR1_LPCR1_RLD));
            break;
        case 1:
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_LPCR1_RLD));
            u32lpcr = _HAL_REG32_R(&_TspCtrl[0].PVR2_LPCR1);
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_LPCR1_RLD));
            break;
    }

    return u32lpcr;
}

void HAL_TSP_SetPVRTimeStamp(MS_U8 u8PVRId, MS_U32 u32Stamp)
{
    switch(u8PVRId)
    {
        case 0:
        default:
            _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_PVR1_LPCR1_WLD));
            _HAL_REG32_W(&_TspCtrl[0].PVR1_LPcr1,u32Stamp);
            _HAL_REG32_W(&_TspCtrl[0].reg160C,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_PVR1_LPCR1_WLD));
            break;
        case 1:
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_LPCR1_WLD));
            _HAL_REG32_W(&_TspCtrl[0].PVR2_LPCR1,u32Stamp);
            _HAL_REG32_W(&_TspCtrl[0].PVR2_Config,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_LPCR1_WLD));
            break;
    }
}

void HAL_TSP_SetPVRTimeStampClk(MS_U8 u8PVRId, MS_U32 u32ClkSrc)
{
    MS_U32 u32Flag = 0;
    switch (u8PVRId)
    {
        case 0:
            u32Flag = TSP_PVR1_CLK_STAMP_27_EN;
            break;
        case 1:
            u32Flag = TSP_PVR2_CLK_STAMP_27_EN;
            break;
        default:
            break;
    }
    if(u32ClkSrc == 0x0) // 90K
    {
        _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), u32Flag));
    }
    else // 27M
    {
        _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), u32Flag));
    }

}

MS_U32 HAL_TSP_GetPlayBackTimeStamp(void)
{
    MS_U32 u32value = 0;

    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_RLD));

    u32value = _HAL_REG32_R(&_TspCtrl[0].LPcr2);

    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_RLD));


    return u32value;
}

void HAL_TSP_SetPlayBackTimeStamp(MS_U32 u32Stamp)
{
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_WLD));
    _HAL_REG32_W(&_TspCtrl[0].LPcr2,u32Stamp);
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_WLD));
}

void HAL_TSP_SetPlayBackTimeStampClk(MS_U8 u8Id, MS_U32 u32ClkSrc)
{
    if(u32ClkSrc == 0x0) // 90K
    {
        _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_TSIF0_CLK_STAMP_27_EN));
    }
    else // 27M
    {
        _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_TSIF0_CLK_STAMP_27_EN));
    }
}

MS_U32 HAL_TSP_GetFileInTimeStamp(void)
{
    return _HAL_REG32_R(&_TspCtrl[0].TimeStamp_FileIn);
}

MS_BOOL HAL_TSP_GetFilinReadAddr(MS_PHYADDR* pu32ReadAddr)
{
    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_FILEIN_RADDR_READ));

    *pu32ReadAddr = (_HAL_REG32_R(&_TspCtrl[0].TsFileIn_RPtr) << MIU_BUS);

    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_FILEIN_RADDR_READ));

    return TRUE;
}

void HAL_TSP_SetDMABurstLen(MS_U32 u32Len)
{
    MS_U32 u32data = 0;

    switch(u32Len)
    {
        case 1:
            u32data = 0;
            break;
        case 2:
            u32data = 1;
            break;
        case 4:
            u32data = 2;
            break;
        default:
            return;
    }

    if((u32Len == 2) || (u32Len == 4))
    {
        _HAL_REG16_W(&_TspCtrl3[0].HWeco0,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl3[0].HWeco0), HW_ECO_BURST_NEW_MODE0|HW_ECO_BURST_NEW_MODE1));
    }
    else
    {
        _HAL_REG16_W(&_TspCtrl3[0].HWeco0,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl3[0].HWeco0), HW_ECO_BURST_NEW_MODE0|HW_ECO_BURST_NEW_MODE1));
    }

    _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_SEC_DMA_BURST_EN));

    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
        _HAL_REG32_R(&_TspCtrl[0].Hw_Config4) | ((u32Len<<TSP_HW_DMA_MODE_SHIFT)&TSP_HW_DMA_MODE_MASK));
}

void HAL_TSP_PVR_PacketMode(MS_U8 u8PVRId, MS_BOOL bSet)
{
    REG32 *pReg = 0;
    MS_U32 u32Flag = 0;

    switch(u8PVRId)
    {
        case 0:
        default:
            pReg = &_TspCtrl[0].reg160C;
            u32Flag = TSP_RECORD192_EN;
            break;
        case 1:
            pReg = &_TspCtrl[0].PVR2_Config;
            u32Flag = TSP_PVR2_PKT192_EN;
            break;
    }

    if (bSet)
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32Flag));
    }
    else
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32Flag));
    }
}

MS_BOOL HAL_TSP_PVR_Fifo_Block_Disable(MS_U8 u8PVRId, MS_BOOL bDisable)
{
    if(bDisable == TRUE)
    {
        switch(u8PVRId)
        {
            case 0:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR1_BLOCK_DIS));
                break;
            case 1:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_BLOCK_DIS));
                break;
            default:
                return FALSE;
        }
    }
    else
    {
        switch(u8PVRId)
        {
            case 0:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR1_BLOCK_DIS));
                break;
            case 1:
                _HAL_REG32_W(&_TspCtrl[0].PVR2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PVR2_Config), TSP_PVR2_BLOCK_DIS));
                break;
            default:
                return FALSE;
        }
    }

    return TRUE;
}

void HAL_ResetAll(void)
{
    printf("Reset ALL registers\n");
    //_HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
    //    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_CPU_EN));
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_DMA_RST));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_WB_DMA_RESET));
    _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_SW_RST));

    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_DMA_RST));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_WB_DMA_RESET));
    _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_SW_RST));
}

#define CKG_CLK_STCSYN         0x160C       //0x03
    #define CKG_CLK_STCSYN_MASK         0x0003
    #define CKG_CLK_STCSYN_432M         0x0001
#define CKG_TS0_TS1             0x16A0       //0x28
    #define CLK_TS0_DISABLE             0x0001
    #define CLK_TS0_INVERT              0x0002
    #define CLK_TS0_CLK_MASK            0x001C
    #define CLK_TS1_DISABLE             0x0100
    #define CLK_TS1_INVERT              0x0200
    #define CLK_TS1_CLK_MASK            0x1C00
#define CKG_TS2_TS4             0x16A4       //0x29   //s2P
    #define CLK_TS4_DISABLE             0x0100
    #define CLK_TS4_INVERT              0x0200
    #define CLK_TS4_CLK_MASK            0x1C00
    #define CKG_TSP_GPONLY_MASK         0x4000
#define CKG_TSP_STC0            0x16A8    //0x2a
    #define CLK_TSP_DISABLE             0x0001
    #define CLK_TSP_INVERT              0x0002
    #define CLK_TSP_CLK_MASK            0x000C
    #define CLK_PAR_DISABLE             0x0010
    #define CLK_PAR_INVERT              0x0020
    #define CLK_PAR_CLK_MASK            0x0040
    #define CLK_STC_DISABLE             0x0100
    #define CLK_STC_INVERT              0x0200
    #define CLK_STC_CLK_MASK            0x0C00
#define CKG_TSP_STAMP           0x16AC    //0x2b
    #define CLK_STAM_DISABLE            0x0010
    #define CLK_STAM_INVERT             0x0020
    #define CLK_STAM_CLK_MASK           0x00C0
#define CKG1_TS_SAMPLE          0x6688    //0x22
    #define CLK_TS_SAMPLE_DISABLE       0x0010
    #define CLK_TS_SAMPLE_INVERT        0x0020
#define CKG_TSP_TSFI            0x6698    //0x26
    #define CKG_TSP_TSFI_DISABLE        0x0100
    #define CKG_TSP_TSFI_INVERT         0x0200
    #define CKG_TSP_TSFI_CLK_MASK       0x1C00
#define CKG_TSP_SW_CLK          0x17E0  // 0x78
    #define CKG_TSP_SW_CLK_MASK         0x0001
#define CHIP_TSP_BOOT_CLK_SEL   0x3D50 //0x54
    #define CHIP_TSP_BOOT_CLK_SEL_MASK  0x0100

void HAL_TSP_PowerCtrl(MS_BOOL bOn)
{
    if (bOn)
    {
        // Enable Gate Clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TS2_TS4), RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TS2_TS4)), CKG_TSP_GPONLY_MASK));

        // Enable TSP Clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STC0),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STC0)), (CLK_TSP_DISABLE|CLK_TSP_INVERT|CLK_TSP_CLK_MASK)));

        //TSP select SRAM
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CHIP_TSP_BOOT_CLK_SEL), RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CHIP_TSP_BOOT_CLK_SEL)), CHIP_TSP_BOOT_CLK_SEL_MASK));

        //Select SRAM
        _HAL_REG16_W(&(_TspCtrl2[0].Qmem_Dbg), SET_FLAG1(_HAL_REG16_R(&(_TspCtrl2[0].Qmem_Dbg)), QMEM_DBG_TSP_SEL_SRAM));

        // Set TSP SW clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_SW_CLK), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_SW_CLK)), CKG_TSP_SW_CLK_MASK));

        // Select STC Sync 432M
         _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_CLK_STCSYN),
            (_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_CLK_STCSYN)) & ~CKG_CLK_STCSYN_MASK) | CKG_CLK_STCSYN_432M);

        // Enable CLK_PARSER clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STC0),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STC0)), (CLK_PAR_DISABLE|CLK_PAR_INVERT|CLK_PAR_CLK_MASK)));

        // Enable TS0 clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TS0_TS1),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TS0_TS1)), (CLK_TS0_DISABLE|CLK_TS0_INVERT|CLK_TS0_CLK_MASK)));

        // Enable TS1 clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TS0_TS1),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TS0_TS1)), (CLK_TS1_DISABLE|CLK_TS1_INVERT|CLK_TS1_CLK_MASK)));

        // Enable STC clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STC0),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STC0)), (CLK_STC_DISABLE|CLK_STC_INVERT|CLK_STC_CLK_MASK)));

        // Enable TIMESTAMP clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STAMP),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STAMP)), (CLK_STAM_DISABLE|CLK_STAM_INVERT|CLK_STAM_CLK_MASK)));

        //Enable TS sample clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG1_TS_SAMPLE),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG1_TS_SAMPLE)), (CLK_TS_SAMPLE_DISABLE|CLK_TS_SAMPLE_INVERT)));

    }
    else
    {
        // DISABLE Gate Clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TS2_TS4), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TS2_TS4)), CKG_TSP_GPONLY_MASK));

        // Disable TS sample clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG1_TS_SAMPLE), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG1_TS_SAMPLE)), CLK_TS_SAMPLE_DISABLE));

        // Disable TS0 clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TS0_TS1), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TS0_TS1)), CLK_TS0_DISABLE));

        // Disable TS1 clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TS0_TS1), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TS0_TS1)), CLK_TS1_DISABLE));

        // Disable STC clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STC0), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STC0)), CLK_STC_DISABLE));

        // Disable CLK_PARSER clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STC0), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STC0)), CLK_PAR_DISABLE));

        // Disable TIMESTAMP clock
        _HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STAMP), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STAMP)), CLK_STAM_DISABLE));

        // Disable TSP clock
        //_HAL_REG32L_W((REG32_L *)(_u32RegBase+CKG_TSP_STC0), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_u32RegBase+CKG_TSP_STC0)), CLK_TSP_DISABLE));

        //Reset PE Pad
        if(_bTsPadUsed[0] == TRUE)
        {
            //TSP_TOP_REG(REG_TOP_TS0_PE) &= ~REG_TOP_TS0_PE_MASK;
            _bTsPadUsed[0] = FALSE;
        }
        if(_bTsPadUsed[1] == TRUE)
        {
            //TSP_TOP_REG(REG_TOP_TS1_PE) &= ~REG_TOP_TS1_PE_MASK;
            _bTsPadUsed[1] = FALSE;
        }
        if(_bTsPadUsed[2] == TRUE)
        {
            //TSP_TOP_REG(REG_TOP_TS2_PE) &= ~REG_TOP_TS2_PE_MASK;
            _bTsPadUsed[2] = FALSE;
        }
    }
}
#undef CKG_CLK_MPLLSYN
    #undef CKG_CLK_MPLLSYN_MASK
    #undef CKG_CLK_MPLLSYN_432M
#undef CKG_TS0_TS1
    #undef CLK_TS0_DISABLE
    #undef CLK_TS0_INVERT
    #undef CLK_TS0_CLK_MASK
    #undef CLK_TS1_DISABLE
    #undef CLK_TS1_INVERT
    #undef CLK_TS1_CLK_MASK
#undef CKG_TS2_TS4
    #undef CLK_TS4_DISABLE
    #undef CLK_TS4_INVERT
    #undef CLK_TS4_CLK_MASK
    #undef CKG_TSP_GPONLY_MASK
#undef CKG_TSP_STC0
    #undef CLK_TSP_DISABLE
    #undef CLK_TSP_INVERT
    #undef CLK_TSP_CLK_MASK
    #undef CLK_PAR_DISABLE
    #undef CLK_PAR_INVERT
    #undef CLK_PAR_CLK_MASK
    #undef CLK_STC_DISABLE
    #undef CLK_STC_INVERT
    #undef CLK_STC_CLK_MASK
#undef CKG_TSP_STAMP
    #undef CLK_STAM_DISABLE
    #undef CLK_STAM_INVERT
    #undef CLK_STAM_CLK_MASK
#undef CKG1_TS_SAMPLE
    #undef CLK_TS_SAMPLE_DISABLE
    #undef CLK_TS_SAMPLE_INVERT
#undef CKG_TSP_TSFI
    #undef CKG_TSP_TSFI_DISABLE
    #undef CKG_TSP_TSFI_INVERT
    #undef CKG_TSP_TSFI_CLK_MASK
#undef CKG_TSP_SW_CLK
    #undef CKG_TSP_SW_CLK_MASK
#undef CHIP_TSP_BOOT_CLK_SEL
    #undef CHIP_TSP_BOOT_CLK_SEL_MASK

MS_U32 HAL_TSP_GetDBGPortInfo(MS_U32 u32dbgsel)
{
    _HAL_REG32_W(&_TspCtrl[0].PKT_CNT, (TSP_DBG_SEL_MASK&(u32dbgsel<<TSP_DBG_SEL_SHIFT)));
    return (_HAL_REG32_R(&_TspCtrl[0].TSP_Debug)&TSP_DEBUG_MASK);
}

void HAL_TSP_Enable_ValidSync_Dectect(void)
{
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_VALID_FALLING_DETECT));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_SYNC_RISING_DETECT));
}

void HAL_Reset_WB(void)
{
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_WB_DMA_RESET));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_WB_DMA_RESET));
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_DMA_RST));
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_DMA_RST));
}

//0: VQ0, 1: VQ_file, 2: VQ1, 3: VQ_2
void HAL_TSP_SetVQBuffer(MS_U8 u8VQId, MS_PHYADDR u32BaseAddr, MS_U32 u32BufLen)
{
    REG32 *pReg = 0;

    switch(u8VQId)
    {
        case 0:
        default:
            pReg = &(_TspCtrl[0].VQ0_BASE);
            break;
        case 1:
            pReg = &(_TspCtrl[0].VQ1_Base);
            break;
        case 2:
            pReg = &(_TspCtrl[0].VQ2_Base);
            break;
    }
    _HAL_REG32_W(pReg, (u32BaseAddr >> MIU_BUS));

    switch(u8VQId)
    {
        case 0:
        default:
            pReg = &(_TspCtrl[0].VQ0_CTRL);
            break;
        case 1:
            pReg = &(_TspCtrl[0].VQ1_Config);
            break;
        case 2:
            pReg = &(_TspCtrl[0].VQ2_Config);
            break;
    }
    _HAL_REG32_W(pReg, (_HAL_REG32_R(pReg) & ~TSP_VQ0_SIZE_208PK_MASK)
        | ((u32BufLen/VQ_PACKET_UNIT_LEN) << TSP_VQ0_SIZE_208PK_SHIFT));

}

void HAL_TSP_VQueue_Enable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        // Reset VQ before VQ enable.
        _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL), TSP_VQ0_RESET));
        _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL), TSP_VQ0_RESET));

        _HAL_REG32_W(&_TspCtrl[0].VQ1_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ1_Config), TSP_VQ1_RESET));
        _HAL_REG32_W(&_TspCtrl[0].VQ1_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ1_Config), TSP_VQ1_RESET));

        _HAL_REG32_W(&_TspCtrl[0].VQ2_Config, SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ2_Config), TSP_VQ2_RESET));
        _HAL_REG32_W(&_TspCtrl[0].VQ2_Config, RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ2_Config), TSP_VQ2_RESET));

        //_HAL_REG32_W(&_TspCtrl[0].reg163C,
        //  SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg163C), TSP_ALL_VALID_EN));
        _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
          SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), (TSP_VQ_EN/*|TSP_VQ2PINGPONG_EN*/)));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_VQ_EN));
    }
}

void HAL_TSP_VQueue_Reset(MS_U8 u8VQId)
{
    REG32 *pReg = &_TspCtrl[0].VQ0_BASE;
    MS_U32 u32flag = 0;

    switch(u8VQId)
    {
        case 0:
        default:
            pReg = &_TspCtrl[0].VQ0_CTRL;
            u32flag = TSP_VQ0_RESET;
            break;
        case 1:
            pReg = &_TspCtrl[0].VQ1_Config;
            u32flag = TSP_VQ1_RESET;
            break;
        case 2:
            pReg = &_TspCtrl[0].VQ2_Config;
            u32flag = TSP_VQ2_RESET;
            break;
    }

    _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32flag));
}

void HAL_TSP_VQueue_OverflowInt_En(MS_U8 u8VQId, MS_BOOL bEnable)
{
    REG32 *pReg = &_TspCtrl[0].VQ0_BASE;
    MS_U32 u32flag = 0;

    switch(u8VQId)
    {
        case 0:
        default:
            pReg = &_TspCtrl[0].VQ0_CTRL;
            u32flag = TSP_VQ0_OVERFLOW_INT_EN;
            break;
        case 1:
            pReg = &_TspCtrl[0].VQ1_Config;
            u32flag = TSP_VQ1_OVF_INT_EN;
            break;
        case 2:
            pReg = &_TspCtrl[0].VQ2_Config;
            u32flag = TSP_VQ2_OVF_INT_EN;
            break;
    }

    if (bEnable)
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32flag));
        _HAL_TSP_HwInt2_BitSet(TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW >> TSP_HWINT2_STATUS_SHIFT);
    }
    else
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32flag));
        _HAL_TSP_HwInt2_BitClr(TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW >> TSP_HWINT2_STATUS_SHIFT);
    }
}

void HAL_TSP_VQueue_Clr_OverflowInt(MS_U8 u8VQId)
{
    REG32 *pReg = 0;
    MS_U32 u32flag = 0;
    MS_U32 u32data = 0;

    switch(u8VQId)
    {
        case 0:
        default:
            pReg = &_TspCtrl[0].VQ0_CTRL;
            u32flag = TSP_VQ0_CLR_OVERFLOW_INT;
            break;
        case 1:
            pReg = &_TspCtrl[0].VQ1_Config;
            u32flag = TSP_VQ1_CLR_OVF_INT;
            break;
        case 2:
            pReg = &_TspCtrl[0].VQ2_Config;
            u32flag = TSP_VQ2_CLR_OVF_INT;
            break;
    }
    u32data = _HAL_REG32_R(pReg);

    _HAL_REG32_W(pReg, u32data | u32flag);
    _HAL_REG32_W(pReg, u32data & ~u32flag);

    _HAL_TSP_HwInt2_BitClr(TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW);

}

void HAL_TSP_Set_Req_VQ_RX_Threshold(MS_U8 u8req_len)
{
    MS_U32 u32Value = TSP_REQ_VQ_RX_THRESHOLD_LEN1;

    switch(u8req_len)
    {
        case 1:
            u32Value = TSP_REQ_VQ_RX_THRESHOLD_LEN1;
            break;
        case 2:
            u32Value = TSP_REQ_VQ_RX_THRESHOLD_LEN2;
            break;
        case 4:
            u32Value = TSP_REQ_VQ_RX_THRESHOLD_LEN4;
            break;
        case 8:
            u32Value = TSP_REQ_VQ_RX_THRESHOLD_LEN8;
            break;
        default:
            break;
    }

    _HAL_REG32_W(&_TspCtrl[0].VQ_PIDFLT_CTRL,
        (_HAL_REG32_R(&_TspCtrl[0].VQ_PIDFLT_CTRL) & ~TSP_REQ_VQ_RX_THRESHOLD_MASKE) | u32Value);
}

MS_U32 HAL_TSP_Get_VQStatus(void)
{
    return (_HAL_REG32_R(&_TspCtrl[0].VQ_STATUS) & TSP_VQ_STATUS_MASK);
}

void HAL_TSP_VQBlock_Disable(MS_U8 u8VQId, MS_BOOL bDisable)
{
    MS_U32 u32Value = 0;

    switch(u8VQId)
    {
        case 1: u32Value = TSP_VQTX0_BLOCK_DIS;
            break;
        case 2: u32Value = TSP_VQTX1_BLOCK_DIS;
            break;
        case 4: u32Value = TSP_VQTX2_BLOCK_DIS;
            break;
    }

    if(bDisable)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), u32Value));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), u32Value));
    }
}

// Addr[0]                         -----> PVr1
// Addr[1]                         -----> Section
// Addr[2]                         -----> Section
// Addr[3]                         -----> PVR2
// The range can be written:  u32StartAddr <= x < u32EndAddr
// Protection range: x >= u32EndAddr && x < u32StartAddr
MS_BOOL HAL_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_U32* pu32StartAddr, MS_U32* pu32EndAddr)
{
    MS_U8 u8ii;

    if (bEnable)
    {
        for(u8ii = 0; u8ii < 4; u8ii++)
        {
            if(pu32StartAddr[u8ii] == pu32EndAddr[u8ii])
                pu32StartAddr[u8ii] += (1 << MIU_BUS);
        }

        _HAL_REG32_W(&_TspCtrl[0].DMAW_LBND0, pu32StartAddr[0] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_UBND0, pu32EndAddr[0] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_LBND1, pu32StartAddr[1] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_UBND1, pu32EndAddr[1] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_LBND2, pu32StartAddr[2] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_UBND2, pu32EndAddr[2] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_LBND4, pu32StartAddr[3] >> MIU_BUS);
        _HAL_REG32_W(&_TspCtrl[0].DMAW_UBND4, pu32EndAddr[3] >> MIU_BUS);

        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_SEC_CB_PVR2_DAMW_PROTECT_EN)); // pvr1,VQ2_tx, VQ3_tx protect
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_SEC_CB_PVR2_DAMW_PROTECT_EN));
    }

    return TRUE;

}

MS_U32 HAL_TSP_Get_FW_VER(void)
{
    MS_U32 i = 0;
    MS_U32 u32Data = 0;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_VERSION_GET);
    while (i< 4)
    {
        if (0 == _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
        {
            u32Data = _HAL_REG32_R(&_TspCtrl[0].MCU_Data0);
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
            return u32Data;
        }
        i++;
        _delay();
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, 0);
    return u32Data;
}

MS_BOOL HAL_TSP_Check_FW_VER(void)
{
    MS_U32  u32FWVer;

    u32FWVer = HAL_TSP_Get_FW_VER();
    if((u32FWVer >> 16) != TSP_FW_DEVICE_ID)
    {
        printf("\nWarning: TSP FW not match!! FW version: 0x%08lx\n\n", u32FWVer);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_SetFwDbgMem(MS_PHYADDR phyAddr, MS_U32 u32Size)
{
    MS_U32 i = 0;

    if(IsCover(phyAddr, phyAddr + u32Size, OPENRISC_IP_1_ADDR, OPENRISC_IP_1_ADDR + OPENRISC_IP_1_SIZE) ||
    IsCover(phyAddr, phyAddr + u32Size, OPENRISC_IP_2_ADDR, OPENRISC_IP_2_ADDR + OPENRISC_IP_2_SIZE) ||
    IsCover(phyAddr, phyAddr + u32Size, OPENRISC_IP_3_ADDR, OPENRISC_IP_3_ADDR + OPENRISC_IP_3_SIZE) ||
    IsCover(phyAddr, phyAddr + u32Size, OPENRISC_QMEM_ADDR, OPENRISC_QMEM_ADDR + OPENRISC_QMEM_SIZE))
    {
        printf("[%s][%d] invalid physical address 0x%x\n", __FUNCTION__, __LINE__, (unsigned int)phyAddr);
        return FALSE;
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, phyAddr);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data1, u32Size);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_DBG_MEM);
    while(i<4)
    {
        if(0 == _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
        {
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data1, 0);
            return TRUE;
        }
        i++;
        _delay();
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data1, 0);

    return FALSE;
}

MS_BOOL HAL_TSP_SetFwDbgWord(MS_U32 u32Word)
{
    MS_U32 i = 0;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, u32Word);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_DBG_WORD);
    while(i<4)
    {
        if(0 == _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd))
        {
            _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
            return TRUE;
        }
        i++;
        _delay();
    }
    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, 0);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, 0);
    return FALSE;
}

// Model : 0 -> File, 1 -> PVR1, 2 -> PVR2
// u8MobfIndex0: 0 -> Disable, 1~31
// u8MobfIndex1: 0 -> Disable, 1~31
void HAL_TSP_MOBF_Select(MS_U8 u8Model, MS_U8 u8MobfIndex0, MS_U8 u8MobfIndex1)
{
    REG32 *pReg = 0;
    MS_U32 u32value = 0;

    switch(u8Model)
    {
        case 0:
            _16MobfKey = (MS_U16)u8MobfIndex0;  //set mobf key with filein Start
            break;
        case 1:
            pReg = &_TspCtrl[0].MOBF_PVR1_Index;
            u32value = (_HAL_REG32_R(pReg) & ~TSP_MOBF_PVR1_INDEX0_MASK) |
                         ((MS_U32)(u8MobfIndex0&0xFF) << TSP_MOBF_PVR1_INDEX0_SHIFT);
            break;
        case 2:
            pReg = &_TspCtrl[0].MOBF_PVR2_Index;
            u32value = (_HAL_REG32_R(pReg) & ~TSP_MOBF_PVR2_INDEX0_MASK) |
                         ((MS_U32)(u8MobfIndex0&0xFF) << TSP_MOBF_PVR2_INDEX0_SHIFT);
            break;
        default:
            break;;
    }

    switch(u8Model)
    {
        case 0:
            break;
        case 1:
            u32value &= ~TSP_MOBF_PVR1_INDEX1_MASK;
            u32value |= ((MS_U32)(u8MobfIndex1&0xFF) << TSP_MOBF_PVR1_INDEX1_SHIFT);
            break;
        case 2:
            u32value &= ~TSP_MOBF_PVR2_INDEX1_MASK;
            u32value |= ((MS_U32)(u8MobfIndex1&0xFF) << TSP_MOBF_PVR2_INDEX1_SHIFT);
            break;
        default:
            return;
    }

    if(u8Model != 0)
        _HAL_REG32_W(pReg, u32value);

}
//---------------------------------------------------------

MS_BOOL HAL_TSP_PVR_Alignment_Enable(MS_U8 u8PVRId, MS_BOOL bEnable)
{
    REG32 *pReg = 0;
    MS_U32 u32flag = 0;

    switch(u8PVRId)
    {
        case 0:
            pReg = &_TspCtrl[0].HW2_Config3;
            u32flag = TSP_PVR1_ALIGN_EN;
            break;
        case 1:
            pReg = &_TspCtrl[0].PVR2_Config;
            u32flag = TSP_PVR2_PVR_ALIGN_EN;
            break;
        default:
            return FALSE;
    }

    if(bEnable)
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    }
    else
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    }
    return TRUE;
}

MS_BOOL HAL_TSP_PVR_ForceSync_Enable(MS_U8 u8PVRId, MS_BOOL bEnable)
{
    REG32 *pReg = 0;
    MS_U32 u32flag = 0;

    switch(u8PVRId)
    {
        case 0:
        case 1:
            pReg = &_TspCtrl[0].HW2_Config3;
            u32flag = TSP_REC_AT_SYNC_DIS;
            _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_REC_AT_SYNC_DIS));
            break;
        default:
            return FALSE;
    }

    if(bEnable)
    {
        _HAL_REG32_W(pReg, RESET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    }
    else
    {
        _HAL_REG32_W(pReg, SET_FLAG1(_HAL_REG32_R(pReg), u32flag));
    }
    return TRUE;
}

MS_BOOL HAL_TSP_DupPktCnt_Clear(void)
{
    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_DUP_PKT_CNT_CLR));

    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_DUP_PKT_CNT_CLR));

    return TRUE;
}

MS_BOOL HAL_TSP_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt)
{
    return FALSE;
}

void HAL_TSP_TSIF0_Enable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                     SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF0_ENABLE));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                     RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF0_ENABLE));
    }
}

void HAL_TSP_TSIF1_Enable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
    }
}

void HAL_TSP_TSIFFI_SrcSelect(MS_BOOL bFileMode)
{
    if(bFileMode == TRUE)
    {
        _HAL_REG16_W(&_TspCtrl5[0].Ts_If_Fi_Cfg,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].Ts_If_Fi_Cfg), TSP_FIIF_MUX_LIVE_PATH));
    }
    else
    {
        _HAL_REG16_W(&_TspCtrl5[0].Ts_If_Fi_Cfg,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl5[0].Ts_If_Fi_Cfg), TSP_FIIF_MUX_LIVE_PATH));
    }
}

MS_BOOL HAL_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
    if (bEnable) {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_BD_AUD_EN));
    }
    else {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_BD_AUD_EN));
    }

    return TRUE;
}

MS_BOOL HAL_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1, MS_U32* pData)
{
    switch (u32Cmd)
    {
        case HAL_CMD_ONEWAY:
            _HAL_REG32_W(&_TspCtrl[0].REG_ONEWAY,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].REG_ONEWAY), u32Config0));
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

MS_U8 HAL_TSP_Get_PesScmb_Sts(MS_U8 u8FltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, (TSP_MCU_CMD_SCMSTS_GET | (MS_U32)(u8FltId & 0xFF)));
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return (MS_U8)(_HAL_REG32_R(&_TspCtrl[0].MCU_Data0));
}

MS_U8 HAL_TSP_Get_TsScmb_Sts(MS_U8 u8FltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, (TSP_MCU_CMD_SCMSTS_GET | (MS_U32)(u8FltId & 0xFF)));
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return (MS_U8)(_HAL_REG32_R(&_TspCtrl[0].MCU_Data0) >> 8);
}

//-------------------------------------------------------------------------------
// @u16Mode : TSP_DEBUG_MODE_DIS_CONT     => discontinuous packet count
//                   TSP_DEBUG_MODE_DROP_COUNT => drop packet count
// @u16Src  : TBD
// @u16Fifo  : TBD
//-------------------------------------------------------------------------------
static MS_U32 _HAL_TSP_Get_PktCnt(TSP_DEBUG_MODE u16Mode, TSP_DEBUG_SRC TspSrc, TSP_DEBUG_FIFO TspFifo)
{
    MS_U16 u16Cfg  = 0;
    MS_U16 u16DropPktmode  = 0;
    REG16 *pReg = 0;
    MS_U32 u32PktCnt = 0;


    switch(TspFifo)
    {
        case TSP_DEBUG_FIFO_VIDEO:
            pReg = &_TspCtrl4[0].PktCnt_video;
            u16Cfg = ((u16Mode==TSP_DEBUG_MODE_DIS_CONT)?V_DIS_CNTR_PKT_CNT_LOAD:V_DROP_PKT_CNT_LOAD);
            break;
        case TSP_DEBUG_FIFO_AUDIO:
            pReg = &_TspCtrl4[0].PktCnt_aud;
            u16Cfg = ((u16Mode==TSP_DEBUG_MODE_DIS_CONT)?AUD_DIS_CNTR_PKT_CNT_LOAD:AUD_DROP_PKT_CNT_LOAD);
            break;
        case TSP_DEBUG_FIFO_VIDEO3D:
            pReg = &_TspCtrl4[0].PktCnt_v3d;
            u16Cfg = ((u16Mode==TSP_DEBUG_MODE_DIS_CONT)?V3D_DIS_CNTR_PKT_CNT_LOAD:V3D_DROP_PKT_CNT_LOAD);
            break;
        case TSP_DEBUG_FIFO_AUDIOB:
            pReg = &_TspCtrl4[0].PktCnt_audB;
            u16Cfg = ((u16Mode==TSP_DEBUG_MODE_DIS_CONT)?AUDB_DIS_CNTR_PKT_CNT_LOAD:AUDB_DROP_PKT_CNT_LOAD);
            break;
        default:
            return (MS_U32)(_HAL_REG16_R(pReg)); // direct return
    }

    if(u16Mode == TSP_DEBUG_MODE_DIS_CONT)
        u16DropPktmode = 0;
    else
        u16DropPktmode = 1;

    _HAL_REG16_W(&(_TspCtrl4[0].DebugSrcSel),
            (_HAL_REG16_R(&(_TspCtrl4[0].DebugSrcSel)) & (~DROP_PKT_MODE_MASK) ) |  u16DropPktmode << 1 );

    _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad1),
            (_HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad1)) & (~u16Cfg) ));

    u32PktCnt = (MS_U32)(_HAL_REG16_R(pReg));

    _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad1),
            (_HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad1)) | u16Cfg ));

    return u32PktCnt;
}

//-------------------------------------------------------------------------------
// @ u16Src : TBD
// @ u16Fifo : TBD
// @ return value : 0 ~ 15
//-------------------------------------------------------------------------------
MS_U32 HAL_TSP_Get_DisContiCnt(TSP_DisconPktCnt_Info* TspDisconPktCntInfo)
{
    if(TspDisconPktCntInfo->TspCmd == TSP_DEBUG_CMD_CLEAR)
    {
        _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr1),
            _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr1)) | (V_DIS_CNTR_PKT_CNT_CLR | V3D_DIS_CNTR_PKT_CNT_CLR | AUD_DIS_CNTR_PKT_CNT_CLR | AUDB_DIS_CNTR_PKT_CNT_CLR));
        _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr1),
            _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr1)) & ~(V_DIS_CNTR_PKT_CNT_CLR | V3D_DIS_CNTR_PKT_CNT_CLR | AUD_DIS_CNTR_PKT_CNT_CLR | AUDB_DIS_CNTR_PKT_CNT_CLR));
    }
    return _HAL_TSP_Get_PktCnt(TSP_DEBUG_MODE_DIS_CONT, TspDisconPktCntInfo->TspSrc, TspDisconPktCntInfo->TspFifo);
}

//-------------------------------------------------------------------------------
// @ u16Src : TBD
// @ u16Fifo : TBD
// @ return value : 0 ~ 15
//-------------------------------------------------------------------------------
MS_U32 HAL_TSP_Get_DropPktCnt(TSP_DropPktCnt_Info* TspDropCntInfo)
{
    if(TspDropCntInfo->TspCmd == TSP_DEBUG_CMD_CLEAR)
    {
        _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr1),
            _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr1)) | (V_DROP_PKT_CNT_CLR | V3D_DROP_PKT_CNT_CLR | AUD_DROP_PKT_CNT_CLR | AUDB_DROP_PKT_CNT_CLR));
        _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr1),
            _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr1)) & ~(V_DROP_PKT_CNT_CLR | V3D_DROP_PKT_CNT_CLR | AUD_DROP_PKT_CNT_CLR | AUDB_DROP_PKT_CNT_CLR));
    }
    return _HAL_TSP_Get_PktCnt(TSP_DEBUG_MODE_DROP_CONT, TspDropCntInfo->TspSrc, TspDropCntInfo->TspFifo);
}

//-------------------------------------------------------------------------------
// @u16Tsif : TBD
// @ return value : 0 ~ 15
//-------------------------------------------------------------------------------
MS_U32 HAL_TSP_Get_LockPktCnt(TSP_LockPktCnt_info* TspLockCntInfo)
{
    MS_U16 u16Clr=0,u16Load=0,u16Src=0;
    MS_U32 u32LockPktCnt = 0;

    switch (TspLockCntInfo->TspTsif)
    {
        case TSP_DEBUG_TSIF0: // TS0
            u16Clr = LOCK_PKT_CNT_0_CLR;
            u16Load = LOCK_PKT_CNT_0_LOAD;
            u16Src = TSIF_SRC_SEL_TSIF0;
            break;
        case TSP_DEBUG_TSIF1: // TS1
            u16Clr = LOCK_PKT_CNT_1_CLR;
            u16Load = LOCK_PKT_CNT_1_LOAD;
            u16Src = TSIF_SRC_SEL_TSIF1;
            break;
        case TSP_DEBUG_TSIFFI: // TSFI
            u16Clr = LOCK_PKT_CNT_FI_CLR;
            u16Load = LOCK_PKT_CNT_FI_LOAD;
            u16Src = TSIF_SRC_SEL_TSIF_FI;
            break;
        default:
            break;
    }

    if(TspLockCntInfo->TspCmd == TSP_DEBUG_CMD_CLEAR)
    {
        _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
            _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) | u16Clr);
        _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
            _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) & (~u16Clr));
    }

    _HAL_REG16_W(&(_TspCtrl4[0].DebugSrcSel),
            (_HAL_REG16_R(&(_TspCtrl4[0].DebugSrcSel)) & (~TSIF_SRC_SEL_MASK)) | (u16Src << TSIF_SRC_SEL_SHIFT));

    _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
        _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) & (~u16Load));

    u32LockPktCnt = (MS_U32)(_HAL_REG16_R(&(_TspCtrl4[0].LockedPktCnt)));

    _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
        _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) | u16Load);

    return u32LockPktCnt;
}

//-------------------------------------------------------------------------------
// @ u16Fifo : TBD
// @ return value : 0 ~ 15
//-------------------------------------------------------------------------------
MS_U32 HAL_TSP_GetAVPktCnt(TSP_AVPktCnt_info* TspAVCntInfo)
{
    MS_U32 u32PktCnt = 0;

    if(TspAVCntInfo->TspCmd == TSP_DEBUG_CMD_CLEAR)
    {
        switch (TspAVCntInfo->TspFifo)
        {
            case TSP_DEBUG_FIFO_VIDEO: // VIDEO
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) | (V_PKT_CNT_CLR));
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) & (~V_PKT_CNT_CLR));

                break;
            case TSP_DEBUG_FIFO_AUDIO:
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) | (AUD_PKT_CNT_CLR));
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) & (~AUD_PKT_CNT_CLR));
                break;
            case TSP_DEBUG_FIFO_VIDEO3D:
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) | (V3D_PKT_CNT_CLR));
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) & (~V3D_PKT_CNT_CLR));
                break;
            case TSP_DEBUG_FIFO_AUDIOB:
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) | (AUDB_PKT_CNT_CLR));
                _HAL_REG16_W(&(_TspCtrl4[0].PktCntClr),
                    _HAL_REG16_R(&(_TspCtrl4[0].PktCntClr)) & (~AUDB_PKT_CNT_CLR));
                break;
            default:
                break;
        }

    }


    switch (TspAVCntInfo->TspFifo)
    {
        case TSP_DEBUG_FIFO_VIDEO: // VIDEO
            _HAL_REG16_W(&(_TspCtrl4[0].DebugSrcSel),
                (_HAL_REG16_R(&(_TspCtrl4[0].DebugSrcSel)) & (~AV_PKT_SRC_SEL_MASK)) | (AV_PKT_SRC_VID << AV_PKT_SRC_SEL_SHIFT));
            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                (_HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad))) & (~V_PKT_CNT_LOAD));

            u32PktCnt = (MS_U32)(_HAL_REG16_R(&(_TspCtrl4[0].AVPktCnt)));

            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                (_HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad))) | V_PKT_CNT_LOAD);

        case TSP_DEBUG_FIFO_AUDIO: // AUDIO
            _HAL_REG16_W(&(_TspCtrl4[0].DebugSrcSel),
                (_HAL_REG16_R(&(_TspCtrl4[0].DebugSrcSel)) & (~AV_PKT_SRC_SEL_MASK)) | (AV_PKT_SRC_AUD << AV_PKT_SRC_SEL_SHIFT));
            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) & (~AUD_PKT_CNT_LOAD));

            u32PktCnt = (MS_U32)(_HAL_REG16_R(&(_TspCtrl4[0].AVPktCnt)));

            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) | AUD_PKT_CNT_LOAD);
        case TSP_DEBUG_FIFO_VIDEO3D: // V3D
            _HAL_REG16_W(&(_TspCtrl4[0].DebugSrcSel),
                (_HAL_REG16_R(&(_TspCtrl4[0].DebugSrcSel)) & (~AV_PKT_SRC_SEL_MASK)) | (AV_PKT_SRC_V3D << AV_PKT_SRC_SEL_SHIFT));
            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) & (~V3D_PKT_CNT_LOAD));

            u32PktCnt = (MS_U32)(_HAL_REG16_R(&(_TspCtrl4[0].AVPktCnt1)));

            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) | V3D_PKT_CNT_LOAD);

        case TSP_DEBUG_FIFO_AUDIOB: // AUDIOB
            _HAL_REG16_W(&(_TspCtrl4[0].DebugSrcSel),
                (_HAL_REG16_R(&(_TspCtrl4[0].DebugSrcSel)) & (~AV_PKT_SRC_SEL_MASK)) | (AV_PKT_SRC_AUDB << AV_PKT_SRC_SEL_SHIFT));
            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) & (~AUDB_PKT_CNT_LOAD));

            u32PktCnt = (MS_U32)(_HAL_REG16_R(&(_TspCtrl4[0].AVPktCnt1)));

            _HAL_REG16_W(&(_TspCtrl4[0].PktCntLoad),
                _HAL_REG16_R(&(_TspCtrl4[0].PktCntLoad)) | AUDB_PKT_CNT_LOAD);

        default:
            return 0;
    }
    return u32PktCnt;
}

MS_U32 HAL_TSP_Get_SecTEI_PktCount(MS_U32 u32PktSrc)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_TEI_COUNT_GET | u32PktSrc);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return (_HAL_REG32_R(&_TspCtrl[0].MCU_Data0));
}

MS_BOOL HAL_TSP_Reset_SecTEI_PktCount(MS_U32 u32PktSrc)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_TEI_COUNT_GET | TSP_MCU_CMD_TEI_COUNT_OPTION_RESET | u32PktSrc);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return TRUE;
}

MS_U32 HAL_TSP_Get_SecDisCont_PktCount(MS_U32 u32FltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_DISCONT_COUNT_GET | u32FltId);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return (_HAL_REG32_R(&_TspCtrl[0].MCU_Data0));
}

MS_BOOL HAL_TSP_Reset_SecDisCont_PktCount(MS_U32 u32FltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_DISCONT_COUNT_GET | TSP_MCU_CMD_DISCONT_COUNT_OPTION_RESET | u32FltId);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return TRUE;
}

MS_BOOL HAL_TSP_DropScmbPkt(MS_U32 u32StreamId,MS_BOOL bEnable)
{
    MS_U16 u32Flag;

    switch(u32StreamId)
    {
        default:
        case 0:
            u32Flag = MASK_SCR_VID_EN;
            break;
        case 1:
            u32Flag = MASK_SCR_AUD_EN;
            break;
        case 2:
            u32Flag = MASK_SCR_AUD_B_EN;
            break;
        case 3:
            u32Flag = MASK_SCR_VID_3D_EN;
            break;
        case 6:
            u32Flag = MASK_SCR_PVR1_EN;
            break;
        case 7:
            u32Flag = MASK_SCR_PVR2_EN;
            break;
    }

    if (bEnable)
    {
        _HAL_REG16_W(&_TspCtrl3[0].HW3_Cfg1,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl3[0].HW3_Cfg1), u32Flag));
    }
    else
    {
        _HAL_REG16_W(&_TspCtrl3[0].HW3_Cfg1,
                RESET_FLAG1(_HAL_REG16_R(&_TspCtrl3[0].HW3_Cfg1), u32Flag));
    }
    return TRUE;
}


// -------------------------------------------------------------
// Merge Stream
// -------------------------------------------------------------
MS_BOOL HAL_TSP_Set_Sync_Byte(MS_U8 u8Path, MS_U8 u8Id, MS_U8 *pu8SyncByte, MS_BOOL bSet)
{
    REG16 *SynReg=0;
    MS_U16 u16Mask = 0x00FF, u16Sync = 0, u16Shift = 0;

    switch(u8Path)
    {
        case TSP_SRC_FROM_TSIF0_LIVE:
            SynReg = &(_TspCtrl3[0].SyncByte_tsif0[u8Id>>1]);
            break;
        case TSP_SRC_FROM_TSIF0_FILE:
            SynReg = &(_TspCtrl3[0].SyncByte_file[u8Id>>1]);
            break;
        case TSP_SRC_FROM_TSIF1:
            SynReg = &(_TspCtrl3[0].SyncByte_tsif1[u8Id>>1]);
            break;
        default:
            return FALSE;
    }

    if(u8Id & 0x1)
    {
        u16Shift = 8;
    }

    if(bSet == TRUE)
    {
        u16Sync = (MS_U16)(*pu8SyncByte & 0xFF);
        _HAL_REG16_W(SynReg,((_HAL_REG16_R(SynReg) & ~(u16Mask << u16Shift)) | (u16Sync << u16Shift)));
    }
    else
    {
        u16Sync = (_HAL_REG16_R(SynReg) & (u16Mask << u16Shift)) >> u16Shift;
        *pu8SyncByte = (MS_U8)u16Sync;
    }

    return TRUE;

}

MS_BOOL HAL_TSP_Set_Src_Id(MS_U8 u8Path, MS_U8 u8Id, MS_U8 *pu8SrcId, MS_BOOL bSet)
{
    REG16 *SrcIdReg =0;
    MS_U16 u16SrcId = 0, u16Mask = 0x000F, u16Shift = 0;

    switch(u8Path)
    {
        case TSP_SRC_FROM_TSIF0_LIVE:
            SrcIdReg = &(_TspCtrl3[0].SourceId_tsif0[u8Id>>2]);
            break;
        case TSP_SRC_FROM_TSIF0_FILE:
            SrcIdReg = &(_TspCtrl3[0].SourceId_file[u8Id>>2]);
            break;
        case TSP_SRC_FROM_TSIF1:
            SrcIdReg = &(_TspCtrl3[0].SourceId_tsif1[u8Id>>2]);
            break;
        default:
            return FALSE;
    }

    switch(u8Id & 0x3)
    {
        case 0x1:
            u16Shift = 4;
            u16SrcId <<= 4;
            u16Mask <<= 4;
            break;
        case 0x2:
            u16Shift = 8;
            u16SrcId <<= 8;
            u16Mask <<= 8;
            break;
        case 0x3:
            u16Shift = 12;
            u16SrcId <<= 12;
            u16Mask <<= 12;
            break;
    }

    if(bSet == TRUE)
    {
        u16SrcId = (MS_U16)(*pu8SrcId & 0xFF);
        _HAL_REG16_W(SrcIdReg,((_HAL_REG16_R(SrcIdReg) & ~(u16Mask << u16Shift)) | (u16SrcId << u16Shift)));
    }
    else
    {
        u16SrcId = (_HAL_REG16_R(SrcIdReg) & (u16Mask << u16Shift)) >> u16Shift;
        *pu8SrcId = (MS_U8)u16SrcId;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_Set_ATS_AdjPeriod(MS_U16 u16Value)
{
    _HAL_REG16_W(&(_TspCtrl5[0].ATS_Adj_Period),
                (_HAL_REG16_R(&(_TspCtrl5[0].ATS_Adj_Period)) & (~TSP_ATS_ADJ_PERIOD_MASK)) | u16Value);

    return TRUE;
}

MS_BOOL HAL_TSP_Set_ATS_AdjEnable(MS_BOOL bEnable)
{
    if(bEnable == TRUE)
    {
        _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                _HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) | TSP_ATS_MODE_FI_ENABLE);
    }
    else
    {
        _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                (_HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) & (~TSP_ATS_MODE_FI_ENABLE)));
    }
    return TRUE;
}

MS_BOOL HAL_TSP_Set_ATS_AdjOffset(MS_BOOL bIncreased, MS_U16 u16Offset)
{
    if(bIncreased == TRUE)
    {
        _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                (_HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) & (~TSP_ATS_OFFSET_FI_NEGATIVE)));
    }
    else
    {
        _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                _HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) | TSP_ATS_OFFSET_FI_NEGATIVE);
    }

    _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                (_HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) & (~TSP_ATS_OFFSET_FI_MASK)) | ((u16Offset << TSP_ATS_OFFSET_FI_SHIFT) & TSP_ATS_OFFSET_FI_MASK));

    _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                _HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) | TSP_ATS_OFFSET_FI_ENABLE);
    _HAL_REG16_W(&(_TspCtrl5[0].AtsCfg),
                (_HAL_REG16_R(&(_TspCtrl5[0].AtsCfg)) & (~TSP_ATS_OFFSET_FI_ENABLE)));

    return FALSE;
}


