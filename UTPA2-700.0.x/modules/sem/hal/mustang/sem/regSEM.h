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
#ifndef __REG_SEM_H__
#define __REG_SEM_H__

#define REG_SEM_MAX_NUM                         (8)
#define REG_SEM_BASE                            0x0C00

#define REG_SEM_ID0                             (REG_SEM_BASE+ 0x0000) // (SEM_REG_BASE+0x00) // 0xC00+ 0x00
#define REG_SEM_ID1                             (REG_SEM_BASE+ 0x0001) // (SEM_REG_BASE+0x02) // 0xC00+ 0x01
#define REG_SEM_ID2                             (REG_SEM_BASE+ 0x0002) // (SEM_REG_BASE+0x04) // 0xC00+ 0x02
#define REG_SEM_ID3                             (REG_SEM_BASE+ 0x0003) // (SEM_REG_BASE+0x06) // 0xC00+ 0x03
#define REG_SEM_ID4                             (REG_SEM_BASE+ 0x0004) // (SEM_REG_BASE+0x08) // 0xC00+ 0x04
#define REG_SEM_ID5                             (REG_SEM_BASE+ 0x0005) // (SEM_REG_BASE+0x0A) // 0xC00+ 0x05
#define REG_SEM_ID6                             (REG_SEM_BASE+ 0x0006) // (SEM_REG_BASE+0x0C) // 0xC00+ 0x06
#define REG_SEM_ID7                             (REG_SEM_BASE+ 0x0007) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID8                             (REG_SEM_BASE+ 0x0008) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID9                             (REG_SEM_BASE+ 0x0009) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID10                            (REG_SEM_BASE+ 0x000A) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID11                            (REG_SEM_BASE+ 0x000B) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID12                            (REG_SEM_BASE+ 0x000C) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID13                            (REG_SEM_BASE+ 0x000D) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID14                            (REG_SEM_BASE+ 0x000E) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07
#define REG_SEM_ID15                            (REG_SEM_BASE+ 0x000F) // (SEM_REG_BASE+0x0E) // 0xC00+ 0x07

#endif // #ifndef __REG_SEM_H__
