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
//==============================================================================
// [mhal_sc.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MHAL_SC_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#endif

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#ifdef UFO_XC_VMARK
#include "apiXC_VMark.h"
#endif
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "mhal_dynamicscaling.h"
#include "mdrv_sc_dynamicscaling.h"
#include "drvMVOP.h"

#include "drvXC_HDMI_Internal.h"

#include "mhal_sc.h"
#include "mhal_menuload.h"
#include "halCHIP.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#include "drv_sc_isr.h"
#include "drvXC_HDMI_if.h"
#include "mhal_pip.h"
#include "XC_private.h"
#include "mhal_menuload.h"
#include "halCHIP.h"
#include "drvSYS.h"
#include "drv_sc_scaling.h"
#include "drvSEAL.h"
#include "hwreg_vmark.h"
#include "hwreg_ipmux.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
#include "drvCMAPool.h"
#include "msos/linux/mdrv_cma_pool_st.h"
#endif
#endif

#ifdef  MSOS_TYPE_LINUX
#include <assert.h>
#else
#define assert(b)   \
    do {    \
        if (!(b)){  \
            printf("assert at [%s][%d] \n", __FUNCTION__, __LINE__);   \
            *((int*)0) = 0; \
        }   \
    } while(0)
#endif

#ifdef ANDROID
#include <cutils/log.h>
#ifndef LOGE
#define printf(format, args...)          ALOGE(format, ##args);
#else
#define printf(format, args...)          LOGE(format, ##args);
#endif
#endif

#ifdef MS_DEBUG
#define SC_DBG(x)   x
#define FPLL_DBG(x) x
#else
#define SC_DBG(x)   //x
#define FPLL_DBG(x) //x
#endif

//#define new_chakra

//==============================================================================
//==============================================================================
MS_VIRT _XC_RIU_BASE;
MS_VIRT _PM_RIU_BASE;

MS_VIRT _HDCP_RIU_BASE;
MS_VIRT _GOP_RIU_BASE;
MS_U32 _HAL_XC_DEVICE_OFFSET[MAX_XC_DEVICE_NUM] = {MAX_XC_DEVICE0_OFFSET, MAX_XC_DEVICE1_OFFSET};

extern PQ_Function_Info    s_PQ_Function_Info;
//extern MS_BOOL             s_bKeepPixelPointerAppear;
//static SC_MIUMASK_t Miu0Mask, Miu1Mask;
//static SC_MIUMASK_t Miu0MaskOld, Miu1MaskOld;

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
MS_U32 u32CMAPatchTimer_Main ;
#endif
#endif

#define IS_V_POST_SCALING(x)    ((x & 0x1000000L) == 0x1000000L)
#define IS_H_POST_SCALING(x)    ((x & 0x1000000L) == 0x1000000L)

#define HAL_SC1_HVSP_SRAM_PHASE 64
static MS_U8 au8Sc1HvspFilterFc100[HAL_SC1_HVSP_SRAM_PHASE][5] =   {{0x00, 0x00, 0x02, 0x00, 0x00},
                                                                    {0x00, 0x00, 0x12, 0x80, 0x80},
                                                                    {0x02, 0x01, 0x32, 0x80, 0x80},
                                                                    {0x03, 0x01, 0x42, 0x80, 0x80},
                                                                    {0x04, 0xff, 0x61, 0x80, 0x80},
                                                                    {0x05, 0xff, 0x71, 0x80, 0x80},
                                                                    {0x07, 0xff, 0x91, 0x80, 0x80},
                                                                    {0x08, 0xff, 0xa1, 0x80, 0x80},
                                                                    {0x09, 0xff, 0xc1, 0x00, 0x81},
                                                                    {0x0a, 0xfd, 0xe1, 0x00, 0x81},
                                                                    {0x0b, 0xfd, 0xf1, 0x00, 0x81},
                                                                    {0x0c, 0xfb, 0x11, 0x01, 0x81},
                                                                    {0x0d, 0xf9, 0x31, 0x01, 0x81},
                                                                    {0x0e, 0xf7, 0x51, 0x01, 0x81},
                                                                    {0x0e, 0xf3, 0x71, 0x01, 0x81},
                                                                    {0x0f, 0xf3, 0x91, 0x81, 0x81},
                                                                    {0x10, 0xf1, 0xb1, 0x81, 0x81},
                                                                    {0x11, 0xef, 0xd1, 0x81, 0x81},
                                                                    {0x11, 0xeb, 0xf1, 0x81, 0x81},
                                                                    {0x12, 0xe9, 0x11, 0x82, 0x81},
                                                                    {0x13, 0xe9, 0x31, 0x02, 0x82},
                                                                    {0x13, 0xe5, 0x51, 0x02, 0x82},
                                                                    {0x14, 0xe3, 0x71, 0x02, 0x82},
                                                                    {0x14, 0xdf, 0x91, 0x02, 0x82},
                                                                    {0x15, 0xdf, 0xb1, 0x82, 0x82},
                                                                    {0x15, 0xd9, 0xe1, 0x82, 0x82},
                                                                    {0x16, 0xd7, 0x01, 0x83, 0x82},
                                                                    {0x16, 0xd5, 0x21, 0x03, 0x83},
                                                                    {0x16, 0xd1, 0x41, 0x03, 0x83},
                                                                    {0x17, 0xcd, 0x71, 0x03, 0x83},
                                                                    {0x17, 0xc9, 0x91, 0x03, 0x83},
                                                                    {0x17, 0xc5, 0xc1, 0x83, 0x83},
                                                                    {0x18, 0xc3, 0xe1, 0x83, 0x83},
                                                                    {0x18, 0xbf, 0x01, 0x84, 0x83},
                                                                    {0x18, 0xbb, 0x31, 0x04, 0x84},
                                                                    {0x18, 0xb7, 0x51, 0x04, 0x84},
                                                                    {0x18, 0xb3, 0x81, 0x84, 0x84},
                                                                    {0x18, 0xad, 0xb1, 0x84, 0x84},
                                                                    {0x19, 0xab, 0xd1, 0x84, 0x84},
                                                                    {0x19, 0xa7, 0x01, 0x05, 0x85},
                                                                    {0x19, 0xa3, 0x21, 0x05, 0x85},
                                                                    {0x19, 0x9f, 0x51, 0x85, 0x85},
                                                                    {0x19, 0x99, 0x81, 0x85, 0x85},
                                                                    {0x19, 0x95, 0xa1, 0x85, 0x85},
                                                                    {0x18, 0x8f, 0xd1, 0x05, 0x86},
                                                                    {0x18, 0x89, 0x01, 0x06, 0x86},
                                                                    {0x18, 0x87, 0x21, 0x86, 0x86},
                                                                    {0x18, 0x81, 0x51, 0x86, 0x86},
                                                                    {0x18, 0x7b, 0x81, 0x86, 0x86},
                                                                    {0x18, 0x79, 0xa1, 0x06, 0x87},
                                                                    {0x18, 0x73, 0xd1, 0x06, 0x87},
                                                                    {0x18, 0x6f, 0x01, 0x87, 0x87},
                                                                    {0x17, 0x67, 0x31, 0x87, 0x87},
                                                                    {0x17, 0x63, 0x51, 0x87, 0x87},
                                                                    {0x17, 0x5f, 0x81, 0x07, 0x88},
                                                                    {0x17, 0x59, 0xb1, 0x07, 0x88},
                                                                    {0x16, 0x53, 0xe1, 0x87, 0x88},
                                                                    {0x16, 0x4f, 0x01, 0x88, 0x88},
                                                                    {0x16, 0x49, 0x31, 0x88, 0x88},
                                                                    {0x15, 0x43, 0x61, 0x08, 0x89},
                                                                    {0x15, 0x3d, 0x91, 0x08, 0x89},
                                                                    {0x15, 0x39, 0xc1, 0x88, 0x89},
                                                                    {0x14, 0x33, 0xe1, 0x88, 0x89},
                                                                    {0x14, 0x2d, 0x11, 0x89, 0x89}};

static MS_U8 au8Sc1HvspFilterC121[HAL_SC1_HVSP_SRAM_PHASE][5] =   {{0x54, 0x58, 0x40, 0x05, 0x16},
                                                                    {0x54, 0x58, 0x40, 0x05, 0x16},
                                                                    {0x54, 0x58, 0x40, 0x05, 0x16},
                                                                    {0x54, 0x58, 0x40, 0x05, 0x16},
                                                                    {0x54, 0x58, 0x40, 0x05, 0x16},
                                                                    {0x54, 0x58, 0x40, 0x05, 0x16},
                                                                    {0x52, 0x5c, 0x20, 0x05, 0x17},
                                                                    {0x52, 0x5c, 0x20, 0x05, 0x17},
                                                                    {0x52, 0x5c, 0x20, 0x05, 0x17},
                                                                    {0x52, 0x5c, 0x20, 0x05, 0x17},
                                                                    {0x52, 0x5c, 0x20, 0x05, 0x17},
                                                                    {0x52, 0x5c, 0x20, 0x05, 0x17},
                                                                    {0x50, 0x60, 0x00, 0x05, 0x18},
                                                                    {0x50, 0x60, 0x00, 0x05, 0x18},
                                                                    {0x50, 0x60, 0x00, 0x05, 0x18},
                                                                    {0x50, 0x60, 0x00, 0x05, 0x18},
                                                                    {0x50, 0x60, 0x00, 0x05, 0x18},
                                                                    {0x50, 0x60, 0x00, 0x05, 0x18},
                                                                    {0x4e, 0x64, 0xe0, 0x04, 0x19},
                                                                    {0x4e, 0x64, 0xe0, 0x04, 0x19},
                                                                    {0x4e, 0x64, 0xe0, 0x04, 0x19},
                                                                    {0x4e, 0x64, 0xe0, 0x04, 0x19},
                                                                    {0x4e, 0x64, 0xe0, 0x04, 0x19},
                                                                    {0x4e, 0x64, 0xe0, 0x04, 0x19},
                                                                    {0x4c, 0x68, 0xc0, 0x04, 0x1a},
                                                                    {0x4c, 0x68, 0xc0, 0x04, 0x1a},
                                                                    {0x4c, 0x68, 0xc0, 0x04, 0x1a},
                                                                    {0x4c, 0x68, 0xc0, 0x04, 0x1a},
                                                                    {0x4c, 0x68, 0xc0, 0x04, 0x1a},
                                                                    {0x4c, 0x68, 0xc0, 0x04, 0x1a},
                                                                    {0x4a, 0x6c, 0xa0, 0x04, 0x1b},
                                                                    {0x4a, 0x6c, 0xa0, 0x04, 0x1b},
                                                                    {0x4a, 0x6c, 0xa0, 0x04, 0x1b},
                                                                    {0x4a, 0x6c, 0xa0, 0x04, 0x1b},
                                                                    {0x4a, 0x6c, 0xa0, 0x04, 0x1b},
                                                                    {0x4a, 0x6c, 0xa0, 0x04, 0x1b},
                                                                    {0x48, 0x70, 0x80, 0x04, 0x1c},
                                                                    {0x48, 0x70, 0x80, 0x04, 0x1c},
                                                                    {0x48, 0x70, 0x80, 0x04, 0x1c},
                                                                    {0x48, 0x70, 0x80, 0x04, 0x1c},
                                                                    {0x48, 0x70, 0x80, 0x04, 0x1c},
                                                                    {0x48, 0x70, 0x80, 0x04, 0x1c},
                                                                    {0x46, 0x74, 0x60, 0x04, 0x1d},
                                                                    {0x46, 0x74, 0x60, 0x04, 0x1d},
                                                                    {0x46, 0x74, 0x60, 0x04, 0x1d},
                                                                    {0x46, 0x74, 0x60, 0x04, 0x1d},
                                                                    {0x46, 0x74, 0x60, 0x04, 0x1d},
                                                                    {0x46, 0x74, 0x60, 0x04, 0x1d},
                                                                    {0x44, 0x78, 0x40, 0x04, 0x1e},
                                                                    {0x44, 0x78, 0x40, 0x04, 0x1e},
                                                                    {0x44, 0x78, 0x40, 0x04, 0x1e},
                                                                    {0x44, 0x78, 0x40, 0x04, 0x1e},
                                                                    {0x44, 0x78, 0x40, 0x04, 0x1e},
                                                                    {0x44, 0x78, 0x40, 0x04, 0x1e},
                                                                    {0x42, 0x7c, 0x20, 0x04, 0x1f},
                                                                    {0x42, 0x7c, 0x20, 0x04, 0x1f},
                                                                    {0x42, 0x7c, 0x20, 0x04, 0x1f},
                                                                    {0x42, 0x7c, 0x20, 0x04, 0x1f},
                                                                    {0x42, 0x7c, 0x20, 0x04, 0x1f},
                                                                    {0x42, 0x7c, 0x20, 0x04, 0x1f},
                                                                    {0x40, 0x80, 0x00, 0x04, 0x20},
                                                                    {0x40, 0x80, 0x00, 0x04, 0x20},
                                                                    {0x40, 0x80, 0x00, 0x04, 0x20},
                                                                    {0x40, 0x80, 0x00, 0x04, 0x20}};


//Default VMark Symbol
static MS_U8 u8VMarkSymbols[1920] =
        {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0xa8, 0xaa, 0x00, 0x00, 0x02, 0x58, 0x96, 0x00, 0x00, 0x02, 0x58, 0x96, 0x00, 0x00, 0x02,
        0x58, 0x96, 0x00, 0x02, 0xaa, 0x5a, 0x96, 0x80, 0x02, 0x55, 0x55, 0x55, 0x80, 0x02, 0xa9, 0x6a,
        0x5a, 0x80, 0x00, 0x09, 0xa2, 0x68, 0x00, 0x00, 0x09, 0x82, 0x60, 0x00, 0x00, 0x29, 0x8a, 0x60,
        0x00, 0x02, 0xa5, 0xa9, 0x6a, 0x80, 0x02, 0x55, 0x55, 0x55, 0x80, 0x02, 0x96, 0xa5, 0xaa, 0x80,
        0x00, 0x96, 0x25, 0x80, 0x00, 0x00, 0x96, 0x25, 0x80, 0x00, 0x00, 0x96, 0x25, 0x80, 0x00, 0x00,
        0xaa, 0x2a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x02, 0x96, 0x80,
        0x00, 0x00, 0x0a, 0x55, 0x80, 0x00, 0x00, 0x09, 0x55, 0xa0, 0x00, 0x00, 0x09, 0xa9, 0x68, 0x00,
        0x00, 0x29, 0x8a, 0x58, 0x00, 0x00, 0xa5, 0x82, 0x9a, 0x00, 0x00, 0x96, 0x80, 0x96, 0x00, 0x00,
        0xaa, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x95, 0x55, 0x56, 0x00, 0x00, 0xaa, 0xaa,
        0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa,
        0x00, 0x00, 0x95, 0x55, 0x56, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00,
        0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00,
        0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00,
        0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00,
        0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98,
        0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00,
        0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x2a, 0x80, 0x00, 0x00, 0x00, 0x25, 0x80, 0x00, 0x00, 0x00, 0x25, 0x80, 0x00, 0x00, 0x00,
        0x26, 0x80, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0xa6, 0x00, 0x00, 0x00, 0x00, 0x96,
        0x00, 0x00, 0x00, 0x00, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x02, 0x98, 0x00,
        0x00, 0x00, 0x02, 0x58, 0x00, 0x00, 0x00, 0x02, 0x68, 0x00, 0x00, 0x00, 0x02, 0x60, 0x00, 0x00,
        0x00, 0x0a, 0x60, 0x00, 0x00, 0x00, 0x09, 0x60, 0x00, 0x00, 0x00, 0x09, 0x60, 0x00, 0x00, 0x00,
        0x0a, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xa0, 0x00, 0x00, 0x00, 0x09, 0x60, 0x00,
        0x00, 0x00, 0x09, 0x60, 0x00, 0x00, 0x00, 0x0a, 0x60, 0x00, 0x00, 0x00, 0x02, 0x60, 0x00, 0x00,
        0x00, 0x02, 0x68, 0x00, 0x00, 0x00, 0x02, 0x58, 0x00, 0x00, 0x00, 0x02, 0x98, 0x00, 0x00, 0x00,
        0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00, 0x00,
        0xa6, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x80, 0x00, 0x00, 0x00, 0x25,
        0x80, 0x00, 0x00, 0x00, 0x25, 0x80, 0x00, 0x00, 0x00, 0x2a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0xaa, 0x00, 0x00, 0x00, 0x02, 0x56, 0x00, 0x00, 0x00, 0x0a, 0x56, 0x80, 0x00, 0x00, 0x09,
        0xa9, 0x80, 0x00, 0x00, 0x29, 0x89, 0xa0, 0x00, 0x00, 0x25, 0x89, 0x60, 0x00, 0x00, 0x26, 0x8a,
        0x60, 0x00, 0x00, 0xa6, 0x02, 0x68, 0x00, 0x00, 0x9a, 0x02, 0x98, 0x00, 0x02, 0x98, 0x00, 0x9a,
        0x00, 0x02, 0x5a, 0xaa, 0x96, 0x00, 0x02, 0x55, 0x55, 0x56, 0x00, 0x0a, 0x6a, 0xaa, 0xa6, 0x80,
        0x09, 0x60, 0x00, 0x25, 0x80, 0x09, 0xa0, 0x00, 0x29, 0x80, 0x29, 0x80, 0x00, 0x09, 0xa0, 0x2a,
        0x80, 0x00, 0x0a, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xaa, 0xaa, 0xa0, 0x00, 0x02, 0x55, 0x55, 0x68,
        0x00, 0x02, 0x6a, 0xaa, 0x5a, 0x00, 0x02, 0x60, 0x02, 0xa6, 0x00, 0x02, 0x60, 0x00, 0x26, 0x00,
        0x02, 0x60, 0x00, 0x26, 0x00, 0x02, 0x60, 0x02, 0xa6, 0x00, 0x02, 0x6a, 0xaa, 0x56, 0x00, 0x02,
        0x55, 0x55, 0x56, 0x00, 0x02, 0x6a, 0xaa, 0x96, 0x80, 0x02, 0x60, 0x00, 0xa9, 0x80, 0x02, 0x60,
        0x00, 0x09, 0x80, 0x02, 0x60, 0x00, 0x09, 0x80, 0x02, 0x60, 0x00, 0xa9, 0x80, 0x02, 0x6a, 0xaa,
        0x96, 0x80, 0x02, 0x55, 0x55, 0x5a, 0x00, 0x02, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x0a, 0xaa, 0xa0, 0x00, 0x00, 0x29, 0x55, 0x6a, 0x00, 0x00, 0xa5, 0xaa, 0x56, 0x80, 0x02, 0x9a,
        0x82, 0xa5, 0x80, 0x0a, 0x68, 0x00, 0x2a, 0x80, 0x09, 0x60, 0x00, 0x00, 0x00, 0x09, 0xa0, 0x00,
        0x00, 0x00, 0x09, 0x80, 0x00, 0x00, 0x00, 0x09, 0x80, 0x00, 0x00, 0x00, 0x09, 0x80, 0x00, 0x00,
        0x00, 0x09, 0xa0, 0x00, 0x00, 0x00, 0x09, 0x60, 0x00, 0x00, 0x00, 0x0a, 0x68, 0x00, 0x2a, 0x80,
        0x02, 0x5a, 0x82, 0xa5, 0x80, 0x02, 0x95, 0xaa, 0x56, 0x80, 0x00, 0xa9, 0x55, 0x6a, 0x00, 0x00,
        0x0a, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xaa, 0xaa, 0xa0, 0x00, 0x02, 0x55, 0x55, 0x68,
        0x00, 0x02, 0x6a, 0xa9, 0x5a, 0x00, 0x02, 0x60, 0x0a, 0xa6, 0x00, 0x02, 0x60, 0x00, 0x26, 0x80,
        0x02, 0x60, 0x00, 0x25, 0x80, 0x02, 0x60, 0x00, 0x29, 0x80, 0x02, 0x60, 0x00, 0x09, 0x80, 0x02,
        0x60, 0x00, 0x09, 0x80, 0x02, 0x60, 0x00, 0x09, 0x80, 0x02, 0x60, 0x00, 0x29, 0x80, 0x02, 0x60,
        0x00, 0x25, 0x80, 0x02, 0x60, 0x00, 0x26, 0x80, 0x02, 0x60, 0x02, 0xa6, 0x00, 0x02, 0x6a, 0xaa,
        0x5a, 0x00, 0x02, 0x55, 0x55, 0x68, 0x00, 0x02, 0xaa, 0xaa, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0xa6, 0x00, 0x00, 0x00, 0x02, 0x96, 0x00, 0x00, 0x00, 0x2a,
        0x56, 0x00, 0x00, 0x00, 0x26, 0xa6, 0x00, 0x00, 0x00, 0x2a, 0x26, 0x00, 0x00, 0x00, 0x28, 0x26,
        0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00,
        0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00,
        0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00,
        0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0x80, 0x00, 0x00, 0x29, 0x55, 0xa0,
        0x00, 0x00, 0xa5, 0xa9, 0x68, 0x00, 0x00, 0x96, 0x8a, 0x98, 0x00, 0x00, 0x9a, 0x00, 0x98, 0x00,
        0x00, 0xa8, 0x00, 0x98, 0x00, 0x00, 0x00, 0x02, 0x98, 0x00, 0x00, 0x00, 0x02, 0x68, 0x00, 0x00,
        0x00, 0x0a, 0x60, 0x00, 0x00, 0x00, 0x29, 0xa0, 0x00, 0x00, 0x02, 0xaa, 0x80, 0x00, 0x00, 0x0a,
        0x68, 0x00, 0x00, 0x00, 0x09, 0xa0, 0x00, 0x00, 0x00, 0x29, 0x80, 0x00, 0x00, 0x00, 0xa5, 0xaa,
        0xa8, 0x00, 0x00, 0x95, 0x55, 0x58, 0x00, 0x00, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0xaa, 0x80, 0x00, 0x00, 0x0a, 0x55, 0xa0, 0x00, 0x00, 0x29, 0xaa, 0x68, 0x00, 0x00, 0xa6,
        0x82, 0x58, 0x00, 0x00, 0x9a, 0x02, 0xa8, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x9a, 0xaa,
        0x80, 0x00, 0x00, 0x95, 0x55, 0xa0, 0x00, 0x00, 0x95, 0xa9, 0x68, 0x00, 0x00, 0x96, 0x8a, 0x58,
        0x00, 0x00, 0x9a, 0x02, 0x98, 0x00, 0x00, 0x98, 0x00, 0x98, 0x00, 0x00, 0x9a, 0x02, 0x98, 0x00,
        0x00, 0xa6, 0x8a, 0x58, 0x00, 0x00, 0x29, 0xa9, 0x68, 0x00, 0x00, 0x0a, 0x55, 0xa0, 0x00, 0x00,
        0x02, 0xaa, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xa8, 0x00, 0x00, 0x95, 0x55, 0x58,
        0x00, 0x00, 0xaa, 0xa9, 0x58, 0x00, 0x00, 0x00, 0x09, 0x68, 0x00, 0x00, 0x00, 0x09, 0xa0, 0x00,
        0x00, 0x00, 0x29, 0x80, 0x00, 0x00, 0x00, 0xa6, 0x80, 0x00, 0x00, 0x00, 0x96, 0x00, 0x00, 0x00,
        0x02, 0x9a, 0x00, 0x00, 0x00, 0x02, 0x58, 0x00, 0x00, 0x00, 0x02, 0x68, 0x00, 0x00, 0x00, 0x0a,
        0x60, 0x00, 0x00, 0x00, 0x09, 0x60, 0x00, 0x00, 0x00, 0x09, 0x60, 0x00, 0x00, 0x00, 0x09, 0xa0,
        0x00, 0x00, 0x00, 0x09, 0x80, 0x00, 0x00, 0x00, 0x0a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x0a, 0xaa, 0x00, 0x00, 0x00, 0x29, 0x56, 0x80, 0x00, 0x00, 0xa5, 0xa9, 0xa0, 0x00, 0x00, 0x96,
        0x8a, 0x68, 0x00, 0x00, 0x9a, 0x02, 0x98, 0x00, 0x00, 0x98, 0x00, 0x98, 0x00, 0x00, 0x9a, 0x02,
        0x98, 0x00, 0x00, 0x96, 0x8a, 0x58, 0x00, 0x00, 0xa5, 0xa9, 0x58, 0x00, 0x00, 0x29, 0x55, 0x58,
        0x00, 0x00, 0x0a, 0xaa, 0x98, 0x00, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0xaa, 0x02, 0x98, 0x00,
        0x00, 0x96, 0x0a, 0x68, 0x00, 0x00, 0xa6, 0xa9, 0xa0, 0x00, 0x00, 0x29, 0x56, 0x80, 0x00, 0x00,
        0x0a, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0xaa, 0x80, 0x00, 0x00, 0x29, 0x55, 0xa0,
        0x00, 0x00, 0x25, 0xa9, 0x60, 0x00, 0x00, 0xa6, 0x8a, 0x68, 0x00, 0x00, 0x96, 0x02, 0x58, 0x00,
        0x00, 0x9a, 0x02, 0x98, 0x00, 0x00, 0x98, 0x00, 0x98, 0x00, 0x00, 0x98, 0x00, 0x98, 0x00, 0x00,
        0x98, 0x00, 0x98, 0x00, 0x00, 0x98, 0x00, 0x98, 0x00, 0x00, 0x98, 0x00, 0x98, 0x00, 0x00, 0x9a,
        0x02, 0x98, 0x00, 0x00, 0x96, 0x02, 0x58, 0x00, 0x00, 0xa6, 0x8a, 0x68, 0x00, 0x00, 0x25, 0xa9,
        0x60, 0x00, 0x00, 0x29, 0x55, 0xa0, 0x00, 0x00, 0x0a, 0xaa, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

#ifdef UFO_XC_AUTO_DOWNLOAD
#define DEBUG_HDR 0
#define  AUTO_DOWNLOAD_HDR_TMO_SRAM_MAX_ADDR     512
#define  AUTO_DOWNLOAD_HDR_3DLUT_SRAM_MAX_ADDR     4944

// HDR client of auto download has these following formats, include format0 and format1.
//
// Format0:
//127      125          116 115         112                             60        57                                   0
// | [x[2]] | [addr(10)] | 0 | [sel0(3)] |           [data1]             | sel1(3) |              [data0]              |
//                               sel0 = 1                            x(52) sel1 = 1              wd0(19),wd1(19),wd2(19)
//                               sel0 = 2     x(4),wd0(16),wd1(16),wd2(16) sel1 = 2         x(9),wd0(16),wd1(16),wd2(16)
//                               sel0 = 3                    x(40),wd0(12) sel1 = 3                        x(45),wd0(12)
//                               sel0 = 4   idx(4),wd0(16),wd1(16),wd2(16) sel1 = 4  x(5),idx(4),wd0(16),wd1(16),wd2(16)
// sel:   0: disable
//        1: De-gamma
//        2: Gamma
//        3: TMO
//        4: 3D-lut

// Format1:
//127      125          116 115          112                             60        57                                   0
// | [x[2]] | [addr(10)] | 1 | [ctrl0(3)] |           [data1]             | sel1(3) |              [data0]              |
//                                                                    x(52) sel1 = 0
//                                           idx(4),wd0(16),wd1(16),wd2(16) sel1 = 4      x(10),degam(19),gam(16),tmo(16)
// ctrl:                 bit: 114  113 112
//                          |degam|gam|tmo|
// sel:   0: disable
//        4: 3D-lut
//
// HW designer recommand using format1.
// These following definitions write datas of tmo/gamma/de-gamma/3d-lut separately by format1,
// please refer to definition of WRITE_TMO_DATA_FORMAT_1, WRITE_GAMMA_DATA_FORMAT_1, WRITE_DEGAMMA_DATA_FORMAT_1 and WRITE_3DLUT_DATA_FORMAT_1.
#define WRITE_TMO_DATA_FORMAT_1(baseaddr, index, value) \
    *(baseaddr) = (((*(baseaddr)) & 0xC0) | (((index) >> 4) & 0x3F));\
    *(baseaddr + 1) = ((((index) << 4) & 0xF0) | ((*(baseaddr + 1)) & 0x0F) | 0x09);\
    *(baseaddr + 14) = ((*(baseaddr + 14) & 0xF0) | (((value) >> 8) & 0x0F));\
    *(baseaddr + 15) = ((value) & 0xFF);

#define WRITE_GAMMA_DATA_FORMAT_1(baseaddr, index, value) \
    *(baseaddr) = (((*(baseaddr)) & 0xC0) | (((index) >> 4) & 0x3F));\
    *(baseaddr + 1) = ((((index) << 4) & 0xF0) | ((*(baseaddr + 1)) & 0x0F) | 0x0A);\
    *(baseaddr + 14) = ((*(baseaddr + 14) & 0x0F) | (((value) << 4) & 0xF0));\
    *(baseaddr + 13) = (((value) >> 4) & 0xFF);\
    *(baseaddr + 12) = ((*(baseaddr + 12) & 0xF0) | (((value) >> 12) & 0x0F));

#define WRITE_DEGAMMA_DATA_FORMAT_1(baseaddr, index, value) \
    *(baseaddr) = (((*(baseaddr)) & 0xC0) | (((index) >> 4) & 0x3F));\
    *(baseaddr + 1) = ((((index) << 4) & 0xF0) | ((*(baseaddr + 1)) & 0x0F) | 0x0C);\
    *(baseaddr + 12) = ((*(baseaddr + 12) & 0x0F) | (((value) << 4) & 0xF0));\
    *(baseaddr + 11) = (((value) >> 4) & 0xFF);\
    *(baseaddr + 10) = ((*(baseaddr + 10) & 0x80) | (((value) >> 12) & 0x7F));

#define WRITE_3DLUT_DATA_FORMAT_1(baseaddr, index, subindex, rval, gval, bval) \
    *(baseaddr) = (((*(baseaddr)) & 0xC0) | (((index) >> 4) & 0x3F));\
    *(baseaddr + 1) = ((((index) << 4) & 0xF0) | ((*(baseaddr + 1)) & 0x0F) | 0x08);\
    *(baseaddr + 8) = ((*(baseaddr + 8) & 0xF1) | 0x08);\
    *(baseaddr + 8) = ((*(baseaddr + 8) & 0x0F) | (((bval) << 4) & 0xF0));\
    *(baseaddr + 7) = (((bval) >> 4) & 0xFF);\
    *(baseaddr + 6) = ((*(baseaddr + 6) & 0xF0) | (((bval) >> 12) & 0x0F));\
    *(baseaddr + 6) = ((*(baseaddr + 6) & 0x0F) | (((gval) << 4) & 0xF0));\
    *(baseaddr + 5) = (((gval) >> 4) & 0xFF);\
    *(baseaddr + 4) = ((*(baseaddr + 4) & 0xF0) | (((gval) >> 12) & 0x0F));\
    *(baseaddr + 4) = ((*(baseaddr + 4) & 0x0F) | (((rval) << 4) & 0xF0));\
    *(baseaddr + 3) = (((rval) >> 4) & 0xFF);\
    *(baseaddr + 2) = ((*(baseaddr + 2) & 0xF0) | (((rval) >> 12) & 0x0F));\
    *(baseaddr + 2) = ((*(baseaddr + 2) & 0x0F) | (((subindex) << 4) & 0xF0));

// The following definition clear bits of format1's ctrl & sel.
#define CLEAR_HDR_DATA_FORMAT_1(baseaddr) \
    *(baseaddr + 1) = ((*(baseaddr + 1)) & 0xF8);\
    *(baseaddr + 8) = (*(baseaddr + 8) & 0xF1);

static MS_U32 _au32_3dlut_entry_num[8] = {736, 656, 656, 576, 656, 576, 576, 512};
#endif
// Put this function here because hwreg_utility2 only for hal.
void Hal_SC_init_riu_base(MS_VIRT u32riu_base, MS_VIRT u32PMriu_base)
{
    _XC_RIU_BASE = u32PMriu_base;
    _PM_RIU_BASE = u32PMriu_base;

    _HDCP_RIU_BASE = u32PMriu_base;
    _GOP_RIU_BASE = u32PMriu_base;

}

//=========================================================//
// Function : Hal_SC_setfield
// Description:
//=========================================================//
void Hal_SC_setfield( void *pInstance, MS_U16 reg_1D, MS_U16 reg_21, MS_U16 reg_23, SCALER_WIN eWindow )
{
    //printf("reg_1D=%x, reg_21=%x, reg_23=%x\n", reg_1D, reg_21, reg_23);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    INPUT_SOURCE_TYPE_t enInputSourceType = pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow];
    if((psXCInstPri->u32DeviceID == 0) && (IsSrcTypeDTV(enInputSourceType) || IsSrcTypeStorage(enInputSourceType)))
    {
        //HW:Source is MVOP must disable auto interlace detect for disable field signal to interfere XC memory R/W
        //user interlace case is decide by driver layer, here only enable user define
        reg_21 |= 0x1;
    }

    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_1D_L, (reg_1D<<8), 0xEF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, reg_21, 0x3FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_23_L, (reg_23<<8), HBMASK);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_1D_L, (reg_1D<<8), 0xEF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, reg_21, 0x3FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_23_L, (reg_23<<8), HBMASK);
    }
//    printf("%x ",SC_R2BYTEMSK(REG_SC_BK01_1D_L, HBMASK));
//    printf("%x ",SC_R2BYTEMSK(REG_SC_BK01_21_L, 0x3FFF));
//    printf("%x ",SC_R2BYTEMSK(REG_SC_BK01_23_L, HBMASK));

//    printf("%x ",SC_R4BYTE(REG_SC_BK01_21_L));
//    printf("%x ",SC_R4BYTE(REG_SC_BK01_23_L));
}

//=========================================================//
// Function : Hal_SC_de_only_en
// Description: DE Only. HSYNC and VSYNC are ignored.
//#0: Disable.
//#1: Enable.
//=========================================================//
void Hal_SC_de_only_en(void *pInstance, MS_BOOL benable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, (benable ? BIT(6):0), BIT(6));
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, (benable ? BIT(6):0), BIT(6));
}

//=========================================================//
// Function : Hal_SC_de_direct_en
// Description: Digital Input Horizontal Sample Range
//#0: Use DE as sample range, only V position can be adjusted.
//#1: Use SPRHST and SPRHDC as sample range, both H and V position can be adjusted.
//=========================================================//
void Hal_SC_de_direct_en(void *pInstance, MS_BOOL benable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, (benable ? BIT(7):0), BIT(7));
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, (benable ? BIT(7):0), BIT(7));

}

void Hal_SC_set_ficlk(void *pInstance, MS_BOOL bPreDown, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        if( eWindow == MAIN_WINDOW )
        {
            if (bPreDown)
            {
                W2BYTEMSK(REG_CKG_FICLK_F2, CKG_FICLK_F2_IDCLK2, CKG_FICLK_F2_MASK); // clk_idclk2
            }
            else
            {
                W2BYTEMSK(REG_CKG_FICLK_F2, CKG_FICLK_F2_FLK, CKG_FICLK_F2_MASK); // clk_fclk
            }
        }
        else
        {
            if (bPreDown)
            {
                W2BYTEMSK(REG_CKG_FICLK_F1, CKG_FICLK_F1_IDCLK1, CKG_FICLK_F1_MASK); // clk_idclk1
            }
            else
            {
                W2BYTEMSK(REG_CKG_FICLK_F1, CKG_FICLK_F1_FCLK, CKG_FICLK_F1_MASK); // clk_fclk
            }
        }
    }
    else // SC1
    {
        // Do nothing
    }
}

void Hal_SC_set_shiftline( void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L, (u8Val & 0x07)<<8, (BIT(10)|BIT(9)|BIT(8)));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_2F_L, (u8Val & 0x03)<<13, (BIT(14)|BIT(13)));
    }
}

void Hal_SC_set_422_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, bEnable ? BIT(6) : 0, BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_0A_L, bEnable ? BIT(6) : 0, BIT(6));
    }
}

void Hal_SC_set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (bEnable)
    {
        if ( eWindow == MAIN_WINDOW )
        {
            // Enable pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2A_L, BIT(15), BIT(15));

            // Number of pixels need to be inserted. (U3 has 15 pixels)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2A_L, pixels << 8 , 0x1F00);
        }
        else
        {
            // Enable pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_2A_L, BIT(15), BIT(15));

            // Number of pixels need to be inserted. (U3 has 15 pixels)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_2A_L, pixels << 8 , 0x1F00);
        }

    }
    else
    {
        if ( eWindow == MAIN_WINDOW )
        {
            // Disable pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2A_L, 0x00 , BIT(15));
        }
        else
        {
            // Disable pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_2A_L, 0x00 , BIT(15));
        }
    }
}

void Hal_XC_Set_FreeFRCMD(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, bEnable? BIT(5) : 0, BIT(5));
}

// This function will return 8/10/12/14/16 field mode or 8 frame mode.
// Otherwise it return IMAGE_STORE_2_FRAMES
XC_FRAME_STORE_NUMBER Hal_SC_GetFrameStoreMode(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16FrameCount = 0x00;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (eWindow == MAIN_WINDOW)
    {
        u16FrameCount = 0x1F & SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L);
    }
    else
    {
        u16FrameCount = 0x1F & SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_59_L);
    }

    switch( u16FrameCount) // Get field type.
    {
        default:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_4_FIELDS;
            }
            else
            {
                return IMAGE_STORE_2_FRAMES;
            }
        case 3:
            return IMAGE_STORE_3_FRAMES;
        case 4:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_4_FIELDS;
            }
            else
            {
                return IMAGE_STORE_4_FRAMES;
            }
        case 6:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_6_FIELDS;
            }
            else
            {
                return IMAGE_STORE_6_FRAMES;
            }
        case 8:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_8_FIELDS;
            }
            else
            {
                return IMAGE_STORE_8_FRAMES;
            }
        case 10:
            return IMAGE_STORE_10_FIELDS;
        case 12:
            if (gSrcInfo[MAIN_WINDOW].bInterlace)
            {
                return IMAGE_STORE_12_FIELDS;
            }
            else
            {
                return IMAGE_STORE_12_FRAMES;
            }
        case 14:
            return IMAGE_STORE_14_FIELDS;
        case 16:
            return IMAGE_STORE_16_FIELDS;

    }

    return IMAGE_STORE_UNDEFINE;

}

// Obsolete in M10/J2/A5
void Hal_SC_set_linearmem_mode(MS_BOOL bEnable, SCALER_WIN eWindow)
{
}

MS_BOOL Hal_SC_get_ip2_csc(void *pInstance, SCALER_WIN eWindow)
{
    return TRUE;//always YUV
}

void Hal_SC_set_wr_bank_mapping(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_DBG(printf("Set WR bank mapping SC_BK12_07/47_L[15:13]= 0x%x\n", ((MS_U16)u8val)<<13));
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, ((MS_U16)u8val)<<13, BIT(15)|BIT(14)|BIT(13));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, ((MS_U16)u8val)<<13, BIT(15)|BIT(14)|BIT(13));
    }
}

void Hal_SC_set_wr_bank_mapping_num(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    Hal_SC_set_wr_bank_mapping(pInstance, u8Val, eWindow);
}

MS_U8 Hal_SC_Get_WR_Bank_Mapping(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8val = 0;

    if( eWindow == MAIN_WINDOW )
    {
        u8val = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(15)|BIT(14)|BIT(13)) >> 13;
    }
    else
    {
        u8val = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(15)|BIT(14)|BIT(13)) >> 13;
    }
    SC_DBG(printf("Get WR bank mapping SC_BK12_07/47_L[15:14]= 0x%x\n", ((MS_U16)u8val)<<13));

    return u8val;
}

MS_U8 Hal_SC_Get_WR_Bank_Mapping_Num(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_Get_WR_Bank_Mapping(pInstance, eWindow);
}

void Hal_SC_set_csc( void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_40_L, (bEnable ? BIT(3):0), BIT(3));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_01_L, (bEnable ? BIT(2):0), BIT(2));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_40_L, (bEnable ? BIT(3):0), BIT(3));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_01_L, (bEnable ? BIT(2):0), BIT(2));
    }

    //SC1 PQ CSC color matrix setting
    //RGB to YUV is used PQ CSC type F_RGB2YCC_HD
    if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
    {
        if( eWindow == MAIN_WINDOW )
        {
            //F_RGB2YCC_HD or OFF
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2B_L, (bEnable ? 0xE010:0x0000), 0xE03F);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2D_L, (bEnable ? 0x01C2:0x0400), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2E_L, (bEnable ? 0x1E67:0x0000), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_2F_L, (bEnable ? 0x1FD7:0x0000), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_30_L, (bEnable ? 0x00BB:0x0000), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_31_L, (bEnable ? 0x0275:0x0400), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_32_L, (bEnable ? 0x003F:0x0000), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_33_L, (bEnable ? 0xFF99:0x0000), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_34_L, (bEnable ? 0xFEA6:0x0000), 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_35_L, (bEnable ? 0x01C2:0x0400), 0x1FFF);
        }
    }
}

void Hal_SC_set_delayline( void *pInstance, MS_U8 u8DelayLines, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //skip bit7, which is other usage
    MS_U16 u16DelayLines = ((MS_U16)u8DelayLines) & 0xFF;
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_1C_L, (u16DelayLines<<4), (BIT(9)|BIT(8)|BIT(7)|BIT(6)|BIT(5)|BIT(4)) );
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_1C_L, (u16DelayLines<<4), (BIT(9)|BIT(8)|BIT(7)|BIT(6)|BIT(5)|BIT(4)));
    }
}

void Hal_SC_set_write_limit(void *pInstance, MS_PHY u32WritelimitAddrBase, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_PHY u32MinLimitAddress = 0;
    MS_PHY u32MaxLimitAddress = 0;

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"eWindow=%x, MHal_XC_GetDNRBufAddress=%tx, MHal_XC_GetDNRBufSize=%tx\n", eWindow, (ptrdiff_t)(MHal_XC_GetDNRBufAddress(pInstance, eWindow)/BYTE_PER_WORD ),(ptrdiff_t) (MHal_XC_GetDNRBufSize(pInstance, eWindow))/BYTE_PER_WORD);
    u32MinLimitAddress = MHal_XC_GetDNRBufAddress(pInstance, eWindow)/BYTE_PER_WORD ;
    if((MHal_XC_GetDNRBufAddress(pInstance, eWindow) + MHal_XC_GetDNRBufSize(pInstance, eWindow))!=0)
    {
        u32MaxLimitAddress = (MHal_XC_GetDNRBufAddress(pInstance, eWindow) + MHal_XC_GetDNRBufSize(pInstance, eWindow))/BYTE_PER_WORD - 1;
    }
    else
    {
        //if (MHal_XC_GetDNRBufAddress(pInstance, eWindow) + MHal_XC_GetDNRBufSize(pInstance, eWindow)) ==0
        //can not set Memory limit
        //(CMA case will (MHal_XC_GetDNRBufAddress(pInstance, eWindow) + MHal_XC_GetDNRBufSize(pInstance, eWindow)) == 0
        u32MaxLimitAddress = u32MinLimitAddress;
        u32WritelimitAddrBase = 0;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"eWindow=%x, u32MinLimitAddress=%tx, u32MaxLimitAddress=%tx\n", eWindow, (ptrdiff_t)u32MinLimitAddress, (ptrdiff_t)u32MaxLimitAddress);

    if(eWindow == MAIN_WINDOW)
    {
        /// need to disable the r/w request, when change the write limit base
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(0)|BIT(1), BIT(0)|BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_38_L, ((u32WritelimitAddrBase&BIT(31))?BIT(0):0),BIT(0)); //F2 memory address limit enable
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_40_L, u32MinLimitAddress); //F2 memory min address
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_42_L, u32MaxLimitAddress); //F2 memory max address
        /// need to enable the r/w request
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0x00, BIT(0)|BIT(1));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(0)|BIT(1), BIT(0)|BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_38_L, ((u32WritelimitAddrBase&BIT(31))?BIT(1):0),BIT(1)); //F1 memory address limit enable
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_44_L, u32MinLimitAddress); //F1 memory min address
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_46_L, u32MaxLimitAddress); //F1 memory max address
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0x00, BIT(0)|BIT(1));
    }

}

#if (ENABLE_OPM_WRITE_SUPPORTED)
void Hal_SC_set_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow)
{
    UNUSED(bEnable);
    UNUSED(bFlag);
    UNUSED(u32OPWlimitAddr);
    UNUSED(eWindow);
}
#endif

MS_BOOL Hal_SC_is_extra_req_en(void *pInstance,
    MS_U16 *pu16MainHStart, MS_U16 *pu16MainHEnd,
    MS_U16 *pu16SubHStart,  MS_U16 *pu16SubHEnd)
{
    UNUSED(pu16MainHStart);
    UNUSED(pu16MainHEnd);
    UNUSED(pu16SubHStart);
    UNUSED(pu16SubHEnd);
    return FALSE;
}

// Temp function for SC1 to load HSD advanced mode filter
static void _Hal_SC1_LoadHSDAdvModeFilter(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_50_L, BIT(0), BIT(0)); // Set 8 phase

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_16_L, 0x00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_17_L, 0x00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_18_L, 0x80);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_19_L, 0x02);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_1A_L, 0xF2);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_1B_L, 0x71);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_1C_L, 0x20);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_1D_L, 0xFA);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_1E_L, 0x01);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_1F_L, 0x02);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_20_L, 0xF2);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_21_L, 0x4C);

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_51_L, 0x01);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_52_L, 0xF7);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_53_L, 0x7D);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_54_L, 0x0E);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_55_L, 0xFD);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_56_L, 0x00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_57_L, 0x02);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_58_L, 0xF1);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_59_L, 0x60);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_5A_L, 0x36);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_5B_L, 0xF6);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_5C_L, 0x01);

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_22_L, 0x00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_23_L, 0x80);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_24_L, 0xF6);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_25_L, 0x71);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_26_L, 0x1D);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_27_L, 0xFC);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_28_L, 0xF8);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_29_L, 0x48);
    }
}

static void _Hal_SC1_LoadVSPFilter(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        MS_U8 i = 0;

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, 0x0000, 0x1000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L, 0x0001, 0x0001);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_42_L, 0x0000, 0x0180);

        // SRAM1  (64x40)
        for (i=0; i<HAL_SC1_HVSP_SRAM_PHASE; i++)
        {
            while(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L) & 0x0100);

            // SRAM address
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_42_L, i, 0x007F);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_43_L, (((MS_U16)au8Sc1HvspFilterFc100[i][1])<<8|(MS_U16)au8Sc1HvspFilterFc100[i][0]));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_44_L, (((MS_U16)au8Sc1HvspFilterFc100[i][3])<<8|(MS_U16)au8Sc1HvspFilterFc100[i][2]));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_45_L, ((MS_U16)au8Sc1HvspFilterFc100[i][4]), 0x00FF);

            // Enable SRAM write
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L, 0x0100, 0x0100);
        }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L, 0x00, 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, 0x1000, 0x1000);


        //C_SRAM1  (64x40)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, 0x0000, 0x1000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L, 0x0002, 0x0002); // enable c_sram_rw
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_42_L, 0x0000, 0x01FF);

        for(i=0; i<HAL_SC1_HVSP_SRAM_PHASE; i++)
        {
            while(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L) & 0x0100);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_42_L, i, 0x007F);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_43_L, (((MS_U16)au8Sc1HvspFilterC121[i][1])<<8|(MS_U16)au8Sc1HvspFilterC121[i][0]));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_44_L, (((MS_U16)au8Sc1HvspFilterC121[i][3])<<8|(MS_U16)au8Sc1HvspFilterC121[i][2]));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_45_L, ((MS_U16)au8Sc1HvspFilterC121[i][4]), 0x00FF);

            // enable write
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L, 0x0100, 0x0100);
        }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_41_L, 0x00, 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, 0x1000, 0x1000);
    }
}

#define HAL_SC_4K2K_WIDTH 3840u
#define HAL_SC_FHD_WIDTH 1920u
#define HAL_SC_720p_WIDTH 1280u
#define HAL_SC_SD_WIDTH 720u
#define HAL_SC_CVBS_TEST_STREAM_WIDTH 704u

static void _Hal_SC_fill_main_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pDBreg)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U64 u64_result = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    // refine memory write protection:
    // we reserve +4 guard-band(total 32 Bytes) for 2 frame mode
    // for example, if the limit addr is set to 0x10000, IPM may write the last packet data from 0x10000 to 0x10003
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (pDBreg->u32WritelimitBase > 3)
    {
        pDBreg->u32WritelimitBase =  pDBreg->u32WritelimitBase - 3;
    }

    //Due to reg_mvop_vsync_off(BK10 0x5D) be set to lately, crop V size cause tearing
    //Martin.Su suggest add another RW_diff condition:
    //Once Write bank check time is over IPM Vertical input size, read bank is force the same with write bank
    if((gSrcInfo[MAIN_WINDOW].bInterlace == FALSE) && (gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES))
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_05_L, BIT(3), BIT(3));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK11_07_L, BIT(3)|BIT(1)|BIT(0), BIT(3)|BIT(1)|BIT(0));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK11_18_L, ((gSrcInfo[MAIN_WINDOW].Status2.u16PreVCusScalingDst)/2)|BIT(15), 0x9FFF);
    }
    else
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_05_L, 0, BIT(3));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK11_07_L, 0, BIT(3)|BIT(1)|BIT(0));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK11_18_L, 0, 0x9FFF);
    }

    //This case used for CVBS performance certification
    //Use both side black edge to cover scaling issue
    //If it exists crop, then it is not the case for CVBS performance certification
    if((pDBreg->u16H_CapSize == HAL_SC_CVBS_TEST_STREAM_WIDTH) && (gSrcInfo[MAIN_WINDOW].ScaledCropWin.width == HAL_SC_CVBS_TEST_STREAM_WIDTH))
    {
        MS_U16 u16DispWidth = pDBreg->u16H_DisEnd - pDBreg->u16H_DisStart + 1;

        if(u16DispWidth == HAL_SC_SD_WIDTH)
        {
            pDBreg->u16H_DisStart += 8;
            pDBreg->u16H_DisEnd -= 8;
#ifdef MSOS_TYPE_LINUX_KERNEL
            H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
            pDBreg->u32H_PostScalingRatio = (MS_U32)u64_result;
#else
            pDBreg->u32H_PostScalingRatio = H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
#endif
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0B_L, 0, 0x00FF);
        }
        else if((u16DispWidth == HAL_SC_720p_WIDTH) || (u16DispWidth == HAL_SC_FHD_WIDTH) || (u16DispWidth == HAL_SC_4K2K_WIDTH))
        {
            MS_U16 u16NewDispWidth = (u16DispWidth * HAL_SC_CVBS_TEST_STREAM_WIDTH) / HAL_SC_SD_WIDTH;

            // Shift display window 1 pixel to avoid the calculated result of SC1 HSD initial factor overflowed
            pDBreg->u16H_DisStart += ((u16DispWidth - u16NewDispWidth) / 2 + 1);
            pDBreg->u16H_DisEnd -= ((u16DispWidth - u16NewDispWidth) / 2 - 1);
#ifdef MSOS_TYPE_LINUX_KERNEL
            H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
            pDBreg->u32H_PostScalingRatio = (MS_U32)u64_result;
#else
            pDBreg->u32H_PostScalingRatio = H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
#endif
            pDBreg->u32H_PostScalingRatio |= BIT(24);
        }
    }

    // Set SC1 pre-scaling factor, initial factor and filter
    if((E_XC_DEVICE1 == psXCInstPri->u32DeviceID) && IsSrcTypeCapture(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        MS_U16 u16H_SC0_BlackPixels = SC_R2BYTE(0, REG_SC_BK10_08_L) - SC_R2BYTE(0, REG_SC_BK10_04_L);
        MS_U16 u16H_SC0_DEWidth = SC_R2BYTE(0, REG_SC_BK10_05_L) - SC_R2BYTE(0, REG_SC_BK10_04_L) + 1;

        _Hal_SC1_LoadHSDAdvModeFilter(pInstance);
#ifdef MSOS_TYPE_LINUX_KERNEL
        H_PostScalingRatio(pDBreg->u16H_CapSize, gSrcInfo[MAIN_WINDOW].u16H_SizeAfterPreScaling);
        pDBreg->u32H_PreScalingRatio = (MS_U32)u64_result;
#else
        pDBreg->u32H_PreScalingRatio = (H_PostScalingRatio(pDBreg->u16H_CapSize, gSrcInfo[MAIN_WINDOW].u16H_SizeAfterPreScaling));
#endif
        pDBreg->u32H_PreScalingRatio |= 0xC0000000L;

        if(u16H_SC0_DEWidth == HAL_SC_4K2K_WIDTH)
        {
            u16H_SC0_BlackPixels/= 2;
        }

        if((pDBreg->u16H_CapSize == HAL_SC_FHD_WIDTH) && (u16H_SC0_BlackPixels > 0))
        {
            MS_U32 u32H_PreScalingInitFactor = (u16H_SC0_BlackPixels << 20) % (pDBreg->u32H_PreScalingRatio & 0x007FFFFF);

            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_02_L, (MS_U16)(u32H_PreScalingInitFactor>>00), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_03_L, (MS_U16)(u32H_PreScalingInitFactor>>16), 0x000F);
        }
        else
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_02_L, 0, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_03_L, 0, 0x000F);
        }
    }

    // Set SC1 post-scaling filter
    if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        if(IS_V_POST_SCALING(pDBreg->u32V_PostScalingRatio) || IS_H_POST_SCALING(pDBreg->u32H_PostScalingRatio))
        {
            if(!pXCResourcePrivate->sthal_SC.bSc1HvspFilterIsSet)
            {
                _Hal_SC1_LoadVSPFilter(pInstance);
                pXCResourcePrivate->sthal_SC.bSc1HvspFilterIsSet = TRUE;
            }
        }

        if(IS_V_POST_SCALING(pDBreg->u32V_PostScalingRatio))
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0B_L, BIT(14)|BIT(12), BIT(15)|BIT(14)|BIT(13)|BIT(12));
        }
        else
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0B_L, 0, BIT(15)|BIT(14)|BIT(13)|BIT(12));
        }

        if(IS_H_POST_SCALING(pDBreg->u32H_PostScalingRatio))
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0B_L, BIT(6)|BIT(4), BIT(7)|BIT(6)|BIT(5)|BIT(4));
        }
        else
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0B_L, 0, BIT(7)|BIT(6)|BIT(5)|BIT(4));
        }
    }

    if((gSrcInfo[MAIN_WINDOW].bR_FBL) || (gSrcInfo[MAIN_WINDOW].bFBL))//set MVOP crop info
    {
#ifndef MSOS_TYPE_OPTEE
        // GET MVOP crop info first
        MVOP_Handle stHdl = { E_MVOP_MODULE_MAIN };
        MVOP_XCGetCrop stCropInfo;
        stCropInfo.u16XStart = gSrcInfo[MAIN_WINDOW].stCropWin.x;
        stCropInfo.u16YStart = gSrcInfo[MAIN_WINDOW].stCropWin.y;
        stCropInfo.u16XSize = gSrcInfo[MAIN_WINDOW].stCropWin.width;
        stCropInfo.u16YSize = gSrcInfo[MAIN_WINDOW].stCropWin.height;

        MDrv_MVOP_GetCommand(&stHdl, E_MVOP_CMD_GET_CROP_FOR_XC, &stCropInfo, sizeof(MVOP_XCGetCrop));

        //Set Mvop Crop info,
        //[crop.x addr, crop.y addr, crop.width addr, crop.height addr] ==>[0x1014 + 0x80, 0x1014 + 0x82, 0x1014 + 0x84, 0x1014 + 0x86]
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1014, 0x80, stCropInfo.u16XStart, 0x1fff);         //0xFFFF000A 0840XXXX
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1014, 0x82, stCropInfo.u16YStart, 0x1fff);         //0xFFFF000A 0840XXXX
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1014, 0x84, stCropInfo.u16XSize, 0x1fff);         //0xFFFF000A 0840XXXX
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1014, 0x86, stCropInfo.u16YSize, 0x1fff);         //0xFFFF000A 0840XXXX
#endif
    }

    //H pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_04_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_05_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>16), 0xFFFF);
    //V pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_08_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK02_09_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>16), 0xFFFF);
    //H post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_07_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_08_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>16), 0xFFFF);
    //V post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_09_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0A_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>16), 0x01FF);
    //SCMI: vertical limitation
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_18_L, (MS_U16)(pDBreg->u16VWritelimit), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_1A_L, (MS_U16)(pDBreg->u32WritelimitBase>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_1B_L, (MS_U16)(pDBreg->u32WritelimitBase>>16), 0xFFFF);
    //SCMI: DNR address 0
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_08_L, (MS_U16)(pDBreg->u32DNRBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_09_L, (MS_U16)(pDBreg->u32DNRBase0>>16), 0xFFFF);
    //SCMI: DNR offset
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_0E_L, (MS_U16)(pDBreg->u16DNROffset>>00), 0xFFFF);
    //SCMI: DNR fetch
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_0F_L, (MS_U16)(pDBreg->u16DNRFetch>>00), 0xFFFF);
    //SCMI: OPM address 0
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_10_L, (MS_U16)(pDBreg->u32OPMBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_11_L, (MS_U16)(pDBreg->u32OPMBase0>>16), 0xFFFF);
    //SCMI: OPM offset
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_16_L, (MS_U16)(pDBreg->u16OPMOffset>>00), 0xFFFF);
    //SCMI: OPM fetch
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_17_L, (MS_U16)(pDBreg->u16OPMFetch>>00), 0xFFFF);

    // If 4K2K bypass mode, turn off XC IPM MIU requests
    //if(Hal_SC_GetBypassMode(pInstance) == E_HAL_SC_BYPASSMODE_BYPASSIP)
    if(gSrcInfo[MAIN_WINDOW].bFBL) // FBL mode
    {
        Hal_SC_IP_Memory_Write_Request(pInstance, DISABLE, MAIN_WINDOW);
    }
    else
    {
        Hal_SC_IP_Memory_Write_Request(pInstance, ENABLE, MAIN_WINDOW);
    }

    //PIP
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_15_L, (MS_U16)(pDBreg->u16VLen>>00), 0xFFFF); // Write V length
    if((gSrcInfo[MAIN_WINDOW].bR_FBL) || (gSrcInfo[MAIN_WINDOW].bFBL))//set MVOP crop info
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1D_L, (MS_U16)(0), 0x00FF);
    }
    else
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1D_L, (MS_U16)(pDBreg->u16LBOffset&0xFF), 0x00FF);
    }
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1C_L, (MS_U16)(pDBreg->u16DispOffset&0xFF), 0x00FF);

    //capture window
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_04_L, (MS_U16)(pDBreg->u16V_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_05_L, (MS_U16)(pDBreg->u16H_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_06_L, (MS_U16)(pDBreg->u16V_CapSize>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_07_L, (MS_U16)(pDBreg->u16H_CapSize>>00), 0x3FFF);


    //Display window
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_08_L, (MS_U16)(pDBreg->u16H_DisStart>>00), VOP_DISPLAY_HSTART_MASK);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_09_L, (MS_U16)(pDBreg->u16H_DisEnd>>00), VOP_DISPLAY_HEND_MASK);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0A_L, (MS_U16)(pDBreg->u16V_DisStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0B_L, (MS_U16)(pDBreg->u16V_DisEnd>>00), 0xFFFF);

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_1C_L, 0x0, 0x1FFF); //always disable IP H mirror line offset note:ds will enable this register at vdec firmware

}

static void _Hal_SC_fill_sub_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pDBreg)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(Hal_SC_is_extra_req_en(pInstance, NULL, NULL, &pDBreg->u16H_DisStart, &pDBreg->u16H_DisEnd))
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_11_L, BIT(15), BIT(15));
    }
    else
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_11_L, 0, BIT(15));
    }

    //H pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK04_04_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK04_05_L, (MS_U16)(pDBreg->u32H_PreScalingRatio>>16), 0xFFFF);
    //V pre-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK04_08_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK04_09_L, (MS_U16)(pDBreg->u32V_PreScalingRatio>>16), 0xFFFF);


    //H post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_27_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_28_L, (MS_U16)(pDBreg->u32H_PostScalingRatio>>16), 0xFFFF);
    //V post-scaling
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_29_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_2A_L, (MS_U16)(pDBreg->u32V_PostScalingRatio>>16), 0xFFFF);

    //SCMI: vertical limitation
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_58_L, (MS_U16)(pDBreg->u16VWritelimit), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_5A_L, (MS_U16)(pDBreg->u32WritelimitBase>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_5B_L, (MS_U16)(pDBreg->u32WritelimitBase>>16), 0xFFFF);
     //SCMI: IPM address 0
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_48_L, (MS_U16)(pDBreg->u32DNRBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_49_L, (MS_U16)(pDBreg->u32DNRBase0>>16), 0xFFFF);
    //SCMI: DNR address 1
    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_4A_L, (MS_U16)(pDBreg->u32DNRBase1>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_4B_L, (MS_U16)(pDBreg->u32DNRBase1>>16), 0xFFFF);

    //SCMI: DNR address 2
    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_4C_L, (MS_U16)(pDBreg->u32DNRBase2>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_4D_L, (MS_U16)(pDBreg->u32DNRBase2>>16), 0xFFFF);

    //SCMI: DNR offset
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_4E_L, (MS_U16)(pDBreg->u16DNROffset>>00), 0xFFFF);
    //SCMI: DNR fetch
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_4F_L, (MS_U16)(pDBreg->u16DNRFetch>>00), 0xFFFF);
    //SCMI: OPM address 2
    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_54_L, (MS_U16)(pDBreg->u32OPMBase2>>00), 0xFFFF);
    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_55_L, (MS_U16)(pDBreg->u32OPMBase2>>16), 0xFFFF);

    //SCMI: OPM address 0
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_50_L, (MS_U16)(pDBreg->u32OPMBase0>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_51_L, (MS_U16)(pDBreg->u32OPMBase0>>16), 0xFFFF);

    //SCMI: OPM offset
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_56_L, (MS_U16)(pDBreg->u16OPMOffset>>00), 0xFFFF);
    //SCMI: OPM fetch
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_57_L, (MS_U16)(pDBreg->u16OPMFetch>>00), 0xFFFF);

        //PIP
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_16_L, (MS_U16)(pDBreg->u16VLen>>00), 0xFFFF); // Write V length
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1D_L, (MS_U16)((pDBreg->u16LBOffset&0xFF)<<8), 0xFF00);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1C_L, (MS_U16)((pDBreg->u16DispOffset&0xFF)<<8), 0xFF00);

    // Extra request setting for fclk limitation
    // Set extra request fetch line from 0x3500 to 0x1300 if V post scaling down is enabled
    // Set jump line threshold to 2 if V post scaling down to half size
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(0))) // Check if main window is enabled
    {
        MS_U32 u32V_PostScalingRatio = 0x100000L;

        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(8))) // Main is top
        {
            u32V_PostScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L) & 0xFFFFFFL;
            SC_DBG(printf("[%s][%d] SUB_WINDOW u32V_PostScalingRatio is %du for compute Extra Request\n", __FUNCTION__, __LINE__, u32V_PostScalingRatio));
        }
        else // Sub is top
        {
            u32V_PostScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L) & 0xFFFFFFL;
            SC_DBG(printf("[%s][%d] MAIN_WINDOW u32V_PostScalingRatio is %du for compute Extra Request\n", __FUNCTION__, __LINE__, u32V_PostScalingRatio));
        }

            if(u32V_PostScalingRatio > 0x100000L)
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_17_L, 0x1300, 0xFF00);
            }
            else
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_17_L, 0x3500, 0xFF00);
            }

            if(u32V_PostScalingRatio > (0x100000L*2)) // If scaling down to at least half size
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_11_L, 0x2, 0x000F);
            }
            else
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_11_L, 0x1, 0x000F);
            }
    }

    // capture window
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_04_L, (MS_U16)(pDBreg->u16V_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_05_L, (MS_U16)(pDBreg->u16H_CapStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_06_L, (MS_U16)(pDBreg->u16V_CapSize>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_07_L, (MS_U16)(pDBreg->u16H_CapSize>>00), 0x3FFF);

        //Display window
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_07_L, (MS_U16)(pDBreg->u16H_DisStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_08_L, (MS_U16)(pDBreg->u16H_DisEnd>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_09_L, (MS_U16)(pDBreg->u16V_DisStart>>00), 0xFFFF);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_0A_L, (MS_U16)(pDBreg->u16V_DisEnd>>00), 0xFFFF);

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_5C_L, 0x0, 0x1FFF); //always disable IP H mirror line offset note:ds will enable this register at vdec firmware

}

void Hal_SC_sw_db_burst( void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow )
{
    _MLOAD_ENTRY(pInstance);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        // Discuss with HW. In k6lite, if post v scaling down case can not use fclk tune
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x1))
        {
            if((pDBreg->u32V_PostScalingRatio & 0x1000000) && ((pDBreg->u32V_PostScalingRatio & 0xFFFFFF) > 0x100000))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x0, 0x1);
            }
        }
    }

    if( eWindow == MAIN_WINDOW )
    {
        _Hal_SC_fill_main_sw_db_burst(pInstance, pDBreg);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
    }
    else
    {
        _Hal_SC_fill_sub_sw_db_burst(pInstance, pDBreg);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
    }

    _MLOAD_RETURN(pInstance);

}

void Hal_SC_dual_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pMainDBreg, P_SC_SWDB_INFO pSubDBreg)
{
    _MLOAD_ENTRY(pInstance);

    _Hal_SC_fill_main_sw_db_burst(pInstance, pMainDBreg);
    _Hal_SC_fill_sub_sw_db_burst(pInstance, pSubDBreg);
    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_sw_db( void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow )
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U64 u64_result = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        // Discuss with HW. In k6lite, if post v scaling down case can not use fclk tune
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x1))
        {
            if((pDBreg->u32V_PostScalingRatio & 0x1000000) && ((pDBreg->u32V_PostScalingRatio & 0xFFFFFF) > 0x100000))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x0, 0x1);
            }
        }
    }

    if( eWindow == MAIN_WINDOW )
    {
        // refine memory write protection:
        // we reserve +4 guard-band(total 32 Bytes) for 2 frame mode
        // for example, if the limit addr is set to 0x10000, IPM may write the last packet data from 0x10000 to 0x10003
        if (pDBreg->u32WritelimitBase > 3)
        {
            pDBreg->u32WritelimitBase =  pDBreg->u32WritelimitBase - 3;
        }

        if((gSrcInfo[MAIN_WINDOW].bInterlace == FALSE) && (gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, BIT(3), BIT(3));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_07_L, BIT(3)|BIT(1)|BIT(0), BIT(3)|BIT(1)|BIT(0));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_18_L, ((gSrcInfo[MAIN_WINDOW].Status2.u16PreVCusScalingDst)/2)|BIT(15), 0x9FFF);
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0, BIT(3));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_07_L, 0, BIT(3)|BIT(1)|BIT(0));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_18_L, 0, 0x9FFF);
        }

        //This case used for CVBS performance certification
        //Use both side black edge to cover scaling issue
        //If it exists crop, then it is not the case for CVBS performance certification
        if((pDBreg->u16H_CapSize == HAL_SC_CVBS_TEST_STREAM_WIDTH) && (gSrcInfo[eWindow].ScaledCropWin.width == HAL_SC_CVBS_TEST_STREAM_WIDTH))
        {
            MS_U16 u16DispWidth = pDBreg->u16H_DisEnd - pDBreg->u16H_DisStart + 1;

            if(u16DispWidth == HAL_SC_SD_WIDTH)
            {
                pDBreg->u16H_DisStart += 8;
                pDBreg->u16H_DisEnd -= 8;
#ifdef MSOS_TYPE_LINUX_KERNEL
                H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
                pDBreg->u32H_PostScalingRatio = (MS_U32)u64_result;
#else
                pDBreg->u32H_PostScalingRatio = H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
#endif
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0, 0x00FF);
            }
            else if((u16DispWidth == HAL_SC_720p_WIDTH) || (u16DispWidth == HAL_SC_FHD_WIDTH) || (u16DispWidth == HAL_SC_4K2K_WIDTH))
            {
                MS_U16 u16NewDispWidth = (u16DispWidth * HAL_SC_CVBS_TEST_STREAM_WIDTH) / HAL_SC_SD_WIDTH;

                // Shift display window 1 pixel to avoid the calculated result of SC1 HSD initial factor overflowed
                pDBreg->u16H_DisStart += ((u16DispWidth - u16NewDispWidth) / 2 + 1);
                pDBreg->u16H_DisEnd -= ((u16DispWidth - u16NewDispWidth) / 2 - 1);
#ifdef MSOS_TYPE_LINUX_KERNEL
                H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
                pDBreg->u32H_PostScalingRatio = (MS_U32)u64_result;
#else
                pDBreg->u32H_PostScalingRatio = H_PostScalingRatio(HAL_SC_SD_WIDTH, u16DispWidth);
#endif
                pDBreg->u32H_PostScalingRatio |= BIT(24);
            }
        }

        // Set SC1 pre-scaling factor, initial factor and filter
        if((E_XC_DEVICE1 == psXCInstPri->u32DeviceID) && IsSrcTypeCapture(gSrcInfo[eWindow].enInputSourceType))
        {
            MS_U16 u16H_SC0_BlackPixels = SC_R2BYTE(0, REG_SC_BK10_08_L) - SC_R2BYTE(0, REG_SC_BK10_04_L);
            MS_U16 u16H_SC0_DEWidth = SC_R2BYTE(0, REG_SC_BK10_05_L) - SC_R2BYTE(0, REG_SC_BK10_04_L) + 1;

            _Hal_SC1_LoadHSDAdvModeFilter(pInstance);
#ifdef MSOS_TYPE_LINUX_KERNEL
            H_PostScalingRatio(pDBreg->u16H_CapSize, gSrcInfo[MAIN_WINDOW].u16H_SizeAfterPreScaling);
            pDBreg->u32H_PreScalingRatio = (MS_U32)u64_result;
#else
            pDBreg->u32H_PreScalingRatio = (H_PostScalingRatio(pDBreg->u16H_CapSize, gSrcInfo[MAIN_WINDOW].u16H_SizeAfterPreScaling));
#endif
            pDBreg->u32H_PreScalingRatio |= 0xC0000000L;

            if(u16H_SC0_DEWidth == HAL_SC_4K2K_WIDTH)
            {
                u16H_SC0_BlackPixels/= 2;
            }

            if((pDBreg->u16H_CapSize == HAL_SC_FHD_WIDTH) && (u16H_SC0_BlackPixels > 0))
            {
                MS_U32 u32H_PreScalingInitFactor = (u16H_SC0_BlackPixels << 20) % (pDBreg->u32H_PreScalingRatio & 0x007FFFFF);

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_02_L, (MS_U16)(u32H_PreScalingInitFactor>>00));
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_03_L, (MS_U16)(u32H_PreScalingInitFactor>>16), 0x000F);
            }
            else
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_02_L, 0);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_03_L, 0, 0x000F);
            }
        }

        // Set SC1 post-scaling filter
        if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            if(IS_V_POST_SCALING(pDBreg->u32V_PostScalingRatio) || IS_H_POST_SCALING(pDBreg->u32H_PostScalingRatio))
            {
                if(!pXCResourcePrivate->sthal_SC.bSc1HvspFilterIsSet)
                {
                    _Hal_SC1_LoadVSPFilter(pInstance);
                    pXCResourcePrivate->sthal_SC.bSc1HvspFilterIsSet = TRUE;
                }
            }

            if(IS_V_POST_SCALING(pDBreg->u32V_PostScalingRatio))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, BIT(14)|BIT(12), BIT(15)|BIT(14)|BIT(13)|BIT(12));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0, BIT(15)|BIT(14)|BIT(13)|BIT(12));
            }

            if(IS_H_POST_SCALING(pDBreg->u32H_PostScalingRatio))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, BIT(6)|BIT(4), BIT(7)|BIT(6)|BIT(5)|BIT(4));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0, BIT(7)|BIT(6)|BIT(5)|BIT(4));
            }
        }

        if((gSrcInfo[MAIN_WINDOW].bR_FBL) || (gSrcInfo[MAIN_WINDOW].bFBL))//set MVOP crop info
        {
#ifndef MSOS_TYPE_OPTEE
            // GET MVOP crop info first
            MVOP_Handle stHdl = { E_MVOP_MODULE_MAIN };
            MVOP_XCGetCrop stCropInfo;
            stCropInfo.u16XStart = gSrcInfo[MAIN_WINDOW].stCropWin.x;
            stCropInfo.u16YStart = gSrcInfo[MAIN_WINDOW].stCropWin.y;
            stCropInfo.u16XSize = gSrcInfo[MAIN_WINDOW].stCropWin.width;
            stCropInfo.u16YSize = gSrcInfo[MAIN_WINDOW].stCropWin.height;

            MDrv_MVOP_GetCommand(&stHdl, E_MVOP_CMD_GET_CROP_FOR_XC, &stCropInfo, sizeof(MVOP_XCGetCrop));
            //Set Mvop Crop info,
            //[crop.x addr, crop.y addr, crop.width addr, crop.height addr] ==>[0x1014 + 0x80, 0x1014 + 0x82, 0x1014 + 0x84, 0x1014 + 0x86]
            MDrv_Write2ByteMask((REG_MVOP_BASE + 0x80), stCropInfo.u16XStart, 0x1fff);
            MDrv_Write2ByteMask((REG_MVOP_BASE + 0x82), stCropInfo.u16YStart, 0x1fff);
            MDrv_Write2ByteMask((REG_MVOP_BASE + 0x84), stCropInfo.u16XSize, 0x1fff);
            MDrv_Write2ByteMask((REG_MVOP_BASE + 0x86), stCropInfo.u16YSize, 0x1fff);
#endif
        }

        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling
        //HVSP
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_07_L, pDBreg->u32H_PostScalingRatio);  // H post-scaling
        //SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L, pDBreg->u32V_PostScalingRatio);  // V post-scaling
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L, pDBreg->u32V_PostScalingRatio);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L, (pDBreg->u32V_PostScalingRatio>>16), 0x01FF);
        //SCMI
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pDBreg->u16VWritelimit); // Set vertical limitation
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_1A_L, pDBreg->u32WritelimitBase);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L, pDBreg->u32DNRBase0);     // input address0
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pDBreg->u16DNROffset);// input Offset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pDBreg->u16DNRFetch); // input fetch
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_10_L, pDBreg->u32OPMBase0); // output address0
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pDBreg->u16OPMOffset); // output offset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pDBreg->u16OPMFetch);  // output fetch

        // If 4K2K bypass mode, turn off XC IPM MIU requests
        //if (Hal_SC_GetBypassMode(pInstance) == E_HAL_SC_BYPASSMODE_BYPASSIP)
        if(gSrcInfo[MAIN_WINDOW].bFBL) // FBL mode
        {
            Hal_SC_IP_Memory_Write_Request(pInstance, DISABLE, MAIN_WINDOW);
        }
        else
        {
            Hal_SC_IP_Memory_Write_Request(pInstance, ENABLE, MAIN_WINDOW);
        }

        //PIP
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pDBreg->u16VLen); // Write V length
        if((gSrcInfo[eWindow].bR_FBL) || (gSrcInfo[eWindow].bFBL))
        {
            //In FBL & R_FBL case,  Mvop do crop , no need do crop by Line buffer offset
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, 0);
        }
        else
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, pDBreg->u16LBOffset);
        }
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, pDBreg->u16DispOffset);

        // capture window
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L, pDBreg->u16V_CapStart); // Capture V start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L, pDBreg->u16H_CapStart); // Capture H start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L, pDBreg->u16V_CapSize);  // Capture V size
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, pDBreg->u16H_CapSize, 0x3FFF);  // Capture H size

        //Display window
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pDBreg->u16H_DisStart); // Display H start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pDBreg->u16H_DisEnd);   // Display H end
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pDBreg->u16V_DisStart); // Display V start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pDBreg->u16V_DisEnd);   // Display V end

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_1C_L, 0x0, 0x1FFF);         //always disable IP H mirror line offset note:ds will enable this register at vdec firmware
    }
    else
    {
        if(Hal_SC_is_extra_req_en(pInstance, NULL, NULL, &pDBreg->u16H_DisStart, &pDBreg->u16H_DisEnd))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, BIT(15), BIT(15));
        }
        else
        {
             SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, 0, BIT(15));
        }

        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_04_L, pDBreg->u32H_PreScalingRatio);   // H pre-scaling
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_08_L, pDBreg->u32V_PreScalingRatio);   // V pre-scaling

        //HVSP
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_27_L, pDBreg->u32H_PostScalingRatio);  // H post-scaling
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L, pDBreg->u32V_PostScalingRatio);  // V post-scaling

        //SCMI
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_58_L, pDBreg->u16VWritelimit); // Set vertical limitation
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_5A_L, pDBreg->u32WritelimitBase);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_48_L, pDBreg->u32DNRBase0);     // input address0
        //SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4A_L, pDBreg->u32DNRBase1); // input address1
        //SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4C_L, pDBreg->u32DNRBase2); // input address2
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4E_L, pDBreg->u16DNROffset);// input Offset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4F_L, pDBreg->u16DNRFetch); // input fetch

        //SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_52_L, pDBreg->u32OPMBase1); // output address1
        //SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_54_L, pDBreg->u32OPMBase2); // output address2

        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pDBreg->u32OPMBase0); // output address0
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pDBreg->u16OPMOffset); // output offset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pDBreg->u16OPMFetch);  // output fetch

        //PIP
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pDBreg->u16VLen); // Write V length
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pDBreg->u16LBOffset&0xFF)<<8, 0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, (pDBreg->u16DispOffset&0xFF)<<8, 0xFF00);

        // Extra request setting for fclk limitation
        // Set extra request fetch line from 0x3500 to 0x1300 if V post scaling down is enabled
        // Set jump line threshold to 2 if V post scaling down to half size
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(0))) // Check if main window is enabled
        {
            MS_U32 u32V_PostScalingRatio = 0x100000L;

            if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(8))) // Main is top
            {
                u32V_PostScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L) & 0xFFFFFFL;
                SC_DBG(printf("[%s][%d] SUB_WINDOW u32V_PostScalingRatio is %du for compute Extra Request\n", __FUNCTION__, __LINE__, u32V_PostScalingRatio));
            }
            else // Sub is top
            {
                u32V_PostScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L) & 0xFFFFFFL;
                SC_DBG(printf("[%s][%d] MAIN_WINDOW u32V_PostScalingRatio is %du for compute Extra Request\n", __FUNCTION__, __LINE__, u32V_PostScalingRatio));
            }

            if(u32V_PostScalingRatio > 0x100000L)
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_17_L, SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_17_L, 0x00FF)|0x1300);
            }
            else
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_17_L, SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_17_L, 0x00FF)|0x3500);
            }

            if(u32V_PostScalingRatio > (0x100000L*2)) // If scaling down to at least half size
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, 0xFFF0)|0x2);
            }
            else
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, 0xFFF0)|0x1);
            }
        }

        // capture window
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L, pDBreg->u16V_CapStart); // Capture V start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L, pDBreg->u16H_CapStart); // Capture H start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L, pDBreg->u16V_CapSize);  // Capture V size
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, pDBreg->u16H_CapSize, 0x3FFF);  // Capture H size

        //Display window
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pDBreg->u16H_DisStart); // Display H start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pDBreg->u16H_DisEnd);   // Display H end
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pDBreg->u16V_DisStart); // Display V start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pDBreg->u16V_DisEnd);   // Display V end

    }

}

void Hal_SC_SWDS_AddCmd(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] eWindow:%d bDynamicScalingEnable%d  \n",__func__,__LINE__,eWindow,pXCResourcePrivate->sthal_SC.bDynamicScalingEnable);

    XC_DS_CMDCNT PSTXC_DS_CMDCNT;

    PSTXC_DS_CMDCNT.u16CMDCNT_IPM =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_IPS =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_OPM =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_OPS =0;

    if(eWindow == MAIN_WINDOW)
    {
        if(pXCResourcePrivate->sthal_SC.bDynamicScalingEnable)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, gSrcInfo[eWindow].Status2.u16VirtualBox_Width);// input Offset
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, gSrcInfo[eWindow].Status2.u16VirtualBox_Width); // output offset

            pXCResourcePrivate->sthal_SC.u8DynamicScalingNextIndex[MAIN_WINDOW] = pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[MAIN_WINDOW];
//Command Priority:
//1.GOP
//2.MVOP
//3.XC
            MS_U32 ActiveSWClearEn = 0;
            //MS_U32 u32SW_Reset_Enable = 0;
            //ActiveSWClearEn enable in first DS IP command ;
            //u32SW_Reset_Enable = 0x4000; // bit10: reg_ds_ipm_active_sel: 0:HW 1:SW
            //ActiveSWClearEn = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_17_L, 0xFFFF);
            //ActiveSWClearEn |= u32SW_Reset_Enable;
            //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_17_L, ActiveSWClearEn);

            //XC_PRINTF("[%s,%5d] bR_FBL:%d bFBL:%d  \n",__func__,__LINE__,gSrcInfo[MAIN_WINDOW].bR_FBL,gSrcInfo[MAIN_WINDOW].bFBL);
            //XC_PRINTF("[%s,%5d] Crop(x,y,w,h)=(%d,%d,%d,%d) \n",__func__,__LINE__,gSrcInfo[eWindow].stCropWin.x,gSrcInfo[eWindow].stCropWin.y,gSrcInfo[eWindow].stCropWin.width,gSrcInfo[eWindow].stCropWin.height);

            if((gSrcInfo[MAIN_WINDOW].bR_FBL) || (gSrcInfo[MAIN_WINDOW].bFBL))//set MVOP crop info
            {
#ifndef MSOS_TYPE_OPTEE
                // GET MVOP crop info first
                MVOP_Handle stHdl = { E_MVOP_MODULE_MAIN };
                MVOP_XCGetCrop stCropInfo;
                stCropInfo.u16XStart = gSrcInfo[eWindow].stCropWin.x;
                stCropInfo.u16YStart = gSrcInfo[eWindow].stCropWin.y;
                stCropInfo.u16XSize = gSrcInfo[eWindow].stCropWin.width;
                stCropInfo.u16YSize = gSrcInfo[eWindow].stCropWin.height;

                MDrv_MVOP_GetCommand(&stHdl, E_MVOP_CMD_GET_CROP_FOR_XC, &stCropInfo, sizeof(MVOP_XCGetCrop));

                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_MVOP_CROP_H_START,stCropInfo.u16XStart,DS_IP,DS_MVOP,&PSTXC_DS_CMDCNT);         //0xFFFF000A 0840XXXX
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_MVOP_CROP_V_START,stCropInfo.u16YStart,DS_IP,DS_MVOP,&PSTXC_DS_CMDCNT);  //0xFFFF000A 0841XXXX
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_MVOP_CROP_H_SIZE,stCropInfo.u16XSize,DS_IP,DS_MVOP,&PSTXC_DS_CMDCNT);           //0xFFFF000A 0842XXXX
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_MVOP_CROP_V_SIZE,stCropInfo.u16YSize,DS_IP,DS_MVOP,&PSTXC_DS_CMDCNT);           //0xFFFF000A 0843XXXX
#endif
            }

            //H pre-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_04_L,(MS_U16)(pDBreg->u32H_PreScalingRatio>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8104XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_05_L,(MS_U16)(pDBreg->u32H_PreScalingRatio>>16),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8105XXXX
            //V pre-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_08_L,(MS_U16)(pDBreg->u32V_PreScalingRatio>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8108XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_09_L,(MS_U16)(pDBreg->u32V_PreScalingRatio>>16),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8109XXXX
            //H post-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_07_L,(MS_U16)(pDBreg->u32H_PostScalingRatio>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 9187XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_08_L,(MS_U16)(pDBreg->u32H_PostScalingRatio>>16),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 9188XXXX
            //V post-scaling
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_09_L,(MS_U16)(pDBreg->u32V_PostScalingRatio>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 9189XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK23_0A_L,(MS_U16)(pDBreg->u32V_PostScalingRatio>>16),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 918AXXXX

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_18_L,(MS_U16)(pDBreg->u16VWritelimit>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8918XXXX

            if(IsVMirrorMode(MAIN_WINDOW))
            {
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_08_L,(MS_U16)(pDBreg->u32DNRBase0>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_09_L,(MS_U16)(pDBreg->u32DNRBase0>>16),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);
            }
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_0F_L,(MS_U16)(pDBreg->u16DNRFetch>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 890FXXXX

            //H_mirror_offset
            if(IsHMirrorMode(MAIN_WINDOW))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] VB_width:%d framNum:%d u8BitPerPixel:%d  \n",__func__,__LINE__,gSrcInfo[MAIN_WINDOW].Status2.u16VirtualBox_Width,MDrv_XC_TransFrameNumToFactor(pInstance,gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber, gSrcInfo[MAIN_WINDOW].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED),gSrcInfo[MAIN_WINDOW].u8BitPerPixel);

                MS_U32 H_mirror_offset;
                H_mirror_offset = (gSrcInfo[MAIN_WINDOW].Status2.u16VirtualBox_Width * gSrcInfo[MAIN_WINDOW].u8BitPerPixel / 8 * MDrv_XC_TransFrameNumToFactor(pInstance,gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber, gSrcInfo[MAIN_WINDOW].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED)) / BYTE_PER_WORD;

                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_1C_L,H_mirror_offset,DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 891CXXXX
            }
            else
            {
                Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_1C_L,0x0000,DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 891CXXXX
            }

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_10_L, (MS_U16)(pDBreg->u32OPMBase0>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // OPM base0  //0xFFFF0009 8910XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_11_L,(MS_U16)(pDBreg->u32OPMBase0>>16),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // OPM base0   //0xFFFF0009 8911XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK12_17_L,(MS_U16)(pDBreg->u16OPMFetch>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // OPM fetch   //0xFFFF0009 8917XXXX
            //Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK20_15_L, (MS_U16)(pDBreg->u16VLen>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // V Len
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK20_1D_L,(MS_U16)(pDBreg->u16LBOffset&0xFF),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); //LBOffset    //0xFFFF0009 901DXXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK20_1D_L,(MS_U16)(pDBreg->u16LBOffset&0xFF),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); //LBOffset    //0xFFFF0009 901DXXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK20_15_L, (MS_U16)(pDBreg->u16VLen>>00),DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // V Len          //0xFFFF0009 9015XXXX
            //Cap info
            /// before enable ML bit mask feature, the capture size for 2P setting BK01_07[15] need to be considered
            pDBreg->u16H_CapSize |= 0x8000;

            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK01_06_L,(MS_U16)(pDBreg->u16V_CapSize>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8086XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK01_07_L,(MS_U16)(pDBreg->u16H_CapSize>>00),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);//0xFFFF0009 8087XXXX

            //Disp info
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_08_L,(MS_U16)(pDBreg->u16H_DisStart),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //0xFFFF0009 8808XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_09_L,(MS_U16)(pDBreg->u16H_DisEnd),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);    //0xFFFF0009 8809XXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_0A_L,(MS_U16)(pDBreg->u16V_DisStart),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //0xFFFF0009 880AXXXX
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK10_0B_L,(MS_U16)(pDBreg->u16V_DisEnd),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);    //0xFFFF0009 880BXXXX

            //toggle reg_ds_ipm_active_set in last DS IP command
            MS_U32 u32SW_Reset_IPM = 0;
            u32SW_Reset_IPM = 0x0010 ;//bit4
            ActiveSWClearEn = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_17_L, 0xFFFF);
            ActiveSWClearEn |= u32SW_Reset_IPM;
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK1F_17_L,(MS_U16)(ActiveSWClearEn),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);

            MS_U8 u8MaxCmdCnt = pXCResourcePrivate->sthal_SC.u8DynamicScalingIndexDepth * 2;// It has two 64bit commands per MIU bus width

            //XC_PRINTF("[%s,%5d] u16CMDCNT_IPM:%d   u8MaxCnt:%d \n",__func__,__LINE__,PSTXC_DS_CMDCNT.u16CMDCNT_IPM,u8MaxCmdCnt);
            //XC_PRINTF("[%s,%5d] u16CMDCNT_OPM:%d   u8MaxCnt:%d \n",__func__,__LINE__,PSTXC_DS_CMDCNT.u16CMDCNT_OPM,u8MaxCmdCnt);
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] %d %d %d  \n",__func__,__LINE__,PSTXC_DS_CMDCNT.u16CMDCNT_IPM,PSTXC_DS_CMDCNT.u16CMDCNT_OPM,u8MaxCmdCnt);
            if(PSTXC_DS_CMDCNT.u16CMDCNT_IPM < u8MaxCmdCnt)
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] Add NULL to IPM\n",__func__,__LINE__);
                Hal_SC_Add_NullCommand(pInstance,MAIN_WINDOW,DS_IP,&PSTXC_DS_CMDCNT);
            }
            if(PSTXC_DS_CMDCNT.u16CMDCNT_OPM < u8MaxCmdCnt)
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] Add NULL to OPM\n",__func__,__LINE__);
                Hal_SC_Add_NullCommand(pInstance,MAIN_WINDOW,DS_OP,&PSTXC_DS_CMDCNT);
            }

        }
        else
        {
            SC_DBG(printf("Error! SWDS_DB flow but DS not enable!!\n"));
        }
    }
}

void Hal_SC_SWDS_Fire(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow]++;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] CurIdx:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow]);

    pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow] = pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex[eWindow] % DS_BUFFER_NUM_EX;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS] CurIdx_Ring:%d  \n",__func__,__LINE__,(int)pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[eWindow])
}

E_APIXC_ReturnValue Hal_SC_VIP_Peaking_Setting(void *pInstance, SCALER_WIN eWindow)
{
    UNUSED(eWindow);
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
}

E_APIXC_ReturnValue Hal_SC_support_source_to_ve(void *pInstance, MS_U16* OutputCapability)
{
	*OutputCapability = E_XC_OP2;
	return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_set_output_capture_enable(void *pInstance, MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE)
{
    if(bEnable != TRUE && bEnable != FALSE)
    {
        SC_DBG(printf("bEnable setting error!\n"));
    }
    if(eSourceToVE == E_XC_NONE)
    {
        SC_DBG(printf("Set Source to VE is XC_None!\n"));
    }
	return E_APIXC_RET_OK;
}

/******************************************************************************/
///Set output DE window size
///@param pstWindow \b IN
///-  pointer to window information
/******************************************************************************/
void Hal_SC_set_de_window(void *pInstance, XC_PANEL_INFO *pPanel_Info)
{
// contrl by dac table,not allow to change,
// or it will cause BOX output fail.
}

void Hal_SC_get_disp_de_window(void *pInstance, MS_WINDOW_TYPE *pWin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    pWin->x = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L);
    pWin->y = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L);
    pWin->width = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L) - pWin->x + 1;
    pWin->height= SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) - pWin->y + 1;
}

MS_PHY Hal_SC_Get_DNRBase0(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L);     // input address0
    }
    else
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_48_L);     // input address0
    }
}

MS_PHY Hal_SC_Get_DNRBase1(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0A_L);     // input address1
    }
    else
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4A_L);     // input address1
    }
}

MS_PHY Hal_SC_Get_OPMBase0(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_10_L);     // output address0
    }
    else
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L);     // output address0
    }
}

MS_PHY Hal_SC_Get_OPMBase1(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_12_L);     // output address1
    }
    else
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_52_L);     // output address1
    }
}

MS_PHY Hal_SC_Get_OPMBase2(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L);     // output address1
    }
    else
    {
        return SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_54_L);     // output address1
    }
}

MS_U8 Hal_SC_Get_LBOffset(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, 0x00FF);
    }
    else
    {
        return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, 0xFF00) >> 8);
    }
}

void Hal_SC_Set_LBOffset(void *pInstance, MS_U8 u8LBOffset, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, u8LBOffset, 0x00FF);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, u8LBOffset << 8, 0xFF00);
    }
}

void Hal_SC_set_DNRBase0(void *pInstance, MS_PHY u32DNRBase0, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L, u32DNRBase0 / BYTE_PER_WORD);     // input address0
    }
    else
    {
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_48_L, u32DNRBase0 / BYTE_PER_WORD);     // input address0
    }
}

void Hal_SC_set_DNRBase1(void *pInstance, MS_PHY u32DNRBase1, SCALER_WIN eWindow)
{
    UNUSED(u32DNRBase1);
    UNUSED(eWindow);
}

void Hal_SC_set_DNRBase2(void *pInstance, MS_PHY u32DNRBase2, SCALER_WIN eWindow)
{
    UNUSED(u32DNRBase2);
    UNUSED(eWindow);
}

void Hal_SC_set_OPMBase0(void *pInstance, MS_PHY u32OPMBase0, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_10_L, u32OPMBase0 / BYTE_PER_WORD);     // input address0
    }
    else
    {
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, u32OPMBase0 / BYTE_PER_WORD);     // input address0
    }
}

void Hal_SC_set_OPMBase1(void *pInstance, MS_PHY u32OPMBase1, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_12_L, u32OPMBase1 / BYTE_PER_WORD);     // input address1
    }
    else
    {
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_52_L, u32OPMBase1 / BYTE_PER_WORD);     // input address1
    }
}

void Hal_SC_set_OPMBase2(void *pInstance, MS_PHY u32OPMBase2, SCALER_WIN eWindow)
{
    UNUSED(u32OPMBase2);
    UNUSED(eWindow);
}

void Hal_SC_set_WriteVLength(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, 0x0);     // Write V length
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, 0x0);     // Write V length
    }
}

void Hal_SC_set_memoryaddress(void *pInstance, MS_PHY u32DNRBase0, MS_PHY u32DNRBase1, MS_PHY u32DNRBase2, MS_PHY u32OPMBase0, MS_PHY u32OPMBase1, MS_PHY u32OPMBase2, SCALER_WIN eWindow)
{
    Hal_SC_set_DNRBase0(pInstance, u32DNRBase0, eWindow);
    Hal_SC_set_DNRBase1(pInstance, u32DNRBase1, eWindow);
    Hal_SC_set_DNRBase2(pInstance, u32DNRBase2, eWindow);
    Hal_SC_set_OPMBase0(pInstance, u32OPMBase0, eWindow);
    Hal_SC_set_OPMBase1(pInstance, u32OPMBase1, eWindow);
    Hal_SC_set_OPMBase2(pInstance, u32OPMBase2, eWindow);
    Hal_SC_set_WriteVLength(pInstance, eWindow); // Write V length

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(psXCInstPri->u32DeviceID == 0 && eWindow == MAIN_WINDOW)
    {
        if(pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow] > 0)
        {
            Hal_SC_IP_Memory_Write_Request(pInstance,ENABLE,MAIN_WINDOW);
        }
        else
        {
            Hal_SC_IP_Memory_Write_Request(pInstance,DISABLE,MAIN_WINDOW);
        }
    }

#endif
#endif

}

static SC_MIUMASK_t Hal_SC_get_miu0mask(void)
{
    SC_MIUMASK_t mask;

    mask.u16MiuG0Mask = MDrv_Read2Byte(MIU0_G0_REQUEST_MASK);
    mask.u16MiuG1Mask = MDrv_Read2Byte(MIU0_G1_REQUEST_MASK);
    mask.u16MiuG2Mask = MDrv_Read2Byte(MIU0_G2_REQUEST_MASK);
    mask.u16MiuG3Mask = MDrv_Read2Byte(MIU0_G3_REQUEST_MASK);
    mask.u16MiuG4Mask = MDrv_Read2Byte(MIU0_G4_REQUEST_MASK);
    mask.u16MiuG5Mask = MDrv_Read2Byte(MIU0_G5_REQUEST_MASK);

    return mask;
}

static SC_MIUMASK_t Hal_SC_get_miu1mask(void)
{
    SC_MIUMASK_t mask;

    mask.u16MiuG0Mask = MDrv_Read2Byte(MIU1_G0_REQUEST_MASK);
    mask.u16MiuG1Mask = MDrv_Read2Byte(MIU1_G1_REQUEST_MASK);
    mask.u16MiuG2Mask = MDrv_Read2Byte(MIU1_G2_REQUEST_MASK);
    mask.u16MiuG3Mask = MDrv_Read2Byte(MIU1_G3_REQUEST_MASK);
    mask.u16MiuG4Mask = MDrv_Read2Byte(MIU1_G4_REQUEST_MASK);
    mask.u16MiuG5Mask = MDrv_Read2Byte(MIU1_G5_REQUEST_MASK);

    return mask;
}

static void Hal_SC_set_miu0mask(SC_MIUMASK_t mask)
{
    MDrv_Write2Byte(MIU0_G0_REQUEST_MASK, mask.u16MiuG0Mask);
    MDrv_Write2Byte(MIU0_G1_REQUEST_MASK, mask.u16MiuG1Mask);
    MDrv_Write2Byte(MIU0_G2_REQUEST_MASK, mask.u16MiuG2Mask);
}

static void Hal_SC_set_miu1mask(SC_MIUMASK_t mask)
{
    MDrv_Write2Byte(MIU1_G0_REQUEST_MASK, mask.u16MiuG0Mask);
    MDrv_Write2Byte(MIU1_G1_REQUEST_MASK, mask.u16MiuG1Mask);
    MDrv_Write2Byte(MIU1_G2_REQUEST_MASK, mask.u16MiuG2Mask);
}

void Hal_SC_Enable_MiuMask(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC.Miu0MaskOld = Hal_SC_get_miu0mask();
    pXCResourcePrivate->sthal_SC.Miu1MaskOld = Hal_SC_get_miu1mask();

    pXCResourcePrivate->sthal_SC.Miu0Mask = pXCResourcePrivate->sthal_SC.Miu0MaskOld;
    pXCResourcePrivate->sthal_SC.Miu1Mask = pXCResourcePrivate->sthal_SC.Miu1MaskOld;

    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG0Mask |= MIU_SC0_G0REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG1Mask |= MIU_SC0_G1REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG2Mask |= MIU_SC0_G2REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG3Mask |= MIU_SC0_G3REQUEST_MASK;

        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG0Mask |= MIU_SC0_G0REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG1Mask |= MIU_SC0_G1REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG2Mask |= MIU_SC0_G2REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG3Mask |= MIU_SC0_G3REQUEST_MASK;
    }
    else  //SC1
    {
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG0Mask |= MIU_SC1_G0REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG1Mask |= MIU_SC1_G1REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG2Mask |= MIU_SC1_G2REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu0Mask.u16MiuG3Mask |= MIU_SC1_G3REQUEST_MASK;

        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG0Mask |= MIU_SC1_G0REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG1Mask |= MIU_SC1_G1REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG2Mask |= MIU_SC1_G2REQUEST_MASK;
        pXCResourcePrivate->sthal_SC.Miu1Mask.u16MiuG3Mask |= MIU_SC1_G3REQUEST_MASK;
    }
    Hal_SC_set_miu0mask(pXCResourcePrivate->sthal_SC.Miu0Mask);
    Hal_SC_set_miu1mask(pXCResourcePrivate->sthal_SC.Miu1Mask);
}

void Hal_SC_Disable_MiuMask(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    Hal_SC_set_miu0mask(pXCResourcePrivate->sthal_SC.Miu0MaskOld);
    Hal_SC_set_miu1mask(pXCResourcePrivate->sthal_SC.Miu1MaskOld);
}

void Hal_SC_set_miusel(void *pInstance, MS_U8 u8MIUSel)
{
    // Scaler control MIU by itself
    // So MIU1 IP-select is set to 1. At this moment, MIU0 Miu select is not working.
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (u8MIUSel==0)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x00, 0x10);     // reg_miu_select_f2

        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF3, 0x00, 0x07);
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF3, 0x07, 0x07);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x10, 0x10);     // reg_miu_select_f2

        MDrv_WriteByteMask(REG_MIU0_BASE + 0xF3, 0x00, 0x07);
        MDrv_WriteByteMask(REG_MIU1_BASE + 0xF3, 0x07, 0x07);
    }

    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF0), 0x00, MIU_SC0_G0REQUEST_MASK); // MIU select (Group0)
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF2), 0x00, MIU_SC0_G1REQUEST_MASK); // MIU select (Group1)
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF4), 0x00, MIU_SC0_G2REQUEST_MASK); // MIU select (Group2)
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF6), 0x00, MIU_SC0_G3REQUEST_MASK); // MIU select (Group3)

        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF0), MIU_SC0_G0REQUEST_MASK, MIU_SC0_G0REQUEST_MASK); // MIU select (Group0)
        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF2), MIU_SC0_G1REQUEST_MASK, MIU_SC0_G1REQUEST_MASK); // MIU select (Group1)
        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF4), MIU_SC0_G2REQUEST_MASK, MIU_SC0_G2REQUEST_MASK); // MIU select (Group2)
        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF6), MIU_SC0_G3REQUEST_MASK, MIU_SC0_G3REQUEST_MASK); // MIU select (Group3)
    }
    else //SC1
    {
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF0), 0x00, MIU_SC1_G0REQUEST_MASK); // MIU select (Group0)
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF2), 0x00, MIU_SC1_G1REQUEST_MASK); // MIU select (Group1)
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF4), 0x00, MIU_SC1_G2REQUEST_MASK); // MIU select (Group2)
        MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF6), 0x00, MIU_SC1_G3REQUEST_MASK); // MIU select (Group3)

        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF0), MIU_SC1_G0REQUEST_MASK, MIU_SC1_G0REQUEST_MASK); // MIU select (Group0)
        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF2), MIU_SC1_G1REQUEST_MASK, MIU_SC1_G1REQUEST_MASK); // MIU select (Group1)
        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF4), MIU_SC1_G2REQUEST_MASK, MIU_SC1_G2REQUEST_MASK); // MIU select (Group2)
        MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF6), MIU_SC1_G3REQUEST_MASK, MIU_SC1_G3REQUEST_MASK); // MIU select (Group3)
    }
}

void Hal_SC_get_miusel(void *pInstance, SC_MIUSEL_t* stMIUSel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, BIT(4)))
    {
        stMIUSel-> u8MainFBSel = 1;
    }
    else
    {
        stMIUSel-> u8MainFBSel = 0;
    }

    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, BIT(14)))
    {
        stMIUSel-> u8SubFBSel = 1;
    }
    else
    {
        stMIUSel-> u8SubFBSel = 0;
    }
}

void Hal_SC_set_blsk(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_21_L, bEn ? BIT(12) : 0, BIT(12));
}

void Hal_SC_set_blsk_burst(void *pInstance, MS_BOOL bEn)
{
    _MLOAD_ENTRY(pInstance);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_21_L, bEn ? BIT(12) : 0, BIT(12));
    MDrv_XC_MLoad_Fire(pInstance, TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_main_black_screen_burst(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _MLOAD_ENTRY(pInstance);

    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_20_L, bEn ? BIT(1) : 0, BIT(1));
    else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_19_L, bEn ? BIT(1) : 0, BIT(1));

    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_main_sub_black_screen_burst(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _MLOAD_ENTRY(pInstance);

    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_20_L, bEn ? (BIT(2)|BIT(1)) : 0, (BIT(2)|BIT(1)));
    else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_19_L, bEn ? (BIT(5)|BIT(1)) : 0, (BIT(6)|BIT(5)|BIT(1)));

    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_main_black_screen(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_20_L, bEn ? BIT(1) : 0, BIT(1));
    else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, bEn ? BIT(1) : 0, BIT(1));
}

#ifdef UFO_XC_SETBLACKVIDEOBYMODE
E_APIXC_ReturnValue Hal_SC_set_main_black_screen_by_mode_burst(void *pInstance, MS_BOOL bEn, EN_XC_BLACK_VIDEO_MODE eMode)
{
    MS_U16 u16MemSyncSetting = 0x1B00;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    if((gSrcInfo[MAIN_WINDOW].bFBL) // FBL mode
        || (BIT(3) == SC_R2BYTEMSK(0, REG_SC_BK12_04_L, BIT(3))) // RFBL mode
       ) // 4K2K bypass mode
    {
        u16MemSyncSetting &= ~(BIT(8)); // Disable HW Black Auto Sync
    }
        printf("Richard.Deng %s()@line %d: 0x%x bEn: %d.\n ", __func__, __LINE__, u16MemSyncSetting, bEn);

    if(bEn == FALSE)
    {
        if(eMode == E_XC_BLACK_VIDEO_MEMSYNC)
        {
            if (0 == SC_R2BYTEMSK(0, REG_SC_BK2F_20_L, BIT(1))) // Already umute skip hardware auto sync umute.
        {
                SC_DBG(printf("Skip triggle hardware auto sync umute\n"));
        }
        else
        {
            if(0 == SC_R2BYTEMSK(0, REG_SC_BK12_40_L, BIT(10)))
            {
                u16MemSyncSetting |= BIT(10);
                SC_W2BYTEMSK(0, REG_SC_BK12_40_L, u16MemSyncSetting, 0x1F00);
                u16MemSyncSetting &= ~(BIT(10));
            }
            }
        }
        else
        {
            u16MemSyncSetting &= ~(BIT(8)); // Disable HW Black Auto Sync in other modes
        }

            SC_W2BYTEMSK(0, REG_SC_BK12_40_L, u16MemSyncSetting, 0x1F00);
        }

    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_20_L, bEn ? BIT(1) : 0, BIT(1));

    if((eMode == E_XC_BLACK_VIDEO_MEMSYNC) && (bEn == TRUE))
    {
        u16MemSyncSetting |= BIT(10);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_40_L, u16MemSyncSetting, 0x1F00);
    }

    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    _MLOAD_RETURN(pInstance);

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_set_main_black_screen_by_mode(void *pInstance, MS_BOOL bEn, EN_XC_BLACK_VIDEO_MODE eMode)
{
    MS_U16 u16MemSyncSetting = 0x1B00;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    if((gSrcInfo[MAIN_WINDOW].bFBL) // FBL mode
        || (BIT(3) == SC_R2BYTEMSK(0, REG_SC_BK12_04_L, BIT(3))) // RFBL mode
       ) // 4K2K bypass mode
    {
        u16MemSyncSetting &= ~(BIT(8)); // Disable HW Black Auto Sync
    }
        SC_DBG(printf("Richard.Deng %s()@line %d: 0x%x bEn: %d.\n ", __func__, __LINE__, u16MemSyncSetting, bEn));

    if(bEn == FALSE)
    {
        if(eMode == E_XC_BLACK_VIDEO_MEMSYNC)
    {
            if (0 == SC_R2BYTEMSK(0, REG_SC_BK2F_20_L, BIT(1))) // Already umute skip hardware auto sync umute.
        {
            SC_DBG(printf("Skip triggle hardware auto sync umute.\n"));
        }
        else
        {
            if(0 == SC_R2BYTEMSK(0, REG_SC_BK12_40_L, BIT(10)))
            {
                u16MemSyncSetting |= BIT(10);
                SC_W2BYTEMSK(0, REG_SC_BK12_40_L, u16MemSyncSetting, 0x1F00);
                u16MemSyncSetting &= ~(BIT(10));
            }
        }
        }
        else
        {
            u16MemSyncSetting &= ~(BIT(8)); // Disable HW Black Auto Sync in other modes
        }

        SC_W2BYTEMSK(0, REG_SC_BK12_40_L, u16MemSyncSetting, 0x1F00);
    }
        SC_DBG(printf("Richard.Deng %s()@line %d: 0x%x bEn: %d.\n ", __func__, __LINE__, u16MemSyncSetting, bEn));

    MDrv_XC_wait_output_vsync(pInstance, 1, 20, MAIN_WINDOW);
    SC_W2BYTEMSK(0, REG_SC_BK2F_20_L, bEn ? BIT(1) : 0, BIT(1));

    if((eMode == E_XC_BLACK_VIDEO_MEMSYNC) && (bEn == TRUE))
    {
        u16MemSyncSetting |= BIT(10);
        SC_W2BYTEMSK(0, REG_SC_BK12_40_L, u16MemSyncSetting, 0x1F00);
    }
        SC_DBG(printf("Richard.Deng %s()@line %d: 0x%x bEn: %d.\n ", __func__, __LINE__, u16MemSyncSetting, bEn));

    return E_APIXC_RET_OK;
}
#endif

void Hal_SC_set_main_sub_black_screen(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_20_L, bEn ? (BIT(2)|BIT(1)) : 0, (BIT(2)|BIT(1)));
    else if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, bEn ? (BIT(5)|BIT(1)) : 0, (BIT(6)|BIT(5)|BIT(1)));

}

void Hal_SC_set_sub_blue_screen_burst(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue)
{
    // Not supporting blue no-signal color in sub window
    UNUSED(bBlue);

    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_20_L, bEn ? BIT(2) : 0, BIT(2));
    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_sub_blue_screen(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    // Not supporting blue no-signal color in sub window
    UNUSED(bBlue);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_20_L, bEn ? BIT(2) : 0, BIT(2));
}

#ifdef UFO_XC_SETBLACKVIDEOBYMODE
E_APIXC_ReturnValue Hal_SC_set_sub_blue_screen_by_mode_burst(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue, EN_XC_BLACK_VIDEO_MODE eMode)
{
    // Not supporting blue no-signal color in sub window
    UNUSED(bBlue);

    if(eMode == E_XC_BLACK_VIDEO_MEMSYNC)
    {
        SC_DBG(printf("Sub window not supporting memsync black video\n"));
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }

    _MLOAD_ENTRY(pInstance);

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_20_L, bEn ? BIT(2) : 0, BIT(2));
    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    _MLOAD_RETURN(pInstance);

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_set_sub_blue_screen_by_mode(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue, EN_XC_BLACK_VIDEO_MODE eMode)
{
    // Not supporting blue no-signal color in sub window
    UNUSED(bBlue);

    if(eMode == E_XC_BLACK_VIDEO_MEMSYNC)
    {
        SC_DBG(printf("Sub window not supporting memsync black video\n"));
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }

    MDrv_XC_wait_output_vsync(pInstance, 1, 20, SUB_WINDOW);
    SC_W2BYTEMSK(0, REG_SC_BK2F_20_L, bEn ? BIT(2) : 0, BIT(2));
    return E_APIXC_RET_OK;
}
#endif

E_APIXC_ReturnValue Hal_SC_IP_Memory_Write_Request(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(MAIN_WINDOW == eWindow)
    {
        if(bEnable)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0 , BIT(0));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(0) , BIT(0));
        }
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, bEnable? 0 : BIT(0) , BIT(0));
    }
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_IP_Memory_Read_Request(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(MAIN_WINDOW == eWindow)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, bEnable? 0 : BIT(1) , BIT(1));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, bEnable? 0 : BIT(1) , BIT(1));
    }
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue Hal_SC_OP_Memory_Write_Request(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_67_L, bEnable? 0 : BIT(1) , BIT(1));//This will stop OPW and OP memory write request
    return E_APIXC_RET_OK;
}

void Hal_SC_disable_inputsource_burst(void *pInstance, MS_BOOL bDisable, SCALER_WIN eWindow)
{
    _MLOAD_ENTRY(pInstance);

    // HW PIP architeucture
    // Becasue BK3_02[8] and BK20_11[15] can not be enabled toghter,
    // otherwise garbage will be showed,we need to use BK12_47[0] to instead.
    if( eWindow == MAIN_WINDOW )
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_02_L, (bDisable ? BIT(7):0), BIT(7));
    }
    else
    {
        //MDrv_XC_MLoad_WriteCmd(REG_SC_BK03_02_L, (bDisable ? BIT(7):0), BIT(7));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_47_L, (bDisable ? BIT(0) : 0), BIT(0));
    }
    MDrv_XC_MLoad_Fire(pInstance, TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_disable_inputsource(void *pInstance, MS_BOOL bDisable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, (bDisable ? BIT(7):0), BIT(7));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, (bDisable ? BIT(7):0), BIT(7));
    }
}

MS_U16 Hal_SC_Is_InputSource_Disable(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L)&0x80);
    else
        return (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L)&0x80);
}

void Hal_SC_set_nosignal_color(void *pInstance, MS_U8 u8Color,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        if ( eWindow == MAIN_WINDOW )
        {
            switch(u8Color)
            {
                case 0xFF:  //white
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_25_L, (0xE9<<8) | 0x80, HBMASK | LBMASK);
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_26_L, 0x80, LBMASK); //Cb:[7:0]
                    break;
                case 0x03:  //Blue
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_25_L, (0x1F<<8) | 0x76, HBMASK | LBMASK);
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_26_L, 0xEF, LBMASK);
                    break;
                case 0xE0:  //Red
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_25_L, (0x3E<<8) | 0xEF, HBMASK | LBMASK);
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_26_L, 0x67, LBMASK);
                    break;
                case 0x1C:  //Green
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_25_L, (0xAC<<8) | 0x1B, HBMASK | LBMASK);
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_26_L, 0x2A, LBMASK);
                    break;
                case 0x00:  //Black
                default :
                    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
                    {
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_25_L, (0x10<<8) | 0x80, HBMASK | LBMASK);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_26_L, 0x80, LBMASK);
                    }
                    else
                    {
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_24_L, 0x82, LBMASK);
                    }
                    break;
            }
        }
        else
        {
           // Clippers does not have sub window
        }
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
        {
            switch(u8Color)
            {
                case 0xFF:  //white
                    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2F_25_L, 0xE980);             //Cr:[7:0] Y:[15:8]
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_26_L, 0x80, LBMASK);    //Cb:[7:0]
                    }
                    else // SC1
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_24_L, 0x9E, LBMASK);
                    }
                    break;
                case 0x03:  //Blue
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2F_25_L, 0x1F76);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_26_L, 0xEF, LBMASK);
                    break;
                case 0xE0:  //Red
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2F_25_L, 0x3EEF);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_26_L, 0x67, LBMASK);
                    break;
                case 0x1C:  //Green
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2F_25_L, 0xAC1B);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_26_L, 0x2A, LBMASK);
                    break;
                case 0x00:  //Black
                default :
                    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2F_25_L, 0x1080);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_26_L, 0x80, LBMASK);
                    }
                    else
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_24_L, 0x82, LBMASK);
                    }
                    break;
            }
        }
        else
        {
            // Clippers does not have sub window
        }
    }
}

void Hal_SC_set_fbl(void *pInstance, MS_BOOL bEn)
{
    //[FIXME][BRINGUP]
    SC_DBG(printf("[Hal_SC_set_fbl] bEn %d\n",bEn));
    //Hal_SC_Set_RequestFBL_Mode(pInstance, bEn);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, bEn?BIT(14):0 , BIT(14)); // Disable DNR
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, bEn?BIT(11):0 , BIT(11)); // bypass

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
// If use CMA and do not get CMA address (or FBL),
// we must disable IPM write to avoid XC write to default address (0) and hit other module's buffer
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_SELF_MEM] != 0)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, (bEn ? 0x3:0), 0x3);
    }
#endif
#else
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, (bEn ? 0x3:0), 0x3); //F2 IP read/write request disable
#endif

}

MS_BOOL Hal_SC_get_fbl(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, BIT(11)) == 0x0)? FALSE : TRUE;  //Enable/Disable FBL
}

void Hal_SC_set_freezeimg_burst(void *pInstance, MS_BOOL bEn, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _MLOAD_ENTRY(pInstance);
    if( eWindow == MAIN_WINDOW )
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_01_L, (bEn<<11), BIT(11));
    }
    else
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_41_L, (bEn<<11), BIT(11));
    }
    MDrv_XC_MLoad_Fire(pInstance, TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_freezeimg(void *pInstance, MS_BOOL bEn, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, (bEn<<11), BIT(11));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, (bEn<<11), BIT(11));
    }
}

MS_BOOL Hal_SC_get_freezeimg(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, BIT(11)) ? TRUE: FALSE);
    }
    else
    {
        return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, BIT(11)) ? TRUE: FALSE);
    }
}

void Hal_SC_exwith_miuprotect_swreset(void *pInstance, MS_U8 x)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //reset IP1F2
    //select pattern generator source
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, x, x);

    //select pattern generator source
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_02_L, 0, x);
}

void Hal_SC_ip_Init_for_internal_timing(void *pInstance, XC_Internal_TimingType timingtype, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == SUB_WINDOW)
    {
        if(Hal_SC_ip_get_IPAutoNoSignal(pInstance, eWindow)) Hal_SC_ip_set_IPAutoNoSignal(pInstance, DISABLE, eWindow);

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, 0x104) ;  //Video source
        Hal_SC_exwith_miuprotect_swreset(pInstance, REST_IP_F1);  //when source is changed, we must reset it to make it work
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, 0x890);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, 0x01, 0x03);   //force progressive mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, BIT(15), BIT(15));   //auto gain 10bit

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L, 0x020);  //capture V start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L, 0x50) ;  //capture H start

        switch (timingtype)
        {
            case E_XC_480P:
            default:

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L, 0x1c0) ; // V size
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, 0x280, 0x3FFF);  // h size

                break;
            case E_XC_720P:
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L, 0x290) ; // V size (720 - 64) = 656 = 0x290
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, 0x460, 0x3FFF); // h size (1280 - 160) = 1120 = 0x460

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4E_L, 0x0800); //DNR Offset
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4F_L, 0x0460); //DNR Fetch
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, 0x0800); //OPM Offsest
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, 0x0460); //OPM Fetch

                break;
            case E_XC_1080P:
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L, 0x3F8) ; // V size (1080 - 64) = 1016 = 0x3F8
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, 0x6E0, 0x3FFF); // h size (1920 - 160) = 1760 = 0x6E0

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4E_L, 0x0800); //DNR Offset
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4F_L, 0x06E0); //DNR Fetch
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, 0x0800); //OPM Offsest
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, 0x06E0); //OPM Fetch
                break;
        }
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0x104) ;  //Video source
        Hal_SC_exwith_miuprotect_swreset(pInstance, REST_IP_F2);  //when source is changed, we must reset it to make it work
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0x890);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, 0x01, 0x03);   //force progressive mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, BIT(15), BIT(15));   //auto gain 10bit

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L, 0x020);  //capture V start
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L, 0x50) ;  //capture H start

        switch (timingtype)
        {
            case E_XC_480P:
            default:

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L, 0x1c0) ; // V size
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x280, 0x3FFF);  // h size

                break;
            case E_XC_720P:
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L, 0x290) ; // V size (720 - 64) = 656 = 0x290
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x460, 0x3FFF); // h size (1280 - 160) = 1120 = 0x460

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, 0x0800); //DNR Offset
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, 0x0460); //DNR Fetch
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, 0x0800); //OPM Offsest
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, 0x0460); //OPM Fetch

                break;
            case E_XC_1080P:
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L, 0x3F8) ; // V size (1080 - 64) = 1016 = 0x3F8
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x6E0, 0x3FFF); // h size (1920 - 160) = 1760 = 0x6E0

                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, 0x0800); //DNR Offset
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, 0x06E0); //DNR Fetch
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, 0x0800); //OPM Offsest
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, 0x06E0); //OPM Fetch
                break;
        }
    }
}

MS_U16 Hal_SC_rgb_average_info(void *pInstance, MS_U16 u16mask, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16value;

    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16mask, 0x0F);
        u16value=SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_11_L);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0F_L, u16mask, 0x0F);
        u16value=SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_11_L);
    }

    return u16value;
}


void Hal_SC_autogain_enable(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //auto gain enable
    if(eWindow == MAIN_WINDOW)
    {
        if(bEnable)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, 0x11, 0x11);
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, 0x00, 0x11);
    }
    else
    {
        if(bEnable)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, 0x11, 0x11);
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, 0x00, 0x11);
    }
}

MS_BOOL Hal_SC_autogain_status(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bvalue;
    if(eWindow == MAIN_WINDOW)
        bvalue = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, BIT(1));
    else
        bvalue = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, BIT(1));

    return bvalue;

}

void Hal_SC_set_mirror(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32NoSigReg, u32MirrorReg;

    if(eWindow == MAIN_WINDOW)
    {
        u32NoSigReg = REG_SC_BK01_02_L;
        u32MirrorReg = REG_SC_BK12_03_L;
    }
    else
    {
        u32NoSigReg = REG_SC_BK03_02_L;
        u32MirrorReg = REG_SC_BK12_43_L;

    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32NoSigReg, BIT(7), BIT(7));

    if( bEnable )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, (BIT(12)|BIT(13)), (BIT(12)|BIT(13)));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, 0x0, (BIT(12)|BIT(13)));
    }
}

MS_U16 Hal_SC_GetAVDStatus(void)
{
    return 0x0;
}

void Hal_SC_set_rep_window(void *pInstance, MS_BOOL bEnable,MS_U16 x,MS_U16 y,MS_U16 w,MS_U16 h,MS_U8 u8Color)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, bEnable?BIT(9):0x0, BIT(9));
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_42_L,x);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_43_L,x + w);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_44_L,y);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_45_L,y + h);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, (~u8Color)<<8, 0xFF00);
}

//--------------------------------------------------
//update display window registers with input window
//IN:
//   pdspwin: input window info
//--------------------------------------------------
void Hal_SC_set_disp_window(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);

        if(eWindow == MAIN_WINDOW)
        {
            //Display window
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_08_L, pdspwin->x,0x1FFF);                  // Display H start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_09_L, pdspwin->x+pdspwin->width-1,0x1FFF);   // Display H end
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0A_L, pdspwin->y,0x1FFF);                  // Display V start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0B_L, pdspwin->y+pdspwin->height-1,0x1FFF);  // Display V end
        }
        else
        {
            //Display window
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_07_L, pdspwin->x,0x1FFF);                  // Display H start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_08_L, pdspwin->x+pdspwin->width-1,0x1FFF);   // Display H end
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_09_L, pdspwin->y,0x1FFF);                  // Display V start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_0A_L, pdspwin->y+pdspwin->height-1,0x1FFF);  // Display V end
        }

        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
        {
            //Display window
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pdspwin->x);                  // Display H start
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pdspwin->x+pdspwin->width-1);   // Display H end
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pdspwin->y);                  // Display V start
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pdspwin->y+pdspwin->height-1);  // Display V end
        }
        else
        {
            //Display window
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pdspwin->x);                  // Display H start
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pdspwin->x+pdspwin->width-1);   // Display H end
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pdspwin->y);                  // Display V start
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pdspwin->y+pdspwin->height-1);  // Display V end
        }
    }
}

//--------------------------------------------------
//get display window registers setting
//OUT:
//   pdspwin: Pointer for ouput disp window register
//--------------------------------------------------
void Hal_SC_get_disp_window(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        //Display window
        pdspwin->x = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L);                   // Display H start
        pdspwin->width  = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L) - pdspwin->x + 1; // Display H end
        pdspwin->y = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L);                   // Display V start
        pdspwin->height = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L) - pdspwin->y + 1; // Display V end
    }
    else
    {
        //Display window
        pdspwin->x = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L);                   // Display H start
        pdspwin->width  = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L) - pdspwin->x + 1; // Display H end
        pdspwin->y = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L);                   // Display V start
        pdspwin->height = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L) - pdspwin->y + 1; // Display V end
    }
}

void Hal_SC_set_Fclk(void *pInstance, EN_SET_FCLK_CASE enCase)
{
    // For Clippers, we control fclk in Hal_XC_IsForcePrescaling

    if (enCase == EN_FCLK_FBL)
    {
        //MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_240MHZ, CKG_FCLK_MASK);
    }
    else
    {
        //MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_216MHZ, CKG_FCLK_MASK);
    }
}

void Hal_SC_get_framebuf_Info(void *pInstance, SC_FRAMEBUF_INFO_t *pFrameBufInfo, SCALER_WIN eWindow)
{
    MS_U8 u8Reg;

    if (!pFrameBufInfo) return;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    pFrameBufInfo->u32IPMBase0 = ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_09_L:REG_SC_BK12_49_L, 0x00FF)<<16) |
                                    (SC_R2BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_08_L:REG_SC_BK12_48_L))) * BYTE_PER_WORD;
    pFrameBufInfo->u32IPMBase1 = ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_0B_L:REG_SC_BK12_4B_L, 0x00FF)<<16) |
                                    (SC_R2BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_0A_L:REG_SC_BK12_4A_L))) * BYTE_PER_WORD;
    pFrameBufInfo->u32IPMBase2 = ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_0D_L:REG_SC_BK12_4D_L, 0x00FF)<<16) |
                                    (SC_R2BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_0C_L:REG_SC_BK12_4C_L))) * BYTE_PER_WORD;
    pFrameBufInfo->u16IPMOffset = SC_R2BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_0E_L:REG_SC_BK12_4E_L);
    pFrameBufInfo->u16IPMFetch = SC_R2BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_0F_L:REG_SC_BK12_4F_L);
    pFrameBufInfo->u16VLength = SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_18_L:REG_SC_BK12_58_L, 0x0FFF);
    pFrameBufInfo->bLinearAddrMode = SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_03_L:REG_SC_BK12_43_L, BIT(4));
    pFrameBufInfo->bYCSeparate = ( SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_02_L:REG_SC_BK12_42_L, BIT(10)) )?TRUE:FALSE;
    pFrameBufInfo->u32WriteLimitBase = SC_R4BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_1A_L:REG_SC_BK12_5A_L);
    pFrameBufInfo->u8BitPerPixel = 24;//default

    u8Reg = SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_01_L:REG_SC_BK12_41_L, 0x00FF);
    if (u8Reg & 0x20)
    {//444
        pFrameBufInfo->bMemFormat422 = FALSE;
        if (u8Reg & 0x10)
        { // 444 8BIT
            pFrameBufInfo->u8BitPerPixel = 24;
        }
        else
        { // 444 10BIT
            pFrameBufInfo->u8BitPerPixel = 32;
        }
        pFrameBufInfo->bInterlace = FALSE;
    }
    else
    {//422
        pFrameBufInfo->bMemFormat422 = TRUE;

        if((BIT(8)|BIT(9)) == SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_02_L:REG_SC_BK12_42_L, BIT(8)|BIT(9))) //user mode
        {
            MS_U8 u8MemFmt = SC_R2BYTE(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_02_L:REG_SC_BK12_42_L) & 0x0F;
            if((4 == u8MemFmt) || (5 == u8MemFmt) || (6 == u8MemFmt) || (7 == u8MemFmt))
            {
                pFrameBufInfo->u8BitPerPixel = 20;
            }
            else if((8 == u8MemFmt) || (9 == u8MemFmt) || (10 == u8MemFmt) || (11 == u8MemFmt))
            {
                pFrameBufInfo->u8BitPerPixel = 24;
            }
        }
        else
        {
            u8Reg = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_01_L:REG_SC_BK12_41_L, 0xFF00)) >> 8;

            switch(u8Reg & 0x77)
            {
                case 0x25:
                    pFrameBufInfo->u8BitPerPixel = 24;
                    pFrameBufInfo->bInterlace = TRUE;
                    break;
                case 0x24:
                    pFrameBufInfo->u8BitPerPixel = 20;
                    pFrameBufInfo->bInterlace = TRUE;
                    break;
                case 0x27:
                    pFrameBufInfo->u8BitPerPixel = 16;
                    pFrameBufInfo->bInterlace = TRUE;
                    break;
                case 0x21:
                    if(BIT(1) == SC_R2BYTEMSK(psXCInstPri->u32DeviceID,(eWindow == MAIN_WINDOW)?REG_SC_BK12_04_L:REG_SC_BK12_44_L, BIT(0)|BIT(1)))
                    {
                        pFrameBufInfo->u8BitPerPixel = 24;
                    }
                    else
                    {
                        pFrameBufInfo->u8BitPerPixel = 20;
                    }
                    pFrameBufInfo->bInterlace = FALSE;
                    break;
                case 0x20:
                    pFrameBufInfo->u8BitPerPixel = 16;
                    pFrameBufInfo->bInterlace = FALSE;
                    break;
                case 0x00:
                default:
                    pFrameBufInfo->u8BitPerPixel = 16;
                    pFrameBufInfo->bInterlace = FALSE;
                    break;
            }
        }
    }

    //if (REG_RR(REG_SC_SCMI(0x04)) & BIT7)
    pFrameBufInfo->u8FBNum = (MS_U8)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, (eWindow == MAIN_WINDOW)?REG_SC_BK12_19_L:REG_SC_BK12_59_L, 0x1F));
#if 0
    printf("base0=0x%x, base1=0x%x, base2=0x%x, offset=%u, 422=%u, i=%u, bpp=%u, fbnum=%u\n",
        (unsigned int)pFrameBufInfo->u32IPMBase0,
        (unsigned int)pFrameBufInfo->u32IPMBase1,
        (unsigned int)pFrameBufInfo->u32IPMBase2,
        pFrameBufInfo->u16IPMOffset,
        pFrameBufInfo->bMemFormat422,
        pFrameBufInfo->bInterlace,
        pFrameBufInfo->u8BitPerPixel,
        pFrameBufInfo->u8FBNum);
#endif
}
void Hal_SC_set_framebuf_Info(void *pInstance, SC_FRAMEBUF_INFO_t pFrameBufInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32IPMBase0, u32IPMBase1, u32IPMBase2;

    u32IPMBase0=pFrameBufInfo.u32IPMBase0/BYTE_PER_WORD;
    u32IPMBase1=pFrameBufInfo.u32IPMBase1/BYTE_PER_WORD;
    u32IPMBase2=pFrameBufInfo.u32IPMBase2/BYTE_PER_WORD;

    //Step 5.4, Memory Limit V : BK12_18[12] = 1, BK12_18[11:0] to the real V line number
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, (pFrameBufInfo.u16VLength|0x1000),0x1FFF);

//    u32WritelimitBase = ((u32IPMBase0 + u32MemSize) / BYTE_PER_WORD - 1) | 0x2000000;
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_1A_L, pFrameBufInfo.u32WriteLimitBase);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_09_L, (MS_U16)(u32IPMBase0 & 0xFF0000)>>16,0x00FF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L, (MS_U16)(u32IPMBase0 & 0xFFFF));

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0B_L, (MS_U16)(u32IPMBase1 & 0xFF0000)>>16,0x00FF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0A_L, (MS_U16)(u32IPMBase1 & 0xFFFF));

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0D_L, (MS_U16)(u32IPMBase2 & 0xFF0000)>>16,0x00FF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0C_L, (MS_U16)(u32IPMBase2 & 0xFFFF));


    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pFrameBufInfo.u16IPMOffset);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pFrameBufInfo.u16IPMFetch);
//    SC_W2BYTEMSK(REG_SC_BK12_18_L, (pFrameBufInfo.u16VLength|0x1000),0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, ((pFrameBufInfo.bLinearAddrMode)?BIT(4):0),BIT(4));

}

MS_U8 Hal_SC_get_cs_det_cnt(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8val;

    if(eWindow == MAIN_WINDOW)
    {
        u8val = ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0C_L) & 0xFF00) >> 8);
    }
    else
    {
        u8val = ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_0C_L) & 0xFF00) >> 8);
    }
    return u8val;
}

void Hal_SC_set_cs_det_cnt(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0C_L, (((MS_U16)u8val)<<8), 0xFF00);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0C_L, (((MS_U16)u8val)<<8), 0xFF00);
    }
}

MS_U8 Hal_SC_get_plus_width(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8val;

    if(eWindow == MAIN_WINDOW)
    {
        u8val = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_22_L) & 0x00FF);
    }
    else
    {
        u8val = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_22_L) & 0x00FF);
    }
    return u8val;
}

void Hal_SC_set_opm_fetch(void *pInstance, SCALER_WIN eWindow, MS_U16 u16OPMFetch)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, u16OPMFetch, 0x0FFF);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, u16OPMFetch, 0x0FFF);
    }
}

MS_U16 Hal_SC_get_opm_fetch(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16Val = 0;
    if(eWindow == MAIN_WINDOW)
    {
        u16Val = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L) & 0x0FFF;
    }
    else
    {
        u16Val = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L) & 0x0FFF;
    }
    return u16Val;
}

MS_U8 HAL_SC_get_dnr_setting(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L, BIT(1)|BIT(0));
    else
        return (MS_U8)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L, BIT(1)|BIT(0));
}

void HAL_SC_enable_dnr(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L, u8Val, BIT(1)|BIT(0));
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L, u8Val, BIT(1)|BIT(0));
}

void HAL_SC_enable_field_avg_y(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK05_21_L, bEnable ? BIT(6) : 0, BIT(6));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK05_01_L, bEnable ? BIT(6) : 0, BIT(6));
    }

}

void HAL_SC_enable_field_avg_c(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK05_21_L, bEnable ? BIT(7) : 0, BIT(7));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK05_01_L, bEnable ? BIT(7) : 0, BIT(7));
    }
}

void HAL_SC_VOP_Set_Contrast_En(void *pInstance, MS_BOOL bEenable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(0):0), BIT(0));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(8):0), BIT(8));
    }
}
void HAL_SC_VOP_Set_Contrast_Value(void *pInstance, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow)
{
//[FIXME][KANO] check Contrast_Value in bank 0x0f?
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_47_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_48_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_49_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_47_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_48_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_49_L, u16Val, 0x0FFF);
                break;
        }
    }
    else
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4A_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4B_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4C_L, u16Val, 0x0FFF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4A_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4B_L, u16Val, 0x0FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4C_L, u16Val, 0x0FFF);
                break;
        }
    }
}

void HAL_SC_VOP_Set_Brightness_En(void *pInstance, MS_BOOL bEenable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(1):0), BIT(1));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_53_L, (MS_U16)(bEenable?BIT(9):0), BIT(9));
    }
}
void HAL_SC_VOP_Set_Brightness_Value(void *pInstance, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow)
{
//[FIXME][KANO] check Brightness_Value in bank 0x0f?
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4D_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4E_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4F_L, u16Val, 0x07FF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4D_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4E_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_4F_L, u16Val, 0x07FF);
                break;
        }
    }
    else
    {
        switch(eVop_Channel)
        {
            default:
            case XC_VOP_CHR:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_50_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHG:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_51_L, u16Val, 0x07FF);
                break;
            case XC_VOP_CHB:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_52_L, u16Val, 0x07FF);
                break;
            case XC_VOP_ALL:
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_50_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_51_L, u16Val, 0x07FF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_52_L, u16Val, 0x07FF);
                break;
        }
    }
}

void HAL_SC_Set_FB_Num(void *pInstance, SCALER_WIN eWindow, XC_FRAME_STORE_NUMBER enFBType, MS_BOOL bInterlace)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8FrameCount = 2;

    switch( enFBType )
    {
        case IMAGE_STORE_2_FRAMES:
            u8FrameCount = 2;
            break;
        case IMAGE_STORE_3_FRAMES:
            u8FrameCount = 3;
            break;
        case IMAGE_STORE_4_FRAMES:
            u8FrameCount = 4;
            break;
        case IMAGE_STORE_6_FRAMES:
            u8FrameCount = 6;
            break;
        case IMAGE_STORE_8_FRAMES:
            u8FrameCount = 8;
            break;
        case IMAGE_STORE_12_FRAMES:
            u8FrameCount = 12;
            break;
        default:
            break;
    }

    if (!bInterlace)
    {
        if (eWindow == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L, u8FrameCount , BITMASK(4:0) );
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_59_L, u8FrameCount , BITMASK(4:0) );
        }
    }

}

void HAL_SC_Enable_SUBONEMode(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L, (MS_U16)(bEnable?BIT(9):0), BIT(9));
    // no sub
}

void HAL_SC_Enable_VInitFactor(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, (MS_U16)(bEnable?BIT(0):0), BIT(0));
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_33_L, (MS_U16)(bEnable?BIT(0):0), BIT(0));
}

void HAL_SC_Set_VInitFactorOne(void *pInstance, MS_U32 u32Value,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_03_L, u32Value);
    else
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_23_L, u32Value);
}

void HAL_SC_Set_VInitFactorTwo(void *pInstance, MS_U32 u32Value,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_05_L, u32Value);
    else
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_25_L, u32Value);
}

void HAL_SC_Set_vsd_output_line_count(void *pInstance, MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16OutputLineCount = 0x00;

    if (bEnable)
    {
        u16OutputLineCount = BIT(15);
        u16OutputLineCount |= (MS_U16)(u32LineCount & 0x1FFF);
    }

    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_37_L, u16OutputLineCount);
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_37_L, u16OutputLineCount);
    }

}

/*
 bEnable : Enable input line count.
 bUserMode : 1 -> the input line count will ref u32UserLineCount
                   0 -> the input line count will ref V capture win
*/
void HAL_SC_Set_vsd_input_line_count(void *pInstance, MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16InputLineCount = 0x00;

    if (bEnable)
    {
        u16InputLineCount = BIT(15);

        if (bUserMode)
        {
            //u16InputLineCount |= BIT(11);
            u16InputLineCount |= (MS_U16)(u32UserLineCount & 0x1FFF);
        }
    }

    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_36_L, u16InputLineCount);
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_36_L, u16InputLineCount);
    }

}

MS_U32 Hal_SC_CheckSubWinPreScaling(MS_U16 u16ScaleDst,MS_BOOL bInterlace)
{
    UNUSED(u16ScaleDst);
    UNUSED(bInterlace);
    //New chip sub window unnecessary to the same with the main window's pre scaling setting.
    return 0;
}

//Add for T3
void HAL_SC_FilmMode_Patch1(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16Value;
    MS_U16 u16RegValue;

    //static MS_U8 u8Time_count = 0;

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16RegValue = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_7D_L, 0xFF);
    if(pXCResourcePrivate->sthal_SC.g_bCntFlg)
        pXCResourcePrivate->sthal_SC.g_u16TmpValue = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_02_L, 0xFF);

    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_21_L, BIT(6)))
    {
        //Counter
        if(pXCResourcePrivate->sthal_SC.u8Time_count != 0xFF)
            pXCResourcePrivate->sthal_SC.u8Time_count ++;

        if(pXCResourcePrivate->sthal_SC.u8Time_count < u16RegValue)  // fake out duration
           u16Value = 0x06;      // madi fake out enable
        else
           u16Value = 0x00;      // madi fake out disable

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_25_L, (0x10<<8), 0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_02_L, 0x22, 0xFF);
        pXCResourcePrivate->sthal_SC.g_bCntFlg = 0;
    }
    else
    {
        pXCResourcePrivate->sthal_SC.u8Time_count = 0;
        u16Value = 0x00;      // madi fake out disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_25_L, (0x08<<8), 0xFF00);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_02_L, pXCResourcePrivate->sthal_SC.g_u16TmpValue, 0xFF);
        pXCResourcePrivate->sthal_SC.g_bCntFlg = 1;
    }
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0A_21_L, u16Value, BIT(2)|BIT(1));

}

void HAL_SC_EnableFPLL(void)
{
    // at first disable i_gain and then enable FPLL; or
    // disable i_gain and enable FPLL at the same time
    MDrv_WriteByteMask(L_BK_LPLL(0x0C), BIT(6)|BIT(3), BIT(6)|BIT(3));
}

#ifdef MSOS_TYPE_LINUX_KERNEL

MS_BOOL HAL_SC_WaitFPLLDone(void *pInstance)
{
    MS_U8 u8TimeoutCnt = 64;
    MS_U16 u16PhaseDiff;
    MS_U16 u16PhaseLimit;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(!gSrcInfo[MAIN_WINDOW].bEnableFPLL)
    {
        // no need to wait
        return FALSE;
    }

    u16PhaseLimit = MDrv_Read2Byte(L_BK_LPLL(0x0A));

    u16PhaseDiff = u16PhaseLimit;

    while (u8TimeoutCnt)
    {
        u16PhaseDiff = MDrv_Read2Byte(L_BK_LPLL(0x11));

        FPLL_DBG(printf("1. (%u) phasediff=0x%4x, phaselimit=0x%4x, h0021=0x%08x, h0028=0x%08x\n",
            u8TimeoutCnt, u16PhaseDiff, u16PhaseLimit,
            (unsigned int)MDrv_Read4Byte(L_BK_LPLL(0x21)),
            (unsigned int)MDrv_Read4Byte(L_BK_LPLL(0x28))
            ));

        if (u16PhaseDiff < u16PhaseLimit/2)
        {
            FPLL_DBG(printf("Exit check phase diff1\n"));
            break;
        }

        u8TimeoutCnt--;
        MDrv_XC_wait_output_vsync(pInstance, 1, 50, MAIN_WINDOW);
    }

    MDrv_WriteRegBit(L_BK_LPLL(0x0C), 0, BIT(6));  //enable i_gain for lock phase

    while (u8TimeoutCnt)
    {
        MDrv_XC_wait_output_vsync(pInstance, 1, 50, MAIN_WINDOW);

        u16PhaseDiff = MDrv_Read2Byte(L_BK_LPLL(0x11));

        FPLL_DBG(printf("2. (%u) phasediff=0x%4x, phaselimit=0x%4x, h0021=0x%08x, h0028=0x%08x\n",
            u8TimeoutCnt, u16PhaseDiff, u16PhaseLimit,
            (unsigned int)MDrv_Read4Byte(L_BK_LPLL(0x21)),
            (unsigned int)MDrv_Read4Byte(L_BK_LPLL(0x28))
            ));

        if (u16PhaseDiff < 0x200)
        {
            FPLL_DBG(printf("Exit check phase diff2\n"));
            break;
        }
        u8TimeoutCnt--;
    }

    return (u8TimeoutCnt == 0 ? FALSE : TRUE);
}
#else
static void _HAL_SC_GetFPLLPhaseDiffISR(SC_INT_SRC eIntNum, void *pParam)
{
    volatile MS_XC_GET_FPLL_PHASEDIFF * pFpllPhaseDiff = (volatile MS_XC_GET_FPLL_PHASEDIFF *) pParam;

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    pFpllPhaseDiff->u16PhaseDiff = MDrv_Read2Byte(L_BK_LPLL(0x11));
    pFpllPhaseDiff->eFpllDir = (MDrv_Read2Byte(L_BK_LPLL(0x12)) & BIT(0)) == BIT(0) ? E_XC_FPLL_DIR_UP : E_XC_FPLL_DIR_DOWN;
    pFpllPhaseDiff->u8Debounce++;
    LPLL_BK_RESTORE;
    if(pFpllPhaseDiff->u8Debounce > 3)
    {
        if(pFpllPhaseDiff->u16PhaseDiff < 0x200)
        {
            pFpllPhaseDiff->eFpllResult = E_XC_FPLL_RES_FINISHED;
        }
        else
        {
            if(pFpllPhaseDiff->u8Debounce > 90)
            {
                pFpllPhaseDiff->eFpllResult = E_XC_FPLL_RES_TIMEOUT;
            }
        }
    }
}

MS_BOOL HAL_SC_WaitFPLLDone(void *pInstance)
{
    MS_XC_GET_FPLL_PHASEDIFF stGetFpllPhaseDiff = {0, 0, E_XC_FPLL_DIR_UNKNOWN, E_XC_FPLL_RES_WAITING};

    MDrv_XC_InterruptAttach(pInstance, SC_INT_VSINT, _HAL_SC_GetFPLLPhaseDiffISR, (void *) &stGetFpllPhaseDiff);
    while(*(volatile MS_BOOL*)(&(stGetFpllPhaseDiff.eFpllResult)) == E_XC_FPLL_RES_WAITING);
    MDrv_XC_InterruptDeAttach(pInstance, SC_INT_VSINT, _HAL_SC_GetFPLLPhaseDiffISR, (void *) &stGetFpllPhaseDiff);

    if(stGetFpllPhaseDiff.eFpllResult == E_XC_FPLL_RES_FINISHED)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif


MS_U16 HAL_SC_GetOutputVFreqX100(MS_U32 u32XTAL_Clock)
{
    MS_U16 u16Value=0;
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u16Value = ((MS_U16) ((u32XTAL_Clock * 100) / MDrv_Read4Byte(L_BK_LPLL(0x23))));
    LPLL_BK_RESTORE;
    return u16Value;

}

//Line buffer offset between Main and Sub, MAX(Main+Sub) = 1920+960
//Main Win Line buffer offset , start from max size of main's
void HAL_SC_SetMainLineBufferOffset(void *pInstance, MS_U16 u16Linebuffer)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_12_L, u16Linebuffer);
}

//Line buffer offset between Main and Sub, MAX(Main+Sub) = 1920+960
//Sub Win Line buffer offset , start from max size of main's
void HAL_SC_SetSubLineBufferOffset(void *pInstance, MS_U16 u16Linebuffer)
{
    // Clippers DI line buffer does not have 3840px
    // Therefore in 3D FP output, sub window share line buffer with main window
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    UNUSED(u16Linebuffer);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_13_L, u16Linebuffer);
    SC_DBG(printf("[HAL_SC_SetSubLineBufferOffset] u16Linebuffer 0x%x\n",u16Linebuffer));
    //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_13_L, u16Linebuffer);
}

//Select the trig mode
//0: Line base(Line Buffer Mode)
//1: Fill line buffer(Ring Buffer Mode)
void HAL_SC_SetDisplay_LineBuffer_Mode(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1F_L, (bEnable ? BIT(4) : 0), BIT(4));
}

//Select the start mode
//0: start at advance 1 display line
//1: start at faling edge of Vsync_init
void HAL_SC_SetDisplay_Start_Mode(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1F_L, (bEnable ? BIT(5) : 0), BIT(5));
}

//bk20_10[10]: Fill the main window's line buffer in vertical blanking for pip left and up corner
//bk20_10[11]: Fill the sub window's line buffer in vertical blanking for pip left and down corner
void HAL_SC_FillLineBuffer(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(bEnable)
    {
        if(eWindow == MAIN_WINDOW)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(10), BIT(10));
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(11), BIT(11));
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0, BIT(10));
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0, BIT(11));
    }
}

void Hal_SC_enable_window(void *pInstance, MS_BOOL bEn,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, bEn ? BIT(0) : 0, BIT(0));
    }
    else
    {
        // Sub prefill line must be enable, when PIP on (enable input source of sub win)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(11) , BIT(11));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, bEn ? BIT(1) : 0, BIT(1));
        // RFBL/FBL + PIP need enable BK20_2D[8]
        // HW says FB + PIP set BK20_2D[8] is OK, so we always enable BK20_2D[8] when PIP enable
        if(bEn)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_2D_L, 0x10D, 0x11F); //enable bit8
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_01_L, 0x8, 0x8); //enable bit3
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_2D_L, 0x00c, 0x11F); //disable bit8
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_01_L, 0x0, 0x8); //enable bit3
        }
    }
}

void Hal_SC_enable_window_burst(void *pInstance, MS_BOOL bEn,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _MLOAD_ENTRY(pInstance);
    if (eWindow == MAIN_WINDOW)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, bEn ? BIT(0) : 0, BIT(0));
    }
    else
    {
        // Sub prefill line must be enable, when PIP on (enable input source of sub win)
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, BIT(11), BIT(11) );
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, bEn ? BIT(1) : 0, BIT(1));
        // RFBL/FBL + PIP need enable BK20_2D[8]
        // HW says FB + PIP set BK20_2D[8] is OK, so we always enable BK20_2D[8] when PIP enable
        if(bEn)
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_2D_L, 0x10D, 0x11F); //enable bit8
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_01_L, 0x8, 0x8); //enable bit3
        }
        else
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_2D_L, 0x00c, 0x11F); //disable bit8
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_01_L, 0x0, 0x8); //enable bit3
        }
    }

    MDrv_XC_MLoad_Fire(pInstance, TRUE);
    _MLOAD_RETURN(pInstance);
}

void Hal_SC_set_trigger_signal(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_19_L, 0x30F8); // trigger mux
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_21_L, 0x0011); // ds trig delay line
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_22_L, 0x0011); // mload trig delay line
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1A_L, 0x0013); // atp delay line
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1B_L, 0x0015); // disp delay line
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_5D_L, 0x1218); // [15:8] mvop ref sync [7:0] mvop start disp
}

void Hal_SC_Set_OSD2VE(void *pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    switch(eVOPSelOSD_MUX)
    {
        case E_VOP_SEL_OSD_BLEND0:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x0001, 0x0001);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0000, 0x00F0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x0800, 0x0800);
            break;

        case E_VOP_SEL_OSD_BLEND1:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x0000, 0x0001);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0010, 0x00F0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x0800, 0x0800);
            break;

        case E_VOP_SEL_OSD_BLEND2:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x0000, 0x0001);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0020, 0x00F0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x0800, 0x0800);
            break;

        case E_VOP_SEL_MACE_RGB:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x0000, 0x0001);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0000, 0x00F0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x0800, 0x0800);
            break;

        case E_VOP_SEL_OSD_NONE:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x0000, 0x0001);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0000, 0x00F0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x0000, 0x0800);
            break;

        case E_VOP_SEL_OSD_LAST:
        default:
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x0000, 0x0001);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0060, 0x00F0);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x0800, 0x0800);
            break;
    }
}

MS_BOOL Hal_SC_IsOPMFetchPatch_Enable(void)
{
    return FALSE;
}

MS_BOOL Hal_SC_Check_HNonLinearScaling(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16Offset = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_15_L)>>8);
    MS_U16 u16MaxOffset = ((SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_07_L) & 0xFFFFFF)>>12);
    //Non-Linear scaling Postive offset, the maximum offset is HPstRatio[23:12]
    if(((u16Offset & BIT(7)) == 0) && (u16Offset > u16MaxOffset))
    {
        return FALSE;
    }
    return TRUE;
}

//IP2 Pre-Filter enable
void Hal_SC_IP2_PreFilter_Enable(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_48_L, (bEnable?BIT(15):0), BIT(15));
}

void Hal_SC_Set_extra_fetch_line(void *pInstance, MS_U8 u8val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_17_L, ((MS_U16)(u8val & 0x0F))<<8, 0x0F00);
}

void Hal_SC_Set_extra_adv_line(void *pInstance, MS_U8 u8val)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_17_L, ((MS_U16)(u8val & 0x0F))<<12, 0xF000);
}

void HAL_SC_Set_FPLL_Limit(void *pInstance, MS_U32 *u32PllSet, MS_U32 u32LowBound, MS_U32 u32UpBound, MS_BOOL _bInFPLLDbgMode, MS_U32 _U32LimitD5D6D7)
{
    MS_U32 u32LpllLimitLow, u32LpllLimitHigh;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //printf("Fpll_Limit: Set %lx, bound (%lx -> %lx)\n", *u32PllSet, u32UpBound, u32LowBound);
    LPLL_BK_SWITCH(0x00);
    if(_bInFPLLDbgMode)
    {
        MDrv_WriteByteMask(L_BK_LPLL(0x0D), 0x00, BIT(4));          // turn off 2 limit
        if(!pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug)
        {
            MDrv_Write3Byte(L_BK_LPLL(0x06), _U32LimitD5D6D7);
        }
    }
    else
    {
        MDrv_WriteByteMask(L_BK_LPLL(0x0D), BIT(4), BIT(4));        // turn on 2 limit

        // check if u32PllSet out of range and also the FPLL limit
        if (*u32PllSet <= u32LowBound)
        {
            *u32PllSet = u32LowBound;
            gSrcInfo[MAIN_WINDOW].bEnableFPLL = FALSE;
        }
        else if (*u32PllSet >= u32UpBound)
        {
            *u32PllSet = u32UpBound;
            gSrcInfo[MAIN_WINDOW].bEnableFPLL = FALSE;
        }

        u32LpllLimitHigh = u32UpBound - *u32PllSet;
        u32LpllLimitLow = *u32PllSet - u32LowBound - 1;

        //printf("Fpll set %lx, limit (high %lx, low %lx)\n", *u32PllSet, u32LpllLimitHigh, u32LpllLimitLow);

        if(!pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug)
        {
            MDrv_Write3Byte(L_BK_LPLL(0x06), u32LpllLimitHigh);
            MDrv_Write3Byte(L_BK_LPLL(0x08), u32LpllLimitLow);
        }

        //printf("Fpll limit reg (high %lx, low %lx)\n", MDrv_Read4Byte(L_BK_LPLL(0x06)), MDrv_Read4Byte(L_BK_LPLL(0x08)));
    }
}

void Hal_XC_H3D_Enable(void *pInstance, MS_BOOL bEn)
{
}

void Hal_XC_H3D_Input3DType(void *pInstance, E_XC_3D_INPUT_MODE e3DType)
{
}

void Hal_XC_H3D_Breakline_Enable(void *pInstance, MS_BOOL bEn)
{
}

void Hal_XC_H3D_HDE(void *pInstance, MS_U16 u16Hde)
{
}

void Hal_XC_H3D_LR_Toggle_Enable(void *pInstance, MS_BOOL bEn, MS_BOOL b2DTo3D, MS_BOOL bSkipDefaultLRFlag)
{
}

void HAL_XC_H3D_OPM_SBYS_PIP_Enable(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_34_L, (bEn<<7), (BIT(7)));
}

void Hal_SC_enable_cursor_report(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(bEn)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, BIT(8) | BIT(10), BIT(8) | BIT(10));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, 0x00 , BIT(8) | BIT(10));
    }
}

MS_BOOL Hal_SC_get_pixel_rgb(void *pInstance, XC_Get_Pixel_RGB *pData)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bret = TRUE;


    //printf("(%x %x) \n", pData->u16x, pData->u16y);
    switch(pData->enStage)
    {
    case E_XC_GET_PIXEL_STAGE_AFTER_DLC:
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x00 , BMASK(3:0));
        break;
    case E_XC_GET_PIXEL_STAGE_PRE_GAMMA:
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x08, BMASK(3:0));
        break;
    default:
        bret = FALSE;
        break;
    }
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(bret)
    {
        // Enable
        if(!pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear)
        {
            Hal_SC_enable_cursor_report(pInstance, TRUE);
        }

        // Initial position (x,y)  Set point you want to read.
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_19_L, pData->u16x);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_1A_L, pData->u16y);

        //SW trigger for reading
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_18_L, BIT(8), BIT(8));
        MsOS_DelayTask(5);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_18_L, 0, BIT(8));
        MsOS_DelayTask(32);  // Must wait .

        pData->u32r = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_2D_L);
        pData->u32g = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_2E_L);
        pData->u32b = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_2F_L);

        // Disable
        if(!pXCResourcePrivate->stdrvXC_MVideo.s_bKeepPixelPointerAppear)
        {
            Hal_SC_enable_cursor_report(pInstance, FALSE);
        }
    }
    return bret;
}

void Hal_SC_SetOPWriteOff(void *pInstance, MS_BOOL bEna)
{
    UNUSED(bEna);
}

MS_BOOL Hal_SC_GetOPWriteOff(void *pInstance)
{
    return TRUE;
}

MS_BOOL Hal_SC_Detect_RequestFBL_Mode(void *pInstance)
{
    UNUSED(pInstance);
    return ENABLE_REQUEST_FBL;
}

MS_BOOL Hal_SC_Set_RequestFBL_Mode(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (psXCInstPri->u32DeviceID == 0)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, bEn?BIT(10):0 , BIT(10)); // Enable RFBL
        SC_DBG(printf("[Hal_SC_Set_RequestFBL_Mode] bEn %d\n",bEn));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, bEn?BIT(3):0 , BIT(3)); // Enable bypass_DI
    }

    return ENABLE_REQUEST_FBL;
}

MS_U32 Hal_XC_ConvertARGB2YUV(void *pInstance, MS_U32 u32aRGB)
{
    MS_S16 s16R, s16G, s16B, s16Y, s16U, s16V;
    MS_U32 u32YUV = 0;

    s16R = (u32aRGB>>16) & 0xFF;
    s16G = (u32aRGB>>8) & 0xFF;
    s16B = (u32aRGB>>0) & 0xFF;

    //////////////////////////////////////////////////
    //RGB to YUV Formula (Limit 709)                //
    //Range: RGB[0-255] -> Y[16-235] UV[16-240]     //
    //Y = R * .183 + G * .614 + B * .062 + 16;      //
    //U = R * -.101 + G * -.339 + B * .439 + 128.;  //
    //V = R * .439 + G * -.399 + B * -.040 + 128.;  //
    //////////////////////////////////////////////////

    s16Y = (s16R*183/1000) + (s16G*614/1000) + (s16B*62/1000) + 16;
    s16U = -(s16R*101/1000) - (s16G*339/1000) + (s16B*439/1000) + 128;
    s16V = (s16R*439/1000) - (s16G*399/1000) - (s16B*40/1000) + 128;

// s16Y, s16U and s16V will cannot be > 255, below condition is dead error condition
#if 0
    if(s16Y > 0xFF)
    {
        s16Y = 0xFF;
    }
    if(s16U > 0xFF)
    {
        s16U = 0xFF;
    }
    if(s16V > 0xFF)
    {
        s16V = 0xFF;
    }
#endif

    u32YUV = (s16Y<<16) + (s16U<<8) + (s16V<<0);

    SC_DBG(printf("Convert ARGB 0x08%X to u32YUV 0x%08X \n", u32aRGB, u32YUV));

    return u32YUV;

}

void Hal_XC_SetFrameColor(void *pInstance, MS_U32 u32aRGB)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32YUV = Hal_XC_ConvertARGB2YUV(pInstance, u32aRGB);

    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_23_L,  (u32YUV >> 8),0xFF00); //Y
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_24_L,  (u32YUV >> 8),0x00FF); //U
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_23_L,  (u32YUV >> 0),0x00FF); //V
    }
    else // SC1
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L,  (u32YUV << 8),0xFF00); ///< V
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1A_L,  (u32YUV >> 16),0x00FF); ///< Y
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1A_L,  (u32YUV >> 0),0xFF00); ///< U
    }
}

MS_U8 Hal_SC_getVSyncWidth(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8val = 0;

    if(eWindow == MAIN_WINDOW)
    {
        u8val = ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_22_L) & 0xFF00) >> 8);
    }
    else
    {
    }
    return u8val;
}

E_APIXC_ReturnValue Hal_SC_Set_OSDLayer(void *pInstance, E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        GOP_W2BYTEMSK(_PK_L_(0x0,0x6B), 1<<13, 1<<13);         //double buffer
        GOP_W2BYTEMSK(_PK_L_(0x0,0x6A), eVOPOSDLayer, 0x0007);
        GOP_W2BYTEMSK(_PK_L_(0x0,0x6B), 1<<15, 1<<15);
        return E_APIXC_RET_OK;
    }

    return E_APIXC_RET_FAIL;
}

E_VOP_OSD_LAYER_SEL Hal_SC_Get_OSDLayer(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        return (E_VOP_OSD_LAYER_SEL)GOP_R2BYTEMSK(_PK_L_(0x0,0x6A), 0x0007);
    }
    else
    {
        return E_VOP_LAYER_RESERVED;
    }
}

E_APIXC_ReturnValue Hal_SC_Set_VideoAlpha(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16PostVal = 0;
    MS_U16 u16PreVal = u8Val;

    u16PostVal = (u16PreVal * (BIT(6) -1)) / (BIT(8)-1);

    if(eWindow == MAIN_WINDOW)
    {
        //Video Alpha is Enable if OSD Layer != E_VOP_LAYER_VIDEO_MUX1_MUX2
        if(Hal_SC_Get_OSDLayer(pInstance, eWindow) == E_VOP_LAYER_VIDEO_MUX1_MUX2)
        {
            Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2, eWindow);
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_36_L, u16PostVal<<10, 0xFC00);
        return E_APIXC_RET_OK;
    }
    return E_APIXC_RET_FAIL;
}

E_APIXC_ReturnValue Hal_SC_Get_VideoAlpha(void *pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16PostVal = 0;
    MS_U16 u16PreVal = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_36_L, 0xFC00) >> 10);

    u16PostVal = (u16PreVal * (BIT(8) -1)) / (BIT(6)-1);

    if(eWindow == MAIN_WINDOW)
    {

        *pu8Val = (MS_U8)u16PostVal;
        return E_APIXC_RET_OK;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
}

E_APIXC_ReturnValue Hal_SC_SetOSDBlendingFormula(void *pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue bRet = E_APIXC_RET_OK;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (eWindow == MAIN_WINDOW)
    {
        switch(enOsdIndex)
        {
            case E_XC_OSD_0:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_37_L, BIT(6)|BIT(5), BIT(6)|BIT(5));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_37_L, 0x00, BIT(6)|BIT(5));
                }
                break;

            case E_XC_OSD_1:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_37_L, BIT(14)|BIT(13), BIT(14)|BIT(13));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_37_L, 0x00, BIT(14)|BIT(13));
                }
                break;

            case E_XC_OSD_2:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_38_L, BIT(6)|BIT(5), BIT(6)|BIT(5));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_38_L, 0x00, BIT(6)|BIT(5));
                }
                break;

            case E_XC_OSD_3:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_38_L, BIT(14)|BIT(13), BIT(14)|BIT(13));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_38_L, 0x00, BIT(14)|BIT(13));
                }
                break;

            default:
                SC_DBG(printf("[MDrv_SC_SetOSDBlendingFormula]Invalid Parameter!enOsdIndex=%u\n", enOsdIndex));
                bRet = E_APIXC_RET_FAIL_INVALID_PARAMETER;
                break;
        }
    }
    else
    {
        // do nothing
        SC_DBG(printf("[MDrv_SC_SetOSDBlendingFormula] Not support sub window now \n"));
        bRet = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }

    return bRet;
}

void Hal_XC_ClearScalingFactorForInternalCalib(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //MDrv_WriteRegBit(H_BK_SCMI(0x09), FALSE, BIT(7));
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L, 0x0000);   // H pre-scaling
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, 0xC010);   // H pre-scaling
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L, 0x0000);   // V pre-scaling
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, 0x0000);   // V pre-scaling

    //HVSP
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_07_L, 0x0000);  // H post-scaling
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L, 0x0100);  // H post-scaling
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L, 0x0000);  // V post-scaling
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L, 0x0100);  // V post-scaling
}

MS_U8 MHal_SC_cal_usedgain_to_reggain(MS_U32 u16UsedGain_x32)
{
    MS_U8 u8Value = 0;

    if      (u16UsedGain_x32 >=65536) u8Value = 16;
    else if (u16UsedGain_x32 >=32768) u8Value = 15;
    else if (u16UsedGain_x32 >=16384) u8Value = 14;
    else if (u16UsedGain_x32 >= 8192) u8Value = 13;
    else if (u16UsedGain_x32 >= 4096) u8Value = 12;
    else if (u16UsedGain_x32 >= 2048) u8Value = 11;
    else if (u16UsedGain_x32 >= 1024) u8Value = 10;
    else if (u16UsedGain_x32 >=  512) u8Value = 9;
    else if (u16UsedGain_x32 >=  256) u8Value = 8;
    else if (u16UsedGain_x32 >=  128) u8Value = 7;
    else if (u16UsedGain_x32 >=   64) u8Value = 6;
    else if (u16UsedGain_x32 >=   32) u8Value = 5;
    else if (u16UsedGain_x32 >=   16) u8Value = 4;
    else if (u16UsedGain_x32 >=    8) u8Value = 3;
    else if (u16UsedGain_x32 >=    4) u8Value = 2;
    else if (u16UsedGain_x32 >=    2) u8Value = 1;
    else if (u16UsedGain_x32 >=    1) u8Value = 0;
    else {
        u8Value = 0;
    }


    if  (u16UsedGain_x32 >=1  )
    {
        u8Value += IPGAIN_REFACTOR ;
    }
    else
    {
        u8Value += IPGAIN_REFACTOR -1;
    }

    return u8Value;
}

E_APIXC_ReturnValue Hal_SC_OP2VOPDESel(void *pInstance, E_OP2VOP_DE_SEL eVopDESel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //Select op2vop de, for DWIN capture etc
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_6B_L, (eVopDESel << 12), (BIT(12)+BIT(13)));
    return E_APIXC_RET_OK;
}

MS_BOOL Hal_XC_SVOutput_GetCaps(void *pInstance)
{
    return FALSE;
}

#if 0
#define XC_POSTSCALING_BUBBLE                   (192)
#define XC_POSTSCALING_BUBBLE_INTERLACE         (100)
#define XC_FCLK                                 (192)

static MS_U16 _Hal_XC_GetODCLK(void)
{
    MS_U16 u16Odclk;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    //if(g_XC_InitData.stPanelInfo.u16HTotal < 900)
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID) // SC0
    {
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal < 900)
        {
            //480I/P, 576I/P (ODCLK 27Mhz)
            u16Odclk = 28;
        }
        //else if(g_XC_InitData.stPanelInfo.u16HTotal < 2000)
        else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal < 2000)
        {
            //720P50, 720P60 (ODCLK 74.25Mhz)
            u16Odclk = 75;
        }
        //else if((g_XC_InitData.stPanelInfo.u16HTotal > 2000) && (g_XC_InitData.stPanelInfo.u16DefaultVFreq <= 400 ))
        else if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal >2000)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq <=400))
        {
            //1080p(24/25/30Hz) (ODCLK 74.25Mhz)
            u16Odclk = 75;
        }
        else
        {
            //1080I/P50, 1080I/P60 (ODCLK 148.5MHZ)
            u16Odclk = 149;
        }
    }
    else
    {
        // SC1 ODCLK is fixed at 13.5MHz
        u16Odclk = 13;
    }
    return u16Odclk;
}

static MS_U16 _Hal_XC_FloorDiv(MS_U16 u16Num1, MS_U16 u16Num2)
{
    return ( (u16Num1 + (u16Num2>>1)) / (u16Num2));
}

//-------------------------------------------------------------------------
// process time of OP1 to handel 1 frame < time of OP2 to output 1 frame
// Vin * (MAX(Hin,Hout)+bubble)) *(1/XC_FCLK) < Vout * H_Total * (1/ODCLK)
//
// note: if H Prescaling --> (Hin == Hout)
//-------------------------------------------------------------------------
static MS_BOOL _Hal_XC_FrameLimitation(void *pInstance, const XC_InternalStatus *pSrcInfo, MS_BOOL bIsHPreScaling)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    MS_U32 u32LeftEquation = 0;
    MS_U32 u32RightEquation = (pSrcInfo->stDispWin.height * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal * XC_FCLK);
    MS_U32 u32Bubble = (pSrcInfo->bInterlace == TRUE) ? XC_POSTSCALING_BUBBLE_INTERLACE : XC_POSTSCALING_BUBBLE;

    if (bIsHPreScaling == TRUE)
    {
        // Atfer H Prescaling: (Hin == H SizeAfterPreScaling)
        MS_U32 u32H_SizeAfterPreScaling = ( (MS_U32)pSrcInfo->stCapWin.width * pSrcInfo->stDispWin.width + ( pSrcInfo->stCropWin.width / 2 ) ) / pSrcInfo->stCropWin.width;
        u32LeftEquation = (pSrcInfo->stCapWin.height * (MAX(u32H_SizeAfterPreScaling, pSrcInfo->stDispWin.width) + u32Bubble ) * _Hal_XC_GetODCLK() );
    }
    else
    {
        u32LeftEquation = (pSrcInfo->stCapWin.height * (MAX(pSrcInfo->stCapWin.width, pSrcInfo->stDispWin.width) + u32Bubble ) * _Hal_XC_GetODCLK() );
    }

    SC_DBG(printf("[%s][%d] Left equation = %lu \n", __FUNCTION__, __LINE__, u32LeftEquation));
    SC_DBG(printf("[%s][%d] Right equation = %lu \n", __FUNCTION__, __LINE__, u32RightEquation));

    if( (u32LeftEquation) < (u32RightEquation) )
    {
        SC_DBG(printf("[%s][%d] _Hal_XC_FrameLimitation PASS \n", __FUNCTION__, __LINE__));
        return TRUE;
    }
    else
    {
        SC_DBG(printf("[%s][%d] _Hal_XC_FrameLimitation  FAILED\n", __FUNCTION__, __LINE__));
        return FALSE;
    }
}

//-----------------------------------------------------------------------------------------
// process time of OP1 to output 1 Line < display FIFO consumption time
// (1) max_disp_fifo = (1024*XC_FCLK/ODCLK) * (Hout/Hin) -- unit pixel
// (2) floor(Vin/Vout - 1) * (Hin+Bubble) *(1/XC_FCLK) < min(1024, max_disp_fifo) * (1/ODCLK)
//
// note: if H Prescaling --> (Hin == Hout)
//-------------------------------------------------------------------------------------------
static MS_BOOL _Hal_XC_FIFOLimitation(const XC_InternalStatus *pSrcInfo, MS_BOOL bIsHPreScaling)
{
    MS_U32 u32Max_disp_fifo = (1024 * XC_FCLK / _Hal_XC_GetODCLK()) * (pSrcInfo->stDispWin.width) / (pSrcInfo->stCapWin.width);
    MS_U16 u16FloorDivResult = _Hal_XC_FloorDiv((MS_U16)(pSrcInfo->stCapWin.height), pSrcInfo->stDispWin.height);
    MS_U32 u32LeftEquation = 0;
    MS_U32 u32RightEquation = (MIN(1024,u32Max_disp_fifo) * XC_FCLK);
    MS_U32 u32Bubble = (pSrcInfo->bInterlace == TRUE) ? XC_POSTSCALING_BUBBLE_INTERLACE : XC_POSTSCALING_BUBBLE;

    if (bIsHPreScaling == TRUE)
    {
        // Atfer H Prescaling: (Hin == H SizeAfterPreScaling)
        MS_U32 u32H_SizeAfterPreScaling = ( (MS_U32)pSrcInfo->stCapWin.width * pSrcInfo->stDispWin.width + ( pSrcInfo->stCropWin.width / 2 ) ) / pSrcInfo->stCropWin.width;

        u32LeftEquation = ((u16FloorDivResult == 0) ? 0 : (u16FloorDivResult-1)) * (u32H_SizeAfterPreScaling + u32Bubble) * _Hal_XC_GetODCLK();
    }
    else
    {
        u32LeftEquation = ((u16FloorDivResult == 0) ? 0 : (u16FloorDivResult-1)) * (pSrcInfo->stCapWin.width + u32Bubble) * _Hal_XC_GetODCLK();
    }

    SC_DBG(printf("[%s][%d] Max_disp_fifo = %lu \n", __FUNCTION__, __LINE__, u32Max_disp_fifo));
    SC_DBG(printf("[%s][%d] Left equation = %lu \n", __FUNCTION__, __LINE__, u32LeftEquation));
    SC_DBG(printf("[%s][%d] Right equation = %lu \n", __FUNCTION__, __LINE__, u32RightEquation));

    if( (u32LeftEquation) < (u32RightEquation) )
    {
        SC_DBG(printf("[%s][%d] _Hal_XC_FIFOLimitation PASS \n", __FUNCTION__, __LINE__));
        return TRUE;
    }
    else
    {
        SC_DBG(printf("[%s][%d] _Hal_XC_FIFOLimitation FAILED \n", __FUNCTION__, __LINE__));
        return FALSE;
    }
}

E_APIXC_ReturnValue Hal_XC_IsForcePrescaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pbForceV, MS_BOOL *pbForceH, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID) // SC0
    {
        if(pSrcInfo->stDispWin.width > 1024)
        {
            //Case1
            SC_DBG(printf("[%s][%d] CASE 1 \n", __FUNCTION__, __LINE__));
            if((TRUE == _Hal_XC_FrameLimitation(pInstance, pSrcInfo, DISABLE)) && (TRUE == _Hal_XC_FIFOLimitation(pSrcInfo, DISABLE)))
            {
                //It is alright to do post-scaling
                *pbForceH = FALSE;
                *pbForceV = FALSE;

                 /*
                    * to avoid H post scaling factor overflow (only 24 bits)
                    */
                {
                    MS_U16 u16ScaleSrc = 0, u16ScaleDst = 0;
                    MS_U32 u32TmpPost = 0;

                    u16ScaleSrc = pSrcInfo->stCapWin.width;
                    u16ScaleDst = pSrcInfo->stDispWin.width;
                    u32TmpPost = H_PostScalingRatio(u16ScaleSrc, u16ScaleDst);

                    if (u32TmpPost > 0xFFFFFFL)
                    {
                        //overflow
                        *pbForceH = TRUE;
                        SC_DBG(printf("[%s][%d] Force H pre-scaling to avoid post-scaling factor overflow \n", __FUNCTION__, __LINE__));
                    }
                }

            }
            else
            {
                //****************************
                //*  Horizontal
                //****************************

                //need to force enable H pre-scaling
                *pbForceH = TRUE;

                //****************************
                //*  Vertical
                //****************************
                if((TRUE == _Hal_XC_FrameLimitation(pSrcInfo, ENABLE)) && (TRUE == _Hal_XC_FIFOLimitation(pSrcInfo, ENABLE)))
                {
                    //It is alright to do V post-scaling
                    *pbForceV = FALSE;
                }
                else
                {
                    //It is needed to force enable V pre-scaling
                    *pbForceV = TRUE;
                }
            }
        }
        else
        {
            //Case2
            SC_DBG(printf("[%s][%d] CASE 2 \n", __FUNCTION__, __LINE__));
            if(TRUE == _Hal_XC_FrameLimitation(pSrcInfo, DISABLE))
            {
                //It is alright to do post-scaling
                *pbForceH = FALSE;
                *pbForceV = FALSE;

                 /*
                    * to avoid H post scaling factor overflow (only 24 bits)
                    */
                {
                    MS_U16 u16ScaleSrc = 0, u16ScaleDst = 0;
                    MS_U32 u32TmpPost = 0;

                    u16ScaleSrc = pSrcInfo->stCapWin.width;
                    u16ScaleDst = pSrcInfo->stDispWin.width;
                    u32TmpPost = H_PostScalingRatio(u16ScaleSrc, u16ScaleDst);

                    if (u32TmpPost > 0xFFFFFFL)
                    {
                        //overflow
                        *pbForceH = TRUE;
                        SC_DBG(printf("[%s][%d] Force H pre-scaling to avoid post-scaling factor overflow \n", __FUNCTION__, __LINE__));
                    }
                }

            }
            else
            {
                //****************************
                //*  Horizontal
                //****************************

                //need to force enable H pre-scaling
                *pbForceH = TRUE;

                //****************************
                //*  Vertical
                //****************************
                if(TRUE == _Hal_XC_FrameLimitation(pSrcInfo, ENABLE))
                {
                    //It is alright to do V post-scaling
                    *pbForceV = FALSE;
                }
                else
                {
                    //It is needed to force enable V pre-scaling
                    *pbForceV = TRUE;
                }

            }
        }
    }
    else
    {
//[FIXME][KANO] Add SC1 here
    }
    return E_APIXC_RET_OK;
}
#endif

#define HAL_SC_FHD_SIZE         (2088960ul)
#define HAL_SC_DI_LB_SIZE       (2048ul)
#define HAL_SC_FHD_HEIGHT       (1080ul)

// SC1 HVSP field difference tolerance line number for P2I
// This is HW limitation
#define SC1_P2I_TOLERANCE_LINE  5

#if 0
static MS_U16 _Hal_SC_get_Fclk(void)
{
    MS_U8 u8FClkReg = MDrv_ReadByte(REG_CKG_FCLK) & CKG_FCLK_MASK;

    if(CKG_FCLK_170MHZ == u8FClkReg)
    {
        return 170;
    }
    else if(CKG_FCLK_144MHZ == u8FClkReg)
    {
        return 144;
    }
    else if(CKG_FCLK_108MHZ == u8FClkReg)
    {
        return 108;
    }
    else if(CKG_FCLK_86MHZ == u8FClkReg)
    {
        return 86;
    }
    else
    {
        return 170;
    }
}
#endif

#define HAL_SC_BYPASS_WIDTH 1920
#define HAL_SC_BYPASS_HEIGHT 1072

E_APIXC_ReturnValue Hal_XC_IsForcePrescaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pbForceV, MS_BOOL *pbForceH, SCALER_WIN eWindow)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U64 u64_result = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    *pbForceV = FALSE;
    *pbForceH = FALSE;

    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        //use max clock now. we use FCLK_TUNE for HW control clock dynamically
        MDrv_WriteByteMask(REG_CKG_FCLK, CKG_FCLK_170MHZ, CKG_FCLK_MASK);
        MDrv_WriteByteMask(REG_CKG_EDCLK, CKG_EDCLK_170MHZ, CKG_EDCLK_MASK);

	if((pSrcInfo->stCapWin.height) * (pSrcInfo->stCapWin.width) > 1920 * 1080)
        {
            SC_DBG(printf("[%s][%d] k6lite overspec input size force pre-scaling\n", __FUNCTION__, __LINE__));
            if(pSrcInfo->stCapWin.height > 1080)
            {   SC_DBG(printf("[%s][%d] k6lite overspec V size force pre-scaling\n", __FUNCTION__, __LINE__));
                *pbForceV = TRUE;
            }
            if(pSrcInfo->stCapWin.width > 1920)
            {   SC_DBG(printf("[%s][%d] k6lite overspec H size force pre-scaling\n", __FUNCTION__, __LINE__));
                *pbForceH = TRUE;
            }
        }

        //-----------------------------------------
        // Vertical
        //-----------------------------------------
        SC_DBG(printf("[%s][%d] V Size, Crop %d, Disp %d\n", __FUNCTION__, __LINE__,pSrcInfo->stCropWin.height,pSrcInfo->stDispWin.height));

        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1000)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 1600)) //consider 1680x1050 into this case
        {
            // Need force pre-scaling if disp.V size < 50% of crop.V size.
            if((pSrcInfo->stDispWin.height * 2) < (pSrcInfo->stCropWin.height * 1))
            {
                *pbForceV = TRUE;
                SC_DBG(printf("[%s][%d] disp.V size < 55%% of crop.V size, need force V pre-scaling\n", __FUNCTION__, __LINE__));
            }
        }
        // Output is smaller than FHD
        else
        {
            // Need force pre-scaling if disp.V size < 30% of crop.V size.
            if((pSrcInfo->stDispWin.height * 10) < (pSrcInfo->stCropWin.height * 3))
            {
                *pbForceV = TRUE;
                SC_DBG(printf("[%s][%d] disp.V size < 30%% of crop.V size, need force V pre-scaling\n", __FUNCTION__, __LINE__));
            }
        }

        //-----------------------------------------
        // Horizontal
        //-----------------------------------------
        SC_DBG(printf("[%s][%d] H Size, Crop %d, Disp %d\n", __FUNCTION__, __LINE__,pSrcInfo->stCropWin.width,pSrcInfo->stDispWin.width));

        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1000)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 1600)) //consider 1680x1050 into this case
        {
            // Need force pre-scaling if disp.H size < 50% of crop.H size.
            if((pSrcInfo->stDispWin.width * 2) < (pSrcInfo->stCropWin.width * 1))
            {
                *pbForceH = TRUE;
                SC_DBG(printf("[%s][%d] disp.H size < 50%% of crop.H size, need force H pre-scaling\n", __FUNCTION__, __LINE__));
            }
        }
        // Output is smaller than FHD
        else
        {
            // Need force pre-scaling if disp.H size < 30% of crop.H size.
            if((pSrcInfo->stDispWin.width * 10) < (pSrcInfo->stCropWin.width * 3))
            {
                *pbForceH = TRUE;
                SC_DBG(printf("[%s][%d] disp.H size < 30%% of crop.H size, need force H pre-scaling\n", __FUNCTION__, __LINE__));
            }
        }

        //******************Follow Curry patch: fe818f4eba67e0e6a98083434c7a75642ca4a271**********************//
        // Enable fclk tuning
        // Discuss with HW. In k6lite, if DS case or post v scaling down case can not use fclk tune
        // (Because of DS write command in FW, we cannot confirm no post v scaling down)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x1, 0x1);
        if((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] != E_XC_3D_INPUT_MODE_NONE)||(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode != E_XC_3D_OUTPUT_MODE_NONE)
           ||(pXCResourcePrivate->stdrvXC_3D._e3DPanelType != E_XC_3D_PANEL_NONE)||
           MDrv_XC_GetDynamicScalingStatus(pInstance))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x0, 0x1);
        }
        //****************************************************************************************************//

        if(*pbForceH)
        {
            MS_U16 u16HSizeAfterPreScaling = (*pbForceH) ?
                                            (((MS_U32)pSrcInfo->stCapWin.width * pSrcInfo->stDispWin.width + (pSrcInfo->stCropWin.width / 2)) / pSrcInfo->stCropWin.width):
                                            pSrcInfo->stCropWin.width;

            if(u16HSizeAfterPreScaling > HAL_SC_DI_LB_SIZE)
            {
                pSrcInfo->Status2.bPreHCusScaling = TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = HAL_SC_DI_LB_SIZE;
                SC_DBG(printf("[%s][%d] u16HSizeAfterPreScaling > DI LB size, force H pre-scaling\n", __FUNCTION__, __LINE__));
            }
        }

        if(*pbForceV)
        {
            if((pSrcInfo->stDispWin.height * 4) < (pSrcInfo->stCropWin.height * 1))
            {
                pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCropWin.height / 4);
                pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCropWin.height);
                pSrcInfo->Status2.bPreVCusScaling = TRUE;
            }
        }

        //For K6lite power saving, we force go power saving path here
        //condition: only FHD in/FHD out/P mode/non-DS case can bypass
        //Bypass to HVSP: have PQ flow issue(422to444 control)
        //Bypass to VIP: can not HVSP => We choose this way
        //Bypass to VOP: can not HVSP, can not 3x3(2f_70 useless), 1920x1072 have garbage at bottom
        if(!MDrv_XC_Is_SupportSWDS(pInstance))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s][%d] bypass choice: %d %d %d %d, %d %d %d %d, %d %d\n",__FUNCTION__,__LINE__,
                pSrcInfo->stDispWin.x,pSrcInfo->stDispWin.y,pSrcInfo->stDispWin.width,pSrcInfo->stDispWin.height,
                pSrcInfo->stCropWin.x,pSrcInfo->stCropWin.y,pSrcInfo->stCropWin.width,pSrcInfo->stCropWin.height,
                gSrcInfo[eWindow].bInterlace,MDrv_XC_GetDynamicScalingStatus(pInstance));

            //Only DTV can enter bypass mode for mantis_1253884 to isolate MM photo from entering bypass mode.
            //Temp:At now, MM movie can't enter bypass mode.
            //However, MM movie should be able to enter bypass mode. But:
            //INPUT_SOURCE_STORAGE can't separate movie/photo yet[SN].
            //INPUT_SOURCE_STORAGE can't separate DTV/movie/photo yet[DDI].
            if((pSrcInfo->stDispWin.width >= HAL_SC_BYPASS_WIDTH) && (pSrcInfo->stDispWin.height >= HAL_SC_BYPASS_HEIGHT) &&
                (pSrcInfo->stCropWin.width == pSrcInfo->stDispWin.width) && (pSrcInfo->stCropWin.height == pSrcInfo->stDispWin.height) &&
                (gSrcInfo[eWindow].bInterlace == FALSE) && (FALSE == MDrv_XC_GetDynamicScalingStatus(pInstance)) &&
                (pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow] == INPUT_SOURCE_DTV))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s][%d] bypass mode\n",__FUNCTION__,__LINE__);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_40_L, 0x200, 0x200);
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK3F_40_L, 0x3300);
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s][%d] normal mode\n",__FUNCTION__,__LINE__);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_40_L, 0x0, 0x200);
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK3F_40_L, 0xFFFF);
            }
        }

        return E_APIXC_RET_OK;
    }
    else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID) // SC1
    {
        MS_U16 u16Width, u16Height = 0;
        MS_U32 u32Vfreqx10 = 0;

        if(!MDrv_XC_Is_SupportSWDS(pInstance))
        {
            u16Width   = gSrcInfo[eWindow].stCapWin.width;
            u16Height  = gSrcInfo[eWindow].stCapWin.height;
        }
        else
        {
            u16Width   = gSrcInfo[eWindow].Status2.u16VirtualBox_Width;
            u16Height  = gSrcInfo[eWindow].Status2.u16VirtualBox_Height;
        }

        u32Vfreqx10 = gSrcInfo[eWindow].u16InputVFreq;

        SC_DBG(printf("force clock setting!!!\n"));
        MDrv_WriteByteMask(REG_CKG_SC1_FCLK, CKG_SC1_FCLK_172MHZ, CKG_SC1_FCLK_MASK);
        MDrv_WriteByteMask(REG_CKG_SC1_EDCLK, CKG_SC1_EDCLK_172MHZ, CKG_SC1_EDCLK_MASK);

        if(pSrcInfo->Status2.bPreVCusScaling || pSrcInfo->Status2.bPreHCusScaling)
        {
            SC_DBG(printf("[%s][%d] Custom pre-scaling is already set! Not doing force pre-scaling check\n", __FUNCTION__, __LINE__));
            return E_APIXC_RET_OK;
        }

        if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))
        {
            if((u16Width >= 0xF00)&&(u16Height >= 0x870))
            {
                if(u32Vfreqx10 >= 330)
                {
                    // IP 2P to 1P H size reduces to half
                    //pSrcInfo->stCapWin.width /= 2;
                    //pSrcInfo->stCropWin.width /= 2;
                    if(pSrcInfo->Status2.bPreHCusScaling)
                    {
                        pSrcInfo->Status2.u16PreHCusScalingSrc /= 2;
                    }
                    else
                    {
                        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width/2;
                        pSrcInfo->Status2.bPreHCusScaling = TRUE;
                    }
                    pSrcInfo->Status2.u16PreHCusScalingDst = 720u;
                }
            }
        }

        MS_U32 u32TmpVPost= 0;
        MS_U32 u32InitFactorTwo = 0;

        /// - Adjust H prescaling ratio
        // SC1 post-scaling line buffer limitation 720px
        if(pSrcInfo->Status2.bPreHCusScaling)
        {
            if(pSrcInfo->Status2.u16PreHCusScalingDst > SC1_SCALER_LINE_BUFFER_MAX)
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = SC1_SCALER_LINE_BUFFER_MAX;
            }
        }
        else
        {
            if(pSrcInfo->stCropWin.width > SC1_SCALER_LINE_BUFFER_MAX)
            {
                pSrcInfo->Status2.bPreHCusScaling = TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = SC1_SCALER_LINE_BUFFER_MAX;
            }
        }

        if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeCapture(gSrcInfo[eWindow].enInputSourceType))
        {
            if((u16Width >= 0xF00)&&(u16Height >= 0x870))
            {
                if(pSrcInfo->Status2.bPreVCusScaling)
                {
                    if(pSrcInfo->Status2.u16PreVCusScalingDst >= 1088u)
                    {
                        pSrcInfo->Status2.u16PreVCusScalingDst /= 2;
                    }
                }
                else
                {
                    if(pSrcInfo->stCropWin.height >= 1088u)
                    {
                        pSrcInfo->Status2.bPreVCusScaling = TRUE;
                        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height/2;
                    }
                }
                pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            }
            else
            {
                if((pSrcInfo->stDispWin.height * 100) < (pSrcInfo->stCropWin.height * 25))
                {
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCropWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCropWin.height;
                    pSrcInfo->Status2.bPreVCusScaling = TRUE;

                    while((pSrcInfo->Status2.u16PreVCusScalingDst*100/4) > (pSrcInfo->stDispWin.height*100))
                    {
                        pSrcInfo->Status2.u16PreVCusScalingDst /=2;
                    }
                }

            }
        }

        /// - Adjust V prescaling ratio
        //Get V post-scaling ratio for current V pre-scaling destination vertical size
        if(pSrcInfo->Status2.bPreVCusScaling)
        {
            if(psXCInstPri->u32DeviceID == 1)
            {
#ifdef MSOS_TYPE_LINUX_KERNEL
                V_PostScalingRatio_P2I(pSrcInfo->Status2.u16PreVCusScalingDst, pSrcInfo->stDispWin.height);
                u32TmpVPost = (MS_U32)u64_result;
#else
                u32TmpVPost = V_PostScalingRatio_P2I(pSrcInfo->Status2.u16PreVCusScalingDst, pSrcInfo->stDispWin.height);
#endif
            }
            else
            {
#ifdef MSOS_TYPE_LINUX_KERNEL
                V_PostScalingRatio(pSrcInfo->Status2.u16PreVCusScalingDst, pSrcInfo->stDispWin.height);
                u32TmpVPost = (MS_U32)u64_result;
#else
                u32TmpVPost = V_PostScalingRatio(pSrcInfo->Status2.u16PreVCusScalingDst, pSrcInfo->stDispWin.height);
#endif
            }
        }
        else
        {
            if(psXCInstPri->u32DeviceID == 1)
            {
#ifdef MSOS_TYPE_LINUX_KERNEL
                V_PostScalingRatio_P2I(pSrcInfo->stCropWin.height, pSrcInfo->stDispWin.height);
                u32TmpVPost = (MS_U32)u64_result;
#else
                u32TmpVPost = V_PostScalingRatio_P2I(pSrcInfo->stCropWin.height, pSrcInfo->stDispWin.height);
#endif
            }
            else
            {
#ifdef MSOS_TYPE_LINUX_KERNEL
                V_PostScalingRatio(pSrcInfo->stCropWin.height, pSrcInfo->stDispWin.height);
                u32TmpVPost = (MS_U32)u64_result;
#else
                u32TmpVPost = V_PostScalingRatio(pSrcInfo->stCropWin.height, pSrcInfo->stDispWin.height);
#endif
            }
        }
        u32TmpVPost &= 0xFFFFFFL;    //scaling factor support 24bits
        u32InitFactorTwo = ((u32TmpVPost/2) & 0xF00000) >> 20;

        // Check vertical initial factor two. To Check HVSP P2I tolerance line number is allowed.
        if( u32InitFactorTwo > SC1_P2I_TOLERANCE_LINE)
        {
            SC_DBG(printf("[%s][%d] vertical initial factor two %du > 5 (SC1_P2I_TOLERANCE_LINE) \n", __FUNCTION__, __LINE__, u32InitFactorTwo));
            pSrcInfo->Status2.bPreVCusScaling = TRUE;
            if ( pSrcInfo->bInterlace )
            {
                pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height / 2;
            }
            else
            {
                if ( pSrcInfo->stCropWin.height == 0 )
                {
                    SC_DBG(printf("[%s][%d] pSrcInfo->stCropWin.height is 0. Set pre-scaling destination vertical size to 32!!\n", __FUNCTION__, __LINE__));
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = 32;
                }
                else
                {
                    MS_U16 u16DispWinHeight = pSrcInfo->stDispWin.height*2;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = ( (MS_U32)pSrcInfo->stCapWin.height * u16DispWinHeight + ( pSrcInfo->stCropWin.height / 2 ) ) / pSrcInfo->stCropWin.height;
                }
            }
        }
    }
    else
    {
        // Invalid XC device ID
    }

    return E_APIXC_RET_OK;
}

MS_U32 MHal_SC_Get_LpllSet_Factor(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 u8LPLL_Type, MS_U32 u32DefaultDClk)
{
    MS_U32 u32Factor = 0;
    UNUSED(u8LPLL_Type);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    u32Factor = LPLL_LOOPGAIN/8;

    if ((LVDS_MPLL_CLOCK_MHZ == 432) && (LPLL_LOOPGAIN == 8))
    {
        u32Factor  = LVDS_MPLL_CLOCK_MHZ * LPLL_LOOPGAIN/8/216;
    }

    return u32Factor;
}

E_APIXC_ReturnValue Hal_SC_Enable_AVMute(void *pInstance, SCALER_WIN eWindow)
{
    //do nothing
    return E_APIXC_RET_FAIL;
}

void MHal_XC_Calc_IGainPGain(void *pInstance, MS_U8 *u8GainI, MS_U8 *u8GainP, MS_U32 u32XTAL_Clock, MS_U8 u8LGain, MS_U8 u8Vco, MS_U16 u16HTotal, MS_U16 u16VTotal, MS_U8 u8FRC_Out)
{
    MS_U32 u32Factor, u32Temp;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
	u32Temp = (MS_U32)(u16VTotal*u16HTotal*u8FRC_Out);
    u32Factor = ((LVDS_MPLL_CLOCK_MHZ*1000000UL/u32XTAL_Clock)*524288*u8LGain)/((MS_U32)(u32Temp*u8Vco/2*8));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MM=%ld,u8LGain1=%d,Htt=%d,Vtt=%d,Ovs=%d,Vco=%d,i_gain=%d,REGIg=%d\n",
    (LVDS_MPLL_CLOCK_MHZ*1000000UL/pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock),u8LGain,u16HTotal,
    u16VTotal,u8FRC_Out,u8Vco/2,u32Factor,MHal_SC_cal_usedgain_to_reggain(u32Factor));

    *u8GainI = MHal_SC_cal_usedgain_to_reggain(u32Factor);
    *u8GainP = *u8GainI+1;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "==>LG1=%d,iGain=%d,I=%d,P=%d\n",u8LGain,u32Factor,*u8GainI,*u8GainP);
}


void MHal_XC_SetForceReadBank(void *pInstance, MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
    {
        if (bEnable)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(11), BIT(11));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, (u8Bank & 0x7) << 8, BIT(10)|BIT(9)|BIT(8));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(11));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(10)|BIT(9)|BIT(8));
        }
    }
    else
    {
        if (bEnable)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(11), BIT(11));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, (u8Bank & 0x7) << 8, BIT(10)|BIT(9)|BIT(8));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(11));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(10)|BIT(9)|BIT(8));
        }
    }
}

E_APIXC_ReturnValue MHal_XC_Vtrack_SetPayloadData(void *pInstance, MS_U16 u16Timecode, MS_U8 u8OperatorID)
{
    /*
     * reg_payload_use from 0x08 to 0x0C (1B - 1F)
     * {14'h0, TimeCode[55:40], Unique ID (OTP)[39:8], Operator ID[7:0]}
     */

    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_08_L, (MS_U16)u8OperatorID, 0x00ff);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_0A_L, (u16Timecode << 8) , 0xff00);
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_0B_L, (u16Timecode >> 8) , 0x00ff);

    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    return  E_APIXC_RET_OK;
}

#define  VTRACK_SETTING_LENGTH 23

static MS_U8 u8VtrackSetting[VTRACK_SETTING_LENGTH]=
{
    0x00, 0x00, 0x53, 0x8c, 0x31,
    0x86, 0x10, 0x42, 0x08, 0x21,
    0x84, 0x10, 0x42, 0x08, 0x21,
    0xc4, 0x18, 0x63, 0x8c, 0x41,
    0x08, 0x21, 0x04,
};

E_APIXC_ReturnValue MHal_XC_Vtrack_SetUserDefindedSetting(void *pInstance, MS_BOOL bUserDefinded, MS_U8 *pu8Setting)
{
    /*
     * Default Setting:
     * setting1 [0e]              => 8'h00
     * setting2 [0f]              => 8'h00
     * setting3 [1a:10]           => 165'h04 21 08 418c6318c4 21084210842108421086318c53
     */
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U8 *pu8Data = NULL;
    MS_U16 u16Index = 0;
    MS_U16 u16Data = 0;

    if (bUserDefinded == TRUE)
    {
        pu8Data = pu8Setting;
    }
    else
    {
        pu8Data = &u8VtrackSetting[0];
    }

    //MenuLoad enable
    if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_0E_L, pu8Data[0], 0xff);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_0F_L, pu8Data[1], 0xff);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_0E_L, pu8Data[0], 0xff);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_0F_L, pu8Data[1], 0xff);
    }

    for (u16Index = 2; u16Index < VTRACK_SETTING_LENGTH; u16Index = u16Index+2)
    {
        MS_U8 u8Offset = (( (u16Index - 2) / 2) *2 );
        MS_U32 u32Addr = REG_SC_BK2F_10_L + u8Offset;

        if ( (VTRACK_SETTING_LENGTH - u16Index) >= 2)
        {
            u16Data = (MS_U16)pu8Data[u16Index +1];
            u16Data = (u16Data << 8) + (MS_U16) pu8Data[u16Index];

            //MenuLoad enable
            if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
                MDrv_XC_MLoad_WriteCmd(pInstance, u32Addr , u16Data, 0xffff); //write 16 bits
            else
                SC_W2BYTE(psXCInstPri->u32DeviceID, u32Addr, u16Data);
        }
        else
        {
            u16Data = (MS_U16) pu8Data[u16Index];

            //MenuLoad enable
            if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
                MDrv_XC_MLoad_WriteCmd(pInstance, u32Addr , u16Data, 0x00ff); //write low 8bits
            else
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr, u16Data, 0x00ff);
        }
    }

    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    return  E_APIXC_RET_OK;
}

#ifdef UFO_XC_VMARK
E_APIXC_ReturnValue MHal_XC_Vtrack_Enable(void *pInstance, MS_U8 u8FrameRate, MS_BOOL bEnable)
{
    //FrameRateIn     => 8'h1E
    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_02_L , u8FrameRate, 0xFF);

    //EnableIn              => 1
    //DebugEn               => 0
    if (bEnable == TRUE)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_01_L , BIT(0), BIT(1)|BIT(0) );
    }
    else
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2F_01_L , 0x00, BIT(1)|BIT(0) );
    }

    MDrv_XC_MLoad_Fire(pInstance, TRUE);

    return  E_APIXC_RET_OK;
}


E_APIXC_ReturnValue MHal_XC_VMark_SetParameters(void* pInstance, MS_U8 u8ServiceIndex, XC_VMARK_PARAMETER *pstXC_VMarkParameters)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U8 u8WatermarkOn = FALSE;
    MS_U16 u16SymbolScale;
    MS_U16 u16TargetScale;
    MS_U16 u16SymbolSpaceVert  = 12;
    MS_U16 u16SymbolSpaceHorz = 10;
    MS_U16 u16SymbolsXpos;
    MS_U16 u16SymbolsYpos;
    MS_U16 u16MarklineWidth;
    MS_S8 s8StrengthMultiply;
    MS_U8 u8FrequencyDistance[3][3];
    MS_U32 M, N, S;
    static const MS_U8 u8FrequencyDistanceMin[3] = {0, 0, 3};
    static const MS_U8 u8FrequencyDistanceMax[3] = {3, 7, 16};
    MS_U16 u16VRes = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    MS_U16 u16HRes = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;


    u8ServiceIndex = u8ServiceIndex; // Not supported in the first implementations.

    if (pstXC_VMarkParameters == NULL)
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }


    if (pstXC_VMarkParameters->u8WatermarkOn == 1) // Watermark on
    {
        MS_U16 u16SymbolIndex;
        MS_BOOL bMode2P = (psXCInstPri->u32DeviceID == E_XC_DEVICE0)?TRUE:FALSE;
        MS_BOOL bInterlaced = (psXCInstPri->u32DeviceID == E_XC_DEVICE1)?TRUE:FALSE;

        // Hide watermark
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_29_L, 0, 0x00FF);

        // Interlaced
        if (bInterlaced)
        {
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, BIT(3), BIT(3));
        }
        else
        {
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, 0, BIT(3));
        }

        /**********************************
        *             Write Symbol Table
        **********************************/
        {
            memcpy(&u8VMarkSymbols, pstXC_VMarkParameters->u8PayloadSymbols, 1920*sizeof(MS_U8));

            // Enable symbol table access
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, BIT(5), BIT(5));

            // Write Symbol Table
            for (u16SymbolIndex = 0; u16SymbolIndex < (1920/2); u16SymbolIndex++) /// **** TBD ****: Fix magic number
            {
                //Address
                VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_30_L, u16SymbolIndex, 0x03FF);
                // WData
                VMARK_W2BYTE(psXCInstPri->u32DeviceID, REG_VMARK_31_L, u8VMarkSymbols[u16SymbolIndex*2+1]<<8 |u8VMarkSymbols[u16SymbolIndex*2]);
            }

            // Disable symbol table access
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, 0, BIT(5));
        }

        // V resolution
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_04_L, u16VRes, 0x0FFF);
        // H resolution
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_05_L, u16HRes, 0x0FFF);

        // Upper bound of Y result
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_02_L, 0x3FF, 0x3FF);
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_03_L, 0, 0x03FF);


        /**********************************
        *             Symbol number
        **********************************/
        //Symbol Rows * Cols <= 16
        if (pstXC_VMarkParameters->u8SymbolsRows * pstXC_VMarkParameters->u8SymbolsCols > 16)
        {
            SC_DBG(printf("[%s][%d] Too many symbol numbers. Max symbol = 16 \n", __FUNCTION__, __LINE__));
            return E_APIXC_RET_FAIL_INVALID_PARAMETER;
        }
        // Symbol Row
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_06_L, pstXC_VMarkParameters->u8SymbolsRows, 0x000F);
        // Symbol Col
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_06_L, pstXC_VMarkParameters->u8SymbolsCols << 4, 0x00F0);


        /**********************************
        *             Calculate Symbol Scale
        **********************************/
        {
            // Calculate target scale
            u16TargetScale = (u16VRes * 10 * pstXC_VMarkParameters->u8SymbolScaleControl / 480
                                          - 10 * pstXC_VMarkParameters->u8SymbolScaleControl + 100) * pstXC_VMarkParameters->u8SymbolSize;

            u16SymbolScale = MIN(u16TargetScale, u16HRes * (95 - pstXC_VMarkParameters->u8SymbolsXpos) / (pstXC_VMarkParameters->u8SymbolsCols * (20 + pstXC_VMarkParameters->u16SpacingHorz) - pstXC_VMarkParameters->u16SpacingHorz));
            u16SymbolScale = MIN(u16SymbolScale, u16VRes * (95 - pstXC_VMarkParameters->u8SymbolsYpos) / (pstXC_VMarkParameters->u8SymbolsRows * (24 + pstXC_VMarkParameters->u16SpacingVert) - pstXC_VMarkParameters->u16SpacingVert));

            // Determine scale as integer multiple
            u16SymbolScale /= 100;
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_06_L, u16SymbolScale << 8, 0xFF00);

        }

        /**********************************
        *             Calculate Symbol Position
        **********************************/
        {
            u16SymbolsXpos = (MS_U16) pstXC_VMarkParameters->u8SymbolsXpos;
            u16SymbolsYpos = (MS_U16) pstXC_VMarkParameters->u8SymbolsYpos;
            // Range 5%~95%
            if (u16SymbolsXpos > 95)
            {
                u16SymbolsXpos = 95;
            }
            else if (u16SymbolsXpos < 5)
            {
                u16SymbolsXpos = 5;
            }

            if (u16SymbolsYpos > 95)
            {
                u16SymbolsYpos = 95;
            }
            else if (u16SymbolsYpos < 5)
            {
                u16SymbolsYpos = 5;
            }

            u16SymbolsXpos = u16SymbolsXpos * u16HRes / 100;
            u16SymbolsYpos = u16SymbolsYpos * u16VRes / 100;

            if (bMode2P)
            {
                // 2P mode. Must on even pixel.
                u16SymbolsXpos =  (u16SymbolsXpos >> 1) << 1;
            }

            // Symbol X Pos
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_07_L, u16SymbolsXpos, 0x0FFF);
            // Symbol Y Pos
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_08_L, u16SymbolsYpos, 0x0FFF);
        }

        /**********************************
        *             Symbol space
        **********************************/
        {
            u16SymbolSpaceHorz = pstXC_VMarkParameters->u16SpacingHorz;
            u16SymbolSpaceVert = pstXC_VMarkParameters->u16SpacingVert;
            // Symbol Space X
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_09_L, u16SymbolSpaceHorz, 0x01FF);
            // Symbol Space Y
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_0A_L, u16SymbolSpaceVert, 0x01FF);
        }

        /**********************************
        *            Scaled frequency distance
        **********************************/
        {
            MS_U8 u8Direction, u8Distance;

            for (u8Direction = 0; u8Direction < 3; u8Direction++)
            {
                for (u8Distance = 0; u8Distance < 3; u8Distance++)
                {
                    u8FrequencyDistance[u8Direction][u8Distance] = pstXC_VMarkParameters->u8FrequencyDistance[u8Direction][u8Distance];
                    //validate frequency distance values against thresholds
                    if(u8FrequencyDistance[u8Direction][u8Distance] < u8FrequencyDistanceMin[u8Distance])
                    {
                        u8FrequencyDistance[u8Direction][u8Distance] = u8FrequencyDistanceMin[u8Distance];
                    }
                    if(u8FrequencyDistance[u8Direction][u8Distance] > u8FrequencyDistanceMax[u8Distance])
                    {
                        u8FrequencyDistance[u8Direction][u8Distance] = u8FrequencyDistanceMax[u8Distance];
                    }
                }
                // validate that frequency distances are in ascending order.
                if(u8FrequencyDistance[u8Direction][0] > u8FrequencyDistance[u8Direction][1])
                {
                    u8FrequencyDistance[u8Direction][0] = MIN(u8FrequencyDistance[u8Direction][1], u8FrequencyDistanceMax[0]);
                }
                if(u8FrequencyDistance[u8Direction][1] > u8FrequencyDistance[u8Direction][2])
                {
                    u8FrequencyDistance[u8Direction][1] = MIN(u8FrequencyDistance[u8Direction][2], u8FrequencyDistanceMax[1]);
                }
            }

            // Scale frequency distance
            for (u8Direction = 0; u8Direction < 3; u8Direction++)
            {
                for (u8Distance= 0; u8Distance< 3; u8Distance++)
                {
                    u8FrequencyDistance[u8Direction][u8Distance] = (u8FrequencyDistance[u8Direction][u8Distance] * u16VRes + 240) / 480;
                    u8FrequencyDistance[u8Direction][u8Distance] = (bMode2P || bInterlaced)? ((u8FrequencyDistance[u8Direction][u8Distance] + 1) & 0xFE) : u8FrequencyDistance[u8Direction][u8Distance];
                }
            }
            VMARK_W2BYTE(psXCInstPri->u32DeviceID, REG_VMARK_0B_L, u8FrequencyDistance[0][1] << 8 | u8FrequencyDistance[0][0]);
            VMARK_W2BYTE(psXCInstPri->u32DeviceID, REG_VMARK_0C_L, u8FrequencyDistance[1][0] << 8 | u8FrequencyDistance[0][2]);
            VMARK_W2BYTE(psXCInstPri->u32DeviceID, REG_VMARK_0D_L, u8FrequencyDistance[1][2] << 8 | u8FrequencyDistance[1][1]);
            VMARK_W2BYTE(psXCInstPri->u32DeviceID, REG_VMARK_0E_L, u8FrequencyDistance[2][0] << 8 | u8FrequencyDistance[2][0]);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_0F_L, u8FrequencyDistance[2][2], 0x00FF);
        }

        /**********************************
        *          Embedding Strength Threshold
        **********************************/
        // Defines the impact of different noise levels to the amount of pixel modification
        {
            // Currently only support 10bit depth
            // Foreground
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_10_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[0], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_11_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[1], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_12_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[2], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_13_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[3], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_14_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[4], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_15_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[5], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_16_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[6], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_17_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[7], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_18_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[8], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_19_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[9], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_1A_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[10], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_1B_L, pstXC_VMarkParameters->u16EmbeddingStrengthThreshold[11], 0x0FFF);
            // Background
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_1C_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[0], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_1D_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[1], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_1E_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[2], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_1F_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[3], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_20_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[4], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_21_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[5], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_22_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[6], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_23_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[7], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_24_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[8], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_25_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[9], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_26_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[10], 0x0FFF);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_27_L, pstXC_VMarkParameters->u16EmbeddingStrengthThresholdBG[11], 0x0FFF);
            // Clipping value (maximum) for the aggregated noise measurement in each direction
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_28_L, pstXC_VMarkParameters->u16DirectionMax, 0x03FF);
        }

        //reg_top_buf_col_spac
        {
            u16MarklineWidth = pstXC_VMarkParameters->u8SymbolsCols * (20+ pstXC_VMarkParameters->u16SpacingHorz) * u16SymbolScale\
                                       - pstXC_VMarkParameters->u16SpacingHorz * u16SymbolScale;
            M = u8FrequencyDistance[2][2];
            N = 3840 / (M + 1);
            S = u16MarklineWidth/N + 1;

            if (S < 4)
            {
                S = 4;
            }

            if (bMode2P)
            {
                S = ((S+1) & 0x0ffe) >> 1;
            }
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_0F_L, S <<8, 0xFF00);
        }

        /**********************************
        *             Strength Multiplier
        **********************************/
        s8StrengthMultiply = pstXC_VMarkParameters->s8StrengthMultiply;
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_29_L, (MS_U8)pstXC_VMarkParameters->s8StrengthMultiply, 0x00FF);


        /**********************************
        *             Enable VME
        **********************************/
        u8WatermarkOn = VMARK_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, BIT(0));
        if (!u8WatermarkOn)
        {
            // VMark SW reset
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, BIT(15), BIT(15));
            MsOS_DelayTask(5);
            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, 0, BIT(15));

            VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_00_L, BIT(0), BIT(0)); // Enable VMark enable bit
            MsOS_DelayTask(100);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_28_L, 0x01, 0x03); //reg_sc_to_ve_sel
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_01_L, 0x20, 0x30); // Mux: VMark
       }

    }
    else // Watermark off
    {
        // Strength Multiplier = 0 to hide
        VMARK_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_VMARK_29_L, 0, 0x00FF);
    }

    return  E_APIXC_RET_OK;
}
#endif


MS_BOOL MHal_XC_SetDNRBufAddress(void *pInstance, MS_PHY u32DNRBaseAddr, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC._u32DNRBaseAddress[eWindow] = u32DNRBaseAddr;
    return TRUE;
}

MS_PHY MHal_XC_GetDNRBufAddress(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_SC._u32DNRBaseAddress[eWindow] ;
}

MS_BOOL MHal_XC_SetDNRBufSize(void *pInstance, MS_PHY u32DNRBufSize, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_SC._u32DNRBufferSize[eWindow] = u32DNRBufSize;
    return TRUE;
}

MS_PHY MHal_XC_GetDNRBufSize(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->sthal_SC._u32DNRBufferSize[eWindow] ;
}

MS_BOOL MHal_XC_IsPNLYUVOutput(void *pInstance)
{
    MS_BOOL bIsPNLYUVOutput = TRUE;
#if 0//below is logic from TV, need box RD's confirm, disable it temporary
    MS_BOOL bIsY2REnable = FALSE, bIsR2YEnable = FALSE;
    SCALER_WIN eWindow = MAIN_WINDOW;
    MS_U16 u16Temp = 0;
    //Get CSC state before OP2
    if(gSrcInfo[MAIN_WINDOW].bBlackscreenEnabled || gSrcInfo[MAIN_WINDOW].bBluescreenEnabled)
    {
        //Main is mute, so check sub
        if(Hal_SC_Is_subwindow_enable(pInstance))
        {
            if(!(gSrcInfo[SUB_WINDOW].bBlackscreenEnabled || gSrcInfo[SUB_WINDOW].bBluescreenEnabled))
            {
                eWindow = SUB_WINDOW;
            }
        }
    }

    if(eWindow == MAIN_WINDOW)
    {
        u16Temp = SC_R2BYTE(REG_SC_BK10_2F_L);
        if((u16Temp & ( BIT(0) | BIT(2) | BIT(4))) == ( BIT(0) | BIT(2) | BIT(4)))
        {
            bIsY2REnable =  TRUE;
        }
    }
    else
    {
        u16Temp = SC_R2BYTE(REG_SC_BK0F_26_L);
        if((u16Temp & ( BIT(0) | BIT(2) | BIT(4))) == ( BIT(0) | BIT(2) | BIT(4)))
        {
            bIsY2REnable =  TRUE;
        }
    }

    if(bIsY2REnable)
    {
        bIsPNLYUVOutput = FALSE;
    }
    else
    {
        //OP Y2R disable, so check op input
        if(eWindow == MAIN_WINDOW)
        {
            bIsR2YEnable = (MS_BOOL)(SC_R2BYTEMSK(REG_SC_BK02_40_L, BIT(3)) || SC_R2BYTEMSK(REG_SC_BK18_6E_L, BIT(0)));

        }
        else
        {
            bIsR2YEnable = (MS_BOOL)(SC_R2BYTEMSK(REG_SC_BK04_40_L, BIT(3)) || SC_R2BYTEMSK(REG_SC_BK18_6E_L, BIT(4)));
        }

        if(bIsR2YEnable)
        {
            //Y2R enable
            bIsPNLYUVOutput = TRUE;
        }
        else
        {
            //All Csc is off, so check input
            //VGA/DVI/rgb HDMI is rgb input
            if(  IsSrcTypeVga(gSrcInfo[eWindow].enInputSourceType) //VGA
               ||IsSrcTypeDVI(gSrcInfo[eWindow].enInputSourceType) //DVI
               ||(  IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
                  &&(g_HdmiPollingStatus.bIsHDMIMode == TRUE)
                  &&(g_HdmiPollingStatus.u8ColorFormat == MS_HDMI_COLOR_RGB))//HDMI RGB
               ||(  IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
                  &&(g_HdmiPollingStatus.bIsHDMIMode == FALSE)) //HDMI_DVI
              )
            {
                bIsPNLYUVOutput = FALSE;
            }
            else
            {
                bIsPNLYUVOutput = TRUE;
            }
        }
    }
#endif
    return bIsPNLYUVOutput;
}

void HAL_SC_Set_LB_MergeAddress(void *pInstance)
{
}


static const MS_U16 tuning_max_pix_table[15] =
{
    0x2C0,0x2D0,0x2E0,0x300,0x310,0x320,0x328,0x330, //lv00~lv07
    0x338,0x340,0x350,0x360,0x380,0x3C0,0x480
};

MS_BOOL Hal_SC_Init(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    // Set IPM tune after DS
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, BIT(7), BIT(7));

    // Set YCbCr enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_01_L, BIT(3), BIT(3));

    // Enable 422to444 filter
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK18_01_L, 0, BIT(0));

    // VE separate XC bank for ve init
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_03_L, 0x3000); // H_linera mode
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_05_L, 0x1200); // src_max
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_45_L, BIT(0)|BIT(1)|BIT(2), BIT(0)|BIT(1)|BIT(2));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_45_L, BIT(10), BIT(10)); // V_linear mode
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_03_L, 0x0); // enable 128bits H-4tap
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_45_L, BIT(0)|BIT(8), BIT(0)|BIT(2)|BIT(8)|BIT(10)); // enable

    if(psXCInstPri->u32DeviceID == 0)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, BIT(11), BIT(11)); // cap_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_46_L, 0x0060, 0x00F0); // cap_stage

        MDrv_WriteByteMask((REG_CHIPTOP_BASE + 0x4A), 0x0, 0x1F);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK6A_05_L, 0xF00); // suggest by HW, defualt to 0xF00
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6A_20_L, 0x0, 0x8); // suggest by HW, reg_hs_mode must == 0 for SC0 to SC1

        //per HW, Kano is whole XC 2P mode
        //in any case(MVOP DE in, HDMI RX in) are all 2p
        //so we can set Image horizontal resolution 2P
        //reg_1p_to_2p_bypass_en to 1 always.
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x8000, 0x8000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x8000, 0x8000);
        //Init 2p mode for sub window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, 0x8000, 0x8000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_0A_L, 0x8000, 0x8000);

        //Per HW Bubu,
        //Kano no signal color need to set BK2F_20[4]
        //only disable input source for no signal color is useless
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_20_L, 0x0030, 0x0030);

        //Enable FCLK_TUNE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_01_L, 0x1, 0x1);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_20_L, tuning_max_pix_table[0], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_21_L, tuning_max_pix_table[1], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_22_L, tuning_max_pix_table[2], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_23_L, tuning_max_pix_table[3], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_24_L, tuning_max_pix_table[4], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_25_L, tuning_max_pix_table[5], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_26_L, tuning_max_pix_table[6], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_27_L, tuning_max_pix_table[7], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_28_L, tuning_max_pix_table[8], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_29_L, tuning_max_pix_table[9], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_2A_L, tuning_max_pix_table[10], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_2B_L, tuning_max_pix_table[11], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_2C_L, tuning_max_pix_table[12], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_2D_L, tuning_max_pix_table[13], 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_2E_L, tuning_max_pix_table[14], 0x1FFF);

        //Fclk tune patch, cannot set 0xFF to avoid clk off and ML can not run
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6C_17_L, 0xFE00, 0xFF00);

        //H2V2SD random shift eco bit
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK6A_22_L, 0x0002, 0x0002);


        // effiency adjust
        //IPM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_20_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_21_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_22_L, 0x1818);
        //IPS
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_60_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_61_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_62_L, 0x1818);
        //OPM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_23_L, 0x1010);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_24_L, 0x0020);

    }
    else // SC1
    {
        pXCResourcePrivate->sthal_SC.bSc1HvspFilterIsSet = FALSE; // For STR

        // effiency adjust
        //IPM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_20_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_21_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_22_L, 0x1818);
        //IPS
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_60_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_61_L, 0x1818);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_62_L, 0x1818);
        //OPM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_23_L, 0x1010);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_24_L, 0x0020);

        //IPMUX
        MDrv_WriteByteMask(REG_IPMUX_03_L, 0x00b0,0x00F0);

        //set bypass buf length for RFBL/FBL (fixing value)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_37_L, 0x00B4, 0x0FFF);

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
        // If use CMA and do not get CMA address (or FBL),
        // we must disable IPM write to avoid XC write to default address (0) and hit other module's buffer
        if(gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_SELF_MEM] == 0)
        {
            Hal_SC_IP_Memory_Write_Request(pInstance,DISABLE,MAIN_WINDOW);
        }
#endif
#endif

    }

    Hal_SC_set_nosignal_color(pInstance, 0x82, MAIN_WINDOW);
    Hal_SC_set_nosignal_color(pInstance, 0x82, SUB_WINDOW);
    Hal_XC_SetFrameColor(pInstance, 0x0);

    // default as handshake mode
    gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode = TRUE;
    gSrcInfo[SUB_WINDOW].Status2.bMVOPHSKMode = TRUE;
    return TRUE;
}

MS_U32 Hal_SC_Get_Device_Offset(MS_U8 deviceIdx)
{
    return _HAL_XC_DEVICE_OFFSET[deviceIdx];
}

MS_BOOL Hal_SC_3D_Is_LR_Sbs2Line(void *pInstance)
{
    if(SC_R2BYTEMSK(0, REG_SC_BK23_53_L, BIT(15)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void Hal_SC_3D_SetLRChgMode(void *pInstance, HAL_SC_3D_LRCHGMODE eLRChgMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, eLRChgMode, BIT(1)|BIT(0));
}

// 0: L is the first frame, 1: R is the first frame
void Hal_SC_3D_SetInitialLRIndex(void *pInstance, MS_BOOL bRFirst)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, (bRFirst==TRUE)?BIT(4):0, BIT(4));
}

// Split 1 frame into 2 frames
void Hal_SC_3D_SetSplitHalf(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, (bEnable==TRUE)?BIT(8):0, BIT(8));
}

void Hal_SC_3D_SetActiveVideoHeight(void *pInstance, MS_U16 u16VideoSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if _ENABLE_SW_DS
    if(MDrv_XC_GetDynamicScalingStatus(pInstance))
    {
        Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_65_L,u16VideoSize,DS_IP);
    }
    else
#endif
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_65_L, u16VideoSize, 0x1FFF);//v_active region
    }
}

// 0 for blank area 0
//v blanking between field1&field2 or field3&field4
void Hal_SC_3D_SetActiveBlankSize0(void *pInstance, MS_U16 u16BlankSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    #if _ENABLE_SW_DS
        if(MDrv_XC_GetDynamicScalingStatus(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_66_L,u16BlankSize,DS_IP);
        }
        else
    #endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_66_L, u16BlankSize, 0x1FFF);
        }
}

// 1 for blank area 1
//v blanking between field2&field3
void Hal_SC_3D_SetActiveBlankSize1(void *pInstance, MS_U16 u16BlankSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    #if _ENABLE_SW_DS
        if(MDrv_XC_GetDynamicScalingStatus(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_67_L,u16BlankSize,DS_IP);
        }
        else
    #endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_67_L, u16BlankSize, 0x1FFF);//v blanking between field1&field2 or field3&field4
        }
}

void Hal_SC_3D_SetActiveVideoWidth(void *pInstance, MS_U16 u16VideoSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16ActiveWidthUsrEnable = 0;
    if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_2F_L, BIT(15)) == BIT(15))
    {
        u16ActiveWidthUsrEnable = BIT(15);
    }

#if _ENABLE_SW_DS
    if(MDrv_XC_GetDynamicScalingStatus(pInstance))
    {
        Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_7F_L,u16VideoSize | u16ActiveWidthUsrEnable,DS_IP);
    }
    else
#endif
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_7F_L, u16VideoSize | u16ActiveWidthUsrEnable, 0x9FFF);//v_active region
    }
}


void Hal_SC_3D_SetActiveVideoHeightAfterVSD(void *pInstance, MS_U16 u16VideoSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16Enable = BIT(15);

#if _ENABLE_SW_DS
    if(MDrv_XC_GetDynamicScalingStatus(pInstance))
    {
        Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_38_L,u16VideoSize | u16Enable,DS_IP);
    }
    else
#endif
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_38_L, u16VideoSize | u16Enable, 0x9FFF);
    }
}

void Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(void *pInstance, MS_U16 u16VideoSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16Enable = BIT(15);

#if _ENABLE_SW_DS
    if(MDrv_XC_GetDynamicScalingStatus(pInstance))
    {
        Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK48_38_L,u16VideoSize | u16Enable,DS_IP);
    }
    else
#endif
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_38_L, u16VideoSize | u16Enable, 0x9FFF);
    }
}

void Hal_SC_3D_SetPixelSeparationWidth(void *pInstance, MS_U16 u16VideoSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16ActiveWidthUsrEnable = 0;
#if (HW_DESIGN_3D_VER >= 3)
    if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_2F_L, BIT(15)) == BIT(15))
    {
        u16ActiveWidthUsrEnable = BIT(15);
    }
#endif

#if _ENABLE_SW_DS
    if(MDrv_XC_GetDynamicScalingStatus(pInstance))
    {
        Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK02_6C_L,u16VideoSize | u16ActiveWidthUsrEnable,DS_IP);
    }
    else
#endif
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6C_L, u16VideoSize | u16ActiveWidthUsrEnable, 0x87FF);
    }
}

void Hal_SC_3D_InvertMemsyncInterlaceMode(void *pInstance)
{
}


#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
MS_U64 MHal_XC_Get_CMA_Addr(void *pInstance, XC_CMA_CLIENT enCmaClient, MS_U32 u32GetCMASize, SCALER_WIN eWindow)
{
    MS_U64 u64CMAAddr = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (u32GetCMASize == 0)
    {
        return u64CMAAddr;
    }

    struct CMA_Pool_Alloc_Param CMA_Pool_GetMem;
    CMA_Pool_GetMem.pool_handle_id = CMA_Pool_Init_PARAM[enCmaClient].pool_handle_id;
    CMA_Pool_GetMem.offset_in_pool = gSrcInfo[eWindow].Status2.stXCConfigCMA[enCmaClient].u64AddrHeapOffset;
    CMA_Pool_GetMem.flags = CMA_FLAG_VIRT_ADDR;
    if(CMA_Pool_Init_PARAM[enCmaClient].heap_length >= u32GetCMASize)
    {
        CMA_Pool_GetMem.length = u32GetCMASize;
    }
    else
    {
        SC_DBG(printf("\033[35m   Function = %s, Line = %d, CMA_POOL_INIT GetMem ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
        assert(0);
    }

    if (MApi_CMA_Pool_GetMem(&CMA_Pool_GetMem) == FALSE)
    {
        SC_DBG(printf("\033[35m   Function = %s, Line = %d, MApi_CMA_Pool_GetMem() fail!!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
    }
    u64CMAAddr = CMA_Pool_Init_PARAM[enCmaClient].heap_miu_start_offset;
    if(CMA_Pool_Init_PARAM[enCmaClient].miu==E_CHIP_MIU_1)
    {
        u64CMAAddr= CMA_Pool_Init_PARAM[enCmaClient].heap_miu_start_offset+HAL_MIU1_BASE;
    }
    else if(CMA_Pool_Init_PARAM[enCmaClient].miu==E_CHIP_MIU_2)
    {
        u64CMAAddr= CMA_Pool_Init_PARAM[enCmaClient].heap_miu_start_offset+HAL_MIU2_BASE;
    }

    return u64CMAAddr;
}

#if 0
static void _Hal_SC_Limit_WriteMem(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(eWindow == MAIN_WINDOW)
    {
        /// need to disable the r/w request, when change the write limit base
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(0)|BIT(1), BIT(0)|BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_38_L, BIT(0), BIT(0)); //F2 memory address limit enable
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_40_L, 0x00); //F2 memory min address
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_42_L, 0x00); //F2 memory max address
        /// need to enable the r/w request
        if(!gSrcInfo[MAIN_WINDOW].bFBL)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0x00, BIT(0)|BIT(1));
        }
        pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] = 0;
        pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow] = 0;
    }
}

static void _Hal_SC_Limit_Frcm_WriteMem(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_50_L, BIT(4), BIT(4)); //F2 memory address limit enable
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_64_L, 0x00); //F2 memory min address
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0D_66_L, 0x00); //F2 memory max address
        pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] = 0;
        pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow] = 0;
    }
}
#endif

MS_BOOL MHal_XC_Release_CMA(void *pInstance, XC_CMA_CLIENT enCmaClient, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    struct CMA_Pool_Free_Param stCMAFreeParam;
    if ((gSrcInfo[eWindow].u32PreCMAMemSCMSize[enCmaClient]+gSrcInfo[eWindow].u32PreCMAMemFRCMSize[enCmaClient])!=0)
    {
        stCMAFreeParam.pool_handle_id = CMA_Pool_Init_PARAM[enCmaClient].heap_id; //in: pool handle id, when pool init, returned by kernel
        stCMAFreeParam.offset_in_pool = gSrcInfo[eWindow].Status2.stXCConfigCMA[enCmaClient].u64AddrHeapOffset;
        stCMAFreeParam.length = gSrcInfo[eWindow].u32PreCMAMemSCMSize[enCmaClient]+gSrcInfo[eWindow].u32PreCMAMemFRCMSize[enCmaClient];
        if (MApi_CMA_Pool_PutMem(&stCMAFreeParam) == FALSE)
        {
            SC_DBG(printf("\033[35m   Function = %s, Line = %d, MApi_CMA_Pool_PutMem() fail!!!\033[m\n", __PRETTY_FUNCTION__, __LINE__));
        }
        gSrcInfo[eWindow].u32PreCMAMemSCMSize[enCmaClient] = 0;
        gSrcInfo[eWindow].u32PreCMAMemFRCMSize[enCmaClient] = 0;
        //Set Write limit here have flow issue? (release CMA enable write limit 0, But can not set again)
        //_Hal_SC_Limit_WriteMem(pInstance, eWindow);
        //_Hal_SC_Limit_Frcm_WriteMem(pInstance, eWindow);
    }

    return TRUE;
}

MS_BOOL MHal_XC_Get_CMA_UsingCondition(void *pInstance, MS_BOOL bIsGetCMABuff[], MS_U32 au32CMAMemSCMSize[],MS_U32 au32CMAMemFRCMSize[], MS_U32* pu32DualMiuMemSize, MS_U32 u32DataSize, SCALER_WIN eWindow)
{
    MS_U32 u32CMAMemSCMSize = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if (u32DataSize < CMA_XC_MEM_NUM)
    {
        return FALSE;
    }

    if(psXCInstPri->u32DeviceID == 0)
    {
        if(eWindow == MAIN_WINDOW || pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE)
        {
            if ((MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance))
                    && IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))
            {
                // MM DS case, use 60MB
                u32CMAMemSCMSize = XC_CMA_60MB;
            }
            else
            {
                MS_BOOL bHSK_md = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_10_L, BIT(0)|BIT(2)|BIT(5)) == 0)? FALSE: TRUE;
                if(bHSK_md != TRUE)
                {
                    //DE mode only RX and HDR case, Need 60MB
                    u32CMAMemSCMSize = XC_CMA_60MB;
                }
                else
                {
                    u32CMAMemSCMSize = XC_CMA_48MB;
                }
            }
        }
        else
        {
            u32CMAMemSCMSize = XC_CMA_30MB;
        }
    }
    else
    {
        u32CMAMemSCMSize = XC_CMA_14MB;
    }


#if 0
    if ((MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance))
        && IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))
    {
        // MM source and DS on
        if(gSrcInfo[eWindow].bInterlace)
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SAVE_MEM_MODE)
            {
                //DS ON , FHD case, 20bit/pilex , scm 4 frame and frcm only 2 frame to save mem
                u32CMAMemSCMSize = XC_CMA_10MB;
                u32CMAMemFRCMSize = XC_CMA_20MB;
            }
            else
            {
                //DS ON , FHD case
                u32CMAMemSCMSize = XC_CMA_48MB;
                u32CMAMemFRCMSize = XC_CMA_24MB;
            }
        }
        else
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SAVE_MEM_MODE)
            {
                //DS ON , 4K2K case, 16bit/pilex , frcm only 2 frame to save mem
                u32CMAMemSCMSize = 0;
                u32CMAMemFRCMSize = XC_CMA_32MB;
            }
            else
            {
                //DS ON , 4K2K case
                u32CMAMemSCMSize = 0;
                u32CMAMemFRCMSize = XC_CMA_96MB;
            }
        }
    }
    else if (  (IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))
             &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SAVE_MEM_MODE))
    {
        if((gSrcInfo[eWindow].stCapWin.width>=XC_4K2K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K2K_HIGH_MIN))
        {
            //4K2K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_32MB;
        }
        else if((gSrcInfo[eWindow].stCapWin.width>=XC_2K2K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_2K2K_HIGH_MIN))
        {
            //2K2K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_32MB;
        }
        else if((gSrcInfo[eWindow].stCapWin.width>=XC_4K1K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K1K_HIGH_MIN))
        {
            //4K1K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_16MB;
        }
        else if((gSrcInfo[eWindow].stCapWin.width>=XC_4K_HALFK_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K_HALFK_HIGH_MIN))
        {
            //4K0.5K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_8MB;
        }
        else
        {
            //FHD case
            u32CMAMemSCMSize = XC_CMA_20MB;
            u32CMAMemFRCMSize = XC_CMA_15MB;
        }
    }
    else if (  (IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType))
             &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SAVE_MEM_MODE))
    {
        if((gSrcInfo[eWindow].stCapWin.width>=XC_4K2K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K2K_HIGH_MIN))
        {
            //4K2K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_32MB;
        }
        else
        {
            //FHD case
            u32CMAMemSCMSize = XC_CMA_20MB;
            u32CMAMemFRCMSize = XC_CMA_15MB;
        }
    }
    else
    {
        if((gSrcInfo[eWindow].stCapWin.width>=XC_4K2K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K2K_HIGH_MIN))
        {
            //4K2K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_96MB;
        }
        else if((gSrcInfo[eWindow].stCapWin.width>=XC_2K2K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_2K2K_HIGH_MIN))
        {
            //2K2K case
            u32CMAMemSCMSize = XC_CMA_48MB;
            u32CMAMemFRCMSize = XC_CMA_48MB;
        }
        else if((gSrcInfo[eWindow].stCapWin.width>=XC_4K1K_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K1K_HIGH_MIN))
        {
            //4K1K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_48MB;
        }
        else if((gSrcInfo[eWindow].stCapWin.width>=XC_4K_HALFK_WIDTH_MIN)&&(gSrcInfo[eWindow].stCapWin.height>=XC_4K_HALFK_HIGH_MIN))
        {
            //4K0.5K case
            u32CMAMemSCMSize = 0;
            u32CMAMemFRCMSize = XC_CMA_24MB;
        }
        else
        {
            //FHD case
            u32CMAMemSCMSize = XC_CMA_48MB;
            u32CMAMemFRCMSize = XC_CMA_24MB;
        }
    }
#endif

    if (gSrcInfo[eWindow].Status2.stXCConfigCMA[CMA_XC_COBUFF_MEM].u32HeapID == XC_INVALID_HEAP_ID)
    {
        bIsGetCMABuff[CMA_XC_SELF_MEM] = TRUE;
        bIsGetCMABuff[CMA_XC_COBUFF_MEM] = FALSE;
        au32CMAMemSCMSize[CMA_XC_SELF_MEM] = u32CMAMemSCMSize;
        au32CMAMemFRCMSize[CMA_XC_SELF_MEM] = 0;
        au32CMAMemFRCMSize[CMA_XC_COBUFF_MEM] = 0;
        au32CMAMemSCMSize[CMA_XC_COBUFF_MEM] = 0;
    }
    else
    {
        if (IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType) ||
            pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE)
        {
            bIsGetCMABuff[CMA_XC_SELF_MEM] = TRUE;
            bIsGetCMABuff[CMA_XC_COBUFF_MEM] = FALSE;
            au32CMAMemSCMSize[CMA_XC_SELF_MEM] = u32CMAMemSCMSize;
            au32CMAMemFRCMSize[CMA_XC_SELF_MEM] = 0;
            au32CMAMemFRCMSize[CMA_XC_COBUFF_MEM] = 0;
            au32CMAMemSCMSize[CMA_XC_COBUFF_MEM] = 0;
        }
        else
        {
            bIsGetCMABuff[CMA_XC_SELF_MEM] = FALSE;
            bIsGetCMABuff[CMA_XC_COBUFF_MEM] = TRUE;
            au32CMAMemSCMSize[CMA_XC_SELF_MEM] = 0;
            au32CMAMemFRCMSize[CMA_XC_SELF_MEM] = 0;
            au32CMAMemFRCMSize[CMA_XC_COBUFF_MEM] = 0;
            au32CMAMemSCMSize[CMA_XC_COBUFF_MEM] = u32CMAMemSCMSize;
        }
    }
    *pu32DualMiuMemSize = 0;

    return TRUE;
}

static void  MHal_XC_CMAPatchISR(SC_INT_SRC eIntNum, void * pParam)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));


    if(((MsOS_GetSystemTime()- u32CMAPatchTimer_Main)>6000))
    {
        u32CMAPatchTimer_Main = MsOS_GetSystemTime();
    }

    if(  (MDrv_XC_IsInputSourceDisabled(pInstance, MAIN_WINDOW)==TRUE)
       &&(  ((gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_SELF_MEM]+gSrcInfo[MAIN_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_SELF_MEM])!=0)
          ||((gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_COBUFF_MEM]+gSrcInfo[MAIN_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_COBUFF_MEM])!=0)))
    {
        //Check timing 5s
        if(((MsOS_GetSystemTime()- u32CMAPatchTimer_Main)>5000))
        {
            u32CMAPatchTimer_Main = MsOS_GetSystemTime();

            Hal_SC_IP_Memory_Write_Request(pInstance, DISABLE, MAIN_WINDOW);
            Hal_SC_set_freezeimg(pInstance, ENABLE, MAIN_WINDOW);
#if (HW_DESIGN_4K2K_VER == 4)
            Hal_SC_frcmw_Memory_Write_Request(pInstance, DISABLE, MAIN_WINDOW);
#endif
            MHal_XC_Release_CMA(pInstance, CMA_XC_SELF_MEM, MAIN_WINDOW);
            MHal_XC_Release_CMA(pInstance, CMA_XC_COBUFF_MEM, MAIN_WINDOW);
            Hal_SC_set_freezeimg(pInstance, DISABLE, MAIN_WINDOW);
        }
    }
    else
    {
        u32CMAPatchTimer_Main = MsOS_GetSystemTime();
    }
}

void MHal_XC_CMAPatch(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(eWindow ==MAIN_WINDOW)
    {
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT,MHal_XC_CMAPatchISR, (void *)NULL))
        {
            //Start CMA  patch, enable ISR
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT,MHal_XC_CMAPatchISR, (void *)NULL);
        }
    }
}

void MHal_XC_CMAPatchClose(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(eWindow ==MAIN_WINDOW)
    {
        if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, MHal_XC_CMAPatchISR, (void *)NULL))
        {
            //Disable ISR
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, MHal_XC_CMAPatchISR, (void *)NULL);
        }
    }
}
#endif
#endif


E_APIXC_ReturnValue Hal_SC_SetOSDDetect(void *pInstance, MS_BOOL bEnable, MS_U32 Threhold)
{
    return E_APIXC_RET_FAIL;
}

E_APIXC_ReturnValue Hal_SC_GetOSDDetect(void *pInstance, MS_BOOL *pbOSD)
{
    return E_APIXC_RET_FAIL;
}

void Hal_SC_SetHDMI_Spliter(void *pInstance,XC_IP_SYNC_STATUS *sXC_Sync_Status,E_MUX_INPUTPORT enInputPort ,SCALER_WIN eWindow)
{

#if SUPPORT_HDMI20
	XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16HDE_temp_size,u16HDE_size =0;

        if((sXC_Sync_Status->u8SyncStatus & XC_MD_SYNC_LOSS) != XC_MD_SYNC_LOSS)
        {
            if(MDrv_HDMI_CheckHDMI20_Setting(enInputPort))
            {
                //For RX HDR case, do not set SC0 mux from HDMI here.
                //printf("[%s][%d]HDMI 4K2K 2P mode\n", __FUNCTION__, __LINE__);
                //if(eWindow == MAIN_WINDOW)
                //    W2BYTEMSK(0x102e02,0x0010,BMASK(7:4)); // ipmux sel=0xb
                //else if(eWindow == SUB_WINDOW)
                //    W2BYTEMSK(0x102e02,0x0001,BMASK(3:0)); // ipmux sel=0xb
            }
            else
            {
                //SUPPORT_HDMI20_420
                //printf("[%s][%d]HDMI 4K2K 2P mode\n", __FUNCTION__, __LINE__);
                //printf("[%s][%d]sXC_Sync_Status->u16Vtotal=%x, sXC_Sync_Status->u16Hperiod=%x\n",
                //    __FUNCTION__, __LINE__, sXC_Sync_Status->u16Vtotal, sXC_Sync_Status->u16Hperiod);
                if((sXC_Sync_Status->u16Vtotal >= 0x8C0)
                 &&(sXC_Sync_Status->u16Hperiod>= 0x50))
                {
                    if((MDrv_HDMI_avi_infoframe_info(_BYTE_1)& 0x60) == 0x60)
                    {
                        //printf("@@#@@4K2K@60 420\n");
                        // clkgen2
                        W2BYTE(0x100a80,0x0004); // spt_l0 1: hdmi d: mhl
                        // ipmux
                        if(eWindow == MAIN_WINDOW)
                        {
                            W2BYTEMSK(0x102e02,0x00b0,BMASK(7:4)); // ipmux sel=0xb
                        }
                        else if(eWindow == SUB_WINDOW)
                        {
                            W2BYTEMSK(0x102e02,0x000b,BMASK(3:0)); // ipmux sel=0xb
                        }

                        //BK40_SPT4k
                        SC_W2BYTE(0, REG_SC_BK40_01_L,0x0000); // [3:0]  reg_spt_ipmux_sel0
                                                            // [11:8] reg_spt_ipmux_sel1
                        SC_W2BYTE(0, REG_SC_BK40_03_L,0x0001); // [7] reg_spt_2p_oe_en [0] reg_spt_420_en
                        SC_W2BYTE(0, REG_SC_BK40_04_L,0x0180); // [7] reg_spt_2p_oe_out_en [9] hsd_en
                                                            // [8] reg_spt_2poe_out_sel 1'b0: dual ch eo in
                                                            //                                    1'b1: dual ch LR in or 420
                        SC_W2BYTE(0, REG_SC_BK40_30_L, 0x0003); // de_only
                        SC_W2BYTE(0, REG_SC_BK40_0B_L, 0x0000); // gb_size

                        u16HDE_temp_size = MDrv_HDMI_GetHDE();
                        //printf("[%s][%d]u16HDE_size=%u\n", __FUNCTION__, __LINE__,u16HDE_temp_size);
                        if((u16HDE_temp_size >= 2028)&&(u16HDE_temp_size<= 2068))
                        {
                            u16HDE_size = 2048;
                        }
                        else //if((u16HDE_temp_size >= 1900)&&(u16HDE_temp_size<= 1940))
                        {
                            u16HDE_size = 1920;
                        }
                        SC_W2BYTE(0, REG_SC_BK40_08_L, u16HDE_size); // de_in_size
                        SC_W2BYTE(0, REG_SC_BK40_0A_L, u16HDE_size); // de_out_with_gb_size

                        SC_W2BYTE(0, REG_SC_BK40_20_L, 0x0000); // ra
                        SC_W2BYTE(0, REG_SC_BK40_21_L, u16HDE_size/2-2); //
                        SC_W2BYTE(0, REG_SC_BK40_22_L, 0x0001);
                        SC_W2BYTE(0, REG_SC_BK40_23_L, u16HDE_size/2-1);

                        SC_W2BYTE(0, REG_SC_BK40_24_L, 0x0000); // wa
                        SC_W2BYTE(0, REG_SC_BK40_25_L, u16HDE_size/2-2);
                        SC_W2BYTE(0, REG_SC_BK40_26_L, 0x0001);
                        SC_W2BYTE(0, REG_SC_BK40_27_L, u16HDE_size/2-1);
                    }
                    else
                    {
                        //printf("@@#@@4K2K@30\n");
                        /// for 4K2K@30Hz
                        if(eWindow == MAIN_WINDOW)
                        {
                            W2BYTEMSK(0x102e02,0x0010,BMASK(7:4)); // ipmux sel=0xb
                        }
                        else if(eWindow == SUB_WINDOW)
                        {
                            W2BYTEMSK(0x102e02,0x0001,BMASK(3:0)); // ipmux sel=0xb
                        }
                    }
                }
            }
        }
#endif

}

// Temp function for SC1 to load MADi settings
MS_U8 Hal_SC1_LoadMADi(void *pInstance, SCALER_WIN eWindow, MS_U8 *pu8BitPerPixel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bInt = gSrcInfo[eWindow].bInterlace;

    if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, (bInt?0x2500:0x2100), 0x770D);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, (bInt?0x6000:0x2000), 0xE000);
        *pu8BitPerPixel = 24;
        return (bInt?MS_DEINT_3DDI_HISTORY:MS_DEINT_OFF);
    }

    return 0;
}

void Hal_SC_3D_enable_FALLRR_out(void *pInstance, MS_BOOL bEnable)
{
    return;
}

void Hal_SC_3D_Adjust_FP_PreHscaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode != E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
        *pb3DPreHScaling = TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = MAX(640, (MS_U32)MIN(1920, pSrcInfo->stDispWin.width) * 2 / 3);
    }
    else// FA out
    {
        if((pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA) && (pSrcInfo->stCapWin.width == DOUBLEHD_1080X2P_HSIZE))//output FA_LLRR for 4k0.5k@240Hz
        {
            *pb3DPreHScaling = TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = MAX(640, (MS_U32)MIN(1920, pSrcInfo->stDispWin.width) / 2);
        }
    }
}

void Hal_SC_3D_Adjust_FA_PreHscaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    *pb3DPreHScaling = TRUE;
    pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width >= 2 * pSrcInfo->stDispWin.width)
    {
        pSrcInfo->Status2.u16PreHCusScalingDst = MIN(960, pSrcInfo->stDispWin.width);
    }
    else
    {
        pSrcInfo->Status2.u16PreHCusScalingDst = MIN(1280, pSrcInfo->stDispWin.width);
    }
}

void Hal_SC_3D_Adjust_PreHVscaling_SaveBW(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling,MS_BOOL *pb3DPreVScaling,MS_BOOL bForcePreHScalingDown,MS_BOOL bForcePreVScalingDown,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_FRAME_PACKING))
    {
        if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode != E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
        {
            *pb3DPreHScaling = TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = MAX(640, (MS_U32)MIN(1920, pSrcInfo->stDispWin.width) * 2 / 3);
        }
        else// FA out
        {
            if((pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA) && (pSrcInfo->stCapWin.width == DOUBLEHD_1080X2P_HSIZE))//output FA_LLRR for 4k0.5k@240Hz
            {
                *pb3DPreHScaling = TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = MAX(640, (MS_U32)MIN(1920, pSrcInfo->stDispWin.width) / 2);
            }
        }
    }
    else if((pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_FRAME_ALTERNATIVE) && (pXCResourcePrivate->stdrvXC_3D._eOutput3DMode != E_XC_3D_OUTPUT_FRAME_ALTERNATIVE) &&(pSrcInfo->stCapWin.width > 1280)
            && (MDrv_ReadRegBit(REG_CKG_FCLK, CKG_FCLK_MASK) < 24))//fclk < 320MHz may have fclk issue for fi
    {
        *pb3DPreHScaling = TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width >= 2 * pSrcInfo->stDispWin.width)
        {
            pSrcInfo->Status2.u16PreHCusScalingDst = MIN(960, pSrcInfo->stDispWin.width);
        }
        else
        {
            pSrcInfo->Status2.u16PreHCusScalingDst = MIN(1280, pSrcInfo->stDispWin.width);
        }
    }
    else
    {
        if(bForcePreHScalingDown)
        {
                *pb3DPreHScaling = FALSE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->u16H_SizeAfterPreScaling;
        }

        if(bForcePreVScalingDown)
        {
            pb3DPreVScaling = FALSE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->u16V_SizeAfterPreScaling;
        }
    }
}

MS_BOOL Hal_SC_CheckMuteStatusByRegister(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bRet = FALSE;

    if(SUB_WINDOW == eWindow)
    {
        bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_20_L, BIT(2)) ? TRUE: FALSE);
    }
    else if(MAIN_WINDOW == eWindow)
    {
        bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2F_20_L, BIT(1)) ? TRUE: FALSE);
    }
    return bRet;
}

#ifdef CONFIG_MSTAR_SRAMPD
MS_BOOL Hal_SC_Power_OnOff(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(bEnable)
    {
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            SC_DBG(printf("[%s][%d] XC sram power on in E_XC_DEVICE0\n", __FUNCTION__, __LINE__));
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x10*2*2)) &= (0x00000000);
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x12*2*2)) &= (0xFC000000);
            MsOS_DelayTask(1);
        }
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            SC_DBG(printf("[%s][%d] XC sram power on in E_XC_DEVICE1\n", __FUNCTION__, __LINE__));
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x12*2*2)) &= (0x3FFFFFF);
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x14*2*2)) &= (~0xFFFFF);
            MsOS_DelayTask(1);
        }
    }
    else
    {
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            SC_DBG(printf("[%s][%d] XC sram power off in E_XC_DEVICE0\n", __FUNCTION__, __LINE__));
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x10*2*2)) |= (0xFFFFFFFF);
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x12*2*2)) |= (0x3FFFFFF);
            MsOS_DelayTask(1);
            }
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            SC_DBG(printf("[%s][%d] XC sram power off in E_XC_DEVICE1\n", __FUNCTION__, __LINE__));
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x12*2*2)) |= (0xFC000000);
            *((volatile MS_U32 *)(MS_VIRT)(_XC_RIU_BASE + 0x71400*2 + 0x14*2*2)) |= (0xFFFFF);
            MsOS_DelayTask(1);
        }
    }
    return TRUE;
}
#endif

void Hal_SC_add_dram_to_shm(void *pInstance,SCALER_WIN eWindow,MS_PHY u32address,MS_U32 u32length)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_cnt].address = u32address;
    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_cnt].length = u32length;
    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_cnt++;
}
void Hal_SC_add_reg_to_shm(void *pInstance,SCALER_WIN eWindow,MS_U32 u32_bk,MS_U16 u16_value,MS_U16 u16_mask)
{
    MS_U32 u32offset = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u32offset = _XC_Device_Offset[psXCInstPri->u32DeviceID];

    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_data[pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_cnt].bk = REG_SCALER_BASE + u32_bk + (u32offset << 8);
    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_data[pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_cnt].value = u16_value;
    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_data[pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_cnt].mask = u16_mask;
    pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].regs_cnt ++;
}

MS_BOOL Hal_SC_secure_lock(void *pInstance, SCALER_WIN eWindow, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
#ifdef MSOS_TYPE_OPTEE
    MS_PHY phy_dramAddr = 0;
    MS_U8 u8i = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    #ifndef MSOS_TYPE_OPTEE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    #else
    XC_RESOURCE_PRIVATE_FOR_TEE* pXCResourcePrivate = NULL;
    #endif
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    #ifdef MSOS_TYPE_OPTEE

    if (u32SecureDMA == 0)
    {
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_XC, 0 , FALSE);
    }

    for (u8i = 0; u8i < pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_cnt; u8i++)
    {
        phy_dramAddr = (MS_PHY) pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[u8i].address;
        if (u8i >= OP_TEE_XC_DRAM_MAX_CNT)
        {
            printf("dram cnt overflow\n");
            return FALSE;
        }
        if (u32SecureDMA == 1)
        {
            if(MDrv_Seal_SetSecureRange(phy_dramAddr, pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[u8i].length , TRUE) == FALSE)
            {
                return FALSE;
            }
        }
        else if (u32SecureDMA == 0)
        {
            if(MDrv_Seal_SetSecureRange(phy_dramAddr , pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[u8i].length , FALSE) == FALSE)
            {
                return FALSE;
            }
        }
    }

    if (u32SecureDMA == 1)
    {
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_XC, 0 , TRUE);
    }

    if (u32OperationMode == 1)
    {
        MDrv_Seal_SecureSlaveSet(E_SEAL_SC0_PROT_NONPM, TRUE);
        MDrv_Seal_BufferLock(E_SEAL_SC_WP_SCM_M, TRUE);
    }
    else if (u32OperationMode == 0)
    {
        MDrv_Seal_SecureSlaveSet(E_SEAL_SC0_PROT_NONPM, FALSE);
        MDrv_Seal_BufferLock(E_SEAL_SC_WP_SCM_M, FALSE);
    }
    #endif
    return TRUE;
}

void Hal_SC_update_to_shm(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (eWindow == MAIN_WINDOW)
    {
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK0D_38_L, 0, BIT(0));
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK12_08_L, ((pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[0].address/BYTE_PER_WORD) & 0xFFFF), 0xFFFF);
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK12_09_L, ((pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[0].address/BYTE_PER_WORD) >> 16), 0xFFFF);
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK12_1C_L, 0x0000, 0xFFFF);
    }
    else if (eWindow == SUB_WINDOW)
    {
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK0D_38_L, 0, BIT(1));
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK12_48_L, ((pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[0].address/BYTE_PER_WORD) & 0xFFFF), 0xFFFF);
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK12_49_L, ((pXCResourcePrivate->sthal_Optee.op_tee_xc[eWindow].drams_data[0].address/BYTE_PER_WORD) >> 16), 0xFFFF);
        Hal_SC_add_reg_to_shm(pInstance, eWindow, REG_SC_BK12_5C_L, 0x0000, 0xFFFF);
    }
}

#ifdef UFO_XC_AUTO_DOWNLOAD
static E_APIXC_ReturnValue _Hal_SC_Set_Auto_Download_WorkMode(MS_U32 u32DeviceID, MS_U32 u32RegAddr, MS_U8 u8Bit, EN_XC_AUTODOWNLOAD_MODE enMode)
{
    switch (enMode)
    {
        case E_XC_AUTODOWNLOAD_TRIGGER_MODE:
        {
            SC_W2BYTEMSK(u32DeviceID, u32RegAddr, 0, BIT(u8Bit));
            break;
        }
        case E_XC_AUTODOWNLOAD_ENABLE_MODE:
        {
            SC_W2BYTEMSK(u32DeviceID, u32RegAddr, 1 << u8Bit, BIT(u8Bit));
            break;
        }
        default:
            return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MHal_XC_AutoDownload_Config(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_PHY phyBaseAddr, EN_XC_AUTODOWNLOAD_MODE enMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    switch(enClient)
    {
        case E_XC_AUTODOWNLOAD_CLIENT_HDR:
        {
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32StartAddr = 0xFFFFFFFF;
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32Depth = 0;
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].enMode = enMode;

            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_29_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_2A_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_28_L, 1, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_02_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_03_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 3, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_12_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_13_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 6, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_XVYCC:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_16_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_17_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 15, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE1:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_04_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_05_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 4, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE2:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_04_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_05_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 4, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE3:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_04_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_05_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 4, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_DEMURA:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_06_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_07_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 5, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2LUT:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_23_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_24_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_22_L, 1, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_0:
        {
            //set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_33_L, phyBaseAddr & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_34_L, (phyBaseAddr >>16 & 0x0000FFFF), 0x07FF);

            //set work mode
            _Hal_SC_Set_Auto_Download_WorkMode(psXCInstPri->u32DeviceID, REG_SC_BK67_32_L, 1, enMode);
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_1:
        {
            return E_APIXC_RET_FAIL;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM:
        {
            return E_APIXC_RET_FAIL;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FOOPM:
        {
            return E_APIXC_RET_FAIL;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_MAX:
        default:
        {
            return E_APIXC_RET_FAIL;
        }

    }

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MHal_XC_AutoDownload_Fire(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    switch(enClient)
    {
        case E_XC_AUTODOWNLOAD_CLIENT_HDR:
        {
            MS_PHY phyBaseAddr = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].phyBaseAddr;
            MS_U32 u32Depth = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32Depth;
            MS_VIRT* pVirtBaseAddr = (MS_VIRT *)MS_PA2KSEG1(phyBaseAddr);
            MS_U8 *pu8BaseAddr = (MS_U8 *)pVirtBaseAddr;
            MS_PHY u32StartAddr = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32StartAddr;
            pu8BaseAddr += 16 * u32StartAddr;

            // set baseaddr
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_29_L, (MS_PHY)MS_VA2PA((MS_VIRT)pu8BaseAddr) & 0x0000FFFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_2A_L, (MS_PHY)(MS_VA2PA((MS_VIRT)pu8BaseAddr) >> 16 & 0x0000FFFF), 0x07FF);

            // set depth
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_2B_L, u32Depth);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_2C_L, u32Depth);

            // enable auto download
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_28_L, 1, BIT(0));

            EN_XC_AUTODOWNLOAD_MODE enMode = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].enMode;
            if (enMode == E_XC_AUTODOWNLOAD_TRIGGER_MODE)
            {
                //check auto download done
                MS_U32 u32Delayms = 0;
                while(((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK67_28_L) & 0x0001) != 0) && u32Delayms < 500)
                {
                    MsOS_DelayTask(1);
                    u32Delayms++;
                }

                // clear buffer
                unsigned int i = 0;
                for (i = 0; i < u32Depth; i++)
                {
                    CLEAR_HDR_DATA_FORMAT_1(pu8BaseAddr + i * 16);
                }
                if (u32Delayms >= 500)
                {
                    printf("Check auto download done timeout.\n");
                }
            }

            pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32StartAddr = 0xFFFFFFFF;
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32Depth = 0;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 1, BIT(0));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_11_L, 1, BIT(0));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_XVYCC:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_11_L, 1 << 2, BIT(2));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE1:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 1 << 1, BIT(1));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE2:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_21_L, 1, BIT(0));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE3:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_21_L, 1 << 1, BIT(1));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_DEMURA:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_01_L, 1 << 2, BIT(2));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2LUT:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_22_L, 1, BIT(0));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_0:
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK67_32_L, 1, BIT(0));
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_1:
        {
            return E_APIXC_RET_FAIL;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM:
        {
            return E_APIXC_RET_FAIL;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FOOPM:
        {
            return E_APIXC_RET_FAIL;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_MAX:
        default:
        {
            return E_APIXC_RET_FAIL;
        }

    }

    return E_APIXC_RET_OK;
}

static E_APIXC_ReturnValue _Hal_SC_Auto_Download_Format_Hdr_Data(void* pInstance,
    EN_XC_AUTODOWNLOAD_SUB_CLIENT enSubClient, MS_U8* pu8Data, MS_U32 u32Size, MS_U16 u16StartAddr)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_PHY phyBaseAddr = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].phyBaseAddr;
    MS_U32 u32Depth = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32Depth;
    MS_VIRT* pVirtBaseAddr = (MS_VIRT *)MS_PA2KSEG1(phyBaseAddr);
    unsigned int i = 0;
    MS_PHY u32Index = u16StartAddr;
    MS_U32 u32WriteNum = 0;
    MS_U8 *pu8BaseAddr = (MS_U8 *)pVirtBaseAddr;
    pu8BaseAddr += 16 * u16StartAddr;

    // update start addr of DRAM
    MS_PHY u32StartAddr = pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32StartAddr;
    if (u32StartAddr == 0xFFFFFFFF)
    {
        pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32StartAddr = u16StartAddr;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32StartAddr = u32StartAddr > u16StartAddr ? u16StartAddr : u32StartAddr;
    }

    //format&write datas into DRAM
    for (i = 0; i < u32Size; )
    {
        switch (enSubClient)
        {
            case E_XC_AUTODOWNLOAD_HDR_SUB_TMO:
            {
                MS_U16 u16Value = *(((MS_U16 *)pu8Data) + i);
                if (u16Value & 0xF000)
                {
                    printf("The %dth data is 0x%x, exceed max value, please check!!!\n", i + 1, u16Value);
                    return E_APIXC_RET_FAIL_INVALID_PARAMETER;
                }
                WRITE_TMO_DATA_FORMAT_1(pu8BaseAddr, u32Index, u16Value);
                i++;
                break;
            }
            case E_XC_AUTODOWNLOAD_HDR_SUB_GAMMA:
            {
                MS_U16 u16Value = *(((MS_U16 *)pu8Data) + i);
                WRITE_GAMMA_DATA_FORMAT_1(pu8BaseAddr, u32Index, u16Value);
                i++;
                break;
            }
            case E_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA:
            {
                MS_U32 u32Value = *(((MS_U32 *)pu8Data) + i);
                if (u32Value & 0xFFF80000)
                {
                    printf("The %dth data is 0x%x, exceed max value, please check!!!\n", i + 1, (unsigned int)u32Value);
                    return E_APIXC_RET_FAIL_INVALID_PARAMETER;
                }
                WRITE_DEGAMMA_DATA_FORMAT_1(pu8BaseAddr, u32Index, u32Value);
                i++;
                break;
            }
            case E_XC_AUTODOWNLOAD_HDR_SUB_3DLUT:
            {
                MS_U16 u16RVal = *(((MS_U16 *)pu8Data) + i);
                MS_U16 u16GVal = *(((MS_U16 *)pu8Data) + i + 1);
                MS_U16 u16BVal = *(((MS_U16 *)pu8Data) + i + 2);
                MS_U16 u16Subindex = 0;
                if(i < _au32_3dlut_entry_num[0])
                {
                    u16Subindex = 0;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1]))
                {
                    u16Subindex = 1;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1] + _au32_3dlut_entry_num[2]))
                {
                    u16Subindex = 2;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1] + _au32_3dlut_entry_num[2] + _au32_3dlut_entry_num[3]))
                {
                    u16Subindex = 3;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1] + _au32_3dlut_entry_num[2] + _au32_3dlut_entry_num[3] + _au32_3dlut_entry_num[4]))
                {
                    u16Subindex = 4;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1] + _au32_3dlut_entry_num[2] + _au32_3dlut_entry_num[3] + _au32_3dlut_entry_num[4] + _au32_3dlut_entry_num[5]))
                {
                    u16Subindex = 5;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1] + _au32_3dlut_entry_num[2] + _au32_3dlut_entry_num[3] + _au32_3dlut_entry_num[4] + _au32_3dlut_entry_num[5] + _au32_3dlut_entry_num[6]))
                {
                    u16Subindex = 6;
                }
                else if(i < (_au32_3dlut_entry_num[0] + _au32_3dlut_entry_num[1] + _au32_3dlut_entry_num[2] + _au32_3dlut_entry_num[3] + _au32_3dlut_entry_num[4] + _au32_3dlut_entry_num[5] + _au32_3dlut_entry_num[6] + _au32_3dlut_entry_num[7]))
                {
                    u16Subindex = 7;
                }
                WRITE_3DLUT_DATA_FORMAT_1(pu8BaseAddr, u32Index, u16Subindex, u16RVal, u16GVal, u16BVal);
                i += 3;
                break;
            }
        }
        pu8BaseAddr += 16;
        u32WriteNum++;
        u32Index++;
    }

    u32Depth = u32Depth > u32WriteNum ? u32Depth : u32WriteNum;
    pXCResourcePrivate->stdrvXC_Display._stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_HDR].u32Depth = u32Depth;

#if DEBUG_HDR
    int j = 0;
    int l = 0;

    printf("\n#####################Dump Input Data####################\n");
    for (j = 0; j < u32Size;)
    {
        printf("The %04dth row: ", u16StartAddr++);
        switch (enSubClient)
        {
            case E_XC_AUTODOWNLOAD_HDR_SUB_TMO:
            {
                MS_U16 u16Value = *(((MS_U16 *)pu8Data) + j);
                printf("%02X \n", u16Value);
                j++;
                break;
            }
            case E_XC_AUTODOWNLOAD_HDR_SUB_GAMMA:
            {
                MS_U16 u16Value = *(((MS_U16 *)pu8Data) + j);
                printf("%02X \n", u16Value);
                j++;
                break;
            }
            case E_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA:
            {
                MS_U32 u32Value = *(((MS_U32 *)pu8Data) + j);
                printf("%04X \n", u32Value);
                j++;
                break;
            }
            case E_XC_AUTODOWNLOAD_HDR_SUB_3DLUT:
            {
                MS_U16 u16RVal = *(((MS_U16 *)pu8Data) + j);
                MS_U16 u16GVal = *(((MS_U16 *)pu8Data) + j + 1);
                MS_U16 u16BVal = *(((MS_U16 *)pu8Data) + j + 2);
                printf("%02X %02X %02X \n", u16RVal, u16GVal, u16BVal);
                j += 3;
                break;
            }
        }
        printf("\n");
    }
    printf("\n#####################Dump End####################\n\n");

    int k = AUTO_DOWNLOAD_HDR_TMO_SRAM_MAX_ADDR;
    printf("\n#####################Dump DRAM Buffer####################\n");
    for (j = 0; j < k; j++)
    {
        MS_U8 *pu8BaseAddr = (MS_U8 *)pVirtBaseAddr;
        printf("\nThe %04dth row: ", j);
        for (l = 0; l < 16; l++)
        {
            switch (enSubClient)
            {
                case E_XC_AUTODOWNLOAD_HDR_SUB_TMO:
                case E_XC_AUTODOWNLOAD_HDR_SUB_GAMMA:
                case E_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA:
                {
                    printf("%02X ", *(pu8BaseAddr + 16 * j + l));
                    break;
                }
                case E_XC_AUTODOWNLOAD_HDR_SUB_3DLUT:
                {
                    printf("%02X ", *(pu8BaseAddr + 16 * j + l));
                    k = AUTO_DOWNLOAD_HDR_3DLUT_SRAM_MAX_ADDR;
                    break;
                }
            }
        }
    }
    printf("\n#####################Dump End####################\n");
#endif

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MHal_XC_AutoDownload_Write(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_U8* pu8Data, MS_U32 u32Size, void* pParam)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    switch(enClient)
    {
        case E_XC_AUTODOWNLOAD_CLIENT_HDR:
        {
            XC_AUTODOWNLOAD_FORMAT_INFO* pstFormatInfo = (XC_AUTODOWNLOAD_FORMAT_INFO *)pParam;
            MS_PHY u16StartAddr = 0;
            MS_U32 u32MaxSize = 0;
            switch (pstFormatInfo->enSubClient)
            {
                case E_XC_AUTODOWNLOAD_HDR_SUB_TMO:
                case E_XC_AUTODOWNLOAD_HDR_SUB_GAMMA:
                case E_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA:
                {
                    u32MaxSize = AUTO_DOWNLOAD_HDR_TMO_SRAM_MAX_ADDR;
                    break;
                }
                case E_XC_AUTODOWNLOAD_HDR_SUB_3DLUT:
                {
                    u32MaxSize = AUTO_DOWNLOAD_HDR_3DLUT_SRAM_MAX_ADDR * 3;
                    break;
                }
                default:
                {
                    printf("Write auto download fail, invaild paramters, subClient: %d\n", pstFormatInfo->enSubClient);
                    return E_APIXC_RET_FAIL_INVALID_PARAMETER;
                }
            }
            if (pstFormatInfo->bEnableRange == TRUE)
            {
                if (pstFormatInfo->u16StartAddr <= pstFormatInfo->u16EndAddr && pstFormatInfo->u16EndAddr < u32MaxSize
                    && (pstFormatInfo->u16StartAddr + u32Size - 1) < u32MaxSize)
                {
                    u16StartAddr = pstFormatInfo->u16StartAddr;
                }
                else
                {
                    printf("Write auto download fail, invaild paramters, size: %d, addr range(enable, start, end) = (%d, %td, %td)\n",
                        u32Size, pstFormatInfo->bEnableRange, (ptrdiff_t)pstFormatInfo->u16StartAddr, (ptrdiff_t)pstFormatInfo->u16EndAddr);
                    return E_APIXC_RET_FAIL_INVALID_PARAMETER;
                }
            }
            else
            {
                if (u32Size > u32MaxSize)
                {
                    printf("Write auto download fail, invaild paramters, size: %d\n", u32Size);
                    return E_APIXC_RET_FAIL_INVALID_PARAMETER;
                }
            }
            return _Hal_SC_Auto_Download_Format_Hdr_Data(pInstance, pstFormatInfo->enSubClient, pu8Data, u32Size, u16StartAddr);
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA:
        case E_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA:
        case E_XC_AUTODOWNLOAD_CLIENT_XVYCC:
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE1:
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE2:
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE3:
        case E_XC_AUTODOWNLOAD_CLIENT_DEMURA:
        case E_XC_AUTODOWNLOAD_CLIENT_OP2LUT:
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_0:
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_1:
        case E_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM:
        case E_XC_AUTODOWNLOAD_CLIENT_FOOPM:
        case E_XC_AUTODOWNLOAD_CLIENT_MAX:
        default:
        {
            return E_APIXC_RET_FAIL;
        }

    }

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MHal_XC_GetAutoDownloadCaps(EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_BOOL *pbSupported)
{
    switch(enClient)
    {
        case E_XC_AUTODOWNLOAD_CLIENT_HDR:
        case E_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA:
        case E_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA:
        case E_XC_AUTODOWNLOAD_CLIENT_XVYCC:
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE1:
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE2:
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE3:
        case E_XC_AUTODOWNLOAD_CLIENT_DEMURA:
        case E_XC_AUTODOWNLOAD_CLIENT_OP2LUT:
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_0:
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_1:
        case E_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM:
        case E_XC_AUTODOWNLOAD_CLIENT_FOOPM:
        {
            *pbSupported = TRUE;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_MAX:
        default:
        {
            *pbSupported = FALSE;
            break;
        }

    }

    return E_APIXC_RET_OK;
}
#endif

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
E_APIXC_ReturnValue MHal_XC_HDR_Control(void* pInstance, EN_XC_HDR_CTRL_TYPE enCtrlType, void *pParam)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    switch(enCtrlType)
    {
        case E_XC_HDR_CTRL_CONFIG_DMA:
        {
            XC_HDR_DMA_CONFIG_INFO stDMAConfigInfo = *(XC_HDR_DMA_CONFIG_INFO *)pParam;

            // set dma mode
            if (stDMAConfigInfo.enMode == E_XC_HDR_DMA_MODE_10BIT)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_01_L, 0, 0x0003);
            }
            /*
            else if (stDMAConfigInfo.enMode == E_XC_HDR_DMA_MODE_YUV444)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_01_L, 1, 0x0003);
            }
            */
            else if (stDMAConfigInfo.enMode == E_XC_HDR_DMA_MODE_8BIT)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_01_L, 2, 0x0003);
            }
            else if (stDMAConfigInfo.enMode == E_XC_HDR_DMA_MODE_10BIT)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_01_L, 3, 0x0003);
            }
            break;
        }
        case E_XC_HDR_CTRL_INIT_DMA:
        {
            XC_HDR_DMA_INIT_INFO stDMAInitInfo = *(XC_HDR_DMA_INIT_INFO *)pParam;
            if ((stDMAInitInfo.phyBaseAddr != 0) && (stDMAInitInfo.u32Size != 0))
            {
                // set IPM/OPM addr
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK42_08_L, stDMAInitInfo.phyBaseAddr / BYTE_PER_WORD);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_09_L, (stDMAInitInfo.phyBaseAddr / BYTE_PER_WORD) >> 16, 0x01FF);
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK42_10_L, stDMAInitInfo.phyBaseAddr / BYTE_PER_WORD);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_11_L, (stDMAInitInfo.phyBaseAddr / BYTE_PER_WORD) >> 16, 0x01FF);

                // set limit addr
                // enable limit
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_50_L, 1, BIT(0));
                // min addr
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK42_52_L, stDMAInitInfo.phyBaseAddr / BYTE_PER_WORD);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_53_L, (stDMAInitInfo.phyBaseAddr / BYTE_PER_WORD) >> 16, 0x01FF);
                // max addr
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK42_54_L, (stDMAInitInfo.phyBaseAddr + stDMAInitInfo.u32Size) / BYTE_PER_WORD - 1);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_55_L, ((stDMAInitInfo.phyBaseAddr + stDMAInitInfo.u32Size) / BYTE_PER_WORD - 1) >> 16, 0x01FF);
            }
            break;
        }
        case E_XC_HDR_CTRL_ENABLE:
        case E_XC_HDR_CTRL_SET_TYPE:
        case E_XC_HDR_CTRL_SET_3DLUT:
        case E_XC_HDR_CTRL_SET_OTT_SHARE_MEMORY:
        case E_XC_HDR_CTRL_SET_OPEN_METADATA:
        default:
        {
            break;
        }
    }

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MHal_XC_HDR_GetCaps(void *pInstance, XC_HDR_SUPPORTED_CAPS *pstHDRCaps)
{
    EN_XC_HDR_TYPE enHDRType = pstHDRCaps->enHDRType;

    switch (enHDRType)
    {
        case E_XC_HDR_TYPE_OPEN:
        {
            pstHDRCaps->bSupported = TRUE;
            break;
        }
        case E_XC_HDR_TYPE_DOLBY:
        {
            pstHDRCaps->bSupported  = FALSE;
            break;
        }
        case E_XC_HDR_TYPE_TECHNICOLOR:
        {
            pstHDRCaps->bSupported = FALSE;
            break;
        }
        case E_XC_HDR_TYPE_HLG:
        {
            pstHDRCaps->bSupported = FALSE;
            break;
        }
        default:
        {
            pstHDRCaps->bSupported = FALSE;
        }
    }

    pstHDRCaps->s32MaxLuminanceData = -1;
    pstHDRCaps->s32MinLuminanceData = -1;
    pstHDRCaps->s32MaxFrameAveLumiance = -1;

    return E_APIXC_RET_OK;
}
#endif
#endif

//#define GET_FB_LEVEL_DBG(x) //x
E_XC_FB_LEVEL MHal_XC_Get_FB_Level(void* pInstance, MS_WINDOW_TYPE* pstCropWin, MS_WINDOW_TYPE* pstDispWin, MS_BOOL bInterlace, MS_U16* pu16HSize, MS_U16* pu16VSize)
{
    return E_XC_FB_LEVEL_FB;
}

MS_BOOL MHal_XC_Check_Bypass(void* pInstance, E_XC_BYPASS_MODULE enModule)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(0 != SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_40_L, 0x300))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_U32 Hal_SC_get_HDMIpolicy(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32HDMIPolicyInfo;
    u32HDMIPolicyInfo = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_7D_L) & 0xFFFFFFFF;
    return u32HDMIPolicyInfo;
}

#undef  MHAL_SC_C
