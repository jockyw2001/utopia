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

#define ADC_BANDWIDTH_CLK_THRSHLD0    6 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD1    14 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD2    15 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD3    16 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD4    17 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD5    18 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD6    20 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD7    21 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD8    23 // 8/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD9    25 // 20/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD10    30 // 50/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD11    60 // 100/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD12    120 // 200/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD13    150 // 300/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD14    190 // 400/0.75
#define ADC_BANDWIDTH_CLK_THRSHLD15    260 // 500/0.75

#define ADC_REFERENCE_VOLATAGE_LDO      0
#define ADC_REFERENCE_VOLATAGE_BANDGAP  1

#define ADC_IDAC_CURRENT_MODE_FULL 0
#define ADC_IDAC_CURRENT_MODE_HALF 1
#define ADC_IDAC_CURRENT_MODE_QUART 2
/******************************************************************************/
/*                   Function Prototypes                     */
/******************************************************************************/

void Hal_ADC_set_cvbs_out(void *pInstance, E_ADC_CVBSOUT_TYPE e_cvbs_out_type);
MS_BOOL Hal_ADC_is_cvbs_out_enabled(void *pInstance, E_MUX_OUTPUTPORT outputPort);
void Hal_ADC_clk_gen_setting(void *pInstance, ADC_Gen_Clock_Type clocktype);
void Hal_ADC_set_mux(void *pInstance, E_MUX_INPUTPORT port);
void Hal_ADC_set_CVBSOMux(void *pInstance, ADC_CVBSOPortNumber CVBSOPortNumber, E_MUX_INPUTPORT port);

void Hal_ADC_gainoffset_reset(void *pInstance);
void Hal_ADC_offset_setting(void *pInstance, XC_AdcGainOffsetSetting *pstADCSetting);
void Hal_ADC_gain_setting(void *pInstance, XC_AdcGainOffsetSetting *pstADCSetting);
void Hal_ADC_ExitExternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE eADC_Source,XC_AdcGainOffsetSetting* InitialGainOffset);
MS_BOOL Hal_ADC_InitExternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE enAdcSource);
void Hal_ADC_InitInternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE enAdcSource, SCALER_WIN eWindow);
void Hal_ADC_ext_clk_en(void *pInstance, MS_BOOL benable);
void Hal_ADC_hdmi_vco_ctrl(void *pInstance, MS_U16 u16InputClock);
void Hal_ADC_vco_ctrl(void *pInstance, MS_U16 u16InputClock);
void Hal_XC_ADC_Set_VClamp_level(void *pInstance, ADC_VClamp_Level_Type type);
void Hal_ADC_set_vco_ctrl(void *pInstance, MS_BOOL bIsDVIPort, MS_U16 u16InputClock);


void Hal_ADC_sog_filter_en(void *pInstance, MS_BOOL bEnable);
void Hal_ADC_reset(void *pInstance, MS_U16 u16Reset);

void Hal_ADC_dtop_internaldc_setting(void *pInstance, ADC_Internal_Voltage InternalVoltage);

//  Set RGB Gain setting
void Hal_ADC_SetGainR(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetGainG(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetGainB(void *pInstance, MS_U16 u16value);
//  Get RGB Gain setting
MS_U16 Hal_ADC_GetGainR(void *pInstance);
MS_U16 Hal_ADC_GetGainG(void *pInstance);
MS_U16 Hal_ADC_GetGainB(void *pInstance);

//  Set RGB Offset setting
void Hal_ADC_SetOffsetR(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetOffsetG(void *pInstance, MS_U16 u16value);
void Hal_ADC_SetOffsetB(void *pInstance, MS_U16 u16value);
//  Get RGB Offset setting
MS_U16 Hal_ADC_GetOffsetR(void *pInstance);
MS_U16 Hal_ADC_GetOffsetG(void *pInstance);
MS_U16 Hal_ADC_GetOffsetB(void *pInstance);

void Hal_ADC_dtop_calibration_target_setting(void *pInstance, MS_BOOL bIsYPbPrFlag);
void Hal_ADC_dtop_sw_mode_setting(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsAutoSWMode);

void Hal_ADC_reset(void *pInstance, MS_U16 u16Reset);
void Hal_ADC_Set_Source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount);
void Hal_ADC_dtop_clk_setting ( void *pInstance, MS_U16 u16Value );
void Hal_ADC_hpolarity_setting(void *pInstance, MS_BOOL bHightActive);
void Hal_ADC_set_phase(void *pInstance, MS_U8 u8Value);
void Hal_ADC_set_phaseEx(void *pInstance, MS_U16 u16Value);
MS_U16 Hal_ADC_get_phase_range(void *pInstance);
MS_U8 Hal_ADC_get_phase(void *pInstance);
MS_U16 Hal_ADC_get_phaseEx(void *pInstance);
void Hal_ADC_Set_Source_Calibration(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType);
void Hal_ADC_clamp_placement_setting(void *pInstance, MS_U16 u16InputClockMHz);
void Hal_ADC_dvi_setting(void *pInstance);
void Hal_ADC_poweroff(void *pInstance);
void Hal_XC_ADC_poweron_source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType);

void Hal_ADC_set_mode(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio);
void Hal_ADC_SCART_RGB_setMode(void *pInstance, MS_BOOL bEnable);
void Hal_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch);

void Hal_ADC_auto_adc_backup(void *pInstance, SCALER_WIN eWindow);
void Hal_ADC_auto_adc_restore(void *pInstance, SCALER_WIN eWindow);
void Hal_ADC_disable_csc(void *pInstance, SCALER_WIN eWindow);

MS_BOOL Hal_ADC_is_scart_rgb(void *pInstance);
MS_U16 Hal_ADC_get_clk (void *pInstance);
void Hal_ADC_get_SoG_LevelRange(void *pInstance, MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value);
void Hal_ADC_set_SoG_Level(void *pInstance, MS_U32 u32Value);
MS_BOOL Hal_ADC_set_SoG_Calibration(void *pInstance);

void Hal_ADC_set_RGB_PIPE_Delay(void *pInstance, MS_U8 u8Value);
void Hal_ADC_set_ScartRGB_SOG_ClampDelay(void *pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur);
void Hal_ADC_set_YPbPrLooseLPF(void *pInstance, MS_BOOL benable);
void Hal_ADC_Set_SOGBW(void *pInstance, MS_U16 u16value);

void Hal_ADC_dtop_iClampDuration_setting(void *pInstance, MS_U16 u16value);
void Hal_ADC_dtop_vClampDuration_setting(void *pInstance, MS_U16 u16value);

void Hal_ADC_Enable_HWCalibration(void *pInstance, MS_BOOL bEnable);
MS_BOOL Hal_ADC_SetCalibrationRefSource(void *pInstance, MS_U16 type);
MS_U16 Hal_ADC_GetCalibrationRefSource(void *pInstance);

void Hal_ADC_ISOG_Enable(void *pInstance, MS_BOOL bEnable);
MS_BOOL Hal_ADC_ISOG_CheckEnabled(void *pInstance);
void Hal_ADC_ISOG_SetResetWidth(void *pInstance, MS_U8 u8value);

MS_BOOL Hal_ADC_SetIdacCurrentMode(void *pInstance, MS_U16 u16mode);
MS_U16 Hal_ADC_GetIdacCurrentMode(void *pInstance);
#endif // MHAL_ADC_H

