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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
#include "halCHIP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define TSP_HAL_REG_SAFE_MODE       1UL                                   // Register protection access between 1 task and 1+ ISR

//[HWBUG]
#define MULTI_ACCESS_SW_PATCH       1UL                                   // It's still risk becuase some registers like readaddr will

// @FIXME: remove the test later
#define LINUX_TEST                  0UL
                                                                        // cause overflow before patching to correct value.
#define MIU_BUS                     4UL

#define VQ_PACKET_UNIT_LEN          192UL  //192 bytes

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static REG_Ctrl*   _TspCtrl  = NULL;  // TSP0 & TSP1
static REG_Ctrl2*  _TspCtrl2 = NULL;  // TSP2
static REG_Ctrl3*  _TspCtrl3 = NULL;  // TSP3

static MS_VIRT _virtRegBase = 0;

extern MS_BOOL _bIsHK;
static MS_S32 _s32HALTSPMutexId = -1;
static MS_BOOL _b64STCMode = FALSE;

static MS_PHY       _phyOrLoadMiuOffset = 0;
static MS_PHY       _phySecBufMiuOffset = 0;
static MS_PHY       _phyFIBufMiuOffset = 0;
static MS_PHY       _phyPVRBufMiuOffset[TSP_PVR_IF_NUM] = {[0 ... (TSP_PVR_IF_NUM-1)] = 0UL};

static MS_BOOL _bTsPadUsed[TSP_TS_PAD_NUM] = {FALSE};

//[NOTE] Jerry
// Some register has write order, for example, writing PCR_L will disable PCR counter
// writing PCR_M trigger nothing, writing PCR_H will enable PCR counter
#define _HAL_REG32_W(reg, value)    do { (reg)->L = ((value) & 0x0000FFFFUL);                          \
                                    (reg)->H = ((value) >> 16UL); } while(0)

#define _HAL_REG32L_W(reg, value)   (reg)->data = ((value) & 0x0000FFFFUL);

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

#define _HAL_HALTSP_LOCK()          (MsOS_ObtainMutex(_s32HALTSPMutexId, MSOS_WAIT_FOREVER))
#define _HAL_HALTSP_UNLOCK()        (MsOS_ReleaseMutex(_s32HALTSPMutexId))

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

#define TSP_INT_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x3200UL + ((addr)<<2UL))))

#define TSP_CLKGEN0_REG(addr)       (*((volatile MS_U16*)(_virtRegBase + 0x1600UL + ((addr)<<2UL))))
    #define REG_CLKGEN0_DC0_SYTNTH              0x04UL
        #define REG_CLKGEN0_STC_CW_SEL          0x0002UL
        #define REG_CLKGEN0_STC_CW_EN           0x0004UL
        #define REG_CLKGEN0_STC_REF_DIV2_EN     0x0008UL
    #define REG_CLKGEN0_DC0_STC_CW_L            0x05UL
    #define REG_CLKGEN0_DC0_STC_CW_H            0x06UL

    #define REG_CLKGEN0_TSN_CLK                 0x28UL
        #define REG_CLKGEN0_TSN_CLK_MASK        0x1FUL
        #define REG_CLKGEN0_TSN_CLK_DEMOD       0x1CUL
        #define REG_CLKGEN0_TSN_CLK_TS0_SHIFT   0UL
        #define REG_CLKGEN0_TSN_CLK_TS1_SHIFT   8UL
        //#define TSP_CLK_TS0                        0
        //#define TSP_CLK_INDEMOD                 7
        // bit[4:0] -> ts0  -> 0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 0: TS0
        //                                      7: Internal Demmod
        // bit[12:8] -> ts1 -> 0: disable clock
        //                     1: invert clock
        //                     bit [4:2] -> 0: TS0
        //                                      7: Internal Demmod
#define TSP_TOP_REG(addr)           (*((volatile MS_U16*)(_virtRegBase + 0x3c00UL + ((addr)<<2UL))))
    #define REG_TOP_TS0_PE                  0x0EUL
    #define REG_TOP_TS1_PE                  0x06UL
    #define REG_TOP_TS2_PE                  0x36UL
    #define REG_TOP_TS_PE_MASK                      0x07FFUL

    #define REG_TOP_TSN_MUX                 0x11UL
        #define REG_TOP_TSN_MUX_MASK                    0x0FUL
        #define REG_TOP_TSN_MUX_TS0_SHIFT               0UL
        #define REG_TOP_TSN_MUX_TS1_SHIFT               8UL
        // bit[3:0] -> ts0  ->   0: TS0
        //                             7: Demod
        // bit[10:8] -> ts1  -> 0: TS0
        //                             7: Demod
    #define REG_TOP_TSCONFIG                    0x57UL
        #define REG_TOP_TS_TS1_CFG_MASK                     0x07UL
        #define REG_TOP_TS1CFG_SHIFT                        11UL
        #define REG_TOP_TSCFG_DISABLE_PAD                   0UL
        #define REG_TOP_TS1_PARALLEL_IN                     1UL
        #define REG_TOP_TS1_SERIAL_IN                       3UL

        #define REG_TOP_TS_TS2_CFG_MASK                     0x03UL
        #define REG_TOP_TS2CFG_SHIFT                        14UL
        #define REG_TOP_TSCFG_DISABLE_PAD                   0UL
        #define REG_TOP_TS2_SERIAL_IN                       1UL


        // bit[10:8] ->    0: Disable
        //                     1: use all PAD_TS0 pads
        //                     2: e PAD_TS0_VLD, PAD_TS0_SYNC, PAD_TS0_CLK and PAD_TS0_D0 pads
        //                     3: mspi mode
        //                     4: 3 wire mode

#define ABS_DIFF(x1, x2)            (((x1) > (x2))? ((x1) - (x2)) : ((x2) - (x1)))
#define IsCover(_start1, _end1, _start2, _end2) (ABS_DIFF(_end1, _end2) < (((_end1) > (_end2))?((_end1)-(_start1)):((_end2)-(_start2))) )?TRUE:FALSE
//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------
static void _HAL_TSP_FW_load(
    MS_PHY                     phyFwAddrPhys,
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
    for (i = 0; i< 0xFFFFUL; i++);
}

static MS_U32 _HAL_REG32_R(REG32 *reg)
{
    MS_U32     value = 0UL;
    value  = (reg)->H << 16UL;
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
    MS_U16              value = 0;
    value = (reg)->u16data;
    return value;
}

static MS_PHY _HAL_TSP_MIU_OFFSET(MS_PHY Phyaddr)
{
    #ifdef HAL_MIU2_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU2_BASE)
        return (MS_PHY)HAL_MIU2_BASE;
    else
    #endif  //HAL_MIU2_BUS_BASE
    #ifdef HAL_MIU1_BASE
    if(Phyaddr >= (MS_PHY)HAL_MIU1_BASE)
        return (MS_PHY)HAL_MIU1_BASE;
    else
    #endif //HAL_MIU1_BUS_BASE
        return (MS_PHY)HAL_MIU0_BASE;
}

#define REG16_T(addr)           (*((volatile MS_U16*)(addr)))
#define ADDR_INDR_CTRL          (_virtRegBase+ 0x2b20UL)
#define ADDR_INDR_ADDR0         (_virtRegBase+ 0x2b24UL)
#define ADDR_INDR_ADDR1         (_virtRegBase+ 0x2b28UL)
#define ADDR_INDR_WRITE0        (_virtRegBase+ 0x2b2cUL)
#define ADDR_INDR_WRITE1        (_virtRegBase+ 0x2b30UL)
#define ADDR_INDR_READ0         (_virtRegBase+ 0x2b34UL)
#define ADDR_INDR_READ1         (_virtRegBase+ 0x2b38UL)

#if 0
#define XBYTE_1591              (_virtRegBase+ 0x2a0cUL)  // TsRec_Head21_Mid20
#define XBYTE_15A4              (_virtRegBase+ 0x2a10UL)  // TsRec_Mid21_Tail20
#define XBYTE_15A6              (_virtRegBase+ 0x2b48UL)  // TsRec_Mid

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

#define TSP_SEM_AEON              (_virtRegBase+ 0xC1480UL)   //TSP_HW_SEMAPHORE0, TS3 0x20
#define TSP_SEM_ORDER             (_virtRegBase+ 0xC1484UL)  // TSP_HW_SEMAPHORE1, TS3 0x21
#define TSP_SEM_MIPS              (_virtRegBase+ 0xC1488UL)  // TSP_HW_SEMAPHORE2, TS3 0x22

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
    MS_VIRT virtReg = (MS_VIRT)reg;
    MS_U32 u32Ret;

    _HAL_TSP_HW_Lock();

    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   (MS_U16)(virtReg>> 1UL);
    REG16_T(ADDR_INDR_ADDR1)=   (MS_U16)(virtReg>> 17UL);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);

    // get read value
    u32Ret = ((MS_U32)(REG16_T(ADDR_INDR_READ0))| ((MS_U32)(REG16_T(ADDR_INDR_READ1)<< 16UL)));

    _HAL_HALTSP_UNLOCK();
    _HAL_TSP_HW_Unlock();
    return u32Ret;
}

static MS_U32 HAL_REG32_IndR_tmp(REG32 *reg)
{
    MS_VIRT virtReg = (MS_VIRT)reg;
    MS_U32 u32Ret;

    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   (MS_U16)(virtReg>> 1UL);
    REG16_T(ADDR_INDR_ADDR1)=   (MS_U16)(virtReg>> 17UL);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_READ | TSP_IDR_START);

    // get read value
    u32Ret = ((MS_U32)(REG16_T(ADDR_INDR_READ0))| ((MS_U32)(REG16_T(ADDR_INDR_READ1)<< 16)));

    _HAL_HALTSP_UNLOCK();

    return u32Ret;
}
static void HAL_REG32_IndW_tmp(REG32 *reg, MS_U32 value)
{
    MS_VIRT virtReg = (MS_VIRT)reg;
    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   (MS_U16)(virtReg>> 1UL);
    REG16_T(ADDR_INDR_ADDR1)=   (MS_U16)(virtReg>> 17UL);

    // set write value
    REG16_T(ADDR_INDR_WRITE0)=  (MS_U16)value;
    REG16_T(ADDR_INDR_WRITE1)=  (MS_U16)(value >> 16UL);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);

    _HAL_HALTSP_UNLOCK();
}

void HAL_REG32_IndW(REG32 *reg, MS_U32 value)
{
    MS_VIRT virtReg = (MS_VIRT)reg;
    _HAL_TSP_HW_Lock();

    _HAL_HALTSP_LOCK();

    // set address
    REG16_T(ADDR_INDR_ADDR0)=   (MS_U16)(virtReg>> 1UL);
    REG16_T(ADDR_INDR_ADDR1)=   (MS_U16)(virtReg>> 17UL);

    // set write value
    REG16_T(ADDR_INDR_WRITE0)=  (MS_U16)value;
    REG16_T(ADDR_INDR_WRITE1)=  (MS_U16)(value >> 16UL);

    // set command
    // REG16_T(ADDR_INDR_CTRL)=    (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) & 0xFF00;
    REG16_T(ADDR_INDR_CTRL)=    REG16_T(ADDR_INDR_CTRL) | (TSP_IDR_MCUWAIT | TSP_IDR_WRITE | TSP_IDR_START);

    _HAL_HALTSP_UNLOCK();

    _HAL_TSP_HW_Unlock();
}

static MS_U16 u16LastAddr0, u16LastAddr1, u16LastWrite0, u16LastWrite1, u16LastRead0, u16LastRead1;

static MS_U32                       _u32PidFltBuf[(TSP_PIDFLT_NUM_ALL*sizeof(REG_PidFlt))>> 3];
static MS_U32                       _u32SecFltBuf[TSP_SECFLT_NUM*((sizeof(REG_SecFlt)-sizeof((((REG_SecFlt*)0)->_x50)))>> 3)];
//[LEGACY] //[OBSOLETE]
MS_BOOL                             _bIsHK = TRUE;
//[LEGACY] //[OBSOLETE]
void HAL_TSP_SaveFltState(void)
{
    MS_U32 u32EngId;
    MS_U32 i, j;
    MS_U32 u32SecEnd = ((size_t)&(((REG_SecFlt*)0)->_x50))/sizeof(REG32);

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
       for (i = 0; i < TSP_PIDFLT_NUM_ALL; i++)
       {
            REG_PidFlt *pPidFilter = &(_TspPid[u32EngId].Flt[i]);
            _u32PidFltBuf[i] = HAL_REG32_IndR(pPidFilter);
       }
       j = 0;
       for (i = 0; i < TSP_SECFLT_NUM; i++)
       {
            REG32* pRegStart = (REG32*) &(_TspSec[u32EngId].Flt[i]);
            // REG32* pRegEnd = (REG32*)&(_TspSec[u32EngId].Flt[i + 1]);
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

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
       for (i = 0; i < TSP_PIDFLT_NUM_ALL; i++)
       {
            REG_PidFlt *pPidFilter = &(_TspPid[u32EngId].Flt[i]);
            HAL_REG32_IndW(pPidFilter, _u32PidFltBuf[i]);
       }
       j = 0;
       for (i = 0; i < TSP_SECFLT_NUM; i++)
       {
            REG32* pRegStart = (REG32*) &(_TspSec[u32EngId].Flt[i]);
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
void HAL_TSP_Scmb_Detect(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_match_pid_scr_ts_ld| TSP_match_pid_scr_fi_ld));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_match_pid_scr_ts_ld| TSP_match_pid_scr_fi_ld));
    }
}

void HAL_TSP_CSA_Set_ScrmPath(MS_U32 u32ScrmPath)
{
    _HAL_REG32L_W(&_TspCtrl[0].CA_CTRL, u32ScrmPath);
}

void HAL_TSP_Set_AVPAUSE(MS_BOOL bSet)
{
    if (bSet)
    {
        _HAL_REG32L_W(&_TspCtrl[0].CA_CTRL,
            SET_FLAG1(_HAL_REG32L_R(&_TspCtrl[0].CA_CTRL), TSP_CA_AVPAUSE));
    }
    else
    {
        _HAL_REG32L_W(&_TspCtrl[0].CA_CTRL,
            RESET_FLAG1(_HAL_REG32L_R(&_TspCtrl[0].CA_CTRL), TSP_CA_AVPAUSE));
    }
}

MS_U32 HAL_TSP_CSA_Get_ScrmPath(void)
{
    return (MS_U32)(_HAL_REG32L_R(&_TspCtrl[0].CA_CTRL) & TSP_CA_CTRL_MASK);
}

//--------------------------------------------------------------------------------------------------
// For PID filter part
//--------------------------------------------------------------------------------------------------
MS_U32 HAL_TSP_PidFlt_GetFltOutput(REG_PidFlt *pPidFilter)
{
    return (HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_OUT_MASK);
}

MS_U32 HAL_TSP_PidFlt_GetPid(REG_PidFlt* pPidFilter)
{
    return (HAS_FLAG(HAL_REG32_IndR((REG32 *)pPidFilter), TSP_PIDFLT_PID_MASK)>> TSP_PIDFLT_PID_SHFT);
}

void HAL_TSP_PidFlt_SetPid(REG_PidFlt *pPidFilter, MS_U32 u32PID)
{
    HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_PID_MASK) | ((u32PID << TSP_PIDFLT_PID_SHFT) & TSP_PIDFLT_PID_MASK));
}

void HAL_TSP_PidFlt_SelFltOutput(REG_PidFlt *pPidFilter, MS_U32 u32FltOutput)
{
    HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_OUT_MASK) | (u32FltOutput & TSP_PIDFLT_OUT_MASK));
}

void HAL_TSP_PidFlt_SelSecFlt(REG_PidFlt *pPidFilter, MS_U32 u32SecFltId)
{
    HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_SECFLT_MASK) | ((u32SecFltId << TSP_PIDFLT_SECFLT_SHFT) & TSP_PIDFLT_SECFLT_MASK));
}

void HAL_TSP_PidFlt_PVREnable(REG_PidFlt *pPidFilter)
{
    HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) | TSP_PIDFLT_PVR_ENABLE));
}


void HAL_TSP_SecFlt_SetType(REG_SecFlt *pSecFilter, MS_U32 u32FltType)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, (HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & ~TSP_SECFLT_TYPE_MASK) | (u32FltType << TSP_SECFLT_TYPE_SHFT));
}

MS_U32 HAL_TSP_PidFlt_GetSecFlt(REG_PidFlt *pPidFilter)
{
    return ((HAL_REG32_IndR((REG32 *)pPidFilter) & TSP_PIDFLT_SECFLT_MASK) >> TSP_PIDFLT_SECFLT_SHFT);
}

void HAL_TSP_PidFlt_SelFltSource(REG_PidFlt *pPidFilter, MS_U32 u32FltSource)
{
    HAL_REG32_IndW((REG32 *)pPidFilter, (HAL_REG32_IndR((REG32 *)pPidFilter) & ~TSP_PIDFLT_IN_MASK) | (u32FltSource & TSP_PIDFLT_IN_MASK));
}

//--------------------------------------------------------------------------------------------------
// For section filter part
//--------------------------------------------------------------------------------------------------
MS_U32 HAL_TSP_SecFlt_GetSecBuf(REG_SecFlt *pSecFilter)
{
    return (pSecFilter - (REG_SecFlt*)REG_SECFLT_BASE)>> 8; // div 0x100
}

void HAL_TSP_SecFlt_ResetState(REG_SecFlt* pSecFilter)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & ~(TSP_SECFLT_STATE_MASK));
}

void HAL_TSP_SecFlt_SetRmnCount(REG_SecFlt *pSecFilter, MS_U32 u32RmnCount)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->RmnReqCnt, (HAL_REG32_IndR((REG32 *)&pSecFilter->RmnReqCnt) & ~TSP_SECFLT_RMNCNT_MASK) |
                                    ((u32RmnCount << TSP_SECFLT_RMNCNT_SHFT) & TSP_SECFLT_RMNCNT_MASK));
}

void HAL_TSP_SecFlt_ClrCtrl(REG_SecFlt *pSecFilter)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, 0);
}


MS_U32 HAL_TSP_SW_INT_STATUS(void)
{
    if (_bIsHK)
    {
        return _HAL_REG32_R(&_TspCtrl[0].SwInt_Stat);
    }
    else
    {
        MS_U32 u32SwIntStatus = (MS_U32)_HAL_REG16_R(&_TspCtrl[0].SwInt_Stat1_M);
        u32SwIntStatus |= ((_HAL_REG32_R(&_TspCtrl[0].SwInt_Stat1_H) & TSP_SWINT1_H_MASK) >> TSP_SWINT1_H_SHFT)<< 16;
        return u32SwIntStatus;
    }
}

// match mask --> 0 will compare
void HAL_TSP_SecFlt_SetMask(REG_SecFlt *pSecFilter, MS_U8 *pu8Mask)
{
    MS_U32 i;
    MS_U32 u32Temp;
    MS_U32 j;

    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp =  (pu8Mask[j]) | (pu8Mask[j+ 1] << 8  ) | (pu8Mask[j+ 2] << 16  )| (pu8Mask[j+ 3] << 24);
        HAL_REG32_IndW((REG32 *)&pSecFilter->Mask[i], u32Temp);
    }
}

// not match mask --> 1 will compare
void HAL_TSP_SecFlt_SetNMask(REG_SecFlt *pSecFilter, MS_U8 *pu8NMask)
{
    MS_U32 u32FltId;
    MS_U32 u32Temp;
    REG_SecFlt* ptempSecFlt = &(_TspSec[0].Flt[31]);

    u32Temp =  (pu8NMask[0x0]) | (pu8NMask[0x1] << 8  ) | (pu8NMask[0x2] << 16  )| (pu8NMask[0x3] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[0]), u32Temp);

    u32Temp =  (pu8NMask[0x4]) | (pu8NMask[0x5] << 8  ) | (pu8NMask[0x6] << 16  )| (pu8NMask[0x7] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[1]), u32Temp);

    u32Temp =  (pu8NMask[0x8]) | (pu8NMask[0x9] << 8  ) | (pu8NMask[0xa] << 16  )| (pu8NMask[0xb] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[2]), u32Temp);

    u32Temp =  (pu8NMask[0xc]) | (pu8NMask[0xd] << 8  ) | (pu8NMask[0xe] << 16  )| (pu8NMask[0xf] << 24);
    HAL_REG32_IndW(&(ptempSecFlt->Match[3]), u32Temp);

    u32FltId = (MS_U32)(pSecFilter - (REG_SecFlt*)REG_SECFLT_BASE);

    //printf("[%s][%d] u32FltId %d\n", __FUNCTION__, __LINE__, (unsigned int)u32FltId);

    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_NMATCH | u32FltId);

    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
}

void HAL_TSP_SecFlt_SetMatch(REG_SecFlt *pSecFilter, MS_U8 *pu8Match)
{
    MS_U32 i;
    MS_U32 u32Temp;
    MS_U32 j;

    for (i = 0; i < (TSP_FILTER_DEPTH/sizeof(MS_U32)); i++)
    {
        j = (i<< 2);
        u32Temp =  (pu8Match[j]) | (pu8Match[j+ 1] << 8  ) | (pu8Match[j+ 2] << 16  )| (pu8Match[j+ 3] << 24);
        HAL_REG32_IndW((REG32 *)&pSecFilter->Match[i], u32Temp);
    }
}

void HAL_TSP_SecFlt_SetReqCount(REG_SecFlt *pSecFilter, MS_U32 u32ReqCount)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->RmnReqCnt, (HAL_REG32_IndR((REG32 *)&pSecFilter->RmnReqCnt) & ~TSP_SECFLT_REQCNT_MASK) |
                                    ((u32ReqCount << TSP_SECFLT_REQCNT_SHFT) & TSP_SECFLT_REQCNT_MASK));
}

void HAL_TSP_SecFlt_SetMode(REG_SecFlt *pSecFilter, MS_U32 u32SecFltMode)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, (HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & ~TSP_SECFLT_MODE_MASK) | ((u32SecFltMode << TSP_SECFLT_MODE_SHFT) & TSP_SECFLT_MODE_MASK));
}

MS_U32 HAL_TSP_SecFlt_GetCRC32(REG_SecFlt *pSecFilter)
{
    return HAL_REG32_IndR((REG32 *)&pSecFilter->CRC32);
}

MS_U32 HAL_TSP_SecFlt_GetState(REG_SecFlt *pSecFilter)
{
    return ((HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & TSP_SECFLT_STATE_MASK) >> TSP_SECFLT_STATE_SHFT);
}

MS_U32 HAL_TSP_SecFlt_GetMode(REG_SecFlt *pSecFilter)
{
    return ((HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) & TSP_SECFLT_MODE_MASK) >> TSP_SECFLT_MODE_SHFT);
}

void HAL_TSP_SecFlt_PcrReset(REG_SecFlt *pSecFilter)
{
    HAL_REG32_IndW((REG32 *)&pSecFilter->Ctrl, HAL_REG32_IndR((REG32 *)&pSecFilter->Ctrl) | TSP_SECFLT_PCRRST);
}

void HAL_TSP_SecFlt_VerReset(MS_U32 u32SecFltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_VER_RESET | u32SecFltId);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
}

void HAL_TSP_SecFlt_SetDataAddr(MS_PHY phyDataAddr)
{
    MS_PHY phyAddr = phyDataAddr - _HAL_TSP_MIU_OFFSET(phyDataAddr);
    MS_U32 u32cmd = TSP_MCU_CMD_MEM_HIGH_ADDR | ((MS_U32)(phyAddr & 0xFFFF0000UL) >> 16UL);

    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32cmd);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));

    u32cmd = TSP_MCU_CMD_MEM_LOW_ADDR | ((MS_U32)(phyAddr & 0xFFFFUL));
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32cmd);
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
}


//--------------------------------------------------------------------------------------------------
//  For section buffer part
//--------------------------------------------------------------------------------------------------
// To avoid SW read hidden HW byte enable information.
#define _TSP_SEC_BUF_ADDR_START(pSecFilter)     (TSP_SECFLT_BUFSTART_MASK & HAL_REG32_IndR((REG32 *)&((pSecFilter)->BufStart)))

void HAL_TSP_SecBuf_SetBuffer(REG_SecFlt *pSecBuf, MS_PHY phyStartAddr, MS_U32 u32BufSize)
{
    MS_PHY phyAddr = 0UL;

    _phySecBufMiuOffset = _HAL_TSP_MIU_OFFSET(phyStartAddr);
    phyAddr = phyStartAddr - _phySecBufMiuOffset;

    HAL_REG32_IndW((REG32 *)&pSecBuf->BufStart, (MS_U32)phyAddr);
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufEnd, ((MS_U32)phyAddr + u32BufSize));
}

void HAL_TSP_SecBuf_SetBufRead(REG_SecFlt *pSecBuf, MS_PHY phyReadAddr)
{
    _phySecBufMiuOffset = _HAL_TSP_MIU_OFFSET(phyReadAddr);

    HAL_REG32_IndW((REG32 *)&pSecBuf->BufRead, (MS_U32)(phyReadAddr-_phySecBufMiuOffset));
}

MS_PHY HAL_TSP_SecBuf_GetBufStart(REG_SecFlt *pSecBuf)
{
    return (((MS_PHY)_TSP_SEC_BUF_ADDR_START(pSecBuf) & 0xFFFFFFFFUL) + _phySecBufMiuOffset);
}

MS_PHY HAL_TSP_SecBuf_GetBufEnd(REG_SecFlt *pSecBuf)
{
    return (((MS_PHY)HAL_REG32_IndR((REG32 *)&pSecBuf->BufEnd) & 0xFFFFFFFFUL) + _phySecBufMiuOffset);
}

MS_PHY HAL_TSP_SecBuf_GetBufCur(REG_SecFlt *pSecBuf)
{
    return (((MS_PHY)HAL_REG32_IndR((REG32 *)&pSecBuf->BufCur) & 0xFFFFFFFFUL) + _phySecBufMiuOffset);
}

void HAL_TSP_SecBuf_ResetBuffer(REG_SecFlt *pSecBuf)
{
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufCur, _TSP_SEC_BUF_ADDR_START(pSecBuf));
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufRead, _TSP_SEC_BUF_ADDR_START(pSecBuf));
    HAL_REG32_IndW((REG32 *)&pSecBuf->BufWrite, _TSP_SEC_BUF_ADDR_START(pSecBuf));
}

MS_PHY HAL_TSP_SecBuf_GetBufRead(REG_SecFlt *pSecBuf)
{
    return (((MS_PHY)HAL_REG32_IndR((REG32 *)&pSecBuf->BufRead)  & 0xFFFFFFFFUL) + _phySecBufMiuOffset);
}

MS_PHY HAL_TSP_SecBuf_GetBufWrite(REG_SecFlt *pSecBuf)
{
    return (((MS_PHY)HAL_REG32_IndR((REG32 *)&pSecBuf->BufWrite) & 0xFFFFFFFFUL) + _phySecBufMiuOffset);
}

#undef _TSP_SEC_BUF_ADDR_START

void HAL_TSP_SecBuf_SetBufRead_tmp(REG_SecFlt *pSecBuf, MS_PHY phyReadAddr)
{
    _phySecBufMiuOffset = _HAL_TSP_MIU_OFFSET(phyReadAddr);
    HAL_REG32_IndW_tmp((REG32 *)&pSecBuf->BufRead, (MS_U32)(phyReadAddr-_phySecBufMiuOffset));
}

MS_PHY HAL_TSP_SecBuf_GetBufWrite_tmp(REG_SecFlt *pSecBuf)
{
    return (((MS_PHY)HAL_REG32_IndR_tmp((REG32 *)&pSecBuf->BufWrite) & 0xFFFFFFFFUL) + _phySecBufMiuOffset);
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
    _HAL_REG32_W(&_TspCtrl[0].TsFileIn_Timer, u32Delay);
}

void HAL_TSP_CmdQ_TsDma_SetAddr(MS_PHY phyStreamAddr)
{
    _phyFIBufMiuOffset = _HAL_TSP_MIU_OFFSET(phyStreamAddr);
    _HAL_REG32_W(&_TspCtrl[0].TsDma_Addr, (MS_U32)(phyStreamAddr-_phyFIBufMiuOffset));
}

MS_BOOL HAL_TSP_CmdQ_TsDma_SetSize(MS_U32 u32StreamSize)
{
    if (u32StreamSize < 16) // t2 HW limite, CMQ had some problem while bufsize < 16
    {
        return FALSE;
    }
    _HAL_REG32_W(&_TspCtrl[0].TsDma_Size, u32StreamSize);
    return TRUE;
}

void HAL_TSP_CmdQ_TsDma_Start(MS_U32 u32TsDmaCtrl)
{
    // enable filein byte timer
    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_BYTE_TIMER_EN));

    _HAL_REG32_W(&_TspCtrl[0].TsDma_Ctrl_CmdQ, TSP_TSDMA_CTRL_START | (u32TsDmaCtrl & TSP_TSDMA_CTRL_PESMODE_MASK));
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
            (MS_U32)(HAS_FLAG(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_PAUSE)));
}

MS_U32 HAL_TSP_CmdQ_EmptyCount(void)
{
    return (TSP_CMDQ_SIZE - ((_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_CMDQ_CNT_MASK)>>TSP_CMDQ_CNT_SHFT));
}

MS_U8 HAL_TSP_Get_CmdQFifoLevel(void)
{
    return (MS_U8)((_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_CMDQ_WR_LEVEL_MASK) >> TSP_CMDQ_WR_LEVEL_SHFT);
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
//--------------------------------------------------------------------------------------------------
void HAL_TSP_PVR_SetBuffer(MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    MS_PHY                     phyBufEnd;

    _phyPVRBufMiuOffset[0] = _HAL_TSP_MIU_OFFSET(phyBufStart0);

    // for buffer 0
    phyBufEnd=              phyBufStart0+ u32BufSize0;
    _HAL_REG32_W(&_TspCtrl[0].TsRec_Head, ((MS_U32)(phyBufStart0-_phyPVRBufMiuOffset[0])>> MIU_BUS));                // 8 bytes unit
    _HAL_REG32_W(&_TspCtrl[0].TsRec_Tail, ((MS_U32)(phyBufEnd-_phyPVRBufMiuOffset[0])>> MIU_BUS));                   // 8 bytes unit

    // for buffer 1
    phyBufEnd=              phyBufStart1+ u32BufSize1;
#define ADDR_PVR_HEAD20         (_virtRegBase+ 0x2a04)
#define ADDR_PVR_HEAD21         (_virtRegBase+ 0x2a08)
#define ADDR_PVR_MID20          (_virtRegBase+ 0x2a0c)
#define ADDR_PVR_MID21          (_virtRegBase+ 0x2a10)
#define ADDR_PVR_TAIL20         (_virtRegBase+ 0x2a14)
#define ADDR_PVR_TAIL21         (_virtRegBase+ 0x2a18)
    REG16_T(ADDR_PVR_HEAD20)=   (MS_U16)((phyBufStart1-_phyPVRBufMiuOffset[0])>> MIU_BUS) & 0xFFFF;
    REG16_T(ADDR_PVR_HEAD21)=   (MS_U16)((phyBufStart1-_phyPVRBufMiuOffset[0])>> (MIU_BUS+ 16)) & 0xFFFF;
    REG16_T(ADDR_PVR_TAIL20)=   (MS_U16)((phyBufEnd-_phyPVRBufMiuOffset[0])>> MIU_BUS) & 0xFFFF;
    REG16_T(ADDR_PVR_TAIL21)=   (MS_U16)((phyBufEnd-_phyPVRBufMiuOffset[0])>> (MIU_BUS+ 16)) & 0xFFFF;
#undef ADDR_PVR_HEAD20
#undef ADDR_PVR_HEAD21
#undef ADDR_PVR_MID20
#undef ADDR_PVR_MID21
#undef ADDR_PVR_TAIL20
#undef ADDR_PVR_TAIL21

    // enable PVR ping pong buffer
    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_STR2MI_MIU_PINPON_EN));

    // flush PVR buffer
    HAL_TSP_PVR_WaitFlush();
}

void HAL_TSP_PVR_Enable(MS_BOOL bEnable)
{
    //set burst len = 1
    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_burst_len_MASK));

    // disable timeshift write protect -- it must enable while record/playback use the same buffer
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILE_CHECK_WP));

    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_ENABLE));
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_ENABLE));
        //_HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
        //        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
    }
}

void HAL_TSP_PVR_Reset(void)
{
    // Richard: @FIXME:
    // Don't know PVR "reset" definition. call flush instead.
    HAL_TSP_PVR_WaitFlush();
}

void HAL_TSP_PVR_All(MS_BOOL bPvrAll)
{
    if (bPvrAll)
    {
        // bypass mode
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_PIDFLT_SEC));
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_REG_REC_PID_EN));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_PIDFLT_SEC));
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_REG_REC_PID_EN));
    }
}

void HAL_TSP_PVR_Filein_Enable(MS_BOOL bBypassHD, MS_BOOL bEnable, MS_BOOL bRecAll)
{
    MS_U32 u32data = _HAL_REG32_R(&_TspCtrl[0].reg15b4);

    if(bBypassHD)
        u32data |= TSP_PVR_TS_HEADER;
    else
        u32data &= ~TSP_PVR_TS_HEADER;

    if(bRecAll)
    {
        u32data |= TSP_REC_ALL_FILE;
    }
    else
    {
        u32data &= ~TSP_REC_ALL_FILE;
    }

    if(bEnable)
        u32data |= TSP_PVR_FILEIN;
    else
        u32data &= ~TSP_PVR_FILEIN;

    _HAL_REG32_W(&_TspCtrl[0].reg15b4, u32data);
}

MS_BOOL HAL_TSP_PVR_Eng_IsEnabled(MS_U32 u32EngId)
{
    if(u32EngId == 0)
    {
        return ((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PVR_ENABLE) > 0);
    }
    else
    {
        return FALSE;
    }
}

void HAL_TSP_LiveAB_En(MS_BOOL bPF_EN)
{
    if (bPF_EN)
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | TSP_LIVEAB_SEL);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein, _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & ~TSP_LIVEAB_SEL);
    }
}

MS_BOOL HAL_TSP_IsLiveAB_Enable(void)
{
    return ((_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & TSP_LIVEAB_SEL) != 0);
}

void HAL_TSP_SetPKTSize(MS_U32 u32PKTSize)
{
    if((u32PKTSize == 0x82) || (u32PKTSize == 0x86)) // RVU
    {
         printf("HAL_TSP_SetPKTSize: This chip not support RVU!\n");  // not support
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


MS_PHY HAL_TSP_PVR_GetBufWrite(void)
{
    MS_PHY phyvalue;

    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_STR2MI_WP_LD));
    phyvalue = (MS_PHY)(_HAL_REG32_R(&_TspCtrl[0].TsRec_WPtr)<< MIU_BUS) + _phyPVRBufMiuOffset[0];
    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_STR2MI_WP_LD));

    return phyvalue;

}

void HAL_TSP_PVR_WaitFlush(void)
{
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_FLUSH));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_FLUSH));
}

void HAL_TSP_PVR_REC_FLT_Enable(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_PIDFLT_SEC));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PVR_PIDFLT_SEC));
    }
}


//--------------------------------------------------------------------------------------------------
// For FW part
//--------------------------------------------------------------------------------------------------
/*
void HAL_TSP_SetFwMsg(MS_U32 u32Mode)
{
    _HAL_REG32_W(&_TspCtrl[0].Mcu_Msg, u32Mode);
}

MS_U32 HAL_TSP_GetFwMsg(void)
{
    return (_HAL_REG32_R(&_TspCtrl[0].Mcu_Msg));
}
*/

static MS_U32 _u32FlowPadMap[2] = { 0x0, 0x0};

//--------------------------------------------------------------------------------------------------
// For pad select part
//--------------------------------------------------------------------------------------------------
void HAL_TSP_SelPad(MS_U32 u32EngId, MS_U32 u32Flow, MS_U32 u32Pad)
{

    MS_U32 u32Shft = 0;

    MS_ASSERT(4 >= u32Pad);

    if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    switch (u32Pad)
    {
        case 0: // internal demod
            u32Pad = TSP_MUX_INDEMOD;
            break;
        //ts0 pad is null
        case 2: // TS1 pad
            u32Pad = TSP_MUX_TS1;
            if(_bTsPadUsed[1] == FALSE)
            {
                TSP_TOP_REG(REG_TOP_TS1_PE) = TSP_TOP_REG(REG_TOP_TS1_PE)| REG_TOP_TS_PE_MASK;
                _bTsPadUsed[1] = TRUE;
            }
            break;
        case 4: // TS2 pad
            u32Pad = TSP_MUX_TS2;
            if(_bTsPadUsed[2] == FALSE)
            {
                TSP_TOP_REG(REG_TOP_TS2_PE) = TSP_TOP_REG(REG_TOP_TS2_PE)| REG_TOP_TS_PE_MASK;
                _bTsPadUsed[2] = TRUE;
            }
            break;
        default:
            return ;
    }

    _u32FlowPadMap[u32Flow] = u32Pad;

    if(u32Flow == 0)
        u32Shft= REG_TOP_TSN_MUX_TS0_SHIFT;
    else
        u32Shft = REG_TOP_TSN_MUX_TS1_SHIFT;

    TSP_TOP_REG(REG_TOP_TSN_MUX) = (TSP_TOP_REG(REG_TOP_TSN_MUX) & ~(REG_TOP_TSN_MUX_MASK<< u32Shft)) | (u32Pad<< u32Shft);

}

void HAL_TSP_SelPad_ClkInv(MS_U32 u32EngId, MS_U32 u32Flow, MS_BOOL bClkInv)
{
    MS_U32 u32Clk = 0;
    MS_U32 u32Shft = 0;

    if(u32Flow >= TSP_IF_NUM)
    {
        return;
    }

    if(_u32FlowPadMap[u32Flow] == TSP_MUX_INDEMOD)
        u32Clk = TSP_CLK_INDEMOD;
    else if(_u32FlowPadMap[u32Flow] == TSP_MUX_TS1)
        u32Clk = TSP_CLK_TS1;
    else if(_u32FlowPadMap[u32Flow] == TSP_MUX_TS2)
        u32Clk = TSP_CLK_TS2;

    if (bClkInv)
    {
        u32Clk |= TSP_CLK_INVERSE;
    }

    u32Shft = (0 == u32Flow)? 0: 8;
    TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) & ~(REG_CLKGEN0_TSN_CLK_MASK<< u32Shft)) | (u32Clk<< u32Shft);
}

void HAL_TSP_SelPad_ExtSync(MS_U32 u32EngId, MS_BOOL bExtSync, MS_U32 u32Flow)
{
    REG32* pReg = NULL;
    MS_U32 u32ExtSync = 0;

    if (0 == u32Flow)
    {
        pReg = &(_TspCtrl[0].Hw_Config0);
        u32ExtSync = TSP_HW_CFG0_TSIF0_EXTSYNC;
    }
    else if (1 == u32Flow)
    {
        pReg = &(_TspCtrl[0].Hw_Config2);
        u32ExtSync = TSP_HW_CFG2_TSIF1_EXTSYNC;
    }
    else
    {
        //ASSERT(0);
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
    MS_U32 u32Pad = _u32FlowPadMap[u32Flow];

    if (0 == u32Flow)       // TS_IF0
    {
        pReg = &(_TspCtrl[0].Hw_Config0);
        u32Parl = TSP_HW_CFG0_TSIF0_PARL;
    }
    else if (1 == u32Flow)  // TS_IF1
    {
        pReg = &(_TspCtrl[0].Hw_Config2);
        u32Parl = TSP_HW_CFG2_TSIF1_PARL;
    }
    else
    {
        //ASSERT(0);
        return;
    }
    
    if(u32Pad == TSP_MUX_TS1)
    {
        if(bParl)
            TSP_TOP_REG(REG_TOP_TSCONFIG) = (TSP_TOP_REG(REG_TOP_TSCONFIG) & ~(REG_TOP_TS_TS1_CFG_MASK<< REG_TOP_TS1CFG_SHIFT)) | (REG_TOP_TS1_PARALLEL_IN<< REG_TOP_TS1CFG_SHIFT);
        else
            TSP_TOP_REG(REG_TOP_TSCONFIG) = (TSP_TOP_REG(REG_TOP_TSCONFIG) & ~(REG_TOP_TS_TS1_CFG_MASK<< REG_TOP_TS1CFG_SHIFT)) | (REG_TOP_TS1_SERIAL_IN<< REG_TOP_TS1CFG_SHIFT);
    }
    else if(u32Pad == TSP_MUX_TS2)
    {
        TSP_TOP_REG(REG_TOP_TSCONFIG) = (TSP_TOP_REG(REG_TOP_TSCONFIG) & ~(REG_TOP_TS_TS2_CFG_MASK<< REG_TOP_TS2CFG_SHIFT)) | (REG_TOP_TS2_SERIAL_IN<< REG_TOP_TS2CFG_SHIFT);
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

void HAL_TSP_Parl_BitOrderSwap(MS_U32 u32EngId, MS_U32 u32Flow, MS_BOOL bInvert)
{
    REG32* pReg = &(_TspCtrl[0].Hw_Config4);
    MS_U32 u32Invert = 0;

    if(0 == u32Flow)
    {
        u32Invert = TSP_HW_CFG4_TS_DATA0_SWAP;
    }
    else if(1 == u32Flow)
    {
        u32Invert = TSP_HW_CFG4_TS_DATA1_SWAP;
    }
    else
    {
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
    case HAL_TSP_CAP_SEC_FILTER_NUM:
        *((MS_U32*)pData) = TSP_SECFLT_NUM;
        break;
    case HAL_TSP_CAP_SEC_BUF_NUM:
        *((MS_U32*)pData) = TSP_SECBUF_NUM;
        break;
    case HAL_TSP_CAP_PVR_ENG_NUM:
        *((MS_U32*)pData) = TSP_PVR_IF_NUM;
        break;
    case HAL_TSP_CAP_PVR_FILTER_NUM:
        *((MS_U32*)pData) = TSP_PIDFLT1_NUM;
        break;
    case HAL_TSP_CAP_MMFI_AUDIO_FILTER_NUM:
        *((MS_U32*)pData) = TSP_MMFI_AUDIO_FILTER_NUM;
        break;
    case HAL_TSP_CAP_MMFI_V3D_FILTER_NUM:
        *((MS_U32*)pData) = TSP_MMFI_V3D_FILTER_NUM;
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
        *((MS_U32*)pData) = 1;
        break;
    case HAL_TSP_CAP_PVRCA0_FLT_RANGE:
        *((MS_U32*)pData) = TSP_SECFLT_NUM;
        *((MS_U32*)pData + 1) = TSP_SECFLT_NUM + TSP_PIDFLT1_NUM - 1;
        break;
    case HAL_TSP_CAP_PVRCA1_FLT_RANGE:
    case HAL_TSP_CAP_PVR1_FILTER_NUM:
    case HAL_TSP_CAP_SHAREKEY_FLT_RANGE:
        *((MS_U32*)pData) = 0;
        break;
    case HAL_TSP_CAP_HW_TYPE:
        *((MS_U32*)pData) = 0x00001001;
        break;

    case HAL_TSP_CAP_HWPCR_SUPPORT:
        *((MS_U32*)pData) = 0;
        break;

    case HAL_TSP_CAP_HWWP_SET_NUM:
		*((MS_U32*)pData) = TSP_WP_SET_NUM;
        break;

    case HAL_TSP_CAP_DSCMB_ENG_NUM:
        *((MS_U32*)pData) = 1;
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

void HAL_TSP_RemoveDupAVPkt(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
            _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) | (TSP_DUP_PKT_SKIP_VD|TSP_DUP_PKT_SKIP_V3D|TSP_DUP_PKT_SKIP_AV|TSP_DUP_PKT_SKIP_AD));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
            _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) & ~(TSP_DUP_PKT_SKIP_VD|TSP_DUP_PKT_SKIP_V3D|TSP_DUP_PKT_SKIP_AV|TSP_DUP_PKT_SKIP_AD));
    }
}

void HAL_TSP_RemoveDupAVFifoPkt(MS_U32 u32StreamId, MS_BOOL bEnable)
{
    MS_U32 u32Flag[4] = {TSP_DUP_PKT_SKIP_VD, TSP_DUP_PKT_SKIP_AV, TSP_DUP_PKT_SKIP_AD, TSP_DUP_PKT_SKIP_V3D};

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

MS_BOOL HAL_TSP_GetTSIF_Status(MS_U8 u8TsIfId, MS_U16* pu16Pad, MS_U16* pu16Clk, MS_BOOL* pbExtSync, MS_BOOL* pbParl)
{
    MS_U16 u16pad;
    MS_U32 u32data;

    *pu16Pad = 0xFFFF;
    *pu16Clk = TSP_CLK_DISABLE;
    *pbExtSync = FALSE;
    *pbParl = FALSE;

    u16pad = TSP_TOP_REG(REG_TOP_TSN_MUX);

    switch(u8TsIfId)
    {
        case 0:
            u16pad >>= REG_TOP_TSN_MUX_TS0_SHIFT;
            *pu16Clk = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) >> REG_CLKGEN0_TSN_CLK_TS0_SHIFT) & REG_CLKGEN0_TSN_CLK_MASK;
            u32data = _HAL_REG32_R(&_TspCtrl[0].Hw_Config0);
            *pbExtSync = ((u32data & TSP_HW_CFG0_TSIF0_EXTSYNC) == TSP_HW_CFG0_TSIF0_EXTSYNC);
            *pbParl = ((u32data & TSP_HW_CFG0_TSIF0_PARL) == TSP_HW_CFG0_TSIF0_PARL);
            break;
        case 1:
            u16pad >>= REG_TOP_TSN_MUX_TS1_SHIFT;
            *pu16Clk = (TSP_CLKGEN0_REG(REG_CLKGEN0_TSN_CLK) >> REG_CLKGEN0_TSN_CLK_TS1_SHIFT) & REG_CLKGEN0_TSN_CLK_MASK;
            u32data = _HAL_REG32_R(&_TspCtrl[0].Hw_Config2);
            *pbExtSync = ((u32data & TSP_HW_CFG2_TSIF1_EXTSYNC) == TSP_HW_CFG2_TSIF1_EXTSYNC);
            *pbParl = ((u32data & TSP_HW_CFG2_TSIF1_PARL) == TSP_HW_CFG2_TSIF1_PARL);
            break;
        default:
            return FALSE;
    }

    *pu16Pad = u16pad & REG_TOP_TSN_MUX_MASK;

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macro function
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void _HAL_TSP_FW_load(
    MS_PHY                     phyFwAddrPhys,
    MS_U32                     u32FwSize,
    MS_BOOL                    bFwDMA,
    MS_BOOL                    bIQmem,
    MS_BOOL                    bDQmem)
{
#define _TSP_QMEM_I_MASK            0xffff8000UL //total: 0x4000
#define _TSP_QMEM_I_ADDR_HIT        0x00000000UL
#define _TSP_QMEM_I_ADDR_MISS       0xffffffffUL
#define _TSP_QMEM_D_MASK            0xffff8000UL
#define _TSP_QMEM_D_ADDR_HIT        0x00000000UL
#define _TSP_QMEM_D_ADDR_MISS       0xffffffffUL
#define _TSP_QMEM_SIZE              0x1000UL // 16K bytes, 32bit aligment  //0x4000

    // bDQmem is always true
    MS_ASSERT(bDQmem);

    _phyOrLoadMiuOffset = _HAL_TSP_MIU_OFFSET(phyFwAddrPhys);

    _HAL_REG32_W(&_TspCtrl2[0].Qmem_Config,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl2[0].Qmem_Config), TSP_TSP_SEL_SRAM));

    // @FIXME: Richard: Only allow TSP FW running in DRAM at this first stage.
    // improve this afterward.
    MS_ASSERT(phyFwAddrPhys & TSP_CPU_BASE_ADDR_MASK);
    _HAL_REG32_W(&_TspCtrl[0].Cpu_Base, 0/*u32FwAddrPhys >> 3*/); // 16 bytes address unit

    if (bFwDMA)
    {
        MS_U32              u32DnldCtrl  = 0;
        MS_U32              u32DnldCtrl1 = 0;
        u32DnldCtrl  =      (MS_U32)(((phyFwAddrPhys-_phyOrLoadMiuOffset) >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT) & TSP_DNLD_ADDR_MASK;
        u32DnldCtrl1 =      (MS_U32)((((phyFwAddrPhys-_phyOrLoadMiuOffset) >> MIU_BUS) >> TSP_DNLD_ADDR_ALI_SHIFT) & TSP_DNLD_ADDR_MASK1) >> TSP_DNLD_ADDR_SHFT1;
        printf("firmware 111 0x%08lx 0x%08lx 0x%08lx\n", (unsigned long)phyFwAddrPhys, (unsigned long)u32DnldCtrl1, (unsigned long)u32DnldCtrl);

        u32DnldCtrl |=      (_TSP_QMEM_SIZE << TSP_DNLD_NUM_SHFT);

        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl, u32DnldCtrl);
        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2, (_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2) & ~(TSP_DNLD_ADDR_MASK1 >> TSP_DNLD_ADDR_SHFT1)) | u32DnldCtrl1);
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_START| TSP_CTRL_DNLD_DONE));
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_START));
        while (!HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_DONE));//printf(".");
        _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_DNLD_START| TSP_CTRL_DNLD_DONE));
    }

    _HAL_REG32_W(&_TspCtrl[0].Qmem_Imask, _TSP_QMEM_I_MASK);

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

    _HAL_REG32_W(&_TspCtrl[0].Qmem_Dmask,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Qmem_Dmask), _TSP_QMEM_D_MASK));
    _HAL_REG32_W(&_TspCtrl[0].Qmem_Dbase,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Qmem_Dbase), _TSP_QMEM_D_ADDR_HIT));

#undef _TSP_QMEM_I_MASK
#undef _TSP_QMEM_I_ADDR_HIT
#undef _TSP_QMEM_I_ADDR_MISS
#undef _TSP_QMEM_D_MASK
#undef _TSP_QMEM_D_ADDR_HIT
#undef _TSP_QMEM_D_ADDR_MISS
#undef _TSP_QMEM_SIZE
}

void HAL_TSP_filein_enable(MS_BOOL b_enable)
{
    // disable timeshift write protect -- it must enable while record/playback use the same buffer
        _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
                  RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILE_CHECK_WP));

    // Richard: enable/disable file in timer as well
    //          file in could only walk through pid filter set 0.
    if (b_enable)
    {
        // Set Data port enable for audio bypass
        //_HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
        //        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
        //_HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
        //        SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_FILEIN_ENABLE| TSP_CTRL1_FILEIN_TIMER_ENABLE));

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

static void _HAL_TSP_tsif_select(MS_U8 u8_tsif)
{
    if (u8_tsif == 1)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF1_ENABLE));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_TSIF0_ENABLE));
    }
}

/*
static void _HAL_TSP_SelPad(MS_U32 u32EngId, MS_U32 PadId)
{
    if (PadId)  // TS1
    {
        _HAL_REG32_W(&_TspCtrl[u32EngId].TSP_Ctrl,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_PAD1_SEL));
    }
    else        // TS0
    {
        _HAL_REG32_W(&_TspCtrl[u32EngId].TSP_Ctrl,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_PAD1_SEL));
    }
}
*/

void HAL_TSP_PS_Path_Disable(void)
{
    // set PS VID/AUD enable while video/audio/audio2 bypass mode
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN)));
    _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));
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
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_VID_EN));
            }
            break;
        case TSP_TSDMA_CTRL_APES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PS_AUD_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD2_EN));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_AUD_EN));
            }
            break;
        case TSP_TSDMA_CTRL_A2PES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Hw_Config4) & TSP_HW_CFG4_PS_AUD2_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN)));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_PS_AUD2_EN));
            }
            break;
        case TSP_TSDMA_CTRL_V3DPES0:
            if((_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2) & TSP_PS_VID3D_EN) == 0)
            {
                _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                    RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN)));

                _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
                _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                    SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));
            }
            break;
        default:
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), (TSP_HW_CFG4_PS_VID_EN|TSP_HW_CFG4_PS_AUD_EN|TSP_HW_CFG4_PS_AUD2_EN)));
            _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PS_VID3D_EN));
            _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), TSP_HW_CFG0_DATA_PORT_EN));
            break;
    }
}

MS_U32 HAL_TSP_GetCtrlMode(MS_U32 u32EngId)
{
    return (_HAL_REG32_R(&_TspCtrl[u32EngId].TSP_Ctrl));
}

void HAL_TSP_DoubleBuf_En(MS_BOOL bBuf_Sel)
{
    if (bBuf_Sel)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) | (TSP_DOUBLE_BUF_EN|TSP_BLK_DISABLE|TSP_DOUBLE_BUF_DESC));
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) & ~TSP_DOUBLE_BUF_SWITCH);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) & ~(TSP_DOUBLE_BUF_EN|TSP_BLK_DISABLE));
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) | (TSP_DOUBLE_BUF_SWITCH|TSP_DOUBLE_BUF_DESC));
    }
    //_HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) | TSP_DOUBLE_BUF_DESC);
}

void HAL_TSP_Flush_AV_FIFO(MS_U32 u32StreamId, MS_BOOL bFlush)
{
    MS_U32 u32Flag;

    switch(u32StreamId)
    {
        case 0:
            u32Flag = TSP_RESET_VFIFO;
            break;
        case 1:
            u32Flag = TSP_RESET_AFIFO;
            break;
        case 2:
            u32Flag = TSP_RESET_AFIFO2;
            break;
        case 3:
            u32Flag = TSP_RESET_VFIFO3D;
            break;
        default:
            return;
    }
    if (bFlush)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), u32Flag));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), u32Flag));
    }
}

#define ADDR_AVFIFO_STATUS         (_virtRegBase+ 0x2adc)
MS_U32 HAL_TSP_Get_AVFifoLevel(MS_U32 u32StreamId)
{
    switch (u32StreamId)
    {
        case 0: // return VFifo status
            return (((REG16_T(ADDR_AVFIFO_STATUS))>>4)&0x0f);
        case 1: // return AFifo 0 status
            return (((REG16_T(ADDR_AVFIFO_STATUS))>>8)&0x0f);
        case 2: // return AFifo 1 status
            return (((REG16_T(ADDR_AVFIFO_STATUS))>>12)&0x0f);
        case 3: // return V3D Fifo status
            return (REG16_T(ADDR_AVFIFO_STATUS)&0x0f);
        default:
            return -1;
    }
}
#undef ADDR_AVFIFO_STATUS

void HAL_TSP_Ind_Enable(void)
{
    _HAL_REG32_W(&_TspCtrl[0].TSP_Ctrl,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].TSP_Ctrl), TSP_CTRL_SW_RST));
}

MS_U32 HAL_TSP_HW_INT_STATUS(void)
{
    return (MS_U32)(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat) & TSP_HWINT_STATUS_MASK);
}

void HAL_TSP_SetBank(MS_VIRT u32NonPmBankAddr, MS_VIRT u32PmBankAddr)
{
    _virtRegBase = u32NonPmBankAddr;
    _TspCtrl  = (REG_Ctrl*)(_virtRegBase  + REG_CTRL_BASE);
    _TspCtrl2 = (REG_Ctrl2*)(_virtRegBase + REG_CTRL_MMFIBASE);
    _TspCtrl3 = (REG_Ctrl3*)(_virtRegBase + REG_CTRL_BASE_TS3);
}

void HAL_TSP_Reset(MS_U32 u32EngId)
{
    _HAL_REG32_W(&_TspCtrl[u32EngId].TSP_Ctrl, 0);
}

void HAL_TSP_HwPatch(void)
{
    _HAL_REG32_W(&_TspCtrl2[0].Qmem_Config,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl2[0].Qmem_Config), TSP_TSP_SEL_SRAM));

    // To prevent the race condition of accessing section filter registers from HW/CPU.
    // It's a HW bug.
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_CFG4_BYTE_ADDR_DMA|TSP_HW_CFG4_ALT_TS_SIZE|TSP_HW_CFG4_ISYNC_PATCH_EN));

    // Bad initial value of TSP_CTRL1
    // Suppose Standby mode for TSP should NOT be enabled.
    // Enabling TSP standby mode cause TSP section registers (SRAM in AEON) malfunction.
    // Disable it by SW at this stage.
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_STANDBY));

    // @FIXME: disable not match mask
    _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            _HAL_REG32_R(&_TspCtrl[0].reg15b4)|TSP_NMATCH_DIS);

    // Jericho helps
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), 0xFFFF0000));
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config0,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config0), 0xBBBB0000));

    _HAL_REG32_W(&_TspCtrl[0].reg160C,
#ifdef VQ_ENABLE
        _HAL_REG32_R(&_TspCtrl[0].reg160C)| (TSP_RM_DMA_GLITCH|TSP_DOUBLE_BUF_DESC));
#else
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_RM_DMA_GLITCH));
#endif

    _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
        _HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein) |
        (TSP_SYSTIME_MODE_STC64|TSP_DUP_PKT_SKIP_VD|TSP_DUP_PKT_SKIP_V3D|TSP_DUP_PKT_SKIP_AV));

    _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PREVENT_OVF_META));

    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), (TSP_STR2MI_WP_LD|TSP_RM_OVERFLOW_GLITCH))); //improve pvr performance


    // Set match PID, default is AV FIFO
    _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_AVPID_ST_AV));

    // Set PUSI 3 byte mode for Audio
    _HAL_REG32_W(&_TspCtrl[0].HW2_Config3,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].HW2_Config3), TSP_PUSI_3BYTE_MODE));

    // 2p sram setting
    _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_HW_STANDBY_MODE));

    // Set filein segment bit to 0
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_PVR_CMD_QUEUE_ENABLE));

    _HAL_REG16_W(&_TspCtrl2[0].HwCfg1,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl2[0].HwCfg1), (AF_PKT_LOSS_BYTE_ECO|UPDATE_SCRAMBLE_PID_PUSI|NEW_OVERFLOW_MODE|FIX_PINPON_SYNC_IN_ECO)));

    _HAL_REG16_W(&_TspCtrl2[0].HwCfg0, _HAL_REG16_R(&_TspCtrl2[0].HwCfg0) | (PREVENT_SRAM_COLLISION|MASK_SCR_VID_EN|MASK_SCR_VID_3D_EN|MASK_SCR_AUD_EN|MASK_SCR_AUDB_EN));

    _HAL_REG16_W(&_TspCtrl2[0].HwCfg2, _HAL_REG16_R(&_TspCtrl2[0].HwCfg2) | REG_RST_CC_MODE);

    _b64STCMode = TRUE;

    //VQ parameters
    _HAL_REG16_W(&_TspCtrl3[0].Hw_Eco0, SET_FLAG1(_HAL_REG16_R(&_TspCtrl3[0].Hw_Eco0), TSP_FIXED_MIU_FLUSH));
    _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL,   (_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL)  & ~TSP_VQ0_FORCE_FIRE_CNT_1K_MASK) | (0x0C << TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT));
    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config, (_HAL_REG32_R(&_TspCtrl[0].VQ1_Config) & ~TSP_VQ1_FORCEFIRE_CNT_1K_MASK) | (0x0C << TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT));
}

void HAL_TSP_DoubleBuf_Disable(void)
{

    _HAL_REG32_W(&_TspCtrl[0].reg160C,
#ifdef VQ_ENABLE
        _HAL_REG32_R(&_TspCtrl[0].reg160C) & ~(TSP_DOUBLE_BUF_EN|TSP_BLK_DISABLE|TSP_DOUBLE_BUF_SWITCH));
#else
        _HAL_REG32_R(&_TspCtrl[0].reg160C) & ~(TSP_DOUBLE_BUF_EN|TSP_BLK_DISABLE|TSP_DOUBLE_BUF_SWITCH|TSP_DOUBLE_BUF_DESC));
#endif
}

//  Default value of low bound is 0, default value of up bound is 0xFFFFFFFF, means no protection
//  If set both low bound and up bound to be 0, means protection all
//  The range can be written:  phyStartAddr <= x <= phyEndAddr
//  Protection range: x > phyEndAddr && x < phyStartAddr
void HAL_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr)
{
    MS_U32  lbnd1_8;
    MS_U32  ubnd1_8;
    MS_U32 lbnd9_24;
    MS_U32 ubnd9_24;
    MS_PHY  phyMiuOffset = _HAL_TSP_MIU_OFFSET(phyStartAddr);

    if (bEnable)
    {
        if(phyStartAddr == phyEndAddr)
            phyStartAddr += (1UL << MIU_BUS);
        else
            phyEndAddr -= (1UL << MIU_BUS);

        phyStartAddr -= phyMiuOffset;
        phyEndAddr -= phyMiuOffset;

        lbnd1_8  = (MS_U32)((phyStartAddr >> MIU_BUS) >> TSP_ORZ_DMAW_BND_ALT_SHIFT) & 0x00FFUL;
        lbnd9_24 = (MS_U32)(((phyStartAddr >> MIU_BUS) >> TSP_ORZ_DMAW_BND_ALT_SHIFT) >> 8UL) & 0xFFFFUL;
        ubnd1_8  = (MS_U32)((phyEndAddr >> MIU_BUS) >> TSP_ORZ_DMAW_BND_ALT_SHIFT) & 0x00FFUL;
        ubnd9_24 = (MS_U32)(((phyEndAddr >> MIU_BUS) >> TSP_ORZ_DMAW_BND_ALT_SHIFT) >> 8UL) & 0xFFFFUL;

        _HAL_REG32_W(&_TspCtrl[0].reg16C0, (_HAL_REG32_R(&_TspCtrl[0].reg16C0) & ~(TSP_ORZ_DMAW_LBND_LSB8)) | lbnd1_8);
        _HAL_REG32_W(&_TspCtrl[0].ORZ_DMAW, (_HAL_REG32_R(&_TspCtrl[0].ORZ_DMAW) & ~(TSP_ORZ_DMAW_LBND)) | lbnd9_24);
        _HAL_REG32_W(&_TspCtrl[0].reg16C0, (_HAL_REG32_R(&_TspCtrl[0].reg16C0) & ~(TSP_ORZ_DMAW_UBND_LSB8)) | (ubnd1_8 << TSP_ORZ_DMAW_UBND_LSB8_SHIFT));
        _HAL_REG32_W(&_TspCtrl[0].ORZ_DMAW, (_HAL_REG32_R(&_TspCtrl[0].ORZ_DMAW) & ~(TSP_ORZ_DMAW_UBND)) | (ubnd9_24 << TSP_ORZ_DMAW_UBND_SHIFT));

        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) | TSP_ORZ_DMAW_PROT_EN);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C, _HAL_REG32_R(&_TspCtrl[0].reg160C) & ~TSP_ORZ_DMAW_PROT_EN);
    }
}

//--------------------------------------------------------------------------------------------------
// For STC part
//--------------------------------------------------------------------------------------------------
void HAL_TSP_Stc_ctrl(MS_U32 u32EngId, MS_U32 u32Sync)
{
    /////////////Set STC control by HK////////////////
    // select synth from chip top : bit 1 -> 0 -> controlled by HK
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~REG_CLKGEN0_STC_CW_SEL;

    // set HK STC synth CW
    //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_L) = (u32Sync & 0xFFFF);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_STC_CW_H) = (u32Sync >> 16) & 0xFFFF;

    // set STC synth
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC_CW_EN;
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) &= ~(REG_CLKGEN0_STC_CW_EN);

    /////////////Set STC control by TSP////////////////
    // select synth from TSP      : bit 1 -> 1 -> controlled by TSP
    TSP_CLKGEN0_REG(REG_CLKGEN0_DC0_SYTNTH) |= REG_CLKGEN0_STC_CW_SEL;

    // set TSP STC synth CW
    //if CLK_MPLL_SYN is 432MHz, set 0x28000000;if CLK_MPLL_SYN is 216MHz, set 0x14000000
    HAL_REG32_IndW((REG32 *)(0x0021024c<<1), u32Sync);

    // t2 , t3 had no 0x0021025c, it was add after t4, synthesizer enable
    HAL_REG32_IndW((REG32 *)(0x0021025c<<1), HAL_REG32_IndR((REG32 *)(0x0021025c<<1))| 0x01);
    HAL_REG32_IndW((REG32 *)(0x0021025c<<1), HAL_REG32_IndR((REG32 *)(0x0021025c<<1))& ~0x01);
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
    if(_b64STCMode)
    {
        MS_U32 u32temp;

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].TsRec_Tail2_Pcr1) & TSP_PCR1_L16_MASK) >> TSP_PCR1_L16_SHFT;
        u32temp |= ((_HAL_REG32_R(&_TspCtrl[0].Pcr1) & 0xFFFF) << 16);
        return u32temp ;
    }
    else
    {
        return HAL_REG32_IndR((REG32 *)(0x00210244<< 1));
    }
}

MS_U32 HAL_TSP_GetSTC_32(MS_U32 u32EngId)
{
    if(_b64STCMode)
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

    _b64STCMode = bEnable;
}

void HAL_TSP_SetSTC(MS_U32 u32EngId, MS_U32 u32STC, MS_U32 u32STC_32)
{
    if(_b64STCMode)
    {
        MS_U32 u32temp;

        u32temp = ((u32STC & 0xFFFF) << TSP_PCR1_L16_SHFT) |
                (_HAL_REG32_R(&_TspCtrl[0].TsRec_Tail2_Pcr1) & ~TSP_PCR1_L16_MASK);
        _HAL_REG32_W(&_TspCtrl[0].TsRec_Tail2_Pcr1, u32temp);

        u32temp = ((u32STC >> 16) & 0xFFFF) | ((u32STC_32 & 0xFFFF) << 16);
        _HAL_REG32_W(&_TspCtrl[0].Pcr1, u32temp);

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr64_H) & ~TSP_PCR64_H16_MASK) | ((u32STC_32 >> 16) & TSP_PCR64_H16_MASK);
        _HAL_REG32_W(&_TspCtrl[0].Pcr64_H, u32temp);
    }
    else
    {
        HAL_REG32_IndW((REG32 *)(0x00210244<< 1), u32STC);
        HAL_REG32_IndW((REG32 *)(0x00210248<< 1), u32STC_32 & 0x01);
    }
}

MS_BOOL HAL_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd)
{
    #if 0
    MS_U32 u32opt = ((MS_U32)bAdd & 0xFF) << TSP_MCU_CMD_SET_STC_OFFSET_OPTION_SHIFT;

    _HAL_REG32_W(&_TspCtrl[0].MCU_Data0, u32Offset);
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_SET_STC_OFFSET | u32opt | u32EngId);

    while(_HAL_REG32_R(&_TspCtrl[0].MCU_Cmd) != 0);

    return TRUE;

    #endif

    return FALSE;
}


#if 0
void HAL_TSP_SetSTC_32(MS_U32 u32EngId, MS_U32 u32STC_32)
{
    if(_b64STCMode)
    {
        MS_U32 u32temp;

        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr1) & ~ 0xFFFF0000) | ((u32STC_32 & 0xFFFF) << 16);
        _HAL_REG32_W(&_TspCtrl[0].Pcr1, u32temp);
        u32temp = (_HAL_REG32_R(&_TspCtrl[0].Pcr64_H) & ~TSP_PCR64_H16_MASK) | ((u32STC_32 >> 16) & TSP_PCR64_H16_MASK);
        _HAL_REG32_W(&_TspCtrl[0].Pcr64_H, u32temp);
    }
    else
    {
        HAL_REG32_IndW((REG32 *)(0x00210248<< 1), u32STC_32 & 0x01);
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

    if (u8Eng == 0)
    {
        u32Cmd |= TSP_MCU_CMD_CTRL_STC_UPDATE;
        _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, u32Cmd);
    }
    else
    {
        return FALSE;
    }

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
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_PCR_GET);
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

void HAL_TSP_Int_Enable(MS_U32 u32Mask)
{
    _HAL_REG16_W(&_TspCtrl[0].HwInt_Stat,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat)|0xFF00, (MS_U16)(u32Mask>>8)));
}

void HAL_TSP_Int_ClearSw(void)
{
    if (_bIsHK)
    {
        _HAL_REG32_W(&_TspCtrl[0].SwInt_Stat, 0);
    }
    else
    {
        _HAL_REG16_W(&_TspCtrl[0].SwInt_Stat1_M, 0);
        _HAL_REG32_W(&_TspCtrl[0].SwInt_Stat1_H,
                     RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].SwInt_Stat1_H), TSP_SWINT1_H_MASK));
    }
}

void HAL_TSP_Int_ClearHw(MS_U32 u32Mask)
{
    _HAL_REG16_W(&_TspCtrl[0].HwInt_Stat,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].HwInt_Stat)|0xFF00, (MS_U16)u32Mask));
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
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_CMDQ_RESET));
    _HAL_REG16_W(&_TspCtrl[0].TSP_Ctrl1,
            RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].TSP_Ctrl1), TSP_CTRL1_CMDQ_RESET));

    //reset the last data that hw is excuting --> HW new design
    _HAL_REG16_W(&_TspCtrl2[0].HwCfg0,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl2[0].HwCfg0), TSP_TSIFCFG_WB_FSM_RESET));

    //check R done
    for(ii = 0; ii < 100; ii++)
    {
        if(_HAL_REG32_R(&_TspCtrl[0].TsDma_Ctrl_CmdQ) & TSP_TSDMA_RDONE)
        {
            break;
        }
        MsOS_DelayTask(1);
    }

    _HAL_REG16_W(&_TspCtrl2[0].HwCfg0,
                RESET_FLAG1(_HAL_REG16_R(&_TspCtrl2[0].HwCfg0), TSP_TSIFCFG_WB_FSM_RESET));

    if(ii == 100)
    {
        printf("%s, wait fine in reset timeout\n", __FUNCTION__);
        return FALSE;
    }

    return TRUE;
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

// u32TSSrc: 0 -> TS0, 1 -> File
// u32GroupId: 0 -> filter0~filter31
MS_U32 HAL_TSP_Scmb_Status(MS_U32 u32TSSrc, MS_U32 u32GroupId, MS_U32 u32PidFltId)
{
    MS_U32              u32PIDFltMask = u32PidFltId;
    MS_U32              u32ScmbSts = 0;

    if(u32PidFltId != 0xFFFFFFFF)
    {
        u32PIDFltMask = (1 << (u32PidFltId & 0x1F));
    }

    if (u32TSSrc == 0)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_match_pid_scr_ts_ld));

        u32ScmbSts = HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].TsPidScmbStatTsin), u32PIDFltMask);

        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_match_pid_scr_ts_ld));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_match_pid_scr_fi_ld));

        u32ScmbSts = HAS_FLAG(_HAL_REG32_R(&_TspCtrl[0].TsPidScmbStatFile), u32PIDFltMask);

        _HAL_REG32_W(&_TspCtrl[0].reg15b4,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg15b4), TSP_match_pid_scr_fi_ld));
    }

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
void HAL_TSP_CPU_SetBase(MS_PHY phyAddr, MS_U32 u32Size)
{
    printf("[%s][%d] load firmware (address, size) = (0x%08lx, 0x%08lx)\n", __FUNCTION__, __LINE__, (unsigned long)phyAddr, (unsigned long)u32Size);
    _HAL_TSP_FW_load(phyAddr, u32Size, TRUE, TRUE, TRUE);
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

void HAL_TSP_SetOwner(REG_SecFlt* pSecFilter, MS_BOOL bOwner)
{
    MS_U32 u32HkId;

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

void HAL_TSP_ResetTimeStamp(void)
{
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_TIMESTAMP_RESET));
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_TIMESTAMP_RESET));
}

MS_U32  HAL_TSP_GetPVRTimeStamp(void)
{
    MS_U32 u32lpcr;

    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR1_RLD));
    u32lpcr = _HAL_REG32_R(&_TspCtrl[0].LPcr1);
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR1_RLD));

    return u32lpcr;
}

void HAL_TSP_SetPVRTimeStamp(MS_U32 u32Stamp)
{
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR1_WLD));
    _HAL_REG32_W(&_TspCtrl[0].LPcr1,u32Stamp);
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR1_WLD));
}

MS_BOOL HAL_TSP_SetPVRTimeStampClk(MS_U8 u8PVRId, MS_U32 u32ClkSrc)
{
    if(u32ClkSrc == 0x0) // 90K
    {
        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PVR1_CLK_STAMP_27_EN));
    }
    else // 27M
    {
        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_PVR1_CLK_STAMP_27_EN));
    }
    return TRUE;
}

MS_U32 HAL_TSP_GetPlayBackTimeStamp(void)
{
    MS_U32 u32lpcr;

    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_RLD));
    u32lpcr = _HAL_REG32_R(&_TspCtrl[0].LPcr2);
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_RLD));

    return u32lpcr;
}

void HAL_TSP_SetPlayBackTimeStamp(MS_U32 u32Stamp)
{
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_WLD));
    _HAL_REG32_W(&_TspCtrl[0].LPcr2,u32Stamp);
    _HAL_REG32_W(&_TspCtrl[0].reg160C,
        RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_LPCR2_WLD));
}

MS_BOOL HAL_TSP_SetPlayBackTimeStampClk(MS_U8 u8Id, MS_U32 u32ClkSrc)
{
    if(u32ClkSrc == 0x0) // 90K
    {
        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_TSIF0_CLK_STAMP_27_EN));
    }
    else // 27M
    {
        _HAL_REG32_W(&_TspCtrl[0].Dnld_Ctrl2,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Dnld_Ctrl2), TSP_TSIF0_CLK_STAMP_27_EN));
    }
    return TRUE;
}

MS_U32 HAL_TSP_GetFileInTimeStamp(void)
{
    return _HAL_REG32_R(&_TspCtrl[0].TimeStamp_FileIn);
}

MS_BOOL HAL_TSP_GetFilinReadAddr(MS_PHY* pphyReadAddr)
{
    *pphyReadAddr = ((MS_PHY)_HAL_REG32_R(&_TspCtrl[0].TsFileIn_RPtr) << MIU_BUS) + _phyFIBufMiuOffset;

    return TRUE;
}

void HAL_TSP_SetDMABurstLen(MS_U32 u32Len)
{
    _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
        _HAL_REG32_R(&_TspCtrl[0].Hw_Config4) | ((u32Len<<TSP_HW_DMA_MODE_SHIFT)&TSP_HW_DMA_MODE_MASK));
    _HAL_REG32_W(&_TspCtrl[0].PktChkSizeFilein,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].PktChkSizeFilein), TSP_SEC_DMA_BURST_EN));
}

void HAL_TSP_PVR_PacketMode(MS_BOOL bSet)
{
    if (bSet)
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
            SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_RECORD192_EN));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg160C,
            RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].reg160C), TSP_RECORD192_EN));
    }
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


 #define CKG_TS0_TS1        0x16A0UL  //0x28
    #define CLK_TS0_DISABLE          0x0001UL
    #define CLK_TS0_INVERT           0x0002UL
    #define CLK_TS0_CLK_MASK         0x001CUL
    #define CLK_TS1_DISABLE          0x0100UL
    #define CLK_TS1_INVERT           0x0200UL
    #define CLK_TS1_CLK_MASK         0x1C00UL
 #define CKG_TSP_STC0       0x16A8UL  //0x2a
    #define CLK_TSP_DISABLE          0x0001UL
    #define CLK_TSP_INVERT           0x0002UL
    #define CLK_TSP_CLK_MASK         0x000CUL
    #define CLK_PAR_DISABLE          0x0010UL
    #define CLK_PAR_INVERT           0x0020UL
    #define CLK_PAR_CLK_MASK         0x0040UL
    #define CLK_STC_DISABLE          0x0100UL
    #define CLK_STC_INVERT           0x0200UL
    #define CLK_STC_CLK_MASK         0x0C00UL
#define CKG_TSP_STAMP      0x16ACUL  //0x2b
    #define CLK_SYN_STC0_MASK        0x0007UL
    #define CLK_SYN_STC0_432M        0x0001UL
    #define CLK_STAM_DISABLE         0x0100UL
    #define CLK_STAM_INVERT          0x0200UL
    #define CLK_STAM_CLK_MASK        0x0C00UL
#define CKG2_TSP_TS_SAMPLE 0x1464UL  //0x019
    #define CKG2_TSP_TS_SAMPLE_DISABLE       0x0010UL
    #define CKG2_TSP_TS_SAMPLE_INVERT        0x0020UL
    #define CKG2_TSP_TS_SAMPLE_CLK_MASK      0x00C0UL
#define CHIP_TSP_BOOT_CLK_SEL   0x3D68 //0x5a
    #define CHIP_TSP_BOOT_CLK_SEL_MASK  0x0020

void HAL_TSP_PowerCtrl(MS_BOOL bOn)
{
    if (bOn)
    {
        //Set PE Pad
        //TSP_TOP_REG(REG_TOP_TS0_PE) |= REG_TOP_TS0_PE_MASK;

        // Enable TSP Clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STC0),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STC0)), (CLK_TSP_DISABLE|CLK_TSP_INVERT|CLK_TSP_CLK_MASK)));

        // Enable CLK_PARSER clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STC0),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STC0)), (CLK_PAR_DISABLE|CLK_PAR_INVERT|CLK_PAR_CLK_MASK)));

        //TSP select SRAM
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CHIP_TSP_BOOT_CLK_SEL), RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CHIP_TSP_BOOT_CLK_SEL)), CHIP_TSP_BOOT_CLK_SEL_MASK));

        //Select SRAM
        _HAL_REG32_W(&(_TspCtrl2[0].Qmem_Config), SET_FLAG1(_HAL_REG32_R(&(_TspCtrl2[0].Qmem_Config)), TSP_TSP_SEL_SRAM));

        // Enable TS0 clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TS0_TS1),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TS0_TS1)), (CLK_TS0_DISABLE|CLK_TS0_INVERT|CLK_TS0_CLK_MASK)));

        // Enable TS1 clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TS0_TS1),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TS0_TS1)), (CLK_TS1_DISABLE|CLK_TS1_INVERT|CLK_TS1_CLK_MASK)));

         // Set SYN_STC to be 432MHz
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STAMP),
            SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STAMP)), CLK_SYN_STC0_432M));

        // Enable STC clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STC0),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STC0)), (CLK_STC_DISABLE|CLK_STC_INVERT|CLK_STC_CLK_MASK)));

        // Enable TIMESTAMP clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STAMP),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STAMP)), (CLK_STAM_DISABLE|CLK_STAM_INVERT|CLK_STAM_CLK_MASK)));

        // Enable Sample clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG2_TSP_TS_SAMPLE),
            RESET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG2_TSP_TS_SAMPLE)), (CKG2_TSP_TS_SAMPLE_DISABLE|CKG2_TSP_TS_SAMPLE_INVERT|CKG2_TSP_TS_SAMPLE_CLK_MASK)));

    }
    else
    {
        // Disable TSP clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STC0), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STC0)), CLK_TSP_DISABLE));

        // Disable TS0 clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TS0_TS1), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TS0_TS1)), CLK_TS0_DISABLE));

        // Disable TS1 clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TS0_TS1), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TS0_TS1)), CLK_TS1_DISABLE));

        // Disable STC clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STC0), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STC0)), CLK_STC_DISABLE));

        // Disable CLK_PARSER clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STC0), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STC0)), CLK_PAR_DISABLE));

        // Disable TIMESTAMP clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG_TSP_STAMP), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG_TSP_STAMP)), CLK_STAM_DISABLE));

        // Disable Sample clock
        _HAL_REG32L_W((REG32_L *)(_virtRegBase+CKG2_TSP_TS_SAMPLE), SET_FLAG1(_HAL_REG32L_R((REG32_L *)(_virtRegBase+CKG2_TSP_TS_SAMPLE)), CKG2_TSP_TS_SAMPLE_DISABLE));

        //Reset PE Pad
        // ts0 pad is null
        if(_bTsPadUsed[1] == TRUE)
        {
            TSP_TOP_REG(REG_TOP_TS1_PE) &= ~REG_TOP_TS_PE_MASK;
            _bTsPadUsed[1] = FALSE;
        }
        if(_bTsPadUsed[2] == TRUE)
        {
            TSP_TOP_REG(REG_TOP_TS2_PE) &= ~REG_TOP_TS_PE_MASK;
            _bTsPadUsed[2] = FALSE;
        }

    }
}

#undef CKG_TS0_TS1
    #undef CLK_TS0_DISABLE
    #undef CLK_TS0_INVERT
    #undef CLK_TS0_CLK_MASK
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
    #undef CLK_SYN_STC0_MASK
    #undef CLK_SYN_STC0_432M
    #undef CLK_STAM_DISABLE
    #undef CLK_STAM_INVERT
    #undef CLK_STAM_CLK_MASK
 #undef CKG2_TSP_TS_SAMPLE
    #undef CKG2_TSP_TS_SAMPLE_DISABLE
    #undef CKG2_TSP_TS_SAMPLE_INVERT
    #undef CKG2_TSP_TS_SAMPLE_CLK_MASK
#undef CHIP_TSP_BOOT_CLK_SEL
    #undef CHIP_TSP_BOOT_CLK_SEL_MASK


MS_U32 HAL_TSP_GetDBGPortInfo(MS_U32 u32dbgsel)
{
    _HAL_REG32_W(&_TspCtrl[0].PKT_CNT, TSP_DBG_SEL_MASK&(u32dbgsel<<TSP_DBG_SEL_SHIFT));
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
        printf("\nWarning: TSP FW not match!! FW version: 0x%08x\n\n", (unsigned int)u32FWVer);
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_TSP_SetFwDbgMem(MS_PHY phyAddr, MS_U32 u32Size)
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

//0: VQ0, 1: VQ_file
void HAL_TSP_SetVQBuffer(MS_PHY phyBaseAddr, MS_U32 u32BufLen)
{
    MS_PHY phyAddr = phyBaseAddr;
    MS_U32 u32Size = 0, u32OneBufSize = 0, u32temp = 0;
    MS_PHY phyVqBufOffset = _HAL_TSP_MIU_OFFSET(phyBaseAddr);

    u32OneBufSize = ((u32BufLen >> MIU_BUS) / TSP_VQ_NUM) << MIU_BUS; //miu alignment
    u32Size = u32OneBufSize / TSP_VQ_PITCH;

    _HAL_REG32_W(&(_TspCtrl[0].VQ0_BASE), (MS_U32)((phyAddr-phyVqBufOffset) >> MIU_BUS));
    u32temp = (u32Size << TSP_VQ0_SIZE_192PK_SHIFT) | (_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL) & ~TSP_VQ0_SIZE_192PK_MASK);
    _HAL_REG32_W(&(_TspCtrl[0].VQ0_CTRL), u32temp);

    phyAddr += u32OneBufSize;
    _HAL_REG32_W(&(_TspCtrl[0].VQ1_Base), (MS_U32)((phyAddr-phyVqBufOffset) >> MIU_BUS));
    u32temp = (u32Size << TSP_VQ1_SIZE_192BYTE_SHIFT) | (_HAL_REG32_R(&_TspCtrl[0].VQ1_Size) & ~TSP_VQ1_SIZE_192BYTE_MASK);
    _HAL_REG32_W(&(_TspCtrl[0].VQ1_Size), u32temp);
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

        _HAL_REG32_W(&_TspCtrl[0].VQ0_STATUS,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_STATUS), (TSP_VQ0_EN|TSP_VQ_FILE_EN|TSP_VQ_PINGPONG_EN)));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].VQ0_STATUS,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_STATUS), (TSP_VQ0_EN|TSP_VQ_FILE_EN|TSP_VQ_PINGPONG_EN)));
    }

}

void HAL_TSP_VQueue_Reset(void)
{
    _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL), TSP_VQ0_RESET));
    _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL), TSP_VQ0_RESET));

    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ1_Config), TSP_VQ1_RESET));
    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ1_Config), TSP_VQ1_RESET));
}

void HAL_TSP_VQueue_OverflowInt_En(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL), TSP_VQ0_OVERFLOW_INT_EN));
        _HAL_REG32_W(&_TspCtrl[0].VQ1_Config,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ1_Config), TSP_VQ1_OVF_INT_EN));

        _HAL_REG16_W(&_TspCtrl[0].SwInt_Stat1_L,
                SET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].SwInt_Stat1_L)|0xFF00 , TSP_HWINT2_VQ_OVERFLOW_STATUS >> 8));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL), TSP_VQ0_OVERFLOW_INT_EN));
        _HAL_REG32_W(&_TspCtrl[0].VQ1_Config,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].VQ1_Config), TSP_VQ1_OVF_INT_EN));

        _HAL_REG16_W(&_TspCtrl[0].SwInt_Stat1_L,
                RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].SwInt_Stat1_L) , TSP_HWINT2_VQ_OVERFLOW_STATUS >> 8));
    }
}

void HAL_TSP_VQueue_Clr_OverflowInt(void)
{
    MS_U32 u32data = _HAL_REG32_R(&_TspCtrl[0].VQ0_CTRL);

    _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL, u32data | TSP_VQ0_CLR_OVERFLOW_INT);
    _HAL_REG32_W(&_TspCtrl[0].VQ0_CTRL, u32data & ~TSP_VQ0_CLR_OVERFLOW_INT);

    u32data = _HAL_REG32_R(&_TspCtrl[0].VQ1_Config);
    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config, u32data | TSP_VQ1_CLR_OVF_INT);
    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config, u32data & ~TSP_VQ1_CLR_OVF_INT);

    _HAL_REG16_W(&_TspCtrl[0].SwInt_Stat1_L,
                RESET_FLAG1(_HAL_REG16_R(&_TspCtrl[0].SwInt_Stat1_L), TSP_HWINT2_VQ_OVERFLOW_STATUS));
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

    _HAL_REG32_W(&_TspCtrl[0].VQ0_STATUS,
        (_HAL_REG32_R(&_TspCtrl[0].VQ0_STATUS) & ~TSP_REQ_VQ_RX_THRESHOLD_MASKE) | u32Value);


    _HAL_REG32_W(&_TspCtrl[0].VQ1_Config,
        (_HAL_REG32_R(&_TspCtrl[0].VQ1_Config) & ~TSP_VQ1_WR_THRESHOLD_MASK) | u32Value);
}

MS_U32 HAL_TSP_VQ_INT_STATUS(void)
{
    return (MS_U32)(_HAL_REG16_R(&_TspCtrl[0].SwInt_Stat1_L) & TSP_HWINT2_VQ_OVERFLOW_STATUS);
}

MS_U32 HAL_TSP_Get_VQStatus(void)
{
    return (_HAL_REG32_R(&_TspCtrl[0].VQ0_STATUS) & TSP_VQ0_STATUS_MASK);
}

REG_PidFlt* HAL_TSP_GetPidFltReg(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    return (&(_TspPid[u32EngId].Flt[u32PidFltId]));
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

//  (Section DMA + PVR) , total 5 set
//  Default value of low bound is 0, default value of up bound is 0xFFFFFFFF, means no protection
//  If set both low bound and up bound to be 0, means protection all
//  MIU bus is 4, Max MIU address is 512MB, should append 1 zero
//  The range can be written:  u32StartAddr <= x < u32EndAddr
//  Protection range: x >= u32EndAddr && x < u32StartAddr
void HAL_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr)
{
    MS_U32  u32value;
    MS_U32  u32lbnd, u32ubnd;

    if (bEnable)
    {
        if(pphyStartAddr[0] == pphyEndAddr[0])
            pphyEndAddr[0] += (1UL << MIU_BUS);

        u32lbnd = (MS_U32)(((pphyStartAddr[0]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[0])) >> MIU_BUS) >> TSP_DMAW_BND_ALI_SHIFT) & TSP_DMAW_LBND_LSB8;
        u32ubnd = (MS_U32)((((pphyEndAddr[0]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[0])) >> MIU_BUS) >> TSP_DMAW_BND_ALI_SHIFT) & TSP_DMAW_LBND_LSB8) << TSP_DMAW_UBND_LSB8_SHIFT;
        u32value = u32lbnd + u32ubnd+ (_HAL_REG32_R(&_TspCtrl[0].reg16C4) & ~(TSP_DMAW_LBND_LSB8|TSP_DMAW_UBND_LSB8));
        _HAL_REG32_W(&_TspCtrl[0].reg16C4, u32value);
        u32lbnd = (MS_U32)(((pphyStartAddr[0]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[0])) >> MIU_BUS) >> TSP_DMAWP_BND_ALI_SHIFT) & TSP_DMAWP_LBND_MASK;
        u32ubnd = (MS_U32)((((pphyEndAddr[0]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[0])) >> MIU_BUS) >> TSP_DMAWP_BND_ALI_SHIFT) & TSP_DMAWP_HBND_MASK) << TSP_DMAWP_HBND_SHFT;
        u32value = u32lbnd + u32ubnd;
        _HAL_REG32_W(&_TspCtrl[0].TSP_DMAWP_BND, u32value);

        if(pphyStartAddr[1] == pphyEndAddr[1])
            pphyStartAddr[1] += (1UL << MIU_BUS);

        u32lbnd = (MS_U32)(((pphyStartAddr[1]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[1])) >> MIU_BUS) >> TSP_DMAW1_BND_ALI_SHIFT) & TSP_DMAW1_LBND_LSB8;
        u32ubnd = (MS_U32)((((pphyEndAddr[1]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[1])) >> MIU_BUS) >> TSP_DMAW1_BND_ALI_SHIFT) & TSP_DMAW1_LBND_LSB8) << TSP_DMAW1_UBND_LSB8_SHIFT;
        u32value = u32lbnd + u32ubnd+ (_HAL_REG32_R(&_TspCtrl[0].reg16C8) & ~(TSP_DMAW1_LBND_LSB8|TSP_DMAW1_UBND_LSB8));
        _HAL_REG32_W(&_TspCtrl[0].reg16C8, u32value);
        u32lbnd = (MS_U32)(((pphyStartAddr[1]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[1])) >> MIU_BUS) >> TSP_DMAW1_ALI_SHIFT) & TSP_DMAW1_LBND_MASK1;
        u32ubnd = (MS_U32)((((pphyEndAddr[1]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[1])) >> MIU_BUS) >> TSP_DMAW1_ALI_SHIFT) & TSP_DMAW1_UBND_MASK1) << TSP_DMAW1_UBND_SHIFT1;
        u32value = u32lbnd + u32ubnd;
        _HAL_REG32_W(&_TspCtrl[0].DMAW1_1, u32value);

        if(pphyStartAddr[2] == pphyEndAddr[2])
            pphyStartAddr[2] += (1UL << MIU_BUS);

        u32lbnd = (MS_U32)(((pphyStartAddr[2]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[2])) >> MIU_BUS) >> TSP_DMAW2_BND_ALI_SHIFT) & TSP_DMAW2_LBND_LSB8;
        u32ubnd = (MS_U32)((((pphyEndAddr[2]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[2])) >> MIU_BUS) >> TSP_DMAW2_BND_ALI_SHIFT) & TSP_DMAW2_LBND_LSB8) << TSP_DMAW2_UBND_LSB8_SHIFT;
        u32value = u32lbnd + u32ubnd+ (_HAL_REG32_R(&_TspCtrl[0].reg16CC) & ~(TSP_DMAW2_LBND_LSB8|TSP_DMAW2_UBND_LSB8));
        _HAL_REG32_W(&_TspCtrl[0].reg16CC, u32value);
        u32lbnd = (MS_U32)(((pphyStartAddr[2]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[2])) >> MIU_BUS) >> TSP_DMAW2_ALI_SHIFT) & TSP_DMAW2_LBND_MASK1;
        u32ubnd = (MS_U32)((((pphyEndAddr[2]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[2])) >> MIU_BUS) >> TSP_DMAW2_ALI_SHIFT) & TSP_DMAW2_UBND_MASK1) << TSP_DMAW2_UBND_SHIFT1;
        u32value = u32lbnd + u32ubnd;
        _HAL_REG32_W(&_TspCtrl[0].DMAW2_1, u32value);

        if(pphyStartAddr[3] == pphyEndAddr[3])
            pphyStartAddr[3] += (1UL << MIU_BUS);

        u32lbnd = (MS_U32)(((pphyStartAddr[3]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[3])) >> MIU_BUS) >> TSP_DMAW3_BND_ALI_SHIFT) & TSP_DMAW3_LBND_MASK;
        u32ubnd = (MS_U32)(((pphyEndAddr[3]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[3])) >> MIU_BUS) >> TSP_DMAW3_BND_ALI_SHIFT) & TSP_DMAW3_UBND_MASK;
        _HAL_REG32_W(&_TspCtrl[0].DMAW3_LBND, (_HAL_REG32_R(&_TspCtrl[0].DMAW3_LBND) & ~TSP_DMAW3_LBND_MASK) | u32lbnd);
        _HAL_REG32_W(&_TspCtrl[0].DMAW3_UBND, (_HAL_REG32_R(&_TspCtrl[0].DMAW3_UBND) & ~TSP_DMAW3_UBND_MASK) | u32ubnd);

        if(pphyStartAddr[4] == pphyEndAddr[4])
            pphyStartAddr[4] += (1UL << MIU_BUS);

        u32lbnd = (MS_U32)(((pphyStartAddr[4]-_HAL_TSP_MIU_OFFSET(pphyStartAddr[4])) >> MIU_BUS) >> TSP_DMAW4_BND_ALI_SHIFT) & TSP_DMAW4_LBND_MASK;
        u32ubnd = (MS_U32)(((pphyEndAddr[4]-_HAL_TSP_MIU_OFFSET(pphyEndAddr[4])) >> MIU_BUS) >> TSP_DMAW4_BND_ALI_SHIFT) & TSP_DMAW4_UBND_MASK;
        _HAL_REG32_W(&_TspCtrl[0].DMAW4_LBND, (_HAL_REG32_R(&_TspCtrl[0].DMAW4_LBND) & ~TSP_DMAW4_LBND_MASK) | u32lbnd);
        _HAL_REG32_W(&_TspCtrl[0].DMAW4_UBND, (_HAL_REG32_R(&_TspCtrl[0].DMAW4_UBND) & ~TSP_DMAW4_UBND_MASK) | u32ubnd);

        _HAL_REG32_W(&_TspCtrl[0].reg15b4, _HAL_REG32_R(&_TspCtrl[0].reg15b4) | TSP_DMAW_PROTECT_EN);
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].reg15b4, _HAL_REG32_R(&_TspCtrl[0].reg15b4) & ~TSP_DMAW_PROTECT_EN);
    }
}

MS_BOOL HAL_TSP_MOBF_Set_PVRKey(MS_U32 u32Key0, MS_U32 u32Key1)
{
    _HAL_REG32_W(&_TspCtrl[0].MOBF_PVR_KEY, (_HAL_REG32_R(&_TspCtrl[0].reg15b4) & ~TSP_MOBF_PVR_KEY0_MASK) | ((u32Key0 << TSP_MOBF_PVR_KEY0_SHIFT) & TSP_MOBF_PVR_KEY0_MASK));
    _HAL_REG32_W(&_TspCtrl[0].MOBF_PVR_KEY, (_HAL_REG32_R(&_TspCtrl[0].reg15b4) & ~TSP_MOBF_PVR_KEY1_MASK) | ((u32Key0 << TSP_MOBF_PVR_KEY1_SHIFT) & TSP_MOBF_PVR_KEY1_MASK));
    return TRUE;
}

MS_BOOL HAL_TSP_MOBF_Set_FileinKey(MS_U32 u32Key)
{
    MS_U32 u32Data = 0;

    u32Data = (_HAL_REG32_R(&_TspCtrl[0].Pcr64_H) & ~TSP_MOBF_FILE_KEY0_L_MASK) |
                ((u32Key << TSP_MOBF_FILE_KEY0_L_SHIFT) & TSP_MOBF_FILE_KEY0_L_MASK);
    _HAL_REG32_W(&_TspCtrl[0].Pcr64_H, u32Data);
    return TRUE;
}

MS_BOOL HAL_TSP_MOBF_Filein_Enable(MS_BOOL bEnable)
{
    return FALSE;
}

MS_BOOL HAL_TSP_MOBF_PVR_Enable(MS_BOOL bEnable)
{
    return FALSE;
}

MS_BOOL HAL_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
    if (bEnable)
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                SET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_BD_AUDIO_EN));
    }
    else
    {
        _HAL_REG32_W(&_TspCtrl[0].Hw_Config4,
                RESET_FLAG1(_HAL_REG32_R(&_TspCtrl[0].Hw_Config4), TSP_HW_BD_AUDIO_EN));
    }

    return TRUE;
}

MS_BOOL HAL_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1, MS_U32* pData)
{
    switch (u32Cmd)
    {
        default:
            return FALSE;
    }

    return TRUE;
}

MS_U8 HAL_TSP_Get_PesScmb_Sts(MS_U8 u8FltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_SCMSTS_GET | (MS_U32)(u8FltId & 0xFF));
    while (0 != _HAL_REG32_R(&_TspCtrl[0].MCU_Cmd));
    return (MS_U8)(_HAL_REG32_R(&_TspCtrl[0].MCU_Data0));
}

MS_U8 HAL_TSP_Get_TsScmb_Sts(MS_U8 u8FltId)
{
    _HAL_REG32_W(&_TspCtrl[0].MCU_Cmd, TSP_MCU_CMD_SCMSTS_GET | (MS_U32)(u8FltId & 0xFF));
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
    REG16* pReg = NULL;
    MS_U16 u16Mask = 0;
    MS_U16 u16Shft = 0;
    MS_U16 u16Cfg  = 0;

    switch (TspSrc)
    {
        case TSP_DEBUG_SRC_TS0: // TS0
            pReg = &(_TspCtrl2[0].PktCnt_TS0);
            break;
        case TSP_DEBUG_SRC_TS1: // TS1
            pReg = &(_TspCtrl2[0].PktCnt_TS1);
            break;
        case TSP_DEBUG_SRC_FILE: // FILE
            pReg = &(_TspCtrl2[0].PktCnt_File);
            break;
        default:
            return 0;
    }

    switch (TspFifo)
    {
        case TSP_DEBUG_FIFO_VIDEO: // VIDEO
            u16Mask = DISCONTI_CNT_VIDEO_MASK;
            u16Shft = DISCONTI_CNT_VIDEO_SHFT;
            u16Cfg = (u16Mode == TSP_DEBUG_MODE_DIS_CONT) ? TSP_HW_CFG_0 : TSP_HW_CFG_2;  // discontinuous packet count : drop packet count
            break;
        case TSP_DEBUG_FIFO_AUDIO: // AUDIO
            u16Mask = DISCONTI_CNT_AUDIO_MASK;
            u16Shft = DISCONTI_CNT_AUDIO_SHFT;
            u16Cfg = (u16Mode == TSP_DEBUG_MODE_DIS_CONT) ? TSP_HW_CFG_0 : TSP_HW_CFG_2;
            break;
        case TSP_DEBUG_FIFO_VIDEO3D: // VIDEO3D
            u16Mask = DISCONTI_CNT_V3D_MASK;
            u16Shft = DISCONTI_CNT_V3D_SHFT;
            u16Cfg = (u16Mode == TSP_DEBUG_MODE_DIS_CONT) ? TSP_HW_CFG_0 : TSP_HW_CFG_2;
            break;
        default:
            return 0;
    }

    _HAL_REG16_W(&(_TspCtrl2[0].HwCfg1),
        (_HAL_REG16_R(&(_TspCtrl2[0].HwCfg1)) & ~HW_INFO_SRC_MODE_MASK) | u16Cfg);

    return (MS_U32)(_HAL_REG16_R(pReg) & u16Mask) >> u16Shft;
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
        // Set the clear source
        if(TspDisconPktCntInfo->TspSrc == TSP_DEBUG_SRC_TS0)
        {
            _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
                (_HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) & ~TSP_CLR_SRC_MASK) | (TSP_CLR_SRC_PIDFLT_0<<TSP_CLR_SRC_SHIFT));
        }
        else if(TspDisconPktCntInfo->TspSrc == TSP_DEBUG_SRC_FILE)
        {
            _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
                (_HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) & ~TSP_CLR_SRC_MASK) | (TSP_CLR_SRC_PIDFLT_FILE<<TSP_CLR_SRC_SHIFT));
        }

        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) | (TSP_DISCONTI_VD_CLR | TSP_DISCONTI_V3D_CLR | TSP_DISCONTI_AUD_CLR|TSP_DISCONTI_AUDB_CLR));
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) & ~(TSP_DISCONTI_VD_CLR | TSP_DISCONTI_V3D_CLR | TSP_DISCONTI_AUD_CLR|TSP_DISCONTI_AUDB_CLR));
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
        // Set the clear source
        if(TspDropCntInfo->TspSrc == TSP_DEBUG_SRC_TS0)
        {
            _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
                (_HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) & ~TSP_CLR_SRC_MASK) | (TSP_CLR_SRC_PIDFLT_0<<TSP_CLR_SRC_SHIFT));
        }
        else if(TspDropCntInfo->TspSrc == TSP_DEBUG_SRC_FILE)
        {
            _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
                (_HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) & ~TSP_CLR_SRC_MASK) | (TSP_CLR_SRC_PIDFLT_FILE<<TSP_CLR_SRC_SHIFT));
        }

        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) | (TSP_DUP_PKT_CNT_CLR));
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl1),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl1)) & ~(TSP_DUP_PKT_CNT_CLR));
    }
    return _HAL_TSP_Get_PktCnt(TSP_DEBUG_MODE_DROP_CONT, TspDropCntInfo->TspSrc, TspDropCntInfo->TspFifo);
}

//-------------------------------------------------------------------------------
// @u16Tsif : TBD
// @ return value : 0 ~ 15
//-------------------------------------------------------------------------------
MS_U32 HAL_TSP_Get_LockPktCnt(TSP_LockPktCnt_info* TspLockCntInfo)
{
    if(TspLockCntInfo->TspCmd == TSP_DEBUG_CMD_CLEAR)
    {
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl)) | (TSP_CLR_LOCKED_PKT_CNT));
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl)) & ~(TSP_CLR_LOCKED_PKT_CNT));
    }
    else if(TspLockCntInfo->TspCmd == TSP_DEBUG_CMD_ENABLE)
    {
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl)) & ~(TSP_DIS_LOCKED_PKT_CNT));
    }
    else if(TspLockCntInfo->TspCmd == TSP_DEBUG_CMD_DISABLE)
    {
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl)) | (TSP_DIS_LOCKED_PKT_CNT));
    }

    switch (TspLockCntInfo->TspTsif)
    {
        case TSP_DEBUG_TSIF0: // TS0
            return (MS_U32)(_HAL_REG16_R(&(_TspCtrl2[0].LockPktCnt)) & TS0_LOCK_CNT_MASK) >> TS0_LOCK_CNT_SHFT;
        case TSP_DEBUG_TSIF1: // TS1
            return (MS_U32)(_HAL_REG16_R(&(_TspCtrl2[0].LockPktCnt)) & TS1_LOCK_CNT_MASK) >> TS1_LOCK_CNT_SHFT;
        default:
            return 0;
    }
}

//-------------------------------------------------------------------------------
// @ u16Fifo : TBD
// @ return value : 0 ~ 15
//-------------------------------------------------------------------------------
MS_U32 HAL_TSP_GetAVPktCnt(TSP_AVPktCnt_info* TspAVCntInfo)
{
    if(TspAVCntInfo->TspCmd == TSP_DEBUG_CMD_CLEAR)
    {
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl)) | (TSP_CLR_AV_PKT_CNT));
        _HAL_REG32_W(&(_TspCtrl[0].DbgInfo_Ctrl),
            _HAL_REG32_R(&(_TspCtrl[0].DbgInfo_Ctrl)) & ~(TSP_CLR_AV_PKT_CNT));
    }

    switch (TspAVCntInfo->TspFifo)
    {
        case TSP_DEBUG_FIFO_VIDEO: // VIDEO
            return (MS_U32)(_HAL_REG16_R(&(_TspCtrl2[0].AVPktCnt)) & VIDEO_PKT_CNT_MASK) >> VIDEO_PKT_CNT_SHFT;
        case TSP_DEBUG_FIFO_AUDIO: // AUDIO
            return (MS_U32)(_HAL_REG16_R(&(_TspCtrl2[0].AVPktCnt)) & AUDIO_PKT_CNT_MASK) >> AUDIO_PKT_CNT_SHFT;
        default:
            return 0;
    }
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

MS_BOOL HAL_TSP_SetPacketMode(MS_U32 u32TSIf, MS_U32 u32PktMode)
{
    return FALSE;
}

MS_BOOL HAL_TSP_SetMergeStrSyncByte(MS_U32 u32SrcID, MS_U8 u8SyncByte)
{
    return FALSE;
}

void HAL_TSP_MOBF_Set_PVRKey_EX(MS_U32 u32PvrEng, MS_U32 u32Key0, MS_U32 u32Key1)
{
    return;
}

