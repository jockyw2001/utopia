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
#ifndef MADP_MS_DPRINTF_H
#define MADP_MS_DPRINTF_H
#if defined(_WIN32)||!defined(_KERNEL_MODE_)
#include "ms_dprintf.h"

#else
// Switch for different debug messages
#define ADP_L1  (1/*0*/ << 16)
#define ADP_L2  (1/*0*/ << 17)
#define ADP_L3  (1/*0*/ << 18)
#define ADP_L4  (1/*0*/ << 19)
#define APP_L1 (1/*0*/ << 28)


// Marked out below line to disable all debug messages!
//#define ADP_DEBUG_LEVEL (ADP_L1|ADP_L2|APP_L1|ADP_L3)
//#define ADP_DEBUG_LEVEL (ADP_L4)
#if !defined(ADP_DEBUG_LEVEL)
#define ADP_DEBUG_LEVEL 0
#endif


#ifdef _MSC_VER

#if ADP_DEBUG_LEVEL == 0

static __inline void ms_dprintf(int level, const char *fmt, ...) {}

#else /* ADP_DEBUG_LEVEL */

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

static __inline void ms_dprintf(int level, const char *fmt, ...)
{
    va_list ap;
    char msg[2048];

    if (!(level & ADP_DEBUG_LEVEL))
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

#endif /* ADP_DEBUG_LEVEL */

#else /* _MSC_VER */

#if ADP_DEBUG_LEVEL == 0
#define ms_dprintf(arg...) ((void) 0)

#else /* ADP_DEBUG_LEVEL */

#ifdef MIPSDEV

#include <cyg/infra/diag.h>

#define ms_dprintf(level, fmt, arg...) \
    do { \
        if (level & ADP_DEBUG_LEVEL) diag_printf(fmt, ##arg); \
    } while (0)

#else /* MIPSDEV */

#define ms_dprintf(level, fmt, arg...) \
    do { \
        if (level & ADP_DEBUG_LEVEL) printf(fmt, ##arg); \
    } while (0)

#endif /* MIPSDEV */

#endif /* ADP_DEBUG_LEVEL */

#endif /* _MSC_VER */
#endif /*_WIN32*/ //#if defined(_WIN32)||!defined(_KERNEL_MODE_)


#endif /* MADP_MS_DPRINTF_H */
