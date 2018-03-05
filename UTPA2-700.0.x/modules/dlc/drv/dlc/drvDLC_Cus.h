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
#include "UFO.h"
#ifdef __cplusplus
extern "C"
{
#endif

#define DLC_HDR_VERSION  4

typedef struct
{
    MS_U16 g_wLumaHistogram32H[32];
    MS_U16 g_wLumiTotalCount;
    MS_U16 g_wLumiAverageTemp;
    MS_U8 g_ucTable[16];
    MS_U8 g_ucHistogramMax;
    MS_U8 g_ucHistogramMin;
} DLC_LUMADATA;


typedef struct
{    /// Default luma curve
    MS_U16 uwLumaCurve[16];
    /// Default luma curve 2a
    MS_U16 uwLumaCurve2_a[16];
    /// Default luma curve 2b
    MS_U16 uwLumaCurve2_b[16];
    /// Default luma curve 2
    MS_U16 uwLumaCurve2[16];
    MS_BOOL b10BitsCruveEn;
}DLC_init_Ext;

/*
 * DLC capture Range
 */
typedef struct
{
	MS_U16 wHStart;
	MS_U16 wHEnd;
	MS_U16 wVStart;
	MS_U16 wVEnd;
}StuDbc_CAPTURE_Range;


typedef struct __attribute__((packed))
{
    MS_U8      u8ColorPrimaries;
    MS_U8      u8TransferCharacteristics;
    MS_U8      u8MatrixCoefficients;
} DLC_HDRMetadataMpegVUI;

// Data format follows HDR_Metadata_MpegSEI_MasteringColorVolume
typedef struct __attribute__((packed))
{
    MS_U16     display_primaries_x[3]; // x = data*0.00002    [709] {15000, 7500, 32000}
    MS_U16     display_primaries_y[3]; // y = data*0.00002    [709] {30000, 3000, 16500}
    MS_U16     white_point_x; // x = data*0.00002    [709] 15635
    MS_U16     white_point_y; // y = data*0.00002    [709] 16450
    MS_U32     max_display_mastering_luminance; // data*0.0001 nits    [600nits] 6000000
    MS_U32     min_display_mastering_luminance; // data*0.0001 nits    [0.3nits] 3000
} DLC_HDRMetadataMpegSEIMasteringColorVolume;

typedef struct __attribute__((packed))
{
    MS_U16 u16Smin; // 0.10
    MS_U16 u16Smed; // 0.10
    MS_U16 u16Smax; // 0.10
    MS_U16 u16Tmin; // 0.10
    MS_U16 u16Tmed; // 0.10
    MS_U16 u16Tmax; // 0.10
    MS_U16 u16MidSourceOffset;
    MS_U16 u16MidTargetOffset;
    MS_U16 u16MidSourceRatio;
    MS_U16 u16MidTargetRatio;
} DLC_HDRToneMappingData;

typedef struct __attribute__((packed))
{
    MS_U16 u16tRx; // target Rx
    MS_U16 u16tRy; // target Ry
    MS_U16 u16tGx; // target Gx
    MS_U16 u16tGy; // target Gy
    MS_U16 u16tBx; // target Bx
    MS_U16 u16tBy; // target By
    MS_U16 u16tWx; // target Wx
    MS_U16 u16tWy; // target Wy
} DLC_HDRGamutMappingData;

typedef struct __attribute__((packed))
{
    MS_U8      u8EOTF; // 0:SDR gamma, 1:HDR gamma, 2:SMPTE ST2084, 3:Future EOTF, 4-7:Reserved
    MS_U16     u16Rx; // display primaries Rx
    MS_U16     u16Ry; // display primaries Ry
    MS_U16     u16Gx; // display primaries Gx
    MS_U16     u16Gy; // display primaries Gy
    MS_U16     u16Bx; // display primaries Bx
    MS_U16     u16By; // display primaries By
    MS_U16     u16Wx; // display primaries Wx
    MS_U16     u16Wy; // display primaries Wy
    MS_U16     u16Lmax; // max display mastering luminance
    MS_U16     u16Lmin; // min display mastering luminance
    MS_U16     u16MaxCLL; // maximum content light level
    MS_U16     u16MaxFALL; // maximum frame-average light level
} DLC_HDRMetadataHdmiTxInfoFrame;

// HDR use customer DLC curve.
typedef struct __attribute__((packed))
{
    MS_BOOL bFixHdrCurve;
    MS_U16 u16DlcCurveSize;
    MS_U8 *pucDlcCurve;
#if !defined (__aarch64__)
    void    *pDummy;
#endif
} DLC_HDRCustomerDlcCurve;

// HDR use customer color primaries.
typedef struct __attribute__((packed))
{
    MS_BOOL bCustomerEnable;
    MS_U16 u16sWx;
    MS_U16 u16sWy;
} DLC_HDRCustomerColorPrimaries;

typedef struct __attribute__((packed))
{
    MS_U8 PixelFormat;              // Pixel Format
    MS_U8 Colorimetry;              // Color imetry
    MS_U8 ExtendedColorimetry;      // Extended Color imetry
    MS_U8 RgbQuantizationRange;     // Rgb Quantization Range
    MS_U8 YccQuantizationRange;     // Ycc Quantization Range
    MS_U8 StaticMetadataDescriptorID; //Static Metadata Descriptor ID
} DLC_HDRHdmiTxAviInfoFrame;

// HDR new tone mapping parameters.
typedef struct __attribute__((packed))
{
    // TMO
    MS_U16 u16SrcMinRatio;			//default 10
    MS_U16 u16SrcMedRatio;                   //default 512
    MS_U16 u16SrcMaxRatio;			//default 990

    MS_U8 u8TgtMinFlag;                         //default 1
    MS_U16 u16TgtMin;					//default 500
    MS_U8 u8TgtMaxFlag;                        //default 0
    MS_U16 u16TgtMax;				//default 300

    MS_U16 u16FrontSlopeMin;			//default 256
    MS_U16 u16FrontSlopeMax;			//default 512
    MS_U16 u16BackSlopeMin;			//default 128
    MS_U16 u16BackSlopeMax;			//default 256

    MS_U16 u16SceneChangeThrd;		//default 1024
    MS_U16 u16SceneChangeRatioMax;	//default 1024

    MS_U8 u8IIRRatio;					//default 31
    MS_U8 u8TMO_TargetMode;      // default 0. 0 : keeps the value in initial function  1 : from output source

} DLC_HDRNewToneMapping;

/*!
 *  Initial  HDR   Settings
 */
typedef struct __attribute__((packed))
{
    /// HDR Version Info
    MS_U16 u16HDRVerInfo;
    /// HDR init Length
    MS_U16 u16HDRInitLength;
    /// HDR Enable
    MS_BOOL bHDREnable;
    /// HDR Function Select
    MS_U16 u16HDRFunctionSelect;
    /// HDR Metadata Mpeg VUI
    DLC_HDRMetadataMpegVUI DLC_HDRMetadataMpegVUI;
    //HDR Tone Mapping Data
    DLC_HDRToneMappingData DLC_HDRToneMappingData;
    //HDR Gamut Mapping Data
    DLC_HDRGamutMappingData DLC_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    DLC_HDRMetadataHdmiTxInfoFrame DLC_HDRMetadataHdmiTxInfoFrame;
    // Customer DLC Curve
    DLC_HDRCustomerDlcCurve DLC_HDRCustomerDlcCurve;
    // Customer color primarie.
    DLC_HDRCustomerColorPrimaries DLC_HDRCustomerColorPrimaries;
    //HDR Hdmi Tx Avi Info Frame
    DLC_HDRHdmiTxAviInfoFrame DLC_HDRHdmiTxAviInfoFrame;
    /// HDR metadata MPEG SEI mastering color volume/
    DLC_HDRMetadataMpegSEIMasteringColorVolume DLC_HDRMetadataMpegSEIMasteringColorVolume;
    /// New tone mapping parameters.
    DLC_HDRNewToneMapping DLC_HDRNewToneMapping;
} DLC_HDRinit;

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)

typedef struct DLL_PACKED
{
    MS_U32 u32OutputNits[512];
    MS_U16 u16InputPoint[512];
    MS_U16 u16ControlPoints;
    MS_S16 s16LastLess1IndexOutputnits;
    MS_S16 s16LastIndexM10000Outputs;
    MS_S16 s16LastIndexM100Outputs;
}ST_DLC_TMO_1DLUT;

typedef struct  DLL_PACKED
{
    MS_U32 u32TMOInfo_Version;         ///<Version of current structure. Please always set to "DLC_TMO_INFO_VERSION" as input
    MS_U16 u16TMOInfo_Length;          ///<Length of this structure, u163DLutInfo_Length=sizeof(ST_DLC_TMO_INFO)
    // TMO
    MS_U16 u16SrcMinRatio;          //default 10
    MS_U16 u16SrcMedRatio;          //default 512
    MS_U16 u16SrcMaxRatio;          //default 990

    MS_U16 u16TgtMin;               //default 500
    MS_U16 u16TgtMed;
    MS_U16 u16TgtMax;               //default 300

    MS_U16 u16FrontSlopeMin;        //default 256
    MS_U16 u16FrontSlopeMax;        //default 512
    MS_U16 u16BackSlopeMin;         //default 128
    MS_U16 u16BackSlopeMax;         //default 256

    MS_U16 u16SceneChangeThrd;      //default 1024
    MS_U16 u16SceneChangeRatioMax;  //default 1024

    MS_U8 u8IIRRatio;               //default 31
    MS_U8 u8TMOTargetMode;          // default 0. 0 : keeps the value in initial function  1 : from output source
    MS_U16 u16SDRPanelGain;        //

    MS_U16 u16Smin;
    MS_U16 u16Smed;
    MS_U16 u16Smax;
    MS_U16 u16Tmin;
    MS_U16 u16Tmed;
    MS_U16 u16Tmax;

    MS_BOOL bRefMode;

    MS_U8 u8TMOUseIniControls;
    ST_DLC_TMO_1DLUT stTMO1DLUT;

} ST_DLC_TMO_INFO;
#endif
#endif

MS_U16  msDlcLibVersionCheck(void);
//void    msDlcInitWithCurve( MS_U16 wHStart, MS_U16 wHEnd, MS_U16 wVStart, MS_U16 wVEnd );
//void    msDlcOnOff(MS_BOOL bSwitch, MS_BOOL bWindow);
//void    msDlcHandler(MS_BOOL bWindow);
MS_BOOL msGetHistogramHandler(MS_BOOL bWindow);
MS_U8   msGetAverageLuminous(void);
MS_U16  msGetAverageLuminous_x4(void);

//void    msDlcSpeedupTrigger(MS_U8 u8Loop);
//MS_U16  msDlcLibVersionCheck(void);
//------------------------------------------------------------------------
// CGC
//------------------------------------------------------------------------
//void    msDlc_CGC_Init(void);
//void    msDlc_CGC_ReInit(void);
//void    msDlc_CGC_SetGain(MS_U8 u8CGain);
//void    msDlc_CGC_Handler(void);
//MS_U16  msDlcCGCLibVersionCheck(void);

//------------------------------------------------------------------------
// For Debug Command
//------------------------------------------------------------------------
typedef void (*MDrv_XC_DLC_Print_Callback)           (MS_U8 /*PWM_VALUE*/);

//void msDLC_ParametersTool(MS_U8 *pCmdBuff);

/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetMFVer()
/// @brief \b Function \b Description:  Get version of DLC library
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b Version
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16  MDrv_DLC_GetMFVer(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_InitCurve()
/// @brief \b Function \b Description:  Initiate DLC HW
/// @param <IN>        \b wHStart
/// @param <IN>        \b wHEnd
/// @param <IN>        \b wVStart
/// @param <IN>        \b wVEnd
/// @param <IN>        \b dwCustomMode
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_InitCurve (MS_U16 u16HStart, MS_U16 u16HEnd, MS_U16 u16VStart, MS_U16 u16VEnd);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetCurve()
/// @brief \b Function \b Description:  Set DLC curve
/// @param <IN>        \b pNormal
/// @param <IN>        \b pLight
/// @param <IN>        \b pDark
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_SetCurve(MS_U8 *pNormal, MS_U8 *pLight, MS_U8 *pDark );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SendDlcInitCurveChangeInfo()
/// @brief \b Function \b Description: Send DLC curve to kernel DLC
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_SendDlcInitCurveChangeInfo(MS_BOOL bCurveChange,MS_U8 *pNormal, MS_U8 *pLight, MS_U8 *pDark);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SendBleChangeInfo()
/// @brief \b Function \b Description: Send BLE Slop Point to kernel
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_SendBleChangeInfo(MS_BOOL bBLEPointChange,MS_U16 *pBLESlopPoint);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetHDRInit()
/// @brief \b Function \b Description: Send HDR Init Info to kernel
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_SetHDRInit(DLC_HDRinit * pstDLC_HDRInitData);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetTMOInfo()
/// @brief \b Function \b Description: Send TMO info to kernel
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
//ST_DLC_TMO_INFO is defined by (UFO_XC_HDR_VERSION == 2)
MS_BOOL MDrv_DLC_SetTMOInfo(ST_DLC_TMO_INFO * pstDLCTMOInfo);
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetBleOnOff()
/// @brief \b Function \b Description:  On/Off BLE function
/// @param <IN>        \b bSwitch
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_SetBleOnOff(MS_BOOL bSwitch);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_Set_SlowDwonStep()
/// @brief \b Function \b Description:  Set slow down step
/// @param <IN>        \b bEn
/// @param <IN>        \b u8Step
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_Set_SlowDwonStep(MS_BOOL bEn, MS_U8 u8Step);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetOnOff()
/// @brief \b Function \b Description:  On/Off DLC function
/// @param <IN>        \b bSwitch
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_SetOnOff(MS_BOOL bSwitch, MS_BOOL bWindow);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SetDlcHandlerOnOff()
/// @brief \b Function \b Description: Set DlcHandler On or Off
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_SetDlcHandlerOnOff(MS_BOOL bSwitch);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_EnableMainSubCurveSynchronization()
/// @brief \b Function \b Description:  On/Off DLC Set Curve both for main and sub function
/// @param <IN>        \b bEnable
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
void MDrv_DLC_EnableMainSubCurveSynchronization(MS_BOOL bEnable);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_Handler()
/// @brief \b Function \b Description:  Handle DLC function
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_Handler(MS_BOOL bWindow);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetHistogram()
/// @brief \b Function \b Description:  Get Histogram
/// @param <IN>        \b bWindow
/// @param <OUT>       \b None
/// @param <RET>       \b ucReturn
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_GetHistogram(MS_BOOL bWindow);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetAverageValue()
/// @brief \b Function \b Description:  Get Average of Luminous Value
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b uwTmpAvr
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8   MDrv_DLC_GetAverageValue(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_GetAverageValue_x4()
/// @brief \b Function \b Description:  Get Average exactness of Luminous Value
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b uwTmpAvr
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16  MDrv_DLC_GetAverageValue_x4(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_SpeedupTrigger()
/// @brief \b Function \b Description:  Speedup Trigger
/// @param <IN>        \b ucLoop
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_SpeedupTrigger(MS_U8 u8Loop);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_ResetCGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_ResetCGain(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_GetCGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8   MDrv_DLC_CGC_GetCGain(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_SetCGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_SetCGain(MS_U8 u8CGain);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_ResetYGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_ResetYGain(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_GetYGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8   MDrv_DLC_CGC_GetYGain(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_SetYGain()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_SetYGain(MS_U8 u8YGain);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_Init(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_ReInit()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_ReInit(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_Handler()
/// @brief \b Function \b Description:  Handle DLC CGC function
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void    MDrv_DLC_CGC_Handler(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_CGC_GetMFVer()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16  MDrv_DLC_CGC_GetMFVer(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_WriteCurve()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_WriteCurve(MS_U8 *pu8Table);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_WriteCurve_Sub()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_DLC_WriteCurve_Sub(MS_U8 *pu8Table);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DLC_WriteCurve()
/// @brief \b Function \b Description:
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_DLC_get_histogram32(MS_U16 *pu16Histogram32);

void    MDrv_DLC_Init(StuDlc_FinetuneParamaters DLC_MFinit);
void    MDrv_DLC_Init_Ext(DLC_init_Ext *pDLC_InitExt);
MS_BOOL MDrv_DLC_GetDLCInfo(StuDlc_FinetuneParamaters *pDLCInitData, MS_U32 u32InitDataLen);
MS_BOOL MDrv_DLC_GetLumaInfo(DLC_LUMADATA *pDLCLumaData, MS_U32 u32LumaDataLen);
void    MDrv_DLC_init_riu_base( MS_VIRT ptr_riu_base );
MS_BOOL MDrv_DLC_GetLumaCurveStatus( void );
void MDrv_DLC_set_CaptureRange(StuDbc_CAPTURE_Range *pu16Cap_rang);

// DCR library start


// DCR library end

#ifdef __cplusplus
}
#endif
