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

////////////////////////////////////////////////////////////////////////////////
///
/// @file  drvmvd_cc.h
/// @brief MPEG-2/4 Video Decoder header file for Closed Caption
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef DRV_MVD_CC_H
#define DRV_MVD_CC_H

////////////////////////////////////////////////////////////////////////////////
// Include List
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
/* MVD INFO */
typedef enum
{
    CC_SELECTOR_708_SW              = 0x0000,
    CC_SELECTOR_CCMVD_RINGBUFFER    = 0x0001

} EN_CC_MVD_INFO;

typedef enum
{
    E_CC_MVD_TYPE_708 = 0x0,
    E_CC_MVD_TYPE_608 = 0x1,
    E_CC_MVD_TYOE_NONE = 0xFF
}EN_CC_MVD_TYPE;


////////////////////////////////////////////////////////////////////////////////
// Type & Structure Declaration
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Function Prototype Declaration
////////////////////////////////////////////////////////////////////////////////
void MDrv_CC_Init(MS_U32 u32Id);
void MDrv_CC_CM_SetMVDRB_HWAddr(MS_U32 u32Id, MS_U32 u32StartAddress, MS_BOOL u8CC608);
void MDrv_CC_CM_SetParsingType(MS_U32 u32Id, MS_U8 u8Operation, MS_U16 u16BufferSize, MS_BOOL u8CC608);
void MDrv_CC_CM_DisableParsing(MS_U32 u32Id, MS_BOOL u8CC608);
MS_U32 MDrv_CC_CM_GetMVDRB_HWAddr(MS_U32 u32Id, MS_BOOL u8CC608);
//void MDrv_CC_CM_ResumeParsingType(void);
//MS_U8 MDrv_CC_CM_GetMVDRB_WrapCount(MS_BOOL u8CC608);
void MDrv_CC_CM_SyncMVDRB_SWAddr2HWAddr(MS_U32 u32Id, MS_BOOL u8CC608);

/* Polling Mode */
MS_U8 MDrv_CC_CM_GetOverflowStatus(MS_U32 u32Id, MS_BOOL u8CC608);
void MDrv_CC_CM_SetMVDRB_SWAddr(MS_U32 u32Id, MS_U32 u32ReadAddress, MS_BOOL u8CC608);
MS_U32 MDrv_CC_PM_GetMVDRB_WriteAddr(MS_U32 u32Id, MS_BOOL u8CC608);
MS_U32 MDrv_CC_PM_GetMVDRB_ReadAddr(MS_U32 u32Id, MS_BOOL u8CC608);
void MDrv_CC_PM_SetMVDRB_ReadAddr(MS_U32 u32Id, MS_U32 u32EachPacketSize, MS_BOOL u8CC608);
MS_BOOL MDrv_CC_CM_GetMVDDecodeStatus(MS_U32 u32Id);

void MDrv_CC_CM_GetInfo(MS_U32 u32Id, MS_U32 selector, MS_U8 type, MS_U32 *p1, MS_U32 *p2);
MS_BOOL MDrv_CC_CM_Get708Sw(MS_U32 u32Id);
MS_BOOL MDrv_CC_CM_IsMvdRstDone(MS_U32 u32Id, MS_U8 type);
MS_BOOL MDrv_CC_InfoEnhanceMode(MS_U32 u32Id,MS_BOOL bEnable);
#endif
#ifdef __cplusplus
}
#endif

#endif

