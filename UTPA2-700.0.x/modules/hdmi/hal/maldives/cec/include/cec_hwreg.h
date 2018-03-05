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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

// Scaler register serpead define
#define SCALER_REGISTER_SPREAD 1

//PM
#define REG_PM_SLP_BASE             0x000E00
#define REG_CEC_BASE                0x001100
#define REG_PM_MCU_BASE             0x001000

//NONPM
#define REG_MIU0_BASE               0x101200
#define REG_MIU1_BASE               0x100600
#define REG_CHIPTOP_BASE            0x101E00  // 0x1E00 - 0x1EFF
#define REG_UHC0_BASE               0x102400
#define REG_ADC_ATOP_BASE           0x102500  // 0x2500 - 0x25FF
#define REG_ADC_DTOP_BASE           0x102600  // 0x2600 - 0x26EF
#define REG_IPMUX_BASE              0x102E00
#if SCALER_REGISTER_SPREAD
#define REG_SCALER_BASE             0x130000
#else
#define REG_SCALER_BASE             0x102F00
#endif
#define REG_LPLL_BASE               0x103100
#define REG_MOD_BASE                0x103200

#define REG_HDMI_BASE               0x102700  // 0x2700 - 0x27FF
#define REG_HDMI2_BASE            0x101A00
#define REG_DVI_ATOP_BASE           0x110900
#define REG_DVI_DTOP_BASE           0x110A00
#define REG_DVI_EQ_BASE             0x110A80     // EQ started from 0x80
#define REG_HDCP_BASE               0x110AC0     // HDCP started from 0xC0
#define REG_DVI_ATOP1_BASE           0x113200
#define REG_DVI_DTOP1_BASE           0x113300
#define REG_DVI_EQ1_BASE             0x113380     // EQ started from 0x80
#define REG_HDCP1_BASE               0x1133C0     // HDCP started from 0xC0
#define REG_DVI_ATOP2_BASE           0x113400
#define REG_DVI_DTOP2_BASE           0x113500
#define REG_DVI_EQ2_BASE             0x113580     // EQ started from 0x80
#define REG_HDCP2_BASE               0x1135C0     // HDCP started from 0xC0
#define REG_DVI_PS_BASE              0x113600 // DVI power saving
#define REG_DVI_DTOP3_BASE           0x113700
#define REG_DVI_EQ3_BASE             0x113780     // EQ started from 0x80
#define REG_HDCP3_BASE               0x1137C0     // HDCP started from 0xC0
#define REG_MHL_TMDS_BASE            0x122700 

#define REG_DVI_ATOP_06_L    (REG_DVI_ATOP_BASE + 0x0C)
#define REG_DVI_ATOP_70_H    (REG_DVI_ATOP_BASE + 0xE1)
#define REG_DVI_ATOP_71_H    (REG_DVI_ATOP_BASE + 0xE3)
#define REG_DVI_ATOP_72_H    (REG_DVI_ATOP_BASE + 0xE4)
#define REG_DVI_ATOP_73_H    (REG_DVI_ATOP_BASE + 0xE6)
#define REG_DVI_ATOP1_06_L    (REG_DVI_ATOP1_BASE + 0x0C)
#define REG_DVI_ATOP1_70_H    (REG_DVI_ATOP1_BASE + 0xE1)
#define REG_DVI_ATOP1_71_H    (REG_DVI_ATOP1_BASE + 0xE3)
#define REG_DVI_ATOP2_06_L    (REG_DVI_ATOP2_BASE + 0x0C)
#define REG_DVI_ATOP2_70_H    (REG_DVI_ATOP2_BASE + 0xE1)
#define REG_DVI_ATOP2_71_H    (REG_DVI_ATOP2_BASE + 0xE3)
#define REG_DVI_EQ_00_L     (REG_DVI_EQ_BASE + 0x00)
#define REG_DVI_EQ_00_H     (REG_DVI_EQ_BASE + 0x01)
#define REG_DVI_EQ1_00_L     (REG_DVI_EQ1_BASE + 0x00)
#define REG_DVI_EQ1_00_H     (REG_DVI_EQ1_BASE + 0x01)
#define REG_DVI_EQ2_00_L     (REG_DVI_EQ2_BASE + 0x00)
#define REG_DVI_EQ2_00_H     (REG_DVI_EQ2_BASE + 0x01)
#define REG_DVI_EQ3_00_L     (REG_DVI_EQ3_BASE + 0x00)
#define REG_DVI_EQ3_00_H     (REG_DVI_EQ3_BASE + 0x01)
#define REG_HDCP_01_H       (REG_HDCP_BASE + 0x02)
#define REG_HDCP_15_L       (REG_HDCP_BASE + 0x2A)
#define REG_HDCP_15_H       (REG_HDCP_BASE + 0x2B)
#define REG_HDCP1_01_H       (REG_HDCP1_BASE + 0x02)
#define REG_HDCP1_15_L       (REG_HDCP1_BASE + 0x2A)
#define REG_HDCP1_15_H       (REG_HDCP1_BASE + 0x2B)
#define REG_HDCP2_01_H       (REG_HDCP2_BASE + 0x02)
#define REG_HDCP2_15_L       (REG_HDCP2_BASE + 0x2A)
#define REG_HDCP2_15_H       (REG_HDCP2_BASE + 0x2B)
#define REG_HDCP3_01_H       (REG_HDCP3_BASE + 0x02)
#define REG_HDCP3_15_L       (REG_HDCP3_BASE + 0x2A)
#define REG_HDCP3_15_H       (REG_HDCP3_BASE + 0x2B)
#define REG_HDMI2_26_L       (REG_HDMI2_BASE + 0x4C)
#define REG_HDMI2_26_H       (REG_HDMI2_BASE + 0x4D)
#define REG_HDMI2_27_L       (REG_HDMI2_BASE + 0x4E)
#define REG_HDMI2_27_H       (REG_HDMI2_BASE + 0x4F)
#define REG_MHL_TMDS_60_L       (REG_MHL_TMDS_BASE + 0xC0)
#define REG_MHL_TMDS_60_H       (REG_MHL_TMDS_BASE + 0xC1)
#define REG_MHL_TMDS_63_L       (REG_MHL_TMDS_BASE + 0xC6)
#define REG_MHL_TMDS_63_H       (REG_MHL_TMDS_BASE + 0xC7)
