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
#include "ULog.h"

#if defined(MSOS_TYPE_LINUX)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_GPIO "GPIO"

#define GPIO_HAL_ERR(x, args...)        //{ULOGE(TAG_GPIO, x, ##args);}
#define GPIO_HAL_NOTSUPPORT()           {ULOGD(TAG_GPIO, "%s not support!\n", __FUNCTION__);}

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
#define BIT8    BIT(8)
#define BIT9    BIT(9)
#define BIT10   BIT(10)
#define BIT11   BIT(11)
#define BIT12   BIT(12)
#define BIT13   BIT(13)
#define BIT14   BIT(14)
#define BIT15   BIT(15)

// Dummy
#define GPIO999_OEN     0, 0
#define GPIO999_OUT     0, 0
#define GPIO999_IN      0, 0

#define GPIO1_PAD PAD_PM_SPI_CK
#define GPIO1_OEN 0x0f30, BIT0
#define GPIO1_OUT 0x0f30, BIT1
#define GPIO1_IN  0x0f30, BIT2

#define GPIO2_PAD PAD_PM_SPI_DI
#define GPIO2_OEN 0x0f32, BIT0
#define GPIO2_OUT 0x0f32, BIT1
#define GPIO2_IN  0x0f32, BIT2

#define GPIO3_PAD PAD_PM_SPI_DO
#define GPIO3_OEN 0x0f34, BIT0
#define GPIO3_OUT 0x0f34, BIT1
#define GPIO3_IN  0x0f34, BIT2

#define GPIO4_PAD PAD_IRIN
#define GPIO4_OEN 0x0f26, BIT0
#define GPIO4_OUT 0x0f26, BIT1
#define GPIO4_IN  0x0f26, BIT2

#define GPIO5_PAD PAD_CEC0
#define GPIO5_OEN 0x0f2a, BIT0
#define GPIO5_OUT 0x0f2a, BIT1
#define GPIO5_IN  0x0f2a, BIT2

#define GPIO6_PAD PAD_PWM_PM
#define GPIO6_OEN 0x0f28, BIT0
#define GPIO6_OUT 0x0f28, BIT1
#define GPIO6_IN  0x0f28, BIT2

#define GPIO7_PAD PAD_DDCA_CK
#define GPIO7_OEN 0x0494, BIT1
#define GPIO7_OUT 0x0494, BIT2
#define GPIO7_IN  0x0494, BIT0

#define GPIO8_PAD PAD_DDCA_DA
#define GPIO8_OEN 0x0494, BIT5
#define GPIO8_OUT 0x0494, BIT6
#define GPIO8_IN  0x0494, BIT4

#define GPIO9_PAD PAD_GPIO0_PM
#define GPIO9_OEN 0x0f00, BIT0
#define GPIO9_OUT 0x0f00, BIT1
#define GPIO9_IN  0x0f00, BIT2

#define GPIO10_PAD PAD_GPIO1_PM
#define GPIO10_OEN 0x0f02, BIT0
#define GPIO10_OUT 0x0f02, BIT1
#define GPIO10_IN  0x0f02, BIT2

#define GPIO11_PAD PAD_GPIO2_PM
#define GPIO11_OEN 0x0f04, BIT0
#define GPIO11_OUT 0x0f04, BIT1
#define GPIO11_IN  0x0f04, BIT2

#define GPIO12_PAD PAD_GPIO3_PM
#define GPIO12_OEN 0x0f06, BIT0
#define GPIO12_OUT 0x0f06, BIT1
#define GPIO12_IN  0x0f06, BIT2

#define GPIO13_PAD PAD_GPIO4_PM
#define GPIO13_OEN 0x0f08, BIT0
#define GPIO13_OUT 0x0f08, BIT1
#define GPIO13_IN  0x0f08, BIT2

#define GPIO14_PAD PAD_GPIO5_PM
#define GPIO14_OEN 0x0f0a, BIT0
#define GPIO14_OUT 0x0f0a, BIT1
#define GPIO14_IN  0x0f0a, BIT2

#define GPIO15_PAD PAD_GPIO6_PM
#define GPIO15_OEN 0x0f0c, BIT0
#define GPIO15_OUT 0x0f0c, BIT1
#define GPIO15_IN  0x0f0c, BIT2

#define GPIO16_PAD PAD_GPIO7_PM
#define GPIO16_OEN 0x0f0e, BIT0
#define GPIO16_OUT 0x0f0e, BIT1
#define GPIO16_IN  0x0f0e, BIT2

#define GPIO17_PAD PAD_GPIO8_PM
#define GPIO17_OEN 0x0f10, BIT0
#define GPIO17_OUT 0x0f10, BIT1
#define GPIO17_IN  0x0f10, BIT2

#define GPIO18_PAD PAD_GPIO9_PM
#define GPIO18_OEN 0x0f12, BIT0
#define GPIO18_OUT 0x0f12, BIT1
#define GPIO18_IN  0x0f12, BIT2

#define GPIO19_PAD PAD_GPIO10_PM
#define GPIO19_OEN 0x0f14, BIT0
#define GPIO19_OUT 0x0f14, BIT1
#define GPIO19_IN  0x0f14, BIT2

#define GPIO20_PAD PAD_GPIO11_PM
#define GPIO20_OEN 0x0f16, BIT0
#define GPIO20_OUT 0x0f16, BIT1
#define GPIO20_IN  0x0f16, BIT2

#define GPIO21_PAD PAD_GPIO12_PM
#define GPIO21_OEN 0x0f18, BIT0
#define GPIO21_OUT 0x0f18, BIT1
#define GPIO21_IN  0x0f18, BIT2

#define GPIO22_PAD PAD_GPIO13_PM
#define GPIO22_OEN 0x0f1a, BIT0
#define GPIO22_OUT 0x0f1a, BIT1
#define GPIO22_IN  0x0f1a, BIT2

#define GPIO23_PAD PAD_GPIO14_PM
#define GPIO23_OEN 0x0f1c, BIT0
#define GPIO23_OUT 0x0f1c, BIT1
#define GPIO23_IN  0x0f1c, BIT2

#define GPIO24_PAD PAD_GPIO15_PM
#define GPIO24_OEN 0x0f1e, BIT0
#define GPIO24_OUT 0x0f1e, BIT1
#define GPIO24_IN  0x0f1e, BIT2

#define GPIO25_PAD PAD_GPIO16_PM
#define GPIO25_OEN 0x0f20, BIT0
#define GPIO25_OUT 0x0f20, BIT1
#define GPIO25_IN  0x0f20, BIT2

#define GPIO26_PAD PAD_LED0
#define GPIO26_OEN 0x2e80, BIT1
#define GPIO26_OUT 0x2e80, BIT0
#define GPIO26_IN  0x2e80, BIT2

#define GPIO27_PAD PAD_LED1
#define GPIO27_OEN 0x2e81, BIT1
#define GPIO27_OUT 0x2e81, BIT0
#define GPIO27_IN  0x2e81, BIT2

#define GPIO28_PAD PAD_HOTPLUGA
#define GPIO28_OEN 0x0e4e, BIT0
#define GPIO28_OUT 0x0e4e, BIT4
#define GPIO28_IN  0x0e4f, BIT0

#define GPIO29_PAD PAD_HOTPLUGB
#define GPIO29_OEN 0x0e4e, BIT1
#define GPIO29_OUT 0x0e4e, BIT5
#define GPIO29_IN  0x0e4f, BIT1

#define GPIO30_PAD PAD_HOTPLUGC
#define GPIO30_OEN 0x0e4e, BIT2
#define GPIO30_OUT 0x0e4e, BIT6
#define GPIO30_IN  0x0e4f, BIT2

#define GPIO31_PAD PAD_HOTPLUGD
#define GPIO31_OEN 0x0e4e, BIT3
#define GPIO31_OUT 0x0e4e, BIT7
#define GPIO31_IN  0x0e4f, BIT3

#define GPIO32_PAD PAD_HOTPLUGA_HDMI20_5V
#define GPIO32_OEN 0x010218, BIT5
#define GPIO32_OUT 0x010218, BIT4
#define GPIO32_IN  0x010218, BIT6

#define GPIO33_PAD PAD_HOTPLUGB_HDMI20_5V
#define GPIO33_OEN 0x010318, BIT5
#define GPIO33_OUT 0x010318, BIT4
#define GPIO33_IN  0x010318, BIT6

#define GPIO34_PAD PAD_HOTPLUGC_HDMI20_5V
#define GPIO34_OEN 0x010418, BIT5
#define GPIO34_OUT 0x010418, BIT4
#define GPIO34_IN  0x010418, BIT6

#define GPIO35_PAD PAD_HOTPLUGD_HDMI20_5V
#define GPIO35_OEN 0x010518, BIT5
#define GPIO35_OUT 0x010518, BIT4
#define GPIO35_IN  0x010518, BIT6

#define GPIO36_PAD PAD_DDCDA_CK
#define GPIO36_OEN 0x0496, BIT1
#define GPIO36_OUT 0x0496, BIT2
#define GPIO36_IN  0x0496, BIT0

#define GPIO37_PAD PAD_DDCDA_DA
#define GPIO37_OEN 0x0496, BIT5
#define GPIO37_OUT 0x0496, BIT6
#define GPIO37_IN  0x0496, BIT4

#define GPIO38_PAD PAD_DDCDB_CK
#define GPIO38_OEN 0x0497, BIT1
#define GPIO38_OUT 0x0497, BIT2
#define GPIO38_IN  0x0497, BIT0

#define GPIO39_PAD PAD_DDCDB_DA
#define GPIO39_OEN 0x0497, BIT5
#define GPIO39_OUT 0x0497, BIT6
#define GPIO39_IN  0x0497, BIT4

#define GPIO40_PAD PAD_DDCDC_CK
#define GPIO40_OEN 0x0498, BIT1
#define GPIO40_OUT 0x0498, BIT2
#define GPIO40_IN  0x0498, BIT0

#define GPIO41_PAD PAD_DDCDC_DA
#define GPIO41_OEN 0x0498, BIT5
#define GPIO41_OUT 0x0498, BIT6
#define GPIO41_IN  0x0498, BIT4

#define GPIO42_PAD PAD_DDCDD_CK
#define GPIO42_OEN 0x0499, BIT1
#define GPIO42_OUT 0x0499, BIT2
#define GPIO42_IN  0x0499, BIT0

#define GPIO43_PAD PAD_DDCDD_DA
#define GPIO43_OEN 0x0499, BIT5
#define GPIO43_OUT 0x0499, BIT6
#define GPIO43_IN  0x0499, BIT4

#define GPIO44_PAD PAD_SAR0
#define GPIO44_OEN 0x1423, BIT0
#define GPIO44_OUT 0x1424, BIT0
#define GPIO44_IN  0x1425, BIT0

#define GPIO45_PAD PAD_SAR1
#define GPIO45_OEN 0x1423, BIT1
#define GPIO45_OUT 0x1424, BIT1
#define GPIO45_IN  0x1425, BIT1

#define GPIO46_PAD PAD_SAR2
#define GPIO46_OEN 0x1423, BIT2
#define GPIO46_OUT 0x1424, BIT2
#define GPIO46_IN  0x1425, BIT2

#define GPIO47_PAD PAD_SAR3
#define GPIO47_OEN 0x1423, BIT3
#define GPIO47_OUT 0x1424, BIT3
#define GPIO47_IN  0x1425, BIT3

#define GPIO48_PAD PAD_SAR4
#define GPIO48_OEN 0x1423, BIT4
#define GPIO48_OUT 0x1424, BIT4
#define GPIO48_IN  0x1425, BIT4

#define GPIO49_PAD PAD_VPLUGIN
#define GPIO49_OEN 0x1423, BIT5
#define GPIO49_OUT 0x1424, BIT5
#define GPIO49_IN  0x1425, BIT5

#define GPIO50_PAD PAD_VID0
#define GPIO50_OEN 0x2e84, BIT1
#define GPIO50_OUT 0x2e84, BIT0
#define GPIO50_IN  0x2e84, BIT2

#define GPIO51_PAD PAD_VID1
#define GPIO51_OEN 0x2e85, BIT1
#define GPIO51_OUT 0x2e85, BIT0
#define GPIO51_IN  0x2e85, BIT2

#define GPIO52_PAD PAD_VID2
#define GPIO52_OEN 0x0f22, BIT0
#define GPIO52_OUT 0x0f22, BIT1
#define GPIO52_IN  0x0f22, BIT2

#define GPIO53_PAD PAD_VID3
#define GPIO53_OEN 0x0f24, BIT0
#define GPIO53_OUT 0x0f24, BIT1
#define GPIO53_IN  0x0f24, BIT2

#define GPIO54_PAD PAD_WOL_INT_OUT
#define GPIO54_OEN 0x2e82, BIT1
#define GPIO54_OUT 0x2e82, BIT0
#define GPIO54_IN  0x2e82, BIT2

#define GPIO55_PAD PAD_ARC0
#define GPIO55_OEN 0x110320, BIT1
#define GPIO55_OUT 0x110320, BIT0
#define GPIO55_IN  0x110320, BIT2

#define GPIO56_PAD PAD_DDCR_CK
#define GPIO56_OEN 0x102b87, BIT1
#define GPIO56_OUT 0x102b87, BIT0
#define GPIO56_IN  0x102b87, BIT2

#define GPIO57_PAD PAD_DDCR_DA
#define GPIO57_OEN 0x102b86, BIT1
#define GPIO57_OUT 0x102b86, BIT0
#define GPIO57_IN  0x102b86, BIT2

#define GPIO58_PAD PAD_DIM0
#define GPIO58_OEN 0x102bee, BIT1
#define GPIO58_OUT 0x102bee, BIT0
#define GPIO58_IN  0x102bee, BIT2

#define GPIO59_PAD PAD_DIM1
#define GPIO59_OEN 0x102bef, BIT1
#define GPIO59_OUT 0x102bef, BIT0
#define GPIO59_IN  0x102bef, BIT2

#define GPIO60_PAD PAD_DIM2
#define GPIO60_OEN 0x102bf0, BIT1
#define GPIO60_OUT 0x102bf0, BIT0
#define GPIO60_IN  0x102bf0, BIT2

#define GPIO61_PAD PAD_DIM3
#define GPIO61_OEN 0x102bf1, BIT1
#define GPIO61_OUT 0x102bf1, BIT0
#define GPIO61_IN  0x102bf1, BIT2

#define GPIO62_PAD PAD_GPIO0
#define GPIO62_OEN 0x102b00, BIT1
#define GPIO62_OUT 0x102b00, BIT0
#define GPIO62_IN  0x102b00, BIT2

#define GPIO63_PAD PAD_GPIO1
#define GPIO63_OEN 0x102b01, BIT1
#define GPIO63_OUT 0x102b01, BIT0
#define GPIO63_IN  0x102b01, BIT2

#define GPIO64_PAD PAD_GPIO2
#define GPIO64_OEN 0x102b02, BIT1
#define GPIO64_OUT 0x102b02, BIT0
#define GPIO64_IN  0x102b02, BIT2

#define GPIO65_PAD PAD_GPIO3
#define GPIO65_OEN 0x102b03, BIT1
#define GPIO65_OUT 0x102b03, BIT0
#define GPIO65_IN  0x102b03, BIT2

#define GPIO66_PAD PAD_GPIO4
#define GPIO66_OEN 0x102b04, BIT1
#define GPIO66_OUT 0x102b04, BIT0
#define GPIO66_IN  0x102b04, BIT2

#define GPIO67_PAD PAD_GPIO5
#define GPIO67_OEN 0x102b05, BIT1
#define GPIO67_OUT 0x102b05, BIT0
#define GPIO67_IN  0x102b05, BIT2

#define GPIO68_PAD PAD_GPIO6
#define GPIO68_OEN 0x102b06, BIT1
#define GPIO68_OUT 0x102b06, BIT0
#define GPIO68_IN  0x102b06, BIT2

#define GPIO69_PAD PAD_GPIO7
#define GPIO69_OEN 0x102b07, BIT1
#define GPIO69_OUT 0x102b07, BIT0
#define GPIO69_IN  0x102b07, BIT2

#define GPIO70_PAD PAD_GPIO8
#define GPIO70_OEN 0x102b08, BIT1
#define GPIO70_OUT 0x102b08, BIT0
#define GPIO70_IN  0x102b08, BIT2

#define GPIO71_PAD PAD_GPIO9
#define GPIO71_OEN 0x102b09, BIT1
#define GPIO71_OUT 0x102b09, BIT0
#define GPIO71_IN  0x102b09, BIT2

#define GPIO72_PAD PAD_GPIO10
#define GPIO72_OEN 0x102b0a, BIT1
#define GPIO72_OUT 0x102b0a, BIT0
#define GPIO72_IN  0x102b0a, BIT2

#define GPIO73_PAD PAD_GPIO11
#define GPIO73_OEN 0x102b0b, BIT1
#define GPIO73_OUT 0x102b0b, BIT0
#define GPIO73_IN  0x102b0b, BIT2

#define GPIO74_PAD PAD_GPIO12
#define GPIO74_OEN 0x102b0c, BIT1
#define GPIO74_OUT 0x102b0c, BIT0
#define GPIO74_IN  0x102b0c, BIT2

#define GPIO75_PAD PAD_GPIO13
#define GPIO75_OEN 0x102b0d, BIT1
#define GPIO75_OUT 0x102b0d, BIT0
#define GPIO75_IN  0x102b0d, BIT2

#define GPIO76_PAD PAD_GPIO14
#define GPIO76_OEN 0x102b0e, BIT1
#define GPIO76_OUT 0x102b0e, BIT0
#define GPIO76_IN  0x102b0e, BIT2

#define GPIO77_PAD PAD_GPIO15
#define GPIO77_OEN 0x102b0f, BIT1
#define GPIO77_OUT 0x102b0f, BIT0
#define GPIO77_IN  0x102b0f, BIT2

#define GPIO78_PAD PAD_GPIO16
#define GPIO78_OEN 0x102b10, BIT1
#define GPIO78_OUT 0x102b10, BIT0
#define GPIO78_IN  0x102b10, BIT2

#define GPIO79_PAD PAD_GPIO17
#define GPIO79_OEN 0x102b11, BIT1
#define GPIO79_OUT 0x102b11, BIT0
#define GPIO79_IN  0x102b11, BIT2

#define GPIO80_PAD PAD_GPIO18
#define GPIO80_OEN 0x102b12, BIT1
#define GPIO80_OUT 0x102b12, BIT0
#define GPIO80_IN  0x102b12, BIT2

#define GPIO81_PAD PAD_GPIO19
#define GPIO81_OEN 0x102b13, BIT1
#define GPIO81_OUT 0x102b13, BIT0
#define GPIO81_IN  0x102b13, BIT2

#define GPIO82_PAD PAD_GPIO20
#define GPIO82_OEN 0x102b14, BIT1
#define GPIO82_OUT 0x102b14, BIT0
#define GPIO82_IN  0x102b14, BIT2

#define GPIO83_PAD PAD_GPIO21
#define GPIO83_OEN 0x102b15, BIT1
#define GPIO83_OUT 0x102b15, BIT0
#define GPIO83_IN  0x102b15, BIT2

#define GPIO84_PAD PAD_GPIO22
#define GPIO84_OEN 0x102b16, BIT1
#define GPIO84_OUT 0x102b16, BIT0
#define GPIO84_IN  0x102b16, BIT2

#define GPIO85_PAD PAD_GPIO23
#define GPIO85_OEN 0x102b17, BIT1
#define GPIO85_OUT 0x102b17, BIT0
#define GPIO85_IN  0x102b17, BIT2

#define GPIO86_PAD PAD_GPIO24
#define GPIO86_OEN 0x102b18, BIT1
#define GPIO86_OUT 0x102b18, BIT0
#define GPIO86_IN  0x102b18, BIT2

#define GPIO87_PAD PAD_GPIO25
#define GPIO87_OEN 0x102b19, BIT1
#define GPIO87_OUT 0x102b19, BIT0
#define GPIO87_IN  0x102b19, BIT2

#define GPIO88_PAD PAD_GPIO26
#define GPIO88_OEN 0x102b1a, BIT1
#define GPIO88_OUT 0x102b1a, BIT0
#define GPIO88_IN  0x102b1a, BIT2

#define GPIO89_PAD PAD_GPIO27
#define GPIO89_OEN 0x102b1b, BIT1
#define GPIO89_OUT 0x102b1b, BIT0
#define GPIO89_IN  0x102b1b, BIT2

#define GPIO90_PAD PAD_GPIO28
#define GPIO90_OEN 0x102ba0, BIT1
#define GPIO90_OUT 0x102ba0, BIT0
#define GPIO90_IN  0x102ba0, BIT2

#define GPIO91_PAD PAD_GPIO29
#define GPIO91_OEN 0x102ba1, BIT1
#define GPIO91_OUT 0x102ba1, BIT0
#define GPIO91_IN  0x102ba1, BIT2

#define GPIO92_PAD PAD_GPIO30
#define GPIO92_OEN 0x102ba2, BIT1
#define GPIO92_OUT 0x102ba2, BIT0
#define GPIO92_IN  0x102ba2, BIT2

#define GPIO93_PAD PAD_GPIO31
#define GPIO93_OEN 0x102ba3, BIT1
#define GPIO93_OUT 0x102ba3, BIT0
#define GPIO93_IN  0x102ba3, BIT2

#define GPIO94_PAD PAD_GPIO32
#define GPIO94_OEN 0x102ba4, BIT1
#define GPIO94_OUT 0x102ba4, BIT0
#define GPIO94_IN  0x102ba4, BIT2

#define GPIO95_PAD PAD_GPIO33
#define GPIO95_OEN 0x102ba5, BIT1
#define GPIO95_OUT 0x102ba5, BIT0
#define GPIO95_IN  0x102ba5, BIT2

#define GPIO96_PAD PAD_GPIO34
#define GPIO96_OEN 0x102ba6, BIT1
#define GPIO96_OUT 0x102ba6, BIT0
#define GPIO96_IN  0x102ba6, BIT2

#define GPIO97_PAD PAD_GPIO35
#define GPIO97_OEN 0x102ba7, BIT1
#define GPIO97_OUT 0x102ba7, BIT0
#define GPIO97_IN  0x102ba7, BIT2

#define GPIO98_PAD PAD_GPIO36
#define GPIO98_OEN 0x102ba8, BIT1
#define GPIO98_OUT 0x102ba8, BIT0
#define GPIO98_IN  0x102ba8, BIT2

#define GPIO99_PAD PAD_GPIO37
#define GPIO99_OEN 0x102ba9, BIT1
#define GPIO99_OUT 0x102ba9, BIT0
#define GPIO99_IN  0x102ba9, BIT2

#define GPIO100_PAD PAD_I2S_IN_BCK
#define GPIO100_OEN 0x102b37, BIT1
#define GPIO100_OUT 0x102b37, BIT0
#define GPIO100_IN  0x102b37, BIT2

#define GPIO101_PAD PAD_I2S_IN_SD
#define GPIO101_OEN 0x102b38, BIT1
#define GPIO101_OUT 0x102b38, BIT0
#define GPIO101_IN  0x102b38, BIT2

#define GPIO102_PAD PAD_I2S_IN_WS
#define GPIO102_OEN 0x102b36, BIT1
#define GPIO102_OUT 0x102b36, BIT0
#define GPIO102_IN  0x102b36, BIT2

#define GPIO103_PAD PAD_I2S_OUT_BCK
#define GPIO103_OEN 0x102b3d, BIT1
#define GPIO103_OUT 0x102b3d, BIT0
#define GPIO103_IN  0x102b3d, BIT2

#define GPIO104_PAD PAD_I2S_OUT_MCK
#define GPIO104_OEN 0x102b3c, BIT1
#define GPIO104_OUT 0x102b3c, BIT0
#define GPIO104_IN  0x102b3c, BIT2

#define GPIO105_PAD PAD_I2S_OUT_SD
#define GPIO105_OEN 0x102b3e, BIT1
#define GPIO105_OUT 0x102b3e, BIT0
#define GPIO105_IN  0x102b3e, BIT2

#define GPIO106_PAD PAD_I2S_OUT_SD1
#define GPIO106_OEN 0x102b3f, BIT1
#define GPIO106_OUT 0x102b3f, BIT0
#define GPIO106_IN  0x102b3f, BIT2

#define GPIO107_PAD PAD_I2S_OUT_SD2
#define GPIO107_OEN 0x102b40, BIT1
#define GPIO107_OUT 0x102b40, BIT0
#define GPIO107_IN  0x102b40, BIT2

#define GPIO108_PAD PAD_I2S_OUT_SD3
#define GPIO108_OEN 0x102b41, BIT1
#define GPIO108_OUT 0x102b41, BIT0
#define GPIO108_IN  0x102b41, BIT2

#define GPIO109_PAD PAD_I2S_OUT_WS
#define GPIO109_OEN 0x102b3b, BIT1
#define GPIO109_OUT 0x102b3b, BIT0
#define GPIO109_IN  0x102b3b, BIT2

#define GPIO110_PAD PAD_PCM2_CD_N
#define GPIO110_OEN 0x102b67, BIT1
#define GPIO110_OUT 0x102b67, BIT0
#define GPIO110_IN  0x102b67, BIT2

#define GPIO111_PAD PAD_PCM2_CE_N
#define GPIO111_OEN 0x102b63, BIT1
#define GPIO111_OUT 0x102b63, BIT0
#define GPIO111_IN  0x102b63, BIT2

#define GPIO112_PAD PAD_PCM2_IRQA_N
#define GPIO112_OEN 0x102b64, BIT1
#define GPIO112_OUT 0x102b64, BIT0
#define GPIO112_IN  0x102b64, BIT2

#define GPIO113_PAD PAD_PCM2_RESET
#define GPIO113_OEN 0x102b66, BIT1
#define GPIO113_OUT 0x102b66, BIT0
#define GPIO113_IN  0x102b66, BIT2

#define GPIO114_PAD PAD_PCM2_WAIT_N
#define GPIO114_OEN 0x102b65, BIT1
#define GPIO114_OUT 0x102b65, BIT0
#define GPIO114_IN  0x102b65, BIT2

#define GPIO115_PAD PAD_PCM_A0
#define GPIO115_OEN 0x102b5d, BIT1
#define GPIO115_OUT 0x102b5d, BIT0
#define GPIO115_IN  0x102b5d, BIT2

#define GPIO116_PAD PAD_PCM_A1
#define GPIO116_OEN 0x102b5c, BIT1
#define GPIO116_OUT 0x102b5c, BIT0
#define GPIO116_IN  0x102b5c, BIT2

#define GPIO117_PAD PAD_PCM_A2
#define GPIO117_OEN 0x102b5a, BIT1
#define GPIO117_OUT 0x102b5a, BIT0
#define GPIO117_IN  0x102b5a, BIT2

#define GPIO118_PAD PAD_PCM_A3
#define GPIO118_OEN 0x102b59, BIT1
#define GPIO118_OUT 0x102b59, BIT0
#define GPIO118_IN  0x102b59, BIT2

#define GPIO119_PAD PAD_PCM_A4
#define GPIO119_OEN 0x102b58, BIT1
#define GPIO119_OUT 0x102b58, BIT0
#define GPIO119_IN  0x102b58, BIT2

#define GPIO120_PAD PAD_PCM_A5
#define GPIO120_OEN 0x102b56, BIT1
#define GPIO120_OUT 0x102b56, BIT0
#define GPIO120_IN  0x102b56, BIT2

#define GPIO121_PAD PAD_PCM_A6
#define GPIO121_OEN 0x102b55, BIT1
#define GPIO121_OUT 0x102b55, BIT0
#define GPIO121_IN  0x102b55, BIT2

#define GPIO122_PAD PAD_PCM_A7
#define GPIO122_OEN 0x102b54, BIT1
#define GPIO122_OUT 0x102b54, BIT0
#define GPIO122_IN  0x102b54, BIT2

#define GPIO123_PAD PAD_PCM_A8
#define GPIO123_OEN 0x102b4e, BIT1
#define GPIO123_OUT 0x102b4e, BIT0
#define GPIO123_IN  0x102b4e, BIT2

#define GPIO124_PAD PAD_PCM_A9
#define GPIO124_OEN 0x102b4c, BIT1
#define GPIO124_OUT 0x102b4c, BIT0
#define GPIO124_IN  0x102b4c, BIT2

#define GPIO125_PAD PAD_PCM_A10
#define GPIO125_OEN 0x102b48, BIT1
#define GPIO125_OUT 0x102b48, BIT0
#define GPIO125_IN  0x102b48, BIT2

#define GPIO126_PAD PAD_PCM_A11
#define GPIO126_OEN 0x102b4a, BIT1
#define GPIO126_OUT 0x102b4a, BIT0
#define GPIO126_IN  0x102b4a, BIT2

#define GPIO127_PAD PAD_PCM_A12
#define GPIO127_OEN 0x102b53, BIT1
#define GPIO127_OUT 0x102b53, BIT0
#define GPIO127_IN  0x102b53, BIT2

#define GPIO128_PAD PAD_PCM_A13
#define GPIO128_OEN 0x102b4f, BIT1
#define GPIO128_OUT 0x102b4f, BIT0
#define GPIO128_IN  0x102b4f, BIT2

#define GPIO129_PAD PAD_PCM_A14
#define GPIO129_OEN 0x102b50, BIT1
#define GPIO129_OUT 0x102b50, BIT0
#define GPIO129_IN  0x102b50, BIT2

#define GPIO130_PAD PAD_PCM_CD_N
#define GPIO130_OEN 0x102b62, BIT1
#define GPIO130_OUT 0x102b62, BIT0
#define GPIO130_IN  0x102b62, BIT2

#define GPIO131_PAD PAD_PCM_CE_N
#define GPIO131_OEN 0x102b47, BIT1
#define GPIO131_OUT 0x102b47, BIT0
#define GPIO131_IN  0x102b47, BIT2

#define GPIO132_PAD PAD_PCM_D0
#define GPIO132_OEN 0x102b5e, BIT1
#define GPIO132_OUT 0x102b5e, BIT0
#define GPIO132_IN  0x102b5e, BIT2

#define GPIO133_PAD PAD_PCM_D1
#define GPIO133_OEN 0x102b5f, BIT1
#define GPIO133_OUT 0x102b5f, BIT0
#define GPIO133_IN  0x102b5f, BIT2

#define GPIO134_PAD PAD_PCM_D2
#define GPIO134_OEN 0x102b60, BIT1
#define GPIO134_OUT 0x102b60, BIT0
#define GPIO134_IN  0x102b60, BIT2

#define GPIO135_PAD PAD_PCM_D3
#define GPIO135_OEN 0x102b42, BIT1
#define GPIO135_OUT 0x102b42, BIT0
#define GPIO135_IN  0x102b42, BIT2

#define GPIO136_PAD PAD_PCM_D4
#define GPIO136_OEN 0x102b43, BIT1
#define GPIO136_OUT 0x102b43, BIT0
#define GPIO136_IN  0x102b43, BIT2

#define GPIO137_PAD PAD_PCM_D5
#define GPIO137_OEN 0x102b44, BIT1
#define GPIO137_OUT 0x102b44, BIT0
#define GPIO137_IN  0x102b44, BIT2

#define GPIO138_PAD PAD_PCM_D6
#define GPIO138_OEN 0x102b45, BIT1
#define GPIO138_OUT 0x102b45, BIT0
#define GPIO138_IN  0x102b45, BIT2

#define GPIO139_PAD PAD_PCM_D7
#define GPIO139_OEN 0x102b46, BIT1
#define GPIO139_OUT 0x102b46, BIT0
#define GPIO139_IN  0x102b46, BIT2

#define GPIO140_PAD PAD_PCM_IORD_N
#define GPIO140_OEN 0x102b4b, BIT1
#define GPIO140_OUT 0x102b4b, BIT0
#define GPIO140_IN  0x102b4b, BIT2

#define GPIO141_PAD PAD_PCM_IOWR_N
#define GPIO141_OEN 0x102b4d, BIT1
#define GPIO141_OUT 0x102b4d, BIT0
#define GPIO141_IN  0x102b4d, BIT2

#define GPIO142_PAD PAD_PCM_IRQA_N
#define GPIO142_OEN 0x102b52, BIT1
#define GPIO142_OUT 0x102b52, BIT0
#define GPIO142_IN  0x102b52, BIT2

#define GPIO143_PAD PAD_PCM_OE_N
#define GPIO143_OEN 0x102b49, BIT1
#define GPIO143_OUT 0x102b49, BIT0
#define GPIO143_IN  0x102b49, BIT2

#define GPIO144_PAD PAD_PCM_REG_N
#define GPIO144_OEN 0x102b5b, BIT1
#define GPIO144_OUT 0x102b5b, BIT0
#define GPIO144_IN  0x102b5b, BIT2

#define GPIO145_PAD PAD_PCM_RESET
#define GPIO145_OEN 0x102b61, BIT1
#define GPIO145_OUT 0x102b61, BIT0
#define GPIO145_IN  0x102b61, BIT2

#define GPIO146_PAD PAD_PCM_WAIT_N
#define GPIO146_OEN 0x102b57, BIT1
#define GPIO146_OUT 0x102b57, BIT0
#define GPIO146_IN  0x102b57, BIT2

#define GPIO147_PAD PAD_PCM_WE_N
#define GPIO147_OEN 0x102b51, BIT1
#define GPIO147_OUT 0x102b51, BIT0
#define GPIO147_IN  0x102b51, BIT2

#define GPIO148_PAD PAD_PWM0
#define GPIO148_OEN 0x102b88, BIT1
#define GPIO148_OUT 0x102b88, BIT0
#define GPIO148_IN  0x102b88, BIT2

#define GPIO149_PAD PAD_PWM1
#define GPIO149_OEN 0x102b89, BIT1
#define GPIO149_OUT 0x102b89, BIT0
#define GPIO149_IN  0x102b89, BIT2

#define GPIO150_PAD PAD_PWM2
#define GPIO150_OEN 0x102b8a, BIT1
#define GPIO150_OUT 0x102b8a, BIT0
#define GPIO150_IN  0x102b8a, BIT2

#define GPIO151_PAD PAD_PWM3
#define GPIO151_OEN 0x102b8b, BIT1
#define GPIO151_OUT 0x102b8b, BIT0
#define GPIO151_IN  0x102b8b, BIT2

#define GPIO152_PAD PAD_PWM4
#define GPIO152_OEN 0x102b8c, BIT1
#define GPIO152_OUT 0x102b8c, BIT0
#define GPIO152_IN  0x102b8c, BIT2

#define GPIO153_PAD PAD_SPDIF_IN
#define GPIO153_OEN 0x102b39, BIT1
#define GPIO153_OUT 0x102b39, BIT0
#define GPIO153_IN  0x102b39, BIT2

#define GPIO154_PAD PAD_SPDIF_OUT
#define GPIO154_OEN 0x102b3a, BIT1
#define GPIO154_OUT 0x102b3a, BIT0
#define GPIO154_IN  0x102b3a, BIT2

#define GPIO155_PAD PAD_SPI1_CK
#define GPIO155_OEN 0x102bb1, BIT1
#define GPIO155_OUT 0x102bb1, BIT0
#define GPIO155_IN  0x102bb1, BIT2

#define GPIO156_PAD PAD_SPI1_DI
#define GPIO156_OEN 0x102bb2, BIT1
#define GPIO156_OUT 0x102bb2, BIT0
#define GPIO156_IN  0x102bb2, BIT2

#define GPIO157_PAD PAD_SPI2_CK
#define GPIO157_OEN 0x102bb3, BIT1
#define GPIO157_OUT 0x102bb3, BIT0
#define GPIO157_IN  0x102bb3, BIT2

#define GPIO158_PAD PAD_SPI2_DI
#define GPIO158_OEN 0x102bb4, BIT1
#define GPIO158_OUT 0x102bb4, BIT0
#define GPIO158_IN  0x102bb4, BIT2

#define GPIO159_PAD PAD_TGPIO0
#define GPIO159_OEN 0x102b8d, BIT1
#define GPIO159_OUT 0x102b8d, BIT0
#define GPIO159_IN  0x102b8d, BIT2

#define GPIO160_PAD PAD_TGPIO1
#define GPIO160_OEN 0x102b8e, BIT1
#define GPIO160_OUT 0x102b8e, BIT0
#define GPIO160_IN  0x102b8e, BIT2

#define GPIO161_PAD PAD_TGPIO2
#define GPIO161_OEN 0x102b8f, BIT1
#define GPIO161_OUT 0x102b8f, BIT0
#define GPIO161_IN  0x102b8f, BIT2

#define GPIO162_PAD PAD_TGPIO3
#define GPIO162_OEN 0x102b90, BIT1
#define GPIO162_OUT 0x102b90, BIT0
#define GPIO162_IN  0x102b90, BIT2

#define GPIO163_PAD PAD_TS0_CLK
#define GPIO163_OEN 0x102b26, BIT1
#define GPIO163_OUT 0x102b26, BIT0
#define GPIO163_IN  0x102b26, BIT2

#define GPIO164_PAD PAD_TS0_D0
#define GPIO164_OEN 0x102b1c, BIT1
#define GPIO164_OUT 0x102b1c, BIT0
#define GPIO164_IN  0x102b1c, BIT2

#define GPIO165_PAD PAD_TS0_D1
#define GPIO165_OEN 0x102b1d, BIT1
#define GPIO165_OUT 0x102b1d, BIT0
#define GPIO165_IN  0x102b1d, BIT2

#define GPIO166_PAD PAD_TS0_D2
#define GPIO166_OEN 0x102b1e, BIT1
#define GPIO166_OUT 0x102b1e, BIT0
#define GPIO166_IN  0x102b1e, BIT2

#define GPIO167_PAD PAD_TS0_D3
#define GPIO167_OEN 0x102b1f, BIT1
#define GPIO167_OUT 0x102b1f, BIT0
#define GPIO167_IN  0x102b1f, BIT2

#define GPIO168_PAD PAD_TS0_D4
#define GPIO168_OEN 0x102b20, BIT1
#define GPIO168_OUT 0x102b20, BIT0
#define GPIO168_IN  0x102b20, BIT2

#define GPIO169_PAD PAD_TS0_D5
#define GPIO169_OEN 0x102b21, BIT1
#define GPIO169_OUT 0x102b21, BIT0
#define GPIO169_IN  0x102b21, BIT2

#define GPIO170_PAD PAD_TS0_D6
#define GPIO170_OEN 0x102b22, BIT1
#define GPIO170_OUT 0x102b22, BIT0
#define GPIO170_IN  0x102b22, BIT2

#define GPIO171_PAD PAD_TS0_D7
#define GPIO171_OEN 0x102b23, BIT1
#define GPIO171_OUT 0x102b23, BIT0
#define GPIO171_IN  0x102b23, BIT2

#define GPIO172_PAD PAD_TS0_SYNC
#define GPIO172_OEN 0x102b25, BIT1
#define GPIO172_OUT 0x102b25, BIT0
#define GPIO172_IN  0x102b25, BIT2

#define GPIO173_PAD PAD_TS0_VLD
#define GPIO173_OEN 0x102b24, BIT1
#define GPIO173_OUT 0x102b24, BIT0
#define GPIO173_IN  0x102b24, BIT2

#define GPIO174_PAD PAD_TS1_CLK
#define GPIO174_OEN 0x102b27, BIT1
#define GPIO174_OUT 0x102b27, BIT0
#define GPIO174_IN  0x102b27, BIT2

#define GPIO175_PAD PAD_TS1_D0
#define GPIO175_OEN 0x102b31, BIT1
#define GPIO175_OUT 0x102b31, BIT0
#define GPIO175_IN  0x102b31, BIT2

#define GPIO176_PAD PAD_TS1_D1
#define GPIO176_OEN 0x102b30, BIT1
#define GPIO176_OUT 0x102b30, BIT0
#define GPIO176_IN  0x102b30, BIT2

#define GPIO177_PAD PAD_TS1_D2
#define GPIO177_OEN 0x102b2f, BIT1
#define GPIO177_OUT 0x102b2f, BIT0
#define GPIO177_IN  0x102b2f, BIT2

#define GPIO178_PAD PAD_TS1_D3
#define GPIO178_OEN 0x102b2e, BIT1
#define GPIO178_OUT 0x102b2e, BIT0
#define GPIO178_IN  0x102b2e, BIT2

#define GPIO179_PAD PAD_TS1_D4
#define GPIO179_OEN 0x102b2d, BIT1
#define GPIO179_OUT 0x102b2d, BIT0
#define GPIO179_IN  0x102b2d, BIT2

#define GPIO180_PAD PAD_TS1_D5
#define GPIO180_OEN 0x102b2c, BIT1
#define GPIO180_OUT 0x102b2c, BIT0
#define GPIO180_IN  0x102b2c, BIT2

#define GPIO181_PAD PAD_TS1_D6
#define GPIO181_OEN 0x102b2b, BIT1
#define GPIO181_OUT 0x102b2b, BIT0
#define GPIO181_IN  0x102b2b, BIT2

#define GPIO182_PAD PAD_TS1_D7
#define GPIO182_OEN 0x102b2a, BIT1
#define GPIO182_OUT 0x102b2a, BIT0
#define GPIO182_IN  0x102b2a, BIT2

#define GPIO183_PAD PAD_TS1_SYNC
#define GPIO183_OEN 0x102b28, BIT1
#define GPIO183_OUT 0x102b28, BIT0
#define GPIO183_IN  0x102b28, BIT2

#define GPIO184_PAD PAD_TS1_VLD
#define GPIO184_OEN 0x102b29, BIT1
#define GPIO184_OUT 0x102b29, BIT0
#define GPIO184_IN  0x102b29, BIT2

#define GPIO185_PAD PAD_TS2_CLK
#define GPIO185_OEN 0x102b35, BIT1
#define GPIO185_OUT 0x102b35, BIT0
#define GPIO185_IN  0x102b35, BIT2

#define GPIO186_PAD PAD_TS2_D0
#define GPIO186_OEN 0x102b32, BIT1
#define GPIO186_OUT 0x102b32, BIT0
#define GPIO186_IN  0x102b32, BIT2

#define GPIO187_PAD PAD_TS2_D1
#define GPIO187_OEN 0x102bf2, BIT1
#define GPIO187_OUT 0x102bf2, BIT0
#define GPIO187_IN  0x102bf2, BIT2

#define GPIO188_PAD PAD_TS2_D2
#define GPIO188_OEN 0x102bf3, BIT1
#define GPIO188_OUT 0x102bf3, BIT0
#define GPIO188_IN  0x102bf3, BIT2

#define GPIO189_PAD PAD_TS2_D3
#define GPIO189_OEN 0x102bf4, BIT1
#define GPIO189_OUT 0x102bf4, BIT0
#define GPIO189_IN  0x102bf4, BIT2

#define GPIO190_PAD PAD_TS2_D4
#define GPIO190_OEN 0x102bf5, BIT1
#define GPIO190_OUT 0x102bf5, BIT0
#define GPIO190_IN  0x102bf5, BIT2

#define GPIO191_PAD PAD_TS2_D5
#define GPIO191_OEN 0x102bf6, BIT1
#define GPIO191_OUT 0x102bf6, BIT0
#define GPIO191_IN  0x102bf6, BIT2

#define GPIO192_PAD PAD_TS2_D6
#define GPIO192_OEN 0x102bf7, BIT1
#define GPIO192_OUT 0x102bf7, BIT0
#define GPIO192_IN  0x102bf7, BIT2

#define GPIO193_PAD PAD_TS2_D7
#define GPIO193_OEN 0x102bf8, BIT1
#define GPIO193_OUT 0x102bf8, BIT0
#define GPIO193_IN  0x102bf8, BIT2

#define GPIO194_PAD PAD_TS2_SYNC
#define GPIO194_OEN 0x102b34, BIT1
#define GPIO194_OUT 0x102b34, BIT0
#define GPIO194_IN  0x102b34, BIT2

#define GPIO195_PAD PAD_TS2_VLD
#define GPIO195_OEN 0x102b33, BIT1
#define GPIO195_OUT 0x102b33, BIT0
#define GPIO195_IN  0x102b33, BIT2

#define GPIO196_PAD PAD_TS3_CLK
#define GPIO196_OEN 0x11030a, BIT1
#define GPIO196_OUT 0x11030a, BIT0
#define GPIO196_IN  0x11030a, BIT2

#define GPIO197_PAD PAD_TS3_D0
#define GPIO197_OEN 0x110300, BIT1
#define GPIO197_OUT 0x110300, BIT0
#define GPIO197_IN  0x110300, BIT2

#define GPIO198_PAD PAD_TS3_D1
#define GPIO198_OEN 0x110301, BIT1
#define GPIO198_OUT 0x110301, BIT0
#define GPIO198_IN  0x110301, BIT2

#define GPIO199_PAD PAD_TS3_D2
#define GPIO199_OEN 0x110302, BIT1
#define GPIO199_OUT 0x110302, BIT0
#define GPIO199_IN  0x110302, BIT2

#define GPIO200_PAD PAD_TS3_D3
#define GPIO200_OEN 0x110303, BIT1
#define GPIO200_OUT 0x110303, BIT0
#define GPIO200_IN  0x110303, BIT2

#define GPIO201_PAD PAD_TS3_D4
#define GPIO201_OEN 0x110304, BIT1
#define GPIO201_OUT 0x110304, BIT0
#define GPIO201_IN  0x110304, BIT2

#define GPIO202_PAD PAD_TS3_D5
#define GPIO202_OEN 0x110305, BIT1
#define GPIO202_OUT 0x110305, BIT0
#define GPIO202_IN  0x110305, BIT2

#define GPIO203_PAD PAD_TS3_D6
#define GPIO203_OEN 0x110306, BIT1
#define GPIO203_OUT 0x110306, BIT0
#define GPIO203_IN  0x110306, BIT2

#define GPIO204_PAD PAD_TS3_D7
#define GPIO204_OEN 0x110307, BIT1
#define GPIO204_OUT 0x110307, BIT0
#define GPIO204_IN  0x110307, BIT2

#define GPIO205_PAD PAD_TS3_SYNC
#define GPIO205_OEN 0x110309, BIT1
#define GPIO205_OUT 0x110309, BIT0
#define GPIO205_IN  0x110309, BIT2

#define GPIO206_PAD PAD_TS3_VLD
#define GPIO206_OEN 0x110308, BIT1
#define GPIO206_OUT 0x110308, BIT0
#define GPIO206_IN  0x110308, BIT2

#define GPIO207_PAD PAD_VSYNC_Like
#define GPIO207_OEN 0x102bb0, BIT1
#define GPIO207_OUT 0x102bb0, BIT0
#define GPIO207_IN  0x102bb0, BIT2


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define GPIO_INT_MUX_REG 0x101ea5
#define INT_COUNT   8
typedef struct
{
    GPIO_Edge edge_type;
    GPIO_Callback pCallback;
    MS_U8 IsDirty;
} GPIO_INTCFG;


const MS_GPIO_NUM gpio_IntPad[INT_COUNT]=
{

    111,  /* PAD_PCM2_CE_N */
    207,  /* PAD_VSYNC_LIKE */
    112,  /*PAD_PCM2_IRQA_N */
    110,  /* PAD_PCM2_CD_N */
    89,   /* PAD_GPIO27 */
    83,   /* PAD_GPIO21 */
    102,  /* PAD_I2S_IN_WS */
    106   /* PAD_I2S_OUT_SD1 */
};

const InterruptNum gpio_IRQnum[INT_COUNT]= {
                                    #if !defined(CONFIG_FRC)
                                        E_INT_FIQ_GPIO0,
                                        E_INT_FIQ_GPIO1,
                                        E_INT_FIQ_GPIO2,
                                        E_INT_FIQ_GPIO3,
                                        E_INT_FIQ_GPIO4,
                                        E_INT_FIQ_GPIO5,
                                        E_INT_FIQ_GPIO6,
                                        E_INT_FIQ_GPIO7
                                    #endif
                                            };
const MS_U32 gpio_IRQpolarity[INT_COUNT][2]={
    {0x10194a, BIT7},
    {0x10194a, BIT11},
    {0x10194a, BIT15},
    {0x10194b, BIT7},
    {0x10194b, BIT8},
    {0x10194b, BIT9},
    {0x10194b, BIT10},
    {0x10194b, BIT15}
};

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

    __GPIO(999),__GPIO(1), __GPIO(2), __GPIO(3), __GPIO(4),
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
    __GPIO(185), __GPIO(186), __GPIO(187), __GPIO(188), __GPIO(189),
    __GPIO(190), __GPIO(191), __GPIO(192), __GPIO(193), __GPIO(194),
    __GPIO(195), __GPIO(196), __GPIO(197), __GPIO(198), __GPIO(199),
    __GPIO(200), __GPIO(201), __GPIO(202), __GPIO(203), __GPIO(204),
    __GPIO(205), __GPIO(206), __GPIO(207)
};

#define FUNCTION_UNUSED  __attribute__((unused))

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT _gMIO_MapBase = 0;
static MS_VIRT _gMIO_MapBase_PM = 0;
static MS_VIRT _gMIO_MapBase_nonPM = 0;
static GPIO_INTCFG _gGPIO_Int_Table[INT_COUNT];

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX)
/*
  * These function are not used for now, mark as FUNCTION_UNUSED
  * to avoid compiler warning.
  */
static void _HAL_Gpio_Set_High(MS_GPIO_NUM gpio)  FUNCTION_UNUSED;
static void _HAL_Gpio_Set_Low(MS_GPIO_NUM gpio)  FUNCTION_UNUSED;
static void _HAL_Gpio_Set_Input(MS_GPIO_NUM gpio)  FUNCTION_UNUSED;
static int _HAL_Gpio_Get_InOut(MS_GPIO_NUM gpio)  FUNCTION_UNUSED;
static int _HAL_Gpio_Get_Level(MS_GPIO_NUM gpio)  FUNCTION_UNUSED;
#endif
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX)
/*
  * Following functions prefixed with underline could only be built for userspace
  * program to control GPIO through sysfs. So if we are building utopia for linux userspace,
  * we compile these functions for runtime to be dynamically hooked, else (e.g. mboot nuttx)
  * exclude from source code.
  */
/* GPIO operations using sysfs */
static void __HAL_Gpio_Set_Level(MS_GPIO_NUM gpio, char* value)
{
	int fd, cnt;
	char SysfsPathFmt[] = "/sys/class/gpio%d/value";
	char GPIOSysfsPath[128];

	snprintf(GPIOSysfsPath, 128, SysfsPathFmt, gpio);
	fd = open(GPIOSysfsPath, O_WRONLY);
	MS_PANIC((fd<0)&&"GPIO: cannot open sysfs file");

	cnt = write (fd, value, 1);
	sync();
	/*FIXME: cnt == 0 is not handled */
	MS_ASSERT((cnt<0)&&"GPIO: Sysfs IO Error");

	/* TODO: cache the fd to reduce open/close overhead*/
	close(fd);
	/* FIXME: cannot return operation status */
}

static void __HAL_Gpio_Set_InOut(MS_GPIO_NUM gpio, char* value)
{
	int fd, cnt;
	char SysfsPathFmt[] = "/sys/class/gpio%d/direction";
	char GPIOSysfsPath[128];

	snprintf(GPIOSysfsPath, 128, SysfsPathFmt, gpio);
	fd = open(GPIOSysfsPath, O_WRONLY);
	MS_PANIC((fd<0)&&"GPIO: cannot open sysfs file");

	cnt = write (fd, value, 0);
	sync();
	MS_ASSERT((cnt < 0)&&"GPIO: Sysfs IO Error");
	/* TODO: cache the fd to reduce open/close overhead*/
	close(fd);
	/* FIXME: cannot return operation status */
}

static void _HAL_Gpio_Set_High(MS_GPIO_NUM gpio)
{
	//MS_ASSERT(isGPIOInit != 1);
	__HAL_Gpio_Set_InOut(gpio, "out");
	__HAL_Gpio_Set_Level(gpio, "1");
}

static void _HAL_Gpio_Set_Low(MS_GPIO_NUM gpio)
{
	__HAL_Gpio_Set_InOut(gpio, "out");
	__HAL_Gpio_Set_Level(gpio, "0");
}

static void _HAL_Gpio_Set_Input(MS_GPIO_NUM gpio)
{
	__HAL_Gpio_Set_InOut(gpio, "in");
}
#if 0
/* nobody use this function, comment out to avoid warning */
static void _HAL_Gpio_Set_Output(MS_GPIO_NUM gpio)
{
	__HAL_Gpio_Set_InOut(gpio, "out");
}
#endif
static int _HAL_Gpio_Get_InOut(MS_GPIO_NUM gpio)
{
	int fd, cnt;
	char SysfsPathFmt[] = "/sys/class/gpio%d/value";
	char GPIOSysfsPath[128];
	char value[5];
	snprintf(GPIOSysfsPath, 128, SysfsPathFmt, gpio);

	fd = open(GPIOSysfsPath, O_RDONLY);
	MS_PANIC((fd<0)&&"GPIO: cannot open sysfs file"); /* notice that there may be permission issue */
	sync();
	cnt = read (fd, value, 0);
	MS_ASSERT(cnt < 0 ); /* IO error */

	/* TODO: cache the fd to reduce open/close overhead,
		if the function is constantly called*/
	close(fd);
	if(!strncmp(value, "out", 3))
	{
		return 1;
	}
	else if (!strncmp(value, "in", 2))
	{
		return 0;
	}

	MS_ASSERT(0&&"GPIO: Invalid direction value");
	return -1;

}

static int _HAL_Gpio_Get_Level(MS_GPIO_NUM gpio)
{
	int fd, cnt;
	char SysfsPathFmt[] = "/sys/class/gpio%d/value";
	char GPIOSysfsPath[128];
	char value[5];
	snprintf(GPIOSysfsPath, 128, SysfsPathFmt, gpio);

	fd = open(GPIOSysfsPath, O_RDONLY);
	MS_PANIC((fd<0)&&"GPIO: cannot open sysfs file");

	cnt = read(fd, value, 1);
	MS_ASSERT(cnt < 0 );
	/* TODO: cache the fd to reduce open/close overhead*/
	close(fd);
	/* FIXME: cannot return operation status */
	return ((value[0] == '0')? 0 : 1);
}
#endif

/* GPIO operations using MMIO */
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

/* Controlling GPIO using mmio by default */
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
    MS_U8 i;

    for(i=0; i<INT_COUNT; i++)
    {
        _gGPIO_Int_Table[i].edge_type = E_GPIO_RISING_EDGE;
        _gGPIO_Int_Table[i].pCallback = NULL;
        _gGPIO_Int_Table[i].IsDirty = FALSE;
    }

    if(INT_COUNT > 8)
    {
        GPIO_HAL_ERR("if INT_COUNT > 8 need to check GPIO interrupt code flow !!");
        MS_ASSERT(0);
    }
}

void HAL_GPIO_INTHandler(InterruptNum eIntNum)
{
    MS_U8 i;

    for(i=0; i<INT_COUNT; i++)
    {
        if((eIntNum == gpio_IRQnum[i]) && (_gGPIO_Int_Table[i].pCallback != NULL))
        {
            MsOS_DisableInterrupt(eIntNum);
            _gGPIO_Int_Table[i].pCallback();
            MsOS_EnableInterrupt(eIntNum);
        }
    }
}

void HAL_GPIO_Interrupt_Action(void)
{
    MS_U8 i;

    for(i=0; i<INT_COUNT; i++)
    {
        if(_gGPIO_Int_Table[i].IsDirty == TRUE)
        {
            HAL_GPIO_Disable_Interrupt(gpio_IntPad[i]);

            if(_gGPIO_Int_Table[i].edge_type == E_GPIO_RISING_EDGE)
                MDrv_Write2Byte(gpio_IRQpolarity[i][0], MDrv_Read2Byte(gpio_IRQpolarity[i][0]) & ~(gpio_IRQpolarity[i][1]));
            else
                MDrv_Write2Byte(gpio_IRQpolarity[i][0], MDrv_Read2Byte(gpio_IRQpolarity[i][0]) | (gpio_IRQpolarity[i][1]));


            if(_gGPIO_Int_Table[i].pCallback != NULL)
            {
                MsOS_AttachInterrupt(gpio_IRQnum[i], (InterruptCb)HAL_GPIO_INTHandler);
                MsOS_EnableInterrupt(gpio_IRQnum[i]);
                HAL_GPIO_Enable_Interrupt(gpio_IntPad[i]);
            }
            else
            {
                MsOS_DisableInterrupt(gpio_IRQnum[i]);
                HAL_GPIO_Disable_Interrupt(gpio_IntPad[i]);
            }

            _gGPIO_Int_Table[i].IsDirty = FALSE;
        }
    }
}


GPIO_Result HAL_GPIO_Attach_Interrupt(MS_GPIO_NUM gpio_num,GPIO_Edge gpio_edge_type,GPIO_Callback pCallback)
{
    MS_U8 i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    for(i=0; i<INT_COUNT; i++)
    {
        if(gpio_IntPad[i] == gpio_num)
        {
            ret_val = E_GPIO_OK;
            break;
        }
    }

    if(ret_val != E_GPIO_OK)
    {
        return E_GPIO_NOT_SUPPORT;
    }

    _gGPIO_Int_Table[i].edge_type = gpio_edge_type;
    _gGPIO_Int_Table[i].pCallback = pCallback;
    _gGPIO_Int_Table[i].IsDirty = TRUE;

    return ret_val;
}

GPIO_Result HAL_GPIO_Datach_Interrupt(MS_GPIO_NUM gpio_num)
{
    MS_U8 i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    for(i=0; i<INT_COUNT; i++)
    {
        if(gpio_IntPad[i] == gpio_num)
        {
            ret_val = E_GPIO_OK;
            break;
        }
    }

    if(ret_val != E_GPIO_OK)
    {
        return E_GPIO_NOT_SUPPORT;
    }

    _gGPIO_Int_Table[i].pCallback = NULL;
    _gGPIO_Int_Table[i].IsDirty = TRUE;

    return ret_val;
}

GPIO_Result HAL_GPIO_Enable_Interrupt(MS_GPIO_NUM gpio)
{
    MS_U8 i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    _gMIO_MapBase = _gMIO_MapBase_PM;
    for(i=0; i<INT_COUNT; i++)
    {
        if((gpio_IntPad[i] == gpio) && (_gGPIO_Int_Table[i].pCallback != NULL))
        {
            MDrv_WriteByte(GPIO_INT_MUX_REG, (MDrv_ReadByte(GPIO_INT_MUX_REG) | (1<<i)) );
            ret_val = E_GPIO_OK;
        }
    }

    return ret_val;
}

GPIO_Result HAL_GPIO_Disable_Interrupt(MS_GPIO_NUM gpio)
{
    MS_U8 i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    _gMIO_MapBase = _gMIO_MapBase_PM;
    for(i=0; i<INT_COUNT; i++)
    {
        if((gpio_IntPad[i] == gpio) && (_gGPIO_Int_Table[i].pCallback != NULL))
        {
            MDrv_WriteByte(GPIO_INT_MUX_REG, (MDrv_ReadByte(GPIO_INT_MUX_REG) & ~(1<<i)) );
            ret_val = E_GPIO_OK;
        }
    }

    return ret_val;
}

void HAL_GPIO_Disable_Interrupt_all(void)
{
    _gMIO_MapBase = _gMIO_MapBase_PM;
    MDrv_WriteByte(GPIO_INT_MUX_REG , 0x00);
}

void HAL_GPIO_Enable_Interrupt_all(void)
{
    MS_U8 i;

    _gMIO_MapBase = _gMIO_MapBase_PM;
    for(i=0; i<INT_COUNT; i++)
    {
        if(_gGPIO_Int_Table[i].pCallback != NULL)
        {
            MDrv_WriteByte(GPIO_INT_MUX_REG, (MDrv_ReadByte(GPIO_INT_MUX_REG) | (1<<i)) );
        }
    }
}

#endif      //_HALGPIO_C_

