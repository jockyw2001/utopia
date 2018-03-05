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

#ifndef _MHAL_MBX_INTERRUPT_REG_H
#define _MHAL_MBX_INTERRUPT_REG_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros
//=============================================================================
#define RIU_MAP _u32RIUBaseAddrMBXINT

#define RIU     ((unsigned short volatile *) RIU_MAP)
#define RIU8    ((unsigned char  volatile *) RIU_MAP)

//#############################
//# host0: 51               (bank : 1019h)
//# host1: Secure R2        (bank : 1211h)
//# host2: ARM CA9 Core0    (bank : 1019h)
//# host3: ARM CA9 Core1    (bank : 1019h)
//#############################
#define REG_FIQ_MASK_BASE               (0xC80<<1)
#define FIQ_REG(address)                RIU[address*2+REG_FIQ_MASK_BASE]

#define REG_FIQ_H1_48_63                0x0047 //mask
#if 0
    // host3 to x
    #define INT_FIQMASK_H3_H2           BIT(0)
    #define INT_FIQMASK_H3_AEON         BIT(1)
    #define INT_FIQMASK_H3_PM           BIT(2)
#endif

#define REG_FIQ_H1_32_47                0x0046 //mask
    // host0 to x
    #define INT_FIQMASK_PM_H3           BIT(4)
    #define INT_FIQMASK_PM_H2           BIT(5)
    #define INT_FIQMASK_PM_AEON         BIT(6)
    // host1 to x
    #define INT_FIQMASK_AEON_H3         BIT(8)
    #define INT_FIQMASK_AEON_H2         BIT(9)
    #define INT_FIQMASK_AEON_PM         BIT(10)

    // host2 to x
    #define INT_FIQMASK_H2_H3           BIT(12)
    #define INT_FIQMASK_H2_AEON         BIT(13)
    #define INT_FIQMASK_H2_PM           BIT(14)

#define REG_FIQS_H1_32_47               0x004e //status
#define REG_FIQS_H1_48_63               0x004f //status


//#############################
//# host0: 51                       (bank : 1019h)
//# host1: ARM CA7 Core0    (bank : 1019h)
//# host2: Secure R2            (bank : 1211h)
//# host3: ARM CA7 Core1    (bank : 1019h)
//#############################
#define REG_CPU_INT_BASE                (0x2A0<<1)
#define CPU_INT_REG(address)            RIU[address*2+REG_CPU_INT_BASE]
#define REG_INT_PMFIRE                  0x0000 //PM51
    #define INT_PM_AEON                 BIT(1)
    #define INT_PM_H2                   BIT(0)
    #define INT_PM_H3                   BIT(2)

#define REG_INT_AEONFIRE                0x0002 //R2
    #define INT_AEON_PM                 BIT(0)
    #define INT_AEON_H2                 BIT(1)
    #define INT_AEON_H3                 BIT(2)

#define REG_INT_H2FIRE                  0x0004 //ARM CA7 Core 0
    #define INT_H2_PM                   BIT(0)
    #define INT_H2_AEON                 BIT(1)
    #define INT_H2_H3                   BIT(2)

#define REG_INT_H3FIRE                  0x0006 //ARM CA7 Core 1
    #define INT_H3_PM                   BIT(0)
    #define INT_H3_AEON                 BIT(1)
    #define INT_H3_H2                   BIT(2)

#endif //_MHAL_MBX_INTERRUPT_REG_H


