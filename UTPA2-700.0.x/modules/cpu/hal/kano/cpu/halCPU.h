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

#ifndef _HAL_CPU_H_
#define _HAL_CPU_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define CHIPREV_U01     0x00
#define CHIPREV_U02     0x01

#define BIT0            BIT(0)
#define BIT1            BIT(1)
#define BIT2            BIT(2)
#define BIT3            BIT(3)
#define BIT4            BIT(4)
#define BIT5            BIT(5)
#define BIT6            BIT(6)
#define BIT7            BIT(7)
#define BIT8            BIT(8)
#define BIT9            BIT(9)
#define BIT10           BIT(10)
#define BIT11           BIT(11)
#define BIT12           BIT(12)
#define BIT13           BIT(13)
#define BIT14           BIT(14)
#define BIT15           BIT(15)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_COPRO_RegSetBase(MS_VIRT u32Base);
MS_U8 HAL_COPRO_RegReadByte(MS_U32 u32RegAddr);
MS_U16 HAL_COPRO_RegRead2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_COPRO_RegRead4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_COPRO_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_COPRO_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_COPRO_RegWrite4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
void    HAL_COPRO_Disable(void);
void    HAL_COPRO_Enable(MS_U32 u32_ADR);
void    HAL_COPRO_Init_Front(void);
void    HAL_COPRO_Init_End(MS_U32 u32_ADR);

MS_U8   HAL_CPU_NPM_RegReadByte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_NPM_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_U16  HAL_CPU_NPM_RegRead2Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_NPM_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_U8   HAL_CPU_PM_RegReadByte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_PM_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_U16  HAL_CPU_PM_RegRead2Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_CPU_PM_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);

MS_U32  HAL_CPU_QueryClock(void);

//No Such Function in Kano
#define HAL_FRCR2_Disable(args...)
#define  HAL_FRCR2_Enable(args...)
#define  HAL_FRCR2_Init_Front(args...)
#define  HAL_FRCR2_Init_End(args...)

#endif // _HAL_CPU_H_
