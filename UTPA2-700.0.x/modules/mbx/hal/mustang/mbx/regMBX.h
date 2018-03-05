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

///////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   regMBX.h
/// @brief  MStar Mailbox Driver Reg Def.
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em>For also support PM/big endian, the regs will only support byte operations \n
/// The short reg access will be removed! </em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MHAL_MBX_REG_H
#define _MHAL_MBX_REG_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros
//=============================================================================
#define RIU_MAP _u32RIUBaseAddrMBX

#define RIU8    ((unsigned char  volatile *) RIU_MAP)

#define REG_MBX_BASE                (0x19C0)
#define MBX_REG8(gid, addr)         RIU8[((gid+REG_MBX_BASE)<<2) + ((addr) * 2) - ((addr) & 1)]

//Reg8 defines:
#define REG8_MBX_CTRL               0x0000
    #define MBX_CTRL_FIRE             BIT(0)
    #define MBX_CTRL_READBACK         BIT(1)
    #define MBX_CTRL_INSTANT          BIT(2)
#define REG8_MBX_MAIL_CLASS          0x0001
#define REG8_MBX_MAIL_IDX            0x0002
#define REG8_MBX_PARAMETER_CNT       0x0003
#define REG8_MBX_PARAMETER_S         0x0004
#define REG8_MBX_PARAMETER_E         0x000D
#define REG8_MBX_STATE_0              0x000E
#define REG8_MBX_STATE_1              0x000F
    #define MBX_STATE1_DISABLED          BIT(4)
    #define MBX_STATE1_OVERFLOW           BIT(5)
    #define MBX_STATE1_ERROR              BIT(6)
    #define MBX_STATE1_BUSY               BIT(7)

#define REG_MBX_GROUP0      0x00
#define REG_MBX_GROUP1      0x08
#define REG_MBX_GROUP2      0x10
#define REG_MBX_GROUP3      0x18
#define REG_MBX_GROUP4      0x20
#define REG_MBX_GROUP5      0x28

#define REG8_MBX_GROUP(gid, addr) MBX_REG8(gid, addr)

#define REG8_MBX_MIPS(addr)        MBX_REG8(REG_MBX_GROUP1, addr) //group0 allocated for mips
#define REG8_MBX_AEON(addr)        MBX_REG8(REG_MBX_GROUP0, addr) //group1 allocated for aeon
#define REG8_MBX_PM(addr)          MBX_REG8(REG_MBX_GROUP3, addr) //group3 allocated for pm

#endif //__MHAL_MBX_REG_H



