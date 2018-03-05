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
///////////////////////////////////////////////////////////////////////////////
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
// rights to any and all damages, losses, costs and expenses resulting
// therefrom.
//
//
/// @file  regMVD.h
/// @brief Hardware register definition for Video Decoder
/// @author MStar Semiconductor Inc.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _REG_AUDIO_H_
#define _REG_AUDIO_H_

#include "audio_mbox.h"
#include "audio_comm2.h"
#include "decR2_proj.h"
#include "sndR2_proj.h"

#define A3_PATCH_DMA_OLD_MODE   1

///////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
// Base Address
//---------------------------------------------------------------------------
#define AUDIO_REG_BASE                           0x2C00  // 0x2C00 - 0x2DFF
#define MIU0_REG_BASE                            0x1200
#define MIU1_REG_BASE                            0x0600

//---------------------------------------------------------------------------
// AUDIO SIF Register
//---------------------------------------------------------------------------
#define REG_AUDIO_ASIF_CONFIG0                   0x2CC0
#define REG_AUDIO_ASIF_CONFIG1                   0x2CC2
#define REG_AUDIO_ASIF_CONFIG2                   0x2CC4
#define REG_AUDIO_ASIF_CONFIG3                   0x2CC6
#define REG_AUDIO_ASIF_CONFIG4                   0x2CC8
#define REG_AUDIO_ASIF_ICTRL                     0x2CCA
#define REG_AUDIO_ASIF_AMUX                      0x2CCC
#define REG_AUDIO_ASIF_TST                       0x2CCE
#define REG_AUDIO_ASIF_ADCREF                    0x2CD0
#define REG_AUDIO_SIFPLL_CTRL                    0x2CD2
#define REG_AUDIO_SIFPLL_MN                      0x2CD4
#define REG_AUDIO_SIFPLL_TEST                    0x2CD6
#define REG_AUDIO_SIFPLL_EXT                     0x2CD8
#define REG_AUDIO_SIFPLL_STATUS                  0x2CDA
#define REG_AUDIO_ASIF_TST_EXT                   0x2CDC
#define REG_AUDIO_VIF_CONFIG0                    0x2CDE
//--------------------------------
// AUDIO SIF Register Value
//--------------------------------
#define VAL0_REG_AUDIO_ASIF_CONFIG0              0x0200
#define VAL0_REG_AUDIO_ASIF_CONFIG1              0x0070
#define VAL0_REG_AUDIO_ASIF_CONFIG2              0x1200
#define VAL0_REG_AUDIO_ASIF_CONFIG3              0x1000
#define VAL0_REG_AUDIO_ASIF_CONFIG4              0x0090
#define VAL0_REG_AUDIO_ASIF_ICTRL                0x1555
#define VAL0_REG_AUDIO_ASIF_AMUX                 0x00EA
#define VAL0_REG_AUDIO_ASIF_TST                  0x0004
#define VAL0_REG_AUDIO_ASIF_ADCREF               0x6C00
#define VAL0_REG_AUDIO_SIFPLL_CTRL               0x1009
#define VAL0_REG_AUDIO_SIFPLL_MN                 0x1109
#define VAL0_REG_AUDIO_SIFPLL_TEST               0x0000
#define VAL0_REG_AUDIO_SIFPLL_EXT                0x0001
#define VAL0_REG_AUDIO_SIFPLL_STATUS             0x2CDA
#define VAL0_REG_AUDIO_ASIF_TST_EXT              0x0000
#define VAL0_REG_AUDIO_VIF_CONFIG0               0x0000
#define MASK_REG_AUDIO_VIF_CONFIG0               0x0040
#define VAL1_REG_AUDIO_VIF_CONFIG0               0x0040
#define VAL2_REG_AUDIO_VIF_CONFIG0               0x0000

//---------------------------------------------------------------------------
// AUDIO Advance Sound Register
//---------------------------------------------------------------------------
#define REG_SOUND_ADV_CFG0                       M2S_MBOX_ADVSND_EN+0
#define REG_SOUND_ADV_CFG1                       M2S_MBOX_ADVSND_EN+1
#define REG_SOUND_ADV_CFG2                       M2S_MBOX_ADVSND_EN+2
#define REG_SOUND_ADV_CFG3                       M2S_MBOX_ADVSND_EN+3

//---------------------------------------------------------------------------
// AUDIO 0x1E00 BANK Register
//---------------------------------------------------------------------------
#define REG_CHIP_ID_MAJOR                        0x1ECC
#define REG_CHIP_ID_MINOR                        0x1ECD
#define REG_CHIP_VERSION                         0x1ECE
#define REG_CHIP_REVISION                        0x1ECF

//---------------------------------------------------------------------------
// AUDIO 0x2A00 BANK Register
//---------------------------------------------------------------------------
#define REG_DEC_IDMA_CTRL0                       0x2A00

#define REG_DEC_DSP_BRG_DATA_L                   0x2A02
#define REG_DEC_DSP_BRG_DATA_H                   0x2A03
#define REG_DEC_IDMA_WRBASE_ADDR_L               0x2A04
#define REG_DEC_IDMA_WRBASE_ADDR_H               0x2A05

#define REG_DEC_IDMA_RDBASE_ADDR_L               0x2A08
#define REG_DEC_IDMA_RDBASE_ADDR_H               0x2A09

#define REG_DEC_IDMA_RDDATA_H_0                  0x2A0C
#define REG_DEC_IDMA_RDDATA_H_1                  0x2A0D
#define REG_DEC_IDMA_RDDATA_L                    0x2A0E

#define REG_DEC_DSP_ICACHE_BASE_L                0x2A10
#define REG_DEC_DSP_ICACHE_BASE_H                0x2A11

#define REG_DEC_AUD_DTRL                         0x2A20
#define REG_DEC_MAD_OFFSET_BASE_L                0x2A22
#define REG_DEC_MAD_OFFSET_BASE_H                0x2A24
#define REG_DEC_MBASE_H                          0x2A26
#define REG_DEC_MSIZE_H                          0x2A28
#define REG_DEC_DECODE_CMD                       0x2A2C
#define REG_DEC_ENCODE_CMD                       0x2A2D // kochien added for MPEG encoder
#define REG_DEC_MCFG                             0x2A2A

#define REG_DEC_DSPDMA_CFG                       0x2A36

#define REG_DEC_BDMA_CFG                         0x2A40
#define REG_DEC_MAD_OFFSET_BASE_EXT              0x2A48

#define REG_FD230_SELECT                         0x2A7E

#define REG_SE_IDMA_CTRL0                        0x2A80

#define REG_SE_DSP_BRG_DATA_L                    0x2A82
#define REG_SE_DSP_BRG_DATA_H                    0x2A83
#define REG_SE_IDMA_WRBASE_ADDR_L                0x2A84
#define REG_SE_IDMA_WRBASE_ADDR_H                0x2A85

#define REG_SE_IDMA_RDBASE_ADDR_L                0x2A88
#define REG_SE_IDMA_RDBASE_ADDR_H                0x2A89

#define REG_SE_IDMA_RDDATA_H_0                   0x2A8C
#define REG_SE_IDMA_RDDATA_H_1                   0x2A8D
#define REG_SE_IDMA_RDDATA_L                     0x2A8E

#define REG_SE_DSP_ICACHE_BASE_L                 0x2A90
#define REG_SE_DSP_ICACHE_BASE_H                 0x2A91

#define REG_SE_AUD_DTRL                          0x2AA0
#define REG_SE_MAD_OFFSET_BASE_L                 0x2AA2
#define REG_SE_MAD_OFFSET_BASE_H                 0x2AA4
#define REG_SE_MBASE_H                           0x2AA6
#define REG_SE_MSIZE_H                           0x2AA8
#define REG_SE_DECODE_CMD                        0x2AAC
#define REG_SE_MCFG                              0x2AAA

#define REG_SE_DSPDMA_CFG                        0x2AB6

#define REG_SE_BDMA_CFG                          0x2AC0
#define REG_SE_MAD_OFFSET_BASE_EXT               0x2AC8

//---------------------------------------------------------------------------
// AUDIO 0x2B00 BANK Register
//---------------------------------------------------------------------------
#define REG_RIU_MAIL_00                           0x2B40

#define REG_AUDIO_DMA_RD_CTRL_0                  0x2B80
#define REG_AUDIO_DMA_RD_CTRL_1                  0x2B82
#define REG_AUDIO_DMA_RD_CTRL_2                  0x2B84
#define REG_AUDIO_DMA_RD_CTRL_3                  0x2B86
#define REG_AUDIO_DMA_RD_CTRL_4                  0x2B88
#define REG_AUDIO_DMA_RD_CTRL_5                  0x2B8A
#define REG_AUDIO_DMA_RD_CTRL_6                  0x2B8C
#define REG_AUDIO_DMA_RD_CTRL_7                  0x2B8E
#define REG_AUDIO_DMA_RD_CTRL_8                  0x2B90
#define REG_AUDIO_DMA_RD_CTRL_9                  0x2B92
#define REG_AUDIO_DMA_RD_CTRL_10                 0x2B94
#define REG_AUDIO_DMA_RD_CTRL_11                 0x2B96
#define REG_AUDIO_DMA_RD_CTRL_12                 0x2B98
#define REG_AUDIO_DMA_RD_CTRL_13                 0x2B9A
#define REG_AUDIO_DMA_RD_CTRL_14                 0x2B9C
#define REG_AUDIO_DMA_RD_CTRL_15                 0x2B9E
#define REG_AUDIO_DMA_RD_CTRL_16                 0x2BA0
#define REG_AUDIO_DMA_RD_CTRL_17                 0x2BA2
#define REG_AUDIO_DMA_RD_CTRL_18                 0x2BA4
#define REG_AUDIO_DMA_RD_CTRL_19                 0x2BA6
#define REG_AUDIO_PARSER_CTRL                    0x2BF2

//---------------------------------------------------------------------------
// AUDIO 0x2C00 BANK Register
// AUDIO Common Register
//---------------------------------------------------------------------------
#define REG_AUDIO_SOFT_RESET                     0x2C00
#define REG_AUDIO_INPUT_CFG                      0x2C02
#define REG_AUDIO_STATUS_DVB_FREQ                0x2C04
#define REG_AUDIO_AUDIO_INIT_CHECK               0x2C05 //dummy reg check audio_init is DONE
#define REG_AUDIO_STATUS_I2S_FREQ                0x2C06
#define REG_AUDIO_STATUS_SIF_FREQ                0x2C08
#define REG_AUDIO_SPDIF_IN_CFG                   0x2C0A
#define REG_AUDIO_STATUS_INPUT                   0x2C0C
#define REG_AUDIO_STATUS_SPDIF_IN_FREQ           0x2C0E
#define REG_AUDIO_STATUS_SPDIF_IN_CS0            0x2C10
#define REG_AUDIO_STATUS_SPDIF_IN_CS1            0x2C12
#define REG_AUDIO_STATUS_SPDIF_IN_CS2            0x2C14
#define REG_AUDIO_STATUS_SPDIF_IN_CS3            0x2C16
#define REG_AUDIO_STATUS_SPDIF_IN_CS4            0x2C18
#define REG_AUDIO_STATUS_SPDIF_IN_PC             0x2C1A
#define REG_AUDIO_STATUS_SPDIF_IN_PD             0x2C1C

#define REG_AUDIO_DMA_RD_SYNTH_NF                0x2C20
#define REG_AUDIO_STATUS_SYNTH                   0x2C2C
#define REG_AUDIO_STATUS_HDMI_PC                 0x2C40
#define REG_AUDIO_STATUS_HDMI_PD                 0x2C42
#define REG_AUDIO_HDMI_MATRIX0                   0x2C44
#define REG_AUDIO_HDMI_MATRIX1                   0x2C46
#define REG_AUDIO_DOWN_SAMPLE                    0x2C48

#define REG_AUDIO_BT_CTRL1                       0x2C50
#define REG_AUDIO_BT_CTRL2                       0x2C52
#define REG_AUDIO_BT_CTRL3                       0x2C54
#define REG_AUDIO_BT_CTRL4                       0x2C56

#define REG_AUDIO_DECIMATION_CFG                 0x2C58

#define REG_AUDIO_DECODER1_CFG                   0x2C60
#define REG_AUDIO_DECODER2_CFG                   0x2C62
#define REG_AUDIO_DECODER3_CFG                   0x2C61
#define REG_AUDIO_DECODER4_CFG                   0x2C63
#define REG_AUDIO_DECODER5_CFG                   0x2C25
#define REG_AUDIO_CH1_CFG                        0x2C64
#define REG_AUDIO_CH2_CFG                        0x2C66
#define REG_AUDIO_CH3_CFG                        0x2C68
#define REG_AUDIO_CH4_CFG                        0x2C6A
#define REG_AUDIO_CH5_CFG                        0x2C65
#define REG_AUDIO_CH6_CFG                        0x2C67
#define REG_AUDIO_CH7_CFG                        0x2C69
#define REG_AUDIO_CH8_CFG                        0x2C6B

#define REG_AUDIO_INPUT_REGEN_CFG                0x2C6C

#define REG_AUDIO_DOUT_FIX_VAL1                  0x2C70
#define REG_AUDIO_DOUT_FIX_VAL2                  0x2C72
#define REG_AUDIO_DOUT_FIX_VAL3                  0x2C74

#define REG_AUDIO_SPDIF_OUT_CS0                  0x2C80
#define REG_AUDIO_SPDIF_OUT_CS1                  0x2C82
#define REG_AUDIO_SPDIF_OUT_CS2                  0x2C84
#define REG_AUDIO_SPDIF_OUT_CS3                  0x2C86
#define REG_AUDIO_SPDIF_OUT_CS4                  0x2C88
#define REG_AUDIO_SPDIF_OUT_CFG                  0x2C8A
#define REG_AUDIO_SPDIF2_OUT_CS0                 0x2C81
#define REG_AUDIO_SPDIF2_OUT_CS1                 0x2C83
#define REG_AUDIO_SPDIF2_OUT_CS2                 0x2C85
#define REG_AUDIO_SPDIF2_OUT_CS3                 0x2C87
#define REG_AUDIO_SPDIF2_OUT_CS4                 0x2C89
#define REG_AUDIO_SPDIF2_OUT_CFG                0x2C5E
#define REG_AUDIO_I2S_OUT1_CFG                   0x2C8C

#define REG_AUDIO_PAD_CFG                        0x2C90
#define REG_AUDIO_MUTE_CFG                       0x2C92
#define REG_AUDIO_MUTE_CTRL1                     0x2C94
#define REG_AUDIO_MUTE_CTRL2                     0x2C96
#define REG_AUDIO_MUTE_CTRL3                     0x2C98
#define REG_AUDIO_MUTE_CTRL4                     0x2C9A

#define REG_AUDIO_CODEC_SYNTH                    0x2CA0
#define REG_CODEC_SYNTH_H                        0x2CA1
#define REG_AUDIO_PLL_REF_CFG                    0x2CA2
#define REG_AUDIO_CLK_CFG0                       0x2CA4
#define REG_CLK_CFG0                             0x2CA4
#define REG_AUDIO_CLK_CFG1                       0x2CA6
#define REG_AUDIO_CLK_CFG2                       0x2CA8
#define REG_AUDIO_CLK_CFG3                       0x2CAA
#define REG_AUDIO_CLK_CFG4                       0x2CAC
#define REG_AUDIO_CLK_CFG5                       0x2CAE
#define REG_AUDIO_CLK_CFG6                       0x2CB0
#define REG_AUDIO_SYNTH_EXPANDER                 0x2CB2
#define REG_AUDIO_SYNTH_768_CFG0                 0x2CB4
#define REG_AUDIO_SYNTH_768_CFG1                 0x2CB6
#define REG_AUDIO_SYNTH_768_FREQ                 0x2CB8
#define REG_AUDIO_OUT_256FS_SEL                  0x2CBA

#define REG_AUDIO_CODEC_CFG0                     0x2CE0
#define REG_AUDIO_CODEC_CFG1                     0x2CE2
#define REG_AUDIO_CODEC_CFG2                     0x2CE4
#define REG_AUDIO_CODEC_CFG3                     0x2CE6
#define REG_AUDIO_CODEC_CFG4                     0x2CE8
#define REG_AUDIO_CODEC_CFG5                     0x2CEA
#define REG_AUDIO_CODEC_CFG6                     0x2CEC
#define REG_AUDIO_CODEC_CFG7                     0x2CEE
#define REG_AUDIO_DC_OFFSET                      0x2CFA

/* SPDIF REGISTER */
#define REG_SPDIF_NPCM_SYNTH_NF_H                0x2BB8
#define REG_SPDIF_NPCM_SYNTH_NF_L                0x2BBA

#define REG_SPDIFTX_CHANNEL_STATUS_TOGGLE  0x2FFC

//---------------------------------------------------------------------------
// AUDIO 0x2E00 BANK Register
// AUDIO Common Register
//-------------------------------------------------------------------------

#define REG_R2_RIU_MAIL_00                         0x163D02
#define REG_R2_1_RIU_MAIL_00                       0x163D06
#define REG_R2_HDMI_NONPCM_PATH                    0x160396
#define REG_R2_DECODE1_TYPE                        0x160398
#define REG_R2_DECODE2_TYPE                        0x16039A
#define REG_R2_DECODE3_TYPE                        0x16039C
#define REG_R2_DECODE1_CMD                         0x160398+1
#define REG_R2_DECODE2_CMD                         0x16039A+1
#define REG_R2_DECODE3_CMD                         0x16039C+1

//---------------------------------------------------------------------------
// AUDIO Sound Effect Register
//---------------------------------------------------------------------------
#define REG_SOUND_MAIN_PERSCALE                  M2S_MBOX_PRESCALE

#define REG_SOUND_AUOUT0_VOLUME                  M2S_MBOX_AUOUT0_VOL+1
#define REG_SOUND_AUOUT1_VOLUME                  M2S_MBOX_AUOUT1_VOL+1
#define REG_SOUND_AUOUT2_VOLUME                  M2S_MBOX_AUOUT2_VOL+1
#define REG_SOUND_AUOUT3_VOLUME                  M2S_MBOX_AUOUT3_VOL+1
#define REG_SOUND_I2S_VOLUME                     M2S_MBOX_I2S_VOL+1
#define REG_SOUND_SPDIF_VOLUME                   M2S_MBOX_SPDIF_VOL+1
#define REG_SOUND_I2S2_VOLUME                    M2S_MBOX_I2S2_VOL+1
#define REG_SOUND_CH7_VOLUME                     M2S_MBOX_CH7_VOL+1
#define REG_SOUND_AUOUT0_VOL_FRAC                M2S_MBOX_AUOUT0_VOL
#define REG_SOUND_AUOUT1_VOL_FRAC                M2S_MBOX_AUOUT1_VOL
#define REG_SOUND_AUOUT2_VOL_FRAC                M2S_MBOX_AUOUT2_VOL
#define REG_SOUND_AUOUT3_VOL_FRAC                M2S_MBOX_AUOUT3_VOL
#define REG_SOUND_I2S_VOL_FRAC                   M2S_MBOX_I2S_VOL
#define REG_SOUND_SPDIF_VOL_FRAC                 M2S_MBOX_SPDIF_VOL
#define REG_SOUND_I2S2_VOL_FRAC                  M2S_MBOX_HDMI_VOL
#define REG_SOUND_CH7_VOL_FRAC                   M2S_MBOX_CH7_VOL

#define REG_SOUND_AD_VOLUME                      M2S_MBOX_AD_CONTROL
#define REG_SOUND_AD_VOLUME_HI                   M2S_MBOX_AD_CONTROL+1


#define REG_SOUND_EQ_BASE                        M2S_MBOX_BASS_CTRL+1
#define REG_SOUND_EQ1                            M2S_MBOX_EQ1_GAIN+1
#define REG_SOUND_EQ2                            M2S_MBOX_EQ2_GAIN+1
#define REG_SOUND_EQ3                            M2S_MBOX_EQ3_GAIN+1
#define REG_SOUND_EQ4                            M2S_MBOX_EQ4_GAIN+1
#define REG_SOUND_EQ5                            M2S_MBOX_EQ5_GAIN+1

#define REG_SOUND_BASS                           M2S_MBOX_BASS_CTRL
#define REG_SOUND_TREBLE                         M2S_MBOX_TREBLE_CTRL
#define REG_SOUND_BALANCEL                       M2S_MBOX_BAL_CTRL+1
#define REG_SOUND_BALANCER                       M2S_MBOX_BAL_CTRL

#define REG_SOUND_AVC_AT                         M2S_MBOX_AVC_CTRL+1
#define REG_SOUND_AVC_RT                         M2S_MBOX_AVC_CTRL+1
#define REG_SOUND_AVC_MODE                       M2S_MBOX_AVC_CTRL+1
#define REG_SOUND_AVC_THRESHOLD                  M2S_MBOX_AVC_CTRL
#define REG_SOUND_DRC_THRESHOLD                  M2S_MBOX_DRC_CTRL
#define REG_SOUND_DRC_ENTRY                      M2S_MBOX_DRC_CTRL+1
#define REG_SOUND_NR_THRESHOLD                   M2S_MBOX_NR_CTRL

#define REG_SOUND_MAIN_SNDEFFECT                 M2S_MBOX_SNDEFF_EN

#define REG_SOUND_MAIN_COUNTER                   S2M_MBOX_WHILE1_CNTR
#define REG_SOUND_TIMER_COUNTER                  S2M_MBOX_TIMER_CNTR+1
#define REG_SOUND_ISR_COUNTER                    S2M_MBOX_ISR_CNTR+1

#define REG_SOUND_UPLOAD_COUNTER                 S2M_MBOX_PCM_UPLOAD_CNT

#define REG_SOUND_CH5_MIX_VOL_INT                 M2S_MBOX_KTV5_VOL+1
#define REG_SOUND_CH5_MIX_VOL_FRC                 M2S_MBOX_KTV5_VOL
#define REG_SOUND_CH6_MIX_VOL_INT                 M2S_MBOX_KTV6_VOL+1
#define REG_SOUND_CH6_MIX_VOL_FRC                 M2S_MBOX_KTV6_VOL
#define REG_SOUND_CH8_MIX_VOL_INT                 M2S_MBOX_KTV8_VOL+1
#define REG_SOUND_CH8_MIX_VOL_FRC                 M2S_MBOX_KTV8_VOL

//---------------------------------------------------------------------------
// AUDIO COMMON MAIL BOX REGISTER
//---------------------------------------------------------------------------
#define REG_M2D_MAILBOX_0_L                      0x2D20
#define REG_M2D_MAILBOX_0_H                      0x2D21

#define REG_M2D_MAILBOX_1_L                      0x2D22
#define REG_M2D_MAILBOX_1_H                      0x2D23

#define REG_M2D_MAILBOX_5_L                      0x2D2A
#define REG_M2D_MAILBOX_5_H                      0x2D2B

#define REG_M2D_MAILBOX_7_L                      0x2D2E
#define REG_M2D_MAILBOX_7_H                      0x2D2F

#define REG_DEC2_DECODE_CMD                      0x2D2C

#define REG_DBG_DATA_L                           0x2D2E

#define REG_DEBUG_REG_3_H                        0x2D37
#define REG_DEBUG_REG_4_L                        0x2D38
#define REG_DEBUG_REG_4_H                        0x2D39
#define REG_DEBUG_REG_5_L                        0x2D3A
#define REG_DEBUG_REG_5_H                        0x2D3B
#define REG_DBG_CMD                              0x2D3D

#define REG_MB_MODE_SELECT                       M2S_MBOX_SOUND_MODE_SEL
#define REG_MB_POWER_DOWN                        M2S_MBOX_POWER_DOWN
#define REG_MB_TIME_STAMP_SEC                    D2M_MBOX_MM_PTS_IN_SEC
#define REG_MB_TIME_STAMP_4ms                    D2M_MBOX_MM_PTS_IN_MSEC

#define REG_D2M_MAILBOX_SE_POWERCTRL             M2S_MBOX_POWER_DOWN+1

#define REG_MB_PCMUPLOAD_CMD                     M2S_MBOX_BT_CTRL
#define REG_MB_PCMUPLOAD_ADDR                    0x2D4E

#define REG_D2M_MAILBOX_3_L                      0x2D46
#define REG_D2M_MAILBOX_3_H                      0x2D47

#define REG_D2M_MAILBOX_4_L                      0x2D48
#define REG_D2M_MAILBOX_4_H                      0x2D49

#define REG_D2M_MAILBOX_6_L                      0x2D4C
#define REG_D2M_MAILBOX_6_H                      0x2D4D

#define REG_D2M_MAILBOX_7_L                      0x2D4E
#define REG_D2M_MAILBOX_7_H                      0x2D4F

#define REG_D2M_MAILBOX_8_L                      0x2D50
#define REG_D2M_MAILBOX_8_H                      0x2D51

#define REG_D2M_MAILBOX_9_L                      0x2D52
#define REG_D2M_MAILBOX_9_H                      0x2D53

#define REG_D2M_MAILBOX_A_L                      0x2D54
#define REG_D2M_MAILBOX_A_H                      0x2D55

#define REG_D2M_MAILBOX_B_L                      0x2D56
#define REG_D2M_MAILBOX_B_H                      0x2D57

#define REG_D2M_MAILBOX_C_L                      0x2D58
#define REG_D2M_MAILBOX_C_H                      0x2D59

#define REG_D2M_MAILBOX_D_L                      0x2D5A
#define REG_D2M_MAILBOX_D_H                      0x2D5B
#define REG_DBG_DATA_HI                          0x2D5C
#define REG_DBG_DATA_LO                          0x2D5E

//---------------------------------------------------------------------------
// AUDIO MM MAIL BOX REGISTER
//---------------------------------------------------------------------------
#define REG_DEC1_DDR_ES_BUF_SIZE                 D2M_MBOX_ES_MEMCNT
#define REG_DEC1_DDR_PCM_BUF_SIZE                D2M_MBOX_PCM_MEMCNT
#define REG_DEC1_TIME_STAMP_SEC                  D2M_MBOX_MM_PTS_IN_SEC
#define REG_DEC1_TIME_STAMP_MS                   D2M_MBOX_MM_PTS_IN_MSEC
#define REG_DEC1_TS_PTS_H                        D2M_MBOX_MM_PTS_HI
#define REG_DEC1_LINE_BUF_ADDR                   D2M_MBOX_MM_FILE_REQ_ADDR
#define REG_DEC1_TS_PTS_M                        D2M_MBOX_MM_PTS_ME
#define REG_DEC1_LINE_BUF_SIZE                   D2M_MBOX_MM_FILE_REQ_SIZE
#define REG_DEC1_TS_PTS_L                        D2M_MBOX_MM_PTS_LO
#define REG_DEC1_RESIDUAL_PCM                    D2M_MBOX_MM_FILE_PLAY_END

#define REG_AUDIO_IRQ_CONTROL1                   0x2D62
#define REG_AUDIO_IRQ_CONTROL1_2                 0x2D63
#define REG_AUDIO_IRQ_CONTROL2                   0x2D64
#define REG_AUDIO_DEMODULATOR_CTRL               0x2D66
#define REG_AUDIO_FIFO_STATUS                    0x2D68

//---------------------------------------------------------------------------
// AUDIO DEC_DSP MAIL BOX REGISTER
//---------------------------------------------------------------------------
#define REG_MAD_MAIN_COUNTER                     D2M_MBOX_TIMER_CNTR + 1
#define REG_MAD_TIMER_COUNTER                    D2M_MBOX_TIMER_CNTR

#define REG_DEC_M2D_MAIL_BOX_BASE                0x2D80
#define REG_DEC_D2M_MAIL_BOX_BASE                0x2DA0
#define REG_DEC_D2M_MAIL_BOX_ENC_LINEADDR        D2M_MBOX_ENC_LINEADDR
#define REG_DEC_D2M_MAIL_BOX_ENC_LINESIZE        D2M_MBOX_ENC_LINESIZE
#define REG_DEC_M2D_MAIL_BOX_ENC_CONTROL         M2D_MBOX_ENC_CONTROL

#define REG_DEC1_UNI_PCM_OUTCNT                  M2D_MBOX_UNI_PCMOUT_CNT
#define REG_DEC1_UNI_PCM3_WPTR                   M2D_MBOX_UNI_PCM3_WRPTR
#define REG_DEC1_UNI_NEED_DEC_FRMNUM             M2D_MBOX_UNI_NEED_DECODE_FRMCNT
#define REG_DEC1_UNI_ES_WPTR                     M2D_MBOX_UNI_INPUT_ES_WPTR
#define REG_DEC1_UNI_ES_MEMCNT                   D2M_MBOX_ES_MEMCNT
#define REG_DEC1_UNI_PCM_WPTR                    D2M_MBOX_UNI_PCM_WPTR
#define REG_DEC1_UNI_PCM_OUTSIZE                 D2M_MBOX_UNI_PCM_SIZE
#define REG_DEC1_UNI_PCM3_LEVEL                  D2M_MBOX_UNI_PCM_BUFFEBT
#define REG_DEC1_UNI_DECODE_TAG                  D2M_MBOX_UNI_FRAME_CNT

#define REG_DEC1_OMX_SPDIF_PARAM                 DSP1DmAddr_dec1_omx_param
#define REG_DEC1_OMX_PCM_DIFF                    DSP1DmAddr_dec1_omx_param+1

#define REG_DEC2_UNI_PCM_OUTCNT                  M2S_MBOX_UNI_PCMOUT_CNT
#define REG_DEC2_UNI_PCM3_WPTR                   M2S_MBOX_UNI_PCM3_WRPTR
#define REG_DEC2_UNI_NEED_DEC_FRMNUM             M2S_MBOX_UNI_NEED_DECODE_FRMCNT
#define REG_DEC2_UNI_ES_WPTR                     M2S_MBOX_UNI_INPUT_ES_WPTR
#define REG_DEC2_UNI_ES_MEMCNT                   S2M_MBOX_ES_MEMCNT
#define REG_DEC2_UNI_PCM_WPTR                    S2M_MBOX_UNI_PCM_WPTR
#define REG_DEC2_UNI_PCM_OUTSIZE                 S2M_MBOX_UNI_PCM_SIZE
#define REG_DEC2_UNI_PCM3_LEVEL                  S2M_MBOX_UNI_PCM_BUFFEBT
#define REG_DEC2_UNI_DECODE_TAG                  S2M_MBOX_UNI_FRAME_CNT

#define REG_DEC2_OMX_SPDIF_PARAM                 DSP2DmAddr_dec1_omx_param
#define REG_DEC2_OMX_PCM_DIFF                    DSP2DmAddr_dec1_omx_param+1

#define REG_MB_DEC_CTRL                          M2D_MBOX_DEC_CTRL
#define REG_MB_DEC_PIO_ID                        M2D_MBOX_PIO_ID
#define REG_MB_DEC1_MM_INT_TAG                   M2D_MBOX_MM_FILEIN_TAG
#define REG_MB_DEC_CMD1                          M2D_MBOX_DBG_CMD1
#define REG_MB_DEC_CMD2                          M2D_MBOX_DBG_CMD2
#define REG_MB_DEC_ID_STATUS                     D2M_MBOX_DEC_DECSTATUS
#define REG_MB_ENC_ID_STATUS                     0x2DBA
#define REG_MB_DE_ACK1                           D2M_MBOX_DBG_RESULT1 + 1
#define REG_MB_DE_ACK2                           D2M_MBOX_DBG_RESULT2

// DDP
#define REG_MB_AC3P_SMPRATE                      D2M_MBOX_SAMPLERATE
#define REG_MB_AC3P_LOW_HIGH_CUT                 M2D_MBOX_LOW_HIGH_CUT

// DTS
#define REG_MB_DTS_SMPRATE                       D2M_MBOX_SAMPLERATE
#define REG_MB_DEC1_DTS_CH_CTRL                  0x2D96
#define REG_SPDIF_DTS_NONPCM_LEV                 0x2DB6

// MS10
#define REG_MB_MS10_DDT_DUAL_CTRL                0x2D80
#define REG_MB_MS10_DDT_ENC_CTRL                 0x2D92

#define REG_MB_DOLBY_LOUDNESS_INFO               M2S_MBOX_DOLBY_LOUDNESS_INFO

/* MCU to DSP */
#define REG_M2D_MAILBOX_PIO_ID                   M2D_MBOX_PIO_ID+1
#define REG_M2D_MAILBOX_SPDIF_CTRL               M2S_MBOX_SPDIF_SETTING
#define REG_M2D_MAILBOX_DEC_DBGCMD               M2D_MBOX_DBG_CMD1+1
#define REG_M2D_MAILBOX_DEC_DBGPARAM1            M2D_MBOX_DBG_CMD1
#define REG_M2D_MAILBOX_DEC_DBGPARAM2            M2D_MBOX_DBG_CMD2+1
#define REG_M2D_MAILBOX_DEC_DBGPARAM3            M2D_MBOX_DBG_CMD2

/* DSP to MCU */
#define REG_D2M_MAILBOX_DEC_ISRCMD               D2M_MBOX_INTR_CMDTYPE
#define REG_DEC1_INT_ID                          D2M_MBOX_INTR_CMDTYPE

//---------------------------------------------------------------------------
// AUDIO MPEG ENCODER in DEC_DSP MAIL BOX REGISTER
//---------------------------------------------------------------------------
#define REG_MB_MPEG_INFO                         0x2DAA
#define REG_MB_MPEG_INFO_L                       0x2DAA
#define REG_MB_MPEG_INFO_H                       0x2DAB
#define REG_MPEG_VERSION                         DSP1DmAddr_mpegVer
#define REG_MPEG_FRAMENUM                        DSP1DmAddr_mpg_frmNum

//---------------------------------------------------------------------------
// AUDIO SND_DSP MAIL BOX REGISTER
//---------------------------------------------------------------------------
#define REG_MB_SE_PIO_ID                         M2S_MBOX_MM_FILEIN_TAG
#define REG_MB_DEC2_MM_INT_TAG                   M2S_MBOX_MM_FILEIN_TAG
#define REG_MB_DEC3_CTRL                         M2S_MBOX_AD_CONTROL
#define REG_MB_SE_CMD1                           M2S_MBOX_DBG_CMD1
#define REG_MB_SE_CMD2                           M2S_MBOX_DBG_CMD2
#define REG_MB_DEC2_ID_STATUS                    S2M_MBOX_DEC_STATUS+1
#define REG_MB_SE_ACK1                           S2M_MBOX_DBG_RESULT1
#define REG_MB_SE_ACK2                           S2M_MBOX_DBG_RESULT2

#define REG_AUD_MADBASE_SEL                      0x2DE0
#define REG_AUD_DIS_DMA                          0x2DE0
#define REG_AUD_RST_MAD                          0x2DE0

#define REG_MCUDSP_CNT_CFG                       0x2DE2
#define REG_MAD_BUF_BASE                         0x2DE3

#define REG_MAD_OFFSET_BASE_L                    0x2DE4
#define REG_MAD_OFFSET_BASE_H                    0x2DE5

#define REG_MBASE_L                              0x2DE6
#define REG_MBASE_H                              0x2DE7
#define REG_MSIZE_L                              0x2DE8
#define REG_MSIZE_H                              0x2DE9

#define REG_MEM_CFG                              0x2DEA

#define REG_SE_M2D_MAIL_BOX_BASE                 0x2DC0
#define REG_SE_D2M_MAIL_BOX_BASE                 0x2DE0

#define REG_D2M_MAILBOX_SE_ISRCMD                S2M_MBOX_INTR_CMDTYPE+1

//---------------------------------------------------------------------------
// AUDIO MAD DECODER1 PM Address
//---------------------------------------------------------------------------
// Decoder REG DM address
#define REG_DEC1_INFO_BASE                       DSP1DmAddr_dec1_info
#define REG_DEC1_PARAM_BASE                      DSP1DmAddr_dec1_param

// Decoder REG PM address
#define REG_AUDIO_IPAUTH                         0
#define REG_DEC_SampleRate                       DSP1PmAddr_smpRate    //MonocoNeedFix

//---------------------------------------------------------------------------
// AUDIO MAD DECODER2 PM Address
//---------------------------------------------------------------------------
// Decoder2 REG DM address
#define REG_DEC2_INFO_BASE                       DSP2DmAddr_dec1_info
#define REG_DEC2_PARAM_BASE                      DSP2DmAddr_dec1_param

// Decoder2 REG PM address for T3 (to do, move to ADEC)
#define REG_DEC2_AUDIO_IPAUTH                    DSP2PmAddr_ipSecurity
#define REG_DEC2_SampleRate                      DSP2PmAddr_smpRate
#define REG_DEC2_SoundMode                       DSP2PmAddr_soundMode

//---------------------------------------------------------------------------
// AUDIO MAD DECODER2 DM Address
//---------------------------------------------------------------------------
#define REG_DEC2_DDR_ES_BUF_SIZE                 S2M_MBOX_ES_MEMCNT
#define REG_DEC2_DDR_PCM_BUF_SIZE                S2M_MBOX_PCM_MEMCNT
#define REG_DEC2_TIME_STAMP_SEC                  S2M_MBOX_MM_PTS_IN_SEC
#define REG_DEC2_TIME_STAMP_MS                   S2M_MBOX_MM_PTS_IN_MSEC
#define REG_DEC2_TS_PTS_H                        S2M_MBOX_MM_PTS_HI
#define REG_DEC2_LINE_BUF_ADDR                   S2M_MBOX_MM_FILE_REQ_ADDR
#define REG_DEC2_TS_PTS_M                        S2M_MBOX_MM_PTS_ME
#define REG_DEC2_LINE_BUF_SIZE                   S2M_MBOX_MM_FILE_REQ_SIZE
#define REG_DEC2_TS_PTS_L                        S2M_MBOX_MM_PTS_LO
#define REG_DEC2_RESIDUAL_PCM                    S2M_MBOX_MM_FILE_PLAY_END

#define REG_ALSA_MODE                            DSP2DmAddr_IO_Info2+IO_INFO2_ALSA_MODE

//---------------------------------------------------------------------------
// Audio DEC-R2 REGISTER SETTING
//---------------------------------------------------------------------------
#define REG_DECR2_ICMEM_BASE_LO                  0x163082
#define REG_DECR2_ICMEM_BASE_HI                  0x163084

#define REG_DECR2_DCMEM_BASE_LO                  0x163086
#define REG_DECR2_DCMEM_BASE_HI                  0x163088

#define REG_DECR2_DQMEM_BASE_LO                  0x16309A
#define REG_DECR2_DQMEM_BASE_HI                  0x16309C
#define REG_DECR2_DQMEM_SIZE_MASK_LO             0x16309E
#define REG_DECR2_DQMEM_SIZE_MASK_HI             0x1630A0

#define REG_DECR2_IO0_MAPPING_BASE_HI            0x1630A8
#define REG_DECR2_IO1_MAPPING_BASE_HI            0x1630AA
#define REG_DECR2_IO2_MAPPING_BASE_HI            0x1630AC
#define REG_DECR2_IO3_MAPPING_BASE_HI            0x1630AE
#define REG_DECR2_MEM_CTRL                       0x1630B0
#define REG_DECR2_RESET_CTRL                     0x163080
#define REG_DECR2_SWITCH_CTRL                    0x1630d6

#define REG_DECR2_SYSTEM_START                   0x16039E
#define REG_DECR2_MAIN_COUNTER                   0x1603B2
#define REG_DECR2_TIMER_COUNTER                  0x1603B3

//---------------------------------------------------------------------------
// Audio SND-R2 REGISTER SETTING
//---------------------------------------------------------------------------
#define REG_SNDR2_ICMEM_BASE_LO                  0x112982
#define REG_SNDR2_ICMEM_BASE_HI                  0x112984

#define REG_SNDR2_DCMEM_BASE_LO                  0x112986
#define REG_SNDR2_DCMEM_BASE_HI                  0x112988

#define REG_SNDR2_DQMEM_BASE_LO                  0x11299A
#define REG_SNDR2_DQMEM_BASE_HI                  0x11299C
#define REG_SNDR2_DQMEM_SIZE_MASK_LO             0x11299E
#define REG_SNDR2_DQMEM_SIZE_MASK_HI             0x1129A0

#define REG_SNDR2_IO0_MAPPING_BASE_HI            0x1129A8
#define REG_SNDR2_IO1_MAPPING_BASE_HI            0x1129AA
#define REG_SNDR2_IO2_MAPPING_BASE_HI            0x1129AC
#define REG_SNDR2_IO3_MAPPING_BASE_HI            0x1129AE
#define REG_SNDR2_MEM_CTRL                       0x1129B0
#define REG_SNDR2_RESET_CTRL                     0x112980
#define REG_SNDR2_SWITCH_CTRL                    0x1129d6

#define REG_SNDR2_SYSTEM_START                   0x112E9E
#define REG_SNDR2_ADVSND_SEL                     0x112E8E
#define REG_SNDR2_MBOX_BYTE_ABS3D_SEL            0x112E92
#define REG_SNDR2_MBOX_BYTE_SRS_SEL              0x112E94

#define REG_SNDR2_MAIN_COUNTER                   0x112EB2
#define REG_SNDR2_TIMER_COUNTER                  0x112EB3

#define DSP1_DEC1_ACCUM_ES_CNT                   DSP1DmAddr_sys_Working+SYS_ACCUM_ES_CNT
#define DSP2_DEC1_ACCUM_ES_CNT                   DSP2DmAddr_sys_Working+SYS_ACCUM_ES_CNT

//---------------------------------------------------------------------------
// Audio NewDMAreader REGISTER SETTING
//---------------------------------------------------------------------------
#define NewDMAreader_ctrl                        0x163E40
#define NewDMAreader_BaseAddress_Lo              0x163E42
#define NewDMAreader_BaseAddress_Hi              0x163E44
#define NewDMAreader_DRAM_size                   0x163E46
#define NewDMAreader_CPU_triggersize             0x163E48
#define NewDMAreader_DRAM_underrun_threshold     0x163E4A
#define NewDMAreader_Enable_ctrl                 0x163E4C
#define NewDMAreader_Syth                        0x163E4E
#define NewDMAreader_DRAM_levelcnt               0x163E54
#define NewDMAreader_DRAM_flag                   0x163E56

//---------------------------------------------------------------------------
// Audio NewDMAWriter REGISTER SETTING
//---------------------------------------------------------------------------
#define NewDMAwriter_ctrl                        0x163E60
#define NewDMAwriter_BaseAddress_Lo              0x163E62
#define NewDMAwriter_BaseAddress_Hi              0x163E64
#define NewDMAwriter_DRAM_size                   0x163E66
#define NewDMAwriter_CPU_triggersize             0x163E68
#define NewDMAwriter_DRAM_underrun_threshold     0x163E6A
#define NewDMAwriter_DRAM_levelcnt               0x163E74
#endif // _REG_AUDIO_H_
