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
#ifndef _GPD_H
#define _GPD_H

//#define __DEBUG__
//#define U02
#define PA
#define FPGA
#define DUMP_LOG
#define DUMP_LOG2
//#define G3D
//#define TOTAL_TIME
//#define HW_TIME
//#define HUFF_REG_TIME


#if 1

#define DRAMBASE         0x00000000  //physical address

#else

//#define DRAMBASE         0x80000000   //cache
#define DRAMBASE        0xA0000000      //non-cache

#endif


#define ARGB8888   0
#define YUV422     1  //YUYV order from lsb to msb
#define RGB565     2
#define YVYU422    3  //YVYU order from lsb to msb
#define GRAY       4
#define GRAY_ALPHA 5


#define MMIO8(addr) (*((volatile MS_U8*)(MS_VIRT)(addr)))
#define MMIO32(addr) (*((volatile MS_U32*)(MS_VIRT)(addr)))

#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif

typedef enum
{
    E_GPD_DEBUG_LEVEL_NONE = 0x0,
    E_GPD_DEBUG_LEVEL_ERROR = 0x1,
    E_GPD_DEBUG_LEVEL_INFO = 0x2,
    E_GPD_DEBUG_LEVEL_TIME = 0x4,
    E_GPD_DEBUG_LEVEL_ALL = 0xf,
} gpd_debug_level;

extern gpd_debug_level eEnableDbgMsg;

#define ENABLE_GPD_MESSAGE    TRUE
#define ENABLE_GPD_PATCH      FALSE

#if (defined CONFIG_MLOG)
#include "ULog.h"

#define console_printf(format, args...)         do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ULOGI("DRV_GPD", format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ULOGE("DRV_GPD", format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) ULOGI("DRV_GPD", format, ##args);}while(0)

#else

#ifdef ANDROID
#define LOG_TAG "API_GPD"
#include <cutils/log.h>
#endif

#if ENABLE_GPD_MESSAGE

#ifndef ANDROID
#ifdef MSOS_TYPE_LINUX_KERNEL
#define console_printf(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printk(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printk(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) printk(format, ##args);}while(0)
#else
#define console_printf(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printf(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)   do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printf(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) printf(format, ##args);}while(0)
#endif
#else
#ifndef LOGD
#define console_printf(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ALOGI(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ALOGI(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) ALOGI(format, ##args);}while(0)
#else
#define console_printf(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) LOGI(format, ##args);}while(0)
#define GPD_DRV_ERROR(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) LOGI(format, ##args);}while(0)
#define GPD_DRV_TIME(format, args...)          do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_TIME) LOGI(format, ##args);}while(0)
#endif
#endif

#else

#define console_printf(...)

#endif


#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

//MS_U32 console_printf(const char* fmt, ...);




#endif
