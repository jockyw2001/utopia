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
#ifndef _MFE_TYPE_H_
#define _MFE_TYPE_H_

#include "MFE_chip.h"

#if defined(_MFE_UTOPIA_)

#include "MsTypes.h"
#include <stdint.h>

/// data type unsigned char, data length 1 byte
typedef MS_U8       MFE_U8;                                  // 1 byte
/// data type unsigned short, data length 2 byte
typedef MS_U16     MFE_U16;                                 // 2 bytes
/// data type unsigned int, data length 4 byte
typedef MS_U32     MFE_U32;                                 // 4 bytes
/// data type unsigned int, data length 8 byte
typedef MS_U64     MFE_U64;                                 // 8 bytes
/// data type signed char, data length 1 byte
typedef MS_S8      MFE_S8;                                  // 1 byte
/// data type signed short, data length 2 byte
typedef MS_S16     MFE_S16;                                 // 2 bytes
/// data type signed int, data length 4 byte
typedef MS_S32     MFE_S32;                                 // 4 bytes

/// data type signed int, data length 4 byte
typedef MS_S64     MFE_S64;                                 // 4 bytes

/// definition for MS_BOOL, this will be removed later
typedef MS_BOOL   MFE_BOOL;

typedef MS_U8   MFE_BYTE;
typedef MS_U16  MFE_WORD;
typedef MS_U32   MFE_DWORD;

#elif defined(_MFE_LG_)
#include "mdrv_types.h"
#include <stdint.h>

/// data type unsigned char, data length 1 byte
typedef U8       MFE_U8;                                  // 1 byte
/// data type unsigned short, data length 2 byte
typedef U16     MFE_U16;                                 // 2 bytes
/// data type unsigned int, data length 4 byte
typedef U32     MFE_U32;                                 // 4 bytes
/// data type unsigned int, data length 8 byte
#if defined(CONFIG_Titania8)
typedef U64     MFE_U64;                                 // 8 bytes
#endif
/// data type signed char, data length 1 byte
typedef S8      MFE_S8;                                  // 1 byte
/// data type signed short, data length 2 byte
typedef S16     MFE_S16;                                 // 2 bytes
/// data type signed int, data length 4 byte
typedef S32     MFE_S32;                                 // 4 bytes
/// data type signed int, data length 4 byte
typedef long long     MFE_S64;                                 // 4 bytes
/// definition for MS_BOOL, this will be removed later
typedef BOOL   MFE_BOOL;

typedef U8   MFE_BYTE;
typedef U16  MFE_WORD;
typedef S32   MFE_DWORD;

#elif defined(_WIN32)
typedef unsigned char           MFE_U8;
typedef unsigned short          MFE_U16;
typedef unsigned int             MFE_U32;
typedef unsigned long long    MFE_U64;
typedef char                        MFE_S8;
typedef short                       MFE_S16;
typedef int                          MFE_S32 ;
typedef BOOL                      MFE_BOOL;

typedef unsigned char   MFE_BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;

#if __STDC_VERSION__ >= 199901L
typedef long long MFE_INT64;
#else
typedef __int64 MFE_INT64;
#endif

#endif

#if defined(_WIN32)&&defined(_MSC_VER)
#if (_MSC_VER < 1400) //vc 6 or older version
#define MFE_INT64_C(val) val##i64
#endif
#else
#define MFE_INT64_C(val) val##ULL
#endif


#ifndef _WIN32
	#define LOWORD(l)        ((unsigned short)(l))
	#define HIWORD(l)        ((unsigned short)(((unsigned int)(l) >> 16) & 0xFFFF))
	#define MAKELONG(a, b)    ((unsigned int)(((unsigned short)(a)) | ((unsigned int)((unsigned short)(b))) << 16))
#endif


#ifndef MFE_MAX
#define  MFE_MAX(a,b)              (((a) > (b)) ? (a) : (b))
#endif
#ifndef MFE_MIN
#define  MFE_MIN(a,b)              (((a) < (b)) ? (a) : (b))
#endif

#ifndef MFE_MUX
#define MFE_MUX(a, b, c)    ((a) ? (b) : (c))
#endif

#define dump(x) //for rate control debug




#define I_VOP		0		/* vop coding modes */
#define P_VOP		1
#define B_VOP           2       // assumed B VOP always without reconstructed

#endif // _MFE_TYPE_H_

