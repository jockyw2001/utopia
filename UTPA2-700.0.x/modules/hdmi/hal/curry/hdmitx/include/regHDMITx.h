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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regHDMITx.h
/// @author MStar Semiconductor,Inc.
/// @brief  HDMITx Register Definition
/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_HDMITX_H_
#define _REG_HDMITX_H_

//#include "MsCommon.h"

//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define HDMITX_MISC_REG_BASE            (0x172A00U)
#define HDMITX_HDCP_REG_BASE            (0x172B00U)
#define HDMITX_REG_BASE                 (0x172C00U)
#define HDMITX_VIDEO_REG_BASE           (0x172D00U)
#define HDMITX_AUDIO_REG_BASE           (0x172E00U)
#define HDMITX_PHY_REG_BASE             (0x173000U)

#define HDMITX_SECUTZPC_BASE            (0x173F00U)
#define HDMITX_HDCP2TX_BASE             (0x172F00U)

#define HDMITX_2_REG_BASE               (0x173800U)

#define HDMIRX_COMBOPHY0_REG_BASE       (0x172800U)
#define HDMIRX_COMBOPHY1_REG_BASE       (0x172900U)

#define PMBK_PMSLEEP_REG_BASE           (0x000E00U)
#define CLKGEN1_REG_BASE                (0x103300U)
#define CHIPTOP_REG_BASE                (0x101E00U)
#define HDCP_REG_BASE                   (0x172500U)

//***** Bank 1728 - COMBO PHY 0 *****//
#define REG_COMBOPHY1_CONFIG_3C         0x3CU

//***** Bank 1729 - COMBO PHY 1 *****//
#define REG_COMBOPHY0_CONFIG_4C         0x4CU
#define REG_COMBOPHY0_CONFIG_4B         0x4BU
#define REG_COMBOPHY0_CONFIG_4A         0x4AU
#define REG_COMBOPHY0_CONFIG_49         0x49U
#define REG_COMBOPHY0_CONFIG_09         0x09U
#define REG_COMBOPHY0_CONFIG_22         0x22U

//***** Bank 1026 - PADTOP *****//
#define REG_SYNC_GPIO0                  0x1EU

//***** Bank 1033(0x28) - CHIPTOP *****//
#define REG_CKG_HDMITx_CLK_28           0x28U
#define REG_I2S_GPIO4                         0x1BU

//***** Bank 172C - HDMITX *****//
#define REG_HDMI_CONFIG1_00             0x00U
#define REG_ACT_HDMI_PKTS_CMD_01        0x01U
#define REG_PKT_NUL_CFG_02              0x02U
#define REG_PKT_GC_CFG_03               0x03U
#define REG_PKT_GC12_04                 0x04U
#define REG_PKT_ACR_1_05                0x05U
#define REG_PKT_ACR_2_06                0x06U
#define REG_PKT_ACR_3_07                0x07U
#define REG_PKT_ACR_CFG_08              0x08U
#define REG_PKT_AVI_1_09                0x09U
#define REG_PKT_AVI_2_0A                0x0AU
#define REG_PKT_AVI_3_0B                0x0BU
#define REG_PKT_AVI_4_0C                0x0CU
#define REG_PKT_AVI_5_0D                0x0DU
#define REG_PKT_AVI_6_0E                0x0EU
#define REG_PKT_AVI_7_0F                0x0FU
#define REG_PKT_AVI_CFG_10              0x10U
#define REG_PKT_AUD_1_11                0x11U
#define REG_PKT_AUD_2_12                0x12U
#define REG_PKT_AUD_3_13                0x13U
#define REG_PKT_AUD_CFG_14              0x14U
#define REG_PKT_SPD_1_15                0x15U
#define REG_PKT_SPD_2_16                0x16U
#define REG_PKT_SPD_3_17                0x17U
#define REG_PKT_SPD_4_18                0x18U
#define REG_PKT_SPD_5_19                0x19U
#define REG_PKT_SPD_6_1A                0x1AU
#define REG_PKT_SPD_7_1B                0x1BU
#define REG_PKT_SPD_8_1C                0x1CU
#define REG_PKT_SPD_9_1D                0x1DU
#define REG_PKT_SPD_10_1E               0x1EU
#define REG_PKT_SPD_11_1F               0x1FU
#define REG_PKT_SPD_12_20               0x20U
#define REG_PKT_SPD_13_21               0x21U
#define REG_PKT_SPD_CFG_22              0x22U
#define REG_PKT_MPG_1_23                0x23U
#define REG_PKT_MPG_2_24                0x24U
#define REG_PKT_MPG_3_25                0x25U
#define REG_PKT_MPG_CFG_26              0x26U
#define REG_PKT_VS_1_27                 0x27U
#define REG_PKT_VS_2_28                 0x28U
#define REG_PKT_VS_3_29                 0x29U
#define REG_PKT_VS_4_2A                 0x2AU
#define REG_PKT_VS_5_2B                 0x2BU
#define REG_PKT_VS_6_2C                 0x2CU
#define REG_PKT_VS_7_2D                 0x2DU
#define REG_PKT_VS_8_2E                 0x2EU
#define REG_PKT_VS_9_2F                 0x2FU
#define REG_PKT_VS_10_30                0x30U
#define REG_PKT_VS_11_31                0x31U
#define REG_PKT_VS_12_32                0x32U
#define REG_PKT_VS_13_33                0x33U
#define REG_PKT_VS_14_34                0x34U
#define REG_PKT_VS_CFG_35               0x35U
#define REG_USER_TYPE_36                0x36U
#define REG_USER_HB_37                  0x37U
#define REG_PKT_ACP_0_38                0x38U
#define REG_PKT_ACP_1_39                0x39U
#define REG_PKT_ACP_2_3A                0x3AU
#define REG_PKT_ACP_3_3B                0x3BU
#define REG_PKT_ACP_4_3C                0x3CU
#define REG_PKT_ACP_5_3D                0x3DU
#define REG_PKT_ACP_6_3E                0x3EU
#define REG_PKT_ACP_7_3F                0x3FU
#define REG_PKT_ACP_CFG_40              0x40U
#define REG_PKT_ISRC_0_41               0x41U
#define REG_PKT_ISRC_1_42               0x42U
#define REG_PKT_ISRC_2_43               0x43U
#define REG_PKT_ISRC_3_44               0x44U
#define REG_PKT_ISRC_4_45               0x45U
#define REG_PKT_ISRC_5_46               0x46U
#define REG_PKT_ISRC_6_47               0x47U
#define REG_PKT_ISRC_7_48               0x48U
#define REG_PKT_ISRC_8_49               0x49U
#define REG_PKT_ISRC_9_4A               0x4AU
#define REG_PKT_ISRC_10_4B              0x4BU
#define REG_PKT_ISRC_11_4C              0x4CU
#define REG_PKT_ISRC_12_4D              0x4DU
#define REG_PKT_ISRC_13_4E              0x4EU
#define REG_PKT_ISRC_14_4F              0x4FU
#define REG_PKT_ISRC_15_50              0x50U
#define REG_PKT_ISRC_CFG_51             0x51U
#define REG_TMDS_DE_CNT_52              0x52U
#define REG_HPLL_LOCK_CNT_53            0x53U
#define REG_PKT_GM_CFG_54               0x54U
#define REG_PKT_GM_HB2_55               0x55U
#define REG_PKT_GM_1_56                 0x56U
#define REG_PKT_GM_3_57                 0x57U
#define REG_PKT_GM_5_58                 0x58U
#define REG_PKT_GM_7_59                 0x59U
#define REG_PKT_GM_9_5A                 0x5AU
#define REG_PKT_GM_11_5B                0x5BU
#define REG_PKT_GM_13_5C                0x5CU
#define REG_PKT_GM_15_5D                0x5DU
#define REG_PKT_GM_17_5E                0x5EU
#define REG_PKT_GM_19_5F                0x5FU
#define REG_PKT_N_PKT_60                0x60U
#define REG_PKT_N_PKT_61                0x61U

//***** Bank 172D - HDMITX_Video *****//
#define REG_VE_CONFIG_00                   0x00U
#define REG_VE_CONFIG_01                   0x01U
#define REG_VE_CONFIG_02                   0x02U
#define REG_VE_CONFIG_03                   0x03U
#define REG_VE_CONFIG_04                   0x04U
#define REG_VE_CONFIG_05                   0x05U
#define REG_VE_CONFIG_06                   0x06U
#define REG_VE_CONFIG_07                   0x07U
#define REG_VE_CONFIG_08                   0x08U
#define REG_VE_CONFIG_09                   0x09U
#define REG_VE_CONFIG_0A                   0x0AU
#define REG_VE_CONFIG_0B                   0x0BU
#define REG_VE_CONFIG_0C                   0x0CU
#define REG_VE_CONFIG_0D                   0x0DU
#define REG_VE_CONFIG_0E                   0x0EU
#define REG_VE_CONFIG_0F                   0x0FU
#define REG_VE_CONFIG_10                   0x10U
#define REG_VE_CONFIG_11                   0x11U
#define REG_VE_CONFIG_12                   0x12U
#define REG_VE_CONFIG_13                   0x13U
#define REG_VE_CONFIG_14                   0x14U
#define REG_VE_STATUS_15                   0x15U
#define REG_VE_STATUS_16                   0x16U
#define REG_VE_CONFIG_17                   0x17U
#define REG_VE_CONFIG_18                   0x18U
#define REG_VE_CONFIG_20                   0x20U
#define REG_VE_CONFIG_21                   0x21U
#define REG_VE_CONFIG_22                   0x22U
#define REG_VE_CONFIG_23                   0x23U
#define REG_VE_CONFIG_24                   0x24U
#define REG_VE_CONFIG_25                   0x25U
#define REG_VE_CONFIG_26                   0x26U
#define REG_VE_CONFIG_27                   0x27U
#define REG_VE_CONFIG_28                   0x28U
#define REG_VE_CONFIG_29                   0x29U
#define REG_VE_CONFIG_2A                   0x2AU
#define REG_VE_CONFIG_30                   0x30U
#define REG_VE_CONFIG_31                   0x31U
#define REG_VE_CONFIG_32                   0x32U
#define REG_VE_CONFIG_33                   0x33U
#define REG_VE_CONFIG_34                   0x34U
#define REG_VE_CONFIG_35                   0x35U
#define REG_VE_CONFIG_36                   0x36U
#define REG_VE_CONFIG_37                   0x37U
#define REG_VE_CONFIG_38                   0x38U
#define REG_VE_CONFIG_39                   0x39U
#define REG_VE_CONFIG_3A                   0x3AU
#define REG_VE_CONFIG_3B                   0x3BU
#define REG_VE_CONFIG_3C                   0x3CU
#define REG_VE_CONFIG_3D                   0x3DU

//***** Bank 172E - HDMITX_Audio *****//
#define REG_AE_CH_STATUS0_00           0x00U
#define REG_AE_CH_STATUS1_01           0x01U
#define REG_AE_CH_STATUS2_02           0x02U
#define REG_AE_CH_STATUS3_03           0x03U
#define REG_AE_CH_STATUS4_04           0x04U
#define REG_AE_CONFIG_05                   0x05U
#define REG_AE_STATUS_06                   0x06U
#define REG_AE_STATUS_07                   0x07U
#define REG_AE_CH_STATUS0_0A           0x0AU
#define REG_AE_CH_STATUS1_0B           0x0BU
#define REG_AE_CH_STATUS2_0C           0x0CU
#define REG_AE_CH_STATUS3_0D           0x0DU
#define REG_AE_CH_STATUS4_0E           0x0EU


//***** Bank 172A - MISC *****//
#define REG_MISC_CONFIG_00               0x00U
#define REG_MISC_CONFIG_01               0x01U
#define REG_MISC_CONFIG_02               0x02U
#define REG_MISC_CONFIG_03               0x03U
#define REG_MISC_CONFIG_04               0x04U
#define REG_MISC_CONFIG_05               0x05U
#define REG_MISC_CONFIG_06               0x06U
#define REG_MISC_CONFIG_07               0x07U
#define REG_MISC_CONFIG_08               0x08U
#define REG_MISC_CONFIG_09               0x09U
#define REG_MISC_STATUS_0A               0x0AU
#define REG_MISC_STATUS_0B               0x0BU
#define REG_MISC_CONFIG_0C               0x0CU
#define REG_MISC_STATUS_0D               0x0DU
#define REG_MISC_STATUS_0E               0x0EU
#define REG_MISC_STATUS_0F               0x0FU
#define REG_MISC_CONFIG_17               0x17U
#define REG_MISC_CONFIG_1B               0x1BU
#define REG_MISC_CONFIG_1C               0x1CU
#define REG_MISC_CONFIG_1D               0x1DU
#define REG_MISC_CONFIG_1E               0x1EU
#define REG_MISC_CONFIG_1F               0x1FU
#define REG_MISC_CONFIG_20               0x20U
#define REG_MISC_CONFIG_21               0x21U
#define REG_MISC_CONFIG_22               0x22U
#define REG_MISC_CONFIG_23               0x23U
#define REG_MISC_CONFIG_24               0x24U
#define REG_MISC_CONFIG_25               0x25U
#define REG_MISC_CONFIG_26               0x26U
#define REG_MISC_CONFIG_27               0x27U
#define REG_MISC_CONFIG_2A               0x2AU
#define REG_MISC_CONFIG_2B               0x2BU
#define REG_MISC_CONFIG_2C               0x2CU
#define REG_MISC_CONFIG_2D               0x2DU
#define REG_MISC_CONFIG_2E               0x2EU
#define REG_MISC_CONFIG_2F               0x2FU
#define REG_MISC_CONFIG_33               0x33U
#define REG_MISC_CONFIG_34               0x34U
#define REG_MISC_CONFIG_36               0x36U
#define REG_MISC_CONFIG_38               0x38U
#define REG_MISC_CONFIG_40               0x40U
#define REG_MISC_CONFIG_41               0x41U
#define REG_MISC_CONFIG_45               0x45U
#define REG_MISC_CONFIG_48               0x48U
#define REG_MISC_CONFIG_4D               0x4DU
#define REG_MISC_CONFIG_52               0x52U
#define REG_MISC_CONFIG_58               0x58U
#define REG_MISC_CONFIG_59               0x59U
#define REG_MISC_CONFIG_5D               0x5DU


//***** Bank 172B - HDCP *****//
#define REG_HDCP_TX_RI_00               0x00U
#define REG_HDCP_TX_MODE_01             0x01U	// Pj[7:0] : 61h[7:0]; Tx_mode[7:0] : 61h[15:8]
#define REG_HDCP_TX_COMMAND_02          0x02U
#define REG_HDCP_TX_RI127_03            0x03U       // RI[15:0] 127th frame : 63[15:0]
#define REG_HDCP_TX_LN_04               0x04U	// Ln[55:0] : 64h[7:0] ~ 67h[7:0]
#define REG_HDCP_TX_LN_SEED_07          0x07U	// Ln seed[7:0] : 67h[15:8]
#define REG_HDCP_TX_AN_08               0x08U	// An[63:0] : 68[7:0] ~ 6B[15:8]
#define REG_HDCP_TX_MI_0C               0x0CU	// Mi[63:0] : 6C[7:0] ~ 6F[15:8]

//***** Bank 1730 - HDMI PHY *****//
#define REG_HDMITxPHY_CONFIG_01                   0x01U
#define REG_HDMITxPHY_CONFIG_02                   0x02U
#define REG_HDMITxPHY_CONFIG_03                   0x03U
#define REG_HDMITxPHY_CONFIG_05                   0x05U
#define REG_HDMITxPHY_CONFIG_06                   0x06U
#define REG_HDMITxPHY_CONFIG_07                   0x07U
#define REG_HDMITxPHY_CONFIG_0F                   0x0FU
#define REG_HDMITxPHY_CONFIG_10                   0x10U
#define REG_HDMITxPHY_CONFIG_11                   0x11U
#define REG_HDMITxPHY_CONFIG_15                   0x15U
#define REG_HDMITxPHY_CONFIG_16                   0x16U
#define REG_HDMITxPHY_CONFIG_17                   0x17U
#define REG_HDMITxPHY_CONFIG_18                   0x18U
#define REG_HDMITxPHY_CONFIG_19                   0x19U
#define REG_HDMITxPHY_CONFIG_26                   0x26U
#define REG_HDMITxPHY_CONFIG_2E                   0x2EU
#define REG_HDMITxPHY_CONFIG_30                   0x30U
#define REG_HDMITxPHY_CONFIG_31                   0x31U
#define REG_HDMITxPHY_CONFIG_32                   0x32U
#define REG_HDMITxPHY_CONFIG_33                   0x33U
#define REG_HDMITxPHY_CONFIG_34                   0x34U
#define REG_HDMITxPHY_CONFIG_35                   0x35U
#define REG_HDMITxPHY_CONFIG_36                   0x36U
#define REG_HDMITxPHY_CONFIG_37                   0x37U
#define REG_HDMITxPHY_CONFIG_38                   0x38U
#define REG_HDMITxPHY_CONFIG_39                   0x39U
#define REG_HDMITxPHY_CONFIG_3A                   0x3AU
#define REG_HDMITxPHY_CONFIG_3C                   0x3CU
#define REG_HDMITxPHY_CONFIG_3D                   0x3DU
#define REG_HDMITxPHY_CONFIG_3F                   0x3FU
#define REG_HDMITxPHY_CONFIG_41                   0x41U
#define REG_HDMITxPHY_CONFIG_42                   0x42U
#define REG_HDMITxPHY_CONFIG_46                   0x46U
#define REG_HDMITxPHY_CONFIG_60                   0x60U
#define REG_HDMITxPHY_CONFIG_79                   0x79U
#define REG_HDMITxPHY_CONFIG_7A                   0x7AU
#define REG_HDMITxPHY_CONFIG_7E                   0x7EU

//***** Bank 1738 - HDMITX 2 *****//
#define REG_HDMI_2_CONFIG_00                    0x00U
#define REG_HDMI_2_CONFIG_10                    0x10U
#define REG_HDMI_2_CONFIG_1D                    0x1DU
#define REG_HDMI_2_CONFIG_1E                    0x1EU
#define REG_HDMI_2_CONFIG_1F                    0x1FU

//***** Bank 0E - PM_SLEEP *****//
#define REG_PM_SLP_0F                   0x0FU
#define REG_PM_SLP_10                   0x10U
#define REG_PM_SLP_12                   0x12U
#define REG_PM_SLP_20                   0x20U
#define REG_PM_SLP_27                   0x27U
#define REG_PM_SLP_4A                   0x4AU
#define REG_PM_SLP_4B                   0x4BU
#define REG_PM_SLP_4C                   0x4CU
#define REG_PM_SLP_57                   0x57U
#define REG_PM_SLP_62                   0x62U

//***** Bank 14 - PM_SAR *****//
#define REG_PM_SAR_11                   0x11U
#define REG_PM_SAR_12                   0x12U

//***** Bank 21 - TX_PM   *****//
#define REG_PM_HDMITX_03                0x03U
#define REG_PM_HDMITX_1C                0x1CU
#define REG_PM_HDMITX_2B                0x2BU
#define REG_PM_HDMITX_2C                0x2CU
#define REG_PM_HDMITX_2E                0x2EU
#define REG_PM_HDMITX_2F                0x2FU
#define REG_PM_HDMITX_33                0x33U
#define REG_PM_HDMITX_34                0x34U
#define REG_PM_HDMITX_38                0x38U

//***** Bank 22 - RX_PM   *****//
#define REG_PM_HDMIRX_ATOP_06           0x06U
#define REG_PM_HDMIRX_ATOP_60           0x60U
#define REG_PM_HDMIRX_ATOP_7F           0x7FU

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#endif // _REG_HDMITX_H_

