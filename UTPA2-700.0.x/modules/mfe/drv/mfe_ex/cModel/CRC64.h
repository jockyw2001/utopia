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
#ifndef _CRC64_H_
#define _CRC64_H_

#include "mfe_type.h"


typedef struct BitArray {
	MS_U32 bit0 : 1;
	MS_U32 bit1 : 1;
	MS_U32 bit2 : 1;
	MS_U32 bit3 : 1;
	MS_U32 bit4 : 1;
	MS_U32 bit5 : 1;
	MS_U32 bit6 : 1;
	MS_U32 bit7 : 1;
	MS_U32 bit8 : 1;
	MS_U32 bit9 : 1;
	MS_U32 bit10 : 1;
	MS_U32 bit11 : 1;
	MS_U32 bit12 : 1;
	MS_U32 bit13 : 1;
	MS_U32 bit14 : 1;
	MS_U32 bit15 : 1;
	MS_U32 bit16 : 1;
	MS_U32 bit17 : 1;
	MS_U32 bit18 : 1;
	MS_U32 bit19 : 1;
	MS_U32 bit20 : 1;
	MS_U32 bit21 : 1;
	MS_U32 bit22 : 1;
	MS_U32 bit23 : 1;
	MS_U32 bit24 : 1;
	MS_U32 bit25 : 1;
	MS_U32 bit26 : 1;
	MS_U32 bit27 : 1;
	MS_U32 bit28 : 1;
	MS_U32 bit29 : 1;
	MS_U32 bit30 : 1;
	MS_U32 bit31 : 1;
	MS_U32 bit32 : 1;
	MS_U32 bit33 : 1;
	MS_U32 bit34 : 1;
	MS_U32 bit35 : 1;
	MS_U32 bit36 : 1;
	MS_U32 bit37 : 1;
	MS_U32 bit38 : 1;
	MS_U32 bit39 : 1;
	MS_U32 bit40 : 1;
	MS_U32 bit41 : 1;
	MS_U32 bit42 : 1;
	MS_U32 bit43 : 1;
	MS_U32 bit44 : 1;
	MS_U32 bit45 : 1;
	MS_U32 bit46 : 1;
	MS_U32 bit47 : 1;
	MS_U32 bit48 : 1;
	MS_U32 bit49 : 1;
	MS_U32 bit50 : 1;
	MS_U32 bit51 : 1;
	MS_U32 bit52 : 1;
	MS_U32 bit53 : 1;
	MS_U32 bit54 : 1;
	MS_U32 bit55 : 1;
	MS_U32 bit56 : 1;
	MS_U32 bit57 : 1;
	MS_U32 bit58 : 1;
	MS_U32 bit59 : 1;
	MS_U32 bit60 : 1;
	MS_U32 bit61 : 1;
	MS_U32 bit62 : 1;
	MS_U32 bit63 : 1;
} BitArray;

typedef struct _CRC_INFO {
	// Input 
	MS_U8 bIsLast;	// When bIsLast=1, the last nBufSize%8 bytes are zero-padded to 8-byte for the last-round CRC.
							// When bIsLast=0, nLastBytes=nBufSize%8 and those bytes are stored in TempBuf for later CRC round.
	MS_U8 *pBuf;	// The input byte data for CRC calculation
	MS_S32 nBufSize;			// How many bytes in pBuf
	// Output
	MS_U8 CRCValue[8];
	// Internal usage
	MS_S32 nLastBytes;
	MS_U8 TempBuf[8];
} CRC_INFO;

void CRC_Init(CRC_INFO* pInfo);
void CRC_Generator(CRC_INFO* pInfo);
void get_CRC_ACC_CRC(CRC_INFO* pAllFrames, CRC_INFO* pCurFrame);
void CRC_ACC_HW_Generator(CRC_INFO* pAllFrames, MS_U8* crc);

#endif
