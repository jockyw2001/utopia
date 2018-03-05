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
/// @file   apiDMS.h
/// @brief  DMS Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_DMS_TYPE_H_
#define _API_DMS_TYPE_H_

#include "MsCommon.h"
#include "MsDevice.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// library information
#define MSIF_DMS_LIB_CODE               {'M','W','S','_'}
#define MSIF_DMS_LIBVER                 {'0','0'}
#define MSIF_DMS_BUILDNUM               {'0','0'}
#define MSIF_DMS_CHANGELIST             {'0','0','0','0','0','0','0','1'}

#define DMS_API_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'    */  \
    MSIF_CLASS,                         /* '00'      */  \
    MSIF_CUS,                           /* 0x0000    */  \
    MSIF_MOD,                           /* 0x0000    */  \
    MSIF_CHIP,                                           \
    MSIF_CPU,                                            \
    MSIF_DMS_LIB_CODE  ,                /* IP__      */  \
    MSIF_DMS_LIBVER    ,                /* 0.0 ~ Z.Z */  \
    MSIF_DMS_BUILDNUM  ,                /* 00 ~ 99   */  \
    MSIF_DMS_CHANGELIST,                /* CL#       */  \
    MSIF_OS

#define VERSION_ST_DMS_DISPFRAMEFORMAT      1
#define VERSION_ST_DMS_COLORDESCRIPTION     1
#define VERSION_ST_DMS_DOLBYHDRINFO         1
#define VERSION_ST_DMS_HDRINFO              1
#define VERSION_ST_DMS_DISP_FRM_INFO_EXT    1
#define VERSION_ST_DMS_COLORHWFORMAT        1
#define VERSION_ST_DMS_COLORSWFORMAT        1
#define VERSION_ST_DMS_FRAMEFORMAT          1

#define VERSION_ST_DMS_INITDATA             1
#define VERSION_ST_DMS_SETWIN_INFO          1
#define VERSION_ST_DMS_GETWIN_INFO          1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Define return value of MApi_DMS
typedef enum
{
    E_DMS_FAIL = 0,
    E_DMS_OK = 1,
} EN_DMS_RESULT;

typedef enum
{
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT,                // in MVC case it is L view 2 bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_INTERLACE = 1,        // interlace bottom 8bit will share the same enum value
    E_DMS_DISP_FRM_INFO_EXT_TYPE_DOLBY_EL = 1,         // with dolby enhance layer 8bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE = 2,  // interlace bottom 2bit will share the same enum
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_DOLBY_EL = 2,   // value with dolby enhance layer 2bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_MVC,            // R view 2 bit
    E_DMS_DISP_FRM_INFO_EXT_TYPE_INTERLACE_MVC,
    E_DMS_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE_MVC = 5, // MVC interlace R-View 2bit will share the
    E_DMS_DISP_FRM_INFO_EXT_TYPE_DOLBY_META = 5,          // same enum with dolby meta data
    E_DMS_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN,
    E_DMS_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN_MVC,
    E_DMS_DISP_FRM_INFO_EXT_TYPE_MAX,
} EN_DMS_DISP_FRM_INFO_EXT_TYPE;

typedef enum
{
    E_DMS_CODEC_TYPE_NONE,
    E_DMS_CODEC_TYPE_MPEG2,
    E_DMS_CODEC_TYPE_H263,
    E_DMS_CODEC_TYPE_MPEG4,
    E_DMS_CODEC_TYPE_DIVX311,
    E_DMS_CODEC_TYPE_DIVX412,
    E_DMS_CODEC_TYPE_FLV,
    E_DMS_CODEC_TYPE_VC1_ADV,
    E_DMS_CODEC_TYPE_VC1_MAIN,
    E_DMS_CODEC_TYPE_RV8,
    E_DMS_CODEC_TYPE_RV9,
    E_DMS_CODEC_TYPE_H264,
    E_DMS_CODEC_TYPE_AVS,
    E_DMS_CODEC_TYPE_MJPEG,
    E_DMS_CODEC_TYPE_MVC,
    E_DMS_CODEC_TYPE_VP8,
    E_DMS_CODEC_TYPE_HEVC,
    E_DMS_CODEC_TYPE_VP9,
    E_DMS_CODEC_TYPE_HEVC_DV,
    E_DMS_CODEC_TYPE_MAX,
} EN_DMS_CODECTYPE;

typedef enum
{
    E_DMS_FRAME_TYPE_I,
    E_DMS_FRAME_TYPE_P,
    E_DMS_FRAME_TYPE_B,
    E_DMS_FRAME_TYPE_OTHER,
    E_DMS_FRAME_TYPE_MAX,
} EN_DMS_FRAMETYPE;

typedef enum
{
    E_DMS_FIELD_TYPE_NONE,
    E_DMS_FIELD_TYPE_TOP,
    E_DMS_FIELD_TYPE_BOTTOM,
    E_DMS_FIELD_TYPE_BOTH,
    E_DMS_FIELD_TYPE_MAX,
} EN_DMS_FIELDTYPE;

typedef enum
{
    E_DMS_VIEW_TYPE_CENTER,
    E_DMS_VIEW_TYPE_LEFT,
    E_DMS_VIEW_TYPE_RIGHT,
    E_DMS_VIEW_TYPE_TOP,
    E_DMS_VIEW_TYPE_BOTTOM,
    E_DMS_VIEW_TYPE_MAX,
} EN_DMS_VIEWTYPE;

typedef enum
{
    E_DMS_COLOR_FORMAT_HW_HVD,                 //YUV420 HVD tiled format
    E_DMS_COLOR_FORMAT_HW_MVD,                 //YUV420 MVD tiled format
    E_DMS_COLOR_FORMAT_SW_YUV420_PLANAR,       //YUV420 Planar
    E_DMS_COLOR_FORMAT_SW_RGB565,              //RGB565
    E_DMS_COLOR_FORMAT_SW_ARGB8888,            //ARGB8888
    E_DMS_COLOR_FORMAT_YUYV,                   //YUV422 YUYV
    E_DMS_COLOR_FORMAT_SW_RGB888,              //RGB888
    E_DMS_COLOR_FORMAT_10BIT_TILE,             //YUV420 tiled 10 bits mode
    E_DMS_COLOR_FORMAT_SW_YUV420_SEMIPLANAR,   //YUV420 SemiPlanar
    E_DMS_COLOR_FORMAT_YUYV_CSC_BIT601,        //YUV422 YUYV from RGB2YUV bit601 mode
    E_DMS_COLOR_FORMAT_YUYV_CSC_255,           //YUV422 YUYV from RGB2YUV 0~255 mode
    E_DMS_COLOR_FORMAT_HW_EVD,                 //YUV420 EVD tiled format
    E_DMS_COLOR_FORMAT_MAX,
} EN_DMS_COLORFORMAT;

typedef enum
{
    E_DMS_STATUS_NOT_SUPPORT            = 0x0,
    E_DMS_STATUS_INITIALIZED              = 0x1,
    E_DMS_STATUS_SUPPORT_MULTI_WINDOW = (0x1<<1),
    E_DMS_STATUS_SUPPORT_VSYNC_BRIDGE  = (0x1<<2),
    E_DMS_STATUS_SUPPORT_AVSYNC         = (0x1<<3),
    E_DMS_STATUS_SUPPORT_SECURE_VIDEO  = (0x1<<4),
} EN_DMS_STATUS;

typedef enum
{
    E_DMS_AR_DEFAULT = 0,
    E_DMS_AR_16x9,
    E_DMS_AR_4x3,
    E_DMS_AR_AUTO,
    E_DMS_AR_MAX,
} EN_DMS_ASPECT_RATIO;

//DMS window define
typedef struct
{
    MS_U32 u32x;
    MS_U32 u32y;
    MS_U32 u32width;
    MS_U32 u32height;
} ST_DMS_WINDOW;

//DMS Init struct
typedef struct
{
    MS_U32 u32Version;                         /// ST_DMS_INITDATA version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_INITDATA)
    MS_PHY phyBuffAddr;                          /// DMS buffer start address
    MS_U32 u32BuffSize;
} ST_DMS_INITDATA;

//DMS set window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_SETWIN_INFO version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_SETWIN_INFO)
    MS_BOOL bXCWinInfoValid;
    ST_DMS_WINDOW stDispWin;
    ST_DMS_WINDOW stCropWin;
    EN_DMS_ASPECT_RATIO enARC;
    ST_DMS_WINDOW stOutputWin;
} ST_DMS_SETWIN_INFO;

//DMS get window info
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                           /// ST_DMS_GETWIN_INFO version
    MS_U32 u32Length;                           /// sizeof(ST_DMS_GETWIN_INFO)
    MS_BOOL bXCWinInfoValid;
    ST_DMS_WINDOW stDispWin;
    ST_DMS_WINDOW stCropWin;
    MS_U32 u16DeviceID;
    MS_U32 enInputSourceType;
    EN_DMS_ASPECT_RATIO enARC;
    ST_DMS_WINDOW stOutputWin;
    MS_U32 u32ZOrder;
} ST_DMS_GETWIN_INFO;

////Frame Info set
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_COLORDESCRIPTION version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_COLORDESCRIPTION)
    //color_description: indicates the chromaticity/opto-electronic coordinates of the source primaries
    MS_U8 u8ColorPrimaries;
    MS_U8 u8TransferCharacteristics;
    // matrix coefficients in deriving YUV signal from RGB
    MS_U8 u8MatrixCoefficients;
} ST_DMS_COLORDESCRIPTION;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_DOLBYHDRINFO version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_DOLBYHDRINFO)
    // bit[0:1] 0: Disable 1:Single layer 2: Dual layer, bit[2] 0:Base Layer 1:Enhance Layer
    MS_U8  u8DVMode;
    MS_PHY phyHDRMetadataAddr;
    MS_U32 u32HDRMetadataSize;
    MS_PHY phyHDRRegAddr;
    MS_U32 u32HDRRegSize;
    MS_PHY phyHDRLutAddr;
    MS_U32 u32HDRLutSize;
    MS_U8  u8DMEnable;
    MS_U8  u8CompEnable;
    MS_U8  u8CurrentIndex;
} ST_DMS_DOLBYHDRINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_HDRINFO version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_HDRINFO)
    // bit[0]: MS_ColorDescription present or valid, bit[1]: MS_MasterColorDisplay present or valid
    MS_U32 u32FrmInfoExtAvail;
    // mastering color display: color volumne of a display
    ST_DMS_COLORDESCRIPTION   stColorDescription;
    //DMS_MasterColorDisplay stMasterColorDisplay;
    ST_DMS_DOLBYHDRINFO       stDolbyHDRInfo;
} ST_DMS_HDRINFO;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_COLORHWFORMAT version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_COLORHWFORMAT)
    MS_PHY phyLumaAddr;
    MS_PHY phyChromaAddr;
    MS_PHY phyLumaAddr2Bit;
    MS_PHY phyChromaAddr2Bit;
    MS_U32 u32LumaPitch;
    MS_U32 u32ChromaPitch;
    MS_U32 u32LumaPitch2Bit;
    MS_U32 u32ChromaPitch2Bit;
    MS_U32 u32MFCodecInfo;
    MS_PHY phyMFCBITLEN;
    MS_PHY phyLumaAddr_subsample;
    MS_PHY phyChromaAddr_subsample;
    MS_U16 u16Pitch_subsample;
    MS_U8  u8TileMode_subsample;
} ST_DMS_COLORHWFORMAT;

typedef struct DLL_PACKED
{
    MS_U32 u32Version;                         /// ST_DMS_FRAMEFORMAT version
    MS_U32 u32Length;                         /// sizeof(ST_DMS_FRAMEFORMAT)
    EN_DMS_FRAMETYPE eFrameType;
    EN_DMS_FIELDTYPE eFieldType;
    EN_DMS_VIEWTYPE eViewType;
    MS_U32 u32Width;
    MS_U32 u32Height;
    MS_U32 u32CropLeft;
    MS_U32 u32CropRight;
    MS_U32 u32CropTop;
    MS_U32 u32CropBottom;
    ST_DMS_COLORHWFORMAT sHWFormat;
    MS_U32 u32Idx;
    MS_U32 u32PriData;
    MS_U8 u8LumaBitdepth;
    MS_U8 u8ChromaBitdepth;
} ST_DMS_FRAMEFORMAT;

//
typedef struct DLL_PACKED
{
    MS_U32 u32Version;                           /// ST_DMS_DispFrameFormat version
    MS_U32 u32Length;                           /// sizeof(ST_DMS_DispFrameFormat)
    MS_U32 OverlayID;
    ST_DMS_FRAMEFORMAT sFrames[2];
    EN_DMS_COLORFORMAT eColorFormat;
    MS_U32 FrameNum;
    MS_PHY u64Pts;
    MS_U32 CodecType;
    MS_U32 u32FrameRate;
    MS_U32 u32AspectWidth;
    MS_U32 u32AspectHeight;
    MS_U32 u32VdecStreamVersion;
    MS_U32 u32VdecStreamId;
    MS_U32 u32UniqueId;
    MS_U8 u8AspectRate;
    MS_U8 u8Interlace;
    MS_U8 u8FrcMode;
    MS_U8 u83DMode;
    MS_U8 u8BottomFieldFirst;
    MS_U8 u8FreezeThisFrame;
    MS_U8 u8ToggleTime;
    MS_U8 u8MCUMode;
    MS_U8 u8FieldCtrl;          // control one field mode, always top or bot when FF or FR
    MS_U8 u8ApplicationType;
    MS_U8 u83DLayout;           // 3D layout from SEI, the possible value is OMX_3D_LAYOUT enum in OMX_Video.h
    MS_U8 u8ColorInXVYCC;
    MS_U8 u8LowLatencyMode;     // for CTS or other application, drop new frame when render too fast
    MS_U8 u8VdecComplexity;
    MS_U8 u8HTLBTableId;
    MS_U8 u8HTLBEntriesSize;
    MS_U8 u8AFD;               //active frame code
    ST_DMS_HDRINFO sHDRInfo;
    MS_U16 u16MIUBandwidth;
    MS_U16 u16Bitrate;
    MS_U32 u32TileMode;
    MS_PHY phyHTLBEntriesAddr;
} ST_DMS_DISPFRAMEFORMAT;

typedef struct
{
    MS_U32 u32R;
    MS_U32 u32G;
    MS_U32 u32B;
    MS_U32 u32A;
} ST_DMS_COLOR;

#ifdef __cplusplus
}
#endif

#endif

