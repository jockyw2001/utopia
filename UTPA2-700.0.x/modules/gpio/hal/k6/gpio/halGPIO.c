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

#ifndef _HALGPIO_C_
#define _HALGPIO_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsTypes.h"
#include "drvGPIO.h"
#include "halGPIO.h"
#include "regGPIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define GPIO_HAL_ERR(x, args...)        //{printf(x, ##args);}
#define GPIO_HAL_NOTSUPPORT()           {printf("%s not support!\n", __FUNCTION__);}

#define _CONCAT( a, b )     a##b
#define CONCAT( a, b )      _CONCAT( a, b )

#define BIT0    BIT(0)
#define BIT1    BIT(1)
#define BIT2    BIT(2)
#define BIT3    BIT(3)
#define BIT4    BIT(4)
#define BIT5    BIT(5)
#define BIT6    BIT(6)
#define BIT7    BIT(7)

// Dummy
#define GPIO999_OEN     0, 0
#define GPIO999_OUT     0, 0
#define GPIO999_IN      0, 0

#define GPIO0_PAD PAD_PM_IRIN
#define GPIO0_OEN 0x000f28, BIT0
#define GPIO0_OUT 0x000f28, BIT1
#define GPIO0_IN  0x000f28, BIT2

#define GPIO1_PAD PAD_PM_SPI_CZ
#define GPIO1_OEN 0x000f30, BIT0
#define GPIO1_OUT 0x000f30, BIT1
#define GPIO1_IN  0x000f30, BIT2

#define GPIO2_PAD PAD_PM_SPI_CK
#define GPIO2_OEN 0x000f32, BIT0
#define GPIO2_OUT 0x000f32, BIT1
#define GPIO2_IN  0x000f32, BIT2

#define GPIO3_PAD PAD_PM_SPI_DI
#define GPIO3_OEN 0x000f34, BIT0
#define GPIO3_OUT 0x000f34, BIT1
#define GPIO3_IN  0x000f34, BIT2

#define GPIO4_PAD PAD_PM_SPI_DO
#define GPIO4_OEN 0x000f36, BIT0
#define GPIO4_OUT 0x000f36, BIT1
#define GPIO4_IN  0x000f36, BIT2

#define GPIO5_PAD PAD_PM_SPI_WPZ
#define GPIO5_OEN 0x000f88, BIT0
#define GPIO5_OUT 0x000f88, BIT1
#define GPIO5_IN  0x000f88, BIT2

#define GPIO6_PAD PAD_PM_SPI_HOLDZ
#define GPIO6_OEN 0x000f8a, BIT0
#define GPIO6_OUT 0x000f8a, BIT1
#define GPIO6_IN  0x000f8a, BIT2

#define GPIO7_PAD PAD_PM_SPI_RSTZ
#define GPIO7_OEN 0x000f8c, BIT0
#define GPIO7_OUT 0x000f8c, BIT1
#define GPIO7_IN  0x000f8c, BIT2

#define GPIO8_PAD PAD_PM_GPIO0
#define GPIO8_OEN 0x000f00, BIT0
#define GPIO8_OUT 0x000f00, BIT1
#define GPIO8_IN  0x000f00, BIT2

#define GPIO9_PAD PAD_PM_GPIO1
#define GPIO9_OEN 0x000f02, BIT0
#define GPIO9_OUT 0x000f02, BIT1
#define GPIO9_IN  0x000f02, BIT2

#define GPIO10_PAD PAD_PM_GPIO2
#define GPIO10_OEN 0x000f04, BIT0
#define GPIO10_OUT 0x000f04, BIT1
#define GPIO10_IN  0x000f04, BIT2

#define GPIO11_PAD PAD_PM_GPIO3
#define GPIO11_OEN 0x000f06, BIT0
#define GPIO11_OUT 0x000f06, BIT1
#define GPIO11_IN  0x000f06, BIT2

#define GPIO12_PAD PAD_PM_GPIO4
#define GPIO12_OEN 0x000f08, BIT0
#define GPIO12_OUT 0x000f08, BIT1
#define GPIO12_IN  0x000f08, BIT2

#define GPIO13_PAD PAD_PM_GPIO5
#define GPIO13_OEN 0x000f0a, BIT0
#define GPIO13_OUT 0x000f0a, BIT1
#define GPIO13_IN  0x000f0a, BIT2

#define GPIO14_PAD PAD_PM_GPIO6
#define GPIO14_OEN 0x000f0c, BIT0
#define GPIO14_OUT 0x000f0c, BIT1
#define GPIO14_IN  0x000f0c, BIT2

#define GPIO15_PAD PAD_PM_GPIO7
#define GPIO15_OEN 0x000f0e, BIT0
#define GPIO15_OUT 0x000f0e, BIT1
#define GPIO15_IN  0x000f0e, BIT2

#define GPIO16_PAD PAD_PM_GPIO8
#define GPIO16_OEN 0x000f10, BIT0
#define GPIO16_OUT 0x000f10, BIT1
#define GPIO16_IN  0x000f10, BIT2

#define GPIO17_PAD PAD_PM_GPIO9
#define GPIO17_OEN 0x000f12, BIT0
#define GPIO17_OUT 0x000f12, BIT1
#define GPIO17_IN  0x000f12, BIT2

#define GPIO18_PAD PAD_PM_GPIO10
#define GPIO18_OEN 0x000f14, BIT0
#define GPIO18_OUT 0x000f14, BIT1
#define GPIO18_IN  0x000f14, BIT2

#define GPIO19_PAD PAD_PM_GPIO11
#define GPIO19_OEN 0x000f16, BIT0
#define GPIO19_OUT 0x000f16, BIT1
#define GPIO19_IN  0x000f16, BIT2

#define GPIO20_PAD PAD_PM_GPIO12
#define GPIO20_OEN 0x000f18, BIT0
#define GPIO20_OUT 0x000f18, BIT1
#define GPIO20_IN  0x000f18, BIT2

#define GPIO21_PAD PAD_PM_GPIO13
#define GPIO21_OEN 0x000f1a, BIT0
#define GPIO21_OUT 0x000f1a, BIT1
#define GPIO21_IN  0x000f1a, BIT2

#define GPIO22_PAD PAD_PM_GPIO14
#define GPIO22_OEN 0x000f1c, BIT0
#define GPIO22_OUT 0x000f1c, BIT1
#define GPIO22_IN  0x000f1c, BIT2

#define GPIO23_PAD PAD_PM_GPIO15
#define GPIO23_OEN 0x000f1e, BIT0
#define GPIO23_OUT 0x000f1e, BIT1
#define GPIO23_IN  0x000f1e, BIT2

#define GPIO24_PAD PAD_PM_CEC
#define GPIO24_OEN 0x000f2c, BIT0
#define GPIO24_OUT 0x000f2c, BIT1
#define GPIO24_IN  0x000f2c, BIT2

#define GPIO25_PAD PAD_HDMITX_HPD
#define GPIO25_OEN 0x001166, BIT3
#define GPIO25_OUT 0x001166, BIT2
#define GPIO25_IN  0x001166, BIT0

#define GPIO26_PAD PAD_HDMIRX_HPD
#define GPIO26_OEN 0x000e4e, BIT0
#define GPIO26_OUT 0x000e4e, BIT4
#define GPIO26_IN  0x000e4f, BIT0

#define GPIO27_PAD PAD_RX_DDCDA_CK
#define GPIO27_OEN 0x000496, BIT1
#define GPIO27_OUT 0x000496, BIT2
#define GPIO27_IN  0x000496, BIT0

#define GPIO28_PAD PAD_RX_DDCDA_DA
#define GPIO28_OEN 0x000496, BIT5
#define GPIO28_OUT 0x000496, BIT6
#define GPIO28_IN  0x000496, BIT4

#define GPIO29_PAD PAD_PM_SD_CDZ
#define GPIO29_OEN 0x000f8e, BIT0
#define GPIO29_OUT 0x000f8e, BIT1
#define GPIO29_IN  0x000f8e, BIT2

#define GPIO30_PAD PAD_HOTPLUGA_HDMI20_5V
#define GPIO30_OEN 0x010718, BIT5
#define GPIO30_OUT 0x010718, BIT4
#define GPIO30_IN  0x010718, BIT6

#define GPIO31_PAD PAD_VID0
#define GPIO31_OEN 0x000f90, BIT0
#define GPIO31_OUT 0x000f90, BIT1
#define GPIO31_IN  0x000f90, BIT2

#define GPIO32_PAD PAD_VID1
#define GPIO32_OEN 0x000f92, BIT0
#define GPIO32_OUT 0x000f92, BIT1
#define GPIO32_IN  0x000f92, BIT2

#define GPIO33_PAD PAD_PM_LED0
#define GPIO33_OEN 0x000f94, BIT0
#define GPIO33_OUT 0x000f94, BIT1
#define GPIO33_IN  0x000f94, BIT2

#define GPIO34_PAD PAD_PM_LED1
#define GPIO34_OEN 0x000f96, BIT0
#define GPIO34_OUT 0x000f96, BIT1
#define GPIO34_IN  0x000f96, BIT2

#define GPIO35_PAD PAD_SAR_GPIO0
#define GPIO35_OEN 0x001423, BIT0
#define GPIO35_OUT 0x001424, BIT0
#define GPIO35_IN  0x001425, BIT0

#define GPIO36_PAD PAD_SAR_GPIO1
#define GPIO36_OEN 0x001423, BIT1
#define GPIO36_OUT 0x001424, BIT1
#define GPIO36_IN  0x001425, BIT1

#define GPIO37_PAD PAD_SAR_GPIO2
#define GPIO37_OEN 0x001423, BIT2
#define GPIO37_OUT 0x001424, BIT2
#define GPIO37_IN  0x001425, BIT2

#define GPIO38_PAD PAD_SAR_GPIO3
#define GPIO38_OEN 0x001423, BIT3
#define GPIO38_OUT 0x001424, BIT3
#define GPIO38_IN  0x001425, BIT3

#define GPIO39_PAD PAD_SAR_GPIO4
#define GPIO39_OEN 0x001423, BIT4
#define GPIO39_OUT 0x001424, BIT4
#define GPIO39_IN  0x001425, BIT4

#define GPIO40_PAD PAD_HSYNC_OUT
#define GPIO40_OEN 0x1025ec, BIT5
#define GPIO40_OUT 0x1025ec, BIT4
#define GPIO40_IN  0x1025ec, BIT0

#define GPIO41_PAD PAD_VSYNC_OUT
#define GPIO41_OEN 0x1025ee, BIT5
#define GPIO41_OUT 0x1025ee, BIT4
#define GPIO41_IN  0x1025ee, BIT0

#define GPIO42_PAD PAD_I2S_OUT_BCK
#define GPIO42_OEN 0x102556, BIT5
#define GPIO42_OUT 0x102556, BIT4
#define GPIO42_IN  0x102556, BIT0

#define GPIO43_PAD PAD_I2S_OUT_MCK
#define GPIO43_OEN 0x102558, BIT5
#define GPIO43_OUT 0x102558, BIT4
#define GPIO43_IN  0x102558, BIT0

#define GPIO44_PAD PAD_I2S_OUT_WS
#define GPIO44_OEN 0x10255a, BIT5
#define GPIO44_OUT 0x10255a, BIT4
#define GPIO44_IN  0x10255a, BIT0

#define GPIO45_PAD PAD_I2S_OUT_SD0
#define GPIO45_OEN 0x10255c, BIT5
#define GPIO45_OUT 0x10255c, BIT4
#define GPIO45_IN  0x10255c, BIT0

#define GPIO46_PAD PAD_I2S_OUT_SD1
#define GPIO46_OEN 0x10255e, BIT5
#define GPIO46_OUT 0x10255e, BIT4
#define GPIO46_IN  0x10255e, BIT0

#define GPIO47_PAD PAD_I2S_OUT_SD2
#define GPIO47_OEN 0x102560, BIT5
#define GPIO47_OUT 0x102560, BIT4
#define GPIO47_IN  0x102560, BIT0

#define GPIO48_PAD PAD_I2S_OUT_SD3
#define GPIO48_OEN 0x102562, BIT5
#define GPIO48_OUT 0x102562, BIT4
#define GPIO48_IN  0x102562, BIT0

#define GPIO49_PAD PAD_I2S_OUT_MUTE
#define GPIO49_OEN 0x102564, BIT5
#define GPIO49_OUT 0x102564, BIT4
#define GPIO49_IN  0x102564, BIT0

#define GPIO50_PAD PAD_SD_CLK
#define GPIO50_OEN 0x1025a4, BIT5
#define GPIO50_OUT 0x1025a4, BIT4
#define GPIO50_IN  0x1025a4, BIT0

#define GPIO51_PAD PAD_SD_CMD
#define GPIO51_OEN 0x1025a6, BIT5
#define GPIO51_OUT 0x1025a6, BIT4
#define GPIO51_IN  0x1025a6, BIT0

#define GPIO52_PAD PAD_SD_D0
#define GPIO52_OEN 0x1025a8, BIT5
#define GPIO52_OUT 0x1025a8, BIT4
#define GPIO52_IN  0x1025a8, BIT0

#define GPIO53_PAD PAD_SD_D1
#define GPIO53_OEN 0x1025aa, BIT5
#define GPIO53_OUT 0x1025aa, BIT4
#define GPIO53_IN  0x1025aa, BIT0

#define GPIO54_PAD PAD_SD_D2
#define GPIO54_OEN 0x1025ac, BIT5
#define GPIO54_OUT 0x1025ac, BIT4
#define GPIO54_IN  0x1025ac, BIT0

#define GPIO55_PAD PAD_SD_D3
#define GPIO55_OEN 0x1025ae, BIT5
#define GPIO55_OUT 0x1025ae, BIT4
#define GPIO55_IN  0x1025ae, BIT0

#define GPIO56_PAD PAD_GPIO0
#define GPIO56_OEN 0x1025b0, BIT5
#define GPIO56_OUT 0x1025b0, BIT4
#define GPIO56_IN  0x1025b0, BIT0

#define GPIO57_PAD PAD_GPIO1
#define GPIO57_OEN 0x1025b2, BIT5
#define GPIO57_OUT 0x1025b2, BIT4
#define GPIO57_IN  0x1025b2, BIT0

#define GPIO58_PAD PAD_GPIO2
#define GPIO58_OEN 0x1025b4, BIT5
#define GPIO58_OUT 0x1025b4, BIT4
#define GPIO58_IN  0x1025b4, BIT0

#define GPIO59_PAD PAD_GPIO3
#define GPIO59_OEN 0x1025b6, BIT5
#define GPIO59_OUT 0x1025b6, BIT4
#define GPIO59_IN  0x1025b6, BIT0

#define GPIO60_PAD PAD_GPIO4
#define GPIO60_OEN 0x1025b8, BIT5
#define GPIO60_OUT 0x1025b8, BIT4
#define GPIO60_IN  0x1025b8, BIT0

#define GPIO61_PAD PAD_GPIO5
#define GPIO61_OEN 0x1025ba, BIT5
#define GPIO61_OUT 0x1025ba, BIT4
#define GPIO61_IN  0x1025ba, BIT0

#define GPIO62_PAD PAD_GPIO6
#define GPIO62_OEN 0x1025bc, BIT5
#define GPIO62_OUT 0x1025bc, BIT4
#define GPIO62_IN  0x1025bc, BIT0

#define GPIO63_PAD PAD_GPIO7
#define GPIO63_OEN 0x1025be, BIT5
#define GPIO63_OUT 0x1025be, BIT4
#define GPIO63_IN  0x1025be, BIT0

#define GPIO64_PAD PAD_GPIO8
#define GPIO64_OEN 0x1025c0, BIT5
#define GPIO64_OUT 0x1025c0, BIT4
#define GPIO64_IN  0x1025c0, BIT0

#define GPIO65_PAD PAD_GPIO9
#define GPIO65_OEN 0x1025c2, BIT5
#define GPIO65_OUT 0x1025c2, BIT4
#define GPIO65_IN  0x1025c2, BIT0

#define GPIO66_PAD PAD_SM0_CLK
#define GPIO66_OEN 0x102500, BIT5
#define GPIO66_OUT 0x102500, BIT4
#define GPIO66_IN  0x102500, BIT0

#define GPIO67_PAD PAD_SM0_RST
#define GPIO67_OEN 0x102502, BIT5
#define GPIO67_OUT 0x102502, BIT4
#define GPIO67_IN  0x102502, BIT0

#define GPIO68_PAD PAD_SM0_VCC
#define GPIO68_OEN 0x102504, BIT5
#define GPIO68_OUT 0x102504, BIT4
#define GPIO68_IN  0x102504, BIT0

#define GPIO69_PAD PAD_SM0_CD
#define GPIO69_OEN 0x102506, BIT5
#define GPIO69_OUT 0x102506, BIT4
#define GPIO69_IN  0x102506, BIT0

#define GPIO70_PAD PAD_SM0_IO
#define GPIO70_OEN 0x102508, BIT5
#define GPIO70_OUT 0x102508, BIT4
#define GPIO70_IN  0x102508, BIT0

#define GPIO71_PAD PAD_SM0_VSEL
#define GPIO71_OEN 0x10250a, BIT5
#define GPIO71_OUT 0x10250a, BIT4
#define GPIO71_IN  0x10250a, BIT0

#define GPIO72_PAD PAD_SM0_C4
#define GPIO72_OEN 0x10250c, BIT5
#define GPIO72_OUT 0x10250c, BIT4
#define GPIO72_IN  0x10250c, BIT0

#define GPIO73_PAD PAD_SM0_C8
#define GPIO73_OEN 0x10250d, BIT5
#define GPIO73_OUT 0x10250d, BIT4
#define GPIO73_IN  0x10250d, BIT0

#define GPIO74_PAD PAD_CI_RST
#define GPIO74_OEN 0x10269a, BIT5
#define GPIO74_OUT 0x10269a, BIT4
#define GPIO74_IN  0x10269a, BIT0

#define GPIO75_PAD PAD_CI_WAITZ
#define GPIO75_OEN 0x10269c, BIT5
#define GPIO75_OUT 0x10269c, BIT4
#define GPIO75_IN  0x10269c, BIT0

#define GPIO76_PAD PAD_CI_IRQAZ
#define GPIO76_OEN 0x10269e, BIT5
#define GPIO76_OUT 0x10269e, BIT4
#define GPIO76_IN  0x10269e, BIT0

#define GPIO77_PAD PAD_CI_IORDZ
#define GPIO77_OEN 0x1026a0, BIT5
#define GPIO77_OUT 0x1026a0, BIT4
#define GPIO77_IN  0x1026a0, BIT0

#define GPIO78_PAD PAD_CI_REGZ
#define GPIO78_OEN 0x1026a2, BIT5
#define GPIO78_OUT 0x1026a2, BIT4
#define GPIO78_IN  0x1026a2, BIT0

#define GPIO79_PAD PAD_CI_WEZ
#define GPIO79_OEN 0x1026a4, BIT5
#define GPIO79_OUT 0x1026a4, BIT4
#define GPIO79_IN  0x1026a4, BIT0

#define GPIO80_PAD PAD_CI_IOWRZ
#define GPIO80_OEN 0x1026a6, BIT5
#define GPIO80_OUT 0x1026a6, BIT4
#define GPIO80_IN  0x1026a6, BIT0

#define GPIO81_PAD PAD_CI_CEZ
#define GPIO81_OEN 0x1026a8, BIT5
#define GPIO81_OUT 0x1026a8, BIT4
#define GPIO81_IN  0x1026a8, BIT0

#define GPIO82_PAD PAD_CI_OEZ
#define GPIO82_OEN 0x1026aa, BIT5
#define GPIO82_OUT 0x1026aa, BIT4
#define GPIO82_IN  0x1026aa, BIT0

#define GPIO83_PAD PAD_CI_CDZ
#define GPIO83_OEN 0x1026ac, BIT5
#define GPIO83_OUT 0x1026ac, BIT4
#define GPIO83_IN  0x1026ac, BIT0

#define GPIO84_PAD PAD_CI_A0
#define GPIO84_OEN 0x1026ae, BIT5
#define GPIO84_OUT 0x1026ae, BIT4
#define GPIO84_IN  0x1026ae, BIT0

#define GPIO85_PAD PAD_CI_A1
#define GPIO85_OEN 0x1026b0, BIT5
#define GPIO85_OUT 0x1026b0, BIT4
#define GPIO85_IN  0x1026b0, BIT0

#define GPIO86_PAD PAD_CI_A2
#define GPIO86_OEN 0x1026b2, BIT5
#define GPIO86_OUT 0x1026b2, BIT4
#define GPIO86_IN  0x1026b2, BIT0

#define GPIO87_PAD PAD_CI_A3
#define GPIO87_OEN 0x1026b4, BIT5
#define GPIO87_OUT 0x1026b4, BIT4
#define GPIO87_IN  0x1026b4, BIT0

#define GPIO88_PAD PAD_CI_A4
#define GPIO88_OEN 0x1026b6, BIT5
#define GPIO88_OUT 0x1026b6, BIT4
#define GPIO88_IN  0x1026b6, BIT0

#define GPIO89_PAD PAD_CI_A5
#define GPIO89_OEN 0x1026b8, BIT5
#define GPIO89_OUT 0x1026b8, BIT4
#define GPIO89_IN  0x1026b8, BIT0

#define GPIO90_PAD PAD_CI_A6
#define GPIO90_OEN 0x1026ba, BIT5
#define GPIO90_OUT 0x1026ba, BIT4
#define GPIO90_IN  0x1026ba, BIT0

#define GPIO91_PAD PAD_CI_A7
#define GPIO91_OEN 0x1026bc, BIT5
#define GPIO91_OUT 0x1026bc, BIT4
#define GPIO91_IN  0x1026bc, BIT0

#define GPIO92_PAD PAD_CI_A8
#define GPIO92_OEN 0x1026be, BIT5
#define GPIO92_OUT 0x1026be, BIT4
#define GPIO92_IN  0x1026be, BIT0

#define GPIO93_PAD PAD_CI_A9
#define GPIO93_OEN 0x1026c0, BIT5
#define GPIO93_OUT 0x1026c0, BIT4
#define GPIO93_IN  0x1026c0, BIT0

#define GPIO94_PAD PAD_CI_A10
#define GPIO94_OEN 0x1026c2, BIT5
#define GPIO94_OUT 0x1026c2, BIT4
#define GPIO94_IN  0x1026c2, BIT0

#define GPIO95_PAD PAD_CI_A11
#define GPIO95_OEN 0x1026c4, BIT5
#define GPIO95_OUT 0x1026c4, BIT4
#define GPIO95_IN  0x1026c4, BIT0

#define GPIO96_PAD PAD_CI_A12
#define GPIO96_OEN 0x1026c6, BIT5
#define GPIO96_OUT 0x1026c6, BIT4
#define GPIO96_IN  0x1026c6, BIT0

#define GPIO97_PAD PAD_CI_A13
#define GPIO97_OEN 0x1026c8, BIT5
#define GPIO97_OUT 0x1026c8, BIT4
#define GPIO97_IN  0x1026c8, BIT0

#define GPIO98_PAD PAD_CI_A14
#define GPIO98_OEN 0x1026ca, BIT5
#define GPIO98_OUT 0x1026ca, BIT4
#define GPIO98_IN  0x1026ca, BIT0

#define GPIO99_PAD PAD_CI_D0
#define GPIO99_OEN 0x1026cc, BIT5
#define GPIO99_OUT 0x1026cc, BIT4
#define GPIO99_IN  0x1026cc, BIT0

#define GPIO100_PAD PAD_CI_D1
#define GPIO100_OEN 0x1026ce, BIT5
#define GPIO100_OUT 0x1026ce, BIT4
#define GPIO100_IN  0x1026ce, BIT0

#define GPIO101_PAD PAD_CI_D2
#define GPIO101_OEN 0x1026d0, BIT5
#define GPIO101_OUT 0x1026d0, BIT4
#define GPIO101_IN  0x1026d0, BIT0

#define GPIO102_PAD PAD_CI_D3
#define GPIO102_OEN 0x1026d2, BIT5
#define GPIO102_OUT 0x1026d2, BIT4
#define GPIO102_IN  0x1026d2, BIT0

#define GPIO103_PAD PAD_CI_D4
#define GPIO103_OEN 0x1026d4, BIT5
#define GPIO103_OUT 0x1026d4, BIT4
#define GPIO103_IN  0x1026d4, BIT0

#define GPIO104_PAD PAD_CI_D5
#define GPIO104_OEN 0x1026d6, BIT5
#define GPIO104_OUT 0x1026d6, BIT4
#define GPIO104_IN  0x1026d6, BIT0

#define GPIO105_PAD PAD_CI_D6
#define GPIO105_OEN 0x1026d8, BIT5
#define GPIO105_OUT 0x1026d8, BIT4
#define GPIO105_IN  0x1026d8, BIT0

#define GPIO106_PAD PAD_CI_D7
#define GPIO106_OEN 0x1026da, BIT5
#define GPIO106_OUT 0x1026da, BIT4
#define GPIO106_IN  0x1026da, BIT0

#define GPIO107_PAD PAD_TS0_CLK
#define GPIO107_OEN 0x102524, BIT5
#define GPIO107_OUT 0x102524, BIT4
#define GPIO107_IN  0x102524, BIT0

#define GPIO108_PAD PAD_TS0_SYNC
#define GPIO108_OEN 0x102526, BIT5
#define GPIO108_OUT 0x102526, BIT4
#define GPIO108_IN  0x102526, BIT0

#define GPIO109_PAD PAD_TS0_VLD
#define GPIO109_OEN 0x102528, BIT5
#define GPIO109_OUT 0x102528, BIT4
#define GPIO109_IN  0x102528, BIT0

#define GPIO110_PAD PAD_TS0_D0
#define GPIO110_OEN 0x10252a, BIT5
#define GPIO110_OUT 0x10252a, BIT4
#define GPIO110_IN  0x10252a, BIT0

#define GPIO111_PAD PAD_TS0_D1
#define GPIO111_OEN 0x10252c, BIT5
#define GPIO111_OUT 0x10252c, BIT4
#define GPIO111_IN  0x10252c, BIT0

#define GPIO112_PAD PAD_TS0_D2
#define GPIO112_OEN 0x10252e, BIT5
#define GPIO112_OUT 0x10252e, BIT4
#define GPIO112_IN  0x10252e, BIT0

#define GPIO113_PAD PAD_TS0_D3
#define GPIO113_OEN 0x102530, BIT5
#define GPIO113_OUT 0x102530, BIT4
#define GPIO113_IN  0x102530, BIT0

#define GPIO114_PAD PAD_TS0_D4
#define GPIO114_OEN 0x102532, BIT5
#define GPIO114_OUT 0x102532, BIT4
#define GPIO114_IN  0x102532, BIT0

#define GPIO115_PAD PAD_TS0_D5
#define GPIO115_OEN 0x102534, BIT5
#define GPIO115_OUT 0x102534, BIT4
#define GPIO115_IN  0x102534, BIT0

#define GPIO116_PAD PAD_TS0_D6
#define GPIO116_OEN 0x102536, BIT5
#define GPIO116_OUT 0x102536, BIT4
#define GPIO116_IN  0x102536, BIT0

#define GPIO117_PAD PAD_TS0_D7
#define GPIO117_OEN 0x102538, BIT5
#define GPIO117_OUT 0x102538, BIT4
#define GPIO117_IN  0x102538, BIT0

#define GPIO118_PAD PAD_TS1_CLK
#define GPIO118_OEN 0x10263a, BIT5
#define GPIO118_OUT 0x10263a, BIT4
#define GPIO118_IN  0x10263a, BIT0

#define GPIO119_PAD PAD_TS1_SYNC
#define GPIO119_OEN 0x10263c, BIT5
#define GPIO119_OUT 0x10263c, BIT4
#define GPIO119_IN  0x10263c, BIT0

#define GPIO120_PAD PAD_TS1_VLD
#define GPIO120_OEN 0x10263e, BIT5
#define GPIO120_OUT 0x10263e, BIT4
#define GPIO120_IN  0x10263e, BIT0

#define GPIO121_PAD PAD_TS1_D0
#define GPIO121_OEN 0x102640, BIT5
#define GPIO121_OUT 0x102640, BIT4
#define GPIO121_IN  0x102640, BIT0

#define GPIO122_PAD PAD_TS1_D1
#define GPIO122_OEN 0x102642, BIT5
#define GPIO122_OUT 0x102642, BIT4
#define GPIO122_IN  0x102642, BIT0

#define GPIO123_PAD PAD_TS1_D2
#define GPIO123_OEN 0x102644, BIT5
#define GPIO123_OUT 0x102644, BIT4
#define GPIO123_IN  0x102644, BIT0

#define GPIO124_PAD PAD_TS1_D3
#define GPIO124_OEN 0x102646, BIT5
#define GPIO124_OUT 0x102646, BIT4
#define GPIO124_IN  0x102646, BIT0

#define GPIO125_PAD PAD_TS1_D4
#define GPIO125_OEN 0x102648, BIT5
#define GPIO125_OUT 0x102648, BIT4
#define GPIO125_IN  0x102648, BIT0

#define GPIO126_PAD PAD_TS1_D5
#define GPIO126_OEN 0x10264a, BIT5
#define GPIO126_OUT 0x10264a, BIT4
#define GPIO126_IN  0x10264a, BIT0

#define GPIO127_PAD PAD_TS1_D6
#define GPIO127_OEN 0x10264c, BIT5
#define GPIO127_OUT 0x10264c, BIT4
#define GPIO127_IN  0x10264c, BIT0

#define GPIO128_PAD PAD_TS1_D7
#define GPIO128_OEN 0x10264e, BIT5
#define GPIO128_OUT 0x10264e, BIT4
#define GPIO128_IN  0x10264e, BIT0

#define GPIO129_PAD PAD_TS2_CLK
#define GPIO129_OEN 0x102650, BIT5
#define GPIO129_OUT 0x102650, BIT4
#define GPIO129_IN  0x102650, BIT0

#define GPIO130_PAD PAD_TS2_SYNC
#define GPIO130_OEN 0x102652, BIT5
#define GPIO130_OUT 0x102652, BIT4
#define GPIO130_IN  0x102652, BIT0

#define GPIO131_PAD PAD_TS2_VLD
#define GPIO131_OEN 0x102654, BIT5
#define GPIO131_OUT 0x102654, BIT4
#define GPIO131_IN  0x102654, BIT0

#define GPIO132_PAD PAD_TS2_D0
#define GPIO132_OEN 0x102656, BIT5
#define GPIO132_OUT 0x102656, BIT4
#define GPIO132_IN  0x102656, BIT0

#define GPIO133_PAD PAD_TS2_D1
#define GPIO133_OEN 0x102658, BIT5
#define GPIO133_OUT 0x102658, BIT4
#define GPIO133_IN  0x102658, BIT0

#define GPIO134_PAD PAD_TS2_D2
#define GPIO134_OEN 0x10265a, BIT5
#define GPIO134_OUT 0x10265a, BIT4
#define GPIO134_IN  0x10265a, BIT0

#define GPIO135_PAD PAD_TS2_D3
#define GPIO135_OEN 0x10265c, BIT5
#define GPIO135_OUT 0x10265c, BIT4
#define GPIO135_IN  0x10265c, BIT0

#define GPIO136_PAD PAD_TS2_D4
#define GPIO136_OEN 0x10265e, BIT5
#define GPIO136_OUT 0x10265e, BIT4
#define GPIO136_IN  0x10265e, BIT0

#define GPIO137_PAD PAD_TS2_D5
#define GPIO137_OEN 0x102660, BIT5
#define GPIO137_OUT 0x102660, BIT4
#define GPIO137_IN  0x102660, BIT0

#define GPIO138_PAD PAD_TS2_D6
#define GPIO138_OEN 0x102662, BIT5
#define GPIO138_OUT 0x102662, BIT4
#define GPIO138_IN  0x102662, BIT0

#define GPIO139_PAD PAD_TS2_D7
#define GPIO139_OEN 0x102664, BIT5
#define GPIO139_OUT 0x102664, BIT4
#define GPIO139_IN  0x102664, BIT0

#define GPIO140_PAD PAD_SPDIF_OUT
#define GPIO140_OEN 0x1025a0, BIT5
#define GPIO140_OUT 0x1025a0, BIT4
#define GPIO140_IN  0x1025a0, BIT0

#define GPIO141_PAD PAD_I2CM0_SCL
#define GPIO141_OEN 0x10251c, BIT5
#define GPIO141_OUT 0x10251c, BIT4
#define GPIO141_IN  0x10251c, BIT0

#define GPIO142_PAD PAD_I2CM0_SDA
#define GPIO142_OEN 0x10251e, BIT5
#define GPIO142_OUT 0x10251e, BIT4
#define GPIO142_IN  0x10251e, BIT0

#define GPIO143_PAD PAD_I2CM1_SCL
#define GPIO143_OEN 0x102520, BIT5
#define GPIO143_OUT 0x102520, BIT4
#define GPIO143_IN  0x102520, BIT0

#define GPIO144_PAD PAD_I2CM1_SDA
#define GPIO144_OEN 0x102522, BIT5
#define GPIO144_OUT 0x102522, BIT4
#define GPIO144_IN  0x102522, BIT0

#define GPIO145_PAD PAD_HDMITX_SCL
#define GPIO145_OEN 0x1025da, BIT5
#define GPIO145_OUT 0x1025da, BIT4
#define GPIO145_IN  0x1025da, BIT0

#define GPIO146_PAD PAD_HDMITX_SDA
#define GPIO146_OEN 0x1025dc, BIT5
#define GPIO146_OUT 0x1025dc, BIT4
#define GPIO146_IN  0x1025dc, BIT0

#define GPIO147_PAD PAD_HDMITX_ARC
#define GPIO147_OEN 0x1025de, BIT5
#define GPIO147_OUT 0x1025de, BIT4
#define GPIO147_IN  0x1025de, BIT0

#define GPIO148_PAD PAD_HDMIRX_ARC
#define GPIO148_OEN 0x1025e0, BIT5
#define GPIO148_OUT 0x1025e0, BIT4
#define GPIO148_IN  0x1025e0, BIT0

#define GPIO149_PAD PAD_EMMC_IO0
#define GPIO149_OEN 0x102570, BIT5
#define GPIO149_OUT 0x102570, BIT4
#define GPIO149_IN  0x102570, BIT0

#define GPIO150_PAD PAD_EMMC_IO1
#define GPIO150_OEN 0x102572, BIT5
#define GPIO150_OUT 0x102572, BIT4
#define GPIO150_IN  0x102572, BIT0

#define GPIO151_PAD PAD_EMMC_IO2
#define GPIO151_OEN 0x102574, BIT5
#define GPIO151_OUT 0x102574, BIT4
#define GPIO151_IN  0x102574, BIT0

#define GPIO152_PAD PAD_EMMC_IO3
#define GPIO152_OEN 0x102576, BIT5
#define GPIO152_OUT 0x102576, BIT4
#define GPIO152_IN  0x102576, BIT0

#define GPIO153_PAD PAD_EMMC_IO4
#define GPIO153_OEN 0x102578, BIT5
#define GPIO153_OUT 0x102578, BIT4
#define GPIO153_IN  0x102578, BIT0

#define GPIO154_PAD PAD_EMMC_IO5
#define GPIO154_OEN 0x10257a, BIT5
#define GPIO154_OUT 0x10257a, BIT4
#define GPIO154_IN  0x10257a, BIT0

#define GPIO155_PAD PAD_EMMC_IO6
#define GPIO155_OEN 0x10257c, BIT5
#define GPIO155_OUT 0x10257c, BIT4
#define GPIO155_IN  0x10257c, BIT0

#define GPIO156_PAD PAD_EMMC_IO7
#define GPIO156_OEN 0x10257e, BIT5
#define GPIO156_OUT 0x10257e, BIT4
#define GPIO156_IN  0x10257e, BIT0

#define GPIO157_PAD PAD_EMMC_IO8
#define GPIO157_OEN 0x102580, BIT5
#define GPIO157_OUT 0x102580, BIT4
#define GPIO157_IN  0x102580, BIT0

#define GPIO158_PAD PAD_EMMC_IO9
#define GPIO158_OEN 0x102582, BIT5
#define GPIO158_OUT 0x102582, BIT4
#define GPIO158_IN  0x102582, BIT0

#define GPIO159_PAD PAD_EMMC_IO10
#define GPIO159_OEN 0x102584, BIT5
#define GPIO159_OUT 0x102584, BIT4
#define GPIO159_IN  0x102584, BIT0

#define GPIO160_PAD PAD_EMMC_IO11
#define GPIO160_OEN 0x102586, BIT5
#define GPIO160_OUT 0x102586, BIT4
#define GPIO160_IN  0x102586, BIT0

#define GPIO161_PAD PAD_EMMC_IO12
#define GPIO161_OEN 0x102588, BIT5
#define GPIO161_OUT 0x102588, BIT4
#define GPIO161_IN  0x102588, BIT0

#define GPIO162_PAD PAD_EMMC_IO13
#define GPIO162_OEN 0x10258a, BIT5
#define GPIO162_OUT 0x10258a, BIT4
#define GPIO162_IN  0x10258a, BIT0

#define GPIO163_PAD PAD_EMMC_IO14
#define GPIO163_OEN 0x10258c, BIT5
#define GPIO163_OUT 0x10258c, BIT4
#define GPIO163_IN  0x10258c, BIT0

#define GPIO164_PAD PAD_EMMC_IO15
#define GPIO164_OEN 0x10258e, BIT5
#define GPIO164_OUT 0x10258e, BIT4
#define GPIO164_IN  0x10258e, BIT0

#define GPIO165_PAD PAD_EMMC_IO16
#define GPIO165_OEN 0x102590, BIT5
#define GPIO165_OUT 0x102590, BIT4
#define GPIO165_IN  0x102590, BIT0

#define GPIO166_PAD PAD_EMMC_IO17
#define GPIO166_OEN 0x102592, BIT5
#define GPIO166_OUT 0x102592, BIT4
#define GPIO166_IN  0x102592, BIT0

#define GPIO167_PAD PAD_SDIO_IO0
#define GPIO167_OEN 0x102594, BIT5
#define GPIO167_OUT 0x102594, BIT4
#define GPIO167_IN  0x102594, BIT0

#define GPIO168_PAD PAD_SDIO_IO1
#define GPIO168_OEN 0x102596, BIT5
#define GPIO168_OUT 0x102596, BIT4
#define GPIO168_IN  0x102596, BIT0

#define GPIO169_PAD PAD_SDIO_IO2
#define GPIO169_OEN 0x102598, BIT5
#define GPIO169_OUT 0x102598, BIT4
#define GPIO169_IN  0x102598, BIT0

#define GPIO170_PAD PAD_SDIO_IO3
#define GPIO170_OEN 0x10259a, BIT5
#define GPIO170_OUT 0x10259a, BIT4
#define GPIO170_IN  0x10259a, BIT0

#define GPIO171_PAD PAD_SDIO_IO4
#define GPIO171_OEN 0x10259c, BIT5
#define GPIO171_OUT 0x10259c, BIT4
#define GPIO171_IN  0x10259c, BIT0

#define GPIO172_PAD PAD_SDIO_IO5
#define GPIO172_OEN 0x10259e, BIT5
#define GPIO172_OUT 0x10259e, BIT4
#define GPIO172_IN  0x10259e, BIT0

#define GPIO173_PAD PAD_GT0_MDIO
#define GPIO173_OEN 0x10253a, BIT5
#define GPIO173_OUT 0x10253a, BIT4
#define GPIO173_IN  0x10253a, BIT0

#define GPIO174_PAD PAD_GT0_MDC
#define GPIO174_OEN 0x10253c, BIT5
#define GPIO174_OUT 0x10253c, BIT4
#define GPIO174_IN  0x10253c, BIT0

#define GPIO175_PAD PAD_GT0_RX_CLK
#define GPIO175_OEN 0x10253e, BIT5
#define GPIO175_OUT 0x10253e, BIT4
#define GPIO175_IN  0x10253e, BIT0

#define GPIO176_PAD PAD_GT0_RX_CTL
#define GPIO176_OEN 0x102540, BIT5
#define GPIO176_OUT 0x102540, BIT4
#define GPIO176_IN  0x102540, BIT0

#define GPIO177_PAD PAD_GT0_RX_D0
#define GPIO177_OEN 0x102542, BIT5
#define GPIO177_OUT 0x102542, BIT4
#define GPIO177_IN  0x102542, BIT0

#define GPIO178_PAD PAD_GT0_RX_D1
#define GPIO178_OEN 0x102544, BIT5
#define GPIO178_OUT 0x102544, BIT4
#define GPIO178_IN  0x102544, BIT0

#define GPIO179_PAD PAD_GT0_RX_D2
#define GPIO179_OEN 0x102546, BIT5
#define GPIO179_OUT 0x102546, BIT4
#define GPIO179_IN  0x102546, BIT0

#define GPIO180_PAD PAD_GT0_RX_D3
#define GPIO180_OEN 0x102548, BIT5
#define GPIO180_OUT 0x102548, BIT4
#define GPIO180_IN  0x102548, BIT0

#define GPIO181_PAD PAD_GT0_TX_CLK
#define GPIO181_OEN 0x10254a, BIT5
#define GPIO181_OUT 0x10254a, BIT4
#define GPIO181_IN  0x10254a, BIT0

#define GPIO182_PAD PAD_GT0_TX_CTL
#define GPIO182_OEN 0x10254c, BIT5
#define GPIO182_OUT 0x10254c, BIT4
#define GPIO182_IN  0x10254c, BIT0

#define GPIO183_PAD PAD_GT0_TX_D0
#define GPIO183_OEN 0x10254e, BIT5
#define GPIO183_OUT 0x10254e, BIT4
#define GPIO183_IN  0x10254e, BIT0

#define GPIO184_PAD PAD_GT0_TX_D1
#define GPIO184_OEN 0x102550, BIT5
#define GPIO184_OUT 0x102550, BIT4
#define GPIO184_IN  0x102550, BIT0

#define GPIO185_PAD PAD_GT0_TX_D2
#define GPIO185_OEN 0x102552, BIT5
#define GPIO185_OUT 0x102552, BIT4
#define GPIO185_IN  0x102552, BIT0

#define GPIO186_PAD PAD_GT0_TX_D3
#define GPIO186_OEN 0x102554, BIT5
#define GPIO186_OUT 0x102554, BIT4
#define GPIO186_IN  0x102554, BIT0



//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

static const struct gpio_setting
{
    MS_U32 r_oen;
    MS_U8  m_oen;
    MS_U32 r_out;
    MS_U8  m_out;
    MS_U32 r_in;
    MS_U8  m_in;
} gpio_table[] =
{
#define __GPIO__(_x_)   { CONCAT(CONCAT(GPIO, _x_), _OEN),   \
                          CONCAT(CONCAT(GPIO, _x_), _OUT),   \
                          CONCAT(CONCAT(GPIO, _x_), _IN) }
#define __GPIO(_x_)     __GPIO__(_x_)

//
// !! WARNING !! DO NOT MODIFIY !!!!
//
// These defines order must match following
// 1. the PAD name in GPIO excel
// 2. the perl script to generate the package header file
//
    __GPIO(999), // 0 is not used

    __GPIO(0), __GPIO(1), __GPIO(2), __GPIO(3), __GPIO(4),
    __GPIO(5), __GPIO(6), __GPIO(7), __GPIO(8), __GPIO(9),
    __GPIO(10), __GPIO(11), __GPIO(12), __GPIO(13), __GPIO(14),
    __GPIO(15), __GPIO(16), __GPIO(17), __GPIO(18), __GPIO(19),
    __GPIO(20), __GPIO(21), __GPIO(22), __GPIO(23), __GPIO(24),
    __GPIO(25), __GPIO(26), __GPIO(27), __GPIO(28), __GPIO(29),
    __GPIO(30), __GPIO(31), __GPIO(32), __GPIO(33), __GPIO(34),
    __GPIO(35), __GPIO(36), __GPIO(37), __GPIO(38), __GPIO(39),
    __GPIO(40), __GPIO(41), __GPIO(42), __GPIO(43), __GPIO(44),
    __GPIO(45), __GPIO(46), __GPIO(47), __GPIO(48), __GPIO(49),
    __GPIO(50), __GPIO(51), __GPIO(52), __GPIO(53), __GPIO(54),
    __GPIO(55), __GPIO(56), __GPIO(57), __GPIO(58), __GPIO(59),
    __GPIO(60), __GPIO(61), __GPIO(62), __GPIO(63), __GPIO(64),
    __GPIO(65), __GPIO(66), __GPIO(67), __GPIO(68), __GPIO(69),
    __GPIO(70), __GPIO(71), __GPIO(72), __GPIO(73), __GPIO(74),
    __GPIO(75), __GPIO(76), __GPIO(77), __GPIO(78), __GPIO(79),
    __GPIO(80), __GPIO(81), __GPIO(82), __GPIO(83), __GPIO(84),
    __GPIO(85), __GPIO(86), __GPIO(87), __GPIO(88), __GPIO(89),
    __GPIO(90), __GPIO(91), __GPIO(92), __GPIO(93), __GPIO(94),
    __GPIO(95), __GPIO(96), __GPIO(97), __GPIO(98), __GPIO(99),
    __GPIO(100), __GPIO(101), __GPIO(102), __GPIO(103), __GPIO(104),
    __GPIO(105), __GPIO(106), __GPIO(107), __GPIO(108), __GPIO(109),
    __GPIO(110), __GPIO(111), __GPIO(112), __GPIO(113), __GPIO(114),
    __GPIO(115), __GPIO(116), __GPIO(117), __GPIO(118), __GPIO(119),
    __GPIO(120), __GPIO(121), __GPIO(122), __GPIO(123), __GPIO(124),
    __GPIO(125), __GPIO(126), __GPIO(127), __GPIO(128), __GPIO(129),
    __GPIO(130), __GPIO(131), __GPIO(132), __GPIO(133), __GPIO(134),
    __GPIO(135), __GPIO(136), __GPIO(137), __GPIO(138), __GPIO(139),
    __GPIO(140), __GPIO(141), __GPIO(142), __GPIO(143), __GPIO(144),
    __GPIO(145), __GPIO(146), __GPIO(147), __GPIO(148), __GPIO(149),
    __GPIO(150), __GPIO(151), __GPIO(152), __GPIO(153), __GPIO(154),
    __GPIO(155), __GPIO(156), __GPIO(157), __GPIO(158), __GPIO(159),
    __GPIO(160), __GPIO(161), __GPIO(162), __GPIO(163), __GPIO(164),
    __GPIO(165), __GPIO(166), __GPIO(167), __GPIO(168), __GPIO(169),
    __GPIO(170), __GPIO(171), __GPIO(172), __GPIO(173), __GPIO(174),
    __GPIO(175), __GPIO(176), __GPIO(177), __GPIO(178), __GPIO(179),
    __GPIO(180), __GPIO(181), __GPIO(182), __GPIO(183), __GPIO(184),
    __GPIO(185), __GPIO(186),

};


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_MapBase_PM = 0;
static MS_VIRT _gMIO_MapBase_nonPM = 0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
static void HAL_Gpio_Set_High(MS_GPIO_NUM gpio)
{
    if (gpio >= 1 && gpio <= END_GPIO_NUM)
    {
        _gMIO_MapBase = _gMIO_MapBase_PM;
        MDrv_WriteRegBit(gpio_table[gpio].r_out, 1, gpio_table[gpio].m_out);
        MDrv_WriteRegBit(gpio_table[gpio].r_oen, 0, gpio_table[gpio].m_oen);
    }
    else
    {
        MS_ASSERT(0);
    }
}

static void HAL_Gpio_Set_Low(MS_GPIO_NUM gpio)
{
    if (gpio >= 1 && gpio <= END_GPIO_NUM)
    {
        _gMIO_MapBase = _gMIO_MapBase_PM;
        MDrv_WriteRegBit(gpio_table[gpio].r_out, 0, gpio_table[gpio].m_out);
        MDrv_WriteRegBit(gpio_table[gpio].r_oen, 0, gpio_table[gpio].m_oen);
    }
    else
    {
        MS_ASSERT(0);
    }
}

static void HAL_Gpio_Set_Input(MS_GPIO_NUM gpio)
{
    if (gpio >= 1 && gpio <= END_GPIO_NUM)
    {
        _gMIO_MapBase = _gMIO_MapBase_PM;
        MDrv_WriteRegBit(gpio_table[gpio].r_oen, 1, gpio_table[gpio].m_oen);
    }
    else
    {
        MS_ASSERT(0);
    }
}

static int HAL_Gpio_Get_InOut(MS_GPIO_NUM gpio)
{
    int r;

    if (gpio >= 1 && gpio <= END_GPIO_NUM)
    {
        _gMIO_MapBase = _gMIO_MapBase_PM;
        r = MDrv_ReadRegBit(gpio_table[gpio].r_oen, gpio_table[gpio].m_oen);
    }
    else
    {
        r = 0;
        MS_ASSERT(0);
    }

    return (r != 0);
}


static int HAL_Gpio_Get_Level(MS_GPIO_NUM gpio)
{
    int r;

    if (gpio >= 1 && gpio <= END_GPIO_NUM)
    {
        _gMIO_MapBase = _gMIO_MapBase_PM;
        r = MDrv_ReadRegBit(gpio_table[gpio].r_in, gpio_table[gpio].m_in);
    }
    else
    {
        r = 0;
        MS_ASSERT(0);
    }

    return (r != 0);
}

const struct gpio_operations __gpio =
{
    .set_high  = HAL_Gpio_Set_High,
    .set_low   = HAL_Gpio_Set_Low,
    .set_input = HAL_Gpio_Set_Input,
    .get_inout = HAL_Gpio_Get_InOut,
    .get_level = HAL_Gpio_Get_Level,
};


void HAL_GPIO_SetIOMapBase(MS_VIRT u32Base)
{
    _gMIO_MapBase_nonPM = u32Base;
}

void HAL_GPIO_SetIOMapBase_PM(MS_VIRT u32Base)
{
    _gMIO_MapBase_PM = u32Base;
}

void HAL_GPIO_Interrupt_Init(void)
{

}

void HAL_GPIO_Interrupt_Action(void)
{

}

GPIO_Result HAL_GPIO_Attach_Interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback)
{
    printf("Not Support HAL_GPIO_Attach_Interrupt %d, %d \n", gpio_num, gpio_edge_type);
    return E_GPIO_NOT_SUPPORT;
}

GPIO_Result HAL_GPIO_Datach_Interrupt(MS_GPIO_NUM gpio_num)
{
    printf("Not Support HAL_GPIO_Datach_Interrupt %d\n", gpio_num);
    return E_GPIO_NOT_SUPPORT;
}

GPIO_Result HAL_GPIO_Enable_Interrupt(MS_GPIO_NUM gpio)
{
    printf("Not Support HAL_GPIO_Enable_Interrupt %d\n", gpio);
    return E_GPIO_NOT_SUPPORT;
}

GPIO_Result HAL_GPIO_Disable_Interrupt(MS_GPIO_NUM gpio)
{
    printf("Not Support HAL_GPIO_Disable_Interrupt %d\n", gpio);
    return E_GPIO_NOT_SUPPORT;
}

void HAL_GPIO_Enable_Interrupt_all(void)
{

}

void HAL_GPIO_Disable_Interrupt_all(void)
{

}

#endif      //_HALGPIO_C_
