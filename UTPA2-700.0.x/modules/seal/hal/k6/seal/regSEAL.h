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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regSEAL.h
/// @brief  SEAL Control Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SEAL_H_
#define _REG_SEAL_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define BITS_RANGE(range)                       (BIT(((1)?range)+1) - BIT((0)?range))
#define BITS_RANGE_VAL(x, range)                ((x & BITS_RANGE(range)) >> ((0)?range))

#define SEAL_SECURE0_RANGE0                     (0x23700)
#define SEAL_SECURE1_RANGE0                     (0x23800)
#define SEAL_TZPC_NONPM                         (0x23900)
#define SEAL_TZPC_PM                            (0x3900)
#define SEAL_TZPC_NONPM_MIU                     (0x22700)
#define SEAL_TZPC_NONPM_MIU1                    (0x22800)

#define RANGE_ADDR_OFFSET   (0x08)

// MIU0 secure range
#define REG_SECURE0_RANGE0_START_ADDR           (SEAL_SECURE0_RANGE0+0x00)
#define REG_SECURE0_RANGE0_END_ADDR             (SEAL_SECURE0_RANGE0+0x04)
#define REG_SECURE0_RANGE0_ATTRIBUTE            (SEAL_SECURE0_RANGE0+0x06)

#define REG_SECURE0_DETECT_ENABLE               (SEAL_SECURE0_RANGE0+0xEC)

// MIU1 secure range
#define REG_SECURE1_RANGE0_START_ADDR           (SEAL_SECURE1_RANGE0+0x00)
#define REG_SECURE1_RANGE0_END_ADDR             (SEAL_SECURE1_RANGE0+0x04)
#define REG_SECURE1_RANGE0_ATTRIBUTE            (SEAL_SECURE1_RANGE0+0x06)

#define REG_SECURE1_DETECT_ENABLE               (SEAL_SECURE1_RANGE0+0xEC)

#define SECURE_RANGE_ADDR_DYN_CH             (BIT15)

//Secure range hitted log
#define REG_SECURE0_HITTED_STATUS              (SEAL_SECURE0_RANGE0+0xE0)
#define REG_SECURE1_HITTED_STATUS              (SEAL_SECURE1_RANGE0+0xE0)
#define REG_SECURE0_HITTED_ADDR                (SEAL_SECURE0_RANGE0+0xE2)
#define REG_SECURE1_HITTED_ADDR                (SEAL_SECURE1_RANGE0+0xE2)
#define REG_SECURE0_HITTED_RANGE_ID            (SEAL_SECURE0_RANGE0+0xEE)
#define REG_SECURE1_HITTED_RANGE_ID            (SEAL_SECURE1_RANGE0+0xEE)
#define REG_SECURE_HITTED_LOG_CLR              (BIT0)
#define REG_SECURE_HITTED_IRQ_MASK             (BIT1)
#define REG_SECURE_HITTED_FALG                 (BIT2)
#define REG_SECURE_HITTED_IS_WRITE             (BIT15)
#define REG_SECURE_HITTED_CLIENT_ID            14:8
#define REG_SECURE_HITTED_RANGE_ID             4:0
#define GET_HIT_RANGE_ID(regval)               BITS_RANGE_VAL(regval, REG_SECURE_HITTED_RANGE_ID)
#define GET_HIT_CLIENT_ID(regval)              BITS_RANGE_VAL(regval, REG_SECURE_HITTED_CLIENT_ID)

//Secure range lock
#define REG_SECURE0_LOCK                       (SEAL_SECURE0_RANGE0+0xFE)
#define REG_SECURE1_LOCK                       (SEAL_SECURE1_RANGE0+0xFE)

//Non secure processor
#define REG_TZPC_NONSECURE_PROCESSOR           (SEAL_TZPC_NONPM+0x02)
#define REG_TZPC_NONPM_SECURE_SLAVE            (SEAL_TZPC_NONPM+0x20)
#define REG_TZPC_PM_SECURE_SLAVE               (SEAL_TZPC_PM+0x20)
#define REG_TZPC_NONSECURE_HEMCU               (SEAL_TZPC_NONPM+0x61)
#define REG_TZPC_PROTECT_CTL                   (SEAL_TZPC_NONPM+0x60)

//IMI secure range
#define REG_IMI_RANGE_START_ADDR               (SEAL_TZPC_NONPM+0xE0)
#define REG_IMI_RANGE_END_ADDR                 (SEAL_TZPC_NONPM+0xE4)

//Buffer lock
#define REG_TZPC_BUFFER_LOCK                   (SEAL_TZPC_NONPM+0xC8)

//Hitted adress offset
#define HITTED_ADDRESS_OFFSET                  (7)

//Mask control
#define REG_TZPC_MASK                          (SEAL_TZPC_NONPM+0xFC)
#define REG_TZPC_RESP_MASK_MIU                 (BIT1)
#define REG_TZPC_RESP_MASK_RIU                 (BIT2)
#define REG_TZPC_RESP_MASK_UNDEF               (BIT3)

//NS Sram control
#define REG_TZPC_MIU0_CTL                      (SEAL_TZPC_NONPM_MIU+0x24)
#define REG_TZPC_MIU1_CTL                      (SEAL_TZPC_NONPM_MIU1+0x24)
#define REG_TZPC_MIU_TOP_EN                    BIT0
#define REG_TZPC_MIU0_ID0                      (SEAL_TZPC_NONPM_MIU+0x38)
#define REG_TZPC_MIU1_ID0                      (SEAL_TZPC_NONPM_MIU1+0x38)
#define REG_TZPC_MIU_ID_ENABLE                 BIT12
#define REG_TZPC_MIU0_BASE_ADDR_LOW            (SEAL_TZPC_NONPM_MIU+0x2E)
#define REG_TZPC_MIU0_BASE_ADDR_HIGH           (SEAL_TZPC_NONPM_MIU+0x30)
#define REG_TZPC_MIU1_BASE_ADDR_LOW            (SEAL_TZPC_NONPM_MIU1+0x2E)
#define REG_TZPC_MIU1_BASE_ADDR_HIGH           (SEAL_TZPC_NONPM_MIU1+0x30)

#endif // _REG_SEAL_H_
