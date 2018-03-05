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
// Copyright (c) 2013-2015 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name:   apiAUDIO_v2_customer_config.h
//  Description: apiAUDIO_v2_customer_config.h
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ROLLS_ROYCE

#ifndef _AUDIO_V2_CUSTOMER_CONFIG_H_
#define _AUDIO_V2_CUSTOMER_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------
//config for different Linux Kernel Version
//--------------------------------------------------------------------------------------
#define LINUX_KERNEL_VERSION_4_4_3              0

//--------------------------------------------------------------------------------------
//config for different UTPA.
//--------------------------------------------------------------------------------------
#define UTPA_SUPPORT_AAC_ENCODE                 0
#define UTPA_SUPPORT_COMMON_DECODER_API         0
#define UTPA_SUPPORT_HDMI_TX_OUT_BYPASS         0
#define UTPA_SUPPORT_DOLBY_DAP                  1
#define UTPA_SUPPORT_COMMON_CMD_API             0
#define UTPA_SUPPORT_XPCM_PARAM_CHANNEL_MASK    0
#define UTPA_SUPPORT_HDMI_TX_STATUS             0
#define UTPA_SUPPORT_SET_POWER_ON               1
#define UTPA_SUPPORT_AVC_R_MODE                 0
#define UTPA_SUPPORT_SND_INTERNAL_DELAY         1
#define UTPA_SUPPORT_IS_ATMOS_STREAM            0
#define UTPA_SUPPORT_AAC_TYPE                   0

//---DDPE Control by MCU---
#define UTPA_SUPPORT_M2S_MBOX_DDPE_ENCODE_CTRL  0
#define M2S_MBOX_DDPE_ENCODE_CTRL               0x112DC2

//---HDMI NPCM Control by MCU---
#define UTPA_SUPPORT_M2S_MBOX_MCU_HDMI_NONPCM_CTRL  0
#define M2S_MBOX_MCU_HDMI_NONPCM_CTRL               0x112DC8
        /*
        #define HDMI_MCU_FORCE_CTRL_BIT     MBOX_BIT0
        #define HDMI_NONPCM_FROM_SER2       MBOX_HDMI_NONPCM_FROM_ASND_R2_BIT       //MBOX_BIT4
        #define HDMI_NONPCM_FROM_SEDSP      MBOX_HDMI_NONPCM_FROM_ASND_DSP_BIT      //MBOX_BIT5
        */

//---[R2] MBOX_BYTE_STATUS_SELECT in decR2_proj.h---
#define MBOX_BYTE_STATUS_SELECT     0x16039E

//---g_DDPBypassenable_1, g_DDPBypassenable_2 in halMAD.c---
#define UTPA_SUPPORT_AC3P_INFOTYPE_HDMITX_BYPASS_ENABLE 1
#define DDPBYPASSENABLE_1_DEFAULT                       0
#define DDPBYPASSENABLE_2_DEFAULT                       0

//---check ASND_DSP_DDR_SIZE in ddr_config.h MUST to be sync with _MAD_DSP2_DRAM_SIZE in mhal_audio.c (ALSA kernel)---
//                                                                _MAD_DMA_READER_BASE_OFFSET
//                                                                _MAD_PCM_PLAYBACK2_BASE_OFFSET
//                                                                _MAD_DMA_READER2_BASE_OFFSET
//                                                                _MAD_PCM_CAPTURE1_BASE_OFFSET
//                                                                _MAD_PCM_CAPTURE2_BASE_OFFSET
//                                                                _MAD_PCM_SWMIXER_CLIENT_INFO_BASE_OFFSET
#define UTPA_ASND_DSP_DDR_SIZE_SYNC_WITH_ALSA   0

//--------------------------------------------------------------------------------------
//config for different R2.
//--------------------------------------------------------------------------------------
#define R2_SUPPORT_MS12_PCM_RENDER_ALWAYS_ENABLE    1
#define R2_SUPPORT_R2DMA_READER2                    1
#define R2_SUPPORT_R2_DEC_ISR2_EN                   0

//--------------------------------------------------------------------------------------
//config for different Chip.
//--------------------------------------------------------------------------------------
#define UTPA_AUDIO_CHIP_TYPE_2R1D           1
#define UTPA_AUDIO_CHIP_TYPE_1R1D           0
#define UTPA_AUDIO_CHIP_TYPE_2D1R           0
#define UTPA_AUDIO_CHIP_TYPE_2D             0
#define UTPA_AUDIO_CHIP_TYPE_1D             0

//--------------------------------------------------------------------------------------
//config for different audio common info
//--------------------------------------------------------------------------------------
#define AUDIO_COMMON_INFO_SEDSP_MS12_DDE_DELAY   60  //ms
#define AUDIO_COMMON_INFO_SNDR2_MS12_DDPE_DELAY  110 //ms
#define AUDIO_COMMON_INFO_SNDR2_MS12_DAP_DELAY   50  //ms

//--------------------------------------------------------------------------------------
//config for different customized behavior
//--------------------------------------------------------------------------------------
//---ES Passthrough---
#define CUSTOMIZED_PATCH_PARAM_ES_PASSTHROUGH       1

//---PCM Capture param rptr---
#define CUSTOMIZED_PATCH_PARAM_PCM_CAPTURE1_RPTR    0
#define M2S_MBOX_PCM_CAPTURE_DDR_RdPtr              0x112DD4

#define CUSTOMIZED_PATCH_PARAM_PCM_CAPTURE2_RPTR    0
#define M2S_MBOX_PCM_CAPTURE2_DDR_RdPtr             0x112D38

//---SPDIF info NPCM wptr---
#define CUSTOMIZED_PATCH_INFO_SPDIF_NPCM_WPTR       0
#define DSP2DmAddr_nonpcm_capture_wptr              0x3FAF

//---PCM Capture info wptr---
#define CUSTOMIZED_PATCH_INFO_PCM_CAPTURE1_WPTR     0
#define S2M_MBOX_PCM_CAPTURE_DDR_WrPtr              0x112DF0

#define CUSTOMIZED_PATCH_INFO_PCM_CAPTURE2_WPTR     0
#define S2M_MBOX_PCM_CAPTURE2_DDR_WrPtr             0x112DF4

//---HDMI RX BYPASS---
#define CUSTOMIZED_PATCH_PARAM_HDMI_RX_BYPASS       1
#define REG_AUDIO_SPDIF_OUT_CFG                     0x112C8A
#define REG_AUDIO_SPDIF2_OUT_CFG                    0x112C5E

//---Low Latency Limiter---
#define CUSTOMIZED_PATCH_PARAM_LOW_LATENCY          0

//---DNSE---
#define CUSTOMIZED_PATCH_PARAM_DNSE                 0

//---DOLBY_ATMOS_CONTROL---
#define CUSTOMIZED_PATCH_PARAM_DOLBY_ATMOS_CTRL     0

//---MCU dump R2 log---
#define CUSTOMIZED_PATCH_PARAM_MCU_DUMP_R2_LOG      0
#define CUSTOMIZED_PATCH_INFO_MCU_DUMP_R2_LOG       0

//---ES Repeat Play---
#define CUSTOMIZED_PATCH_PARAM_ES_REPEAT_PLAY       0

//---ASND DSP DDR size---
#define ASND_DSP_DDR_SIZE                           0x310000

//---DEC PCM1 buffer info---
#define CUSTOMIZED_PATCH_INFO_DEC_PCM1_BUFFER       0
#define OFFSET_PCM1_DRAM_ADDR                       0x0048000
#define PCM1_DRAM_SIZE                              0xFF000

//---HDMI NPCM buffer info---
#define CUSTOMIZED_PATCH_INFO_HDMI_NPCM_BUFFER      1
#define OFFSET_HDMI_NONPCM_DRAM_BASE                0x02A0000
#define HDMI_NONPCM_DRAM_SIZE                       0xD8000
#define S2M_MBOX_HDMI_NPCM_RPTR                     0x112DE8
#define S2M_MBOX_HDMI_NPCM_WPTR                     0x112DEA

//---MS12 META DATA buffer info---
#define CUSTOMIZED_PATCH_INFO_MS12_DDPE_METADATA_BUFFER 0
#define OFFSET_DDPENC_METADATA_DRAM_ADDR                0x030C000
#define DDPENC_METADATA_DRAM_SIZE                       0x03100
#define S2M_MBOX_DDPE_METADATA_WPTR                     0x112DEC

//---DEC ES1 buffer info---
#define CUSTOMIZED_PATCH_INFO_DEC_ES1_BUFFER    1
#define ES1_DRAM_SIZE                           0x20000

//---SE-DSP Force MS12 DD/DDP Encode without main sound---
#define CUSTOMIZED_PATCH_PARAM_SEDSP_MS12_ENC_WO_MAIN_SND   0

//---SND-R2 Force MS12 DD/DDP Encode without main sound---
#define CUSTOMIZED_PATCH_PARAM_SNDR2_MS12_ENC_WO_MAIN_SND   1

#define MBOX_BYTE_DEC_SPDIF_SEL                             0x160396
        /*
        1R1D:
            [7:5] SPDIF      / HDMI owner (MCU inform R2 to decide who is encode owner)
            000:  DEC-R2
            001:  SND-R2
            010:  SE-DSP-DD  / SE-DSP-DD
            011:  SE-DSP-PCM / SE-DSP-DDP

            [4:0] raw decoder selct

        2R1D:
            [7:5] SPDIF      / HDMI owner (MCU inform R2 to decide who is encode owner)
            000:  DEC-R2
            001:  SND-R2
            010:  SE-DSP-DD  / SE-DSP-DD
            011:  SE-DSP-DD  / SND-R2-DDP

            [4:0] raw decoder selct
        */

//---HDMI RX swtich to PCM Debounce---
#define CUSTOMIZED_PATCH_PARAM_HDMI_RX_SWITCH_TO_PCM_DEBOUNCE    1

//---HDMI RX Monitor Event CallBack---
#define CUSTOMIZED_PATCH_PARAM_HDMI_RX_MONITOR_EVENT_CALLBACK    1

//--------------------------------------------------------------------------------------
//config for different customized internal patch
//--------------------------------------------------------------------------------------
//---ADEC set gain, mute---
#define CUSTOMIZED_INTERNAL_PATCH_PARAM_ADEC_SET_GAIN 1
#define CUSTOMIZED_INTERNAL_PATCH_PARAM_ADEC_SET_MUTE 1

//---MS12 HDMI TX PCM ouput Auto Delay (for sync with NPCM)---
#define CUSTOMIZED_INTERNAL_PATCH_PARAM_MS12_HDMI_TX_PCM_AUTO_AUDIO_DELAY_ENABLE    0

//input: MS12 DD/DDP/DP, HDMI output type: DD
#define CUSTOMIZED_INTERNAL_PATCH_MS12_HDMI_TX_PCM_DD_IN_AUTO_AUDIO_DELAY     100   //ms
#define CUSTOMIZED_INTERNAL_PATCH_MS12_HDMI_TX_PCM_DDP_IN_AUTO_AUDIO_DELAY    100   //ms
#define CUSTOMIZED_INTERNAL_PATCH_MS12_HDMI_TX_PCM_DP_IN_AUTO_AUDIO_DELAY     100   //ms

//---General Buffer Dump---
#define CUSTOMIZED_INTERNAL_PATCH_PARAM_GENERAL_BUFFER_DUMP  1

//---SNDR2_MS12_PCMR_METADATA_SELECT---
#define CUSTOMIZED_INTERNAL_PATCH_PARAM_SNDR2_MS12_PCMR_METADATA_SELECT 1
#define SNDR2_MS12_MBOX_PCMR_METADATA_SELECT                            0x112E92

//---HW SRC Output Gain Compensate---
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_I2S_OUT    13
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_SPDIF_OUT  13
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_0_OUT 13
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_1_OUT 13
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_2_OUT 13
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_LINE_3_OUT 13
#define CUSTOMIZED_INTERNAL_PATCH_HW_SRC_GAIN_COMPENSATE_HDMI_OUT   13

//---DDP_71 Auto Bypass---
#define CUSTOMIZED_INTERNAL_PATCH_DDP_71_AUTO_BYPASS_ENABLE         1
#define CUSTOMIZED_INTERNAL_PATCH_DDP_71_AUTO_BYPASS_STB_HDMI_TX    0
#define CUSTOMIZED_INTERNAL_PATCH_DDP_71_AUTO_BYPASS_TV_HDMI_ARC    1

//---Avoid ADEC Pop Noise---
#define CUSTOMIZED_INTERNAL_PATCH_AVOID_ADEC_STOP_POP_NOISE     1
#define CUSTOMIZED_INTERNAL_PATCH_AVOID_ADEC_PAUSE_POP_NOISE    1

//---Play/pause Cmd Delay---
#define CUSTOMIZED_INTERNAL_PATCH_SET_PLAY_CMD_BY_ADEC_SOURCE_DELAY 1
#define CUSTOMIZED_INTERNAL_PATCH_PAUSE_DECODING_DELAY              1

//---DDPE SRS/DAP Auto Bypass---
#define CUSTOMIZED_INTERNAL_PATCH_DDPE_SRS_AUTO_BYPASS_ENABLE    0
#define CUSTOMIZED_INTERNAL_PATCH_DDPE_DAP_AUTO_BYPASS_ENABLE    0

//---LIMITED AUDIO_DELAY for DDPE--- [SE-DSP] AUDIO_DELAY_LOWER_BOUND in audio_comm2.h
#define CUSTOMIZED_INTERNAL_PATCH_LIMITED_AUDIO_DELAY_ENABLE    1
#define AUDIO_DELAY_LOWER_BOUND                                 0x30

//---R2_Active_Monitor---
#define CUSTOMIZED_INTERNAL_PATCH_R2_ACTIVE_MONITOR_ENABLE  0
#define REG_DECR2_ACK1                                      0xFFFFFF
#define REG_DECR2_ACK1_RESPONSE                             0xFF

//---Initial Codec to AC3---
#define CUSTOMIZED_INTERNAL_PATCH_ADEC0_INITIAL_CODEC_TYPE_AC3_ENABLE   1

//---Initial Dolby DRC mode---
#define CUSTOMIZED_INTERNAL_PATCH_ADEC0_INITIAL_DOLBY_DRC_LINE_MODE     1

//---ADC Fast charge---
#define CUSTOMIZED_INTERNAL_PATCH_STR_RESUME_ADC_FAST_CHARGE_ENABLE     0
#define REG_ADC_FAST_CHARGE                                             0x112CEE

#ifdef __cplusplus
}
#endif

#endif //_AUDIO_V2_CUSTOMER_CONFIG_H_

#endif // #ifdef ROLLS_ROYCE
