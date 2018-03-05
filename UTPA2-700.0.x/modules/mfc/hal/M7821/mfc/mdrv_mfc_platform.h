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
#ifndef _MDRV_MFC_PLATFORM_H_
#define _MDRV_MFC_PLATFORM_H_

#define CODEBASE_51		 	0x01
#define CODEBASE_LINUX		0x02
#define CODEBASE_UTOPIA		0x03

#define CODESIZE_ALL		0x01
#define CODESIZE_SMALL		0x02


#define CODEBASE_SEL				CODEBASE_UTOPIA //CODEBASE_51 //

#if(CODEBASE_SEL == CODEBASE_51)
	#include "board.h"
    #include "system.h"
    #include "common.h"
    #include "msreg.h"
    #define mfcSleepMs(x) Delay1ms(x)
    #define mfcSleepMsNop(x) Delay1ms_Nop(x)

#elif(CODEBASE_SEL == CODEBASE_LINUX)
	#include "mdrv_types.h"
	#include "mdrv_mfc_define.h"
    #include "mst_platform.h"

    #ifndef MFC_USE_IN_BOOLLOAD
        #define MFC_USE_IN_BOOLLOAD 0
    #endif

    #if MFC_USE_IN_BOOLLOAD
        #define mfcSleepMs(x) udelay(1000*x)
        #define mfcSleepMsNop(x)  udelay(1000*x)
    #include "../mdrv_mfc.h"
    #else
    #include <unistd.h>
    #include <sys/time.h>
    #define mfcSleepMs(x) usleep(1000*x)
    #define mfcSleepMsNop(x)  usleep(1000*x)
    #include "mdrv_mfc.h"
    #endif
	#define code
    #define XDATA
    #define MFC_IIC_CHANNEL_ID 0x02
    #define MFC_IIC_SLAVE_ADDR 0xB4

#elif(CODEBASE_SEL == CODEBASE_UTOPIA)
	#include "mdrv_mfc_define.h"
	//#include "MsTypes.h"
	//#include "MsOS.h"
	#include "MsCommon.h"

	#define code
	#define U8 MS_U8
	#define U16 MS_U16
	#define U32 MS_U32
	#define S8 MS_S8
	#define S16 MS_S16
	#define S32 MS_S32
	#define BOOL MS_BOOL

	#define _6BITS						0
	#define _8BITS						1
	#define _10BITS						2

	#define _TTL             			0
	#define _MINI_LVDS                  1
	#define _LVDS                       2
	#define _RSDS                       3
	#define _MINI_LVDS_GIP              4
    #define _MINI_LVDS_GIP_V5           5

	#define _SINGLE						0
	#define _DUAL       				1
	#define _QUAD       				2
	#define _QUAD_LR       				3
	#define _V_BY1                      4

	// for Lvds output channel,
	// REG_321F[7:6]=>D, [5:4]=>C, [3:2]=>B, [1:0]=>A
	#define CHANNEL_SWAP(D, C, B, A)	((D<<6)&0xC0)|((C<<4)&0x30)|((B<<2)&0x0C)|(A&0x03)
	#define _CH_A                       0
	#define _CH_B                       1
	#define _CH_C                       2
	#define _CH_D                       3
    #define mfcSleepMs(x) OS_DELAY_TASK(x)
    #define mfcSleepMsNop(x)  OS_DELAY_TASK(x)
    #define ENABLE_USER_TOTAL 0
    #define XDATA
    #define MFC_IIC_CHANNEL_ID 0x01
    #define MFC_IIC_SLAVE_ADDR 0xB4
#endif
extern BOOL S7M;
#endif

