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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
// file   halCMDQ.c
// @brief  CMDQ HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "regCMDQ.h"
#include "halCMDQ.h"

#include "ULog.h"
#define TAG_HAL_CMDQ "HAL_CMDQ"
//--------------------------------------------------------------------------------------------------
//  Utility Functions
//--------------------------------------------------------------------------------------------------
#if (defined ANDROID) && (defined TV_OS)
#include <cutils/log.h>
#define printf LOGD
#ifndef LOGD // android 4.1 rename LOGx to ALOGx
#define LOGD ALOGD
#endif
#if 1
#define _msg(fmt, ...)
#define _dbg(fmt, ...)
#define _err(fmt, ...)
#define _cri(fmt, ...)
#else
#define _msg(fmt, ...) do { if(_u32CMDQDBGLevel) LOGD(fmt, ##__VA_ARGS__); } while(0)
#define _dbg LOGD
#define _err LOGD
#define _cri LOGD
#endif

#elif defined (MSOS_TYPE_OPTEE)
#define _msg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define _dbg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define _err(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define _cri(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
static void _log(int lv, const char *fn, int ln, char *fmt, ...) {
    va_list ap;
    char *tag[] = {"DBG", "ERR", "WTF"};
    int i;
    i = (lv==2)?3:1;
    va_start(ap, fmt);

    do {
        if(lv!=0) ULOGE(TAG_HAL_CMDQ, "\033[35m");
        ULOGE(TAG_HAL_CMDQ, "[cmdq][%s][%s:%d] ", tag[lv], fn, ln);
        if(lv!=0) ULOGE(TAG_HAL_CMDQ, "\033[m");
        vprintf(fmt, ap);
    } while(--i);
    va_end(ap);
}
#define _dbg(fmt, ...) _log(0, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define _err(fmt, ...) _log(1, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define _cri(fmt, ...) _log(2, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#endif

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT                  _u32RegBase[NUMBER_OF_CMDQ_HW] = { NULL, NULL };
static REG_CMDQCtrl             *_CMDQCtrl[NUMBER_OF_CMDQ_HW] = { (REG_CMDQCtrl*)REG_CMDQCTRL_BASE,
                                                                  (REG_CMDQCtrl*)REG_CMDQCTRL_BASE2 };

#define REG32_W(reg, value)     do {    \
                                    (reg)->H = ((value) >> 16); \
                                    (reg)->L = ((value) & 0x0000FFFFUL);  \
                                } while(0)

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------
///concatenate (reg)->H and (reg)->L
/// @param  reg                     \b IN: concatenate data
//---------------------------------------------------------------------------
MS_U32 _CMDQ_REG32_R(REG32 *reg)
{
    MS_U32     value;
    value = (reg)->H << 16;
    value |= (reg)->L;
    return value;
}

//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------

void HAL_CMDQ_SetBank(MS_U32 hnd, MS_VIRT u32BankAddr)
{
    _u32RegBase[hnd] = u32BankAddr;
    if(hnd == 0)
    {
        _CMDQCtrl[hnd] = (REG_CMDQCtrl*)(_u32RegBase[hnd] + REG_CMDQCTRL_BASE);
    }
    else if (hnd == 1)
    {
        _CMDQCtrl[hnd] = (REG_CMDQCtrl*)(_u32RegBase[hnd] + REG_CMDQCTRL_BASE2);
    }
    else
    {
        _cri("INVALID HANDLE! %d\n", hnd);
    }

    _dbg("\033[35m[HAL_CMDQ_SetBank] Set Verify Code Setting...\033[m\n");
    *(MS_U32 *)(_u32RegBase[hnd] + 0x0025C) = 0x0001;  // 0x2 0025C, open this for using write_mask(SN only), this is a RIU setting, set to 1 for enable 16-bit mask/16-bit data, set to 0 for 32-bit data only(mali will failed if set to 1)
    //*(MS_U32 *)(_u32RegBase[hnd] + 0x46A88) = 0x0080;  // 0x2 46A88
    *(MS_U32 *)(_u32RegBase[hnd] + 0x47204UL) = 0x0000;  // 0x2 47204, Enable CMDQ wirte RIU  0x247204 >> 1 ==> 0x123902
                                                  // bank 0x1239 h0001 set bit_12 to be 0(cmdq host access RIU will be secure, so that cmdq can access secure/non-secure bank)
#if (NUMBER_OF_CMDQ_HW == 2)
    *(MS_U32 *)(_u32RegBase[hnd] + 0x46848UL) = 0x0008;
#endif
}

//---------------------------------------------------------------------------
///set the element of _CMDQCtrl
///  .CMDQ_Enable
///  .CMDQ_Length_ReadMode
///  .CMDQ_Mask_Setting
//---------------------------------------------------------------------------
void HAL_CMDQ_Enable(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Enable), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Enable) | (CMDQ_CMDQ_EN));
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Length_ReadMode), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Length_ReadMode) | (CMDQ_REQ_LEN_MASK) | (CMDQ_REQ_TH_MASK) | (REQ_DMA_BURST_MODE));
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Mask_Setting), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Mask_Setting) | (CMDQ_MASK_BIT));

    // do not jump wait/polling_eq/polling_neq command while timer reaches, very important, set to 0
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Tout_Base_Amount) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Tout_Base_Amount)&(TOUT_DO_NOT_JUMP));
}

void HAL_CMDQ_Stop(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Enable), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Enable) | (CMDQ_CMDQ_DISEN));
}

//---------------------------------------------------------------------------
///set the element of _CMDQCtrl
///  .CMDQ_En_Clk_Miu
//---------------------------------------------------------------------------
void HAL_CMDQ_Reset(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_En_Clk_Miu), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_En_Clk_Miu) | (CMDQ_SOFT_RSTZ));
}

MS_BOOL HAL_CMDQ_Set_Mode(MS_U32 hnd, MS_U32 ModeSel)
{
    if(ModeSel == 1)
    {
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) | (CMDQ_CMD_BUF_DIRECT_MODE));
        return TRUE;
    }
    else if(ModeSel == 0)
    {
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) | (CMDQ_CMD_INCREAMENT_MODE));
        return TRUE;
    }
    else if(ModeSel == 4)
    {
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) | (CMDQ_RING_BUFFER_MODE));
        return TRUE;
    }
    else
    {
        _err("\033[35mHAL_CMDQ_Set_Mode ERROR!! Unknown mode, ModeSel = %d\033[m\n", ModeSel); // joe.liu
        return FALSE;
    }
}

void HAL_CMDQ_Set_Start_Pointer(MS_U32 hnd, MS_PHY StartAddr)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Cmd_St_Ptr), StartAddr);
    _dbg("\033[35mset SATRT_ADDR: %llu\033[m\n", (unsigned long long)StartAddr); // joe.liu
}

void HAL_CMDQ_Set_End_Pointer(MS_U32 hnd, MS_PHY EndAddr)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Cmd_End_Ptr), EndAddr);
    _dbg("\033[35mset END_ADDR: %llu\033[m\n", (unsigned long long)EndAddr); // joe.liu
}

void HAL_CMDQ_Set_Offset_Pointer(MS_U32 hnd, MS_U32 OffsetAddr)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Cmd_End_Ptr) , OffsetAddr);
}

void HAL_CMDQ_Set_Timer(MS_U32 hnd, MS_U32 time)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Poll_Ratio_Wait_Time) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Poll_Ratio_Wait_Time) | time);
    _dbg("\033[35mSet Timer: 0x%X\033[m\n", (unsigned int)time); // joe.liu
}

void HAL_CMDQ_Set_Ratio(MS_U32 hnd, MS_U32 Ratio)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Poll_Ratio_Wait_Time) ,_CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Poll_Ratio_Wait_Time) | Ratio);
    _dbg("\033[35mSet Ratio: 0x%X\033[m\n", (unsigned int)Ratio); // joe.liu
}

void HAL_CMDQ_Reset_Soft_Interrupt(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Sw_Trig_Cap_Sel_Irq_Clr) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Sw_Trig_Cap_Sel_Irq_Clr)|(CMDQ_SOFT_INTER_CLR));
}

void HAL_CMDQ_Reset_Start_Pointer_bit(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Trig_Mode)|(CMDQ_RST_CMD_ST_PTR_TRIG));
}

void HAL_CMDQ_Read_Soft_Interrupt(MS_U32 hnd)
{
    _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Raw_Irq_Final_Irq);
}

void HAL_CMDQ_Read_Dec_Done(MS_U32 hnd)
{
    _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Raw_Irq_Final_Irq);
}

//---------------------------------------------------------------------------
///Trigger for update start pointer and end pointer
//---------------------------------------------------------------------------
void HAL_CMDQ_Start(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Trig_Mode), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) | (CMDQ_MOV_CMD_PTR));
}

MS_U32 HAL_CMDQ_Read_Dummy_Register(MS_U32 hnd)
{
    MS_U32 reg_value=0;
    reg_value= _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_dummy);
    return reg_value;
}

void HAL_CMDQ_Write_Dummy_Register(MS_U32 hnd, MS_U32 DummyValue)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_dummy), (_CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_dummy) & (CMDQ__DUMMY_WRITE_ZERO)) | DummyValue);
}

void HAL_CMDQ_Set_MIU_SELECT(MS_U32 hnd, MS_U32 miu_select)
{
    if(miu_select == 1)
    {
        _dbg("\033[35mset miu_1\033[m\n");
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_Length_ReadMode) , (_CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Length_ReadMode) | CMDQ_MIU_SELECT_MIU1));
    }
    else
    {
        _dbg("\033[35mset miu_0\033[m\n");
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_Length_ReadMode) , (_CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Length_ReadMode) & ~(CMDQ_MIU_SELECT_MIU1)));
    }
}

MS_U32 HAL_CMDQ_Read_Start_Pointer(MS_U32 hnd)
{
    MS_U32 reg_value = 0;
    reg_value= _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Cmd_St_Ptr);
    return reg_value;
}

MS_U32 HAL_CMDQ_Read_End_Pointer(MS_U32 hnd)
{
    MS_U32 reg_value = 0;
    reg_value= _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Cmd_End_Ptr);
    return reg_value;
}

//---------------------------------------------------------------------------
///Set Previous Dummy Register bit to be 1(which means this CAF is already write to DRAM)
//---------------------------------------------------------------------------
void HAL_CMDQ_Write_Dummy_Register_release_polling(MS_U32 hnd, MS_U32 Write_bit)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_dummy), SET_FLAG1(_CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_dummy), 0x0001 << Write_bit));
}

MS_U32 HAL_CMDQ_Error_Command(MS_U32 hnd, MS_U32 select_bit)
{
    MS_U32 reg_value = 0;
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Cmd_Sel_Decode_State), select_bit);
    reg_value = _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Dma_State_Rb_Cmd) & 0x0000ffff;
    return reg_value;
}

void HAL_CMDQ_Write_Pointer(MS_U32 hnd, MS_PHY Write_value)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Sw_Wr_Mi_Wadr), Write_value);
}

//---------------------------------------------------------------------------
///Set rd_mi_radr_trig to grab current read address pointer(rd_mi_radr will be store in CMDQ_Rd_Mi_Radr)
//---------------------------------------------------------------------------
MS_U32 HAL_CMDQ_Read_Pointer(MS_U32 hnd)
{
    MS_U32 reg_value = 0;
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Trig_Mode), _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Trig_Mode) | CMDQ_READ_TRIG);
    reg_value = _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Rd_Mi_Radr);
    return reg_value;
}

//---------------------------------------------------------------------------
///get the Write_Pointer(for multi-process, preventing write address is wrong(each process will start from Buffer_Start) ==> get current write_cmd ptr, next will write from here
//---------------------------------------------------------------------------
MS_U32 HAL_CMDQ_Get_Write_Pointer(MS_U32 hnd)
{
    MS_U32 reg_value = 0;
    reg_value = _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Sw_Wr_Mi_Wadr);
    return reg_value;
}

//---------------------------------------------------------------------------
///Force trigger signal on wait bus. (good for debug), current sigbits 0-15
//---------------------------------------------------------------------------
void HAL_CMDQ_Trigger_Wait(MS_U32 hnd, MS_U32 sigbits)
{
    MS_U32 v;
    v = _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Wait_Trig)&0xFFFF0000;
    v = v|(sigbits&0x0000FFFF);

    //@FIXME, do we need skip mask here. or add individual function to set it.
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Wait_Trig),v);
    //@FIXME, do we need to recover skip mask bit, if we set above 2 line.
}

//---------------------------------------------------------------------------
///Skip WR[0] WAIT[1], POLLEQ[2], POLLNEQ[3] commands. EX. 0xF skip those 4 cmds.
//---------------------------------------------------------------------------
void HAL_CMDQ_Skip_Commands(MS_U32 hnd, MS_U32 skipbits)
{
    MS_U32 v;
    //@FIXME, r0a0, active low, does it really active low???

    //we also set skip mask bit, to force skip even mask is not set.
    v = _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_Force_Skip);
    if(skipbits & 0x01)
        v |= 0x00000011;
    else if(skipbits & 0x02)
        v |= 0x00000022;
    else if(skipbits & 0x03)
        v |= 0x00000044;
    else if(skipbits & 0x04)
        v |= 0x00000088;

    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Force_Skip),v);
}

//---------------------------------------------------------------------------
///Real CMDQ reset
//---------------------------------------------------------------------------
void HAL_CMDQ_Reset2(MS_U32 hnd, MS_BOOL high)
{
    MS_U32 v;
    v = _CMDQ_REG32_R(&_CMDQCtrl[hnd]->CMDQ_En_Clk_Miu);
    if(high)
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_En_Clk_Miu), v | (CMDQ_SOFT_RSTZ_BIT));
    else {
        REG32_W((&_CMDQCtrl[hnd]->CMDQ_En_Clk_Miu), v & (~CMDQ_SOFT_RSTZ_BIT));
    }

}

//---------------------------------------------------------------------------
///Read misc status bits.
//---------------------------------------------------------------------------
MS_U32 HAL_CMDQ_Read_Misc_Status(MS_U32 hnd)
{
    return _CMDQ_REG32_R((&_CMDQCtrl[hnd]->CMDQ_Cmd_Ptr_Vld));
}

void HAL_CMDQ_Set_Debug_Step_Mode(MS_U32 hnd, MS_BOOL on)
{
    MS_U32 r;
    r = _CMDQ_REG32_R((&_CMDQCtrl[hnd]->CMDQ_Cmd_Mode_Enable));
    if(on)
        r |= (CMDQ_DEBUG_MODE_ENABLE|CMDQ_DEBUG_ONESTEP_ENABLE);
    else
        r &= ~(CMDQ_DEBUG_MODE_ENABLE|CMDQ_DEBUG_ONESTEP_ENABLE);
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_Cmd_Mode_Enable), r);
}

MS_U32 HAL_CMDQ_Get_Debug_Step_Mode(MS_U32 hnd)
{
    return _CMDQ_REG32_R((&_CMDQCtrl[hnd]->CMDQ_Cmd_Mode_Enable));
}

//---------------------------------------------------------------------------
///Trigger one step when debug-mode, step-mode enabled.
//---------------------------------------------------------------------------
void HAL_CMDQ_Debug_One_Step(MS_U32 hnd)
{
    REG32_W((&_CMDQCtrl[hnd]->CMDQ_One_Step_Trig),
            _CMDQ_REG32_R((&_CMDQCtrl[hnd]->CMDQ_One_Step_Trig))|CMDQ_DEBUG_ONESTEP);
}
