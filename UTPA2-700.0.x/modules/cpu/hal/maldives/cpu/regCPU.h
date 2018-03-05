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
//
/// @file  regMVD.h
/// @brief Hardware register definition for Video Decoder
/// @author MStar Semiconductor Inc.
//
///////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_COPRO_H_
#define _REG_COPRO_H_


////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Base Address
//------------------------------------------------------------------------------
// For Co-Processor
#define R2_REG_BASE                             0x022A00    //0x122A00 //sec_r2
#define R2_MAULV1_REG_BASE                      0x022B00UL    //0x122B00 //sec_r2_maulv1
// For Non-PM
#define NPM_REG_CLKGEN0                         0x000B00    //0x100b00 //clkgen0
#define NPM_REG_CLKGEN1                         0x003300    //0x103300 //clkgen1
#define NPM_REG_CHIPTOP                         0x001E00    //0x101E00 //chiptop
#define NPM_REG_MIU0                            0x001200    //0x101200 //miu0
#define NPM_REG_MIU1                            0x000600    //0x100600 //miu1
#define NPM_REG_ANAMISC                         0x010C00    //0x110c00 //ana misc
#define NPM_REG_PORT_STATUS                     0x000500    //0x100500 //port status

//For PM
#define PM_REG_BASE_SLEEP                       0x000E00    //0x002E00 //pm_sleep
#define PM_REG_BASE_MISC                        0x002E00    //0x002E00 //pm_misc

//------------------------------------------------------------------------------
// MCU and PIU Reg
//------------------------------------------------------------------------------
// For Co-Processor
#define R2_REG_STOP                             (R2_REG_BASE + 0x0080)
#define R2_REG_SDR_LO_INST_BASE                 (R2_REG_BASE + 0x0082)
#define R2_REG_SDR_HI_INST_BASE                 (R2_REG_BASE + 0x0084)
#define R2_REG_SDR_LO_DATA_BASE                 (R2_REG_BASE + 0x0086)
#define R2_REG_SDR_HI_DATA_BASE                 (R2_REG_BASE + 0x0088)
#define R2_REG_RST_BASE                         (R2_REG_BASE + 0x00B4)
#define R2_REG_RIU_BASE                         (R2_REG_BASE + 0x008A)
#define R2_REG_IO1_BASE                         (R2_REG_BASE + 0x00AA)
#define R2_REG_SPI_BASE                         (R2_REG_BASE+0x0090UL)
#define R2_REG_SPI_BASE1                        (R2_REG_BASE+0x00ACUL)
#define R2_REG_DQMEM_BASE                       (R2_REG_BASE+0x009CUL)
#define R2_REG_SPACE_EN                         (R2_REG_BASE + 0x00B0)
#define R2_REG_QMEM_MASK_HIGH                   (R2_REG_BASE + 0x00A0)
#define R2_REG_QMEM_BASE_HIGH                   (R2_REG_BASE + 0x009C)
#define R2_MAULV1_REG                           (R2_MAULV1_REG_BASE + 0x0002UL)    //0x122B00 //sec_r2_maulv1
// For Non-PM
#define NPM_REG_CLKGEN0_SECR2                   (NPM_REG_CLKGEN0 + 0x0094) //4A*2
#define NPM_REG_CHIPTOP_UART                    (NPM_REG_CHIPTOP + 0x00A6)
#define NPM_REG_MIPS_PLLCLK                     (NPM_REG_ANAMISC + 0x0026)
#define NPM_REG_LPF_LOW                         (NPM_REG_ANAMISC + 0x00C0)
#define NPM_REG_LPF_HIGH                        (NPM_REG_ANAMISC + 0x00C2)

#define NPM_REG_CPU_CLOCK                       (NPM_REG_PORT_STATUS + 0x001E)

// For PM
#define PM_REG_CPUX_SW_RSTZ                     (PM_REG_BASE_MISC + 0x0052)

#define NONE_CACHEABLE                          0x80000000

#endif // _REG_COPRO_H_



