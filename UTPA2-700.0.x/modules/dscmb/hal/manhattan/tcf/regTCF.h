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
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regTCF.h
//  Description: CF, CFKE Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_TCF_H_
#define _REG_TCF_H_


//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// TCF                             Transport Crypto Firewall
// CFKE                            Crypto Firewall Key Engine


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------
//#define TEST_PM_BASE  // temporary solution-- calculate from PM Base
#ifdef TEST_PM_BASE
#define CLKGEN0_BASE                (0x100B00*2) // h100B
#define X32_CRI_BASE                (0x1D0200*2) // h1D02
#define CRI_KL_BASE                 (0x173E00*2) // h173E
#else
#define CLKGEN0_BASE                (0x00B00*2) // h100B
#define X32_CRI_BASE                (0xD0200*2) // h1D02
#define CRI_KL_BASE                 (0x73E00*2) // h173E
#endif

/// CLK
#define REG_CLK_TCF_SEL             (0x09*4)
#define TCF_CLK_54_MHZ              (0x00000001) // [0] 0: XTAL 12Mhz (default); 1: 54Mhz

/// X32_CRI
#define CF_BASE                     (X32_CRI_BASE)
#define CFKE_BASE                   ((X32_CRI_BASE)+(0x40*4))


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct _REG32
{
    volatile MS_U32                 u32Reg;
} REG32;


typedef struct _REG_CFCtrl
{
    REG32               Cf_Status;                                 // (REG_CF_BASE + 0x00*4)
        #define CF_TRANS_STATUS_MASK                0xF0000000     //
        #define CF_NVM_STATUS_MASK                  0x0C000000     //
        #define CF_DIFF_STATUS_MASK                 0x02000000     //
        #define CF_RESERVED_MASK                    0x01FE0000     //
        #define CF_USE_NVMKEY_MASK                  0x00010000     //
        #define CF_OPERATION_TYPE_MASK              0x0000E000     //
        #define CF_DECM_SOURCE_MASK                 0x00001800     //
        #define CF_OUTPUT_USAGE_MASK                0x00000600     //
        #define CF_PRODUCT_RANGE_MASK               0x00000100     //
        #define CF_PRODUCT_OFFSET_MASK              0x000000FF     //
        #define CF_TRANS_STATUS_POS                 28
        #define CF_NVM_STATUS_POS                   26
        #define CF_DIFF_STATUS_POS                  25
        #define CF_RESERVED_POS                     17
        #define CF_USE_NVMKEY_POS                   16
        #define CF_OPERATION_TYPE_POS               13
        #define CF_DECM_SOURCE_POS                  11
        #define CF_OUTPUT_USAGE_POS                 9
        #define CF_PRODUCT_RANGE_POS                8
        #define CF_PRODUCT_OFFSET_POS               0
    REG32               Cf_Reserve01_03[3];
    //REG32               Cf_Reserve02;
    //REG32               Cf_Reserve03;
    REG32               Cf_Input;                                  // (REG_CF_BASE + 0x04*4)
        //
    REG32               Cf_Reserve05_07[3];
    //REG32               Cf_Reserve06;
    //REG32               Cf_Reserve07;
    REG32               Cf_Output;                                 // (REG_CF_BASE + 0x08*4)
        //
    REG32               Cf_Reserve09_0B[3];
    //REG32               Cf_Reserve0A;
    //REG32               Cf_Reserve0B;
    REG32               Cf_Platform;                               // (REG_CF_BASE + 0x0C*4)
        #define CF_PLATFORM_RECENT_RESET_MASK       0x80000000     //
        #define CF_PLATFORM_TRANSACTION_DONE_MASK   0x40000000     //
        #define CF_PLATFORM_CF_ALERT_MASK           0x02000000     //
        #define CF_PLATFORM_HW_DECM_ERROR_MASK      0x01000000     //
        #define CF_PLATFORM_CWC_VALID_MASK          0x00800000     //
        #define CF_PLATFORM_HW_DECM_VALID_MASK      0x00400000     //
        #define CF_PLATFORM_HW_DECM_FLUSH_MASK      0x00200000     //
        #define CF_PLATFORM_CF_ACTIVATED_MASK       0x00020000     //
        #define CF_PLATFORM_DEVELOPMENT_MODE_MASK   0x00010000 //~ Note: 15 in Spec.  ~//   //
        #define CF_PLATFORM_RECENT_RESET_POS        31
        #define CF_PLATFORM_TRANSACTION_DONE_POS    30
        #define CF_PLATFORM_CF_ALERT_POS            25
        #define CF_PLATFORM_HW_DECM_ERROR_POS       24
        #define CF_PLATFORM_CWC_VALID_POS           23
        #define CF_PLATFORM_HW_DECM_VALID_POS       22
        #define CF_PLATFORM_HW_DECM_FLUSH_POS       21
        #define CF_PLATFORM_CF_ACTIVATED_POS        17
        #define CF_PLATFORM_DEVELOPMENT_MODE_POS    16 //~ Note: 15 in Spec.  ~//
    REG32               Cf_Reserve0D_0F[3];
    //REG32               Cf_Reserve0E;
    //REG32               Cf_Reserve0F;
    REG32               Cf_Feature;                                // (REG_CF_BASE + 0x10*4)
        //
	REG32               Cf_Reserve11_13[3];
    //REG32               Cf_Reserve12;
    //REG32               Cf_Reserve13;
    REG32               Cf_Version;                                // (REG_CF_BASE + 0x14*4)
        #define CF_VERSION_MANUFACTURER_ID_MASK     0x07000000     //
        #define CF_VERSION_NETLIST_VERSION_MASK     0x003F0000     //
        #define CF_VERSION_VERSION_EPOCH_MASK       0x00000F00     //
        #define CF_VERSION_BUILD_ID_MASK            0x000000FF     //
        #define CF_VERSION_MANUFACTURER_ID_POS      24
        #define CF_VERSION_NETLIST_VERSION_POS      16
        #define CF_VERSION_VERSION_EPOCH_POS        8
        #define CF_VERSION_BUILD_ID_POS             0
    REG32               Cf_Reserve15_17[3];
    //REG32               Cf_Reserve16;
    //REG32               Cf_Reserve17;
    REG32               Cf_Perso_Config;                           // (REG_CF_BASE + 0x18*4)
        //
}REG_CFCtrl;


typedef struct _REG_CFKECtrl
{
    REG32               Cfke_Command;                              // (REG_CFB_BASE + 0x40*4)
        #define CFKE_CMD_OP_MASK                    0x0000F000     // cfkeCmdOp
        #define CFKE_CMD_DEST_MASK                  0x00000F00     // cfkeCmdDest
        #define CFKE_CMD_SRC1_MASK                  0x000000F0     // cfkeCmdSrc1
        #define CFKE_CMD_SRC2_MASK                  0x0000000F     // cfkeCmdSrc2
        #define CFKE_CMD_OP_POS                     12
        #define CFKE_CMD_DEST_POS                   8
        #define CFKE_CMD_SRC1_POS                   4
        #define CFKE_CMD_SRC2_POS                   0
    REG32               Cfke_Reserve41_43[3];
    //REG32               Cf_Reserve42;
    //REG32               Cf_Reserve43;
    REG32               Cfke_Status;                               // (REG_CFB_BASE + 0x44*4)
        #define CFKE_OPERATION_STATUS_MASK          0xF0000000     // cfkeOperationStatus
        #define CFKE_OPERATION_COUNT_MASK           0x0FF00000     // cfkeOperationCount
        #define CFKE_ERROR_CODE_MASK                0x000F0000     // cfkeErrorCode
        #define CFKE_OPERATION_TYPE_MASK            0x0000E000     // cfkeOperationType
        #define CFKR_RESERVED_MASK                  0x00001FF0     // reserved
        #define CFKE_DIFF_ALLOW_NON_UNW_MASK        0x00000008     // cfkeDiffAllowNonUnw
        #define CFKE_DIFF_ALLOW_CK_MASK             0x00000004     // cfkeDiffAllowCk
        #define CFKE_DIFF_CONTENT_OUTPUT_ALLOWED_MASK 0x00000003   // cfkeDiffContentOutputAllowed
        #define CFKE_OPERATION_STATUS_POS           28
        #define CFKE_OPERATION_COUNT_POS            20
        #define CFKE_ERROR_CODE_POS                 16
        #define CFKE_OPERATION_TYPE_POS             13
        #define CFKE_RESERVED_POS                   4
        #define CFKE_DIFF_ALLOW_NON_UNW_POS         3
        #define CFKE_DIFF_ALLOW_CK_POS              2
        #define CFKE_DIFF_CONTENT_OUTPUT_ALLOWED_POS 0
    REG32               Cfke_Reserve45_47[3];
    //REG32               Cf_Reserve46;
    //REG32               Cf_Reserve47;
    REG32               Cfke_Platform;                             // (REG_CFB_BASE + 0x48*4)
        #define CFKE_TRANSACTION_DONE_MASK          0x40000000     // cfkeTransactionDone
        #define CK_OUTPUT_ALLOWED_MASK              0x00000600     // ckOutputAllowed
        #define CK_VALID_MASK                       0x00000100     // ckValid
        #define CW_OUTPUT_ALLOWED_MASK              0x00000006     // ckOutputAllowed
        #define CW_VALID_MASK                       0x00000001     // ckValid
        #define CFKE_TRANSACTION_DONE_POS           30
        #define CK_OUTPUT_ALLOWED_POS               9
        #define CK_VALID_POS                        8
        #define CW_OUTPUT_ALLOWED_POS               1
        #define CW_VALID_POS                        0
    REG32               Cfke_Reserve49_4B[3];
    //REG32               Cf_Reserve4A;
    //REG32               Cf_Reserve4B;
    REG32               Cfke_Unw_Path;                             // (REG_CFB_BASE + 0x4C*4)
        //
    REG32               Cfke_Reserve4D_4F[3];
    //REG32               Cf_Reserve4E;
    //REG32               Cf_Reserve4F;

    REG32               Cfke_Validator_0;                          // (REG_CFB_BASE + 0x50*4)
        //
    REG32               Cfke_Reserve51_53[3];
    //REG32               Cf_Reserve52;
    //REG32               Cf_Reserve53;
    REG32               Cfke_Validator_1;                          // (REG_CFB_BASE + 0x54*4)
        //
    REG32               Cfke_Reserve55_57[3];
    //REG32               Cf_Reserve56;
    //REG32               Cf_Reserve57;

    REG32               Cfke_Slot_A;                               // (REG_CFB_BASE + 0x58*4)
        //
    REG32               Cfke_Reserve59_5B[3];
    //REG32               Cf_Reserve5A;
    //REG32               Cf_Reserve5B;
    REG32               Cfke_Slot_B;                               // (REG_CFB_BASE + 0x5C*4)
        //
    REG32               Cfke_Reserve5D_5F[3];
    //REG32               Cf_Reserve5E;
    //REG32               Cf_Reserve5F;
    REG32               Cfke_Slot_C;                               // (REG_CFB_BASE + 0x60*4)
        //
    REG32               Cfke_Reserve61_63[3];
    //REG32               Cf_Reserve62;
    //REG32               Cf_Reserve63;
    REG32               Cfke_Slot_D;                               // (REG_CFB_BASE + 0x64*4)
        //
    REG32               Cfke_Reserve65_67[3];
    //REG32               Cf_Reserve66;
    //REG32               Cf_Reserve67;

    REG32               Cfke_User_Hash;                            // (REG_CFB_BASE + 0x68*4)
        //
}REG_CFKECtrl;


typedef struct _REG_CRIKLCtrl
{
    REG32               reg_cfstatusout_l;                         // 0x00
    REG32               reg_cfstatusout_h;                         // 0x01

    REG32               reg_cfkestatusout_l;                       // 0x02
    REG32               reg_cfkestatusout_h;                       // 0x03

    REG32               reg_cfplatformout_l;                       // 0x04
    REG32               reg_cfplatformout_h;                       // 0x05

    REG32               reg_cffeatureout_l;                        // 0x06
    REG32               reg_cffeatureout_h;                        // 0x07

    REG32               reg_transaction;                           // 0x08
        #define CRI_KT_CF_TRANSACTION_DONE          0x0001
        #define CRI_KT_CFKE_TRANSACTION_DONE        0x0002
        #define CRI_KT_CF_TRAN_INT                  0x0004
        #define CRI_KT_CFKE_TRAN_INT                0x0008
        #define CRI_KT_CF_TRAN_INT_MASK             0x0010
        #define CRI_KT_CFKE_TRAN_INT_MASK           0x0020
        #define CRI_KT_PROCTION_MODE                0x0100
    //
    REG32               reg_cri_kt;                                // 0x09
        #define CRI_KT_INT                          0x0001
        #define CRI_KT_INT_NONSEC                   0x0002
        #define CRI_KT_INT_MASK                     0x0010
        #define CRI_KT_INT_MASK_NONSEC              0x0020
        #define CRI_KT_CLR_ERR                      0x0100
        #define CRI_KT_ERR_FLAG                     0xF000
    REG32               reg_cri_kt_index;                          // 0x0a
        #define CRI_KT_SCMB_CTL_POS                 4

    REG32               reg_cri_kt_para_15_00;                     // 0x0b
        #define CRI_KT_DEST_AESDMA_POS              12
        #define CRI_KT_DEST_PVR_POS                 11
        #define CRI_KT_DEST_CIPHER_POS              10
        #define CRI_KT_ENCRYPT_POS                  9
        #define CRI_KT_DECRYPT_POS                  8
        #define CRI_KT_KTE_3                        7
        #define CRI_KT_KTE_2                        6
        #define CRI_KT_KTE_1                        5
        #define CRI_KT_KTE_0                        4
        #define CRI_KT_LOCK                         3
        #define CRI_KT_PARA_VLA                     2
        #define CRI_KT_KL_EN                        0
    REG32               reg_cri_kt_para_31_16;                     // 0x0c
        #define CRI_KT_DROP_MASK                    0x00F0
        #define CRI_KT_CNT_MASK                     0x000F
        #define CRI_KT_ENG_NPD_POS                  15
        #define CRI_KT_ENG_MULTI2_POS               14
        #define CRI_KT_ENG_CSAV2_POS                13
        #define CRI_KT_ENG_CSAV3_POS                12
        #define CRI_KT_ENG_ASA_POS                  11
        #define CRI_KT_ENG_DES_POS                  10
        #define CRI_KT_ENG_TDES_POS                 9
        #define CRI_KT_ENG_AES_POS                  8
        #define CRI_KT_DROP_POS                     4
        #define CRI_KT_CNT_POS                      0
    REG32               reg_cri_kt_para_47_32;                     // 0x0d
        #define CRI_KT_VENDOR_ATT_L_POS             14
        #define CRI_KT_OUTPUT_ALLOWED_POS           12
        #define CRI_KT_MULTI_ENG_POS                5
        #define CRI_KT_LSB64_POS                    4
        #define CRI_KT_KEY_SIZE_256_POS             3
        #define CRI_KT_KEY_SIZE_192_POS             2
        #define CRI_KT_KEY_SIZE_128_POS             1
        #define CRI_KT_KEY_SIZE_64_POS              0
        // VendorAtt
        #define CRI_KT_VENDOR_ATT_L_VAL_MASK        0x0003
    REG32               reg_cri_kt_para_63_48;                     // 0x0e
        #define CRI_KT_VENDOR_NO_POS                12
        #define CRI_KT_VENDOR_ATT_H_POS             0
        // VendorAtt
        #define CRI_KT_VENDOR_ATT_H_VAL_MASK        0xFFFC
        #define CRI_KT_VENDOR_ATT_L_OFFSET          2

    REG32               reg_reserve_1d_0f[15];                     // 0x1d~0xf

    REG32               reg_cri_kt_dbg;                            // 0x1e
        #define CRI_KT_DBG_SEL                      0x00FF
        #define CRI_KT_BRIDGE_DBG_L                 0xFF00
    REG32               reg_cri_kt_bridge_debug;                   // 0x1f
        #define CRI_KT_BRIDGE_DBG_H                 0xFFFF
    //
    REG32               reg_cri_ks;                                // 0x20
        #define CRI_KS_INT                          0x0001
        #define CRI_KS_INT_NONSEC                   0x0002
        #define CRI_KS_INT_MASK                     0x0010
        #define CRI_KS_INT_MASK_NONSEC              0x0020
        #define CRI_KS_CLR_ERR                      0x0100
        #define CRI_KS_ERR_FLAG                     0xF000
    REG32               reg_cri_ks_index;                          // 0x21
        #define CRI_KS_IDX_VAL_MASK                 0x001F

    REG32               reg_cri_ks_para_15_00;                     // 0x22
        #define CRI_KS_DEST_AESDMA_POS              12
        #define CRI_KS_DEST_PVR_POS                 11
        #define CRI_KS_DEST_CIPHER_POS              10
        #define CRI_KS_ENCRYPT_POS                  9
        #define CRI_KS_DECRYPT_POS                  8
        #define CRI_KS_KTE_3                        7
        #define CRI_KS_KTE_2                        6
        #define CRI_KS_KTE_1                        5
        #define CRI_KS_KTE_0                        4
        #define CRI_KS_LOCK                         3
        #define CRI_KS_PARA_VLA                     2
        #define CRI_KS_KL_EN                        0
    REG32               reg_cri_ks_para_31_16;                     // 0x23
        #define CRI_KS_DROP_MASK                    0x00F0
        #define CRI_KS_CNT_MASK                     0x000F
        #define CRI_KS_ENG_NPD_POS                  15
        #define CRI_KS_ENG_MULTI2_POS               14
        #define CRI_KS_ENG_CSAV2_POS                13
        #define CRI_KS_ENG_CSAV3_POS                12
        #define CRI_KS_ENG_ASA_POS                  11
        #define CRI_KS_ENG_DES_POS                  10
        #define CRI_KS_ENG_TDES_POS                 9
        #define CRI_KS_ENG_AES_POS                  8
        #define CRI_KS_DROP_POS                     4
        #define CRI_KS_CNT_POS                      0
    REG32               reg_cri_ks_para_47_32;                     // 0x24
        #define CRI_KS_VENDOR_ATT_L_POS             14
        #define CRI_KS_OUTPUT_ALLOWED_POS           12
        #define CRI_KS_MULTI_ENG_POS                5
        #define CRI_KS_LSB64_POS                    4
        #define CRI_KS_KEY_SIZE_256_POS             3
        #define CRI_KS_KEY_SIZE_192_POS             2
        #define CRI_KS_KEY_SIZE_128_POS             1
        #define CRI_KS_KEY_SIZE_64_POS              0
        // VendorAtt
        #define CRI_KS_VENDOR_ATT_L_VAL_MASK        0x0003
    REG32               reg_cri_ks_para_63_48;                     // 0x25
        #define CRI_KS_VENDOR_NO_POS                12
        #define CRI_KS_VENDOR_ATT_H_POS             0
        // VendorAtt
        #define CRI_KS_VENDOR_ATT_H_VAL_MASK        0xFFFC
        #define CRI_KS_VENDOR_ATT_L_OFFSET          2

    REG32               reg_cri_ks_dbg;                            // 0x26
        #define CRI_KS_DBG_SEL                      0x00FF
        #define CRI_KS_BRIDGE_DBG_L                 0xFF00
    REG32               reg_cri_ks_bridge_debug;                   // 0x27
        #define CRI_KS_BRIDGE_DBG_H                 0xFFFF
}REG_CRIKLCtrl;

#endif // #ifndef _REG_TCF_H_
