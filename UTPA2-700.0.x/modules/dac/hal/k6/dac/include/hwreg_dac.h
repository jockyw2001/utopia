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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

#ifndef _HWREG_DAC_H_
#define _HWREG_DAC_H_

#include "MsCommon.h"

#define REG_DAC_BASE        (0x101A00)
#define REG_HDGEN_BASE      (0x103000)
#define REG_DACPLL_BASE     (0x121100)

#define REG_DAC_00_L        (REG_DAC_BASE + 0x00)
#define REG_DAC_00_H        (REG_DAC_BASE + 0x01)
#define REG_DAC_01_L        (REG_DAC_BASE + 0x02)
#define REG_DAC_01_H        (REG_DAC_BASE + 0x03)
#define REG_DAC_02_L        (REG_DAC_BASE + 0x04)
#define REG_DAC_02_H        (REG_DAC_BASE + 0x05)
#define REG_DAC_03_L        (REG_DAC_BASE + 0x06)
#define REG_DAC_03_H        (REG_DAC_BASE + 0x07)
#define REG_DAC_04_L        (REG_DAC_BASE + 0x08)
#define REG_DAC_04_H        (REG_DAC_BASE + 0x09)
#define REG_DAC_05_L        (REG_DAC_BASE + 0x0A)
#define REG_DAC_05_H        (REG_DAC_BASE + 0x0B)
#define REG_DAC_06_L        (REG_DAC_BASE + 0x0C)
#define REG_DAC_06_H        (REG_DAC_BASE + 0x0D)
#define REG_DAC_07_L        (REG_DAC_BASE + 0x0E)
#define REG_DAC_07_H        (REG_DAC_BASE + 0x0F)
#define REG_DAC_08_L        (REG_DAC_BASE + 0x10)
#define REG_DAC_08_H        (REG_DAC_BASE + 0x11)

#define REG_DAC_PLL_15_L    (REG_DACPLL_BASE + 0x2A)
#define REG_DAC_PLL_15_H    (REG_DACPLL_BASE + 0x2B)
#define REG_DAC_PLL_16_L    (REG_DACPLL_BASE + 0x2C)
#define REG_DAC_PLL_16_H    (REG_DACPLL_BASE + 0x2D)
#define REG_DAC_PLL_17_L    (REG_DACPLL_BASE + 0x2E)
#define REG_DAC_PLL_17_H    (REG_DACPLL_BASE + 0x2F)
#define REG_DAC_PLL_31_L    (REG_DACPLL_BASE + 0x62)
#define REG_DAC_PLL_31_H    (REG_DACPLL_BASE + 0x63)
#define REG_DAC_PLL_32_L    (REG_DACPLL_BASE + 0x64)
#define REG_DAC_PLL_32_H    (REG_DACPLL_BASE + 0x65)
#define REG_DAC_PLL_33_L    (REG_DACPLL_BASE + 0x66)
#define REG_DAC_PLL_33_H    (REG_DACPLL_BASE + 0x67)
#define REG_DAC_PLL_34_L    (REG_DACPLL_BASE + 0x68)
#define REG_DAC_PLL_34_H    (REG_DACPLL_BASE + 0x69)
#define REG_DAC_PLL_35_L    (REG_DACPLL_BASE + 0x6A)
#define REG_DAC_PLL_35_H    (REG_DACPLL_BASE + 0x6B)

#define CLK_ODCLK_P                 0x00
#define CLK_VEDAC_P                 0x01
#define REG_CKG_DAC1                (0x59)
#define REG_CKG_DAC2                (0x59)
#define REG_HSYNC_VSYNC_EN       	(0x47)
#define REG_CKG_DAC1_DISABLE_MASK           0:0
#define REG_CKG_DAC2_DISABLE_MASK           8:8
#define REG_CKG_DAC1_MASK           3:2
#define REG_CKG_DAC2_MASK           11:10
#define REG_HD_DAT_CLK_INV          (0x04)
#define REG_HD_DAT_CLK_INV_MSK      0:0
#define REG_HD_DAT_CLK_DLY          (0x04)
#define REG_HD_DAT_CLK_DLY_MSK      7:4
#define REG_HD_DAC_OUT_SEL          (0x05)
#define REG_HD_DAC_OUT_SEL_MSK      1:0  // src sel: [00]HD GEN, [01]VE out(CVBS+S), [10]test, [11]VE out(YUV)
#define REG_HD_DAC_SWAP_SEL         (0x05)
#define REG_HD_DAC_SWAP_SEL_MSK     10:8
#define REG_EN_IDAC_HDR_MSK         0:0
#define REG_EN_IDAC_HDG_MSK         1:1
#define REG_EN_IDAC_HDB_MSK         2:2

#define REG_MODE_HD                 (0x08)
#define REG_MODE_HD_MSK             9:9
#define REG_MODE_SD                 (0x08)
#define REG_MODE_SD_MSK             10:10

#define REG_EN_IDAC_SDX             (0x00)
#define REG_EN_IDAC_SDX_MSK         0:0
#define REG_EN_IDAC_SDY             (0x00)
#define REG_EN_IDAC_SDY_MSK         1:1
#define REG_EN_IDAC_SDC             (0x00)
#define REG_EN_IDAC_SDC_MSK         2:2
#define REG_EN_IDAC_REF             (0x00)
#define REG_EN_IDAC_REF_MSK         4:4
#define REG_EN_IDAC_SEL_REXT_MSK    0:0
#define REG_EN_IDAC_HDCLK_MSK       5:5


#define REG_GCR_IDAC_GAINSD         (0x00)
#define REG_GCR_IDAC_GAINSD_MSK     14:8
#define REG_EN_REF_IDAC_GAINSD      (0x00)
#define REG_EN_REF_IDAC_GAINSD_MSK  15:15

#define REG_SD_DAT_CLK_INV          (0x01)
#define REG_SD_DAT_CLK_INV_MSK      0:0
#define REG_SD_DAT_CLK_DLY          (0x01)
#define REG_SD_DAT_CLK_DLY_MSK      7:4

#define REG_SD_DAC_OUT_SEL          (0x02)
#define REG_SD_DAC_OUT_SEL_MSK      1:0

#define REG_SD_DAC_SWAP_SEL         (0x02)
#define REG_SD_DAC_SWAP_SEL_MSK     10:8
#endif

