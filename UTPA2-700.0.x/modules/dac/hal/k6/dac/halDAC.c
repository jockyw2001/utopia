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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halDAC.c
/// @author MStar Semiconductor Inc.
/// @brief  DAC Utility HAL
///////////////////////////////////////////////////////////////////////////////////////////////////

#define  HAL_DAC_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsIRQ.h"
#include "drvDAC.h"
#include "halDAC.h"
#include "UFO.h"
#include "hdgen_table.c"
#include "hwreg_dac.h"

#include "mdrv_dac_tbl.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#else
#include "string.h"
#define do_div(x,y) ((x)/=(y))
#endif

#include "drvSYS.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DAC_DETECT_DELAY 1000
#define DAC_DETECT_COUNT 3
#define DAC_DETECT_COUNT_HALF 0x1FE   //FULL is 0x3FC
#define DAC_DETECT_TIMEOUT 10
#define DAC_DETECT_G_Channel 0
#define DAC_DETECT_B_Channel BIT(8)
#define DAC_DETECT_R_Channel BIT(9)
#define DAC_TAB_MAX_SIZE  2048
#define DAC_DBG(msg) //msg
//#define DAC_DETECT_ISR_ENABLE 1

#define DAC_HDDAC_DEFAULT_OUTPUTSWAPSEL   DAC_R_B_G
#define DAC_SDDAC_DEFAULT_OUTPUTSWAPSEL   DAC_R_G_B

#define DAC_ASSERT(_cnd, _fmt, _args...)                                                                            \
                                    if (!(_cnd)) {                                                                  \
                                        printf("DAC ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args);              \
                                        while(1);                                                                   \
                                    }


MS_BOOL bDacIRQInit = FALSE;
MS_VIRT gu32DacRiuBaseAddr = 0;
static MS_BOOL bIsSDDACInited = FALSE;
static MS_BOOL bIsHDDACInited = FALSE;

static E_DAC_PANEL_INDEX _geDACPanel_index = DAC_PANEL_480I_60;
extern DAC_TAB_INFO DACPANEL_MainTbl[DAC_PANEL_NUMS];

#define HDGEN_VBITBL_ADDR_SIZE          2
#define HDGEN_VBITBL_ADDR_MASK_SIZE     1
#define HDGEN_VBITBL_DATA_SIZE          1
#define HDGEN_ADDR_BASE     0x3000
#define HDGEN_REG(reg)      ((reg>>8)&0xFF), (reg&0xFF)
#define REG_ADDR_HDGEN_00_L       (HDGEN_ADDR_BASE + 0x00)
#define REG_ADDR_HDGEN_00_H       (HDGEN_ADDR_BASE + 0x01)
#define REG_ADDR_HDGEN_01_L       (HDGEN_ADDR_BASE + 0x02)
#define REG_ADDR_HDGEN_02_L       (HDGEN_ADDR_BASE + 0x04)
#define REG_ADDR_HDGEN_02_H       (HDGEN_ADDR_BASE + 0x05)
#define REG_ADDR_HDGEN_03_L       (HDGEN_ADDR_BASE + 0x06)
#define REG_ADDR_HDGEN_03_H       (HDGEN_ADDR_BASE + 0x07)
#define REG_ADDR_HDGEN_05_L       (HDGEN_ADDR_BASE + 0x0A)
#define REG_ADDR_HDGEN_08_L       (HDGEN_ADDR_BASE + 0x10)
#define REG_ADDR_HDGEN_08_H       (HDGEN_ADDR_BASE + 0x11)
#define REG_ADDR_HDGEN_09_L       (HDGEN_ADDR_BASE + 0x12)
#define REG_ADDR_HDGEN_09_H       (HDGEN_ADDR_BASE + 0x13)
#define REG_ADDR_HDGEN_0B_L       (HDGEN_ADDR_BASE + 0x16)
#define REG_ADDR_HDGEN_0B_H       (HDGEN_ADDR_BASE + 0x17)
#define REG_ADDR_HDGEN_0E_L       (HDGEN_ADDR_BASE + 0x1C)
#define REG_ADDR_HDGEN_0E_H       (HDGEN_ADDR_BASE + 0x1D)
#define REG_ADDR_HDGEN_0F_L       (HDGEN_ADDR_BASE + 0x1E)
#define REG_ADDR_HDGEN_0F_H       (HDGEN_ADDR_BASE + 0x1F)
#define REG_ADDR_HDGEN_14_L       (HDGEN_ADDR_BASE + 0x28)
#define REG_ADDR_HDGEN_14_H       (HDGEN_ADDR_BASE + 0x29)
#define REG_ADDR_HDGEN_15_L       (HDGEN_ADDR_BASE + 0x2A)
#define REG_ADDR_HDGEN_15_H       (HDGEN_ADDR_BASE + 0x2B)
#define REG_ADDR_HDGEN_16_L       (HDGEN_ADDR_BASE + 0x2C)
#define REG_ADDR_HDGEN_16_H       (HDGEN_ADDR_BASE + 0x2D)
#define REG_ADDR_HDGEN_17_L       (HDGEN_ADDR_BASE + 0x2E)
#define REG_ADDR_HDGEN_17_H       (HDGEN_ADDR_BASE + 0x2F)
#define REG_ADDR_HDGEN_18_L       (HDGEN_ADDR_BASE + 0x30)
#define REG_ADDR_HDGEN_18_H       (HDGEN_ADDR_BASE + 0x31)
#define REG_ADDR_HDGEN_19_L       (HDGEN_ADDR_BASE + 0x32)
#define REG_ADDR_HDGEN_19_H       (HDGEN_ADDR_BASE + 0x33)
#define REG_ADDR_HDGEN_1A_L       (HDGEN_ADDR_BASE + 0x34)
#define REG_ADDR_HDGEN_1A_H       (HDGEN_ADDR_BASE + 0x35)
#define REG_ADDR_HDGEN_1B_L       (HDGEN_ADDR_BASE + 0x36)
#define REG_ADDR_HDGEN_1B_H       (HDGEN_ADDR_BASE + 0x37)
#define REG_ADDR_HDGEN_1C_L       (HDGEN_ADDR_BASE + 0x38)
#define REG_ADDR_HDGEN_1C_H       (HDGEN_ADDR_BASE + 0x39)
#define REG_ADDR_HDGEN_2B_L       (HDGEN_ADDR_BASE + 0x56)

#ifndef REG_TABLE_END
#define REG_TABLE_END            0xFFFF
#endif

typedef enum
{
    HDGEN_TABTYPE_VBI,
    HDGEN_TABTYPE_NUM,
} E_HDGEN_TAB_TYPE;

MS_U8 HDGEN_VBITBL_WSS525_A_480I_60[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x01/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0x0A/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x02/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x04/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x06/*ALL*/, },     // enable wss525A
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x14/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x1B/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x01/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0x2E/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x01/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0x1F/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0x7C/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0xF0/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x21/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

MS_U8 HDGEN_VBITBL_WSS525_A_480P_60[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x00/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0x06/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x02/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x03/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x06/*ALL*/, },     // enable wss525A
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x29/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x29/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x00/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0x9C/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x00/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0x4F/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0xEC/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0xC4/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x4E/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

MS_U8 HDGEN_VBITBL_WSS_576I_50[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x07/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0x6F/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x02/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x04/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x02/*ALL*/, },     // enable wss
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x17/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x17/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x00/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0x29/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x01/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0xDA/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0x4B/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0x68/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x2F/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

MS_U8 HDGEN_VBITBL_WSS_576P_50[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x00/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0x70/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x02/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x03/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x02/*ALL*/, },     // enable wss
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x2B/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x2B/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x00/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0x94/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x00/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0xB4/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0x97/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0xD0/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x5E/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

MS_U8 HDGEN_VBITBL_WSS525_A_720P_50[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x00/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0xE7/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x02/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x2B/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x06/*ALL*/, },     // enable wss525A
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x18/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x18/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x00/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0xE8/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x00/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0xC2/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0x72/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0x4F/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x23/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

MS_U8 HDGEN_VBITBL_WSS525_A_720P_60[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x00/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0xE6/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x02/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x2B/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x06/*ALL*/, },     // enable wss525A
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x18/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x18/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x00/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0xE8/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x00/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0xC2/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0x72/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0x4F/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x23/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

MS_U8 HDGEN_VBITBL_WSS525_A_1080I_50[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x01/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0x5D/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x04/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x30/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x06/*ALL*/, },     // enable wss525A
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x13/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x46/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x02/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0x34/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x01/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0x60/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0xEF/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0x98/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x1A/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

MS_U8 HDGEN_VBITBL_WSS525_A_1080I_60[][HDGEN_VBITBL_ADDR_SIZE+HDGEN_VBITBL_ADDR_MASK_SIZE+HDGEN_VBITBL_DATA_SIZE]=
{                 // Reg           Mask  Value
    { HDGEN_REG(REG_ADDR_HDGEN_00_H), 0xFF, 0x00/*ALL*/, },     // Bank switch to HDGEN VBI
    { HDGEN_REG(REG_ADDR_HDGEN_00_L), 0xFF, 0x03/*ALL*/, },     // Bank switch to HDGEN VBI

    { HDGEN_REG(REG_ADDR_HDGEN_01_L), 0x0F, 0x01/*ALL*/, },     // vbi interlaced mode
    { HDGEN_REG(REG_ADDR_HDGEN_02_L), 0xFF, 0x5D/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_02_H), 0x07, 0x04/*ALL*/, },     // vbi start line
    { HDGEN_REG(REG_ADDR_HDGEN_03_L), 0xFF, 0x30/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_03_H), 0x1F, 0x00/*ALL*/, },     // vbi start pixel
    { HDGEN_REG(REG_ADDR_HDGEN_05_L), 0x0E, 0x06/*ALL*/, },     // enable wss525A
    { HDGEN_REG(REG_ADDR_HDGEN_08_L), 0xFF, 0x13/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_08_H), 0x07, 0x00/*ALL*/, },     // wss line top
    { HDGEN_REG(REG_ADDR_HDGEN_09_L), 0xFF, 0x46/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_09_H), 0x07, 0x02/*ALL*/, },     // wss line bom
    { HDGEN_REG(REG_ADDR_HDGEN_0B_L), 0xFF, 0x34/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0B_H), 0x03, 0x01/*ALL*/, },     // wss start position
    { HDGEN_REG(REG_ADDR_HDGEN_0E_L), 0xFF, 0x60/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0E_H), 0xFF, 0xEF/*ALL*/, },     // wss phase step low
    { HDGEN_REG(REG_ADDR_HDGEN_0F_L), 0xFF, 0x98/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_ADDR_HDGEN_0F_H), 0xFF, 0x1A/*ALL*/, },     // wss phase step high
    { HDGEN_REG(REG_TABLE_END)   , 0x00, 0x00,  }

};

static void _Hal_HDGEN_VBI_GetTabSize(MS_U8 *pDacTab, E_HDGEN_TAB_TYPE etabtype, MS_U32 *pu32DacTabsize)
{
    MS_U8 u8Colslen = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U16 u16Addr;

    if (pDacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DacTable error \n.", __FUNCTION__);
        return;
    }

    switch(etabtype)
    {
        case HDGEN_TABTYPE_VBI:
            u8Colslen = HDGEN_VBITBL_ADDR_SIZE + HDGEN_VBITBL_ADDR_MASK_SIZE + HDGEN_VBITBL_DATA_SIZE;
            break;
        default:
            DAC_ASSERT(0, "[%s] GetTable Size :unknown Tab Size \n.", __FUNCTION__);
            return ;
    }

    while (--u16timeout)
    {
        u16Addr = (pDacTab[*pu32DacTabsize]<<8) + pDacTab[(*pu32DacTabsize +1)];
        *pu32DacTabsize = *pu32DacTabsize + u8Colslen;
        if (u16Addr == REG_TABLE_END) // check end of table
        {
            break;
        }
    }

    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }

    DAC_DBG(printf("<<*pu32DacTabsize= %ld>>\n", *pu32DacTabsize));

    if(*pu32DacTabsize > DAC_TAB_MAX_SIZE)
    {
        DAC_ASSERT(0, "[%s] Tab size too large than DAC_TAB_MAX_SIZE = %d \n.", __FUNCTION__, DAC_TAB_MAX_SIZE);
    }
}

static void _Hal_HDGEN_DumpVBIRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;

    MS_U16 u16Bank;

    u16Bank = R2BYTE_TAB(HDGEN_BK_SELECT, 0xFF);

    DAC_DBG(printf( "tab: HDGEN \n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DACTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF;
        u8Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u8Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        DAC_DBG(printf( "[addr=%04lx, msk=%02x, val=%02x] \n", BK_HDGEN((u32Addr & 0x00FF)), u8Mask, u8Value));

        W1BYTE_TAB(BK_HDGEN(u32Addr & 0x00FF), u8Value, u8Mask);
        u32tabIdx = u32tabIdx + HDGEN_VBITBL_ADDR_SIZE + HDGEN_VBITBL_ADDR_MASK_SIZE + HDGEN_VBITBL_DATA_SIZE;
    }

    W2BYTE_TAB(HDGEN_BK_SELECT, u16Bank, 0xFFFF);

    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}

static void _Hal_DAC_GetTabSize(MS_U8 *pDacTab, MS_U16 u16tabtype, MS_U32 *pu32DacTabsize)
{
    MS_U8 u8Colslen = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U16 u16Addr;
    if (pDacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DacTable error \n.", __FUNCTION__);
        return;
    }
    switch(u16tabtype )
    {
    case DAC_TABTYPE_INIT:
    case DAC_TABTYPE_INIT_GPIO:
    case DAC_TABTYPE_INIT_SC:
    case DAC_TABTYPE_INIT_MOD:
    case DAC_TABTYPE_INIT_HDGEN:
    case DAC_TABTYPE_INIT_HDMITX_8BIT:
    case DAC_TABTYPE_INIT_HDMITX_10BIT:
    case DAC_TABTYPE_INIT_HDMITX_12BIT:
    case DAC_TABTYPE_INIT_HDMITX_16BIT:
    case DAC_TABTYPE_INIT_HDMITX_8BIT_Divider:
    case DAC_TABTYPE_INIT_HDMITX_10BIT_Divider:
    case DAC_TABTYPE_INIT_HDMITX_12BIT_Divider:
    case DAC_TABTYPE_INIT_HDMITX_16BIT_Divider:
    case DAC_TABTYPE_SC1_INIT:
    case DAC_TABTYPE_SC1_INIT_SC:
        u8Colslen = 4;
        break;
   // case DAC_TABTYPE_POWER_SEQUENCE_ON:
   // case DAC_TABTYPE_POWER_SEQUENCE_OFF:
   //     u8Colslen = 7;
   //     break;
    default:
        DAC_ASSERT(0, "[%s] GetTable Size :unknown Tab Size \n.", __FUNCTION__);
        return ;
    }
    while (--u16timeout)
    {
        u16Addr = (pDacTab[*pu32DacTabsize]<<8) + pDacTab[(*pu32DacTabsize +1)];
        if (u16Addr == REG_TABLE_END) // check end of table
        {
            *pu32DacTabsize = *pu32DacTabsize + 4;
            break;
        }
        *pu32DacTabsize = *pu32DacTabsize + u8Colslen;
    }
    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }

    DAC_DBG(printf("<<*pu32DacTabsize= %ld>>\n", *pu32DacTabsize));

    if(*pu32DacTabsize > DAC_TAB_MAX_SIZE)
    {
        DAC_ASSERT(0, "[%s] Tab size too large than DAC_TAB_MAX_SIZE = %d \n.", __FUNCTION__, DAC_TAB_MAX_SIZE);
    }
}

#ifdef CONFIG_MSTAR_CLKM
#include "drvCLKM.h"
#endif
static void _Hal_DAC_DumpGENRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;

    DAC_DBG(printf("tab: gen \n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DacTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u16Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        DAC_DBG(printf("[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value));
#ifdef CONFIG_MSTAR_CLKM
        //control hdgen clock by CLKM
        if(u32Addr == 0xBB0)
        {
            MS_U32 u32DacHandle = Drv_Clkm_Get_Handle("g_clk_hdgen");
            if(u16Value == 0x04)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV2");
            }
            else if(u16Value == 0x08)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV4");
            }
            else if(u16Value == 0x0C)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV8");
            }
            else if(u16Value == 0x10)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV1");
            }
            else if(u16Value == 0x14)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV16");
            }
            else if(u16Value == 0x18)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_27M");
            }
        }
        else if(u32Addr == 0xBB1)
        {
            MS_U32 u32DacHandle = Drv_Clkm_Get_Handle("g_clk_hdgen_fir");
            Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV1");
        }
        else if(u32Addr == 0xBB4)
        {
            MS_U32 u32DacHandle = Drv_Clkm_Get_Handle("g_clk_hdgen_in");
            if(u16Value == 0x04)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV2");
            }
            else if(u16Value == 0x08)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV4");
            }
            else if(u16Value == 0x0C)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV8");
            }
            else if(u16Value == 0x10)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV1");
            }
            else if(u16Value == 0x14)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_DAC_DIV16");
            }
            else if(u16Value == 0x18)
            {
                Drv_Clkm_Set_Clk_Source(u32DacHandle,"CLK_27M");
            }
        }
        else
        {
            u32Addr|=0x100000;  //Convert to NonPM address

            if (u32Addr & 0x1)
            {
                u32Addr --;
                W2BYTE_TAB(u32Addr, (u16Value << 8), (u16Mask << 8));
            }
            else
            {
                W2BYTE_TAB(u32Addr, u16Value, u16Mask);
            }
        }
        u32tabIdx = u32tabIdx + 4;
#else
        //DAC clock setting
        u32Addr|=0x100000;  //Convert to NonPM address

        if (u32Addr & 0x1)
        {
            u32Addr --;
            W2BYTE_TAB(u32Addr, (u16Value << 8), (u16Mask << 8));
        }
        else
        {
            W2BYTE_TAB(u32Addr, u16Value, u16Mask);
        }
        u32tabIdx = u32tabIdx + 4;
#endif
    }

    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}


static void _Hal_DAC_DumpHDMITxRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;

    DAC_DBG(printf( "tab: gen HDMITX\n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DACTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u16Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if ((((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF) == REG_TABLE_END) // check end of table
            break;

        DAC_DBG(printf( "[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value));
        //printf("_MDrv_DAC_DumpHighbyteRegTab [addr=%lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);

        if(((u32Addr | DAC_PLL_ADDR_OFFSET) & 0xFFFF00) == DAC_PLL_BASE)
        {
            u32Addr |= DAC_PLL_ADDR_OFFSET;
        }
        else
        {
            u32Addr |= HDMITX_DIV_ADDR_OFFSET;  //Convert to NonPM address
        }
        if (u32Addr & 0x1)
        {
            u32Addr --;
            W2BYTE_TAB(u32Addr, (u16Value << 8), (u16Mask << 8));
        }
        else
        {
            W2BYTE_TAB(u32Addr, u16Value, u16Mask);
        }

        u32tabIdx = u32tabIdx + 4;

    }

    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}


static void _Hal_DAC_DumpHDMITxDivRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;

    DAC_DBG(printf( "tab: gen HDMITX\n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DACTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u16Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if ((((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF) == REG_TABLE_END) // check end of table
            break;

        DAC_DBG(printf( "[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value));
        //printf("_MDrv_DAC_DumpHighbyteRegTab [addr=%lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);

        if(((u32Addr | DAC_PLL_ADDR_OFFSET) & 0xFFFF00) == DAC_PLL_BASE)
        {
            u32Addr |= DAC_PLL_ADDR_OFFSET;
        }
        else
        {
            u32Addr |= HDMITX_DIV_ADDR_OFFSET;  //Convert to PM or NonPM address
        }
        if (u32Addr & 0x1)
        {
            u32Addr --;
            W2BYTE_TAB(u32Addr, (u16Value << 8), (u16Mask << 8));
        }
        else
        {
            W2BYTE_TAB(u32Addr, u16Value, u16Mask);
        }

        u32tabIdx = u32tabIdx + 4;

    }

    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}


static void _Hal_DAC_DumpSCRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16CurBank;
    MS_U16 u16LastBank = 0xff;
    MS_U8 u8Mask;
    MS_U8 u8Value;

    MS_U16 u16Bank;

    u16Bank = (R2BYTE_TAB(XC_SC_BK_SELECT, 0xFF) & 0xFF);

    DAC_DBG(printf( "tab: sc \n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DACTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = (((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF);
        u8Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u8Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        u16CurBank = ((u32Addr & 0xFFFF) >> 8);
        if (u16CurBank != u16LastBank)
        {
            DAC_DBG(printf( "<<bankswitch=%02x>> \n", u16CurBank));
            W2BYTE_TAB(XC_SC_BK_SELECT, u16CurBank, 0xFF);
            u16LastBank = u16CurBank;
        }
        DAC_DBG(printf( "[addr=%04lx, msk=%02x, val=%02x] \n", BK_SC((u32Addr & 0x00FF)), u8Mask, u8Value));

        W1BYTE_TAB( (BK_SC((u32Addr & 0x00FF))|0x100000), u8Value, u8Mask);

        u32tabIdx = u32tabIdx + 4;
    }
    W2BYTE_TAB(XC_SC_BK_SELECT, u16Bank, 0xFF);
    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}

static void _Hal_DAC_DumpHDGENRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U16 u16CurBank;
    MS_U16 u16LastBank = 0xff;

    MS_U16 u16Bank;

    u16Bank = (R2BYTE_TAB(HDGEN_BK_SELECT, 0xFF) & 0xFF);

    DAC_DBG(printf( "tab: HDGEN \n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DACTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF;
        u8Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u8Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        u16CurBank = ((u32Addr & 0xFFFF) >> 8);
        if (u16CurBank != u16LastBank)
        {
            DAC_DBG(printf( "<<bankswitch=%02x>> \n", u16CurBank));
            W2BYTE_TAB(HDGEN_BK_SELECT, u16CurBank, 0xFF);
            u16LastBank = u16CurBank;
        }

        DAC_DBG(printf( "[addr=%04lx, msk=%02x, val=%02x] \n", BK_HDGEN((u32Addr & 0x00FF)), u8Mask, u8Value));
        //printf("[addr=%04lx, msk=%02x, val=%02x] \n", BK_HDGEN((u32Addr & 0x00FF))|bHiByte, u16Mask, u16Value);

        W1BYTE_TAB( (BK_HDGEN((u32Addr & 0x00FF))|0x100000), u8Value, u8Mask);
        u32tabIdx = u32tabIdx + 4;
    }
    W2BYTE_TAB(HDGEN_BK_SELECT, u16Bank, 0xFF);
    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}

static void _Hal_DAC_DumpMODRegTab(MS_U8 u8DacTab[DAC_TAB_MAX_SIZE])
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;
    MS_U16 u16CurBank;
    MS_U16 u16LastBank = 0xff;
    MS_BOOL bHiByte;

    MS_U16 u16Bank;

    u16Bank = (R2BYTE_TAB(XC_MOD_BK_SELECT, 0xFF) & 0xFF);

    DAC_DBG(printf( "tab: mod \n"));

    if (u8DacTab == NULL)
    {
        DAC_ASSERT(0, "[%s] DACable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8DacTab[u32tabIdx]<<8) + u8DacTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = u8DacTab[(u32tabIdx +2)] & 0xFF;
        u16Value = u8DacTab[(u32tabIdx +3)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        u16CurBank = ((u32Addr & 0x7FFF) >> 8);
        bHiByte = (u32Addr & 0x8000) ? TRUE : FALSE;
        if (u16CurBank != u16LastBank)
        {
            DAC_DBG(printf( "<<bankswitch=%02x>> \n", u16CurBank));
            W2BYTE_TAB(XC_MOD_BK_SELECT, u16CurBank, 0xFF);
            u16LastBank = u16CurBank;
        }
        DAC_DBG(printf( "[addr=%04lx, msk=%02x, val=%02x] \n", BK_MOD((u32Addr & 0x00FF))|bHiByte, u16Mask, u16Value));

        if (bHiByte)
        {
            W2BYTE_TAB(  (BK_MOD((u32Addr & 0x00FF))|0x100000), (u16Value << 8), (u16Mask << 8));
        }
        else
        {
            W2BYTE_TAB( (BK_MOD((u32Addr & 0x00FF))|0x100000)  , u16Value, u16Mask);
        }
        u32tabIdx = u32tabIdx + 4;
    }
    W2BYTE_TAB(XC_MOD_BK_SELECT, u16Bank, 0xFF);
    if (u16timeout==0)
    {
        DAC_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}


void _HAL_DAC_HpdIsr(InterruptNum eIntNum, void* dev_id)
{
    //eIntNum = eIntNum;      // prevent compile warning


    //MS_U16 regval = R2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), 15:0);
    //DAC_DBG(printf("detect ISR: 0x%04x\n", regval));

    //HD Dac plug in
    if(R2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), 8:8) == 0x0100)
    {
        DAC_DBG(printf("Detect HD DAC plug in !!\n"));

        //change register to detect HD DAC plug out
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), FALSE, 12:12);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), FALSE, 14:14);
        W2BYTE(BK_DAC(REG_DAC_HD_DETECT_CTRL), 0x2422, 15:0);
        W2BYTE(BK_DAC(REG_DAC_Delay_Sel), 0x0002, 3:0);
        W2BYTE(BK_DAC(REG_DAC_Delay_Sel), 0x000a, 15:12);
        W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 13:11);

        //toggle to clear final status
        W2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), TRUE, 4:4);
        W2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), FALSE, 4:4);
    }
    //HD DAC plug out
    else if(R2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), 9:9) == 0x0200)
    {
        DAC_DBG(printf("Detect HD DAC plug out !!\n"));

        //change register to detect HD DAC plug in
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 12:12);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 14:14);
        W2BYTE(BK_DAC(REG_DAC_HD_DETECT_CTRL), 0xff21, 15:0);
        W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 13:11);

        //toggle to clear final status
        W2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), TRUE, 5:5);
        W2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), FALSE, 5:5);
    }


    //SD DAC plug in
    if(R2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), 10:10) == 0x0400)
    {
        DAC_DBG(printf("Detect SD DAC plug in !!\n"));

        //change register to detect SD DAC plug out
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), FALSE, 13:13);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), FALSE, 15:15);
        W2BYTE(BK_DAC(REG_DAC_SD_DETECT_CTRL), 0x0B12, 15:0);
        W2BYTE(BK_DAC(REG_DAC_Delay_Sel), 0x0002, 3:0);
        W2BYTE(BK_DAC(REG_DAC_Delay_Sel), 0x000a, 11:8);
        W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 10:8);

        //toggle to clear final status
        W2BYTE(BK_DAC(REG_DAC_SD_IRQ_CTRL), TRUE, 4:4);
        W2BYTE(BK_DAC(REG_DAC_SD_IRQ_CTRL), FALSE, 4:4);
    }
    //SD DAC plug out
    else if(R2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), 11:11) == 0x0800)
    {
        DAC_DBG(printf("Detect SD DAC plug out !!\n"));

        //change register to detect HD DAC plug in
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 13:13);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 15:15);
        W2BYTE(BK_DAC(REG_DAC_SD_DETECT_CTRL), 0xff11, 15:0);
        W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 10:8);

        //toggle to clear final status
        W2BYTE(BK_DAC(REG_DAC_SD_IRQ_CTRL), TRUE, 5:5);
        W2BYTE(BK_DAC(REG_DAC_SD_IRQ_CTRL), FALSE, 5:5);
    }


    MsOS_EnableInterrupt(E_INT_IRQ_IDAC_PLUG_DET);
}

static void _HAL_DAC_InitHpdInt(void)
{
    //init register for detect HD DAC plug in
    W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 12:12);
    W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 14:14);
    W2BYTE(BK_DAC(REG_DAC_HD_DETECT_CTRL), 0xff21, 15:0);
    W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 13:11);

    //init register for detect SD DAC detect plug in
    W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 13:13);
    W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 15:15);
    W2BYTE(BK_DAC(REG_DAC_SD_DETECT_CTRL), 0xff11, 15:0);
    W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 10:8);

    //HD DAC plug in/out threshld
    W2BYTE(BK_DAC(REG_DAC_HD_PLUG_IN_THD), 0x001F, 9:0);
    W2BYTE(BK_DAC(REG_DAC_HD_PLUG_OUT_THD), 0x03E0, 9:0);

    //SD DAC plug in/out threshld
    W2BYTE(BK_DAC(REG_DAC_SD_PLUG_IN_THD), 0x001F, 9:0);
    W2BYTE(BK_DAC(REG_DAC_SD_PLUG_OUT_THD), 0x03E0, 9:0);

    //Clear interrupt mask of HD DAC
    W2BYTE(BK_DAC(REG_DAC_HD_IRQ_CTRL), 0x0000, 1:0);
    //Clear interrupt mask of SD DAC
    W2BYTE(BK_DAC(REG_DAC_SD_IRQ_CTRL), 0x0000, 1:0);

    //enable interrupt
    MsOS_AttachInterrupt(E_INT_IRQ_IDAC_PLUG_DET, (InterruptCb)_HAL_DAC_HpdIsr);
    MsOS_EnableInterrupt(E_INT_IRQ_IDAC_PLUG_DET);

}

void Hal_DAC_Power_Saving(MS_BOOL POWER_SAVING)
{
     if( 0 == POWER_SAVING )
     {
        W1BYTE(L_BK_CHIPTOP(0x59), 0x00, 0:0);
        W1BYTE(H_BK_CHIPTOP(0x59), 0x00, 0:0);
        W1BYTE(L_BK_CHIPTOP(0x58), 0x00, 0:0);
     }
     else
     {
        W1BYTE(L_BK_CHIPTOP(0x59), 0x01, 0:0);
        W1BYTE(H_BK_CHIPTOP(0x59), 0x01, 0:0);
        W1BYTE(L_BK_CHIPTOP(0x58), 0x01, 0:0);
     }
}

void Hal_DAC_SetIOMapBase(MS_VIRT u32NPMBase, MS_VIRT u32PMBase)
{
    gu32DacRiuBaseAddr = u32PMBase;
}

void Hal_HDMITx_InitSeq(void)
{
    W1BYTE(L_BK_HDMITX(0x45), 0x00, 0:0);//enable PLL power control
    W1BYTE(REG_DAC_04_H, 0x00, REG_EN_IDAC_SEL_REXT_MSK); //Disable External Resistor Selection
    //DAC PLL
    W1BYTE(L_BK_ANA_MISC_HDMI(0x01), 0x01, 0:0); //MPLL_432_IN=432MHz

    //W1BYTE(REG_DAC_PLL_15_L, 0x00, 7:0);
    //W1BYTE(REG_DAC_PLL_15_H, 0x00, 7:0);
    W1BYTE(REG_DAC_PLL_35_L, 0x02, 7:0); //synth_ssc_mode[0] = 0, synth_ssc_sync[1] = 1
    W1BYTE(REG_DAC_PLL_35_H, 0x00, 7:0);
    W1BYTE(REG_DAC_PLL_33_L, 0x00, 7:0); //synth_ssc_step[11:0]=7
    W1BYTE(REG_DAC_PLL_33_H, 0x00, 7:0);
    W1BYTE(REG_DAC_PLL_34_L, 0x00, 7:0); //synth_ssc_span[13:0]=445
    W1BYTE(REG_DAC_PLL_34_H, 0x00, 7:0);

    W2BYTE(BK_DAC(REG_DAC_VSYNC_DETECT_CTRL), 1, 0:0);  //Enable ECO: CVBS hotplug detect in VSync
    W2BYTE(BK_DAC(REG_DAC_PLUG_INOUT_PERIOD), 0x00, 15:8);

}

// for DAC, SD means AV/SV output control, HD means YPbPr output control
// SD = AV/SV
// HD = YPbPr
void Hal_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{

    //Enable IDAC ref.
    W1BYTE(BK_DAC(REG_EN_IDAC_REF), 1, REG_EN_IDAC_REF_MSK);

    //Enable new mode
    W1BYTE(BK_DAC(REG_EN_IDAC_REF), 0, REG_EN_IDAC_HDCLK_MSK);

    //MS_U32 u32DACMode = R1BYTE(H_BK_CHIPTOP_TOP(0x60),15:0);
    //printf("Dac Mode %d\n",u32DACMode);
    //if(u32DACMode == DAC_MODE_VDAC) //VDAC
    //{
    //    W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), 0xf200 , 15:0);
    //    W2BYTE(BK_DAC(REG_DAC_BIAS_CUR_CTRL), 0x0001 , 0:0);
    //    W2BYTE(BK_DAC(REG_DAC_CHN_BUF_EN), 0x000f , 4:0);
    //}
    //else
    {
        //iDAC Mode
        //W2BYTE(BK_DAC(REG_DAC_SD_CTRL), 0x0001 , 1:1);
        //W2BYTE(BK_DAC(REG_DAC_SD_CTRL), 0x0010 , 5:4);
        //W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE , 9:9);
        W2BYTE(BK_DAC(REG_DAC_BIAS_CUR_CTRL), 0x0000 , 0:0);
        W2BYTE(BK_DAC(REG_DAC_CHN_BUF_EN), 0x0000 , 4:0);
    }

    if (bIsYPbPr)
    {
        //On/OFF clock to avoid garbage when TVSystem change
        W2BYTE(L_BK_CHIPTOP(REG_CKG_DAC2), (~bEnable), REG_CKG_DAC2_DISABLE_MASK); //set clock MUX for R/G/B DAC (CLK_ODCLK_P)
        MsOS_DelayTask(30);

        //W2BYTE(L_BK_CHIPTOP(REG_CKG_DAC2), 0x00, REG_CKG_DAC2_MASK); //set clock MUX for R/G/B DAC (CLK_ODCLK_P)
        W1BYTE(BK_DAC(REG_DAC_HD_CTRL), bEnable, REG_EN_IDAC_HDR_MSK);//enable or disable HDGEN YPbPr
        W1BYTE(BK_DAC(REG_DAC_HD_CTRL), bEnable, REG_EN_IDAC_HDG_MSK);//enable or disable HDGEN YPbPr
        W1BYTE(BK_DAC(REG_DAC_HD_CTRL), bEnable, REG_EN_IDAC_HDB_MSK);//enable or disable HDGEN YPbPr
        W1BYTE(BK_DAC(REG_HD_DAT_CLK_INV), bEnable, REG_HD_DAT_CLK_INV_MSK);//enable or disable HDGEN YPbPr
        W1BYTE(BK_DAC(REG_HD_DAT_CLK_DLY), 0, REG_HD_DAT_CLK_DLY_MSK);

        //Disable HDGEN pattern Gen
        MS_U16 u16Bank = (R2BYTE_TAB(HDGEN_BK_SELECT, 0xFF) & 0xFF);
        W2BYTE_TAB(HDGEN_BK_SELECT, 0x2, 0xFF);
        W1BYTE_TAB(BK_HDGEN(0x60), 0, 0x3);
        //LPF and upsample
        W2BYTE_TAB(HDGEN_BK_SELECT, 0x3, 0xFF);
        W1BYTE_TAB(BK_HDGEN(0xC0), 1, 0x1);
        W2BYTE_TAB(HDGEN_BK_SELECT, 0x2, 0xFF);
        W1BYTE_TAB(BK_HDGEN(0x80), 0, 0x1);
        W2BYTE_TAB(HDGEN_BK_SELECT, u16Bank, 0xFF);

        //Set HD Output Swap Sel
        if(!bIsHDDACInited)
        {
            Hal_DAC_SetOutputSwapSel(DAC_HDDAC_DEFAULT_OUTPUTSWAPSEL, bIsYPbPr);
        }
    }
    else
    {
        //On/OFF clock to avoid garbage when TVSystem change
        W1BYTE(L_BK_CHIPTOP(REG_CKG_DAC1), (~bEnable), REG_CKG_DAC1_DISABLE_MASK); //set clock MUX for X/Y/C DAC (CLK_VEDAC_P)
        MsOS_DelayTask(30);

        //W1BYTE(L_BK_CHIPTOP(REG_CKG_DAC1), 0x03, REG_CKG_DAC1_MASK); //set clock MUX for X/Y/C DAC (CLK_VEDAC_P)
        // SD GAIN & Enable SD GAIN or not
        W2BYTE(BK_DAC(REG_GCR_IDAC_GAINSD),    0, REG_GCR_IDAC_GAINSD_MSK);
        W2BYTE(BK_DAC(REG_EN_REF_IDAC_GAINSD), 0, REG_EN_REF_IDAC_GAINSD_MSK);
        // Clk inverse & phase
        W1BYTE(BK_DAC(REG_SD_DAT_CLK_INV), 1, REG_SD_DAT_CLK_INV_MSK);
        W1BYTE(BK_DAC(REG_SD_DAT_CLK_DLY), 0, REG_SD_DAT_CLK_DLY_MSK);

        if (bEnable)
        {
            // [0] = CVBS, [1] = Y of S-video, [2] = C of S-video
            // Enable C/Y/X,
            W1BYTE(BK_DAC(REG_EN_IDAC_SDX), 1, REG_EN_IDAC_SDX_MSK);
            W1BYTE(BK_DAC(REG_EN_IDAC_SDY), 1, REG_EN_IDAC_SDY_MSK);
            W1BYTE(BK_DAC(REG_EN_IDAC_SDC), 1, REG_EN_IDAC_SDC_MSK);
        }
        else
        {
            // [0] = CVBS, [1] = Y of S-video, [2] = C of S-video
            // Enable C/Y/X,
            W1BYTE(BK_DAC(REG_EN_IDAC_SDX), 0, REG_EN_IDAC_SDX_MSK);
            W1BYTE(BK_DAC(REG_EN_IDAC_SDY), 0, REG_EN_IDAC_SDY_MSK);
            W1BYTE(BK_DAC(REG_EN_IDAC_SDC), 0, REG_EN_IDAC_SDC_MSK);
        }

        //Set SD Output Swap Sel
        if(!bIsSDDACInited)
        {
            Hal_DAC_SetOutputSwapSel(DAC_SDDAC_DEFAULT_OUTPUTSWAPSEL, bIsYPbPr);
        }
    }

    W1BYTE(BK_DAC(REG_EN_IDAC_REF), 1, 6:6);
    W1BYTE(BK_DAC(REG_EN_IDAC_REF), 0, 6:6);

}

void Hal_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
//    if (bIsYPbPr)
    {
        W2BYTE(BK_DAC(REG_DAC_HD_CLK), bEnable, 0:0);
    }
//    else
    {
        W2BYTE(BK_DAC(REG_DAC_SD_CLK), bEnable, 0:0);
    }
}

void Hal_DAC_SetOutputSource(EN_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    MS_U8 Output_type,DacClk;

    switch(enOutputType)
    {
        case DAC_TO_HDGEN:
            Output_type = 0x00;
            DacClk = 0x00;
            break;

        case DAC_TO_VE:
            Output_type = 0x01;
            DacClk = 0x03;
            break;

        case DAC_TO_VE_YPBPR:
            Output_type = 0x03;
            DacClk = 0x03;
            break;
        default:
            printf("Invalid DAC output %d", enOutputType);
            return;
    }
    if (bIsYPbPr)
    {
        W1BYTE(BK_DAC(REG_DAC_HD_SEL), Output_type, 1:0);
        W1BYTE(H_BK_CHIPTOP(REG_CKG_DAC2), DacClk, 3:2);

        if((enOutputType == DAC_TO_VE_YPBPR)||(enOutputType == DAC_TO_VE))
        {
            W1BYTE(BK_DAC(REG_EN_IDAC_REF), 1, REG_EN_IDAC_HDCLK_MSK);
        }
        else
        {
            W1BYTE(BK_DAC(REG_EN_IDAC_REF), 0, REG_EN_IDAC_HDCLK_MSK);
        }
    }
    else
    {
        W1BYTE(BK_DAC(REG_DAC_SD_SEL), Output_type, 1:0);
        W1BYTE(L_BK_CHIPTOP(REG_CKG_DAC1), DacClk, 3:2);
    }


}

void Hal_DAC_SetOutputLevel(EN_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    //MS_U32 u32DACMode = R1BYTE(H_BK_CHIPTOP_TOP(0x60),15:0);

    //if(u32DACMode == DAC_MODE_VDAC) //VDAC
    //{
        //return; // VDAC mode need ignore this
    //}

    if (bIsYPbPr)
    {
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)enLevel, 9:9);
    }
    else
    {
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)enLevel, 10:10);
    }
}

void Hal_DAC_SetOutputSwapSel(EN_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    if (bIsYPbPr)
    {
        W2BYTE(BK_DAC(REG_HD_DAC_SWAP_SEL), enSwap, 10:8);
    }
    else
    {
        W2BYTE(BK_DAC(REG_SD_DAC_SWAP_SEL), enSwap, 10:8);
    }
}

void Hal_DAC_OnOffSD(EN_DAC_SD_ENABLE_CTRL enBit)
{
    W1BYTE(BK_DAC(REG_DAC_SD_CTRL),enBit , 2:0);
}
EN_DAC_SD_ENABLE_CTRL Hal_DAC_GetSDStatus(void)
{
    return R1BYTE(BK_DAC(REG_DAC_SD_CTRL), 2:0);
}
void Hal_DAC_OnOffHD(EN_DAC_HD_ENABLE_CTRL enBit)
{
    W1BYTE(BK_DAC(REG_DAC_HD_CTRL),enBit , 2:0);
}
EN_DAC_HD_ENABLE_CTRL Hal_DAC_GetHDStatus(void)
{
    return R1BYTE(BK_DAC(REG_DAC_HD_CTRL), 2:0);
}
void Hal_HDGEN_SetTiming(EN_OUTPUT_VIDEO_TIMING_TYPE enTiming)
{
#if 0 //use mdrv_dac_tbl instead

    MS_U8 *pu8Table;
    MS_U32 u32Addr = 0;

    if (enTiming == RES_720x480I_60Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_480I_60;
    }
    else if (enTiming == RES_720x480P_60Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_480P_60;
    }
    else if (enTiming == RES_720x576I_50Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_576I_50;
    }
    else if (enTiming == RES_720x576P_50Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_576P_50;
    }
    else if (enTiming == RES_1280x720P_50Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_720P_50;
    }
    else if (enTiming == RES_1280x720P_60Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_720P_60;
    }
    else if (enTiming == RES_1920x1080I_50Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_1080I_50;
    }
    else if (enTiming == RES_1920x1080I_60Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_1080I_60;
    }
    else if (enTiming == RES_1920x1080P_50Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_1080P_50;
    }
    else if (enTiming == RES_1920x1080P_60Hz)
    {
        pu8Table = (MS_U8 *)HDGEN_1080P_60;
    }
    else
    {
        printf("Invalid timing type %d", enTiming);
        return;
    }

    while (1)
    {
        u32Addr = (pu8Table[0]<<8) + pu8Table[1];
        if (u32Addr == 0xFFFF)
            break;
        //printf("%x: %x\n", (int)(NONPM_BASE+u32Addr), pu8Table[2]);
        W1BYTE((NONPM_BASE+u32Addr), pu8Table[2], 7:0);
        pu8Table += 4;
    }
#else

    //Use the timing to get the reference Dac Panel index
    switch(enTiming)
    {
        case RES_720x480I_60Hz:
            _geDACPanel_index = DAC_PANEL_480I_60;
            break;
        case RES_720x480P_60Hz:
            _geDACPanel_index = DAC_PANEL_480P_60;
            break;
        case RES_720x576I_50Hz:
            _geDACPanel_index = DAC_PANEL_576I_50;
            break;
        case RES_720x576P_50Hz:
            _geDACPanel_index = DAC_PANEL_576P_50;
            break;
        case RES_1280x720P_50Hz:
            _geDACPanel_index = DAC_PANEL_720P_50;
            break;
        case RES_1280x720P_60Hz:
            _geDACPanel_index = DAC_PANEL_720P_60;
            break;
        case RES_1920x1080I_50Hz:
            _geDACPanel_index = DAC_PANEL_1080I_50;
            break;
        case RES_1920x1080I_60Hz:
            _geDACPanel_index = DAC_PANEL_1080I_60;
            break;
        case RES_1920x1080P_50Hz:
            _geDACPanel_index = DAC_PANEL_1080P_50;
            break;
        case RES_1920x1080P_60Hz:
            _geDACPanel_index = DAC_PANEL_1080P_60;
            break;
        case RES_1920x1080P_24Hz:
            _geDACPanel_index = DAC_PANEL_1080P_24;
            break;
        case RES_1920x1080P_25Hz:
            _geDACPanel_index = DAC_PANEL_1080P_25;
            break;
        case RES_1920x1080P_30Hz:
            _geDACPanel_index = DAC_PANEL_1080P_30;
            break;
        case RES_1920x2205P_24Hz:
            _geDACPanel_index = DAC_PANEL_1920x2205P_24;
            break;
        case RES_1280x1470P_50Hz:
            _geDACPanel_index = DAC_PANEL_1280x1470P_50;
            break;
        case RES_1280x1470P_60Hz:
            _geDACPanel_index = DAC_PANEL_1280x1470P_60;
            break;
        case RES_640x480P_60Hz:
            _geDACPanel_index = DAC_PANEL_640x480_60;
            break;
        case RES_3840x2160P_24Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P_24;
            break;
        case RES_3840x2160P_25Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P_25;
            break;
        case RES_3840x2160P_30Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P_30;
            break;
        case RES_3840x2160P_50Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P_50;
            break;
        case RES_3840x2160P_60Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P_60;
            break;
        case RES_4096x2160P_24Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P_24;
            break;
        case RES_4096x2160P_25Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P_25;
            break;
        case RES_4096x2160P_30Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P_30;
            break;
        case RES_4096x2160P_50Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P_50;
            break;
        case RES_4096x2160P_60Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P_60;
            break;
        case RES_3840x2160P420_30Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P420_30;
            break;
        case RES_3840x2160P420_50Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P420_50;
            break;
        case RES_3840x2160P420_60Hz:
            _geDACPanel_index = DAC_PANEL_3840x2160P420_60;
            break;
        case RES_4096x2160P420_30Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P420_30;
            break;
        case RES_4096x2160P420_50Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P420_50;
            break;
        case RES_4096x2160P420_60Hz:
            _geDACPanel_index = DAC_PANEL_4096x2160P420_60;
            break;
        case RES_1600x1200P_60Hz:
            _geDACPanel_index = DAC_PANEL_1600x1200P_60;
            break;
        case RES_1440x900P_60Hz:
            _geDACPanel_index = DAC_PANEL_1440x900P_60;
            break;
        case RES_1280x1024P_60Hz:
            _geDACPanel_index = DAC_PANEL_1280x1024P_60;
            break;
        case RES_1024x768P_60Hz:
            _geDACPanel_index = DAC_PANEL_1024x768P_60;
            break;
        default:
            printf("[%s][%d]Wrong Output Video Timing Type %d!!!\n",__FUNCTION__,__LINE__,enTiming);
            return;
    }

    printf("DAC Panel Table Index =%d\n", _geDACPanel_index);

#endif
}

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
void Hal_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    MS_U32 u32DACMode = R1BYTE(H_BK_CHIPTOP_TOP(0x60),15:0);

    if(u32DACMode == DAC_MODE_VDAC) //VDAC
    {
        return; // VDAC mode need ignore this
    }

    if (bIsYPbPr)
    {
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)bEnable, 12:12);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), DISABLE, 14:14);
    }
    else
    {
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)bEnable, 13:13);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), DISABLE, 15:15);
    }
}

//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
void Hal_DAC_SetQuartOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    MS_U32 u32DACMode = R1BYTE(H_BK_CHIPTOP_TOP(0x60),15:0);

    if(u32DACMode == DAC_MODE_VDAC) //VDAC
    {
        return; // VDAC mode need ignore this
    }

    if (bIsYPbPr)
    {
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)bEnable, 12:12);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)bEnable, 14:14);
    }
    else
    {
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)bEnable, 13:13);
        W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), (MS_BOOL)bEnable, 15:15);
    }
}

//----------------------------------------------------------------
// Hal_DAC_SetDACState - Set DAC init state
// @param: bEnable: TRUE for DAC is initialized, FALSE for not initialized
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return none
//----------------------------------------------------------------
void Hal_DAC_SetDACState(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (bIsYPbPr)
    {
        bIsHDDACInited = bEnable;
    }
    else
    {
        bIsSDDACInited = bEnable;
    }
}

//----------------------------------------------------------------
// Hal_DAC_GetDACState - Get DAC init state
// @param: bIsYPbPr: TRUE for YPbPr, FALSE for CVBS
// @return: TRUE is DAC is initialized
//----------------------------------------------------------------
MS_BOOL Hal_DAC_GetDACState(MS_BOOL bIsYPbPr)
{
    if (bIsYPbPr)
    {
        return bIsHDDACInited;
    }
    else
    {
        return bIsSDDACInited;
    }
}

//----------------------------------------------------------------
// Hal_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL Hal_DAC_HotPlugDetect(EN_DAC_DETECT SelDAC, EN_DAC_DETECT_TYPE DetectType, MS_BOOL *bIsSignIn)
{
    MS_U16 u16DetectCount = DAC_DETECT_COUNT;
    MS_U16 u16TimeoutCount = 0;
    MS_U16 Channel_Select = BIT(5);

    if(bIsSignIn == NULL)
    {
        printf("[%s][%d]ERROR: Function pointer is NULL!!!\n",__FUNCTION__,__LINE__);
        return  FALSE;
    }

    *bIsSignIn = FALSE;

    switch(R2BYTE(BK_DAC(REG_DAC_HD_SEL), 9:8))
    {
        case DAC_DETECT_R_Channel:
            Channel_Select = BIT(4);
            break;
        case DAC_DETECT_G_Channel:
            Channel_Select = BIT(5);
            break;
        case DAC_DETECT_B_Channel:
            Channel_Select = BIT(6);
            break;
        default:
            Channel_Select = BIT(5);
            break;
    }

    if(SelDAC == DAC_DETECT_HD)
    {
        switch(DetectType)
        {
            case DAC_DETECT_PLUGIN:
                W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 12:12);
                W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 14:14);
                W2BYTE(BK_DAC(REG_DAC_HD_DETECT_CTRL), (0xff01|Channel_Select), 15:0);
                W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 13:11);
                break;
            case  DAC_DETECT_PLUGOUT:
                W2BYTE(BK_DAC(REG_DAC_HD_DETECT_CTRL), (0x2402|Channel_Select), 15:0);
                W2BYTE(BK_DAC(REG_DAC_Delay_Sel), 0xaa02, 15:0);
                W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 13:11);
                break;
            default:
                printf("[%s][%d]Wrong DAC DetectType %d!!!\n",__FUNCTION__,__LINE__,DetectType);
                return FALSE;
        }

        MsOS_DelayTask(DAC_DETECT_DELAY);  //delay for voltage stable
        while((u16DetectCount > 0)&&(u16TimeoutCount < DAC_DETECT_TIMEOUT))  // check detect timeout
        {
            if((R2BYTE(BK_DAC(REG_DAC_HD_CMP_RESULT), 15:6) >> 6) <= DAC_DETECT_COUNT_HALF) // check detect result
            {
                u16DetectCount--;  //detect dac is using
                DAC_DBG(printf("[%s][%d]==HD=DELETE=%d===\n",__FUNCTION__,__LINE__,u16DetectCount));
                if(u16DetectCount == 0)
                {
                    *bIsSignIn = true;
                    if(DetectType == DAC_DETECT_PLUGIN)
                    {
                        W2BYTE(BK_DAC(REG_DAC_HD_DETECT_CTRL), FALSE, 5:5);
                    }
                }
            }

            DAC_DBG(printf("[%s][%d]==HD=u16TimeoutCount=%d===\n",__FUNCTION__,__LINE__,u16TimeoutCount));
            u16TimeoutCount++;
        }
    }
    else if(SelDAC == DAC_DETECT_SD)
    {
        switch(DetectType)
        {
            case DAC_DETECT_PLUGIN:
                W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 13:13);
                W2BYTE(BK_DAC(REG_DAC_LEVEL_CTRL), TRUE, 15:15);
                W2BYTE(BK_DAC(REG_DAC_SD_DETECT_CTRL), 0xff11, 15:0);
                W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0000, 10:8);
                break;
            case  DAC_DETECT_PLUGOUT:
                W2BYTE(BK_DAC(REG_DAC_SD_DETECT_CTRL), 0x0b12, 15:0);
                W2BYTE(BK_DAC(REG_DAC_Delay_Sel), 0xaa02, 15:0);
                W2BYTE(BK_DAC(REG_DAC_GCR_LEVEL_CTRL), 0x0006, 10:8);
                break;
            default:
                printf("[%s][%d]Wrong DAC DetectType %d!!!\n",__FUNCTION__,__LINE__,DetectType);
                return FALSE;
        }

        MsOS_DelayTask(DAC_DETECT_DELAY); //delay for voltage stable
        while((u16DetectCount > 0)&&(u16TimeoutCount < DAC_DETECT_TIMEOUT)) // check detect timeout
        {
                if((R2BYTE(BK_DAC(REG_DAC_SD_CMP_RESULT), 15:6) >> 6) <= DAC_DETECT_COUNT_HALF) // check detect result
                {
                    u16DetectCount--; //detect dac is using
                    DAC_DBG(printf("[%s][%d]==SD=DELETE=%d===\n",__FUNCTION__,__LINE__,u16DetectCount));
                    if(u16DetectCount == 0)
                    {
                        *bIsSignIn = true;
                        if(DetectType == DAC_DETECT_PLUGIN)
                        {
                            W2BYTE(BK_DAC(REG_DAC_SD_DETECT_CTRL), FALSE, 4:4);
                        }
                    }
                }

            DAC_DBG(printf("[%s][%d]==SD=u16TimeoutCount=%d===\n",__FUNCTION__,__LINE__,u16TimeoutCount));
            u16TimeoutCount++;
        }
    }
    else
    {
        printf("[%s][%d]Wrong DAC Type %d!!!\n",__FUNCTION__,__LINE__,SelDAC);
        return  FALSE;
    }

    return TRUE;
}
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
//----------------------------------------------------------------
// Hal_DAC_GetOutputInterlaceTiming
// @return: TRUE is interlace or not
//----------------------------------------------------------------
MS_BOOL Hal_DAC_GetOutputInterlaceTiming(void)
{
    MS_U8 u8Ret = 0;
    u8Ret = R2BYTE(DAC_SC_OUTPUT_INTERLACE, 1:1);
    if(u8Ret == 0x02)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
//----------------------------------------------------------------
// Hal_DAC_EnableHotPlugDetectISR - Enable DAC HotPlug detect ISR
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL Hal_DAC_EnableHotPlugDetectISR(MS_BOOL bEnabled)
{
    if(bDacIRQInit == bEnabled)
    {
        return TRUE;
    }

    if(bEnabled)
    {
        _HAL_DAC_InitHpdInt();
        bDacIRQInit = TRUE;
    }
    else
    {
        MsOS_DisableInterrupt(E_INT_IRQ_IDAC_PLUG_DET);
        MsOS_DetachInterrupt(E_INT_IRQ_IDAC_PLUG_DET);
        bDacIRQInit = FALSE;
    }

    return TRUE;
}

//----------------------------------------------------------------
// Hal_DAC_DumpTable - Dump DAC tables
// @param: pDACTable: pointer to DAC table
// @param: u8DACtype: DAC table type
//----------------------------------------------------------------
void Hal_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    MS_U8 u8DacTab[DAC_TAB_MAX_SIZE];
    MS_U32 u32DacTabsize = 0;
    DAC_TAB_INFO DacTbl;

    DacTbl = DACPANEL_MainTbl[_geDACPanel_index];

    MS_BOOL bForce8BitMode = FALSE;
    // we only support max 6G bandwidth, 4K2K 50/60 must force 8 bit mode(more than 8 bit will cause over 6G if not 420 timing))
    // This solution need sync with HDMI setting
    if ((_geDACPanel_index == DAC_PANEL_3840x2160P_50) ||
        (_geDACPanel_index == DAC_PANEL_3840x2160P_60) ||
        (_geDACPanel_index == DAC_PANEL_4096x2160P_50) ||
        (_geDACPanel_index == DAC_PANEL_4096x2160P_60))
    {
        bForce8BitMode = TRUE;
    }

    switch(u8DACtype)
    {
        case DAC_TABTYPE_INIT:
            pDACTable = DacTbl.pDacINITTab;
            break;
        case DAC_TABTYPE_INIT_GPIO:
            pDACTable = DacTbl.pDacINIT_GPIOTab;
            break;
        case DAC_TABTYPE_INIT_SC:
            pDACTable = DacTbl.pDacINIT_SCTab;
            break;
        case DAC_TABTYPE_INIT_MOD:
            pDACTable = DacTbl.pDacINIT_MODTab;
            break;
        case DAC_TABTYPE_INIT_HDGEN:
            pDACTable = DacTbl.pDacINIT_HDGENTab;
            break;
        case DAC_TABTYPE_INIT_HDMITX_8BIT:
            pDACTable = DacTbl.pDacINIT_HDMITX_8BITTab;
            break;
        case DAC_TABTYPE_INIT_HDMITX_10BIT:
            if(TRUE == bForce8BitMode)
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_8BITTab;
            }
            else
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_10BITTab;
            }
            break;
        case DAC_TABTYPE_INIT_HDMITX_12BIT:
            if(TRUE == bForce8BitMode)
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_8BITTab;
            }
            else
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_12BITTab;
            }
            break;
        case DAC_TABTYPE_INIT_HDMITX_16BIT:
            if(TRUE == bForce8BitMode)
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_8BITTab;
            }
            else
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_16BITTab;
            }
            break;
        case DAC_TABTYPE_INIT_HDMITX_8BIT_Divider:
            pDACTable = DacTbl.pDacINIT_HDMITX_8BIT_DividerTab;
            break;
        case DAC_TABTYPE_INIT_HDMITX_10BIT_Divider:
            if(TRUE == bForce8BitMode)
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_8BIT_DividerTab;
            }
            else
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_10BIT_DividerTab;
            }
            break;
        case DAC_TABTYPE_INIT_HDMITX_12BIT_Divider:
            if(TRUE == bForce8BitMode)
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_8BIT_DividerTab;
            }
            else
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_12BIT_DividerTab;
            }
            break;
        case DAC_TABTYPE_INIT_HDMITX_16BIT_Divider:
            if(TRUE == bForce8BitMode)
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_8BIT_DividerTab;
            }
            else
            {
                pDACTable = DacTbl.pDacINIT_HDMITX_16BIT_DividerTab;
            }
            break;
        case DAC_TABTYPE_SC1_INIT:
            pDACTable = DacTbl.pDacSC1_INITTab;
            break;
        case DAC_TABTYPE_SC1_INIT_SC:
            pDACTable = DacTbl.pDacSC1_INIT_SCTab;
            break;
        default:
            DAC_DBG(printf( "GetTable:unknown Tab type> \n"));
            return;
    }

    _Hal_DAC_GetTabSize(pDACTable, u8DACtype, &u32DacTabsize);
    memcpy(u8DacTab, pDACTable, u32DacTabsize);

    switch(u8DACtype)
    {
    case DAC_TABTYPE_INIT:
    case DAC_TABTYPE_INIT_GPIO:
    case DAC_TABTYPE_SC1_INIT:
        _Hal_DAC_DumpGENRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_SC:
        _Hal_DAC_DumpSCRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_MOD:
        _Hal_DAC_DumpMODRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDGEN:
        _Hal_DAC_DumpHDGENRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDMITX_8BIT:
        _Hal_DAC_DumpHDMITxRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDMITX_10BIT:
        _Hal_DAC_DumpHDMITxRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDMITX_12BIT:
        _Hal_DAC_DumpHDMITxRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDMITX_8BIT_Divider:
        _Hal_DAC_DumpHDMITxDivRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDMITX_10BIT_Divider:
        _Hal_DAC_DumpHDMITxDivRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_INIT_HDMITX_12BIT_Divider:
        _Hal_DAC_DumpHDMITxDivRegTab(u8DacTab);
        break;
    case DAC_TABTYPE_SC1_INIT_SC:
        _Hal_DAC_DumpSCRegTab(u8DacTab);
        break;
    default:
        DAC_DBG(printf( "GetTable:unknown Tab type> \n"));
        break;
    }

}

void Hal_DAC_SetDataReverse(MS_BOOL bTrue)
{
    W2BYTE(BK_DAC(REG_DAC_BIAS_CUR_CTRL), bTrue, 0:0);
}

void Hal_DAC_EnableChannelBuf(MS_BOOL bTrue)
{
    W2BYTE(BK_DAC(REG_DAC_CHN_BUF_EN), bTrue, 0:0);
    W2BYTE(BK_DAC(REG_DAC_CHN_BUF_EN), bTrue, 1:1);
    W2BYTE(BK_DAC(REG_DAC_CHN_BUF_EN), bTrue, 2:2);
    W2BYTE(BK_DAC(REG_DAC_CHN_BUF_EN), bTrue, 3:3);
}

void Hal_HDGEN_SetWSSOnOff(MS_BOOL bEnable)
{
    MS_U32 u32Addr = 0;

    //Change HDGEN_HDVBI Bank
    W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

    u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_05_L;
    W1BYTE_TAB(u32Addr, bEnable << 1, 0x02);
}

MS_BOOL Hal_HDGEN_GetWSSStatus(void)
{
    MS_U32 u32Addr = 0;

    //Change HDGEN_HDVBI Bank
    W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);
    u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_05_L;

    if(R2BYTE_TAB(u32Addr, 0x0002) == 0x0002)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void Hal_HDGEN_ResetWSSData(void)
{
    MS_U32 u32Addr = 0;

    //Change HDGEN_HDVBI Bank
    W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

    //Reset HDGEN VBI 0x14~0x1C 134 bit Data
    for(u32Addr = (HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_14_L); u32Addr <= (HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_1C_L); u32Addr=u32Addr+2)
    {
        W2BYTE_TAB(u32Addr, 0x0000, ((u32Addr == (HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_1C_L))? 0x002F:0xFFFF));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_DAC_Set_WSS_data()
/// @brief \b Function \b Description : Set the WSS data of YPbPr
///
/// @param <IN>        \b bEn       : Enable, Disable
/// @param <IN>        \b u8WSSData : The WSS data
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Hal_HDGEN_Set_WSS_data(MS_BOOL ben, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, MS_U16 u16wssdata)
{
    MS_U32 u32Addr = 0;

    if(ben)
    {
        MS_U8 *pDACTable;
        MS_U8 u8DacTab[DAC_TAB_MAX_SIZE];
        MS_U32 u32DacTabsize = 0;

        switch(eVideo_Timing)
        {
            case RES_720x576I_50Hz:
                pDACTable = &HDGEN_VBITBL_WSS_576I_50[0][0];
                break;
            case RES_720x576P_50Hz:
                pDACTable = &HDGEN_VBITBL_WSS_576P_50[0][0];
                break;
            default:
                DAC_DBG(printf( "GetTable:unknown Tab type> \n"));
                return FALSE;
        }

        _Hal_HDGEN_VBI_GetTabSize(pDACTable, HDGEN_TABTYPE_VBI, &u32DacTabsize);
        memcpy(u8DacTab, pDACTable, u32DacTabsize);

        _Hal_HDGEN_DumpVBIRegTab(u8DacTab);

        //Change HDGEN_HDVBI Bank
        W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

        u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_14_L;
        W2BYTE_TAB(u32Addr, u16wssdata&0x3FFF, 0x3FFF);
    }
    else
    {
        //Change HDGEN_HDVBI Bank
        W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

        u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_05_L;

        W1BYTE_TAB(u32Addr, 0x00, 0x01);

        //MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(2));
    }

    return TRUE;
}

MS_U16 Hal_HDGEN_Get_WSS_data(void)
{
    MS_U32 u32Addr = 0;

    //Change HDGEN_HDVBI Bank
    W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

    u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_14_L;

    return ((MS_U16)R2BYTE_TAB(u32Addr, 0x3FFF));
}

#define MS_HDGEN_CRC6_START_BIT    (14)
static void _Hal_HDGEN_CalcCRC6(MS_U32 *u32wssdata)
{
    // initialization, bit5-bit0 = b'111111
    MS_U32 u32TempWssData = *u32wssdata;
    MS_U8 u8Crc = 0x3F, u8CrcTemp = 0x3F;
    MS_U8 u8Index = 0;
    MS_U8 u8LowestBit, u8Bit5, u8Bit4 = 0x0;

    DAC_DBG(printf("[%s][%d] u32TempWssData = 0x%05lx \n", __FUNCTION__, __LINE__, u32TempWssData));
    /* for each bit, do operations as below:
     * (1) for last 4 bits, shift right 1 bit
     * (2) for the first 2 bits, do xor operation base on different algorithm
     */
    for(u8Index = 0; u8Index < MS_HDGEN_CRC6_START_BIT; u8Index++)
    {
        u8LowestBit = (( u32TempWssData >> u8Index) & 0x1);
        u8CrcTemp = (u8Crc >> 1) & 0x0F;            // for last 4 CRC bits (b3-b0), temp CRC = CRC shift right 1 bit
        u8Bit5 = u8LowestBit ^ ( u8Crc & BIT(0) );  // for CRC bit5, let lowest bit of WSS to do xor with lowest CRC bit
        u8CrcTemp = u8CrcTemp | ((u8Bit5) << 5);
        u8Bit4 = (u8CrcTemp >> 5) ^ (u8Crc >> 5);   // for CRC bit4, let bit5 of CRC to do xor with bit 5 of tempCRC
        u8CrcTemp = u8CrcTemp | ((u8Bit4) << 4);
        u8Crc = u8CrcTemp;                          // re-store CRC
        DAC_DBG(printf("[%s][%d] u8Crc = 0x%x \n", __FUNCTION__, __LINE__, u8Crc));
    }

    // reorder the result of WSS data, b19-b0
    u32TempWssData = (u32TempWssData & 0x3FFF) | (u8Crc << MS_HDGEN_CRC6_START_BIT);
    *u32wssdata = u32TempWssData;
    DAC_DBG(printf("[%s][%d] u32WssData = 0x%05lx \n", __FUNCTION__, __LINE__, u32TempWssData));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_wss525_data()
/// @brief \b Function \b Description : Set the WSS data of video encoder for 525i System
///
/// @param <IN>        \b bEn       : Enable, Disable
/// @param <IN>        \b u8WSSData : The WSS data
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Hal_HDGEN_Set_WSS525_A_data(MS_BOOL ben, EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing, MS_U32 u32wssdata)
{
    MS_U16 u16Wss525_A_Info1 = 0, u16Wss525_A_Info2 = 0;
    MS_U32 u32Addr = 0;

    if(ben)
    {
        MS_U8 *pDACTable;
        MS_U8 u8DacTab[DAC_TAB_MAX_SIZE];
        MS_U32 u32DacTabsize = 0;

        switch(eVideo_Timing)
        {
            case RES_720x480I_60Hz:
                pDACTable = &HDGEN_VBITBL_WSS525_A_480I_60[0][0];
                break;
            case RES_720x480P_60Hz:
                pDACTable = &HDGEN_VBITBL_WSS525_A_480P_60[0][0];
                break;
            case RES_1280x720P_50Hz:
                pDACTable = &HDGEN_VBITBL_WSS525_A_720P_50[0][0];
                break;
            case RES_1280x720P_60Hz:
                pDACTable = &HDGEN_VBITBL_WSS525_A_720P_60[0][0];
                break;
            case RES_1920x1080I_50Hz:
                pDACTable = &HDGEN_VBITBL_WSS525_A_1080I_50[0][0];
                break;
            case RES_1920x1080I_60Hz:
                pDACTable = &HDGEN_VBITBL_WSS525_A_1080I_60[0][0];
                break;
            default:
                DAC_DBG(printf( "GetTable:unknown Tab type> \n"));
                return FALSE;
        }

        _Hal_HDGEN_VBI_GetTabSize(pDACTable, HDGEN_TABTYPE_VBI, &u32DacTabsize);
        memcpy(u8DacTab, pDACTable, u32DacTabsize);

        _Hal_HDGEN_DumpVBIRegTab(u8DacTab);

        /*
             * The WSS consists of 2 bits of star code(generated by HW),
             * 14 bits of data (b13 - b0) and 6 bits of CRC (b19:b14).
             * b19:b0 -> info2, info1
             * b19:b0 -> 0000, 0000 0000 0000 0000
             *
             * Cyclic Redundancy Check (CRC):
             * The Generator polynomial G(X) of the CRC is G(X) = X6 + X + 1,
             * in which, with an initial state of all 1's
             */
        _Hal_HDGEN_CalcCRC6(&u32wssdata);

        u16Wss525_A_Info2 = (MS_U16)( (u32wssdata >>16) & 0x000F );
        u16Wss525_A_Info1 = (MS_U16)( u32wssdata & 0xFFFF);

        //Change HDGEN_HDVBI Bank
        W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

        u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_15_L;
        W2BYTE_TAB(u32Addr, u16Wss525_A_Info2, 0x000F);
        u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_14_L;
        W2BYTE_TAB(u32Addr, u16Wss525_A_Info1, 0xFFFF);
    }
    else
    {
        //Change HDGEN_HDVBI Bank
        W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

        u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_05_L;
        W2BYTE_TAB(u32Addr, 0x00, 0x01);

        //MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(2));
    }

    return TRUE;
}

MS_U32 Hal_HDGEN_Get_WSS525_A_data(void)
{
    MS_U32 u32Wss525Data = 0;
    MS_U32 u32Addr = 0;

    //Change HDGEN_HDVBI Bank
    W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_HDVBI, 0xFFFF);

    u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_15_L;
    u32Wss525Data = (MS_U32)R2BYTE_TAB(u32Addr, 0xF)<< 16;
    u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_14_L;
    u32Wss525Data = u32Wss525Data | (MS_U32)R2BYTE_TAB(u32Addr, 0xFFFF);

    return ( u32Wss525Data & 0xFFFFF );
}

MS_BOOL Hal_HDGEN_EnableICT(MS_BOOL bEnable)
{
    MS_U32 u32Addr = 0;
    //Change HDGEN_HDVBI Bank
    W2BYTE_TAB(HDGEN_BK_SELECT, HDGEN_P2I, 0xFFFF);

    u32Addr = HDGEN_ADDR_OFFSET | REG_ADDR_HDGEN_2B_L;
    if (bEnable)
    {
        W1BYTE_TAB(u32Addr, 0x0002, 0x0002);
    }
    else
    {
        W1BYTE_TAB(u32Addr, 0x0000, 0x0002);
    }
    return TRUE;
}


//----------------------------------------------------------------
// DAC - set VGA Hsync Vsync
// @return none
//----------------------------------------------------------------
void Hal_DAC_SetVGAHsyncVsync(MS_BOOL bEnable)
{
    if (bEnable)
    {
        W1BYTE(L_BK_CHIPTOP_TOP(REG_HSYNC_VSYNC_EN), 0x03, 1:0);    //Enable Hsync Vsync
        W2BYTE_TAB(HDGEN_BK_SELECT, 0x1, 0x1);
        W1BYTE_TAB(BK_HDGEN(0x7C), 0x1, 0x1);           //select source of oversample filter : #1: vga out
    }
    else
    {
        W1BYTE(L_BK_CHIPTOP_TOP(REG_HSYNC_VSYNC_EN), 0x00, 1:0);    //Disable Hsync Vsync
        W2BYTE_TAB(HDGEN_BK_SELECT, 0x1, 0x1);
        W1BYTE_TAB(BK_HDGEN(0x7C), 0x0, 0x1);           //select source of oversample filter : #0: hdgen out
    }
}

