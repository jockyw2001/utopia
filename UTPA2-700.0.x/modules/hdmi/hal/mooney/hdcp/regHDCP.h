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
//  File name: regAESDMA.h
//  Description: AESDMA Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HDCP_REG_MCU_H_
#define _HDCP_REG_MCU_H_

#define DEF_HDCP14_TX_KEY_REG_BANK  0x000000U
#define DEF_HDCP22_TX_KEY_REG_BANK  0x072700U // 0x172700U
#define DEF_HDCP14_TX_REG_BANK      0x072B00U // 0x172B00U
#define DEF_HDCP22_TX_REG_BANK      0x072F00U // 0x172F00U
#define DEF_HDMITX_PHY_REG_BANK     0x073000U // 0x173000U

#define DEF_HDCP14_RX_KEY_REG_BANK  0x000000U
#define DEF_HDCP22_RX_KEY_REG_BANK  0x073A00U // 0x173A00U
#define DEF_HDCP14_RX_REG_BANK      0x071200U // 0x171200U
#define DEF_HDCP22_RX_REG_BANK      0x071200U // 0x171200U

#define REG_HDCP14_TX_00_L          0x00U
#define REG_HDCP14_TX_01_L          0x01U
#define REG_HDCP14_TX_02_L          0x02U
#define REG_HDCP14_TX_03_L          0x03U
#define REG_HDCP14_TX_04_L          0x04U
#define REG_HDCP14_TX_05_L          0x05U
#define REG_HDCP14_TX_06_L          0x06U
#define REG_HDCP14_TX_07_L          0x07U
#define REG_HDCP14_TX_08_L          0x08U
#define REG_HDCP14_TX_09_L          0x09U
#define REG_HDCP14_TX_0A_L          0x0AU
#define REG_HDCP14_TX_0B_L          0x0BU
#define REG_HDCP14_TX_0C_L          0x0CU
#define REG_HDCP14_TX_0D_L          0x0DU
#define REG_HDCP14_TX_0E_L          0x0EU
#define REG_HDCP14_TX_0F_L          0x0FU
#define REG_HDCP14_TX_10_L          0x10U
#define REG_HDCP14_TX_11_L          0x11U
#define REG_HDCP14_TX_12_L          0x12U
#define REG_HDCP14_TX_13_L          0x13U
#define REG_HDCP14_TX_14_L          0x14U
#define REG_HDCP14_TX_15_L          0x15U
#define REG_HDCP14_TX_16_L          0x16U
#define REG_HDCP14_TX_17_L          0x17U
#define REG_HDCP14_TX_18_L          0x18U
#define REG_HDCP14_TX_19_L          0x19U
#define REG_HDCP14_TX_1A_L          0x1AU
#define REG_HDCP14_TX_1B_L          0x1BU
#define REG_HDCP14_TX_1C_L          0x1CU
#define REG_HDCP14_TX_1D_L          0x1DU
#define REG_HDCP14_TX_1E_L          0x1EU
#define REG_HDCP14_TX_1F_L          0x1FU
#define REG_HDCP14_TX_20_L          0x20U
#define REG_HDCP14_TX_21_L          0x21U
#define REG_HDCP14_TX_22_L          0x22U
#define REG_HDCP14_TX_23_L          0x23U
#define REG_HDCP14_TX_24_L          0x24U
#define REG_HDCP14_TX_25_L          0x25U
#define REG_HDCP14_TX_26_L          0x26U
#define REG_HDCP14_TX_27_L          0x27U
#define REG_HDCP14_TX_28_L          0x28U
#define REG_HDCP14_TX_29_L          0x29U
#define REG_HDCP14_TX_2A_L          0x2AU
#define REG_HDCP14_TX_2B_L          0x2BU
#define REG_HDCP14_TX_2C_L          0x2CU
#define REG_HDCP14_TX_2D_L          0x2DU
#define REG_HDCP14_TX_2E_L          0x2EU
#define REG_HDCP14_TX_2F_L          0x2FU
#define REG_HDCP14_TX_30_L          0x30U
#define REG_HDCP14_TX_31_L          0x31U
#define REG_HDCP14_TX_32_L          0x32U
#define REG_HDCP14_TX_33_L          0x33U
#define REG_HDCP14_TX_34_L          0x34U
#define REG_HDCP14_TX_35_L          0x35U
#define REG_HDCP14_TX_36_L          0x36U
#define REG_HDCP14_TX_37_L          0x37U
#define REG_HDCP14_TX_38_L          0x38U
#define REG_HDCP14_TX_39_L          0x39U
#define REG_HDCP14_TX_3A_L          0x3AU
#define REG_HDCP14_TX_3B_L          0x3BU
#define REG_HDCP14_TX_3C_L          0x3CU
#define REG_HDCP14_TX_3D_L          0x3DU
#define REG_HDCP14_TX_3E_L          0x3EU
#define REG_HDCP14_TX_3F_L          0x3FU

#define REG_HDCP22_TX_00_L          0x00U
#define REG_HDCP22_TX_01_L          0x01U
#define REG_HDCP22_TX_02_L          0x02U
#define REG_HDCP22_TX_03_L          0x03U
#define REG_HDCP22_TX_04_L          0x04U
#define REG_HDCP22_TX_05_L          0x05U
#define REG_HDCP22_TX_06_L          0x06U
#define REG_HDCP22_TX_07_L          0x07U
#define REG_HDCP22_TX_08_L          0x08U
#define REG_HDCP22_TX_09_L          0x09U
#define REG_HDCP22_TX_0A_L          0x0AU
#define REG_HDCP22_TX_0B_L          0x0BU
#define REG_HDCP22_TX_0C_L          0x0CU
#define REG_HDCP22_TX_0D_L          0x0DU
#define REG_HDCP22_TX_0E_L          0x0EU
#define REG_HDCP22_TX_0F_L          0x0FU
#define REG_HDCP22_TX_10_L          0x10U
#define REG_HDCP22_TX_11_L          0x11U
#define REG_HDCP22_TX_12_L          0x12U
#define REG_HDCP22_TX_13_L          0x13U
#define REG_HDCP22_TX_14_L          0x14U
#define REG_HDCP22_TX_15_L          0x15U
#define REG_HDCP22_TX_16_L          0x16U
#define REG_HDCP22_TX_17_L          0x17U
#define REG_HDCP22_TX_18_L          0x18U
#define REG_HDCP22_TX_19_L          0x19U
#define REG_HDCP22_TX_1A_L          0x1AU
#define REG_HDCP22_TX_1B_L          0x1BU
#define REG_HDCP22_TX_1C_L          0x1CU
#define REG_HDCP22_TX_1D_L          0x1DU
#define REG_HDCP22_TX_1E_L          0x1EU
#define REG_HDCP22_TX_1F_L          0x1FU
#define REG_HDCP22_TX_20_L          0x20U
#define REG_HDCP22_TX_21_L          0x21U
#define REG_HDCP22_TX_22_L          0x22U
#define REG_HDCP22_TX_23_L          0x23U
#define REG_HDCP22_TX_24_L          0x24U
#define REG_HDCP22_TX_25_L          0x25U
#define REG_HDCP22_TX_26_L          0x26U
#define REG_HDCP22_TX_27_L          0x27U
#define REG_HDCP22_TX_28_L          0x28U
#define REG_HDCP22_TX_29_L          0x29U
#define REG_HDCP22_TX_2A_L          0x2AU
#define REG_HDCP22_TX_2B_L          0x2BU
#define REG_HDCP22_TX_2C_L          0x2CU
#define REG_HDCP22_TX_2D_L          0x2DU
#define REG_HDCP22_TX_2E_L          0x2EU
#define REG_HDCP22_TX_2F_L          0x2FU
#define REG_HDCP22_TX_30_L          0x30U
#define REG_HDCP22_TX_31_L          0x31U
#define REG_HDCP22_TX_32_L          0x32U
#define REG_HDCP22_TX_33_L          0x33U
#define REG_HDCP22_TX_34_L          0x34U
#define REG_HDCP22_TX_35_L          0x35U
#define REG_HDCP22_TX_36_L          0x36U
#define REG_HDCP22_TX_37_L          0x37U
#define REG_HDCP22_TX_38_L          0x38U
#define REG_HDCP22_TX_39_L          0x39U
#define REG_HDCP22_TX_3A_L          0x3AU
#define REG_HDCP22_TX_3B_L          0x3BU
#define REG_HDCP22_TX_3C_L          0x3CU
#define REG_HDCP22_TX_3D_L          0x3DU
#define REG_HDCP22_TX_3E_L          0x3EU
#define REG_HDCP22_TX_3F_L          0x3FU

#define REG_HDMITX_PHY_00_L         0x00U
#define REG_HDMITX_PHY_01_L         0x01U
#define REG_HDMITX_PHY_02_L         0x02U
#define REG_HDMITX_PHY_03_L         0x03U
#define REG_HDMITX_PHY_04_L         0x04U
#define REG_HDMITX_PHY_05_L         0x05U
#define REG_HDMITX_PHY_06_L         0x06U
#define REG_HDMITX_PHY_07_L         0x07U
#define REG_HDMITX_PHY_08_L         0x08U
#define REG_HDMITX_PHY_09_L         0x09U
#define REG_HDMITX_PHY_0A_L         0x0AU
#define REG_HDMITX_PHY_0B_L         0x0BU
#define REG_HDMITX_PHY_0C_L         0x0CU
#define REG_HDMITX_PHY_0D_L         0x0DU
#define REG_HDMITX_PHY_0E_L         0x0EU
#define REG_HDMITX_PHY_0F_L         0x0FU
#define REG_HDMITX_PHY_10_L         0x10U
#define REG_HDMITX_PHY_11_L         0x11U
#define REG_HDMITX_PHY_12_L         0x12U
#define REG_HDMITX_PHY_13_L         0x13U
#define REG_HDMITX_PHY_14_L         0x14U
#define REG_HDMITX_PHY_15_L         0x15U
#define REG_HDMITX_PHY_16_L         0x16U
#define REG_HDMITX_PHY_17_L         0x17U
#define REG_HDMITX_PHY_18_L         0x18U
#define REG_HDMITX_PHY_19_L         0x19U
#define REG_HDMITX_PHY_1A_L         0x1AU
#define REG_HDMITX_PHY_1B_L         0x1BU
#define REG_HDMITX_PHY_1C_L         0x1CU
#define REG_HDMITX_PHY_1D_L         0x1DU
#define REG_HDMITX_PHY_1E_L         0x1EU
#define REG_HDMITX_PHY_1F_L         0x1FU
#define REG_HDMITX_PHY_20_L         0x20U
#define REG_HDMITX_PHY_21_L         0x21U
#define REG_HDMITX_PHY_22_L         0x22U
#define REG_HDMITX_PHY_23_L         0x23U
#define REG_HDMITX_PHY_24_L         0x24U
#define REG_HDMITX_PHY_25_L         0x25U
#define REG_HDMITX_PHY_26_L         0x26U
#define REG_HDMITX_PHY_27_L         0x27U
#define REG_HDMITX_PHY_28_L         0x28U
#define REG_HDMITX_PHY_29_L         0x29U
#define REG_HDMITX_PHY_2A_L         0x2AU
#define REG_HDMITX_PHY_2B_L         0x2BU
#define REG_HDMITX_PHY_2C_L         0x2CU
#define REG_HDMITX_PHY_2D_L         0x2DU
#define REG_HDMITX_PHY_2E_L         0x2EU
#define REG_HDMITX_PHY_2F_L         0x2FU
#define REG_HDMITX_PHY_30_L         0x30U
#define REG_HDMITX_PHY_31_L         0x31U
#define REG_HDMITX_PHY_32_L         0x32U
#define REG_HDMITX_PHY_33_L         0x33U
#define REG_HDMITX_PHY_34_L         0x34U
#define REG_HDMITX_PHY_35_L         0x35U
#define REG_HDMITX_PHY_36_L         0x36U
#define REG_HDMITX_PHY_37_L         0x37U
#define REG_HDMITX_PHY_38_L         0x38U
#define REG_HDMITX_PHY_39_L         0x39U
#define REG_HDMITX_PHY_3A_L         0x3AU
#define REG_HDMITX_PHY_3B_L         0x3BU
#define REG_HDMITX_PHY_3C_L         0x3CU
#define REG_HDMITX_PHY_3D_L         0x3DU
#define REG_HDMITX_PHY_3E_L         0x3EU
#define REG_HDMITX_PHY_3F_L         0x3FU

#endif // #ifndef _HDCP_REG_MCU_H_
