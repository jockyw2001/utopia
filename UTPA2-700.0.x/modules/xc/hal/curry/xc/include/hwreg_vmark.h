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

#ifndef _HWREG_VMARK_H_
#define _HWREG_VMARK_H_

//=============================================================
//VMark
//#define REG_VMARK0_BASE            0x173200UL
//#define REG_VMARK1_BASE            0x173300UL

#define	REG_VMARK_00_L		_PK_L_(0x00, 0x00)
#define	REG_VMARK_00_H		_PK_H_(0x00, 0x00)
#define	REG_VMARK_01_L		_PK_L_(0x00, 0x01)
#define	REG_VMARK_01_H		_PK_H_(0x00, 0x01)
#define	REG_VMARK_02_L		_PK_L_(0x00, 0x02)
#define	REG_VMARK_02_H		_PK_H_(0x00, 0x02)
#define	REG_VMARK_03_L		_PK_L_(0x00, 0x03)
#define	REG_VMARK_03_H		_PK_H_(0x00, 0x03)
#define	REG_VMARK_04_L		_PK_L_(0x00, 0x04)
#define	REG_VMARK_04_H		_PK_H_(0x00, 0x04)
#define	REG_VMARK_05_L		_PK_L_(0x00, 0x05)
#define	REG_VMARK_05_H		_PK_H_(0x00, 0x05)
#define	REG_VMARK_06_L		_PK_L_(0x00, 0x06)
#define	REG_VMARK_06_H		_PK_H_(0x00, 0x06)
#define	REG_VMARK_07_L		_PK_L_(0x00, 0x07)
#define	REG_VMARK_07_H		_PK_H_(0x00, 0x07)
#define	REG_VMARK_08_L		_PK_L_(0x00, 0x08)
#define	REG_VMARK_08_H		_PK_H_(0x00, 0x08)
#define	REG_VMARK_09_L		_PK_L_(0x00, 0x09)
#define	REG_VMARK_09_H		_PK_H_(0x00, 0x09)
#define	REG_VMARK_0A_L		_PK_L_(0x00, 0x0A)
#define	REG_VMARK_0A_H		_PK_H_(0x00, 0x0A)
#define	REG_VMARK_0B_L		_PK_L_(0x00, 0x0B)
#define	REG_VMARK_0B_H		_PK_H_(0x00, 0x0B)
#define	REG_VMARK_0C_L		_PK_L_(0x00, 0x0C)
#define	REG_VMARK_0C_H		_PK_H_(0x00, 0x0C)
#define	REG_VMARK_0D_L		_PK_L_(0x00, 0x0D)
#define	REG_VMARK_0D_H		_PK_H_(0x00, 0x0D)
#define	REG_VMARK_0E_L		_PK_L_(0x00, 0x0E)
#define	REG_VMARK_0E_H		_PK_H_(0x00, 0x0E)
#define	REG_VMARK_0F_L		_PK_L_(0x00, 0x0F)
#define	REG_VMARK_0F_H		_PK_H_(0x00, 0x0F)
#define	REG_VMARK_10_L		_PK_L_(0x00, 0x10)
#define	REG_VMARK_10_H		_PK_H_(0x00, 0x10)
#define	REG_VMARK_11_L		_PK_L_(0x00, 0x11)
#define	REG_VMARK_11_H		_PK_H_(0x00, 0x11)
#define	REG_VMARK_12_L		_PK_L_(0x00, 0x12)
#define	REG_VMARK_12_H		_PK_H_(0x00, 0x12)
#define	REG_VMARK_13_L		_PK_L_(0x00, 0x13)
#define	REG_VMARK_13_H		_PK_H_(0x00, 0x13)
#define	REG_VMARK_14_L		_PK_L_(0x00, 0x14)
#define	REG_VMARK_14_H		_PK_H_(0x00, 0x14)
#define	REG_VMARK_15_L		_PK_L_(0x00, 0x15)
#define	REG_VMARK_15_H		_PK_H_(0x00, 0x15)
#define	REG_VMARK_16_L		_PK_L_(0x00, 0x16)
#define	REG_VMARK_16_H		_PK_H_(0x00, 0x16)
#define	REG_VMARK_17_L		_PK_L_(0x00, 0x17)
#define	REG_VMARK_17_H		_PK_H_(0x00, 0x17)
#define	REG_VMARK_18_L		_PK_L_(0x00, 0x18)
#define	REG_VMARK_18_H		_PK_H_(0x00, 0x18)
#define	REG_VMARK_19_L		_PK_L_(0x00, 0x19)
#define	REG_VMARK_19_H		_PK_H_(0x00, 0x19)
#define	REG_VMARK_1A_L		_PK_L_(0x00, 0x1A)
#define	REG_VMARK_1A_H		_PK_H_(0x00, 0x1A)
#define	REG_VMARK_1B_L		_PK_L_(0x00, 0x1B)
#define	REG_VMARK_1B_H		_PK_H_(0x00, 0x1B)
#define	REG_VMARK_1C_L		_PK_L_(0x00, 0x1C)
#define	REG_VMARK_1C_H		_PK_H_(0x00, 0x1C)
#define	REG_VMARK_1D_L		_PK_L_(0x00, 0x1D)
#define	REG_VMARK_1D_H		_PK_H_(0x00, 0x1D)
#define	REG_VMARK_1E_L		_PK_L_(0x00, 0x1E)
#define	REG_VMARK_1E_H		_PK_H_(0x00, 0x1E)
#define	REG_VMARK_1F_L		_PK_L_(0x00, 0x1F)
#define	REG_VMARK_1F_H		_PK_H_(0x00, 0x1F)
#define	REG_VMARK_20_L		_PK_L_(0x00, 0x20)
#define	REG_VMARK_20_H		_PK_H_(0x00, 0x20)
#define	REG_VMARK_21_L		_PK_L_(0x00, 0x21)
#define	REG_VMARK_21_H		_PK_H_(0x00, 0x21)
#define	REG_VMARK_22_L		_PK_L_(0x00, 0x22)
#define	REG_VMARK_22_H		_PK_H_(0x00, 0x22)
#define	REG_VMARK_23_L		_PK_L_(0x00, 0x23)
#define	REG_VMARK_23_H		_PK_H_(0x00, 0x23)
#define	REG_VMARK_24_L		_PK_L_(0x00, 0x24)
#define	REG_VMARK_24_H		_PK_H_(0x00, 0x24)
#define	REG_VMARK_25_L		_PK_L_(0x00, 0x25)
#define	REG_VMARK_25_H		_PK_H_(0x00, 0x25)
#define	REG_VMARK_26_L		_PK_L_(0x00, 0x26)
#define	REG_VMARK_26_H		_PK_H_(0x00, 0x26)
#define	REG_VMARK_27_L		_PK_L_(0x00, 0x27)
#define	REG_VMARK_27_H		_PK_H_(0x00, 0x27)
#define	REG_VMARK_28_L		_PK_L_(0x00, 0x28)
#define	REG_VMARK_28_H		_PK_H_(0x00, 0x28)
#define	REG_VMARK_29_L		_PK_L_(0x00, 0x29)
#define	REG_VMARK_29_H		_PK_H_(0x00, 0x29)
#define	REG_VMARK_2A_L		_PK_L_(0x00, 0x2A)
#define	REG_VMARK_2A_H		_PK_H_(0x00, 0x2A)
#define	REG_VMARK_2B_L		_PK_L_(0x00, 0x2B)
#define	REG_VMARK_2B_H		_PK_H_(0x00, 0x2B)
#define	REG_VMARK_2C_L		_PK_L_(0x00, 0x2C)
#define	REG_VMARK_2C_H		_PK_H_(0x00, 0x2C)
#define	REG_VMARK_2D_L		_PK_L_(0x00, 0x2D)
#define	REG_VMARK_2D_H		_PK_H_(0x00, 0x2D)
#define	REG_VMARK_2E_L		_PK_L_(0x00, 0x2E)
#define	REG_VMARK_2E_H		_PK_H_(0x00, 0x2E)
#define	REG_VMARK_2F_L		_PK_L_(0x00, 0x2F)
#define	REG_VMARK_2F_H		_PK_H_(0x00, 0x2F)
#define	REG_VMARK_30_L		_PK_L_(0x00, 0x30)
#define	REG_VMARK_30_H		_PK_H_(0x00, 0x30)
#define	REG_VMARK_31_L		_PK_L_(0x00, 0x31)
#define	REG_VMARK_31_H		_PK_H_(0x00, 0x31)
#define	REG_VMARK_32_L		_PK_L_(0x00, 0x32)
#define	REG_VMARK_32_H		_PK_H_(0x00, 0x32)
#define	REG_VMARK_33_L		_PK_L_(0x00, 0x33)
#define	REG_VMARK_33_H		_PK_H_(0x00, 0x33)
#define	REG_VMARK_34_L		_PK_L_(0x00, 0x34)
#define	REG_VMARK_34_H		_PK_H_(0x00, 0x34)


#endif

