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

#ifndef _HWREG_HDCP_H_
#define _HWREG_HDCP_H_

//=============================================================
//HDCP
//#define REG_HDCP_BASE       0x26F0

#define REG_HDCPKEY_00_L    (REG_HDCPKEY_BASE + 0x00)
#define REG_HDCPKEY_00_H    (REG_HDCPKEY_BASE + 0x01)
#define REG_HDCPKEY_01_L    (REG_HDCPKEY_BASE + 0x02)
#define REG_HDCPKEY_01_H    (REG_HDCPKEY_BASE + 0x03)
#define REG_HDCPKEY_02_L    (REG_HDCPKEY_BASE + 0x04)
#define REG_HDCPKEY_02_H    (REG_HDCPKEY_BASE + 0x05)
#define REG_HDCPKEY_03_L    (REG_HDCPKEY_BASE + 0x06)
#define REG_HDCPKEY_03_H    (REG_HDCPKEY_BASE + 0x07)
#define REG_HDCPKEY_04_L    (REG_HDCPKEY_BASE + 0x08)
#define REG_HDCPKEY_04_H    (REG_HDCPKEY_BASE + 0x09)
#define REG_HDCPKEY_05_L    (REG_HDCPKEY_BASE + 0x0A)
#define REG_HDCPKEY_05_H    (REG_HDCPKEY_BASE + 0x0B)
#define REG_HDCPKEY_06_L    (REG_HDCPKEY_BASE + 0x0C)
#define REG_HDCPKEY_06_H    (REG_HDCPKEY_BASE + 0x0D)
#define REG_HDCPKEY_07_L    (REG_HDCPKEY_BASE + 0x0E)
#define REG_HDCPKEY_07_H    (REG_HDCPKEY_BASE + 0x0F)
#define REG_HDCPKEY_08_L    (REG_HDCPKEY_BASE + 0x10)
#define REG_HDCPKEY_08_H    (REG_HDCPKEY_BASE + 0x11)
#define REG_HDCPKEY_09_L    (REG_HDCPKEY_BASE + 0x12)
#define REG_HDCPKEY_09_H    (REG_HDCPKEY_BASE + 0x13)
#define REG_HDCPKEY_0A_L    (REG_HDCPKEY_BASE + 0x14)
#define REG_HDCPKEY_0A_H    (REG_HDCPKEY_BASE + 0x15)
#define REG_HDCPKEY_0B_L    (REG_HDCPKEY_BASE + 0x16)
#define REG_HDCPKEY_0B_H    (REG_HDCPKEY_BASE + 0x17)
#define REG_HDCPKEY_0C_L    (REG_HDCPKEY_BASE + 0x18)
#define REG_HDCPKEY_0C_H    (REG_HDCPKEY_BASE + 0x19)
#define REG_HDCPKEY_0D_L    (REG_HDCPKEY_BASE + 0x1A)
#define REG_HDCPKEY_0D_H    (REG_HDCPKEY_BASE + 0x1B)
#define REG_HDCPKEY_0E_L    (REG_HDCPKEY_BASE + 0x1C)
#define REG_HDCPKEY_0E_H    (REG_HDCPKEY_BASE + 0x1D)
#define REG_HDCPKEY_0F_L    (REG_HDCPKEY_BASE + 0x1E)
#define REG_HDCPKEY_0F_H    (REG_HDCPKEY_BASE + 0x1F)
#define REG_HDCPKEY_10_L    (REG_HDCPKEY_BASE + 0x20)
#define REG_HDCPKEY_10_H    (REG_HDCPKEY_BASE + 0x21)
#define REG_HDCPKEY_11_L    (REG_HDCPKEY_BASE + 0x22)
#define REG_HDCPKEY_11_H    (REG_HDCPKEY_BASE + 0x23)
#define REG_HDCPKEY_12_L    (REG_HDCPKEY_BASE + 0x24)
#define REG_HDCPKEY_12_H    (REG_HDCPKEY_BASE + 0x25)

#define REG_HDCP_00_L       (REG_HDCP_BASE + 0x00)
#define REG_HDCP_00_H       (REG_HDCP_BASE + 0x01)
#define REG_HDCP_01_L       (REG_HDCP_BASE + 0x02)
#define REG_HDCP_01_H       (REG_HDCP_BASE + 0x03)
#define REG_HDCP_02_L       (REG_HDCP_BASE + 0x04)
#define REG_HDCP_02_H       (REG_HDCP_BASE + 0x05)
#define REG_HDCP_03_L       (REG_HDCP_BASE + 0x06)
#define REG_HDCP_03_H       (REG_HDCP_BASE + 0x07)
#define REG_HDCP_09_L       (REG_HDCP_BASE + 0x12)
#define REG_HDCP_09_H       (REG_HDCP_BASE + 0x13)
#define REG_HDCP_15_L       (REG_HDCP_BASE + 0x2A)
#define REG_HDCP_15_H       (REG_HDCP_BASE + 0x2B)
#define REG_HDCP_15_L       (REG_HDCP_BASE + 0x2A)
#define REG_HDCP_15_H       (REG_HDCP_BASE + 0x2B)
#define REG_HDCP_17_L       (REG_HDCP_BASE + 0x2E)
#define REG_HDCP_17_H       (REG_HDCP_BASE + 0x2F)
#define REG_HDCP_18_L       (REG_HDCP_BASE + 0x30)
#define REG_HDCP_18_H       (REG_HDCP_BASE + 0x31)
#define REG_HDCP_19_L       (REG_HDCP_BASE + 0x32)
#define REG_HDCP_19_H       (REG_HDCP_BASE + 0x33)
#define REG_HDCP_1C_L       (REG_HDCP_BASE + 0x38)
#define REG_HDCP_1C_H       (REG_HDCP_BASE + 0x39)

#define REG_HDCP1_1C_L      (REG_HDCP1_BASE + 0x38)
#define REG_HDCP1_1C_H      (REG_HDCP1_BASE + 0x39)

#define REG_HDCP2_1C_L      (REG_HDCP2_BASE + 0x38)
#define REG_HDCP2_1C_H      (REG_HDCP2_BASE + 0x39)

#define REG_HDCP3_09_L      (REG_HDCP3_BASE + 0x12)
#define REG_HDCP3_09_H      (REG_HDCP3_BASE + 0x13)
#define REG_HDCP3_1C_L      (REG_HDCP3_BASE + 0x38)
#define REG_HDCP3_1C_H      (REG_HDCP3_BASE + 0x39)
#endif

