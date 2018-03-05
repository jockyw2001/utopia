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
// Copyright (c) 20010-20012 MStar Semiconductor, Inc.
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
// file   halMMFilein.h
// @brief  Multimedia File In (MMFILEIN) HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_MMFILEIN_H__
#define __HAL_MMFILEIN_H__

#include "regMMFilein.h"

//#define MMFI_VER_1_0                     0x0100UL
//#define MMFI_VER_2_0                     0x0200UL
//#define MMFI_VERSION                     MMFI_VER_2_0

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void   HAL_MMFI_SetBank(MS_VIRT virtBank);

//
// APIs
//
void        HAL_MMFI_PidFlt_Set(MS_U8 u8Eng, MS_U8 u8Idx, MS_U16 u16PID, MS_U32 u32entype);
void        HAL_MMFI_PidFlt_SetPid(MS_U8 u8Eng, MS_U8 u8Idx, MS_U16 u16PID);
void        HAL_MMFI_PidFlt_Enable(MS_U8 u8Eng, MS_U8 u8Idx, MS_U32 u32entype, MS_BOOL benable);
void        HAL_MMFI_PidFlt_Reset(MS_U8 u8Eng, MS_U8 u8Idx);
void        HAL_MMFI_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_PHY phyAddr);
void        HAL_MMFI_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U32 u32len);
void        HAL_MMFI_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U16 u16ctrl, MS_BOOL bEnable);
MS_U16      HAL_MMFI_Get_Filein_Ctrl(MS_U8 u8Eng);
void        HAL_MMFI_Set_FileinTimer(MS_U8 u8Eng, MS_U8 u8timer);
MS_PHY    HAL_MMFI_Get_Filein_WriteAddr(MS_U8 u8Eng);

MS_U16      HAL_MMFI_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng);
MS_BOOL     HAL_MMFI_CmdQ_FIFO_IsFull(MS_U8 u8Eng);
MS_BOOL     HAL_MMFI_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng);
MS_U16      HAL_MMFI_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng);
void        HAL_MMFI_Cfg_Enable(MS_U8 u8Eng, MS_U32 u32CfgItem, MS_BOOL benable);
void        HAL_MMFI_Cfg_Set(MS_U8 u8Eng, MS_U32 u32CfglItem);
MS_U32      HAL_MMFI_Cfg_Get(MS_U8 u8Eng);
void        HAL_MMFI_Cfg2_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL benable);
void        HAL_MMFI_Cfg2_Set(MS_U8 u8Eng, MS_U16 u16CfglItem);
MS_U16      HAL_MMFI_Cfg2_Get(MS_U8 u8Eng);
void        HAL_MMFI_Get_TsHeaderInfo(MS_U8 u8Eng, MS_U32 *pu32header);
void        HAL_MMFI_Get_APid_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_Get_APidB_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_Get_VPid_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_Get_VPid3D_Status(MS_U8 u8Eng, MS_U16 *pu16pid, MS_BOOL *pbchanged);
void        HAL_MMFI_LPcr2_Set(MS_U8 u8Eng, MS_U32 u32lpcr2);
MS_U32      HAL_MMFI_LPcr2_Get(MS_U8 u8Eng);
MS_U32      HAL_MMFI_TimeStamp_Get(MS_U8 u8Eng);
void        HAL_MMFI_PktChkSize_Set(MS_U8 u8Eng, MS_U16 u16size);
void        HAL_MMFI_MOBF_Set_FileinKey(MS_U8 u8Eng, MS_U16 u16KeyIndex);
void        HAL_MMFI_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable);
void        HAL_MMFI_AU_RemoveDupPkt(MS_U8 u8Eng, MS_BOOL bEnable);
void        HAL_MMFI_TimeStampClk_Set(MS_U8 u8Eng, MS_U32 u32ClkSrc);
//
// General API
void        HAL_MMFI_Reset(void);
void        HAL_MMFI_Reset_SubItem(MS_U16 u16RstItem);
void        HAL_MMFI_Reset_All(void);
void        HAL_MMFI_HWInt_Enable(MS_BOOL benable, MS_U16 u16init);
void        HAL_MMFI_HWInt_Clear(MS_U16 u16Int);
MS_U32      HAL_MMFI_HWInt_Status(void);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL     HAL_MMFI_SaveRegs(void);
MS_BOOL     HAL_MMFI_RestoreRegs(void);
#endif //MSOS_TYPE_LINUX_KERNEL


#endif // #ifndef __HAL_MMFILEIN_H__


