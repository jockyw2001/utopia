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
#ifndef MS_DPRINTF_H
#define MS_DPRINTF_H

#include "MFE_chip.h"
#include "ULog.h"

// Switch for different debug messages
#define HAL_L1  (1UL /*0*/ << 0)    // debug messages
#define HAL_L2  (1UL /*0*/ << 1)    // warning messages
#define DRV_L0	(1UL /*0*/ << 15)   // error messages
#define DRV_L1	(1UL /*0*/ << 4)    // info messages
#define DRV_L2	(1UL /*0*/ << 5)    // debug messages
#define DRV_L3  (1UL /*0*/ << 6)    // more debug messages
#define DRV_L4  (1UL /*0*/ << 7)    // warning messages
#define DRV_L5  (1UL /*0*/ << 8)    // debug messages (dump_reg)
#define DRV_L6  (1UL /*0*/ << 9)    // debug messagse (timer)
#define DRV_L7  (1UL /*0*/ << 10)   // debug messages (VT rate control)
#if defined(_WIN32)||!defined(_KERNEL_MODE_)
#define ADP_L0  (1UL /*0*/ << 15)   // error messages
#define ADP_L1  (1UL /*0*/ << 16)   // info messages
#define ADP_L2  (1UL /*0*/ << 17)   // debug messages
#define ADP_L3  (1UL /*0*/ << 18)   // more debug messages
#define ADP_L4  (1UL /*0*/ << 19)   // warning messages
#define APP_L1  (1UL /*0*/ << 28)   // fatal error messages
// Marked out below line to disable all debug messages!
#define DEBUG_LEVEL (HAL_L1|HAL_L2|DRV_L0|DRV_L1|DRV_L2|DRV_L4|ADP_L0|ADP_L1|ADP_L2|ADP_L4|APP_L1)
//#define DEBUG_LEVEL (HAL_L1|HAL_L2|DRV_L0|DRV_L1|DRV_L2|DRV_L4|ADP_L0|ADP_L1|ADP_L2|ADP_L4|APP_L1|DRV_L3|ADP_L3|DRV_L5)
#else
//#define DEBUG_LEVEL (HAL_L1|HAL_L2|DRV_L0|DRV_L1|DRV_L2|DRV_L4|ADP_L0|ADP_L1|ADP_L2|ADP_L4|APP_L1|DRV_L3|ADP_L3|DRV_L5)
#endif

#if !defined(DEBUG_LEVEL)
#define DEBUG_LEVEL 0UL
#endif

#if !defined(_KERNEL_MODE_)&&(DEBUG_LEVEL!=0)
#define ms_dprintk ms_dprintf
#elif defined(_MFE_T8_)
//#include <linux/kernel.h>
#endif

#ifdef _MSC_VER

#if DEBUG_LEVEL == 0
#define ms_dprintk ms_dprintf
static __inline void ms_dprintf(MS_S32 level, const MS_S8 *fmt, ...) {}

#else /* DEBUG_LEVEL */

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static __inline void ms_dprintf(MS_S32 level, const MS_S8 *fmt, ...)
{
    va_list ap;
    MS_S8 msg[2048];

    if (!(level & DEBUG_LEVEL))
        return;

    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
#ifdef WINCE
    fprintf(stderr, "%s", msg);
#else
    OutputDebugStringA(msg);
#endif
    va_end(ap);
}

#endif /* DEBUG_LEVEL */

#else /* _MSC_VER */

#if (DEBUG_LEVEL & DRV_L5) || (DEBUG_LEVEL & DRV_L7)
#define DEBUG_MSG    (HAL_L1|DRV_L2|DRV_L3|ADP_L2|ADP_L3|DRV_L5|DRV_L7)
#elif (DEBUG_LEVEL & DRV_L3) || (DEBUG_LEVEL & ADP_L3)
#define DEBUG_MSG    (HAL_L1|DRV_L2|ADP_L2|DRV_L3|ADP_L3)
#else
#define DEBUG_MSG    (HAL_L1|DRV_L2|ADP_L2)
#endif
#define INFO_MSG     (DRV_L1|ADP_L1)
#define WARNING_MSG  (HAL_L2|DRV_L4|ADP_L4)
#define ERROR_MSG    (DRV_L0|ADP_L0)
#define FATAL_MSG    (APP_L1)

#define ms_dprintf(level, fmt, arg...)    \
    do {                                  \
        if (level & DEBUG_MSG) {          \
            ULOGD("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & INFO_MSG) {      \
            ULOGI("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & WARNING_MSG) {   \
            ULOGW("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & ERROR_MSG) {     \
            ULOGE("MFE_EX", fmt, ##arg);  \
        }                                 \
        else if (level & FATAL_MSG) {     \
            ULOGF("MFE_EX", fmt, ##arg);  \
        }                                 \
    } while (0)

#endif /* _MSC_VER */

#endif /* MS_DPRINTF_H */

