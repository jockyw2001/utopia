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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////

#ifndef _REGPM_H_
#define _REGPM_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
#define __BIT(x)    ((MS_U8)(1 << (x)))
#define __BIT0       __BIT(0)
#define __BIT1       __BIT(1)
#define __BIT2       __BIT(2)
#define __BIT3       __BIT(3)
#define __BIT4       __BIT(4)
#define __BIT5       __BIT(5)
#define __BIT6       __BIT(6)
#define __BIT7       __BIT(7)


//-----------------------------------------------------------------------------
// PM_SLEEP register
//-----------------------------------------------------------------------------
#define PM_REG_BASE                            (0x0700UL*2)
//                                              bank, regiter
#define REG_PM_DUMMY_ACTIVE_STANDBY       ((PM_REG_BASE + 0x3AUL*2+0))
#define REG_PM_DUMMY_POWERON_MODE         ((PM_REG_BASE + 0x37UL*2+1))
#define REG_PM_DUMMY_WAKEUP_SOURCE		  ((PM_REG_BASE + 0x34UL*2+1))
#define REG_PM_DUMMY_WAKEUP_KEY           ((PM_REG_BASE + 0x36UL*2+0))
#define REG_PM_DUMMY_RT51_STATUS		  ((PM_REG_BASE + 0x39UL*2+1))
#define REG_PM_CHIP_CFG_OVERWRITE         ((PM_REG_BASE + 0x1FUL*2+0))
	#define CHIP_CFG_MIPS_EN			  __BIT2
	#define CHIP_CFG_MIPS_VAL			  __BIT6
#define REG_PM_CPU_SW_RST                 ((PM_REG_BASE + 0x29UL*2+1))
	#define PM_51_SW_RST				  __BIT4

//-----------------------------------------------------------------------------
// PM_SLEEP RTC register
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------
// RTC Reg
//------------------------------------------------------------------------------
#define PMRTC_REG_BASE                            (0x0900UL*2)
//                                              bank, regiter


#define REG_PMRTC_CTRL            ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x00UL*2))//RTC_REG_BASE + 0x00

#define PMRTC_CTRL_NOT_RSTZ       (1 << 0)
#define PMRTC_CTRL_CNT_EN         (1 << 1)
#define PMRTC_CTRL_WRAP_EN        (1 << 2)
#define PMRTC_CTRL_LOAD_EN        (1 << 3)
#define PMRTC_CTRL_READ_EN        (1 << 4)
#define PMRTC_CTRL_INT_MASK       (1 << 5)
#define PMRTC_CTRL_INT_FORCE      (1 << 6)
#define PMRTC_CTRL_INT_CLEAR      (1 << 7)


#define REG_PMRTC_FREQ_CW         ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x01UL*2))//(RTC_REG_BASE + 0x01 * 2)
#define REG_PMRTC_LOAD_VAL        ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x03UL*2))//(RTC_REG_BASE + 0x03 * 2)
#define REG_PMRTC_MATCH_VAL       ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x05UL*2))//(RTC_REG_BASE + 0x05 * 2)
#define REG_PMRTC_INTERRUPT       ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x07UL*2))//(RTC_REG_BASE + 0x07 * 2)

#define PMRTC_INT_RAW_STATUS      (1 << 0)
#define PMRTC_INT_STATUS          (1 << 1)

#define REG_PMRTC_CNT             ((0x00UL*0x10000UL)+ (PMRTC_REG_BASE + 0x08UL*2))//(PMRTC_REG_BASE + 0x08 * 2)


//------------------------------------------------------------------------------
// RTC1 Reg
//------------------------------------------------------------------------------
#define PMRTC1_REG_BASE                            (0x0980UL*2)
//                                              bank, regiter


#define REG_PMRTC1_CTRL            ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x00UL*2))//RTC_REG_BASE + 0x00

#define PMRTC1_CTRL_NOT_RSTZ       (1 << 0)
#define PMRTC1_CTRL_CNT_EN         (1 << 1)
#define PMRTC1_CTRL_WRAP_EN        (1 << 2)
#define PMRTC1_CTRL_LOAD_EN        (1 << 3)
#define PMRTC1_CTRL_READ_EN        (1 << 4)
#define PMRTC1_CTRL_INT_MASK       (1 << 5)
#define PMRTC1_CTRL_INT_FORCE      (1 << 6)
#define PMRTC1_CTRL_INT_CLEAR      (1 << 7)


#define REG_PMRTC1_FREQ_CW         ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x01UL*2))//(RTC_REG_BASE + 0x01 * 2)
#define REG_PMRTC1_LOAD_VAL        ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x03UL*2))//(RTC_REG_BASE + 0x03 * 2)
#define REG_PMRTC1_MATCH_VAL       ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x05UL*2))//(RTC_REG_BASE + 0x05 * 2)
#define REG_PMRTC1_INTERRUPT       ((0x00UL*0x10000UL)+ (PMRTC1_REG_BASE + 0x07UL*2))//(RTC_REG_BASE + 0x07 * 2)

#define PMRTC1_INT_RAW_STATUS      (1 << 0)
#define PMRTC1_INT_STATUS          (1 << 1)

#define REG_PMRTC1_CNT             ((0x00UL*0x10000)+ (PMRTC1_REG_BASE + 0x08UL*2))//(PMRTC_REG_BASE + 0x08 * 2)




#endif  //_REGPM_H_
