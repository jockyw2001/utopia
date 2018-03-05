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
#ifndef _HAL_PNL_C_
#define _HAL_PNL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "halPNL.h"
#include "PNL_private.h"
#include "pnl_hwreg_utility2.h"
#include "Maserati_pnl_lpll_tbl.h"
#include "Maserati_pnl_lpll_ext_tbl.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#else
#include "string.h"
#define do_div(x,y) ((x)/=(y))
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define UNUSED(x)       (x=x)
#if 1
#define HAL_PNL_DBG(_dbgSwitch_, _fmt, _args...)      { if((_dbgSwitch_ & _u16PnlDbgSwitch) != 0) printf("PNL:"_fmt, ##_args); }
#define HAL_MOD_CAL_DBG(x)    //x
#else
#define HAL_PNL_DBG(_dbgSwitch_, _fmt, _args...)      { }
#endif

#define DAC_LPLL_ICTRL     0x0002
#define LVDS_LPLL_ICTRL    0x0001

//Get MOD calibration time
#define MOD_CAL_TIMER   FALSE

//if mboot read eFuse and fill the register, then add this define to mark utopia efuse code flow
#define MOD_EFUSE_IN_MBOOT

//for LVDS HW Calibration ICON limitation
#define MOD_LVDS_ICON_HIGH_LIMIT 0x2E
#define MOD_LVDS_ICON_LOW_LIMIT  0x06
#define MOD_LVDS_ICON_DEFAULT    0x19

//for LVDS HW Calibration timeout (i.e. retry times after hw calibration failed)
#define MOD_LVDS_HW_CALI_TIME_OUT  0
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
#define CLK_GEN0_REG_VIDEO_NUM    4
#define CLK_GEN0_REG_OSD_NUM      3
#define CLK_GEN2_REG_VIDEO_NUM    2
#define CLK_GEN2_REG_OSD_NUM      1

typedef enum
{
    E_PNL_SUPPORTED_CLK_TTL,                          // 0
    E_PNL_SUPPORTED_CLK_LVDS_1CH,                     // 1
    E_PNL_SUPPORTED_CLK_LVDS_2CH,                     // 2
    E_PNL_SUPPORTED_CLK_HS_LVDS_1CH,                  // 3
    E_PNL_SUPPORTED_CLK_HS_LVDS_2CH,                  // 4
    E_PNL_SUPPORTED_CLK_VBY1_1CH_10BIT,               // 5
    E_PNL_SUPPORTED_CLK_VBY1_1CH_8BIT,                // 6
    E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT,               // 7
    E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT,                // 8
    E_PNL_SUPPORTED_CLK_VBY1_4CH_10BIT,               // 9
    E_PNL_SUPPORTED_CLK_VBY1_4CH_8BIT,                // 10
    E_PNL_SUPPORTED_CLK_VBY1_8CH_10BIT,               // 11
    E_PNL_SUPPORTED_CLK_VBY1_8CH_8BIT,                // 12
    E_PNL_SUPPORTED_CLK_VBY1_16CH_10BIT,              // 13
    E_PNL_SUPPORTED_CLK_VBY1_16CH_8BIT,               // 14
    E_PNL_SUPPORTED_CLK_EPI_24_10BIT_12PAIR_X_1,      // 15
    E_PNL_SUPPORTED_CLK_EPI_28_8BIT_12PAIR_X_1,       // 16
    E_PNL_SUPPORTED_CLK_CMPI_27_8BIT_12PAIR_X_1,      // 17
    E_PNL_SUPPORTED_CLK_CMPI_24_10BIT_12PAIR_X_1,     // 18
    E_PNL_SUPPORTED_CLK_USIT_8BIT_12PAIR_X_1,         // 19
    E_PNL_SUPPORTED_CLK_USIT_10BIT_12PAIR_X_1,        // 20
    E_PNL_SUPPORTED_ISP_9_8BIT_12PAIR_X_1,            // 21
    E_PNL_SUPPORTED_ISP_9_8BIT_6PAIR_X_1,             // 22
    E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT,              // 23
    E_PNL_SUPPORTED_CLK_VBY1_8V4O_8BIT,               // 24
    E_PNL_SUPPORTED_CLK_VBY1_4V4O_10BIT,              // 25
    E_PNL_SUPPORTED_CLK_VBY1_4V4O_8BIT,               // 26
    E_PNL_SUPPORTED_CLK_VBY1_4V2O_10BIT,              // 27
    E_PNL_SUPPORTED_CLK_VBY1_4V2O_8BIT,               // 28
    E_PNL_SUPPORTED_CLK_MAX,                          // 29
} E_PNL_SUPPORTED_CLK_TYPE;

TBLStruct CLKGEN2SettingTBL_Video[E_PNL_SUPPORTED_CLK_MAX][CLK_GEN2_REG_VIDEO_NUM]=
{
    { //E_PNL_SUPPORTED_CLK_TTL    NO.0
      //Address,Value,Mask
        {0x44,0x0014,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_1CH    NO.1
      //Address,Value,Mask
        {0x44,0x0014,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_2CH    NO.2
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_1CH    NO.3
      //Address,Value,Mask
        {0x44,0x0014,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_2CH    NO.4
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_1CH_10BIT    NO.5
      //Address,Value,Mask
        {0x44,0x0014,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_1CH_8BIT    NO.6
      //Address,Value,Mask
        {0x44,0x0014,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT    NO.7
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT    NO.8
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_10BIT    NO.9
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_8BIT    NO.10
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_10BIT    NO.11
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_8BIT    NO.12
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_10BIT    NO.13
      //Address,Value,Mask
        {0x44,0x000C,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_8BIT    NO.14
      //Address,Value,Mask
        {0x44,0x000C,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_EPI_24_10BIT_12PAIR_X_1    NO.15
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_EPI_28_8BIT_12PAIR_X_1    NO.16
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_27_8BIT_12PAIR_X_1    NO.17
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_24_10BIT_12PAIR_X_1    NO.18
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_USIT_8BIT_12PAIR_X_1    NO.19
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_USIT_10BIT_12PAIR_X_1    NO.20
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_12PAIR_X_1    NO.21
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_6PAIR_X_1    NO.22
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0001,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT    NO.23
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_8BIT    NO.24
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_10BIT    NO.25
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_8BIT    NO.26
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_10BIT    NO.27
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_8BIT    NO.28
      //Address,Value,Mask
        {0x44,0x0004,0x001F},//clk_odclk
        {0x09,0x0000,0x000F},//clk_vby1
    },
};

TBLStruct CLKGEN2SettingTBL_OSD[E_PNL_SUPPORTED_CLK_MAX][CLK_GEN2_REG_OSD_NUM]=
{
    { //E_PNL_SUPPORTED_CLK_TTL    NO.0
      //Address,Value,Mask
        {0x09,0x0100,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_1CH    NO.1
      //Address,Value,Mask
        {0x09,0x0100,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_2CH    NO.2
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_1CH    NO.3
      //Address,Value,Mask
        {0x09,0x0100,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_2CH    NO.4
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_1CH_10BIT    NO.5
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_1CH_8BIT    NO.6
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT    NO.7
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT    NO.8
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_10BIT    NO.9
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_8BIT    NO.10
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_10BIT    NO.11
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_8BIT    NO.12
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_10BIT    NO.13
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_8BIT    NO.14
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_EPI_24_10BIT_12PAIR_X_1    NO.15
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_EPI_28_8BIT_12PAIR_X_1    NO.16
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_27_8BIT_12PAIR_X_1    NO.17
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_24_10BIT_12PAIR_X_1    NO.18
      //Address,Value,Mask
        {0x09,0x0000,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_USIT_8BIT_12PAIR_X_1    NO.19
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_USIT_10BIT_12PAIR_X_1    NO.20
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_12PAIR_X_1    NO.21
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_6PAIR_X_1    NO.22
      //Address,Value,Mask
        {0x09,0x0800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT    NO.23
      //Address,Value,Mask
        {0x09,0x1800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_8BIT    NO.24
      //Address,Value,Mask
        {0x09,0x1800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_10BIT    NO.25
      //Address,Value,Mask
        {0x09,0x1800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_8BIT    NO.26
      //Address,Value,Mask
        {0x09,0x1800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_10BIT    NO.27
      //Address,Value,Mask
        {0x09,0x1800,0x3F00},//clk_vby1_osd (wclk)
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_8BIT    NO.28
      //Address,Value,Mask
        {0x09,0x1800,0x3F00},//clk_vby1_osd (wclk)
    },
};

TBLStruct CLKGEN0SettingTBL_Video[E_PNL_SUPPORTED_CLK_MAX][CLK_GEN0_REG_VIDEO_NUM]=
{
    { //E_PNL_SUPPORTED_CLK_TTL    NO.0
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_1CH    NO.1
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_2CH    NO.2
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_1CH    NO.3
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_2CH    NO.4
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_1CH_10BIT    NO.5
      //Address,Value,Mask
        {0x5A,0x0A00,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_1CH_8BIT    NO.6
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT    NO.7
      //Address,Value,Mask
        {0x5A,0x0A00,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT    NO.8
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_10BIT    NO.9
      //Address,Value,Mask
        {0x5A,0x0900,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_8BIT    NO.10
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_10BIT    NO.11
      //Address,Value,Mask
        {0x5A,0x0800,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_8BIT    NO.12
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_10BIT    NO.13
      //Address,Value,Mask
        {0x5A,0x0800,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_8BIT    NO.14
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_EPI_24_10BIT_12PAIR_X_1    NO.15
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_EPI_28_8BIT_12PAIR_X_1    NO.16
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_27_8BIT_12PAIR_X_1    NO.17
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_24_10BIT_12PAIR_X_1    NO.18
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x0000,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_USIT_8BIT_12PAIR_X_1    NO.19
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_USIT_10BIT_12PAIR_X_1    NO.20
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_12PAIR_X_1    NO.21
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_6PAIR_X_1    NO.22
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0C00,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT    NO.23
      //Address,Value,Mask
        {0x5A,0x0800,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_8BIT    NO.24
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_10BIT    NO.25
      //Address,Value,Mask
        {0x5A,0x0900,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_8BIT    NO.26
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_10BIT    NO.27
      //Address,Value,Mask
        {0x5A,0x0900,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_8BIT    NO.28
      //Address,Value,Mask
        {0x5A,0x0000,0x0F00},//Vmode
        {0x53,0x0400,0x3F00},//clk_odclk_mft
        {0x57,0x000C,0x000F},//wclk
        {0x58,0x0000,0x000F},//rclk
    },
};

TBLStruct CLKGEN0SettingTBL_OSD[E_PNL_SUPPORTED_CLK_MAX][CLK_GEN0_REG_OSD_NUM]=
{
    { //E_PNL_SUPPORTED_CLK_TTL    NO.0
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0100,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_1CH    NO.1
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0000,0x003F},//clk_osd2mod//{0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk//{0x63,0x0100,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_LVDS_2CH    NO.2
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0000,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_1CH    NO.3
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0100,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_HS_LVDS_2CH    NO.4
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0000,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT    NO.5
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT    NO.6
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },


    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT    NO.7
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT    NO.8
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_10BIT    NO.9
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4CH_8BIT    NO.10
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_10BIT    NO.11
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8CH_8BIT    NO.12
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_10BIT    NO.13
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_16CH_8BIT    NO.14
      //Address,Value,Mask

        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_EPI_24_10BIT_12PAIR_X_1    NO.15
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_EPI_28_8BIT_12PAIR_X_1    NO.16
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_27_8BIT_12PAIR_X_1    NO.17
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_CMPI_24_10BIT_12PAIR_X_1    NO.18
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_USIT_8BIT_12PAIR_X_1    NO.19
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_USIT_10BIT_12PAIR_X_1    NO.20
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_12PAIR_X_1    NO.21
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_ISP_9_8BIT_6PAIR_X_1    NO.22
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0001,0x003F},//clk_osd2mod
        {0x63,0x0000,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT    NO.23
      //Address,Value,Mask
        {0x5A,0x8000,0xF000},//Omode
        {0x63,0x0010,0x003F},//clk_osd2mod
        {0x63,0x0400,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_8V4O_8BIT    NO.24
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0010,0x003F},//clk_osd2mod
        {0x63,0x0400,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_10BIT    NO.25
      //Address,Value,Mask
        {0x5A,0x8000,0xF000},//Omode
        {0x63,0x0010,0x003F},//clk_osd2mod
        {0x63,0x0400,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V4O_8BIT    NO.26
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0010,0x003F},//clk_osd2mod
        {0x63,0x0400,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_10BIT    NO.27
      //Address,Value,Mask
        {0x5A,0x9000,0xF000},//Omode
        {0x63,0x0010,0x003F},//clk_osd2mod
        {0x63,0x0400,0x0F00},//osd_rclk
    },

    { //E_PNL_SUPPORTED_CLK_VBY1_4V2O_8BIT    NO.28
      //Address,Value,Mask
        {0x5A,0x0000,0xF000},//Omode
        {0x63,0x0010,0x003F},//clk_osd2mod
        {0x63,0x0400,0x0F00},//osd_rclk
    },
};
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define LANE_NUM_EACH_PINMAPPING_GROUP1 4
#define LANE_NUM_EACH_PINMAPPING_GROUP2 4
#define LANE_NUM_EACH_PINMAPPING_GROUP3 4
#define LANE_NUM_EACH_PINMAPPING_GROUP4 4

#define PINMAPPING_EXP 16

#define PINMAPPING_MAX_LANE (0xF)
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U8 LANE_AND_CLK_TBL[VBY1_CLK_TBL_ROW][3]=
{ //lane(from)  lane(to) bit(mask)
 { 0, 3, 0x01, },
 { 4, 7, 0x02, },
 { 8, 11,0x04, },
 { 12,15,0x08, }
};

extern MS_BOOL MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub(void);
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _Hal_MOD_External_eFuse(void);
static void _Hal_MOD_VB1_CH_SWICH(PNL_TYPE eLPLL_Type_Ext);
static MS_U16 _Hal_MOD_Refine_ICON(MS_U16 u16ICON);
static E_PNL_SUPPORTED_CLK_TYPE _MHal_Transfer_PanelType_To_CLKType(PNL_TYPE eLPLL_Type, PNL_MODE eLPLL_Mode);
static E_PNL_SUPPORTED_CLK_TYPE _MHal_Transfer_PanelType_To_CLKType_OSD(PNL_TYPE eLPLL_OSD_Type,PNL_TYPE eLPLL_Video_Type,PNL_MODE eLPLL_Mode);
static void _MHal_PNL_DumpVideoClkTable(MS_U8 u8CLKTblIndex);
static void _MHal_PNL_DumpOSDClkTable(MS_U8 u8CLKTblIndex);
static void _MHal_PNL_Init_MFT(void *pInstance, PNL_InitData *pstPanelInitData);

static void _MHal_PNL_Auto_Set_Config(void *pInstance,
                                      MS_U16 u16OutputOrder0_3,
                                      MS_U16 u16OutputOrder4_7,
                                      MS_U16 u16OutputOrder8_11,
                                      MS_U16 u16OutputOrder12_15);

static void _MHal_PNL_Set_Clk(void *pInstance,
                              MS_U8 u8LaneNum,
                              MS_U16 u16OutputOrder0_3,
                              MS_U16 u16OutputOrder4_7,
                              MS_U16 u16OutputOrder8_11,
                              MS_U16 u16OutputOrder12_15);

static MS_U8 _MHal_PNL_Get_LaneNum(void *pInstance);
//-------------------------------------------------------------------------------------------------
//  Global Function
//-------------------------------------------------------------------------------------------------
/**
*   @brief: Power On MOD. but not mutex protected
*
*/
#define OUTPUT_CHANNEL_HALF_L 0x0055
#define OUTPUT_CHANNEL_HALF_H 0x5500

MS_U8 MHal_MOD_PowerOn(void *pInstance, MS_BOOL bEn, MS_U8 u8LPLL_Type,MS_U8 DualModeType, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    MS_U16 u16ChannelClk_En = 0;

    if( bEn )
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, 0x00, BIT(4)); //reg_ck1x_4dig_phsel_path2

        //analog MOD power down. 1: power down, 0: power up
        // For Mod2 no output signel
        ///////////////////////////////////////////////////

        //2. Power on MOD (current and regulator)
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, BIT(1) , BIT(1)); //reg_gcr_en_reg: enable clk tree pwr
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x00 , BIT(0)); //reg_pd_ib_mod: power down mod atop

        //enable ib, enable ck
        // [0]:ch0_3 [1]:ch4_7 [2]:ch8_11 [3]:ch12_15
        if(u16OutputCFG0_7  & OUTPUT_CHANNEL_HALF_L)
            u16ChannelClk_En |= BIT(0);
        if(u16OutputCFG0_7  & OUTPUT_CHANNEL_HALF_H)
            u16ChannelClk_En |= BIT(1);
        if(u16OutputCFG8_15 & OUTPUT_CHANNEL_HALF_L)
            u16ChannelClk_En |= BIT(2);
        if(u16OutputCFG8_15 & OUTPUT_CHANNEL_HALF_H)
            u16ChannelClk_En |= BIT(3);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, u16ChannelClk_En, 0x001F);

        // clock gen of dot-mini
        if(u8LPLL_Type == E_PNL_TYPE_MINILVDS)
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0400, 0x0FFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0444, 0x0FFF);
        }
        else if( (u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_16LANE)||
                 (u8LPLL_Type == E_PNL_LPLL_VBY1_8BIT_16LANE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0000, 0x0FFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0000, 0x0FFF);
        }
        else if(u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0000, 0x0FFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0000, 0x0FFF);
        }
        else if((u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)||
                (u8LPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)||
                (u8LPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE) ||
                (u8LPLL_Type == E_PNL_LPLL_VBY1_8BIT_2LANE))

        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0000, 0x0FFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0000, 0x0FFF);
        }
        //// for osd dedicated output port, 1 port for video and 1 port for osd
        else if((u8LPLL_Type == E_PNL_TYPE_HS_LVDS)&&
                (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode == E_PNL_MODE_SINGLE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0044, 0x0FFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0400, 0x0FFF);
        }
        else
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0000, 0x0FFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0000, 0x0FFF);
        }

        // 3. 4. 5.
        MHal_Output_LVDS_Pair_Setting(pInstance, DualModeType, u16OutputCFG0_7, u16OutputCFG8_15, u16OutputCFG16_21);
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, BIT(4), BIT(4));// reg_ck1x_4dig_phsel_path2: test phase
        if(u8LPLL_Type !=E_PNL_TYPE_MINILVDS)
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, BIT(0), BIT(0)); //analog MOD power down. 1: power down, 0: power up
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x00, BIT(1));   //reg_gcr_en_reg: enable clk tree pwr
        }

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x0000, 0x001F);                           //enable ib, enable ck

        // clock gen of dot-mini
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_76_L, 0x0100, 0x0FFF);
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0111, 0x0FFF);

        if(  IsVBY1(u8LPLL_Type) )
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x2000,0x2000);
            MsOS_DelayTask(1);
            MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0000);
            MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x0000,0x2000);
        }
    }
    return 1;
}

/**
*   @brief: Setup the PVDD power 1:2.5V, 0:3.3V
*
*/
void MHal_MOD_PVDD_Power_Setting(void *pInstance, MS_BOOL bIs2p5)
{
    //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, ((bIs2p5)? BIT(6):0), BIT(6));    //MOD PVDD=1: 0.9
}

void MHal_PNL_TCON_Init(void *pInstance)
{

}

void MHal_Shift_LVDS_Pair(void *pInstance, MS_U8 Type)
{
    UNUSED(Type);
}

void MHal_Output_LVDS_Pair_Setting(void *pInstance, MS_U8 Type, MS_U16 u16OutputCFG0_7, MS_U16 u16OutputCFG8_15, MS_U16 u16OutputCFG16_21)
{

    if(Type == LVDS_DUAL_OUTPUT_SPECIAL )
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0555);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x1554);
    }
    else if(Type == LVDS_SINGLE_OUTPUT_A)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_18_L, 0x5550, 0xFFF0);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0555);
    }
    else if( Type == LVDS_SINGLE_OUTPUT_B)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_18_L, 0x5550, 0xFFF0);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0555);
    }
    else if( Type == LVDS_OUTPUT_User)
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, u16OutputCFG0_7);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, u16OutputCFG8_15);
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_18_L, 0x5550, 0xFFF0);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0555);
    }

    MsOS_DelayTask(2);

}

void MHal_Output_Channel_Order(void *pInstance,
                               MS_U8 Type,
                               MS_U16 u16OutputOrder0_3,
                               MS_U16 u16OutputOrder4_7,
                               MS_U16 u16OutputOrder8_11,
                               MS_U16 u16OutputOrder12_13)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if(Type == APIPNL_OUTPUT_CHANNEL_ORDER_USER )
    {
        MOD_W2BYTE(REG_MOD_BK00_55_L, u16OutputOrder0_3);
        MOD_W2BYTE(REG_MOD_BK00_56_L, u16OutputOrder4_7);
        MOD_W2BYTE(REG_MOD_BK00_57_L, u16OutputOrder8_11);
        MOD_W2BYTE(REG_MOD_BK00_58_L, u16OutputOrder12_13);
    }
    else
    {
        if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
            ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1)) // 2 Divisoin
            {//APN 8V setting
                MOD_W2BYTE(REG_MOD_BK00_55_L, 0x3210);
                MOD_W2BYTE(REG_MOD_BK00_56_L, 0xBA98);
                MOD_W2BYTE(REG_MOD_BK00_57_L, 0xFFFF);
                MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFFFF);
            }
            else
            {//APN 8V setting
                MOD_W2BYTE(REG_MOD_BK00_55_L, 0x9810);
                MOD_W2BYTE(REG_MOD_BK00_56_L, 0xBA32);
                MOD_W2BYTE(REG_MOD_BK00_57_L, 0xFFFF);
                MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFFFF);
            }
        }
        else if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)
        {//LVDS
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode==E_PNL_MODE_SINGLE)
            {
                MOD_W2BYTE(REG_MOD_BK00_55_L, 0x76FF);
                MOD_W2BYTE(REG_MOD_BK00_56_L, 0xBA98);
                MOD_W2BYTE(REG_MOD_BK00_57_L, 0x3210);
                MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFF54);
            }
            else
            {
                MOD_W2BYTE(REG_MOD_BK00_55_L, 0x10FF);
                MOD_W2BYTE(REG_MOD_BK00_56_L, 0x5432);
                MOD_W2BYTE(REG_MOD_BK00_57_L, 0x9876);
                MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFFBA);
            }
        }
        else if( (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)
               ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE))
        {
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            { // 4 Divisoin
                MOD_W2BYTE(REG_MOD_BK00_55_L, 0x6420);
                MOD_W2BYTE(REG_MOD_BK00_56_L, 0x7531);
                MOD_W2BYTE(REG_MOD_BK00_57_L, 0xECA8);
                MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFDB9);
            }
            else
            { // 2 division
                MOD_W2BYTE(REG_MOD_BK00_55_L, 0x3210);
                MOD_W2BYTE(REG_MOD_BK00_56_L, 0x7654);
                MOD_W2BYTE(REG_MOD_BK00_57_L, 0xBA98);
                MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFEDC);
            }
        }
        else if( (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
               ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE))
        {// APN just video setting
            MOD_W2BYTE(REG_MOD_BK00_55_L, 0x9810);
            MOD_W2BYTE(REG_MOD_BK00_56_L, 0xFFFF);
            MOD_W2BYTE(REG_MOD_BK00_57_L, 0xFFFF);
            MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFFFF);
        }
        else if( (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE)
               ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_2LANE))
        {//APN just video setting
            MOD_W2BYTE(REG_MOD_BK00_55_L, 0xFF10);
            MOD_W2BYTE(REG_MOD_BK00_56_L, 0xFFFF);
            MOD_W2BYTE(REG_MOD_BK00_57_L, 0xFFFF);
            MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFFFF);
        }
        else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_1LANE)
               ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_1LANE))
        {
            MOD_W2BYTE(REG_MOD_BK00_55_L, 0xFFF0);
            MOD_W2BYTE(REG_MOD_BK00_56_L, 0xFFF0);
            MOD_W2BYTE(REG_MOD_BK00_57_L, 0xFFFF);
            MOD_W2BYTE(REG_MOD_BK00_58_L, 0xFFFF);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_55_L, 0x76DC);
            MOD_W2BYTE(REG_MOD_BK00_56_L, 0xBA98);
            MOD_W2BYTE(REG_MOD_BK00_57_L, 0x3210);
            MOD_W2BYTE(REG_MOD_BK00_58_L, 0x0054);
        }
    }

}

void MHal_PQ_Clock_Gen_For_Gamma(void *pInstance)
{
    W2BYTEMSK(REG_CLKGEN0_52_L, 0x00, 0x07);
}

void MHal_VOP_SetGammaMappingMode(void *pInstance, MS_U8 u8Mapping)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if(u8Mapping & GAMMA_MAPPING)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_74_L, (u8Mapping & GAMMA_10BIT_MAPPING)? BIT(15):0, BIT(15));
    }
    else
    {
        PNL_ASSERT(0, "Invalid eSupportGammaMapMode [%d] Passed to [%s], please make sure the u8Mapping[%d] is valid\n.",
                       u8Mapping, __FUNCTION__, u8Mapping);
    }
}

MS_BOOL Hal_VOP_Is_GammaMappingMode_enable(void *pInstance)
{
    // Only support 1024 entry
    return TRUE;
}

// After A5, 8 bit mode only support burst write!!!
MS_BOOL Hal_VOP_Is_GammaSupportSignalWrite(void *pInstance, DRVPNL_GAMMA_MAPPEING_MODE u8Mapping)
{
    if( u8Mapping == E_DRVPNL_GAMMA_10BIT_MAPPING )
        return TRUE;
    else
        return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Gamma format (12 bit LUT)
//      0, 1, 2, 3, ..., NumOfLevel, totally N Sets of tNormalGammaR/G/B[],
//      1 set uses 2 bytes of memory.
//
// [T2 and before ] N = 256
// [T3]             N = 256 or 1024
// ______________________________________________________________________________
//  Byte | 0         1           2               n-1        n
//    [G1|G0]       [G0]       [G1] . ...... .  [Gmax]    [Gmax]
//    3:0  3:0      11:4       11:4              3:0       11:4
//
#ifdef MONACO_SC2
void Hal_PNL_Set12BitGammaPerChannel_SC2(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode )
{
    MS_U16 u16Addr             = 0;
    MS_U16 u16CodeTableIndex  = u16Addr/2*3;
    MS_U16 u16GammaValue      = 0;
    MS_U16 u16MaxGammaValue   = 0;
    MS_U16 u16NumOfLevel = GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING ? 256 : 1024;
    MS_BOOL bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance,GammaMapMode);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // Go to burst write if not support
    if ( bUsingBurstWrite )
    {
        // 1.   initial burst write address, LUT_ADDR[7:0]
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6C_L, 0x00 , 0x3FF);

        // 2.   select burst write channel, REG_LUT_BW_CH_SEL[1:0]
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, 0x00 , BIT(3) | BIT(2) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, BIT(2) , BIT(3) | BIT(2) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, BIT(3) , BIT(3) | BIT(2) );
                break;
        }

        // 3.   enable burst write mode, REG_LUT_BW_MAIN_EN
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, BIT(0) , BIT(0)); // Burst write enable

    }

    //printf("\33[0;31m Gamma Mapping mode %d \n \33[m",GammaMapMode );
    // write gamma table per one channel
    for(; u16Addr < u16NumOfLevel; u16CodeTableIndex += 3)
    {
        // gamma x
        u16GammaValue = u8Tab[u16CodeTableIndex] & 0x0F;
        u16GammaValue |= u8Tab[u16CodeTableIndex+1] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA,"Gamma x: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x, GammaLvl=%d\n",
                                    u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+1, u8Tab[u16CodeTableIndex+1], u16GammaValue, u16NumOfLevel);

        if(u16MaxGammaValue < u16GammaValue)
        {
            u16MaxGammaValue = u16GammaValue;
        }

        // write gamma value
        hal_PNL_WriteGamma12Bit(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;

        // gamma x+1
        u16GammaValue = (u8Tab[u16CodeTableIndex] & 0xF0) >> 4;
        u16GammaValue |= u8Tab[u16CodeTableIndex+2] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA, "Gamma x+1: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x\n", u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+2, u8Tab[u16CodeTableIndex+2], u16GammaValue);

        if(u16MaxGammaValue < u16GammaValue)
            {
            u16MaxGammaValue = u16GammaValue;
            }

        // write gamma value
        hal_PNL_WriteGamma12Bit(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;
    }

    if ( bUsingBurstWrite )
    {
        // 5.   after finish burst write data of one channel, disable burst write mode
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, 0x00 , BIT(0));
    }

    hal_PNL_SetMaxGammaValue(pInstance,u8Channel, u16MaxGammaValue);
}
#endif

#ifdef USE_PANEL_GAMMA
static void _hal_PNL_WriteGamma12Bit_PanelGamma(void *pInstance, MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_U16 u16Delay = 0xFFFF;

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Write [ch %d][addr 0x%x]: 0x%x \n", u8Channel, u16Addr, u16GammaValue);

    if (!bBurstWrite )
    {
        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, 0x0C) && (--u16Delay));          // Check whether the Write chanel is ready
        PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_1B_L, u16Addr, 0xFF);                          // set address port
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK24_1D_L + u8Channel *2), u16GammaValue, 0xFFF);      // Set channel data

        // kick off write
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, 0x00 , BIT(3) | BIT(2) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, BIT(2) , BIT(3) | BIT(2) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, BIT(3) , BIT(3) | BIT(2) );
                break;

        }

        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_1C_L, 0x0C) && (--u16Delay));          // Check whether the Write chanel is ready
    }
    else
    {

        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1D_L, u16GammaValue, 0xFFF);
        SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK00_7F_L, 0x00); // make little time delay
    }


    PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");
}

static void _hal_PNL_SetMaxGammaValue_PanelGamma(void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Max gamma of SC%tu %d is 0x%x\n", (ptrdiff_t)pPNLInstancePrivate->u32DeviceID, u8Channel, u16MaxGammaValue);
        switch(u8Channel)
        {
            case 0:  // max. Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_2C_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                break;

            case 1:  // max. Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_2E_L , u16MaxGammaValue, 0xFFF);           // max. base 1
                break;

            case 2:  //max.  Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_30_L , u16MaxGammaValue, 0xFFF);           // max. base 1
                break;
           }
}

static void _Hal_PNL_Set12BitGammaPerChannel_PanelGamma(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode )
{
    MS_U16 u16Addr             = 0;
    MS_U16 u16CodeTableIndex  = u16Addr/2*3;
    MS_U16 u16GammaValue      = 0;
    MS_U16 u16MaxGammaValue   = 0;
    MS_U16 u16NumOfLevel = GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING ? 256 : 1024;
    MS_BOOL bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance,GammaMapMode);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // Go to burst write if not support
    if ( bUsingBurstWrite )
    {
        // 1.   initial burst write address, LUT_ADDR[7:0]
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1B_L, 0x00 , 0xFF);

        // 2.   select burst write channel, REG_LUT_BW_CH_SEL[1:0]
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, 0x00 , BIT(3) | BIT(2) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, BIT(2) , BIT(3) | BIT(2) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, BIT(3) , BIT(3) | BIT(2) );
                break;
        }

        // 3.   enable burst write mode, REG_LUT_BW_MAIN_EN
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, BIT(0) , BIT(0)); // Burst write enable

    }

    //printf("\33[0;31m Gamma Mapping mode %d \n \33[m",GammaMapMode );
    // write gamma table per one channel
    for(; u16Addr < u16NumOfLevel; u16CodeTableIndex += 3)
    {
        // gamma x
        u16GammaValue = u8Tab[u16CodeTableIndex] & 0x0F;
        u16GammaValue |= u8Tab[u16CodeTableIndex+1] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA,"Gamma x: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x, GammaLvl=%d\n",
                                    u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+1, u8Tab[u16CodeTableIndex+1], u16GammaValue, u16NumOfLevel);

        if(u16MaxGammaValue < u16GammaValue)
        {
            u16MaxGammaValue = u16GammaValue;
        }

        // write gamma value
        _hal_PNL_WriteGamma12Bit_PanelGamma(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;

        // gamma x+1
        u16GammaValue = (u8Tab[u16CodeTableIndex] & 0xF0) >> 4;
        u16GammaValue |= u8Tab[u16CodeTableIndex+2] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA, "Gamma x+1: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x\n", u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+2, u8Tab[u16CodeTableIndex+2], u16GammaValue);

        if(u16MaxGammaValue < u16GammaValue)
            {
            u16MaxGammaValue = u16GammaValue;
            }

        // write gamma value
        _hal_PNL_WriteGamma12Bit_PanelGamma(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;
    }

    if ( bUsingBurstWrite )
    {
        // 5.   after finish burst write data of one channel, disable burst write mode
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK24_1C_L, 0x00 , BIT(0));
    }
    _hal_PNL_SetMaxGammaValue_PanelGamma(pInstance,u8Channel, u16MaxGammaValue);
}

#endif

static void _hal_PNL_WriteGamma12Bit(void *pInstance, MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_U16 u16Delay = 0xFFFF;

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Write [ch %d][addr 0x%x]: 0x%x \n", u8Channel, u16Addr, u16GammaValue);

    if (!bBurstWrite )
    {
        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_6D_L, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
        PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");

        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6C_L, u16Addr, 0x3FF);                          // set address port
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_6E_L + u8Channel *2), u16GammaValue, 0xFFF);      // Set channel data

        // kick off write
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, BIT(5), BIT(5));
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, BIT(6), BIT(6));
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, BIT(7), BIT(7));
                break;
        }

        while (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_6D_L, 0xE0) && (--u16Delay));          // Check whether the Write chanel is ready
    }
    else
    {

        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_72_L, u16GammaValue, 0xFFF);
        SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK00_7F_L, 0x00); // make little time delay
    }


    PNL_ASSERT(u16Delay > 0, "%s\n", "WriteGamma timeout");
}


static void _hal_PNL_SetMaxGammaValue(void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_GAMMA, "Max gamma of SC%tu %d is 0x%x\n", (ptrdiff_t)pPNLInstancePrivate->u32DeviceID, u8Channel, u16MaxGammaValue);
#ifdef MONACO_SC2
    if(pPNLInstancePrivate->u32DeviceID == 0)
    {
#endif
        switch(u8Channel)
        {
            case 0:  // max. Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_7D_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                break;

            case 1:  // max. Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_7E_L , u16MaxGammaValue, 0xFFF);           // max. base 1
                break;

            case 2:  //max.  Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_7F_L , u16MaxGammaValue, 0xFFF);           // max. base 1
                break;
           }
#ifdef MONACO_SC2
    }else    //Nike
    {
    switch(u8Channel)
    {
        case 0:  // max. Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7A_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7B_L , u16MaxGammaValue, 0xFFF);           // max. base 1
            break;

        case 1:  // max. Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7C_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7D_L , u16MaxGammaValue, 0xFFF);           // max. base 1
            break;

        case 2:  //max.  Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7E_L , u16MaxGammaValue, 0xFFF);           // max. base 0
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_7F_L , u16MaxGammaValue, 0xFFF);           // max. base 1
            break;
     }

    }
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Gamma format (12 bit LUT)
//      0, 1, 2, 3, ..., NumOfLevel, totally N Sets of tNormalGammaR/G/B[],
//      1 set uses 2 bytes of memory.
//
// [T2 and before ] N = 256
// [T3]             N = 256 or 1024
// ______________________________________________________________________________
//  Byte | 0         1           2               n-1        n
//    [G1|G0]       [G0]       [G1] . ...... .  [Gmax]    [Gmax]
//    3:0  3:0      11:4       11:4              3:0       11:4
//
static void _Hal_PNL_Set12BitGammaPerChannel(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_U16 u16Addr            = 0;
    MS_U16 u16CodeTableIndex  = u16Addr/2*3;
    MS_U16 u16GammaValue      = 0;
    MS_U16 u16MaxGammaValue   = 0;
    MS_U16 u16NumOfLevel = GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING ? 256 : 1024;
    MS_BOOL bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance,GammaMapMode);

    // Go to burst write if not support
    if ( bUsingBurstWrite )
    {
        // 1.   initial burst write address, LUT_ADDR[7:0]
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_71_L, 0x00 , 0x3FF);

        // 2.   select burst write channel, REG_LUT_BW_CH_SEL[1:0]
        switch(u8Channel)
        {
            case 0:  // Red
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, 0x00 , BIT(6) | BIT(5) );
                break;

            case 1:  // Green
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, BIT(5) , BIT(6) | BIT(5) );
                break;

            case 2:  // Blue
                SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, BIT(6) , BIT(6) | BIT(5) );
                break;
        }

        // 3.   enable burst write mode, REG_LUT_BW_MAIN_EN
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, BIT(7) , BIT(7)); // Burst write enable

    }

    //printf("\33[0;31m Gamma Mapping mode %d \n \33[m",GammaMapMode );
    // write gamma table per one channel
    for(; u16Addr < u16NumOfLevel; u16CodeTableIndex += 3)
    {
        // gamma x
        u16GammaValue = u8Tab[u16CodeTableIndex] & 0x0F;
        u16GammaValue |= u8Tab[u16CodeTableIndex+1] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA,"Gamma x: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x, GammaLvl=%d\n",
                                    u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+1, u8Tab[u16CodeTableIndex+1], u16GammaValue, u16NumOfLevel);

        if(u16MaxGammaValue < u16GammaValue)
        {
            u16MaxGammaValue = u16GammaValue;
        }

        // write gamma value
        _hal_PNL_WriteGamma12Bit(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;

        // gamma x+1
        u16GammaValue = (u8Tab[u16CodeTableIndex] & 0xF0) >> 4;
        u16GammaValue |= u8Tab[u16CodeTableIndex+2] << 4;

        PNL_DBG(PNL_DBGLEVEL_GAMMA, "Gamma x+1: SrcGTbl[%d] = 0x%x, [%d] = 0x%x, Gamma = 0x%x\n", u16CodeTableIndex, u8Tab[u16CodeTableIndex], u16CodeTableIndex+2, u8Tab[u16CodeTableIndex+2], u16GammaValue);

        if(u16MaxGammaValue < u16GammaValue)
        {
            u16MaxGammaValue = u16GammaValue;
        }

        // write gamma value
        _hal_PNL_WriteGamma12Bit(pInstance,u8Channel,bUsingBurstWrite, u16Addr, u16GammaValue);
        u16Addr++;
    }

    if ( bUsingBurstWrite )
    {
        // 5.   after finish burst write data of one channel, disable burst write mode
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK10_70_L, 0x00 , BIT(7));
    }

    _hal_PNL_SetMaxGammaValue(pInstance,u8Channel, u16MaxGammaValue);
}

void hal_PNL_WriteGamma12Bit(void *pInstance, MS_U8 u8Channel, MS_BOOL bBurstWrite, MS_U16 u16Addr, MS_U16 u16GammaValue)
{
    #ifdef USE_PANEL_GAMMA
        _hal_PNL_WriteGamma12Bit_PanelGamma(pInstance, u8Channel, bBurstWrite, u16Addr, u16GammaValue);
    #endif
    _hal_PNL_WriteGamma12Bit(pInstance, u8Channel, bBurstWrite, u16Addr, u16GammaValue);

}

void hal_PNL_SetMaxGammaValue(void *pInstance, MS_U8 u8Channel, MS_U16 u16MaxGammaValue)
{
    #ifdef USE_PANEL_GAMMA
        _hal_PNL_SetMaxGammaValue_PanelGamma(pInstance, u8Channel, u16MaxGammaValue);
    #endif
    _hal_PNL_SetMaxGammaValue(pInstance, u8Channel, u16MaxGammaValue);
}

void Hal_PNL_Set12BitGammaPerChannel(void *pInstance, MS_U8 u8Channel, MS_U8 * u8Tab, DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode)
{
    #ifdef USE_PANEL_GAMMA
        _Hal_PNL_Set12BitGammaPerChannel_PanelGamma(pInstance, u8Channel, u8Tab, GammaMapMode);
    #endif
    _Hal_PNL_Set12BitGammaPerChannel(pInstance, u8Channel, u8Tab, GammaMapMode);
}

// src : 1 (scaler lpll)
// src : 0 (frc lpll)
MS_U8 MHal_PNL_FRC_lpll_src_sel(void *pInstance, MS_U8 u8src)
{
    if (u8src > 1)
    {
        return FALSE;
    }
    else
    {
//Not support two LPLL (frc lpll) for Manhattan
#if 0
        W2BYTEMSK(L_BK_LPLL(0x00), 0x00, 0x0F);
        W2BYTEMSK(L_BK_LPLL(0x7F), u8src?BIT(8):0, BIT(8));

        if(u8src==0)
        {
            W2BYTEMSK(L_BK_LPLL(0x00), 0x01, 0x0F);
            W2BYTEMSK(L_BK_LPLL(0x7F), BIT(8), BIT(8));
            W2BYTEMSK(L_BK_LPLL(0x00), 0x00, 0x0F); // restore to sub bnak 0
        }
#endif
        return TRUE;
    }

}

static MS_U8 _MHal_PNL_GetSupportedLPLLIndex(void *pInstance,
                                                                 PNL_TYPE eLPLL_Type,
                                                                 PNL_MODE eLPLL_Mode,
                                                                 MS_U64 ldHz, PNL_LPLL_TYPE_SEL lpll_type_sel)
{
    MS_U8 u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%lu, lpll_type_sel=%u\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz, lpll_type_sel);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%llu, lpll_type_sel=%u\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz, lpll_type_sel);
#endif

    /// Mini LVDS, EPI34/28, LVDS_1CH, Vx1_1P are 1P structure
    if(!((eLPLL_Type == E_PNL_TYPE_TTL)||
        ((eLPLL_Type == E_PNL_TYPE_LVDS)&&(eLPLL_Mode==E_PNL_MODE_SINGLE))||
        ((eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&(eLPLL_Mode==E_PNL_MODE_SINGLE))||
        (eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_1LANE)||(eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_1LANE)||
        ((eLPLL_Type >= E_PNL_LPLL_MINILVDS_2CH_3P_8BIT)&&(eLPLL_Type <= E_PNL_LPLL_MINILVDS_1CH_6P_6BIT))||
        ((eLPLL_Type >= E_PNL_LPLL_EPI34_2P)&&(eLPLL_Type <= E_PNL_LPLL_EPI28_4P))))
    {
        ldHz/=2;
    }

    switch(lpll_type_sel)
    {
        default:
        case E_PNL_LPLL_VIDEO:
        {
            switch (eLPLL_Type)
            {
                case E_PNL_TYPE_TTL:
                    if (ldHz < 250000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_25to25MHz;
                    }
                    else if ((ldHz >= 250000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_25to50MHz;
                    }
                    else if((ldHz >= 500000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_50to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_TTL_100to150MHz;
                    }
                break;

                case E_PNL_TYPE_LVDS:
                    switch (eLPLL_Mode)
                    {
                        case E_PNL_MODE_SINGLE:
                            if (ldHz < 500000000UL)
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz;
                            }
                        break;

                        default:
                        case E_PNL_MODE_DUAL:
                            if (ldHz < 250000000UL)
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz;
                            }
                            else if ((ldHz >= 250000000UL) && (ldHz < 500000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to80MHz;
                            }
                        break;
                    }
                break;

                case E_PNL_TYPE_HS_LVDS:

                    switch (eLPLL_Mode)
                    {
                        case E_PNL_MODE_SINGLE:
                            if(ldHz < 500000000UL)
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz;
                            }
                            else if((ldHz >= 500000000UL) && (ldHz < 1000000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to100MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz;
                            }
                        break;

                        default:
                        case E_PNL_MODE_DUAL:
                            if(ldHz < 250000000UL)
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz;
                            }
                            else if((ldHz >= 250000000UL) && (ldHz < 500000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to50MHz;
                            }
                            else if((ldHz >= 500000000UL) && (ldHz < 1000000000UL))
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_50to100MHz;
                            }
                            else
                            {
                                u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz;
                            }
                        break;
                    }
                break;
///Not Support
#if 0
                case E_PNL_LPLL_MINILVDS_1CH_3P_8BIT:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_8BIT_50to80MHz;
                break;

                case E_PNL_LPLL_MINILVDS_2CH_3P_8BIT:
                    if((ldHz >= 500000000) && (ldHz < 1000000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_50to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_100to150MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_2CH_6P_8BIT:
                    if((ldHz >= 500000000) && (ldHz < 1000000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_50to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_100to150MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_1CH_3P_6BIT:
                    if((ldHz >= 500000000) && (ldHz < 666700000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_50to66_67MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_66_67to80MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_2CH_3P_6BIT:
                    if ((ldHz <= 500000000) && (ldHz < 666700000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_50to66_67MHz;
                    }
                    else if((ldHz >= 666700000) && (ldHz < 1333300000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_66_67to133_33MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_133_33to150MHz;
                    }
                break;

                case E_PNL_LPLL_MINILVDS_2CH_6P_6BIT:
                    if ((ldHz <= 500000000) && (ldHz < 670000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_50to67MHz;
                    }
                    else if((ldHz >= 670000000) && (ldHz < 1330000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_67to133MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_133to150MHz;
                    }
                break;

                case E_PNL_LPLL_EPI34_4P:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_4PAIR_95to150MHz;
                break;

                case E_PNL_LPLL_EPI34_6P:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_6PAIR_80to150MHz;
                break;

                case E_PNL_LPLL_EPI34_8P:
                    if((ldHz >= 800000000) && (ldHz < 940000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_80to94MHz;
                    }
                    else if((ldHz >= 940000000) && (ldHz < 1880000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_94to188MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_188to300MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_4P:
                    if((ldHz >= 800000000) && (ldHz < 1140000000))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_80to114MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_114to150MHz;
                    }
                break;
#endif

                case E_PNL_LPLL_EPI28_6P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_6P_150to150MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 1800000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_6P_150to180MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_6P_180to330MHz;
                    }
                break;

                case E_PNL_LPLL_EPI28_8P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8P_150to150MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2400000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8P_150to240MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_8P_240to330MHz;
                    }
                break;
                case E_PNL_LPLL_EPI24_12P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_24_12P_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_24_12P_150to330MHz;
                    }
                break;
                case E_PNL_LPLL_EPI28_12P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_12P_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EPI_28_12P_150to330MHz;
                    }
                break;
/*
                case E_PNL_LPLL_EPI34_12P:
                case E_PNL_LPLL_EPI24_12P:
                break;
*/

                case E_PNL_LPLL_VBY1_10BIT_8LANE:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_8LANE_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_8LANE_150to300MHz;
                    }
                    printf("@@11=%u\n",u8SupportedLPLLIndex);
                break;

                case E_PNL_LPLL_VBY1_10BIT_4LANE:
                    if(ldHz < 750000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_4LANE_75to75MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_4LANE_75to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_2LANE:
                    if(ldHz < 375000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_2LANE_37_5to37_5MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_2LANE_37_5to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_1LANE:
                    if(ldHz < 400000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_1LANE_40to40MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_1LANE_40to80MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_8LANE:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_8LANE_150to150MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2200000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_8LANE_150to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_8LANE_200to300MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_4LANE:
                    if(ldHz < 750000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_4LANE_75to75MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_4LANE_75to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_4LANE_100to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_2LANE:
                    if(ldHz < 375000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_2LANE_37_5to37_5MHz;
                    }
                    else if((ldHz >= 375000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_2LANE_37_5to50MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_2LANE_50to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_1LANE:
                    if(ldHz < 400000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_1LANE_40to40MHz;
                    }
                    else if((ldHz >= 400000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_1LANE_40to50MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_1LANE_50to80MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_16LANE:
                    if(ldHz < 2000000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_16LANE_200to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_10BIT_16LANE_200to300MHz;
                    }
                break;
                case E_PNL_LPLL_VBY1_8BIT_16LANE:
                    if(ldHz < 2000000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_16LANE_200to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_VBY1_8BIT_16LANE_200to300MHz;
                    }
                break;
                case E_PNL_LPLL_USI_T_8BIT_12P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to330MHz;
                    }
                break;
                case E_PNL_LPLL_USI_T_10BIT_12P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to330MHz;
                    }
                break;
                case E_PNL_LPLL_ISP_8BIT_12P:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to330MHz;
                    }
                break;
                case E_PNL_LPLL_ISP_8BIT_6P_D:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to330MHz;
                    }
                break;
                default:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
                break;
            }
        }
        break;
        case E_PNL_LPLL_OSD:
        {
            switch (eLPLL_Type)
            {
                case E_PNL_TYPE_HS_LVDS:
                {
                    if(ldHz < 250000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to25MHz;
                    }
                    else if((ldHz >= 250000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_25to50MHz;
                    }
                    else if((ldHz >= 500000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_50to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1CH_100to150MHz;
                    }
                }
                break;

                case E_PNL_LPLL_VBY1_10BIT_4LANE:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to150MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_4LANE_150to340MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_2LANE:
                    if(ldHz < 750000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to75MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_2LANE_75to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_10BIT_1LANE:
                    if(ldHz < 375000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to37_5MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_10BIT_1LANE_37_5to75MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_4LANE:
                    if(ldHz < 1500000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to150MHz;
                    }
                    else if((ldHz >= 1500000000UL) && (ldHz < 2200000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_150to200MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_4LANE_200to340MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_2LANE:
                    if(ldHz < 750000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to75MHz;
                    }
                    else if((ldHz >= 750000000UL) && (ldHz < 1000000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_75to100MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_2LANE_100to150MHz;
                    }
                break;

                case E_PNL_LPLL_VBY1_8BIT_1LANE:
                    if(ldHz < 375000000UL)
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to37_5MHz;
                    }
                    else if((ldHz >= 380000000UL) && (ldHz < 500000000UL))
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_37_5to50MHz;
                    }
                    else
                    {
                        u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_VBY1_8BIT_1LANE_50to75MHz;
                    }
                break;

                default:
                    u8SupportedLPLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;
                break;
            }
        }
        break;
    }
    return u8SupportedLPLLIndex;
}

static void _MHal_PNL_DumpLPLLTable(void *pInstance, MS_U8 LPLLTblIndex, PNL_LPLL_TYPE_SEL lpll_type_sel)
{
    if(lpll_type_sel == E_PNL_LPLL_VIDEO)
    {
        if (LPLLTblIndex == E_PNL_SUPPORTED_LPLL_MAX)
        {
            printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
            return;
        }

        int indexCounter = 0;

        for(indexCounter = 0 ; indexCounter<LPLL_REG_NUM; indexCounter++)
        {
            if (LPLLSettingTBL[LPLLTblIndex][indexCounter].address == 0xFF) //delay in micro second
            {
                MsOS_DelayTaskUs(LPLLSettingTBL[LPLLTblIndex][indexCounter].value);
                continue; // step forward to next register setting.
            }

            W2BYTEMSK(L_BK_LPLL(LPLLSettingTBL[LPLLTblIndex][indexCounter].address),
                      LPLLSettingTBL[LPLLTblIndex][indexCounter].value,
                      LPLLSettingTBL[LPLLTblIndex][indexCounter].mask);
        }
        PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, LPLLTblIndex);
    }
    else
    {
        if (LPLLTblIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
        {
            printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
            return;
        }

        int indexCounter = 0;

        for(indexCounter = 0 ; indexCounter<LPLL_EXT_REG_NUM; indexCounter++)
        {
            if (LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].address == 0xFF) //delay in micro second
            {
                MsOS_DelayTaskUs(LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].value);
                continue; // step forward to next register setting.
            }

            W2BYTEMSK(L_BK_LPLL(LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].address),
                      LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].value,
                      LPLLSettingTBL_Ext[LPLLTblIndex][indexCounter].mask);
        }
        PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, LPLLTblIndex);
    }
}

void MHal_PNL_Init_LPLL(void *pInstance, PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz, E_PNL_LPLL_VIDEO);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("Not Supported LPLL Type, skip LPLL Init\n");
        return;
    }

    _MHal_PNL_DumpLPLLTable(pInstance, u8SupportedLPLLLIndex, E_PNL_LPLL_VIDEO);

    //for Maserati sync too fat, so frame lock fail
    W2BYTEMSK(L_BK_LPLL(0x3C),0x0000 ,BIT(12));

    MHal_MOD_PVDD_Power_Setting(pInstance, pPNLResourcePrivate->sthalPNL._bPVDD_2V5); // Einstein is always use 3.3V PVDD Power.
}

MS_U8 MHal_PNL_Get_Loop_DIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_div = 0;
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, u8LPLL_Mode=%u, ldHz=%lu\n", __FUNCTION__, __LINE__, eLPLL_Type, u8LPLL_Mode, ldHz);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, u8LPLL_Mode=%u, ldHz=%llu\n", __FUNCTION__, __LINE__, eLPLL_Type, u8LPLL_Mode, ldHz);
#endif
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,u8LPLL_Mode,ldHz,E_PNL_LPLL_VIDEO);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("[%s,%5d] Error LPLL type\n",__FUNCTION__,__LINE__);
        u16loop_div = 0 ;
    }
    else
    {
        u16loop_div = u16LoopDiv[u8SupportedLPLLLIndex];
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "E_PNL_LPLL_VIDEO : u16loop_div=%u\n", u16loop_div);

    u16loop_div *= 2;
    return u16loop_div;
}

MS_U16 MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_gain = 0;
    E_PNL_SUPPORTED_LPLL_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_MAX;
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%lu\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]E_PNL_LPLL_VIDEO : eLPLL_Type=%u, eLPLL_Mode=%u, ldHz=%llu\n", __FUNCTION__, __LINE__, eLPLL_Type, eLPLL_Mode, ldHz);
#endif
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz,E_PNL_LPLL_VIDEO);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_MAX)
    {
        printf("[%s,%5d] Error LPLL type\n",__FUNCTION__,__LINE__);
        u16loop_gain = 0 ;
    }
    else
    {
        u16loop_gain = u16LoopGain[u8SupportedLPLLLIndex];
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "E_PNL_LPLL_VIDEO : u16loop_gain=%u\n", u16loop_gain);
    return u16loop_gain;
}

#define SKIP_TIMING_CHANGE_CAP  TRUE
MS_BOOL Hal_PNL_SkipTimingChange_GetCaps(void *pInstance)
{
    #if (SKIP_TIMING_CHANGE_CAP)
        return TRUE;
    #else
        return FALSE;
    #endif
}

void MHal_PNL_PreSetModeOn(void *pInstance, MS_BOOL bSetMode)
{
    if (bSetMode == TRUE)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_77_L, 0, BIT(15)); //software reset, 0:reset
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_77_L, BIT(15), BIT(15));
    }
}

void MHal_PNL_HWLVDSReservedtoLRFlag(void *pInstance, PNL_DrvHW_LVDSResInfo lvdsresinfo)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if (lvdsresinfo.bEnable)
    {
        if (lvdsresinfo.u16channel & BIT(0))  // Channel A
        {
            if (lvdsresinfo.u32pair & BIT(3))  // pair 3
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_04_L, BIT(15), BIT(15));
            }
            if (lvdsresinfo.u32pair & BIT(4))  // pair 4
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_04_L, BIT(14), BIT(14));
            }
        }
        if (lvdsresinfo.u16channel & BIT(1))  // Channel B
        {
            if (lvdsresinfo.u32pair & BIT(3))  // pair 3
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_04_L, BIT(13), BIT(13));
            }
            if (lvdsresinfo.u32pair & BIT(4))  // pair 4
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_04_L, BIT(12), BIT(12));
            }
        }

        if(  IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_54_L, BIT(10), BIT(10)); //reg_sel_ext_bit: sel extend bit, 0: osd_de 1: three_d_flag
        }
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_04_L, 0x0000, (BIT(15) | BIT(14) | BIT(13) | BIT(12)));

        if(  IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x00, BIT(10)); //reg_sel_ext_bit: sel extend bit, 0: osd_de 1: three_d_flag
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Turn OD function
////////////////////////////////////////////////////////////////////////
void MHal_PNL_OverDriver_Init(void *pInstance, MS_PHY u32OD_MSB_Addr, MS_PHY u32OD_MSB_limit, MS_PHY u32OD_LSB_Addr, MS_PHY u32OD_LSB_limit, MS_U8 u8MIUSel)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK16_61_L,u8MIUSel<<8,BIT(8)|BIT(9)); // OD MIU select

    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_15_L, (MS_U16)(u32OD_MSB_Addr & 0xFFFF)); // OD MSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_16_L, (MS_U16)((u32OD_MSB_Addr >> 16) & 0x00FF), 0x00FF); // OD MSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_60_L, (MS_U16)((u32OD_MSB_Addr >> 24) & 0x0003), 0x0003); // OD MSB request base address
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_17_L, (MS_U16)(u32OD_MSB_limit & 0xFFFF)); // OD MSB request address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_18_L, (MS_U16)((u32OD_MSB_limit >> 16) & 0x00FF), 0x00FF); // OD MSB request address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_60_L, (MS_U16)((u32OD_MSB_limit >> 24) & 0x0003)<<2, 0x000C); // OD MSB request address limit
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_39_L, (MS_U16)(u32OD_LSB_limit & 0xFFFF)); // OD frame buffer write address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3A_L, (MS_U16)((u32OD_LSB_limit >> 16) & 0x00FF), 0x00FF); // OD frame buffer write address limit
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3B_L, (MS_U16)(u32OD_LSB_limit & 0xFFFF)); // OD frame buffer read address limit
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3C_L, (MS_U16)((u32OD_LSB_limit >> 16) & 0x00FF), 0x00FF); // OD frame buffer read address limit
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_4F_L, (MS_U16)(u32OD_LSB_Addr & 0xFFFF)); // OD LSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_50_L, (MS_U16)((u32OD_LSB_Addr >> 16) & 0x00FF), 0x00FF); // OD LSB request base address
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_50_L, (MS_U16)((u32OD_LSB_limit & 0x00FF) << 8), 0xFF00); // OD LSB request limit address
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_51_L, (MS_U16)((u32OD_LSB_limit >> 8) & 0xFFFF)); // OD LSB request limit address

    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_1A_L, 0x4020); // OD request rFIFO limit threshold, priority threshold
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_1C_L, 0x4020); // OD request wFIFO limit threshold, priority threshold
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3A_L, 0x00, BIT(14)); // OD strength gradually bypass
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_3A_L, 0x2F00, 0x3F00);    // OD strength gradually slop
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_12_L, 0x0C, 0xFF);    // OD active threshold

}

void MHal_PNL_OverDriver_Enable(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // When OD enable, disable OD/RGBW SRAM PowerDown.
#ifdef CONFIG_MSTAR_SRAMPD
    if(pPNLInstancePrivate->u32DeviceID == 0)
    {
        if(bEnable)
        {
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_10_L, 0, BIT(0));   //OD SRAM PD Enable  : SC_SPD_BK3F_10[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, BIT(8), BIT(8));        //OD Clock gate : ~SC_SPD_BK3F_50[8]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(15)); //OD Bypass Enable :SC_OD_BK16_6F[15]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_10_L, 0, BIT(1));   // RGBW SRAM PD Enable : SC_SPD_BK3F_10[1]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, BIT(9), BIT(9));        // RGBW Clock Gate : ~SC_SPD_BK3F_50[9]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(12)); // RGBW bypass enable :SC_OD_BK16_6F[12]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, BIT(13), BIT(13));       // RGBW bypass enable : ~SC_OD_BK16_6F[13]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_17_L, 0, BIT(0));   //M+ SRAM PD Enable  : SC_SPD_BK3F_17[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, BIT(10), BIT(10));        //M+ Clock gate : ~SC_SPD_BK3F_50[10]
            //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L,0 , BIT(12)); // M+ bypass enable :SC_OD_BK16_6F[12]
            //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(13));       // M+ bypass enable : ~SC_OD_BK16_6F[13]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_0B_L, 0, BIT(4));   //Demura SRAM PD Enable  : SC_SPD_BK3F_0B[4]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK77_26_L, 0, BIT(14));   //Demura Clock Gate  : SC_BK77_26[14]
            W2BYTEMSK(REG_RVD_45_L, 0, BIT(0));                                              //Demura Clock gate : BK100A_CLKGEN2_45[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK77_26_L, 0, BIT(15));   //Demura Clock Gate  : SC_BK77_26[15]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_0B_L, 0, BIT(5));   //DGA_GAMMA SRAM PD Enable  : SC_SPD_BK3F_0B[5]
        }
        else
        {
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_10_L, BIT(0), BIT(0));   //OD SRAM PD Enable  : SC_SPD_BK3F_10[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, 0, BIT(8));        //OD Clock gate : ~SC_SPD_BK3F_50[8]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, BIT(15), BIT(15)); //OD Bypass Enable :SC_OD_BK16_6F[15]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_10_L, BIT(1), BIT(1));   // RGBW SRAM PD Enable : SC_SPD_BK3F_10[1]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, 0, BIT(9));        // RGBW Clock Gate : ~SC_SPD_BK3F_50[9]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, BIT(12), BIT(12)); // RGBW bypass enable :SC_OD_BK16_6F[12]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(13));       // RGBW bypass enable : ~SC_OD_BK16_6F[13]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_17_L, BIT(0), BIT(0));   //M+ SRAM PD Enable  : SC_SPD_BK3F_17[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, 0, BIT(10));        //M+ Clock gate : ~SC_SPD_BK3F_50[10]
            //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L,0 , BIT(12)); // M+ bypass enable :SC_OD_BK16_6F[12]
            //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(13));       // M+ bypass enable : ~SC_OD_BK16_6F[13]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_0B_L, BIT(4), BIT(4));   //Demura SRAM PD Enable  : SC_SPD_BK3F_0B[4]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK77_26_L, BIT(14), BIT(14));   //Demura Clock Gate  : SC_BK77_26[14]
            W2BYTEMSK(REG_RVD_45_L, BIT(0), BIT(0));                                              //Demura Clock gate : BK100A_CLKGEN2_45[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK77_26_L, BIT(15), BIT(15));   //Demura Clock Gate  : SC_BK77_26[15]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_0B_L, BIT(5), BIT(5));   //DGA_GAMMA SRAM PD Enable  : SC_SPD_BK3F_0B[5]
        }
    }
#endif

    // OD mode
    // OD used user weight to output blending directly
    // OD Enable
    if (bEnable)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, 0x2D, 0x2F);
    }
    else
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, 0x2C, 0x2F);
    }
}

void MHal_PNL_OverDriver_TBL(void *pInstance, MS_U8 u8ODTbl[1056])
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_U16 i;
    MS_U8 u8target;
    MS_BOOL bEnable;

    bEnable = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, BIT(0));
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, 0x00, BIT(0)); // OD enable
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_01_L, 0x0E, 0x0E); // OD table SRAM enable, RGB channel

    u8target= u8ODTbl[9];
    for (i=0; i<272; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_03_L, (i == 9)?u8target:(u8target ^ u8ODTbl[i]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_02_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_02_L, BIT(15)));
    }

    u8target= u8ODTbl[(272+19)];
    for (i=0; i<272; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_06_L, (i == 19)?u8target:(u8target ^ u8ODTbl[(272+i)]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_05_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_05_L, BIT(15)));
    }

    u8target= u8ODTbl[(272*2+29)];
    for (i=0; i<256; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_09_L, (i == 29)?u8target:(u8target ^ u8ODTbl[(272*2+i)]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_08_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_08_L, BIT(15)));
    }

    u8target= u8ODTbl[(272*2+256+39)];
    for (i=0; i<256; i++)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_0C_L, (i == 39)?u8target:(u8target ^ u8ODTbl[(272*2+256+i)]), 0x00FF);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_0B_L, (i|0x8000), 0x81FF);
        while(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_0D_L, BIT(15)));
    }

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_01_L, 0x00, 0x0E); // OD table SRAM enable, RGB channel
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_10_L, bEnable, BIT(0)); // OD enable
}

MS_U16 _MHal_PNL_MOD_Swing_Refactor_AfterCAL(void *pInstance, MS_U16 u16Swing_Level)
{
    MS_U8 u8ibcal = 0x00;
    MS_U16 u16AfterCal_value = 0;
    MS_U16 u16Cus_value = 0;

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    // =========
    // GCR_CAL_LEVEL[1:0] : REG_MOD_A_BK00_70_L =>
    // 2'b00 250mV ' GCR_ICON_CHx[5:0]=2'h15 (decimal 21)
    // 2'b01 350mV ' GCR_ICON_CHx[5:0]=2'h1F (decimal 31)
    // 2'b10 300mV ' GCR_ICON_CHx[5:0]=2'h1A (decimal 26)
    // 2'b11 200mV ' GCR_ICON_CHx[5:0]=2'h10 (decimal 16)
    // =========
    switch(pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET)
    {
        default:
        case 0:
            u8ibcal = 0x15;
        break;
        case 1:
            u8ibcal = 0x1F;
        break;
        case 2:
            u8ibcal = 0x1A;
        break;
        case 3:
            u8ibcal = 0x10;
        break;
    }
    u16Cus_value = (u16Swing_Level) * (pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE + 4)/(u8ibcal + 4);
    u16AfterCal_value = (u16Cus_value-40)/10+2;

    HAL_MOD_CAL_DBG(printf("\r\n--Swing value after refactor = %d\n", u16AfterCal_value));

    return u16AfterCal_value;
}

MS_U16 MHal_PNL_MODSwingRegToRealLevelValue(void *pInstance, MS_U16 u16SwingRegValue)
{
    MS_U8 u8ibcal = 0x00;
    MS_U16 u16SwingRealLevelValue = 0;
    MS_U16 u16CusValue = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    // =========
    // GCR_CAL_LEVEL[1:0] : REG_MOD_A_BK00_70_L =>
    // 2'b00 250mV ' GCR_ICON_CHx[5:0]=2'h15 (decimal 21)
    // 2'b01 350mV ' GCR_ICON_CHx[5:0]=2'h1F (decimal 31)
    // 2'b10 300mV ' GCR_ICON_CHx[5:0]=2'h1A (decimal 26)
    // 2'b11 200mV ' GCR_ICON_CHx[5:0]=2'h10 (decimal 16)
    // =========
    switch(pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET)
    {
        default:
        case 0:
            u8ibcal = 0x15;
        break;
        case 1:
            u8ibcal = 0x1F;
        break;
        case 2:
            u8ibcal = 0x1A;
        break;
        case 3:
            u8ibcal = 0x10;
        break;
    }

    u16CusValue =  ((u16SwingRegValue-2)*10)+40;
    u16SwingRealLevelValue=(u16CusValue*(u8ibcal + 4))/(pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE + 4);

    HAL_MOD_CAL_DBG(printf("\r\n--Swing Real Level Value = %d\n", u16SwingRealLevelValue));

    return u16SwingRealLevelValue;
}

MS_BOOL MHal_PNL_MOD_Control_Out_Swing(void *pInstance, MS_U16 u16Swing_Level)
{
    MS_BOOL bStatus = FALSE;

    MS_U16 u16ValidSwing = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)||
      (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_HS_LVDS))
    {
        if(u16Swing_Level>600)
            u16Swing_Level=600;
        if(u16Swing_Level<40)
            u16Swing_Level=40;

        u16ValidSwing = _MHal_PNL_MOD_Swing_Refactor_AfterCAL(pInstance, u16Swing_Level);
    }
    else
    {
        u16ValidSwing = u16Swing_Level;
    }

    // Disable HW calibration keep mode first, to make SW icon value can write into register.
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7A_L, 0, BIT(15));

    u16ValidSwing &=0x0F;
    // LVDS fill ICON
    // ch0+ch1+ch2+ch3
    MOD_A_W2BYTE(REG_MOD_A_BK00_50_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
    // ch4+ch5+ch6+ch7
    MOD_A_W2BYTE(REG_MOD_A_BK00_51_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
    // ch8+ch9+ch10+ch11
    MOD_A_W2BYTE(REG_MOD_A_BK00_52_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));
    // ch12+ch13+ch14+ch15
    MOD_A_W2BYTE(REG_MOD_A_BK00_53_L, (u16ValidSwing << 12 | u16ValidSwing << 8 | u16ValidSwing << 4 | u16ValidSwing));

    bStatus = TRUE;

    return bStatus;
}

////////////////////////////////////////////////////////////////////////
// Turn Pre-Emphasis Current function
////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_PNL_MOD_Control_Out_PE_Current (void *pInstance, MS_U16 u16Current_Level)
{
    MS_BOOL bStatus = FALSE;
    MS_U16 u16ValidCurrent = u16Current_Level & 0x0F;

    MOD_A_W2BYTE(REG_MOD_A_BK00_22_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    MOD_A_W2BYTE(REG_MOD_A_BK00_23_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    MOD_A_W2BYTE(REG_MOD_A_BK00_24_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    MOD_A_W2BYTE(REG_MOD_A_BK00_25_L,
        ( (u16ValidCurrent  ) |(u16ValidCurrent << 4 )|(u16ValidCurrent << 8 )
        |(u16ValidCurrent << 12 )));

    bStatus = TRUE;

    return bStatus;
}

void MHal_PNL_MOD_PECurrent_Setting(void *pInstance, MS_U16 u16Current_Level, MS_U16 u16Channel_Select)
{
    MS_U16 u16ValidCurrent = u16Current_Level & 0x0F;
    MS_U16 u16Ch00_03_mask,u16Ch04_07_mask,u16Ch08_11_mask,u16Ch12_15_mask  = 0;

    u16Ch00_03_mask = (((u16Channel_Select & BIT(0))? 0x000F:0x00)|((u16Channel_Select & BIT(1))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(2))? 0x0F00:0x00)|((u16Channel_Select & BIT(3))? 0xF000:0x00));
    u16Ch04_07_mask = (((u16Channel_Select & BIT(4))? 0x000F:0x00)|((u16Channel_Select & BIT(5))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(6))? 0x0F00:0x00)|((u16Channel_Select & BIT(7))? 0xF000:0x00));
    u16Ch08_11_mask = (((u16Channel_Select & BIT(8))? 0x000F:0x00)|((u16Channel_Select & BIT(9))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(10))? 0x0F00:0x00)|((u16Channel_Select & BIT(11))? 0xF000:0x00));
    u16Ch12_15_mask = (((u16Channel_Select & BIT(12))? 0x000F:0x00)|((u16Channel_Select & BIT(13))? 0x00F0:0x00)
                     |((u16Channel_Select & BIT(14))? 0x0F00:0x00)|((u16Channel_Select & BIT(15))? 0xF000:0x00));

    if(u16Ch00_03_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_22_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch00_03_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_22_L,0x00);
    }

    if(u16Ch04_07_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_23_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch04_07_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_23_L,0x00);
    }

    if(u16Ch08_11_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_24_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch08_11_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_24_L,0x00);
    }

    if(u16Ch12_15_mask)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_25_L,
            ((u16ValidCurrent)|(u16ValidCurrent << 4)|(u16ValidCurrent << 8)|(u16ValidCurrent << 12 )), u16Ch12_15_mask);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_25_L,0x00);
    }
}

////////////////////////////////////////////////////////////////////////
// 1.Turn TTL low-power mode function
// 2.Turn internal termination function
// 3.Turn DRIVER BIAS OP function
////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (void *pInstance, MS_BOOL bEnble)
{
    MS_BOOL bStatus = FALSE;
    if(bEnble)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_0A_L, 0xFFFF, 0xFFFF); //Enable TTL low-power mode

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_06_L, 0xFFFF, 0xFFFF); //GCR_EN_RINT (internal termination open)

        //MOD_W2BYTEMSK(REG_MOD_BK00_76_L, 0x003F, 0x003F);// can not find in Manhattan register table

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_08_L, 0xFFFF, 0xFFFF); //Disable DRIVER BIAS OP
        MOD_W2BYTEMSK(REG_MOD_BK00_5E_L, 0x003F, 0x003F);
    }
    else
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_34_L, 0x0000, 0xFFFF); //Disable TTL low-power mode
//        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, 0x0000, 0x001E);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_06_L, 0x0000, 0xFFFF); //GCR_EN_RINT (internal termination close)
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, 0x0000, 0x003F);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_08_L, 0x0000, 0xFFFF); //Enable DRIVER BIAS OP
        MOD_W2BYTEMSK(REG_MOD_BK00_5E_L, 0x0000, 0x003F);
    }

    bStatus = TRUE;
    return bStatus;
}

void MHal_PNL_PreInit(void *pInstance, PNL_OUTPUT_MODE eParam)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions = eParam;
}

PNL_OUTPUT_MODE MHal_PNL_Get_Output_MODE(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    PNL_OUTPUT_MODE eParam = pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions;

    return eParam;
}

MS_U32 msReadEfuse(void *pInstance, MS_U8 u8Bank, MS_U32 u32Mask)
{
    MS_U32 u32Result = 0;
    MS_U8 u8Count = 0;

    W2BYTEMSK(0x2050, u8Bank<<2, BMASK(8:2));  /// reg28[8:2]Addr 6~0
    W2BYTEMSK(0x2050, BIT(13), BIT(13));       /// Reg28[13] Margin Read
    while(R2BYTEMSK(0x2050, BIT(13)) == BIT(13))
    {
        MsOS_DelayTask(1);
        u8Count ++;

        if (u8Count >10)
            break;
    }

    u32Result = (R4BYTE(0x2058)& u32Mask);    /// reg2C,2D read value
    printf("[%s][%d]u32Result=%tx, after mask u32Result=%tx\n", __FUNCTION__, __LINE__,(ptrdiff_t) R4BYTE(0x2058), (ptrdiff_t)u32Result);
    return u32Result;

}

void msSetVBY1RconValue(void *pInstance)
{
    MS_U16 u16DefaultICON_Max = 40, u16DefaultICON_Min = 7;
    MS_U16 u16DefaultICON = 18;
    MS_U32 u32Mask = 0x3F;
    MS_BOOL bEfuseMode = FALSE;
    MS_U16 u16SwingOffset = 0;  // by HW RD request
    MS_U16 u16temp = 0;

    if(!(_Hal_MOD_External_eFuse()))
    {
        if (msReadEfuse(pInstance, 0x4E, BIT(6)) == BIT(6))
            bEfuseMode = TRUE;


        // Disable HW calibration keep mode first, to make SW icon value can write into register.
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7A_L, 0, BIT(15));

        if (bEfuseMode)
        {
            if(((MS_U16)msReadEfuse(pInstance, 0x4E, u32Mask) + u16SwingOffset) > u16DefaultICON_Max)
                u16temp = u16DefaultICON;
            else if(((MS_U16)msReadEfuse(pInstance, 0x4E, u32Mask) + u16SwingOffset) < u16DefaultICON_Min)
                u16temp = u16DefaultICON;
            else
                u16temp = (MS_U16)msReadEfuse(pInstance, 0x4E, u32Mask) + u16SwingOffset;
        }
        else
        {
            u16temp = u16DefaultICON;
        }

        //ch0~ch13 rcon setting
        u16temp &= (u16temp&(MS_U16)u32Mask);
        printf("[%s][%d]u16temp= %x\n", __FUNCTION__, __LINE__, u16temp);

        MOD_A_W2BYTE(REG_MOD_A_BK00_40_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_41_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_42_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_43_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_44_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_45_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_46_L, (u16temp<<8|u16temp));
        MOD_A_W2BYTE(REG_MOD_A_BK00_47_L, (u16temp<<8|u16temp));
    }
}

void MHal_PNL_SetOutputType(void *pInstance, PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type)
{
    MS_U16 u16ValidSwing2 = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if( eLPLL_Type == E_PNL_TYPE_TTL)
    {
        // select pair output to be TTL
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x2000,0x2000);
        MsOS_DelayTask(1);
        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0000);
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000);
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x0000,0x2000);

        // other TTL setting
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6B_L, 0x0000,0x0FF0);
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, 0x0000,0xFF00);


        MOD_A_W2BYTE(REG_MOD_A_BK00_66_L, 0x0000);
        MOD_A_W2BYTE(REG_MOD_A_BK00_67_L, 0x0000);
        //MOD_W2BYTEMSK(REG_MOD_BK00_5D_L, 0x0000, 0xE000);//just debug status in Manhattan

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, (BIT(3)|BIT(2)), (BIT(3)|BIT(2)));       // TTL skew: G
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, (BIT(5)|BIT(4)), (BIT(5)|BIT(4)));       // TTL skew: B

    }
    else if(( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_16LANE)||
            ( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)||
            ( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)||
            ( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)||
            ( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_1LANE))
    {
        // rcon
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & (MS_U32)E_APIPNL_MISC_SKIP_ICONVALUE)
        {
            HAL_MOD_CAL_DBG(printf("User define Swing Value=%u\n", __FUNCTION__, __LINE__, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel));
            MHal_PNL_MOD_Control_Out_Swing(pInstance, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel);
        }
        else
        {
            HAL_MOD_CAL_DBG(printf("Use RconValue\n", __FUNCTION__, __LINE__));
            msSetVBY1RconValue(pInstance);
        }

        //reg_gcr_en_rint_ch: enable double termination function
        MOD_A_W2BYTE(REG_MOD_A_BK00_06_L, 0x0000);

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6B_L, 0x03F0,0x0FF0);
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, 0x0000,0xFF00);

        // manhattan: reg_1ms_cnt/when 12MHz xtal clock, count 12000 times is 1ms
        // maserati : reg_mod_dummy2
        MOD_A_W2BYTE(REG_MOD_A_BK00_7F_L, 0xFFFF);

        // mod_a_77[1]: reg_vby1_ctrl_mode, select Hdtpn/Lockn is from Atop or PAD 1: from atop 0: from PAD
        //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x0000,BIT(1));  // there is no data when soc change mod output timing, so mark this setting

        MOD_W2BYTE(REG_MOD_BK00_61_L, 0x8F3F); //[15]all dk scr[13:8]aln_de_cnt [7:0] aln_pix_cnt

        MOD_A_W2BYTE(REG_MOD_A_BK00_66_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_67_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_68_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_69_L, 0xFFFF);

        // reg 60/61:reg_gpo_oez, Disable TTL output
        MOD_A_W2BYTE(REG_MOD_A_BK00_60_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_61_L, 0xFFFF);

        // reg 62/63:reg_gpo_datain, general purpose output data
        MOD_A_W2BYTE(REG_MOD_A_BK00_62_L, 0xFFFF);
        MOD_A_W2BYTE(REG_MOD_A_BK00_63_L, 0xFFFF);

        switch(eOutputMode)
        {
            case E_PNL_OUTPUT_NO_OUTPUT:
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x2000,0x2000);
                MsOS_DelayTask(1);
                MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000);
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x0000,0x2000);
                if(1)//( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)
                {
                    //-------------------------------------
                    //## icon (Swing)
                    MOD_A_W2BYTE(REG_MOD_A_BK00_30_L, 0x0000);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_31_L, 0x0000);

                    //-------------------------------------
                    //vby1
                    MOD_W2BYTE(REG_MOD_BK00_61_L, 0x8f3f); //[15]all dk scr[13:8]aln_de_cnt [7:0] aln_pix_cnt
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)
                {
                    //-------------------------------------
                    //## icon (Swing)
                    MOD_A_W2BYTE(REG_MOD_A_BK00_30_L, 0x0000);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_31_L, 0x0000);

                    //vby1
                    MOD_W2BYTE(REG_MOD_BK00_61_L, 0x8f3f);
                }
                break;

            case E_PNL_OUTPUT_CLK_ONLY:
            case E_PNL_OUTPUT_DATA_ONLY:
            case E_PNL_OUTPUT_CLK_DATA:
            default:
                if(eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_16LANE)
                {
                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);

                        _MHal_PNL_Set_Clk(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x5555); //[15:0]reg_output_conf[15:0]
                        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x5555); //[15:0]reg_output_conf[15:0]
                    }
                }
                else if(eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)
                {
                    //MOD_A_W2BYTE(REG_MOD_A_BK00_3A_L, 0xC100);
                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);

                        _MHal_PNL_Set_Clk(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x5555); //[15:0]reg_output_conf[15:0]
                        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000); //[15:0]reg_output_conf[15:0]
                    }
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)
                {
                    //ori Manhattan: MOD_W2BYTE(REG_MOD_BK00_4A_L, 0x0002);
                    MOD_W2BYTEMSK(REG_MOD_BK00_52_L, 0x0000, 0xC000); //[15]:reg_abswitch_l [14]:reg_abswitch_r
                    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, 0x0000, BIT(1));

                    MOD_A_W2BYTE(REG_MOD_A_BK00_32_L, 0x7f7f);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_33_L, 0x7f7f);

                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);

                        _MHal_PNL_Set_Clk(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0055); //[15:0]reg_output_conf[15:0]
                        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000); //[15:0]reg_output_conf[15:0]
                    }
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE )
                {
                    //ori Manhattan: MOD_W2BYTE(REG_MOD_BK00_4A_L, 0x0000);
                    MOD_W2BYTEMSK(REG_MOD_BK00_52_L, 0x0000, 0xC000); //[15]:reg_abswitch_l [14]:reg_abswitch_r
                    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, 0x0000, BIT(1));

                    MOD_A_W2BYTE(REG_MOD_A_BK00_32_L, 0x7f7f);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_33_L, 0x0000);

                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);

                        _MHal_PNL_Set_Clk(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0005);
                        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000); //[15:0]reg_output_conf[15:0]
                    }
                }
                else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_1LANE ) //Maserati Raptor output 576p/480p case
                {
                    //ori Manhattan: MOD_W2BYTE(REG_MOD_BK00_4A_L, 0x0000);
                    MOD_W2BYTEMSK(REG_MOD_BK00_52_L, 0x0000, 0xC000); //[15]:reg_abswitch_l [14]:reg_abswitch_r
                    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, 0x0000, BIT(1));

                    MOD_A_W2BYTE(REG_MOD_A_BK00_32_L, 0x7f7f);
                    MOD_A_W2BYTE(REG_MOD_A_BK00_33_L, 0x0000);

                    if( APIPNL_OUTPUT_CHANNEL_ORDER_USER == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType )
                    {
                        _MHal_PNL_Auto_Set_Config(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);

                        _MHal_PNL_Set_Clk(pInstance,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
                    }
                    else
                    {
                        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0101);
                        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000); //[15:0]reg_output_conf[15:0]
                    }
                }
                break;
        }
    }
    //// for osd dedicated output port, 1 port for video and 1 port for osd
    else if((eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&
            (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode == E_PNL_MODE_SINGLE))
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x001F , 0x001F); // enable clk_dot_mini_pre_osd & clk_dot_mini_osd
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, BIT(13),BIT(13));

        // enable osd lvds path
        MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(14) );
        MOD_W2BYTEMSK(REG_MOD_BK00_2E_L, BIT(10), BIT(10) );
        MOD_W2BYTEMSK(REG_MOD_BK00_05_L, 0x0000, BIT(12) );
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, 0x0000, BIT(0) );
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, (BIT(2)|BIT(3)|BIT(4)) );

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_77_L, BIT(0), BIT(0)); //[15]sw_rst
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, 0x0C00 , 0x1F00); // enable clk_dot_mini_pre_osd & clk_dot_mini_osd
    }
    else
    {
        switch(eOutputMode)
        {
            case E_PNL_OUTPUT_NO_OUTPUT:
                // if MOD_45[5:0] = 0x3F && XC_MOD_EXT_DATA_EN_L = 0x0,
                // then if XC_MOD_OUTPUT_CONF_L = 0x0 ---> output TTL as tri-state
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x2000,0x2000);
                MsOS_DelayTask(1);
                MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0x0000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0000);
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x0000,0x2000);

                //----------------------------------
                // Purpose: Set the output to be the GPO, and let it's level to Low
                // 1. External Enable, Pair 0~5
                // 2. GPIO Enable, pair 0~5
                // 3. GPIO Output data : All low, pair 0~5
                // 4. GPIO OEZ: output piar 0~5
                //----------------------------------

                //1.External Enable, Pair 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_66_L, 0x0FFF, 0x0FFF);
                //2.GPIO Enable, pair 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_68_L, 0x0FFF, 0x0FFF);
                //3.GPIO Output data : All low, pair 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_62_L, 0x0000, 0x0FFF);
                //4.GPIO OEZ: output piar 0~5
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_60_L, 0x0000, 0x0FFF);

                //1.External Enable, Pair 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_66_L, 0xF000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_67_L, 0xFFFF);
                //2.GPIO Enable, pair 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_68_L, 0xF000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_69_L, 0xFFFF);
                //3.GPIO Output data : All low, pair 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_62_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_63_L, 0x0000);
                //4.GPIO OEZ: output piar 6~15
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_60_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_61_L, 0x0000);

                //1234.External Enable, Pair 16~17
                //MOD_W2BYTE(REG_MOD_BK00_7E_L, 0xFF00);//remove in Manhattan

                //1.External Enable, Pair 18~20, 2.GPIO Enable, pair 18~20
                MOD_W2BYTEMSK(REG_MOD_BK00_5C_L, 0x3F00, 0x3F00);
                MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0x003F, 0x003F);
                //3.GPIO Output data : All low, pair 18~20
                MOD_W2BYTEMSK(REG_MOD_BK00_5E_L, 0x0000, 0x3F00);
                //4.GPIO OEZ: output piar 18~20
                //MOD_W2BYTEMSK(REG_MOD_BK00_7F_L, 0x0000, 0xFC00); //remove in Manhattan
                break;

            case E_PNL_OUTPUT_CLK_ONLY:
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_18_L, 0, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x4004);
                break;

            case E_PNL_OUTPUT_DATA_ONLY:
            case E_PNL_OUTPUT_CLK_DATA:
            default:

                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_68_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_69_L, 0x0000);
                //1. set GCR_PVDD_2P5=1¡¦b1;           MOD PVDD power:    1: 2.5V
                //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_39_L, 0, BIT(6));
                //2. set PD_IB_MOD=1¡¦b0;
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_01_L, 0x00, BIT(0));
                //  save ch6 init value
                u16ValidSwing2 = (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_33_L, 0x3F00)>>8);
                //3. set Desired Pairs: GCR_ICON[5:0]=6h3f (current all open);
                MHal_PNL_MOD_Control_Out_Swing(pInstance, MHal_PNL_MODSwingRegToRealLevelValue(pInstance, 0x3F));
                //4. set Desired Pairs: GCR_PE_ADJ[2:0]=3h7 (pre-emphasis current all open )
                MHal_PNL_MOD_Control_Out_PE_Current (pInstance, 0x07);
                //5. Enable low-power modeinternal termination Open, Enable OP
                MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (pInstance, 1);

                MsOS_DelayTask(1);

                //6. Enable low-power modeinternal termination Open, Enable OP
                MHal_Output_LVDS_Pair_Setting(pInstance,
                                              pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15,
                                              pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
                MHal_Shift_LVDS_Pair(pInstance, pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift);

                //7. set Desired Pairs: GCR_PE_ADJ[2:0]=3¡¦h0 (pre-emphasis current all Close)
                MHal_PNL_MOD_Control_Out_PE_Current (pInstance, 0x00);
                //8. set Desired Pairs: GCR_ICON[5:0]    (current all init);
                MHal_PNL_MOD_Control_Out_Swing(pInstance, MHal_PNL_MODSwingRegToRealLevelValue(pInstance, u16ValidSwing2));
                //9. Disable low-power modeinternal termination Close, Disable OP
                MHal_PNL_MOD_Control_Out_TTL_Resistor_OP (pInstance, 0);

                // other TTL setting
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6B_L, 0x03F0,0x0FF0);// LVDS output enable, [5:4] Output enable: PANEL_LVDS/ PANEL_miniLVDS/ PANEL_RSDS
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, 0x0000,0xFF00);

                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_66_L, 0x0000, 0xF000);
                MOD_A_W2BYTE(REG_MOD_A_BK00_67_L, 0x0000);

                //MOD_W2BYTEMSK(REG_MOD_BK00_7E_L, 0x0000, 0x000F); //remove in Manhattan

                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, 0x0000, (BIT(3)|BIT(2)));       // TTL skew: G
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_74_L, 0x0000, (BIT(5)|BIT(4)));       // TTL skew: B

                break;
        }
    }

//    MHal_PNL_Bringup(pInstance);
}

void Mhal_PNL_Flock_LPLLSet(void *pInstance, MS_U64 ldHz)
{
    UNUSED(ldHz);
}

void MHal_PNL_MISC_Control(void *pInstance, MS_U32 u32PNL_MISC)
{
    UNUSED(u32PNL_MISC);
}

void MHal_PNL_Init_XC_Clk(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    //load clk table: CLKGEN0SettingTBL / CLKGEN2SettingTBL
    E_PNL_SUPPORTED_CLK_TYPE eCLKType= _MHal_Transfer_PanelType_To_CLKType(pstPanelInitData->eLPLL_Type, pstPanelInitData->eLPLL_Mode);
    _MHal_PNL_DumpVideoClkTable(eCLKType);

    if((pPNLResourcePrivate->stapiPNL._bSkipTimingChange == FALSE))
    {
        //load clk table: CLKGEN0SettingTBL / CLKGEN2SettingTBL
        E_PNL_SUPPORTED_CLK_TYPE eCLKType= _MHal_Transfer_PanelType_To_CLKType_OSD(0, pstPanelInitData->eLPLL_Type, pstPanelInitData->eLPLL_Mode);
        _MHal_PNL_DumpOSDClkTable(eCLKType);
    }


    //set XC CLK
    if( ( E_PNL_LPLL_VBY1_10BIT_16LANE == pstPanelInitData->eLPLL_Type)||
        ( E_PNL_LPLL_VBY1_8BIT_16LANE == pstPanelInitData->eLPLL_Type ) )
    {
        //16 lane
        // CLK GEN0
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                          // [0] disable clock
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);                         // [1] invert clock
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_SEL_LPLL, CKG_ODCLK_SEL_SOURCE);          // [2] select source tobe LPLL clock,
                                                                                       // 0:synthetic clock out, 1:LPLL clock out
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_XTAL, CKG_ODCLK_MASK);                    // [4..3] LPLL clock div

        // CLK GEN2
        // CLK_ODCLK_2P clock setting
        // [0]:disable clock [1]:invert clock
        // [2]: select clock source, 0:synthetic clock out, 1:LPLL clock out
        // [3]: select clock source, 0:LPLL output clock, 1:LPLL output clk divN
        W2BYTEMSK(REG_RVD_43_L, 0x0C00, 0x1F00);
        W2BYTEMSK(REG_RVD_46_L, 0x0000, 0x1F1F);  //odclk_freq div n/m, [4:0]reg_ckg_odclk_div_nm_m, [12:8]reg_ckg_odclk_div_nm_n,
        //set LPLL mux

        W2BYTEMSK(L_CLKGEN0(0x7E), 0x0030, 0x00F0);           //LPLL_ODCLK setting  reg_ckg_odclk = reg_clkgen0_reserved0[7:2]
                                                              //[0] : diable clock
                                                              //[1] : invert clock
                                                              //[4:2] : xx0 : floclk_odclk_synth_out
                                                              // 011 / 111: LPLL output clock
        if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
        {
            W2BYTEMSK(REG_RVD_63_L, 0x04, 0x0C);//sub oclck flow main odclk
        }
    }
    else if( ( E_PNL_LPLL_VBY1_10BIT_8LANE == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_VBY1_8BIT_8LANE == pstPanelInitData->eLPLL_Type ) )
    {
        // 8 lane
        // CLK GEN0
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                          // [0] disable clock
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);                         // [1] invert clock
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_SEL_LPLL, CKG_ODCLK_SEL_SOURCE);          // [2] select source tobe LPLL clock,
                                                                                       // 0:synthetic clock out, 1:LPLL clock out
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_XTAL, CKG_ODCLK_MASK);               // [4..3] LPLL clock div

        // CLK GEN2
        // CLK_ODCLK_2P clock setting
        // [0]:disable clock [1]:invert clock
        // [2]: select clock source, 0:synthetic clock out, 1:LPLL clock out
        // [3]: select clock source, 0:LPLL output clock, 1:LPLL output clk divN
        W2BYTEMSK(REG_RVD_43_L, 0x0400, 0x1F00);
        W2BYTEMSK(REG_RVD_46_L, 0x0000, 0x1F1F);  //odclk_freq div n/m, [4:0]reg_ckg_odclk_div_nm_m, [12:8]reg_ckg_odclk_div_nm_n,

        //set LPLL mux

        W2BYTEMSK(L_CLKGEN0(0x7E), 0x0010, 0x00F0);           //LPLL_ODCLK setting  reg_ckg_odclk = reg_clkgen0_reserved0[7:2]
                                                              //[0] : diable clock
                                                              //[1] : invert clock
                                                              //[4:2] : xx0 : floclk_odclk_synth_out
                                                              //001 : LPLL output clock div2
        if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
        {
            W2BYTEMSK(REG_RVD_63_L, 0x0C, 0x0C);//sub oclck flow main odclk
        }

    }
    else
    {
        // 4 lane or LVDS
        // CLK GEN0
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                          // [0] disable clock
        W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);                         // [1] invert clock
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_SEL_LPLL, CKG_ODCLK_SEL_SOURCE);          // [2] select source tobe LPLL clock,
                                                                                       // 0:synthetic clock out, 1:LPLL clock out
        W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_XTAL, CKG_ODCLK_MASK);               // [4..3] LPLL clock div

        // CLK GEN2
        // CLK_ODCLK_2P clock setting
        // [0]:disable clock [1]:invert clock
        // [2]: select clock source, 0:synthetic clock out, 1:LPLL clock out
        // [3]: select clock source, 0:LPLL output clock, 1:LPLL output clk divN
        if( ( E_PNL_TYPE_TTL == pstPanelInitData->eLPLL_Type)||
            ((E_PNL_TYPE_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_SINGLE==pstPanelInitData->eLPLL_Mode))||
            ((E_PNL_TYPE_HS_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_SINGLE==pstPanelInitData->eLPLL_Mode)) ||
            ((E_PNL_LPLL_VBY1_8BIT_1LANE == pstPanelInitData->eLPLL_Type) || (E_PNL_LPLL_VBY1_10BIT_1LANE == pstPanelInitData->eLPLL_Type)))
        {
            W2BYTEMSK(REG_RVD_43_L, 0x0C00, 0x1F00);  //[12:8]reg_ckg_odclk_2p, [11]Select clock source, 1: LPLL output clock divN  (control by reg_ckg_odclk_div_nm)
                                                      //[4:0]reg_ckg_clk_misc,
            W2BYTEMSK(REG_RVD_46_L, 0x0201, 0x1F1F);  //odclk_freq div n/m, [4:0]reg_ckg_odclk_div_nm_m, [12:8]reg_ckg_odclk_div_nm_n,

            if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
            {
                W2BYTEMSK(REG_RVD_63_L, 0x04, 0x0C); //sub oclck flow main odclk
            }
        }
        else
        {
            W2BYTEMSK(REG_RVD_43_L, 0x0400, 0x1F00);

            W2BYTEMSK(REG_RVD_46_L, 0x0000, 0x1F1F);  //odclk_freq div n/m, [4:0]reg_ckg_odclk_div_nm_m, [12:8]reg_ckg_odclk_div_nm_n,

            if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
            {
                W2BYTEMSK(REG_RVD_63_L, 0x0C, 0x0C); //sub oclck flow main odclk
            }
        }

        //set LPLL mux
        if( ( (E_PNL_TYPE_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_DUAL==pstPanelInitData->eLPLL_Mode))||
             ( (E_PNL_TYPE_HS_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_DUAL==pstPanelInitData->eLPLL_Mode))||
             ( E_PNL_LPLL_VBY1_10BIT_2LANE == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_VBY1_8BIT_2LANE == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_VBY1_10BIT_4LANE == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_VBY1_8BIT_4LANE == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_EPI24_12P == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_EPI28_12P == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_USI_T_8BIT_12P == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_USI_T_10BIT_12P == pstPanelInitData->eLPLL_Type)||
             ( E_PNL_LPLL_ISP_8BIT_12P == pstPanelInitData->eLPLL_Type)||
             ( (E_PNL_LPLL_ISP_8BIT_6P_D == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_DUAL==pstPanelInitData->eLPLL_Mode)))
        {
             W2BYTEMSK(L_CLKGEN0(0x7E), 0x0010, 0x00F0);      //LPLL_ODCLK setting  reg_ckg_odclk = reg_clkgen0_reserved0[7:2]
                                                              //[0] : diable clock
                                                              //[1] : invert clock
                                                              //[4:2] : xx0 : floclk_odclk_synth_out
                                                              //001 : LPLL output clock div2
        }
        else if( ( E_PNL_TYPE_TTL == pstPanelInitData->eLPLL_Type)||
             ((E_PNL_TYPE_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_SINGLE==pstPanelInitData->eLPLL_Mode))||
             ((E_PNL_TYPE_HS_LVDS == pstPanelInitData->eLPLL_Type)&&(E_PNL_MODE_SINGLE==pstPanelInitData->eLPLL_Mode)))
        {
             W2BYTEMSK(L_CLKGEN0(0x7E), 0x0050, 0x00F0);      //LPLL_ODCLK setting  reg_ckg_odclk = reg_clkgen0_reserved0[7:2]
                                                              //[0] : diable clock
                                                              //[1] : invert clock
                                                              //[4:2] : xx0 : floclk_odclk_synth_out
                                                              // 101 : LPLL output clock div4
        }

    }

    if(  IsVBY1(pstPanelInitData->eLPLL_Type_Ext) )
    {
        // [0]reg_vx1gpi_mode, refer to tmux table : pad_list:  reg_vx1gpi_mode
        W2BYTEMSK(REG_CHIP_25_L, 0x0001, 0x0001);
    }

    if(  IsVBY1(pstPanelInitData->eLPLL_Type_Ext) )
    {
        W2BYTEMSK(L_BK_TCON(0x00), 0x0000, 0xFFFF);
        W2BYTEMSK(L_BK_TCON(0x69), 0x0400, 0x0400);
    }

    // reg_ckg_odclk_2p invert
    // SW patch for Maserati only
    // For sub video flicker issue, need to invert clk by default
    W2BYTEMSK(REG_RVD_43_L, 0x0200, 0x0200); //FIXME:patch for PIP

#if 0
    // setup output dot clock

    W2BYTEMSK(L_CLKGEN0(0x53), CKG_ODCLK_CLK_LPLL, CKG_ODCLK_MASK);      // select source tobe LPLL clock
    W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_INVERT);               // clock not invert
    W2BYTEMSK(L_CLKGEN0(0x53), DISABLE, CKG_ODCLK_GATED);                // enable clock
    W2BYTEMSK(L_CLKGEN0(0x53), 0xC0, 0xF0);      //  reg_ckg_odclk_mft

    W2BYTE(L_CLKGEN0(0x58),0x0000); //[3:0]ckg_tx_mod
    W2BYTE(L_CLKGEN1(0x31), 0x0000); //[11:8]ckg_odclk_frc



    if(  IsVBY1(pstPanelInitData->eLPLL_Type) )
    {

        W2BYTE(REG_CLKGEN0_57_L,0x0008); //[3:0]ckg_fifo
        W2BYTE(L_CLKGEN0(0x63), 0x0410);   //[11:8]ckg_tx_mod_osd[4:0]osd2mod
        W2BYTE(REG_RVD_09_L, 0x1800); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo
        W2BYTEMSK(L_CLKGEN0(0x55), 0x00, 0xF00); //[11:8] reg_ckg_osdc
    }
    else
    {

        W2BYTE(REG_CLKGEN0_57_L,0x0000); //[3:0]ckg_fifo
        if((pstPanelInitData->eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&(pstPanelInitData->eLPLL_Mode == E_PNL_MODE_SINGLE))
        {
            W2BYTE(L_CLKGEN0(0x63), 0x0410);   //[11:8]ckg_tx_mod_osd[4:0]osd2mod
            W2BYTE(REG_RVD_09_L, 0x1000); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo+
        }
        else
        {
            W2BYTE(L_CLKGEN0(0x63),0x0000); //[11:8]ckg_tx_mod [3:0]ckg_osd2mod
            W2BYTE(REG_RVD_09_L, 0x0000); //[12:8]ckg_vby1_fifo_osd [3:0]clk_vby1_fifo
        }
    }
#endif
}

void MHal_PNL_Init_MOD(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    //------------------------------------------------------------------------

    PNL_DBG(PNL_DBGLEVEL_INIT, "u16MOD_CTRL0 = %x\n", pstPanelInitData->u16MOD_CTRL0);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16MOD_CTRL9 = %x\n", pstPanelInitData->u16MOD_CTRL9);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16MOD_CTRLA = %x\n", pstPanelInitData->u16MOD_CTRLA);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u8MOD_CTRLB  = %x\n", pstPanelInitData->u8MOD_CTRLB);

    //-------------------------------------------------------------------------
    // Set MOD registers
    //-------------------------------------------------------------------------

    //reg_rsclk_testmd
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, ( pstPanelInitData->u16MOD_CTRL0 & (BIT(1)) )<<14, (BIT(15)) );
    //reg_lvds_ti:LVDS JEIDA/VESA mode select;l         0: JEIDAl         1: VESA
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, ( pstPanelInitData->u16MOD_CTRL0 & (BIT(2)) )<<12, (BIT(14)) );
    //reg_pdp_10bit:pdp_10bit
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, ( pstPanelInitData->u16MOD_CTRL0 & (BIT(3)) )<<10, (BIT(13)) );
    //reg_lvds_plasma:
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, ( pstPanelInitData->u16MOD_CTRL0 & (BIT(4)) )<<8 , (BIT(12)) );
    //reg_ch_polarity:polarity swap
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, ( pstPanelInitData->u16MOD_CTRL0 & (BIT(5)) )<<6 , (BIT(11)) );
    //reg_osd_on_de_a: PDP osd de on DE channel A
    MOD_W2BYTEMSK(REG_MOD_BK00_51_L, ( pstPanelInitData->u16MOD_CTRL0 & (BIT(7)) )>>5, (BIT(2)) ); //reg_rsclk_testmd

    //    GPIO is controlled in drvPadConf.c
    //    MDrv_Write2Byte(L_BK_MOD(0x46), 0x0000);    //EXT GPO disable
    //    MDrv_Write2Byte(L_BK_MOD(0x47), 0x0000);    //EXT GPO disable

    //new setting from Masrati scripts
    MOD_W2BYTEMSK(REG_MOD_BK00_3C_L, 0x0000, 0xFFFF);

    //MFT setting
    _MHal_PNL_Init_MFT(pInstance,pstPanelInitData);

    if(  IsVBY1(pstPanelInitData->eLPLL_Type_Ext) )
    {
        //new setting from Masrati scripts
        //reg_data_format: data format of digital serializer
        //0: LVDS 2: mini-LVDS 3: EPI-8bit 4: EPI-10bit(case2) 5: Vby1 7: EPI-10bit(case1) 8: CMPI-10bit 9: CMPI-8bit 10: USI-T 11: ISP
        MOD_W2BYTEMSK(REG_MOD_BK00_30_L, 0x5000, 0xF000);

        //new setting from Masrati scripts
        //switch channel, i.e 16 lane / 8 lane / 4 lane
        _Hal_MOD_VB1_CH_SWICH(pstPanelInitData->eLPLL_Type_Ext);

        //new setting from Masrati scripts
        // arrange MFT 4p output order
        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_1LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_1LANE) )
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_0F_L, 0xF800, 0xFF00 ); //reg_rsclk_testmd
        }
        else
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_0F_L, 0xD800, 0xFF00 ); //reg_rsclk_testmd
        }

        // sc4 62 [15]reg_vby1_proc_st: vby1 packer  process start
        // sc4 62 [13..12]reg_vby1_byte_mode: vby1 packer byte mode;l 0x : 5 bytel  10: 4 bytel  11: 3 byte
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, ( BIT(15)|BIT(13) ), 0xFFFF);

        // sc4 62 [5..4]reg_vby1_pair_mirror: mirror vby1 pairs
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, (pstPanelInitData->u16MOD_CTRL9 & (BIT(12)|BIT(11)))>>7, BIT(5)|BIT(4));

        // sc4 62 [14]reg_vby1_8ch: vby1 8ch mode
        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE) )
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(14), BIT(14));
        }
        else
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x0000, BIT(14));
        }

        // sc4 00[9]: reg_quadmode, LVDS dual channel;  0: dual channel  1: quad channel
        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(9), BIT(9));
            MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(0), BIT(0));
        }
        else
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_00_L, 0x0000, BIT(9));
            MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(0), BIT(0));
        }


        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))  // use Dual port to decide the Vx1 1 or 2 devision config
        {
            printf("\n[%s][%d]Vx1 2 division\n", __FUNCTION__, __LINE__);
            if(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
            {

                //Manhattan: MOD_W2BYTEMSK(REG_MOD_BK00_21_L, 0x1002, 0xFFFF); //[11:0]reg_dly_value
                //Maserati : do not to set reg_dly_value
                // mark for Maserati subbank
                //MOD_W2BYTEMSK(REG_MOD_BK00_7F_L, BIT(0), (BIT(1)|BIT(0))); // reg.7f[1:0]=reg_vfde_dly_mux

                //Manhattan: MOD_W2BYTEMSK(REG_MOD_BK00_7F_L, 0x0002, 0xFFFF); //[2:0]reg_sram_usage
                //Maserati : MFT have sram, so we do not set any setting.

                MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(12)); //[12]reg_vbi_en:vbi information on lvds enable
                MOD_W2BYTEMSK(REG_MOD_BK00_54_L, BIT(13), BIT(13)); //[13]reg_vfde_mask:mask vfde AND de
            }
        }
        else
        {
            printf("\n[%s][%d]Vx1 1 division\n", __FUNCTION__, __LINE__);

            MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(12)); //[12]reg_vbi_en:vbi information on lvds enable
            MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(13)); //[13]reg_vfde_mask:mask vfde AND de

        }

        if((pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE))
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(14), BIT(14)); // [14] enable 8ch vx1 mode
        }
        else if((pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)||(pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x0000, BIT(14)); // [14] enable 8ch vx1 mode

            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            {
                // 2 Divisoin

                //be deleted in Maserati
                //MOD_W2BYTEMSK(REG_MOD_BK00_63_L, BIT(11), BIT(11)); // [11:10]reg_vby1_pair_mirror2
            }
            else
            {
                // 1 Division

                //be deleted in Maserati
                //MOD_W2BYTEMSK(REG_MOD_BK00_63_L, 0x00, BIT(11)); // [11:10]reg_vby1_pair_mirror2
            }
        }
        else //if   ///E_PNL_LPLL_VBY1_10BIT_4LANE, E_PNL_LPLL_VBY1_10BIT_2LANE
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x00, BIT(14)); // [14] enable 8ch vx1 mode
        }

        MHal_Output_Channel_Order(pInstance,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);

        ////per RD's suggestion ---Start
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_04_L, 0xFFFF, 0xFFFF);   //reg_gcr_pe_en_ch: Differential output pre-emphasis enable
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_06_L, 0x0000, 0xFFFF);   //reg_gcr_en_rint_ch: enable double termination function
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_08_L, 0xFFFF, 0xFFFF);   //vby1 channel enable: vby1 channel enable

        /// reg_gcr_pe_adj ch0~ch13: Differential output data/clock pre-emphasis level adjust of channel X
        MOD_A_W2BYTE(REG_MOD_A_BK00_22_L,0x2222);
        MOD_A_W2BYTE(REG_MOD_A_BK00_23_L,0x2222);
        MOD_A_W2BYTE(REG_MOD_A_BK00_24_L,0x2222);
        MOD_A_W2BYTE(REG_MOD_A_BK00_25_L,0x2222);

        // [15] reg_vby1_hw_lock : set Vby1 lock sequence by HW mode
        // [13] reg_vby1_ext_fsm_en : config each vby1 set use same FSM state (split video/osd path)
        // [12] reg_vby1_proc_auto_fix : vby1 hw auto gen proc_st trig
        // [10] reg_vby1_ext_ptr_en : configure each vby1 fifo used same read/write pointer
        MOD_W2BYTEMSK(REG_MOD_BK00_63_L, (BIT(15)|BIT(13)|BIT(12)|BIT(10)), 0xFF00);

        //disparity setting: for 8bits/10bits encode hw error
        MOD_W2BYTEMSK(REG_MOD_BK00_3A_L, BIT(7), BIT(7));

        //The threshold value be set too strict ( ori: MOD_77/79 =0x0 )
        //And this reg should be set before enable serializer function
        //[15]reg_sw_rptr_fix_en: pointer fix by sw mode enable
        //[14:12]reg_sw_wptr_check: sw mode to decision write point check point
        //[10:8]reg_sw_rptr_fix_ini: sw mode to decision read point initial value
        //[6:4]reg_sw_rptr_fix_hi_th: sw mode to decision read pointer hi boundary
        //[2:0]reg_sw_rptr_fix_lo_th: sw mode to decision read pointer low boundary
        MOD_W2BYTEMSK(REG_MOD_BK00_79_L,0x8142,0xFFFF);
        MOD_W2BYTEMSK(REG_MOD_BK00_77_L,0x8142,0xFFFF);

        //[13]reg_seri_auto_fix_osd : enable osd serializer auto fix read/write point mis-balance
        //[14]reg_seri_osd_mod:for OSD, switch chanel 8~13 as OSD path
        //[15]reg_dbg_status_switch:switch debug information(reg_dbg_status_sel/reg_dbg_status) between Video/OSD path     0: Video       1: OSD
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, (BIT(14)|BIT(13)) , (BIT(15)|BIT(14)|BIT(13)) );

        //[0]reg_seri_enable:enable serializer function
        //[1]reg_seri_auto_fix:enable serializer auto fix read/write point mis-balance
        //[2]reg_fix_cnt_clr
        //[3]reg_dbg_status_sel:select debug status, read status from reg_dbg_status[15:0] 0: auto fix cnt         1: point diff value
        //[15..8]reg_tester_pix_ext: test pixel extension for 16bit serializer
        MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x0003 , 0xFF0F);
    }
    else
    {
        if(pstPanelInitData->eLPLL_Type == E_PNL_TYPE_LVDS)
        {
            //new setting from Masrati scripts
            //reg_data_format: data format of digital serializer
            //0: LVDS 2: mini-LVDS 3: EPI-8bit 4: EPI-10bit(case2) 5: Vby1 7: EPI-10bit(case1) 8: CMPI-10bit 9: CMPI-8bit 10: USI-T 11: ISP
            MOD_W2BYTEMSK(REG_MOD_BK00_30_L, 0x0000, 0xF000);

            //new setting from Masrati scripts
            // arrange MFT 4p output order (LVDS)
            if(pstPanelInitData->eLPLL_Mode == E_PNL_MODE_SINGLE)
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_0F_L, 0xE400, 0xFF00 ); //reg_rsclk_testmd
            }
            else
            {
                MOD_W2BYTEMSK(REG_MOD_BK00_0F_L, 0xF800, 0xFF00 ); //reg_rsclk_testmd
            }

            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_08_L, 0x0000, 0xFFFF);   //vby1 channel enable: vby1 channel enable

            // sc4 00[9]: reg_quadmode, LVDS dual channel;  0: dual channel  1: quad channel
            MOD_W2BYTEMSK(REG_MOD_BK00_00_L, 0x0000, BIT(9));
            MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(0), BIT(0));
        }

        // [7,6] : output formate selction 10: 8bit, 01: 6bit :other 10bit, bit shift
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, (pstPanelInitData->u16MOD_CTRL9 & (BIT(7)|BIT(6)))<<8 , (BIT(15)|BIT(14)));
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, (pstPanelInitData->u16MOD_CTRL9 & BIT(8) )<<5 , BIT(13));
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, (pstPanelInitData->u16MOD_CTRL9 & BIT(9) )<<3 , BIT(12));

        MHal_Output_Channel_Order(pInstance,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                                  pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
    }

    // Manhattan ori :MOD_W2BYTE(REG_MOD_BK00_4A_L, pstPanelInitData->u16MOD_CTRLA);
    //MOD_W2BYTEMSK(REG_MOD_BK00_52_L, (pstPanelInitData->u16MOD_CTRLA & BIT(0))<<14, BIT(14)); //[15]:reg_abswitch_l [14]:reg_abswitch_r
    //MOD_W2BYTEMSK(REG_MOD_BK00_52_L, (pstPanelInitData->u16MOD_CTRLA & BIT(0))<<15, BIT(15)); //[15]:reg_abswitch_l [14]:reg_abswitch_r
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, (pstPanelInitData->u16MOD_CTRLA & BIT(4))>>3 , BIT(1));// PANEL_INV_DCLK
    MOD_W2BYTEMSK(REG_MOD_BK00_00_L, (pstPanelInitData->u16MOD_CTRLA & BIT(12))>>7, BIT(5));// PANEL_INV_HSYNC
    MOD_W2BYTEMSK(REG_MOD_BK00_00_L, (pstPanelInitData->u16MOD_CTRLA & BIT(3))<<3, BIT(6)); // PANEL_INV_VSYNC
    MOD_W2BYTEMSK(REG_MOD_BK00_00_L, (pstPanelInitData->u16MOD_CTRLA & BIT(2))<<5, BIT(7)); // PANEL_INV_DE

    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, (pstPanelInitData->u8MOD_CTRLB & (BIT(0)|BIT(1)))<<2, (BIT(2)|BIT(3))); //for LVDS: [1:0]ti_bitmode / 10:8bit  11:6bit  0x:10bit
    MOD_W2BYTEMSK(REG_MOD_BK00_05_L, BIT(13), BIT(13)); // for LVDS: reg_pdp_10bit

    //dual port lvds _start_//
    // output configure for 26 pair output 00: TTL, 01: LVDS/RSDS/mini-LVDS data differential pair, 10: mini-LVDS clock output, 11: RSDS clock output
    _MHal_PNL_Set_Clk(pInstance,
                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType,
                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder0_3,
                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder4_7,
                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder8_11,
                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputOrder12_13);
    //dual port lvds _end_//

    //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_3A_L, (_u8PnlDiffSwingLevel << 1), 0xFE);       //differential output swing level
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, 0x0000 , 0x1F00);       //bank selection for skew clock

    //if(!MHal_PNL_MOD_Control_Out_Swing(_u8PnlDiffSwingLevel))
    //    printf(">>Swing Level setting error!!\n");

    //mark in Maserati
    //if(pstPanelInitData->eLPLL_Type != E_PNL_TYPE_MINILVDS)
    //{
    //    MOD_W2BYTEMSK(REG_MOD_A_BK00_78_L, 0x7, 0x07);
    //}

    //// Patch for Vx1 and it should be control by panel ini
    if(  IsVBY1(pstPanelInitData->eLPLL_Type_Ext) )
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_02_L, 0xFFFF, 0xFFFF);
    }
    else
    {
        MOD_A_W2BYTE(REG_MOD_A_BK00_02_L, pstPanelInitData->u16LVDSTxSwapValue);
    }

#ifdef CONFIG_MSTAR_SRAMPD
    // At initial step, if non TCON panel, enable OD/RGBW/M+/Demura/DGA_Gamma SRAM PD
    if((pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI34_8P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI28_8P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI34_6P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI28_6P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI34_2P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI34_4P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI28_2P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI28_4P)&&
       (pstPanelInitData->eLPLL_Type_Ext != E_PNL_LPLL_EPI28_12P))
    {
        if(pPNLInstancePrivate->u32DeviceID == 0)
        {
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_10_L, BIT(0), BIT(0));   //OD SRAM PD Enable  : SC_SPD_BK3F_10[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, 0, BIT(8));        //OD Clock gate : ~SC_SPD_BK3F_50[8]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, BIT(15), BIT(15)); //OD Bypass Enable :SC_OD_BK16_6F[15]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_10_L, BIT(1), BIT(1));   // RGBW SRAM PD Enable : SC_SPD_BK3F_10[1]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, 0, BIT(9));        // RGBW Clock Gate : ~SC_SPD_BK3F_50[9]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, BIT(12), BIT(12)); // RGBW bypass enable :SC_OD_BK16_6F[12]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(13));       // RGBW bypass enable : ~SC_OD_BK16_6F[13]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_17_L, BIT(0), BIT(0));   //M+ SRAM PD Enable  : SC_SPD_BK3F_17[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_50_L, 0, BIT(10));        //M+ Clock gate : ~SC_SPD_BK3F_50[10]
            //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L,0 , BIT(12)); // M+ bypass enable :SC_OD_BK16_6F[12]
            //SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK16_6F_L, 0, BIT(13));       // M+ bypass enable : ~SC_OD_BK16_6F[13]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_0B_L, BIT(4), BIT(4));   //Demura SRAM PD Enable  : SC_SPD_BK3F_0B[4]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK77_26_L, BIT(14), BIT(14));   //Demura Clock Gate  : SC_BK77_26[14]
            W2BYTEMSK(REG_RVD_45_L, BIT(0), BIT(0));                                              //Demura Clock gate : BK100A_CLKGEN2_45[0]
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK77_26_L, BIT(15), BIT(15));   //Demura Clock Gate  : SC_BK77_26[15]

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK3F_0B_L, BIT(5), BIT(5));   //DGA_GAMMA SRAM PD Enable  : SC_SPD_BK3F_0B[5]
        }
    }

    // At initial step, SRAM PD for LD
    // To avoid mantis 1082875: boot logo flash issue, remove  SRAM PD for LD from XC_init to PNL_init
    // only used it in mboot, to avoid panel auto detect if panel type changed.
#if defined (MSOS_TYPE_NOS)
    if(pPNLInstancePrivate->u32DeviceID == 0)
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BKC9_65_L, (BIT(0)), 0x0001);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BKCE_01_L, (0x0000), 0x1000);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BKCE_37_L, (BIT(15)), 0x8000);
        //non-FO setting
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK3F_12_L, (BIT(0)| BIT(1)), 0x0003);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK3F_50_L, (0x0000), 0x3800);
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID,REG_SC_BK2E_37_L, (BIT(15)), 0x8000);
    }
#endif
#endif

    // TODO: move from MDrv_Scaler_Init(), need to double check!
    //MOD_W2BYTEMSK(REG_MOD_BK00_53_L, BIT(0), BIT(0)); //can not find this register in Manhattan register table


    //--------------------------------------------------------------
    //Depend On Bitmode to set Dither
    //--------------------------------------------------------------


    // always enable noise dither and disable TAILCUT
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, ((pstPanelInitData->u8PanelNoiseDith ? XC_PAFRC_DITH_NOISEDITH_EN : (1 - XC_PAFRC_DITH_NOISEDITH_EN)) <<3) , BIT(3));
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, XC_PAFRC_DITH_TAILCUT_DISABLE, BIT(4));

    switch(pstPanelInitData->u8MOD_CTRLB & 0x03)//[1:0]ti_bitmode b'10:8bit  11:6bit  0x:10bit
    {
        case HAL_TI_6BIT_MODE:
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, BIT(2), BIT(2));
            break;

        case HAL_TI_8BIT_MODE:
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, 0x00, BIT(2));
            break;

        case HAL_TI_10BIT_MODE:
        default:
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, 0x00, BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_3F_L, 0x00, BIT(2));
            break;
    }


    //-----depend on bitmode to set Dither------------------------------
    MHal_PNL_SetOutputType(pInstance, pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions, pstPanelInitData->eLPLL_Type);     // TTL to Ursa
    //MHal_PNL_Bringup(pInstance);

    MHal_PNL_MISC_Control(pInstance, pstPanelInitData->u32PNL_MISC);

    PNL_DBG(PNL_DBGLEVEL_INIT, "OutputType  = %x, eLPLL_Type = %x\n", pPNLResourcePrivate->sthalPNL._eDrvPnlInitOptions, pstPanelInitData->eLPLL_Type);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u32PNL_MISC  = %tx\n", (ptrdiff_t)pstPanelInitData->u32PNL_MISC);

}

void MHal_PNL_DumpMODReg(void *pInstance, MS_U32 u32Addr, MS_U16 u16Value, MS_BOOL bHiByte, MS_U16 u16Mask)
{
    if (bHiByte)
    {
        MOD_W2BYTEMSK(u32Addr, (u16Value << 8), (u16Mask << 8));
    }
    else
    {
        MOD_W2BYTEMSK(u32Addr, u16Value, u16Mask);
    }
}

void MHal_MOD_Calibration_Init(void *pInstance, PNL_ModCali_InitData *pstModCaliInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    // Setup the default swing level
    pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel = pstModCaliInitData->u16ExpectSwingLevel;   //mv
#if 0
    // Pair setting
    // =========
    // Select calibration source pair, 00: ch2, 01: ch6, 10:ch8, 11:ch12
    //MOD_7D_L[3:2]
    // =========
    //in msModCurrentCalibration, it will transfer to the real data

    switch(pstModCaliInitData->u8ModCaliPairSel)
    {
        default:
        case 0:
        //ch 2
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x00; // ch2
        break;
        case 1:
        //ch 6
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x01; // ch6, calibration initialized value
        break;
        case 2:
        //ch 8
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x02;
        break;
        case 3:
        //ch 12
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_PAIR_SEL = 0x03;
        break;
    }
#endif
    // Target setting
    // =========
    // GCR_CAL_LEVEL[1:0] : REG_MOD_A_BK00_70_L =>
    // =========
    //in msModCurrentCalibration, it will transfer to the real data
    switch(pstModCaliInitData->u8ModCaliTarget)
    {
        default:
        case 0:
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 0;
        break;
        case 1:
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 1;
        break;
        case 2:
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 2;
        break;
        case 3:
            pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET = 3;
        break;
    }
    // Offset setting, for fine tune
    //_usMOD_CALI_OFFSET = pstModCaliInitData->s8ModCaliOffset;
    // _u8MOD_CALI_VALUE is a real value; the _u8MOD_CALI_VALUE is an idea value
    // Target value should be the same with _u8MOD_CALI_VALUE to be a default value
    pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE= pstModCaliInitData->u8ModCaliTarget;
    // PVDD setting
    pPNLResourcePrivate->sthalPNL._bPVDD_2V5 = pstModCaliInitData->bPVDD_2V5;

    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16ExpectSwingLevel = %u\n", pstModCaliInitData->u16ExpectSwingLevel);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u8ModCaliTarget     = %x\n", pstModCaliInitData->u8ModCaliTarget);
    PNL_DBG(PNL_DBGLEVEL_INIT, "_u8MOD_CALI_TARGET  = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET);
    PNL_DBG(PNL_DBGLEVEL_INIT, "_u8MOD_CALI_VALUE   = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_CALI_VALUE);
    PNL_DBG(PNL_DBGLEVEL_INIT, "bPVDD_2V5           = %x\n", pstModCaliInitData->bPVDD_2V5);

}

void MHal_BD_LVDS_Output_Type(void *pInstance, MS_U16 Type)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    if(Type == LVDS_DUAL_OUTPUT_SPECIAL )
    {
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Shift = LVDS_DUAL_OUTPUT_SPECIAL;
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type = 1;
    }
    else
    {
        pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type  = Type;
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);
    PNL_DBG(PNL_DBGLEVEL_INIT, "_u8MOD_LVDS_Pair_Type = %u\n", pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type);

}

MS_BOOL msModCalDDAOUT(void)
{
   // W2BYTEMSK(BK_MOD(0x7D), ENABLE, 8:8);
   // MsOS_DelayTask(10);  //10ms
    return (MS_BOOL)((MOD_R2BYTEMSK(REG_MOD_A_BK00_78_L, BIT(14))) >> 14);
}

MS_U8 msModCurrentCalibration(void *pInstance)
{
#if MOD_CAL_TIMER
        MS_U32 delay_start_time;
        delay_start_time=MsOS_GetSystemTime();
#endif

#if (!ENABLE_Auto_ModCurrentCalibration)
        return 0x60;
#else
    MS_U8 u8cur_ibcal=0;
    MS_U16 u16reg_32da = 0, u16reg_32dc = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    u16reg_32da = MOD_A_R2BYTE(REG_MOD_A_BK00_18_L);
    u16reg_32dc = MOD_A_R2BYTE(REG_MOD_A_BK00_19_L);

    PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "[%s][%d]\n", __FUNCTION__, __LINE__);

    // (1) Set keep mode to auto write calibration result into register.
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7A_L, BIT(15), BIT(15));

    // (2) Set calibration step waiting time
    MOD_A_W2BYTE(REG_MOD_A_BK00_7F_L, 0x0009); // (about 5us)
    //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7E_L, 0x0009, 0x00FF); //remove in Manhattan

    // (3) Set calibration toggle time
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7A_L, 0x0500, 0x0F00);

    // (4) Select calibration level (LVDS is 250mV)
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_78_L, pPNLResourcePrivate->sthalPNL._u8MOD_CALI_TARGET, BIT(2)|BIT(1)|BIT(0));    // Select calibration target voltage, 00: 250mV, 01:350mV, 10: 300mV, 11: 200mV

    // (5) Store output configuration value and Enable each pair test mode
    MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, 0xFFFF);
    MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0xFFFF);

    // (6) Enable Calibration mode
    MOD_A_W2BYTEMSK(REG_MOD_A_BK00_78_L, BIT(15), BIT(15));         // Enable calibration function

    MS_U8 u8CheckTimes = 0;
    while(1)
    {
        // (7) Enable Hardware calibration
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_79_L, BIT(15), BIT(15));

        // (8) Wait 3ms
        MsOS_DelayTask(3);

        // mark this step in Maserati
        // (9) Disable Hardware calibration
        //MOD_A_W2BYTEMSK(REG_MOD_A_BK00_79_L, 0x0000, BIT(15));

        // (9) Check Finish and Fail flag bit
        if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_79_L, 0x6000) == 0x4000)
        {
            //printf("\033[0;31m [%s][%d] cal ok, break  \033[0m\n", __FUNCTION__, __LINE__);
            // (10)success, refine icon value
            MS_U16 u16ICONtempCH0_1 = 0;
            MS_U16 u16ICONtempCH2_3 = 0;
            MS_U16 u16ICONtempCH4_5 = 0;
            MS_U16 u16ICONtempCH6_7 = 0;
            MS_U16 u16ICONtempCH8_9 = 0;
            MS_U16 u16ICONtempCH10_11 = 0;
            MS_U16 u16ICONtempCH12_13 = 0;
            MS_U16 u16ICONtempCH14_15 = 0;

            u16ICONtempCH0_1   = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_30_L, 0xFFFF) );
            u16ICONtempCH2_3   = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_31_L, 0xFFFF) );
            u16ICONtempCH4_5   = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_32_L, 0xFFFF) );
            u16ICONtempCH6_7   = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_33_L, 0xFFFF) );
            u16ICONtempCH8_9   = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_34_L, 0xFFFF) );
            u16ICONtempCH10_11 = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_35_L, 0xFFFF) );
            u16ICONtempCH12_13 = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_36_L, 0xFFFF) );
            u16ICONtempCH14_15 = _Hal_MOD_Refine_ICON( MOD_A_R2BYTEMSK(REG_MOD_A_BK00_37_L, 0xFFFF) );

            //close reg_keep_cal_value
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7A_L, 0x0000, BIT(15));

            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_30_L, u16ICONtempCH0_1, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_31_L, u16ICONtempCH2_3, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_32_L, u16ICONtempCH4_5, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_33_L, u16ICONtempCH6_7, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_34_L, u16ICONtempCH8_9, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_35_L, u16ICONtempCH10_11, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_36_L, u16ICONtempCH12_13, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_37_L, u16ICONtempCH14_15, 0xFFFF);

            break;
        }
        else
        {
            u8CheckTimes ++;
            //printf("\033[0;31m [%s][%d] cal ng, u8CheckTimes: %d  \033[0m\n", __FUNCTION__, __LINE__, u8CheckTimes);
        }

        if (u8CheckTimes > MOD_LVDS_HW_CALI_TIME_OUT)
        {
            // (11)fail , set to default value
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_7A_L, 0x00, BIT(15));

            MS_U16 u16ICONtempDefault = ( (MOD_LVDS_ICON_DEFAULT<<8) | (MOD_LVDS_ICON_DEFAULT) );
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_30_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_31_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_32_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_33_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_34_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_35_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_36_L, u16ICONtempDefault, 0xFFFF);
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_37_L, u16ICONtempDefault, 0xFFFF);
            //printf("\033[0;31m [%s][%d] If 3 times all fail, set all pair to nominal value by disable keep mode  \033[0m\n", __FUNCTION__, __LINE__);
            break;
        }
    }

    if (u8CheckTimes <= MOD_LVDS_HW_CALI_TIME_OUT)
    {
         PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "\r\n----- Calibration ok \n");
    }
    else
    {
        PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "\r\n----- Calibration fail: 0x%x \n", MOD_R2BYTEMSK(REG_MOD_BK00_3D_L, 0x6000));
    }

    // Wait 2ms to make sure HW auto write calibration result into register
    MsOS_DelayTask(2);

    // (12) Restore each pair output configuration
    MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, u16reg_32da);
    MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, u16reg_32dc);

    // (13) Disable calibration mode
    MOD_W2BYTEMSK(REG_MOD_A_BK00_78_L, 0x00, BIT(15));         // Disable calibration function

    // With HW calibration mode, HW would cal for each channel, and each channel would get different value
    // Return channel 2 vaule
    u8cur_ibcal = MOD_A_R2BYTEMSK(REG_MOD_A_BK00_31_L, 0x00FF); // return ch2 calibration result

#if MOD_CAL_TIMER
    PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "[%s] takes %ld ms\n", __FUNCTION__, (MsOS_GetSystemTime()-delay_start_time));
#endif
    PNL_DBG(PNL_DBGLEVEL_CALIBRATION, "\r\n Calibration result= %x\n", u8cur_ibcal);

    return (u8cur_ibcal&0xFF);
#endif
}

PNL_Result MHal_PNL_MOD_Calibration(void *pInstance)
{
    MS_U8 u8Cab;
    MS_U8 u8BackUSBPwrStatus;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

    W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

    u8Cab = msModCurrentCalibration(pInstance);

    W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));

    if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type !=E_PNL_TYPE_MINILVDS)
        MOD_W2BYTEMSK(REG_MOD_A_BK00_78_L, u8Cab, 0x07);

    return E_PNL_OK;

}

static void MHal_PNL_PowerDownLPLL(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
        W2BYTEMSK(L_BK_LPLL(0x03), BIT(5), BIT(5));
    }
    else
    {
        W2BYTEMSK(L_BK_LPLL(0x03), FALSE, BIT(5));
    }
}

PNL_Result MHal_PNL_En(void *pInstance, MS_BOOL bPanelOn, MS_BOOL bCalEn)
{
    MS_U8 u8Cab;
    MS_U8 u8BackUSBPwrStatus;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "[%s][%d]\n", __FUNCTION__, __LINE__);

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u32PNL_MISC = %tx\n", (ptrdiff_t)pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "bPanelOn = %x\n", bPanelOn);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "eLPLL_Type            = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "_u8MOD_LVDS_Pair_Type = %x\n", pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG0_7       = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG8_15      = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG16_21     = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

    MS_U16 u16PortA = MOD_A_R2BYTE(REG_MOD_A_BK00_18_L);
    MS_U16 u16PortB = MOD_A_R2BYTE(REG_MOD_A_BK00_19_L);

    if((u16PortA!=0)||(u16PortB!=0))
    {
        MHal_BD_LVDS_Output_Type(pInstance, LVDS_OUTPUT_User);
    }

    if(u16PortA!=0)
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7   = MOD_A_R2BYTE(REG_MOD_A_BK00_18_L);
    if(u16PortB!=0)
        pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15  = MOD_A_R2BYTE(REG_MOD_A_BK00_19_L);

    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "==========================\n\n");
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG0_7       = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG8_15      = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15);
    PNL_DBG(PNL_DBGLEVEL_PANEL_EN, "u16OutputCFG16_21     = %x\n", pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);


    if(bPanelOn)
    {
        // The order is PanelVCC -> delay pnlGetOnTiming1() -> VOP -> MOD
        // VOP
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, 0x4000, HBMASK);

        // For Napoli compatible
        // need to wait 1ms to wait LDO stable before MOD power on
        MsOS_DelayTask(1);

        // turn on LPLL
        MHal_PNL_PowerDownLPLL(pInstance, FALSE);

        // mod power on
        MHal_MOD_PowerOn(pInstance
                        , ENABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);

        if(bCalEn)
        {

            u8BackUSBPwrStatus = R2BYTEMSK(L_BK_UTMI1(0x04), BIT(7));

            W2BYTEMSK(L_BK_UTMI1(0x04), 0x00, BIT(7));

            u8Cab = msModCurrentCalibration(pInstance);

            W2BYTEMSK(L_BK_UTMI1(0x04), u8BackUSBPwrStatus, BIT(7));

        }
        else
        {
            if( ( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) ) &&
                ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC & (MS_U32)E_APIPNL_MISC_SKIP_ICONVALUE) == FALSE))
            {
                HAL_MOD_CAL_DBG(printf("Use RCON value \n", __FUNCTION__, __LINE__));
                msSetVBY1RconValue(pInstance);
            }
            else
            {
                HAL_MOD_CAL_DBG(printf("User define Swing Value=%u\n", __FUNCTION__, __LINE__, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel));

                if(!MHal_PNL_MOD_Control_Out_Swing(pInstance, pPNLResourcePrivate->sthalPNL._u16PnlDefault_SwingLevel))
                    printf(">>Swing Level setting error!!\n");
            }
        }

        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En)
            MHal_PNL_VBY1_Hardware_TrainingMode_En(pInstance, TRUE, ENABLE);
    }
    else
    {
        // The order is LPLL -> MOD -> VOP -> delay for MOD power off -> turn off VCC

        // LPLL
        // MHal_PNL_PowerDownLPLL(TRUE); //Remove to keep op vsync if panel off

        MHal_MOD_PowerOn(pInstance
                        , DISABLE
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type
                        , pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15
                        , pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
        // VOP
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS ||
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_I ||
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_DAC_P)//(bIsLVDS)
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, 0xFF, LBMASK);
        else
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, 0x00, 0xFF);
    }

    return E_PNL_OK;
}

void MHal_PNL_SetOutputPattern(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
{
    if (bEnable)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_69_L, u16Red , 0x03FF);
        MOD_W2BYTEMSK(REG_MOD_BK00_6A_L, u16Green , 0x03FF);
        MOD_W2BYTEMSK(REG_MOD_BK00_6B_L, u16Blue , 0x03FF);
        MsOS_DelayTask(10);
        MOD_W2BYTEMSK(REG_MOD_BK00_68_L, BIT(12) , BIT(12));
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_68_L, DISABLE , BIT(12));
    }

}

void MHal_PNL_Switch_LPLL_SubBank(void *pInstance, MS_U16 u16Bank)
{
    UNUSED(u16Bank);
}

void MHal_PNL_Switch_TCON_SubBank(void *pInstance, MS_U16 u16Bank)
{
    W2BYTEMSK(L_BK_TCON(0x00), u16Bank&0xff, 0xFF);
}

MS_U16 MHal_PNL_Read_TCON_SubBank(void *pInstance)
{
    return (MS_U16)R2BYTEMSK(L_BK_TCON(0x00),0xFF);
}

MS_BOOL MHal_PNL_Is_VBY1_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0300) == 0x00)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MHal_PNL_Is_VBY1_LockN_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0100) == 0x00)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MHal_PNL_VBY1_Handshake(void *pInstance)
{
    MS_BOOL bIsLock = FALSE;

    if (MHal_PNL_Is_VBY1_Locked(pInstance) == FALSE)
    {
        MS_U16 u16CheckTimes = 0;
        //MS_U16 u16DeboundTimes = 0;

        // need to toggle vby1 packer  process start first
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, 0x00, BIT(11));
        MOD_W2BYTEMSK(REG_MOD_BK00_62_L, BIT(11), BIT(11));

        MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0F56); // set reg. initial value
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0xD6, 0x00FF); // after power on go to stand-by
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0x96, 0x00FF); // connection is established, go to Acquisition
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0xB6, 0x00FF); // when internal clock is stable, got to CDR training
        MOD_W2BYTEMSK(REG_MOD_BK00_60_L, 0xBE, 0x00FF); // enable encoder for DC blance

        while(u16CheckTimes < 10)
        {
#if 0
            u16DeboundTimes = 2;
            while ((!MHal_PNL_Is_VBY1_LockN_Locked()) && (u16DeboundTimes --))
            {
                MsOS_DelayTask(1); // can't remove
            }
#endif
            if(MHal_PNL_Is_VBY1_LockN_Locked(pInstance))
            {
                //-------------------------------------------------------------------
                // step1. Toggle clock when training
                MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0FAE);
                //--------------------------------------------------------------------
                bIsLock = TRUE;
                // pass 2 times debound to make sure VBY1 is locked
                break;
            }

            u16CheckTimes++;
            MsOS_DelayTaskUs(40);
        }

        if(bIsLock)
            {
            // step3. Disable HW check when lock done, Enable when loss lock
            //MOD_W2BYTEMSK(REG_MOD_BK00_33_L, 0x00, BIT15);

            /// Add the delay to increase time to send
            //MDrv_TIMER_Delayms(10);
        }
    }
    else
    {
        if(MOD_R2BYTEMSK(REG_MOD_BK00_60_L, 0x0FFF) != 0x0FAE)
        {
            MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0FAE);
        }
        bIsLock = TRUE;
            }

    return bIsLock;
}

MS_BOOL MHal_PNL_Is_VBY1_OC_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0C00) == 0x00)  // MOD_BK00_56_L[11:10]        for OSD
            {
        return TRUE;
    }
    else
    {
        return FALSE;
            }
}

MS_BOOL MHal_PNL_Is_VBY1_OC_LockN_Locked(void *pInstance)
{
    if (MOD_A_R2BYTEMSK(REG_MOD_A_BK00_47_L, 0x0400) == 0x00)  // MOD_BK00_56_L[11:10]        for OSD
            {
        return TRUE;
    }
    else
    {
        return FALSE;
            }
}

MS_BOOL MHal_PNL_VBY1_OC_Handshake(void *pInstance)
{
    MS_BOOL bIsLock = FALSE;

    if (MHal_PNL_Is_VBY1_OC_Locked(pInstance) == FALSE)
    {
        MS_U16 u16CheckTimes = 0;
//        MS_U16 u16DeboundTimes = 0;

        // need to toggle vby1 packer  process start first
        MOD_W2BYTEMSK(REG_MOD_BK00_66_L, 0x00, BIT(11));
        MOD_W2BYTEMSK(REG_MOD_BK00_66_L, BIT(11), BIT(11));


        MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0F56); // set reg. initial value
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0xD6, 0x00FF); // after power on go to stand-by
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0x96, 0x00FF); // connection is established, go to Acquisition
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0xB6, 0x00FF); // when internal clock is stable, got to CDR training
        MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0xBE, 0x00FF); // enable encoder for DC blance

        while(u16CheckTimes < 10)
        {
        #if 0
            u16DeboundTimes = 2;
            while ((!MHal_PNL_Is_VBY1_OC_LockN_Locked()) && (u16DeboundTimes --))
            {
            MsOS_DelayTask(1);
            }
        #endif
            if(MHal_PNL_Is_VBY1_OC_LockN_Locked(pInstance))
            {
                //-------------------------------------------------------------------
                // step1. Toggle clock when training

                MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0FAE);
                bIsLock = TRUE;
                // pass 2 times debound to make sure VBY1 is locked
                break;
            }

            u16CheckTimes++;
            MsOS_DelayTaskUs(40);
        }

        if(bIsLock)
        {
            // step3. Disable HW check when lock done, Enable when loss lock
//            MOD_W2BYTEMSK(REG_MOD_BK00_33_L, 0x00, BIT15);
        }
    }
    else
    {
        if(MOD_R2BYTEMSK(REG_MOD_BK00_64_L, 0x0FFF) != 0x0FAE)
        {
            MOD_W2BYTE(REG_MOD_BK00_64_L, 0x0FAE);
        }
        bIsLock = TRUE;
    }

    return bIsLock;
}

MS_BOOL MHal_PNL_IsYUVOutput(void *pInstance)
{
   return FALSE;
}

MS_BOOL MHal_PNL_SetOutputInterlaceTiming(void *pInstance, MS_BOOL bEnable)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if (bEnable == TRUE)
    {
        //interlace output vtotal modify
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L, BIT(9), BIT(9));

        // two different interlace information through channel A reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_05_L, BIT(12), BIT(12));
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, BIT(2), BIT(2));
        // two different interlace information through channel B reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, BIT(3), BIT(3));
    }
    else
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L  , 0, BIT(9));
        // two different interlace information through channel A reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_05_L, 0x0000, BIT(12));
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, BIT(2));
        // two different interlace information through channel B reserved bit
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, BIT(3));
    }

    return TRUE;
}

MS_BOOL MHal_PNL_GetOutputInterlaceTiming(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    MS_BOOL bIsInterlaceOutput = FALSE;
    //interlace output vtotal modify
    if (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L, BIT(9)) == BIT(9))
    {
        if (   (MOD_R2BYTEMSK(REG_MOD_BK00_05_L, BIT(12)) == (BIT(12)))
            || (MOD_R2BYTEMSK(REG_MOD_BK00_51_L, (BIT(2) | BIT(3))) == (BIT(2)|BIT(3))))
        {
            bIsInterlaceOutput = TRUE;
        }
    }
    else
    {
        bIsInterlaceOutput = FALSE;
    }
    return bIsInterlaceOutput;
}

////Ext LPLL setting
static void _MHal_PNL_Init_ExtLPLL(void *pInstance, PNL_TYPE eLPLL_Type,PNL_MODE eLPLL_Mode,MS_U64 ldHz)
{
    E_PNL_SUPPORTED_LPLL_EXT_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;

    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz, E_PNL_LPLL_OSD);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
    {
        printf("Not Supported LPLL Type, skip LPLL Init\n");
        return;
    }

    _MHal_PNL_DumpLPLLTable(pInstance, u8SupportedLPLLLIndex, E_PNL_LPLL_OSD);
}

static MS_U8 _MHal_PNL_Get_ExtLPLL_LoopDIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_div = 0;
    E_PNL_SUPPORTED_LPLL_EXT_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,u8LPLL_Mode,ldHz,E_PNL_LPLL_OSD);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
    {
        u16loop_div = 0 ;
    }
    else
    {
        u16loop_div = u16EXT_LoopDiv[u8SupportedLPLLLIndex];
    }

    u16loop_div *= 2;
    return u16loop_div;
}

static MS_U8 _MHal_PNL_Get_ExtLPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz)
{
    MS_U16 u16loop_gain = 0;
    E_PNL_SUPPORTED_LPLL_EXT_TYPE u8SupportedLPLLLIndex = E_PNL_SUPPORTED_LPLL_EXT_MAX;
    u8SupportedLPLLLIndex = _MHal_PNL_GetSupportedLPLLIndex(pInstance, eLPLL_Type,eLPLL_Mode,ldHz,E_PNL_LPLL_OSD);

    if (u8SupportedLPLLLIndex == E_PNL_SUPPORTED_LPLL_EXT_MAX)
    {
        u16loop_gain = 0 ;
    }
    else
    {
        u16loop_gain = u16EXT_LoopGain[u8SupportedLPLLLIndex];
    }
    return u16loop_gain;
}


// Output Dclk
void MHal_PNL_CalExtLPLLSETbyDClk(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 u8LPLL_Type, MS_U64 ldHz)
{

    MS_U64 u64LdPllSet = 0;
    MS_U64 u64DclkFactor = 0;
    MS_U32 u32Div = 0;
    // loop div and loop gain use default parameters to avoid dclk floating out of range and getting wrong value
    MS_U32 u32Factor = 10;

    _MHal_PNL_Init_ExtLPLL(pInstance, u8LPLL_Type, u8LPLL_Mode, ldHz);

    //the first " *2 " is from  the dual mode
    u32Div=(MS_U32)(_MHal_PNL_Get_ExtLPLL_LoopDIV(pInstance, u8LPLL_Mode, u8LPLL_Type, ldHz));
    u64DclkFactor=((MS_U64)LVDS_MPLL_CLOCK_MHZ * (MS_U64)524288 * (MS_U64)_MHal_PNL_Get_ExtLPLL_LoopGain(pInstance, u8LPLL_Mode, u8LPLL_Type, ldHz));
    u64LdPllSet = (u64DclkFactor * 1000000 * u32Factor *2) + ((ldHz * u32Div) >> 1);
    do_div(u64LdPllSet, ldHz);
    do_div(u64LdPllSet, u32Div);

    W4BYTE(L_BK_LPLL(0x48), (MS_U32)u64LdPllSet);
    //printf("MHal_PNL_CalExtLPLLSETbyDClk u32KHz = %u, u32LpllSet = %x\n", ldHz, (MS_U32)u64LdPllSet);

}

void MHal_PNL_SetOSDCOutputType(void *pInstance, PNL_TYPE eLPLL_Type, E_PNL_OSDC_OUTPUT_FORMAT eOC_OutputFormat)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_TYPE eVideo_LPLL_Type = pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type;

    //load clk table: CLKGEN0SettingTBL / CLKGEN2SettingTBL
    // in Maserati : the OSD path just in VBY1
    if(( E_PNL_LPLL_VBY1_10BIT_4LANE == eLPLL_Type ) ||
       ( E_PNL_LPLL_VBY1_8BIT_4LANE  == eLPLL_Type ) ||
       ( E_PNL_LPLL_VBY1_10BIT_2LANE == eLPLL_Type ) ||
       ( E_PNL_LPLL_VBY1_8BIT_2LANE  == eLPLL_Type ))
    {
        E_PNL_SUPPORTED_CLK_TYPE eCLKType= _MHal_Transfer_PanelType_To_CLKType_OSD(eLPLL_Type, 0, 0);
        _MHal_PNL_DumpOSDClkTable(eCLKType);
    }

    // VBy1 co-registers
    if(  IsVBY1(eLPLL_Type) )
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_2E_L, BIT(10), BIT(10)); //[10]enable osd lvds path
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(0), BIT(0)); //sw reset
        MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(14));
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, (BIT(2)|BIT(3)|BIT(4)) );

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_77_L, BIT(0), BIT(0)); //[0]sw_rst

        // swith osd lpllset source
        // 0: osd with video lpllset
        // 1: osd with osd lpllset
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, BIT(10), BIT(10));

        // DIG clk en(OSD)
        // reg.6F [3:0]: reg_ckg_mod_sr_rclk_osd
        // reg.6F [7:4]: reg_ckg_mod_sr_rclk_pre_osd
        // reg.6F [11:8]: reg_ckg_mod_sr_rclk_final_osd
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_6F_L, 0x0444, 0x0FFF);

        //-------------------------------------
        //## pe
//        MOD_A_W2BYTE(REG_MOD_A_BK00_30_L, 0x3fff);
//        MOD_W2BYTE(REG_MOD_BK00_23_L, 0x7000);
//        MOD_W2BYTE(REG_MOD_BK00_24_L, 0x7fff);
//        MOD_W2BYTE(REG_MOD_BK00_25_L, 0x003f);

        MOD_W2BYTE(REG_MOD_BK00_5A_L, 0x8f3f); //[15]all dk scr[13:8]aln_de_cnt [7:0] aln_pix_cnt

        // maserati only
        // HW force setting free swap in OSD
        // in case of 8v4o: 0 1 2 3 8 9 A B ( video ) OSD0 OSD1 OSD2 OSD3 (OSD)
        // in case of 4v2o: 0 1 x x 8 9 x x ( video ) OSD0 OSD1 x    x    (OSD)
        // so 4v2o: need to add free swap setting ( 0x5410 0xffff ) to put video lane together
        // 4v2o: 0 1 8 9 x x x x ( video ) OSD0 OSD1 x    x    (OSD)
        MOD_W2BYTEMSK(REG_MOD_BK00_7C_L, BIT(0), BIT(0));

    }

    if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, BIT(13),BIT(13));

        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0055); //[7:0]reg_output_conf[27:16]

        MOD_W2BYTEMSK(REG_MOD_BK00_2E_L, 0x0000, BIT(10)); //[10]enable osd lvds path
        MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(0), BIT(0)); //sw reset
        MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(14));
        MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, (BIT(2)|BIT(3)|BIT(4)) );

        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, BIT(13),BIT(13));

        //MOD_W2BYTE(REG_MOD_BK00_71_L, 0xffff);  //can not find in Manhattan register table

        //[13]reg_seri_auto_fix_osd : enable osd serializer auto fix read/write point mis-balance
        //[14]reg_seri_osd_mod:for OSD, switch chanel 8~13 as OSD path
        //[15]reg_dbg_status_switch:switch debug information(reg_dbg_status_sel/reg_dbg_status) between Video/OSD path     0: Video       1: OSD
        MOD_W2BYTEMSK(REG_MOD_BK00_76_L, (BIT(14)|BIT(13)) , (BIT(15)|BIT(14)|BIT(13)) );

        //[0]reg_seri_enable:enable serializer function
        //[1]reg_seri_auto_fix:enable serializer auto fix read/write point mis-balance
        //[2]reg_fix_cnt_clr
        //[3]reg_dbg_status_sel:select debug status, read status from reg_dbg_status[15:0] 0: auto fix cnt         1: point diff value
        //[15..8]reg_tester_pix_ext: test pixel extension for 16bit serializer
        MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x0003 , 0xFF0F);

        MOD_W2BYTE(REG_MOD_BK00_5A_L, 0x8f3f);
        //-------------------------------------
        //## icon (Swing)
        MOD_A_W2BYTE(REG_MOD_A_BK00_34_L, 0x7f7f);
        MOD_A_W2BYTE(REG_MOD_A_BK00_35_L, 0x7f7f);
        MOD_A_W2BYTE(REG_MOD_A_BK00_36_L, 0x0000);

        // vby1 osd 4 lane
        //[15]proc_st[13:12]byte_mode 4 byte mode[6]2ch_vby1_osd[9]swap
        MOD_W2BYTEMSK(REG_MOD_BK00_5B_L, 0xa260 , 0xFFE0);
    }
    else if( eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_2LANE)
    {
        MOD_A_W2BYTEMSK(REG_MOD_A_BK00_10_L, BIT(13),BIT(13));
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, 0x0005);

        //-------------------------------------
        //## icon (Swing)
        MOD_A_W2BYTE(REG_MOD_A_BK00_34_L, 0x7f7f);
        MOD_A_W2BYTE(REG_MOD_A_BK00_35_L, 0x0000);
        MOD_A_W2BYTE(REG_MOD_A_BK00_36_L, 0x0000);

        //vby1 osd 2 lane
        //[15]proc_st[13:12]byte_mode 4 byte mode[6]2ch_vby1_osd[9]swap
        MOD_W2BYTEMSK(REG_MOD_BK00_5B_L, 0xa240 , 0xFFE0);
    }

    // Control VBY1 output format and bit orders
    switch(eOC_OutputFormat)
    {
        case E_PNL_OSDC_OUTPUT_FORMAT_VBY1_ARGB1:
            MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0x0000, BIT(10));
            break;

        case E_PNL_OSDC_OUTPUT_FORMAT_VBY1_ARGB2:
            MOD_W2BYTEMSK(REG_MOD_BK00_64_L, BIT(10), BIT(10));
            break;

        default:
            printf("OSDC output format uses default value\n");
            MOD_W2BYTEMSK(REG_MOD_BK00_64_L, 0x0000, BIT(10));
            break;
    }

    //maserati : force set VBY1 OSD default free swap
    if( (IsVBY1(eLPLL_Type)) && ( APIPNL_OUTPUT_CHANNEL_ORDER_DEFAULT == pPNLResourcePrivate->stdrvPNL._stPnlInitData.u8OutputOrderType ) )
    {
        if(( E_PNL_LPLL_VBY1_10BIT_8LANE == eVideo_LPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_8LANE == eVideo_LPLL_Type ))
        {
            if(( E_PNL_LPLL_VBY1_10BIT_4LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_4LANE  == eLPLL_Type ))
            {
                // 8v4o
                MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0x7632,0xBA98,0xFFFF);
                _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0x7632,0xBA98,0xFFFF);
            }
            else if(( E_PNL_LPLL_VBY1_10BIT_2LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_2LANE  == eLPLL_Type ))
            {
                // 8v2o
                MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0x7632,0xFF98,0xFFFF);
                _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0x7632,0xFF98,0xFFFF);
            }
        }
        else if(( E_PNL_LPLL_VBY1_10BIT_4LANE == eVideo_LPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_4LANE == eVideo_LPLL_Type ))
        {
            if(( E_PNL_LPLL_VBY1_10BIT_4LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_4LANE  == eLPLL_Type ))
            {
                // 4v4o
                MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0xFFFF,0xBA98,0xFFFF);
                _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0xFFFF,0xBA98,0xFFFF);
            }
            else if(( E_PNL_LPLL_VBY1_10BIT_2LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_2LANE  == eLPLL_Type ))
            {
                // 4v2o
                MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0xFFFF,0xFF98,0xFFFF);
                _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0x5410,0xFFFF,0xFF98,0xFFFF);
            }
        }
        else if(( E_PNL_LPLL_VBY1_10BIT_2LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_2LANE  == eLPLL_Type ))
        {
            if(( E_PNL_LPLL_VBY1_10BIT_4LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_4LANE  == eLPLL_Type ))
            {
                // 2v4o
                MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0xFF10,0xFFFF,0xBA98,0xFFFF);
                _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0xFF10,0xFFFF,0xBA98,0xFFFF);
            }
            else if(( E_PNL_LPLL_VBY1_10BIT_2LANE == eLPLL_Type ) || ( E_PNL_LPLL_VBY1_8BIT_2LANE  == eLPLL_Type ))
            {
                // 2v2o
                MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0xFF10,0xFFFF,0xFF98,0xFFFF);
                _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0xFF10,0xFFFF,0xFF98,0xFFFF);
            }
        }
        else
        {
            // error
            MHal_Output_Channel_Order(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0xFFFF,0xFFFF,0xFFFF,0xFFFF);
            _MHal_PNL_Set_Clk(pInstance,APIPNL_OUTPUT_CHANNEL_ORDER_USER,0xFFFF,0xFFFF,0xFFFF,0xFFFF);
        }
    }
}

MS_BOOL MHal_PNL_SetOSDSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    MS_U16 u16Span;
    MS_U16 u16Step;
    MS_U32 u32PLL_SET;/// = MDrv_Read3Byte(L_BK_LPLL(0x0F));

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x48));
    // Set SPAN
    if(u16Fmodulation < 200 || u16Fmodulation > 400)
        u16Fmodulation = 300;
    u16Span =( ( (((MS_U32)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR ) / (u16Fmodulation) ) * 10000) / ((MS_U32)u32PLL_SET) ) ;

    // Set STEP
    if(u16Rdeviation > 300)
        u16Rdeviation = 300;
    u16Step = ((MS_U32)u32PLL_SET*u16Rdeviation) / ((MS_U32)u16Span*10000);

    W2BYTE(L_BK_LPLL(0x4E), u16Step & 0x0FFF);// LPLL_STEP
    W2BYTE(L_BK_LPLL(0x4F), u16Span & 0x3FFF);// LPLL_SPAN
    W2BYTEMSK((L_BK_LPLL(0x4E)), (bEnable << 15), BIT(15)); // Enable ssc


    return TRUE;
}

void MHal_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable)
{
    //printf("bEnable = %d\n", bEnable);
    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    W2BYTEMSK((L_BK_LPLL(0x4E)), (bEnable << 15), BIT(15)); // Enable ssc
}

void MHal_PNL_Set_T3D_Setting(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_55_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width, 0x1FFF);//pixel width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_66_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height, 0x1FFF);//reg_col_height
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_51_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width, 0x1FFF);//reg_ln_width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_52_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height, 0x1FFF);//reg_col_height
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_61_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width, 0x3FFF);//reg_ln_width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_62_L, pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Height, 0x1FFF);//reg_col_height

    //per designer, should always enable t3d, since it will affect osd/video's pipeline
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_61_L, BIT(0), BIT(0));//Enable Depth Render, for osd pipe line adjustment
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_08_L, BIT(4), BIT(4));//mtv bypass mode
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_57_L,(BIT(0)|BIT(1)),(BIT(0)|BIT(1)));//T3D fix subde enable, fix for T3D/PIP conflict issue (bit 0)     Bug Fix miu eco (bit 1)

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_63_L, 0x00, BIT(0)); // default disable T3D SRAM
}

void MHal_PNL_Set_Device_Bank_Offset(void *pInstance)
{
    UNUSED(pInstance);
    memset(u32PNL_XCDeviceBankOffset, 0, sizeof(MS_U32)*E_PNL_DEVICE_ID_NUM);
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_0] = E_HALPNL_DEVICE0_XC_BANK_OFFSET; // Set SC0 reg bank offset
    u32PNL_XCDeviceBankOffset[E_PNL_DEVICE_ID_1] = E_HALPNL_DEVICE1_XC_BANK_OFFSET; // Set SC2 reg bank offset
}

void MHal_PNL_Init(void *pInstance)
{
    // Do nothing
    //UNUSED(pInstance);
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // STGEN reset
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK68_50_L, BIT(3), BIT(3));
}

void MHal_PNL_Bringup(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    ///patch for bring up
    if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)
    {
    }
    else if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_8LANE)
    {
    }

}

MS_U16 MHal_PNL_GetPanelVStart(void)
{
    return 8;
}

MS_BOOL MHal_PNL_Check_VBY1_Handshake_Status(void *pInstance)
{
    if(MOD_R2BYTEMSK(REG_MOD_BK00_60_L, 0x0FFF) != 0xFAE)
    {
        //printf("VBY1 handshake return because the reg value is 0x%u, not 0xFAE.\n", MOD_R2BYTEMSK(REG_MOD_BK00_60_L, 0x0FFF));
        return FALSE;
    }
    else
    {
        //printf("VBY handshake check success.\n");
        return TRUE;
    }
}

void MHal_PNL_ChannelFIFOPointerADjust(void *pInstance)
{
//do not support in Maserati
}

void MHal_PNL_VBY1_Hardware_TrainingMode_En(void *pInstance, MS_BOOL bIsVideoMode ,MS_BOOL bEnable)
{
    if(bIsVideoMode)
    {
        if(bEnable)
        {
            MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0AAE);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0AA6);
        }
    }
    else
    {
        if(bEnable)
        {
            MOD_W2BYTE(REG_MOD_BK00_59_L, 0x0AAE);
        }
        else
        {
            MOD_W2BYTE(REG_MOD_BK00_59_L, 0x0AA6);
        }
    }
}

MS_BOOL MHal_PNL_VBY1_IsSupport_Hardware_TrainingMode(void *pInstance)
{
    #ifdef SUPPORT_VBY1_HWTRAINING_MODE
        return TRUE;
    #else
        return FALSE;
    #endif
}

void MHal_PNL_TCON_Patch(void)
{
    // MOD sw reset
    MOD_W2BYTEMSK(REG_MOD_BK00_00_L, 0x0000, BIT(0)); //sw reset
    MOD_W2BYTEMSK(REG_MOD_BK00_2E_L, 0x0000, BIT(10)); //[10]enable osd lvds path
    MOD_W2BYTEMSK(REG_MOD_BK00_54_L, 0x0000, BIT(14));
    MOD_W2BYTEMSK(REG_MOD_BK00_51_L, 0x0000, (BIT(2)|BIT(3)|BIT(4)) );

    MOD_W2BYTEMSK(REG_MOD_BK00_00_L, BIT(0), BIT(0)); //sw reset


    // Setting TCON signal through MOD PAD
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x1818);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x9818);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x0000);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x1928);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x9928);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x0000);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x1a38);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x9a38);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x0000);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x1b78);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x9b78);
    MOD_A_W2BYTE(REG_MOD_A_BK00_6A_L,0x0000);
}

static MS_BOOL _Hal_MOD_External_eFuse(void)
{
#ifdef MOD_EFUSE_IN_MBOOT
    return TRUE;
#else
    return FALSE;
#endif
}

static void _Hal_MOD_VB1_CH_SWICH(PNL_TYPE eLPLL_Type_Ext)
{
    //SC4 7C[11]:reg_vby1_4ch / enable 4ch vx1mode
    //SC4 7C[12]:reg_vby1_16v4o / vby1 16 vedio 4 osd mode
    //SC4 7C[13]:reg_vby1_8v4o_mode / vby1 8 vedio 4 osd mode
    //SC4 7C[14]:reg_vby1_8ch / enable 8ch vx1mode
    //SC4 7C[15]:reg_vby1_16ch / enable 16ch vx1mode
    if( (eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE) || (eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE) )
    {// 4 lane
        MOD_W2BYTEMSK(REG_MOD_BK00_7C_L, BIT(11), 0xF800);
    }
    else if( (eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE) || (eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE) )
    {// 8 lane
        MOD_W2BYTEMSK(REG_MOD_BK00_7C_L, BIT(14), 0xF800);
    }
    else if( (eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE) || (eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE) )
    {// 16 lane
        MOD_W2BYTEMSK(REG_MOD_BK00_7C_L, BIT(15), 0xF800);
    }
    else
    {// 2 lane
        MOD_W2BYTEMSK(REG_MOD_BK00_7C_L, 0x0000, 0xF800);
    }
}

static MS_U16 _Hal_MOD_Refine_ICON(MS_U16 u16ICON)
{
    MS_U16 u16ICON_L = (u16ICON & 0x00FF);
    MS_U16 u16ICON_H = ((u16ICON & 0xFF00)>>8);
    MS_U16 u16Result = 0;
    if( (u16ICON_L > MOD_LVDS_ICON_HIGH_LIMIT) || (u16ICON_L < MOD_LVDS_ICON_LOW_LIMIT) )
    {
        u16Result = MOD_LVDS_ICON_DEFAULT;
    }
    else
    {
        u16Result = u16ICON_L;
    }

    if( (u16ICON_H > MOD_LVDS_ICON_HIGH_LIMIT) || (u16ICON_H < MOD_LVDS_ICON_LOW_LIMIT) )
    {
        u16Result |= (MOD_LVDS_ICON_DEFAULT<<8);
    }
    else
    {
        u16Result |= (u16ICON_H<<8);
    }
    return u16Result;
}

static void _MHal_PNL_DumpVideoClkTable(MS_U8 u8CLKTblIndex)
{
    // step1. load CLK GEN0 table
    if (u8CLKTblIndex == E_PNL_SUPPORTED_CLK_MAX)
    {
        printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
        return;
    }

    int indexCounter = 0;

    for(indexCounter = 0 ; indexCounter<CLK_GEN0_REG_VIDEO_NUM; indexCounter++)
    {
        if (CLKGEN0SettingTBL_Video[u8CLKTblIndex][indexCounter].address == 0xFF) //delay in micro second
        {
            MsOS_DelayTaskUs(CLKGEN0SettingTBL_Video[u8CLKTblIndex][indexCounter].value);
            continue; // step forward to next register setting.
        }

        W2BYTEMSK(L_CLKGEN0(CLKGEN0SettingTBL_Video[u8CLKTblIndex][indexCounter].address),
                  CLKGEN0SettingTBL_Video[u8CLKTblIndex][indexCounter].value,
                  CLKGEN0SettingTBL_Video[u8CLKTblIndex][indexCounter].mask);
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, u8CLKTblIndex);

    // step2. load CLK GEN2 table
    if (u8CLKTblIndex == E_PNL_SUPPORTED_CLK_MAX)
    {
        printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
        return;
    }

    for(indexCounter = 0 ; indexCounter<CLK_GEN2_REG_VIDEO_NUM; indexCounter++)
    {
        if (CLKGEN2SettingTBL_Video[u8CLKTblIndex][indexCounter].address == 0xFF) //delay in micro second
        {
            MsOS_DelayTaskUs(CLKGEN2SettingTBL_Video[u8CLKTblIndex][indexCounter].value);
            continue; // step forward to next register setting.
        }

        W2BYTEMSK(L_CLKGEN2(CLKGEN2SettingTBL_Video[u8CLKTblIndex][indexCounter].address),
                  CLKGEN2SettingTBL_Video[u8CLKTblIndex][indexCounter].value,
                  CLKGEN2SettingTBL_Video[u8CLKTblIndex][indexCounter].mask);
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, u8CLKTblIndex);
}

static void _MHal_PNL_DumpOSDClkTable(MS_U8 u8CLKTblIndex)
{
    // step1. load CLK GEN0 table
    if (u8CLKTblIndex == E_PNL_SUPPORTED_CLK_MAX)
    {
        printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
        return;
    }

    int indexCounter = 0;

    for(indexCounter = 0 ; indexCounter<CLK_GEN0_REG_OSD_NUM; indexCounter++)
    {
        if (CLKGEN0SettingTBL_OSD[u8CLKTblIndex][indexCounter].address == 0xFF) //delay in micro second
        {
            MsOS_DelayTaskUs(CLKGEN0SettingTBL_OSD[u8CLKTblIndex][indexCounter].value);
            continue; // step forward to next register setting.
        }

        W2BYTEMSK(L_CLKGEN0(CLKGEN0SettingTBL_OSD[u8CLKTblIndex][indexCounter].address),
                  CLKGEN0SettingTBL_OSD[u8CLKTblIndex][indexCounter].value,
                  CLKGEN0SettingTBL_OSD[u8CLKTblIndex][indexCounter].mask);
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, u8CLKTblIndex);

    // step2. load CLK GEN2 table
    if (u8CLKTblIndex == E_PNL_SUPPORTED_CLK_MAX)
    {
        printf("[%s,%5d] Unspported LPLL Type, skip LPLL setting\n",__FUNCTION__,__LINE__);
        return;
    }

    for(indexCounter = 0 ; indexCounter<CLK_GEN2_REG_OSD_NUM; indexCounter++)
    {
        if (CLKGEN2SettingTBL_OSD[u8CLKTblIndex][indexCounter].address == 0xFF) //delay in micro second
        {
            MsOS_DelayTaskUs(CLKGEN2SettingTBL_OSD[u8CLKTblIndex][indexCounter].value);
            continue; // step forward to next register setting.
        }

        W2BYTEMSK(L_CLKGEN2(CLKGEN2SettingTBL_OSD[u8CLKTblIndex][indexCounter].address),
                  CLKGEN2SettingTBL_OSD[u8CLKTblIndex][indexCounter].value,
                  CLKGEN2SettingTBL_OSD[u8CLKTblIndex][indexCounter].mask);
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]LPLLTblIndex=%u\n", __FUNCTION__, __LINE__, u8CLKTblIndex);
}

static E_PNL_SUPPORTED_CLK_TYPE _MHal_Transfer_PanelType_To_CLKType(PNL_TYPE eLPLL_Type, PNL_MODE eLPLL_Mode)
{
    E_PNL_SUPPORTED_CLK_TYPE eCLKTypeResult = E_PNL_SUPPORTED_CLK_TTL;

    switch(eLPLL_Type)
    {
        case E_PNL_TYPE_TTL:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_TTL;
        break;

        case E_PNL_TYPE_LVDS:
            if(eLPLL_Mode == E_PNL_MODE_SINGLE)
                eCLKTypeResult = E_PNL_SUPPORTED_CLK_LVDS_1CH;
            else
                eCLKTypeResult = E_PNL_SUPPORTED_CLK_LVDS_2CH;
        break;

        case E_PNL_TYPE_HS_LVDS:
            if(eLPLL_Mode == E_PNL_MODE_SINGLE)
                eCLKTypeResult = E_PNL_SUPPORTED_CLK_HS_LVDS_1CH;
            else
                eCLKTypeResult = E_PNL_SUPPORTED_CLK_HS_LVDS_2CH;
        break;

        case E_PNL_LPLL_VBY1_10BIT_16LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_16CH_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_16LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_16CH_8BIT;
        break;

        case E_PNL_LPLL_VBY1_10BIT_8LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_8CH_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_8LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_8CH_8BIT;
        break;

        case E_PNL_LPLL_VBY1_10BIT_4LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_4CH_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_4LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_4CH_8BIT;
        break;

        case E_PNL_LPLL_VBY1_10BIT_2LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_2CH_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_2LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_2CH_8BIT;
        break;

        case E_PNL_LPLL_VBY1_10BIT_1LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_1CH_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_1LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_1CH_8BIT;
        break;

        default:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_TTL;
        break;
    }
    return eCLKTypeResult;
}

static E_PNL_SUPPORTED_CLK_TYPE _MHal_Transfer_PanelType_To_CLKType_OSD(PNL_TYPE eLPLL_OSD_Type,PNL_TYPE eLPLL_Video_Type,PNL_MODE eLPLL_Mode)
{
    E_PNL_SUPPORTED_CLK_TYPE eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT;

    switch(eLPLL_OSD_Type)
    {
        case E_PNL_LPLL_VBY1_10BIT_4LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_8V4O_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_4LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_8V4O_8BIT;
        break;

        case E_PNL_LPLL_VBY1_10BIT_2LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_4V2O_10BIT;
        break;

        case E_PNL_LPLL_VBY1_8BIT_2LANE:
            eCLKTypeResult = E_PNL_SUPPORTED_CLK_VBY1_4V2O_8BIT;
        break;

        default:
            eCLKTypeResult = _MHal_Transfer_PanelType_To_CLKType(eLPLL_Video_Type, eLPLL_Mode);
        break;
    }
    return eCLKTypeResult;
}

#define HALFLINE_PIXEL_ALIGN 16

static void _MHal_PNL_Init_MFT(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    MS_BOOL bMFTenable = TRUE;
    MS_U16 u16halfline_pixel = 0;   //Half line pixel number of active

    if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE) || (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE) )
    {
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
        {
            // vby1 16 lane 4 division, open
            bMFTenable = TRUE;
        }
        else
        {
            // vby1 16 lane 2 division, close
            bMFTenable = FALSE;
        }
    }
    else if( (IsVBY1(pstPanelInitData->eLPLL_Type_Ext)) &&
             (pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1)) )
    {
        // vby1 1 lane / 2 lane / 4 lane / 8 lane 2 Divisoin, close
        bMFTenable = FALSE;
    }
    else
    {
        // case1. single LVDS : 4P->1P, so open
        // case2. vby1 1/4/8 division: format be changed, so need to open MFT
        // case3. dual LVDS / TCON : 4P->1P, so open
        bMFTenable = TRUE;
    }

    if(bMFTenable)
    {
        // MFT setting
        // MFT reg00[11:0] reg_halfline: Half line pixel number of active(from 0)
        // 1/2/4 division: hde/4 - 1
        // 8 division: hde/8 -1

        ///Maserati is 4P mode and the half line pixel align is 16
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width % HALFLINE_PIXEL_ALIGN)
        {
            u16halfline_pixel = ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width + (HALFLINE_PIXEL_ALIGN-1))/HALFLINE_PIXEL_ALIGN * HALFLINE_PIXEL_ALIGN)/4 -1;
        }
        else
            u16halfline_pixel = pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16Width/4 -1;

        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE )||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE )||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE ))
        {// 4k2k
            W2BYTEMSK(REG_MFT_00_L, u16halfline_pixel, 0x0FFF);   // half_line = hde/4-1 = 3840 / 4 - 1 = 959
        }
        else
        {// fullHD
            W2BYTEMSK(REG_MFT_00_L, u16halfline_pixel, 0x0FFF);   // half_line = hde/16-1 = 1920 / 16 - 1 = 479
        }

        // MFT reg01[4] reg_1ch_lvds: 1ch lvds mux
        if( ((pstPanelInitData->eLPLL_Type == E_PNL_TYPE_LVDS)&&(pstPanelInitData->eLPLL_Mode==E_PNL_MODE_SINGLE)) ||
            (pstPanelInitData->eLPLL_Type == E_PNL_TYPE_TTL) ||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_1LANE) ||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_1LANE))
        {
            // single LVDS / TTL / vby1 1lane is one ch, so open
            W2BYTEMSK(REG_MFT_01_L, BIT(4), BIT(4));
        }
        else
        {
            W2BYTEMSK(REG_MFT_01_L, 0x0000, BIT(4));
        }

        // MFT reg01[3..0] reg_dout_sel: Data output select
        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE )||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE )||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE ) )
        {
            // 4p -> 4p, so close
            W2BYTEMSK(REG_MFT_01_L, 0x0000, BIT(0));
        }
        else
        {
            //case1. 4p->2p, so open
            //case2. 4p->1p, so open
            W2BYTEMSK(REG_MFT_01_L, BIT(0), BIT(0));
        }

        // MFT reg03[15..14] reg_mode: mode of pixel sequence, 0:LR2OE, 1:LR2LRGB, 2:OE2LR
        // if 4 division, then MFT reg03[15:14]=2'b10, other default=2'b00

        // MFT reg03[11] reg_2ch_lvds: 2ch lvds mux
        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE )||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE )||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)||
            (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE ) )
        {
            // 4p -> 4p, so close
            W2BYTEMSK(REG_MFT_03_L, 0x0000, BIT(11));
        }
        else
        {
            //case1. 4p->2p, so open
            //case2. 4p->1p, so open
            if( ((pstPanelInitData->eLPLL_Type == E_PNL_TYPE_LVDS)&&(pstPanelInitData->eLPLL_Mode==E_PNL_MODE_SINGLE)) ||
                (pstPanelInitData->eLPLL_Type == E_PNL_TYPE_TTL) ||
                (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_1LANE) ||
                (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_1LANE) )
            {
                // single LVDS / TTL / vby1 1lane is one ch, so close
                W2BYTEMSK(REG_MFT_03_L, 0x00, BIT(11));
            }
            else
            {
                W2BYTEMSK(REG_MFT_03_L, BIT(11), BIT(11));
            }
        }

        if( (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE) || (pstPanelInitData->eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE) )
        {
            // 1 / 2 / 8 division
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            {
                // vby1 16 lane 4 division
                W2BYTEMSK(REG_MFT_03_L, BIT(15), (BIT(15) | BIT(14)));
            }
        }
    }


    // rule: for TCON, if MFT 03[10]=1 , then we have to set MFT 02[11:0]
    // BUT script:MFT reg02[11..0] reg_vst_dly: control delay value
    if(pstPanelInitData->eLPLL_Type == E_PNL_TYPE_LVDS)
    {
        W2BYTEMSK(REG_MFT_02_L, 0x000F, 0x0FFF);
    }

    // MFT reg01[15] reg_mft_lb_en: Enable mft for format translate
    if(bMFTenable)
    {
        W2BYTEMSK(REG_MFT_01_L, BIT(15), BIT(15));
    }
    else
    {
        W2BYTEMSK(REG_MFT_01_L, 0x0000, BIT(15));
    }

}

static MS_U8 _MHal_PNL_Get_LaneNum(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    MS_U8 u8LaneNum = 0;
    //check lane num
    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)
     ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE))
    {
        u8LaneNum = 16;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
     ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
    {
        u8LaneNum = 8;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
          ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE))
    {
        u8LaneNum = 4;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE)
          ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_2LANE))
    {
        u8LaneNum = 2;
    }
    else if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_1LANE)
          ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_1LANE))
    {
        u8LaneNum = 1;
    }
    else
    {
        u8LaneNum = 0;
    }
    return u8LaneNum;
}


static void _MHal_PNL_Auto_Set_Config(void *pInstance,
                                      MS_U16 u16OutputOrder0_3,
                                      MS_U16 u16OutputOrder4_7,
                                      MS_U16 u16OutputOrder8_11,
                                      MS_U16 u16OutputOrder12_15)
{
    //attention : This function just support vby1 now.
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));


    MS_U8   u8OutputConfigCount = 0;
    MS_U16  u16Config =0;
    MS_U8   u8Count = 0;
    MS_U8   u8LaneNum = 0;
    MS_BOOL bSkip = TRUE;
    MS_U16   u16LaneLimit = 0;

    //check lane num
    u8LaneNum = _MHal_PNL_Get_LaneNum(pInstance);
    if(u8LaneNum!=0)
    {
        bSkip = FALSE;
    }
    else
    {
        bSkip = TRUE;

        //use default config
        MHal_Output_LVDS_Pair_Setting(pInstance,
                                      pPNLResourcePrivate->sthalPNL._u8MOD_LVDS_Pair_Type,
                                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG0_7,
                                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG8_15,
                                      pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16OutputCFG16_21);
    }

    //set Lane Limit in 16 lane case
    if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)
     ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE))
    {
        // 16 lane
        u16LaneLimit = PINMAPPING_MAX_LANE + 1;
    }
    else
    {
        // 1 / 2 / 4 / 8 lane
        u16LaneLimit = PINMAPPING_MAX_LANE;

    }

    if(!bSkip)
    {
        //set output config
        u16Config = 0;
        u8OutputConfigCount = 0;
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP1 ; u8Count++ )
        {
            if( ( u16OutputOrder0_3 % PINMAPPING_EXP ) < u16LaneLimit)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder0_3 = u16OutputOrder0_3 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP2 ; u8Count++ )
        {
            if( (u16OutputOrder4_7 % PINMAPPING_EXP ) < u16LaneLimit)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder4_7 = u16OutputOrder4_7 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        MOD_A_W2BYTE(REG_MOD_A_BK00_18_L, u16Config);

        u16Config =0;
        u8OutputConfigCount = 0;
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP3 ; u8Count++ )
        {
            if( (u16OutputOrder8_11 % PINMAPPING_EXP ) < u16LaneLimit)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder8_11 = u16OutputOrder8_11 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        for( u8Count = 0 ; u8Count < LANE_NUM_EACH_PINMAPPING_GROUP4 ; u8Count++ )
        {
            if( (u16OutputOrder12_15 % PINMAPPING_EXP ) < u16LaneLimit)
            {
                u16Config += CONFIG_FOR_VBY1_DATA<<u8OutputConfigCount;
            }
            u16OutputOrder12_15 = u16OutputOrder12_15 / PINMAPPING_EXP;
            u8OutputConfigCount += CONFIG_FOR_VBY1_DATA_BIT_NUM;
        }
        MOD_A_W2BYTE(REG_MOD_A_BK00_19_L, u16Config);
    }
}

static void _MHal_PNL_Set_Clk(void *pInstance,
                              MS_U8 Type,
                              MS_U16 u16OutputOrder0_3,
                              MS_U16 u16OutputOrder4_7,
                              MS_U16 u16OutputOrder8_11,
                              MS_U16 u16OutputOrder12_15)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    if(Type == APIPNL_OUTPUT_CHANNEL_ORDER_USER )
    {
        MS_U8 u8Clk = 0;
        MS_U8   u8LaneNum = 0;
        MS_BOOL bSkip = TRUE;
        MS_U8   u8Count = 0;
        MS_U8   u8Count1 = 0;
        MS_U8   u8StartLane = 0;
        MS_U16   u16LaneLimit = 0;

        //check lane num
        u8LaneNum = _MHal_PNL_Get_LaneNum(pInstance);
        if(u8LaneNum!=0)
        {
            bSkip = FALSE;
        }
        else
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x1F, 0x1F); //open all clk
            bSkip = TRUE;
        }

        //set Lane Limit in 16 lane case
        if((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)
         ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE))
        {
            // 16 lane
            u16LaneLimit = PINMAPPING_MAX_LANE + 1;
        }
        else
        {
            // 1 / 2 / 4 / 8 lane
            u16LaneLimit = PINMAPPING_MAX_LANE;
        }

        //count clk
        if(!bSkip)
        {
            u8Clk = 0;
            u8StartLane = 0;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP1) ; u8Count++ )
            {//lane 0 - lane 3
                if( ( u16OutputOrder0_3 % PINMAPPING_EXP ) < u16LaneLimit)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder0_3 /= PINMAPPING_EXP;
            }

            u8StartLane = 4;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP2) ; u8Count++ )
            {//lane 4 - lane 7
                if( ( u16OutputOrder4_7 % PINMAPPING_EXP ) < u16LaneLimit)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder4_7 /= PINMAPPING_EXP;
            }

            u8StartLane = 8;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP3) ; u8Count++ )
            {//lane 8 - lane 11
                if( ( u16OutputOrder8_11 % PINMAPPING_EXP ) < u16LaneLimit)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder8_11 /= PINMAPPING_EXP;

            }

            u8StartLane = 12;
            for( u8Count = u8StartLane ; u8Count < (u8StartLane+LANE_NUM_EACH_PINMAPPING_GROUP4) ; u8Count++ )
            {//lane 12 - lane 15
                if( ( u16OutputOrder12_15 % PINMAPPING_EXP ) < u16LaneLimit)
                {
                    u8Count1 = 0;
                    do
                    {
                        if(u8Count>=LANE_AND_CLK_TBL[u8Count1][0] && u8Count<=LANE_AND_CLK_TBL[u8Count1][1])
                        {
                            u8Clk |= LANE_AND_CLK_TBL[u8Count1][2];
                            break;
                        }
                        u8Count1 ++;
                    }
                    while(u8Count1<VBY1_CLK_TBL_ROW);
                }
                u16OutputOrder12_15 /= PINMAPPING_EXP;
            }

            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, u8Clk, 0x1F);
        }
    }
    else
    {
        if( (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_16LANE)
          ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_16LANE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x000F, 0x001F);
        }
        else if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_8LANE)
                 ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_8LANE))
        {
            if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16MOD_CTRLA & BIT(1))
            {
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x0003, 0x001F);
            }
            else
            {
                MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x0003, 0x001F);
            }
        }
        else if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_4LANE)
                 ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_4LANE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x0001, 0x001F);
        }
        else if(   (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_10BIT_2LANE)
                 ||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type_Ext == E_PNL_LPLL_VBY1_8BIT_2LANE))
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x0001, 0x001F);
        }
        else if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)
        {//LVDS
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x001F, 0x001F);
        }
        else
        {
            MOD_A_W2BYTEMSK(REG_MOD_A_BK00_00_L, 0x001F, 0x001F);
        }
    }
}
#if defined(PATCH_HW_VTT_LIMITATION)
//There is the hareware bug on Maserati U1 Chip REG_SC_BK10_0D.
//That is replaced by REG_SC_BK68 byMHal_PNL_SetVopVtt function.
void MHal_PNL_SetVopVttByBK68(MS_U32 u32DeviceID, MS_U16 u16Vtt)
{
    MS_U16 u16temp;

    u16temp  = SC_R2BYTE(u32DeviceID, REG_SC_BK68_31_L);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK68_31_L, (u16temp & ~BIT(0) & ~BIT(8)) | BIT(1), 0xFFFF);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK68_32_L, 0xff01 , 0xFFFF);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK68_38_L, 10, 0xFFFF);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK68_36_L, u16Vtt , 0xFFFF);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK68_37_L, u16Vtt , 0xFFFF);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK0F_55_L, 0x10 , 0xFFFF);
    u16temp  = SC_R2BYTE(u32DeviceID, REG_SC_BK68_31_L);
    SC_W2BYTEMSK(u32DeviceID, REG_SC_BK68_31_L, (u16temp & ~BIT(8))|BIT(0) | BIT(1) , 0xFFFF);
}
#endif

#endif


