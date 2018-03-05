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
// Copyright (c) 2007-2008 MStar Semiconductor, Inc.
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
/// @file drvMFE.h
/// @brief MFE driver
/// @author MStar Semiconductor Inc.
///
/// MFE is the Mpeg4 ASP encoder, used for PVR function.
///
/// Features:
/// - Mpeg 4 ASP ( part 2 ) encoder.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MFE_H_
#define _MHAL_MFE_H_

#ifndef _KERNEL_MODE_
#include <stdio.h>
#endif

#include "MFE_chip.h"

#ifdef _AEON_PLATFORM_
#if defined(_MFE_T8_)
#include "DataType.h"
#else
#include "MsTypes.h"
#endif
#endif

#if defined(_MIPS_PLATFORM_)
#if defined(_MFE_BIG2_)
#include <sys/bsdtypes.h>
#include "shellcfg.h"   //for diag_printf
#elif defined(_KERNEL_MODE_)&&defined(_MFE_T8_)
#include "mdrv_types.h"
#else
//#include "mdrv_types.h"
#include "MsTypes.h"
#endif
#endif

#include "mfe_type.h"
#include "mfe_common.h"

#ifdef _MFE_BIG2_
#define MIU_SHIFT     2//3
#define MIU_SIZE     4//8
#else
#define MIU_SHIFT     3
#define MIU_SIZE     8
#endif


typedef struct {
    unsigned long   start_addr;
    unsigned long   end_addr;
    long            used_size;	// 0 means not used
} OutBitSBUF;

#if !(defined(_KERNEL_MODE_)&&defined(_MIPS_PLATFORM_)&&defined(_MFE_T8_))
#ifdef _NO_FILESYSTEM_ //defined(_AEON_PLATFORM_)
extern int fp_script;
#else
extern FILE *fp_script;
#endif
#endif
#ifdef _WIN32//_BCB_PLATFORM_
#define MFE_SCRIPT_OUT 1
//#define _IPB_FRAMEQP_
#elif defined(_TRACE32_CMM_)
#define MFE_SCRIPT_OUT 0
#else
#define MFE_SCRIPT_OUT 0
#endif
#if (MFE_SCRIPT_OUT == 1)
    #define MFE_SCRIPT(x)	(x)
#else
    #define MFE_SCRIPT(x)  {}
#endif

#if defined(_AEON_PLATFORM_) && defined(_MFE_T8_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0x111000
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg*2)*2) )
#elif defined(_AEON_PLATFORM_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0x1200
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg)*4) )
//extern U8 FSwrite_ready;
#elif defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
	extern U32 RIU_BASE;// = 0xBF834000;
	#define REG_BANK_MFE    0
	#define __MFE_REG(reg)        (*(volatile U16 *) ( RIU_BASE + REG_BANK_MFE + (reg)*4) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
	extern MFE_U32 RIU_BASE;// = 0xBF200000; //CH4
	#define REG_BANK_MFE    0x8800
	#define __MFE_REG(reg)		(*(volatile MFE_U16 *) ( RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)
	#define T8_RIU_BASE u32MFERegOSBase // = 0xBF200000; //CH4
	#define REG_BANK_MFE    0x8800
	#define __MFE_REG(reg)		(*(volatile MFE_U16 *) ( T8_RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
#elif defined(_HIF_) && defined(_MFE_BIG2_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0xd000
	#define __MFE_REG(reg)        FPGA_RIURead16(REG_BANK_MFE+reg, &val_64)
#elif defined(_FPGA_)
	extern U32 RIU_BASE;// = 0xA0000000;
	#define REG_BANK_MFE    0xa80
	#define __MFE_REG(reg)        FPGA_RIURead16(REG_BANK_MFE+reg, &val_64)
#else //if defined(_WIN32)//defined(_BCB_PLATFORM_)
	extern unsigned short REG_BANK_MFE[0x100];
	#define __MFE_REG(reg)		REG_BANK_MFE[reg]
#endif

#if defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
void MHAL_MFE_InitRegBase(MFE_U32 u32RegBase);
#endif
void MHal_MFE_PowerOff(MFE_U32 is_off,MFE_U32 clock_level);
MFE_U32 MHal_MFE_GetBitstreamEncodedLen(void);
void MHal_MFE_set_outbitsbuf(OutBitSBUF *bitsbuf);
void MHal_MFE_SetIrqMask(MFE_U16 mask);
#ifdef _MFE_T8_
void MHal_MFE_Enable_MIU_Protection(int MIU_TEST_MODE,MFE_CONFIG* pConfig);
void MHal_MFE_Enable_MIU_Protection_Check(int MIU_TEST_MODE,int TYPE);
#endif

void MHal_MFE_ClearIRQ(MFE_U16 irq_bits);
void MHal_MFE_GetIRQ(MFE_U16 *irq_bits);
void MHal_MFE_SWReset(void);
void MHal_MFE_start(void);
void MHal_MFE_SetCLKCTL(void);
void MHal_MFE_ResetReg(void);
void MHal_MFE_GetCRC(MFE_U8 checksum_HW[8]);
#endif//_MHAL_MFE_H_
