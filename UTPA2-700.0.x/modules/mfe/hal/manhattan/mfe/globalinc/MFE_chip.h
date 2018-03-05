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
#ifndef _MFE_CHIP_H_
#define _MFE_CHIP_H_
/*
This file includes all definitions which we use in MHal, MDrv, and MAdp layer.
We also have some definitions only belonged to MHal(in drvMFE.h), MDrv(in mdrv_m4ve.c), and MAdp(in madp_m4ve.c)
*/

// Chip definition.        << Choose one and only one >>
//#define _MFE_BIG2_
#define _MFE_T8_

// CPU-arch platform.        << Choose one and only one >>
#define _MIPS_PLATFORM_
//#define _AEON_PLATFORM_
//#define _FPGA_    // For PC UDMA/HIF FPGA testing. Refer to fpga_def.h

// File system
//#define _NO_FILESYSTEM_

// Bitstream getting, ISR
#define _ENABLE_ISR_
#define _NO_WAIT_FRAMEDONE_    // NOTE: Must defined if _ENABLE_ISR_ is defined.

//#define MEM_CHECK //check mem write after my_malloc
//#define CRC_CHECK //check crc

#ifdef CRC_CHECK
#define _CRC_PATTERN_GEN_    //check CRC pattern which from C MODEL
#endif
// Linux/TV-specific
#if defined(_MFE_T8_)
    //#define _MFE_LG_ //must also define _KERNEL_MODE and _MIPS_PLATFORM_
    #define _MFE_UTOPIA_
    #define _MFE_A3_
    //#define _KERNEL_MODE_        // Acts as kernel-mode driver.
    //#define CHECK_OUTPUT_FROM_DRAM
    //#define _MEM_READ_WRITE_CONTROL_
    //_INTERNAL_DIP_ and CUSTOM_INTERFACE1 are used for VOIP demo
    //#define _INTERNAL_DIP_
    //#define CUSTOM_INTERFACE1
    //#define DRV_MFE_TIME_MEASURE
#ifndef _CRC_PATTERN_GEN_
    #define SPS_PPS_IN_EACH_I
#endif
#endif

 #define OBUF_SIZE   409600    // Bytes
 #define OBUF_NUM    1
 //#define OBUF_SIZE   306000    // Bytes
//#define OBUF_SIZE   128000    // Bytes
//#define OBUF_NUM    4


// Mobile-specific
#if defined(_MFE_BIG2_)
    #define _USE_PSRAM_
    //#define _TRACE32_CMM_
#endif

#if defined(_FPGA_)
    #include "fpga_def.h"
#endif

#endif    // _MFE_CHIP_H_
