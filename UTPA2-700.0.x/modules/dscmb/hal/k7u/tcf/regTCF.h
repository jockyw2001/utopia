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
//  Description: TCF Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_TCF_H_
#define _REG_TCF_H_


//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// TCF                             Transport Crypto Firewall


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_CLK_CF_BASE             (0x03300*2)  // h1033
//#define REG_CLK_TSP_BASE            (0x00b00*2)  // h100b
#define REG_CLK_GEN0_BASE           (0x00B00*2) //h100B
#define REG_CFB_BASE                (0xA0E00*2)  // h1A0E
#define REG_CF_BASE                 (0xA0F00*2)  // h1A0F

#define REG_CLK_CF                  (0x24*4)
#define ENABLE_48_MHZ_CF_CLK_MASK   0x00000010  //[4] 0: enable 48MHz TCG clock
#define ENABLE_48_MHZ_CF_CLK        0x00000000

#define REG_CLK_CFTOP_P_SEL         (0x31*4)
#define ENABLE_CFTOP_P_SEL_MASK     0x00000300  //[9:8] 00: xtail 12M clock, 01: 54M clock, others: N/A
#define ENABLE_CFTOP_P_12M_CLK      0x00000000
#define ENABLE_CFTOP_P_54M_CLK      0x00000100

//#define REG_CLK_TSP                 (0x2a*4)
//#define SET_192_MHZ_TSP_CLK_MASK    0x0000001F  //[4;0] 8: set 192MHz TSP clock
//#define SET_192_MHZ_TSP_CLK         0x00000008


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


typedef struct _REG_CFBCtrl
{
    REG32               Cfb_Use_Case;                              // (REG_CFB_BASE + 0x00*4)
        #define REG_USE_CASE_MASK                   0x0000000F
        #define REG_RESET_CFB                       0x00000010
        #define REG_INITIAL_SM                      0x00000020
        #define REG_CFB_INT_CLR                     0x00000040
        #define REG_TCF_KEY_SEL_MASK                0x00000F00
        #define REG_CFB_CACWC_SEL                0x00010000
    REG32               Cfb_Status;                                // (REG_CFB_BASE + 0x01*4)
        #define REG_CFB_DONE                        0x00000002
        #define REG_CFB_READY                       0x00000004
        #define REG_AES_DONE                        0x00000008
        //...
        #define REG_FORBID_TCF_KEY_OP               0x00000020
        #define REG_TCF_CWC_WR_DONE_LTH             0x00000040
        #define REG_TCF_CWC_WR_RESP_LTH             0x00001F00
    REG32               Cfb_State;                                 // (REG_CFB_BASE + 0x02*4)
        #define REG_CF_STATE                        0x000000FF

    REG32               Cfb_Reserve;

    REG32               Cfb_Cacwc0;                                // (REG_CFB_BASE + 0x04*4)
    REG32               Cfb_Cacwc1;                                // (REG_CFB_BASE + 0x05*4)
    REG32               Cfb_Cacwc2;                                // (REG_CFB_BASE + 0x06*4)
    REG32               Cfb_Cacwc3;                                // (REG_CFB_BASE + 0x07*4)
    REG32               Cfb_Epk0;                                  // (REG_CFB_BASE + 0x08*4)
    REG32               Cfb_Epk1;                                  // (REG_CFB_BASE + 0x09*4)
    REG32               Cfb_Epk2;                                  // (REG_CFB_BASE + 0x0A*4)
    REG32               Cfb_Epk3;                                  // (REG_CFB_BASE + 0x0B*4)
    REG32               Cfb_Efuv0;                                 // (REG_CFB_BASE + 0x0C*4)
    REG32               Cfb_Efuv1;                                 // (REG_CFB_BASE + 0x0D*4)
    REG32               Cfb_Efuv2;                                 // (REG_CFB_BASE + 0x0E*4)
    REG32               Cfb_Efuv3;                                 // (REG_CFB_BASE + 0x0F*4)

    REG32               Cfb_Tcf_Key_Otp;                           // (REG_CFB_BASE + 0x10*4)
        #define REG_TCF_CWC_TSID                    0x00000F00
        #define REG_TCF_CWC_SCB                     0x00030000
        #define REG_TCF_CWC_FSCB                    0x000C0000
        #define REG_TCF_CWC_FLD                     0x00300000
        #define REG_TCF_CWC_PID                     0xFF000000
    REG32               Cfb_Key_Dst;                           // (REG_CFB_BASE + 0x11*4)
    REG32               Cfb_Reserve11_5F[0x4E];                   // (REG_CFB_BASE + (0x12~0x5F)*4)
    REG32               Cfb_Btw[0x7];                             // (REG_CFB_BASE + 0x60*4)
    REG32               Cfb_Reserve67_6F[0x9];                    // (REG_CFB_BASE + (0x67~0x6F)*4)
    REG32               Cfb_TcfProductionMode;                    // (REG_CFB_BASE + 0x70*4)
        #define REG_TCF_PRODUCTION_MODE             0x00000001
}REG_CFBCtrl;

#endif // #ifndef _REG_TCF_H_
