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

#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_

////////////////////////////////////////////////////////////////////////////////
/// @file HALPWM.h
/// @author MStar Semiconductor Inc.
/// @brief Pulse Width Modulation hal
////////////////////////////////////////////////////////////////////////////////

#define PWM_Num     4       /* Max. 6 */
#define MAX_3DPWM_NUM    (4)
////////////////////////////////////////////////////////////////////////////////
// Macro for utility
////////////////////////////////////////////////////////////////////////////////
#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#define UNUSED( var )       ((void)(var))

////////////////////////////////////////////////////////////////////////////////
// Macro for bitwise
////////////////////////////////////////////////////////////////////////////////
#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x) _BITMASK(1?x, 0?x)
#define BITFLAG(loc) (1U << (loc))

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

#define SETBIT(REG, BIT)   ((REG) |= (1UL << (BIT)))
#define CLRBIT(REG, BIT)   ((REG) &= ~(1UL << (BIT)))
#define GETBIT(REG, BIT)   (((REG) >> (BIT)) & 0x01UL)
#define COMPLEMENT(a)      (~(a))

typedef struct _PWM_3D_RegisterInfo
{
    MS_U32 u32RegOffset;
    MS_U8 u8NumBit;
} PWM_3D_RegisterInfo;

typedef struct _PWM_3D_RegisterOffset
{
    PWM_3D_RegisterInfo regShift;
    PWM_3D_RegisterInfo regDuty;
} PWM_3D_RegisterOffset;

////////////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_PWM_ReadByte(MS_U32 u32RegAddr);
MS_U16 HAL_PWM_Read2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_PWM_Read4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_PWM_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_PWM_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_PWM_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_PWM_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
void HAL_PWM_SetIOMapBase(MS_VIRT virtBase, MS_VIRT virtBase1);
void HAL_PWM_SetChipTopIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_PWM_GetIOMapBase(void);
MS_BOOL HAL_PWM_Init(void);
MS_BOOL HAL_PWM_Oen(PWM_ChNum index, MS_BOOL letch);
MS_BOOL HAL_PWM_GetOen(PWM_ChNum index);
MS_BOOL HAL_PWM_UnitDiv(MS_U16 u16DivPWM);
void HAL_PWM_Period(PWM_ChNum index, MS_U32 u32PeriodPWM);
void HAL_PWM_DutyCycle(PWM_ChNum index, MS_U32 u32DutyPWM);
void HAL_PWM_Div(PWM_ChNum index, MS_U16 u16DivPWM);
MS_U16 HAL_PWM_GetDiv(PWM_ChNum index);
void HAL_PWM_Polarity(PWM_ChNum index, MS_BOOL bPolPWM);
MS_BOOL HAL_PWM_GetPolarity(PWM_ChNum index);
void HAL_PWM_VDBen(PWM_ChNum index, MS_BOOL bVdbenPWM);
MS_BOOL HAL_PWM_GetVDBen(PWM_ChNum index);
void HAL_PWM_Vrest(PWM_ChNum index, MS_BOOL bRstPWM);
MS_BOOL HAL_PWM_GetVrest(PWM_ChNum index);
void HAL_PWM_DBen(PWM_ChNum index, MS_BOOL bdbenPWM);
MS_BOOL HAL_PWM_GetDBen(PWM_ChNum index);
void HAL_PWM_RstMux(PWM_ChNum index, MS_BOOL bMuxPWM);
void HAL_PWM_RstCnt(PWM_ChNum index, MS_U8 u8RstCntPWM);
void HAL_PWM_BypassUnit(PWM_ChNum index, MS_BOOL bBypassPWM);
MS_BOOL HAL_PWM01_CntMode(PWM_CntMode CntMode);
MS_BOOL HAL_PWM23_CntMode(PWM_CntMode CntMode);
MS_BOOL HAL_PWM67_CntMode(PWM_CntMode CntMode);
MS_BOOL HAL_PWM_Shift(PWM_ChNum index, MS_U32 u32ShiftPWM);
MS_U32 HAL_PWM_GetShift(PWM_ChNum index);
void HAL_PWM_IMPULSE_EN(PWM_ChNum index, MS_BOOL bdbenPWM);
void HAL_PWM_ODDEVEN_SYNC(PWM_ChNum index, MS_BOOL bdbenPWM);
void HAL_PWM_Nvsync(PWM_ChNum index, MS_BOOL bNvsPWM);
void HAL_PWM_Align(PWM_ChNum index, MS_BOOL bAliPWM);
MS_BOOL HAL_PWM_IsSupport3D(PWM_ChNum index);
void HAL_PWM_SetMultiDiff(MS_BOOL bEnable);
MS_BOOL HAL_PWM_Set3D_DiffWaveform(PWM_ChNum index, MS_U8 u8WaveformIndex, MS_U32 u32Shift, MS_U32 u32Duty);
void HAL_PM_PWM_Enable(void);
void HAL_PM_PWM_Period(MS_U16 u16PeriodPWM);
void HAL_PM_PWM_DutyCycle(MS_U16 u16DutyPWM);
void HAL_PM_PWM_Div(MS_U8 u8DivPWM);
void HAL_PM_PWM_Polarity(MS_BOOL bPolPWM);
void HAL_PM_PWM_DBen(MS_BOOL bdbenPWM);
MS_U32 HAL_PWM_GetPeriod(PWM_ChNum index);
MS_U32 HAL_PWM_GetDutyCycle(PWM_ChNum index);
MS_BOOL HAL_PWM_INV_3D_Flag(MS_BOOL bInvPWM);
void HAL_PWM_LR_RST_SEL(PWM_ChNum index, N_LR_SYNC_SEL eLR_Sync);
#endif // _HAL_PWM_H_

