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

#ifndef _HAL_HVD_SUB_H_
#define _HAL_HVD_SUB_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Feature
/////#define HAL_HVD_SUB_ENABLE_VPU_LIB    1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_HVD_Sub_EnableISR(MS_BOOL bEnable);
void HAL_HVD_Sub_CheckMIUSel( MS_BOOL bChange);
MS_U32 HAL_HVD_Sub_Get_HWVersionID(void);
void HAL_HVD_Sub_PowerCtrl(MS_BOOL bEnable);
void HAL_HVD_Sub_InitRegBase(MS_U32 u32RegBase);
HVD_Return HAL_HVD_Sub_InitVariables( MS_U32 drvctrl  );
HVD_Return HAL_HVD_Sub_InitShareMem( void);
HVD_Return HAL_HVD_Sub_InitRegCPU( void);
HVD_Return HAL_HVD_Sub_SetData( HVD_SetData u32type , MS_U32 u32Data);
MS_U32 HAL_HVD_Sub_GetData( HVD_GetData eType );
HVD_Return HAL_HVD_Sub_SetCmd( HVD_User_Cmd u32Cmd , MS_U32 u32CmdArg);
HVD_Return HAL_HVD_Sub_DeInit(void);
HVD_Return HAL_HVD_Sub_PushPacket(HVD_BBU_Info* pInfo  );
void HAL_HVD_Sub_Enable_ISR(MS_BOOL bEnable);
void HAL_HVD_Sub_SetForceISR(MS_BOOL bEnable);
void HAL_HVD_Sub_SetClearISR(void);
MS_BOOL HAL_HVD_Sub_IsISROccured(void);
MS_BOOL HAL_HVD_Sub_IsEnableISR(void);
MS_BOOL HAL_HVD_Sub_RstPTSCtrlVariable(void);
MS_BOOL HAL_HVD_Sub_FlushRstShareMem(void);
MS_BOOL HAL_Sub_IsBBUEntryOdd(void);
void HAL_HVD_Sub_UartSwitch2FW( MS_BOOL bEnable );

MS_U32 HAL_HVD_Sub_GetData_Dbg( MS_U32 u32Addr );
void HAL_HVD_Sub_SetData_Dbg( MS_U32 u32Addr , MS_U32 u32Data);
MS_U16 HAL_HVD_Sub_GetCorretClock(MS_U16 u16Clock);
void HAL_HVD_Sub_UpdateESWptr_Fire(void);
void HAL_HVD_Sub_MVD_PowerCtrl(MS_BOOL bEnable);
void HAL_HVD_Sub_Dump_FW_Status(void);
void HAL_HVD_Sub_Dump_HW_Status(MS_U32 numb);

//???
void HAL_HVD_Sub_GetBBUEntry( MS_U32 u32Idx , MS_U32* u32NalOffset , MS_U32* u32NalSize  );
void HAL_HVD_Sub_Dump_BBUs(  MS_U32 u32StartIdx, MS_U32 u32EndIdx, MS_BOOL bShowEmptyEntry );


#endif // _HAL_HVD_SUB_H_

