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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMS.h
/// @author MStar Semiconductor Inc.
/// @brief  DMS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DMS_H_
#define _DRV_DMS_H_


#ifdef __cplusplus
extern "C"
{
#endif
#include "UFO.h"

#include "apiDMS_Type.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define DMS_VERSION_DIP_GOP
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
#ifdef MDRV_DMS_C
#define DRV_DMS_INTERFACE
#else
#define DRV_DMS_INTERFACE extern
#endif

#define DMS_VERSION_VSYNCBRDIGE
#define DMS_MAX_WINDOW_NUM 2
#define DMS_DIP_QUEUEDEPTH  8
#define DMS_Invalid_ReadPointer_ID  DMS_DIP_QUEUEDEPTH

//SHM resource
typedef enum
{
    /// YCrYCb.
    E_DMS_DIP_FMT_YUV422 = 0,
    /// RGB domain
    E_DMS_DIP_FMT_RGB565,
    /// RGB domain
    E_DMS_DIP_FMT_ARGB8888,
    /// YUV420 HVD tile
    E_DMS_DIP_FMT_YUV420,
    /// YC separate 422
    E_DMS_DIP_FMT_YC422,
    /// YUV420 H265 tile
    E_DMS_DIP_FMT_YUV420_H265,
    /// YUV420 H265_10bits tile
    E_DMS_DIP_FMT_YUV420_H265_10BITS,
    /// YUV420 planer
    E_DMS_DIP_FMT_YUV420_PLANER,
    /// YUV420 semi planer
    E_DMS_DIP_FMT_YUV420_SEMI_PLANER,
    /// YUV422 2.5D DI Top field
    E_DMS_DIP_FMT_YUV422_MED_DI_FIELDTYPE_TOP,
    /// YUV422 2.5D DI Bottom field
    E_DMS_DIP_FMT_YUV422_MED_DI_FIELDTYPE_BOTTOM,
    /// YUV420 2.5D DI Top field
    E_DMS_DIP_FMT_YUV420_MED_DI_FIELDTYPE_TOP,
    /// YUV420 2.5D DI Bottom field
    E_DMS_DIP_FMT_YUV420_MED_DI_FIELDTYPE_BOTTOM,
    E_DMS_DIP_FMT_MAX
} EN_DMS_DIP_FMT;

typedef enum
{
    E_DMS_VIDEO_CODEC_UNKNOWN = -1,     /// Video codec type is unknown.
    E_DMS_VIDEO_CODEC_MPEG4,            /// Video codec type is MPEG 4.
    E_DMS_VIDEO_CODEC_MJPEG,            /// Video codec type is motion JPG.
    E_DMS_VIDEO_CODEC_H264,             /// Video codec type is H264.
    E_DMS_VIDEO_CODEC_RM,               /// Video codec type is RealVideo.
    E_DMS_VIDEO_CODEC_TS,               /// Video codec type is TS File.
    E_DMS_VIDEO_CODEC_MPEG,             /// Video codec type is MPEG 1/2.
    E_DMS_VIDEO_CODEC_VC1,              /// Video codec type is VC1.
    E_DMS_VIDEO_CODEC_AVS,              /// Video codec type is Audio Video Standard.
    E_DMS_VIDEO_CODEC_FLV,              /// Video codec type is FLV.
    E_DMS_VIDEO_CODEC_MVC,              /// Video codec type is MVC.
    E_DMS_VIDEO_CODEC_VP6,              /// Video codec type is VP6.
    E_DMS_VIDEO_CODEC_VP8,              /// Video codec type is VP8.
    E_DMS_VIDEO_CODEC_HEVC,             /// Video codec type is HEVC.
    E_DMS_VIDEO_CODEC_VP9,              /// Video codec type is VP9.
}E_DMS_VIDEO_CODEC;

typedef enum
{
    E_DMS_VIDEO_SCAN_TYPE_PROGRESSIVE = 0 ,
    E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME,
    E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD,
    E_DMS_VIDEO_SCAN_TYPE_MAX,
}E_DMS_VIDEO_SCAN_TYPE;

typedef enum
{
    E_DMS_VIDEO_FIELD_ORDER_TYPE_BOTTOM = 0,
    E_DMS_VIDEO_FIELD_ORDER_TYPE_TOP,
    E_DMS_VIDEO_FIELD_ORDER_TYPE_MAX,
}E_DMS_VIDEO_FIELD_ORDER_TYPE;

typedef enum
{
    E_DMS_VIDEO_FIELD_TYPE_NONE = 0,
    E_DMS_VIDEO_FIELD_TYPE_TOP,
    E_DMS_VIDEO_FIELD_TYPE_BOTTOM,
    E_DMS_VIDEO_FIELD_TYPE_BOTH,
    E_DMS_VIDEO_FIELD_TYPE_MAX,
}E_DMS_VIDEO_FIELD_TYPE;

typedef enum
{
    E_DMS_VIDEO_TILE_MODE_NONE = 0,
    E_DMS_VIDEO_TILE_MODE_16x16,
    E_DMS_VIDEO_TILE_MODE_16x32,
    E_DMS_VIDEO_TILE_MODE_32x16,
    E_DMS_VIDEO_TILE_MODE_32x32,
    E_DMS_VIDEO_TILE_MODE_MAX,
}E_DMS_VIDEO_TILE_MODE;

/// DIP source data format
typedef enum
{

    E_DMS_VIDEO_DATA_FMT_YUV422 = 0,    /// YCrYCb.
    E_DMS_VIDEO_DATA_FMT_RGB565,        /// RGB domain
    E_DMS_VIDEO_DATA_FMT_ARGB8888,      /// RGB domain
    E_DMS_VIDEO_DATA_FMT_YUV420,        /// YUV420 HVD tile
    E_DMS_VIDEO_DATA_FMT_YC422,          /// YC separate 422
    E_DMS_VIDEO_DATA_FMT_YUV420_H265,    /// YUV420 H265 tile
    E_DMS_VIDEO_DATA_FMT_YUV420_H265_10BITS,/// YUV420 H265_10bits tile
    E_DMS_VIDEO_DATA_FMT_YUV420_PLANER, /// YUV420 planer
    E_DMS_VIDEO_DATA_FMT_YUV420_SEMI_PLANER,/// YUV420 semi planer
    E_DMS_VIDEO_DATA_FMT_MAX
} E_DMS_VIDEO_DATA_FMT;

typedef struct
{
    MS_U32 u32version;
    MS_U32 u32size;
} DMS_VIDEO_MFDEC_VER_CRL;

typedef enum
{
    E_DMS_DUMMY_VIDEO_H26X_MODE =0x00,
    E_DMS_DUMMY_VIDEO_VP9_MODE  =0x01,
}E_DMS_VIDEO_MFDEC_VP9_MODE;

typedef struct
{
    DMS_VIDEO_MFDEC_VER_CRL stMFDec_HTLB_VerCtl;
    MS_PHY u32HTLBEntriesAddr;
    MS_U8  u8HTLBEntriesSize;
    MS_U8  u8HTLBTableId;
    void* pHTLBInfo;
} DMS_VIDEO_MFDEC_HTLB_INFO;

typedef struct
{
    MS_BOOL bMFDec_Enable;
    MS_U8 u8MFDec_Select;
    MS_BOOL bHMirror; // no use
    MS_BOOL bVMirror; // no use
    MS_BOOL bUncompress_mode;
    MS_BOOL bBypass_codec_mode;
    E_DMS_VIDEO_MFDEC_VP9_MODE en_MFDecVP9_mode;
    MS_U16 u16StartX;
    MS_U16 u16StartY;
    MS_PHY phyBitlen_Base;
    MS_U16 u16Bitlen_Pitch;
    DMS_VIDEO_MFDEC_HTLB_INFO stMFDec_HTLB_Info; //reserve
    void* pMFDecInfo; //reserve
}DMS_VIDEO_MFDEC_INFO;

typedef struct
{
    MS_BOOL bValid;                                 /// frame buffer Valid
    MS_U16 u16SrcWidth;                             /// frame buffer Width
    MS_U16 u16SrcHeight;                            /// frame buffer Height
    MS_U16 u16CropRight;                            ///right cropping
    MS_U16 u16CropLeft;                             ///left cropping
    MS_U16 u16CropBottom;                           ///bottom cropping
    MS_U16 u16CropTop;                              ///top cropping
    MS_U16 u16SrcPitch;                             /// frame buffer pitch
    MS_PHY u32SrcLumaAddr;                          /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_PHY u32SrcChromaAddr;                        /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_PHY u32SrcLumaAddr_2bit;                     /// physical address of Luma LSB 2bit buffer (Main10 profile)
    MS_PHY u32SrcChromaAddr_2bit;                   /// physical address of Chroma LSB 2bit buffer (Main10 profile)
    MS_PHY u32SrcLumaAddrI;                         /// physical address of Luma bottom field (interlace)
    MS_PHY u32SrcLumaAddrI_2bit;                    /// physical address of Luma LSB 2bit bottom field (interlace)
    MS_PHY u32SrcChromaAddrI;                       /// physical address of Chroma bottom field (interlace)
    MS_PHY u32SrcChromaAddrI_2bit;                  /// physical address of Chroma LSB 2bit bottom field (interlace)
    MS_U16 u16Src10bitPitch;                        /// frame buffer 10bit Pitch
    MS_BOOL b10bitData;                             /// frame buffer 10bit data
    MS_U8 u8LumaBitdepth;
    MS_U32 u32FrameRate ;
    E_DMS_VIDEO_DATA_FMT eFmt;
    MS_U32 u32Window;
    E_DMS_VIDEO_CODEC eCODEC;
    E_DMS_VIDEO_SCAN_TYPE eScanType;
    E_DMS_VIDEO_FIELD_ORDER_TYPE eFieldOrderType;
    E_DMS_VIDEO_FIELD_TYPE eFieldType;
    E_DMS_VIDEO_TILE_MODE eTileMode;
    DMS_VIDEO_MFDEC_INFO stMFdecInfo;
    // New Flip
    MS_U32 u32FrameIndex;                           /// Frame index
    MS_U32 u32VDECStreamID;                         /// Provide the VDEC stream ID to XC to access MApi_VDEC_EX_DisplayFrame() and MApi_VDEC_EX_ReleaseFrame()
    MS_U32 u32VDECStreamVersion;                    /// Provide the VDEC stream ID Version to XC to access MApi_VDEC_EX_DisplayFrame() and MApi_VDEC_EX_ReleaseFrame()
    MS_U32 u32PriData;                              /// Frame reference count
    MS_U64 u64Pts;
    //2nd buffer
    MS_PHY phySrc2ndBufferLumaAddr;
    MS_PHY phySrc2ndBufferChromaAddr;
    MS_U16 u16Src2ndBufferPitch;
    MS_U8 u8Src2ndBufferTileMode;

    //Di Task using member
    MS_BOOL bIsAfterDiTask;
    MS_BOOL bIs2ndField;
    MS_U8 u8DiOutputRingBufferID;
} DMS_VDECFRAME_INFO;

typedef struct
{
    union
    {
        struct
        {
            MS_U8 u4DSIndex0 : 4;
            MS_U8 u4DSIndex1 : 4;
        };
        MS_U8 u8DSIndex;
    };
}DS_Index;

typedef struct
{
    MS_U8 u8SizeChange;
    MS_U8 u8FrcMode;
    MS_BOOL bDS_Enable;
    MS_BOOL bVsyncIntAlive;
    MS_U16 u16SrcWidth;
    MS_U16 u16SrcHeight;
    MS_U16 u16FrameRate;
    MS_U32 CodecType;
    MS_U32 u32AspectWidth;
    MS_U32 u32AspectHeight;
    MS_U32 u32CropLeft;
    MS_U32 u32CropRight;
    MS_U32 u32CropTop;
    MS_U32 u32CropBottom;
    MS_U32 panelWidth;
    MS_U32 panelHeight;
    MS_U8 u8DNRFrameCnt;
    MS_U8 u8FieldCtrl;
    MS_U8 u8Freeze;
    MS_U8 u8Interlace;
    MS_U8 u8UpdateFrcMode;
    MS_U8 u8ForcePMode;
    MS_U8 u8ForceInterlace;
    MS_U8 u8SaveBandwidthMode;
    MS_U8 u8MirrorMode;
    DS_Index ds_curr_index;
    MS_U32 u32LastTime;
    MS_U32 u32FrameCount;
    ST_DMS_DISPFRAMEFORMAT lastdff;
    ST_DMS_WINDOW stOutSideCropWin;
    MS_U32 u32WidthForOutsideCrop;
    MS_U32 u32HeightForOutsideCrop;
    ST_DMS_WINDOW stCurrentCropWin;
} DMS_DRV_CTRL;

//
typedef enum
{
    E_DMS_MVOP_FLOW_UNKNOWN           = 0,
    E_DMS_MVOP_FLOW_FROM_DIP          = 1,
    E_DMS_MVOP_FLOW_FROM_VDEC         = 2,
    E_DMS_MVOP_FLOW_MAX,
} E_DMS_MVOP_FLOW_CONTROL;

typedef enum
{
    E_DMS_DIP_MULTIVIEW_BUF_ID_0      = 0,
    E_DMS_DIP_MULTIVIEW_BUF_ID_1      = 1,
    E_DMS_DIP_MULTIVIEW_BUF_ID_MAX,
} E_DMS_DIP_MULTIVIEW_BUF_ID;

typedef enum
{
    E_DMS_DIP_CLIENT_GOP              = 0,
    E_DMS_DIP_CLIENT_MVOP             = 1,
    E_DMS_DIP_CLIENT_MAX,
} E_DMS_DIP_CLIENT;

typedef enum
{
    E_DMS_DIP_NORMAL              = 0,
    E_DMS_DIP_DI                  = 1,
    E_DMS_DIP_CAPTURE             = 2,
    E_DMS_DIP_MAX,
} E_DMS_DIP_ACTION;

#ifdef DMS_VERSION_DIPGOP
typedef struct
{
    MS_BOOL bDIPwinUse;
    ST_DMS_WINDOW stCropWin;
    ST_DMS_WINDOW stDstWin;
    MS_PHY u32DIPStartMemAddr;
    MS_PHY u32DIPEndMemAddr;
    MS_PHY u32DIPDoubleBufStartMemAddr;
    MS_PHY u32DIPDoubleBufEndMemAddr;
    MS_PHY phyDIPRingBufMemAddr[DMS_DI_RING_BUFFER];
    MS_U32 u32RingBufferSize;
    MS_U32 u32Window;
    MS_PHY u32GEStartMemAddr;
    MS_PHY u32GEEndMemAddr;
} DMS_DIPWIN_INFO;

/// DIP tile block
typedef enum
{
    DMS_DIP_TILE_BLOCK_R_NONE  = 0x0,
    DMS_DIP_TILE_BLOCK_W_NONE  = 0x1,
    DMS_DIP_TILE_BLOCK_R_16_32 = 0x2,
    DMS_DIP_TILE_BLOCK_W_16_32 = 0x3,
    DMS_DIP_TILE_BLOCK_R_32_16 = 0x4,
    DMS_DIP_TILE_BLOCK_W_32_16 = 0x5,
    DMS_DIP_TILE_BLOCK_R_32_32 = 0x6,
    DMS_DIP_TILE_BLOCK_W_32_32 = 0x7,
}EN_DMS_DIP_TILE_BLOCK;

typedef struct
{
    MS_U32 u32Enable;
    MS_U32 u32Visible;
    MS_U32 u32FrameRate;
    MS_U32 u32Width;
    MS_U32 u32Height;
    EN_DMS_DIP_FMT enColorFormat;
    EN_DMS_DIP_TILE_BLOCK enTileBlock;
    MS_PHY u32StartMemAddr;
    MS_PHY u32EndMemAddr;
    MS_PHY u32SecondBufStartMemAddr;
    MS_PHY u32SecondBufEndMemAddr;
    MS_PHY u32ThirdBufStartMemAddr;
    MS_PHY u32ThirdBufEndMemAddr;
    //MS_PHY phyRingBufMemAddr[DMS_CAPTUREWIN_RING_BUFFER];
    ST_DMS_CAPTURE_INFO stCaptureRingBuf[DMS_CAPTUREWIN_RING_BUFFER];
    MS_U32 u32Window;
} DMS_DIPCAPTUREWIN_INFO;
#endif

typedef struct
{
    MS_U16  u16WritePointer;
    MS_U16  u16PreWritePointer;
} DMS_DIPCaptureWin_WritePointer;

typedef struct
{
    MS_U16  u16WindowCaptured;
    MS_U16  u16WindowRemoved;
} DMS_DIPCaptureWin_WindowChanged;

typedef struct
{
    MS_U16  u16WritePointer;
    MS_U16  u16ReadPointer;
} DMS_DIPWriteReadPointer;

typedef struct
{
    EN_DMS_DIP_FMT eDIPRFmt;
    EN_DMS_DIP_FMT eDIPWFmt;
    MS_U32 u32PanelWidth;
    MS_U32 u32PanelHeight;
} DMS_DIPMEM_INFO;

typedef struct
{
    MS_BOOL     bCleanBuf;
    MS_BOOL     bDispChange;
} DMS_DISP_BUFFER_FLAG;

typedef enum
{
    E_DMS_ID_VAR = 0,          //
} EN_DMS_POOL_TYPE;

MS_U32 MDrv_DMS_Get_Semaphore(void* pInstance, EN_DMS_POOL_TYPE ePoolType);
MS_U32 MDrv_DMS_Release_Semaphore(void* pInstance, EN_DMS_POOL_TYPE ePoolType);

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Init(void* pInstance, ST_DMS_INITDATA *pstDMS_InitData);
EN_DMS_RESULT MDrv_DMS_SetMemoryType(void* pInstance, ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_CreateWindow(void* pInstance, ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_SetDigitalDecodeSignalInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_ClearDigitalDecodeSignalInfo(void* pInstance, MS_U32 u32WindowID);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_DestroyWindow(void* pInstance, MS_U32 u32WindowID);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_SetWindow(void* pInstance, MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_GetWindowInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Video_Flip(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_SetZOrder(void* pInstance, MS_U32 u32WindowID, MS_U32 u32ZOrder);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Video_Freeze(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Video_Mute(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Set_MuteColor(void* pInstance, MS_U32 u32WindowID, ST_DMS_COLOR stMuteColor);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_GetStatus(void* pInstance, MS_U32 *pu32Status);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_GetCapability(void* pInstance, EN_DMS_CAPABILITY *peCapability);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_SetOutputLayer(void* pInstance, ST_DMS_WINDOW *pstLayer);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Set_3D_Mode(void* pInstance, MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Set_CaptureInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Get_CaptureBuffer(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
DRV_DMS_INTERFACE EN_DMS_RESULT MDrv_DMS_Release_CaptureBuffer(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);

#ifdef __cplusplus
}
#endif

#endif // _DRV_DMS_H_
