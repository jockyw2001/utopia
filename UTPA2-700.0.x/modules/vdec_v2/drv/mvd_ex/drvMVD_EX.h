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
/// @file  drvMVD.h
/// @brief MPEG-2/4 Video Decoder header file
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MVD_H_
#define _DRV_MVD_H_




////////////////////////////////////////////////////////////////////////////////
// Include List
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

#include "drvmvd_cc.h"
////////////////////////////////////////////////////////////////////////////////
// Constant & Macro Definition
////////////////////////////////////////////////////////////////////////////////
/// Version string.
#define MVD_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'M','V','D','_'},                  /* IP__                                             */  \
    {'0','A'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','1'},                          /* 00 ~ 99                                          */  \
    {'0','0','6','4','1','6','6','7'},  /* CL#                                              */  \
    MSIF_OS

#define MIU_SEL_0         0
#define MIU_SEL_1         1
#define MIU_SEL_2         2
#define MIU_SEL_3         3


#define MST_MODE_TRUE     1
#define MST_MODE_FALSE    0

////////////////////////////////////////////////////////////////////////////////
// Type & Structure Declaration
////////////////////////////////////////////////////////////////////////////////
/// MVD's capability
typedef struct _MVD_Caps
{
    MS_BOOL bMPEG2;
    MS_BOOL bMPEG4;
    MS_BOOL bVC1;
} MVD_Caps;

/// MVD driver info.
typedef struct _MVD_DrvInfo
{
    MS_U32 u32Resource;
    MS_U32 u32DeviceNum;
    MS_U32 u32FWVersion;
    MVD_Caps stCaps;
} MVD_DrvInfo;

/// Firmware status
typedef enum _MVD_DecStat
{
    E_MVD_STAT_IDLE             = 0x00,
    E_MVD_STAT_FIND_STARTCODE   = 0x01,//start code
    E_MVD_STAT_FIND_SPECIALCODE = 0x11,//special code [00_00_01_C5_ab_08_06_27]
                                       //for flush data using
    E_MVD_STAT_FIND_FRAMEBUFFER = 0x02,
    //fw is trying to find empty FB to continue decoding.
    //if hang in this state, please check "vsync" or AVSync.
    E_MVD_STAT_WAIT_DECODEDONE  = 0x03,
    E_MVD_STAT_DECODE_DONE      = 0x04,
    E_MVD_STAT_WAIT_VDFIFO      = 0x05,
    E_MVD_STAT_INIT_SUCCESS     = 0x06,
    E_MVD_STAT_UNKNOWN          = 0xff,
} MVD_DecStat;

/// MVD driver status
typedef struct _MVD_DrvStatus
{
//    MS_U32      u32FWVer;
    MVD_DecStat eDecStat;
//    MS_BOOL     bIsBusy;
    MS_U8       u8LastFWCmd;
} MVD_DrvStatus;

/// MVD stream types for dual decoders
typedef enum
{
    E_MVD_DRV_STREAM_NONE = 0,
    E_MVD_DRV_MAIN_STREAM,
    E_MVD_DRV_SUB_STREAM,
    E_MVD_DRV_STREAM_MAX = E_MVD_DRV_SUB_STREAM,
} MVD_DRV_StreamType;

/// Enumerate CodecType that MVD supports
typedef enum _MVD_CodecType
{
    E_MVD_CODEC_MPEG2                    = 0x10,
    E_MVD_CODEC_MPEG4                    = 0x00,
    E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER = 0x01,
    E_MVD_CODEC_DIVX311                  = 0x02,
    E_MVD_CODEC_FLV                      = 0x03,
    E_MVD_CODEC_VC1_ADV                  = 0x04,
    E_MVD_CODEC_VC1_MAIN                 = 0x05,
    E_MVD_CODEC_UNKNOWN                  = 0xff
} MVD_CodecType;

/// Mode of MVD input source
typedef enum _MVD_SrcMode
{
    E_MVD_FILE_MODE       = 0x00,
    E_MVD_SLQ_MODE        = 0x01,
    E_MVD_TS_MODE         = 0x02,
    E_MVD_SLQ_TBL_MODE    = 0x03,
    E_MVD_TS_FILE_MODE    = 0x04,
    E_MVD_SRC_UNKNOWN     = 0x05
} MVD_SrcMode;

/// AVSync mode for file playback
typedef enum
{
    E_MVD_TIMESTAMP_FREERUN  = 0,  ///Player didn't set PTS/DTS, so display freerun
    E_MVD_TIMESTAMP_PTS      = 1,  ///Player set PTS to MVD decoder
    E_MVD_TIMESTAMP_DTS      = 2,  ///Player set DTS to MVD decoder
    E_MVD_TIMESTAMP_STS      = 3,  ///SortedTimeStamp means decoder should sort timestamp
    E_MVD_TIMESTAMP_PTS_RVU  = 4,  ///Player set PTS to MVD decoder
    E_MVD_TIMESTAMP_DTS_RVU  = 5,  ///Player set DTS to MVD decoder
    E_MVD_TIMESTAMP_NEW_STS  = 6,  ///Player set PTS to MVD decoder
} MVD_TIMESTAMP_TYPE;

/// ErrCode obtained from firmware
typedef enum _MVD_ErrCode
{
    E_MVD_ERR_UNKNOWN            = 0,
    E_MVD_ERR_SHAPE              = 1,
    E_MVD_ERR_USED_SPRITE        = 2,
    E_MVD_ERR_NOT_8_BIT          = 3,   //error_status : bits per pixel
    E_MVD_ERR_NERPRED_ENABLE     = 4,
    E_MVD_ERR_REDUCED_RES_ENABLE = 5,
    E_MVD_ERR_SCALABILITY        = 6,
    E_MVD_ERR_OTHER              = 7,
    E_MVD_ERR_H263_ERROR         = 8,
    E_MVD_ERR_RES_NOT_SUPPORT    = 9,   //error_status : none
    E_MVD_ERR_MPEG4_NOT_SUPPORT  = 10,  //error_status : none
    E_MVD_ERR_PROFILE_NOT_SUPPORT= 11,   //error_status : none
    E_MVD_ERR_RCV_ERROR_OCCUR= 12   //error_status : none
} MVD_ErrCode;

/// Detailed error status when error occurs
typedef enum _MVD_ErrStatus
{
    //error_status for E_MVD_ERR_SHAPE
    E_MVD_ERR_SHAPE_RECTANGULAR    = 0x10,
    E_MVD_ERR_SHAPE_BINARY         = 0x11,
    E_MVD_ERR_SHAPE_BINARY_ONLY    = 0x12,
    E_MVD_ERR_SHAPE_GRAYSCALE      = 0x13,

    //error_status for E_MVD_ERR_USED_SPRITE
    E_MVD_ERR_USED_SPRITE_UNUSED   = 0x20, //sprite not used
    E_MVD_ERR_USED_SPRITE_STATIC   = 0x21,
    E_MVD_ERR_USED_SPRITE_GMC      = 0x22,
    E_MVD_ERR_USED_SPRITE_RESERVED = 0x23,

    E_MVD_ERR_STATUS_NONE          = 0x77,
    E_MVD_ERR_STATUS_UNKOWN        = 0x00,
} MVD_ErrStatus;

/// Picture type of MPEG video
typedef enum _MVD_PicType
{
    E_MVD_PIC_I = 0,
    E_MVD_PIC_P = 1,
    E_MVD_PIC_B = 2,
    E_MVD_PIC_UNKNOWN = 0xf,
} MVD_PicType;

/// Mode of frame rate conversion
typedef enum _MVD_FrcMode {
    E_MVD_FRC_NORMAL = 0,
    E_MVD_FRC_DISP_TWICE = 1,
    E_MVD_FRC_3_2_PULLDOWN = 2,
    E_MVD_FRC_PAL_TO_NTSC = 3,
    E_MVD_FRC_NTSC_TO_PAL = 4,
    E_MVD_FRC_DISP_ONEFIELD = 5,//Removed, call MDrv_MVD_EnableDispOneField() instead.
} MVD_FrcMode;

/// MVD play mode
typedef enum
{
    E_MVD_PLAY          = 0x00,
    E_MVD_STEPDISP      = 0x01,
    E_MVD_PAUSE         = 0x02,
    E_MVD_FASTFORWARD   = 0x03,
    E_MVD_BACKFORWARD   = 0x04,
    E_MVD_UNKNOWMODE    = 0xff
} MVD_PlayMode;

/// Store the packet information used for SLQ table file mode
typedef struct
{
    MS_VIRT u32StAddr;    ///< Packet offset from bitstream buffer base address. unit: byte.
    MS_U32 u32Length;    ///< Packet size. unit: byte.
    MS_U32 u32TimeStamp; ///< Packet time stamp. unit: ms.
    MS_U32 u32ID_L;      ///< Packet ID low part.
    MS_U32 u32ID_H;      ///< Packet ID high part.
} MVD_PacketInfo;

///MVD frame info data structure
typedef struct
{
    MS_U16  u16HorSize;
    MS_U16  u16VerSize;
    MS_U32  u32FrameRate;
    MS_U8   u8AspectRate;
    MS_U8   u8Interlace;
    MS_U8   u8AFD;
    MS_U16  u16par_width;
    MS_U16  u16par_height;
    MS_U16  u16SarWidth;
    MS_U16  u16SarHeight;
    MS_U16  u16CropRight;
    MS_U16  u16CropLeft;
    MS_U16  u16CropBottom;
    MS_U16  u16CropTop;
    MS_U16  u16Pitch;
    MS_U16  u16PTSInterval;
    MS_U8   u8MPEG1;
    MS_U8   u8PlayMode;
    MS_U8   u8FrcMode;
    MS_VIRT  u32DynScalingAddr;    ///Dynamic scaling address
    MS_U8   u8DynScalingDepth;    ///Dynamic scaling depth
    MS_U32  u32DynScalingBufSize;    ///Dynamic scaling BufSize
    MS_BOOL bEnableMIUSel;        ///Dynamic scaling DS buffer on miu1 or miu0
} MVD_FrameInfo;

/// MVD AVSync Configuration
typedef struct
{
    MS_U32  u32Delay;       //unit: ms
    MS_U16  u16Tolerance;   //unit: ms
    MS_BOOL bEnable;
    MS_U8   u8Rsvd;         //reserved
} MVD_AVSyncCfg;

/// MVD Command arguments
typedef struct
{
    MS_U8 Arg0;    ///< argument 0
    MS_U8 Arg1;    ///< argument 1
    MS_U8 Arg2;    ///< argument 2
    MS_U8 Arg3;    ///< argument 3
    MS_U8 Arg4;    ///< argument 4
    MS_U8 Arg5;    ///< argument 5
} MVD_CmdArg;

/// MVD commands needing handshake
typedef enum
{
    MVD_HANDSHAKE_PAUSE,
    MVD_HANDSHAKE_SLQ_RST,
    MVD_HANDSHAKE_STOP,
    MVD_HANDSHAKE_SKIP_DATA,
    MVD_HANDSHAKE_SKIP_TO_PTS,
    MVD_HANDSHAKE_SINGLE_STEP,
    MVD_HANDSHAKE_SCALER_INFO,
    MVD_HANDSHAKE_GET_NXTDISPFRM,
    MVD_HANDSHAKE_PARSER_RST,
    MVD_HANDSHAKE_RST_CC608,
    MVD_HANDSHAKE_RST_CC708
} MVD_HANDSHAKE_CMD;



typedef enum
{
    E_MVD_FB_REDUCTION_NONE = 0,        ///< FB reduction disable
    E_MVD_FB_REDUCTION_1_2 = 1,         ///< FB reduction 1/2
    E_MVD_FB_REDUCTION_1_4 = 2,         ///< FB reduction 1/4
} MVD_FB_Reduction_Type;

typedef struct
{
    MVD_FB_Reduction_Type LumaFBReductionMode;     ///< Luma frame buffer reduction mode.
    MVD_FB_Reduction_Type ChromaFBReductionMode;   ///< Chroma frame buffer reduction mode.
    MS_U8                 u8EnableAutoMode;        /// 0: Disable, 1: Enable
    MS_U8                 u8ReservedByte;          /// Reserved byte
} MVD_FB_Reduction;

/// Configuration of MVD firmware
typedef struct
{
    MVD_CodecType    eCodecType;        ///< Specify the active codec type
    MVD_SrcMode      eSrcMode;          ///< Specify the input source
    MS_U8            bDisablePESParsing;///< MVD parser enable/disable
    MS_BOOL          bNotReload;        ///< TRUE to not allow loading f/w after the 1st init to speed up.
    MVD_FB_Reduction stFBReduction;     ///< MVD Frame buffer reduction type
    MS_U16           u16FBReduceValue;
    MS_U8            u8FBMode;          ///driver internal attribute: SD or HD
    MS_U8            u8FBNum;           ///driver internal attribute: # of frames in FB
    MS_VIRT           u32FBUsedSize;     ///driver internal attribute: used size
} MVD_FWCfg;

/// The type of fw binary input source
typedef enum
{
    E_MVD_FW_SOURCE_NONE,       ///< No input fw.
    E_MVD_FW_SOURCE_DRAM,       ///< input source from DRAM.
    E_MVD_FW_SOURCE_FLASH,      ///< input source from FLASH.
} MVD_FWSrcType;

/// Configuration of memory layout
typedef struct
{
    MVD_FWSrcType eFWSrcType;         //!< the input FW source type.
    MS_VIRT     u32FWSrcVAddr;         //!< virtual address of input FW binary in DRAM
    MS_PHY u32FWBinAddr;          //!< physical address in Flash/DRAM of FW code
    MS_VIRT     u32FWBinSize;
    MS_PHY u32FWCodeAddr;
    MS_VIRT     u32FWCodeSize;
    MS_PHY u32FBAddr;
    MS_VIRT     u32FBSize;
    MS_PHY u32BSAddr;
    MS_VIRT     u32BSSize;
    MS_PHY u32DrvBufAddr;
    MS_VIRT     u32DrvBufSize;
    MS_PHY u32DynSacalingBufAddr;
    MS_VIRT     u32DynSacalingBufSize;
    MS_PHY u32Miu1BaseAddr;
    MS_PHY u32Miu2BaseAddr;
    MS_PHY u32Miu3BaseAddr;
    MS_BOOL    bFWMiuSel;
    MS_BOOL    bHWMiuSel;
    MS_U8      u8FWMiuSel;
    MS_U8      u8HWMiuSel;
    MS_BOOL    bEnableDynScale;       /// dynamic scaling control bit
    MS_BOOL    bSupportSDModeOnly;    /// Config from IP Check
} MVD_MEMCfg;

/// Return value of MVD driver
typedef enum
{
    E_MVD_RET_OK            = 1,
    E_MVD_RET_FAIL          = 0,
    E_MVD_RET_INVALID_PARAM = 2,
    E_MVD_RET_QUEUE_FULL    = 3,
    E_MVD_RET_TIME_OUT      = 4
} E_MVD_Result;

/// Mode of trick decoding
typedef enum
{
    E_MVD_TRICK_DEC_ALL = 0,
    E_MVD_TRICK_DEC_IP,
    E_MVD_TRICK_DEC_I,
    E_MVD_TRICK_DEC_UNKNOWN
} MVD_TrickDec;

/// Speed type of playing
typedef enum
{
    E_MVD_SPEED_DEFAULT = 0,
    E_MVD_SPEED_FAST,
    E_MVD_SPEED_SLOW,
    E_MVD_SPEED_TYPE_UNKNOWN
} MVD_SpeedType;

/// MVD pattern type
typedef enum
{
    E_MVD_PATTERN_FLUSH = 0,           ///< Used after MDrv_MVD_Flush().
    E_MVD_PATTERN_FILEEND,             ///< Used for file end.
} MVD_PatternType;

/// MVD frame info structure
typedef struct
{
    MS_PHY u32LumaAddr;    ///< The start physical of luma data. Unit: byte.
    MS_PHY u32ChromaAddr;  ///< The start physcal of chroma data. Unit: byte.
    MS_U32 u32TimeStamp;       ///< Time stamp(DTS, PTS) of current displayed frame. Unit: 90khz.
    MS_U32 u32ID_L;            ///< low part of ID number set by MDrv_MVD_PushQueue().
    MS_U32 u32ID_H;            ///< high part of ID number set by MDrv_MVD_PushQueue().
    MS_U16 u16Pitch;           ///< The pitch of current frame.
    MS_U16 u16Width;           ///< pixel width of current frame.
    MS_U16 u16Height;          ///< pixel height of current frame.
    MS_U16 u16FrmIdx;          ///< index of current frame.
    MVD_PicType eFrmType;     ///< picture type: I, P, B frame
} MVD_FrmInfo;

/// MVD extension display info structure
typedef struct
{
    MS_U16 u16VSize;         /// vertical size from sequene_display_extension
    MS_U16 u16HSize;         /// horizontal size from sequene_display_extension
    MS_U16 u16VOffset;       /// vertical offset from picture_display_extension
    MS_U16 u16HOffset;       /// horizontal offset from picture_display_extension
} MVD_ExtDispInfo;

/// Type of frame info that can be queried
typedef enum
{
    E_MVD_FRMINFO_DISPLAY=0,   ///< Displayed frame.
    E_MVD_FRMINFO_DECODE =1,   ///< Decoded frame.
    E_MVD_FRMINFO_NEXT_DISPLAY =2,///< Next frame to be displayed.
} MVD_FrmInfoType;

typedef enum {
    E_MVD_FRAME_FLIP    = 0,
    E_MVD_FRAME_RELEASE = 1,
} MVD_FrmOpt;

///MVD set debug mode
typedef enum
{
    E_MVD_EX_DBG_MODE_ORI = 0,
    E_MVD_EX_DBG_MODE_BYPASS_INSERT_START_CODE , 	/// for UT
    E_MVD_EX_DBG_MODE_BYPASS_DIVX_MC_PATCH,			/// for UT
    E_MVD_EX_DBG_MODE_NUM
} MVD_DbgMode;

typedef struct
{
	union
	{
		struct
		{
			MS_U32 bBypassInsertStartCode : 1;  // TRUE: for bypass insert start code for UT...
			MS_U32 bBypassDivxMCPatch : 1;      // TRUE: for bypass divx MC patch for UT...
		};
		MS_U32 value;
	};
}MVD_DbgModeCfg;

/// Attributes used by drvMVD & halMVD
/// Updated by halMVD, read-only for drvMVD
typedef struct
{
    MS_BOOL         bAVSyncOn;
    MS_BOOL         bDropErrFrm;
    MS_BOOL         bDropDispfrm;
    MS_BOOL         bDecodeIFrame;
    MS_BOOL         bStepDecode;
    MS_BOOL         bStepDisp;
    MS_BOOL         bStep2Pts;
    MS_BOOL         bSkip2Pts;
    MS_BOOL         bEnableLastFrmShow;
    MS_BOOL         bSlqTblSync;
    MS_U8           u8MstMode; //MStreamer mode=1; Non-MSt mode=0.
    MS_U8           u8McuMode; //MCU mode=1;
    MS_U8           u8DynScalingDepth;
    MVD_TrickDec    eTrickMode;
    MVD_FrcMode     eFrcMode;
    MVD_TIMESTAMP_TYPE eFileSyncMode;
    MVD_SpeedType   ePreSpeedType;
    MS_VIRT          u32UsrDataRd;
    MS_U32          u32IntCnt;      /// interrupt ocurred counter
    MVD_AVSyncCfg   stSyncCfg;
    MVD_DbgModeCfg  stDbgModeCfg;
    MS_BOOL         bExternalDSBuf;
} MVD_CtrlCfg;

/// MVD time code structure
typedef struct
{
    MS_U8   u8TimeCodeHr;   ///<  time_code_hours
    MS_U8   u8TimeCodeMin;  ///<  time_code_minutes
    MS_U8   u8TimeCodeSec;  ///<  time_code_seconds
    MS_U8   u8TimeCodePic;  ///<  time_code_pictures

    MS_U8   u8DropFrmFlag;  ///<  drop_frame_flag
    MS_U8   u8Reserved[3];  ///<  reserved fields for 4-byte alignment
} MVD_TimeCode;

/// Format of CC (Closed Caption)
typedef enum _MVD_CCFormat
{
    E_MVD_CC_NONE       = 0x00,
    E_MVD_CC_608        = 0x01, //For CC608 or 157
    E_MVD_CC_708        = 0x02, //For CC708
    E_MVD_CC_UNPACKED   = 0x03,
} MVD_CCFormat;

/// Type of CC
typedef enum _MVD_CCType
{
    E_MVD_CC_TYPE_NONE = 0,
    E_MVD_CC_TYPE_NTSC_FIELD1 = 1,
    E_MVD_CC_TYPE_NTSC_FIELD2 = 2,
    E_MVD_CC_TYPE_DTVCC = 3,
    E_MVD_CC_TYPE_NTSC_TWOFIELD = 4,
} MVD_CCType;

/// Data structure of CC Configuration
typedef struct
{
    MVD_CCFormat eFormat;
    MVD_CCType   eType;
    MS_VIRT       u32BufStAdd;
    MS_U32       u32BufSize;
} MVD_CCCfg;

/// Info. of user data
typedef struct
{
    MS_U32 u32Pts;
    MS_U8  u8PicStruct;           /* picture_structure*/
    MS_U8  u8PicType;             /* picture type: 1->I picture, 2->P,3->B */
    MS_U8  u8TopFieldFirst;       /* Top field first: 1 if top field first*/
    MS_U8  u8RptFirstField;       /* Repeat first field: 1 if repeat field first*/

    MS_U16 u16TmpRef;            /* Temporal reference of the picture*/
    MS_U8  u8ByteCnt;
    MS_U8  u8Reserve;

    MS_U32 u32DataBuf;
} MVD_UsrDataInfo;

/// MVD interrupt events
typedef enum
{
    E_MVD_EVENT_DISABLE_ALL    = 0,           ///< unregister all events notification
    E_MVD_EVENT_USER_DATA      = BIT(0),      ///< found user data
    E_MVD_EVENT_DISP_VSYNC     = BIT(1),      ///< vsync interrupt
    E_MVD_EVENT_PIC_FOUND      = BIT(2),      ///<
    E_MVD_EVENT_FIRST_FRAME    = BIT(3),      ///< first frame decoded
    E_MVD_EVENT_DISP_RDY       = BIT(4),      ///< MVD ready to display.
    E_MVD_EVENT_SEQ_FOUND      = BIT(5),      ///< found sequence header
    //E_MVD_EVENT_DEC_CC_FOUND = BIT(6),      ///< MVD found one user data with decoded frame.
    E_MVD_EVENT_DEC_DATA_ERR   = BIT(7),      ///< ES Data error.
    E_MVD_EVENT_UNMUTE         = BIT(8),      ///< video unmute
    E_MVD_EVENT_USER_DATA_DISP = BIT(9),      ///< found user data in display order
    E_MVD_EVENT_DEC_ERR        = BIT(10),     ///< MVD HW found decode error.
    E_MVD_EVENT_DEC_I          = BIT(11),     ///< MVD HW decode I frame.
    E_MVD_EVENT_DEC_ONE_FRAME  = BIT(15),     ///< Decode one frame done
    E_MVD_EVENT_XC_LOW_DEALY   = BIT(16),     ///< speed up the un-mute screen on XC.
} MVD_Event;

/// MVD clock speed
typedef enum
{
    E_MVD_EX_CLOCK_SPEED_NONE = 0,
    E_MVD_EX_CLOCK_SPEED_HIGHEST,
    E_MVD_EX_CLOCK_SPEED_HIGH,
    E_MVD_EX_CLOCK_SPEED_MEDIUM,
    E_MVD_EX_CLOCK_SPEED_LOW,
    E_MVD_EX_CLOCK_SPEED_LOWEST,
    E_MVD_EX_CLOCK_SPEED_DEFAULT,
} MVD_EX_ClockSpeed;

//MVD CRC value
typedef struct
{
    MS_U32 u32HSize;
    MS_U32 u32VSize;
    MS_U32 u32Strip;
    MS_VIRT u32YStartAddr;
    MS_VIRT u32UVStartAddr;
}MVD_CrcIn;

typedef struct
{
    MS_U32 u32YCrc;
    MS_U32 u32UVCrc;
}MVD_CrcOut;

typedef struct
{
    MS_VIRT  u32DSBufAddr;       // Buffer Address
    MS_U32  u32DSBufSize;       // Buffer Size
}MVD_EX_ExternalDSBuf;


typedef struct
{
    MS_BOOL bHWBufferReMapping;
} MVD_Pre_Ctrl;


typedef void (*MVD_InterruptCb)(MS_U32 u32CbData);

////////////////////////////////////////////////////////////////////////////////
// Function Prototype Declaration
////////////////////////////////////////////////////////////////////////////////
E_MVD_Result MDrv_MVD_SetCfg(MS_U32 u32Id, MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg);
MS_U32 MDrv_MVD_GetFWVer(MS_U32 u32Id);
void MDrv_MVD_SetDbgLevel(MS_U8 level);
const MVD_DrvInfo* MDrv_MVD_GetInfo(void);
E_MVD_Result MDrv_MVD_GetLibVer(const MSIF_Version **ppVersion);

void MDrv_MVD_SetCodecInfo(MS_U32 u32Id, MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing);
void MDrv_MVD_SetDivXCfg(MS_U32 u32Id, MS_U8 u8MvAdjust, MS_U8 u8IdctSel);

void MDrv_MVD_SetFrameBuffAddr(MS_U32 u32Id, MS_VIRT u32addr);
void MDrv_MVD_GetFrameInfo(MS_U32 u32Id, MVD_FrameInfo *pinfo );
void MDrv_MVD_SetOverflowTH(MS_U32 u32Id, MS_U32 u32Threshold);
void MDrv_MVD_SetUnderflowTH(MS_U32 u32Id, MS_U32 u32Threshold);

void MDrv_MVD_RstIFrameDec(MS_U32 u32Id,MVD_CodecType eCodecType);
MS_BOOL MDrv_MVD_GetIsIFrameDecoding(MS_U32 u32Id);

MS_U8 MDrv_MVD_GetSyncStatus(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetIsFreerun(MS_U32 u32Id);
MS_U8 MDrv_MVD_GetIsIPicFound(MS_U32 u32Id);

MS_BOOL MDrv_MVD_DecodeIFrame(MS_U32 u32Id, MS_PHY u32FrameBufAddr, MS_PHY u32StreamBufAddr, MS_PHY u32StreamBufEndAddr );
MS_BOOL MDrv_MVD_GetValidStreamFlag(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetChromaFormat(MS_U32 u32Id);
//for MM
void MDrv_MVD_SetFrameInfo(MS_U32 u32Id, MVD_FrameInfo *pinfo );
void MDrv_MVD_GetErrInfo(MS_U32 u32Id, MVD_ErrCode *errCode, MVD_ErrStatus *errStatus);
MS_U32 MDrv_MVD_GetSkipPicCounter(MS_U32 u32Id);

void MDrv_MVD_SetSLQWritePtr(MS_U32 u32Id, MS_BOOL bCheckData);
MS_U32 MDrv_MVD_GetSLQReadPtr(MS_U32 u32Id);

MVD_PicType MDrv_MVD_GetPicType(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetBitsRate(MS_U32 u32Id);
MS_U8 MDrv_MVD_GetVideoRange(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetLowDelayFlag(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetIs32PullDown(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetIsDynScalingEnabled(MS_U32 u32Id);
MS_S32 MDrv_MVD_GetPtsStcDiff(MS_U32 u32Id);

void MDrv_MVD_Pause(MS_U32 u32Id);
void MDrv_MVD_Resume(MS_U32 u32Id);
MS_BOOL MDrv_MVD_StepDisp(MS_U32 u32Id);
MS_BOOL MDrv_MVD_IsStepDispDone(MS_U32 u32Id);
MS_BOOL MDrv_MVD_StepDecode(MS_U32 u32Id);
MS_BOOL MDrv_MVD_IsStepDecodeDone(MS_U32 u32Id);
MS_BOOL MDrv_MVD_SeekToPTS(MS_U32 u32Id, MS_U32 u32Pts);
MS_BOOL MDrv_MVD_IsStep2PtsDone(MS_U32 u32Id);
MS_BOOL MDrv_MVD_SkipToPTS(MS_U32 u32Id, MS_U32 u32Pts);
MS_BOOL MDrv_MVD_TrickPlay(MS_U32 u32Id, MVD_TrickDec trickDec, MS_U8 u8DispDuration);
void MDrv_MVD_EnableForcePlay(MS_U32 u32Id);

void MDrv_MVD_RegSetBase(MS_VIRT u32RegBaseAddr);
E_MVD_Result MDrv_MVD_GetFreeStream(MS_U32 *pu32Id, MVD_DRV_StreamType eStreamType);
MS_BOOL MDrv_MVD_Init(MS_U32 u32Id,MVD_CodecType eCodecType);
MS_BOOL MDrv_MVD_Exit(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_Rst(MS_U32 u32Id,MVD_CodecType eCodecType);

void MDrv_MVD_Play(MS_U32 u32Id);
void MDrv_MVD_SetAVSync(MS_U32 u32Id, MS_BOOL bEnable, MS_U32 u32Delay);
void MDrv_MVD_SetAVSyncThreshold(MS_U32 u32Id, MS_U32 u32Th);
void MDrv_MVD_SetAVSyncFreerunThreshold(MS_U32 u32Id, MS_U32 u32Th);
MS_U32 MDrv_MVD_GetAVSyncDelay(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetIsAVSyncOn(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetIsSyncRep(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetIsSyncSkip(MS_U32 u32Id);
MS_BOOL MDrv_MVD_ChangeAVsync(MS_U32 u32Id, MS_BOOL bEnable, MS_U16 u16PTS);
MS_BOOL MDrv_MVD_DispCtrl(MS_U32 u32Id, MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode);
MS_BOOL MDrv_MVD_DispRepeatField(MS_U32 u32Id, MS_BOOL bEnable);
MS_U8 MDrv_MVD_GetColorFormat(MS_U32 u32Id);
MS_U8 MDrv_MVD_GetMatrixCoef(MS_U32 u32Id);
MS_U8 MDrv_MVD_GetActiveFormat(MS_U32 u32Id);
MS_U8 MDrv_MVD_GetDispRdy(MS_U32 u32Id);
MS_BOOL MDrv_MVD_Is1stFrmRdy(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetGOPCount(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetPicCounter(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetParserByteCnt(MS_U32 u32Id);
MVD_DecStat MDrv_MVD_GetDecodeStatus(MS_U32 u32Id);
MS_U8 MDrv_MVD_GetLastCmd(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetVldErrCount(MS_U32 u32Id);
MS_BOOL MDrv_MVD_DropErrorFrame(MS_U32 u32Id, MS_BOOL bDrop);
MS_BOOL MDrv_MVD_MVDCommand ( MS_U8 u8cmd, MVD_CmdArg *pstCmdArg );
MS_BOOL MDrv_MVD_SkipData(MS_U32 u32Id);
MS_BOOL MDrv_MVD_SkipToIFrame(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetCaps(MVD_Caps* pCaps);
MS_BOOL MDrv_MVD_GetFrmRateIsSupported(MS_U32 u32Id);

E_MVD_Result MDrv_MVD_DisableErrConceal(MS_U32 u32Id, MS_BOOL bDisable);
E_MVD_Result MDrv_MVD_PushQueue(MS_U32 u32Id, MVD_PacketInfo* pInfo);
E_MVD_Result MDrv_MVD_FlushQueue(MS_U32 u32Id);
MS_BOOL MDrv_MVD_FlushDisplayBuf(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetQueueVacancy(MS_U32 u32Id, MS_BOOL bCached);
MS_U32 MDrv_MVD_GetESReadPtr(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetESWritePtr(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_EnableLastFrameShow(MS_U32 u32Id, MS_BOOL bEnable);
E_MVD_Result MDrv_MVD_IsDispFinish(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_SetSpeed(MS_U32 u32Id, MVD_SpeedType eSpeedType, MS_U8 u8Multiple);
E_MVD_Result MDrv_MVD_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase);
MS_U32 MDrv_MVD_GetPTS(MS_U32 u32Id);
MS_U64 MDrv_MVD_GetU64PTS(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetNextPTS(MS_U32 u32Id);
MVD_TrickDec MDrv_MVD_GetTrickMode(MS_U32 u32Id);
MS_BOOL MDrv_MVD_IsPlaying(MS_U32 u32Id);
MS_BOOL MDrv_MVD_IsIdle(MS_U32 u32Id);
MS_BOOL MDrv_MVD_IsSeqChg(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_DbgSetData(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32 u32Data);
E_MVD_Result MDrv_MVD_DbgGetData(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32* u32Data);
MS_U8 MDrv_MVD_GetDecodedFrameIdx (MS_U32 u32Id);
MS_BOOL MDrv_MVD_SetFileModeAVSync(MS_U32 u32Id, MVD_TIMESTAMP_TYPE eSyncMode);
MS_BOOL MDrv_MVD_IsAllBufferEmpty(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GenPattern(MS_U32 u32Id, MVD_PatternType ePattern, MS_PHY u32PAddr, MS_U32* pu32Size);
MS_U32 MDrv_MVD_GetPatternInfo(void);
E_MVD_Result MDrv_MVD_SetDynScalingParam(MS_U32 u32Id, MS_PHY u32StAddr, MS_VIRT u32Size);
MS_BOOL MDrv_MVD_SetVirtualBox(MS_U32 u32Id, MS_U16 u16Width, MS_U16 u16Height);
MS_BOOL MDrv_MVD_SetBlueScreen(MS_U32 u32Id, MS_BOOL bEn);
MS_BOOL MDrv_MVD_EnableInt(MS_U32 u32Id, MS_U32 bEn);
E_MVD_Result MDrv_MVD_EnableDispOneField(MS_U32 u32Id, MS_BOOL bEn);
E_MVD_Result MDrv_MVD_GetExtDispInfo(MS_U32 u32Id, MVD_ExtDispInfo* pInfo);
E_MVD_Result MDrv_MVD_GetFrmInfo(MS_U32 u32Id, MVD_FrmInfoType eType, MVD_FrmInfo* pInfo);
E_MVD_Result MDrv_MVD_GetTimeCode(MS_U32 u32Id, MVD_FrmInfoType eType, MVD_TimeCode* pInfo);
MS_BOOL MDrv_MVD_GetUsrDataIsAvailable(MS_U32 u32Id);
MS_BOOL MDrv_MVD_GetUsrDataInfo(MS_U32 u32Id, MVD_UsrDataInfo* pUsrInfo);
E_MVD_Result MDrv_MVD_SetFreezeDisp(MS_U32 u32Id, MS_BOOL bEn);
MS_U32 MDrv_MVD_GetDispCount(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_GetXcLowDelayIntState(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_SetFdMaskDelayCount(MS_U32 u32Id, MS_U16 u16Cnt);
E_MVD_Result MDrv_MVD_SetOutputFRCMode(MS_U32 u32Id, MS_U8 u8FrameRate, MS_U8 u8Interlace);
E_MVD_Result MDrv_MVD_SetFRCDropType(MS_U32 u32Id, MS_U8 u8DropType);
E_MVD_Result MDrv_MVD_SetDisableSeqChange(MS_U32 u32Id, MS_BOOL bEnable);

MS_BOOL MDrv_MVD_SetMStreamerMode(MS_U32 u32Id, MS_U8 u8Mode);
MS_BOOL MDrv_MVD_SetMcuMode(MS_U32 u32Id, MS_U8 u8Mode);
MS_BOOL MDrv_MVD_FrameFlip(MS_U32 u32Id, MS_U8 u8FrmIdx);
MS_BOOL MDrv_MVD_FrameRelease(MS_U32 u32Id, MS_U8 u8FrmIdx);
MS_BOOL MDrv_MVD_FrameCapture(MS_U32 u32Id, MS_U8 u8FrmIdx, MS_BOOL bEnable);
MS_BOOL MDrv_MVD_ReleaseFdMask(MS_U32 u32Id, MS_BOOL bRls);
MS_BOOL MDrv_MVD_ParserRstDone(MS_U32 u32Id, MS_BOOL bEnable);
MS_BOOL MDrv_MVD_SetVSizeAlign(MS_U32 u32Id, MS_BOOL bEnable);
E_MVD_Result MDrv_MVD_SetAutoMute(MS_U32 u32Id, MS_BOOL bEnable);

MS_BOOL MDrv_MVD_SetSkipRepeatMode(MS_U32 u32Id, MS_U8 u8Mode);
MS_BOOL MDrv_MVD_SetSingleDecodeMode(MS_BOOL bEnable);
MS_BOOL MDrv_MVD_FlushPTSBuf(MS_U32 u32Id,MS_BOOL bEnable);
MS_BOOL MDrv_MVD_ShowDecodeOrder(MS_U32 u32Id, MS_U8 u8Mode);
E_MVD_Result MDrv_MVD_GetCrcValue(MS_U32 u32Id, MVD_CrcIn *pCrcIn, MVD_CrcOut *pCrcOut);
E_MVD_Result MDrv_MVD_SetDbgMode(MS_U32 u32Id, MVD_DbgMode enDbgMode, MS_BOOL bEn);
MS_BOOL MDrv_MVD_SuspendDynamicScale(MS_U32 u32Id, MS_BOOL bEnable);
MS_U8 MDrv_MVD_GetSuspendDynamicScale(MS_U32 u32Id);
void MDrv_MVD_DbgDump(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetDivxVer(MS_U32 u32Id);
MS_BOOL MDrv_MVD_SetIdctMode(MS_U32 u32Id, MS_U8 u8Mode);
E_MVD_Result MDrv_MVD_Init_Share_Mem(void);
MS_BOOL MDrv_MVD_PreSetCodecType(MS_U32 u32Id,MVD_CodecType eMvdCodecType ,MS_BOOL *bHDMode);
E_MVD_Result MDrv_MVD_SetXCLowDelayPara(MS_U32 u32Id,MS_U32 u32Para);
void MDrv_MVD_Drop_One_PTS(MS_U32 u32Id);

#define MVD_ENABLE_ISR

#ifdef MVD_ENABLE_ISR
E_MVD_Result MDrv_MVD_SetIsrEvent(MS_U32 u32Id, MS_U32 eEvent, MVD_InterruptCb fnHandler);
MS_U32 MDrv_MVD_GetIsrEvent(MS_U32 u32Id);
#else
#define MDrv_MVD_SetIsrEvent(x, y, z) (E_MVD_RET_OK)
#define MDrv_MVD_GetIsrEvent(x)     (MS_U32)0
#endif
MS_U32 MDrv_MVD_GetSLQNum(MS_U32 u32Id);
MS_U32 MDrv_MVD_GetDispQNum(MS_U32 u32Id);

E_MVD_Result MDrv_MVD_EX_SetMVDClockSpeed(MVD_EX_ClockSpeed eClockSpeed);
MS_BOOL MDrv_MVD_ShowFirstFrameDirect(MS_U32 u32Id,MS_BOOL bEnable);
E_MVD_Result MDrv_MVD_SetSelfSeqChange(MS_U32 u32Id, MS_BOOL bEnable);
MS_U8 MDrv_MVD_GetESBufferStatus(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_SetExternalDSBuffer(MS_U32 u32Id, MVD_EX_ExternalDSBuf *pExternalBuf);
MS_BOOL MDrv_MVD_Field_Polarity_Display_One_field(MS_U32 u32Id, MS_BOOL bEnable,MS_U8 top_bottom);

MS_BOOL MDrv_MVD_SetShareMemoryBase(MS_U32 u32Id, MS_U32 u32base);
MS_U32 MDrv_MVD_GetShareMemoryOffset(MS_U32 u32Id, MS_U32 *u32base);
MS_BOOL MDrv_MVD_EnableVPUSecurityMode(MS_BOOL enable);
MS_BOOL MDrv_MVD_GetSupport2ndMVOPInterface(void);
MS_BOOL MDrv_MVD_REE_RegisterMBX(void);
MS_BOOL MDrv_MVD_REE_SetSHMBaseAddr(void);
MS_U32 MDrv_MVD_GetVsyncBridgeAddr(MS_U32 u32Id);


MS_BOOL MDrv_MVD_SetExternal_CC608_Buffer(MS_U32 u32Id, MS_U32 u32base,MS_U8 u8size);
MS_BOOL MDrv_MVD_SetExternal_CC708_Buffer(MS_U32 u32Id, MS_U32 u32base,MS_U8 u8size);
MS_BOOL MDrv_MVD_SetPrebufferSize(MS_U32 u32Id, MS_U32 size);
E_MVD_Result MDrv_MVD_HWBuffer_ReMappingMode(MS_U32 u32Id,MS_BOOL bEnable);
void MDrv_MVD_REE_GetSHMInformation(MS_VIRT* u32SHMAddr,MS_U32* u32VsyncSHMOffset);
E_MVD_Result MDrv_MVD_SetTimeIncPredictParam(MS_U32 u32Id,MS_U32 u32Para);
MS_BOOL MDrv_MVD_SetDcodeTimeoutParam(MS_U32 u32Id,MS_BOOL enable,MS_U32 u32timeout);
MS_BOOL MDrv_MVD_SetFramebufferAutoMode(MS_U32 u32Id,MS_BOOL bEnable);
E_MVD_Result MDrv_MVD_Set_Smooth_Rewind(MS_U32 u32Id, MS_U8 bEnable);
E_MVD_Result MDrv_MVD_IsAlive(MS_U32 u32Id);
E_MVD_Result MDrv_MVD_Set_Err_Tolerance(MS_U32 u32Id, MS_U16 u16Para);
void MDrv_MVD_EnableAutoInsertDummyPattern(MS_U32 u32Id, MS_BOOL bEnable);
E_MVD_Result MDrv_MVD_PVR_Seamless_mode(MS_U32 u32Id, MS_U8 u8Arg);
E_MVD_Result MDrv_MVD_SetDispFinishMode(MS_U32 u32Id,MS_U8 u8Mode);


#endif

#ifdef __cplusplus
}
#endif

#endif
