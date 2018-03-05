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

///////////////////////////////////////////////////////
// Register bank
///////////////////////////////////////////////////////
#define REG_MHL_TMDS_BASE    0x122700
#define REG_MHL_CBUS_BANK    0x001F00
#define REG_PM_MHL_CBUS_BANK    0x002F00


// MHL CBUS Bank - CBUS logic control
#define REG_MHL_CBUS_01    (REG_MHL_CBUS_BANK + 0x02)
#define REG_MHL_CBUS_02    (REG_MHL_CBUS_BANK + 0x04)
#define REG_MHL_CBUS_03    (REG_MHL_CBUS_BANK + 0x06)
#define REG_MHL_CBUS_04    (REG_MHL_CBUS_BANK + 0x08)
#define REG_MHL_CBUS_05    (REG_MHL_CBUS_BANK + 0x0A)
#define REG_MHL_CBUS_06    (REG_MHL_CBUS_BANK + 0x0C)
#define REG_MHL_CBUS_07    (REG_MHL_CBUS_BANK + 0x0E)
#define REG_MHL_CBUS_08    (REG_MHL_CBUS_BANK + 0x10)
#define REG_MHL_CBUS_14    (REG_MHL_CBUS_BANK + 0x28)
#define REG_MHL_CBUS_18    (REG_MHL_CBUS_BANK + 0x30)
#define REG_MHL_CBUS_19    (REG_MHL_CBUS_BANK + 0x32)
#define REG_MHL_CBUS_1B    (REG_MHL_CBUS_BANK + 0x36)
#define REG_MHL_CBUS_1F    (REG_MHL_CBUS_BANK + 0x3E)
#define REG_MHL_CBUS_21    (REG_MHL_CBUS_BANK + 0x42)
#define REG_MHL_CBUS_22    (REG_MHL_CBUS_BANK + 0x44)
#define REG_MHL_CBUS_23    (REG_MHL_CBUS_BANK + 0x46)
#define REG_MHL_CBUS_24    (REG_MHL_CBUS_BANK + 0x48)
#define REG_MHL_CBUS_25    (REG_MHL_CBUS_BANK + 0x4A)
#define REG_MHL_CBUS_26    (REG_MHL_CBUS_BANK + 0x4C)
#define REG_MHL_CBUS_3A    (REG_MHL_CBUS_BANK + 0x74)
#define REG_MHL_CBUS_3B    (REG_MHL_CBUS_BANK + 0x76)
#define REG_MHL_CBUS_3C    (REG_MHL_CBUS_BANK + 0x78)
#define REG_MHL_CBUS_3D    (REG_MHL_CBUS_BANK + 0x7A)
#define REG_MHL_CBUS_52    (REG_MHL_CBUS_BANK + 0xA4)
#define REG_MHL_CBUS_53    (REG_MHL_CBUS_BANK + 0xA6)
#define REG_MHL_CBUS_55    (REG_MHL_CBUS_BANK + 0xAA)
#define REG_MHL_CBUS_57    (REG_MHL_CBUS_BANK + 0xAE)
#define REG_MHL_CBUS_58    (REG_MHL_CBUS_BANK + 0xB0)
#define REG_MHL_CBUS_5D    (REG_MHL_CBUS_BANK + 0xBA)
#define REG_MHL_CBUS_63    (REG_MHL_CBUS_BANK + 0xC6)
#define REG_MHL_CBUS_65    (REG_MHL_CBUS_BANK + 0xCA)
#define REG_MHL_CBUS_70    (REG_MHL_CBUS_BANK + 0xE0)
#define REG_MHL_CBUS_71    (REG_MHL_CBUS_BANK + 0xE2)
#define REG_MHL_CBUS_78    (REG_MHL_CBUS_BANK + 0xF0)


// PM MHL CBUS Bank - CBUS PHY control
#define REG_PM_MHL_CBUS_00    (REG_PM_MHL_CBUS_BANK + 0x00)
#define REG_PM_MHL_CBUS_0C    (REG_PM_MHL_CBUS_BANK + 0x18)
#define REG_PM_MHL_CBUS_0D    (REG_PM_MHL_CBUS_BANK + 0x1A)
#define REG_PM_MHL_CBUS_16    (REG_PM_MHL_CBUS_BANK + 0x2C)
#define REG_PM_MHL_CBUS_17    (REG_PM_MHL_CBUS_BANK + 0x2E)
#define REG_PM_MHL_CBUS_18    (REG_PM_MHL_CBUS_BANK + 0x30)
#define REG_PM_MHL_CBUS_20    (REG_PM_MHL_CBUS_BANK + 0x40)
#define REG_PM_MHL_CBUS_21    (REG_PM_MHL_CBUS_BANK + 0x42)
#define REG_PM_MHL_CBUS_30    (REG_PM_MHL_CBUS_BANK + 0x60)
#define REG_PM_MHL_CBUS_31    (REG_PM_MHL_CBUS_BANK + 0x62)
#define REG_PM_MHL_CBUS_38    (REG_PM_MHL_CBUS_BANK + 0x70)

