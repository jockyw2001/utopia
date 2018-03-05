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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvTEMP.h
/// @brief  TEMP Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ADC_INTERNAL_H_
#define _DRV_ADC_INTERNAL_H_
#include "mvideo_context.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// ADC setting
typedef struct
{
    MS_U16 u16RedGain;      ///< ADC red gain
    MS_U16 u16GreenGain;    ///< ADC green gain
    MS_U16 u16BlueGain;     ///< ADC blue gain
    MS_U16 u16RedOffset;    ///< ADC red offset
    MS_U16 u16GreenOffset;  ///< ADC green offset
    MS_U16 u16BlueOffset;   ///< ADC blue offset
} XC_AdcGainOffsetSetting;

typedef struct __attribute__((packed))
{
    MS_U8  u8L_BkAtop_00;
    MS_U8  u8L_BkAtop_01;
    MS_U8  u8L_BkAtop_03;
    MS_U8  u8L_BkAtop_0C;
    MS_U8  u8L_BkAtop_2C;
    MS_U8  u8L_BkAtop_1F;
    MS_U8  u8H_BkAtop_2D;
    MS_U8  u8L_BkDtop_06;
    MS_U8  u8H_BkChipTop_1F;
    MS_U8  u8L_BkChipTop_55;
    MS_U8  u8L_BkIpMux_1;
    MS_U16  u16SC_BK1_21;
    MS_U16  u16SC_BK10_19;
    MS_U16 u16L_BkAtop_05;
    MS_U16 u16L_BkAtop_5E;
    MS_U16 u16BkAtop_1C;
    MS_U16 u16BkAtop_04;
    MS_U16 u16BkAtop_05;
    MS_U16 u16BkAtop_06;
    MS_U16 u16BkDtop_01;
    MS_U16 u16BkDtop_02;

    MS_U16 R_UserOffset;
    MS_U16 G_UserOffset;
    MS_U16 B_UserOffset;

    MS_U16 R_BlankLevel;
    MS_U16 G_BlankLevel;
    MS_U16 B_BlankLevel;

    MS_U16 u16SC_BK1_02;
    MS_U16 u16SC_BK1_03;
    MS_U16 u16SC_BK1_04;
    MS_U16 u16SC_BK1_05;
    MS_U16 u16SC_BK1_06;
    MS_U16 u16SC_BK1_07;
    MS_U16 u16SC_BK1_0E;
    MS_U16 u16SC_BK12_01;
    MS_U16 u16SC_BK12_03;
    MS_U16 u16SC_BK12_04;
    MS_U16 u16SC_BK12_0E;
    MS_U16 u16SC_BK12_0F;
    MS_U16 u16SC_BK12_16;
    MS_U16 u16SC_BK12_17;
    MS_U32 u32SC_BK12_10;
    MS_U32 u32SC_BK12_12;
    MS_U32 u32SC_BK12_14;
    MS_U16 u16SC_BK12_07;
    MS_U32 u32SC_BK12_08;
    MS_U32 u32SC_BK12_0A;
    MS_U32 u32SC_BK12_0C;
    MS_U16 u16SC_BK02_04;
    MS_U16 u16SC_BK02_05;
    MS_U16 u16SC_BK02_08;
    MS_U16 u16SC_BK02_09;
    MS_U16 u16SC_BK23_07;
    MS_U16 u16SC_BK23_08;
    MS_U16 u16SC_BK23_09;
    MS_U16 u16SC_BK23_0A;
    MS_U16 u16SC_BK12_1A;
    MS_U16 u16SC_BK12_1B;
    MS_U16 u16SC_BK06_01;
    MS_U16 u16SC_BK06_21;
    MS_U16 u16SC_BK12_44;
    MS_U16 u16SC_BK12_47;
    MS_U16 u16SC_BK02_2B;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_U16 u16SC_BKCB_17;
#endif
} XC_Adc_BackupSetting;


typedef enum
{
    En_ADC_A    =  0x01,
    En_ADC_B    =  0x02,
    En_DVI      =  0x04,
    En_VD       =  0x08,
    En_VD_YC    =  0x10,
    En_FB_RGB   =  0x20,
    En_ADC_AMUX =  0x40,
    EN_ADC_FB   =  0x80,
    Mask_VD_En  =  En_VD|En_VD_YC|En_FB_RGB,
} E_ADC_FUNC_ENABLE;

/*
    ADC multiple inputsource enum.
    Common interface for Hal and Api
 */

typedef enum
{
    // Unknow
    ADC_INPUTSOURCE_UNKNOW = 0x00,

    // Single source
    ADC_INPUTSOURCE_ONLY_RGB    = 0x01,
    ADC_INPUTSOURCE_ONLY_YPBPR  = 0x02,
    ADC_INPUTSOURCE_ONLY_MVOP   = 0x04,
    ADC_INPUTSOURCE_ONLY_DVI    = 0x08,
    ADC_INPUTSOURCE_ONLY_ATV    = 0x10,
    ADC_INPUTSOURCE_ONLY_SVIDEO = 0x20,
    ADC_INPUTSOURCE_ONLY_SCART  = 0x40,
    ADC_INPUTSOURCE_ONLY_CVBS   = 0x80,


    ADC_INPUTSOURCE_MULTI_RGB_MVOP   = (ADC_INPUTSOURCE_ONLY_RGB | ADC_INPUTSOURCE_ONLY_MVOP),
    ADC_INPUTSOURCE_MULTI_RGB_DVI    = (ADC_INPUTSOURCE_ONLY_RGB | ADC_INPUTSOURCE_ONLY_DVI),
    ADC_INPUTSOURCE_MULTI_RGB_ATV    = (ADC_INPUTSOURCE_ONLY_RGB | ADC_INPUTSOURCE_ONLY_ATV),
    ADC_INPUTSOURCE_MULTI_RGB_SVIDEO = (ADC_INPUTSOURCE_ONLY_RGB | ADC_INPUTSOURCE_ONLY_SVIDEO),
    ADC_INPUTSOURCE_MULTI_RGB_SCART  = (ADC_INPUTSOURCE_ONLY_RGB | ADC_INPUTSOURCE_ONLY_SCART),
    ADC_INPUTSOURCE_MULTI_RGB_CVBS   = (ADC_INPUTSOURCE_ONLY_RGB | ADC_INPUTSOURCE_ONLY_CVBS),

    ADC_INPUTSOURCE_MULTI_YPBPR_MVOP   = (ADC_INPUTSOURCE_ONLY_YPBPR | ADC_INPUTSOURCE_ONLY_MVOP),
    ADC_INPUTSOURCE_MULTI_YPBPR_DVI    = (ADC_INPUTSOURCE_ONLY_YPBPR | ADC_INPUTSOURCE_ONLY_DVI),
    ADC_INPUTSOURCE_MULTI_YPBPR_ATV    = (ADC_INPUTSOURCE_ONLY_YPBPR | ADC_INPUTSOURCE_ONLY_ATV),
    ADC_INPUTSOURCE_MULTI_YPBPR_SVIDEO = (ADC_INPUTSOURCE_ONLY_YPBPR | ADC_INPUTSOURCE_ONLY_SVIDEO),
    ADC_INPUTSOURCE_MULTI_YPBPR_SCART  = (ADC_INPUTSOURCE_ONLY_YPBPR | ADC_INPUTSOURCE_ONLY_SCART),
    ADC_INPUTSOURCE_MULTI_YPBPR_CVBS   = (ADC_INPUTSOURCE_ONLY_YPBPR | ADC_INPUTSOURCE_ONLY_CVBS),

    ADC_INPUTSOURCE_MULTI_MVOP_DVI    = (ADC_INPUTSOURCE_ONLY_MVOP | ADC_INPUTSOURCE_ONLY_DVI),
    ADC_INPUTSOURCE_MULTI_MVOP_SVIDEO = (ADC_INPUTSOURCE_ONLY_MVOP | ADC_INPUTSOURCE_ONLY_SVIDEO),
    ADC_INPUTSOURCE_MULTI_MVOP_SCART  = (ADC_INPUTSOURCE_ONLY_MVOP | ADC_INPUTSOURCE_ONLY_SCART),
    ADC_INPUTSOURCE_MULTI_MVOP_CVBS   = (ADC_INPUTSOURCE_ONLY_MVOP | ADC_INPUTSOURCE_ONLY_CVBS),

    ADC_INPUTSOURCE_MULTI_DVI_ATV    = (ADC_INPUTSOURCE_ONLY_DVI | ADC_INPUTSOURCE_ONLY_ATV),
    ADC_INPUTSOURCE_MULTI_DVI_SVIDEO = (ADC_INPUTSOURCE_ONLY_DVI | ADC_INPUTSOURCE_ONLY_SVIDEO),
    ADC_INPUTSOURCE_MULTI_DVI_SCART  = (ADC_INPUTSOURCE_ONLY_DVI | ADC_INPUTSOURCE_ONLY_SCART),
    ADC_INPUTSOURCE_MULTI_DVI_CVBS   = (ADC_INPUTSOURCE_ONLY_DVI | ADC_INPUTSOURCE_ONLY_CVBS),
}ADC_INPUTSOURCE_TYPE;

/*
    ADC CVBS out enum.
    Common interface for Hal and Api
 */
typedef enum
{
    // ch1 out, source select
    ADC_CVBSOUT_DISABLE_1,
    ADC_CVBSOUT_SV_ON_1,
    ADC_CVBSOUT_SV_OFF_1,
    ADC_CVBSOUT_CVBS_ON_1,
    ADC_CVBSOUT_CVBS_OFF_1,
    ADC_CVBSOUT_VIF_VE_1,
    ADC_CVBSOUT_VIF_VIF_1,

    // ch2 out, source select
    ADC_CVBSOUT_DISABLE_2,
    ADC_CVBSOUT_SV_ON_2,
    ADC_CVBSOUT_SV_OFF_2,
    ADC_CVBSOUT_CVBS_ON_2,
    ADC_CVBSOUT_CVBS_OFF_2,
    ADC_CVBSOUT_VIF_VE_2,
    ADC_CVBSOUT_VIF_VIF_2,
}E_ADC_CVBSOUT_TYPE;


typedef enum
{
    MS_ADC_A_POWER_ON,
    MS_ADC_B_POWER_ON,
    MS_VDB_CVBS_POWER_ON,
    MS_VDB_SV_POWER_ON,
    MS_VDB_FBLANK_POWER_ON,
    MS_VDA_ATV_POWER_ON,
    MS_VDA_CVBS_POWER_ON,
    MS_VDA_SV_POWER_ON,
    MS_VDA_FBLANK_POWER_ON,
    MS_DVI_POWER_ON,
    MS_ADC_VD_BLEND_POWER_ON,
    MS_ADC_POWER_ALL_OFF,
    MS_VDB_DTV_POWER_ON,
} MS_ADC_POWER_ON_TYPE;

typedef struct
{
    MS_U8 *pTable;
    MS_U8 u8TabCols;
    MS_U8 u8TabRows;
    MS_U8 u8TabIdx;
} TAB_Info;

typedef enum
{
    E_ADC_CVBSO_PORT_1,
    E_ADC_CVBSO_PORT_2,
    E_ADC_CVBSO_PORT_MAX,
} ADC_CVBSOPortNumber;

typedef enum
{
    E_ADC_ISOG_NORMAL_MODE,
    E_ADC_ISOG_STANDBY_MODE,
    E_ADC_ISOG_MODE_MAX,
} ADC_iSogDetectMode;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void Hal_ADC_init(void *pInstance, MS_U16 u16XTAL_CLK,MS_BOOL IsShareGrd,  MS_U16 eScartIDPortSelection);
#define MDrv_ADC_init  Hal_ADC_init

void MDrv_XC_ADC_hpolarity_setting(void *pInstance, MS_BOOL bHightActive);
void MDrv_XC_ADC_Set_Freerun(void *pInstance, MS_BOOL bEnable);
void MDrv_XC_ADC_Reload(void *pInstance, INPUT_SOURCE_TYPE_t enSourceType, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio);
void MDrv_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch);
// reset
void MDrv_XC_ADC_reset(void *pInstance, MS_U16 u16Reset);
void MDrv_XC_ADC_clamp_placement_setting(void *pInstance, MS_U16 u16InputClockMHz);
void MDrv_XC_ADC_vco_ctrl (void *pInstance, MS_BOOL bIsDVIPort, MS_U16 u16InputClock);

void MDrv_XC_ADC_SetOffsetSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting);
void MDrv_XC_ADC_SetGainSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting);
//get adc setting from HW
MS_BOOL MDrv_XC_ADC_GetOffsetSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting);
MS_BOOL MDrv_XC_ADC_GetGainSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting);

// Main functions for ADC.
void MDrv_XC_ADC_poweron_source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType);
// ADC cvbs out
void MDrv_XC_ADC_set_cvbs_out(void *pInstance, E_ADC_CVBSOUT_TYPE e_cvbs_out_type);
MS_BOOL MDrv_XC_ADC_Is_cvbs_out_enabled(void *pInstance, E_DEST_TYPE enOutputType);
// src & mode setting
void MDrv_XC_ADC_Set_Source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount);
void MDrv_XC_ADC_SetPcClock(void *pInstance, MS_U16 u16Value);
// backup setting before auto-adc
void MDrv_XC_ADC_BackupAdcReg(void *pInstance, SCALER_WIN eWindow);
// restore setting after auto-adc
void MDrv_XC_ADC_RestoreAdcReg(void *pInstance, SCALER_WIN eWindow);
// set SOG BW
void MDrv_XC_ADC_Set_SOGBW(void *pInstance, MS_U16 u16value);

ADC_INPUTSOURCE_TYPE MDrv_XC_ADC_ConvertSrcToADCSrc(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType);
void MDrv_XC_ADC_SetClampDuration(void *pInstance, MS_U16 u16Value);
void MDrv_XC_Mux_GetPort(void *pInstance, INPUT_SOURCE_TYPE_t src, E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count );
void MApi_XC_AutoGainEnable(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow);
MS_BOOL MApi_XC_IsAutoGainResultReady(void *pInstance, SCALER_WIN eWindow);
MS_U32 MApi_XC_GetPhaseValue(void *pInstance, SCALER_WIN eWindow);

void    MApi_XC_ADC_Source_Calibrate(INPUT_SOURCE_TYPE_t enInputSourceType); //NO_NEED  for API use
void    MDrv_XC_ADC_SetInputSource(void *pInstance, INPUT_SOURCE_TYPE_t*  enInputSource, MS_U8 u8InputSourceCount);

// ADC calibration
void MDrv_XC_ADC_Source_Calibrate(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput);
MS_BOOL MDrv_XC_ADC_InitExternalCalibration(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset);
MS_BOOL MDrv_XC_ADC_InitExternalCalibrationWithFixedOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset);
MS_BOOL MDrv_XC_ADC_InitInternalCalibration(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset);
MS_BOOL MDrv_XC_ADC_InitInternalCalibrationWithFixedOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset, SCALER_WIN eWindow);
MS_BOOL MDrv_ADC_GetInitCalibrationGain(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGain);
MS_BOOL MDrv_ADC_GetInitCalibrationOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialOffset);
void MApi_XC_ADC_ExitExternalCalibration(void *pInstance, INPUT_SOURCE_TYPE_t eSource, APIXC_AdcGainOffsetSetting* InitialGainOffset);
void MApi_XC_ADC_GenClock(void *pInstance, ADC_Gen_Clock_Type clocktype);
void MApi_XC_ADC_SetInternalVoltage(void *pInstance, ADC_Internal_Voltage InternalVoltage);
void MDrv_XC_ADC_GetDefaultGainOffset(void *pInstance, INPUT_SOURCE_TYPE_t eSource , APIXC_AdcGainOffsetSetting* pstADCSetting);
MS_U16 MDrv_XC_ADC_GetCenterGain(void *pInstance);
MS_U16 MDrv_XC_ADC_GetCenterOffset(void *pInstance);
MS_U16 MDrv_XC_ADC_GetMaximalOffsetValue(void *pInstance);
MS_U16 MDrv_XC_ADC_GetMaximalGainValue(void *pInstance);
void MDrv_XC_ADC_SetGain(void *pInstance, MS_U8 u8Color, MS_U16 u16Value);
void MDrv_XC_ADC_SetOffset(void *pInstance, MS_U8 u8Color, MS_U16 u16Value);
MS_BOOL MDrv_XC_ADC_use_hardware_auto_offset(void *pInstance, INPUT_SOURCE_TYPE_t eSource);
void MApi_XC_ADC_AutoSetting(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag, MS_BOOL bIsAutoSWMode);
void MApi_XC_ADC_SetVClampLevel(void *pInstance, ADC_VClamp_Level_Type type);

void MDrv_XC_ADC_SetPhaseWithVSync(void *pInstance, MS_U16 u16Value);
void MDrv_XC_ADC_SetPhaseEx(void *pInstance, MS_U16 u16Value );
MS_U16 MDrv_XC_ADC_GetPhaseRange(void *pInstance);
void MDrv_XC_ADC_GetSoGLevelRange(void *pInstance, MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value);
void MDrv_XC_ADC_SetSoGLevel(void *pInstance, MS_U32 u32Value);
void MDrv_XC_ADC_SetSoGCal(void *pInstance);
MS_U8 MDrv_XC_ADC_GetPhase(void *pInstance);
MS_U16 MDrv_XC_ADC_GetPhaseEx(void *pInstance);
void MDrv_XC_ADC_PowerOff(void *pInstance);
MS_BOOL MDrv_XC_ADC_IsScartRGB(void *pInstance);
MS_U16 MDrv_XC_ADC_GetPcClock(void *pInstance);
void MDrv_XC_ADC_SetRGB_PIPE_Delay(void *pInstance, MS_U8 u8Value);
void MDrv_XC_ADC_ScartRGB_SOG_ClampDelay(void *pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur);
void MDrv_XC_ADC_Set_YPbPrLooseLPF(void *pInstance, MS_BOOL bEnable);
void MDrv_XC_ADC_SetMode(void *pInstance, INPUT_SOURCE_TYPE_t enSourceType, XC_InternalStatus *pSrcInfo, PQ_ADC_SAMPLING_INFO *pstPQADCSamplingInfo, SCALER_WIN eWindow);
void MDrv_XC_ADC_GetPQADCSamplingInfo(void *pInstance, INPUT_SOURCE_TYPE_t enSourceType, XC_InternalStatus *pSrcInfo, PQ_ADC_SAMPLING_INFO *pstPQADCSamplingInfo, SCALER_WIN eWindow);
void MApi_XC_ADC_SwitchSource(void *pInstance, MS_BOOL bUpdateStatus);
void MDrv_XC_ADC_EnableHWCalibration(void *pInstance, MS_BOOL bEnable);
MS_BOOL MDrv_XC_ADC_CheckHWCalibrationSupport(void *pInstance);
void MDrv_ADC_ISOG_Enable(void *pInstance, MS_BOOL bEnable);
void MDrv_ADC_ISOG_SetDetectMode(void *pInstance, ADC_iSogDetectMode stMode);
MS_BOOL MDrv_XC_ADC_GetFixedGain(void *pInstance, const INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstAdcFixedGain);
MS_BOOL MDrv_ADC_GetFixedOffset(void *pInstance, const INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialOffset);
MS_BOOL MDrv_XC_Auto_GetHWFixedGainOffset(void *pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting);
MS_BOOL MDrv_XC_Auto_Geometry_Ex(void *pInstance, XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info_Ex *ActiveInfo, XC_Auto_Signal_Info_Ex *StandardInfo , SCALER_WIN eWindow);
MS_BOOL MDrv_XC_ADC_SetIdacCurrentMode(void *pInstance, ADC_DacCurrentMode_t eMode);
ADC_DacCurrentMode_t MDrv_XC_ADC_GetIdacCurrentMode(void *pInstance);
void MDrv_XC_ADC_SCART_RGB_setMode(void *pInstance, MS_BOOL bEnable);

#endif // _DRV_ADC_INTERNAL_H_

