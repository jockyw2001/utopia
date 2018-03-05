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
#ifndef _INTRA_UDPATE_H_
#define _INTRA_UDPATE_H_

#include "mfe_type.h"

// "in" means sw-->hw
// "out" means hw-->sw
typedef struct {
	MS_U32 intra : 1;    // [in, out]: Forced-intra; intra-coded
	MS_U32 motion : 1;   // [out] This MB is regarded as motion area
} HW_MB_MAP;

typedef struct {
	MS_S32 inter_count;	// For FIR usage
	MS_S32 intra_count;	// For AIR usage
} SW_MB_MAP;

typedef struct _IntraUpdateContext {
	// Input
	MS_S32 nWidth, nHeight;
	MS_S32 nTotalMb;
	HW_MB_MAP *pHwMbMap;	// Hw/sw shared. of size nTotalSize
	SW_MB_MAP *pSwMbMap;	// sw only. of size nTotalSize
	// Intra Refresh switches
	MS_U8 bFIR;		// H.263 Forced Updating
	MS_U8 bCIR;		// Cyclic intra updating
	MS_U8 bAIR;		// Adaptive Intra-Refresh. Must be FALSE if bCIR is FALSE
	// Intra Refresh constants
	MS_S32 FIR_THR;
	MS_S32 CIR_COUNT;
	MS_S32 AIR_COUNT, AIR_ROUND;
	// Internal counter
	MS_S32 nIndexCIR;
	MS_S32 nIndexAIR;
} IntraUpdateContext;

#ifdef __cplusplus
extern "C" {
#endif

void IntraUpdate_Init(IntraUpdateContext* ctx);
void IntraUpdate_Frame(IntraUpdateContext* ctx);

#ifdef __cplusplus
}
#endif

#endif // _INTRA_UDPATE_H_
