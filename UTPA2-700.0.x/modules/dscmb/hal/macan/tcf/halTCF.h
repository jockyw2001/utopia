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
#ifndef __HAL_TCF_H__
#define __HAL_TCF_H__


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

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
} HAL_CFB_CWCCFG;


////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_TCF_SetBank(MS_U32 u32NonPmBankAddr);
void HAL_TCF_Clk(MS_BOOL bEnable);


// CF
//
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


// CFKE
//
MS_U8 HAL_CFKE_Get_OperationStatus(void);
MS_U8 HAL_CFKE_Get_OperationCount(void);
MS_U8 HAL_CFKE_Get_ErrorCode(void);
MS_U8 HAL_CFKE_Get_OperationType(void);
MS_U8 HAL_CFKE_Get_DiffAllowNonUnw(void);
MS_U8 HAL_CFKE_Get_DiffAllowCk(void);
MS_U8 HAL_CFKE_Get_DiffContentOutputAllowed(void);

MS_U32 HAL_CFKE_Read_Command(void);
void HAL_CFKE_Write_Command(MS_U32 u32Cmd);

MS_U32 HAL_CFKE_Read_Platform(void);
MS_BOOL HAL_CFKE_IsTransactionDone(void);

MS_U32 HAL_CFKE_Read_UnwPath(void);
void HAL_CFKE_Write_UnwPath(MS_U32 u32Cmd);

MS_U32 HAL_CFKE_Read_Validator0(void);
void HAL_CFKE_Write_Validator0(MS_U32 u32Cmd);
MS_U32 HAL_CFKE_Read_Validator1(void);
void HAL_CFKE_Write_Validator1(MS_U32 u32Cmd);

void HAL_CFKE_Write_SlotA(MS_U32 u32Cmd);
void HAL_CFKE_Write_SlotB(MS_U32 u32Cmd);
void HAL_CFKE_Write_SlotC(MS_U32 u32Cmd);
void HAL_CFKE_Write_SlotD(MS_U32 u32Cmd);
void HAL_CFKE_Write_UserHash(MS_U32 u32Cmd);


// CRIKL
// Mirror of CF, CFKE Resigers
MS_U32 HAL_CRIKL_Read_CfStatus(void);
MS_U32 HAL_CRIKL_Read_CfkeStatus(void);
MS_U32 HAL_CRIKL_Read_CfPlatform(void);
MS_U32 HAL_CRIKL_Read_CfFeature(void);

// Key Table control
void HAL_CRIKL_Set_KTIndex( MS_U8 eScmbCtl, MS_U8 u8DscmbId);
void HAL_CRIKL_Write_KTPara_High_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KTPara_2nd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KTPara_3rd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KTPara_Low_HalfWord(MS_U16 u16HalfWord);
MS_U16 HAL_CRIKL_Read_KTIndex(void);
MS_U16 HAL_CRIKL_Read_KTPara_High_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KTPara_2nd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KTPara_3rd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KTPara_Low_HalfWord(void);
// Key Slot control
void HAL_CRIKL_Set_KSIndex(MS_U8 u8KsIdx);
void HAL_CRIKL_Write_KSPara_High_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KSPara_2nd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KSPara_3rd_HalfWord(MS_U16 u16HalfWord);
void HAL_CRIKL_Write_KSPara_Low_HalfWord(MS_U16 u16HalfWord);
MS_U16 HAL_CRIKL_Read_KSIndex(void);
MS_U16 HAL_CRIKL_Read_KSPara_High_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KSPara_2nd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KSPara_3rd_HalfWord(void);
MS_U16 HAL_CRIKL_Read_KSPara_Low_HalfWord(void);

// Key Parameter
void HAL_CRIKL_Reset_KeyPara(void);
void HAL_CRIKL_Write_KeyPara(MS_BOOL bDscmbKey);

void HAL_CRIKL_Setup_VendorNo(MS_BOOL bDscmbKey, MS_U8 u8VendorNo);
void HAL_CRIKL_Setup_VendorAtt(MS_BOOL bDscmbKey, MS_U16 u16VendorAtt);
void HAL_CRIKL_Setup_OutputAllowed(MS_BOOL bDscmbKey, MS_U8 u8OutputAllowed);
void HAL_CRIKL_Setup_bMultiEng(MS_BOOL bDscmbKey, MS_BOOL bMultiEng);
void HAL_CRIKL_Setup_bLsb64(MS_BOOL bDscmbKey, MS_BOOL bLsb64);
void HAL_CRIKL_Setup_KeySize(MS_BOOL bDscmbKey, MS_U8 eKeySize);
void HAL_CRIKL_Setup_KeyEngine(MS_BOOL bDscmbKey, MS_U8 uKeyEng);
void HAL_CRIKL_Setup_Drop(MS_BOOL bDscmbKey, MS_U8 u8Drop);
void HAL_CRIKL_Setup_Cnt(MS_BOOL bDscmbKey, MS_U8 u8Cnt);
void HAL_CRIKL_Setup_KeyDest(MS_BOOL bDscmbKey, MS_U8 u8KeyDest);
void HAL_CRIKL_Setup_OpType(MS_BOOL bDscmbKey, MS_U8 u8Op);
void HAL_CRIKL_Setup_KteSel(MS_BOOL bDscmbKey, MS_U8 eKteSel);
void HAL_CRIKL_Setup_bLock(MS_BOOL bDscmbKey, MS_BOOL bLock);
void HAL_CRIKL_Setup_bParaValid(MS_BOOL bDscmbKey, MS_BOOL bParaValid);
void HAL_CRIKL_Setup_bKlEn(MS_BOOL bDscmbKey, MS_BOOL bKlEn);

#endif // #ifndef __HAL_TCF_H__
