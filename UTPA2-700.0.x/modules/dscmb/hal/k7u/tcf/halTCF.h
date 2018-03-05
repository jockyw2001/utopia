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
// file   halTCF.h
// @brief  TCF HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "regTCF.h"

#ifndef __HAL_TCF_H__
#define __HAL_TCF_H__


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#undef TCF_CFKE

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
typedef struct      // MSTAR_CF_VERSION //
{
    MS_U8                         _u8VersionEpoch;
    MS_U8                         _u8ManufacturerId;
    MS_U8                         _u8NetlistVersion;
    MS_U8                         _u8VersionBuildId;
} HAL_CF_VERSION_INFO;

typedef struct      // MSTAR_CF_FEATURE //
{
    MS_U32                        _u32FeatureVector;
} HAL_CF_FEATURE_VECTOR;

typedef struct      // MSTAR_CF_TRANS_STATUS //
{
    MS_U8                         _eTransStatus;
    MS_U8                         _u8UseNvmKey;
    MS_U8                         _eOperationType;
    MS_U8                         _eDecmSrc;
    MS_U8                         _eOutputUsage;
    MS_U8                         _u8ProductRange;
    MS_U8                         _u8ProductOffset;
} HAL_CF_TRANS_STATUS;

typedef struct      // MSTAR_CF_CF_STATUS //
{
    MS_U8                         _eNvmStatus;
    MS_U8                         _eDiffStatus;
    MS_U8                         _u8RecentReset;
    MS_U8                         _u8CfAlert;
    MS_U8                         _u8DevelopmentMode;
    MS_U8                         _u8FuseActivate;
    MS_U8                         _u8FuseBlock;
} HAL_CF_CF_STATUS;



typedef struct
{
    MS_U8                         _u8CwcPid;
    MS_U8                         _u8CwcFld;
    MS_U8                         _u8CwcFscb;
    MS_U8                         _u8CwcScb;
    MS_U8                         _u8CwcTsid;
    MS_U8                         _u8CwcDst;
} HAL_CFB_CWCCFG;

typedef enum
{
    E_CFB_DST_KT,
    E_CFB_DST_DMA_SK0,
    E_CFB_DST_DMA_SK1,
    E_CFB_DST_DMA_SK2,
    E_CFB_DST_DMA_SK3,
    E_CFB_DST_TSIO,

    E_HAL_CFB_DST_KT                 =0x0000,
    E_HAL_CFB_DST_DMA_SK0   =0x0001,
    E_HAL_CFB_DST_DMA_SK1   =0x0101,
    E_HAL_CFB_DST_DMA_SK2   =0x0201,
    E_HAL_CFB_DST_DMA_SK3   =0x0301,
    E_HAL_CFB_DST_TSIO            =0x0002,
} HAL_CFB_CWC_DST;

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_CF_SetBank(MS_VIRT u32NonPmBankAddr);
void HAL_CF_Clk(MS_BOOL bEnable);

void HAL_CF_Version_Info(HAL_CF_VERSION_INFO *_pstVersion);
void HAL_CF_Trans_Status(HAL_CF_TRANS_STATUS *_pstTransStatus);
void HAL_CF_Cf_Status(HAL_CF_CF_STATUS *_pstCfStatus);
void HAL_CF_Cf_FeatureVector(HAL_CF_FEATURE_VECTOR *_pstCfFeature);

MS_U8 HAL_CF_Get_Trans_Status(void);
MS_U8 HAL_CF_Get_Operation_Type(void);
MS_U8 HAL_CF_Get_CwcValid(void);
MS_U32 HAL_CF_Read_Input(void);
MS_U32 HAL_CF_Read_Output(void);
void HAL_CF_Write_Input(MS_U32 u32Cmd);
void HAL_CF_Write_Output(MS_U32 u32Cmd);
MS_U8 HAL_CF_Get_CF_IsFinished(void);


MS_U8 HAL_CFB_Init(void);
MS_U8 HAL_CFB_Enable(void);
MS_U8 HAL_CFB_Reset(void);

void HAL_CFB_Set_Use_Case(MS_U8 u8UseCase);
void HAL_CFB_Set_Otp_Key(MS_U8 u8OtpKeyIdx);
void HAL_CFB_Set_CACWC_Src(MS_U8 u8CAcwcSrc);
void HAL_CFB_SET_CACWC(MS_U32 *pCACWC);
void HAL_CFB_SET_EPK(MS_U32 *pEPK);
void HAL_CFB_SET_EFUV(MS_U32 *pEFUV);
void HAL_CFB_Set_Cwc_Cfg(HAL_CFB_CWCCFG *pstCwcCfg);

MS_BOOL HAL_CFB_Is_Ready(void);
MS_BOOL HAL_CFB_Is_Done(void);

//////
MS_BOOL HAL_CFB_DBG_KT_Response(void);
MS_U8 HAL_CFB_DBG_CFB_FSM(void);

#endif // #ifndef __HAL_TCF_H__
