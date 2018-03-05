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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_AUDIO_INTERNAL_H_
#define _DRV_AUDIO_INTERNAL_H_

#include "MsTypes.h"
#include "ULog.h"
#include "apiAUDIO.h"
#include "drvAUDIO_if.h"

#ifdef __cplusplus
extern "C"
{
#endif

//=====================================================
//  Macro and Define
//=====================================================
#define AUDIO_DO_ALIGNMENT(Value, AlignmentSize) \
    do { \
        Value = (Value /AlignmentSize) * AlignmentSize; \
    } while(0)

//====== DSP code Type========
typedef enum
{
    AU_DVB_STANDARD_INVALID             = -1,

    AU_DVB_NONE                         = 0x0,
    AU_DVB_STANDARD_MPEG,
    AU_DVB_STANDARD_AC3,
    AU_DVB_STANDARD_AC3P,
    AU_DVB_STANDARD_AAC,
    AU_DVB_STANDARD_MP3,
    AU_DVB_STANDARD_WMA,
    AU_DVB_STANDARD_RA8LBR,
    AU_DVB_STANDARD_XPCM,
    AU_DVB_STANDARD_TONE,
    AU_DVB_STANDARD_DTS,
    AU_DVB_STANDARD_MS10_DDT,
    AU_DVB_STANDARD_MS10_DDC,
    AU_DVB_STANDARD_WMA_PRO,
    AU_DVB_STANDARD_FLAC,
    AU_DVB_STANDARD_VORBIS,
    AU_DVB_STANDARD_DTSLBR,
    AU_DVB_STANDARD_AMR_NB,
    AU_DVB_STANDARD_AMR_WB,
    AU_DVB_STANDARD_DRA,
    AU_DVB_STANDARD_MPEG_AD,
    AU_DVB_STANDARD_DolbyTrueHD_Bypass,
    AU_DVB_STANDARD_DTSHD_ADO,
    AU_DVB_MAX,

    AU_DVB_STANDARD_ENC,
    AU_DVB_STANDARD_MPEG_EN,

    AU_DVB2_NONE,
    AU_DVB2_STANDARD_MPEG_AD ,
    AU_DVB2_STANDARD_AC3_AD,
    AU_DVB2_STANDARD_AC3P_AD,
    AU_DVB2_STANDARD_AAC_AD,
    AU_DVB2_STANDARD_DDE,
    AU_DVB2_STANDARD_GAAC_AD,
    AU_DVB2_STANDARD_MP3,
    AU_DVB2_STANDARD_MPEG_EN,
    AU_DVB2_STANDARD_XPCM,
    AU_DVB2_STANDARD_KTV,
    AU_DVB2_STANDARD_KTV2,
    AU_DVB2_STANDARD_DTS,
    AU_DVB2_STANDARD_TONEGEN,
    AU_DVB2_STANDARD_DolbyTrueHD_Bypass,
    AU_DVB2_STANDARD_DTSHD_ADO,
    AU_DVB2_MAX,

    AU_DVB2_ADVSND_NONE,
    AU_DVB2_ADVSND_PL2,
    AU_DVB2_ADVSND_BBE,
    AU_DVB2_ADVSND_SRS,
    AU_DVB2_ADVSND_VDS,
    AU_DVB2_ADVSND_VSPK,
    AU_DVB2_ADVSND_SUPV,
    AU_DVB2_ADVSND_TSHD,
    AU_DVB2_ADVSND_DTS_ULTRA_TV,
    AU_DVB2_ADVSND_TSHDVIQ,
    AU_DVB2_ADVSND_AUDSY,
    AU_DVB2_ADVSND_DBX,
    AU_DVB2_ADVSND_THEATERSOUND,
    AU_DVB2_ADVSND_PURESND,
    AU_DVB2_ADVSND_STUDIOSOUND_3D,
    AU_DVB2_ADVSND_BONGIOVI_DPS,
    AU_DVB2_ADVSND_SONICMOTION_ABS3D,
    AU_DVB2_ADVSND_DOLBY_DAP,
    AU_DVB2_ADVSND_MAX,

    AU_DEC_SYSTEM,
    AU_SE_SYSTEM,

    AU_KTV_ENC,
    AU_KTV_FUNC,
    AU_KTV_SYSTEM,

    AU_SND_EFFECT,

    AU_SIF_NONE,
    AU_SIF_BTSC,
    AU_SIF_EIAJ,
    AU_SIF_PALSUM,
    AU_SIF_FM_RADIO,
    AU_SIF_MAX,

    AU_ATVENC_NONE,
    AU_ATVENC_FMTx,
    AU_ATVENC_BTSC,

    AU_DVB2_ENCODE_NONE,    // define for snd_dsp encode code for because 0x30 has no free enum
    AU_DVB2_ENCODE_DTSE,
    AU_DVB2_ENCODE_MS10_DDE,
    AU_DVB2_ENCODE_DDE,
    AU_DVB2_ENCODE_MAX,

    AU_DEC_ES_BYPASS,

}AUDIO_DSP_CODE_TYPE;

#define IS_AUDIO_DSP_CODE_TYPE_DEC1_SYSTEM(dspCodeType)  ((dspCodeType >= AU_DVB_NONE) && (dspCodeType < AU_DVB_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_DEC2_SYSTEM(dspCodeType)  ((dspCodeType >= AU_DVB2_NONE) && (dspCodeType < AU_DVB2_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_ADVSND(dspCodeType)       ((dspCodeType >= AU_DVB2_ADVSND_NONE) && (dspCodeType < AU_DVB2_ADVSND_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_DEC_SYSTEM(dspCodeType)   (dspCodeType == AU_DEC_SYSTEM)
#define IS_AUDIO_DSP_CODE_TYPE_SE_SYSTEM(dspCodeType)    (dspCodeType == AU_SE_SYSTEM)
#define IS_AUDIO_DSP_CODE_TYPE_SIF(dspCodeType)          ((dspCodeType >= AU_SIF_NONE) && (dspCodeType < AU_SIF_MAX))
#define IS_AUDIO_DSP_CODE_TYPE_SE_ENC(dspCodeType)       ((dspCodeType >= AU_DVB2_ENCODE_NONE) && (dspCodeType < AU_DVB2_ENCODE_MAX))

//=====================================================
// Define
//=====================================================
#define MAX_AUDIO_BASE_ADDRESS_NUM2     3
#define AUDIO_PCM_MAXNUM                32
#define AUDIO_ENCODER_MAXNUM            2

//==============================
//  IP Security Define
//==============================
#define  AU_DVB2_AUTHBIT_BBE            0
#define  AU_DVB2_AUTHBIT_SRS            2
#define  AU_DVB2_AUTHBIT_TSHD           3
#define  AU_DVB2_AUTHBIT_VIQ            4
#define  AU_DVB2_AUTHBIT_VDS            5
#define  AU_DVB2_AUTHBIT_VSPK           6
#define  AU_DVB2_AUTHBIT_AUDSY          9
// Dolby DD
#define  AU_DVB_AUTHBIT_AC3             11
#define  AU_DVB2_AUTHBIT_AC3_AD         11
// Dolby DDP
#define  AU_DVB_AUTHBIT_AC3P            12
#define  AU_DVB2_AUTHBIT_AC3P_AD        12
// Dolby DDCO
#define  AU_DVB2_AUTHBIT_DDE            13
// Dolby Pulse(DDT)
#define  AU_DVB_AUTHBIT_MS10_DDT        14
// DTS DMP (Core 2.0)
#define  AU_DVB_AUTHBIT_DTSDEC          15
#define  AU_DVB2_AUTHBIT_DTSDEC         15
// DTS HD
#define  AU_DVB_AUTHBIT_DTSHD           18
// COOK(RA8-LBR) same with RM video
#define  AU_DVB_AUTHBIT_COOK            28
// WMA
#define  AU_DVB_AUTHBIT_WMA             30
#define  AU_DVB2_AUTHBIT_WMA            30
// SRS StudioSound
#define  AU_DVB2_AUTHBIT_SRS_StudioSND  55
// Dolby Volume
#define  AU_DVB2_AUTHBIT_DolbyVolume    56
// Dolby DD Plus Encode
#define  AU_DVB2_AUTHBIT_DDP_ENC        57
// DTS LBR
#define  AU_DVB_AUTHBIT_DTS_LBR         58
#define  AU_DVB2_AUTHBIT_DTS_LBR        58
// DTS XLL
#define  AU_DVB_AUTHBIT_DTS_XLL         59
#define  AU_DVB2_AUTHBIT_DTS_XLL        59
#define  AU_DVB2_AUTHBIT_DBX            64
#define  AU_DVB_AUTHBIT_DRA             65
// SRS PureSound
#define  AU_DVB2_AUTHBIT_SRS_PURESND    66
//DTS StudioSound 3D(=TheaterSound3D)
#define  AU_DVB2_AUTHBIT_DTS_StudioSound3D  69
#define  AU_DVB_AUTHBIT_GAAC            70
#define  AU_DVB_AUTHBIT_DTSNeoUltra     72
#define  AU_DVB2_AUTHBIT_DTSNeoUltra    72
#define  AU_DVB_AUTHBIT_DTSE            73
#define  AU_DVB2_AUTHBIT_DTSE           73
#define  AU_DVB_AUTHBIT_DOLBY_MS11      80
#define  AU_DVB2_AUTHBIT_DOLBY_MS11     80
#define  AU_DVB_AUTHBIT_DOLBY_MS12_B    81
#define  AU_DVB_AUTHBIT_DOLBY_MS12_D    82
#define  AU_DVB_AUTHBIT_DOLBY_MS12_LC   83
#define  AU_DVB2_AUTHBIT_SONICMOTION_ABS3D  92
#define  AU_DVB2_AUTHBIT_BONGIOVI_DPS   93
#define  AU_DVB2_AUTHBIT_NULL           126
#define  AU_DVB_AUTHBIT_DEMOMODE        127
#define  AU_DVB2_AUTHBIT_DOLBY_MS11_WO_AAC       115
#define  AU_DVB2_AUTHBIT_DOLBY_MS12_B_WO_AAC     116
#define  AU_DVB2_AUTHBIT_DOLBY_MS12_D_WO_AAC     117

#define AUDIO_PRINT(level, fmt, args...) \
    do { \
        if ( g_AudioVars2 != NULL) \
        { \
            if ( level <= g_AudioVars2->g_debug_level) \
            { \
                switch(level) \
                { \
                    case AUDIO_DEBUG_LEVEL_FATAL: \
                        ULOGF("AUDIO", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_ERROR: \
                        ULOGE("AUDIO", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_WARN: \
                        ULOGW("AUDIO", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_INFO: \
                        ULOGI("AUDIO", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_DEBUG: \
                        ULOGD("AUDIO", fmt, ## args); \
                        break; \
                    case AUDIO_DEBUG_LEVEL_VERBOSE: \
                        ULOGD("AUDIO", fmt, ## args); \
                        break; \
                    default : \
                        break; \
                } \
            } \
        } \
    } while(0)


//=====================================================
// Structure
//=====================================================


//=====================================================
// Enum
//=====================================================
typedef enum
{
    AU_CHIP_T2,
    AU_CHIP_U3,
    AU_CHIP_T3,
    AU_CHIP_T4,
    AU_CHIP_T7,                         //single DSP
    AU_CHIP_JANUS,                      //single DSP
    AU_CHIP_U4,
    AU_CHIP_T8,
    AU_CHIP_T9,
    AU_CHIP_T10,
    AU_CHIP_T11,
    AU_CHIP_T13,
    AU_CHIP_T12,
    AU_CHIP_J2,
    AU_CHIP_K1,
    AU_CHIP_K2,
    AU_CHIP_A1,
    AU_CHIP_A5,
    AU_CHIP_A6,
    AU_CHIP_A7,
    AU_CHIP_A3,
    AU_CHIP_A5P,
    AU_CHIP_AMETHYST,
    AU_CHIP_EAGLE,
    AU_CHIP_EDISON,
    AU_CHIP_EMERALD,
    AU_CHIP_M12,
    AU_CHIP_EINSTEIN,
    AU_CHIP_EIFFEL,
    AU_CHIP_NIKE,
    AU_CHIP_Euler,
    AU_CHIP_NUGGET,
    AU_CHIP_NIKON,
    AU_CHIP_EINSTEIN3,
    AU_CHIP_NAPOLI,
    AU_CHIP_MONACO,
    AU_CHIP_MADISON,
    AU_CHIP_MIAMI,
    AU_CHIP_CLIPPERS,
    AU_CHIP_MUJI,
    AU_CHIP_KERES,
    AU_CHIP_MUNICH,
}AU_CHIP_TYPE;

typedef enum
{
    AUDIO_ETMODE_DTV,     // DTV Mode
    AUDIO_ETMODE_KTV,     // KTV Mode 1
    AUDIO_ETMODE_GAME,  // GAME Mode
    AUDIO_ETMODE_KTV2,   // KTV Mode 2
    AUDIO_ETMODE_VOIP_ON,
    AUDIO_ETMODE_VOIP_OFF,
    AUDIO_ETMODE_KTV_STB, // STB KTV Mode, forward mix ch.6 and ch.7
}AUDIO_ENTERTAINMENT_MODE;

// for dvb2 decoder command
typedef enum
{
    AU_DVB2_DECCMD_STOP,                //0
    AU_DVB2_DECCMD_PLAY,                //1
    AU_DVB2_DECCMD_PLAYFILETSP          = 2,
    AU_DVB2_DECCMD_RESYNC,
    AU_DVB2_DECCMD_PLAYFILE             = 4,
    AU_DVB2_DECCMD_PAUSE                = 6
}AU_DVB2_DECCMD;

// for dvb2 free run mode
typedef enum
{
    AU_DVB2_FreeRunMode_AVsync,         //0
    AU_DVB2_FreeRunMode_FreeRun,        //1
}AU_DVB2_FreeRunMode;

typedef enum
{
    DSP_MEM_TYPE_PM,                    // DSP PM memory
    DSP_MEM_TYPE_DM                     // DSP DM memory
}AUDIO_DSP_MEMORY_TYPE;

typedef enum
{
    AUDIO_DTV_DVB,
    AUDIO_DTV_ATSC,
    AUDIO_DTV_ISDB,
    AUDIO_DTV_DMBT,
    AUDIO_DTV_OTHER = 15
}AUDIO_DTV_SYSTEM;

typedef enum
{
    AUDIO_MULTICH_PCM_DMXL,
    AUDIO_MULTICH_PCM_DMXR,
    AUDIO_MULTICH_PCM_L,
    AUDIO_MULTICH_PCM_C,
    AUDIO_MULTICH_PCM_R,
    AUDIO_MULTICH_PCM_LS,
    AUDIO_MULTICH_PCM_RS,
    AUDIO_MULTICH_PCM_LFE,
    AUDIO_MULTICH_PCM_LRS,
    AUDIO_MULTICH_PCM_RRS,
}AUDIO_MULTICH_PCM_TYPE;

typedef enum
{
    /* Set Cmd */
    AUDIO_ENCODER_IOCTL_SET_READ_PTR_OFFSET,

    /* Get Cmd */
    AUDIO_ENCODER_IOCTL_GET_WRITE_PTR_OFFSET,
    AUDIO_ENCODER_IOCTL_GET_THRESHOLD,
    AUDIO_ENCODER_IOCTL_GET_FRAME_SIZE,
    AUDIO_ENCODER_IOCTL_GET_AVAILABLE_SIZE,
    AUDIO_ENCODER_IOCTL_GET_FRAME_COUNT,
    AUDIO_ENCODER_IOCTL_GET_TIMESTAMP_HIGH32BIT,
    AUDIO_ENCODER_IOCTL_GET_TIMESTAMP_LOW32BIT,
}AUDIO_ENCODER_IOCTL_CMD;

typedef struct
{
    MS_S32 (*open)(void *pData);
    MS_S32 (*close)(void);
    MS_S32 (*start)(void);
    MS_S32 (*stop)(void);
    MS_S32 (*set)(MS_U32 u32Cmd, const void *pData);
    MS_S32 (*get)(MS_U32 u32Cmd, void *pData);
    MS_U32 (*read)(void *pBuf, MS_U32 u32Size);
    MS_U32 (*write)(const void *pBuf, MS_U32 u32Size);
    MS_S32 (*flush)(void);
} AUDIO_PCM_OPS_t;

typedef struct
{
    MS_U8 u8MultiChFlag;
    MS_U8 u8MixingFlag;
    MS_U8 u8CaptureFlag;
    MS_U32 u32Channel[16];
    MS_U32 u32SampleRate[32];
    MS_U32 u32BitWidth[8];
} AUDIO_PCM_CAPS_t;

typedef struct
{
    MS_S8 *pBuffer;
    MS_S8 *pReadPtr;
    MS_S8 *pWritePtr;
    MS_U32 u32BufferSize;
    MS_U32 u32BufferHighThreshold;
    MS_U32 u32RemainSize;
} AUDIO_PCM_BUFFER_INFO_t;

typedef struct
{
    MS_U8 u8Name[AUDIO_COMMON_PCM_IO_NAME_LENGTH]; /* PCM name (only 32 bytes, if change this length, must change u8Name of AUDIO_PCM_INFO_t as well!) */
    AUDIO_PCM_OPS_t tPcmOps;  /* PCM operators */
    AUDIO_PCM_CAPS_t tPcmCaps;  /* PCM operators */
    AUDIO_PCM_BUFFER_INFO_t tPcmBufferInfo;  /* PCM buffer information */
    AUDIO_PCM_INFO_t *pPcmInfo; /* PCM information */
    MS_U8 u8SettingChangeFlag;
    MS_U32 u32RemainSize;
} AUDIO_PCM_t;

typedef struct
{
    MS_S32 (*open)(void);
    MS_S32 (*close)(void);
    MS_S32 (*start)(void);
    MS_S32 (*stop)(void);
    MS_S32 (*ioctl)(MS_U32 u32Cmd, void *pData);
    MS_U32 (*read)(void *pBuf, MS_U32 u32Size);
    MS_S32 (*flush)(void);
} AUDIO_ENCODER_OPS_t;

typedef struct __attribute__((packed))
{
    MS_BOOL          bDecInUsed;
    AUDIO_USAGE_TYPE eUsageType;
    MMA_AudioType    eAudioType;
} AUDIO_DSP_USAGE;

typedef struct
{
    MS_U32  g_ShmID;

    MS_BOOL g_bDecPlayFileFlag;   // DEC MM interrupt flag
    MS_BOOL g_bSePlayFileFlag;    // SE  MM interrupt flag
    MS_BOOL g_bMMPlayFlag;        // MM  play flag
    MS_BOOL g_Audio_PreInitFlag;
    MS_BOOL g_Audio_InitFlag;
    MS_BOOL g_SPDIF_MuteStatus;
    AUDIO_SPDIF_OUTPUT_TYPE g_eSpdifOutputType;
    MS_U8   g_SpdifNpcmPath;
    MS_U8   g_PreMainDecodeId;
    AUDIO_DSP_CODE_TYPE g_PreMainDspCodeType;

    MS_BOOL g_dolbyLoudness_Enable;
    MS_U8   g_dolbyLoudness_DTVsystem;

    AUDIO_DSP_CODE_TYPE g_DspCodeType;
    AUDIO_DSP_CODE_TYPE g_Dsp2CodeType;

    MS_U16  u16DecodeFrmCnt[AU_DEC_MAX];

    AUDIO_DSP_CODE_TYPE g_AdvSndEffDspCodeType;
    MS_U8   g_u8DspAliveFlag;
    MS_U8   g_u8AudioCrashFlag;

    MS_U8   g_u8SifDspType;
    MS_U16  sif_gain_0[6];
    MS_U16  sif_shift_0[6];

    MS_U8   g_u8IntTag;   // DEC MM interrupt flag
    MS_U16  int_mb_cnt;

    MS_U8   CompressInfo;
    MS_U8   g_DSPMadMIUBank[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_PHY  g_DSPBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_PHY  g_DSPMadBaseBufferAdr[MAX_AUDIO_BASE_ADDRESS_NUM2];
    MS_PHY  g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];

    MS_U32  g_u32ClientCounter;

    AUDIO_SOURCE_INFO_TYPE eMainSourceType;
    AUDIO_SOURCE_INFO_TYPE ePrevSourceType;
    AUDIO_DSP_USAGE        DspUsage[AU_DEC_MAX];
    AudioDecStatus_t            AudioDecStatus[AU_DEC_MAX];

    AUDIO_SOURCE_INFO_TYPE eAudioSource;
    AUDIO_INPUT_TYPE       eMainAudioSource;
    AUDIO_INPUT_TYPE       eSubAudioSource;
    AUDIO_INPUT_TYPE       eScartAudioSource;

    AUDIO_INIT_INFO        AudioSystemInfo;
    AUDIO_OUT_INFO         AudioOutputInfo;             // Store the connectivity of audio output device
    AUDIO_PATH_INFO        AudioPathInfo;               // Store the connectivity of audio DSP & output path
    OUTPUT_SOURCE_INFO     AudioOutputSourceInfo;       // Store the output port source setting

    DigitalOutSetting_t stDigitalOutSetting[DIGITAL_MAX_OUTPUT];
    Digital_Out_Channel_Status_t stDigitalOutChannelStatus[DIGITAL_MAX_OUTPUT];

    MS_U16  mainAudioDelay;
    MS_U8   ChipRevision;

    AUDIO_CAPABILITIES AudioCaps[AU_DEC_MAX];
    Digital_Out_Device_Capability_t stDigitalOutCodecCapability[2][CODEC_MAX];

    MS_BOOL DolbyAACFlag;
    MS_BOOL DolbyEncFlag;

    AUDIO_PCM_INFO_t g_PcmInfo[AUDIO_PCM_MAXNUM];

    AUDIO_DEBUG_LEVEL g_debug_level;

    MS_U8   g_R2_NonPcm_Sel;
    MS_U8   g_DolbyDAPEnable;

    AUDIO_OUTPUT_DEVICE_SELECTION_t g_AudioOutputDeviceSelection;

    AUDIO_SPECIFIED_DIGITAL_OUTPUT_t g_AudioSpecifiedDigitalOutput;

    MS_BOOL  g_hbr_bypass_enable;

    MS_U8   g_u8MADSkipResetFlag;
    MS_U8   g_u8MAD2SkipResetFlag;

    MS_BOOL  g_R2ShmInitFlag;

    MS_U8   g_HDMINonPcmPath;

    MS_BOOL g_bHashkeyFlag;

    DOLBY_DRC_MODE g_dolbyDrcMode;
    AUDIO_INPUT_TYPE AudioSrcType;
    AUDIO_INPUT_TYPE AudioSubSrcType;
    MS_U8   g_ddenc_owner;    // 0:R2 1:DSP

    DIGITAL_OUTPUT_TYPE g_EdidOutputInterfaceType;
    MS_BOOL g_EdidOutViaArc;
} AUDIO_SHARED_VARS2;

typedef struct
{
    MS_U32 Address;
    MS_U8 Length;
    MS_U16 u16Data[8];
} Audio_MBOX_BackUp_t;

//=====================================================

void MDrv_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE u16Mode);
void MDrv_AUDIO_SPDIF_SetOutputType (AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType);
MS_BOOL MDrv_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);
AUDIO_DEC_ID MDrv_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);
MS_BOOL MDrv_AUDIO_CheckPlayDone(void);

//================================================================
//  For Audio initialize
//================================================================
void    MDrv_AUDIO_SetPowerOn(MS_BOOL bPowerOn);
void    MDrv_AUDIO_RebootDSP(MS_BOOL bDspType);
MS_BOOL MDrv_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);
MS_BOOL MDrv_AUDIO_GetEncodeDoneFlag(void);
MS_BOOL MDrv_AUDIO_Check_AsndR2_SupportStatus(void);
MS_BOOL MDrv_AUDIO_CheckBootOnInitState(void);
void    MDrv_AUDIO_WriteStopDecTable(void);

//================================================================
// Audio I/O Config. function
//================================================================
void    MDrv_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);
void    MDrv_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd);
void    MDrv_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8 u8Value);
void    MDrv_AUDIO_USBPCM_Enable(MS_BOOL bEnable);
void    MDrv_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);
MS_BOOL MDrv_AUDIO_USBPCM_GetFlag(void);
void    MDrv_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);
MS_BOOL MDrv_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL MDrv_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MDrv_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);
MS_BOOL MDrv_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void* buffer, const MS_U32 bytes);

//=====================================================
// AUDIO SPDIF Relational Hal Function
//=====================================================
void    MDrv_AUDIO_SPDIF_HWEN(MS_U8 spdif_hwen);
MS_U8   MDrv_AUDIO_SPDIF_GetMode(void);
void    MDrv_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
MS_U8   MDrv_AUDIO_SPDIF_GetSCMS(void);
MS_BOOL MDrv_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);
MS_BOOL Mdrv_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
//=====================================================
// AUDIO HDMI Relational Hal Function
//=====================================================
MS_BOOL MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);
void    MDrv_AUDIO_HDMI_Tx_SetMute(MS_U8 bEnMute);
AUDIO_HDMI_RX_TYPE MDrv_AUDIO_HDMI_SetNonpcm(MS_U8 u8NonPCM_en);
MS_U16  MDrv_AUDIO_HDMI_GetSynthFreq(void);
void    MDrv_AUDIO_HDMI_SetDownSample(MS_U8 ratio);
MS_BOOL MDrv_AUDIO_HDMI_NonpcmMonitor(void);
//================================================================
// for audio driver internal use function
//================================================================
void    MDrv_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);
void    MDrv_AUDIO_DmaReader_AllInput_Init(void);
MS_BOOL MDrv_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);
void    MDrv_AUDIO_DmaWriter_Init(void);
MS_U16  MDrv_AUDIO_AbsReadReg(MS_U32 u32RegAddr);
void    MDrv_AUDIO_AbsWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_S32 MDrv_AUDIO_InputSwitch(AUDIO_SWITCH_GROUP enGroup);

//================================================================
// Audio VoIP Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_VIRT param1, MS_VIRT param2);

//================================================================
// Audio ALSA Interface Relational Function
//================================================================
MS_BOOL MDrv_AUDIO_ALSA_Check(void);


MS_PHY MDrv_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);

void MDrv_AUDIO_PCM_Init(void);
void MDrv_AUDIO_PCM_DeInit(void);
MS_S32 MDrv_AUDIO_PCM_Open(void *pData);
MS_S32 MDrv_AUDIO_PCM_Close(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Start(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Stop(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData);
MS_S32 MDrv_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData);
MS_U32 MDrv_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_U32 MDrv_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size);
MS_S32 MDrv_AUDIO_PCM_Flush(MS_S32 s32DeviceId);

MS_S32 MDrv_AUDIO_ENCODER_Open(const char *pName);
MS_S32 MDrv_AUDIO_ENCODER_Close(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_ENCODER_Start(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId);
MS_S32 MDrv_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData);
MS_U32 MDrv_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size);
MS_S32 MDrv_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId);

#ifdef __cplusplus
}
#endif

#endif // _DRV_AUDIO_IF_H_
