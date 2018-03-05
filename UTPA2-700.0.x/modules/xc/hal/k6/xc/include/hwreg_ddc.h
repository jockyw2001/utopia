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

#ifndef _HWREG_DDC_H_
#define _HWREG_DDC_H_

//=============================================================
//DDC
//#define REG_DDC_BASE        0x0400

#define REG_DDC_00_L        (REG_DDC_BASE + 0x00)
#define REG_DDC_00_H        (REG_DDC_BASE + 0x01)
#define REG_DDC_01_L        (REG_DDC_BASE + 0x02)
#define REG_DDC_01_H        (REG_DDC_BASE + 0x03)
#define REG_DDC_02_L        (REG_DDC_BASE + 0x04)
#define REG_DDC_02_H        (REG_DDC_BASE + 0x05)
#define REG_DDC_03_L        (REG_DDC_BASE + 0x06)
#define REG_DDC_03_H        (REG_DDC_BASE + 0x07)
#define REG_DDC_04_L        (REG_DDC_BASE + 0x08)
#define REG_DDC_04_H        (REG_DDC_BASE + 0x09)
#define REG_DDC_05_L        (REG_DDC_BASE + 0x0A)
#define REG_DDC_05_H        (REG_DDC_BASE + 0x0B)
#define REG_DDC_06_L        (REG_DDC_BASE + 0x0C)
#define REG_DDC_06_H        (REG_DDC_BASE + 0x0D)
#define REG_DDC_07_L        (REG_DDC_BASE + 0x0E)
#define REG_DDC_07_H        (REG_DDC_BASE + 0x0F)
#define REG_DDC_08_L        (REG_DDC_BASE + 0x10)
#define REG_DDC_08_H        (REG_DDC_BASE + 0x11)
#define REG_DDC_09_L        (REG_DDC_BASE + 0x12)
#define REG_DDC_09_H        (REG_DDC_BASE + 0x13)
#define REG_DDC_0A_L        (REG_DDC_BASE + 0x14)
#define REG_DDC_0A_H        (REG_DDC_BASE + 0x15)
#define REG_DDC_0B_L        (REG_DDC_BASE + 0x16)
#define REG_DDC_0B_H        (REG_DDC_BASE + 0x17)
#define REG_DDC_0C_L        (REG_DDC_BASE + 0x18)
#define REG_DDC_0C_H        (REG_DDC_BASE + 0x19)
#define REG_DDC_0D_L        (REG_DDC_BASE + 0x1A)
#define REG_DDC_0D_H        (REG_DDC_BASE + 0x1B)
#define REG_DDC_0E_L        (REG_DDC_BASE + 0x1C)
#define REG_DDC_0E_H        (REG_DDC_BASE + 0x1D)
#define REG_DDC_0F_L        (REG_DDC_BASE + 0x1E)
#define REG_DDC_0F_H        (REG_DDC_BASE + 0x1F)
#define REG_DDC_10_L        (REG_DDC_BASE + 0x20)
#define REG_DDC_10_H        (REG_DDC_BASE + 0x21)
#define REG_DDC_11_L        (REG_DDC_BASE + 0x22)
#define REG_DDC_11_H        (REG_DDC_BASE + 0x23)
#define REG_DDC_12_L        (REG_DDC_BASE + 0x24)
#define REG_DDC_12_H        (REG_DDC_BASE + 0x25)
#define REG_DDC_13_L        (REG_DDC_BASE + 0x26)
#define REG_DDC_13_H        (REG_DDC_BASE + 0x27)
#define REG_DDC_14_L        (REG_DDC_BASE + 0x28)
#define REG_DDC_14_H        (REG_DDC_BASE + 0x29)
#define REG_DDC_15_L        (REG_DDC_BASE + 0x2A)
#define REG_DDC_15_H        (REG_DDC_BASE + 0x2B)
#define REG_DDC_16_L        (REG_DDC_BASE + 0x2C)
#define REG_DDC_16_H        (REG_DDC_BASE + 0x2D)
#define REG_DDC_17_L        (REG_DDC_BASE + 0x2E)
#define REG_DDC_17_H        (REG_DDC_BASE + 0x2F)
#define REG_DDC_18_L        (REG_DDC_BASE + 0x30)
#define REG_DDC_18_H        (REG_DDC_BASE + 0x31)
#define REG_DDC_19_L        (REG_DDC_BASE + 0x32)
#define REG_DDC_19_H        (REG_DDC_BASE + 0x33)
#define REG_DDC_1A_L        (REG_DDC_BASE + 0x34)
#define REG_DDC_1A_H        (REG_DDC_BASE + 0x35)
#define REG_DDC_1B_L        (REG_DDC_BASE + 0x36)
#define REG_DDC_1B_H        (REG_DDC_BASE + 0x37)
#define REG_DDC_1C_L        (REG_DDC_BASE + 0x38)
#define REG_DDC_1C_H        (REG_DDC_BASE + 0x39)
#define REG_DDC_1D_L        (REG_DDC_BASE + 0x3A)
#define REG_DDC_1D_H        (REG_DDC_BASE + 0x3B)
#define REG_DDC_1E_L        (REG_DDC_BASE + 0x3C)
#define REG_DDC_1E_H        (REG_DDC_BASE + 0x3D)
#define REG_DDC_1F_L        (REG_DDC_BASE + 0x3E)
#define REG_DDC_1F_H        (REG_DDC_BASE + 0x3F)
#define REG_DDC_20_L        (REG_DDC_BASE + 0x40)
#define REG_DDC_20_H        (REG_DDC_BASE + 0x41)
#define REG_DDC_21_L        (REG_DDC_BASE + 0x42)
#define REG_DDC_21_H        (REG_DDC_BASE + 0x43)
#define REG_DDC_22_L        (REG_DDC_BASE + 0x44)
#define REG_DDC_22_H        (REG_DDC_BASE + 0x45)
#define REG_DDC_23_L        (REG_DDC_BASE + 0x46)
#define REG_DDC_23_H        (REG_DDC_BASE + 0x47)
#define REG_DDC_24_L        (REG_DDC_BASE + 0x48)
#define REG_DDC_24_H        (REG_DDC_BASE + 0x49)
#define REG_DDC_25_L        (REG_DDC_BASE + 0x4A)
#define REG_DDC_25_H        (REG_DDC_BASE + 0x4B)
#define REG_DDC_26_L        (REG_DDC_BASE + 0x4C)
#define REG_DDC_26_H        (REG_DDC_BASE + 0x4D)
#define REG_DDC_27_L        (REG_DDC_BASE + 0x4E)
#define REG_DDC_27_H        (REG_DDC_BASE + 0x4F)
#define REG_DDC_28_L        (REG_DDC_BASE + 0x50)
#define REG_DDC_28_H        (REG_DDC_BASE + 0x51)
#define REG_DDC_29_L        (REG_DDC_BASE + 0x52)
#define REG_DDC_29_H        (REG_DDC_BASE + 0x53)
#define REG_DDC_2A_L        (REG_DDC_BASE + 0x54)
#define REG_DDC_2A_H        (REG_DDC_BASE + 0x55)
#define REG_DDC_2B_L        (REG_DDC_BASE + 0x56)
#define REG_DDC_2B_H        (REG_DDC_BASE + 0x57)
#define REG_DDC_2C_L        (REG_DDC_BASE + 0x58)
#define REG_DDC_2C_H        (REG_DDC_BASE + 0x59)
#define REG_DDC_2D_L        (REG_DDC_BASE + 0x5A)
#define REG_DDC_2D_H        (REG_DDC_BASE + 0x5B)
#define REG_DDC_2E_L        (REG_DDC_BASE + 0x5C)
#define REG_DDC_2E_H        (REG_DDC_BASE + 0x5D)
#define REG_DDC_2F_L        (REG_DDC_BASE + 0x5E)
#define REG_DDC_2F_H        (REG_DDC_BASE + 0x5F)
#define REG_DDC_30_L        (REG_DDC_BASE + 0x60)
#define REG_DDC_30_H        (REG_DDC_BASE + 0x61)
#define REG_DDC_31_L        (REG_DDC_BASE + 0x62)
#define REG_DDC_31_H        (REG_DDC_BASE + 0x63)
#define REG_DDC_32_L        (REG_DDC_BASE + 0x64)
#define REG_DDC_32_H        (REG_DDC_BASE + 0x65)
#define REG_DDC_33_L        (REG_DDC_BASE + 0x66)
#define REG_DDC_33_H        (REG_DDC_BASE + 0x67)
#define REG_DDC_34_L        (REG_DDC_BASE + 0x68)
#define REG_DDC_34_H        (REG_DDC_BASE + 0x69)
#define REG_DDC_35_L        (REG_DDC_BASE + 0x6A)
#define REG_DDC_35_H        (REG_DDC_BASE + 0x6B)
#define REG_DDC_36_L        (REG_DDC_BASE + 0x6C)
#define REG_DDC_36_H        (REG_DDC_BASE + 0x6D)
#define REG_DDC_37_L        (REG_DDC_BASE + 0x6E)
#define REG_DDC_37_H        (REG_DDC_BASE + 0x6F)
#define REG_DDC_38_L        (REG_DDC_BASE + 0x70)
#define REG_DDC_38_H        (REG_DDC_BASE + 0x71)
#define REG_DDC_39_L        (REG_DDC_BASE + 0x72)
#define REG_DDC_39_H        (REG_DDC_BASE + 0x73)
#define REG_DDC_3A_L        (REG_DDC_BASE + 0x74)
#define REG_DDC_3A_H        (REG_DDC_BASE + 0x75)
#define REG_DDC_3B_L        (REG_DDC_BASE + 0x76)
#define REG_DDC_3B_H        (REG_DDC_BASE + 0x77)
#define REG_DDC_3C_L        (REG_DDC_BASE + 0x78)
#define REG_DDC_3C_H        (REG_DDC_BASE + 0x79)
#define REG_DDC_3D_L        (REG_DDC_BASE + 0x7A)
#define REG_DDC_3D_H        (REG_DDC_BASE + 0x7B)
#define REG_DDC_3E_L        (REG_DDC_BASE + 0x7C)
#define REG_DDC_3E_H        (REG_DDC_BASE + 0x7D)
#define REG_DDC_3F_L        (REG_DDC_BASE + 0x7E)
#define REG_DDC_3F_H        (REG_DDC_BASE + 0x7F)
#define REG_DDC_40_L        (REG_DDC_BASE + 0x80)
#define REG_DDC_40_H        (REG_DDC_BASE + 0x81)
#define REG_DDC_41_L        (REG_DDC_BASE + 0x82)
#define REG_DDC_41_H        (REG_DDC_BASE + 0x83)
#define REG_DDC_42_L        (REG_DDC_BASE + 0x84)
#define REG_DDC_42_H        (REG_DDC_BASE + 0x85)
#define REG_DDC_43_L        (REG_DDC_BASE + 0x86)
#define REG_DDC_43_H        (REG_DDC_BASE + 0x87)
#define REG_DDC_44_L        (REG_DDC_BASE + 0x88)
#define REG_DDC_44_H        (REG_DDC_BASE + 0x89)
#define REG_DDC_45_L        (REG_DDC_BASE + 0x8A)
#define REG_DDC_45_H        (REG_DDC_BASE + 0x8B)
#define REG_DDC_46_L        (REG_DDC_BASE + 0x8C)
#define REG_DDC_46_H        (REG_DDC_BASE + 0x8D)
#define REG_DDC_47_L        (REG_DDC_BASE + 0x8E)
#define REG_DDC_47_H        (REG_DDC_BASE + 0x8F)
#define REG_DDC_48_L        (REG_DDC_BASE + 0x90)
#define REG_DDC_48_H        (REG_DDC_BASE + 0x91)
#define REG_DDC_49_L        (REG_DDC_BASE + 0x92)
#define REG_DDC_49_H        (REG_DDC_BASE + 0x93)
#define REG_DDC_4A_L        (REG_DDC_BASE + 0x94)
#define REG_DDC_4A_H        (REG_DDC_BASE + 0x95)
#define REG_DDC_4B_L        (REG_DDC_BASE + 0x96)
#define REG_DDC_4B_H        (REG_DDC_BASE + 0x97)
#define REG_DDC_4C_L        (REG_DDC_BASE + 0x98)
#define REG_DDC_4C_H        (REG_DDC_BASE + 0x99)
#define REG_DDC_4D_L        (REG_DDC_BASE + 0x9A)
#define REG_DDC_4D_H        (REG_DDC_BASE + 0x9B)
#define REG_DDC_4E_L        (REG_DDC_BASE + 0x9C)
#define REG_DDC_4E_H        (REG_DDC_BASE + 0x9D)
#define REG_DDC_4F_L        (REG_DDC_BASE + 0x9E)
#define REG_DDC_4F_H        (REG_DDC_BASE + 0x9F)
#define REG_DDC_50_L        (REG_DDC_BASE + 0xA0)
#define REG_DDC_50_H        (REG_DDC_BASE + 0xA1)
#define REG_DDC_51_L        (REG_DDC_BASE + 0xA2)
#define REG_DDC_51_H        (REG_DDC_BASE + 0xA3)
#define REG_DDC_52_L        (REG_DDC_BASE + 0xA4)
#define REG_DDC_52_H        (REG_DDC_BASE + 0xA5)
#define REG_DDC_53_L        (REG_DDC_BASE + 0xA6)
#define REG_DDC_53_H        (REG_DDC_BASE + 0xA7)
#define REG_DDC_54_L        (REG_DDC_BASE + 0xA8)
#define REG_DDC_54_H        (REG_DDC_BASE + 0xA9)
#define REG_DDC_55_L        (REG_DDC_BASE + 0xAA)
#define REG_DDC_55_H        (REG_DDC_BASE + 0xAB)
#define REG_DDC_56_L        (REG_DDC_BASE + 0xAC)
#define REG_DDC_56_H        (REG_DDC_BASE + 0xAD)
#define REG_DDC_57_L        (REG_DDC_BASE + 0xAE)
#define REG_DDC_57_H        (REG_DDC_BASE + 0xAF)
#define REG_DDC_58_L        (REG_DDC_BASE + 0xB0)
#define REG_DDC_58_H        (REG_DDC_BASE + 0xB1)
#define REG_DDC_59_L        (REG_DDC_BASE + 0xB2)
#define REG_DDC_59_H        (REG_DDC_BASE + 0xB3)
#define REG_DDC_5A_L        (REG_DDC_BASE + 0xB4)
#define REG_DDC_5A_H        (REG_DDC_BASE + 0xB5)
#define REG_DDC_5B_L        (REG_DDC_BASE + 0xB6)
#define REG_DDC_5B_H        (REG_DDC_BASE + 0xB7)
#define REG_DDC_5C_L        (REG_DDC_BASE + 0xB8)
#define REG_DDC_5C_H        (REG_DDC_BASE + 0xB9)
#define REG_DDC_5D_L        (REG_DDC_BASE + 0xBA)
#define REG_DDC_5D_H        (REG_DDC_BASE + 0xBB)
#define REG_DDC_5E_L        (REG_DDC_BASE + 0xBC)
#define REG_DDC_5E_H        (REG_DDC_BASE + 0xBD)
#define REG_DDC_5F_L        (REG_DDC_BASE + 0xBE)
#define REG_DDC_5F_H        (REG_DDC_BASE + 0xBF)
#define REG_DDC_60_L        (REG_DDC_BASE + 0xC0)
#define REG_DDC_60_H        (REG_DDC_BASE + 0xC1)
#define REG_DDC_61_L        (REG_DDC_BASE + 0xC2)
#define REG_DDC_61_H        (REG_DDC_BASE + 0xC3)
#define REG_DDC_62_L        (REG_DDC_BASE + 0xC4)
#define REG_DDC_62_H        (REG_DDC_BASE + 0xC5)
#define REG_DDC_63_L        (REG_DDC_BASE + 0xC6)
#define REG_DDC_63_H        (REG_DDC_BASE + 0xC7)
#define REG_DDC_64_L        (REG_DDC_BASE + 0xC8)
#define REG_DDC_64_H        (REG_DDC_BASE + 0xC9)
#define REG_DDC_65_L        (REG_DDC_BASE + 0xCA)
#define REG_DDC_65_H        (REG_DDC_BASE + 0xCB)
#define REG_DDC_66_L        (REG_DDC_BASE + 0xCC)
#define REG_DDC_66_H        (REG_DDC_BASE + 0xCD)
#define REG_DDC_67_L        (REG_DDC_BASE + 0xCE)
#define REG_DDC_67_H        (REG_DDC_BASE + 0xCF)
#define REG_DDC_68_L        (REG_DDC_BASE + 0xD0)
#define REG_DDC_68_H        (REG_DDC_BASE + 0xD1)
#define REG_DDC_69_L        (REG_DDC_BASE + 0xD2)
#define REG_DDC_69_H        (REG_DDC_BASE + 0xD3)
#define REG_DDC_6A_L        (REG_DDC_BASE + 0xD4)
#define REG_DDC_6A_H        (REG_DDC_BASE + 0xD5)
#define REG_DDC_6B_L        (REG_DDC_BASE + 0xD6)
#define REG_DDC_6B_H        (REG_DDC_BASE + 0xD7)
#define REG_DDC_6C_L        (REG_DDC_BASE + 0xD8)
#define REG_DDC_6C_H        (REG_DDC_BASE + 0xD9)
#define REG_DDC_6D_L        (REG_DDC_BASE + 0xDA)
#define REG_DDC_6D_H        (REG_DDC_BASE + 0xDB)
#define REG_DDC_6E_L        (REG_DDC_BASE + 0xDC)
#define REG_DDC_6E_H        (REG_DDC_BASE + 0xDD)
#define REG_DDC_6F_L        (REG_DDC_BASE + 0xDE)
#define REG_DDC_6F_H        (REG_DDC_BASE + 0xDF)
#define REG_DDC_70_L        (REG_DDC_BASE + 0xE0)
#define REG_DDC_70_H        (REG_DDC_BASE + 0xE1)
#define REG_DDC_71_L        (REG_DDC_BASE + 0xE2)
#define REG_DDC_71_H        (REG_DDC_BASE + 0xE3)
#define REG_DDC_72_L        (REG_DDC_BASE + 0xE4)
#define REG_DDC_72_H        (REG_DDC_BASE + 0xE5)
#define REG_DDC_73_L        (REG_DDC_BASE + 0xE6)
#define REG_DDC_73_H        (REG_DDC_BASE + 0xE7)
#define REG_DDC_74_L        (REG_DDC_BASE + 0xE8)
#define REG_DDC_74_H        (REG_DDC_BASE + 0xE9)
#define REG_DDC_75_L        (REG_DDC_BASE + 0xEA)
#define REG_DDC_75_H        (REG_DDC_BASE + 0xEB)
#define REG_DDC_76_L        (REG_DDC_BASE + 0xEC)
#define REG_DDC_76_H        (REG_DDC_BASE + 0xED)
#define REG_DDC_77_L        (REG_DDC_BASE + 0xEE)
#define REG_DDC_77_H        (REG_DDC_BASE + 0xEF)
#define REG_DDC_78_L        (REG_DDC_BASE + 0xF0)
#define REG_DDC_78_H        (REG_DDC_BASE + 0xF1)
#define REG_DDC_79_L        (REG_DDC_BASE + 0xF2)
#define REG_DDC_79_H        (REG_DDC_BASE + 0xF3)
#define REG_DDC_7A_L        (REG_DDC_BASE + 0xF4)
#define REG_DDC_7A_H        (REG_DDC_BASE + 0xF5)
#define REG_DDC_7B_L        (REG_DDC_BASE + 0xF6)
#define REG_DDC_7B_H        (REG_DDC_BASE + 0xF7)
#define REG_DDC_7C_L        (REG_DDC_BASE + 0xF8)
#define REG_DDC_7C_H        (REG_DDC_BASE + 0xF9)
#define REG_DDC_7D_L        (REG_DDC_BASE + 0xFA)
#define REG_DDC_7D_H        (REG_DDC_BASE + 0xFB)
#define REG_DDC_7E_L        (REG_DDC_BASE + 0xFC)
#define REG_DDC_7E_H        (REG_DDC_BASE + 0xFD)
#define REG_DDC_7F_L        (REG_DDC_BASE + 0xFE)
#define REG_DDC_7F_H        (REG_DDC_BASE + 0xFF)

//------------------------------------------------------------------------------
// DDC
//------------------------------------------------------------------------------
#define REG_DDC_SRAM_SEL                 REG_DDC_22_L
    #define REG_DDC_SRAM_SEL_MASK        BMASK(12:11)

#define REG_DDC_A_ACCESS                 REG_DDC_21_L
    #define REG_DDC_A_ACCESS_BITS(x)     BITS(9:9,x)
    #define REG_DDC_A_ACCESS_MASK        BMASK(9:9)
#define REG_DDC_A_WRITEADDR              REG_DDC_23_L
    #define REG_DDC_A_WRITEADDR_BITS(x)  BITS(15:8,x)
    #define REG_DDC_A_WRITEADDR_MASK     BMASK(15:8)
#define REG_DDC_A_WRITEDATA              REG_DDC_24_L
    #define REG_DDC_A_WRITEDATA_BITS(x)  BITS(7:0,x)
    #define REG_DDC_A_WRITEDATA_MASK     BMASK(7:0)
#define REG_DDC_A_WRITEPULSE             REG_DDC_21_L
    #define REG_DDC_A_WRITEPULSE_BITS(x) BITS(15:15,x)
    #define REG_DDC_A_WRITEPULSE_MASK    BMASK(15:15)
#define REG_DDC_A_READDATA               REG_DDC_07_L
    #define REG_DDC_A_READDATA_BITS(x)   BITS(15:8,x)
    #define REG_DDC_A_READDATA_MASK      BMASK(15:8)
#define REG_DDC_A_READPULSE              REG_DDC_21_L
    #define REG_DDC_A_READPULSE_BITS(x)  BITS(14:14,x)
    #define REG_DDC_A_READPULSE_MASK     BMASK(14:14)
#define REG_DDC_A_SRAM_BASEADDR          REG_DDC_75_L
    #define REG_DDC_A_BASEADDR_BITS(x)  BITS(12:8,x)
    #define REG_DDC_A_BASEADDR_MASK     BMASK(12:8)

#define REG_DDC_D_ACCESS                 REG_DDC_21_L
    #define REG_DDC_D_ACCESS_BITS(x)     BITS(9:9,x)
    #define REG_DDC_D_ACCESS_MASK        BMASK(9:9)
#define REG_DDC_D_WRITEADDR              REG_DDC_23_L
    #define REG_DDC_D_WRITEADDR_BITS(x)  BITS(15:8,x)
    #define REG_DDC_D_WRITEADDR_MASK     BMASK(15:8)
#define REG_DDC_D_WRITEDATA              REG_DDC_24_L
    #define REG_DDC_D_WRITEDATA_BITS(x)  BITS(7:0,x)
    #define REG_DDC_D_WRITEDATA_MASK     BMASK(7:0)
#define REG_DDC_D_WRITEPULSE             REG_DDC_21_L
    #define REG_DDC_D_WRITEPULSE_BITS(x) BITS(15:15,x)
    #define REG_DDC_D_WRITEPULSE_MASK    BMASK(15:15)
#define REG_DDC_D_READDATA               REG_DDC_07_L
    #define REG_DDC_D_READDATA_BITS(x)   BITS(15:8,x)
    #define REG_DDC_D_READDATA_MASK      BMASK(15:8)
#define REG_DDC_D_READPULSE              REG_DDC_21_L
    #define REG_DDC_D_READPULSE_BITS(x)  BITS(14:14,x)
    #define REG_DDC_D_READPULSE_MASK     BMASK(14:14)   
#define REG_DDC_D0_SRAM_BASEADDR         REG_DDC_76_L
    #define REG_DDC_D0_BASEADDR_BITS(x)  BITS(4:0,x)
    #define REG_DDC_D0_BASEADDR_MASK     BMASK(4:0)
#define REG_DDC_D1_SRAM_BASEADDR         REG_DDC_76_L
    #define REG_DDC_D1_BASEADDR_BITS(x)  BITS(12:8,x)
    #define REG_DDC_D1_BASEADDR_MASK     BMASK(12:8)
#define REG_DDC_D2_SRAM_BASEADDR         REG_DDC_77_L
    #define REG_DDC_D2_BASEADDR_BITS(x)  BITS(4:0,x)
    #define REG_DDC_D2_BASEADDR_MASK     BMASK(4:0)
#define REG_DDC_D3_SRAM_BASEADDR         REG_DDC_77_L
    #define REG_DDC_D3_BASEADDR_BITS(x)  BITS(12:8,x)
    #define REG_DDC_D3_BASEADDR_MASK     BMASK(12:8)

#define REG_DDC_DVI0_EN         (REG_DDC_22_L)//H
    #define REG_DDC_DVI0_EN_MASK        15
#define REG_DDC_DVI1_EN         (REG_DDC_26_L)//H
    #define REG_DDC_DVI1_EN_MASK        15
#define REG_DDC_DVI2_EN         (REG_DDC_2C_L)//L
    #define REG_DDC_DVI2_EN_MASK        7
#define REG_DDC_DVI3_EN         (REG_DDC_54_L)//L
    #define REG_DDC_DVI3_EN_MASK        7
#define REG_DDC_ADC0_EN         (REG_DDC_24_L)//H
    #define REG_DDC_ADC0_EN_MASK        15

#define REG_DDC_DVI0_WP         (REG_DDC_22_L)//H
    #define REG_DDC_DVI0_WP_MASK        13
#define REG_DDC_DVI1_WP         (REG_DDC_26_L)//H
    #define REG_DDC_DVI1_WP_MASK        13
#define REG_DDC_DVI2_WP        (REG_DDC_2C_L)//L
    #define REG_DDC_DVI2_WP_MASK        5
#define REG_DDC_DVI3_WP         (REG_DDC_54_L)//L
    #define REG_DDC_DVI3_WP_MASK        5
#define REG_DDC_ADC0_WP        (REG_DDC_24_L)//H
    #define REG_DDC_ADC0_WP_MASK        13

#endif

