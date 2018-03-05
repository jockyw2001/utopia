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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// file    Mdrv_ld.c
/// @brief  local dimming Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _MDRV_LD_C_

#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "halCHIP.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#ifndef UNUSED
#define UNUSED(x)  ((x) = (x))
#endif

#ifndef ASSERT
#define ASSERT(arg) do {\
                        if (!(arg)) {\
                            printf("BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #arg);\
                        }\
                    } while (0)
#endif

#define H_SCACLING_TABLE_SIZE   128
#define V_SCACLING_TABLE_SIZE   128
#define COMPENSATION_LENGTH     512

#if  (LD_ENABLE == 1)
#include "mdrv_ld_compensation.h"
#include "mdrv_ld_edge2d.h"

#define LD_LED_NUMBER   (((MS_U16)pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH) * pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV)
#define LD_PANEL_WIDTH  pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u16PanelWidth // 1920
#define LD_PANEL_HEIGHT pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u16PanelHeight // 1080


#define EDGE2D_PACKET_LENGTH      16
#define H_V_RATIO_CONST           1048576ul

// When mspi_internal_clk=27M
// DIV_2   = 0.074us
// DIV_4   = 0.148us
// DIV_8   = 0.296us
// DIV_16  = 0.592us
// DIV_32  = 1.185us
// DIV_64  = 2.370us
// DIV_128 = 4.740us
// DIV_256 = 9.481us

// LG spec:
// SPI clock:           1.66us ~ 10us   ==> MSPI_CLK_SPD__CPU_DIV_256
// V sync to first clk: 20us ~ 50u5     ==> 20 = Tr_Start=16, Tr_End=0
// Between byte to byte: 5 ~ 20         ==> 10 = Tr_Start=5, Tr_End=0
static ST_DRV_LD_INFO _stDrvLDInfo_Defalut =
{
    1920, 1080, // Panel size

    4, // u8LedNumH
    2, // u8LedNumV

    32, 32, // Backlight

    Tbl_LD_Compensation_LG37inch_LR10,
#if !defined (__aarch64__)
    0,
#endif

    Tbl_LD_Edge2D_LG37inch_LR10,
#if !defined (__aarch64__)
    0,
#endif
   // MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    10, // u8SpiTime_VSyncWidth
    20, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG37inch_LR10 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    5, // u8LedNumV

    16, 10, // Backlight

    Tbl_LD_Compensation_LG37inch_LR10,
#if !defined (__aarch64__)
    0,
#endif
    Tbl_LD_Edge2D_LG37inch_LR10,
#if !defined (__aarch64__)
    0,
#endif
   // MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    10, // u8SpiTime_VSyncWidth
    20, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG42inch_LR16 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    8, // u8LedNumV

    24, 16, // Backlight

    Tbl_LD_Compensation_LG42inch_LR16,
#if !defined (__aarch64__)
    0,
#endif
    Tbl_LD_Edge2D_LG42inch_LR16,
#if !defined (__aarch64__)
    0,
#endif
    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG55inch_LR12 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    6, // u8LedNumV

    24, 18, // Backlight

    Tbl_LD_Compensation_LG55inch_LR12,
#if !defined (__aarch64__)
    0,
#endif
    Tbl_LD_Edge2D_LG55inch_LR12,
#if !defined (__aarch64__)
    0,
#endif
    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x85}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_CMO42inch_LR16 =
{
    1920, 1080, // Panel size

    2, // u8LedNumH
    8, // u8LedNumV

    24, 16, // Backlight

    Tbl_LD_Compensation_CMO42inch_LR16,
#if !defined (__aarch64__)
    0,
#endif
    Tbl_LD_Edge2D_CMO42inch_LR16,
#if !defined (__aarch64__)
    0,
#endif
    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

static ST_DRV_LD_INFO _stDrvLDInfo_LD_PANEL_LG50inch_4K2K_VB1 =
{
    3840, 2160, // Panel size

    1, // u8LedNumH
    8, // u8LedNumV

    24, 16, // Backlight

    Tbl_LD_Compensation_CMO42inch_LR16,
#if !defined (__aarch64__)
    0,
#endif
    Tbl_LD_Edge2D_CMO42inch_LR16,
#if !defined (__aarch64__)
    0,
#endif
    //MSPI_CLK_SPD__DIV_64, // eMSPIClockSpeed
    0, // u8SpiClockPolarity
    0, // u8SpiClockPhase

    20, // u8SpiTime_VSyncWidth
    32, // u8SpiTime_VSync_To_FirstClock
    5, // u8SpiTime_Byte_to_Byte

    2, // SPI Command length
    {0xAA,0x83}, // SPI Command
    1, // u8CheckSumMode
};

//static MS_U8 s_au8DrvLD_SPIBuf[DRV_LD_SPI_BUF_MAX_SIZE];
//static MS_U8 s_au8DrvLD_SPI_delay[DRV_LD_SPI_BUF_MAX_SIZE];

MS_U8 au8Coeff_HScaling[128*6] =
{
    0x00,    0x00,    0x01,    0xFF,    0x00,    0x00,
    0x00,    0x00,    0x02,    0xFE,    0x00,    0x00,
    0x00,    0x00,    0x03,    0xFD,    0x00,    0x00,
    0x00,    0x00,    0x04,    0xFC,    0x00,    0x00,
    0x00,    0x00,    0x05,    0xFB,    0x00,    0x00,
    0x00,    0x00,    0x06,    0xFA,    0x00,    0x00,
    0x00,    0x00,    0x07,    0xF9,    0x00,    0x00,
    0x00,    0x00,    0x08,    0xF8,    0x00,    0x00,
    0x00,    0x00,    0x09,    0xF7,    0x00,    0x00,
    0x00,    0x00,    0x0A,    0xF6,    0x00,    0x00,
    0x00,    0x00,    0x0B,    0xF5,    0x00,    0x00,
    0x00,    0x00,    0x0C,    0xF4,    0x00,    0x00,
    0x00,    0x00,    0x0D,    0xF3,    0x00,    0x00,
    0x00,    0x00,    0x0E,    0xF2,    0x00,    0x00,
    0x00,    0x00,    0x0F,    0xF1,    0x00,    0x00,
    0x00,    0x00,    0x10,    0xF0,    0x00,    0x00,
    0x00,    0x00,    0x11,    0xEF,    0x00,    0x00,
    0x00,    0x00,    0x12,    0xEE,    0x00,    0x00,
    0x00,    0x00,    0x13,    0xED,    0x00,    0x00,
    0x00,    0x00,    0x14,    0xEC,    0x00,    0x00,
    0x00,    0x00,    0x15,    0xEB,    0x00,    0x00,
    0x00,    0x00,    0x16,    0xEA,    0x00,    0x00,
    0x00,    0x00,    0x17,    0xE9,    0x00,    0x00,
    0x00,    0x00,    0x18,    0xE8,    0x00,    0x00,
    0x00,    0x00,    0x19,    0xE7,    0x00,    0x00,
    0x00,    0x00,    0x1A,    0xE6,    0x00,    0x00,
    0x00,    0x00,    0x1B,    0xE5,    0x00,    0x00,
    0x00,    0x00,    0x1C,    0xE4,    0x00,    0x00,
    0x00,    0x00,    0x1D,    0xE3,    0x00,    0x00,
    0x00,    0x00,    0x1E,    0xE2,    0x00,    0x00,
    0x00,    0x00,    0x1F,    0xE1,    0x00,    0x00,
    0x00,    0x00,    0x20,    0xE0,    0x00,    0x00,
    0x00,    0x00,    0x21,    0xDF,    0x00,    0x00,
    0x00,    0x00,    0x22,    0xDE,    0x00,    0x00,
    0x00,    0x00,    0x23,    0xDD,    0x00,    0x00,
    0x00,    0x00,    0x24,    0xDC,    0x00,    0x00,
    0x00,    0x00,    0x25,    0xDB,    0x00,    0x00,
    0x00,    0x00,    0x26,    0xDA,    0x00,    0x00,
    0x00,    0x00,    0x27,    0xD9,    0x00,    0x00,
    0x00,    0x00,    0x28,    0xD8,    0x00,    0x00,
    0x00,    0x00,    0x29,    0xD7,    0x00,    0x00,
    0x00,    0x00,    0x2A,    0xD6,    0x00,    0x00,
    0x00,    0x00,    0x2B,    0xD5,    0x00,    0x00,
    0x00,    0x00,    0x2C,    0xD4,    0x00,    0x00,
    0x00,    0x00,    0x2D,    0xD3,    0x00,    0x00,
    0x00,    0x00,    0x2E,    0xD2,    0x00,    0x00,
    0x00,    0x00,    0x2F,    0xD1,    0x00,    0x00,
    0x00,    0x00,    0x30,    0xD0,    0x00,    0x00,
    0x00,    0x00,    0x31,    0xCF,    0x00,    0x00,
    0x00,    0x00,    0x32,    0xCE,    0x00,    0x00,
    0x00,    0x00,    0x33,    0xCD,    0x00,    0x00,
    0x00,    0x00,    0x34,    0xCC,    0x00,    0x00,
    0x00,    0x00,    0x35,    0xCB,    0x00,    0x00,
    0x00,    0x00,    0x36,    0xCA,    0x00,    0x00,
    0x00,    0x00,    0x37,    0xC9,    0x00,    0x00,
    0x00,    0x00,    0x38,    0xC8,    0x00,    0x00,
    0x00,    0x00,    0x39,    0xC7,    0x00,    0x00,
    0x00,    0x00,    0x3A,    0xC6,    0x00,    0x00,
    0x00,    0x00,    0x3B,    0xC5,    0x00,    0x00,
    0x00,    0x00,    0x3C,    0xC4,    0x00,    0x00,
    0x00,    0x00,    0x3D,    0xC3,    0x00,    0x00,
    0x00,    0x00,    0x3E,    0xC2,    0x00,    0x00,
    0x00,    0x00,    0x3F,    0xC1,    0x00,    0x00,
    0x00,    0x00,    0x40,    0xC0,    0x00,    0x00,
    0x00,    0x00,    0x41,    0xBF,    0x00,    0x00,
    0x00,    0x00,    0x42,    0xBE,    0x00,    0x00,
    0x00,    0x00,    0x43,    0xBD,    0x00,    0x00,
    0x00,    0x00,    0x44,    0xBC,    0x00,    0x00,
    0x00,    0x00,    0x45,    0xBB,    0x00,    0x00,
    0x00,    0x00,    0x46,    0xBA,    0x00,    0x00,
    0x00,    0x00,    0x47,    0xB9,    0x00,    0x00,
    0x00,    0x00,    0x48,    0xB8,    0x00,    0x00,
    0x00,    0x00,    0x49,    0xB7,    0x00,    0x00,
    0x00,    0x00,    0x4A,    0xB6,    0x00,    0x00,
    0x00,    0x00,    0x4B,    0xB5,    0x00,    0x00,
    0x00,    0x00,    0x4C,    0xB4,    0x00,    0x00,
    0x00,    0x00,    0x4D,    0xB3,    0x00,    0x00,
    0x00,    0x00,    0x4E,    0xB2,    0x00,    0x00,
    0x00,    0x00,    0x4F,    0xB1,    0x00,    0x00,
    0x00,    0x00,    0x50,    0xB0,    0x00,    0x00,
    0x00,    0x00,    0x51,    0xAF,    0x00,    0x00,
    0x00,    0x00,    0x52,    0xAE,    0x00,    0x00,
    0x00,    0x00,    0x53,    0xAD,    0x00,    0x00,
    0x00,    0x00,    0x54,    0xAC,    0x00,    0x00,
    0x00,    0x00,    0x55,    0xAB,    0x00,    0x00,
    0x00,    0x00,    0x56,    0xAA,    0x00,    0x00,
    0x00,    0x00,    0x57,    0xA9,    0x00,    0x00,
    0x00,    0x00,    0x58,    0xA8,    0x00,    0x00,
    0x00,    0x00,    0x59,    0xA7,    0x00,    0x00,
    0x00,    0x00,    0x5A,    0xA6,    0x00,    0x00,
    0x00,    0x00,    0x5B,    0xA5,    0x00,    0x00,
    0x00,    0x00,    0x5C,    0xA4,    0x00,    0x00,
    0x00,    0x00,    0x5D,    0xA3,    0x00,    0x00,
    0x00,    0x00,    0x5E,    0xA2,    0x00,    0x00,
    0x00,    0x00,    0x5F,    0xA1,    0x00,    0x00,
    0x00,    0x00,    0x60,    0xA0,    0x00,    0x00,
    0x00,    0x00,    0x61,    0x9F,    0x00,    0x00,
    0x00,    0x00,    0x62,    0x9E,    0x00,    0x00,
    0x00,    0x00,    0x63,    0x9D,    0x00,    0x00,
    0x00,    0x00,    0x64,    0x9C,    0x00,    0x00,
    0x00,    0x00,    0x65,    0x9B,    0x00,    0x00,
    0x00,    0x00,    0x66,    0x9A,    0x00,    0x00,
    0x00,    0x00,    0x67,    0x99,    0x00,    0x00,
    0x00,    0x00,    0x68,    0x98,    0x00,    0x00,
    0x00,    0x00,    0x69,    0x97,    0x00,    0x00,
    0x00,    0x00,    0x6A,    0x96,    0x00,    0x00,
    0x00,    0x00,    0x6B,    0x95,    0x00,    0x00,
    0x00,    0x00,    0x6C,    0x94,    0x00,    0x00,
    0x00,    0x00,    0x6D,    0x93,    0x00,    0x00,
    0x00,    0x00,    0x6E,    0x92,    0x00,    0x00,
    0x00,    0x00,    0x6F,    0x91,    0x00,    0x00,
    0x00,    0x00,    0x70,    0x90,    0x00,    0x00,
    0x00,    0x00,    0x71,    0x8F,    0x00,    0x00,
    0x00,    0x00,    0x72,    0x8E,    0x00,    0x00,
    0x00,    0x00,    0x73,    0x8D,    0x00,    0x00,
    0x00,    0x00,    0x74,    0x8C,    0x00,    0x00,
    0x00,    0x00,    0x75,    0x8B,    0x00,    0x00,
    0x00,    0x00,    0x76,    0x8A,    0x00,    0x00,
    0x00,    0x00,    0x77,    0x89,    0x00,    0x00,
    0x00,    0x00,    0x78,    0x88,    0x00,    0x00,
    0x00,    0x00,    0x79,    0x87,    0x00,    0x00,
    0x00,    0x00,    0x7A,    0x86,    0x00,    0x00,
    0x00,    0x00,    0x7B,    0x85,    0x00,    0x00,
    0x00,    0x00,    0x7C,    0x84,    0x00,    0x00,
    0x00,    0x00,    0x7D,    0x83,    0x00,    0x00,
    0x00,    0x00,    0x7E,    0x82,    0x00,    0x00,
    0x00,    0x00,    0x7F,    0x81,    0x00,    0x00,
    0x00,    0x00,    0x80,    0x80,    0x00,    0x00
};

MS_BOOL MDrv_XC_LD_Init(void* pInstance, EN_LD_PANEL_TYPE enLDPanelType)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LD._bIsLDPanel = FALSE;
    switch(enLDPanelType)
    {
        case E_LD_PANEL_LG37inch_LR10:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG37inch_LR10;
            pXCResourcePrivate->stdrv_LD._bIsLDPanel = TRUE;
            break;
        case E_LD_PANEL_LG42inch_LR16:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG42inch_LR16;
            pXCResourcePrivate->stdrv_LD._bIsLDPanel = TRUE;
            break;
        case E_LD_PANEL_LG55inch_LR12:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG55inch_LR12;
            pXCResourcePrivate->stdrv_LD._bIsLDPanel = TRUE;
            break;
        case E_LD_PANEL_CMO42inch_LR16:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_CMO42inch_LR16;
            pXCResourcePrivate->stdrv_LD._bIsLDPanel = TRUE;
            break;
        case E_LD_PANEL_LG50inch_4K2K_VB1:
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_LD_PANEL_LG50inch_4K2K_VB1;
            break;
        case E_LD_PANEL_DEFAULT:
            //_stDrvLDInfo = _stDrvLDInfo__defalut;
            pXCResourcePrivate->stdrv_LD._stDrvLDInfo = _stDrvLDInfo_Defalut;
            break;
        default:
        //printf("Unknown LD panel type=%u\n", enLDPanelType);
            ASSERT(enLDPanelType < E_LD_PANEL_NUMS);
            bReturn = FALSE;
            return bReturn;
    }

    pXCResourcePrivate->stdrv_LD._enLDPanelType = enLDPanelType;

    MDrv_LD_Init(pInstance);
    MDrv_LD_CommonInit(pInstance);
    MDrv_XC_LD_SetPanelType(pInstance, enLDPanelType);

    MDrv_XC_LD_ISR(pInstance);
    pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished = TRUE;
    bReturn = TRUE;
    return bReturn;
}

MS_BOOL MApi_XC_LD_Init_U2(void* pInstance, EN_LD_PANEL_TYPE enLDPanelType )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bReturn = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_LD_Init(pInstance, enLDPanelType);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_LD_Init(EN_LD_PANEL_TYPE enLDPanelType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_LD_INIT XCArgs;
    XCArgs.enLDPanelType = enLDPanelType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_LD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_XC_LD_SetMemoryAddress(void* pInstance,
                                 MS_PHY u32LDFBase0,
                                 MS_PHY u32LDFBase1,
                                 MS_PHY u32LDBBase0,
                                 MS_PHY u32LDBBase1,
                                 MS_PHY u32EDGE2DBase,
                                 MS_PHY u32LEDOffset)
{
    MS_PHY u32R2BaseAddr = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    MDrv_LD_Set_LDF_FrameBufBaseAddr(pInstance, 0, u32LDFBase0);
    MDrv_LD_Set_LDF_FrameBufBaseAddr(pInstance, 1, u32LDFBase1);
    MDrv_LD_Set_LDB_FrameBufBaseAddr(pInstance, 0, u32LDBBase0);
    MDrv_LD_Set_LDB_FrameBufBaseAddr(pInstance, 1, u32LDBBase1);
    MDrv_LD_Set_EDGE2D_BaseAddr(pInstance, u32EDGE2DBase);
    MDrv_LD_Set_LEDData_BaseOffset(pInstance, u32LEDOffset);

    //DRAM buffer address Init
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf0Base = MDrv_LD_Get_LDF_FrameBufBaseAddr(pInstance, 0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf1Base = MDrv_LD_Get_LDF_FrameBufBaseAddr(pInstance, 1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf0Base= MDrv_LD_Get_LDB_FrameBufBaseAddr(pInstance, 0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf1Base= MDrv_LD_Get_LDB_FrameBufBaseAddr(pInstance, 1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf0Base = MDrv_LD_Get_SPIData_BaseAddr(pInstance, 0) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf1Base = MDrv_LD_Get_SPIData_BaseAddr(pInstance, 1) - u32R2BaseAddr;
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32Edge2DCoeffBase = MDrv_LD_Get_EDGE2D_BaseAddr(pInstance) - u32R2BaseAddr;
}

MS_BOOL MApi_XC_LD_SetMemoryAddress_U2(void* pInstance,
                                      MS_U8 u8MIU ,
                                      MS_PHY u32LDFBase0,
                                      MS_PHY u32LDFBase1,
                                      MS_PHY u32LDBBase0,
                                      MS_PHY u32LDBBase1,
                                      MS_PHY u32EDGE2DBase,
                                      MS_PHY u32LEDOffset)
{
#if(HW_DESIGN_LD_VER > 4)
        printf("\r\n Local Dimming do not Support  this HW Version ");
        return FALSE;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_LD_Set_MIUSel(pInstance, u8MIU);

    if (u8MIU != 0x00)
    {
        u32LDFBase0 = u32LDFBase0 + HAL_MIU1_BASE;
        u32LDFBase1 = u32LDFBase1 + HAL_MIU1_BASE;
        u32LDBBase0 = u32LDBBase0 + HAL_MIU1_BASE;
        u32LDBBase1 = u32LDBBase1 + HAL_MIU1_BASE;
        u32EDGE2DBase = u32EDGE2DBase + HAL_MIU1_BASE;
    }

    MDrv_XC_LD_SetMemoryAddress(pInstance, u32LDFBase0,
                                u32LDFBase1,
                                u32LDBBase0,
                                u32LDBBase1,
                                u32EDGE2DBase,
                                u32LEDOffset);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
}

MS_BOOL MApi_XC_LD_SetMemoryAddress(MS_U8 u8MIU ,
                                    MS_PHY u32LDFBase0,
                                    MS_PHY u32LDFBase1,
                                    MS_PHY u32LDBBase0,
                                    MS_PHY u32LDBBase1,
                                    MS_PHY u32EDGE2DBase,
                                    MS_PHY u32LEDOffset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_MEMORYADDRESS XCArgs;
    XCArgs.u8MIU = u8MIU;
    XCArgs.u32LDFBase0 = u32LDFBase0;
    XCArgs.u32LDFBase1 = u32LDFBase1;
    XCArgs.u32LDBBase0 = u32LDBBase0;
    XCArgs.u32LDBBase1 = u32LDBBase1;
    XCArgs.u32EDGE2DBase = u32EDGE2DBase;
    XCArgs.u32LEDOffset = u32LEDOffset;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_MEMORYADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_XC_LD_Set_MIUSel(void* pInstance, MS_U8 u8MIU)
{
    Hal_SC_Enable_MiuMask(pInstance);
#if(HW_DESIGN_LD_VER == 1) // add
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x0C, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x00, 0x0C); // IP select
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x00, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x0C, 0x0C); // IP select
    }
#elif(HW_DESIGN_LD_VER == 2)
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF0, 0x08, 0x08); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF0, 0x00, 0x08); // IP select
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF0, 0x00, 0x08); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF0, 0x08, 0x08); // IP select
    }
#elif(HW_DESIGN_LD_VER == 3)
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x0C, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x00, 0x0C); // IP select
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF4, 0x00, 0x0C); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF4, 0x0C, 0x0C); // IP select
    }
#elif(HW_DESIGN_LD_VER == 4)
    if(u8MIU != 0x00) // MIU1
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF2, 0x40, 0x40); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF2, 0x00, 0x40); // IP select
    }
    else
    {
        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF2, 0x00, 0x40); // MIU select (Group1 BIT15)
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF2, 0x40, 0x40); // IP select
    }
#elif(HW_DESIGN_LD_VER > 4)
    {
      printf("\r\n Local Dimming do not Support  this HW Version ");
    }
#endif
    Hal_SC_Disable_MiuMask(pInstance);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u8MIU = u8MIU;
}

MS_BOOL MApi_XC_LD_Get_Value_U2(void* pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_LD_Get_Value(pInstance, pu8LDValue, u8WSize, u8HSize );
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_LD_Get_Value(MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_LD_VALUE XCArgs;
    XCArgs.pu8LDValue = pu8LDValue;
    XCArgs.u8WSize = u8WSize;
    XCArgs.u8HSize = u8HSize;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_LD_VALUE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_CommonInit(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16PanelWidth = LD_PANEL_WIDTH;
    MS_U16 u16PanelHeight = LD_PANEL_HEIGHT;
    MS_U16 u16Value = 0, u16Mask = 0;

    //LDF Init
    //horizontal low-pass filter before MAXin enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, (ENABLE<<12), BIT(12));
    if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
    {
        //blending weight parameter DC*(8-reg_alpha)+MAX*reg_alpha
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x02, 0x0F);
    }

    //pixel white mode enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, (ENABLE<<7), BIT(7));
    //Write out RGB/DC/MAX data
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, (DISABLE<<8), BIT(8));
    //LSF HSU Init
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_38_L, 0x8000 & 0xFFFFF);
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, 0x8000 & 0xFFFFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, ((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT)&(~0)), (((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT)|0)&LD_LSFCONFIG_VALID));
#elif(HW_DESIGN_LD_VER == 1)
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3B_L, 0x80,0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3B_L, 0x80<<8,0xFF00);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, ((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT|E_LD_LSF_SIGNBIT)&(~0)), ((E_LD_LSF_H_SHIFT|E_LD_LSF_V_SHIFT|E_LD_LSF_SIGNBIT)|0)&LD_LSFCONFIG_VALID);
    //vlpf enable hlpf enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3E_L, (MS_U16)(((DISABLE)<<14)|((DISABLE)<<15)), 0xC000);
    //hlpf 10-bit mode
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3D_L, (DISABLE<<14),  BIT(14));
    //LSFLPF HTapsSel
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (E_LD_LSFLPF_9TAPS<<7), 0x0180);
    //pel_offset_en
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (DISABLE<<9), BIT(9));
#endif
    // lsf out mode
    if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, (E_LD_LSF_OUT_0x3FF<<1), 0x0E);
    }
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_70_L, (u16PanelWidth-1)&0x1FFF, 0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_71_L, (u16PanelHeight-1)&0x0FFF, 0x0FFF);

    //MDrv_LD_Load_Hscaling_Table(au8Coeff_HScaling);
    //MDrv_LD_Load_Vscaling_Table(au8Coeff_HScaling);

    //Compensation Init
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (DISABLE<<6),  BIT(6));
    //pixel blending between cmpensation and ori-data
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (0x07<<4), 0x01F0);
    //comp_out_mode
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, (E_LD_COMP_OUT_NORMAL<<4), 0x0030);
#if(HW_DESIGN_LD_VER == 1)
    //Offset Init
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (DISABLE<<9), BIT(9));
    ////Compensation Init
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (DISABLE<<14), BIT(14));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, 0x00, 0x3E00);
    //Dither Init
    u16Value = (E_LD_DITHER_COMPENSATION_OUT&(~(E_LD_DITHER_MIN_SELECT|E_LD_DITHER_COMPENSATION|E_LD_DITHER_SATURATION)));
    u16Mask  = (E_LD_DITHER_COMPENSATION_OUT|(E_LD_DITHER_MIN_SELECT|E_LD_DITHER_COMPENSATION|E_LD_DITHER_SATURATION))&LD_DITHER3C_VALID;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, u16Value, u16Mask);
    //combine value and mask.
    u16Value = (0&(~E_LD_DITHER_VLPF|E_LD_DITHER_HLPF|E_LD_DITHER_OFFSET));
    u16Mask  = (0|(E_LD_DITHER_VLPF|E_LD_DITHER_HLPF|E_LD_DITHER_OFFSET))&LD_DITHER3D_VALID;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3D_L, u16Value, u16Mask);
    //min clamp value
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, 0x9, 0x03FF);
#elif(HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
    //Dither Init
    u16Value = (E_LD_DITHER_COMPENSATION_OUT&(~(E_LD_DITHER_COMPENSATION)));
    u16Mask  = (E_LD_DITHER_COMPENSATION_OUT|(E_LD_DITHER_COMPENSATION))&LD_DITHER3C_VALID;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, u16Value, u16Mask);
#endif
    //BL Globe Gain Init
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L,(0x20)&0x00FF, 0x00FF);
    //BL GlobeGainMode
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L,(E_LD_BL_GLOBAL_GAIN_8BIT<<8), 0x0700);
}

#if 0
void MDrv_LD_Print_Compensation_Table(void)
{
    MS_U16 u16Addr = 0;
    MS_U16 u16Index = 0;

    printf("\nHal_LD_Print_Compensation_Table\n");

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_72_L, ENABLE, BIT(0));

    for( u16Addr = 0; u16Addr < 256; ++ u16Addr )
    {
        if(u16Addr % 0x10 == 0) printf("\n");
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, u16Addr,0x00FF);// write address for compensation LUT
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, ENABLE<<11, BIT(11)); // Compensation LUT read
        printf("%02X%02X ", ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3E_L)&0xFF00)>>8), (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3E_L)&0x00FF));
    }

    SC_W2BYTEMSK(REG_SC_BK2E_72_L, 0, BIT(0));
}
#endif

void MDrv_LD_Load_Compensation_Table(void *pInstance, MS_U8* pu8CompTable,MS_U16 u16CompTableLength)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16Addr = 0;
    MS_U16 u16Index = 0;
    MS_U16 u16Tmp = 0, u16Tmp2 =0;
    if (pu8CompTable != NULL)
    {
        for( u16Addr = 0; u16Addr < u16CompTableLength/2; ++ u16Addr )
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, u16Addr, 0x00FF);// write address for compensation LUT
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3D_L, ((pu8CompTable[u16Index+1]<<8) + pu8CompTable[u16Index]), 0x0FFF);

            u16Index += 2;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3F_L, (ENABLE<<10), BIT(10)); // Compensation LUT write

            //MsOS_DelayTaskUs(1);
            for(u16Tmp = 0; u16Tmp < 255; ++u16Tmp)
            {
                u16Tmp2 = u16Tmp;
            }
        }
    }
}

/*
Edge 2D Table Format
Normal: Width = LED Number(HxV) / 4
        Height = Backlight size (HxV)

Special(for Edge-LR panel and odd V size)
        Width = (LED Height+1) x V / 4
        Height = Backlight size (HxV)

H-direction: zero padding to Packet Length (16byte)
*/
void MDrv_LD_Load_EDGE2D_Table(void *pInstance, MS_U8 u8LEDWidth, MS_U8 u8LEDHeight, MS_U8 u8BLWidth, MS_U8 u8BLHeight, MS_U8* pu8Edge2DTable)
{
    MS_U16 u16RowIdx = 0, u16ColumnIdx = 0;
    MS_U16 u16Tbl_Active_Width = ((u8LEDWidth*u8LEDHeight)+2)/4;
    MS_U16 u16Tbl_Active_Height = (MS_U16)u8BLWidth * u8BLHeight;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(u16RowIdx = 0; u16RowIdx < u16Tbl_Active_Height; ++u16RowIdx)
    {
        for(u16ColumnIdx = 0; u16ColumnIdx < EDGE2D_PACKET_LENGTH; ++u16ColumnIdx)
        {
            if(u16ColumnIdx < u16Tbl_Active_Width)
            {
                if (pu8Edge2DTable != NULL)
                {
                    MDrv_LD_MIUWriteByte(LD_BUF_Edge2DCoe, (u16RowIdx*EDGE2D_PACKET_LENGTH + u16ColumnIdx), pu8Edge2DTable[u16RowIdx*u16Tbl_Active_Width + u16ColumnIdx]);
                }
            }
            else
            {
                MDrv_LD_MIUWriteByte(LD_BUF_Edge2DCoe, (u16RowIdx*EDGE2D_PACKET_LENGTH + u16ColumnIdx), 0);
            }
        }
    }
}

MS_BOOL MDrv_XC_LD_SetPanelType(void *pInstance, EN_LD_PANEL_TYPE enLDPanelType )
{
#if 0
    MS_U16 i;
    MS_U16 u16TotalSPIDataLen = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32Ratio = 0;
    MS_U16 u16Tmp = 0;
    MS_U16 u16PanelWidth   = LD_PANEL_WIDTH;
    MS_U16 u16PanelHeight  = LD_PANEL_HEIGHT;
    UNUSED(enLDPanelType);
#if 0 //
    if( _enLDPanelType != enLDPanelType )
    {
    #if( DRV_LD_USE_SWSPI )
        MDrv_SPI_SetTriggerMode(E_SPI_TRIGGER_NONE);
    #else
    #endif
    }
#endif
#if 0
    Hal_MSPI_SetClockSpeed( _stDrvLDInfo.eMSPIClockSpeed );
    Hal_MSPI_SetClockPolarity( _stDrvLDInfo.u8SpiClockPolarity );
    Hal_MSPI_SetClockPhase( _stDrvLDInfo.u8SpiClockPhase );

#if( DRV_LD_USE_SWSPI )

    // Setup SPI cmd data...
    for( i = 0; i < _stDrvLDInfo.u8SpiCmdLen; ++ i )
    {
        s_au8DrvLD_SPIBuf[i] = _stDrvLDInfo.au16SpiCmd[i];
    }

    // Setup MSPI timing ...
    u16TotalSPIDataLen = _stDrvLDInfo.u8SpiCmdLen + LD_LED_NUMBER;
    if( _stDrvLDInfo.u8CheckSumMode )
    {
        u16TotalSPIDataLen += 1;
    }

    // Setup the delay time before every data
    //s_au8DrvLD_SPI_delay[0] = (_stDrvLDInfo.u8SpiTime_VSyncWidth + _stDrvLDInfo.u8SpiTime_VSync_To_FirstClock);
    s_au8DrvLD_SPI_delay[0] = _stDrvLDInfo.u8SpiTime_VSync_To_FirstClock;

    for( i = 1; i < u16TotalSPIDataLen; ++ i )
    {
        s_au8DrvLD_SPI_delay[i] = _stDrvLDInfo.u8SpiTime_Byte_to_Byte;
    }
    MDrv_SPI_SetWriteBuf_Delay( s_au8DrvLD_SPI_delay, u16TotalSPIDataLen );

#else
#endif
#endif
    //LR and odd vertical block case, need patch for HW Edge2D bug
    if((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH == 2) && (pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV % 2) == 1)
    {
        u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<5)|((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV+1)-1);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, u16Tmp, 0x3FF);
#if ( HW_DESIGN_LD_VER == 3)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV+1)-1), 0x003F);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<8), 0x3F00);
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, u16Tmp, 0x3FF);
#endif
    }
    else if((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH == 2) && !(pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV % 2))
    {
        u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<5)|((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV)-1);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, u16Tmp, 0x3FF);
#if ( HW_DESIGN_LD_VER == 3)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV)-1), 0x003F);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH-1)<<8), 0x3F00);
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, u16Tmp, 0x3FF);
#endif
    }
    else
    {
        u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV-1)<<5)|((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH)-1);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_63_L, u16Tmp, 0x3FF);
#if ( HW_DESIGN_LD_VER == 3)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH)-1), 0x003F);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV-1)<<8), 0x3F00);
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_60_L, u16Tmp, 0x3FF);
#endif
    }

    u16Tmp = ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV-1)<<5)|(pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH-1);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, u16Tmp, 0x03FF);
#if ( HW_DESIGN_LD_VER == 3)
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, (pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH-1), 0x003F);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, ((pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV-1)<<8), 0x03F00);
#else
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, u16Tmp, 0x03FF); // For LSF
#endif


    u32Ratio= (H_V_RATIO_CONST * pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH / u16PanelWidth)+1;
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_19_L, u32Ratio&0x0FFFFF);
    u32Ratio= (H_V_RATIO_CONST * pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV / u16PanelHeight)+1;
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_1B_L, u32Ratio&0x0FFFFF);

    u32Ratio= H_V_RATIO_CONST*pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH/(u16PanelWidth-1);
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_30_L, u32Ratio&0x0FFFFF);
    u32Ratio= H_V_RATIO_CONST*pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV/(u16PanelHeight-1);
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_32_L, u32Ratio&0x0FFFFF);


    MDrv_LD_Load_Compensation_Table(pInstance, pXCResourcePrivate->stdrv_LD._stDrvLDInfo.pu8CompTable,COMPENSATION_LENGTH);

    if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (ENABLE<<6),  BIT(6));
    }

    MDrv_LD_Load_EDGE2D_Table(pInstance,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumH,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LedNumV,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV,
                              pXCResourcePrivate->stdrv_LD._stDrvLDInfo.pu8Edge2DTable);
    //MDrv_LD_Print_Compensation_Table();
    return TRUE;
}

EN_LD_PANEL_TYPE Mdrv_LD_GetPanelType(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return  pXCResourcePrivate->stdrv_LD._enLDPanelType;
}

MS_BOOL MDrv_LD_Init(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if(HW_DESIGN_LD_VER == 1)
    MDrv_WriteByte(0x1012F4, 0x03);
#endif
    //----------------------
    // Local Dimming
    //wriu -w 0x102f08 0x6000
    //wriu -w 0x102f0a 0x0001
    //wriu -w 0x102f0c 0x7000
    //wriu -w 0x102f0e 0x0001 //write base address
    //----------------------
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_02_L, 0x0010); //////
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_03_L, 0x023f); /////////
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4C_L, 0x023f);

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_19_L, 0x4445);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_1B_L, 0x4445);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_1D_L, 0x1000);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x0088);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_30_L, 0x444D);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_32_L, 0x4454);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, 0x0903);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_70_L, 0x077f);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_71_L, 0x0437);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_4B_L, 0x0320); ///
    //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_76_L, 0x0000); ///
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7B_L, 0x0000); ///
    //spi
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_64_L, 0x0, 0x00FF); ///
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_65_L, 0x5, 0x00FF); ///
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_66_L, 0x0, 0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_67_L, 0x0, 0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_68_L, 0x0, 0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_69_L, 0x0, 0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6A_L, 0x0, 0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6B_L, 0x0, 0x00FF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_6C_L, 0x0, 0x00FF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_6D_L, 0xffff );///
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_6E_L, 0xffff );///
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_6F_L, 0xffff );///


    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L) | 0x0800);
    //MDrv_Write2ByteMask(0x100bb2, 0x0, 0x3);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_24_L, 0xfa20);
    if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, 0x40, 0x00FF);
    }

    //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, 0x0001);
    MDrv_LD_Set_LocalDimmingEn(pInstance, TRUE);
    return TRUE;
}


#if 0
static MS_U8 _MDrv_LD_CalCheckSum( MS_U8* pu8DataBuf, MS_U16 u16DataLen )
{
    MS_U8 u8CheckSum = 0;
    MS_U16 i;

    if (pu8DataBuf == NULL)
    {
        return u8CheckSum;
    }
    switch( _stDrvLDInfo.u8CheckSumMode )
    {
        case 1: // TODO
            u8CheckSum = pu8DataBuf[0];
            for( i = 1; i < u16DataLen; ++ i )
            {
                u8CheckSum = u8CheckSum ^ pu8DataBuf[i];
            }
            break;
        default:
            u8CheckSum = 0;
            break;
    }

    return u8CheckSum;
}
#endif

MS_BOOL MDrv_LD_SetLocalDimmingData(void *pInstance, MS_U8* pu8LocalDimmingData, MS_U16 u16DataLen)
{
    #if 0
    MS_U8 h,v;
    MS_U16 i;
    MS_U16 u16SPIDataLen = 0;
    #endif

//  PRINT_FUNCTION_LINE();
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( (pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished == FALSE) || (pu8LocalDimmingData == NULL)
        || (u16DataLen == 0) || (u16DataLen > LD_LED_NUMBER))
    {

        return FALSE;
    }
    #if 0
    printf("MDrv_LD_SetLocalDimmingData:\n");
    i = 0;
    for( v = 0; v < _stDrvLDInfo.u8LedNumV; ++ v )
    {
        for( h = 0; h < _stDrvLDInfo.u8LedNumH; ++ h )
        {
          printf("0x%02X,", pu8LocalDimmingData[i] );
            i += 1;
        }
       printf("\n" );
    }
   #endif

#if 0 // mark for hw spi driver
#if( DRV_LD_USE_SWSPI )
    // Cmd len + data len must be small 256
    if( (_stDrvLDInfo.u8SpiCmdLen+u16DataLen) >= DRV_LD_SPI_BUF_MAX_SIZE )
    {
        return FALSE;
    }

    u16SPIDataLen = _stDrvLDInfo.u8SpiCmdLen;

    // Setup dimming data
    for( i = 0; i < u16DataLen; ++ i )
    {
        s_au8DrvLD_SPIBuf[u16SPIDataLen+i] = pu8LocalDimmingData[i];
    }
    u16SPIDataLen += u16DataLen;

    // Setup check sum
    if( _stDrvLDInfo.u8CheckSumMode )
    {
        s_au8DrvLD_SPIBuf[u16SPIDataLen] = _MDrv_LD_CalCheckSum(s_au8DrvLD_SPIBuf, u16SPIDataLen);
        u16SPIDataLen += 1;
    }

#if 0 // Debug
    {
        printf("Send to SPI:\n");
        for( i = 0; i < u16SPIDataLen; ++ i )
        {
            printf("[%X]", s_au8DrvLD_SPIBuf[i]);
        }
        printf("\n");
    }
#endif

    MDrv_SPI_WriteBytes( s_au8DrvLD_SPIBuf, u16SPIDataLen, E_SPI_TRIGGER_BY_EVERY_V_SYNC);

#else // Use DLM-DMA
#endif
#endif
    return TRUE;
}


void MApi_LD_Set3DMode(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, bEnable, BIT(0));
}

MS_BOOL MApi_XC_LD_SetLevel_U2(void* pInstance, EN_LD_MODE enMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    switch(enMode)
    {
        case E_LD_MODE_OFF:
            MDrv_LD_EnableLD(pInstance, FALSE); //LDimmingFunction
            MDrv_LD_Set_LocalDimmingEn(pInstance, FALSE);

            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (ENABLE<<6),  BIT(6));
            }

            break;
        case E_LD_MODE_LOW:

            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                //blending weight parameter DC*(8-reg_alpha)+MAX*reg_alpha
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x01, 0x0F);
            }

            //pixel blending between cmpensation and ori-data
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (0x10<<4), 0x01F0);
            MDrv_LD_SetGammaControl(pInstance, 3);//_u8Gamma_Control = //BLgamma
            MDrv_LD_EnableLD(pInstance, TRUE); //LDimmingFunction
            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (ENABLE<<6),  BIT(6));
            }

            MDrv_LD_Set_LocalDimmingEn(pInstance, TRUE);
            break;
        case E_LD_MODE_MID:

            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x02, 0x0F);
            }

            //pixel blending between cmpensation and ori-data
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (0x0C<<4), 0x01F0);
            MDrv_LD_SetGammaControl(pInstance, 0);//_u8Gamma_Control = //BLgamma
            MDrv_LD_EnableLD(pInstance, TRUE); //LDimmingFunction

            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (ENABLE<<6),  BIT(6));
            }

            MDrv_LD_Set_LocalDimmingEn(pInstance, TRUE);
            break;
        case E_LD_MODE_HIGH:

            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                //blending weight parameter DC*(8-reg_alpha)+MAX*reg_alpha
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_1F_L, 0x02, 0x0F);

            }

            //pixel blending between cmpensation and ori-data
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3C_L, (0x0E<<4), 0x01F0);
            MDrv_LD_SetGammaControl(pInstance, 0);//_u8Gamma_Control = //BLgamma
            MDrv_LD_EnableLD(pInstance, TRUE); //LDimmingFunction

            if (pXCResourcePrivate->stdrv_LD._bIsLDPanel== TRUE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_46_L, (ENABLE<<6),  BIT(6));
            }

            MDrv_LD_Set_LocalDimmingEn(pInstance, TRUE);
            break;
        default:
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return FALSE;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
}

MS_BOOL MApi_XC_LD_SetLevel(EN_LD_MODE enMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_LEVEL XCArgs;
    XCArgs.enMode = enMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_LD_Test1(void *pInstance)
{
    MS_U16 i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

//    PRINT_FUNCTION_LINE();

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( pXCResourcePrivate->stdrv_LD._bMDrvLD_InitFinished == FALSE )
    {
        return;
    }
    if( pXCResourcePrivate->stdrv_LD._u16LedPos < LD_LED_NUMBER )
    {
        for( i = 0; (i < LD_LED_NUMBER)&&(i<DRV_LD_SPI_BUF_MAX_SIZE); ++ i )
        {
            pXCResourcePrivate->stdrv_LD._au8Test_LedData[i] = 0;
        }
        pXCResourcePrivate->stdrv_LD._au8Test_LedData[pXCResourcePrivate->stdrv_LD._u16LedPos] = 0xFF;
    }
    else if( pXCResourcePrivate->stdrv_LD._u16LedPos == (LD_LED_NUMBER) )
    {
        for( i = 0; (i < LD_LED_NUMBER)&&(i<DRV_LD_SPI_BUF_MAX_SIZE); ++ i )
        {
            pXCResourcePrivate->stdrv_LD._au8Test_LedData[i] = 0;
        }
    }
    else // LD_LED_NUMBER + 1
    {
        for( i = 0; (i < LD_LED_NUMBER)&&(i<DRV_LD_SPI_BUF_MAX_SIZE); ++ i )
        {
            pXCResourcePrivate->stdrv_LD._au8Test_LedData[i] = 0xFF;
        }
    }
    pXCResourcePrivate->stdrv_LD._u16LedPos += 1;

    if( pXCResourcePrivate->stdrv_LD._u16LedPos >= (LD_LED_NUMBER+2) )
    {
        pXCResourcePrivate->stdrv_LD._u16LedPos = 0;
    }

/*
    _au8Test_LedData[0] = 0x33;
    _au8Test_LedData[1] = 0x44;
    _au8Test_LedData[2] = 0x55;
    _au8Test_LedData[3] = 0x66;
    _au8Test_LedData[4] = 0x77;
    _au8Test_LedData[5] = 0x88;
    _au8Test_LedData[6] = 0x99;
    _au8Test_LedData[7] = 0xAA;
*/

    MDrv_LD_SetLocalDimmingData(pInstance, pXCResourcePrivate->stdrv_LD._au8Test_LedData, LD_LED_NUMBER);
}

//DRAM R/W test, Move frame buffer 0 data from LDF to LDB
void MDrv_LD_Test2(void *pInstance)
{
    MS_U8 u8Idx = 0;
    MS_U16 u16Buf = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(u8Idx = 0; u8Idx < (((MS_U16)pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightH) *pXCResourcePrivate->stdrv_LD._stDrvLDInfo.u8LD_BackLightV); u8Idx += 2)
    {
        u16Buf = MDrv_LD_MIURead2Bytes(LD_BUF_ReadDBuf_0, u8Idx);
        MDrv_LD_MIUWrite2Bytes(LD_BUF_WriteBuf_0, u8Idx, u16Buf);
    }
}

#if(HW_DESIGN_LD_VER == 1)
void MDrv_LD_Load_Hscaling_Table(void *pInstance, MS_U8* pu8CoefTable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8ByAddr = 0;
    MS_U16 u16WIndex = 0;

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, (ENABLE<<10), BIT(10)); //Write Horizontal
    if (pu8CoefTable == NULL)
    {
        return;
    }

    while (u8ByAddr < H_SCACLING_TABLE_SIZE)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, u8ByAddr ,0x00FF);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_34_L, pu8CoefTable[u16WIndex++] , 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_34_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_35_L, pu8CoefTable[u16WIndex++] , 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_35_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_36_L,  pu8CoefTable[u16WIndex++] ,0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_36_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, (ENABLE<<8), BIT(8)); //Write trigger
        u8ByAddr++;
    }
}

void MDrv_LD_Load_Vscaling_Table(void *pInstance, MS_U8* pu8CoefTable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8ByAddr = 0;
    MS_U16 u16WIndex = 0;

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, (DISABLE<<10), BIT(10)); //Write Vertical

    if (pu8CoefTable == NULL)
    {
        return;
    }

    while (u8ByAddr < V_SCACLING_TABLE_SIZE)
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, u8ByAddr);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_34_L, pu8CoefTable[u16WIndex++] , 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_34_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_35_L, pu8CoefTable[u16WIndex++] ,0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_35_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_36_L, pu8CoefTable[u16WIndex++] ,0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_36_L, (MS_U16)((pu8CoefTable[u16WIndex++] & 0x00FF)<<8),0xFF00);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_3A_L, (ENABLE<<8), BIT(8)); //Write trigger
        u8ByAddr++;
    }
}

void MDrvl_LD_Set_DmaXOffset(void *pInstance, MS_U8 u8DmaIdx, MS_U8 u8XStart, MS_U8 u8XEnd)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32RegAddr = REG_SC_BK2E_64_L + (u8DmaIdx*2);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32RegAddr,u8XStart, 0x001F);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32RegAddr,(u8XEnd<<8), 0x1F00);
}

void MDrv_LD_Set_DmaYOffset(void *pInstance, MS_U8 u8DmaIdx, MS_U8 u8YStart)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32RegAddr = REG_SC_BK2E_6C_L + u8DmaIdx/2;
    if(u8DmaIdx%2 == 0)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32RegAddr, u8YStart, 0x1F);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32RegAddr, (u8YStart)<<8, 0x1F00);
    }
}

#endif


void MDrv_LD_Set_LDF_FrameBufBaseAddr(void *pInstance, MS_U8 u8BufIdx, MS_PHY u32LDFBase)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32RegAddr = REG_SC_BK2E_04_L + (u8BufIdx*4);
    SC_W4BYTE(psXCInstPri->u32DeviceID, u32RegAddr, (u32LDFBase/LD_MIU_ALIGN)& MEMORY_MASK);
}

MS_PHY MDrv_LD_Get_LDF_FrameBufBaseAddr(void *pInstance, MS_PHY u8BufIdx)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32RegAddr = REG_SC_BK2E_04_L + (u8BufIdx*4);
    return ((MS_PHY)((SC_R4BYTE(psXCInstPri->u32DeviceID, u32RegAddr)& MEMORY_MASK)*LD_MIU_ALIGN) );
}


void MDrv_LD_Set_LDB_FrameBufBaseAddr(void *pInstance, MS_U8 u8BufIdx, MS_PHY u32LDBBase)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32RegAddr = REG_SC_BK2E_4D_L + (u8BufIdx*4);
    SC_W4BYTE(psXCInstPri->u32DeviceID, u32RegAddr, (u32LDBBase/LD_MIU_ALIGN));
}

MS_PHY MDrv_LD_Get_LDB_FrameBufBaseAddr(void *pInstance, MS_PHY u8BufIdx)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32RegAddr = REG_SC_BK2E_4D_L + (u8BufIdx*4);
    return ((MS_PHY)(SC_R4BYTE(psXCInstPri->u32DeviceID, u32RegAddr)& MEMORY_MASK)*LD_MIU_ALIGN);
}

void MDrv_LD_Set_LEDData_BaseOffset(void *pInstance, MS_PHY u32LEDOffset)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //SPI data base address = LDB baseaddress + SPI data base address offset
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7E_L,(u32LEDOffset/LD_MIU_ALIGN) & MEMORY_MASK);
}

MS_PHY MDrv_LD_Get_SPIData_BaseAddr(void *pInstance, MS_PHY u8BufIdx)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //SPI data base address = LDB baseaddress + SPI data base address offset
    MS_U32 u32SPIBaseOffset = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_7E_L)& MEMORY_MASK;
    MS_U32 u32LDBBaseAddr = MDrv_LD_Get_LDB_FrameBufBaseAddr(pInstance, u8BufIdx);
    return ((MS_PHY)u32LDBBaseAddr + (MS_PHY)(u32SPIBaseOffset*LD_MIU_ALIGN));
}

void MDrv_LD_Set_EDGE2D_BaseAddr(void *pInstance, MS_PHY u32EDGE2DBase)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_79_L, (u32EDGE2DBase/LD_MIU_ALIGN) & MEMORY_MASK);
}

MS_PHY MDrv_LD_Get_EDGE2D_BaseAddr(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return ((MS_PHY)(SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_79_L)& MEMORY_MASK)*LD_MIU_ALIGN);
}


void MDrv_LD_Set_LocalDimmingEn(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (bEn)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, BIT(0), BIT(0));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L, 0x0, BIT(0));
    }
}

MS_BOOL MDrv_LD_Get_LocalDimmingEn(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return ((MS_BOOL)SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2E_01_L)&0x01);
}

ST_DRV_LD_INFO MDrv_LD_GetLDInfo(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrv_LD._stDrvLDInfo;
}

MS_BOOL MApi_Set_BLLevel_U2(void* pInstance, MS_U8 u8BLLevel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, ((MS_U16)(u8BLLevel<<8)), 0xFF00);
    return TRUE;
}

MS_BOOL MApi_Set_BLLevel(MS_U8 u8BLLevel)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_BL_LEVEL XCArgs;
    XCArgs.u8BLLevel = u8BLLevel;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BL_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags_U2(void* pInstance, MS_BOOL bRefreshSPIData)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, (bRefreshSPIData)<<2 ,BIT(2));
    return TRUE;
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags(MS_BOOL bRefreshSPIData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_NOTUPDATE_SPI_DATA_FLAGS XCArgs;
    XCArgs.bRefreshSPIData = bRefreshSPIData;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_NOTUPDATE_SPI_DATA_FLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_TurnoffLDBL_U2(void* pInstance, MS_BOOL bTurnoffBL)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, (bTurnoffBL)<<1 ,BIT(1));
    return TRUE;
}

MS_BOOL MApi_Set_TurnoffLDBL(MS_BOOL bTurnoffBL)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_TURNOFF_LDBL XCArgs;
    XCArgs.bTurnoffBL = bTurnoffBL;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_TURNOFF_LDBL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_UsermodeLDFlags_U2(void* pInstance, MS_BOOL bUserLDFlags)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2E_29_L, bUserLDFlags , BIT(0));
    return TRUE;
}

MS_BOOL MApi_Set_UsermodeLDFlags(MS_BOOL bUserLDFlags)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_USERMODEFLAGS XCArgs;
    XCArgs.bUserLDFlags = bUserLDFlags;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_USERMODEFLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#else
MS_BOOL MApi_XC_LD_SetMemoryAddress_U2(void* pInstance,
                                      MS_U8 u8MIU ,
                                      MS_PHY u32LDFBase0,
                                      MS_PHY u32LDFBase1,
                                      MS_PHY u32LDBBase0,
                                      MS_PHY u32LDBBase1,
                                      MS_PHY u32EDGE2DBase,
                                      MS_PHY u32LEDOffset)
{
    //not support
    UNUSED(pInstance);
    UNUSED(u8MIU);
    UNUSED(u32LDFBase0);
    UNUSED(u32LDFBase1);
    UNUSED(u32LDBBase0);
    UNUSED(u32LDBBase1);
    UNUSED(u32EDGE2DBase);
    UNUSED(u32EDGE2DBase);
    UNUSED(u32LEDOffset);
    return TRUE;
}

MS_BOOL MApi_XC_LD_SetMemoryAddress(MS_U8 u8MIU ,
                                    MS_PHY u32LDFBase0,
                                    MS_PHY u32LDFBase1,
                                    MS_PHY u32LDBBase0,
                                    MS_PHY u32LDBBase1,
                                    MS_PHY u32EDGE2DBase,
                                    MS_PHY u32LEDOffset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_MEMORYADDRESS XCArgs;
    XCArgs.u8MIU = u8MIU;
    XCArgs.u32LDFBase0 = u32LDFBase0;
    XCArgs.u32LDFBase1 = u32LDFBase1;
    XCArgs.u32LDBBase0 = u32LDBBase0;
    XCArgs.u32LDBBase1 = u32LDBBase1;
    XCArgs.u32EDGE2DBase = u32EDGE2DBase;
    XCArgs.u32LEDOffset = u32LEDOffset;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_MEMORYADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_LD_Get_Value_U2(void* pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    //not support
    UNUSED(pu8LDValue);
    UNUSED(u8WSize);
    UNUSED(u8HSize);
    return TRUE;
}

MS_BOOL MApi_XC_LD_Get_Value(MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_LD_VALUE XCArgs;
    XCArgs.pu8LDValue = pu8LDValue;
    XCArgs.u8WSize = u8WSize;
    XCArgs.u8HSize = u8HSize;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_LD_VALUE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_LD_SetLevel_U2(void* pInstance, EN_LD_MODE enMode)
{
    //not support
    UNUSED(pInstance);
    UNUSED(enMode);
    return TRUE;
}

MS_BOOL MApi_XC_LD_SetLevel(EN_LD_MODE enMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_LEVEL XCArgs;
    XCArgs.enMode = enMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_LD_Init_U2(void* pInstance, EN_LD_PANEL_TYPE enLDPanelType )
{
    UNUSED(pInstance);
    UNUSED(enLDPanelType);
    return TRUE;
}

MS_BOOL MApi_XC_LD_Init(EN_LD_PANEL_TYPE enLDPanelType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_LD_INIT XCArgs;
    XCArgs.enLDPanelType = enLDPanelType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_LD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_BLLevel_U2(void* pInstance, MS_U8 u8BLLevel)
{
    UNUSED(pInstance);
    UNUSED(u8BLLevel);
    return TRUE;
}

MS_BOOL MApi_Set_BLLevel(MS_U8 u8BLLevel)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_BL_LEVEL XCArgs;
    XCArgs.u8BLLevel = u8BLLevel;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BL_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_TurnoffLDBL_U2(void* pInstance, MS_BOOL bTurnoffBL)
{
    UNUSED(pInstance);
    UNUSED(bTurnoffBL);
    return TRUE;
}

MS_BOOL MApi_Set_TurnoffLDBL(MS_BOOL bTurnoffBL)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_TURNOFF_LDBL XCArgs;
    XCArgs.bTurnoffBL = bTurnoffBL;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_TURNOFF_LDBL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_UsermodeLDFlags_U2(void* pInstance, MS_BOOL bUserLDFlags)
{
    UNUSED(pInstance);
    UNUSED(bUserLDFlags);
    return TRUE;
}

MS_BOOL MApi_Set_UsermodeLDFlags(MS_BOOL bUserLDFlags)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_LD_USERMODEFLAGS XCArgs;
    XCArgs.bUserLDFlags = bUserLDFlags;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LD_USERMODEFLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags_U2(void* pInstance, MS_BOOL bRefreshSpidata)
{
    UNUSED(pInstance);
    UNUSED(bRefreshSpidata);
    return TRUE;
}

MS_BOOL MApi_Set_notUpdateSPIDataFlags(MS_BOOL bRefreshSPIData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_NOTUPDATE_SPI_DATA_FLAGS XCArgs;
    XCArgs.bRefreshSPIData = bRefreshSPIData;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_NOTUPDATE_SPI_DATA_FLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#endif

