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

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT      		_u32RegBase = NULL;
static REG_CMDQCtrl        	*_CMDQCtrl = (REG_CMDQCtrl*)REG_CMDQCTRL_BASE;
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

void HAL_CMDQ_SetBank(MS_VIRT u32BankAddr)
{
    _u32RegBase = u32BankAddr;
    _CMDQCtrl = (REG_CMDQCtrl*)(_u32RegBase + REG_CMDQCTRL_BASE);

    printf("\033[35m[HAL_CMDQ_SetBank] Set Verify Code Setting...\033[m\n");
    //*(MS_U32 *)(_u32RegBase + 0x0025C) = 0x0001;  // 0x2 0025C, open this for using write_mask(SN only), this is a RIU setting, set to 1 for enable 16-bit mask/16-bit data, set to 0 for 32-bit data only(mali will failed if set to 1)
    //*(MS_U32 *)(_u32RegBase + 0x46A88) = 0x0080;  // 0x2 46A88
    *(MS_U32 *)(_u32RegBase + 0x47204UL) = 0x0000;  // 0x2 47204, Enable CMDQ wirte RIU  0x247204 >> 1 ==> 0x123902
	                                              // bank 0x1239 h0001 set bit_12 to be 0(cmdq host access RIU will be secure, so that cmdq can access secure/non-secure bank)
}

//---------------------------------------------------------------------------
///set the element of _CMDQCtrl
///  .CMDQ_Enable
///  .CMDQ_Length_ReadMode
///  .CMDQ_Mask_Setting
//---------------------------------------------------------------------------
void HAL_CMDQ_Enable()
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Enable), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Enable) | (CMDQ_CMDQ_EN));
    REG32_W((&_CMDQCtrl[0].CMDQ_Length_ReadMode), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Length_ReadMode) | (CMDQ_REQ_LEN_MASK) | (CMDQ_REQ_TH_MASK) | (REQ_DMA_BURST_MODE));
    REG32_W((&_CMDQCtrl[0].CMDQ_Mask_Setting), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Mask_Setting) | (CMDQ_MASK_BIT));

	// do not jump wait/polling_eq/polling_neq command while timer reaches, very important, set to 0
	REG32_W((&_CMDQCtrl[0].CMDQ_Tout_Base_Amount) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Tout_Base_Amount)&(TOUT_DO_NOT_JUMP));
}

void HAL_CMDQ_Stop(void)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Enable), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Enable) | (CMDQ_CMDQ_DISEN));
}

//---------------------------------------------------------------------------
///set the element of _CMDQCtrl
///  .CMDQ_En_Clk_Miu
//---------------------------------------------------------------------------
void HAL_CMDQ_Reset(void)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_En_Clk_Miu), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_En_Clk_Miu) | (CMDQ_SOFT_RSTZ));
}

MS_BOOL HAL_CMDQ_Set_Mode(MS_U32 ModeSel)
{
    if(ModeSel == 1)
    {
        REG32_W((&_CMDQCtrl[0].CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Trig_Mode) | (CMDQ_CMD_BUF_DIRECT_MODE));
        return TRUE;
    }
    else if(ModeSel == 0)
    {
        REG32_W((&_CMDQCtrl[0].CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Trig_Mode) | (CMDQ_CMD_INCREAMENT_MODE));
        return TRUE;
    }
	else if(ModeSel == 4)
	{
        REG32_W((&_CMDQCtrl[0].CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Trig_Mode) | (CMDQ_RING_BUFFER_MODE));
        return TRUE;
	}
    else
    {
        printf("\033[35mFunction = %s, Line = %d, HAL_CMDQ_Set_Mode ERROR!! Unknown mode, ModeSel = %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, ModeSel); // joe.liu
        return FALSE;
    }
}

void HAL_CMDQ_Set_Start_Pointer(MS_PHY StartAddr)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Cmd_St_Ptr), StartAddr);
    printf("\033[35mFunction = %s, Line = %d, set SATRT_ADDR: %llu\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long long)StartAddr); // joe.liu
}

void HAL_CMDQ_Set_End_Pointer(MS_PHY EndAddr)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Cmd_End_Ptr), EndAddr);
	printf("\033[35mFunction = %s, Line = %d, set END_ADDR: %llu\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned long long)EndAddr); // joe.liu
}

void HAL_CMDQ_Set_Offset_Pointer(MS_U32 OffsetAddr)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Cmd_End_Ptr) , OffsetAddr);
}

void HAL_CMDQ_Set_Timer(MS_U32 time)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Poll_Ratio_Wait_Time) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Poll_Ratio_Wait_Time) | time);
	printf("\033[35mFunction = %s, Line = %d, Set Timer: 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)time); // joe.liu
}

void HAL_CMDQ_Set_Ratio(MS_U32 Ratio)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Poll_Ratio_Wait_Time) ,_CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Poll_Ratio_Wait_Time) | Ratio);
	printf("\033[35mFunction = %s, Line = %d, Set Ratio: 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, (unsigned int)Ratio); // joe.liu
}

void HAL_CMDQ_Reset_Soft_Interrupt(void)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Sw_Trig_Cap_Sel_Irq_Clr) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Sw_Trig_Cap_Sel_Irq_Clr)|(CMDQ_SOFT_INTER_CLR));
}

void HAL_CMDQ_Reset_Start_Pointer_bit(void)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Trig_Mode) , _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Trig_Mode)|(CMDQ_RST_CMD_ST_PTR_TRIG));
}

void HAL_CMDQ_Read_Soft_Interrupt(void)
{
    _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Raw_Irq_Final_Irq);
}

void HAL_CMDQ_Read_Dec_Done(void)
{
    _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Raw_Irq_Final_Irq);
}

//---------------------------------------------------------------------------
///Trigger for update start pointer and end pointer
//---------------------------------------------------------------------------
void HAL_CMDQ_Start(void)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Trig_Mode), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Trig_Mode) | (CMDQ_MOV_CMD_PTR));
}

MS_U32 HAL_CMDQ_Read_Dummy_Register(void)
{
    MS_U32 reg_value=0;
    reg_value= _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_dummy);
    return reg_value;
}

void HAL_CMDQ_Write_Dummy_Register(MS_U32 DummyValue)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_dummy), (_CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_dummy) & (CMDQ__DUMMY_WRITE_ZERO)) | DummyValue);
}

void HAL_CMDQ_Set_MIU_SELECT(MS_U32 miu_select)
{
	if(miu_select == 1)
	{
		printf("\033[35mFunction = %s, Line = %d, set miu_1\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		REG32_W((&_CMDQCtrl[0].CMDQ_Length_ReadMode) , (_CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Length_ReadMode) | CMDQ_MIU_SELECT_MIU1));
	}
	else
	{
		printf("\033[35mFunction = %s, Line = %d, set miu_0\033[m\n", __PRETTY_FUNCTION__, __LINE__);
		REG32_W((&_CMDQCtrl[0].CMDQ_Length_ReadMode) , (_CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Length_ReadMode) & ~(CMDQ_MIU_SELECT_MIU1)));
	}
}

MS_U32 HAL_CMDQ_Read_Start_Pointer(void)
{
    MS_U32 reg_value = 0;
    reg_value= _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Cmd_St_Ptr);
    return reg_value;
}

MS_U32 HAL_CMDQ_Read_End_Pointer(void)
{
    MS_U32 reg_value = 0;
    reg_value= _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Cmd_End_Ptr);
    return reg_value;
}

//---------------------------------------------------------------------------
///Set Previous Dummy Register bit to be 1(which means this CAF is already write to DRAM)
//---------------------------------------------------------------------------
void HAL_CMDQ_Write_Dummy_Register_release_polling(MS_U32 Write_bit)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_dummy), SET_FLAG1(_CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_dummy), 0x0001 << Write_bit));
}

MS_U32 HAL_CMDQ_Error_Command(MS_U32 select_bit)
{
    MS_U32 reg_value = 0;
    REG32_W((&_CMDQCtrl[0].CMDQ_Cmd_Sel_Decode_State), select_bit);
    reg_value = _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Dma_State_Rb_Cmd) & 0x0000ffff;
    return reg_value;
}

void HAL_CMDQ_Write_Pointer(MS_PHY Write_value)
{
    REG32_W((&_CMDQCtrl[0].CMDQ_Sw_Wr_Mi_Wadr), Write_value);
}

//---------------------------------------------------------------------------
///Set rd_mi_radr_trig to grab current read address pointer(rd_mi_radr will be store in CMDQ_Rd_Mi_Radr)
//---------------------------------------------------------------------------
MS_U32 HAL_CMDQ_Read_Pointer(void)
{
    MS_U32 reg_value = 0;
	REG32_W((&_CMDQCtrl[0].CMDQ_Trig_Mode), _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Trig_Mode) | CMDQ_READ_TRIG);
	reg_value = _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Rd_Mi_Radr);
	return reg_value;
}

//---------------------------------------------------------------------------
///get the Write_Pointer(for multi-process, preventing write address is wrong(each process will start from Buffer_Start) ==> get current write_cmd ptr, next will write from here
//---------------------------------------------------------------------------
MS_U32 HAL_CMDQ_Get_Write_Pointer(void)
{
    MS_U32 reg_value = 0;
	reg_value = _CMDQ_REG32_R(&_CMDQCtrl[0].CMDQ_Sw_Wr_Mi_Wadr);
	return reg_value;
}
