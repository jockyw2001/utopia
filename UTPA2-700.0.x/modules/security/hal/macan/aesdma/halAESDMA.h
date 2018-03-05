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
// file   halAESDMA.h
// @brief  AESDMA HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_AESDMA_H__
#define __HAL_AESDMA_H__

#include "regAESDMA.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  AESDMA Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

// TSP Register
#define _TspPid                     ((REG_Pid*)(REG_PIDFLT_BASE))
#define _TspSec                     ((REG_Sec*)(REG_SECFLT_BASE))

#define DIGEST_SIZE_SHA1			5UL //160-bit
#define DIGEST_SIZE_SHA256			8UL //256-bit
#define SHA_MIU1_ENABLE			    0x80000000UL //Enable access MIU1
#define SHA_UNIT_SIZE      1UL
#define PARSER_PID_COUNT   2UL

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))

/// SHA Mode
typedef enum
{
    E_AESDMA_SHA1 = 0,
    E_AESDMA_SHA256,
} AESDMA_SHAMode;

/// AESDMA Cipher Mode
typedef enum
{
    E_AESDMA_CIPHER_ECB = 0,
    E_AESDMA_CIPHER_CTR,
    E_AESDMA_CIPHER_CBC,
    E_AESDMA_CIPHER_DES_ECB,
    E_AESDMA_CIPHER_DES_CTR,
    E_AESDMA_CIPHER_DES_CBC,
    E_AESDMA_CIPHER_TDES_ECB,
    E_AESDMA_CIPHER_TDES_CTR,
    E_AESDMA_CIPHER_TDES_CBC,
    E_AESDMA_CIPHER_CTS_CBC,
    E_AESDMA_CIPHER_CTS_ECB,
    E_AESDMA_CIPHER_DES_CTS_CBC,
    E_AESDMA_CIPHER_DES_CTS_ECB,
    E_AESDMA_CIPHER_TDES_CTS_CBC,
    E_AESDMA_CIPHER_TDES_CTS_ECB,
} AESDMA_CipherMode;

/// HW PARSER Mode
typedef enum
{
    E_AESDMA_PARSER_TS_PKT192 = 0,
    E_AESDMA_PARSER_TS_PKT192_CLEAR,
    E_AESDMA_PARSER_TS_PKT188,
    E_AESDMA_PARSER_TS_PKT188_CLEAR,
    E_AESDMA_PARSER_HDCP20_PKT192,
    E_AESDMA_PARSER_HDCP20_PKT192_CLEAR,
    E_AESDMA_PARSER_HDCP20_PKT188,
    E_AESDMA_PARSER_HDCP20_PKT188_CLEAR,
} AESDMA_ParserMode;

typedef enum
{
    E_AESDMA_PARSER_SCRMB_10 = 0,
    E_AESDMA_PARSER_SCRMB_11,
    E_AESDMA_PARSER_SCRMB_CLEAR,
} AESDMA_ScrmbPattern;

typedef enum
{
    E_RSA_ADDRESS_E = 0,
    E_RSA_ADDRESS_N,
    E_RSA_ADDRESS_A,
    E_RSA_ADDRESS_Z,
} RSA_IND32Address;

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_AESDMA_SetBank(MS_VIRT u32NonPmBankAddr, MS_VIRT u32PmBankAddr);
MS_PHY AESDMA_GetMIU1Base(void);
MS_PHY AESDMA_GetMIU2Base(void);
void AESDMA_Reset(void);
void AESDMA_Set_CipherKey(MS_U32 *cipherkey);
void AESDMA_Set_InitVector(MS_U32 *pInitVector);
void AESDMA_Sel_Key(MS_BOOL keysel);
void AESDMA_Sel_SecureKey(void);
void AESDMA_Set_FileinDesc(MS_PHY FileinAddr , MS_PHY FileinNum);
void AESDMA_Set_FileoutDesc(MS_PHY FileoutSAddr , MS_PHY FileoutEAddr);
void AESDMA_Start(MS_BOOL AESDMAStart);
void AESDMA_Set_PS_PTN(MS_U32 MatchPTN);
void AESDMA_Set_PS_Mask(MS_U32 MatchMask);
void AESDMA_Set_PS_ENG(MS_BOOL PSin_en, MS_BOOL PSout_en);
MS_U32 AESDMA_Get_PS_MatchedBytecnt(void);
MS_U32 AESDMA_Get_PS_MatchedPTN(void);
void AESDMA_Set_PS_Release(void);
MS_U32 AESDMA_Get_AESDMA_Status(void);
MS_U32 AESDMA_Get_AESDMA_IsFinished(void);
void AESDMA_Set_CIPHER_ENG(AESDMA_CipherMode eMode, MS_BOOL Descrypt);
MS_U32 AESDMA_QueryCipherMode(AESDMA_CipherMode eMode);
void AESDMA_Set_MIU_Path(MS_BOOL MIU_R, MS_BOOL MIU_W);
void AESDMA_Set_MIU2_Path(MS_BOOL MIU_R, MS_BOOL MIU_W);
void AESDMA_Enable_Int(void);
void AESDMA_Disable_Int(void);
void AESDMA_Clear_Int(void);
void AESDMA_HW_Patch(void);
void AESDMA_Enable_Clk(void);
void AESDMA_Disable_Clk(void);
MS_U8 AESDMA_Rand(void);
void SHA_Reset(void);
MS_U32 SHA_Get_Status(void);
void SHA_SelMode(AESDMA_SHAMode eMode);
void SHA_SetAddress(MS_PHY u32Addr);
void SHA_SetLength(MS_U32 u32Size);
void SHA_Start(MS_BOOL SHAStart);
void SHA_Out(MS_VIRT u32Buf);
void MOBF_OneWay(void);
void MOBF_WriteKey(MS_U32 u32WriteKey);
void MOBF_ReadKey(MS_U32 u32ReadKey);
void MOBF_WriteEnable(MS_BOOL bEnable);
void MOBF_ReadEnable(MS_BOOL bEnable);
void MOBF_DisableTDES(void);
void AESDMA_Parser_Set_Mode(AESDMA_ParserMode eMode);
void AESDMA_Parser_Insert_Scrmb(MS_BOOL bEnable);
void AESDMA_Parser_Remove_Scrmb(MS_BOOL bEnable);
void AESDMA_Parser_Mask_Scrmb(MS_BOOL bEnable);
void AESDMA_Parser_Bypass_Pid(MS_BOOL bEnable);
void AESDMA_Parser_Set_ScrmbPattern(AESDMA_ScrmbPattern ePattern);
void AESDMA_Parser_Set_AddedScrmbPattern(AESDMA_ScrmbPattern ePattern);
void AESDMA_Parser_Set_Pid(MS_U8 u8Index, MS_U16 u16Pid);
MS_U8 AESDMA_Parser_Query_PidCount(void);
void AESDMA_Parser_Enable_HWParser(MS_BOOL bEnable);
void RSA_ClearInt(void);
void RSA_Reset(void);
void RSA_Ind32Ctrl(MS_U8 u8dirction);
void RSA_LoadSram(MS_U32 *u32Buf, RSA_IND32Address eMode);
void RSA_SetKeyLength(MS_U32 u32keylen);
void RSA_SetKeyType(MS_U8 u8hwkey, MS_U8 u8pubkey);
void RSA_ExponetialStart(void);
MS_U32 RSA_GetStatus(void);
void RSA_FileOutStart(void);
void RSA_SetFileOutAddr(MS_U32 u32offset);
MS_U32 RSA_FileOut(void);
MS_U32 RSA_Get_RSA_IsFinished(void);
void AESDMA_Parser_Enable_Two_Keys(MS_BOOL bEnable);
void AESDMA_Set_CipherOddKey(MS_U32 *cipherkey);
void AESDMA_Set_CipherEvenKey(MS_U32 *cipherkey);
void AESDMA_Set_OddInitVector(MS_U32 *pInitVector);
void AESDMA_Set_EvenInitVector(MS_U32 *pInitVector);

MS_U32 AESDMA_IsSecretKeyInNormalBank(void);
void HDCP_ProcessCipher (MS_U8 u8Idx, MS_U8 *pu8Data, MS_U8* pu8privData);
void AESDMA_NormalReset(void);
void HDCP_GetHdcpCipherState(MS_U8 u8Idx, MS_U8 *pu8State);
void AESDMA_Set_SecureBankEnable (MS_BOOL bEnable);

//reserved for secure protect function
/*
void AESDMA_Secure_SetCipherKey(MS_U32 *cipherkey);
void AESDMA_Secure_SetInitVector(MS_U32 *pInitVector);
void AESDMA_SecureProtect(MS_BOOL bEnable);
void AESDMA_Set_FileOutEnable(MS_BOOL bEnable);
void AESDMA_Secure_Start(MS_BOOL AESDMAStart);
MS_U32 AESDMA_Get_AESDMA_SecureIsFinished(void);
*/

#endif // #ifndef __HAL_TSP_H__
