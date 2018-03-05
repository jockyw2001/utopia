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

#ifndef _REGPWS_H_
#define _REGPWS_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define BIT0                            (0x0001)
#define BIT1                            (0x0002)
#define BIT2                            (0x0004)
#define BIT3                            (0x0008)
#define BIT4                            (0x0010)
#define BIT5                            (0x0020)
#define BIT6                            (0x0040)
#define BIT7                            (0x0080)
#define BIT8                            (0x0100)
#define BIT9                            (0x0200)
#define BIT10                           (0x0400)
#define BIT11                           (0x0800)
#define BIT12                           (0x1000)
#define BIT13                           (0x2000)
#define BIT14                           (0x4000)
#define BIT15                           (0x8000)

#define PM_OFF_FLAG                     (0x0E68)
#define CHIPTOP(x)                      (0x1E00 + x)

//PM
#define PM_ADC_RGB                      (0x2508)
#define PM_ADC_Y                        (0x2509)
#define PM_GMC                          (0x250A)
#define PM_CVBS_BUF_OUT                 (0x2570)
#define PM_DAC_CVBS                     (0x258C)
#define PM_ADC_VCTRL_L                  (0x2534)
#define PM_ADC_VCTRL_H                  (0x2535)
#define PM_ADC_ICTRL_RGB_L              (0x2536)
#define PM_ADC_ICTRL_RGB_H              (0x2537)
#define PM_ADC_VTRL_YC_L                (0x253A)
#define PM_ADC_VTRL_YC_H                (0x253B)
#define PM_ADC_ICTRL_YC_L               (0x253C)
#define PM_ADC_ICTRL_YC_H               (0x253D)

//H264
#define SVD_REG_BASE                    (0x1B00)  // 0x1B00 - 0x1BFF
#define REG_SVD_SW_RST                  (SVD_REG_BASE + 0x0A * 2)
    #define REG_SVD_RESET_SWRST         BIT0
    #define REG_SVD_RESET_CPURST        BIT1
    #define REG_SVD_RESET_SWRST_FIN     BIT2
    #define REG_SVD_RESET_CPURST_FIN    BIT3

#define MVD_CTRL                        (0x1100)
    #define MVD_CTRL_RST                BIT0
    #define MVD_CTRL_INIT               BIT2

#endif
