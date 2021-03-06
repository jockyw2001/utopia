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
// Copyright (c) 2006-20012 MStar Semiconductor, Inc.
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
// file   halCMDQ.h
// @brief  CMDQ HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_CMDQ_H__
#define __HAL_CMDQ_H__

#include "regCMDQ.h"

// Trigger Bus Defination
#define HAL_GE_GE2CMDQ_TRI                  0xFFFF
#define HAL_BDMA_INT_BDMA_0                 0xFFFF
#define HAL_BDMA_INT_BDMA_1                 0xFFFF
#define HAL_GPU_GPU2CMDQ_TRIG               0xFFFF
#define HAL_GOPG0_GOP_CMDQ_INT_0            0xFFFF
#define HAL_GOPG1_GOP_CMDQ_INT_1            0xFFFF
#define HAL_GOPG2_GOP_CMDQ_INT_2            0xFFFF
#define HAL_GOPG3_GOP_CMDQ_INT_3            0xFFFF
#define HAL_SCALAR_SC2CMDQ_TRIG_0           0xFFFF
#define HAL_SCALAR_SC2CMDQ_TRIG_1           0xFFFF
#define HAL_SCALAR_SC2CMDQ_TRIG_2           0xFFFF
#define HAL_SCALAR_SC2CMDQ_TRIG_3           0xFFFF
#define HAL_AESDMA_CMDQDMA_FINISHED         0xFFFF
#define HAL_AESDMA_CMDQDMA_FINISHED_2       0xFFFF
#define HAL_VOP_DCM2GOP_VDE                 0xFFFF
#define HAL_VOP_DCS2GOP_VDE                 0xFFFF
#define HAL_XC_DIP_CMDQ_INT                 0xFFFF


#define BUS_SHIFT 4

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))
////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_CMDQ_SetBank(MS_VIRT u32BankAddr);
void HAL_CMDQ_Enable(void);
void HAL_CMDQ_Stop(void);
void HAL_CMDQ_Reset(void);
void HAL_CMDQ_Set_Miu_Length(MS_U32 value);
void HAL_CMDQ_Set_Miu_Request(MS_U32 value);
MS_BOOL HAL_CMDQ_Set_Mode(MS_U32 ModeSel);
void HAL_CMDQ_Set_Start_Pointer(MS_PHY StartAddr);
void HAL_CMDQ_Set_End_Pointer(MS_PHY EndAddr);
void HAL_CMDQ_Set_Offset_Pointer(MS_U32 OffsetAddr);
void HAL_CMDQ_Set_Timer(MS_U32 time);
void HAL_CMDQ_Set_Ratio(MS_U32 ratio);
void HAL_CMDQ_Reset_Soft_Interrupt(void);
void HAL_CMDQ_Reset_Start_Pointer_bit(void);
void HAL_CMDQ_Read_Soft_Interrupt(void);
void HAL_CMDQ_Read_Dec_Done(void);
void HAL_CMDQ_Start(void);
MS_U32 HAL_CMDQ_Read_Dummy_Register(void);
void HAL_CMDQ_Write_Dummy_Register(MS_U32 DummyValue);
void HAL_CMDQ_Set_MIU_SELECT(MS_U32 miu_select);
MS_U32 HAL_CMDQ_Read_Start_Pointer(void);
MS_U32 HAL_CMDQ_Read_End_Pointer(void);
void HAL_CMDQ_Write_Dummy_Register_release_polling(MS_U32 Write_bit);
MS_U32 HAL_CMDQ_Error_Command(MS_U32 select_bit);
void HAL_CMDQ_Write_Pointer(MS_PHY Write_value);
MS_U32 HAL_CMDQ_Read_Pointer(void);
MS_U32 HAL_CMDQ_Get_Write_Pointer(void);



#endif // #ifndef _CMDQ_REG_MCU_H_
