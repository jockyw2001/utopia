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
// reverse bEngineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HALADVSOUND_H_
#define _HALADVSOUND_H_

#include "MsTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	SRS_TBHDX_SPEAKERSIZE_40HZ,///0
	SRS_TBHDX_SPEAKERSIZE_50HZ,///1
	SRS_TBHDX_SPEAKERSIZE_60HZ,///2
	SRS_TBHDX_SPEAKERSIZE_70HZ,///3
	SRS_TBHDX_SPEAKERSIZE_80HZ,///4
	SRS_TBHDX_SPEAKERSIZE_90HZ,///5
	SRS_TBHDX_SPEAKERSIZE_100HZ,///6
	SRS_TBHDX_SPEAKERSIZE_110HZ,///7
	SRS_TBHDX_SPEAKERSIZE_120HZ,///8
	SRS_TBHDX_SPEAKERSIZE_130HZ,///9
	SRS_TBHDX_SPEAKERSIZE_140HZ,///10
	SRS_TBHDX_SPEAKERSIZE_150HZ,///11
	SRS_TBHDX_SPEAKERSIZE_160HZ,///12
	SRS_TBHDX_SPEAKERSIZE_170HZ,///13
	SRS_TBHDX_SPEAKERSIZE_180HZ,///14
	SRS_TBHDX_SPEAKERSIZE_190HZ,///15
	SRS_TBHDX_SPEAKERSIZE_200HZ,///16
	SRS_TBHDX_SPEAKERSIZE_210HZ,///17
	SRS_TBHDX_SPEAKERSIZE_220HZ,///18
	SRS_TBHDX_SPEAKERSIZE_230HZ,///19
	SRS_TBHDX_SPEAKERSIZE_240HZ,///20
	SRS_TBHDX_SPEAKERSIZE_250HZ,///21
	SRS_TBHDX_SPEAKERSIZE_300HZ,///22
	SRS_TBHDX_SPEAKERSIZE_400HZ,///23
	SRS_TBHDX_SPEAKERSIZE_500HZ,///24
	SRS_TBHDX_SPEAKERSIZE_600HZ,///25
	SRS_TBHDX_SPEAKERSIZE_CUSTOM,
	SRS_TBHDX_SPEAKERSIZE_SIZES,
	SRS_TBHDX_SPEAKERSIZE_UNUSED = 0x7FFFFFFF
} SRSTBHDXSpeakerSize;
#define AUDIO_USE_SOUND_EFFECT_PL2            0
#define AUDIO_USE_SOUND_EFFECT_BBE            0
#define AUDIO_USE_SOUND_EFFECT_SRS            0
#define AUDIO_USE_SOUND_EFFECT_VDS            0
#define AUDIO_USE_SOUND_EFFECT_VSPK           0
#define AUDIO_USE_SOUND_EFFECT_SUPVOICE       0
#define AUDIO_USE_SOUND_EFFECT_TSHD           0
#define AUDIO_USE_SOUND_EFFECT_XEN            0
#define AUDIO_USE_SOUND_EFFECT_TSHDVIQ        0
#define AUDIO_USE_SOUND_EFFECT_ADV            0
#define AUDIO_USE_SOUND_EFFECT_DBX            1
#define AUDIO_USE_SOUND_EFFECT_THEATERSOUND   0
#define AUDIO_USE_SOUND_EFFECT_PURESND        1

#if(AUDIO_USE_SOUND_EFFECT_DBX==1)
typedef struct _DBXTV_PARAMS
{
    MS_U32 DVCOnOff;
    MS_U32 DVCFreq;
    MS_U32 DVCHiLevel;
    MS_U32 DVCHiRMSTime;
    MS_U32 DVCHiAvgAttTime;
    MS_U32 DVCHiAvgRelTime;
    MS_U32 DVCHiAttRat;
    MS_U32 DVCHiRelRat;
    MS_U32 DVCHiAttThr;
    MS_U32 DVCHiRelThr;
    MS_U32 DVCHiAttDel;
    MS_U32 DVCHiRelDel;
    MS_U32 DVCHiGate;
    MS_U32 DVCHiGain;
    MS_U32 DVCHiHold;
    MS_U32 DVCHiRMS;
    MS_U32 DVCHiRMSTimeConst;
    MS_U32 DVCHiAvgAttTimeConst;
    MS_U32 DVCHiAvgRelTimeConst;
    MS_U32 DVCHiAvgAttTCNormal;
    MS_U32 DVCHiAvgAttTCFast;
    MS_U32 DVCHiAvgRelTCNormal;
    MS_U32 DVCHiAvgRelTCFast;
    MS_U32 DVCLoLevel;
    MS_U32 DVCLoRMSTime;
    MS_U32 DVCLoAvgAttTime;
    MS_U32 DVCLoAvgRelTime;
    MS_U32 DVCLoAttRat;
    MS_U32 DVCLoRelRat;
    MS_U32 DVCLoAttThr;
    MS_U32 DVCLoRelThr;
    MS_U32 DVCLoAttDel;
    MS_U32 DVCLoRelDel;
    MS_U32 DVCLoGate;
    MS_U32 DVCLoGain;
    MS_U32 DVCLoHold;
    MS_U32 DVCLoRMS;
    MS_U32 DVCLoRMSTimeConst;
    MS_U32 DVCLoAvgAttTimeConst;
    MS_U32 DVCLoAvgRelTimeConst;
    MS_U32 DVCLoAvgAttTCNormal;
    MS_U32 DVCLoAvgAttTCFast;
    MS_U32 DVCLoAvgRelTCNormal;
    MS_U32 DVCLoAvgRelTCFast;
    MS_U32 DVCMuteOnOff;
    MS_U32 DVCMuteTime;
    MS_U32 DVCMuteLevel;
    MS_U32 DVCMuteDetect;
    MS_U32 DVCMuteHold;
    MS_U32 DVCHold;
    MS_U32 DVCOrder;
    MS_U32 DVCCoupling;
    MS_U32 DVCHoldTime;
    MS_U32 DVCHoldTC;
    MS_U32 DPPOnOff;
    MS_U32 DPPThresh;
    MS_U32 DPPAvgTime;
    MS_U32 DPPSumRMS;
    MS_U32 DPPDifRMS;
    MS_U32 DPPProtect;
    MS_U32 DPPGain;
    MS_U32 DPPHPBypass;
    MS_U32 DPPFreq;
    MS_U32 DPPEQBypass;
    MS_U32 DPPGL;
    MS_U32 DPPGM;
    MS_U32 DPPGH;
    MS_U32 DPPFL;
    MS_U32 DPPFH;
    MS_U32 DPPRMSTime;
    MS_U32 DPPRMSTimeConstSum;
    MS_U32 DPPRMSTimeConstDif;
    MS_U32 DPPCenter;
    MS_U32 DPPAvgTimeConst;
    MS_U32 IWBypass;
    MS_U32 IWSpaceGain;
    MS_U32 IWClip;
    MS_U32 EQBypass;
    MS_U32 EQNumBands;
    MS_U32 BEOnOff;
    MS_U32 BEFreq;
    MS_U32 BEMaxGain;
    MS_U32 BEOrder;
    MS_U32 BEHPFFreq;
    MS_U32 BELevel;
    MS_U32 BERMSTime;
    MS_U32 BEAvgAttTime;
    MS_U32 BEAvgRelTime;
    MS_U32 BEGainRat;
    MS_U32 BEGain;
    MS_U32 BERMS;
    MS_U32 BERMSTimeConst;
    MS_U32 BEAvgAttTimeConst;
    MS_U32 BEAvgRelTimeConst;
    MS_U32 BEPeakThreshLin;
} DBXTV_PARAMS;

/** Coefficients structure.
    This defines the DSP coefficients that get copied from the preset to
	the DSP PM memory.
*/
typedef struct _DBXTV_COEFS
{
    MS_S32 DVClpfL1coef[5];
    MS_S32 DVChpfL1coef[5];
    MS_S32 DPPeqlcoef[5];
    MS_S32 DPPeqlmcoef[5];
    MS_S32 DPPeqhmcoef[5];
    MS_S32 DPPeqhcoef[5];
    MS_S32 DPPhpfdifcoef[5];
    MS_S32 IWEQCoefs[5];
    MS_S32 DEQCoefs0[5];
    MS_S32 DEQCoefs1[5];
    MS_S32 DEQCoefs2[5];
    MS_S32 DEQCoefs3[5];
    MS_S32 DEQCoefs4[5];
    MS_S32 DEQCoefs5[5];
    MS_S32 DEQCoefs6[5];
    MS_S32 DVCBELPF1Coefs[5];
    MS_S32 DVCBELPF2Coefs[5];
    MS_S32 DVCBEHPF1Coefs[5];
    MS_S32 DVCBEHPF2Coefs[5];
    MS_S32 BEHPFCoefs[5];
} DBXTV_COEFS;


/** DM structure */
typedef union _DBXTV_DM
{
    MS_U32 Data[95];
    DBXTV_PARAMS Params;
} DBXTV_DM;

/** PM structure */
typedef union _DBXTV_PM
{
    MS_U32 Data[100];
    DBXTV_COEFS Coefs;
} DBXTV_PM;


/** Total Sonics preset length in DM */
#define TOTSON_LENGTH_DM (sizeof(DBXTV_DM)/4)
/** Total Sonics preset length in PM */
#define TOTSON_LENGTH_PM (sizeof(DBXTV_PM)/4)


/* Lengths of presets (in words) */
/* Note if the DM structure is changed, these values will need to be updated */
#define DVC_LENGTH_DM (54)
#define DVC_LENGTH_PM (10)
#define DPP_LENGTH_DM (20)
#define DPP_LENGTH_PM (25)
#define IW_LENGTH_DM (3)
#define IW_LENGTH_PM (5)
#define EQ_LENGTH_DM (2)
#define EQ_LENGTH_PM (35)
#define BE_LENGTH_DM (16)
#define BE_LENGTH_PM (25)

/* Offsets of presets (in words) */
/* Note if the DM structure is changed, these values will need to be updated */
#define DVC_OFFSET_DM (0)
#define DVC_OFFSET_PM (0)
#define DPP_OFFSET_DM (54)
#define DPP_OFFSET_PM (10)
#define IW_OFFSET_DM (74)
#define IW_OFFSET_PM (35)
#define EQ_OFFSET_DM (77)
#define EQ_OFFSET_PM (40)
#define BE_OFFSET_DM (79)
#define BE_OFFSET_PM (75)

/* Offsets for DVCOnOff, DPPOnOff, IWBypass, EQBypass, BEOnOff (in words) */
/* Note if the DM structure is changed, these values will need to be updated */
#define DM_OFFSET_DVCOnOff (0)
#define DM_OFFSET_DPPOnOff (54)
#define DM_OFFSET_IWBypass (74)
#define DM_OFFSET_EQBypass (77)
#define DM_OFFSET_BEOnOff (79)
#define DM_OFFSET_DVCHiLevel (2)
#define DM_OFFSET_DVCLoLevel (23)

#define Enable_DVC (1<<0)
#define Enable_DPP (1<<1)
#define Enable_IW (1<<2)
#define Enable_EQ (1<<3)
#define Enable_BE (1<<4)
#define Enable_All (Enable_DVC | Enable_DPP | Enable_IW | Enable_EQ | Enable_BE)
#endif

//SRS APOLLO
typedef enum
{
    HAL_APOLLO_EN,
    HAL_TSHD_EN,
    HAL_TRUBASS_FRONT_EN,
    HAL_DEFINITION_FRONT_EN,
    HAL_DIALOG_CLARITY_EN,
    HAL_SURROUND_LEVEL_CTRL,
    HAL_TSHD_INPUT_GAIN,
    HAL_DEFINITION_FRONT_CTRL,
    HAL_DIALOG_CLARITY_CTRL,
    HAL_TRUBASS_FRONT_CTRL,
    HAL_LOW_END_FRONT_SPEAKER_FREQ,
    HAL_HL_EN,
    HAL_HL_LIMIT_LEVEL,
    HAL_HL_BOOST_GAIN,
    HAL_APOLLO_HEADROOM_GAIN,
    HAL_AEQ_PEQ_EN,
    HAL_AEQ_HPF_EN,
    HAL_AEQ_GEQ_EN,
    HAL_VIQ_EN,
    HAL_VIQ_MODE,
    HAL_VIQ_VOL_REF_LEVEL,
    HAL_VIQ_MAX_GAIN,
    HAL_VIQ_NOISE_MNGR_THLD,
    HAL_VIQ_CALIBRATE,
} HAL_ADVAUDIO_SRSAPOLLOTYPE;

//=====================================================
// Declare audio function here.
//=====================================================
void    HAL_SOUND_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr);
void    HAL_SOUND_VDS_Enable(MS_U8 u8enVDS);
void    HAL_SOUND_VSPK_Enable(MS_U8 u8enVSPK);
void    HAL_SOUND_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value);
void    HAL_SOUND_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value);
void    HAL_SOUND_SRS_Enable(MS_U8 u8enSRS);
void    HAL_SOUND_TSHD_Enable(MS_U8 u8enSRS);
void    HAL_SOUND_VSPK_SetWMod(MS_U8 u8VSPK_WMode);
void    HAL_SOUND_VSPK_SetSMod(MS_U8 u8VSPK_SMode);
void    HAL_SOUND_BBE_SetBbeMode(MS_U8 u8EnBBE, MS_U8 u8BBEMode);
void    HAL_SOUND_SetPara(MS_U8 u8Addr1, MS_U8 u8Addr2, MS_U8 u8Value1, MS_U8 u8Value2, MS_U8 u8Value3);
void    HAL_SOUND_SRS_DC(MS_U8 u8SRSenDC);
void    HAL_SOUND_SRS_TruBass(MS_U8 u8SRSenTruBass);
void    HAL_SOUND_TSHD_TruBass(MS_U8 u8TSHDenTruBass);
void    HAL_SOUND_TSHD_Definition(MS_U8 u8TSHDenDefinition);
void    HAL_SOUND_TSHD_Clarity(MS_U8 u8TSHDenClarity);
void    HAL_SOUND_TSHD_TruSurr(MS_U8 u8TSHDenTruSurr);
void    HAL_SOUND_TSHD_Limiter(MS_U8 u8TSHDenLimiter);
void    HAL_SOUND_TSHD_SRS3D(MS_U8 u8TSHDenSRS3D);
void    HAL_SOUND_TruVolume_Enable(MS_U8 u8TSHDenTVol);
void    HAL_SOUND_ADV_SetPara(MS_U8 u8mode, MS_U8 u8value);
void    HAL_SOUND_ADSY_SetPara(MS_U8 u8mode, MS_U8 u8value);
void    HAL_SOUND_SRS_APOLLO_SetPara(MS_U8 u8mode, MS_U32 u32param, MS_U16 u16value2);

MS_BOOL HAL_ADVSOUND_ProcessEnable(AUDIO_DSP_CODE_TYPE type);
MS_BOOL HAL_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable);
MS_BOOL HAL_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32param, MS_U16 u16value2);
void HAL_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size);
MS_BOOL  HAL_SOUND_DBX_Enable(MS_BOOL u8enDBX);
MS_BOOL  HAL_SET_DBX_MODE(MS_U8 u8enMODE);
int HAL_Copy(MS_U32 *ptr_data, MS_U32 Address, MS_U32 Length);
void HAL_DBXTV_SetMode(EN_DBXTV_TotSonMode enTotSonMode, EN_DBXTV_TotVolMode enTotVolMode, EN_DBXTV_TotSurMode enTotSurMode,MS_U32 enable);
void    HAL_SOUND_DBX_COPY_DM(MS_U32 *DM_ptr,MS_U32 size);
void HAL_SOUND_DBX_COPY_PARAMETERS_FROM_APP(ADVSND_PARAM type, MS_U32 *Parameter_ptr,MS_U32 size);
void Hal_DBXTV_SetVolPath(AUDIO_OUTPUT_TYPE path);

#define u32param_normalize(u32value, u32param, iwl, Q24H, Q24L)    {\
    u32value = u32param/(1<<iwl);\
    if(u32value > 8388607)\
    {\
        u32value = 8388607;\
    }\
    Q24H=(unsigned short )((u32value>>16) & 0xFFFF);\
    Q24L=(unsigned short )(u32value & 0xFFFF);\
    }

#ifdef __cplusplus
}
#endif
#endif //_DRVADVSOUND_H_
