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
/// @file   apiXC_Dlc.h
/// @brief  DLC API layer Interface
/// @author MStar Semiconductor Inc.
//  CL171075++: sync to DLC libv32.59 :
//      1.Add guard condition to msDlcOnOff & msDlc_CGC_Init; msDlcOnOff & msDlc_CGC_Init will check
//        DLC On/Off status before execution.
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup Video Video modules

 *  \defgroup XC_BE XC_BE modules  
 *  \ingroup Video
 
    DLC is used for\n
    1. Dynamic Luma Curve : Calculate the DLC curve base on histogram and average value.\n
    For more information,see \link DLC DLC interface (apiXC_DLC.h) \endlink

 *  \defgroup DLC DLC interface (apiXC_DLC.h) 
 *  \ingroup XC_BE    
 
    DLC is used for: 
    
    - Dynamic Luma Curve : Calculate the DLC curve base on histogram and average value.

    <b> Operation Code Flow: </b> \n
    check flow chart directly.
    \image html apiXC_DLC_DBC_Handler_flow.png

 *  \defgroup DLC_INIT DLC init control 
 *  \ingroup DLC

 *! \defgroup DLC_FEATURE DLC feature operation
 *  \ingroup DLC     

 *! \defgroup DLC_INFO DLC Infomation pool
 *  \ingroup DLC     

 *! \defgroup DLC_ToBeModified DLC api to be modified 
 *  \ingroup DLC     

 *! \defgroup DLC_ToBeRemove DLC api to be removed 
 *  \ingroup DLC 
 */

#ifndef _API_XC_DLC_H_
#define _API_XC_DLC_H_
#include "MsCommon.h"
#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define MSIF_DLC_LIB_CODE               {'D','L','C','_'}
#define MSIF_DLC_LIBVER                 {'0','0'}
#define MSIF_DLC_BUILDNUM               {'3','0'}
#define MSIF_DLC_CHANGELIST             {'0','0','6','1','8','0','7','0'}
#define XC_DLC_API_VERSION /* Character String for DRV/API version */  \
    MSIF_TAG           ,   /* 'MSIF'                               */  \
    MSIF_CLASS         ,   /* '00'                                 */  \
    MSIF_CUS           ,   /* 0x0000                               */  \
    MSIF_MOD           ,   /* 0x0000                               */  \
    MSIF_CHIP          , \
    MSIF_CPU           , \
    MSIF_DLC_LIB_CODE  ,   /* IP__                                 */  \
    MSIF_DLC_LIBVER    ,   /* 0.0 ~ Z.Z                            */  \
    MSIF_DLC_BUILDNUM  ,   /* 00 ~ 99                              */  \
    MSIF_DLC_CHANGELIST,   /* CL#                                  */  \
    MSIF_OS

#define XC_DLC_HDR_VERSION  1

#define ENABLE_10_BIT_DLC   0

/// DLC_MFINIT_Ex version of current DLC lib
#define DLC_MFINIT_EX_VERSION                      1

/// The ucDlcHistogramLimitCurve arrary num
#define DLC_HISTOGRAM_LIMIT_CURVE_ARRARY_NUM       17

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

/*!
 *  The DLC curve settings, ways of enable DLC or, disable DLC.
 */
typedef enum
{
    /// DLC curve
    E_XC_DLC_PURE_IMAGE_DLC_CURVE = 0,
    /// Linear curve
    E_XC_DLC_PURE_IMAGE_LINEAR_CURVE ,
    /// get base address failed when initialize panel driver
    E_XC_DLC_PURE_IMAGE_DO_NOTHING ,
} E_XC_DLC_PURE_IMAGE_CURVE ;

/*!
 *  The DLC curve mode, ways of decide DLC algorithm.
 */
typedef enum
{
    ///
    E_XC_DLC_CURVE_MODE_BY_YAVG = 0,
    ///
    E_XC_DLC_CURVE_MODE_BY_HISTOGRAM ,
    ///
    E_XC_DLC_CURVE_MODE_BY_MIX ,
} E_XC_DLC_CURVE_MODE ;

/*!
 *  The DLC algorthm mode, ways of decide DLC algorithm.
 */
typedef enum
{
    ///
    E_XC_DLC_ALGORITHM_OLD = 0,
    ///
    E_XC_DLC_ALGORITHM_NEW ,
} E_XC_DLC_ALGORITHM_MODE ;
/*!
 *  The DLC curve settings, ways of enable DLC or, disable DLC.
 */
typedef enum
{
    /// fail
    E_XC_DLC_FAIL = 0,
    /// ok
    E_XC_DLC_OK = 1,
    /// get base address failed when initialize panel driver
    E_XC_DLC_GET_BASEADDR_FAIL,
    /// obtain mutex timeout when calling this function
    E_XC_DLC_OBTAIN_MUTEX_FAIL,
} E_XC_DLC_RESULT;


typedef enum
{
    E_XC_DLC_HISTOGRAM_32,
    E_XC_DLC_HISTOGRAM_8,
}E_XC_DLC_HISTOGRAM_TYPE;
/******************************************************************************/
/*         Structures                                                         */
/******************************************************************************/

//----------------------------
// XC DLC initialize
//----------------------------
/*!
 *  Initial Settings of MF Dyanmic Luma Curve
 */
typedef struct
{
    /// Default luma curve
    MS_U8 ucLumaCurve[16];
    /// Default luma curve 2a
    MS_U8 ucLumaCurve2_a[16];
    /// Default luma curve 2b
    MS_U8 ucLumaCurve2_b[16];
    /// Default luma curve 2
    MS_U8 ucLumaCurve2[16];

    /// default value: 10
    MS_U8 u8_L_L_U;
    /// default value: 10
    MS_U8 u8_L_L_D;
    /// default value: 10
    MS_U8 u8_L_H_U;
    /// default value: 10
    MS_U8 u8_L_H_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_D;
    /// -31 ~ 31 (bit7 = minus, ex. 0x88 => -8)
    MS_U8 ucCGCCGain_offset;
    /// 0x00~0x6F
    MS_U8 ucCGCChroma_GainLimitH;
    /// 0x00~0x10
    MS_U8 ucCGCChroma_GainLimitL;
    /// 0x01~0x20
    MS_U8 ucCGCYCslope;
    /// 0x01
    MS_U8 ucCGCYth;
    /// Compare difference of max and min bright
    MS_U8 ucDlcPureImageMode;
    /// n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcLevelLimit;
    /// n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDelta;
    /// n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    MS_U8 ucDlcAvgDeltaStill;
    /// min 17 ~ max 32
    MS_U8 ucDlcFastAlphaBlending;
    /// some event is triggered, DLC must do slowly // for PIP On/Off, msMultiPic.c
    MS_U8 ucDlcSlowEvent;
    /// for IsrApp.c
    MS_U8 ucDlcTimeOut;
    /// for force to do fast DLC in a while
    MS_U8 ucDlcFlickAlphaStart;
    /// default value: 128
    MS_U8 ucDlcYAvgThresholdH;
    /// default value: 0
    MS_U8 ucDlcYAvgThresholdL;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcBLEPoint;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint;
    /// 1: enable; 0: disable
    MS_U8 bCGCCGainCtrl : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableBLE : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableWLE : 1;
} XC_DLC_MFinit;


typedef struct
{
    MS_U32 u32DLC_MFinit_Ex_Version;   ///<Version of current structure. Please always set to "DLC_MFINIT_EX_VERSION" as input
    /// Default luma curve
    MS_U8 ucLumaCurve[16];
    /// Default luma curve 2a
    MS_U8 ucLumaCurve2_a[16];
    /// Default luma curve 2b
    MS_U8 ucLumaCurve2_b[16];
    /// Default luma curve 2
    MS_U8 ucLumaCurve2[16];

    /// default value: 10
    MS_U8 u8_L_L_U;
    /// default value: 10
    MS_U8 u8_L_L_D;
    /// default value: 10
    MS_U8 u8_L_H_U;
    /// default value: 10
    MS_U8 u8_L_H_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_L_D;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_U;
    /// default value: 128 (0x80)
    MS_U8 u8_S_H_D;
    /// -31 ~ 31 (bit7 = minus, ex. 0x88 => -8)
    MS_U8 ucCGCCGain_offset;
    /// 0x00~0x6F
    MS_U8 ucCGCChroma_GainLimitH;
    /// 0x00~0x10
    MS_U8 ucCGCChroma_GainLimitL;
    /// 0x01~0x20
    MS_U8 ucCGCYCslope;
    /// 0x01
    MS_U8 ucCGCYth;
    /// Compare difference of max and min bright
    MS_U8 ucDlcPureImageMode;
    /// n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcLevelLimit;
    /// n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDelta;
    /// n = 0 ~ 15 => 0: disable still curve, 1 ~ 15: enable still curve
    MS_U8 ucDlcAvgDeltaStill;
    /// min 17 ~ max 32
    MS_U8 ucDlcFastAlphaBlending;
    /// some event is triggered, DLC must do slowly // for PIP On/Off, msMultiPic.c
    MS_U8 ucDlcSlowEvent;
    /// for IsrApp.c
    MS_U8 ucDlcTimeOut;
    /// for force to do fast DLC in a while
    MS_U8 ucDlcFlickAlphaStart;
    /// default value: 128
    MS_U8 ucDlcYAvgThresholdH;
    /// default value: 0
    MS_U8 ucDlcYAvgThresholdL;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcBLEPoint;
    /// n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint;
    /// 1: enable; 0: disable
    MS_U8 bCGCCGainCtrl : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableBLE : 1;
    /// 1: enable; 0: disable
    MS_U8 bEnableWLE : 1;
    /// default value: 64
    MS_U8 ucDlcYAvgThresholdM;
    /// Compare difference of max and min bright
    MS_U8 ucDlcCurveMode;
    /// min 00 ~ max 128
    MS_U8 ucDlcCurveModeMixAlpha;
    ///
    MS_U8 ucDlcAlgorithmMode;
    /// Dlc Histogram Limit Curve
    MS_U8 ucDlcHistogramLimitCurve[DLC_HISTOGRAM_LIMIT_CURVE_ARRARY_NUM];
    ///
    MS_U8 ucDlcSepPointH;
    ///
    MS_U8 ucDlcSepPointL;
    ///
    MS_U16 uwDlcBleStartPointTH;
    ///
    MS_U16 uwDlcBleEndPointTH;
    ///
    MS_U8 ucDlcCurveDiff_L_TH;
    ///
    MS_U8 ucDlcCurveDiff_H_TH;
    ///
    MS_U16 uwDlcBLESlopPoint_1;
    ///
    MS_U16 uwDlcBLESlopPoint_2;
    ///
    MS_U16 uwDlcBLESlopPoint_3;
    ///
    MS_U16 uwDlcBLESlopPoint_4;
    ///
    MS_U16 uwDlcBLESlopPoint_5;
    ///
    MS_U16 uwDlcDark_BLE_Slop_Min;
    ///
    MS_U8 ucDlcCurveDiffCoringTH;
    ///
    MS_U8 ucDlcAlphaBlendingMin;
    ///
    MS_U8 ucDlcAlphaBlendingMax;
    ///
    MS_U8 ucDlcFlicker_alpha;
    ///
    MS_U8 ucDlcYAVG_L_TH;
    ///
    MS_U8 ucDlcYAVG_H_TH;
    ///
    MS_U8 ucDlcDiffBase_L;
    ///
    MS_U8 ucDlcDiffBase_M;
    ///
    MS_U8 ucDlcDiffBase_H;
} XC_DLC_MFinit_Ex;

/*!
 *  Initial Settings of Dynamic Backlight Control
 */
typedef struct
{
    /// Max PWM
    MS_U8 ucMaxPWM;
    /// Min PWM
    MS_U8 ucMinPWM;
    /// Max Video
    MS_U8 ucMax_Video;
    /// Mid Video
    MS_U8 ucMid_Video;
    /// Min Video
    MS_U8 ucMin_Video;
    /// Current PWM
    MS_U8 ucCurrentPWM;
    /// Alpha
    MS_U8 ucAlpha;
    /// Backlight thres
    MS_U8 ucBackLight_Thres;
    /// Avg delta
    MS_U8 ucAvgDelta;
    /// Flick alpha
    MS_U8 ucFlickAlpha;
    /// Fast alpha blending, min 17 ~ max 32
    MS_U8 ucFastAlphaBlending;
    // TBD
    MS_U8 ucLoop_Dly;
    // TBD
    MS_U8 ucLoop_Dly_H_Init;
    // TBD
    MS_U8 ucLoop_Dly_MH_Init;
    // TBD
    MS_U8 ucLoop_Dly_ML_Init;
    // TBD
    MS_U8 ucLoop_Dly_L_Init;
    /// Y gain H
    MS_U8 ucY_Gain_H;
    /// C gain H
    MS_U8 ucC_Gain_H;
    /// Y gain M
    MS_U8 ucY_Gain_M;
    /// C gain M
    MS_U8 ucC_Gain_M;
    /// Y gain L
    MS_U8 ucY_Gain_L;
    /// C gain L
    MS_U8 ucC_Gain_L;
    /// 1: enable; 0: disable
    MS_U8 bYGainCtrl : 1;
    /// 1: enable; 0: disable
    MS_U8 bCGainCtrl : 1;
}XC_DLC_DBC_MFinit;

typedef struct
{
    MS_U8      u8ColorPrimaries;
    MS_U8      u8TransferCharacteristics;
    MS_U8      u8MatrixCoefficients;
} XC_DLC_HDRMetadataMpegVUI;

///HDR Code////
typedef struct
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
} XC_DLC_HDRToneMappingData;

typedef struct
{
    MS_U16 u16tRx; // target Rx
    MS_U16 u16tRy; // target Ry
    MS_U16 u16tGx; // target Gx
    MS_U16 u16tGy; // target Gy
    MS_U16 u16tBx; // target Bx
    MS_U16 u16tBy; // target By
    MS_U16 u16tWx; // target Wx
    MS_U16 u16tWy; // target Wy
} XC_DLC_HDRGamutMappingData;

typedef struct
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
} XC_DLC_HDRMetadataHdmiTxInfoFrame;

/*!
 *  Initial  HDR   Settings
 */
typedef struct
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
    XC_DLC_HDRMetadataMpegVUI DLC_HDRMetadataMpegVUI;
    //HDR Tone Mapping Data
    XC_DLC_HDRToneMappingData DLC_HDRToneMappingData;
    //HDR Gamut Mapping Data
    XC_DLC_HDRGamutMappingData DLC_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    XC_DLC_HDRMetadataHdmiTxInfoFrame DLC_HDRMetadataHdmiTxInfoFrame;

} XC_DLC_HDRinit;


/*!
 *  Initial Settings of Dyanmic Luma Curve
 */
typedef struct
{
    /// Scaler DCL MF init
    XC_DLC_MFinit DLC_MFinit;
    /// Scaler DCL MF init Ex
    XC_DLC_MFinit_Ex DLC_MFinit_Ex;
    /// Curve Horizontal start
    MS_U16 u16CurveHStart;
    /// Curve Horizontal end
    MS_U16 u16CurveHEnd;
    /// Curve Vertical start
    MS_U16 u16CurveVStart;
    /// Curve Vertical end
    MS_U16 u16CurveVEnd;
    /// Scaler DLC MF init
    XC_DLC_DBC_MFinit DLC_DBC_MFinit;
#if (ENABLE_10_BIT_DLC)
    /// DLC init ext
    MS_BOOL b10BitsEn;
#endif
} XC_DLC_init;

/*!
 *  Luma Statistics & Data of Dyanmic Luma Curve
 */
typedef struct
{
    /// Luma histogram 32H
    MS_U16 g_wLumaHistogram32H[32];
    /// Luma total count
    MS_U16 g_wLumiTotalCount;
    /// Luma average temp
    MS_U16 g_wLumiAverageTemp;
    /// table
    MS_U8 g_ucTable[16];
    /// Histogram max
    MS_U8 g_ucHistogramMax;
    /// Histogram min
    MS_U8 g_ucHistogramMin;
} XC_DLC_lumadata;

/*!
 *  Infomation of XC_DLC library.
 */
typedef struct
{

} XC_DLC_ApiInfo;


/*!
 *  Current Status of XC_DLC and library info.
 */
typedef struct
{
    /// Scaler DLC init
    XC_DLC_init DLCinit;
    /// Scaler DLC Luma data
    XC_DLC_lumadata DLCluma;
    /// DLC Lib version
    MS_U8  u8DLC_LibVer;
    /// DLC MF version
    MS_U16 u16DLC_MFVer;
    /// DLC CGC MF version
    MS_U16 u16DLC_CGC_MFVer;
    /// DLC DBC init
    XC_DLC_DBC_MFinit DLC_DBCinit;
    /// DLC DBC MF version
    MS_U16 u16DLC_DBC_MFVer;
} XC_DLC_ApiStatus;
/*
 * DLC capture Range
 */
typedef struct
{
    MS_U16 wHStart;
    MS_U16 wHEnd;
    MS_U16 wVStart;
    MS_U16 wVEnd;
}XC_DLC_CAPTURE_Range;


/******************************************************************************/
/*         Variables                                                         */
/******************************************************************************/

/******************************************************************************/
/*         Function Prototypes                                                */
/******************************************************************************/

//-------------------------------------------------------------------------------------------------
/// DLC initiation
/// @ingroup DLC_INIT
// @param  pstXC_DLC_InitData             \b IN: TBD
// @param  u32InitDataLen                 \b IN: TBD
// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_Init_Ex(XC_DLC_init *pstXC_DLC_InitData, MS_U32 u32InitDataLen);

//-------------------------------------------------------------------------------------------------
/// Setting DLC
/// @ingroup DLC_FEATURE
// @param  DLC_MFinit_Ex             \b IN: DLC data
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_SetSetting_Ex(XC_DLC_MFinit_Ex *DLC_MFinit_Ex );


//-------------------------------------------------------------------------------------------------
/// Set DLC curve
/// @ingroup DLC_FEATURE
// @param  pNormal             \b IN: DLC normal curve
// @param  pLight              \b IN: DLC light curve
// @param  pDark               \b IN: DLC dark curve
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_SetCurve(MS_U8 *pNormal, MS_U8 *pLight, MS_U8 *pDark);

//-------------------------------------------------------------------------------------------------
/// Set BLE Slop Point
/// @ingroup DLC_FEATURE
// @param  pBLESlopPoint             \b IN: BLE Slop Point
//-------------------------------------------------------------------------------------------------
// void MApi_XC_DLC_SetBleSlopPoint(MS_U16 *pBLESlopPoint);

//-------------------------------------------------------------------------------------------------
/// Set Set HDR Init
// @param  pstDLC_HDRInitData             \b IN: HDR Init Info
//-------------------------------------------------------------------------------------------------
MS_BOOL SYMBOL_WEAK MApi_XC_DLC_SetHDRInit(XC_DLC_HDRinit *pstDLC_HDRInitData);

//-------------------------------------------------------------------------------------------------
/// DLC exit
/// @ingroup DLC_INIT
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
// MS_BOOL MApi_XC_DLC_Exit(void);

//-------------------------------------------------------------------------------------------------
///Enable/disable DLC to control MDrv_DLC_SetOnOff ,used DLC enbale register to control.
/// @ingroup DLC_FEATURE
///@param bSwitch \b IN:
///   - # TRUE  Enable
///   - # FALSE Disable
///@param  bWindow                 \b IN: The window which applys the function effect, MAIN_WINDOW:0
///                                                                                    SUB_WINDOW: 1
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_SetOnOff(MS_BOOL bSwitch, MS_BOOL bWindow);

//-------------------------------------------------------------------------------------------------
///Enable/disable DLC to control MDrv_DLC_Handler function.
/// @ingroup DLC_FEATURE
///@param bSwitch \b IN:
///   - # TRUE  Enable
///   - # FALSE Disable
// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
// MS_BOOL MApi_XC_DLC_SetDlcHandlerOnOff(MS_BOOL bDlcEnable );

/******************************************************************************/
//Enable/disable BLE to control BLE function.
/// @ingroup DLC_FEATURE
//@param bSwitch \b IN
//   - # TRUE  Enable
//   - # FALSE Disable
// @return @ref MS_BOOL
/******************************************************************************/
MS_BOOL MApi_XC_DLC_SetBleOnOff( MS_BOOL bSwitch );

/******************************************************************************/
//Enable/disable DLC Set Curve Both for main and sub.
/// @ingroup DLC_FEATURE
//@param bEnable \b IN
//   - # TRUE  Enable
//   - # FALSE Disable
/******************************************************************************/
void MApi_XC_DLC_EnableMainSubCurveSynchronization (MS_BOOL bEnable);

//-------------------------------------------------------------------------------------------------
/// DLC handler
/// @ingroup DLC_FEATURE
///@param  bWindow                 \b IN: The window which applys the function effect, MAIN_WINDOW:0
///                                                                                    SUB_WINDOW: 1
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_Handler(MS_BOOL bWindow);

//-------------------------------------------------------------------------------------------------
/// DLC get histogram handler
/// @ingroup DLC_INFO
///@param  bWindow                 \b IN: The window which applys the function effect, MAIN_WINDOW:0
///                                                                                    SUB_WINDOW: 1
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_GetHistogramHandler(MS_BOOL bWindow);

//-------------------------------------------------------------------------------------------------
/// DLC get average value from 0 to 255
/// @ingroup DLC_INFO
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8   MApi_XC_DLC_GetAverageValue(void);

//-------------------------------------------------------------------------------------------------
/// DLC get average exactness value from 0 to 1023
/// @ingroup DLC_INFO
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
// MS_U16 MApi_XC_DLC_GetAverageValue_Ex(void);

//-------------------------------------------------------------------------------------------------
/// DLC initilization curve
/// @ingroup DLC_INIT
// @param  u16HStart               \b IN: TBD
// @param  u16HEnd                 \b IN: TBD
// @param  u16VStart               \b IN: TBD
// @param  u16VEnd                 \b IN: TBD
//-------------------------------------------------------------------------------------------------
// void    MApi_XC_DLC_InitCurve (MS_U16 u16HStart, MS_U16 u16HEnd, MS_U16 u16VStart, MS_U16 u16VEnd);

//-------------------------------------------------------------------------------------------------
/// DLC speedup trigger
/// @ingroup DLC_FEATURE
// @param  u8Loop               \b IN: TBD
//-------------------------------------------------------------------------------------------------
// void    MApi_XC_DLC_SpeedupTrigger (MS_U8 u8Loop);

//-------------------------------------------------------------------------------------------------
/// DLC get luma curve status
/// @ingroup DLC_INFO
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_GetLumaCurveStatus( void );

//-------------------------------------------------------------------------------------------------
/// DLC CGC reset C gain
/// @ingroup DLC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_ResetCGain(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC check C gain in PQCom
/// @ingroup DLC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_CheckCGainInPQCom(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC reset Y gain
/// @ingroup DLC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_ResetYGain(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC reset Y gain in PQCom
/// @ingroup DLC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_CheckYGainInPQCom(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC reset
/// @ingroup DLC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_Reset(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC initilization
/// @ingroup DLC_INIT
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_Init(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC Re-initilization
/// @ingroup DLC_INIT
//-------------------------------------------------------------------------------------------------
// void    MApi_XC_DLC_CGC_ReInit(void);

//-------------------------------------------------------------------------------------------------
/// DLC CGC handler
/// @ingroup DLC_FEATURE
//-------------------------------------------------------------------------------------------------
void    MApi_XC_DLC_CGC_Handler(void);

//-------------------------------------------------------------------------------------------------
/// Get version (without Mutex protect)
/// @ingroup DLC_INFO
// @param ppVersion               \b IN: TBD
/// @return @ref E_XC_DLC_RESULT
//-------------------------------------------------------------------------------------------------
// E_XC_DLC_RESULT MApi_XC_DLC_GetLibVer(const MSIF_Version **ppVersion) ;

//-------------------------------------------------------------------------------------------------
/// DLC get information
/// @ingroup DLC_INFO
/// @return XC_DLC_ApiInfo
//-------------------------------------------------------------------------------------------------
// const XC_DLC_ApiInfo * MApi_XC_DLC_GetInfo(void);                                      ///< Get info from driver (without Mutex protect)

//-------------------------------------------------------------------------------------------------
/// Get DLC current status
/// @ingroup DLC_INFO
// @param pDrvStatus               \b IN: TBD
// @param bWindow                  \b IN: TBD
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_GetStatus_Ex(XC_DLC_ApiStatus *pDrvStatus, MS_BOOL bWindow);       ///< Get panel current status

//-------------------------------------------------------------------------------------------------
/// DLC get Dbg level
/// @ingroup DLC_INFO
/// @param u16DbgSwitch               \b IN: Set debug level
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_SetDbgLevel(MS_U16 u16DbgSwitch);                                  ///< Set debug level (without Mutex protect), refer to XC_DBGLEVEL_OFF

//-------------------------------------------------------------------------------------------------
/// Write DLC curve
/// @ingroup DLC_FEATURE
/// @param pu8Table                     \b IN: the value of curve
// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_WriteCurve(MS_U8 *pu8Table);

//-------------------------------------------------------------------------------------------------
/// Write DLC curve
/// @ingroup DLC_FEATURE
/// @param pu8Table                     \b IN: the value of curve
// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
// MS_BOOL MApi_XC_DLC_WriteCurve_Sub(MS_U8 *pu8Table);

//-------------------------------------------------------------------------------------------------
/// get histogram data
/// @ingroup DLC_INFO
/// @param pu16Histogram                \b OUT: the value of histogram
/// @param u8section_num                \b IN: the number of histogram section
// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
// MS_BOOL MApi_XC_DLC_GetHistogram(MS_U16 *pu16Histogram, E_XC_DLC_HISTOGRAM_TYPE enHistogramType);

/******************************************************************************/
/// API DLC DBC library start
/******************************************************************************/
typedef void   (*MApi_XC_DLC_Print_Callback)(MS_U8 /*PWM_VALUE*/);

typedef struct {
    MS_U8 *pCmdBuff;                           ///<1. command buffer from UART
    MS_U16 u16PNL_Width;                       ///<2. Panel Widht
    MS_U16 u16PNL_Height;                      ///<3. Panel Height
    MApi_XC_DLC_Print_Callback fnDLC_Putchar;  ///<4. User putchar function
}tDLC_CONTROL_PARAMS;

//-------------------------------------------------------------------------------------------------
/// brief uart debug command decoder for Dynamic Luminance Control.
/// @ingroup DLC_FEATURE
/// @param params   passin params
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_DecodeExtCmd(tDLC_CONTROL_PARAMS *params);

//-------------------------------------------------------------------------------------------------
/// Setup DLC capture range
/// @ingroup DLC_FEATURE
/// @param StuDlc_Range                     \b IN: Hstart, Hend, Vstart, Vend
//-------------------------------------------------------------------------------------------------
void MApi_XC_DLC_SetCaptureRange(XC_DLC_CAPTURE_Range *pu16_Range);

//-------------------------------------------------------------------------------------------------
/// Dlc power state control for fastboot
/// @ingroup DLC_FEATURE
/// @param  u16PowerState                 \b IN: power state
// @return @ref MS_U32                      \b return result of power state control.
//-------------------------------------------------------------------------------------------------
// MS_U32 MApi_XC_DLC_SetPowerState(EN_POWER_MODE enPowerState);

/******************************************************************************/
/// API DLC DBC library end
/******************************************************************************/


//================================ Obsolete Function Start =======================================
//-------------------------------------------------------------------------------------------------
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Alert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Alert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!! below function will be "REMOVED" or "REFACTORING" later, please do not use it !!!!!!!!!
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// @ingroup DLC_ToBeRemove
//-------------------------------------------------------------------------------------------------

// void MApi_XC_DLC_SetSetting(XC_DLC_MFinit DLC_MFinit ); // Setting DLC
//-------------------------------------------------------------------------------------------------
/// @ingroup DLC_ToBeRemove
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_Init(XC_DLC_init *pstXC_DLC_InitData, MS_U32 u32InitDataLen); // DLC initiation

//-------------------------------------------------------------------------------------------------
/// @ingroup DLC_ToBeRemove
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DLC_GetStatus(XC_DLC_ApiStatus *pDrvStatus, MS_BOOL bWindow);  // Get DLC current status

//================================ Obsolete Function End =======================================


#ifdef __cplusplus
}
#endif

#endif // _API_XC_DLC_H_


