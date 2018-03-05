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
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_gflip_st.h
// @brief  GFlip KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MDRV_GFLIP_ST_H
#define _MDRV_GFLIP_ST_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Type and Structure Declaration
//=============================================================================
#define MULTIINFO_NUM               8UL

#define MS_HDR_VERSION  4

//IO Ctrl struct defines:
typedef enum
{
    /// Color format RGB555 and Blink.
    E_GFLIP_COLOR_RGB555_BLINK    =0,
    /// Color format RGB565.
    E_GFLIP_COLOR_RGB565          =1,
    /// Color format ARGB4444.
    E_GFLIP_COLOR_ARGB4444        =2,
    /// Color format alpha blink.
    E_GFLIP_COLOR_2266      =3,
    /// Color format I8 (256-entry palette).
    E_GFLIP_COLOR_I8              =4,
    /// Color format ARGB8888.
    E_GFLIP_COLOR_ARGB8888        =5,
    /// Color format ARGB1555.
    E_GFLIP_COLOR_ARGB1555        =6,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_ABGR8888        =7,
    /// Color format RGB555/YUV422.
    E_GFLIP_COLOR_RGB555YUV422    =8,
    /// Color format YUV422.
    E_GFLIP_COLOR_YUV422          =9,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_RGBA5551        =10,
    /// Color format ARGB8888.  - Andriod format
    E_GFLIP_COLOR_RGBA4444        =11,

    /// Invalid color format.
    E_GFLIP_COLOR_INVALID
} GFLIP_COLOR_TYPE;

typedef union
{
    MS_U8 u8DWinIntInfo;
    struct
    {
        MS_U8 bDWinIntWADR:1;
        MS_U8 bDWinIntPROG:1;
        MS_U8 bDWinIntTF:1;
        MS_U8 bDWinIntBF:1;
        MS_U8 bDWinIntVS:1;
        MS_U8 reserved:3;
    }sDwinIntInfo;
}GFLIP_DWININT_INFO, *PGFLIP_DWININT_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY u64Addr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
}MS_GFLIP_INFO, *PMS_GFLIP_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY u64Addr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_BOOL bTLBEnable;
    MS_PHY u64TLBAddr;
}MS_TLB_GFLIP_INFO, *PMS_TLB_GFLIP_INFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY u64MainAddr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_PHY u64SubAddr; //in
}MS_GFLIP_3DINFO, *PMS_GFLIP_3DINFO;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_PHY u64MainAddr; //in
    MS_U32 u32TagId; //in
    MS_U32 u32QEntry; //InOut
    MS_U32 u32Result; //Out
    MS_PHY u64SubAddr; //in
    MS_BOOL bTLBEnable;
    MS_PHY u64TLBAddr;
}MS_TLB_GFLIP_3DINFO, *PMS_TLB_GFLIP_3DINFO;

typedef struct
{
    MS_U8 u8GopInfoCnt;
    MS_GFLIP_3DINFO astGopInfo[MULTIINFO_NUM];
}MS_GFLIP_MULTIINFO, *PMS_GFLIP_MULTIINFO;

typedef struct
{
    MS_U8 u8GopInfoCnt;
    MS_TLB_GFLIP_3DINFO astTLBGopInfo[MULTIINFO_NUM];
}MS_TLB_GFLIP_MULTIINFO, *PMS_TLB_GFLIP_MULTIINFO;

typedef struct
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;
    MS_BOOL bResetDWinIntInfo;

}MS_GFLIP_DWININT_INFO, *PMS_GFLIP_DWININT_INFO;

typedef struct
{
    GFLIP_DWININT_INFO gflipDWinIntInfo;
    MS_U32  u32Timeout;
    MS_BOOL bResetDWinIntInfo;
}MS_GFLIP_DWININT_INFO2, *PMS_GFLIP_DWININT_INFO2;

typedef struct
{
    MS_BOOL bEnable; //InOut
    MS_U8   u8FrameCount; //Out, value range: 0~3
    MS_U8   u8Result; //Out
}MS_GFLIP_VECAPTURESTATE, *PMS_GFLIP_VECAPTURESTATE;

typedef struct
{
    MS_U32 u32GopIdx; //in
    MS_U32 u32GwinIdx; //in
    MS_U32 u32Result; //Out
}MS_GFLIP_GOPGWINIDX,*PMS_GFLIP_GOPGWINIDX;

typedef struct
{
    MS_PHY u64Addr;
    MS_U16 u16X;
    MS_U16 u16Y;
    MS_U16 u16W;
    MS_U16 u16H;
    MS_U8  u8GopIdx;
    MS_U8  u8GwinIdx;
    GFLIP_COLOR_TYPE clrType;
}MS_GWIN_INFO;

typedef struct
{
    MS_BOOL bCurveChange;  // Curve Change
    MS_U8  u8LumaCurve[16];  // Default luma curve from Supernova DLC.ini
    MS_U8  u8LumaCurve2_a[16];  // Default luma curve 2a  from Supernova DLC.ini
    MS_U8  u8LumaCurve2_b[16];  // Default luma curve 2b  from Supernova DLC.ini
    MS_U8  u8Result; //Out
}MS_DLC_INFO;

typedef struct
{
    MS_BOOL bBLEPointChange;  // BLE Point Change
    MS_U16  u16BLESlopPoint[6];  // Default BLE Slop Point from Supernova DLC.ini
    MS_U8  u8Result; //Out
}MS_BLE_INFO;


typedef struct
{
    MS_BOOL bInitInfoChange;  // Init Info Change

    // Default luma curve
    MS_U8 ucLumaCurve[16];
    MS_U8 ucLumaCurve2_a[16];
    MS_U8 ucLumaCurve2_b[16];
    MS_U8 ucDlcHistogramLimitCurve[17];

    MS_U8 ucDlcPureImageMode; // Compare difference of max and min bright
    MS_U8 ucDlcLevelLimit; // n = 0 ~ 4 => Limit n levels => ex. n=2, limit 2 level 0xF7, 0xE7
    MS_U8 ucDlcAvgDelta; // n = 0 ~ 50, default value: 12
    MS_U8 ucDlcAvgDeltaStill; // n = 0 ~ 15 => 0: disable still curve, 1 ~ 15 => 0.1 ~ 1.5 enable still curve
    MS_U8 ucDlcFastAlphaBlending; // min 17 ~ max 32
    MS_U8 ucDlcYAvgThresholdL; // default value: 0
    MS_U8 ucDlcYAvgThresholdH; // default value: 128

    MS_U8 ucDlcBLEPoint; // n = 24 ~ 64, default value: 48
    MS_U8 ucDlcWLEPoint; // n = 24 ~ 64, default value: 48
    MS_U8 bEnableBLE : 1; // 1: enable; 0: disable
    MS_U8 bEnableWLE : 1; // 1: enable; 0: disable
    MS_U8 ucDlcYAvgThresholdM;
    MS_U8 ucDlcCurveMode;
    MS_U8 ucDlcCurveModeMixAlpha;
    MS_U8 ucDlcAlgorithmMode;
    MS_U8 ucDlcSepPointH;
    MS_U8 ucDlcSepPointL;
    MS_U16 uwDlcBleStartPointTH;
    MS_U16 uwDlcBleEndPointTH;
    MS_U8 ucDlcCurveDiff_L_TH;
    MS_U8 ucDlcCurveDiff_H_TH;
    MS_U16 uwDlcBLESlopPoint_1;
    MS_U16 uwDlcBLESlopPoint_2;
    MS_U16 uwDlcBLESlopPoint_3;
    MS_U16 uwDlcBLESlopPoint_4;
    MS_U16 uwDlcBLESlopPoint_5;
    MS_U16 uwDlcDark_BLE_Slop_Min;
    MS_U8 ucDlcCurveDiffCoringTH;
    MS_U8 ucDlcAlphaBlendingMin;
    MS_U8 ucDlcAlphaBlendingMax;
    MS_U8 ucDlcFlicker_alpha;
    MS_U8 ucDlcYAVG_L_TH;
    MS_U8 ucDlcYAVG_H_TH;
    MS_U8 ucDlcDiffBase_L;
    MS_U8 ucDlcDiffBase_M;
    MS_U8 ucDlcDiffBase_H;
    // NEW DLC
    MS_U8 ucLMaxThreshold;
    MS_U8 ucLMinThreshold;
    MS_U8 ucLMaxCorrection;
    MS_U8 ucLMinCorrection;
    MS_U8 ucRMaxThreshold;
    MS_U8 ucRMinThreshold;
    MS_U8 ucRMaxCorrection;
    MS_U8 ucRMinCorrection;
    MS_U8 ucAllowLoseContrast;
    MS_U8 ucKernelDlcAlgorithm;
    MS_U8 ucDlcHistogramSource;
}MS_DLC_INIT_INFO;

typedef struct __attribute__((packed))
{
    MS_U8      u8ColorPrimaries;
    MS_U8      u8TransferCharacteristics;
    MS_U8      u8MatrixCoefficients;
} MS_DLC_HDR_METADATA_MPEG_VUI;

// Data format follows HDR_Metadata_MpegSEI_MasteringColorVolume
typedef struct __attribute__((packed))
{
    MS_U16     display_primaries_x[3]; // x = data*0.00002    [709] {15000, 7500, 32000}
    MS_U16     display_primaries_y[3]; // y = data*0.00002    [709] {30000, 3000, 16500}
    MS_U16     white_point_x; // x = data*0.00002    [709] 15635
    MS_U16     white_point_y; // y = data*0.00002    [709] 16450
    MS_U32     max_display_mastering_luminance; // data*0.0001 nits    [600nits] 6000000
    MS_U32     min_display_mastering_luminance; // data*0.0001 nits    [0.3nits] 3000
} MS_DLC_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME;

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
} MS_DLC_HDRToneMappingData;

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
} MS_DLC_HDRGamutMappingData;

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
} MS_DLC_HDRMetadataHdmiTxInfoFrame;

typedef struct __attribute__((packed))
{
    MS_U8 PixelFormat;              // Pixel Format
    MS_U8 Colorimetry;              // Color imetry
    MS_U8 ExtendedColorimetry;      // Extended Color imetry
    MS_U8 RgbQuantizationRange;     // Rgb Quantization Range
    MS_U8 YccQuantizationRange;     // Ycc Quantization Range
    MS_U8 StaticMetadataDescriptorID; //Static Metadata Descriptor ID
} MS_DLC_HDRHdmiTxAviInfoFrame;

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
    MS_DLC_HDR_METADATA_MPEG_VUI DLC_HDRMetadataMpegVUI;
    /// HDR Tone Mapping Data
    MS_DLC_HDRToneMappingData DLC_HDRToneMappingData;
    /// HDR Gamut Mapping Data
    MS_DLC_HDRGamutMappingData DLC_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    MS_DLC_HDRMetadataHdmiTxInfoFrame DLC_HDRMetadataHdmiTxInfoFrame;
    //HDR Hdmi Tx Avi Info Frame
    MS_DLC_HDRHdmiTxAviInfoFrame DLC_HDRHdmiTxInfoAviFrame;
} MS_HDR_INFO;

// HDR use customer DLC curve.
typedef struct __attribute__((packed))
{
    MS_BOOL bFixHdrCurve;
    MS_U16 u16DlcCurveSize;
    MS_U8 *pucDlcCurve;
#if !defined (__aarch64__)
    void    *pDummy;
#endif
} MS_DLC_HDRCustomerDlcCurve;

// HDR use customer color primaries.
typedef struct __attribute__((packed))
{
    MS_BOOL bCustomerEnable;
    MS_U16 u16sWx;
    MS_U16 u16sWy;
} MS_DLC_HDRCustomerColorPrimaries;

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
    MS_U8 u8TMO_Algorithm;                // default 0.  0: 18 level TMO algorithm, 1: 512 level TMO algorithm.

} MS_DLC_HDRNewToneMapping;

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
    MS_DLC_HDR_METADATA_MPEG_VUI DLC_HDRMetadataMpegVUI;
    /// HDR Tone Mapping Data
    MS_DLC_HDRToneMappingData DLC_HDRToneMappingData;
    /// HDR Gamut Mapping Data
    MS_DLC_HDRGamutMappingData DLC_HDRGamutMappingData;
    //HDR Metadata Hdmi Tx Info Frame
    MS_DLC_HDRMetadataHdmiTxInfoFrame DLC_HDRMetadataHdmiTxInfoFrame;
    // Customer DLC Curve
    MS_DLC_HDRCustomerDlcCurve DLC_HDRCustomerDlcCurve;
    // Customer color primarie.
    MS_DLC_HDRCustomerColorPrimaries DLC_HDRCustomerColorPrimaries;
    //HDR Hdmi Tx Avi Info Frame
    MS_DLC_HDRHdmiTxAviInfoFrame DLC_HDRHdmiTxInfoAviFrame;
    /// HDR metadata MPEG SEI mastering color volume/
    MS_DLC_HDR_METADATA_MPEG_SEI_MASTERING_COLOR_VOLUME DLC_HDRMetadataMpegSEIMasteringColorVolume;
    /// New tone mapping parameters.
    MS_DLC_HDRNewToneMapping DLC_HDRNewToneMapping;
} MS_HDR_INFO_EXTEND;

#endif //_MDRV_GFLIP_ST_H
