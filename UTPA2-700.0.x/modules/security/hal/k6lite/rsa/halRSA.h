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
#ifndef __HAL_RSA_H__
#define __HAL_RSA_H__


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  AESDMA Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

// TSP Register
#define _TspPid                     ((REG_Pid*)(REG_PIDFLT_BASE))
#define _TspSec                     ((REG_Sec*)(REG_SECFLT_BASE))

#define DIGEST_SIZE_SHA1   5          //160-bit
#define DIGEST_SIZE_SHA256 8          //256-bit
#define SHA_MIU1_ENABLE    0x80000000 //Enable  access MIU1
#define SHA_UNIT_SIZE      8          //64-bits

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))


typedef enum {
    RAM_READ = 0x0,
    RAM_WRITE = 0x1
} RAM_DIR_t;


typedef enum {
    E_RSA_SW_RESET_OFF = 0x0,
    E_RSA_SW_RESET_ON = 0x1
} RSA_SW_RESET_t;

typedef enum {
    E_RSA_USE_SW_KEY = 0x0,
    E_RSA_USE_HW_KEY = 0x1
} RSA_HW_KEY_t;

typedef enum {
    E_RSA_USE_PRIV_KEY = 0x0,
    E_RSA_USE_PUB_KEY = 0x1
} RSA_PUB_KEY_t;

typedef enum {
    E_RSA_HW_PRI_KEY  = 0x0,
    E_RSA_HW_PUB_KEY1 = 0x1,
    E_RSA_HW_PUB_KEY2 = 0x2
} RSA_PUB_KEY_SEL_t;


/// HW PARSER Mode


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
void HAL_RSA_ClearInt(void);
void HAL_RSA_Reset(void);
void HAL_RSA_Ind32Ctrl(MS_U8 u8dirction);
void HAL_RSA_LoadSram(MS_U32 *u32Buf, RSA_IND32Address eMode);
void HAL_RSA_SetKeyLength(MS_U32 u32keylen);
void HAL_RSA_SetKeyType(MS_U8 u8hwkey, MS_U8 u8pubkey);
void HAL_RSA_Rootkey_Sel(MS_U8 u8KeySrc);
void HAL_RSA_ExponetialStart(void);
MS_U32 HAL_RSA_GetStatus(void);
void HAL_RSA_FileOutStart(void);
void HAL_RSA_SetFileOutAddr(MS_U32 u32offset);
MS_U32 HAL_RSA_FileOut(void);
MS_U32 HAL_RSA_Get_RSA_IsFinished(void);
void HAL_RSA_SetBank(MS_VIRT u32NonPmBankAddr);

MS_BOOL HAL_RSA_SetSecureRange(MS_U32 u32addr, MS_U32 u32size);

void HAL_RSA_Hwkey_Switch(void);
MS_U32 HAL_RSA_Read_Hwkey_Switch(void);

#endif // #ifndef __HAL_TSP_H__
