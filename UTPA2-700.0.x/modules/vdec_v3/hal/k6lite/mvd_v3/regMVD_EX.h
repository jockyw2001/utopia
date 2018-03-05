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
//
/// @file  regMVD.h
/// @brief Hardware register definition for Video Decoder
/// @author MStar Semiconductor Inc.
//
///////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_MVD_H_
#define _REG_MVD_H_


////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Base Address
//------------------------------------------------------------------------------
#define MVD_REG_BASE                            0x1100UL  // 0x1100 - 0x11FF
#define CHIP_REG_BASE                           0x1E00UL  // 0x1E00 - 0x1EFF

#define MIU0_REG_BASE                           0x1200UL
#define MIU1_REG_BASE                           0x0600UL
#define MIU2_REG_BASE                           0x62000UL

#define MIU0_REG_BASE2                          0x61500UL
#define MIU1_REG_BASE2                          0x62200UL
#define MIU2_REG_BASE2                          0x62300UL

//------------------------------------------------------------------------------
// MIU register
//------------------------------------------------------------------------------
//MIU request mask
#define MIU0_RQ0_MASK_L                 (MIU0_REG_BASE + 0x23*2)
#define MIU0_RQ0_MASK_H                 (MIU0_REG_BASE + 0x23*2 +1)
#define MIU0_RQ1_MASK_L                 (MIU0_REG_BASE + 0x33*2)
#define MIU0_RQ1_MASK_H                 (MIU0_REG_BASE + 0x33*2 +1)
#define MIU0_RQ2_MASK_L                 (MIU0_REG_BASE + 0x43*2)
#define MIU0_RQ2_MASK_H                 (MIU0_REG_BASE + 0x43*2 +1)
#define MIU0_RQ3_MASK_L                 (MIU0_REG_BASE + 0x53*2)
#define MIU0_RQ3_MASK_H                 (MIU0_REG_BASE + 0x53*2 +1)
#define MIU0_RQ4_MASK_L                 (MIU0_REG_BASE2 + 0x0003*2)
#define MIU0_RQ4_MASK_H                 (MIU0_REG_BASE2 + 0x0003*2 +1)
#define MIU0_SEL0_L                     (MIU0_REG_BASE + 0xF0)
#define MIU0_SEL0_H                     (MIU0_REG_BASE + 0xF1)
#define MIU0_SEL2_L                     (MIU0_REG_BASE + 0xF4)
#define MIU0_SEL2_H                     (MIU0_REG_BASE + 0xF5)
#define MIU0_SEL3_L                     (MIU0_REG_BASE + 0xF6)
#define MIU0_SEL3_H                     (MIU0_REG_BASE + 0xF7)

#define MIU1_RQ0_MASK_L                 (MIU1_REG_BASE + 0x23*2)
#define MIU1_RQ0_MASK_H                 (MIU1_REG_BASE + 0x23*2 +1)
#define MIU1_RQ1_MASK_L                 (MIU1_REG_BASE + 0x33*2)
#define MIU1_RQ1_MASK_H                 (MIU1_REG_BASE + 0x33*2 +1)
#define MIU1_RQ2_MASK_L                 (MIU1_REG_BASE + 0x43*2)
#define MIU1_RQ2_MASK_H                 (MIU1_REG_BASE + 0x43*2 +1)
#define MIU1_RQ3_MASK_L                 (MIU1_REG_BASE + 0x53*2)
#define MIU1_RQ3_MASK_H                 (MIU1_REG_BASE + 0x53*2 +1)
#define MIU1_RQ4_MASK_L                 (MIU1_REG_BASE2 + 0x0003*2)
#define MIU1_RQ4_MASK_H                 (MIU1_REG_BASE2 + 0x0003*2 +1)

#define MIU2_RQ0_MASK_L                 (MIU2_REG_BASE + 0x23*2)
#define MIU2_RQ0_MASK_H                 (MIU2_REG_BASE + 0x23*2 +1)
#define MIU2_RQ1_MASK_L                 (MIU2_REG_BASE + 0x33*2)
#define MIU2_RQ1_MASK_H                 (MIU2_REG_BASE + 0x33*2 +1)
#define MIU2_RQ2_MASK_L                 (MIU2_REG_BASE + 0x43*2)
#define MIU2_RQ2_MASK_H                 (MIU2_REG_BASE + 0x43*2 +1)
#define MIU2_RQ3_MASK_L                 (MIU2_REG_BASE + 0x53*2)
#define MIU2_RQ3_MASK_H                 (MIU2_REG_BASE + 0x53*2 +1)
#define MIU2_RQ4_MASK_L                 (MIU2_REG_BASE2 + 0x0003*2)
#define MIU2_RQ4_MASK_H                 (MIU2_REG_BASE2 + 0x0003*2 +1)

#define MIU2_SEL0_L                     (MIU2_REG_BASE + 0xF0)
#define MIU2_SEL0_H                     (MIU2_REG_BASE + 0xF1)
#define MIU2_SEL2_L                     (MIU2_REG_BASE + 0xF4)
#define MIU2_SEL2_H                     (MIU2_REG_BASE + 0xF5)
#define MIU2_SEL3_L                     (MIU2_REG_BASE + 0xF6)
#define MIU2_SEL3_H                     (MIU2_REG_BASE + 0xF7)

//------------------------------------------------------------------------------
// MVD Reg
//------------------------------------------------------------------------------
    #define MVD_CTRL_RST                        BIT0//1: reset MVD; 0: release reset
    #define MVD_CTRL_CLR_INT                    BIT2//Clear MVD interrupt.
    #define MVD_CTRL_CLK_SYNCMODE               BIT4//1: sync_mode; 0: async_mode
    #define MVD_CTRL_CLK_ALLON                  BIT5//1: enable all clocks in mvd
    #define MVD_CTRL_DISCONNECT_MIU             BIT6//1: disconnect; 0: release reset
#define MVD_CTRL                                (MVD_REG_BASE + 0x00)

    #define MVD_STATUS_READY                    BIT1
#define MVD_STATUS                              (MVD_REG_BASE + 0x01)
    #define MVD_T8_MIU_128_0                   BIT2  // enable MVD to 128 bit mode
    #define MVD_T8_MIU_128_1                   BIT3  // enable MVD to 128 bit mode
#define MVD_COMMAND                             (MVD_REG_BASE + 0x02)
#define MVD_ARG0                                (MVD_REG_BASE + 0x04)
#define MVD_ARG1                                (MVD_REG_BASE + 0x05)
#define MVD_ARG2                                (MVD_REG_BASE + 0x06)
#define MVD_ARG3                                (MVD_REG_BASE + 0x07)
#define MVD_ARG4                                (MVD_REG_BASE + 0x08)
#define MVD_ARG5                                (MVD_REG_BASE + 0x09)

    #define MVD_SLQCTRL_WADR_RELOAD             BIT0 //reload "slq_wadr" into write address
                                                //w reload: program 1, then program 0, and reload complete
    #define MVD_SLQCTRL_RADR_PROBE              BIT1 //SLQ read address probe
    #define MVD_SLQCTRL_WADR_PROBE              BIT2 //SLQ write address probe
                                                //r/w probe: program 1, then program 0, and read "slq_caddr"
#define MVD_SLQCTRL                             (MVD_REG_BASE + 0x16)

//SLQ write address value[24:0]
#define MVD_SLQ_WADR0                           (MVD_REG_BASE + 0x18)
#define MVD_SLQ_WADR1                           (MVD_REG_BASE + 0x19)
#define MVD_SLQ_WADR2                           (MVD_REG_BASE + 0x1A)
#define MVD_SLQ_WADR3                           (MVD_REG_BASE + 0x1B)

//SLQ probe address value[24:0]
#define MVD_SLQ_CADR0                           (MVD_REG_BASE + 0x1C)
#define MVD_SLQ_CADR1                           (MVD_REG_BASE + 0x1D)
#define MVD_SLQ_CADR2                           (MVD_REG_BASE + 0x1E)
#define MVD_SLQ_CADR3                           (MVD_REG_BASE + 0x1F)

//CRC in/out
#define MVD_CRC_CTL                             (MVD_REG_BASE + 0x23)
    #define MVD_CRC_CTL_FIRE                    BIT6
    #define MVD_CRC_CTL_DONE                    BIT7
#define MVD_CRC_HSIZE                           (MVD_REG_BASE + 0x22)   //CRC hsize[13:4]
#define MVD_CRC_VSIZE                           (MVD_REG_BASE + 0x24)   //CRC vsize[13:0]
#define MVD_CRC_STRIP                           (MVD_REG_BASE + 0x26)   //CRC strip[13:0]
#define MVD_CRC_Y_START                         (MVD_REG_BASE + 0x28)   //CRC y start address[25:0]
    #define MVD_CRC_Y_START_LEN                 BMASK(25:0)
#define MVD_CRC_UV_START                        (MVD_REG_BASE + 0x2C)   //CRC uv start address[25:0]
    #define MVD_CRC_UV_START_LEN                BMASK(25:0)
#define MVD_CRC_Y_L                             (MVD_REG_BASE + 0x30)
#define MVD_CRC_Y_H                             (MVD_REG_BASE + 0x32)
#define MVD_CRC_UV_L                            (MVD_REG_BASE + 0x34)
#define MVD_CRC_UV_H                            (MVD_REG_BASE + 0x36)

#define REG_CHIPTOP_BASE        0x0b00UL

#define REG_CKG_MVD_ENABLE        (REG_CHIPTOP_BASE + 0x37*2)
    #define CKG_MVD_ENABLE_GATED          BIT0
    #define CKG_MVD_PAS_ENABLE_GATED      BIT1

#define REG_CKG_MVD_SYNC        (REG_CHIPTOP_BASE + 0x38*2 +1)
    #define CKG_MVD_SYNC_GATED      BIT0

#define REG_CKG_MVD             (REG_CHIPTOP_BASE + 0x39*2)
    #define CKG_MVD_GATED           BIT0
    #define CKG_MVD_INVERT          BIT1
    #define CKG_MVD_MASK            (BIT4 | BIT3 | BIT2)
    #define CKG_MVD_288MHZ          (0 << 2)
    #define CKG_MVD_216MHZ          (1 << 2)
    #define CKG_MVD_172MHZ          (2 << 2)
    #define CKG_MVD_144MHZ          (3 << 2)
    #define CKG_MVD_NA1             (4 << 2)
    #define CKG_MVD_NA2             (5 << 2)
    #define CKG_MVD_72MHZ           (6 << 2)
    #define CKG_MVD_NA3             (7 << 2)


#define REG_CKG_MVD_CHROMA_A      (REG_CHIPTOP_BASE + 0x3a*2)
    #define CKG_MVD_CHROMA_A_GATED           BIT0
    #define CKG_MVD_CHROMA_A_INVERT          BIT1

#define REG_CKG_MVD_CHROMA_B      (REG_CHIPTOP_BASE + 0x3a*2)
    #define CKG_MVD_CHROMA_B_GATED           BIT2
    #define CKG_MVD_CHROMA_B_INVERT          BIT3

#define REG_CKG_MVD_CHROMA_C      (REG_CHIPTOP_BASE + 0x3a*2)
    #define CKG_MVD_CHROMA_C_GATED           BIT4
    #define CKG_MVD_CHROMA_C_INVERT          BIT5

#define REG_CKG_MVD_LUMA_A      (REG_CHIPTOP_BASE + 0x3a*2 + 1)
    #define CKG_MVD_LUMA_A_GATED           BIT0
    #define CKG_MVD_LUMA_A_INVERT          BIT1

#define REG_CKG_MVD_LUMA_B      (REG_CHIPTOP_BASE + 0x3b*2)
    #define CKG_MVD_LUMA_B_GATED           BIT0
    #define CKG_MVD_LUMA_B_INVERT          BIT1

#define REG_CKG_MVD_LUMA_C      (REG_CHIPTOP_BASE + 0x3b*2 + 1)
    #define CKG_MVD_LUMA_C_GATED           BIT0
    #define CKG_MVD_LUMA_C_INVERT          BIT1


#define REG_CKG_MVD_RMEM        (REG_CHIPTOP_BASE + 0x3c*2)
    #define CKG_MVD_RMEM_GATED           BIT0
    #define CKG_MVD_RMEM_INVERT          BIT1

#define REG_CKG_MVD_RMEM1       (REG_CHIPTOP_BASE + 0x3c*2 + 1)
    #define CKG_MVD_RMEM1_GATED           BIT0
    #define CKG_MVD_RMEM1_INVERT          BIT1

#define REG_CKG_MVD_RREFDAT       (REG_CHIPTOP_BASE + 0x3c*2 + 1)
    #define CKG_MVD_RREFDAT_GATED           BIT2
    #define CKG_MVD_RREFDAT_INVERT          BIT3

#define REG_CKG_VD_AEON        (REG_CHIPTOP_BASE + 0x30*2)
    #define CKG_VD_AEON_GATED             BIT0
    #define CKG_VD_AEON_INVERT            BIT1
    #define CKG_VD_AEON_MASK            (BIT6 | BIT5 | BIT4 | BIT3 | BIT2)
    #define CKG_VD_AEON_160MHZ          (0 << 2)
                //Notice: The clock 160M comes from UTMI.
                //Please start UTMI's clock before you switch to 160M
    #define CKG_VD_AEON_144MHZ          (1 << 2)
    #define CKG_VD_AEON_123MHZ          (2 << 2)
    #define CKG_VD_AEON_108MHZ          (3 << 2)
    #define CKG_VD_AEON_96MHZ           (4 << 2)
    #define CKG_VD_AEON_72MHZ           (5 << 2)
    #define CKG_VD_AEON_DISABLE0        (6 << 2)    //disable
    #define CKG_VD_AEON_DISABLE1        (7 << 2)    //disable
    #define CKG_VD_AEON_CLK_MCU         (1 << 5)    //01xxx
    #define CKG_VD_AEON_CLK_MIU         (2 << 5)    //10xxx
    #define CKG_VD_AEON_XTAL            (3 << 5)    //11xxx

#ifdef CONFIG_MSTAR_SRAMPD
#define PATGEN_REG_BASE                         0x71200
#define REG_CODEC_SRAM_SD_EN       (PATGEN_REG_BASE + 0x10*2)
    #define SRAM_SD_EN_MVD           BIT2
#endif

#define REG_CHIP_ID_MAJOR                       (CHIP_REG_BASE + 0xCC)
#define REG_CHIP_ID_MINOR                       (CHIP_REG_BASE + 0xCD)
#define REG_CHIP_VERSION                        (CHIP_REG_BASE + 0xCE)
#define REG_CHIP_REVISION                       (CHIP_REG_BASE + 0xCF)

#endif // _REG_MVD_H_

