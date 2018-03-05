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
/// @file  drvACE.h
/// @brief ACE library interface header file
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DRV_ACE_H
#define DRV_ACE_H

#ifdef _MDRV_ACE_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif
#ifdef __cplusplus
extern "C"
{
#endif
//******** ENUM DEFINITIONS ********//
INTERFACE enum
{
    E_ACE_POOL_ID_INTERNAL = 0, // just for test, please refine.
    E_ACE_POOL_ID_INTERNAL_1, // just for test, please refine.
    E_ACE_POOL_ID_MAX, // just for test, please refine.
} E_ACE_POOL_ID;

/******************************************************************************/
/*                   Export variables                                                                                     */
/******************************************************************************/
typedef enum
{
    ACE_INFO_TYPE_VERSION,
    ACE_INFO_TYPE_CONTRAST,
    ACE_INFO_TYPE_R_GAIN,
    ACE_INFO_TYPE_G_GAIN,
    ACE_INFO_TYPE_B_GAIN,
    ACE_INFO_TYPE_SATURATION,
    ACE_INFO_TYPE_HUE,
    ACE_INFO_TYPE_COLOR_CORRECTION_XY_R,
    ACE_INFO_TYPE_COLOR_CORRECTION_XY_G,
    ACE_INFO_TYPE_COLOR_CORRECTION_XY_B,
    ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R,
    ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G,
    ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B,
    ACE_INFO_TYPE_BRIGHTNESS_R,
    ACE_INFO_TYPE_BRIGHTNESS_G,
    ACE_INFO_TYPE_BRIGHTNESS_B,

    ACE_INFO_TYPE_COMPILER_VERSION,
    ACE_INFO_TYPE_YUV_TO_RGB_MATRIX_SEL,

    ACE_INFO_TYPE_IHC_USER_COLOR1,
    ACE_INFO_TYPE_IHC_USER_COLOR2,
    ACE_INFO_TYPE_IHC_USER_COLOR3,
    ACE_INFO_TYPE_IHC_USER_COLOR4,
    ACE_INFO_TYPE_IHC_USER_COLOR5,
    ACE_INFO_TYPE_IHC_USER_COLOR6,
    ACE_INFO_TYPE_IHC_USER_COLOR7,
    ACE_INFO_TYPE_IHC_USER_COLOR8,
    ACE_INFO_TYPE_IHC_USER_COLOR9,
    ACE_INFO_TYPE_IHC_USER_COLOR10,
    ACE_INFO_TYPE_IHC_USER_COLOR11,
    ACE_INFO_TYPE_IHC_USER_COLOR12,
    ACE_INFO_TYPE_IHC_USER_COLOR13,
    ACE_INFO_TYPE_IHC_USER_COLOR14,
    ACE_INFO_TYPE_IHC_USER_COLOR15,
    ACE_INFO_TYPE_IHC_USER_COLOR0,

    ACE_INFO_TYPE_ICC_USER_COLOR1,
    ACE_INFO_TYPE_ICC_USER_COLOR2,
    ACE_INFO_TYPE_ICC_USER_COLOR3,
    ACE_INFO_TYPE_ICC_USER_COLOR4,
    ACE_INFO_TYPE_ICC_USER_COLOR5,
    ACE_INFO_TYPE_ICC_USER_COLOR6,
    ACE_INFO_TYPE_ICC_USER_COLOR7,
    ACE_INFO_TYPE_ICC_USER_COLOR8,
    ACE_INFO_TYPE_ICC_USER_COLOR9,
    ACE_INFO_TYPE_ICC_USER_COLOR10,
    ACE_INFO_TYPE_ICC_USER_COLOR11,
    ACE_INFO_TYPE_ICC_USER_COLOR12,
    ACE_INFO_TYPE_ICC_USER_COLOR13,
    ACE_INFO_TYPE_ICC_USER_COLOR14,
    ACE_INFO_TYPE_ICC_USER_COLOR15,
    ACE_INFO_TYPE_ICC_USER_COLOR0,

    ACE_INFO_TYPE_IBC_USER_COLOR1,
    ACE_INFO_TYPE_IBC_USER_COLOR2,
    ACE_INFO_TYPE_IBC_USER_COLOR3,
    ACE_INFO_TYPE_IBC_USER_COLOR4,
    ACE_INFO_TYPE_IBC_USER_COLOR5,
    ACE_INFO_TYPE_IBC_USER_COLOR6,
    ACE_INFO_TYPE_IBC_USER_COLOR7,
    ACE_INFO_TYPE_IBC_USER_COLOR8,
    ACE_INFO_TYPE_IBC_USER_COLOR9,
    ACE_INFO_TYPE_IBC_USER_COLOR10,
    ACE_INFO_TYPE_IBC_USER_COLOR11,
    ACE_INFO_TYPE_IBC_USER_COLOR12,
    ACE_INFO_TYPE_IBC_USER_COLOR13,
    ACE_INFO_TYPE_IBC_USER_COLOR14,
    ACE_INFO_TYPE_IBC_USER_COLOR15,
    ACE_INFO_TYPE_IBC_USER_COLOR0,
    ACE_INFO_TYPE_CONTRAST_MIN_VALUE,
    ACE_INFO_TYPE_CONTRAST_MAX_VALUE,
    ACE_INFO_TYPE_DUMMY,
} EnuACEInfoType;

typedef enum
{
    ACE_MATRIX_ID__YUV_TO_RGB,
    ACE_MATRIX_ID__COLOR_CORRECTION,
    ACE_MATRIX_ID__SRGB
}EnuAceMatrixId;

typedef enum
{
    ACE_YUV_TO_RGB_MATRIX_SDTV, // Use Std CSCM for SDTV
    ACE_YUV_TO_RGB_MATRIX_HDTV, // Use Std CSCM for HDTV
    ACE_YUV_TO_RGB_MATRIX_USER, // Use user define CSCM, specified by tUserYVUtoRGBMatrix
} E_ACE_YUVTORGBInfoType;

/// MWE border color
typedef enum
{
    MWE_BORDER_COLOR_BLACK,
    MWE_BORDER_COLOR_WHITE,
    MWE_BORDER_COLOR_BLUE,
    MWE_BORDER_COLOR_RED,
    MWE_BORDER_COLOR_GREEN,
    MWE_BORDER_COLOR_MAX,
}EN_MWE_BORDER_COLOR;

typedef struct  __attribute__((packed))
{
    MS_U32 Xpos;
    MS_U32 Ypos;
    MS_U32 Xsize;
    MS_U32 Ysize;
}MWE_RECT;

typedef struct __attribute__((packed)) _PanelSizeType
{
    MS_U16 u16Pnl_Hstart;
    MS_U16 u16Pnl_Vstart;
    MS_U16 u16Pnl_Width;
    MS_U16 u16Pnl_Height;
}PanelSizeType;

typedef enum MWEType
{
    MWE_OFF,
    MWE_H_SPLIT,
    MWE_MOVE,
    MWE_ZOOM,
    MWE_NUMS,
}MWEType;

typedef struct
{
    MS_BOOL bSValid; // source data valid
    MS_BOOL bTValid; // target data valid
    MS_U8 u8SMode; // 0->KbKr, 1->YCgCo, 2->2020CL
    MS_U16 u16SKb;
    MS_U16 u16SKr;
    MS_U8 u8TMode; // 0->KbKr, 1->YCgCo, 2->2020CL
    MS_U16 u16TKb;
    MS_U16 u16TKr;
} ACE_DRV_HDR_YUV2RGBData;

typedef struct __attribute__((packed))
{
    MS_U8      u8ColorPrimaries;
    MS_U8      u8TransferCharacteristics;
    MS_U8      u8MatrixCoefficients;
} ACE_DRV_HDRMetadataMpegVUI;

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
} ACE_DRV_HDRToneMappingData;

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
} ACE_DRV_HDRGamutMappingData;

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
} ACE_DRV_HDRMetadataHdmiTxInfoFrame;

typedef struct __attribute__((packed))
{
    MS_U8 PixelFormat;              // Pixel Format
    MS_U8 Colorimetry;              // Color imetry
    MS_U8 ExtendedColorimetry;      // Extended Color imetry
    MS_U8 RgbQuantizationRange;     // Rgb Quantization Range
    MS_U8 YccQuantizationRange;     // Ycc Quantization Range
    MS_U8 StaticMetadataDescriptorID; //Static Metadata Descriptor ID
} ACE_DRV_HDRHdmiTxAviInfoFrame;

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
    ACE_DRV_HDRMetadataMpegVUI ACE_HDRMetadataMpegVUI;
    //HDR Tone Mapping Data
    ACE_DRV_HDRToneMappingData ACE_HDRToneMappingData;
    //HDR Gamut Mapping Data
    ACE_DRV_HDRGamutMappingData ACE_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    ACE_DRV_HDRMetadataHdmiTxInfoFrame ACE_HDRMetadataHdmiTxInfoFrame;
    //HDR Metadata Hdmi Tx Info Frame
    ACE_DRV_HDRHdmiTxAviInfoFrame ACE_HDRHdmiTxAviInfoFrame;
} ACE_DRV_HDRinit;

INTERFACE ACE_DRV_HDRinit g_ACEinitParameters;


MS_U16  msACE_GetVersion(void);
void    InitACEVar(void* pInstance);
void    msACEPCForceYUVtoRGB(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable );
void    msAdjustPCContrast(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Contrast );
void    msAdjustPCRGB(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Red, MS_U8 u8Green, MS_U8 u8Blue );
void    msAdjustVideoContrast(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Contrast );
void    msAdjustVideoRGB(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Red, MS_U8 u8Green, MS_U8 u8Blue);
void    msAdjustPCRGB_10Bits( void* pInstance, MS_BOOL bScalerWin, MS_U16 u16Red, MS_U16 u16Green, MS_U16 u16Blue );
void    msAdjustVideoRGB_10Bits(void* pInstance,  MS_BOOL bScalerWin, MS_U16 u16RCon, MS_U16 u16GCon, MS_U16 u16BCon);
void    msACESelectYUVtoRGBMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Matrix, MS_S16 *psUserYUVtoRGBMatrix );
void    msACESetColorCorrectionTable(void* pInstance, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable );
void    msACESetPCsRGBTable(void* pInstance, MS_BOOL bScalerWin, MS_S16 *psPCsRGBTable );
void    msAdjustVideoSaturation(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Saturation );
void    msAdjustVideoHue(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Hue );
MS_U16  msACE_GetInfo(void* pInstance, MS_BOOL bWindow, EnuACEInfoType eACEInfoType );
void    msSetVideoColorMatrix(void* pInstance, MS_BOOL bScalerWin );
void    msSetPCColorMatrix(void* pInstance, MS_BOOL bScalerWin );
void    msACE_SetColorMatrixControl(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable );
void    msACE_SetRBChannelRange(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bRange );
MS_BOOL  msACESetBypassColorMatrix(void* pInstance, MS_BOOL bEnable);

/******************************************************************************/
/*                   Function Prototypes                                      */
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_GetVersion()
/// @brief \b Function \b Description:  Get version of ACE library
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b Version
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_ACE_GetVersion(void);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_Init()
/// @brief \b Function \b Description:  Initialize ACE
/// @param <IN>        \b None
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_Init(void* pInstance, MS_BOOL bScalerWin, MS_S16* psColorCorrectionTable, MS_S16* psPCsRGBTable );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PCForceYUVtoRGB()
/// @brief \b Function \b Description:  Force transfer YUV to RGB
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b bEnable
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PCForceYUVtoRGB(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetContrast()
/// @brief \b Function \b Description:  Adjust Contrast
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Contrast
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetContrast(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetRGB()
/// @brief \b Function \b Description:  Adjust Video RGB
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Red
/// @param <IN>        \b u8Green
/// @param <IN>        \b u8Blue
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetRGB(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bUseYUVSpace, MS_U8 u8Red, MS_U8 u8Green, MS_U8 u8Blue );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetRGB_10Bits()
/// @brief \b Function \b Description:  Adjust Video RGB
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u16Red
/// @param <IN>        \b u16Green
/// @param <IN>        \b u16Blue
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetRGB_10Bits( void* pInstance,MS_BOOL bScalerWin, MS_BOOL bUseYUVSpace, MS_U16 u16Red, MS_U16 u16Green, MS_U16 u16Blue );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetPostRGBGain()
/// @brief \b Function \b Description:  Adjust Video Post RGB Gain
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8RedGain
/// @param <IN>        \b u8GreenGain
/// @param <IN>        \b u8BlueGain
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetPostRGBGain(void* pInstance,MS_BOOL bScalerWin, MS_U16 u8RedGain, MS_U16 u8GreenGain, MS_U16 u8BlueGain);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetPostRGBGainOffset()
/// @brief \b Function \b Description:  Adjust Post RGB Gain and Offset simultaneously
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8RedGain
/// @param <IN>        \b u8GreenGain
/// @param <IN>        \b u8BlueGain
/// @param <IN>        \b u16RedOffset
/// @param <IN>        \b u16GreenOffset
/// @param <IN>        \b u16BlueOffset
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetPostRGBGainOffset(void* pInstance,MS_BOOL bScalerWin,
                                      MS_U16 u8RedGain,    MS_U16 u8GreenGain,    MS_U16 u8BlueGain,
                                      MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset, MS_U8 u8Step);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_GetInfo()
/// @brief \b Function \b Description:  Get ACE Info
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucInfoType
/// @param <OUT>       \b None
/// @param <RET>       \b wReturnValue
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_ACE_GetPostRGBGain(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16ACEinfo);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_PicSetPostRGBOffset()
/// @brief \b Function \b Description:  Adjust Video Post RGB Offset
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u16RedOffset
/// @param <IN>        \b u16GreenOffset
/// @param <IN>        \b u16BlueOffset
/// @param <IN>        \b u8Step
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_PicSetPostRGBOffset(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset, MS_U8 u8Step);
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SelectYUVtoRGBMatrix()
/// @brief \b Function \b Description:  Select YUV to RGB Matrix
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucMatrix
/// @param <IN>        \b psUserYUVtoRGBMatrix
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SelectYUVtoRGBMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetColorCorrectionTable()
/// @brief \b Function \b Description:  Set Color Correction Table
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b psColorCorrectionTable
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetColorCorrectionTable(void* pInstance, MS_BOOL bScalerWin);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetPCsRGBTable()
/// @brief \b Function \b Description:  Set PCs RGB Table
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b psPCsRGBTable
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetPCsRGBTable(void* pInstance, MS_BOOL bScalerWin);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_AdjustVideoSaturation()
/// @brief \b Function \b Description:  Adjust Video Saturation
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Saturation
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetSaturation(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Saturation );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_SetHue()
/// @brief \b Function \b Description:  Adjust Video Hue
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b u8Hue
/// @param <OUT>       \b None
/// @param <RET>       \b None
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
void MDrv_ACE_SetHue(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Hue );

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ACE_GetInfo()
/// @brief \b Function \b Description:  Get ACE Info
/// @param <IN>        \b bScalerWin
/// @param <IN>        \b ucInfoType
/// @param <OUT>       \b None
/// @param <RET>       \b wReturnValue
/// @param <GLOBAL>    \b None
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_ACE_GetACEInfo(void* pInstance, MS_BOOL bWindow, MS_U16 u16ACEinfo );


void MDrv_ACE_DMS(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bisATV );
void MDrv_ACE_GetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix );
void MDrv_ACE_SetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix );
MS_BOOL MDrv_ACE_SetBypassColorMatrix(void* pInstance, MS_BOOL bEnable );
void MDrv_ACE_PatchDTGColorChecker(void* pInstance, MS_U8 u8Mode);
void MDrv_ACE_SetFleshTone(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEn, MS_U8 u8FlashTone); ///< set flash tone
void MDrv_ACE_SetBlackAdjust(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8BlackAdjust);

void MDrv_ACE_init_riu_base(MS_VIRT ptr_riu_base );
void MDrv_ACE_SetSharpness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Sharpness );
void MDrv_ACE_SetBrightness (void* pInstance, MS_BOOL bScalerWin, MS_U8 u8RedBrightness, MS_U8 u8GreenBrightness, MS_U8 u8BlueBrightness );
void MDrv_ACE_SetBrightnessPrecise(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedBrightness, MS_U16 u16GreenBrightness, MS_U16 u16BlueBrightness);
MS_U16 MDrv_ACE_GetBrightness (void* pInstance, MS_BOOL bScalerWin, EnuACEInfoType eACEInfoType);
MS_BOOL MDrv_ACE_SetPreYOffset(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8PreYOffset);
MS_U8 MDrv_ACE_GetPreYOffset(void* pInstance, MS_BOOL bScalerWin);
MS_BOOL MDrv_ACE_IsPostRGBGain(void* pInstance, MS_BOOL bWindow);
MS_BOOL MDrv_ACE_IsPostRGBOffset(void* pInstance, MS_BOOL bWindow);
MS_BOOL MDrv_XC_ACE_Set_IHC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize);
MS_BOOL MDrv_XC_ACE_Set_ICC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize);

//------------------------------
// MWE related
//------------------------------
void MDrv_XC_ACE_MWEinit(void* pInstance, MS_U16 u16MWEHstart, MS_U16 u16MWEVstart, MS_U16 u16MWEWidth, MS_U16 u16MWEHeight);
void MDrv_XC_ACE_MWESetWin(void* pInstance, MWE_RECT *pRect);
void MDrv_XC_ACE_MWESetBorder(void* pInstance, MS_BOOL bScalerWin, MS_BOOL ben, MS_U8 u8color, MS_U16 u16_l, MS_U16 u16_r, MS_U16 u16_t, MS_U16 u16_d);
void MDrv_XC_ACE_MWEEnable(void* pInstance, MS_BOOL bEnable, MS_BOOL bLoadFromTable);
void MDrv_XC_ACE_MWECloneVisualEffect(void* pInstance);
void MDrv_ACE_3DClonePQMap(void* pInstance, XC_ACE_WEAVETYPE enWeaveType);
void MDrv_XC_ACE_MWESetRegRow(void* pInstance, MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask);

//------------------------------
// DynamicnNR related
//------------------------------

MS_U8 MDrv_XC_ACE_DNR_GetMotion(void* pInstance);
MS_U8 MDrv_ACE_DNR_GetCoringThreshold(void* pInstance, MS_BOOL bScalerWin);
MS_U8 MDrv_ACE_DNR_GetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin);
MS_U8 MDrv_ACE_DNR_GetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin);

void MDrv_ACE_DNR_SetCoringThreshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetNM_V(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetGNR_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetGNR_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetCP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetDP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetNM_H_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetNM_H_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetGray_Ground_Gain(void* pInstance, MS_U16 u16val);
void MDrv_ACE_DNR_SetGray_Ground_En(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetSC_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetSpikeNR_0(void* pInstance, MS_U16 u16val);
void MDrv_ACE_DNR_SetSpikeNR_1(void* pInstance, MS_U16 u16val);
void MDrv_ACE_DNR_SetSNR_NM(void* pInstance, MS_U16 u16val);
void MDrv_ACE_DNR_SetBank_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void MDrv_ACE_DNR_SetNRTable(void* pInstance, MS_U8 *pTbl);
void MDrv_ACE_SetColorMatrixControl(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable );
void MDrv_ACE_SetRBChannelRange(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bRange);
void MDrv_ACE_WriteColorMatrixBurst(void* pInstance, MS_BOOL bWindow, MS_U16* psMatrix );
MS_BOOL MDrv_ACE_IsSupportMLoad(void* pInstance);
MS_BOOL MDrv_ACE_SetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val);
MS_BOOL MDrv_ACE_GetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8* pu8Val);
MS_BOOL MDrv_ACE_SetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val);
MS_BOOL MDrv_ACE_GetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8* pu8Val);
MS_BOOL MDrv_ACE_SetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val);
MS_BOOL MDrv_ACE_GetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8* pu8Val);

MS_BOOL MDrv_ACE_SetHDRInit(void* pInstance,ACE_DRV_HDRinit * pstACE_HDRInitData);
void msACE_HDR_Main(void *pInstance);

void MDrv_ACE_ColorCorrectionTable(void* pInstance, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable );
//OS dependent.

void MDrv_ACE_SetSkipWaitVsync(void* pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn);
MS_BOOL MDrv_ACE_GetSkipWaitVsync(void* pInstance, MS_BOOL bScalerWin);

MS_U32 MDrv_ACE_GetDeviceNum(void);


#ifdef __cplusplus
}
#endif
#endif // DRV_ACE_H

