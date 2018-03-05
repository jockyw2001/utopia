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
#define AUDIO_USE_SOUND_EFFECT_PL2      0   //ID = 0
#define AUDIO_USE_SOUND_EFFECT_BBE      0   //ID = 1
#define AUDIO_USE_SOUND_EFFECT_SRS      0   //ID = 2
#define AUDIO_USE_SOUND_EFFECT_VDS      0   //ID = 3
#define AUDIO_USE_SOUND_EFFECT_VSPK     0   //ID = 4
#define AUDIO_USE_SOUND_EFFECT_SUPVOICE 0   //ID = 5
#define AUDIO_USE_SOUND_EFFECT_TSHD     0   //ID = 6
#define AUDIO_USE_SOUND_EFFECT_XEN      0   //ID = 7
#define AUDIO_USE_SOUND_EFFECT_TSHDVIQ  0   //ID = 8
#define AUDIO_USE_SOUND_EFFECT_ADV      0   //ID = 9
#define AUDIO_USE_SOUND_EFFECT_DBX   0        //ID = 10
#define AUDIO_USE_SOUND_EFFECT_THEATERSOUND    0      //ID = 11
#define AUDIO_USE_SOUND_EFFECT_PURESND   1        //ID = 12

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
void HAL_SOUND_DTSPRTV_SYM_Enable( MS_U8 enable);
void HAL_SOUND_DTSPRTV_SYM_Default(MS_U8 Value);
void HAL_SOUND_DTSPRTV_SYM_Mode(MS_U8 Value);
void HAL_SOUND_DTSPRTV_SYM_Level(MS_U8 Value);
void HAL_SOUND_DTSPRTV_EVO_Enable(MS_U8 Value);
void HAL_SOUND_DTSPRTV_EVO_MonoInput(MS_U8 Value);
void HAL_SOUND_DTSPRTV_EVO_WideningOn(MS_U8 Value);
void HAL_SOUND_DTSPRTV_EVO_PCELevel(MS_U8 Value);
void HAL_SOUND_DTSPRTV_EVO_VLFELevel(MS_U8 Value);

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
