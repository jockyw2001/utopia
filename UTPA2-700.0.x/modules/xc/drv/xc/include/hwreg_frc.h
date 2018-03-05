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

#ifndef _HWREG_FRC_H_
#define _HWREG_FRC_H_

#include "MsCommon.h"


#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

//=============================================================
#define REG_FRC_CHIP_BASE           (REG_FRC_BANK_BASE+0x1E00)   //REG_FRC_CHIP_BASE

#define REG_FRC_SC16_BASE           (REG_FRC_BANK_BASE+0x1600)  //REG_FRC_SC0_IP_BASE

#define REG_FRC_SC18_BASE           (REG_FRC_BANK_BASE+0x1800)  //

#define REG_FRC_SC20_BASE           (REG_FRC_BANK_BASE+0x2000)  //REG_FRC_SC0_IP_BASE
#define REG_FRC_SC21_BASE           (REG_FRC_BANK_BASE+0x2100)  //REG_FRC_SC1_VBI_BASE
#define REG_FRC_SC22_BASE           (REG_FRC_BANK_BASE+0x2200)  //REG_FRC_SC2_LVDSRX_BASE
#define REG_FRC_SC23_BASE           (REG_FRC_BANK_BASE+0x2300)  //REG_FRC_SC3_TCON_BASE
#define REG_FRC_SC24_BASE           (REG_FRC_BANK_BASE+0x2400)  //REG_FRC_SC4_SCTOP_BASE
#define REG_FRC_SC25_BASE           (REG_FRC_BANK_BASE+0x2500)  //REG_FRC_SC5_PWM_BASE
#define REG_FRC_SC26_BASE           (REG_FRC_BANK_BASE+0x2600)  //REG_FRC_SC6_MCPLUS_BASE
#define REG_FRC_SC27_BASE           (REG_FRC_BANK_BASE+0x2700)  //REG_FRC_SC7_MC_3FRAME_BASE
#define REG_FRC_SC28_BASE           (REG_FRC_BANK_BASE+0x2800)  //REG_FRC_SC8_OD_BASE
#define REG_FRC_SC29_BASE           (REG_FRC_BANK_BASE+0x2900)  //REG_FRC_SC9_MC2D_BASE
#define REG_FRC_SC2A_BASE           (REG_FRC_BANK_BASE+0x2A00)  //REG_FRC_SC10_DISP_LPLL_BASE
#define REG_FRC_SC2C_BASE           (REG_FRC_BANK_BASE+0x2C00)  //REG_FRC_SC16_MC2D_BASE
#define REG_FRC_SC2D_BASE           (REG_FRC_BANK_BASE+0x2D00)  //REG_FRC_SC17_IP_BASE
#define REG_FRC_SC2E_BASE           (REG_FRC_BANK_BASE+0x2E00)  //REG_FRC_SC11_SNR_BASE
#define REG_FRC_SC2F_BASE           (REG_FRC_BANK_BASE+0x2F00)  //REG_FRC_SC12_DISP_TGEN_BASE
#define REG_FRC_SC30_BASE           (REG_FRC_BANK_BASE+0x3000)  //REG_FRC_SC13_VOP2_BASE
#define REG_FRC_SC31_BASE           (REG_FRC_BANK_BASE+0x3100)  //REG_FRC_SC14_MFT_BASE
#define REG_FRC_SC32_BASE           (REG_FRC_BANK_BASE+0x3200)  //REG_FRC_SC15_MISC_BASE
#define REG_FRC_SC33_BASE           (REG_FRC_BANK_BASE+0x3300)  //REG_FRC_SC18_LD_BASE
#define REG_FRC_SC38_BASE           (REG_FRC_BANK_BASE+0x3800)  //REG_FRC_SC19_OD_LG_BASE
#define REG_FRC_SC3A_BASE           (REG_FRC_BANK_BASE+0x3A00)  //REG_FRC_SC20_MC2D_BASE
#define REG_FRC_SC3B_BASE           (REG_FRC_BANK_BASE+0x3B00)  //REG_FRC_SC21_NEW_FILM_BASE
#define REG_FRC_SC3D_BASE           (REG_FRC_BANK_BASE+0x3D00)  //REG_FRC_SC22_ADL_BASE
#define REG_FRC_SC3E_BASE           (REG_FRC_BANK_BASE+0x3E00)  //REG_FRC_SC25_HVSP_BASE
#define REG_FRC_SC130_BASE          (REG_FRC_BANK_BASE+0x13000) //REG_FRC_SC26_3DD_BASE
#define REG_FRC_SC134_BASE          (REG_FRC_BANK_BASE+0x13400) //REG_FRC_SC134_IPM_BASE
#define REG_FRC_SC135_BASE          (REG_FRC_BANK_BASE+0x13500) //REG_FRC_SC135_OPM_BASE

//=============================================================
#define L_FRC_CHIP(x)               BK_REG_L(REG_FRC_CHIP_BASE, x)
#define H_FRC_CHIP(x)               BK_REG_H(REG_FRC_CHIP_BASE, x)

#define L_FRC_SC16(x)               BK_REG_L(REG_FRC_SC16_BASE, x)
#define H_FRC_SC16(x)               BK_REG_H(REG_FRC_SC16_BASE, x)

#define L_FRC_SC18(x)               BK_REG_L(REG_FRC_SC18_BASE, x)
#define H_FRC_SC18(x)               BK_REG_H(REG_FRC_SC18_BASE, x)

#define L_FRC_SC20(x)               BK_REG_L(REG_FRC_SC20_BASE, x)
#define H_FRC_SC20(x)               BK_REG_H(REG_FRC_SC20_BASE, x)
#define L_FRC_SC21(x)               BK_REG_L(REG_FRC_SC21_BASE, x)
#define H_FRC_SC21(x)               BK_REG_H(REG_FRC_SC21_BASE, x)
#define L_FRC_SC22(x)               BK_REG_L(REG_FRC_SC22_BASE, x)
#define H_FRC_SC22(x)               BK_REG_H(REG_FRC_SC22_BASE, x)
#define L_FRC_SC23(x)               BK_REG_L(REG_FRC_SC23_BASE, x)
#define H_FRC_SC23(x)               BK_REG_H(REG_FRC_SC23_BASE, x)
#define L_FRC_SC24(x)               BK_REG_L(REG_FRC_SC24_BASE, x)
#define H_FRC_SC24(x)               BK_REG_H(REG_FRC_SC24_BASE, x)
#define L_FRC_SC25(x)               BK_REG_L(REG_FRC_SC25_BASE, x)
#define H_FRC_SC25(x)               BK_REG_H(REG_FRC_SC25_BASE, x)
#define L_FRC_SC26(x)               BK_REG_L(REG_FRC_SC26_BASE, x)
#define H_FRC_SC26(x)               BK_REG_H(REG_FRC_SC26_BASE, x)
#define L_FRC_SC27(x)               BK_REG_L(REG_FRC_SC27_BASE, x)
#define H_FRC_SC27(x)               BK_REG_H(REG_FRC_SC27_BASE, x)
#define L_FRC_SC28(x)               BK_REG_L(REG_FRC_SC28_BASE, x)
#define H_FRC_SC28(x)               BK_REG_H(REG_FRC_SC28_BASE, x)
#define L_FRC_SC29(x)               BK_REG_L(REG_FRC_SC29_BASE, x)
#define H_FRC_SC29(x)               BK_REG_H(REG_FRC_SC29_BASE, x)
#define L_FRC_SC2A(x)               BK_REG_L(REG_FRC_SC2A_BASE, x)
#define H_FRC_SC2A(x)               BK_REG_H(REG_FRC_SC2A_BASE, x)
#define L_FRC_SC2C(x)               BK_REG_L(REG_FRC_SC2C_BASE, x)
#define H_FRC_SC2C(x)               BK_REG_H(REG_FRC_SC2C_BASE, x)
#define L_FRC_SC2D(x)               BK_REG_L(REG_FRC_SC2D_BASE, x)
#define H_FRC_SC2D(x)               BK_REG_H(REG_FRC_SC2D_BASE, x)
#define L_FRC_SC2E(x)               BK_REG_L(REG_FRC_SC2E_BASE, x)
#define H_FRC_SC2E(x)               BK_REG_H(REG_FRC_SC2E_BASE, x)
#define L_FRC_SC2F(x)               BK_REG_L(REG_FRC_SC2F_BASE, x)
#define H_FRC_SC2F(x)               BK_REG_H(REG_FRC_SC2F_BASE, x)
#define L_FRC_SC30(x)               BK_REG_L(REG_FRC_SC30_BASE, x)
#define H_FRC_SC30(x)               BK_REG_H(REG_FRC_SC30_BASE, x)
#define L_FRC_SC31(x)               BK_REG_L(REG_FRC_SC31_BASE, x)
#define H_FRC_SC31(x)               BK_REG_H(REG_FRC_SC31_BASE, x)
#define L_FRC_SC32(x)               BK_REG_L(REG_FRC_SC32_BASE, x)
#define H_FRC_SC32(x)               BK_REG_H(REG_FRC_SC32_BASE, x)
#define L_FRC_SC33(x)               BK_REG_L(REG_FRC_SC33_BASE, x)
#define H_FRC_SC33(x)               BK_REG_H(REG_FRC_SC33_BASE, x)
#define L_FRC_SC38(x)               BK_REG_L(REG_FRC_SC38_BASE, x)
#define H_FRC_SC38(x)               BK_REG_H(REG_FRC_SC38_BASE, x)
#define L_FRC_SC3A(x)               BK_REG_L(REG_FRC_SC3A_BASE, x)
#define H_FRC_SC3A(x)               BK_REG_H(REG_FRC_SC3A_BASE, x)
#define L_FRC_SC3B(x)               BK_REG_L(REG_FRC_SC3B_BASE, x)
#define H_FRC_SC3B(x)               BK_REG_H(REG_FRC_SC3B_BASE, x)
#define L_FRC_SC3D(x)               BK_REG_L(REG_FRC_SC3D_BASE, x)
#define H_FRC_SC3D(x)               BK_REG_H(REG_FRC_SC3D_BASE, x)
#define L_FRC_SC3E(x)               BK_REG_L(REG_FRC_SC3E_BASE, x)
#define H_FRC_SC3E(x)               BK_REG_H(REG_FRC_SC3E_BASE, x)
#define L_FRC_SC3F(x)               BK_REG_L(REG_FRC_SC3E_BASE, x)
#define H_FRC_SC3F(x)               BK_REG_H(REG_FRC_SC3E_BASE, x)
#define L_FRC_SC130(x)              BK_REG_L(REG_FRC_SC130_BASE, x)
#define H_FRC_SC130(x)              BK_REG_H(REG_FRC_SC130_BASE, x)
#define L_FRC_SC134(x)              BK_REG_L(REG_FRC_SC134_BASE, x)
#define H_FRC_SC134(x)              BK_REG_H(REG_FRC_SC134_BASE, x)
#define L_FRC_SC135(x)              BK_REG_L(REG_FRC_SC135_BASE, x)
#define H_FRC_SC135(x)              BK_REG_H(REG_FRC_SC135_BASE, x)


#endif


