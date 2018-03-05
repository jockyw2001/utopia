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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_IRQ_H_
#define _REG_IRQ_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define RIUBASE_IRQ         0x1900   //(0x101900-0x100000)
#define RIUBASE_IRQ_EXP     0x1900   //(0x101900-0x100000)
#define RIUBASE_IRQ_HYP     0x1000   //(0x101000-0x100000)

#define RIUBASE_IRQ_FRC     0x300500 //(0x400500-0x100000)

#if 1//(frcr2_integration###)
//################
//
// For FRC banks
// Host 0 : NON-PM
// Host 1 : FRC-R2
//
#define REG_FRC_C_FIQ_MASK                     (RIUBASE_IRQ_FRC + (0x24*2))
#define REG_FRC_C_FIQ_CLR                      (RIUBASE_IRQ_FRC + (0x2C*2))
#define REG_FRC_FIQ_FINAL_STATUS               (RIUBASE_IRQ_FRC + (0x2C*2))

#define REG_FRC_C_IRQ_MASK                     (RIUBASE_IRQ_FRC + (0x34*2))
#define REG_FRC_IRQ_FINAL_STATUS               (RIUBASE_IRQ_FRC + (0x3C*2))

#define REG_FRC_C_FIQ_EXP_MASK                 (RIUBASE_IRQ_FRC + (0x24*2))
#define REG_FRC_C_FIQ_EXP_CLR                  (RIUBASE_IRQ_FRC + (0x2C*2))
#define REG_FRC_C_FIQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x2C*2))

#define REG_FRC_C_FIQ_HYP_MASK                 (RIUBASE_IRQ_FRC + (0x24*2))
#define REG_FRC_C_FIQ_HYP_CLR                  (RIUBASE_IRQ_FRC + (0x2C*2))
#define REG_FRC_C_FIQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x2C*2))

#define REG_FRC_C_IRQ_EXP_MASK                 (RIUBASE_IRQ_FRC + (0x34*2))
#define REG_FRC_C_IRQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x3C*2))

#define REG_FRC_C_IRQ_HYP_MASK                 (RIUBASE_IRQ_FRC + (0x34*2))
#define REG_FRC_C_IRQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_FRC + (0x3C*2))
#endif

//################
//
// For ARM Core0+SEC-R2 in NonPM
// Host 1 : ARM CA9 Core 0
// Host 3 : SEC-R2
//
#define REG_AEON_C_FIQ_MASK                     (RIUBASE_IRQ + (0x64*2))
#define REG_AEON_C_FIQ_CLR                      (RIUBASE_IRQ + (0x6C*2))
#define REG_AEON_FIQ_FINAL_STATUS               (RIUBASE_IRQ + (0x6C*2))

#define REG_AEON_C_IRQ_MASK                     (RIUBASE_IRQ + (0x74*2))
#define REG_AEON_IRQ_FINAL_STATUS               (RIUBASE_IRQ + (0x7C*2))

#define REG_AEON_C_FIQ_EXP_MASK                 (RIUBASE_IRQ_EXP + (0x66*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_EXP_CLR                  (RIUBASE_IRQ_EXP + (0x6E*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + (0x6E*2)) //NOT EXIST NOW

#define REG_AEON_C_FIQ_HYP_MASK                 (RIUBASE_IRQ_HYP + (0x64*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_HYP_CLR                  (RIUBASE_IRQ_HYP + (0x6C*2)) //NOT EXIST NOW
#define REG_AEON_C_FIQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + (0x6C*2)) //NOT EXIST NOW

#define REG_AEON_C_IRQ_EXP_MASK                 (RIUBASE_IRQ_EXP + (0x76*2)) //NOT EXIST NOW
#define REG_AEON_C_IRQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + (0x7E*2)) //NOT EXIST NOW

#define REG_AEON_C_IRQ_HYP_MASK                 (RIUBASE_IRQ_HYP + (0x74*2)) //NOT EXIST NOW
#define REG_AEON_C_IRQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + (0x7C*2)) //NOT EXIST NOW

//################
//
// For PM+MIPS in NonPM
// Host 0 : PM
// Host 1 : MIPS (V)
//
#define REG_MIPS_C_FIQ_MASK                     (RIUBASE_IRQ + 0x24*2) //[IRQ][HAL][001] Mask bit of FIQ in main CPU
#define REG_MIPS_C_FIQ_CLR                      (RIUBASE_IRQ + 0x2C*2) //[IRQ][HAL][002] Clear bit of FIQ in main CPU
#define REG_MIPS_FIQ_FINAL_STATUS               (RIUBASE_IRQ + 0x2C*2) //[IRQ][HAL][003] Status bit of FIQ in main CPU

#define REG_MIPS_C_IRQ_MASK                     (RIUBASE_IRQ + 0x34*2) //[IRQ][HAL][007] Mask bit of IRQ in main CPU
#define REG_MIPS_IRQ_FINAL_STATUS               (RIUBASE_IRQ + 0x3C*2) //[IRQ][HAL][008] Status bit of IRQ in main CPU

#define REG_MIPS_C_FIQ_EXP_MASK                 (RIUBASE_IRQ_EXP + 0x26*2) //[IRQ][HAL][004] Mask bit of FIQ expansion in main CPU
#define REG_MIPS_C_FIQ_EXP_CLR                  (RIUBASE_IRQ_EXP + 0x2E*2) //[IRQ][HAL][005] Clear bit of FIQ expansion in main CPU
#define REG_MIPS_C_FIQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + 0x2E*2) //[IRQ][HAL][006] Status bit of FIQ expansion in main CPU

#define REG_MIPS_C_FIQ_HYP_MASK                 (RIUBASE_IRQ_HYP + 0x24*2) //[IRQ][HAL][011] Mask bit of FIQ hyperspace in main CPU
#define REG_MIPS_C_FIQ_HYP_CLR                  (RIUBASE_IRQ_HYP + 0x2C*2) //[IRQ][HAL][012] Clear bit of FIQ hyperspace in main CPU
#define REG_MIPS_C_FIQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + 0x2C*2) //[IRQ][HAL][013] Status bit of FIQ hyperspace in main CPU

#define REG_MIPS_C_IRQ_EXP_MASK                 (RIUBASE_IRQ_EXP + 0x36*2) //[IRQ][HAL][009] Mask bit of IRQ expansion in main CPU
#define REG_MIPS_C_IRQ_EXP_FINAL_STATUS         (RIUBASE_IRQ_EXP + 0x3E*2) //[IRQ][HAL][010] Status bit of IRQ expansion in main CPU

#define REG_MIPS_C_IRQ_HYP_MASK                 (RIUBASE_IRQ_HYP + 0x34*2) //[IRQ][HAL][014] Mask bit of IRQ hyperspace in main CPU
#define REG_MIPS_C_IRQ_HYP_FINAL_STATUS         (RIUBASE_IRQ_HYP + 0x3C*2) //[IRQ][HAL][015] Status bit of IRQ hyperspace in main CPU

#ifdef MCU_AEON
#if defined(CONFIG_FRC)//frcr2_integration###
#define REG_C_FIQ_MASK                          REG_FRC_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_FRC_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_FRC_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_FRC_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_FRC_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_FRC_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_FRC_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_FRC_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_FRC_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_FRC_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_FRC_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_FRC_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_FRC_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_FRC_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_FRC_C_IRQ_HYP_FINAL_STATUS
#else
#define REG_C_FIQ_MASK                          REG_AEON_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_AEON_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_AEON_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_AEON_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_AEON_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_AEON_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_AEON_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_AEON_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_AEON_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_AEON_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_AEON_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_AEON_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_AEON_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_AEON_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_AEON_C_IRQ_HYP_FINAL_STATUS
#endif
#endif


#ifdef MCU_MIPS_34K
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS

#endif


#ifdef MCU_MIPS_4KE

#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS

#endif

#ifdef MCU_MIPS_74K
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif

#ifdef MCU_ARM_CA12
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif


#ifdef MCU_ARM_CA7
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif

#ifdef CONFIG_FPGA
#define REG_C_FIQ_MASK                          REG_MIPS_C_FIQ_MASK
#define REG_C_FIQ_CLR                           REG_MIPS_C_FIQ_CLR
#define REG_FIQ_FINAL_STATUS                    REG_MIPS_FIQ_FINAL_STATUS

#define REG_C_IRQ_MASK                          REG_MIPS_C_IRQ_MASK
#define REG_IRQ_FINAL_STATUS                    REG_MIPS_IRQ_FINAL_STATUS

#define REG_C_FIQ_EXP_MASK                      REG_MIPS_C_FIQ_EXP_MASK
#define REG_C_FIQ_EXP_CLR                       REG_MIPS_C_FIQ_EXP_CLR
#define REG_C_FIQ_EXP_FINAL_STATUS              REG_MIPS_C_FIQ_EXP_FINAL_STATUS

#define REG_C_FIQ_HYP_MASK                      REG_MIPS_C_FIQ_HYP_MASK
#define REG_C_FIQ_HYP_CLR                       REG_MIPS_C_FIQ_HYP_CLR
#define REG_C_FIQ_HYP_FINAL_STATUS              REG_MIPS_C_FIQ_HYP_FINAL_STATUS

#define REG_C_IRQ_EXP_MASK                      REG_MIPS_C_IRQ_EXP_MASK
#define REG_C_IRQ_EXP_FINAL_STATUS              REG_MIPS_C_IRQ_EXP_FINAL_STATUS

#define REG_C_IRQ_HYP_MASK                      REG_MIPS_C_IRQ_HYP_MASK
#define REG_C_IRQ_HYP_FINAL_STATUS              REG_MIPS_C_IRQ_HYP_FINAL_STATUS
#endif
/*
#define REG_SEND_IRQ_FROM_CPU0                  (RIUBASE_IRQ + 0x7C)
#define REG_SEND_IRQ_FROM_CPU1                  (RIUBASE_IRQ + 0x7E)
#define REG_SEND_IRQ_FROM_CPU2                  (RIUBASE_IRQ_AEON1 + 0x7E)
*/

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_IRQ_H_
