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

#define _CONCAT( a, b )                 a##b
#define CONCAT( a, b )                  _CONCAT( a, b )

#define BIT0                            BIT(0)
#define BIT1                            BIT(1)
#define BIT2                            BIT(2)
#define BIT3                            BIT(3)
#define BIT4                            BIT(4)
#define BIT5                            BIT(5)
#define BIT6                            BIT(6)
#define BIT7                            BIT(7)
#define BIT8                            BIT(8)
#define BIT9                            BIT(9)
#define BIT10                           BIT(10)
#define BIT11                           BIT(11)
#define BIT12                           BIT(12)
#define BIT13                           BIT(13)
#define BIT14                           BIT(14)
#define BIT15                           BIT(15)

// Dummy
#define GPIO999_OEN                     0, 0
#define GPIO999_OUT                     0, 0
#define GPIO999_IN                      0, 0

#define GPIO0_PAD PAD_IRIN
#define GPIO0_OEN 0x0f26, BIT0
#define GPIO0_OUT 0x0f26, BIT1
#define GPIO0_IN  0x0f26, BIT2

#define GPIO1_PAD PAD_CEC0
#define GPIO1_OEN 0x0f2a, BIT0
#define GPIO1_OUT 0x0f2a, BIT1
#define GPIO1_IN  0x0f2a, BIT2

#define GPIO2_PAD PAD_PWM_PM
#define GPIO2_OEN 0x0f28, BIT0
#define GPIO2_OUT 0x0f28, BIT1
#define GPIO2_IN  0x0f28, BIT2

#define GPIO3_PAD PAD_DDCA_CK
#define GPIO3_OEN 0x0494, BIT1
#define GPIO3_OUT 0x0494, BIT2
#define GPIO3_IN  0x0494, BIT0

#define GPIO4_PAD PAD_DDCA_DA
#define GPIO4_OEN 0x0494, BIT5
#define GPIO4_OUT 0x0494, BIT6
#define GPIO4_IN  0x0494, BIT4

#define GPIO5_PAD PAD_GPIO0_PM
#define GPIO5_OEN 0x0f00, BIT0
#define GPIO5_OUT 0x0f00, BIT1
#define GPIO5_IN  0x0f00, BIT2

#define GPIO6_PAD PAD_GPIO1_PM
#define GPIO6_OEN 0x0f02, BIT0
#define GPIO6_OUT 0x0f02, BIT1
#define GPIO6_IN  0x0f02, BIT2

#define GPIO7_PAD PAD_GPIO2_PM
#define GPIO7_OEN 0x0f04, BIT0
#define GPIO7_OUT 0x0f04, BIT1
#define GPIO7_IN  0x0f04, BIT2

#define GPIO8_PAD PAD_GPIO3_PM
#define GPIO8_OEN 0x0f06, BIT0
#define GPIO8_OUT 0x0f06, BIT1
#define GPIO8_IN  0x0f06, BIT2

#define GPIO9_PAD PAD_GPIO4_PM
#define GPIO9_OEN 0x0f08, BIT0
#define GPIO9_OUT 0x0f08, BIT1
#define GPIO9_IN  0x0f08, BIT2

#define GPIO10_PAD PAD_GPIO5_PM
#define GPIO10_OEN 0x0f0a, BIT0
#define GPIO10_OUT 0x0f0a, BIT1
#define GPIO10_IN  0x0f0a, BIT2

#define GPIO11_PAD PAD_GPIO6_PM
#define GPIO11_OEN 0x0f0c, BIT0
#define GPIO11_OUT 0x0f0c, BIT1
#define GPIO11_IN  0x0f0c, BIT2

#define GPIO12_PAD PAD_GPIO7_PM
#define GPIO12_OEN 0x0f0e, BIT0
#define GPIO12_OUT 0x0f0e, BIT1
#define GPIO12_IN  0x0f0e, BIT2

#define GPIO13_PAD PAD_GPIO8_PM
#define GPIO13_OEN 0x0f10, BIT0
#define GPIO13_OUT 0x0f10, BIT1
#define GPIO13_IN  0x0f10, BIT2

#define GPIO14_PAD PAD_GPIO9_PM
#define GPIO14_OEN 0x0f12, BIT0
#define GPIO14_OUT 0x0f12, BIT1
#define GPIO14_IN  0x0f12, BIT2

#define GPIO15_PAD PAD_GPIO10_PM
#define GPIO15_OEN 0x0f14, BIT0
#define GPIO15_OUT 0x0f14, BIT1
#define GPIO15_IN  0x0f14, BIT2

#define GPIO16_PAD PAD_GPIO11_PM
#define GPIO16_OEN 0x0f16, BIT0
#define GPIO16_OUT 0x0f16, BIT1
#define GPIO16_IN  0x0f16, BIT2

#define GPIO17_PAD PAD_GPIO12_PM
#define GPIO17_OEN 0x0f18, BIT0
#define GPIO17_OUT 0x0f18, BIT1
#define GPIO17_IN  0x0f18, BIT2

#define GPIO18_PAD PAD_HOTPLUGA
#define GPIO18_OEN 0x0e4e, BIT0
#define GPIO18_OUT 0x0e4e, BIT4
#define GPIO18_IN  0x0e4f, BIT0

#define GPIO19_PAD PAD_HOTPLUGB
#define GPIO19_OEN 0x0e4e, BIT1
#define GPIO19_OUT 0x0e4e, BIT5
#define GPIO19_IN  0x0e4f, BIT1

#define GPIO20_PAD PAD_HOTPLUGC
#define GPIO20_OEN 0x0e4e, BIT2
#define GPIO20_OUT 0x0e4e, BIT6
#define GPIO20_IN  0x0e4f, BIT2

#define GPIO21_PAD PAD_HOTPLUGA_HDMI20_5V
#define GPIO21_OEN 0x010218, BIT5
#define GPIO21_OUT 0x010218, BIT4
#define GPIO21_IN  0x010218, BIT6

#define GPIO22_PAD PAD_HOTPLUGB_HDMI20_5V
#define GPIO22_OEN 0x010318, BIT5
#define GPIO22_OUT 0x010318, BIT4
#define GPIO22_IN  0x010318, BIT6

#define GPIO23_PAD PAD_HOTPLUGC_HDMI20_5V
#define GPIO23_OEN 0x010418, BIT5
#define GPIO23_OUT 0x010418, BIT4
#define GPIO23_IN  0x010418, BIT6

#define GPIO24_PAD PAD_DDCDA_CK
#define GPIO24_OEN 0x0496, BIT1
#define GPIO24_OUT 0x0496, BIT2
#define GPIO24_IN  0x0496, BIT0

#define GPIO25_PAD PAD_DDCDA_DA
#define GPIO25_OEN 0x0496, BIT5
#define GPIO25_OUT 0x0496, BIT6
#define GPIO25_IN  0x0496, BIT4

#define GPIO26_PAD PAD_DDCDB_CK
#define GPIO26_OEN 0x0497, BIT1
#define GPIO26_OUT 0x0497, BIT2
#define GPIO26_IN  0x0497, BIT0

#define GPIO27_PAD PAD_DDCDB_DA
#define GPIO27_OEN 0x0497, BIT5
#define GPIO27_OUT 0x0497, BIT6
#define GPIO27_IN  0x0497, BIT4

#define GPIO28_PAD PAD_DDCDC_CK
#define GPIO28_OEN 0x0498, BIT1
#define GPIO28_OUT 0x0498, BIT2
#define GPIO28_IN  0x0498, BIT0

#define GPIO29_PAD PAD_DDCDC_DA
#define GPIO29_OEN 0x0498, BIT5
#define GPIO29_OUT 0x0498, BIT6
#define GPIO29_IN  0x0498, BIT4

#define GPIO30_PAD PAD_SAR0
#define GPIO30_OEN 0x1423, BIT0
#define GPIO30_OUT 0x1424, BIT0
#define GPIO30_IN  0x1425, BIT0

#define GPIO31_PAD PAD_SAR1
#define GPIO31_OEN 0x1423, BIT1
#define GPIO31_OUT 0x1424, BIT1
#define GPIO31_IN  0x1425, BIT1

#define GPIO32_PAD PAD_SAR2
#define GPIO32_OEN 0x1423, BIT2
#define GPIO32_OUT 0x1424, BIT2
#define GPIO32_IN  0x1425, BIT2

#define GPIO33_PAD PAD_SAR3
#define GPIO33_OEN 0x1423, BIT3
#define GPIO33_OUT 0x1424, BIT3
#define GPIO33_IN  0x1425, BIT3

#define GPIO34_PAD PAD_SAR4
#define GPIO34_OEN 0x1423, BIT4
#define GPIO34_OUT 0x1424, BIT4
#define GPIO34_IN  0x1425, BIT4

#define GPIO35_PAD PAD_VPLUGIN
#define GPIO35_OEN 0x1423, BIT5
#define GPIO35_OUT 0x1424, BIT5
#define GPIO35_IN  0x1425, BIT5

#define GPIO36_PAD PAD_VID0
#define GPIO36_OEN 0x2e84, BIT1
#define GPIO36_OUT 0x2e84, BIT0
#define GPIO36_IN  0x2e84, BIT2

#define GPIO37_PAD PAD_VID1
#define GPIO37_OEN 0x2e85, BIT1
#define GPIO37_OUT 0x2e85, BIT0
#define GPIO37_IN  0x2e85, BIT2

#define GPIO38_PAD PAD_VID2
#define GPIO38_OEN 0x0f22, BIT0
#define GPIO38_OUT 0x0f22, BIT1
#define GPIO38_IN  0x0f22, BIT2

#define GPIO39_PAD PAD_VID3
#define GPIO39_OEN 0x0f24, BIT0
#define GPIO39_OUT 0x0f24, BIT1
#define GPIO39_IN  0x0f24, BIT2

#define GPIO40_PAD PAD_WOL_INT_OUT
#define GPIO40_OEN 0x2e82, BIT1
#define GPIO40_OUT 0x2e82, BIT0
#define GPIO40_IN  0x2e82, BIT2

#define GPIO41_PAD PAD_ARC0
#define GPIO41_OEN 0x110320, BIT1
#define GPIO41_OUT 0x110320, BIT0
#define GPIO41_IN  0x110320, BIT2

#define GPIO42_PAD PAD_DDCR_CK
#define GPIO42_OEN 0x102b87, BIT1
#define GPIO42_OUT 0x102b87, BIT0
#define GPIO42_IN  0x102b87, BIT2

#define GPIO43_PAD PAD_DDCR_DA
#define GPIO43_OEN 0x102b86, BIT1
#define GPIO43_OUT 0x102b86, BIT0
#define GPIO43_IN  0x102b86, BIT2

#define GPIO44_PAD PAD_GPIO2
#define GPIO44_OEN 0x102b00, BIT1
#define GPIO44_OUT 0x102b00, BIT0
#define GPIO44_IN  0x102b00, BIT2

#define GPIO45_PAD PAD_GPIO3
#define GPIO45_OEN 0x102b01, BIT1
#define GPIO45_OUT 0x102b01, BIT0
#define GPIO45_IN  0x102b01, BIT2

#define GPIO46_PAD PAD_GPIO4
#define GPIO46_OEN 0x102b02, BIT1
#define GPIO46_OUT 0x102b02, BIT0
#define GPIO46_IN  0x102b02, BIT2

#define GPIO47_PAD PAD_GPIO5
#define GPIO47_OEN 0x102b03, BIT1
#define GPIO47_OUT 0x102b03, BIT0
#define GPIO47_IN  0x102b03, BIT2

#define GPIO48_PAD PAD_GPIO9
#define GPIO48_OEN 0x102b04, BIT1
#define GPIO48_OUT 0x102b04, BIT0
#define GPIO48_IN  0x102b04, BIT2

#define GPIO49_PAD PAD_GPIO10
#define GPIO49_OEN 0x102b05, BIT1
#define GPIO49_OUT 0x102b05, BIT0
#define GPIO49_IN  0x102b05, BIT2

#define GPIO50_PAD PAD_GPIO11
#define GPIO50_OEN 0x102b06, BIT1
#define GPIO50_OUT 0x102b06, BIT0
#define GPIO50_IN  0x102b06, BIT2

#define GPIO51_PAD PAD_GPIO12
#define GPIO51_OEN 0x102b07, BIT1
#define GPIO51_OUT 0x102b07, BIT0
#define GPIO51_IN  0x102b07, BIT2

#define GPIO52_PAD PAD_GPIO19
#define GPIO52_OEN 0x102b08, BIT1
#define GPIO52_OUT 0x102b08, BIT0
#define GPIO52_IN  0x102b08, BIT2

#define GPIO53_PAD PAD_GPIO20
#define GPIO53_OEN 0x102b09, BIT1
#define GPIO53_OUT 0x102b09, BIT0
#define GPIO53_IN  0x102b09, BIT2

#define GPIO54_PAD PAD_GPIO25
#define GPIO54_OEN 0x102b0a, BIT1
#define GPIO54_OUT 0x102b0a, BIT0
#define GPIO54_IN  0x102b0a, BIT2

#define GPIO55_PAD PAD_GPIO26
#define GPIO55_OEN 0x102b0b, BIT1
#define GPIO55_OUT 0x102b0b, BIT0
#define GPIO55_IN  0x102b0b, BIT2

#define GPIO56_PAD PAD_GPIO30
#define GPIO56_OEN 0x102b0c, BIT1
#define GPIO56_OUT 0x102b0c, BIT0
#define GPIO56_IN  0x102b0c, BIT2

#define GPIO57_PAD PAD_GPIO31
#define GPIO57_OEN 0x102b0d, BIT1
#define GPIO57_OUT 0x102b0d, BIT0
#define GPIO57_IN  0x102b0d, BIT2

#define GPIO58_PAD PAD_I2S_OUT_BCK
#define GPIO58_OEN 0x102b38, BIT1
#define GPIO58_OUT 0x102b38, BIT0
#define GPIO58_IN  0x102b38, BIT2

#define GPIO59_PAD PAD_I2S_OUT_MCK
#define GPIO59_OEN 0x102b39, BIT1
#define GPIO59_OUT 0x102b39, BIT0
#define GPIO59_IN  0x102b39, BIT2

#define GPIO60_PAD PAD_I2S_OUT_SD
#define GPIO60_OEN 0x102b3b, BIT1
#define GPIO60_OUT 0x102b3b, BIT0
#define GPIO60_IN  0x102b3b, BIT2

#define GPIO61_PAD PAD_I2S_OUT_SD1
#define GPIO61_OEN 0x102b3c, BIT1
#define GPIO61_OUT 0x102b3c, BIT0
#define GPIO61_IN  0x102b3c, BIT2

#define GPIO62_PAD PAD_I2S_OUT_SD2
#define GPIO62_OEN 0x102b3d, BIT1
#define GPIO62_OUT 0x102b3d, BIT0
#define GPIO62_IN  0x102b3d, BIT2

#define GPIO63_PAD PAD_I2S_OUT_WS
#define GPIO63_OEN 0x102b3a, BIT1
#define GPIO63_OUT 0x102b3a, BIT0
#define GPIO63_IN  0x102b3a, BIT2

#define GPIO64_PAD PAD_PWM0
#define GPIO64_OEN 0x102b88, BIT1
#define GPIO64_OUT 0x102b88, BIT0
#define GPIO64_IN  0x102b88, BIT2

#define GPIO65_PAD PAD_PWM1
#define GPIO65_OEN 0x102b89, BIT1
#define GPIO65_OUT 0x102b89, BIT0
#define GPIO65_IN  0x102b89, BIT2

#define GPIO66_PAD PAD_PWM2
#define GPIO66_OEN 0x102b8a, BIT1
#define GPIO66_OUT 0x102b8a, BIT0
#define GPIO66_IN  0x102b8a, BIT2

#define GPIO67_PAD PAD_PWM3
#define GPIO67_OEN 0x102b8b, BIT1
#define GPIO67_OUT 0x102b8b, BIT0
#define GPIO67_IN  0x102b8b, BIT2

#define GPIO68_PAD PAD_SD_CLK
#define GPIO68_OEN 0x102bfa, BIT1
#define GPIO68_OUT 0x102bfa, BIT0
#define GPIO68_IN  0x102bfa, BIT2

#define GPIO69_PAD PAD_SD_CMD
#define GPIO69_OEN 0x102bfb, BIT1
#define GPIO69_OUT 0x102bfb, BIT0
#define GPIO69_IN  0x102bfb, BIT2

#define GPIO70_PAD PAD_SD_D0
#define GPIO70_OEN 0x102bac, BIT1
#define GPIO70_OUT 0x102bac, BIT0
#define GPIO70_IN  0x102bac, BIT2

#define GPIO71_PAD PAD_SD_D1
#define GPIO71_OEN 0x102bad, BIT1
#define GPIO71_OUT 0x102bad, BIT0
#define GPIO71_IN  0x102bad, BIT2

#define GPIO72_PAD PAD_SD_D2
#define GPIO72_OEN 0x102bae, BIT1
#define GPIO72_OUT 0x102bae, BIT0
#define GPIO72_IN  0x102bae, BIT2

#define GPIO73_PAD PAD_SD_D3
#define GPIO73_OEN 0x102bf9, BIT1
#define GPIO73_OUT 0x102bf9, BIT0
#define GPIO73_IN  0x102bf9, BIT2

#define GPIO74_PAD PAD_SPDIF_IN
#define GPIO74_OEN 0x102b36, BIT1
#define GPIO74_OUT 0x102b36, BIT0
#define GPIO74_IN  0x102b36, BIT2

#define GPIO75_PAD PAD_SPDIF_OUT
#define GPIO75_OEN 0x102b37, BIT1
#define GPIO75_OUT 0x102b37, BIT0
#define GPIO75_IN  0x102b37, BIT2

#define GPIO76_PAD PAD_TGPIO0
#define GPIO76_OEN 0x102b8d, BIT1
#define GPIO76_OUT 0x102b8d, BIT0
#define GPIO76_IN  0x102b8d, BIT2

#define GPIO77_PAD PAD_TGPIO1
#define GPIO77_OEN 0x102b8e, BIT1
#define GPIO77_OUT 0x102b8e, BIT0
#define GPIO77_IN  0x102b8e, BIT2

#define GPIO78_PAD PAD_TGPIO2
#define GPIO78_OEN 0x102b8f, BIT1
#define GPIO78_OUT 0x102b8f, BIT0
#define GPIO78_IN  0x102b8f, BIT2

#define GPIO79_PAD PAD_TGPIO3
#define GPIO79_OEN 0x102b90, BIT1
#define GPIO79_OUT 0x102b90, BIT0
#define GPIO79_IN  0x102b90, BIT2

#define GPIO80_PAD PAD_TS1_CLK
#define GPIO80_OEN 0x102b27, BIT1
#define GPIO80_OUT 0x102b27, BIT0
#define GPIO80_IN  0x102b27, BIT2

#define GPIO81_PAD PAD_TS1_D0
#define GPIO81_OEN 0x102b31, BIT1
#define GPIO81_OUT 0x102b31, BIT0
#define GPIO81_IN  0x102b31, BIT2

#define GPIO82_PAD PAD_TS1_D1
#define GPIO82_OEN 0x102b30, BIT1
#define GPIO82_OUT 0x102b30, BIT0
#define GPIO82_IN  0x102b30, BIT2

#define GPIO83_PAD PAD_TS1_D2
#define GPIO83_OEN 0x102b2f, BIT1
#define GPIO83_OUT 0x102b2f, BIT0
#define GPIO83_IN  0x102b2f, BIT2

#define GPIO84_PAD PAD_TS1_D3
#define GPIO84_OEN 0x102b2e, BIT1
#define GPIO84_OUT 0x102b2e, BIT0
#define GPIO84_IN  0x102b2e, BIT2

#define GPIO85_PAD PAD_TS1_D4
#define GPIO85_OEN 0x102b2d, BIT1
#define GPIO85_OUT 0x102b2d, BIT0
#define GPIO85_IN  0x102b2d, BIT2

#define GPIO86_PAD PAD_TS1_D5
#define GPIO86_OEN 0x102b2c, BIT1
#define GPIO86_OUT 0x102b2c, BIT0
#define GPIO86_IN  0x102b2c, BIT2

#define GPIO87_PAD PAD_TS1_D6
#define GPIO87_OEN 0x102b2b, BIT1
#define GPIO87_OUT 0x102b2b, BIT0
#define GPIO87_IN  0x102b2b, BIT2

#define GPIO88_PAD PAD_TS1_D7
#define GPIO88_OEN 0x102b2a, BIT1
#define GPIO88_OUT 0x102b2a, BIT0
#define GPIO88_IN  0x102b2a, BIT2

#define GPIO89_PAD PAD_TS1_SYNC
#define GPIO89_OEN 0x102b28, BIT1
#define GPIO89_OUT 0x102b28, BIT0
#define GPIO89_IN  0x102b28, BIT2

#define GPIO90_PAD PAD_TS1_VLD
#define GPIO90_OEN 0x102b29, BIT1
#define GPIO90_OUT 0x102b29, BIT0
#define GPIO90_IN  0x102b29, BIT2

#define GPIO91_PAD PAD_TS2_CLK
#define GPIO91_OEN 0x102b35, BIT1
#define GPIO91_OUT 0x102b35, BIT0
#define GPIO91_IN  0x102b35, BIT2

#define GPIO92_PAD PAD_TS2_D0
#define GPIO92_OEN 0x102b32, BIT1
#define GPIO92_OUT 0x102b32, BIT0
#define GPIO92_IN  0x102b32, BIT2

#define GPIO93_PAD PAD_TS2_SYNC
#define GPIO93_OEN 0x102b34, BIT1
#define GPIO93_OUT 0x102b34, BIT0
#define GPIO93_IN  0x102b34, BIT2

#define GPIO94_PAD PAD_TS2_VLD
#define GPIO94_OEN 0x102b33, BIT1
#define GPIO94_OUT 0x102b33, BIT0
#define GPIO94_IN  0x102b33, BIT2

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define GPIO_INT_MUX_REG                0x101ea5
#define INT_COUNT                       8
typedef struct
{
    GPIO_Edge       edge_type;
    GPIO_Callback   pCallback;
    MS_U8           IsDirty;

} GPIO_INTCFG;

const MS_GPIO_NUM gpio_IntPad[INT_COUNT] =
{
    68,     //PAD_SD_CLK
    44,     //PAD_GPIO2
    69,     //PAD_SD_CMD
    70,     //PAD_SD_D0
    55,     //PAD_GPIO26
    53,     //PAD_GPIO20
    74,     //PAD_SPDIF_IN
    61,     //PAD_I2S_OUT_SD1
};

const InterruptNum gpio_IRQnum[INT_COUNT] =
{
    E_INT_FIQ_GPIO0,
    E_INT_FIQ_GPIO1,
    E_INT_FIQ_GPIO2,
    E_INT_FIQ_GPIO3,
    E_INT_FIQ_GPIO4,
    E_INT_FIQ_GPIO5,
    E_INT_FIQ_GPIO6,
    E_INT_FIQ_GPIO7,
};

const MS_U32 gpio_IRQpolarity[INT_COUNT][2] =
{
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
    MS_U32  r_oen;
    MS_U8   m_oen;
    MS_U32  r_out;
    MS_U8   m_out;
    MS_U32  r_in;
    MS_U8   m_in;
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
    __GPIO(90), __GPIO(91), __GPIO(92), __GPIO(93), __GPIO(94)
};


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT      _gMIO_MapBase = 0;
static MS_VIRT      _gMIO_MapBase_PM = 0;
static MS_VIRT      _gMIO_MapBase_nonPM = 0;
static GPIO_INTCFG  _gGPIO_Int_Table[INT_COUNT];

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
    MS_U8   i;

    for(i = 0; i < INT_COUNT; i++)
    {
        _gGPIO_Int_Table[i].edge_type = E_GPIO_RISING_EDGE;
        _gGPIO_Int_Table[i].pCallback = NULL;
        _gGPIO_Int_Table[i].IsDirty = FALSE;
    }

    if(INT_COUNT > 8)
    {
        printf("if INT_COUNT > 8 need to check GPIO interrupt code flow !!");
        MS_ASSERT(0);
    }
}

void HAL_GPIO_INTHandler(InterruptNum eIntNum)
{
    MS_U8   i;

    for(i = 0; i < INT_COUNT; i++)
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
    MS_U8   i;

    for(i = 0; i < INT_COUNT; i++)
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
    MS_U8       i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    for(i = 0; i < INT_COUNT; i++)
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
    MS_U8       i;
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
    MS_U8       i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    _gMIO_MapBase = _gMIO_MapBase_PM;
    for(i = 0; i < INT_COUNT; i++)
    {
        if((gpio_IntPad[i] == gpio) && (_gGPIO_Int_Table[i].pCallback != NULL))
        {
            MDrv_WriteByte(GPIO_INT_MUX_REG, (MDrv_ReadByte(GPIO_INT_MUX_REG) | (1 << i)));
            ret_val = E_GPIO_OK;
        }
    }

    return ret_val;
}

GPIO_Result HAL_GPIO_Disable_Interrupt(MS_GPIO_NUM gpio)
{
    MS_U8       i;
    GPIO_Result ret_val = E_GPIO_FAIL;

    _gMIO_MapBase = _gMIO_MapBase_PM;
    for(i = 0; i < INT_COUNT; i++)
    {
        if((gpio_IntPad[i] == gpio) && (_gGPIO_Int_Table[i].pCallback != NULL))
        {
            MDrv_WriteByte(GPIO_INT_MUX_REG, (MDrv_ReadByte(GPIO_INT_MUX_REG) & ~(1 << i)));
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


