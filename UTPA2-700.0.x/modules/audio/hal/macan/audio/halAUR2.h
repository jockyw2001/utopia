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
#ifndef _AUR2_H_
#define _AUR2_H_

//  enum for r2 decoder command
typedef enum
{
    AU_DEC_R2_CMD_STOP                  = 0x0,
    AU_DEC_R2_CMD_PLAY                  = 0x1,
    AU_DEC_R2_CMD_PLAYFILETSP           = 0x2,
    AU_DEC_R2_CMD_PLAYUNI                  = 0x3,
    AU_DEC_R2_CMD_PLAYFILE              = 0x4,
    AU_DEC_R2_CMD_PLAY_AD               = 0x10,
    AU_DEC_R2_CMD_STARTBROWSE           = 0x20,
    AU_DEC_R2_CMD_PAUSE                 = 0x40,
    AU_DEC_R2_CMD_FREERUN               = 0x80,   //if not free_run, just AV_sync
} AU_DEC_R2_CMD;

typedef enum
{
    R2_DECCMD_STOP,
    R2_DECCMD_PAUSE,
    R2_DECCMD_PLAY,
} R2_DEC_PLAYCMD;

typedef enum
{
    adec_type_dummy,
    adec_type_ac3,
    adec_type_ac3p,
    adec_type_mpeg,
    adec_type_dts,
    adec_type_aac,
    adec_type_aacp,
    adec_type_xpcm,
    adec_type_vorbis,
    adec_type_flac,
    adec_type_wma,
    adec_type_wmap,
    adec_type_ra8,
    adec_type_gaac,
    adec_type_ext_pcm,
    adec_type_dolby_truhd_bypass,
    adec_type_dra,

    adec_type_reserved = 0x1F,          //Maxium Number of codec type
    adec_type_regmask = 0x1F,           //Maxium Bit use for register
} adec_type;

typedef enum
{
    adec_capa_mpeg = 1 << adec_type_mpeg,
    adec_capa_ac3  = 1 << adec_type_ac3,
    adec_capa_ac3p = 1 << adec_type_ac3p,
    adec_capa_dts  = 1 << adec_type_dts,
    adec_capa_aac  = 1 << adec_type_aac,
    adec_capa_aacp = 1 << adec_type_aacp,
    adec_capa_xpcm = 1 << adec_type_xpcm,
    adec_capa_vorbis = 1 << adec_type_vorbis,
    adec_capa_flac = 1 << adec_type_flac,
    adec_capa_wma = 1 << adec_type_wma,
    adec_capa_wmap = 1 << adec_type_wmap,
    adec_capa_ra8 = 1 << adec_type_ra8,
    adec_capa_gaac = 1 << adec_type_gaac,
    adec_capa_dra = 1<< adec_type_dra,
    adec_capa_reserved = 1 << adec_type_reserved,
} adec_capability;

typedef enum
{
    aenc_type_dummy,
    aenc_type_mpeg,
    aenc_type_ac3,
    aenc_type_dts,

    aenc_type_reserved = 0x1F,
} aenc_type;

typedef enum
{
    aenc_capa_mpeg = 1 << aenc_type_mpeg,
    aenc_capa_ac3  = 1 << aenc_type_ac3,
    aenc_capa_dts  = 1 << aenc_type_dts,

    aenc_capa_reserved = 1 << aenc_type_reserved,
} aenc_capability;

typedef enum
{
    aase_type_dummy,
    aase_type_srs,
    aase_type_srsTshd,
    aase_type_srsApollo,
    aase_type_srsTheater,

    aase_type_customer  = 0x1E,
    aase_type_reserverd = 0x1F,
} aase_type;

typedef enum
{
    aase_capa_srs     = 1 << aase_type_srs,
    aase_capa_srsTshd = 1 << aase_type_srsTshd,
    aase_capa_srsApollo = 1 << aase_type_srsApollo,
    aase_capa_srsTheater = 1 << aase_type_srsTheater,

    aase_capa_customer= 1 << aase_type_customer,
    aase_capa_reserved= 1 << aase_type_reserverd,
} aase_capability;

typedef enum
{
    adec_ctrl_stop,
    adec_ctrl_play,
    adec_ctrl_pause,
    adec_ctrl_ff2x,
    adec_ctrl_ff4x,
} adec_ctrl;

typedef enum
{
    aenc_ctrl_stop,
    aenc_ctrl_play,
} aenc_ctrl;

typedef enum
{
    adec_mode_stereo,
    adec_mode_LL,
    adec_mode_RR,
    adec_mode_mixed,

} adec_sound_mode;

typedef enum
{
    adec_drc_lineMode,
    adec_drc_RfMode,
} adec_drcMode;

typedef enum
{
    adec_dmx_LoRo,
    adec_dmx_LtRt,

} adec_dmxMode;

typedef enum
{
    adec_unlocked,
    adec_locked,
    adec_ip_check_fail,

} adec_status;

typedef enum
{
    adec_ch_mono,
    adec_ch_dual_mono,
    adec_ch_joint_stereo,
    adec_ch_stereo,
    adec_ch_multi,
} adec_acmod;

typedef enum
{
    adec_smpRate_48K,
    adec_smpRate_44K,
    adec_smpRate_32K,
} adec_smpRate;

typedef enum
{
    ADEC1   = 0x00,     //audio decoder 1
    ADEC2   = 0x01,     //audio decoder 2
    ADEC3   = 0x02,     //audio decoder 3

    ABSE    = 0x00,     //audio basic sound effect

    AASE1   = 0x00,     //audio advanced sound effect 1
    AASE2   = 0x01,     //audio advanced sound effect 2
    AASE3   = 0x02,     //audio advanced sound effect 3

    AIO     = 0x00,     //audio IO
} Audio_id;

typedef enum
{
    //***********************************************
    //  Audio decoder param
    //***********************************************
    AUR2_CommParam_ADEC_setAudioDecoder = 0x0000,
    AUR2_CommParam_ADEC_audioCtrl,
    AUR2_CommParam_ADEC_DualCtrl,
    AUR2_CommParam_ADEC_SoundMode,
    AUR2_CommParam_ADEC_dmxMode,
    AUR2_CommParam_ADEC_drcMode,
    AUR2_CommParam_ADEC_drcHighCut,
    AUR2_CommParam_ADEC_drcLowBosst,
    AUR2_CommParam_ADEC_mute,

        AUR2_CommParam_ADEC_mpeg_xxxx = 0x0100,
        AUR2_CommParam_ADEC_mpeg_xxxx2,

        AUR2_CommParam_ADEC_ac3_xxxx = 0x0200,
        AUR2_CommParam_ADEC_ac3_xxxx2,

        AUR2_CommParam_ADEC_aac_xxxx = 0x0300,
        AUR2_CommParam_ADEC_aac_xxxx2,

        AUR2_CommParam_ADEC_aacp_xxxx = 0x0400,
        AUR2_CommParam_ADEC_aacp_xxxx2,

        AUR2_CommParam_ADEC_wma_xxxx = 0x0500,
        AUR2_CommParam_ADEC_wma_xxxx2,

    AUR2_CommParam_ADEC_lastcmd = 0x3FFF,

    //***********************************************
    //  Advanced sound effect
    //***********************************************
    AUR2_CommParam_AASE_setAaseType = 0x4000,
    AUR2_CommParam_AASE_enableAase,

        AUR2_CommParam_AASE_SRSHD_HdEn  = 0x4100,
        AUR2_CommParam_AASE_SRSHD_truBassEn,
        AUR2_CommParam_AASE_SRSHD_definitionEn,
        AUR2_CommParam_AASE_SRSHD_dialogClarityEn,
        AUR2_CommParam_AASE_SRSHD_inputMode,
        AUR2_CommParam_AASE_SRSHD_outputMode,
        AUR2_CommParam_AASE_SRSHD_spkrSize,
        AUR2_CommParam_AASE_SRSHD_truBassCtrl,
        AUR2_CommParam_AASE_SRSHD_definitionCtrl,
        AUR2_CommParam_AASE_SRSHD_focusCtrl,
        AUR2_CommParam_AASE_SRSHD_surrLevel,
        AUR2_CommParam_AASE_SRSHD_inputGain,
        AUR2_CommParam_AASE_SRSHD_outputGain,

        AUR2_CommParam_AASE_SRS_Apollo_processEn    = 0x4200,
        AUR2_CommParam_AASE_SRS_Apollo_InputGain,
        AUR2_CommParam_AASE_SRS_Apollo_OutputGain,
        AUR2_CommParam_AASE_SRS_Apollo_BypassGain,
        AUR2_CommParam_AASE_SRS_Apollo_HeadroomGain,
        AUR2_CommParam_AASE_SRS_Apollo_HdEn = 0x4210,
        AUR2_CommParam_AASE_SRS_Apollo_HdOutputMode,
        AUR2_CommParam_AASE_SRS_Apollo_HdSndLevelCtrl,
        AUR2_CommParam_AASE_SRS_Apollo_HdInputGain,
        AUR2_CommParam_AASE_SRS_Apollo_HdOutputGain,
        AUR2_CommParam_AASE_SRS_Apollo_HdBypassGain,
        AUR2_CommParam_AASE_SRS_Apollo_HddialogClarityEn = 0x4220,
        AUR2_CommParam_AASE_SRS_Apollo_HddialogClarityCtrl,
        AUR2_CommParam_AASE_SRS_Apollo_HddefintionEn = 0x4230,
        AUR2_CommParam_AASE_SRS_Apollo_HddefintionCtrl,
        AUR2_CommParam_AASE_SRS_Apollo_HdtruBassEn = 0x4240,
        AUR2_CommParam_AASE_SRS_Apollo_HdtruBassProcessMode,
        AUR2_CommParam_AASE_SRS_Apollo_HdtruBassFrontCtrl,
        AUR2_CommParam_AASE_SRS_Apollo_HdtruBassSpkSize,
        AUR2_CommParam_AASE_SRS_Apollo_hardlimiterEn = 0x4250,
        AUR2_CommParam_AASE_SRS_Apollo_hardlimiterCtrl,
        AUR2_CommParam_AASE_SRS_Apollo_hardlimiterdelaylength,
        AUR2_CommParam_AASE_SRS_Apollo_hardlimiterBoost,
        AUR2_CommParam_AASE_SRS_Apollo_truvolumeEn = 0x4260,
        AUR2_CommParam_AASE_SRS_Apollo_truvolRefLevel,
        AUR2_CommParam_AASE_SRS_Apollo_truvolspkSize,
        AUR2_CommParam_AASE_SRS_Apollo_truvolInputGain,
        AUR2_CommParam_AASE_SRS_Apollo_truvolOutGain,
        AUR2_CommParam_AASE_SRS_Apollo_truvolMaxGain,
        AUR2_CommParam_AASE_SRS_Apollo_truvolBlockSize,
        AUR2_CommParam_AASE_SRS_Apollo_truvolnoiseManagerEn = 0x4270,
        AUR2_CommParam_AASE_SRS_Apollo_truvolnoiseManagerThd,
        AUR2_CommParam_AASE_SRS_Apollo_truvolCalibrate,
        AUR2_CommParam_AASE_SRS_Apollo_PEQleftEn = 0x4280,
        AUR2_CommParam_AASE_SRS_Apollo_PEQleftnumBands,
        AUR2_CommParam_AASE_SRS_Apollo_PEQleftBandEn = 0x4290,
        AUR2_CommParam_AASE_SRS_Apollo_PEQrightEn = 0x42A0,
        AUR2_CommParam_AASE_SRS_Apollo_PEQrightnumBands,
        AUR2_CommParam_AASE_SRS_Apollo_PEQrightBandEn = 0x42B0,
        AUR2_CommParam_AASE_SRS_Apollo_HPFEn = 0x42C0,
        AUR2_CommParam_AASE_SRS_Apollo_GEQEn = 0x42D0,
        AUR2_CommParam_AASE_SRS_Apollo_GEQBandGain1,
        AUR2_CommParam_AASE_SRS_Apollo_GEQBandGain2,
        AUR2_CommParam_AASE_SRS_Apollo_GEQBandGain3,
        AUR2_CommParam_AASE_SRS_Apollo_GEQBandGain4,
        AUR2_CommParam_AASE_SRS_Apollo_GEQBandGain5,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogEn = 0x42E0,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogInputGain,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogOutputGain,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogProcessGain,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogClarityEn = 0x42F0,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogClarityGain,
        AUR2_CommParam_AASE_SRS_Apollo_trudialogClarityThreshold,

        AUR2_CommParam_AASE_SRS_Theater_processEn    = 0x4300,
        AUR2_CommParam_AASE_SRS_Theater_InputGain,
        AUR2_CommParam_AASE_SRS_Theater_OutputGain,
        AUR2_CommParam_AASE_SRS_Theater_BypassGain,
        AUR2_CommParam_AASE_SRS_Theater_HeadroomGain,
        AUR2_CommParam_AASE_SRS_Theater_InputMode,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_Process_Path,
        AUR2_CommParam_AASE_SRS_Theater_HPFEn = 0x4310,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDEn = 0x4320,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDInputGain,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDOutGain,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDBypassGain,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDMode,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDMaxGainControl,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDNormalizerEnable,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDNormThresh,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDSmoothEnable,
        AUR2_CommParam_AASE_SRS_Theater_truvolHDLKFS,
        AUR2_CommParam_AASE_SRS_Theater_CSEn = 0x4330,
        AUR2_CommParam_AASE_SRS_Theater_CSInputGain,
        AUR2_CommParam_AASE_SRS_Theater_CSProcessingMode,
        AUR2_CommParam_AASE_SRS_Theater_CSLROutputGain,
        AUR2_CommParam_AASE_SRS_Theater_CSLsRsOutputGain,
        AUR2_CommParam_AASE_SRS_Theater_CSCenterOutputGain,
        AUR2_CommParam_AASE_SRS_Theater_trudialogEn = 0x4340,
        AUR2_CommParam_AASE_SRS_Theater_trudialogInputGain,
        AUR2_CommParam_AASE_SRS_Theater_trudialogOutputGain,
        AUR2_CommParam_AASE_SRS_Theater_trudialogBypassGain,
        AUR2_CommParam_AASE_SRS_Theater_trudialogProcessGain,
        AUR2_CommParam_AASE_SRS_Theater_trudialogClarityGain,

        AUR2_CommParam_AASE_SRS_Theater_TSHD_En = 0x4350,
        AUR2_CommParam_AASE_SRS_Theater_tshd_input_gain,
        AUR2_CommParam_AASE_SRS_Theater_tshd_output_gain,
        AUR2_CommParam_AASE_SRS_Theater_TSHD_Sur_En = 0x4360,
        AUR2_CommParam_AASE_SRS_Theater_tshd_surround_level_ctrl,
        AUR2_CommParam_AASE_SRS_Theater_TSHD_Definition_En = 0x4370,
        AUR2_CommParam_AASE_SRS_Theater_tshd_definition_ctrl,
        AUR2_CommParam_AASE_SRS_Theater_TSHD_DialogClarity_En = 0x4380,
        AUR2_CommParam_AASE_SRS_Theater_tshd_dialog_clarity_ctrl,
        AUR2_CommParam_AASE_SRS_Theater_TSHD_TrubassFront_En = 0x4390,
        AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_front_ctrl,
        AUR2_CommParam_AASE_SRS_Theater_TSHD_Trubass_level_indep_En = 0x43A0,
        AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_speaker_size,
        AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_compressor_ctrl,
        AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_process_mode,
        AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_speaker_audio,
        AUR2_CommParam_AASE_SRS_Theater_tshd_trubass_speaker_analysis,
        AUR2_CommParam_AASE_SRS_Theater_HPF_End_En = 0x43B0,
        AUR2_CommParam_AASE_SRS_Theater_HL_En = 0x43C0,
        AUR2_CommParam_AASE_SRS_Theater_HL_limit_ctrl,
        AUR2_CommParam_AASE_SRS_Theater_HL_boost_gain,

        AUR2_CommParam_AASE_SRS_Theater_CC3D_En = 0x43D0,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_DepthProcessing_En,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_3DSurroundBoost_En,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TSHD_Mix_En,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_Fade_En,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_InputGain,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_OutputGain,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_BypassGain,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TSHD_SurMode,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TSHD_MixFadeCtrl,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_Aperture,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GainLimit,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_FFDepth,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_NFDepth,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDx_En = 0x43E0,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxInputGain,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxBassLevel,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxSpeakerSize,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxMode,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxDynamics,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxHPOrder,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxCustomFilter,
        //AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxHighPassRatio,
        //AUR2_CommParam_AASE_SRS_Theater_CC3D_TBHDxExtendedBass,

        AUR2_CommParam_AASE_SRS_Theater_GEQ_5B_En = 0x43F0,
        AUR2_CommParam_AASE_Wall_Filter_En,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_input_gain,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band0,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band1,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band2,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band3,
        AUR2_CommParam_AASE_SRS_Theater_CC3D_GEQ_gain_band4,

        AUR2_CommParam_AASE_VDS_xxxx    = 0x4400,
        AUR2_CommParam_AASE_VDS_xxxx2,
        AUR2_CommParam_AASE_VDS_xxxx3,

        AUR2_CommParam_AASE_VSPK_xxxx    = 0x4500,
        AUR2_CommParam_AASE_VSPK_xxxx2,
        AUR2_CommParam_AASE_VSPK_xxxx3,

        AUR2_CommParam_AASE_PL2_xxxx    = 0x4600,
        AUR2_CommParam_AASE_PL2_xxxx2,
        AUR2_CommParam_AASE_PL2_xxxx3,

        AUR2_CommParam_AASE_DtsSymmetry_xxxx = 0x4700,
        AUR2_CommParam_AASE_DtsSymmetry_xxxx2,

        AUR2_CommParam_AASE_CV3_xxxx = 0x4800,
        AUR2_CommParam_AASE_CV3_xxxx2,

        AUR2_CommParam_AASE_CV4_xxxx = 0x4900,
        AUR2_CommParam_AASE_CV4_xxxx2,

        AUR2_CommParam_AASE_DOLBY_DAP_SET_PARAMS = 0x4A00,
        AUR2_CommParam_AASE_DOLBY_DAP_GET_PARAMS_R2_ADDR = 0x4A00,
        AUR2_CommParam_AASE_DOLBY_DAP_GET_PARAMS_SIZE = 0x4A01,

        AUR2_CommParam_AASE_SONICEMOTION_ABS3D_SET_PARAMS = 0x4B00,
        AUR2_CommParam_AASE_SONICEMOTION_ABS3D_GET_PARAMS_R2_ADDR = 0x4B00,
        AUR2_CommParam_AASE_SONICEMOTION_ABS3D_GET_PARAMS_SIZE = 0x4B01,

        AUR2_CommParam_AASE_CUSFUNC_xxxx  = 0x7E00,
        AUR2_CommParam_AASE_CUSFUNC_xxxx2,
        AUR2_CommParam_AASE_CUSFUNC_xxxx3,

    AUR2_CommParam_AASE_lastcmd = 0x7FFF,

    //***********************************************
    //  Basic sound effect
    //***********************************************
    AUR2_CommParam_ABSE_Volume  =         0x8000,
    AUR2_CommParam_ABSE_Prescale,
    AUR2_CommParam_ABSE_Treble,
    AUR2_CommParam_ABSE_Bass,
    AUR2_CommParam_ABSE_EQ,

    AUR2_CommParam_ABSE_lastcmd = 0x8FFF,

    //***********************************************
    //  Encoder
    //***********************************************
    AUR2_CommParam_AENC_setEncoder  =  0x9000,
    AUR2_CommParam_AENC_audioCtrl,

    AUR2_CommParam_AENC_lastcmd = 0x9FFF,

    //***********************************************
    //  IO
    //***********************************************
    AUR2_CommParam_AIO_spdifMode =        0xA000,

    AUR2_CommParam_AIO_lastcmd = 0xAFFF,

    //***********************************************
    //  Others
    //***********************************************
    AUR2_CommParam_Misc_country   = 0xB000,

} R2_AudioParamType;

typedef enum
{
    //***********************************************
    //  Audio decoder param
    //***********************************************
    AUR2_CommParam_ADEC_PTS_input = 0x0000,
    AUR2_CommParam_ADEC_updateEsWritePtr,
    AUR2_CommParam_ADEC_UNI_setOutput,

    AUR2_CommInfo_ADEC_capability,
    AUR2_CommInfo_ADEC_audioType,
    AUR2_CommInfo_ADEC_audioCtrl,
    AUR2_CommInfo_ADEC_decStatus,
    AUR2_CommInfo_ADEC_esLevel,
    AUR2_CommInfo_ADEC_pcmLevel,
    AUR2_CommInfo_ADEC_33bitPts,
    AUR2_CommInfo_ADEC_33Bit_stcPts_DIFF,
    AUR2_CommInfo_ADEC_acmode,
    AUR2_CommInfo_ADEC_DualCtrl,
    AUR2_CommInfo_ADEC_SoundMode,
    AUR2_CommInfo_ADEC_dmxMode,
    AUR2_CommInfo_ADEC_drcMode,
    AUR2_CommInfo_ADEC_drcHighCut,
    AUR2_CommInfo_ADEC_drcLowBosst,
    AUR2_CommInfo_ADEC_ok_frmCnt,
    AUR2_CommInfo_ADEC_err_frmCnt,
    AUR2_CommInfo_ADEC_sampleRate,
    AUR2_CommInfo_ADEC_bitRate,

        AUR2_CommInfo_ADEC_mpeg_xxxx = 0x0100,
        AUR2_CommInfo_ADEC_mpeg_xxxx2,

        AUR2_CommInfo_ADEC_ac3_xxxx = 0x0200,
        AUR2_CommInfo_ADEC_ac3_xxxx2,

        AUR2_CommInfo_ADEC_aac_xxxx = 0x0300,
        AUR2_CommInfo_ADEC_aac_xxxx2,

        AUR2_CommInfo_ADEC_aacp_xxxx = 0x0400,
        AUR2_CommInfo_ADEC_aacp_xxxx2,

    AUR2_CommInfo_ADEC_lastcmd = 0x3FFF,

    //***********************************************
    //  Advanced sound effect
    //***********************************************
    AUR2_CommInfo_AASE_AaseType = 0x4000,

        AUR2_CommInfo_AASE_SRS_xxxx      = 0x4100,
        AUR2_CommInfo_AASE_SRS_xxxx2,
        AUR2_CommInfo_AASE_SRS_xxxx3,

        AUR2_CommInfo_AASE_SRSHD_xxxx    = 0x4200,
        AUR2_CommInfo_AASE_SRSHD_xxxx2,
        AUR2_CommInfo_AASE_SRSHD_xxxx3,

        AUR2_CommInfo_AASE_SRS_Apollo_xxxx    = 0x4300,
        AUR2_CommInfo_AASE_SRS_Apollo_xxxx2,
        AUR2_CommInfo_AASE_SRS_Apollo_xxxx3,

        AUR2_CommInfo_AASE_VDS_xxxx    = 0x4400,
        AUR2_CommInfo_AASE_VDS_xxxx2,
        AUR2_CommInfo_AASE_VDS_xxxx3,

        AUR2_CommInfo_AASE_VSPK_xxxx    = 0x4500,
        AUR2_CommInfo_AASE_VSPK_xxxx2,
        AUR2_CommInfo_AASE_VSPK_xxxx3,

        AUR2_CommInfo_AASE_PL2_xxxx    = 0x4600,
        AUR2_CommInfo_AASE_PL2_xxxx2,
        AUR2_CommInfo_AASE_PL2_xxxx3,

        AUR2_CommInfo_AASE_DtsSymmetry_xxxx = 0x4700,
        AUR2_CommInfo_AASE_DtsSymmetry_xxxx2,

        AUR2_CommInfo_AASE_CV3_xxxx = 0x4800,
        AUR2_CommInfo_AASE_CV3_xxxx2,

        AUR2_CommInfo_AASE_CV4_xxxx = 0x4900,
        AUR2_CommInfo_AASE_CV4_xxxx2,

        AUR2_CommInfo_AASE_CUSFUNC_xxxx  = 0x7E00,
        AUR2_CommInfo_AASE_CUSFUNC_xxxx2,
        AUR2_CommInfo_AASE_CUSFUNC_xxxx3,

    AUR2_CommInfo_AASE_lastcmd = 0x7FFF,

    //***********************************************
    //  Basic sound effect
    //***********************************************
    AUR2_CommInfo_ABSE_Volume  =         0x8000,
    AUR2_CommInfo_ABSE_Prescale,
    AUR2_CommInfo_ABSE_Treble,
    AUR2_CommInfo_ABSE_Bass,
    AUR2_CommInfo_ABSE_EQ,

    AUR2_CommInfo_ABSE_lastcmd = 0x8FFF,

    //***********************************************
    //  Encoder
    //***********************************************
    AUR2_CommInfo_AENC_xxxx  =  0x9000,
    AUR2_CommInfo_AENC_xxxx2,

    AUR2_CommInfo_AENC_lastcmd = 0x9FFF,

    //***********************************************
    //  IO
    //***********************************************
    AUR2_CommInfo_AIO_xxxx =        0xA000,

    AUR2_CommInfo_AIO_lastcmd = 0xAFFF,

    //***********************************************
    //  Others
    //***********************************************
    AUR2_CommInfo_Misc_country   = 0xB000,

} R2_AudioInfoType;

typedef enum
{
    R2_SHM_INFO_CAPABILITY,
    R2_SHM_INFO_DEC_STAUS,
    R2_SHM_INFO_DEC_ERROR_ID,
    R2_SHM_INFO_SMP_RATE,
    R2_SHM_INFO_BIT_RATE,
    R2_SHM_INFO_SYNTH_RATE,
    R2_SHM_INFO_DEC_CHANNEL_MODE,
    R2_SHM_INFO_DEC_CHANNEL_MAPPING,

    R2_SHM_INFO_ES_LEVEL,
    R2_SHM_INFO_PCM_LEVEL,
    R2_SHM_INFO_PCM_DMX_LEVEL,
    R2_SHM_INFO_ES_RD_PTR,                  //a
    R2_SHM_INFO_PCM_WR_PTR,
    R2_SHM_INFO_MM_FILE_REQ_SIZE,           //c

    R2_SHM_INFO_ACCUM_ES_BYTECNT,
    R2_SHM_INFO_WAIT_1STPTS_CNT,
    R2_SHM_INFO_WAIT_STC_CNT,
    R2_SHM_INFO_SMP_FLUSH_CNT,

    R2_SHM_INFO_AVSYNC_STATE,
    R2_SHM_INFO_AVSYNC_FREERUN_TYPE,
    R2_SHM_INFO_AVSYNC_OFFSET,

    R2_SHM_INFO_PTS_TAG_WR_IDX,
    R2_SHM_INFO_PTS_TAG_RD_IDX,

    R2_SHM_INFO_PLAY_STATE,
    R2_SHM_INFO_DEC_TYPE,
    R2_SHM_INFO_PLAYSMPFLAG,
    R2_SHM_INFO_DEC_CALLING_CNT,
    R2_SHM_INFO_RECEIVE_STOP_CNT,

    R2_SHM_INFO_OK_FRMCNT,
    R2_SHM_INFO_ERR_FRMCNT,
    R2_SHM_INFO_SKIP_FRMCNT,
    R2_SHM_INFO_REPT_FRMCNT,
    R2_SHM_INFO_SYNC_MISS_CNT,
    R2_SHM_INFO_PCM_EMPTY_CNT,

    R2_SHM_INFO_STC,
    R2_SHM_INFO_PTS,
    R2_SHM_INFO_CURR_PTS,
    R2_SHM_INFO_TD,
    R2_SHM_INFO_PTS_LATENCY,

    R2_SHM_INFO_UNI_DECODE_DONE_CNT,
    R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR,
    R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE,

    R2_SHM_INFO_PCM_ADDR,
    R2_SHM_INFO_PCM_SIZE,

    R2_SHM_INFO_spdifbuf_LEVEL,
    R2_SHM_INFO_spdifbuf_WR_PTR,

    R2_SHM_INFO_DOLBY_DEC_TYPE,
    R2_SHM_INFO_DOLBY_FRAME_SIZE,
    R2_SHM_INFO_DOLBY_BS_MODE,
    R2_SHM_INFO_DOLBY_AAC_TYPE,
    R2_SHM_INFO_DOLBY_DDP_71_STREAM,

    R2_SHM_INFO_DTS_CD_MODE,
    R2_SHM_INFO_DTS_ES_TYPE,
    R2_SHM_INFO_MPEG_LAYER,
    R2_SHM_INFO_MPEG_STEREOMODE,
    R2_SHM_INFO_MPEG_HEADER,
    R2_SHM_INFO_UNSUPPORT_TYPE,
    R2_SHM_INFO_TRANSCODE_FLAG,

    R2_SHM_INFO_SPDIFTx_SMP_RATE,
    R2_SHM_INFO_HDMITx_SMP_RATE,

    R2_SHM_INFO_PCM_OUTPUT_CHANNEL,
    R2_SHM_INFO_OMX_MCH_VALID,

    /* ouput channel info  */
    R2_SHM_INFO_CH_MPEG,
    R2_SHM_INFO_CH_AC3,
    R2_SHM_INFO_CH_AAC,
    R2_SHM_INFO_CH_DTS,
    R2_SHM_INFO_CH_DTS_HD,
    R2_SHM_INFO_CH_DTS_LBR,
    R2_SHM_INFO_CH_WMA,
    R2_SHM_INFO_CH_DRA,
    R2_SHM_INFO_CH_COOK,
    R2_SHM_INFO_CH_XPCM,
    R2_SHM_INFO_CH_VORBIS,
} R2_SHM_INFO_TYPE;

typedef enum
{
    R2_SHM_PARAM_ES_WR_PTR,
    R2_SHM_PARAM_MPG_AVOFFSET,
    R2_SHM_PARAM_AC3_AVOFFSET,
    R2_SHM_PARAM_AAC_AVOFFSET,

    R2_SHM_PARAM_DOLBY_DRC_MODE,
    R2_SHM_PARAM_DOLBY_RF_MODE,
    R2_SHM_PARAM_DOLBY_DMX_MODE,
    R2_SHM_PARAM_DOLBY_HIGH_CUT,
    R2_SHM_PARAM_DOLBY_LOW_BOOST,
    R2_SHM_PARAM_DOLBY_GAIN,
    R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER,
    R2_SHM_PARAM_DOLBY_AAC_BYPASS,
    R2_SHM_PARAM_DOLBY_DDP_HDMI_BYPASS,
    R2_SHM_PARAM_DOLBY_TB11_ENABLE,
    R2_SHM_PARAM_DOLBY_MIXER_BALANCE,

    R2_SHM_PARAM_PARSER_SPEC,
    R2_SHM_PARAM_PARSER_PES_BYPASS,
    R2_SHM_PARAM_PARSER_PUSI_DISABLE,
    R2_SHM_PARAM_REPORT_MM_TS_PTS_WITH_MSB,
    R2_SHM_PARAM_MM_TS_SYNC_STC,
    R2_SHM_PARAM_MM_FF2X,
    R2_SHM_PARAM_DECODE_MUTE,
    R2_SHM_PARAM_SOUND_MODE,
    R2_SHM_PARAM_EXT_SMPRATE_CTRL,

    R2_SHM_PARAM_MM_FILE_REQ_SIZE,
    R2_SHM_PARAM_MM_INPUT_PTS,

    R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT,
    R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT,
    R2_SHM_PARAM_ES2_WR_PTR,

    R2_SHM_PARAM_OMX_SPDIF_CTRL,
    R2_SHM_PARAM_OMX_SPDIF_PCM_LEVEL,

    R2_SHM_PARAM_MAIN_VOLUME,
    R2_SHM_PARAM_MAIN_VOLUME_MUTE,
    R2_SHM_PARAM_AD_VOLUME,
    R2_SHM_PARAM_AD_VOLUME_MUTE,

    R2_SHM_PARAM_MPEG_SOUNDMODE,
    R2_SHM_PARAM_MPEG_GAIN,

    R2_SHM_PARAM_COOK_NUMCODECS,
    R2_SHM_PARAM_COOK_SAMPLES,
    R2_SHM_PARAM_COOK_SAMPLERATES,
    R2_SHM_PARAM_COOK_CHANNELS,
    R2_SHM_PARAM_COOK_REGIONS,
    R2_SHM_PARAM_COOK_CPLSTART,
    R2_SHM_PARAM_COOK_CPLQBITS,
    R2_SHM_PARAM_COOK_FRAMESIZE,

    R2_SHM_PARAM_DTS_DMX_LFE_ENABLE,
    R2_SHM_PARAM_DTS_DMX_MODE,

    R2_SHM_PARAM_ASF_VERSION,
    R2_SHM_PARAM_ASF_CHANNELS,
    R2_SHM_PARAM_ASF_SAMPLERATE,
    R2_SHM_PARAM_ASF_BYTERATE,
    R2_SHM_PARAM_ASF_BLOCKALIGN,
    R2_SHM_PARAM_ASF_ENCOPT,
    R2_SHM_PARAM_ASF_PARSINGBYAPP,
    R2_SHM_PARAM_ASF_BITS_PER_SAMPLE,
    R2_SHM_PARAM_ASF_CHANNELMASK,
    R2_SHM_PARAM_ASF_DRC_PARAM_EXIST,
    R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF,
    R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET,
    R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF,
    R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET,
    R2_SHM_PARAM_ASF_MAX_PACKET_SIZE,

    R2_SHM_PARAM_XPCM_TYPE,
    R2_SHM_PARAM_XPCM_CHANNELS,
    R2_SHM_PARAM_XPCM_SAMPLERATES,
    R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE,
    R2_SHM_PARAM_XPCM_BLOCK_SIZE,
    R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK,

    R2_SHM_PARAM_HASH_KEY,

    R2_SHM_PARAM_SPDIF_TYPE,

    R2_SHM_PARAM_RFSIGNAL_TYPE,

    R2_SHM_PARAM_VORBIS_HEADER_SIZE,

    R2_SHM_PARAM_TTS_EN,

    R2_SHM_PARAM_ES_Limiter_EN,
    R2_SHM_PARAM_ES_Limiter_Threshold,
    R2_SHM_PARAM_PCM_Limiter_EN,
    R2_SHM_PARAM_PCM_Limiter_Threshold,
    R2_SHM_PARAM_STC_SELECT,

    R2_SHM_PARAM_MULTI_CHANNEL,
    R2_SHM_PARAM_DAP_EN,
    R2_SHM_PARAM_SPEAKER_EN,
    R2_SHM_PARAM_DDP_ENCODE_EN,

    R2_SHM_PARAM_ES_PassThrough_DelaySmp,
    R2_SHM_PARAM_ES_PassThrough_PcmSamRate,
    R2_SHM_PARAM_ES_PassThrough_HdmiSmpRate,

} R2_SHM_PARAM_TYPE;

MS_U8   HAL_AUR2_ReadByte(MS_U32 u32RegAddr);
MS_U16  HAL_AUR2_ReadReg(MS_U32 u32RegAddr);
void    HAL_AUR2_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
void    HAL_AUR2_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
void    HAL_AUR2_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);
void    HAL_AUR2_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);

void HAL_DEC_R2_EnableR2( MS_BOOL en );
void HAL_DEC_R2_init_SHM_param( void );
void HAL_DEC_R2_SetCommInfo(R2_AudioParamType infoType, Audio_id id, MS_U16 param1, MS_U16 param2);
MS_U32 HAL_DEC_R2_GetCommInfo(R2_AudioInfoType infoType, Audio_id id, MS_U16 param1, MS_U16 param2);
MS_U32 HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TYPE decInfoType, MS_U8 dec_id );
MS_BOOL HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param, MS_U32 param2 );
MS_U32 HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param );

void HAL_SND_R2_EnableR2( MS_BOOL en );
void HAL_SND_R2_SetCommInfo(R2_AudioParamType infoType, Audio_id id, MS_U16 param1, MS_U16 param2);
MS_U32 HAL_SND_R2_GetCommInfo(R2_AudioInfoType infoType, Audio_id id, MS_U16 param1, MS_U16 param2);
MS_U32 HAL_SND_R2_Get_SHM_INFO(R2_SHM_INFO_TYPE decInfoType, MS_U8 dec_id );
MS_BOOL HAL_SND_R2_Set_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param, MS_U32 param2 );
MS_U32 HAL_SND_R2_Get_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param );


#endif
