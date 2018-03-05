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
///
/// file   drvDscmb.h
/// @brief  Descrambler (Dscmb) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HAL_DSCMB_H__
#define __HAL_DSCMB_H__

#define HAL_DSCMB_FLT_NULL                      0xFFFFFFFFUL

typedef enum
{
    E_HAL_DSCMB_TYPE_CSA = 0,   // CSA, NSA as ESA
    E_HAL_DSCMB_TYPE_CSA_CONF,  // CSA conformance mechanism
    E_HAL_DSCMB_TYPE_DES,       // CI+ DES
    E_HAL_DSCMB_TYPE_AES,       // CI+ AES
    E_HAL_DSCMB_TYPE_AES_CTR,   // HDCP2 AES CTR
    E_HAL_DSCMB_TYPE_NSA_AS_ESA,// NSA as ESA
    E_HAL_DSCMB_TYPE_AES_LEADING_CLR_CBC,
    E_HAL_DSCMB_TYPE_AES_LEADING_CLR_EBC,
    E_HAL_DSCMB_TYPE_CSA3,
    E_HAL_DSCMB_TYPE_DES_CBC,
    E_HAL_DSCMB_TYPE_TDES_CBC,
    E_HAL_DSCMB_TYPE_AES_ECB,
    E_HAL_DSCMB_TYPE_AES_SCTE52, // DBook, AES+CBC, T8 new
    E_HAL_DSCMB_TYPE_TDES_ECB,
    E_HAL_DSCMB_TYPE_TDES_SCTE52,

    E_HAL_DSCMB_TYPE_MULTI2,
    E_HAL_DSCMB_TYPE_OC, // OpenCable
    E_HAL_DSCMB_TYPE_INVALID = 0xFFFFFFFFUL   // NSA as ESA
} HAL_DSCMB_Type;

typedef enum
{
    E_HAL_DSCMB_NONE=           0x00000000,
    E_HAL_DSCMB_TS=             0x00000001,
    E_HAL_DSCMB_PES=            0x00000002,
    E_HAL_DSCMB_TS_PES=         (E_HAL_DSCMB_TS| E_HAL_DSCMB_PES),
} HAL_Scmb_Level;

// descrambler key ladder
typedef enum
{
    E_HAL_DSCMB_KLAD_SRC_ACPU =             0,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY1 =      1,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY2 =      2,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY3 =      3,
    E_HAL_DSCMB_KLAD_SRC_SECRET_KEY4 =      4,
    E_HAL_DSCMB_KLAD_SRC_VGK =              7,
} HAL_DscmbKLadderSrc;

typedef enum
{
    E_HAL_DSCMB_KLAD_DST_KTAB_ESA =         0x4,
    E_HAL_DSCMB_KLAD_DST_KTAB_NSA =         0x2,
    E_HAL_DSCMB_KLAD_DST_AESDMA_AES =       0x8,
    E_HAL_DSCMB_KLAD_DST_AESDMA_TDES =      0x10,
    E_HAL_DSCMB_KLAD_DST_ACPU =             0x1,
} HAL_DscmbKLadderDst;

typedef enum
{
    // descrambler engine
    E_HAL_DSCMB_CAP_ENGINE_NUM,
    // descrambler slot
    E_HAL_DSCMB_CAP_FLT_NUM,
    // descrambler type supported
    E_HAL_DSCMB_CAP_SUPPORT_ALGORITHM,
    // descrambler mapping, start tsp pid filter
    E_HAL_DSCMB_CAP_PIDFILTER_MAP_START,
    // descrambler mapping, end tsp pid filter
    E_HAL_DSCMB_CAP_PIDFILTER_MAP_END,
    // share key slot max number
    E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_NUM,
    // share key slot max number
    E_HAL_DSCMB_CAP_SHARE_KEY_SLOT_MAX_NUM,
    // share key region number
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_NUM,
    // share key region start
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_START,
    // share key region end
    E_HAL_DSCMB_CAP_SHARE_KEY_REGION_END,
} HAL_DSCMB_Query_Type;

MS_BOOL HAL_DSCMB_SetBank(MS_VIRT u32Bank);
MS_BOOL HAL_DSCMB_Init(void);
MS_BOOL HAL_DSCMB_CheckShareConnect(MS_U32 u32EngId, MS_U32 u32FltId, DSCMB_SLOT_Type SlotType);
MS_BOOL HAL_DSCMB_GetCap(MS_U32 u32EngId, HAL_DSCMB_Query_Type eQueryType, void* pInData, void* pOutData);
MS_BOOL HAL_DSCMB_OptConfig(MS_U32 u32EngId, DSCMB_Option eOpt, void* pInput, void* pOutput, MS_BOOL bSet);

// MS_BOOL HAL_DSCMB_SlotAlloc(MS_U32 u32DscmbId, HAL_DscmbKey_Type DscmbKeyType, MS_U32* pu32SlotId);
// MS_BOOL HAL_DSCMB_SlotFree(MS_U32 u32DscmbId, HAL_DscmbKey_Type, MS_U32 u32SlotId);
MS_BOOL HAL_DSCMB_SlotConnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId);
MS_BOOL HAL_DSCMB_SlotDisconnectPidFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type KeyType, MS_U32 u32SlotId, MS_U32 u32PidFltId);
MS_BOOL HAL_DSCMB_FltTypeSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, MS_BOOL bDscmb);
MS_BOOL HAL_DSCMB_FltKeySet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type, MS_U8* pu8Key);
MS_BOOL HAL_DSCMB_FltKeyReset(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType);
MS_BOOL HAL_DSCMB_FltIVSet(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType, MS_U8* pu8IV);

MS_BOOL HAL_DSCMB_EcmCorruptSet(MS_U8 u8CorruptDataBytes, MS_U8 u8FromByte);
MS_BOOL HAL_DSCMB_EcmCorruptEnable(MS_U32 u32PidFltId, MS_BOOL bEnable);

MS_BOOL HAL_DSCMB_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, HAL_Scmb_Level* pScmbLevel);

MS_BOOL HAL_DSCMB_KLadder_Reset(void);
MS_BOOL HAL_DSCMB_KLadder_SetInput(MS_U32 u32Level, MS_U8* pu8In, MS_U32 u32InLen);
MS_BOOL HAL_DSCMB_KLadder_SetKey(HAL_DscmbKLadderSrc KLSrc, MS_U8* pu8Key, MS_U32 u32KeyLen);
MS_BOOL HAL_DSCMB_KLadder_SetDst(HAL_DscmbKLadderDst KLDst);
void HAL_DSCMB_KLadder_Decrypt(MS_BOOL bDecrypt);
void HAL_DSCMB_KLadder_ByteInv(MS_BOOL bInverse);
void HAL_DSCMB_KLadder_KeepRound(MS_BOOL bKeepRound);
void HAL_DSCMB_KLadder_Round(MS_U32 u32Round);
void HAL_DSCMB_KLadder_EngMode(MS_U32 u32Mode);
MS_BOOL HAL_DSCMB_KLadder_Start(void);
MS_BOOL HAL_DSCMB_KLadder_IsComplete(void);
MS_BOOL HAL_DSCMB_KLadder_Stop(void);
MS_BOOL HAL_DSCMB_KLadder_Output_Start(MS_U32 u32PidFltId, MS_U32 u32Slot, HAL_DSCMB_Type DscmbType, DSCMB_Key_Type eKeyType);
MS_BOOL HAL_DSCMB_KLadder_Output_IsComplete(void);
MS_BOOL HAL_DSCMB_KLadder_Output_Stop(void);

MS_BOOL HAL_DSCMB_KLadder_Acpu_Data(MS_U8* pu8Out);
MS_BOOL HAL_DSCMB_KLadder_Acpu_Ack(MS_BOOL bEn);

MS_BOOL HAL_DSCMB_HDCP2_SetRiv(MS_U32 u32EngId, MS_U8* pu8RIV);
// Only for scramble
MS_BOOL HAL_DSCMB_2ndarySet(MS_U32 u32PidPrim, MS_U32 u32Pid2ndary);
MS_BOOL HAL_DSCMB_2ndaryReset(MS_U32 u32PidPrim);

MS_BOOL HAL_DSCMB_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round);
MS_BOOL HAL_DSCMB_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8SysKey);

#endif
