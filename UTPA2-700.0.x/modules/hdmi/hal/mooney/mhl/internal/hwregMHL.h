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
/******************************************************************************
 Copyright (c) 2012 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: hwreg_mhl.h
 [Date]:        04-March-2012
 [Comment]:
 [Reversion History]:
*******************************************************************************/

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
enum EN_MUX_INPUTPORT_MHL_INFO
{
    E_MUX_NOT_SUPPORT_MHL = 0x0,
    E_MUX_SUPPORT_MHL_PORT_DVI0 = 0x1,
    E_MUX_SUPPORT_MHL_PORT_DVI1 = 0x2,
    E_MUX_SUPPORT_MHL_PORT_DVI2 = 0x4,
    E_MUX_SUPPORT_MHL_PORT_DVI3 = 0x8,
    E_MUX_ALL_SUPPORT_MHL = 0xF,
};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////
// Register bank
///////////////////////////////////////////////////////
#define REG_MHL_CBUS_BANK       0x001F00UL
#define REG_PM_MHL_CBUS_BANK    0x002F00UL
#define REG_MHL_ECBUS_BANK      0x170F00UL
#define REG_MHL_ECBUS_PHY_BANK  0x173C00UL

//=============================================================
// MHL CBUS
// #define REG_MHL_CBUS_BANK           0x001F00UL
#define REG_MHL_CBUS_00         (REG_MHL_CBUS_BANK + 0x00)
#define REG_MHL_CBUS_01         (REG_MHL_CBUS_BANK + 0x02)
#define REG_MHL_CBUS_02         (REG_MHL_CBUS_BANK + 0x04)
#define REG_MHL_CBUS_03         (REG_MHL_CBUS_BANK + 0x06)
#define REG_MHL_CBUS_04         (REG_MHL_CBUS_BANK + 0x08)
#define REG_MHL_CBUS_05         (REG_MHL_CBUS_BANK + 0x0A)
#define REG_MHL_CBUS_06         (REG_MHL_CBUS_BANK + 0x0C)
#define REG_MHL_CBUS_07         (REG_MHL_CBUS_BANK + 0x0E)
#define REG_MHL_CBUS_08         (REG_MHL_CBUS_BANK + 0x10)
#define REG_MHL_CBUS_09         (REG_MHL_CBUS_BANK + 0x12)
#define REG_MHL_CBUS_0A         (REG_MHL_CBUS_BANK + 0x14)
#define REG_MHL_CBUS_0B         (REG_MHL_CBUS_BANK + 0x16)
#define REG_MHL_CBUS_0C         (REG_MHL_CBUS_BANK + 0x18)
#define REG_MHL_CBUS_0D         (REG_MHL_CBUS_BANK + 0x1A)
#define REG_MHL_CBUS_0E         (REG_MHL_CBUS_BANK + 0x1C)
#define REG_MHL_CBUS_0F         (REG_MHL_CBUS_BANK + 0x1E)
#define REG_MHL_CBUS_10         (REG_MHL_CBUS_BANK + 0x20)
#define REG_MHL_CBUS_11         (REG_MHL_CBUS_BANK + 0x22)
#define REG_MHL_CBUS_12         (REG_MHL_CBUS_BANK + 0x24)
#define REG_MHL_CBUS_13         (REG_MHL_CBUS_BANK + 0x26)
#define REG_MHL_CBUS_14         (REG_MHL_CBUS_BANK + 0x28)
#define REG_MHL_CBUS_15         (REG_MHL_CBUS_BANK + 0x2A)
#define REG_MHL_CBUS_16         (REG_MHL_CBUS_BANK + 0x2C)
#define REG_MHL_CBUS_17         (REG_MHL_CBUS_BANK + 0x2E)
#define REG_MHL_CBUS_18         (REG_MHL_CBUS_BANK + 0x30)
#define REG_MHL_CBUS_19         (REG_MHL_CBUS_BANK + 0x32)
#define REG_MHL_CBUS_1A         (REG_MHL_CBUS_BANK + 0x34)
#define REG_MHL_CBUS_1B         (REG_MHL_CBUS_BANK + 0x36)
#define REG_MHL_CBUS_1C         (REG_MHL_CBUS_BANK + 0x38)
#define REG_MHL_CBUS_1D         (REG_MHL_CBUS_BANK + 0x3A)
#define REG_MHL_CBUS_1E         (REG_MHL_CBUS_BANK + 0x3C)
#define REG_MHL_CBUS_1F         (REG_MHL_CBUS_BANK + 0x3E)
#define REG_MHL_CBUS_20         (REG_MHL_CBUS_BANK + 0x40)
#define REG_MHL_CBUS_21         (REG_MHL_CBUS_BANK + 0x42)
#define REG_MHL_CBUS_22         (REG_MHL_CBUS_BANK + 0x44)
#define REG_MHL_CBUS_23         (REG_MHL_CBUS_BANK + 0x46)
#define REG_MHL_CBUS_24         (REG_MHL_CBUS_BANK + 0x48)
#define REG_MHL_CBUS_25         (REG_MHL_CBUS_BANK + 0x4A)
#define REG_MHL_CBUS_26         (REG_MHL_CBUS_BANK + 0x4C)
#define REG_MHL_CBUS_27         (REG_MHL_CBUS_BANK + 0x4E)
#define REG_MHL_CBUS_28         (REG_MHL_CBUS_BANK + 0x50)
#define REG_MHL_CBUS_29         (REG_MHL_CBUS_BANK + 0x52)
#define REG_MHL_CBUS_2A         (REG_MHL_CBUS_BANK + 0x54)
#define REG_MHL_CBUS_2B         (REG_MHL_CBUS_BANK + 0x56)
#define REG_MHL_CBUS_2C         (REG_MHL_CBUS_BANK + 0x58)
#define REG_MHL_CBUS_2D         (REG_MHL_CBUS_BANK + 0x5A)
#define REG_MHL_CBUS_2E         (REG_MHL_CBUS_BANK + 0x5C)
#define REG_MHL_CBUS_2F         (REG_MHL_CBUS_BANK + 0x5E)
#define REG_MHL_CBUS_30         (REG_MHL_CBUS_BANK + 0x60)
#define REG_MHL_CBUS_31         (REG_MHL_CBUS_BANK + 0x62)
#define REG_MHL_CBUS_32         (REG_MHL_CBUS_BANK + 0x64)
#define REG_MHL_CBUS_33         (REG_MHL_CBUS_BANK + 0x66)
#define REG_MHL_CBUS_34         (REG_MHL_CBUS_BANK + 0x68)
#define REG_MHL_CBUS_35         (REG_MHL_CBUS_BANK + 0x6A)
#define REG_MHL_CBUS_36         (REG_MHL_CBUS_BANK + 0x6C)
#define REG_MHL_CBUS_37         (REG_MHL_CBUS_BANK + 0x6E)
#define REG_MHL_CBUS_38         (REG_MHL_CBUS_BANK + 0x70)
#define REG_MHL_CBUS_39         (REG_MHL_CBUS_BANK + 0x72)
#define REG_MHL_CBUS_3A         (REG_MHL_CBUS_BANK + 0x74)
#define REG_MHL_CBUS_3B         (REG_MHL_CBUS_BANK + 0x76)
#define REG_MHL_CBUS_3C         (REG_MHL_CBUS_BANK + 0x78)
#define REG_MHL_CBUS_3D         (REG_MHL_CBUS_BANK + 0x7A)
#define REG_MHL_CBUS_3E         (REG_MHL_CBUS_BANK + 0x7C)
#define REG_MHL_CBUS_3F         (REG_MHL_CBUS_BANK + 0x7E)
#define REG_MHL_CBUS_40         (REG_MHL_CBUS_BANK + 0x80)
#define REG_MHL_CBUS_41         (REG_MHL_CBUS_BANK + 0x82)
#define REG_MHL_CBUS_42         (REG_MHL_CBUS_BANK + 0x84)
#define REG_MHL_CBUS_43         (REG_MHL_CBUS_BANK + 0x86)
#define REG_MHL_CBUS_44         (REG_MHL_CBUS_BANK + 0x88)
#define REG_MHL_CBUS_45         (REG_MHL_CBUS_BANK + 0x8A)
#define REG_MHL_CBUS_46         (REG_MHL_CBUS_BANK + 0x8C)
#define REG_MHL_CBUS_47         (REG_MHL_CBUS_BANK + 0x8E)
#define REG_MHL_CBUS_48         (REG_MHL_CBUS_BANK + 0x90)
#define REG_MHL_CBUS_49         (REG_MHL_CBUS_BANK + 0x92)
#define REG_MHL_CBUS_4A         (REG_MHL_CBUS_BANK + 0x94)
#define REG_MHL_CBUS_4B         (REG_MHL_CBUS_BANK + 0x96)
#define REG_MHL_CBUS_4C         (REG_MHL_CBUS_BANK + 0x98)
#define REG_MHL_CBUS_4D         (REG_MHL_CBUS_BANK + 0x9A)
#define REG_MHL_CBUS_4E         (REG_MHL_CBUS_BANK + 0x9C)
#define REG_MHL_CBUS_4F         (REG_MHL_CBUS_BANK + 0x9E)
#define REG_MHL_CBUS_50         (REG_MHL_CBUS_BANK + 0xA0)
#define REG_MHL_CBUS_51         (REG_MHL_CBUS_BANK + 0xA2)
#define REG_MHL_CBUS_52         (REG_MHL_CBUS_BANK + 0xA4)
#define REG_MHL_CBUS_53         (REG_MHL_CBUS_BANK + 0xA6)
#define REG_MHL_CBUS_54         (REG_MHL_CBUS_BANK + 0xA8)
#define REG_MHL_CBUS_55         (REG_MHL_CBUS_BANK + 0xAA)
#define REG_MHL_CBUS_56         (REG_MHL_CBUS_BANK + 0xAC)
#define REG_MHL_CBUS_57         (REG_MHL_CBUS_BANK + 0xAE)
#define REG_MHL_CBUS_58         (REG_MHL_CBUS_BANK + 0xB0)
#define REG_MHL_CBUS_59         (REG_MHL_CBUS_BANK + 0xB2)
#define REG_MHL_CBUS_5A         (REG_MHL_CBUS_BANK + 0xB4)
#define REG_MHL_CBUS_5B         (REG_MHL_CBUS_BANK + 0xB6)
#define REG_MHL_CBUS_5C         (REG_MHL_CBUS_BANK + 0xB8)
#define REG_MHL_CBUS_5D         (REG_MHL_CBUS_BANK + 0xBA)
#define REG_MHL_CBUS_5E         (REG_MHL_CBUS_BANK + 0xBC)
#define REG_MHL_CBUS_5F         (REG_MHL_CBUS_BANK + 0xBE)
#define REG_MHL_CBUS_60         (REG_MHL_CBUS_BANK + 0xC0)
#define REG_MHL_CBUS_61         (REG_MHL_CBUS_BANK + 0xC2)
#define REG_MHL_CBUS_62         (REG_MHL_CBUS_BANK + 0xC4)
#define REG_MHL_CBUS_63         (REG_MHL_CBUS_BANK + 0xC6)
#define REG_MHL_CBUS_64         (REG_MHL_CBUS_BANK + 0xC8)
#define REG_MHL_CBUS_65         (REG_MHL_CBUS_BANK + 0xCA)
#define REG_MHL_CBUS_66         (REG_MHL_CBUS_BANK + 0xCC)
#define REG_MHL_CBUS_67         (REG_MHL_CBUS_BANK + 0xCE)
#define REG_MHL_CBUS_68         (REG_MHL_CBUS_BANK + 0xD0)
#define REG_MHL_CBUS_69         (REG_MHL_CBUS_BANK + 0xD2)
#define REG_MHL_CBUS_6A         (REG_MHL_CBUS_BANK + 0xD4)
#define REG_MHL_CBUS_6B         (REG_MHL_CBUS_BANK + 0xD6)
#define REG_MHL_CBUS_6C         (REG_MHL_CBUS_BANK + 0xD8)
#define REG_MHL_CBUS_6D         (REG_MHL_CBUS_BANK + 0xDA)
#define REG_MHL_CBUS_6E         (REG_MHL_CBUS_BANK + 0xDC)
#define REG_MHL_CBUS_6F         (REG_MHL_CBUS_BANK + 0xDE)
#define REG_MHL_CBUS_70         (REG_MHL_CBUS_BANK + 0xE0)
#define REG_MHL_CBUS_71         (REG_MHL_CBUS_BANK + 0xE2)
#define REG_MHL_CBUS_72         (REG_MHL_CBUS_BANK + 0xE4)
#define REG_MHL_CBUS_73         (REG_MHL_CBUS_BANK + 0xE6)
#define REG_MHL_CBUS_74         (REG_MHL_CBUS_BANK + 0xE8)
#define REG_MHL_CBUS_75         (REG_MHL_CBUS_BANK + 0xEA)
#define REG_MHL_CBUS_76         (REG_MHL_CBUS_BANK + 0xEC)
#define REG_MHL_CBUS_77         (REG_MHL_CBUS_BANK + 0xEE)
#define REG_MHL_CBUS_78         (REG_MHL_CBUS_BANK + 0xF0)
#define REG_MHL_CBUS_79         (REG_MHL_CBUS_BANK + 0xF2)
#define REG_MHL_CBUS_7A         (REG_MHL_CBUS_BANK + 0xF4)
#define REG_MHL_CBUS_7B         (REG_MHL_CBUS_BANK + 0xF6)
#define REG_MHL_CBUS_7C         (REG_MHL_CBUS_BANK + 0xF8)
#define REG_MHL_CBUS_7D         (REG_MHL_CBUS_BANK + 0xFA)
#define REG_MHL_CBUS_7E         (REG_MHL_CBUS_BANK + 0xFC)
#define REG_MHL_CBUS_7F         (REG_MHL_CBUS_BANK + 0xFE)

//=============================================================
// PM MHL CBUS
// #define REG_PM_MHL_CBUS_BANK    0x002F00UL
#define REG_PM_MHL_CBUS_00      (REG_PM_MHL_CBUS_BANK + 0x00)
#define REG_PM_MHL_CBUS_01      (REG_PM_MHL_CBUS_BANK + 0x02)
#define REG_PM_MHL_CBUS_02      (REG_PM_MHL_CBUS_BANK + 0x04)
#define REG_PM_MHL_CBUS_03      (REG_PM_MHL_CBUS_BANK + 0x06)
#define REG_PM_MHL_CBUS_04      (REG_PM_MHL_CBUS_BANK + 0x08)
#define REG_PM_MHL_CBUS_05      (REG_PM_MHL_CBUS_BANK + 0x0A)
#define REG_PM_MHL_CBUS_06      (REG_PM_MHL_CBUS_BANK + 0x0C)
#define REG_PM_MHL_CBUS_07      (REG_PM_MHL_CBUS_BANK + 0x0E)
#define REG_PM_MHL_CBUS_08      (REG_PM_MHL_CBUS_BANK + 0x10)
#define REG_PM_MHL_CBUS_09      (REG_PM_MHL_CBUS_BANK + 0x12)
#define REG_PM_MHL_CBUS_0A      (REG_PM_MHL_CBUS_BANK + 0x14)
#define REG_PM_MHL_CBUS_0B      (REG_PM_MHL_CBUS_BANK + 0x16)
#define REG_PM_MHL_CBUS_0C      (REG_PM_MHL_CBUS_BANK + 0x18)
#define REG_PM_MHL_CBUS_0D      (REG_PM_MHL_CBUS_BANK + 0x1A)
#define REG_PM_MHL_CBUS_0E      (REG_PM_MHL_CBUS_BANK + 0x1C)
#define REG_PM_MHL_CBUS_0F      (REG_PM_MHL_CBUS_BANK + 0x1E)
#define REG_PM_MHL_CBUS_10      (REG_PM_MHL_CBUS_BANK + 0x20)
#define REG_PM_MHL_CBUS_11      (REG_PM_MHL_CBUS_BANK + 0x22)
#define REG_PM_MHL_CBUS_12      (REG_PM_MHL_CBUS_BANK + 0x24)
#define REG_PM_MHL_CBUS_13      (REG_PM_MHL_CBUS_BANK + 0x26)
#define REG_PM_MHL_CBUS_14      (REG_PM_MHL_CBUS_BANK + 0x28)
#define REG_PM_MHL_CBUS_15      (REG_PM_MHL_CBUS_BANK + 0x2A)
#define REG_PM_MHL_CBUS_16      (REG_PM_MHL_CBUS_BANK + 0x2C)
#define REG_PM_MHL_CBUS_17      (REG_PM_MHL_CBUS_BANK + 0x2E)
#define REG_PM_MHL_CBUS_18      (REG_PM_MHL_CBUS_BANK + 0x30)
#define REG_PM_MHL_CBUS_19      (REG_PM_MHL_CBUS_BANK + 0x32)
#define REG_PM_MHL_CBUS_1A      (REG_PM_MHL_CBUS_BANK + 0x34)
#define REG_PM_MHL_CBUS_1B      (REG_PM_MHL_CBUS_BANK + 0x36)
#define REG_PM_MHL_CBUS_1C      (REG_PM_MHL_CBUS_BANK + 0x38)
#define REG_PM_MHL_CBUS_1D      (REG_PM_MHL_CBUS_BANK + 0x3A)
#define REG_PM_MHL_CBUS_1E      (REG_PM_MHL_CBUS_BANK + 0x3C)
#define REG_PM_MHL_CBUS_1F      (REG_PM_MHL_CBUS_BANK + 0x3E)
#define REG_PM_MHL_CBUS_20      (REG_PM_MHL_CBUS_BANK + 0x40)
#define REG_PM_MHL_CBUS_21      (REG_PM_MHL_CBUS_BANK + 0x42)
#define REG_PM_MHL_CBUS_22      (REG_PM_MHL_CBUS_BANK + 0x44)
#define REG_PM_MHL_CBUS_23      (REG_PM_MHL_CBUS_BANK + 0x46)
#define REG_PM_MHL_CBUS_24      (REG_PM_MHL_CBUS_BANK + 0x48)
#define REG_PM_MHL_CBUS_25      (REG_PM_MHL_CBUS_BANK + 0x4A)
#define REG_PM_MHL_CBUS_26      (REG_PM_MHL_CBUS_BANK + 0x4C)
#define REG_PM_MHL_CBUS_27      (REG_PM_MHL_CBUS_BANK + 0x4E)
#define REG_PM_MHL_CBUS_28      (REG_PM_MHL_CBUS_BANK + 0x50)
#define REG_PM_MHL_CBUS_29      (REG_PM_MHL_CBUS_BANK + 0x52)
#define REG_PM_MHL_CBUS_2A      (REG_PM_MHL_CBUS_BANK + 0x54)
#define REG_PM_MHL_CBUS_2B      (REG_PM_MHL_CBUS_BANK + 0x56)
#define REG_PM_MHL_CBUS_2C      (REG_PM_MHL_CBUS_BANK + 0x58)
#define REG_PM_MHL_CBUS_2D      (REG_PM_MHL_CBUS_BANK + 0x5A)
#define REG_PM_MHL_CBUS_2E      (REG_PM_MHL_CBUS_BANK + 0x5C)
#define REG_PM_MHL_CBUS_2F      (REG_PM_MHL_CBUS_BANK + 0x5E)
#define REG_PM_MHL_CBUS_30      (REG_PM_MHL_CBUS_BANK + 0x60)
#define REG_PM_MHL_CBUS_31      (REG_PM_MHL_CBUS_BANK + 0x62)
#define REG_PM_MHL_CBUS_32      (REG_PM_MHL_CBUS_BANK + 0x64)
#define REG_PM_MHL_CBUS_33      (REG_PM_MHL_CBUS_BANK + 0x66)
#define REG_PM_MHL_CBUS_34      (REG_PM_MHL_CBUS_BANK + 0x68)
#define REG_PM_MHL_CBUS_35      (REG_PM_MHL_CBUS_BANK + 0x6A)
#define REG_PM_MHL_CBUS_36      (REG_PM_MHL_CBUS_BANK + 0x6C)
#define REG_PM_MHL_CBUS_37      (REG_PM_MHL_CBUS_BANK + 0x6E)
#define REG_PM_MHL_CBUS_38      (REG_PM_MHL_CBUS_BANK + 0x70)
#define REG_PM_MHL_CBUS_39      (REG_PM_MHL_CBUS_BANK + 0x72)
#define REG_PM_MHL_CBUS_3A      (REG_PM_MHL_CBUS_BANK + 0x74)
#define REG_PM_MHL_CBUS_3B      (REG_PM_MHL_CBUS_BANK + 0x76)
#define REG_PM_MHL_CBUS_3C      (REG_PM_MHL_CBUS_BANK + 0x78)
#define REG_PM_MHL_CBUS_3D      (REG_PM_MHL_CBUS_BANK + 0x7A)
#define REG_PM_MHL_CBUS_3E      (REG_PM_MHL_CBUS_BANK + 0x7C)
#define REG_PM_MHL_CBUS_3F      (REG_PM_MHL_CBUS_BANK + 0x7E)
#define REG_PM_MHL_CBUS_40      (REG_PM_MHL_CBUS_BANK + 0x80)
#define REG_PM_MHL_CBUS_41      (REG_PM_MHL_CBUS_BANK + 0x82)
#define REG_PM_MHL_CBUS_42      (REG_PM_MHL_CBUS_BANK + 0x84)
#define REG_PM_MHL_CBUS_43      (REG_PM_MHL_CBUS_BANK + 0x86)
#define REG_PM_MHL_CBUS_44      (REG_PM_MHL_CBUS_BANK + 0x88)
#define REG_PM_MHL_CBUS_45      (REG_PM_MHL_CBUS_BANK + 0x8A)
#define REG_PM_MHL_CBUS_46      (REG_PM_MHL_CBUS_BANK + 0x8C)
#define REG_PM_MHL_CBUS_47      (REG_PM_MHL_CBUS_BANK + 0x8E)
#define REG_PM_MHL_CBUS_48      (REG_PM_MHL_CBUS_BANK + 0x90)
#define REG_PM_MHL_CBUS_49      (REG_PM_MHL_CBUS_BANK + 0x92)
#define REG_PM_MHL_CBUS_4A      (REG_PM_MHL_CBUS_BANK + 0x94)
#define REG_PM_MHL_CBUS_4B      (REG_PM_MHL_CBUS_BANK + 0x96)
#define REG_PM_MHL_CBUS_4C      (REG_PM_MHL_CBUS_BANK + 0x98)
#define REG_PM_MHL_CBUS_4D      (REG_PM_MHL_CBUS_BANK + 0x9A)
#define REG_PM_MHL_CBUS_4E      (REG_PM_MHL_CBUS_BANK + 0x9C)
#define REG_PM_MHL_CBUS_4F      (REG_PM_MHL_CBUS_BANK + 0x9E)
#define REG_PM_MHL_CBUS_50      (REG_PM_MHL_CBUS_BANK + 0xA0)
#define REG_PM_MHL_CBUS_51      (REG_PM_MHL_CBUS_BANK + 0xA2)
#define REG_PM_MHL_CBUS_52      (REG_PM_MHL_CBUS_BANK + 0xA4)
#define REG_PM_MHL_CBUS_53      (REG_PM_MHL_CBUS_BANK + 0xA6)
#define REG_PM_MHL_CBUS_54      (REG_PM_MHL_CBUS_BANK + 0xA8)
#define REG_PM_MHL_CBUS_55      (REG_PM_MHL_CBUS_BANK + 0xAA)
#define REG_PM_MHL_CBUS_56      (REG_PM_MHL_CBUS_BANK + 0xAC)
#define REG_PM_MHL_CBUS_57      (REG_PM_MHL_CBUS_BANK + 0xAE)
#define REG_PM_MHL_CBUS_58      (REG_PM_MHL_CBUS_BANK + 0xB0)
#define REG_PM_MHL_CBUS_59      (REG_PM_MHL_CBUS_BANK + 0xB2)
#define REG_PM_MHL_CBUS_5A      (REG_PM_MHL_CBUS_BANK + 0xB4)
#define REG_PM_MHL_CBUS_5B      (REG_PM_MHL_CBUS_BANK + 0xB6)
#define REG_PM_MHL_CBUS_5C      (REG_PM_MHL_CBUS_BANK + 0xB8)
#define REG_PM_MHL_CBUS_5D      (REG_PM_MHL_CBUS_BANK + 0xBA)
#define REG_PM_MHL_CBUS_5E      (REG_PM_MHL_CBUS_BANK + 0xBC)
#define REG_PM_MHL_CBUS_5F      (REG_PM_MHL_CBUS_BANK + 0xBE)
#define REG_PM_MHL_CBUS_60      (REG_PM_MHL_CBUS_BANK + 0xC0)
#define REG_PM_MHL_CBUS_61      (REG_PM_MHL_CBUS_BANK + 0xC2)
#define REG_PM_MHL_CBUS_62      (REG_PM_MHL_CBUS_BANK + 0xC4)
#define REG_PM_MHL_CBUS_63      (REG_PM_MHL_CBUS_BANK + 0xC6)
#define REG_PM_MHL_CBUS_64      (REG_PM_MHL_CBUS_BANK + 0xC8)
#define REG_PM_MHL_CBUS_65      (REG_PM_MHL_CBUS_BANK + 0xCA)
#define REG_PM_MHL_CBUS_66      (REG_PM_MHL_CBUS_BANK + 0xCC)
#define REG_PM_MHL_CBUS_67      (REG_PM_MHL_CBUS_BANK + 0xCE)
#define REG_PM_MHL_CBUS_68      (REG_PM_MHL_CBUS_BANK + 0xD0)
#define REG_PM_MHL_CBUS_69      (REG_PM_MHL_CBUS_BANK + 0xD2)
#define REG_PM_MHL_CBUS_6A      (REG_PM_MHL_CBUS_BANK + 0xD4)
#define REG_PM_MHL_CBUS_6B      (REG_PM_MHL_CBUS_BANK + 0xD6)
#define REG_PM_MHL_CBUS_6C      (REG_PM_MHL_CBUS_BANK + 0xD8)
#define REG_PM_MHL_CBUS_6D      (REG_PM_MHL_CBUS_BANK + 0xDA)
#define REG_PM_MHL_CBUS_6E      (REG_PM_MHL_CBUS_BANK + 0xDC)
#define REG_PM_MHL_CBUS_6F      (REG_PM_MHL_CBUS_BANK + 0xDE)
#define REG_PM_MHL_CBUS_70      (REG_PM_MHL_CBUS_BANK + 0xE0)
#define REG_PM_MHL_CBUS_71      (REG_PM_MHL_CBUS_BANK + 0xE2)
#define REG_PM_MHL_CBUS_72      (REG_PM_MHL_CBUS_BANK + 0xE4)
#define REG_PM_MHL_CBUS_73      (REG_PM_MHL_CBUS_BANK + 0xE6)
#define REG_PM_MHL_CBUS_74      (REG_PM_MHL_CBUS_BANK + 0xE8)
#define REG_PM_MHL_CBUS_75      (REG_PM_MHL_CBUS_BANK + 0xEA)
#define REG_PM_MHL_CBUS_76      (REG_PM_MHL_CBUS_BANK + 0xEC)
#define REG_PM_MHL_CBUS_77      (REG_PM_MHL_CBUS_BANK + 0xEE)
#define REG_PM_MHL_CBUS_78      (REG_PM_MHL_CBUS_BANK + 0xF0)
#define REG_PM_MHL_CBUS_79      (REG_PM_MHL_CBUS_BANK + 0xF2)
#define REG_PM_MHL_CBUS_7A      (REG_PM_MHL_CBUS_BANK + 0xF4)
#define REG_PM_MHL_CBUS_7B      (REG_PM_MHL_CBUS_BANK + 0xF6)
#define REG_PM_MHL_CBUS_7C      (REG_PM_MHL_CBUS_BANK + 0xF8)
#define REG_PM_MHL_CBUS_7D      (REG_PM_MHL_CBUS_BANK + 0xFA)
#define REG_PM_MHL_CBUS_7E      (REG_PM_MHL_CBUS_BANK + 0xFC)
#define REG_PM_MHL_CBUS_7F      (REG_PM_MHL_CBUS_BANK + 0xFE)

//=============================================================
// MHL ECBUS
// #define REG_MHL_ECBUS_BANK      0x170F00UL
#define REG_MHL_ECBUS_00        (REG_MHL_ECBUS_BANK + 0x00)
#define REG_MHL_ECBUS_01        (REG_MHL_ECBUS_BANK + 0x02)
#define REG_MHL_ECBUS_02        (REG_MHL_ECBUS_BANK + 0x04)
#define REG_MHL_ECBUS_03        (REG_MHL_ECBUS_BANK + 0x06)
#define REG_MHL_ECBUS_04        (REG_MHL_ECBUS_BANK + 0x08)
#define REG_MHL_ECBUS_05        (REG_MHL_ECBUS_BANK + 0x0A)
#define REG_MHL_ECBUS_06        (REG_MHL_ECBUS_BANK + 0x0C)
#define REG_MHL_ECBUS_07        (REG_MHL_ECBUS_BANK + 0x0E)
#define REG_MHL_ECBUS_08        (REG_MHL_ECBUS_BANK + 0x10)
#define REG_MHL_ECBUS_09        (REG_MHL_ECBUS_BANK + 0x12)
#define REG_MHL_ECBUS_0A        (REG_MHL_ECBUS_BANK + 0x14)
#define REG_MHL_ECBUS_0B        (REG_MHL_ECBUS_BANK + 0x16)
#define REG_MHL_ECBUS_0C        (REG_MHL_ECBUS_BANK + 0x18)
#define REG_MHL_ECBUS_0D        (REG_MHL_ECBUS_BANK + 0x1A)
#define REG_MHL_ECBUS_0E        (REG_MHL_ECBUS_BANK + 0x1C)
#define REG_MHL_ECBUS_0F        (REG_MHL_ECBUS_BANK + 0x1E)
#define REG_MHL_ECBUS_10        (REG_MHL_ECBUS_BANK + 0x20)
#define REG_MHL_ECBUS_11        (REG_MHL_ECBUS_BANK + 0x22)
#define REG_MHL_ECBUS_12        (REG_MHL_ECBUS_BANK + 0x24)
#define REG_MHL_ECBUS_13        (REG_MHL_ECBUS_BANK + 0x26)
#define REG_MHL_ECBUS_14        (REG_MHL_ECBUS_BANK + 0x28)
#define REG_MHL_ECBUS_15        (REG_MHL_ECBUS_BANK + 0x2A)
#define REG_MHL_ECBUS_16        (REG_MHL_ECBUS_BANK + 0x2C)
#define REG_MHL_ECBUS_17        (REG_MHL_ECBUS_BANK + 0x2E)
#define REG_MHL_ECBUS_18        (REG_MHL_ECBUS_BANK + 0x30)
#define REG_MHL_ECBUS_19        (REG_MHL_ECBUS_BANK + 0x32)
#define REG_MHL_ECBUS_1A        (REG_MHL_ECBUS_BANK + 0x34)
#define REG_MHL_ECBUS_1B        (REG_MHL_ECBUS_BANK + 0x36)
#define REG_MHL_ECBUS_1C        (REG_MHL_ECBUS_BANK + 0x38)
#define REG_MHL_ECBUS_1D        (REG_MHL_ECBUS_BANK + 0x3A)
#define REG_MHL_ECBUS_1E        (REG_MHL_ECBUS_BANK + 0x3C)
#define REG_MHL_ECBUS_1F        (REG_MHL_ECBUS_BANK + 0x3E)
#define REG_MHL_ECBUS_20        (REG_MHL_ECBUS_BANK + 0x40)
#define REG_MHL_ECBUS_21        (REG_MHL_ECBUS_BANK + 0x42)
#define REG_MHL_ECBUS_22        (REG_MHL_ECBUS_BANK + 0x44)
#define REG_MHL_ECBUS_23        (REG_MHL_ECBUS_BANK + 0x46)
#define REG_MHL_ECBUS_24        (REG_MHL_ECBUS_BANK + 0x48)
#define REG_MHL_ECBUS_25        (REG_MHL_ECBUS_BANK + 0x4A)
#define REG_MHL_ECBUS_26        (REG_MHL_ECBUS_BANK + 0x4C)
#define REG_MHL_ECBUS_27        (REG_MHL_ECBUS_BANK + 0x4E)
#define REG_MHL_ECBUS_28        (REG_MHL_ECBUS_BANK + 0x50)
#define REG_MHL_ECBUS_29        (REG_MHL_ECBUS_BANK + 0x52)
#define REG_MHL_ECBUS_2A        (REG_MHL_ECBUS_BANK + 0x54)
#define REG_MHL_ECBUS_2B        (REG_MHL_ECBUS_BANK + 0x56)
#define REG_MHL_ECBUS_2C        (REG_MHL_ECBUS_BANK + 0x58)
#define REG_MHL_ECBUS_2D        (REG_MHL_ECBUS_BANK + 0x5A)
#define REG_MHL_ECBUS_2E        (REG_MHL_ECBUS_BANK + 0x5C)
#define REG_MHL_ECBUS_2F        (REG_MHL_ECBUS_BANK + 0x5E)
#define REG_MHL_ECBUS_30        (REG_MHL_ECBUS_BANK + 0x60)
#define REG_MHL_ECBUS_31        (REG_MHL_ECBUS_BANK + 0x62)
#define REG_MHL_ECBUS_32        (REG_MHL_ECBUS_BANK + 0x64)
#define REG_MHL_ECBUS_33        (REG_MHL_ECBUS_BANK + 0x66)
#define REG_MHL_ECBUS_34        (REG_MHL_ECBUS_BANK + 0x68)
#define REG_MHL_ECBUS_35        (REG_MHL_ECBUS_BANK + 0x6A)
#define REG_MHL_ECBUS_36        (REG_MHL_ECBUS_BANK + 0x6C)
#define REG_MHL_ECBUS_37        (REG_MHL_ECBUS_BANK + 0x6E)
#define REG_MHL_ECBUS_38        (REG_MHL_ECBUS_BANK + 0x70)
#define REG_MHL_ECBUS_39        (REG_MHL_ECBUS_BANK + 0x72)
#define REG_MHL_ECBUS_3A        (REG_MHL_ECBUS_BANK + 0x74)
#define REG_MHL_ECBUS_3B        (REG_MHL_ECBUS_BANK + 0x76)
#define REG_MHL_ECBUS_3C        (REG_MHL_ECBUS_BANK + 0x78)
#define REG_MHL_ECBUS_3D        (REG_MHL_ECBUS_BANK + 0x7A)
#define REG_MHL_ECBUS_3E        (REG_MHL_ECBUS_BANK + 0x7C)
#define REG_MHL_ECBUS_3F        (REG_MHL_ECBUS_BANK + 0x7E)
#define REG_MHL_ECBUS_40        (REG_MHL_ECBUS_BANK + 0x80)
#define REG_MHL_ECBUS_41        (REG_MHL_ECBUS_BANK + 0x82)
#define REG_MHL_ECBUS_42        (REG_MHL_ECBUS_BANK + 0x84)
#define REG_MHL_ECBUS_43        (REG_MHL_ECBUS_BANK + 0x86)
#define REG_MHL_ECBUS_44        (REG_MHL_ECBUS_BANK + 0x88)
#define REG_MHL_ECBUS_45        (REG_MHL_ECBUS_BANK + 0x8A)
#define REG_MHL_ECBUS_46        (REG_MHL_ECBUS_BANK + 0x8C)
#define REG_MHL_ECBUS_47        (REG_MHL_ECBUS_BANK + 0x8E)
#define REG_MHL_ECBUS_48        (REG_MHL_ECBUS_BANK + 0x90)
#define REG_MHL_ECBUS_49        (REG_MHL_ECBUS_BANK + 0x92)
#define REG_MHL_ECBUS_4A        (REG_MHL_ECBUS_BANK + 0x94)
#define REG_MHL_ECBUS_4B        (REG_MHL_ECBUS_BANK + 0x96)
#define REG_MHL_ECBUS_4C        (REG_MHL_ECBUS_BANK + 0x98)
#define REG_MHL_ECBUS_4D        (REG_MHL_ECBUS_BANK + 0x9A)
#define REG_MHL_ECBUS_4E        (REG_MHL_ECBUS_BANK + 0x9C)
#define REG_MHL_ECBUS_4F        (REG_MHL_ECBUS_BANK + 0x9E)
#define REG_MHL_ECBUS_50        (REG_MHL_ECBUS_BANK + 0xA0)
#define REG_MHL_ECBUS_51        (REG_MHL_ECBUS_BANK + 0xA2)
#define REG_MHL_ECBUS_52        (REG_MHL_ECBUS_BANK + 0xA4)
#define REG_MHL_ECBUS_53        (REG_MHL_ECBUS_BANK + 0xA6)
#define REG_MHL_ECBUS_54        (REG_MHL_ECBUS_BANK + 0xA8)
#define REG_MHL_ECBUS_55        (REG_MHL_ECBUS_BANK + 0xAA)
#define REG_MHL_ECBUS_56        (REG_MHL_ECBUS_BANK + 0xAC)
#define REG_MHL_ECBUS_57        (REG_MHL_ECBUS_BANK + 0xAE)
#define REG_MHL_ECBUS_58        (REG_MHL_ECBUS_BANK + 0xB0)
#define REG_MHL_ECBUS_59        (REG_MHL_ECBUS_BANK + 0xB2)
#define REG_MHL_ECBUS_5A        (REG_MHL_ECBUS_BANK + 0xB4)
#define REG_MHL_ECBUS_5B        (REG_MHL_ECBUS_BANK + 0xB6)
#define REG_MHL_ECBUS_5C        (REG_MHL_ECBUS_BANK + 0xB8)
#define REG_MHL_ECBUS_5D        (REG_MHL_ECBUS_BANK + 0xBA)
#define REG_MHL_ECBUS_5E        (REG_MHL_ECBUS_BANK + 0xBC)
#define REG_MHL_ECBUS_5F        (REG_MHL_ECBUS_BANK + 0xBE)
#define REG_MHL_ECBUS_60        (REG_MHL_ECBUS_BANK + 0xC0)
#define REG_MHL_ECBUS_61        (REG_MHL_ECBUS_BANK + 0xC2)
#define REG_MHL_ECBUS_62        (REG_MHL_ECBUS_BANK + 0xC4)
#define REG_MHL_ECBUS_63        (REG_MHL_ECBUS_BANK + 0xC6)
#define REG_MHL_ECBUS_64        (REG_MHL_ECBUS_BANK + 0xC8)
#define REG_MHL_ECBUS_65        (REG_MHL_ECBUS_BANK + 0xCA)
#define REG_MHL_ECBUS_66        (REG_MHL_ECBUS_BANK + 0xCC)
#define REG_MHL_ECBUS_67        (REG_MHL_ECBUS_BANK + 0xCE)
#define REG_MHL_ECBUS_68        (REG_MHL_ECBUS_BANK + 0xD0)
#define REG_MHL_ECBUS_69        (REG_MHL_ECBUS_BANK + 0xD2)
#define REG_MHL_ECBUS_6A        (REG_MHL_ECBUS_BANK + 0xD4)
#define REG_MHL_ECBUS_6B        (REG_MHL_ECBUS_BANK + 0xD6)
#define REG_MHL_ECBUS_6C        (REG_MHL_ECBUS_BANK + 0xD8)
#define REG_MHL_ECBUS_6D        (REG_MHL_ECBUS_BANK + 0xDA)
#define REG_MHL_ECBUS_6E        (REG_MHL_ECBUS_BANK + 0xDC)
#define REG_MHL_ECBUS_6F        (REG_MHL_ECBUS_BANK + 0xDE)
#define REG_MHL_ECBUS_70        (REG_MHL_ECBUS_BANK + 0xE0)
#define REG_MHL_ECBUS_71        (REG_MHL_ECBUS_BANK + 0xE2)
#define REG_MHL_ECBUS_72        (REG_MHL_ECBUS_BANK + 0xE4)
#define REG_MHL_ECBUS_73        (REG_MHL_ECBUS_BANK + 0xE6)
#define REG_MHL_ECBUS_74        (REG_MHL_ECBUS_BANK + 0xE8)
#define REG_MHL_ECBUS_75        (REG_MHL_ECBUS_BANK + 0xEA)
#define REG_MHL_ECBUS_76        (REG_MHL_ECBUS_BANK + 0xEC)
#define REG_MHL_ECBUS_77        (REG_MHL_ECBUS_BANK + 0xEE)
#define REG_MHL_ECBUS_78        (REG_MHL_ECBUS_BANK + 0xF0)
#define REG_MHL_ECBUS_79        (REG_MHL_ECBUS_BANK + 0xF2)
#define REG_MHL_ECBUS_7A        (REG_MHL_ECBUS_BANK + 0xF4)
#define REG_MHL_ECBUS_7B        (REG_MHL_ECBUS_BANK + 0xF6)
#define REG_MHL_ECBUS_7C        (REG_MHL_ECBUS_BANK + 0xF8)
#define REG_MHL_ECBUS_7D        (REG_MHL_ECBUS_BANK + 0xFA)
#define REG_MHL_ECBUS_7E        (REG_MHL_ECBUS_BANK + 0xFC)
#define REG_MHL_ECBUS_7F        (REG_MHL_ECBUS_BANK + 0xFE)

//=============================================================
// MHL ECBUS PHY
// #define REG_MHL_ECBUS_PHY_BANK  0x173C00UL
#define REG_MHL_ECBUS_PHY_00    (REG_MHL_ECBUS_PHY_BANK + 0x00)
#define REG_MHL_ECBUS_PHY_01    (REG_MHL_ECBUS_PHY_BANK + 0x02)
#define REG_MHL_ECBUS_PHY_02    (REG_MHL_ECBUS_PHY_BANK + 0x04)
#define REG_MHL_ECBUS_PHY_03    (REG_MHL_ECBUS_PHY_BANK + 0x06)
#define REG_MHL_ECBUS_PHY_04    (REG_MHL_ECBUS_PHY_BANK + 0x08)
#define REG_MHL_ECBUS_PHY_05    (REG_MHL_ECBUS_PHY_BANK + 0x0A)
#define REG_MHL_ECBUS_PHY_06    (REG_MHL_ECBUS_PHY_BANK + 0x0C)
#define REG_MHL_ECBUS_PHY_07    (REG_MHL_ECBUS_PHY_BANK + 0x0E)
#define REG_MHL_ECBUS_PHY_08    (REG_MHL_ECBUS_PHY_BANK + 0x10)
#define REG_MHL_ECBUS_PHY_09    (REG_MHL_ECBUS_PHY_BANK + 0x12)
#define REG_MHL_ECBUS_PHY_0A    (REG_MHL_ECBUS_PHY_BANK + 0x14)
#define REG_MHL_ECBUS_PHY_0B    (REG_MHL_ECBUS_PHY_BANK + 0x16)
#define REG_MHL_ECBUS_PHY_0C    (REG_MHL_ECBUS_PHY_BANK + 0x18)
#define REG_MHL_ECBUS_PHY_0D    (REG_MHL_ECBUS_PHY_BANK + 0x1A)
#define REG_MHL_ECBUS_PHY_0E    (REG_MHL_ECBUS_PHY_BANK + 0x1C)
#define REG_MHL_ECBUS_PHY_0F    (REG_MHL_ECBUS_PHY_BANK + 0x1E)
#define REG_MHL_ECBUS_PHY_10    (REG_MHL_ECBUS_PHY_BANK + 0x20)
#define REG_MHL_ECBUS_PHY_11    (REG_MHL_ECBUS_PHY_BANK + 0x22)
#define REG_MHL_ECBUS_PHY_12    (REG_MHL_ECBUS_PHY_BANK + 0x24)
#define REG_MHL_ECBUS_PHY_13    (REG_MHL_ECBUS_PHY_BANK + 0x26)
#define REG_MHL_ECBUS_PHY_14    (REG_MHL_ECBUS_PHY_BANK + 0x28)
#define REG_MHL_ECBUS_PHY_15    (REG_MHL_ECBUS_PHY_BANK + 0x2A)
#define REG_MHL_ECBUS_PHY_16    (REG_MHL_ECBUS_PHY_BANK + 0x2C)
#define REG_MHL_ECBUS_PHY_17    (REG_MHL_ECBUS_PHY_BANK + 0x2E)
#define REG_MHL_ECBUS_PHY_18    (REG_MHL_ECBUS_PHY_BANK + 0x30)
#define REG_MHL_ECBUS_PHY_19    (REG_MHL_ECBUS_PHY_BANK + 0x32)
#define REG_MHL_ECBUS_PHY_1A    (REG_MHL_ECBUS_PHY_BANK + 0x34)
#define REG_MHL_ECBUS_PHY_1B    (REG_MHL_ECBUS_PHY_BANK + 0x36)
#define REG_MHL_ECBUS_PHY_1C    (REG_MHL_ECBUS_PHY_BANK + 0x38)
#define REG_MHL_ECBUS_PHY_1D    (REG_MHL_ECBUS_PHY_BANK + 0x3A)
#define REG_MHL_ECBUS_PHY_1E    (REG_MHL_ECBUS_PHY_BANK + 0x3C)
#define REG_MHL_ECBUS_PHY_1F    (REG_MHL_ECBUS_PHY_BANK + 0x3E)
#define REG_MHL_ECBUS_PHY_20    (REG_MHL_ECBUS_PHY_BANK + 0x40)
#define REG_MHL_ECBUS_PHY_21    (REG_MHL_ECBUS_PHY_BANK + 0x42)
#define REG_MHL_ECBUS_PHY_22    (REG_MHL_ECBUS_PHY_BANK + 0x44)
#define REG_MHL_ECBUS_PHY_23    (REG_MHL_ECBUS_PHY_BANK + 0x46)
#define REG_MHL_ECBUS_PHY_24    (REG_MHL_ECBUS_PHY_BANK + 0x48)
#define REG_MHL_ECBUS_PHY_25    (REG_MHL_ECBUS_PHY_BANK + 0x4A)
#define REG_MHL_ECBUS_PHY_26    (REG_MHL_ECBUS_PHY_BANK + 0x4C)
#define REG_MHL_ECBUS_PHY_27    (REG_MHL_ECBUS_PHY_BANK + 0x4E)
#define REG_MHL_ECBUS_PHY_28    (REG_MHL_ECBUS_PHY_BANK + 0x50)
#define REG_MHL_ECBUS_PHY_29    (REG_MHL_ECBUS_PHY_BANK + 0x52)
#define REG_MHL_ECBUS_PHY_2A    (REG_MHL_ECBUS_PHY_BANK + 0x54)
#define REG_MHL_ECBUS_PHY_2B    (REG_MHL_ECBUS_PHY_BANK + 0x56)
#define REG_MHL_ECBUS_PHY_2C    (REG_MHL_ECBUS_PHY_BANK + 0x58)
#define REG_MHL_ECBUS_PHY_2D    (REG_MHL_ECBUS_PHY_BANK + 0x5A)
#define REG_MHL_ECBUS_PHY_2E    (REG_MHL_ECBUS_PHY_BANK + 0x5C)
#define REG_MHL_ECBUS_PHY_2F    (REG_MHL_ECBUS_PHY_BANK + 0x5E)
#define REG_MHL_ECBUS_PHY_30    (REG_MHL_ECBUS_PHY_BANK + 0x60)
#define REG_MHL_ECBUS_PHY_31    (REG_MHL_ECBUS_PHY_BANK + 0x62)
#define REG_MHL_ECBUS_PHY_32    (REG_MHL_ECBUS_PHY_BANK + 0x64)
#define REG_MHL_ECBUS_PHY_33    (REG_MHL_ECBUS_PHY_BANK + 0x66)
#define REG_MHL_ECBUS_PHY_34    (REG_MHL_ECBUS_PHY_BANK + 0x68)
#define REG_MHL_ECBUS_PHY_35    (REG_MHL_ECBUS_PHY_BANK + 0x6A)
#define REG_MHL_ECBUS_PHY_36    (REG_MHL_ECBUS_PHY_BANK + 0x6C)
#define REG_MHL_ECBUS_PHY_37    (REG_MHL_ECBUS_PHY_BANK + 0x6E)
#define REG_MHL_ECBUS_PHY_38    (REG_MHL_ECBUS_PHY_BANK + 0x70)
#define REG_MHL_ECBUS_PHY_39    (REG_MHL_ECBUS_PHY_BANK + 0x72)
#define REG_MHL_ECBUS_PHY_3A    (REG_MHL_ECBUS_PHY_BANK + 0x74)
#define REG_MHL_ECBUS_PHY_3B    (REG_MHL_ECBUS_PHY_BANK + 0x76)
#define REG_MHL_ECBUS_PHY_3C    (REG_MHL_ECBUS_PHY_BANK + 0x78)
#define REG_MHL_ECBUS_PHY_3D    (REG_MHL_ECBUS_PHY_BANK + 0x7A)
#define REG_MHL_ECBUS_PHY_3E    (REG_MHL_ECBUS_PHY_BANK + 0x7C)
#define REG_MHL_ECBUS_PHY_3F    (REG_MHL_ECBUS_PHY_BANK + 0x7E)
#define REG_MHL_ECBUS_PHY_40    (REG_MHL_ECBUS_PHY_BANK + 0x80)
#define REG_MHL_ECBUS_PHY_41    (REG_MHL_ECBUS_PHY_BANK + 0x82)
#define REG_MHL_ECBUS_PHY_42    (REG_MHL_ECBUS_PHY_BANK + 0x84)
#define REG_MHL_ECBUS_PHY_43    (REG_MHL_ECBUS_PHY_BANK + 0x86)
#define REG_MHL_ECBUS_PHY_44    (REG_MHL_ECBUS_PHY_BANK + 0x88)
#define REG_MHL_ECBUS_PHY_45    (REG_MHL_ECBUS_PHY_BANK + 0x8A)
#define REG_MHL_ECBUS_PHY_46    (REG_MHL_ECBUS_PHY_BANK + 0x8C)
#define REG_MHL_ECBUS_PHY_47    (REG_MHL_ECBUS_PHY_BANK + 0x8E)
#define REG_MHL_ECBUS_PHY_48    (REG_MHL_ECBUS_PHY_BANK + 0x90)
#define REG_MHL_ECBUS_PHY_49    (REG_MHL_ECBUS_PHY_BANK + 0x92)
#define REG_MHL_ECBUS_PHY_4A    (REG_MHL_ECBUS_PHY_BANK + 0x94)
#define REG_MHL_ECBUS_PHY_4B    (REG_MHL_ECBUS_PHY_BANK + 0x96)
#define REG_MHL_ECBUS_PHY_4C    (REG_MHL_ECBUS_PHY_BANK + 0x98)
#define REG_MHL_ECBUS_PHY_4D    (REG_MHL_ECBUS_PHY_BANK + 0x9A)
#define REG_MHL_ECBUS_PHY_4E    (REG_MHL_ECBUS_PHY_BANK + 0x9C)
#define REG_MHL_ECBUS_PHY_4F    (REG_MHL_ECBUS_PHY_BANK + 0x9E)
#define REG_MHL_ECBUS_PHY_50    (REG_MHL_ECBUS_PHY_BANK + 0xA0)
#define REG_MHL_ECBUS_PHY_51    (REG_MHL_ECBUS_PHY_BANK + 0xA2)
#define REG_MHL_ECBUS_PHY_52    (REG_MHL_ECBUS_PHY_BANK + 0xA4)
#define REG_MHL_ECBUS_PHY_53    (REG_MHL_ECBUS_PHY_BANK + 0xA6)
#define REG_MHL_ECBUS_PHY_54    (REG_MHL_ECBUS_PHY_BANK + 0xA8)
#define REG_MHL_ECBUS_PHY_55    (REG_MHL_ECBUS_PHY_BANK + 0xAA)
#define REG_MHL_ECBUS_PHY_56    (REG_MHL_ECBUS_PHY_BANK + 0xAC)
#define REG_MHL_ECBUS_PHY_57    (REG_MHL_ECBUS_PHY_BANK + 0xAE)
#define REG_MHL_ECBUS_PHY_58    (REG_MHL_ECBUS_PHY_BANK + 0xB0)
#define REG_MHL_ECBUS_PHY_59    (REG_MHL_ECBUS_PHY_BANK + 0xB2)
#define REG_MHL_ECBUS_PHY_5A    (REG_MHL_ECBUS_PHY_BANK + 0xB4)
#define REG_MHL_ECBUS_PHY_5B    (REG_MHL_ECBUS_PHY_BANK + 0xB6)
#define REG_MHL_ECBUS_PHY_5C    (REG_MHL_ECBUS_PHY_BANK + 0xB8)
#define REG_MHL_ECBUS_PHY_5D    (REG_MHL_ECBUS_PHY_BANK + 0xBA)
#define REG_MHL_ECBUS_PHY_5E    (REG_MHL_ECBUS_PHY_BANK + 0xBC)
#define REG_MHL_ECBUS_PHY_5F    (REG_MHL_ECBUS_PHY_BANK + 0xBE)
#define REG_MHL_ECBUS_PHY_60    (REG_MHL_ECBUS_PHY_BANK + 0xC0)
#define REG_MHL_ECBUS_PHY_61    (REG_MHL_ECBUS_PHY_BANK + 0xC2)
#define REG_MHL_ECBUS_PHY_62    (REG_MHL_ECBUS_PHY_BANK + 0xC4)
#define REG_MHL_ECBUS_PHY_63    (REG_MHL_ECBUS_PHY_BANK + 0xC6)
#define REG_MHL_ECBUS_PHY_64    (REG_MHL_ECBUS_PHY_BANK + 0xC8)
#define REG_MHL_ECBUS_PHY_65    (REG_MHL_ECBUS_PHY_BANK + 0xCA)
#define REG_MHL_ECBUS_PHY_66    (REG_MHL_ECBUS_PHY_BANK + 0xCC)
#define REG_MHL_ECBUS_PHY_67    (REG_MHL_ECBUS_PHY_BANK + 0xCE)
#define REG_MHL_ECBUS_PHY_68    (REG_MHL_ECBUS_PHY_BANK + 0xD0)
#define REG_MHL_ECBUS_PHY_69    (REG_MHL_ECBUS_PHY_BANK + 0xD2)
#define REG_MHL_ECBUS_PHY_6A    (REG_MHL_ECBUS_PHY_BANK + 0xD4)
#define REG_MHL_ECBUS_PHY_6B    (REG_MHL_ECBUS_PHY_BANK + 0xD6)
#define REG_MHL_ECBUS_PHY_6C    (REG_MHL_ECBUS_PHY_BANK + 0xD8)
#define REG_MHL_ECBUS_PHY_6D    (REG_MHL_ECBUS_PHY_BANK + 0xDA)
#define REG_MHL_ECBUS_PHY_6E    (REG_MHL_ECBUS_PHY_BANK + 0xDC)
#define REG_MHL_ECBUS_PHY_6F    (REG_MHL_ECBUS_PHY_BANK + 0xDE)
#define REG_MHL_ECBUS_PHY_70    (REG_MHL_ECBUS_PHY_BANK + 0xE0)
#define REG_MHL_ECBUS_PHY_71    (REG_MHL_ECBUS_PHY_BANK + 0xE2)
#define REG_MHL_ECBUS_PHY_72    (REG_MHL_ECBUS_PHY_BANK + 0xE4)
#define REG_MHL_ECBUS_PHY_73    (REG_MHL_ECBUS_PHY_BANK + 0xE6)
#define REG_MHL_ECBUS_PHY_74    (REG_MHL_ECBUS_PHY_BANK + 0xE8)
#define REG_MHL_ECBUS_PHY_75    (REG_MHL_ECBUS_PHY_BANK + 0xEA)
#define REG_MHL_ECBUS_PHY_76    (REG_MHL_ECBUS_PHY_BANK + 0xEC)
#define REG_MHL_ECBUS_PHY_77    (REG_MHL_ECBUS_PHY_BANK + 0xEE)
#define REG_MHL_ECBUS_PHY_78    (REG_MHL_ECBUS_PHY_BANK + 0xF0)
#define REG_MHL_ECBUS_PHY_79    (REG_MHL_ECBUS_PHY_BANK + 0xF2)
#define REG_MHL_ECBUS_PHY_7A    (REG_MHL_ECBUS_PHY_BANK + 0xF4)
#define REG_MHL_ECBUS_PHY_7B    (REG_MHL_ECBUS_PHY_BANK + 0xF6)
#define REG_MHL_ECBUS_PHY_7C    (REG_MHL_ECBUS_PHY_BANK + 0xF8)
#define REG_MHL_ECBUS_PHY_7D    (REG_MHL_ECBUS_PHY_BANK + 0xFA)
#define REG_MHL_ECBUS_PHY_7E    (REG_MHL_ECBUS_PHY_BANK + 0xFC)
#define REG_MHL_ECBUS_PHY_7F    (REG_MHL_ECBUS_PHY_BANK + 0xFE)


