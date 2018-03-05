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
// (!!¡ÓMStar Confidential Information!!L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [mhal_adc.h]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_ADC_H
#define MHAL_ADC_H


//#include "hwreg_utility.h"
#include "MsCommon.h"

#define ADC_SOG_FILTER_THRSHLD      40
#define ADC_FILTER_BW_DCLK(isVga, OrgDclk) ((isVga)?(OrgDclk*12/10):(OrgDclk/2))

/******************************************************************************/
/*                   Function Prototypes                     */
/******************************************************************************/

void Hal_ADC_set_cvbs_out(E_ADC_CVBSOUT_TYPE e_cvbs_out_type);
void Hal_ADC_clk_gen_setting(ADC_Gen_Clock_Type clocktype);

void Hal_ADC_gainoffset_reset(void);
void Hal_ADC_offset_setting(XC_AdcGainOffsetSetting *pstADCSetting);
void Hal_ADC_gain_setting(XC_AdcGainOffsetSetting *pstADCSetting);

void Hal_ADC_ext_clk_en(MS_BOOL benable);
void Hal_ADC_hdmi_vco_ctrl(MS_U16 u16InputClock);
void Hal_ADC_vco_ctrl(MS_U16 u16InputClock);
void Hal_XC_ADC_Set_VClamp_level(ADC_VClamp_Level_Type type);
void Hal_ADC_set_vco_ctrl(MS_BOOL bIsDVIPort, MS_U16 u16InputClock);


void Hal_ADC_sog_filter_en(MS_BOOL bEnable);
void Hal_ADC_reset(MS_U16 u16Reset);

void Hal_ADC_dtop_internaldc_setting(ADC_Internal_Voltage InternalVoltage);

//  Set RGB Gain setting
void Hal_ADC_SetGainR(MS_U16 u16value);
void Hal_ADC_SetGainG(MS_U16 u16value);
void Hal_ADC_SetGainB(MS_U16 u16value);
//  Get RGB Gain setting
MS_U16 Hal_ADC_GetGainR(void);
MS_U16 Hal_ADC_GetGainG(void);
MS_U16 Hal_ADC_GetGainB(void);

//  Set RGB Offset setting
void Hal_ADC_SetOffsetR(MS_U16 u16value);
void Hal_ADC_SetOffsetG(MS_U16 u16value);
void Hal_ADC_SetOffsetB(MS_U16 u16value);
//  Get RGB Offset setting
MS_U16 Hal_ADC_GetOffsetR(void);
MS_U16 Hal_ADC_GetOffsetG(void);
MS_U16 Hal_ADC_GetOffsetB(void);

void Hal_ADC_dtop_calibration_target_setting(MS_BOOL bIsYPbPrFlag);
void Hal_ADC_dtop_sw_mode_setting(MS_BOOL bEnable, MS_BOOL bIsAutoSWMode);

void Hal_ADC_reset(MS_U16 u16Reset);
void Hal_ADC_Set_Source(ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount);
void Hal_ADC_ExitExternalCalibration(ADC_INPUTSOURCE_TYPE eADC_Source,XC_AdcGainOffsetSetting* InitialGainOffset);
MS_BOOL Hal_ADC_InitExternalCalibration(void *pInstance, MS_U32 enAdcSource);
void Hal_ADC_InitInternalCalibration(ADC_INPUTSOURCE_TYPE enAdcSource);

void Hal_ADC_dtop_clk_setting ( MS_U16 u16Value );
void Hal_ADC_hpolarity_setting(MS_BOOL bHightActive);
void Hal_ADC_set_phase(MS_U8 u8Value);
void Hal_ADC_set_phaseEx(MS_U16 u16Value);
MS_U16 Hal_ADC_get_phase_range(void);
MS_U8 Hal_ADC_get_phase(void);
MS_U16 Hal_ADC_get_phaseEx(void);
void Hal_ADC_Set_Source_Calibration(ADC_INPUTSOURCE_TYPE enADC_SourceType);
void Hal_ADC_clamp_placement_setting(MS_U16 u16InputClockMHz);
void Hal_ADC_dvi_setting(void);
void Hal_ADC_poweroff(void);
void Hal_XC_ADC_poweron_source(ADC_INPUTSOURCE_TYPE enADC_SourceType);

void Hal_ADC_set_mode(ADC_INPUTSOURCE_TYPE enADCInput, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio);
#define Hal_ADC_SCART_RGB_setMode(args...)
void Hal_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch);

void Hal_ADC_auto_adc_backup(SCALER_WIN eWindow);
void Hal_ADC_auto_adc_restore(SCALER_WIN eWindow);
#define Hal_ADC_disable_csc(args...);

MS_BOOL Hal_ADC_is_scart_rgb(void);
MS_U16 Hal_ADC_get_clk (void);
void Hal_ADC_get_SoG_LevelRange(MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value);
void Hal_ADC_set_SoG_Level(MS_U32 u32Value);
MS_BOOL Hal_ADC_set_SoG_Calibration(void);

void Hal_ADC_set_RGB_PIPE_Delay(MS_U8 u8Value);
void Hal_ADC_set_ScartRGB_SOG_ClampDelay(MS_U16 u16Clpdly, MS_U16 u16Caldur);
void Hal_ADC_set_YPbPrLooseLPF(MS_BOOL benable);
void Hal_ADC_Set_SOGBW(MS_U16 u16value);

void Hal_ADC_dtop_iClampDuration_setting(MS_U16 u16value);
void Hal_ADC_dtop_vClampDuration_setting(MS_U16 u16value);

//In U4 no ADC input source , so set ADC function to NULL
#define Hal_ADC_set_cvbs_out(args...)
#define Hal_ADC_is_cvbs_out_enabled(args...) 0
#define  Hal_ADC_clk_gen_setting(args...)
#define  Hal_ADC_gainoffset_reset(args...)
#define  Hal_ADC_offset_setting(args...)
#define  Hal_ADC_gain_setting(args...)
#define  Hal_ADC_ext_clk_en(args...)
#define  Hal_ADC_hdmi_vco_ctrl(args...)
#define  Hal_ADC_vco_ctrl(args...)
#define  Hal_XC_ADC_Set_VClamp_level(args...)
#define  Hal_ADC_set_vco_ctrl(args...)
#define  Hal_ADC_sog_filter_en(args...)
#define  Hal_ADC_reset(args...)
#define  Hal_ADC_dtop_internaldc_setting(args...)
#define  Hal_ADC_SetGainR(args...)
#define  Hal_ADC_SetGainG(args...)
#define  Hal_ADC_SetGainB(args...)
#define Hal_ADC_GetGainR(args...) 0
#define Hal_ADC_GetGainG(args...) 0
#define Hal_ADC_GetGainB(args...) 0
#define  Hal_ADC_SetOffsetR(args...)
#define  Hal_ADC_SetOffsetG(args...)
#define  Hal_ADC_SetOffsetB(args...)
#define Hal_ADC_GetOffsetR(args...) 0
#define Hal_ADC_GetOffsetG(args...) 0
#define Hal_ADC_GetOffsetB(args...) 0
#define  Hal_ADC_dtop_calibration_target_setting(args...)
#define  Hal_ADC_dtop_sw_mode_setting(args...)
#define  Hal_ADC_reset(args...)
#define  Hal_ADC_Set_Source(args...)
#define  Hal_ADC_ExitExternalCalibration(args...)
#define  Hal_ADC_InitInternalCalibration(args...)
#define  Hal_ADC_dtop_clk_setting(args...)
#define  Hal_ADC_hpolarity_setting(args...)
#define  Hal_ADC_set_phase(args...)
#define  Hal_ADC_set_phaseEx(args...)
#define  Hal_ADC_get_phase_range(args...) 0
#define  Hal_ADC_get_phase(args...) 0
#define  Hal_ADC_get_phaseEx(args...) 0
#define  Hal_ADC_Set_Source_Calibration(args...)
#define  Hal_ADC_clamp_placement_setting(args...)
#define  Hal_ADC_dvi_setting(args...)
#define  Hal_ADC_poweroff(args...)
#define  Hal_XC_ADC_poweron_source(args...)
#define  Hal_ADC_set_mode(args...)
#define  Hal_ADC_auto_adc_backup(args...)
#define  Hal_ADC_auto_adc_restore(args...)
#define  Hal_ADC_is_scart_rgb(args...) 0
#define  Hal_ADC_get_clk(args...) 0
#define  Hal_ADC_get_SoG_LevelRange(args...)
#define  Hal_ADC_set_SoG_Level(args...)
#define  Hal_ADC_set_SoG_Calibration(args...)
#define  Hal_ADC_set_RGB_PIPE_Delay(args...)
#define  Hal_ADC_set_ScartRGB_SOG_ClampDelay(args...)
#define  Hal_ADC_set_YPbPrLooseLPF(args...)
#define  Hal_ADC_Set_SOGBW(args...)
#define  Hal_ADC_dtop_iClampDuration_setting(args...)
#define  Hal_ADC_dtop_vClampDuration_setting(args...)
#define Hal_ADC_Enable_HWCalibration(args...)

#define Hal_ADC_ISOG_Enable(args...)
#define Hal_ADC_ISOG_CheckEnabled(x) FALSE
#define Hal_ADC_ISOG_SetResetWidth(args...)

#define Hal_ADC_SetIdacCurrentMode(args...) FALSE
#define Hal_ADC_GetIdacCurrentMode(x) FALSE
//MS_BOOL Hal_ADC_SetIdacCurrentMode(MS_U16 u16mode);
//MS_U16 Hal_ADC_GetIdacCurrentMode(void);
#endif // MHAL_ADC_H

