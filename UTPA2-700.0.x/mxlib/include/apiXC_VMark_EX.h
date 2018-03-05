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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
/// @file  apiXC_VMark_EX.h
/// @brief VMark Ex header file
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _API_XC_VMARK_EX_H_
#define _API_XC_VMARK_EX_H_

#include "MsCommon.h"

// Driver
#include "drvXC_IOPort_EX.h"

// Api
#include "apiXC_EX.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define XC_EX_VMARK_PARAMETER_VERSION                   1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct __attribute__((packed))
{
    //versioning
    MS_U32 u32XC_VMark_version;                   ///<Version of current structure.
    MS_U32 u32XC_VMarK_Length;                   ///<Length of this structure, u32XC_VMarK_Length=sizeof(XC_VMARK_PARAMETER)
    //embedding part
    MS_U8 u8WatermarkOn;
    MS_U8 u8FrequencyDistance[3][3];
    MS_U8 u8BackgroundEmbeddingOn;
    MS_U16 u16EmbeddingStrengthThreshold[48];
    MS_U16 u16EmbeddingStrengthThresholdBG[48];
    MS_U16 u16DirectionMax;
    MS_S8 s8StrengthMultiply;
    //rendering part
    MS_U8 u8PayloadSymbols[1920];
    MS_U8 u8SymbolsRows;
    MS_U8 u8SymbolsCols;
    MS_U8 u8SymbolsXpos;
    MS_U8 u8SymbolsYpos;
    MS_U8 u8SymbolSize;
    MS_U16 u16SpacingVert;
    MS_U16 u16SpacingHorz;
    MS_U8 u8SymbolScaleControl;
}XC_EX_VMARK_PARAMETER;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef UFO_XC_VMARK
//-------------------------------------------------------------------------------------------------
/// MApi_XC_VMark_SetParameters
/// @param  u8ServiceIndex                       \b IN: Identifies uniquely a video stream inside the decoder. May not be supported if the support of a single decode is sufficient.
/// @param  pstXC_VMarkParameters           \b IN: VMark parameters for HW embedding
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC XC_EX_RETURN_VALUE MApi_XC_EX_VMark_SetParameters(XC_DEVICE_ID *pDeviceId, MS_U8 u8ServiceIndex, XC_EX_VMARK_PARAMETER *pstXC_VMarkParameters);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _API_XC_VMARK_EX_H_ */
