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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLKGEN_REG_S4LE_H__
#define __CLKGEN_REG_S4LE_H__

#define REG_CKG_USB30           0x1E24
    #define CKG_USB30_GATED         BIT0
    #define CKG_USB30_INVERT        BIT1

#define REG_CKG_DOT_MINI        0x1E25
    #define CKG_DOT_MINI_GATED      BIT0
    #define CKG_DOT_MINI_INVERT     BIT1
    #define CKG_DOT_MINI_MASK       (BIT3 | BIT2)
    #define CKG_DOT_MINI_CLK_LPLL   (0 << 2)
    #define CKG_DOT_MINI_FIFO_CLK   (1 << 2)
    #define CKG_DOT_MINI_DFT_LIVE   (3 << 2)

#define REG_CKG_MIU             0x1E25
    #define CKG_MIU_GATED           BIT4
    #define CKG_MIU_INVERT          BIT5
    #define CKG_MIU_MASK            (BIT7 | BIT6)
#define REG_CKG_MIU_4           0x1E46
    #define CKG_MIU_4_MASK          BIT1
    #define CKG_MIU_MEMPLL          (0 << 6)
    #define CKG_MIU_170MHZ          (1 << 6)
    #define CKG_MIU_MEMPLL_DIV2     (2 << 6)
    #define CKG_MIU_216MHZ          (3 << 6)
    #define CKG_MIU_SCPLL           (4 << 6)
    #define CKG_MIU_192MHZ          (5 << 6)

#define REG_CKG_TS0             0x1E26
    #define CKG_TS0_GATED           BIT0
    #define CKG_TS0_INVERT          BIT1
    #define CKG_TS0_MASK            (BIT3 | BIT2)
    #define CKG_TS0_TS0_CLK         (0 << 2)
    #define CKG_TS0_TS2_CLK         (1 << 2)
    #define CKG_TS0_0               (2 << 2)
    #define CKG_TS0_XTAL            (3 << 2)

#define REG_CKG_TCK             0x1E26
    #define CKG_TCK_GATED           BIT4
    #define CKG_TCK_INVERT          BIT5

#define REG_CKG_AEON            0x1E26
    #define CKG_AEON_GATED          BIT6
    #define CKG_AEON_INVERT         BIT7

#define REG_CKG_TSP             0x1E27
    #define CKG_TSP_GATED           BIT0
    #define CKG_TSP_INVERT          BIT1
    #define CKG_TSP_MASK            (BIT4 | BIT3 | BIT2)
    #define CKG_TSP_144MHZ          (0 << 2)
    #define CKG_TSP_123MHZ          (1 << 2)
    #define CKG_TSP_72MHZ           (2 << 2)
    #define CKG_TSP_XTAL            (7 << 2)

#define REG_CKG_STC0            0x1E27
    #define CKG_STC0_GATED          BIT4
    #define CKG_STC0_INVERT         BIT5
    #define CKG_STC0_MASK           (BIT7 | BIT6)
    #define CKG_STC0_STC0_SYNTH     (0 << 6)
    #define CKG_STC0_1              (1 << 6)
    #define CKG_STC0_27MHZ          (2 << 6)
    #define CKG_STC0_XTAL           (3 << 6)

#define REG_CKG_MAD_STC         0x1E28
    #define CKG_MAD_STC_GATED       BIT4
    #define CKG_MAD_STC_INVERT      BIT5
    #define CKG_MAD_STC_MASK        (BIT7 | BIT6)
    #define CKG_MAD_STC_STC0_SYNTH  (0 << 6)
    #define CKG_MAD_STC_1           (1 << 6)
    #define CKG_MAD_STC_27MHZ       (2 << 6)
    #define CKG_MAD_STC_XTAL        (3 << 6)

#define REG_MVD_BOOT            0x1E29
    #define CKG_MVD_BOOT_GATED      BIT0
    #define CKG_MVD_BOOT_INVERT     BIT1
    #define CKG_MVD_BOOT_MASK       (BIT3 | BIT2)
    #define CKG_MVD_BOOT_144MHZ     (0 << 2)
    #define CKG_MVD_BOOT_123MHZ     (1 << 2)
    #define CKG_MVD_BOOT_CLK_MIU    (2 << 2)
    #define CKG_MVD_BOOT_XTAL       (3 << 2)

#define REG_CKG_MVD             0x1E29
    #define CKG_MVD_GATED           BIT4
    #define CKG_MVD_INVERT          BIT5
    #define CKG_MVD_MASK            (BIT7 | BIT6)
    #define CKG_MVD_144MHZ          (0 << 6)
    #define CKG_MVD_123MHZ          (1 << 6)
    #define CKG_MVD_CLK_MIU         (2 << 6)
    #define CKG_MVD_XTAL            (3 << 6)

#define REG_CKG_MVD_IAP_RMEM    0x1E2A
    #define CKG_MVD_IAP_RMEM_GATED  BIT0
    #define CKG_MVD_IAP_RMEM_INVERT BIT1
    #define CKG_MVD_IAP_RMEM_MASK   (BIT3 | BIT2)
    #define CKG_MVD_IAP_RMEM_CLK_MVD_P  (0 << 2)
    #define CKG_MVD_IAP_RMEM_CLK_MIU_P  (1 << 2)
    #define CKG_MVD_IAP_RMEM_XTAL       (3 << 2)

#define REG_CKG_DC0             0x1E2A
    #define CKG_DC0_GATED           BIT4
    #define CKG_DC0_INVERT          BIT5
    #define CKG_DC0_MASK            (BIT7 | BIT6)
#define REG_CKG_DC0_3           0x1E46
    #define CKG_DC0_3_MASK          BIT0
    #define CKG_DC0_SYNCHRONOUS     (0 << 6)
    #define CKG_DC0_FREERUN         (1 << 6)
    #define CKG_DC0_27MHZ           (2 << 6)
    #define CKG_DC0_54MHZ           (3 << 6)
    #define CKG_DC0_72MHZ           (4 << 6)
    #define CKG_DC0_86MHZ           (5 << 6)
    #define CKG_DC0_108MHZ          (6 << 6)
    #define CKG_DC0_144MHZ          (7 << 6)

#define REG_CKG_RVD             0x1E2B
    #define CKG_RVD_GATED           BIT0
    #define CKG_RVD_INVERT          BIT1
    #define CKG_RVD_MASK            (BIT3 | BIT2)
    #define CKG_RVD_108MHZ          (0 << 2)
    #define CKG_RVD_86MHZ           (1 << 2)
    #define CKG_RVD_72MHZ           (2 << 2)
    #define CKG_RVD_XTAL            (3 << 2)

#define REG_CKG_GE              0x1E2B
    #define CKG_GE_GATED            BIT4
    #define CKG_GE_INVERT           BIT5
    #define CKG_GE_MASK             (BIT7 | BIT6)
    #define CKG_GE_170MHZ           (0 << 6)
    #define CKG_GE_123MHZ           (1 << 6)
    #define CKG_GE_86MHZ            (2 << 6)
    #define CKG_GE_144MHZ           (3 << 6)

#define REG_CKG_GOPG0           0x1E2C
    #define CKG_GOPG0_GATED         BIT0
    #define CKG_GOPG0_INVERT        BIT1
    #define CKG_GOPG0_MASK          (BIT3 | BIT2)
    #define CKG_GOPG0_ODCLK         (0 << 2)
    #define CKG_GOPG0_0             (1 << 2)
    #define CKG_GOPG0_IDCLK2        (2 << 2)
    #define CKG_GOPG0_XTAL          (3 << 2)

#define REG_CKG_GOPG1           0x1E2C
    #define CKG_GOPG1_GATED         BIT4
    #define CKG_GOPG1_INVERT        BIT5
    #define CKG_GOPG1_MASK          (BIT7 | BIT6)
    #define CKG_GOPG1_ODCLK         (0 << 6)
    #define CKG_GOPG1_0             (1 << 6)
    #define CKG_GOPG1_IDCLK2        (2 << 6)
    #define CKG_GOPG1_XTAL          (3 << 6)

#define REG_CKG_GOPD            0x1E2D
    #define CKG_GOPD_GATED          BIT0
    #define CKG_GOPD_INVERT         BIT1
    #define CKG_GOPD_MASK           (BIT3 | BIT2)
    #define CKG_GOPD_CLK_ADC        (0 << 2)
    #define CKG_GOPD_CLK_ODCLK      (1 << 2)
    #define CKG_GOPD_CLK_DC0        (2 << 2)
    #define CKG_GOPD_XTAL           (3 << 2)

#define REG_CKG_VD              0x1E2D
    #define CKG_VD_GATED            BIT4
    #define CKG_VD_INVERT           BIT5
    #define CKG_VD_MASK             (BIT7 | BIT6)
    #define CKG_VD_CLK_VD           (0 << 6)
    #define CKG_VD_CLK_VD_          (1 << 6)
    #define CKG_VD_TESTMODE_CLK     (2 << 6)
    #define CKG_VD_XTAL             (3 << 6)

#define REG_CKG_VDMCU           0x1E2E
    #define CKG_VDMCU_GATED         BIT0
    #define CKG_VDMCU_INVERT        BIT1
    #define CKG_VDMCU_MASK          (BIT3 | BIT2)
    #define CKG_VDMCU_108MHZ        (0 << 2)
    #define CKG_VDMCU_86MHZ         (1 << 2)
    #define CKG_VDMCU_54MHZ         (2 << 2)
    #define CKG_VDMCU_XTAL          (3 << 2)

#define REG_CKG_VD200           0x1E2E
    #define CKG_VD200_GATED         BIT4
    #define CKG_VD200_INVERT        BIT5
    #define CKG_VD200_MASK          (BIT7 | BIT6)
    #define CKG_VD200_216MHZ        (0 << 6)
    #define CKG_VD200_216MHZ_       (1 << 6)
    #define CKG_VD200_216MHZ__      (2 << 6)
    #define CKG_VD200_XTAL          (3 << 6)

#define REG_CKG_FICLK_F2        0x1E30
    #define CKG_FICLK_F2_GATED      BIT4
    #define CKG_FICLK_F2_INVERT     BIT5
    #define CKG_FICLK_F2_MASK       (BIT7 | BIT6)
    #define CKG_FICLK_F2_CLK_IDCLK2 (0 << 6)
    #define CKG_FICLK_F2_CLK_FCLK   (1 << 6)
    #define CKG_FICLK_F2_0          (2 << 6)
    #define CKG_FICLK_F2_XTAL       (3 << 6)

#define REG_CKG_GOPG2           0x1E31
    #define CKG_GOPG2_GATED         BIT0
    #define CKG_GOPG2_INVERT        BIT1
    #define CKG_GOPG2_MASK          (BIT3 | BIT2)
    #define CKG_GOPG2_CLK_ODCLK     (0 << 2)
    #define CKG_GOPG2_0             (1 << 2)
    #define CKG_GOPG2_CLK_IDCLK2    (2 << 2)
    #define CKG_GOPG2_XTAL          (3 << 2)

#define REG_CKG_PCM             0x1E31
    #define CKG_PCM_GATED           BIT4
    #define CKG_PCM_INVERT          BIT5
    #define CKG_PCM_MASK            (BIT7 | BIT6)
    #define CKG_PCM_27MHZ           (0 << 6)
    #define CKG_PCM_27MHZ_          (1 << 6)
    #define CKG_PCM_XTAL            (2 << 6)
    #define CKG_PCM_XTAL_           (3 << 6)

#define REG_CKG_VE              0x1E33
    #define CKG_VE_GATED            BIT0
    #define CKG_VE_INVERT           BIT1
    #define CKG_VE_MASK             (BIT3 | BIT2)
    #define CKG_VE_27MHZ            (0 << 2)
    #define CKG_VE_27MHZ_           (1 << 2)
    #define CKG_VE_XTAL             (2 << 2)
    #define CKG_VE_XTAL_            (3 << 2)

#define REG_CKG_VEDAC           0x1E33
    #define CKG_VEDAC_GATED         BIT4
    #define CKG_VEDAC_INVERT        BIT5
    #define CKG_VEDAC_MASK          (BIT7 | BIT6)
    #define CKG_VEDAC_27MHZ         (0 << 6)
    #define CKG_VEDAC_54MHZ         (1 << 6)
    #define CKG_VEDAC_108MHZ        (2 << 6)
    #define CKG_VEDAC_DFT_LIVE      (3 << 6)

#define REG_CKG_FODCLK          0x1E34
    #define CKG_FODCLK_GATED        BIT0
    #define CKG_FODCLK_INVERT       BIT1
    #define CKG_FODCLK_MASK         (BIT3 | BIT2)
    #define CKG_FODCLK_FCLK_P       (0 << 2)
    #define CKG_FODCLK_ODCLK_P      (1 << 2)
    #define CKG_FODCLK_XTAL         (3 << 2)

#define REG_CKG_FCLK            0x1E35
    #define CKG_FCLK_GATED          BIT0
    #define CKG_FCLK_INVERT         BIT1
    #define CKG_FCLK_MASK           (BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_FCLK_170MHZ         (0 << 2)
    #define CKG_FCLK_CLK_MIU        (1 << 2)
    #define CKG_FCLK_CLK_ODCLK      (2 << 2)
    #define CKG_FCLK_216MHZ         (3 << 2)
    #define CKG_FCLK_CLK_IDCLK2     (4 << 2)
    #define CKG_FCLK_SCPLL          (5 << 2)
    #define CKG_FCLK_0              (6 << 2)
    #define CKG_FCLK_XTAL           (7 << 2)
    #define CKG_FCLK_XTAL_          (8 << 2)

#define REG_CKG_CPUM            0x1E36
    #define CKG_CPUM_GATED          BIT0

#define REG_CKG_ODCLK           0x1E37
    #define CKG_ODCLK_GATED         BIT0
    #define CKG_ODCLK_INVERT        BIT1
    #define CKG_ODCLK_MASK          (BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_ODCLK_CLK_ADC       (0 << 2)
    #define CKG_ODCLK_CLK_DVI       (1 << 2)
    #define CKG_ODCLK_CLK_VD        (2 << 2)
    #define CKG_ODCLK_CLK_MPEG0     (3 << 2)
    #define CKG_ODCLK_1             (4 << 2)
    #define CKG_ODCLK_CLK_EXT_DI    (5 << 2)
    #define CKG_ODCLK_XTAL          (6 << 2)
    #define CKG_ODCLK_CLK_LPLL      (7 << 2)
    #define CKG_ODCLK_XTAL_         (8 << 2)

#define REG_CKG_VE_IN           0x1E38
    #define CKG_VE_IN_GATED         BIT0
    #define CKG_VE_IN_INVERT        BIT1
    #define CKG_VE_IN_MASK          (BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_VE_IN_CLK_ADC       (0 << 2)
    #define CKG_VE_IN_CLK_DVI       (1 << 2)
    #define CKG_VE_IN_CLK_VD        (2 << 2)
    #define CKG_VE_IN_CLK_MPEG0     (3 << 2)
    #define CKG_VE_IN_1             (4 << 2)
    #define CKG_VE_IN_CLK_EXT_DI    (5 << 2)
    #define CKG_VE_IN_0             (6 << 2)
    #define CKG_VE_IN_0_            (7 << 2)
    #define CKG_VE_IN_DFT_LIVE      (8 << 2)

#define REG_CKG_NFIE            0x1E39
    #define CKG_NFIE_GATED          BIT0
    #define CKG_NFIE_INVERT         BIT1
    #define CKG_NFIE_MASK           (BIT4 | BIT3 | BIT2)
    #define CKG_NFIE_62MHZ          (0 << 2)
    #define CKG_NFIE_54MHZ          (1 << 2)
    #define CKG_NFIE_43MHZ          (2 << 2)
    #define CKG_NFIE_36MHZ          (3 << 2)
    #define CKG_NFIE_27MHZ          (4 << 2)
    #define CKG_NFIE_18MHZ          (5 << 2)
    #define CKG_NFIE_13MHZ          (6 << 2)
    #define CKG_NFIE_XTAL           (7 << 2)

#define REG_CKG_TS2             0x1E3A
    #define CKG_TS2_GATED           BIT0
    #define CKG_TS2_INVERT          BIT1
    #define CKG_TS2_MASK            (BIT3 | BIT2)
    #define CKG_TS2_TS2_CLK         (0 << 2)
    #define CKG_TS2_0               (1 << 2)
    #define CKG_TS2_0_              (2 << 2)
    #define CKG_TS2_XTAL            (3 << 2)

#define REG_CKG_TSOUT           0x1E3A
    #define CKG_TSOUT_GATED         BIT4
    #define CKG_TSOUT_INVERT        BIT5
    #define CKG_TSOUT_MASK          (BIT7 | BIT6)
    #define CKG_TSOUT_27MHZ         (0 << 6)
    #define CKG_TSOUT_36MHZ         (1 << 6)
    #define CKG_TSOUT_43MHZ         (2 << 6)
    #define CKG_TSOUT_XTAL          (3 << 6)

#define REG_CKG_UART            0x1E3C
    #define CKG_UART_GATED          BIT4        // FIXME
    #define CKG_UART_INVERT         BIT5
    #define CKG_UART_MASK           (BIT7 | BIT6)
#define REG_CKG_UART_4          0x1E46
    #define CKG_UART_4_MASK         BIT2
    #define CKG_UART_170MHZ         (0 << 6)
    #define CKG_UART_160MHZ         (1 << 6)
    #define CKG_UART_144MHZ         (2 << 6)
    #define CKG_UART_123MHZ         (3 << 6)
    #define CKG_UART_108MHZ         (4 << 6)
    #define CKG_UART_MEMPLL         (5 << 6)
    #define CKG_UART_MEMPLL_DIV2    (6 << 6)

#define REG_CKG_DC1             0x1E3D
    #define CKG_DC1_GATED           BIT0
    #define CKG_DC1_INVERT          BIT1
    #define CKG_DC1_MASK            (BIT3 | BIT2)
    #define CKG_DC1_SYNCHRONOUS     (0 << 2)
    #define CKG_DC1_FREERUN         (1 << 2)
    #define CKG_DC1_27MHZ           (2 << 2)
    #define CKG_DC1_DFT_LIVE        (3 << 2)

#define REG_CKG_IDCLK1          0x1E3E
    #define CKG_IDCLK1_GATED        BIT0
    #define CKG_IDCLK1_INVERT       BIT1
    #define CKG_IDCLK1_MASK         (BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_IDCLK1_CLK_ADC      (0 << 2)
    #define CKG_IDCLK1_CLK_DVI      (1 << 2)
    #define CKG_IDCLK1_CLK_VD       (2 << 2)
    #define CKG_IDCLK1_CLK_DC0      (3 << 2)
    #define CKG_IDCLK1_1            (4 << 2)
    #define CKG_IDCLK1_CLK_EXT_DI   (5 << 2)
    #define CKG_IDCLK1_CLK_VD_ADC   (6 << 2)
    #define CKG_IDCLK1_0            (7 << 2)
    #define CKG_IDCLK1_XTAL         (8 << 2)

#define REG_CKG_IDCLK2          0x1E3F
    #define CKG_IDCLK2_GATED        BIT0
    #define CKG_IDCLK2_INVERT       BIT1
    #define CKG_IDCLK2_MASK         (BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_IDCLK2_CLK_ADC      (0 << 2)
    #define CKG_IDCLK2_CLK_DVI      (1 << 2)
    #define CKG_IDCLK2_CLK_VD       (2 << 2)
    #define CKG_IDCLK2_CLK_DC0      (3 << 2)
    #define CKG_IDCLK2_1            (4 << 2)
    #define CKG_IDCLK2_CLK_EXT_DI   (5 << 2)
    #define CKG_IDCLK2_CLK_VD_ADC   (6 << 2)
    #define CKG_IDCLK2_0            (7 << 2)
    #define CKG_IDCLK2_XTAL         (8 << 2)

#define REG_CKG_STRLD           0x1E44
    #define CKG_STRLD_GATED         BIT0
    #define CKG_STRLD_INVERT        BIT1
    #define CKG_STRLD_MASK          (BIT3 | BIT2)
    #define CKG_STRLD_144MHZ        (0 << 2)
    #define CKG_STRLD_123MHZ        (1 << 2)
    #define CKG_STRLD_108MHZ        (2 << 2)
    #define CKG_STRLD_XTAL          (3 << 2)

#define REG_CKG_MCU             0x1E45
    #define CKG_MCU_GATED           BIT0
    #define CKG_MCU_INVERT          BIT1
    #define CKG_MCU_MASK            (BIT4 | BIT3 | BIT2)
    #define CKG_MCU_170MHZ          (0 << 2)
    #define CKG_MCU_160MHZ          (1 << 2)
    #define CKG_MCU_144MHZ          (2 << 2)
    #define CKG_MCU_123MHZ          (3 << 2)
    #define CKG_MCU_108MHZ          (4 << 2)
    #define CKG_MCU_CLK_MIU         (5 << 2)
    #define CKG_MCU_CLK_MIU_DIV2    (6 << 2)
    #define CKG_MCU_0               (7 << 2)

#define REG_CKG_SVD             0x1E58
    #define CKG_SVD_GATED           BIT0
    #define CKG_SVD_INVERT          BIT1
    #define CKG_SVD_MASK            BITMASK(6:2)
    #define CKG_SVD_240MHZ          (0 << 2)
    #define CKG_SVD_216MHZ          (1 << 2)
    #define CKG_SVD_CLK_MVD_P       (2 << 2)
    #define CKG_SVD_CLK_RVD_P       (3 << 2)
    #define CKG_SVD_CLK_MIU         (8 << 2)
    #define CKG_SVD_XTAL            (18 << 2)

#define REG_CKG_PSRAM0          0x1E5A
    #define CKG_PSRAM0_GATED        BIT0
    #define CKG_PSRAM0_INVERT       BIT1

#define REG_CKG_PSRAM1          0x1E5A
    #define CKG_PSRAM1_GATED        BIT2
    #define CKG_PSRAM1_INVERT       BIT3

#define REG_CKG_JPD             0x1E9A
    #define CKG_JPD_GATED           BIT0
    #define CKG_JPD_INVERT          BIT1
    #define CKG_JPD_MASK            (BIT3 | BIT2)
    #define CKG_JPD_72MHZ           (0 << 2)
    #define CKG_JPD_108MHZ          (1 << 2)
    #define CKG_JPD_123MHZ          (2 << 2)

#define REG_CKG_VDS             REG_CKG_VDMCU
#define REG_CKG_VDGOPD          REG_CKG_VD
#define REG_CKG_AEONTS0         REG_CKG_AEON
#define REG_CKG_STC0TSP         REG_CKG_TSP
#define REG_CKG_VEIN            REG_CKG_VE_IN
#define REG_CKG_AEON1DC0        REG_CKG_DC0

#endif /* __CLKGEN_REG_S4LE_H__ */

