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

#ifndef _HWREG_ADC_ATOP_H_
#define _HWREG_ADC_ATOP_H_

//=============================================================
//ADC ATOP
//#define REG_ADC_ATOP_BASE       0x2500
#ifndef REG_TABLE_END
#define REG_TABLE_END            0xFFFF
#endif

#define REG_ADC_ATOP_00_L        (REG_ADC_ATOP_BASE + 0x00)
#define REG_ADC_ATOP_00_H        (REG_ADC_ATOP_BASE + 0x01)
#define REG_ADC_ATOP_01_L        (REG_ADC_ATOP_BASE + 0x02)
#define REG_ADC_ATOP_01_H        (REG_ADC_ATOP_BASE + 0x03)
#define REG_ADC_ATOP_02_L        (REG_ADC_ATOP_BASE + 0x04)
#define REG_ADC_ATOP_02_H        (REG_ADC_ATOP_BASE + 0x05)
#define REG_ADC_ATOP_03_L        (REG_ADC_ATOP_BASE + 0x06)
#define REG_ADC_ATOP_03_H        (REG_ADC_ATOP_BASE + 0x07)
#define REG_ADC_ATOP_04_L        (REG_ADC_ATOP_BASE + 0x08)
#define REG_ADC_ATOP_04_H        (REG_ADC_ATOP_BASE + 0x09)
#define REG_ADC_ATOP_05_L        (REG_ADC_ATOP_BASE + 0x0A)
#define REG_ADC_ATOP_05_H        (REG_ADC_ATOP_BASE + 0x0B)
#define REG_ADC_ATOP_06_L        (REG_ADC_ATOP_BASE + 0x0C)
#define REG_ADC_ATOP_06_H        (REG_ADC_ATOP_BASE + 0x0D)
#define REG_ADC_ATOP_07_L        (REG_ADC_ATOP_BASE + 0x0E)
#define REG_ADC_ATOP_07_H        (REG_ADC_ATOP_BASE + 0x0F)
#define REG_ADC_ATOP_08_L        (REG_ADC_ATOP_BASE + 0x10)
#define REG_ADC_ATOP_08_H        (REG_ADC_ATOP_BASE + 0x11)
#define REG_ADC_ATOP_09_L        (REG_ADC_ATOP_BASE + 0x12)
#define REG_ADC_ATOP_09_H        (REG_ADC_ATOP_BASE + 0x13)
#define REG_ADC_ATOP_0A_L        (REG_ADC_ATOP_BASE + 0x14)
#define REG_ADC_ATOP_0A_H        (REG_ADC_ATOP_BASE + 0x15)
#define REG_ADC_ATOP_0B_L        (REG_ADC_ATOP_BASE + 0x16)
#define REG_ADC_ATOP_0B_H        (REG_ADC_ATOP_BASE + 0x17)
#define REG_ADC_ATOP_0C_L       (REG_ADC_ATOP_BASE + 0x18)
#define REG_ADC_ATOP_0C_H       (REG_ADC_ATOP_BASE + 0x19)
#define REG_ADC_ATOP_0D_L       (REG_ADC_ATOP_BASE + 0x1A)
#define REG_ADC_ATOP_0D_H       (REG_ADC_ATOP_BASE + 0x1B)
#define REG_ADC_ATOP_0E_L       (REG_ADC_ATOP_BASE + 0x1C)
#define REG_ADC_ATOP_0E_H       (REG_ADC_ATOP_BASE + 0x1D)
#define REG_ADC_ATOP_0F_L       (REG_ADC_ATOP_BASE + 0x1E)
#define REG_ADC_ATOP_0F_H       (REG_ADC_ATOP_BASE + 0x1F)
#define REG_ADC_ATOP_10_L       (REG_ADC_ATOP_BASE + 0x20)
#define REG_ADC_ATOP_10_H       (REG_ADC_ATOP_BASE + 0x21)
#define REG_ADC_ATOP_11_L       (REG_ADC_ATOP_BASE + 0x22)
#define REG_ADC_ATOP_11_H       (REG_ADC_ATOP_BASE + 0x23)
#define REG_ADC_ATOP_12_L       (REG_ADC_ATOP_BASE + 0x24)
#define REG_ADC_ATOP_12_H       (REG_ADC_ATOP_BASE + 0x25)
#define REG_ADC_ATOP_13_L       (REG_ADC_ATOP_BASE + 0x26)
#define REG_ADC_ATOP_13_H       (REG_ADC_ATOP_BASE + 0x27)
#define REG_ADC_ATOP_14_L       (REG_ADC_ATOP_BASE + 0x28)
#define REG_ADC_ATOP_14_H       (REG_ADC_ATOP_BASE + 0x29)
#define REG_ADC_ATOP_15_L       (REG_ADC_ATOP_BASE + 0x2A)
#define REG_ADC_ATOP_15_H       (REG_ADC_ATOP_BASE + 0x2B)
#define REG_ADC_ATOP_16_L       (REG_ADC_ATOP_BASE + 0x2C)
#define REG_ADC_ATOP_16_H       (REG_ADC_ATOP_BASE + 0x2D)
#define REG_ADC_ATOP_17_L       (REG_ADC_ATOP_BASE + 0x2E)
#define REG_ADC_ATOP_17_H       (REG_ADC_ATOP_BASE + 0x2F)
#define REG_ADC_ATOP_18_L       (REG_ADC_ATOP_BASE + 0x30)
#define REG_ADC_ATOP_18_H       (REG_ADC_ATOP_BASE + 0x31)
#define REG_ADC_ATOP_19_L       (REG_ADC_ATOP_BASE + 0x32)
#define REG_ADC_ATOP_19_H       (REG_ADC_ATOP_BASE + 0x33)
#define REG_ADC_ATOP_1A_L       (REG_ADC_ATOP_BASE + 0x34)
#define REG_ADC_ATOP_1A_H       (REG_ADC_ATOP_BASE + 0x35)
#define REG_ADC_ATOP_1B_L       (REG_ADC_ATOP_BASE + 0x36)
#define REG_ADC_ATOP_1B_H       (REG_ADC_ATOP_BASE + 0x37)
#define REG_ADC_ATOP_1C_L       (REG_ADC_ATOP_BASE + 0x38)
#define REG_ADC_ATOP_1C_H       (REG_ADC_ATOP_BASE + 0x39)
#define REG_ADC_ATOP_1D_L       (REG_ADC_ATOP_BASE + 0x3A)
#define REG_ADC_ATOP_1D_H       (REG_ADC_ATOP_BASE + 0x3B)
#define REG_ADC_ATOP_1E_L       (REG_ADC_ATOP_BASE + 0x3C)
#define REG_ADC_ATOP_1E_H       (REG_ADC_ATOP_BASE + 0x3D)
#define REG_ADC_ATOP_1F_L       (REG_ADC_ATOP_BASE + 0x3E)
#define REG_ADC_ATOP_1F_H       (REG_ADC_ATOP_BASE + 0x3F)
#define REG_ADC_ATOP_20_L       (REG_ADC_ATOP_BASE + 0x40)
#define REG_ADC_ATOP_20_H       (REG_ADC_ATOP_BASE + 0x41)
#define REG_ADC_ATOP_21_L       (REG_ADC_ATOP_BASE + 0x42)
#define REG_ADC_ATOP_21_H       (REG_ADC_ATOP_BASE + 0x43)
#define REG_ADC_ATOP_22_L       (REG_ADC_ATOP_BASE + 0x44)
#define REG_ADC_ATOP_22_H       (REG_ADC_ATOP_BASE + 0x45)
#define REG_ADC_ATOP_23_L       (REG_ADC_ATOP_BASE + 0x46)
#define REG_ADC_ATOP_23_H       (REG_ADC_ATOP_BASE + 0x47)
#define REG_ADC_ATOP_24_L       (REG_ADC_ATOP_BASE + 0x48)
#define REG_ADC_ATOP_24_H       (REG_ADC_ATOP_BASE + 0x49)
#define REG_ADC_ATOP_25_L       (REG_ADC_ATOP_BASE + 0x4A)
#define REG_ADC_ATOP_25_H       (REG_ADC_ATOP_BASE + 0x4B)
#define REG_ADC_ATOP_26_L       (REG_ADC_ATOP_BASE + 0x4C)
#define REG_ADC_ATOP_26_H       (REG_ADC_ATOP_BASE + 0x4D)
#define REG_ADC_ATOP_27_L       (REG_ADC_ATOP_BASE + 0x4E)
#define REG_ADC_ATOP_27_H       (REG_ADC_ATOP_BASE + 0x4F)
#define REG_ADC_ATOP_28_L       (REG_ADC_ATOP_BASE + 0x50)
#define REG_ADC_ATOP_28_H       (REG_ADC_ATOP_BASE + 0x51)
#define REG_ADC_ATOP_29_L       (REG_ADC_ATOP_BASE + 0x52)
#define REG_ADC_ATOP_29_H       (REG_ADC_ATOP_BASE + 0x53)
#define REG_ADC_ATOP_2A_L       (REG_ADC_ATOP_BASE + 0x54)
#define REG_ADC_ATOP_2A_H       (REG_ADC_ATOP_BASE + 0x55)
#define REG_ADC_ATOP_2B_L       (REG_ADC_ATOP_BASE + 0x56)
#define REG_ADC_ATOP_2B_H       (REG_ADC_ATOP_BASE + 0x57)
#define REG_ADC_ATOP_2C_L       (REG_ADC_ATOP_BASE + 0x58)
#define REG_ADC_ATOP_2C_H       (REG_ADC_ATOP_BASE + 0x59)
#define REG_ADC_ATOP_2D_L       (REG_ADC_ATOP_BASE + 0x5A)
#define REG_ADC_ATOP_2D_H       (REG_ADC_ATOP_BASE + 0x5B)
#define REG_ADC_ATOP_2E_L       (REG_ADC_ATOP_BASE + 0x5C)
#define REG_ADC_ATOP_2E_H       (REG_ADC_ATOP_BASE + 0x5D)
#define REG_ADC_ATOP_2F_L       (REG_ADC_ATOP_BASE + 0x5E)
#define REG_ADC_ATOP_2F_H       (REG_ADC_ATOP_BASE + 0x5F)
#define REG_ADC_ATOP_30_L       (REG_ADC_ATOP_BASE + 0x60)
#define REG_ADC_ATOP_30_H       (REG_ADC_ATOP_BASE + 0x61)
#define REG_ADC_ATOP_31_L       (REG_ADC_ATOP_BASE + 0x62)
#define REG_ADC_ATOP_31_H       (REG_ADC_ATOP_BASE + 0x63)
#define REG_ADC_ATOP_32_L       (REG_ADC_ATOP_BASE + 0x64)
#define REG_ADC_ATOP_32_H       (REG_ADC_ATOP_BASE + 0x65)
#define REG_ADC_ATOP_33_L       (REG_ADC_ATOP_BASE + 0x66)
#define REG_ADC_ATOP_33_H       (REG_ADC_ATOP_BASE + 0x67)
#define REG_ADC_ATOP_34_L       (REG_ADC_ATOP_BASE + 0x68)
#define REG_ADC_ATOP_34_H       (REG_ADC_ATOP_BASE + 0x69)
#define REG_ADC_ATOP_35_L       (REG_ADC_ATOP_BASE + 0x6A)
#define REG_ADC_ATOP_35_H       (REG_ADC_ATOP_BASE + 0x6B)
#define REG_ADC_ATOP_36_L       (REG_ADC_ATOP_BASE + 0x6C)
#define REG_ADC_ATOP_36_H       (REG_ADC_ATOP_BASE + 0x6D)
#define REG_ADC_ATOP_37_L       (REG_ADC_ATOP_BASE + 0x6E)
#define REG_ADC_ATOP_37_H       (REG_ADC_ATOP_BASE + 0x6F)
#define REG_ADC_ATOP_38_L       (REG_ADC_ATOP_BASE + 0x70)
#define REG_ADC_ATOP_38_H       (REG_ADC_ATOP_BASE + 0x71)
#define REG_ADC_ATOP_39_L       (REG_ADC_ATOP_BASE + 0x72)
#define REG_ADC_ATOP_39_H       (REG_ADC_ATOP_BASE + 0x73)
#define REG_ADC_ATOP_3A_L       (REG_ADC_ATOP_BASE + 0x74)
#define REG_ADC_ATOP_3A_H       (REG_ADC_ATOP_BASE + 0x75)
#define REG_ADC_ATOP_3B_L       (REG_ADC_ATOP_BASE + 0x76)
#define REG_ADC_ATOP_3B_H       (REG_ADC_ATOP_BASE + 0x77)
#define REG_ADC_ATOP_3C_L       (REG_ADC_ATOP_BASE + 0x78)
#define REG_ADC_ATOP_3C_H       (REG_ADC_ATOP_BASE + 0x79)
#define REG_ADC_ATOP_3D_L       (REG_ADC_ATOP_BASE + 0x7A)
#define REG_ADC_ATOP_3D_H       (REG_ADC_ATOP_BASE + 0x7B)
#define REG_ADC_ATOP_3E_L       (REG_ADC_ATOP_BASE + 0x7C)
#define REG_ADC_ATOP_3E_H       (REG_ADC_ATOP_BASE + 0x7D)
#define REG_ADC_ATOP_3F_L       (REG_ADC_ATOP_BASE + 0x7E)
#define REG_ADC_ATOP_3F_H       (REG_ADC_ATOP_BASE + 0x7F)
#define REG_ADC_ATOP_40_L       (REG_ADC_ATOP_BASE + 0x80)
#define REG_ADC_ATOP_40_H       (REG_ADC_ATOP_BASE + 0x81)
#define REG_ADC_ATOP_41_L       (REG_ADC_ATOP_BASE + 0x82)
#define REG_ADC_ATOP_41_H       (REG_ADC_ATOP_BASE + 0x83)
#define REG_ADC_ATOP_42_L       (REG_ADC_ATOP_BASE + 0x84)
#define REG_ADC_ATOP_42_H       (REG_ADC_ATOP_BASE + 0x85)
#define REG_ADC_ATOP_43_L       (REG_ADC_ATOP_BASE + 0x86)
#define REG_ADC_ATOP_43_H       (REG_ADC_ATOP_BASE + 0x87)
#define REG_ADC_ATOP_44_L       (REG_ADC_ATOP_BASE + 0x88)
#define REG_ADC_ATOP_44_H       (REG_ADC_ATOP_BASE + 0x89)
#define REG_ADC_ATOP_45_L       (REG_ADC_ATOP_BASE + 0x8A)
#define REG_ADC_ATOP_45_H       (REG_ADC_ATOP_BASE + 0x8B)
#define REG_ADC_ATOP_46_L       (REG_ADC_ATOP_BASE + 0x8C)
#define REG_ADC_ATOP_46_H       (REG_ADC_ATOP_BASE + 0x8D)
#define REG_ADC_ATOP_47_L       (REG_ADC_ATOP_BASE + 0x8E)
#define REG_ADC_ATOP_47_H       (REG_ADC_ATOP_BASE + 0x8F)
#define REG_ADC_ATOP_48_L       (REG_ADC_ATOP_BASE + 0x90)
#define REG_ADC_ATOP_48_H       (REG_ADC_ATOP_BASE + 0x91)
#define REG_ADC_ATOP_49_L       (REG_ADC_ATOP_BASE + 0x92)
#define REG_ADC_ATOP_49_H       (REG_ADC_ATOP_BASE + 0x93)
#define REG_ADC_ATOP_4A_L       (REG_ADC_ATOP_BASE + 0x94)
#define REG_ADC_ATOP_4A_H       (REG_ADC_ATOP_BASE + 0x95)
#define REG_ADC_ATOP_4B_L       (REG_ADC_ATOP_BASE + 0x96)
#define REG_ADC_ATOP_4B_H       (REG_ADC_ATOP_BASE + 0x97)
#define REG_ADC_ATOP_4C_L       (REG_ADC_ATOP_BASE + 0x98)
#define REG_ADC_ATOP_4C_H       (REG_ADC_ATOP_BASE + 0x99)
#define REG_ADC_ATOP_4D_L       (REG_ADC_ATOP_BASE + 0x9A)
#define REG_ADC_ATOP_4D_H       (REG_ADC_ATOP_BASE + 0x9B)
#define REG_ADC_ATOP_4E_L       (REG_ADC_ATOP_BASE + 0x9C)
#define REG_ADC_ATOP_4E_H       (REG_ADC_ATOP_BASE + 0x9D)
#define REG_ADC_ATOP_4F_L       (REG_ADC_ATOP_BASE + 0x9E)
#define REG_ADC_ATOP_4F_H       (REG_ADC_ATOP_BASE + 0x9F)
#define REG_ADC_ATOP_50_L       (REG_ADC_ATOP_BASE + 0xA0)
#define REG_ADC_ATOP_50_H       (REG_ADC_ATOP_BASE + 0xA1)
#define REG_ADC_ATOP_51_L       (REG_ADC_ATOP_BASE + 0xA2)
#define REG_ADC_ATOP_51_H       (REG_ADC_ATOP_BASE + 0xA3)
#define REG_ADC_ATOP_52_L       (REG_ADC_ATOP_BASE + 0xA4)
#define REG_ADC_ATOP_52_H       (REG_ADC_ATOP_BASE + 0xA5)
#define REG_ADC_ATOP_53_L       (REG_ADC_ATOP_BASE + 0xA6)
#define REG_ADC_ATOP_53_H       (REG_ADC_ATOP_BASE + 0xA7)
#define REG_ADC_ATOP_54_L       (REG_ADC_ATOP_BASE + 0xA8)
#define REG_ADC_ATOP_54_H       (REG_ADC_ATOP_BASE + 0xA9)
#define REG_ADC_ATOP_55_L       (REG_ADC_ATOP_BASE + 0xAA)
#define REG_ADC_ATOP_55_H       (REG_ADC_ATOP_BASE + 0xAB)
#define REG_ADC_ATOP_56_L       (REG_ADC_ATOP_BASE + 0xAC)
#define REG_ADC_ATOP_56_H       (REG_ADC_ATOP_BASE + 0xAD)
#define REG_ADC_ATOP_57_L       (REG_ADC_ATOP_BASE + 0xAE)
#define REG_ADC_ATOP_57_H       (REG_ADC_ATOP_BASE + 0xAF)
#define REG_ADC_ATOP_58_L       (REG_ADC_ATOP_BASE + 0xB0)
#define REG_ADC_ATOP_58_H       (REG_ADC_ATOP_BASE + 0xB1)
#define REG_ADC_ATOP_59_L       (REG_ADC_ATOP_BASE + 0xB2)
#define REG_ADC_ATOP_59_H       (REG_ADC_ATOP_BASE + 0xB3)
#define REG_ADC_ATOP_5A_L       (REG_ADC_ATOP_BASE + 0xB4)
#define REG_ADC_ATOP_5A_H       (REG_ADC_ATOP_BASE + 0xB5)
#define REG_ADC_ATOP_5B_L       (REG_ADC_ATOP_BASE + 0xB6)
#define REG_ADC_ATOP_5B_H       (REG_ADC_ATOP_BASE + 0xB7)
#define REG_ADC_ATOP_5C_L       (REG_ADC_ATOP_BASE + 0xB8)
#define REG_ADC_ATOP_5C_H       (REG_ADC_ATOP_BASE + 0xB9)
#define REG_ADC_ATOP_5D_L       (REG_ADC_ATOP_BASE + 0xBA)
#define REG_ADC_ATOP_5D_H       (REG_ADC_ATOP_BASE + 0xBB)
#define REG_ADC_ATOP_5E_L       (REG_ADC_ATOP_BASE + 0xBC)
#define REG_ADC_ATOP_5E_H       (REG_ADC_ATOP_BASE + 0xBD)
#define REG_ADC_ATOP_5F_L       (REG_ADC_ATOP_BASE + 0xBE)
#define REG_ADC_ATOP_5F_H       (REG_ADC_ATOP_BASE + 0xBF)
#define REG_ADC_ATOP_60_L       (REG_ADC_ATOP_BASE + 0xC0)
#define REG_ADC_ATOP_60_H       (REG_ADC_ATOP_BASE + 0xC1)
#define REG_ADC_ATOP_61_L       (REG_ADC_ATOP_BASE + 0xC2)
#define REG_ADC_ATOP_61_H       (REG_ADC_ATOP_BASE + 0xC3)
#define REG_ADC_ATOP_62_L       (REG_ADC_ATOP_BASE + 0xC4)
#define REG_ADC_ATOP_62_H       (REG_ADC_ATOP_BASE + 0xC5)
#define REG_ADC_ATOP_63_L       (REG_ADC_ATOP_BASE + 0xC6)
#define REG_ADC_ATOP_63_H       (REG_ADC_ATOP_BASE + 0xC7)
#define REG_ADC_ATOP_64_L       (REG_ADC_ATOP_BASE + 0xC8)
#define REG_ADC_ATOP_64_H       (REG_ADC_ATOP_BASE + 0xC9)
#define REG_ADC_ATOP_65_L       (REG_ADC_ATOP_BASE + 0xCA)
#define REG_ADC_ATOP_65_H       (REG_ADC_ATOP_BASE + 0xCB)
#define REG_ADC_ATOP_66_L       (REG_ADC_ATOP_BASE + 0xCC)
#define REG_ADC_ATOP_66_H       (REG_ADC_ATOP_BASE + 0xCD)
#define REG_ADC_ATOP_67_L       (REG_ADC_ATOP_BASE + 0xCE)
#define REG_ADC_ATOP_67_H       (REG_ADC_ATOP_BASE + 0xCF)
#define REG_ADC_ATOP_68_L       (REG_ADC_ATOP_BASE + 0xD0)
#define REG_ADC_ATOP_68_H       (REG_ADC_ATOP_BASE + 0xD1)
#define REG_ADC_ATOP_69_L       (REG_ADC_ATOP_BASE + 0xD2)
#define REG_ADC_ATOP_69_H       (REG_ADC_ATOP_BASE + 0xD3)
#define REG_ADC_ATOP_6A_L       (REG_ADC_ATOP_BASE + 0xD4)
#define REG_ADC_ATOP_6A_H       (REG_ADC_ATOP_BASE + 0xD5)
#define REG_ADC_ATOP_6B_L       (REG_ADC_ATOP_BASE + 0xD6)
#define REG_ADC_ATOP_6B_H       (REG_ADC_ATOP_BASE + 0xD7)
#define REG_ADC_ATOP_6C_L       (REG_ADC_ATOP_BASE + 0xD8)
#define REG_ADC_ATOP_6C_H       (REG_ADC_ATOP_BASE + 0xD9)
#define REG_ADC_ATOP_6D_L       (REG_ADC_ATOP_BASE + 0xDA)
#define REG_ADC_ATOP_6D_H       (REG_ADC_ATOP_BASE + 0xDB)
#define REG_ADC_ATOP_6E_L       (REG_ADC_ATOP_BASE + 0xDC)
#define REG_ADC_ATOP_6E_H       (REG_ADC_ATOP_BASE + 0xDD)
#define REG_ADC_ATOP_6F_L       (REG_ADC_ATOP_BASE + 0xDE)
#define REG_ADC_ATOP_6F_H       (REG_ADC_ATOP_BASE + 0xDF)
#define REG_ADC_ATOP_70_L       (REG_ADC_ATOP_BASE + 0xE0)
#define REG_ADC_ATOP_70_H       (REG_ADC_ATOP_BASE + 0xE1)
#define REG_ADC_ATOP_71_L       (REG_ADC_ATOP_BASE + 0xE2)
#define REG_ADC_ATOP_71_H       (REG_ADC_ATOP_BASE + 0xE3)
#define REG_ADC_ATOP_72_L       (REG_ADC_ATOP_BASE + 0xE4)
#define REG_ADC_ATOP_72_H       (REG_ADC_ATOP_BASE + 0xE5)
#define REG_ADC_ATOP_73_L       (REG_ADC_ATOP_BASE + 0xE6)
#define REG_ADC_ATOP_73_H       (REG_ADC_ATOP_BASE + 0xE7)
#define REG_ADC_ATOP_74_L       (REG_ADC_ATOP_BASE + 0xE8)
#define REG_ADC_ATOP_74_H       (REG_ADC_ATOP_BASE + 0xE9)
#define REG_ADC_ATOP_75_L       (REG_ADC_ATOP_BASE + 0xEA)
#define REG_ADC_ATOP_75_H       (REG_ADC_ATOP_BASE + 0xEB)
#define REG_ADC_ATOP_76_L       (REG_ADC_ATOP_BASE + 0xEC)
#define REG_ADC_ATOP_76_H       (REG_ADC_ATOP_BASE + 0xED)
#define REG_ADC_ATOP_77_L       (REG_ADC_ATOP_BASE + 0xEE)
#define REG_ADC_ATOP_77_H       (REG_ADC_ATOP_BASE + 0xEF)
#define REG_ADC_ATOP_78_L       (REG_ADC_ATOP_BASE + 0xF0)
#define REG_ADC_ATOP_78_H       (REG_ADC_ATOP_BASE + 0xF1)
#define REG_ADC_ATOP_79_L       (REG_ADC_ATOP_BASE + 0xF2)
#define REG_ADC_ATOP_79_H       (REG_ADC_ATOP_BASE + 0xF3)
#define REG_ADC_ATOP_7A_L       (REG_ADC_ATOP_BASE + 0xF4)
#define REG_ADC_ATOP_7A_H       (REG_ADC_ATOP_BASE + 0xF5)
#define REG_ADC_ATOP_7B_L       (REG_ADC_ATOP_BASE + 0xF6)
#define REG_ADC_ATOP_7B_H       (REG_ADC_ATOP_BASE + 0xF7)
#define REG_ADC_ATOP_7C_L       (REG_ADC_ATOP_BASE + 0xF8)
#define REG_ADC_ATOP_7C_H       (REG_ADC_ATOP_BASE + 0xF9)
#define REG_ADC_ATOP_7D_L       (REG_ADC_ATOP_BASE + 0xFA)
#define REG_ADC_ATOP_7D_H       (REG_ADC_ATOP_BASE + 0xFB)
#define REG_ADC_ATOP_7E_L       (REG_ADC_ATOP_BASE + 0xFC)
#define REG_ADC_ATOP_7E_H       (REG_ADC_ATOP_BASE + 0xFD)
#define REG_ADC_ATOP_7F_L       (REG_ADC_ATOP_BASE + 0xFE)
#define REG_ADC_ATOP_7F_H       (REG_ADC_ATOP_BASE + 0xFF)


#endif

