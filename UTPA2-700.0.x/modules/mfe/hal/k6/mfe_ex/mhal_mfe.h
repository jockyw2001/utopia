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
////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MFE_H_
#define _MHAL_MFE_H_

#if defined(_MFE_UTOPIA_)
#include <stdio.h>
#endif

#include "MFE_chip.h"
#include "drv_mfe_st.h"
#if defined(_MIPS_PLATFORM_)
#if defined(_MFE_BIG2_)
#include <sys/bsdtypes.h>
#include "shellcfg.h"   //for diag_printf
#elif defined(_KERNEL_MODE_)&&defined(_MFE_T8_)
#include "mdrv_types.h"
#elif defined(__MOBILE_CASE__)
#include "msutil/MsTypes.h"
#else
#include "MsTypes.h"
#endif
#endif

#include "mfe_type.h"
#include "mfe_common.h"

#ifdef _MFE_BIG2_
#define MIU_SHIFT    2UL
#define MIU_SIZE     4UL //8
#else
#define MIU_SHIFT    3UL
#define MIU_SIZE     8UL
#endif

#if defined(_AEON_PLATFORM_) && defined(_MFE_T8_)
	extern MS_U32 RIU_BASE;// = 0xA0000000UL;
	#define REG_BANK_MFE    0x111000UL
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg*2)*2) )
#elif defined(__MFE_G2__)
	#define T8_RIU_BASE ((MS_VIRT)u32MFERegOSBase) // = 0x25000000UL; G2 RIU base.
	#define REG_BANK_MFE  0x400UL
	#define __MFE_REG(reg) (*(volatile MS_U32 *) (void*)( T8_RIU_BASE + (reg)*4 ) )
#elif defined(_AEON_PLATFORM_)
	extern MS_U32 RIU_BASE;// = 0xA0000000UL;
	#define REG_BANK_MFE    0x1200UL
	#define __MFE_REG(reg)		(*(volatile U16 *) ( RIU_BASE + (REG_BANK_MFE + reg)*4) )
//extern U8 FSwrite_ready;
#elif defined(_MFE_BIG2_) && defined(_MIPS_PLATFORM_)
	extern MS_U32 RIU_BASE;// = 0xBF834000UL;
	#define REG_BANK_MFE    0UL
	#define __MFE_REG(reg)        (*(volatile U16 *) ( RIU_BASE + REG_BANK_MFE + (reg)*4) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
	extern MS_U32 RIU_BASE;// = 0xBF200000UL; //CH4
	#define REG_BANK_MFE    0x8800UL
	#define __MFE_REG(reg)		(*(volatile MS_U16 *) ( RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_)&& defined(_MFE_UTOPIA_)
	#define T8_RIU_BASE ((MS_VIRT)u32MFERegOSBase) // = 0xBF200000UL; //CH4
#ifdef _MFE_KAISER_
	#define REG_BANK_MFE    0x11380UL
#else
    #define REG_BANK_MFE    0x8800UL
#endif
	#define __MFE_REG(reg)		(*(volatile MS_U16 *) ( T8_RIU_BASE + (REG_BANK_MFE + reg) * 4 ) )
    #define REG_BANK_MFE1    0x8880UL
    #define __MFE_REG1(reg)		(*(volatile MS_U16 *) ( T8_RIU_BASE + (REG_BANK_MFE1 + reg) * 4 ) )

// ((clkgen_bank - 0x1000) << 7) + 16bit_reg_offset
#if defined(_MFE_KANO_) || defined(_MFE_K6_)
    #define REG_CKG_MFE     (0x510)     // ((0x100a - 0x1000) << 7) + 0x10
#else
    #define REG_CKG_MFE     (0x1998)    // ((0x1033 - 0x1000) << 7) + 0x18
#endif
#else //if defined(_WIN32)//defined(_BCB_PLATFORM_)
    #define T8_RIU_BASE 0x0a80UL // = 0xBF200000UL; //CH4
	#define REG_BANK_MFE 0x0a80UL //Local FPGA
	#define REG_BANK_MFE1 0x0b80UL //Local FPGA
    //#define T8_RIU_BASE 0x0b80UL // = 0xBF200000UL; //CH4
	//#define REG_BANK_MFE 0x0b80UL //Local FPGA
	//#define REG_BANK_MFE1 0x01080UL //Local FPGA
#endif

MS_BOOL MHal_MFE_GetHWCap(MS_U16 *width, MS_U16 *height);

#if (defined(_MFE_T8_)||defined(_MFE_M1_)) && defined(_MIPS_PLATFORM_)&& !defined(_KERNEL_MODE_)
void MHAL_MFE_InitRegBase(MS_U32 u32RegBase);
void MHAL_MFE_CreateRegMap(MFE_REG* mfe_reg, MFE_REG1* mfe_reg1);
void MHAL_MFE_DelRegMap(MFE_REG* mfe_reg, MFE_REG1* mfe_reg1);
#endif
void MHal_MFE_PowerOff(MS_U32 is_off,MFE_CLK_LEVEL clock_level);
MS_U32 MHal_MFE_GetBitstreamEncodedLen(void);
void MHal_MFE_set_outbitsbuf(MFE_REG* mfe_reg, OutBitSBUF *bitsbuf, MS_S32 outbufsize);
void MHal_MFE_SetIrqMask(MS_U16 mask);

#ifdef MFE_MIU_PROTECT
void MHal_MFE_Enable_MIU_Protection(MS_S32 MIU_TEST_MODE,MFE_CONFIG* pConfig);
void MHal_MFE_Enable_MIU_Protection_Check(MS_S32 MIU_TEST_MODE, MS_S32 TYPE);
#endif
MS_U32 MHal_MFE_CycleReport(void);
void MHal_MFE_ClearIRQ(MS_U16 irq_bits);
void MHal_MFE_GetIRQ(MS_U16 *irq_bits);
void MHal_MFE_SWReset(MFE_REG* mfe_reg);
void MHal_MFE_start(void);
void MHal_MFE_SetCLKCTL(MFE_REG* mfe_reg);
void MHal_MFE_ResetReg(MFE_REG* mfe_reg);
void MHal_MFE_GetCRC(MS_U8 checksum_HW[8]);
#endif//_MHAL_MFE_H_
