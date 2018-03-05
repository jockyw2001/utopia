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

#define DSCMB_SHAREFLT_ENABLE   1  // share key support or not
#define DSCMB_PARAMETER_TABLE   1  // table to support n filter to 1 key slot

#define ENG_NUM                 1UL
//
// source of cipher (CA_CTRL)
// * TSIF0_LIVE => 16~31
// * TSIF0_FILE => 16~31
// * TSIF1      => 48~63
// * TSIF2      => 48~63
//
#define GROUP0_MAP_PID_START      16UL
#define GROUP0_MAP_PID_LENGTH     16UL
#define GROUP1_MAP_PID_START      32UL
#define GROUP1_MAP_PID_LENGTH     16UL

#define MAX_NUM                   16UL
#define MAX_DSCMB_PIDFLT_NUM      128UL
#define DSCMB_SHARE_SLOT_REV_IDX  (MAX_NUM-1UL)  ///< the index of slot which is reserved for share key
#define REG_DSCMB_MAX_SLOT        (MAX_NUM*2UL)  ///< slot number x (odd/even)
#define REG_DSCMB_MAX_PIDFLT      128UL
#define MAX_KEY                   16UL

// Share key filter range definition
// CA0
#define DSCMB0_SHARE0_FLT_START_ID  0UL
#define DSCMB0_SHARE0_FLT_END_ID    127UL
#define DSCMB0_SHARE1_FLT_START_ID  0UL
#define DSCMB0_SHARE1_FLT_END_ID    127UL
#define DSCMB0_SHARE2_FLT_START_ID  0UL
#define DSCMB0_SHARE2_FLT_END_ID    127UL
// CA1
#define DSCMB1_SHARE0_FLT_START_ID  0UL
#define DSCMB1_SHARE0_FLT_END_ID    127UL
#define DSCMB1_SHARE1_FLT_START_ID  0UL
#define DSCMB1_SHARE1_FLT_END_ID    127UL
#define DSCMB1_SHARE2_FLT_START_ID  0UL
#define DSCMB1_SHARE2_FLT_END_ID    127UL

#define REG_DSCMB_IN_TS0_LIVE       0x00000001UL
#define REG_DSCMB_IN_TS0_FILE       0x00000002UL
#define REG_DSCMB_IN_TS1            0x00000004UL
#define REG_DSCMB_OUT_LIVE          0x00000010UL
#define REG_DSCMB_OUT_FILE          0x00000020UL
#define REG_DSCMB_OUT_RECORD        0x00000040UL
#define REG_DSCMB_IN_TS2            0x00001000UL
#define REG_DSCMB1_IN_TS2           0x00100000UL
#define REG_DSCMB1_IN_TS0_LIVE      0x01000000UL
#define REG_DSCMB1_IN_TS0_FILE      0x02000000UL
#define REG_DSCMB1_IN_TS1           0x04000000UL
#define REG_DSCMB1_OUT_LIVE         0x10000000UL
#define REG_DSCMB1_OUT_FILE         0x20000000UL
#define REG_DSCMB1_OUT_RECORD       0x40000000UL


#define DSCMB_GROUP0_SOURCE   (REG_DSCMB_IN_TS0_LIVE | REG_DSCMB_IN_TS0_FILE)
#define DSCMB_GROUP1_SOURCE   (REG_DSCMB_IN_TS1 | REG_DSCMB_IN_TS2)

#define DSCMB1_GROUP0_SOURCE   (REG_DSCMB1_IN_TS0_LIVE | REG_DSCMB1_IN_TS0_FILE)
#define DSCMB1_GROUP1_SOURCE   (REG_DSCMB1_IN_TS1 | REG_DSCMB1_IN_TS2)


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


//-------------------------
// KTE Parameter Table define
//-------------------------
// [15:00]
#define REG_KTE_PARA_TABLE_KL_EN            0x0001UL
#define REG_KTE_PARA_TABLE_PARA_VLD         0x0004UL
#define REG_KTE_PARA_TABLE_LOCK             0x0008UL
#define REG_KTE_PARA_TABLE_KTE0             0x0010UL
#define REG_KTE_PARA_TABLE_KTE1             0x0020UL
#define REG_KTE_PARA_TABLE_KTE2             0x0040UL
#define REG_KTE_PARA_TABLE_KTE3             0x0080UL
#define REG_KTE_PARA_TABLE_DEC              0x0100UL
#define REG_KTE_PARA_TABLE_ENC              0x0200UL
// [31:16]
#define REG_KTE_PARA_TABLE_AES              0x0100UL
#define REG_KTE_PARA_TABLE_TDES             0x0200UL
#define REG_KTE_PARA_TABLE_DES              0x0400UL
#define REG_KTE_PARA_TABLE_CSAV2            0x2000UL
#define REG_KTE_PARA_TABLE_MULTI2           0x4000UL
// [47:32]
#define REG_KTE_PARA_TABLE_64BIT            0x0001UL
#define REG_KTE_PARA_TABLE_128BIT           0x0002UL
#define REG_KTE_PARA_TABLE_192BIT           0x0004UL
#define REG_KTE_PARA_TABLE_LSB64            0x0010UL
#define REG_KTE_PARA_TABLE_MULTI_ENG        0x0020UL
#define REG_KTE_PARA_TABLE_OUTALLOW_MASK    0x3000UL
// [63:48]
#define REG_KTE_PARA_TABLE_VERSION_NO       0xF000UL

////////////////////////////////////////////////////////////////////////////////////////////////
// Descambler bank 0
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB_BANK                          0x0600UL

#define REG_DSCMB_CTRL                          (REG_DSCMB_BANK+ 0x0000UL) // ESA + CSA control
    #define REG_DSCMB_CTRL_CSA_ENABLE           0x0001UL
    #define REG_DSCMB_CTRL_CONFORMANCE_MECH     0x0020UL
    #define REG_DSCMB_CTRL_CORRECT_SCRMBFLAG    0x0040UL
    #define REG_DSCMB_CTRL_OLD_TSC              0x0080UL
//    #define REG_DSCMB_SEL_2RD_GROUP_MASK        0x1800UL
//        #define REG_DSCMB_SEL_2RD_GROUP_00      0x0000UL
//        #define REG_DSCMB_SEL_2RD_GROUP_01      0x0800UL
//        #define REG_DSCMB_SEL_2RD_GROUP_10      0x1000UL
//        #define REG_DSCMB_SEL_2RD_GROUP_11      0x1800UL
    #define REG_DSCMB_CTRL_SW_RST               0x8000UL

#define REG_DSCMB_CTRL1                         (REG_DSCMB_BANK+ 0x0001UL)
    #define REG_DSCMB_CTRL1_NEW_TSC_MASK        0x0003UL
    #define REG_DSCMB_CTRL1_NEW_TSC_EVEN        0x0002UL
    #define REG_DSCMB_CTRL1_NEW_TSC_ODD         0x0003UL
    #define REG_DSCMB_CTRL1_HDCP2_ECO           0x0004UL // ECO bit for HDCP2 mode issue(Agate)

#define REG_DSCMB_SCMB_TS                       (REG_DSCMB_BANK+ 0x0002UL)
// #define REG_DSCMB_SCMB_PES                      (REG_DSCMB_BANK+ 0x0004UL)

#define REG_DSCMB0_HDCP2_RIV0             (REG_DSCMB_BANK + 0x0010UL)
#define REG_DSCMB0_HDCP2_RIV1             (REG_DSCMB_BANK + 0x0011UL)
#define REG_DSCMB0_HDCP2_RIV2             (REG_DSCMB_BANK + 0x0012UL)
#define REG_DSCMB0_HDCP2_RIV3             (REG_DSCMB_BANK + 0x0013UL)
#define REG_DSCMB1_HDCP2_RIV0             (REG_DSCMB_BANK + 0x0014UL)
#define REG_DSCMB1_HDCP2_RIV1             (REG_DSCMB_BANK + 0x0015UL)
#define REG_DSCMB1_HDCP2_RIV2             (REG_DSCMB_BANK + 0x0016UL)
#define REG_DSCMB1_HDCP2_RIV3             (REG_DSCMB_BANK + 0x0017UL)


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

#define REG_DSCMB_KL_KEY                        (REG_DSCMB_BANK+ 0x0018UL)

#define REG_DSCMB_ACPU_START                    (REG_DSCMB_BANK+ 0x0020UL)
    #define REG_ACPU_CMD_START                  0x0001UL
    #define REG_ACPU_CMD_ENG_SHIFT              0x8UL
    #define REG_ACPU_ENG0                       0x0UL
    #define REG_ACPU_ENG1                       0x1UL
    #define REG_ACPU_ENG2                       0x2UL
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
    #define REG_ACPU_CMD_CLEAR                  0x0002UL
    #define REG_ACPU_CMD_POS2                   0x0008UL  // set 1 to r/w parameter table
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

#define REG_DSCMB_KT_CTRL_EXT                   (REG_DSCMB_BANK+ 0x002fUL)
    #define REG_DSCMB_ESA_OC_MODE               0x0001UL

#define REG_DSCMB_KT_CTRL                       (REG_DSCMB_BANK+ 0x0030UL)
    #define REG_DSCMB_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB_CIP_ESA_TDES_MODE         0x0002UL
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

    //
    //    EX: For CIP HD AES: 0xC8
    //    EX: For CIP SD DES: 0x91
    //    EX: MULTI2: 0x1020
    //    EX: HDCP2 : 0xA808
    //

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

#define REG_DSCMB_PID_VALID0                    (REG_DSCMB_BANK+ 0x0040UL)
#define REG_DSCMB_PID_VALID1                    (REG_DSCMB_BANK+ 0x0042UL)
#define REG_DSCMB_PID_VALID2                    (REG_DSCMB_BANK+ 0x0043UL)

#define REG_DSCMB_CW_LEVEL2                     (REG_DSCMB_BANK+ 0x0044UL)
#define REG_DSCMB_ENG2_CTRL                     (REG_DSCMB_BANK+ 0x005FUL)
    #define REG_DSCMB_MULTI2_ROUNDS_MASK        0x00FFUL
////////////////////////////////////////////////////////////////////////////////////////////////
// Descambler bank 2
////////////////////////////////////////////////////////////////////////////////////////////////
#define REG_DSCMB1_BANK                         0x9E00UL

#define REG_ESA1_CTRL                            (REG_DSCMB1_BANK + 0x0000UL)
    #define REG_ESA1_CTRL_CORRECT_SCRMBFLAG    0x0002UL
    #define REG_ESA1_CTRL_OLD_TSC              0x0004UL
    #define REG_ESA1_CTRL_NEW_TSC_MASK         0x0030UL
    #define REG_ESA1_CTRL_NEW_TSC_EVEN         0x0020UL
    #define REG_ESA1_CTRL_NEW_TSC_ODD          0x0030UL
//    #define REG_DSCMB1_SEL_2RD_GROUP_MASK        0x00C0UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_00      0x0000UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_01      0x0040UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_10      0x0080UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_11      0x00C0UL
    #define REG_ESA1_CTRL_SWITCH_KEY_VALID     0x2000UL
    #define REG_ESA1_CTRL_USE_CSA_PARSER       0x4000UL
    #define REG_ESA1_CTRL_USE_ESA_PARSER       0x8000UL


#define REG_DSCMB1_SCMB_TS                       (REG_DSCMB1_BANK + 0x0001UL)

#define REG_DSCMB1_KT_CTRL                       (REG_DSCMB1_BANK + 0x0003UL)
    #define REG_DSCMB1_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB1_CIP_ESA_TDES_MODE         0x0002UL
    #define REG_DSCMB1_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB1_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB1_CIP_ESA_CBC_MODE          0x0020UL
    #define REG_DSCMB1_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB1_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB1_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB1_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB1_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB1_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB1_CIP_ESA_HDCP2_MODE        0x8000UL
//
//    EX: For CIP HD AES: 0xC8
//    EX: For CIP SD DES: 0x91
//    EX: MULTI2: 0x1020
//    EX: HDCP2 : 0xA808
//

#define REG_DSCMB1_PID_VALID0                    (REG_DSCMB1_BANK + 0x0006UL)
#define REG_DSCMB1_MULTI2_ROUND                  (REG_DSCMB1_BANK + 0x0007UL)

#define REG_CSA1_CTRL                            (REG_DSCMB1_BANK + 0x0008UL)
    #define REG_DSCMB1_CTRL_CSA_MODE             0x0001UL
    #define REG_DSCMB1_CTRL_CONFORMANCE_MECH     0x0002UL
    #define REG_DSCMB1_CTRL_CLR_OVERFLOW_CSA     0x0004UL

#define REG_ESA2_CTRL                            (REG_DSCMB1_BANK + 0x0010UL)
    #define REG_ESA2_CTRL_CORRECT_SCRMBFLAG      0x0002UL
    #define REG_ESA2_CTRL_OLD_TSC                0x0004UL
    #define REG_ESA2_CTRL_NEW_TSC_MASK           0x0030UL
    #define REG_ESA2_CTRL_NEW_TSC_EVEN           0x0020UL
    #define REG_ESA2_CTRL_NEW_TSC_ODD            0x0030UL
//    #define REG_DSCMB1_SEL_2RD_GROUP_MASK        0x00C0UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_00      0x0000UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_01      0x0040UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_10      0x0080UL
//        #define REG_DSCMB1_SEL_2RD_GROUP_11      0x00C0UL
    #define REG_ESA2_CTRL_SWITCH_KEY_VALID     0x2000UL


#define REG_DSCMB1_PID_VALID1                    (REG_DSCMB1_BANK + 0x000AUL)
#define REG_DSCMB1_PID_VALID2                    (REG_DSCMB1_BANK + 0x000BUL)

#define REG_DSCMB2_CIPHERENG_CTRL                (REG_DSCMB1_BANK + 0x000CUL)
    #define REG_BLK_AF_ENG0                      0x0100UL                    // DSCMB status don't care packet which full of AF data
    #define REG_BLK_AF_ENG1                      0x0200UL
    #define REG_BLK_AF_ENG2                      0x0400UL
    #define REG_BLK_AF_ENG3                      0x0800UL

#define REG_DSCMB1_KT_CTRL_EXT                   (REG_DSCMB1_BANK + 0x000dUL)
    #define REG_DSCMB1_ESA_OC_MODE               0x0001UL

#define REG_DSCMB2_CTRL                          (REG_DSCMB1_BANK + 0x0010UL)
//    #define REG_DSCMB2_SEL_2RD_GROUP_MASK        0x0C00UL
//        #define REG_DSCMB2_SEL_2RD_GROUP_00      0x0000UL
//        #define REG_DSCMB2_SEL_2RD_GROUP_01      0x0400UL
//        #define REG_DSCMB2_SEL_2RD_GROUP_10      0x0800UL
//        #define REG_DSCMB2_SEL_2RD_GROUP_11      0x0C00UL

#define REG_DSCMB2_SCMB_TS                       (REG_DSCMB1_BANK + 0x0011UL)

#define REG_DSCMB2_KT_CTRL                       (REG_DSCMB1_BANK + 0x0013UL)
    #define REG_DSCMB2_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB2_CIP_ESA_TDES_MODE         0x0002UL
    #define REG_DSCMB2_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB2_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB2_CIP_ESA_CBC_MODE          0x0020UL
    #define REG_DSCMB2_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB2_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB2_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB2_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB2_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB2_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB2_CIP_ESA_HDCP2_MODE        0x8000UL

#define REG_DSCMB2_PID_VALID0                   (REG_DSCMB1_BANK+ 0x0016UL)
#define REG_DSCMB2_MULTI2_ROUND                 (REG_DSCMB1_BANK+ 0x0017UL)

#define REG_CSA2_CTRL                           (REG_DSCMB1_BANK+ 0x0018UL)
    #define REG_DSCMB2_CTRL_CSA_MODE             0x0001UL
    #define REG_DSCMB2_CTRL_CONFORMANCE_MECH     0x0002UL
    #define REG_DSCMB2_CTRL_CLR_OVERFLOW_CSA     0x0004UL

#define REG_DSCMB2_PID_VALID1                   (REG_DSCMB1_BANK+ 0x001AUL)
#define REG_DSCMB2_PID_VALID2                   (REG_DSCMB1_BANK+ 0x001BUL)

#define REG_DSCMB2_KT_CTRL_EXT                  (REG_DSCMB1_BANK+ 0x001DUL)
    #define REG_DSCMB2_ESA_OC_MODE               0x0001UL

#define REG_DSCMB0_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0020UL) // systemkey 0
#define REG_DSCMB0_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0022UL)
#define REG_DSCMB0_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0024UL) // systemkey 1
#define REG_DSCMB0_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0026UL)
#define REG_DSCMB0_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0028UL) // systemkey 2
#define REG_DSCMB0_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x002AUL)
#define REG_DSCMB0_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x002CUL) // systemkey 3
#define REG_DSCMB0_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x002EUL)

#define REG_DSCMB1_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0030UL) // systemkey 0
#define REG_DSCMB1_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0032UL)
#define REG_DSCMB1_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0034UL) // systemkey 1
#define REG_DSCMB1_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0036UL)
#define REG_DSCMB1_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0038UL) // systemkey 2
#define REG_DSCMB1_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x003AUL)
#define REG_DSCMB1_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x003CUL) // systemkey 3
#define REG_DSCMB1_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x003EUL)

#define REG_DSCMB2_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0040UL) // systemkey 0
#define REG_DSCMB2_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0042UL)
#define REG_DSCMB2_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0044UL) // systemkey 1
#define REG_DSCMB2_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0046UL)
#define REG_DSCMB2_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0048UL) // systemkey 2
#define REG_DSCMB2_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x004AUL)
#define REG_DSCMB2_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x004CUL) // systemkey 3
#define REG_DSCMB2_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x004EUL)

#define REG_DSCMB2_HDCP2_RIV0                   (REG_DSCMB1_BANK + 0x0050UL)
#define REG_DSCMB2_HDCP2_RIV1                   (REG_DSCMB1_BANK + 0x0051UL)
#define REG_DSCMB2_HDCP2_RIV2                   (REG_DSCMB1_BANK + 0x0052UL)
#define REG_DSCMB2_HDCP2_RIV3                   (REG_DSCMB1_BANK + 0x0053UL)

#define REG_DSCMB2_ARRAY_WDATA                  (REG_DSCMB1_BANK+ 0x0054UL)
#define REG_DSCMB2_ARRAY_RDATA                  (REG_DSCMB1_BANK+ 0x0055UL)
    #define REG_DSCMB2_ARRAY_ENG0_VLD           0x0100UL
    #define REG_DSCMB2_ARRAY_ENG1_VLD           0x0200UL
    #define REG_DSCMB2_ARRAY_ENG2_VLD           0x0400UL
    #define REG_DSCMB2_ARRAY_ENG3_VLD           0x0800UL
    #define REG_DSCMB2_KEY_IDX_MASK             0x000FUL

#define REG_DSCMB2_ARRAY_ADR                    (REG_DSCMB1_BANK+ 0x0056UL) // equal to PID index
    #define REG_DSCMB2_ARRAY_ADR_MASK           0x00FFUL

#define REG_DSCMB2_ARRAY_RW_TRIG                (REG_DSCMB1_BANK+ 0x0057UL)
    #define REG_DSCMB2_TRIG_W                   0x0001UL
    #define REG_DSCMB2_TRIG_R                   0x0002UL

#define REG_DSCMB2_ARRAY_RDATA1                 (REG_DSCMB1_BANK+ 0x0058UL)
    #define REG_DSCMB2_ENG0_SHFT                0UL
    #define REG_DSCMB2_ENG1_SHFT                2UL
    #define REG_DSCMB2_ENG2_SHFT                4UL
    #define REG_DSCMB2_ENG3_SHFT                6UL
    #define REG_DSCMB2_ENG_TS                   0x0001UL
    #define REG_DSCMB2_ENG_PES                  0x0002UL

#define REG_DSCMB3_CTRL                         (REG_DSCMB1_BANK + 0x0060UL)

#define REG_DSCMB3_SCMB_TS                      (REG_DSCMB1_BANK + 0x0061UL)

#define REG_DSCMB3_KT_CTRL                      (REG_DSCMB1_BANK + 0x0063UL)
    #define REG_DSCMB3_CIP_ESA_DES_MODE          0x0001UL
    #define REG_DSCMB3_CIP_ESA_AES_MODE          0x0008UL
    #define REG_DSCMB3_CIP_ESA_ECB_MODE          0x0010UL
    #define REG_DSCMB3_CIP_ESA_CBC_MODE          0x0020UL
    #define REG_DSCMB3_CIP_ESA_CBC_CLR_MODE      0x0040UL
    #define REG_DSCMB3_CIP_ESA_DECRYPT           0x0080UL
    #define REG_DSCMB3_CIP_ESA_PES_MODE          0x0800UL
    #define REG_DSCMB3_CIP_ESA_MULTI2_MODE       0x1000UL
    #define REG_DSCMB3_CIP_ESA_CTR_MODE          0x2000UL
    #define REG_DSCMB3_CIP_ESA_CTR_CLR_MODE      0x4000UL
    #define REG_DSCMB3_CIP_ESA_HDCP2_MODE        0x8000UL

#define REG_DSCMB3_MULTI2_ROUND                 (REG_DSCMB1_BANK+ 0x0067UL)

#define REG_CSA3_CTRL                           (REG_DSCMB1_BANK+ 0x0068UL)

#define REG_DSCMB3_KT_CTRL_EXT                  (REG_DSCMB1_BANK+ 0x006DUL)
    #define REG_DSCMB3_ESA_OC_MODE               0x0001UL

#define REG_DSCMB3_MULTI2_SYSKEY_L0             (REG_DSCMB1_BANK+ 0x0070UL) // systemkey 0
#define REG_DSCMB3_MULTI2_SYSKEY_H0             (REG_DSCMB1_BANK+ 0x0072UL)
#define REG_DSCMB3_MULTI2_SYSKEY_L1             (REG_DSCMB1_BANK+ 0x0074UL) // systemkey 1
#define REG_DSCMB3_MULTI2_SYSKEY_H1             (REG_DSCMB1_BANK+ 0x0076UL)
#define REG_DSCMB3_MULTI2_SYSKEY_L2             (REG_DSCMB1_BANK+ 0x0078UL) // systemkey 2
#define REG_DSCMB3_MULTI2_SYSKEY_H2             (REG_DSCMB1_BANK+ 0x007AUL)
#define REG_DSCMB3_MULTI2_SYSKEY_L3             (REG_DSCMB1_BANK+ 0x007CUL) // systemkey 3
#define REG_DSCMB3_MULTI2_SYSKEY_H3             (REG_DSCMB1_BANK+ 0x007EUL)

#endif // #ifndef __REG_DSCMB_H__
