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
//alex_tung
#ifndef _HWREG_PM_SLEEP_H_
#define _HWREG_PM_SLEEP_H_

//=============================================================
//PM SLEEP
//#define REG_PM_SLEEP_BASE       0x0E00UL
#define REG_PM_SLEEP_00_L       (REG_PM_SLEEP_BASE + 0x00)
#define REG_PM_SLEEP_00_H       (REG_PM_SLEEP_BASE + 0x01)
#define REG_PM_SLEEP_01_L       (REG_PM_SLEEP_BASE + 0x02)
#define REG_PM_SLEEP_01_H       (REG_PM_SLEEP_BASE + 0x03)
#define REG_PM_SLEEP_02_L       (REG_PM_SLEEP_BASE + 0x04)
#define REG_PM_SLEEP_02_H       (REG_PM_SLEEP_BASE + 0x05)
#define REG_PM_SLEEP_03_L       (REG_PM_SLEEP_BASE + 0x06)
#define REG_PM_SLEEP_03_H       (REG_PM_SLEEP_BASE + 0x07)
#define REG_PM_SLEEP_04_L       (REG_PM_SLEEP_BASE + 0x08)
#define REG_PM_SLEEP_04_H       (REG_PM_SLEEP_BASE + 0x09)
#define REG_PM_SLEEP_05_L       (REG_PM_SLEEP_BASE + 0x0A)
#define REG_PM_SLEEP_05_H       (REG_PM_SLEEP_BASE + 0x0B)
#define REG_PM_SLEEP_06_L       (REG_PM_SLEEP_BASE + 0x0C)
#define REG_PM_SLEEP_06_H       (REG_PM_SLEEP_BASE + 0x0D)
#define REG_PM_SLEEP_07_L       (REG_PM_SLEEP_BASE + 0x0E)
#define REG_PM_SLEEP_07_H       (REG_PM_SLEEP_BASE + 0x0F)
#define REG_PM_SLEEP_08_L       (REG_PM_SLEEP_BASE + 0x10)
#define REG_PM_SLEEP_08_H       (REG_PM_SLEEP_BASE + 0x11)
#define REG_PM_SLEEP_09_L       (REG_PM_SLEEP_BASE + 0x12)
#define REG_PM_SLEEP_09_H       (REG_PM_SLEEP_BASE + 0x13)
#define REG_PM_SLEEP_0A_L       (REG_PM_SLEEP_BASE + 0x14)
#define REG_PM_SLEEP_0A_H       (REG_PM_SLEEP_BASE + 0x15)
#define REG_PM_SLEEP_0B_L       (REG_PM_SLEEP_BASE + 0x16)
#define REG_PM_SLEEP_0B_H       (REG_PM_SLEEP_BASE + 0x17)
#define REG_PM_SLEEP_0C_L       (REG_PM_SLEEP_BASE + 0x18)
#define REG_PM_SLEEP_0C_H       (REG_PM_SLEEP_BASE + 0x19)
#define REG_PM_SLEEP_0D_L       (REG_PM_SLEEP_BASE + 0x1A)
#define REG_PM_SLEEP_0D_H       (REG_PM_SLEEP_BASE + 0x1B)
#define REG_PM_SLEEP_0E_L       (REG_PM_SLEEP_BASE + 0x1C)
#define REG_PM_SLEEP_0E_H       (REG_PM_SLEEP_BASE + 0x1D)
#define REG_PM_SLEEP_0F_L       (REG_PM_SLEEP_BASE + 0x1E)
#define REG_PM_SLEEP_0F_H       (REG_PM_SLEEP_BASE + 0x1F)
#define REG_PM_SLEEP_10_L       (REG_PM_SLEEP_BASE + 0x20)
#define REG_PM_SLEEP_10_H       (REG_PM_SLEEP_BASE + 0x21)
#define REG_PM_SLEEP_11_L       (REG_PM_SLEEP_BASE + 0x22)
#define REG_PM_SLEEP_11_H       (REG_PM_SLEEP_BASE + 0x23)
#define REG_PM_SLEEP_12_L       (REG_PM_SLEEP_BASE + 0x24)
#define REG_PM_SLEEP_12_H       (REG_PM_SLEEP_BASE + 0x25)
#define REG_PM_SLEEP_13_L       (REG_PM_SLEEP_BASE + 0x26)
#define REG_PM_SLEEP_13_H       (REG_PM_SLEEP_BASE + 0x27)
#define REG_PM_SLEEP_14_L       (REG_PM_SLEEP_BASE + 0x28)
#define REG_PM_SLEEP_14_H       (REG_PM_SLEEP_BASE + 0x29)
#define REG_PM_SLEEP_15_L       (REG_PM_SLEEP_BASE + 0x2A)
#define REG_PM_SLEEP_15_H       (REG_PM_SLEEP_BASE + 0x2B)
#define REG_PM_SLEEP_16_L       (REG_PM_SLEEP_BASE + 0x2C)
#define REG_PM_SLEEP_16_H       (REG_PM_SLEEP_BASE + 0x2D)
#define REG_PM_SLEEP_17_L       (REG_PM_SLEEP_BASE + 0x2E)
#define REG_PM_SLEEP_17_H       (REG_PM_SLEEP_BASE + 0x2F)
#define REG_PM_SLEEP_18_L       (REG_PM_SLEEP_BASE + 0x30)
#define REG_PM_SLEEP_18_H       (REG_PM_SLEEP_BASE + 0x31)
#define REG_PM_SLEEP_19_L       (REG_PM_SLEEP_BASE + 0x32)
#define REG_PM_SLEEP_19_H       (REG_PM_SLEEP_BASE + 0x33)
#define REG_PM_SLEEP_1A_L       (REG_PM_SLEEP_BASE + 0x34)
#define REG_PM_SLEEP_1A_H       (REG_PM_SLEEP_BASE + 0x35)
#define REG_PM_SLEEP_1B_L       (REG_PM_SLEEP_BASE + 0x36)
#define REG_PM_SLEEP_1B_H       (REG_PM_SLEEP_BASE + 0x37)
#define REG_PM_SLEEP_1C_L       (REG_PM_SLEEP_BASE + 0x38)
#define REG_PM_SLEEP_1C_H       (REG_PM_SLEEP_BASE + 0x39)
#define REG_PM_SLEEP_1D_L       (REG_PM_SLEEP_BASE + 0x3A)
#define REG_PM_SLEEP_1D_H       (REG_PM_SLEEP_BASE + 0x3B)
#define REG_PM_SLEEP_1E_L       (REG_PM_SLEEP_BASE + 0x3C)
#define REG_PM_SLEEP_1E_H       (REG_PM_SLEEP_BASE + 0x3D)
#define REG_PM_SLEEP_1F_L       (REG_PM_SLEEP_BASE + 0x3E)
#define REG_PM_SLEEP_1F_H       (REG_PM_SLEEP_BASE + 0x3F)
#define REG_PM_SLEEP_20_L       (REG_PM_SLEEP_BASE + 0x40)
#define REG_PM_SLEEP_20_H       (REG_PM_SLEEP_BASE + 0x41)
#define REG_PM_SLEEP_21_L       (REG_PM_SLEEP_BASE + 0x42)
#define REG_PM_SLEEP_21_H       (REG_PM_SLEEP_BASE + 0x43)
#define REG_PM_SLEEP_22_L       (REG_PM_SLEEP_BASE + 0x44)
#define REG_PM_SLEEP_22_H       (REG_PM_SLEEP_BASE + 0x45)
#define REG_PM_SLEEP_23_L       (REG_PM_SLEEP_BASE + 0x46)
#define REG_PM_SLEEP_23_H       (REG_PM_SLEEP_BASE + 0x47)
#define REG_PM_SLEEP_24_L       (REG_PM_SLEEP_BASE + 0x48)
#define REG_PM_SLEEP_24_H       (REG_PM_SLEEP_BASE + 0x49)
#define REG_PM_SLEEP_25_L       (REG_PM_SLEEP_BASE + 0x4A)
#define REG_PM_SLEEP_25_H       (REG_PM_SLEEP_BASE + 0x4B)
#define REG_PM_SLEEP_26_L       (REG_PM_SLEEP_BASE + 0x4C)
#define REG_PM_SLEEP_26_H       (REG_PM_SLEEP_BASE + 0x4D)
#define REG_PM_SLEEP_27_L       (REG_PM_SLEEP_BASE + 0x4E)
#define REG_PM_SLEEP_27_H       (REG_PM_SLEEP_BASE + 0x4F)
#define REG_PM_SLEEP_28_L       (REG_PM_SLEEP_BASE + 0x50)
#define REG_PM_SLEEP_28_H       (REG_PM_SLEEP_BASE + 0x51)
#define REG_PM_SLEEP_29_L       (REG_PM_SLEEP_BASE + 0x52)
#define REG_PM_SLEEP_29_H       (REG_PM_SLEEP_BASE + 0x53)
#define REG_PM_SLEEP_2A_L       (REG_PM_SLEEP_BASE + 0x54)
#define REG_PM_SLEEP_2A_H       (REG_PM_SLEEP_BASE + 0x55)
#define REG_PM_SLEEP_2B_L       (REG_PM_SLEEP_BASE + 0x56)
#define REG_PM_SLEEP_2B_H       (REG_PM_SLEEP_BASE + 0x57)
#define REG_PM_SLEEP_2C_L       (REG_PM_SLEEP_BASE + 0x58)
#define REG_PM_SLEEP_2C_H       (REG_PM_SLEEP_BASE + 0x59)
#define REG_PM_SLEEP_2D_L       (REG_PM_SLEEP_BASE + 0x5A)
#define REG_PM_SLEEP_2D_H       (REG_PM_SLEEP_BASE + 0x5B)
#define REG_PM_SLEEP_2E_L       (REG_PM_SLEEP_BASE + 0x5C)
#define REG_PM_SLEEP_2E_H       (REG_PM_SLEEP_BASE + 0x5D)
#define REG_PM_SLEEP_2F_L       (REG_PM_SLEEP_BASE + 0x5E)
#define REG_PM_SLEEP_2F_H       (REG_PM_SLEEP_BASE + 0x5F)
#define REG_PM_SLEEP_30_L       (REG_PM_SLEEP_BASE + 0x60)
#define REG_PM_SLEEP_30_H       (REG_PM_SLEEP_BASE + 0x61)
#define REG_PM_SLEEP_31_L       (REG_PM_SLEEP_BASE + 0x62)
#define REG_PM_SLEEP_31_H       (REG_PM_SLEEP_BASE + 0x63)
#define REG_PM_SLEEP_32_L       (REG_PM_SLEEP_BASE + 0x64)
#define REG_PM_SLEEP_32_H       (REG_PM_SLEEP_BASE + 0x65)
#define REG_PM_SLEEP_33_L       (REG_PM_SLEEP_BASE + 0x66)
#define REG_PM_SLEEP_33_H       (REG_PM_SLEEP_BASE + 0x67)
#define REG_PM_SLEEP_34_L       (REG_PM_SLEEP_BASE + 0x68)
#define REG_PM_SLEEP_34_H       (REG_PM_SLEEP_BASE + 0x69)
#define REG_PM_SLEEP_35_L       (REG_PM_SLEEP_BASE + 0x6A)
#define REG_PM_SLEEP_35_H       (REG_PM_SLEEP_BASE + 0x6B)
#define REG_PM_SLEEP_36_L       (REG_PM_SLEEP_BASE + 0x6C)
#define REG_PM_SLEEP_36_H       (REG_PM_SLEEP_BASE + 0x6D)
#define REG_PM_SLEEP_37_L       (REG_PM_SLEEP_BASE + 0x6E)
#define REG_PM_SLEEP_37_H       (REG_PM_SLEEP_BASE + 0x6F)
#define REG_PM_SLEEP_38_L       (REG_PM_SLEEP_BASE + 0x70)
#define REG_PM_SLEEP_38_H       (REG_PM_SLEEP_BASE + 0x71)
#define REG_PM_SLEEP_39_L       (REG_PM_SLEEP_BASE + 0x72)
#define REG_PM_SLEEP_39_H       (REG_PM_SLEEP_BASE + 0x73)
#define REG_PM_SLEEP_3A_L       (REG_PM_SLEEP_BASE + 0x74)
#define REG_PM_SLEEP_3A_H       (REG_PM_SLEEP_BASE + 0x75)
#define REG_PM_SLEEP_3B_L       (REG_PM_SLEEP_BASE + 0x76)
#define REG_PM_SLEEP_3B_H       (REG_PM_SLEEP_BASE + 0x77)
#define REG_PM_SLEEP_3C_L       (REG_PM_SLEEP_BASE + 0x78)
#define REG_PM_SLEEP_3C_H       (REG_PM_SLEEP_BASE + 0x79)
#define REG_PM_SLEEP_3D_L       (REG_PM_SLEEP_BASE + 0x7A)
#define REG_PM_SLEEP_3D_H       (REG_PM_SLEEP_BASE + 0x7B)
#define REG_PM_SLEEP_3E_L       (REG_PM_SLEEP_BASE + 0x7C)
#define REG_PM_SLEEP_3E_H       (REG_PM_SLEEP_BASE + 0x7D)
#define REG_PM_SLEEP_3F_L       (REG_PM_SLEEP_BASE + 0x7E)
#define REG_PM_SLEEP_3F_H       (REG_PM_SLEEP_BASE + 0x7F)
#define REG_PM_SLEEP_40_L       (REG_PM_SLEEP_BASE + 0x80)
#define REG_PM_SLEEP_40_H       (REG_PM_SLEEP_BASE + 0x81)
#define REG_PM_SLEEP_41_L       (REG_PM_SLEEP_BASE + 0x82)
#define REG_PM_SLEEP_41_H       (REG_PM_SLEEP_BASE + 0x83)
#define REG_PM_SLEEP_42_L       (REG_PM_SLEEP_BASE + 0x84)
#define REG_PM_SLEEP_42_H       (REG_PM_SLEEP_BASE + 0x85)
#define REG_PM_SLEEP_43_L       (REG_PM_SLEEP_BASE + 0x86)
#define REG_PM_SLEEP_43_H       (REG_PM_SLEEP_BASE + 0x87)
#define REG_PM_SLEEP_44_L       (REG_PM_SLEEP_BASE + 0x88)
#define REG_PM_SLEEP_44_H       (REG_PM_SLEEP_BASE + 0x89)
#define REG_PM_SLEEP_45_L       (REG_PM_SLEEP_BASE + 0x8A)
#define REG_PM_SLEEP_45_H       (REG_PM_SLEEP_BASE + 0x8B)
#define REG_PM_SLEEP_46_L       (REG_PM_SLEEP_BASE + 0x8C)
#define REG_PM_SLEEP_46_H       (REG_PM_SLEEP_BASE + 0x8D)
#define REG_PM_SLEEP_47_L       (REG_PM_SLEEP_BASE + 0x8E)
#define REG_PM_SLEEP_47_H       (REG_PM_SLEEP_BASE + 0x8F)
#define REG_PM_SLEEP_48_L       (REG_PM_SLEEP_BASE + 0x90)
#define REG_PM_SLEEP_48_H       (REG_PM_SLEEP_BASE + 0x91)
#define REG_PM_SLEEP_49_L       (REG_PM_SLEEP_BASE + 0x92)
#define REG_PM_SLEEP_49_H       (REG_PM_SLEEP_BASE + 0x93)
#define REG_PM_SLEEP_4A_L       (REG_PM_SLEEP_BASE + 0x94)
#define REG_PM_SLEEP_4A_H       (REG_PM_SLEEP_BASE + 0x95)
#define REG_PM_SLEEP_4B_L       (REG_PM_SLEEP_BASE + 0x96)
#define REG_PM_SLEEP_4B_H       (REG_PM_SLEEP_BASE + 0x97)
#define REG_PM_SLEEP_4C_L       (REG_PM_SLEEP_BASE + 0x98)
#define REG_PM_SLEEP_4C_H       (REG_PM_SLEEP_BASE + 0x99)
#define REG_PM_SLEEP_4D_L       (REG_PM_SLEEP_BASE + 0x9A)
#define REG_PM_SLEEP_4D_H       (REG_PM_SLEEP_BASE + 0x9B)
#define REG_PM_SLEEP_4E_L       (REG_PM_SLEEP_BASE + 0x9C)
#define REG_PM_SLEEP_4E_H       (REG_PM_SLEEP_BASE + 0x9D)
#define REG_PM_SLEEP_4F_L       (REG_PM_SLEEP_BASE + 0x9E)
#define REG_PM_SLEEP_4F_H       (REG_PM_SLEEP_BASE + 0x9F)
#define REG_PM_SLEEP_50_L       (REG_PM_SLEEP_BASE + 0xA0)
#define REG_PM_SLEEP_50_H       (REG_PM_SLEEP_BASE + 0xA1)
#define REG_PM_SLEEP_51_L       (REG_PM_SLEEP_BASE + 0xA2)
#define REG_PM_SLEEP_51_H       (REG_PM_SLEEP_BASE + 0xA3)
#define REG_PM_SLEEP_52_L       (REG_PM_SLEEP_BASE + 0xA4)
#define REG_PM_SLEEP_52_H       (REG_PM_SLEEP_BASE + 0xA5)
#define REG_PM_SLEEP_53_L       (REG_PM_SLEEP_BASE + 0xA6)
#define REG_PM_SLEEP_53_H       (REG_PM_SLEEP_BASE + 0xA7)
#define REG_PM_SLEEP_54_L       (REG_PM_SLEEP_BASE + 0xA8)
#define REG_PM_SLEEP_54_H       (REG_PM_SLEEP_BASE + 0xA9)
#define REG_PM_SLEEP_55_L       (REG_PM_SLEEP_BASE + 0xAA)
#define REG_PM_SLEEP_55_H       (REG_PM_SLEEP_BASE + 0xAB)
#define REG_PM_SLEEP_56_L       (REG_PM_SLEEP_BASE + 0xAC)
#define REG_PM_SLEEP_56_H       (REG_PM_SLEEP_BASE + 0xAD)
#define REG_PM_SLEEP_57_L       (REG_PM_SLEEP_BASE + 0xAE)
#define REG_PM_SLEEP_57_H       (REG_PM_SLEEP_BASE + 0xAF)
#define REG_PM_SLEEP_58_L       (REG_PM_SLEEP_BASE + 0xB0)
#define REG_PM_SLEEP_58_H       (REG_PM_SLEEP_BASE + 0xB1)
#define REG_PM_SLEEP_59_L       (REG_PM_SLEEP_BASE + 0xB2)
#define REG_PM_SLEEP_59_H       (REG_PM_SLEEP_BASE + 0xB3)
#define REG_PM_SLEEP_5A_L       (REG_PM_SLEEP_BASE + 0xB4)
#define REG_PM_SLEEP_5A_H       (REG_PM_SLEEP_BASE + 0xB5)
#define REG_PM_SLEEP_5B_L       (REG_PM_SLEEP_BASE + 0xB6)
#define REG_PM_SLEEP_5B_H       (REG_PM_SLEEP_BASE + 0xB7)
#define REG_PM_SLEEP_5C_L       (REG_PM_SLEEP_BASE + 0xB8)
#define REG_PM_SLEEP_5C_H       (REG_PM_SLEEP_BASE + 0xB9)
#define REG_PM_SLEEP_5D_L       (REG_PM_SLEEP_BASE + 0xBA)
#define REG_PM_SLEEP_5D_H       (REG_PM_SLEEP_BASE + 0xBB)
#define REG_PM_SLEEP_5E_L       (REG_PM_SLEEP_BASE + 0xBC)
#define REG_PM_SLEEP_5E_H       (REG_PM_SLEEP_BASE + 0xBD)
#define REG_PM_SLEEP_5F_L       (REG_PM_SLEEP_BASE + 0xBE)
#define REG_PM_SLEEP_5F_H       (REG_PM_SLEEP_BASE + 0xBF)
#define REG_PM_SLEEP_60_L       (REG_PM_SLEEP_BASE + 0xC0)
#define REG_PM_SLEEP_60_H       (REG_PM_SLEEP_BASE + 0xC1)
#define REG_PM_SLEEP_61_L       (REG_PM_SLEEP_BASE + 0xC2)
#define REG_PM_SLEEP_61_H       (REG_PM_SLEEP_BASE + 0xC3)
#define REG_PM_SLEEP_62_L       (REG_PM_SLEEP_BASE + 0xC4)
#define REG_PM_SLEEP_62_H       (REG_PM_SLEEP_BASE + 0xC5)
#define REG_PM_SLEEP_63_L       (REG_PM_SLEEP_BASE + 0xC6)
#define REG_PM_SLEEP_63_H       (REG_PM_SLEEP_BASE + 0xC7)
#define REG_PM_SLEEP_64_L       (REG_PM_SLEEP_BASE + 0xC8)
#define REG_PM_SLEEP_64_H       (REG_PM_SLEEP_BASE + 0xC9)
#define REG_PM_SLEEP_65_L       (REG_PM_SLEEP_BASE + 0xCA)
#define REG_PM_SLEEP_65_H       (REG_PM_SLEEP_BASE + 0xCB)
#define REG_PM_SLEEP_66_L       (REG_PM_SLEEP_BASE + 0xCC)
#define REG_PM_SLEEP_66_H       (REG_PM_SLEEP_BASE + 0xCD)
#define REG_PM_SLEEP_67_L       (REG_PM_SLEEP_BASE + 0xCE)
#define REG_PM_SLEEP_67_H       (REG_PM_SLEEP_BASE + 0xCF)
#define REG_PM_SLEEP_68_L       (REG_PM_SLEEP_BASE + 0xD0)
#define REG_PM_SLEEP_68_H       (REG_PM_SLEEP_BASE + 0xD1)
#define REG_PM_SLEEP_69_L       (REG_PM_SLEEP_BASE + 0xD2)
#define REG_PM_SLEEP_69_H       (REG_PM_SLEEP_BASE + 0xD3)
#define REG_PM_SLEEP_6A_L       (REG_PM_SLEEP_BASE + 0xD4)
#define REG_PM_SLEEP_6A_H       (REG_PM_SLEEP_BASE + 0xD5)
#define REG_PM_SLEEP_6B_L       (REG_PM_SLEEP_BASE + 0xD6)
#define REG_PM_SLEEP_6B_H       (REG_PM_SLEEP_BASE + 0xD7)
#define REG_PM_SLEEP_6C_L       (REG_PM_SLEEP_BASE + 0xD8)
#define REG_PM_SLEEP_6C_H       (REG_PM_SLEEP_BASE + 0xD9)
#define REG_PM_SLEEP_6D_L       (REG_PM_SLEEP_BASE + 0xDA)
#define REG_PM_SLEEP_6D_H       (REG_PM_SLEEP_BASE + 0xDB)
#define REG_PM_SLEEP_6E_L       (REG_PM_SLEEP_BASE + 0xDC)
#define REG_PM_SLEEP_6E_H       (REG_PM_SLEEP_BASE + 0xDD)
#define REG_PM_SLEEP_6F_L       (REG_PM_SLEEP_BASE + 0xDE)
#define REG_PM_SLEEP_6F_H       (REG_PM_SLEEP_BASE + 0xDF)
#define REG_PM_SLEEP_70_L       (REG_PM_SLEEP_BASE + 0xE0)
#define REG_PM_SLEEP_70_H       (REG_PM_SLEEP_BASE + 0xE1)
#define REG_PM_SLEEP_71_L       (REG_PM_SLEEP_BASE + 0xE2)
#define REG_PM_SLEEP_71_H       (REG_PM_SLEEP_BASE + 0xE3)
#define REG_PM_SLEEP_72_L       (REG_PM_SLEEP_BASE + 0xE4)
#define REG_PM_SLEEP_72_H       (REG_PM_SLEEP_BASE + 0xE5)
#define REG_PM_SLEEP_73_L       (REG_PM_SLEEP_BASE + 0xE6)
#define REG_PM_SLEEP_73_H       (REG_PM_SLEEP_BASE + 0xE7)
#define REG_PM_SLEEP_74_L       (REG_PM_SLEEP_BASE + 0xE8)
#define REG_PM_SLEEP_74_H       (REG_PM_SLEEP_BASE + 0xE9)
#define REG_PM_SLEEP_75_L       (REG_PM_SLEEP_BASE + 0xEA)
#define REG_PM_SLEEP_75_H       (REG_PM_SLEEP_BASE + 0xEB)
#define REG_PM_SLEEP_76_L       (REG_PM_SLEEP_BASE + 0xEC)
#define REG_PM_SLEEP_76_H       (REG_PM_SLEEP_BASE + 0xED)
#define REG_PM_SLEEP_77_L       (REG_PM_SLEEP_BASE + 0xEE)
#define REG_PM_SLEEP_77_H       (REG_PM_SLEEP_BASE + 0xEF)
#define REG_PM_SLEEP_78_L       (REG_PM_SLEEP_BASE + 0xF0)
#define REG_PM_SLEEP_78_H       (REG_PM_SLEEP_BASE + 0xF1)
#define REG_PM_SLEEP_79_L       (REG_PM_SLEEP_BASE + 0xF2)
#define REG_PM_SLEEP_79_H       (REG_PM_SLEEP_BASE + 0xF3)
#define REG_PM_SLEEP_7A_L       (REG_PM_SLEEP_BASE + 0xF4)
#define REG_PM_SLEEP_7A_H       (REG_PM_SLEEP_BASE + 0xF5)
#define REG_PM_SLEEP_7B_L       (REG_PM_SLEEP_BASE + 0xF6)
#define REG_PM_SLEEP_7B_H       (REG_PM_SLEEP_BASE + 0xF7)
#define REG_PM_SLEEP_7C_L       (REG_PM_SLEEP_BASE + 0xF8)
#define REG_PM_SLEEP_7C_H       (REG_PM_SLEEP_BASE + 0xF9)
#define REG_PM_SLEEP_7D_L       (REG_PM_SLEEP_BASE + 0xFA)
#define REG_PM_SLEEP_7D_H       (REG_PM_SLEEP_BASE + 0xFB)
#define REG_PM_SLEEP_7E_L       (REG_PM_SLEEP_BASE + 0xFC)
#define REG_PM_SLEEP_7E_H       (REG_PM_SLEEP_BASE + 0xFD)
#define REG_PM_SLEEP_7F_L       (REG_PM_SLEEP_BASE + 0xFE)
#define REG_PM_SLEEP_7F_H       (REG_PM_SLEEP_BASE + 0xFF)

//=============================================================
//PM TOP
//#define REG_PM_TOP_BASE             0x001E00UL
#define REG_PM_TOP_BANK_01_L    (REG_PM_TOP_BASE + 0x02)
#define REG_PM_TOP_BANK_01_H    (REG_PM_TOP_BASE + 0x03)

//=============================================================
//PM EFUSE
//#define REG_EFUSE_BASE              0x002000UL
#define REG_PM_EFUSE_28_L       (REG_EFUSE_BASE + 0x50)
#define REG_PM_EFUSE_28_H       (REG_EFUSE_BASE + 0x51)
#define REG_PM_EFUSE_2C_L       (REG_EFUSE_BASE + 0x58)
#define REG_PM_EFUSE_2C_H       (REG_EFUSE_BASE + 0x59)
#define REG_PM_EFUSE_2D_L       (REG_EFUSE_BASE + 0x5A)
#define REG_PM_EFUSE_2D_H       (REG_EFUSE_BASE + 0x5B)

//=============================================================
//PM MISC
//#define REG_PM_MISC_BASE           0x002E00UL
#define REG_PM_MISC_62_L        (REG_PM_MISC_BASE + 0xC4)


#endif

