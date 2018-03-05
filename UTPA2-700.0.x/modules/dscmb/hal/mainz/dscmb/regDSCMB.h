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

#ifndef __REG_DSCMB_H__
#define __REG_DSCMB_H__

#define DSCMB_SHAREFLT_ENABLE   0

#define ENG_NUM                 1UL
#define ENG0_MAP_PID_START      16UL
#define ENG0_MAP_PID_LENGTH     16UL

#define MAX_NUM                  17UL
#define MAX_DSCMB_PIDFLT_NUM     8UL
#define DSCMB_SHARE_SLOT_REV_IDX (MAX_NUM-1UL)
#define REG_DSCMB_MAX_SLOT       (MAX_NUM*2UL)
#define REG_DSCMB_MAX_PIDFLT     16UL

#define TSP_PID_FLT_NUM         32UL

/*
typedef enum
{
    HAL_DSCMB_KEY_TYPE_CLEAR = 0,
    HAL_DSCMB_KEY_TYPE_EVEN = 1,
    HAL_DSCMB_KEY_TYPE_ODD = 2,
} HAL_DscmbKeyType;
*/

// #define REG_DSCMB_KEY_TYPE_CLEAR        0UL
#define REG_DSCMB_KEY_TYPE_ODD          3UL
#define REG_DSCMB_KEY_TYPE_EVEN         2UL
#define REG_DSCMB_POS_SWITCH2           13UL

////////////////////////////////////////////////////////////////////////////////////////////////
// TSP bank 1
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_TSP1_BANK                           0x0B00UL

#define REG_TSP1_CACTRL                         (REG_TSP1_BANK+ 0x0040UL)
    #define REG_TSP1_CACTRL_MASK                0x00FFUL
    #define REG_TSP1_CACTRL_INPUT_TS0LIVE       0x0001UL
    #define REG_TSP1_CACTRL_INPUT_TS0FILE       0x0002UL
    #define REG_TSP1_CACTRL_INPUT_TS1           0x0004UL

////////////////////////////////////////////////////////////////////////////////////////////////
// Descambler bank 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_BANK                          0x0600UL

#define REG_DSCMB_CTRL                          (REG_DSCMB_BANK+ 0x0000UL)
    #define REG_DSCMB_CTRL_CSA_ENABLE           0x0001UL
    #define REG_DSCMB_CTRL_CONFORMANCE_MECH     0x0020UL    
    #define REG_DSCMB_CTRL_CORRECT_SCRMBFLAG    0x0040UL
    #define REG_DSCMB_CTRL_OLD_TSC              0x0080UL
    #define REG_DSCMB_CTRL_SW_RST               0x8000UL

#define REG_DSCMB_CTRL1                         (REG_DSCMB_BANK+ 0x0001UL)
    #define REG_DSCMB_CTRL1_NEW_TSC_MASK        0x0003UL
    #define REG_DSCMB_CTRL1_NEW_TSC_EVEN        0x0002UL
    #define REG_DSCMB_CTRL1_NEW_TSC_ODD         0x0003UL

#define REG_DSCMB_SCMB_TS                       (REG_DSCMB_BANK+ 0x0002UL)
// #define REG_DSCMB_SCMB_PES                      (REG_DSCMB_BANK+ 0x0004UL)


#define REG_DSCMB_CIPHER_CONNECT_L              (REG_DSCMB_BANK+ 0x000aUL)
    #define REG_DSCMB_CSA_CIP                   0x1045UL
    #define REG_DSCMB_CSA_CIP_ENCRYPT           0x1044UL // HDCP2
    #define REG_DSCMB_CIP_CSA                   0x0511UL
    #define REG_DSCMB_CSA                       0x0109UL
    #define REG_DSCMB_CIP                       0x0030UL

    /*

#define REG_DSCMB_CIPHER_CONNECT_H              (REG_DSCMB_BANK+ 0x000bUL)
    #define REG_DSCMB_KT_TO_PASER               0x0001UL
    #define REG_DSCMB_IV_ENABLE                 0x0100UL
*/

#define REG_DSCMB_KL_CTRL1                      (REG_DSCMB_BANK+ 0x000cUL)
    #define REG_KL_START                        0x0001UL
    #define REG_KL_DECRYPT                      0x0002UL // 1: decrypt. 0: encrypt
    #define REG_KL_BYTE_INV                     0x0008UL
    #define REG_KL_KEEP_ROUNDS                  0x0010UL
    #define REG_KL_SWRST                        0x0080UL
    #define REG_KL_KEY_SRC_MASK                 0x0700UL
    #define REG_KL_KEY_SRC_SHFT                 8UL
        #define REG_KL_KEY_SRC_ACPU             0UL
        #define REG_KL_KEY_SRC_SECRET1          1UL
        #define REG_KL_KEY_SRC_SECRET2          2UL
        #define REG_KL_KEY_SRC_SECRET3          3UL
        #define REG_KL_KEY_SRC_SECRET4          4UL
        #define REG_KL_KEY_SRC_VGK              7UL
    #define REG_KL_ROUNDS_MASK                  0xf000UL
    #define REG_KL_ROUNDS_SHFT                  12UL

#define REG_DSCMB_KL_CTRL2                      (REG_DSCMB_BANK+ 0x000dUL)
    #define REG_KL_ENG_MODE_MASK                0x003fUL
    #define REG_KL_ENG_MODE_SHFT                0UL
        #define REG_LK_ENG_MODE_TDES            0UL
    #define REG_KL_KEY_DST_MASK                 0x3f00UL
    #define REG_KL_KEY_DST_SHFT                 8UL
        #define REG_KL_KEY_DST_KTAB_ESA         0x4UL
        #define REG_KL_KEY_DST_KTAB_NSK         0x2UL
        #define REG_KL_KEY_DST_AESDMA_AES       0x8UL
        #define REG_KL_KEY_DST_AESDMA_TDES      0x10UL
        #define REG_KL_KEY_DST_ACPU             0x1UL

#define REG_DSCMB_KL_CTRL3                      (REG_DSCMB_BANK+ 0x000eUL)
    #define REG_KL_ACPU_ACK                     0x0001UL
    #define REG_KL_FORCE_ACK                    0x0008UL

#define REG_DSCMB_KL_STATUS                     (REG_DSCMB_BANK+ 0x000fUL)
    #define REG_KL_KTE_STATUS_DONE              0x0001UL
    #define REG_KL_STATUS_CW_RDY_MASK           0x007cUL
    #define REG_KL_STATUS_CW_RDY_SHFT           2UL

#define REG_DSCMB_CIPHER0_HDCP2_RIV0             (REG_DSCMB_BANK + 0x0010UL)
#define REG_DSCMB_CIPHER0_HDCP2_RIV1             (REG_DSCMB_BANK + 0x0011UL)
#define REG_DSCMB_CIPHER0_HDCP2_RIV2             (REG_DSCMB_BANK + 0x0012UL)
#define REG_DSCMB_CIPHER0_HDCP2_RIV3             (REG_DSCMB_BANK + 0x0013UL)


#define REG_DSCMB_KL_KEY                        (REG_DSCMB_BANK+ 0x0018UL)

#define REG_DSCMB_ACPU_START                    (REG_DSCMB_BANK+ 0x0020UL)
    #define REG_ACPU_CMD_START                  0x0001UL
/*
    #define REG_ACPU_CMD_WEN_SHFT               4UL
    #define REG_ACPU_CMD_WEN_MASK               0x00F0UL
    #define REG_ACPU_CMD_WEN_ACPU               0x0010UL
    #define REG_ACPU_CMD_WEN_KLADDER            0x0020UL
    #define REG_ACPU_CMD_WEN_NSK                0x0040UL
    #define REG_ACPU_CMD_WEN_SWITCH2            0x0080UL
*/

#define REG_DSCMB_ACPU_CMD                      (REG_DSCMB_BANK+ 0x0022UL)
    #define REG_ACPU_CMD_READ                   0x0000UL
    #define REG_ACPU_CMD_WRITE                  0x0001UL
    #define REG_ACPU_CMD_POS_SHFT               4UL
    #define REG_ACPU_CMD_POS_MASK               0x00F0UL
    #define REG_ACPU_PIDFLTID_SHFT              8UL
    #define REG_ACPU_PIDFLTID_MASK              0x0F00UL
    #define REG_ACPU_DSCMB_TYPE_SHFT            12UL
    #define REG_ACPU_DSCMB_TYPE_MASK            0x3000UL
        #define REG_ACPU_DSCMB_TYPE_CLEAR       HAL_DSCMB_KEY_TYPE_CLEAR
        #define REG_ACPU_DSCMB_TYPE_EVEN        HAL_DSCMB_KEY_TYPE_EVEN
        #define REG_ACPU_DSCMB_TYPE_ODD         HAL_DSCMB_KEY_TYPE_ODD
    #define REG_ACPU_DSCMB_VALID                14UL

#define REG_DSCMB_WRITE                         (REG_DSCMB_BANK+ 0x0024UL)
#define REG_DSCMB_READ                          (REG_DSCMB_BANK+ 0x0026UL)

#define REG_DSCMB_ESA_MODE_EXT                  (REG_DSCMB_BANK+ 0x002fUL)
    #define REG_DSCMB_ESA_OC_MODE               0x0001UL
    #define REG_DSCMB_ESA_CLR_START_MODE        0x0002UL
    #define REG_DSCMB_ESA_TSC_PUSI_MODE         0x0004UL

#define REG_DSCMB_KT_CTRL                       (REG_DSCMB_BANK+ 0x0030UL)
    #define REG_DSCMB_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB_CIP_ESA_CBC_MODE          0x0020UL  //T8 new
    #define REG_DSCMB_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB_CIP_ESA_HDCP2_MODE        0x8000UL

/*
    #define REG_DSCMB_NSAD2KT_VALID             0x0001UL
    #define REG_DSCMB_ESA2KT_VALID              0x0002UL
    #define REG_DSCMB_NSAS2KT_VALID             0x0004UL
    #define REG_DSCMB_SWITCH2KT_VALID           0x0008UL
    #define REG_DSCMB_NSAD2KT_WEN               0x0010UL
    #define REG_DSCMB_ESA2KT_WEN                0x0020UL
    #define REG_DSCMB_NSAS2KT_WEN               0x0040UL
    #define REG_DSCMB_SWITCH2KT_WEN             0x0080UL
    #define REG_DSCMB_SWITCH2WNASK              0x0F00UL
*/
#define REG_DSCMB_MULTI2_ROUND                  (REG_DSCMB_BANK+ 0x0031UL)
#define REG_DSCMB_CW_LEVEL0                     (REG_DSCMB_BANK+ 0x0034UL)
#define REG_DSCMB_CW_LEVEL1                     (REG_DSCMB_BANK+ 0x003cUL)
#define REG_DSCMB_CW_LEVEL2                     (REG_DSCMB_BANK+ 0x0044UL)
#define REG_DSCMB_ENG2_CTRL                     (REG_DSCMB_BANK+ 0x005FUL)
    #define REG_DSCMB_MULTI2_ROUNDS_MASK        0x00FFUL
////////////////////////////////////////////////////////////////////////////////////////////////
// Descambler bank 2
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB2_BANK                         0x9E00UL

#define REG_DSCMB2_CIPHERENG_CTRL               (REG_DSCMB2_BANK+ 0x000CUL)
    #define REG_BLK_AF                          0x0008UL                    // DSCMB status don't care packet which full of AF data

#define REG_DSCMB2_MULTI2_SYSKEY_L0_0           (REG_DSCMB2_BANK+ 0x0020UL) // systemkey 0
#define REG_DSCMB2_MULTI2_SYSKEY_H0_0           (REG_DSCMB2_BANK+ 0x0022UL)
#define REG_DSCMB2_MULTI2_SYSKEY_L1_0           (REG_DSCMB2_BANK+ 0x0024UL) // systemkey 1
#define REG_DSCMB2_MULTI2_SYSKEY_H1_0           (REG_DSCMB2_BANK+ 0x0026UL)
#define REG_DSCMB2_MULTI2_SYSKEY_L2_0           (REG_DSCMB2_BANK+ 0x0028UL) // systemkey 2
#define REG_DSCMB2_MULTI2_SYSKEY_H2_0           (REG_DSCMB2_BANK+ 0x002AUL)
#define REG_DSCMB2_MULTI2_SYSKEY_L3_0           (REG_DSCMB2_BANK+ 0x002CUL) // systemkey 3
#define REG_DSCMB2_MULTI2_SYSKEY_H3_0           (REG_DSCMB2_BANK+ 0x002EUL)

#define REG_DSCMB2_PIDSLOT0                     (REG_DSCMB2_BANK+ 0x0040UL)
    #define REG_PIDSLOT_SLOTID_CLEAR_MASK       0x001FUL
    #define REG_PIDSLOT_SLOTID_CLEAR_SHFT       0UL
    #define REG_PIDSLOT_SLOTID_EVEN_MASK        0x03E0UL
    #define REG_PIDSLOT_SLOTID_EVEN_SHFT        5UL
    #define REG_PIDSLOT_SLOTID_ODD_MASK         0x7C00UL
    #define REG_PIDSLOT_SLOTID_ODD_SHFT         10UL

// Slot/switch

#endif // #ifndef __REG_DSCMB_H__
