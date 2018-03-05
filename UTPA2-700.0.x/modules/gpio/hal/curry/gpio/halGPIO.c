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

#define GPIO5_PAD PAD_PM_GPIO0
#define GPIO5_OEN 0x000f00, BIT0
#define GPIO5_OUT 0x000f00, BIT1
#define GPIO5_IN  0x000f00, BIT2

#define GPIO6_PAD PAD_PM_GPIO1
#define GPIO6_OEN 0x000f02, BIT0
#define GPIO6_OUT 0x000f02, BIT1
#define GPIO6_IN  0x000f02, BIT2

#define GPIO7_PAD PAD_PM_GPIO2
#define GPIO7_OEN 0x000f04, BIT0
#define GPIO7_OUT 0x000f04, BIT1
#define GPIO7_IN  0x000f04, BIT2

#define GPIO8_PAD PAD_PM_GPIO3
#define GPIO8_OEN 0x000f06, BIT0
#define GPIO8_OUT 0x000f06, BIT1
#define GPIO8_IN  0x000f06, BIT2

#define GPIO9_PAD PAD_PM_GPIO4
#define GPIO9_OEN 0x000f08, BIT0
#define GPIO9_OUT 0x000f08, BIT1
#define GPIO9_IN  0x000f08, BIT2

#define GPIO10_PAD PAD_PM_GPIO5
#define GPIO10_OEN 0x000f0a, BIT0
#define GPIO10_OUT 0x000f0a, BIT1
#define GPIO10_IN  0x000f0a, BIT2

#define GPIO11_PAD PAD_PM_GPIO6
#define GPIO11_OEN 0x000f0c, BIT0
#define GPIO11_OUT 0x000f0c, BIT1
#define GPIO11_IN  0x000f0c, BIT2

#define GPIO12_PAD PAD_PM_GPIO7
#define GPIO12_OEN 0x000f0e, BIT0
#define GPIO12_OUT 0x000f0e, BIT1
#define GPIO12_IN  0x000f0e, BIT2

#define GPIO13_PAD PAD_PM_CEC
#define GPIO13_OEN 0x000f2c, BIT0
#define GPIO13_OUT 0x000f2c, BIT1
#define GPIO13_IN  0x000f2c, BIT2

#define GPIO14_PAD PAD_HDMITX_HPD
#define GPIO14_OEN 0x001166, BIT3
#define GPIO14_OUT 0x001166, BIT2
#define GPIO14_IN  0x001166, BIT0

#define GPIO15_PAD PAD_VID0
#define GPIO15_OEN 0x000f90, BIT0
#define GPIO15_OUT 0x000f90, BIT1
#define GPIO15_IN  0x000f90, BIT2

#define GPIO16_PAD PAD_VID1
#define GPIO16_OEN 0x000f92, BIT0
#define GPIO16_OUT 0x000f92, BIT1
#define GPIO16_IN  0x000f92, BIT2

#define GPIO17_PAD PAD_PM_LED0
#define GPIO17_OEN 0x000f94, BIT0
#define GPIO17_OUT 0x000f94, BIT1
#define GPIO17_IN  0x000f94, BIT2

#define GPIO18_PAD PAD_PM_LED1
#define GPIO18_OEN 0x000f96, BIT0
#define GPIO18_OUT 0x000f96, BIT1
#define GPIO18_IN  0x000f96, BIT2

#define GPIO19_PAD PAD_I2S_OUT_BCK
#define GPIO19_OEN 0x102556, BIT4
#define GPIO19_OUT 0x102556, BIT4
#define GPIO19_IN  0x102556, BIT0

#define GPIO20_PAD PAD_I2S_OUT_MCK
#define GPIO20_OEN 0x102558, BIT5
#define GPIO20_OUT 0x102558, BIT4
#define GPIO20_IN  0x102558, BIT0

#define GPIO21_PAD PAD_I2S_OUT_WS
#define GPIO21_OEN 0x10255a, BIT5
#define GPIO21_OUT 0x10255a, BIT4
#define GPIO21_IN  0x10255a, BIT0

#define GPIO22_PAD PAD_I2S_OUT_SD0
#define GPIO22_OEN 0x10255c, BIT5
#define GPIO22_OUT 0x10255c, BIT4
#define GPIO22_IN  0x10255c, BIT0

#define GPIO23_PAD PAD_I2S_OUT_SD1
#define GPIO23_OEN 0x10255e, BIT5
#define GPIO23_OUT 0x10255e, BIT4
#define GPIO23_IN  0x10255e, BIT0

#define GPIO24_PAD PAD_I2S_OUT_SD2
#define GPIO24_OEN 0x102560, BIT5
#define GPIO24_OUT 0x102560, BIT4
#define GPIO24_IN  0x102560, BIT0

#define GPIO25_PAD PAD_I2S_OUT_SD3
#define GPIO25_OEN 0x102562, BIT5
#define GPIO25_OUT 0x102562, BIT4
#define GPIO25_IN  0x102562, BIT0

#define GPIO26_PAD PAD_I2S_OUT_MUTE
#define GPIO26_OEN 0x102564, BIT5
#define GPIO26_OUT 0x102564, BIT4
#define GPIO26_IN  0x102564, BIT0

#define GPIO27_PAD PAD_SD_CLK
#define GPIO27_OEN 0x1025a4, BIT5
#define GPIO27_OUT 0x1025a4, BIT4
#define GPIO27_IN  0x1025a4, BIT0

#define GPIO28_PAD PAD_SD_CMD
#define GPIO28_OEN 0x1025a6, BIT5
#define GPIO28_OUT 0x1025a6, BIT4
#define GPIO28_IN  0x1025a6, BIT0

#define GPIO29_PAD PAD_SD_D0
#define GPIO29_OEN 0x1025a8, BIT5
#define GPIO29_OUT 0x1025a8, BIT4
#define GPIO29_IN  0x1025a8, BIT0

#define GPIO30_PAD PAD_SD_D1
#define GPIO30_OEN 0x1025aa, BIT5
#define GPIO30_OUT 0x1025aa, BIT4
#define GPIO30_IN  0x1025aa, BIT0

#define GPIO31_PAD PAD_SD_D2
#define GPIO31_OEN 0x1025ac, BIT5
#define GPIO31_OUT 0x1025ac, BIT4
#define GPIO31_IN  0x1025ac, BIT0

#define GPIO32_PAD PAD_SD_D3
#define GPIO32_OEN 0x1025ae, BIT5
#define GPIO32_OUT 0x1025ae, BIT4
#define GPIO32_IN  0x1025ae, BIT0

#define GPIO33_PAD PAD_GPIO0
#define GPIO33_OEN 0x1025b0, BIT5
#define GPIO33_OUT 0x1025b0, BIT4
#define GPIO33_IN  0x1025b0, BIT0

#define GPIO34_PAD PAD_GPIO1
#define GPIO34_OEN 0x1025b2, BIT5
#define GPIO34_OUT 0x1025b2, BIT4
#define GPIO34_IN  0x1025b2, BIT0

#define GPIO35_PAD PAD_GPIO2
#define GPIO35_OEN 0x1025b4, BIT5
#define GPIO35_OUT 0x1025b4, BIT4
#define GPIO35_IN  0x1025b4, BIT0

#define GPIO36_PAD PAD_GPIO3
#define GPIO36_OEN 0x1025b6, BIT5
#define GPIO36_OUT 0x1025b6, BIT4
#define GPIO36_IN  0x1025b6, BIT0

#define GPIO37_PAD PAD_GPIO4
#define GPIO37_OEN 0x1025b8, BIT5
#define GPIO37_OUT 0x1025b8, BIT4
#define GPIO37_IN  0x1025b8, BIT0

#define GPIO38_PAD PAD_GPIO5
#define GPIO38_OEN 0x1025ba, BIT5
#define GPIO38_OUT 0x1025ba, BIT4
#define GPIO38_IN  0x1025ba, BIT0

#define GPIO39_PAD PAD_GPIO6
#define GPIO39_OEN 0x1025bc, BIT5
#define GPIO39_OUT 0x1025bc, BIT4
#define GPIO39_IN  0x1025bc, BIT0

#define GPIO40_PAD PAD_GPIO7
#define GPIO40_OEN 0x1025be, BIT5
#define GPIO40_OUT 0x1025be, BIT4
#define GPIO40_IN  0x1025be, BIT0

#define GPIO41_PAD PAD_TS_CLK
#define GPIO41_OEN 0x102524, BIT5
#define GPIO41_OUT 0x102524, BIT4
#define GPIO41_IN  0x102524, BIT0

#define GPIO42_PAD PAD_TS_SYNC
#define GPIO42_OEN 0x102526, BIT5
#define GPIO42_OUT 0x102526, BIT4
#define GPIO42_IN  0x102526, BIT0

#define GPIO43_PAD PAD_TS_VLD
#define GPIO43_OEN 0x102528, BIT5
#define GPIO43_OUT 0x102528, BIT4
#define GPIO43_IN  0x102528, BIT0

#define GPIO44_PAD PAD_TS_D0
#define GPIO44_OEN 0x10252a, BIT5
#define GPIO44_OUT 0x10252a, BIT4
#define GPIO44_IN  0x10252a, BIT0

#define GPIO45_PAD PAD_TS_D1
#define GPIO45_OEN 0x10252c, BIT5
#define GPIO45_OUT 0x10252c, BIT4
#define GPIO45_IN  0x10252c, BIT0

#define GPIO46_PAD PAD_TS_D2
#define GPIO46_OEN 0x10252e, BIT5
#define GPIO46_OUT 0x10252e, BIT4
#define GPIO46_IN  0x10252e, BIT0

#define GPIO47_PAD PAD_TS_D3
#define GPIO47_OEN 0x102530, BIT5
#define GPIO47_OUT 0x102530, BIT4
#define GPIO47_IN  0x102530, BIT0

#define GPIO48_PAD PAD_TS_D4
#define GPIO48_OEN 0x102532, BIT5
#define GPIO48_OUT 0x102532, BIT4
#define GPIO48_IN  0x102532, BIT0

#define GPIO49_PAD PAD_TS_D5
#define GPIO49_OEN 0x102534, BIT5
#define GPIO49_OUT 0x102534, BIT4
#define GPIO49_IN  0x102534, BIT0

#define GPIO50_PAD PAD_TS_D6
#define GPIO50_OEN 0x102536, BIT5
#define GPIO50_OUT 0x102536, BIT4
#define GPIO50_IN  0x102536, BIT0

#define GPIO51_PAD PAD_TS_D7
#define GPIO51_OEN 0x102538, BIT5
#define GPIO51_OUT 0x102538, BIT4
#define GPIO51_IN  0x102538, BIT0

#define GPIO52_PAD PAD_SPDIF_OUT
#define GPIO52_OEN 0x1025a0, BIT5
#define GPIO52_OUT 0x1025a0, BIT4
#define GPIO52_IN  0x1025a0, BIT0

#define GPIO53_PAD PAD_I2CM0_SCL
#define GPIO53_OEN 0x10251c, BIT5
#define GPIO53_OUT 0x10251c, BIT4
#define GPIO53_IN  0x10251c, BIT0

#define GPIO54_PAD PAD_I2CM0_SDA
#define GPIO54_OEN 0x10251e, BIT5
#define GPIO54_OUT 0x10251e, BIT4
#define GPIO54_IN  0x10251e, BIT0

#define GPIO55_PAD PAD_I2CM1_SCL
#define GPIO55_OEN 0x102520, BIT5
#define GPIO55_OUT 0x102520, BIT4
#define GPIO55_IN  0x102520, BIT0

#define GPIO56_PAD PAD_I2CM1_SDA
#define GPIO56_OEN 0x102522, BIT5
#define GPIO56_OUT 0x102522, BIT4
#define GPIO56_IN  0x102522, BIT0

#define GPIO57_PAD PAD_HDMITX_SCL
#define GPIO57_OEN 0x1025da, BIT5
#define GPIO57_OUT 0x1025da, BIT4
#define GPIO57_IN  0x1025da, BIT0

#define GPIO58_PAD PAD_HDMITX_SDA
#define GPIO58_OEN 0x1025dc, BIT5
#define GPIO58_OUT 0x1025dc, BIT4
#define GPIO58_IN  0x1025dc, BIT0

#define GPIO59_PAD PAD_GT0_MDIO
#define GPIO59_OEN 0x10253a, BIT5
#define GPIO59_OUT 0x10253a, BIT4
#define GPIO59_IN  0x10253a, BIT0

#define GPIO60_PAD PAD_GT0_MDC
#define GPIO60_OEN 0x10253c, BIT5
#define GPIO60_OUT 0x10253c, BIT4
#define GPIO60_IN  0x10253c, BIT0

#define GPIO61_PAD PAD_GT0_RX_CLK
#define GPIO61_OEN 0x10253e, BIT5
#define GPIO61_OUT 0x10253e, BIT4
#define GPIO61_IN  0x10253e, BIT0

#define GPIO62_PAD PAD_GT0_RX_CTL
#define GPIO62_OEN 0x102540, BIT5
#define GPIO62_OUT 0x102540, BIT4
#define GPIO62_IN  0x102540, BIT0

#define GPIO63_PAD PAD_GT0_RX_D0
#define GPIO63_OEN 0x102542, BIT5
#define GPIO63_OUT 0x102542, BIT4
#define GPIO63_IN  0x102542, BIT0

#define GPIO64_PAD PAD_GT0_RX_D1
#define GPIO64_OEN 0x102544, BIT5
#define GPIO64_OUT 0x102544, BIT4
#define GPIO64_IN  0x102544, BIT0

#define GPIO65_PAD PAD_GT0_RX_D2
#define GPIO65_OEN 0x102546, BIT5
#define GPIO65_OUT 0x102546, BIT4
#define GPIO65_IN  0x102546, BIT0

#define GPIO66_PAD PAD_GT0_RX_D3
#define GPIO66_OEN 0x102548, BIT5
#define GPIO66_OUT 0x102548, BIT4
#define GPIO66_IN  0x102548, BIT0

#define GPIO67_PAD PAD_GT0_TX_CLK
#define GPIO67_OEN 0x10254a, BIT5
#define GPIO67_OUT 0x10254a, BIT4
#define GPIO67_IN  0x10254a, BIT0

#define GPIO68_PAD PAD_GT0_TX_CTL
#define GPIO68_OEN 0x10254c, BIT5
#define GPIO68_OUT 0x10254c, BIT4
#define GPIO68_IN  0x10254c, BIT0

#define GPIO69_PAD PAD_GT0_TX_D0
#define GPIO69_OEN 0x10254e, BIT5
#define GPIO69_OUT 0x10254e, BIT4
#define GPIO69_IN  0x10254e, BIT0

#define GPIO70_PAD PAD_GT0_TX_D1
#define GPIO70_OEN 0x102550, BIT5
#define GPIO70_OUT 0x102550, BIT4
#define GPIO70_IN  0x102550, BIT0

#define GPIO71_PAD PAD_GT0_TX_D2
#define GPIO71_OEN 0x102552, BIT5
#define GPIO71_OUT 0x102552, BIT4
#define GPIO71_IN  0x102552, BIT0

#define GPIO72_PAD PAD_GT0_TX_D3
#define GPIO72_OEN 0x102554, BIT5
#define GPIO72_OUT 0x102554, BIT4
#define GPIO72_IN  0x102554, BIT0

#define GPIO73_PAD PAD_SAR_GPIO0
#define GPIO73_OEN 0x001423, BIT0
#define GPIO73_OUT 0x001424, BIT0
#define GPIO73_IN  0x001425, BIT0

#define GPIO74_PAD PAD_SAR_GPIO1
#define GPIO74_OEN 0x001423, BIT1
#define GPIO74_OUT 0x001424, BIT1
#define GPIO74_IN  0x001425, BIT1

#define GPIO75_PAD PAD_SAR_GPIO2
#define GPIO75_OEN 0x001423, BIT2
#define GPIO75_OUT 0x001424, BIT2
#define GPIO75_IN  0x001425, BIT2

#define GPIO76_PAD PAD_SAR_GPIO3
#define GPIO76_OEN 0x001423, BIT3
#define GPIO76_OUT 0x001424, BIT3
#define GPIO76_IN  0x001425, BIT3

#define GPIO77_PAD PAD_SAR_GPIO4
#define GPIO77_OEN 0x001423, BIT4
#define GPIO77_OUT 0x001424, BIT4
#define GPIO77_IN  0x001425, BIT4

#define GPIO78_PAD PAD_EMMC_IO12
#define GPIO78_OEN 0x102588, BIT5
#define GPIO78_OUT 0x102588, BIT4
#define GPIO78_IN  0x102588, BIT0

#define GPIO79_PAD PAD_EMMC_IO14
#define GPIO79_OEN 0x10258c, BIT5
#define GPIO79_OUT 0x10258c, BIT4
#define GPIO79_IN  0x10258c, BIT0

#define GPIO80_PAD PAD_EMMC_IO15
#define GPIO80_OEN 0x10258e, BIT5
#define GPIO80_OUT 0x10258e, BIT4
#define GPIO80_IN  0x10258e, BIT0

#define GPIO81_PAD PAD_EMMC_IO16
#define GPIO81_OEN 0x102590, BIT5
#define GPIO81_OUT 0x102590, BIT4
#define GPIO81_IN  0x102590, BIT0

#define GPIO82_PAD PAD_EMMC_IO17
#define GPIO82_OEN 0x102592, BIT5
#define GPIO82_OUT 0x102592, BIT4
#define GPIO82_IN  0x102592, BIT0

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
    __GPIO(80), __GPIO(81), __GPIO(82),
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
    gpio_num = gpio_num;
    gpio_edge_type = gpio_edge_type;
    pCallback = pCallback;
    return E_GPIO_NOT_SUPPORT;
}

GPIO_Result HAL_GPIO_Datach_Interrupt(MS_GPIO_NUM gpio_num)
{
    gpio_num = gpio_num;
    return E_GPIO_NOT_SUPPORT;
}

GPIO_Result HAL_GPIO_Enable_Interrupt(MS_GPIO_NUM gpio)
{
    gpio = gpio;
    return E_GPIO_NOT_SUPPORT;
}

GPIO_Result HAL_GPIO_Disable_Interrupt(MS_GPIO_NUM gpio)
{
    gpio = gpio;
    return E_GPIO_NOT_SUPPORT;
}

void HAL_GPIO_Enable_Interrupt_all(void)
{

}

void HAL_GPIO_Disable_Interrupt_all(void)
{

}

#endif      //_HALGPIO_C_


