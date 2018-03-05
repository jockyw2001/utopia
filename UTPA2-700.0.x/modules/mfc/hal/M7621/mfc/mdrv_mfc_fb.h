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
#ifndef _MSFB_H_
#define _MSFB_H_

#ifdef _MSFB_C_
#define _MSFBDEC_
#else
#define _MSFBDEC_ extern
#endif

#include "mdrv_mfc_platform.h"

#define HINIBBLE(value)           ((value) / 0x10)
#define LONIBBLE(value)           ((value) & 0x0f)

typedef struct MST_MFC_MiuBaseAddr_s
{
	U8  u8MfcMode:2;
	U32 u32MfcBase;
	U32 u32MfcSize;

	U8  u8IpMode:3;
	U32 u32IpYcoutBase;
	U32 u32IpYcoutSize;

	U8  u8GammaMode:2;
	U32 u32GammaBase0;
	U32 u32GammaBase1;
	U32 u32GammaMLSize;

	U8  u8OdMode;
	U32 u32OdBaseEven;
	U32 u32OdLimitEven;
	U32 u32OdBaseOdd;
	U32 u32OdLimitOdd;
	U32 u32OdSize;
	U32 u32OdSizehalf;

	U32 u32OdLsbBase;
	U32 u32OdLsbLimit;
	U32 u32OdLsbSize;

	U32 u32AutoBist;

	U8  u8XDataOnDramMode:1;
	U32 u8XDataOnDramBase;
	U32 u8XDataOnDramSize;

	U8	u8MPifMode:1;
	U32	u32PifBase;
	U32	u32PifBase2;
	U32 u32PifSize;
}MST_MFC_MiuBaseAddr_t, *PMST_MFC_MiuBaseAddr_t;
		
_MSFBDEC_ XDATA MST_MFC_MiuBaseAddr_t gmfcMiuBaseAddr;

#define ENABLE_MEM_DQS_SELF_TEST    0

//====Block Size======================================================
#if 0
#if (MEMORY_TYPE_SEL == DDR1_08M_16BITS_1)
  #define MEMORY_SIZE  			0x1000000
#elif (MEMORY_TYPE_SEL == DDR1_08M_16BITS_2 || MEMORY_TYPE_SEL == DDR2_16M_16BITS_1)
  #define MEMORY_SIZE  			0x2000000
#elif (MEMORY_TYPE_SEL == DDR2_16M_16BITS_2 || MEMORY_TYPE_SEL == DDR2_32M_16BITS_1)
  #define MEMORY_SIZE  			0x4000000
#elif (MEMORY_TYPE_SEL == DDR2_32M_16BITS_2)
  #define MEMORY_SIZE  			0x8000000
#endif
#endif

void MDrv_MFC_AutoMemPhase(void);
void MDrv_MFC_InitializeMiu(void);
void MDrv_MFC_SetMiuSSC(U16 u16KHz, U16  u16Percent, BOOL bEnable);
#endif

