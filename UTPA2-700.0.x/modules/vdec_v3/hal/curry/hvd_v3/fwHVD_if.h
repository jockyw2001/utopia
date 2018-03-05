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

#ifndef _FW_HVD_IF_H_
#define _FW_HVD_IF_H_

#include "controller.h"
//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------
#define HVD_FW_VERSION 0x00001499
#define HVD_FW_IF_VERSION 0x00790199

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// TOP
//#if (!(defined( MSOS_TYPE_NOS) ||defined( MSOS_TYPE_ECOS) || defined( MSOS_TYPE_LINUX)))
#if (!defined( _MS_TYPES_H_)  && (!defined(_DRVHVD_COMMON_H_)))
typedef unsigned char               MS_BOOL;                            // 1 byte
/// data type unsigned char, data length 1 byte
typedef unsigned char               MS_U8;                              // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short              MS_U16;                             // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned long               MS_U32;                             // 4 bytes
/// data type unsigned int64, data length 8 byte
typedef unsigned long long          MS_U64;                             // 8 bytes
/// data type signed char, data length 1 byte
typedef signed char                 MS_S8;                              // 1 byte
/// data type signed short, data length 2 byte
typedef signed short                MS_S16;                             // 2 bytes
/// data type signed int, data length 4 byte
typedef signed long                 MS_S32;                             // 4 bytes
/// data type signed int64, data length 8 byte
typedef signed long long            MS_S64;                             // 8 bytes
#endif



#define HVD_DRAM_CMDQ_CMD_SIZE 4
#define HVD_DRAM_CMDQ_ARG_SIZE 4



#define HVD_DRAM_SIZE    0x40000       // Default HVD DRAM heap size, 256k
#define EVD_DRAM_SIZE    0xD0000       // Default EVD DRAM heap size, 832k
#define EVD_DV_DRAM_SIZE 0x1A0000      // Default Dolby vision EVD DRAM heap size, 1664k

#define TEE_ONE_TASK_SHM_SIZE           (0x30000)  // 192K

#define HVD_SHARE_MEM_ST_SIZE           (0x1000)

#define HVD_VBBU_DRAM_ST_SIZE           (0x2000)
#define HVD_DISP_FRM_INFO_EXT_ST_SIZE   (0x1D00)
#define HVD_CMDQ_DRAM_ST_SIZE           (0x100) //Command Queue must align (CMD + ARG) length, ex. 8 bytes, or will encounter bug when use circular buff
#define HVD_DISPCMDQ_DRAM_ST_SIZE       (0x200) //Command Queue must align (CMD + ARG) length, ex. 8 bytes, or will encounter bug when use circular buff
#define HVD_PTS_TABLE_ST_SIZE           (0x4000)

#define HVD_BBU_DRAM_ST_SIZE            (0x2000)
#define HVD_BBU2_DRAM_ST_SIZE           (0x3000)
#define HVD_AVC_DTVINFO_SIZE            (0x1000)
#define HVD_AVC_INFO608_SIZE            (0x1000)
#define HVD_AVC_INFO708_SIZE            (0x4800)
#define HVD_AVC_USERDATA_SIZE           (0x2900)

#define MIN_4K2K_WIDTH  3800
#define MIN_4K2K_HEIGHT 2000

#define VDEC_MIUSEL_MASK                  (0x3)
#define VDEC_BS_MIUSEL                    (0)
#define VDEC_LUMA8_MIUSEL                 (2)
#define VDEC_LUMA2_MIUSEL                 (4)
#define VDEC_CHROMA8_MIUSEL               (6)
#define VDEC_CHROMA2_MIUSEL               (8)
#define VDEC_HWBUF_MIUSEL                 (10)
#define VDEC_BUF1_MIUSEL                  (12)
#define VDEC_BUF2_MIUSEL                  (14)
#define VDEC_PPIN_MIUSEL                  (16)
#define VDEC_XCSHM_MIUSEL                 (18)

#define HVD_DisplayColourVolume_SEI_SIZE  (sizeof(HVD_MasteringDisplayColourVolume))
#define HVD_DisplayColourVolume_SEI_NUM   (2)
#define HVD_ContentLightLevel_SEI_SIZE    (sizeof(HVD_ContentLightLevelInfo))
#define HVD_ContentLightLevel_SEI_NUM     (0x2)


//========= HW settings (Offset base is code buffer address.) =========

#define MAX_PTS_TABLE_SIZE              1024 // 1024 * 16 = 0x4000

#define AVOID_PTS_TABLE_OVERFLOW_THRESHOLD   24
#define HVD_BYTE_COUNT_MASK             0x1FFFFFFF // hvd fw reg_byte_pos 29bit

// bbu entry. 64bits(8 bytes) every entry.
#define HVD_BBU_DRAM_TBL_ENTRY          (HVD_BBU_TBL_SIZE/8)
#define HVD_BBU_DRAM_TBL_ENTRY_TH       (HVD_BBU_DRAM_TBL_ENTRY-4)
#define HVD_BBU2_DRAM_TBL_ENTRY         (HVD_BBU_TBL_SIZE/8)
#define HVD_BBU2_DRAM_TBL_ENTRY_TH      (HVD_BBU2_DRAM_TBL_ENTRY-4)

#define MVC_BBU_DRAM_TBL_ENTRY          (HVD_BBU_TBL_SIZE/8)
#define MVC_BBU_DRAM_TBL_ENTRY_TH       (MVC_BBU_DRAM_TBL_ENTRY-4)
#define MVC_BBU2_DRAM_TBL_ENTRY         (HVD_BBU_TBL_SIZE/8)
#define MVC_BBU2_DRAM_TBL_ENTRY_TH      (MVC_BBU2_DRAM_TBL_ENTRY-4)

#define RVD_BBU_DRAM_TBL_ENTRY          (HVD_BBU_TBL_SIZE/8)
#define RVD_BBU_DRAM_TBL_ENTRY_TH       (RVD_BBU_DRAM_TBL_ENTRY-4)

#define VP8_BBU_DRAM_TBL_ENTRY          (VP8_BBU_TBL_SIZE/8)
#define VP8_BBU_DRAM_TBL_ENTRY_TH       (VP8_BBU_DRAM_TBL_ENTRY-4)

#ifdef LIGHTWEIGHT  //FW31_1.8M
#define HVD_SHARE_MEM_ST_OFFSET         0xC0000
#else
#if defined(SUPPORT_EVD) && (SUPPORT_EVD==1)
#define HVD_SHARE_MEM_ST_OFFSET        0x100000
#else
#define HVD_SHARE_MEM_ST_OFFSET         0xA0000
#endif
#endif

#define HVD_DYNAMIC_SCALING_SIZE         0x1F00
#define HVD_DYNAMIC_SCALING_SIZE_3K       0xC00 // allocate 6k.   actually use: 16 align => 3k
#define HVD_DYNAMIC_SCALING_SIZE_6K      0x1800 // allocate 6k.   actually use: 32 align => 6k
#define HVD_DYNAMIC_SCALING_SIZE_8K      0x1F00
#define HVD_DYNAMIC_SCALING_DEPTH          0x10
#define HVD_DYNAMIC_SCALING_3D_DEPTH       0x18 /// 3D Dynamic scaling use 24.
#define HVD_SCALER_INFO_SIZE              0x100

#define HVD_AVC_FRAME_PACKING_SEI_SIZE  0x100
#define HVD_AVC_FRAME_PACKING_SEI_NUM   2

//DBG
#define HVD_DBG_DUMP_SIZE                0x6500
#define HVD_DUMMY_WRITE_MAX_SIZE          0x200

#define MAX_VSYNC_BRIDGE_DISPQ_NUM      8
#define HVD_DISP_QUEUE_MAX_SIZE         42
#define FRAMEQ_SIZE                      16

// AVC
#define HVD_FW_AVC_DUMMY_FIFO                 256  // bytes
#define HVD_FW_AVC_MAX_DECODE_TICK         100000  // tick ???
#define HVD_FW_AVC_MAX_VIDEO_DELAY           1000  // ms ; based on ???
#define HVD_FW_AVC_ES_MIN_TSP_DATA_SIZE     0x100
#define HVD_FW_AVC_ES_MIN_TSP_DATA_SIZE2   0x1800
#define HVD_FW_BROKEN_BY_US_MIN_DATA_SIZE  0x1800

#define HVD_FW_AVC_ES_UNDER_THRESHOLD     0x800   // 2048
#define HVD_FW_AVC_ES_OVER_THRESHOLD    0x40000 // 256*1024

// User CC
#define USER_CC_DATA_SIZE               38
#define USER_CC_IDX_SIZE                12

// AVS
#define HVD_FW_AVS_DUMMY_FIFO           2048 //BYTES

// RM
#define HVD_FW_RM_DUMMY_FIFO            256  // ??
#define HVD_RM_INIT_PICTURE_SIZE_NUMBER 8

#define EXT_CC_INFO_LENGTH            16
#define EXT_608_CC_PACKET_LENGTH      16
#define EXT_608_CC_DATA_ALIGN   EXT_608_CC_PACKET_LENGTH
#define EXT_708_CC_PACKET_LENGTH     128
#define EXT_708_CC_DATA_ALIGN   EXT_708_CC_PACKET_LENGTH



// Debug
#define HVD_FW_AVS_OUTPUT_INFO_ADDR     0x20001F00UL
#define HVD_FW_AVC_OUTPUT_INFO_ADDR     0x20001F00UL

#define HVD_HW_PREFETCH_SIZE            128
#define HVD_DUMMY_PACKET_SIZE           (HVD_HW_PREFETCH_SIZE << 1)
#define HVD_DUMMY_PACKET_DATA           0x0
#define HVD_DUMMY_PACKET_OFFSET         0x0

#define PRESET_ONE_PENDING_BUFFER       (1 << 0)  /// For AVC, one pending buffer mode, reduce from two to one
#define PRESET_IAP_GN_SHARE_BW_MODE     (1 << 1)  /// For AVC 4K2K, move IAP GN buffer to another miu to share BW mode  //johnny.ko
#define PRESET_DUMMY_PACKET_READY       (1 << 2)  /// For checking HW BBU status after filling nal table
#define PRESET_4K2K_CHECK               (1 << 3)  /// For checking 4k2k need support or not
#define PRESET_FORCE_START_NON_I_SLICE  (1 << 4)  /// For force start decode frame when 1st slice is not I slice.
#define PRESET_CONNECT_DISPLAY_PATH     (1 << 5)  ///
#define PRESET_CAL_FRAMERATE            (1 << 6)  /// For HVD,calculate framerate
#define PRESET_CONNECT_INPUT_TSP        (1 << 7)  ///
#define PRESET_VP9_4K2K_CHECK           (1 << 8)  /// For checking VP9 4k2k need support or not

#define MFCODEC_INFO_VP9_MODE_OFFSET    29
#define MFCODEC_INFO_UNCOMPRESS_OFFSET  28
#define MFCODEC_INFO_MIU_SELECT_OFFSET  24
#define MFCODEC_INFO_PITCH_OFFSET       16
#define MFCODEC_INFO_MFDEC_ID_OFFSET    8

// For Git Changes
#define GIT_TIMESTAMP 1480676357

typedef enum
{
    E_HVD_IQMEM_INIT_NONE = 0,
    E_HVD_IQMEM_INIT_LOADING,   //HK -> FW
    E_HVD_IQMEM_INIT_LOADED,    //FW -> HK
    E_HVD_IQMEM_INIT_FINISH     //HK -> FW

}HVD_IQMEM_INIT_STATUS;



typedef enum
{
    E_HVD_FLUSH_NONE = 0,
    E_HVD_FLUSH_RUNNING,     //HK -> FW
    E_HVD_FLUSH_DONE         //FW -> HK

}HVD_FLUSH_STATUS;

typedef enum
{
    E_HVD_ISR_EVENT_NONE = 0,                        ///< disable ISR
    E_HVD_ISR_EVENT_DISP_ONE = BIT(0),               ///< HVD display one frame on screen.
    E_HVD_ISR_EVENT_DISP_REPEAT = BIT(1),            ///< The current displayed frame is repeated frame.
    E_HVD_ISR_EVENT_DISP_WITH_CC = BIT(2),           ///< Current displayed frame should be displayed with user data.
    E_HVD_ISR_EVENT_DISP_FIRST_FRM = BIT(3),         ///< HVD display first frame on screen.
    E_HVD_ISR_EVENT_DISP_FINISH = BIT(4),            ///< HVD display finish event.

    E_HVD_ISR_EVENT_DEC_CMA_ACTION = BIT(7),         ///< HVD CMA allocate/release memory ISR
    E_HVD_ISR_EVENT_DEC_ONE = BIT(8),                ///< HVD decoded one frame done.
    E_HVD_ISR_EVENT_DEC_I = BIT(9),                  ///< HVD decoded one I frame done.
    E_HVD_ISR_EVENT_DEC_HW_ERR = BIT(10),            ///< HVD HW found decode error.
    E_HVD_ISR_EVENT_DEC_CC_FOUND = BIT(11),          ///< HVD found one user data with decoded frame(with display order).
    E_HVD_ISR_EVENT_DEC_DISP_INFO_CHANGE = BIT(12),  ///< HVD found display information change.
    E_HVD_ISR_EVENT_DEC_DATA_ERR = BIT(13),          ///< HVD HW found decode error.
    E_HVD_ISR_EVENT_DEC_FIRST_FRM = BIT(14),         ///< HVD decode first frame.
    E_HVD_ISR_EVENT_DEC_SEQ_HDR_FOUND = BIT(15),     ///< HVD found sequence header.
} HVD_ISR_Event_Type;

typedef enum
{
    E_HVD_USER_DATA_MODE_DVB_NORMAL                 = 0x00,
    E_HVD_USER_DATA_MODE_DIRECTTV_CC                = 0x01,
    E_HVD_USER_DATA_MODE_FRM_PACKING_ARRANGEMENT    = 0x02,
    E_HVD_USER_DATA_MODE_ATSC_CC_RAW                = 0x04,
    E_HVD_USER_DATA_MODE_CC_UNTIL_START_CODE        = 0x08
} HVD_USER_DATA_MODE;


typedef enum
{
    E_HVD_DRV_AUTO_BBU_MODE = 0x00,
    E_HVD_FW_AUTO_BBU_MODE  = 0x01,
} HVD_BBU_MODE;

typedef enum
{
    E_HVD_FW_STATUS_NONE = 0,               ///< NONE Flag
    E_HVD_FW_STATUS_SEEK_TO_I = BIT(0),     ///< Seek to I slice/frame flag
} HVD_FW_STATUS_FLAG;

typedef enum
{
    //sharemem u32CodecMiscInfo bit assignment
    E_VIDEO_FULL_RANGE = BIT(0),
} CODEC_MISC_INFO;

typedef enum
{
    E_DIVX_PROFILE_NONE = 0,
    E_DIVX_PROFILE_DIVX_PLUS,
    E_DIVX_PROFILE_DIVX_HEVC
} DIVX_PROFILE;

/*
//interupt flag  , value is in VPU RISC MBOX 1 ( for LG GP DTV only)
#define HVD_ISR_USER_DATA               (1 << 0)
#define HVD_ISR_DATA_ERR                (1 << 1)
#define HVD_ISR_PIC_DEC_ERR             (1 << 2)
#define HVD_ISR_DEC_OVER                (1 << 3)
#define HVD_ISR_DEC_UNDER               (1 << 4)
#define HVD_ISR_DEC_I                   (1 << 5)
#define HVD_ISR_DIS_READY               (1 << 6)
#define HVD_ISR_SEQ_INFO                (1 << 7)
#define HVD_ISR_VIDEO_SKIP              (1 << 8)
#define HVD_ISR_VIDEO_REPEAT            (1 << 9)
#define HVD_ISR_VIDEO_FREERUN           (1 << 10)
#define HVD_ISR_INVALID_STREAM          (1 << 11)
#define HVD_ISR_VIDEO_AVSYNC_DONE       (1 << 12)
#define HVD_ISR_VIDEO_VSYNC             (1 << 31)
*/

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
// User CC
#define USR_BUF_SIZE (256)

typedef struct _DTV_BUF_type
{
    MS_U8 type;                 // 0xCC:continue, 0:P 1:B 2:I
    MS_U8 len;                  // size byte of buf
    MS_U8 active;               // 0:free 1:already dma out or not assign 2:assign
    MS_U8 pic_struct;           // pic_struct, Reserved when 0, Top Field when 1, Bottom Field when 2, and Frame picture when 3.
    MS_U32 pts;
    MS_U16 u16TempRefCnt;       // Temp Ref Count for UserData ,Value that increases by 1 for each frame (like time stamp)
    MS_U16 u16Res;              // Reserved
    MS_U32 u32Res;              // Reserved
    MS_U8 buf[USR_BUF_SIZE];       //user data
} DTV_BUF_type;                 //size must <= 276, currently only use 272

#define HVD_FRM_PACKIMG_PAYLOAD_SIZE ((HVD_AVC_FRAME_PACKING_SEI_SIZE/HVD_AVC_FRAME_PACKING_SEI_NUM)-20)  /// 20: HVD_Frame_packing_SEI size expect payload data

typedef struct
{
    MS_BOOL bUsed;
    MS_BOOL bvaild;
    MS_U8   u8Frm_packing_arr_cnl_flag;         // u(1)
    MS_U8   u8Frm_packing_arr_type;             // u(7)
    MS_U8   u8content_interpretation_type;      // u(6)
    MS_U8   u1Quincunx_sampling_flag:1;         // u(1)
    MS_U8   u1Spatial_flipping_flag:1;          // u(1)
    MS_U8   u1Frame0_flipping_flag:1;           // u(1)
    MS_U8   u1Field_views_flag:1;               // u(1)
    MS_U8   u1Current_frame_is_frame0_flag:1;   // u(1)
    MS_U8   u1Frame0_self_contained_flag:1;     // u(1)
    MS_U8   u1Frame1_self_contained_flag:1;     // u(1)
    MS_U8   u1Reserved1:1;                      // u(1)
    MS_U8   u4Frame0_grid_position_x:4;         // u(4)
    MS_U8   u4Frame0_grid_position_y:4;         // u(4)
    MS_U8   u4Frame1_grid_position_x:4;         // u(4)
    MS_U8   u4Frame1_grid_position_y:4;         // u(4)
    MS_U16  u16CropRight;
    MS_U16  u16CropLeft;
    MS_U16  u16CropBottom;
    MS_U16  u16CropTop;
    MS_U8   u8payload_len;
    MS_U8   u8WaitSPS;
    MS_U8   u8Reserved[2];
    MS_U8   u8payload[HVD_FRM_PACKIMG_PAYLOAD_SIZE];
} HVD_Frame_packing_SEI;

typedef struct
{
    MS_BOOL bUsed;
    MS_BOOL colourVolumeSEIEnabled;
    MS_U32 maxLuminance;
    MS_U32 minLuminance;
    MS_U16 primaries[3][2];
    MS_U16 whitePoint[2];
} HVD_MasteringDisplayColourVolume;

typedef struct
{
    MS_BOOL bUsed;
    MS_BOOL ContentLightLevelEnabled;
    MS_U16  maxContentLightLevel;
    MS_U16  maxPicAverageLightLevel;
} HVD_ContentLightLevelInfo;

// stuct
typedef struct
{
    MS_U16 u16HorSize;
    MS_U16 u16VerSize;
    MS_U32 u32FrameRate;                // Unit: ms
    MS_U8 u8AspectRate;                 // aspect ration ID; for AVC only
    MS_U8 u8Interlace;
    MS_U8 u8AFD;
    //MS_U8 u8par_width;
    //MS_U8 u8par_height;
    MS_U8 bChroma_idc_Mono;             // 1: mono 0: colorful, not mono ; AVC only currently. AVS,RM??
    MS_U16 u16DispWidth;                // Display width or aspect ratio width
    MS_U16 u16DispHeight;               // Display height or aspect ratio height
    MS_U16 u16CropRight;
    MS_U16 u16CropLeft;
    MS_U16 u16CropBottom;
    MS_U16 u16CropTop;
    MS_U16 u16Pitch;                    // ???
    MS_U8  u8ColourPrimaries;           // Color Primaries in VUI
    //****************************
    MS_U8 u8IsOriginInterlace;          // Is Original Interlace mode
    //******************************
    // MS_U16 u16PTSInterval;           // ??? not fill
    // MS_U8 u8MPEG1;                   // may be removed
    // MS_U8 u8PlayMode;                // ??? not fill
    // MS_U8 u8FrcMode;                 // may be removed
} HVD_Display_Info;                     //  bytes

typedef struct
{
    MS_U8 bIsShowErrFrm;
    MS_U8 bIsRepeatLastField;
    MS_U8 bIsErrConceal;
    MS_U8 bIsSyncOn;
    MS_U8 bIsPlaybackFinish;
    MS_U8 u8SyncType;                   // HVD_Sync_Tbl_Type
    MS_U8 u8SkipMode;                   // HVD_Skip_Decode_Type
    MS_U8 u8DropMode;                   // HVD_Drop_Disp_Type
    MS_S8 s8DisplaySpeed;               // HVD_Disp_Speed
    MS_U8 u8FrcMode;                    // HVD_FRC_Mode
    MS_U8 bIsBlueScreen;
    MS_U8 bIsFreezeImg;
    MS_U8 bShowOneField;
    //*****************************
    MS_U8 u8reserve8_1;
    MS_U16 u16reserve16_1;
    //*****************************
} HVD_Mode_Status;                      // 12 bytes

typedef struct
{
    MS_U16 u16Width;
    MS_U16 u16Height;
} HVD_PictureSize;

typedef struct
{
    MS_U32 u32LumaAddr;                 ///< The start offset of luma data. Unit: byte.
    MS_U32 u32ChromaAddr;               ///< The start offset of chroma data. Unit: byte.
    MS_U32 u32PpInLumaAddr;             ///< Luma address (For post-process use)
    MS_U32 u32PpInChromaAddr;           ///< Chroma address (For post-process use)
    MS_U32 u32TimeStamp;                ///< Time stamp(DTS, PTS) of current displayed frame. Unit: 90khz.
    MS_U32 u32ID_L;                     ///< low part of ID number decided by MDrv_HVD_PushQueue().
    MS_U32 u32ID_H;                     ///< high part of ID number decided by MDrv_HVD_PushQueue().
    MS_U8  u8FrmType;                   ///< HVD_Picture_Type, picture type: I, P, B frame
    MS_U8  u8FieldType;                 ///< HVD_Field_Type, none, top , bottom, both field
    MS_U16 u16Pitch;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U32 u32Status;                   ///< 0:None, 1:Init, 2:View, 3:Disp, 4:Free
    MS_U32 u32PrivateData;              ///[STB]only for AVC
    MS_U32 u32LumaAddr_2bit;            ///< The start offset of 2bit luma data. Unit: byte.
    MS_U32 u32ChromaAddr_2bit;          ///< The start offset of 2bit chroma data. Unit: byte.
    MS_U16 u16Pitch_2bit;
    MS_U8  u8LumaBitdepth;
    MS_U8  u8ChromaBitdepth;
    MS_U8  u2Luma0Miu:2;
    MS_U8  u2Luma1Miu:2;
    MS_U8  u2Chroma0Miu:2;
    MS_U8  u2Chroma1Miu:2;
    MS_U8  u8FbIndex;
    MS_U8  reserved8[2];
} HVD_Frm_Information;

typedef enum
{
    HVD_FRM_INFO_EXT_TYPE_10BIT,           // 2bits in 10 bits case
    HVD_FRM_INFO_EXT_TYPE_INTERLACE,       // 2nd field 8bits in interlace case
    HVD_FRM_INFO_EXT_TYPE_10BIT_INTERLACE, // 2nd field 2bits in 10bits interlace case
    HVD_FRM_INFO_EXT_TYPE_MFCBITLEN,       // bit length in MFC case
    HVD_FRM_INFO_EXT_TYPE_MAX,
} HVD_FRM_INFO_EXT_TYPE;

typedef enum
{
    E_DISP_PATH_DEFAULT = 0,
    E_DISP_PATH_DYNMC_DISCONNECT,
    E_DISP_PATH_DYNMC_HANDLING,
    E_DISP_PATH_DYNMC_CONNECTTED

}DISP_PATH_CONNECT_STATUS;


typedef enum _HVD_COMPLEXITY_LEVEL
{
    HVD_COMPLEXITY_LEVEL1 = 1,             // lower than level 2
    HVD_COMPLEXITY_LEVEL2,                 // higher than avg avg BW of 4k30p 8bit
    HVD_COMPLEXITY_LEVEL3,                 // higher than avg avg BW of 4k30p 10bit
    HVD_COMPLEXITY_LEVEL4,                 // higher than avg BW of 4k60p 8bit
    HVD_COMPLEXITY_LEVEL5,                 // higher than avg BW of 4k60p 10bit
} HVD_COMPLEXITY_LEVEL;

typedef enum
{
    E_HVD_HDR_METATYPE_VUI_OFFSET                 = 0, ///< HVD display one frame on screen.
    E_HVD_HDR_METATYPE_SEI_MASTERING_COLOR_OFFSET = 1, ///< The current displayed frame is repeated frame.
    E_HVD_HDR_METATYPE_DOLBY_VISION_OFFSET        = 2, ///< Current displayed frame should be displayed with user data.
    E_HVD_HDR_METATYPE_TCH_OFFSET                 = 3,
    E_HVD_HDR_METATYPE_DOLBY_HDR10_VISION_OFFSET  = 4,
} HVD_HDR_EXT_META_TYPE_OFFSET;

/****************************************************************************************
                                  MFCodecInfo
          +---------------------------------------------------------------+
          |Uncompress|BitLen Miu Select|  Pitch  |   MFCodec Version      |
          |   4 bits |      4 bits     |  8 bits |       16 bits          |
          +---------------------------------------------------------------+
***************************************************************************************/
typedef struct
{
    MS_U32 u32LumaAddrExt[HVD_FRM_INFO_EXT_TYPE_MAX];
    MS_U32 u32ChromaAddrExt[HVD_FRM_INFO_EXT_TYPE_MAX];

    MS_U32 MFCodecInfo;
        // bit[29]     MFCodec 3.0 vp9 mode (1: vp9, 0: h26x)
        // bit[28]     MFCodec uncompress mode
        // bits[27:24] MFCodec blens buffer miu sel
        // bit[23:16]  MFCodec pitch setting
        // bit[8]      MFDec ID
        // bits[7:0]   MFCodec mode

    // SEI start //
    MS_U32 maxLuminance;
    MS_U32 minLuminance;
    MS_U16 primaries[3][2];
    MS_U16 whitePoint[2];
    // SEI end //
    MS_U8 Frm_Info_Ext_avail; ///bit[2]: DV_Enabled, bit[1]: SEI_Enabled,  bit[0]: colur_description_present_flag
    // colour_description start //
    MS_U8 colour_primaries;                            // u(8)
    MS_U8 transfer_characteristics;                    // u(8)
    MS_U8 matrix_coefficients;                         // u(8)
    ////Dolby_Vision////////////
    MS_U8 u8DVMode; // bit[0:1] 0: Disable 1:Single layer 2: Dual layer, bit[2] 0:Base Layer 1:Enhance Layer
    MS_U8 u8CurrentIndex;
    MS_U8 bDMEnable;
    MS_U8 bCompEnable;
    MS_U32 u32DVMetadataAddr;
    MS_U32 u32DVDMSize;

    MS_U32 u32DVCompSize;
    MS_U32 u32DVRegAddr;
    MS_U32 u32DVRegSize;
    MS_U32 u32DVLutAddr;
    MS_U32 u32DVLutSize;
    // Other
    MS_U8  u8ComplexityLevel; // from HVD_COMPLEXITY_LEVEL1 to HVD_COMPLEXITY_LEVEL5. Higher level means more complexity
    MS_U8  u8TileMode;
    MS_U8  u8Reserve[2];
    // Pixel aspect ratio info, crop info
    MS_U32 u32ParWidth;
    MS_U32 u32ParHeight;
    MS_U16 u16CropRight;
    MS_U16 u16CropLeft;
    MS_U16 u16CropBottom;
    MS_U16 u16CropTop;
    // Profiling / benchmark
    MS_U16 u16MIUBandwidth;
    MS_U16 u16Bitrate;
    // HTLB
    MS_U8  u8HTLBTableId;
    MS_U8  u8HTLBEntriesSize;
    MS_U32 u32HTLBEntriesAddr;
    MS_U8  u8Reserve1[2];

    //qos
    MS_S16 s16MinQp;
    MS_S16 s16AvgQp;
    MS_S16 s16MaxQp;
    MS_S16 s16MinMV;
    MS_S16 s16AvgMV;
    MS_S16 s16MaxMV;
    MS_U32 u32SkipMV;
    MS_U32 u32NonSkipMV;
} HVD_Frm_Information_EXT_Entry; // sizeof(HVD_Frm_Information_EXT_Entry) * HVD_DISP_QUEUE_MAX_SIZE must be smaller than HVD_DISP_FRM_INFO_EXT_ST_SIZE

typedef struct
{
    HVD_Frm_Information_EXT_Entry stEntry[HVD_DISP_QUEUE_MAX_SIZE];
} HVD_Frm_Information_EXT; // size must be smaller than HVD_DISP_FRM_INFO_EXT_ST_SIZE

typedef struct
{
    MS_BOOL aspect_ratio_info_present_flag;            // u(1)
    MS_U8 aspect_ratio_idc;                            // u(8)
    MS_U16 sar_width;                                  // u(16)
    MS_U16 sar_height;                                 // u(16)
    MS_BOOL overscan_info_present_flag;                // u(1)
    MS_BOOL overscan_appropriate_flag;                 // u(1)
    MS_BOOL video_signal_type_present_flag;            // u(1)
    MS_U8 video_format;                                // u(3)
    MS_BOOL video_full_range_flag;                     // u(1)
    MS_BOOL colour_description_present_flag;           // u(1)
    MS_U8 colour_primaries;                            // u(8)
    MS_U8 transfer_characteristics;                    // u(8)
    MS_U8 matrix_coefficients;                         // u(8)
    MS_BOOL chroma_location_info_present_flag;         // u(1)
    MS_U8 chroma_sample_loc_type_top_field;            // ue(v) 0~5
    MS_U8 chroma_sample_loc_type_bottom_field;         // ue(v) 0~5
    MS_BOOL timing_info_present_flag;                  // u(1)
    MS_BOOL fixed_frame_rate_flag;                     // u(1)
    MS_U32 num_units_in_tick;                          // u(32)
    MS_U32 time_scale;                                 // u(32)
} HVD_AVC_VUI_DISP_INFO;

typedef struct
{
    MS_U32 u32FrmrateUpBound;       //Framerate filter upper bound
    MS_U32 u32FrmrateLowBound;      //Framerate filter lower bound
    MS_U32 u32MvopUpBound;          //mvop filter upper bound
    MS_U32 u32MvopLowBound;         //mvop filter lower bound
} HVD_DISP_THRESHOLD;

typedef struct tDynmcDispPath
{
    MS_U8 u8Connect; //TRUE: connect , FALSE: disconnect
    /*CTL_DISPLAY_PATH*/
    MS_U8 u8DispPath;
    /*DISP_PATH_CONNECT_STATUS*/
    MS_U8 u8ConnectStatus;
} DynmcDispPath;

typedef struct
{
    MS_U32  u32Version;
    MS_U8   u8MatrixCoefficients;
    MS_U8   u8BitsPerChannel;
    MS_U8   u8ChromaSubsamplingHorz;
    MS_U8   u8ChromaSubsamplingVert;
    MS_U8   u8CbSubsamplingHorz;
    MS_U8   u8CbSubsamplingVert;
    MS_U8   u8ChromaSitingHorz;
    MS_U8   u8ChromaSitingVert;
    MS_U8   u8ColorRange;
    MS_U8   u8TransferCharacteristics;
    MS_U8   u8ColourPrimaries;
    MS_U8   reserved;
    MS_U16  u16MaxCLL;          //Max Content Light Level
    MS_U16  u16MaxFALL;         //Max Frame Average Light Level
    MS_U16  u16Primaries[3][2];
    MS_U16  u16WhitePoint[2];
    MS_U32  u32MaxLuminance;
    MS_U32  u32MinLuminance;
} HVD_Config_VP9HDR10;

typedef struct
{
    // switch
    MS_U32 u32CodecType;                //0x0000
    MS_U32 u32FrameBufAddr;             //0x0004
    MS_U32 u32FrameBufSize;             //0x0008
    MS_U32 u32CPUClock;                 //0x000C
    HVD_Display_Info DispInfo;          //0x0010

    // FW -> HK
    // report info
    //AFD_Info AFDInfo;
    MS_U32 u32DispSTC;                  //0x002C // Current Display Frame STC
    MS_U32 u32DecodeCnt;                //0x0030 // Decoded picture count
    MS_U32 u32DecErrCnt;                //0x0034 // HW decode err or not finish.
    MS_U32 u32DataErrCnt;               //0x0038 // FW process data error, like SPS, slice header .etc.
    MS_U16 u16ErrCode;                  //0x003C // Drv/FW error code ; HVD_Err_Code
    MS_U8  u8FrameMbsOnlyFlag;          //0x003E // frame_mbs_only_flag of AVC SPS.
    MS_U8  u8ForceBreakCnt;             //0x003F //
    MS_U32 u32VPUIdleCnt;               //0x0040 // VPU idle count
    MS_U32 u32FrameRate;                //0x0044 // Input Frame Rate
    MS_U32 u32FrameRateBase;            //0x0048 // Input Frame Rate Base
    HVD_Mode_Status ModeStatus;         //0x004C // FW mode
    HVD_Frm_Information DispFrmInfo;    //0x005C // current displayed frame information.
    HVD_Frm_Information DecoFrmInfo;    //0x0098 // specified decoded frame information.
    //MS_U8 u8DecPictType;                // Current decode picture type: E_HVD_PICT_TYPE_I: I frm, E_HVD_PICT_TYPE_P: ref(P) , E_HVD_PICT_TYPE_B: non-ref(B) (GP2 need only)
#if defined(INTERLEAVE_SW_SEEK) || defined(SW_GETBITS) || defined(INTERLEAVE_SW_PARSE)
    MS_U32 u32BBUReadPtr;
#endif
    // internal control info
    MS_U8 bInitDone;                    //0x00D4
    MS_U8 bIs1stFrameRdy;               //0x00D5 // first frame are showed on screen
    MS_U8 bIsIFrmFound;                 //0x00D6 // 1: First I frame found. 0: fw should set to zero after user cmd, "Flush"
    MS_U8 bIsSyncStart;                 //0x00D7 // under sync mode, 1: FW start doing sync action. 0: FW freerun or freerun mode.
    MS_U8 bIsSyncReach;                 //0x00D8 // under sync mode, 1: FW sync reach. 0: FW freerun or sync not reach.

    //****************************************

    MS_U8 u8SrcMode;                    //0x00D9
    MS_U8 bEnableDispQueue;             //0x00DA
    MS_U8 bEnableDispOutSide;           //0x00DB
    //****************************************
    MS_U32 u32FWVersionID;              //0x00DC // FW version ID
    MS_U32 u32FWIfVersionID;            //0x00E0 // FW IF version ID
    MS_U32 u32ESWritePtr;               //0x00E4 // the write pointer of bitstream buffer.
    MS_U16 u16DecQNumb;                 //0x00E8 // current decoded queue total entry number. old oq size
    MS_U16 u16DispQNumb;                //0x00EA // current display queue total entry number. old Used Size
    MS_U32 u32PTStableWptrAddr;         //0x00EC // The address of PTS table write pointer.
    MS_U32 u32PTStableRptrAddr;         //0x00F0 // The address of PTS table read pointer.
    MS_U32 u32PTStableByteCnt;          //0x00F4 // The value of byte count of TSP. FW update it after init() and flush().

    // debug info
    MS_U32 u32SkipCnt;                  //0x00F8 // skipped picture count count by command: E_HVD_DECODE_ALL, E_HVD_DECODE_I, E_HVD_DECODE_IP
    MS_U32 u32DropCnt;                  //0x00FC // dorpped decoded picture counter by command: drop_auto or drop_once
    MS_U32 u32CCBase;                   //0x0100 // CC Ring Base Address
    MS_U32 u32CCSize;                   //0x0104 // CC Ring Size
    MS_U32 u32CCWrtPtr;                 //0x0108 // CC Ring Write Pointer
    MS_U32 u32NtscCCBase;               //0x010C // NTSC CC Ring Base Address
    MS_U32 u32NtscCCSize;               //0x0110 // NTSC CC Ring Size
    MS_U32 u32NtscCCWrtPtr;             //0x0114 // NTSC CC Ring Write Pointer
    //****************************************
    MS_U32 u32CurrentPts;               //0x0118 // only useful when Jump to pts command is activated
    MS_U32 u32DispCnt;                  //0x011C // Display picture count
    MS_U32 u32FWBaseAddr;               //0x0120
    //****************************************
    MS_U32 u32UserCCBase;               //0x0124 // User CC Base Address
    MS_U32 u32UserCCIdxWrtPtr;          //0x0128 // User CC Idx Write Pointer
    MS_U8 u8UserCCIdx[USER_CC_IDX_SIZE];//0x012C // User CC Idx
    //****************************************
    MS_U32 u32VirtualBoxWidth;          //0x0138 // Dynamic Scale: DRV -> FW
    MS_U32 u32VirtualBoxHeight;         //0x013C // Dynamic Scale: DRV -> FW
    MS_U32 u32SrcWidth;                 //0x0140 // Dynamic Scale: Source Width
    MS_U32 u32SrcHeight;                //0x0144 // Dynamic Scale: Source Height
    //****************************************
    MS_U8 u8DivxProfile;                //0x0148  // see DIVX_PROFILE, E_DIVX_PROFILE_NONE is not a DivX stream
    //****************************************

    // -------- AVC info --------
    //MS_U32 u32AVC_NalCnt;             // Decoded nal count >> change to SRAM
    MS_U8  u8AVC_SPS_LowDelayHrdFlag;   //0x0149 // VUI low_delay_hrd_flag
    MS_U16 u16AVC_SPS_LevelIDC;         //0x014A // sps level idc
    MS_U32 u32AVC_VUIDispInfo_Addr;     //0x014C // VUI Display Info Address
    //MS_U32 u32AVC_SPS_Addr;           // FW sps structure start address

    // -------- AVS info --------
    // .....
    //MS_U32 u32AVS_xxx;

    // -------- RM info --------
    // HK -> FW
    MS_U8 u8RM_Version;                 //0x0150
    MS_U8 u8RM_NumSizes;                //0x0151
    MS_U8 u8BitDepth;                   //0x0152 Bit0~3 Y bitdepth, Bit4~7 UV bitdepth
    //****************************************
    MS_U8 reserved8_2;                  //0x0153
    //****************************************
    HVD_PictureSize  pRM_PictureSize[HVD_RM_INIT_PICTURE_SIZE_NUMBER];  //0x0154
    MS_U32 u32RM_VLCTableAddr;          //0x0174

    // -------- common info --------
    MS_U32 u32MainLoopCnt;              //0x0178
    MS_U32 u32VsyncCnt;                 //0x017C
    HVD_DISP_THRESHOLD DispThreshold;   //0x0180
    MS_U32 u32ESReadPtr;                //0x0190 // the read pointer of bitstream buffer.
    MS_U32 u32SeqChangeInfo;            //0x0194
    MS_S64 s64PtsStcDiff;               //0x0198 // 90Khz
    MS_U16 u16ChipID;                   //0x01A0 // enum MSTAR_CHIP_ID
    MS_U16 u16ChipECONum;               //0x01A2 // ECO num of chip
    MS_U32 u32NextPTS;                  //0x01A4 // ms


    MS_U16 u16DispQSize;                //0x01A8
    MS_U16 u16DispQPtr;                 //0x01AA
    HVD_Frm_Information DispQueue[HVD_DISP_QUEUE_MAX_SIZE];   //0x01AC
    //----------------------------------------------------------------------
    MS_U32 u32RealFrameRate;            //0x0B84

    MS_U8 bSpsChange;                   //0x0B88
    MS_U8 bEnableDispCtrl;              //0x0B89
    MS_U8 bIsTrigDisp;                  //0x0B8A
    MS_U8 bHVDUseTlbMode;               //0x0B8B //0: default to disable TLB , 1: use TLB (HK->FW)
    MS_U32 u32FwState;                  //0x0B8C
    MS_U32 u32FwInfo;                   //0x0B90
    MS_U32 u32IntCount;                 //0x0B94

    //----------------------------------------------------------------------
    MS_U16 u16FreeQWtPtr;              //0x0B98
    MS_U16 u16FreeQRdPtr;              //0x0B9A
    MS_U32 FreeQueue[HVD_DISP_QUEUE_MAX_SIZE];  //0x0B9C

    // --------- MVC info (Sub view buffer and 2nd input pointer) ---------
    HVD_Frm_Information DispFrmInfo_Sub;    //0x0C44  // current displayed Sub frame information.
    HVD_Frm_Information DecoFrmInfo_Sub;    //0x0C80  // specified decoded Sub frame information.
    MS_U32 u32ES2WritePtr;              //0x0CBC
    MS_U32 u32ES2ReadPtr;               //0x0C08

    // --------- MJPEG share memory ------------------------------------------
    MS_U32 u32MJPEGFrameBuffIdx;        //0x0CC4 <----LOUIS DONE
    MS_U32 u32MJPEGTimeStamp;           //0x0CC8
    MS_U32 u32MJPEGID_L;                //0x0CCC
    MS_U32 u32MJPEGID_H;                //0x0CD0
    MS_U32 u32MJPEG_NextFrameBuffIdx;   //0x0CD4
    MS_U8 u8MJPEG_bStepPlay;            //0x0CD8
    MS_U8 u8MJPEG_bPlaying;             //0x0CD9
    MS_U8 u8MJPEG_bIsAVSyncOn;          //0x0CDA
    MS_U8 u8MJPEG_bIsReachAVSync;       //0x0CDB
    MS_U8 u8MJPEG_bFlushQueue;          //0x0CDC
    MS_U8 u8MJPEG_bIsDispFinish;        //0x0CDD
    MS_U8 u8MJPEG_bQueueFull;           //0x0CDE
    MS_U8 bIsLeastDispQSize;            //0x0CDF

    // --------- SEI: frame packing ------------------------------------------
    MS_U32 u32Frm_packing_arr_data_addr;  //0x0CE0

    //---------- report 3k/6k for 16/32 Mem-Align DS --------------------------
    MS_U32 u32DSBuffSize;               //0x0CE4  // Dynamic Scale Buffer Size actually used for different DS Mem Align
    MS_U8 bDSIsRunning;                 //0x0CE8
    MS_U8 reserved8_3;                  //0x0CE9
    MS_U8 reserved8_4;                  //0x0CEA
    MS_U8 reserved8_5;                  //0x0CEB
    MS_U8 u8FlushStatus;                //0x0CEC
    MS_U8 u8DSBufferDepth;              //0x0CED

    //---------- TemporalScalability -----------------------------------------
    MS_U8 u8TemporalScalabilty;         //0x0CEE
    MS_U8 u8MaxTemporalLayer;           //0x0CEF

    MS_U16 u16DispQWptr[2];             //0x0CF0
    MS_U8 u8ESBufStatus;                //0x0CF4
    MS_U8 u8FieldPicFlag;               //0x0CF5
    MS_U8 u8CMAAllocationStatus;        //0x0CF6
    MS_U8 u8CMAReleaseStatus;           //0x0CF7

    // reserved for MJPEG
    MS_U32 u32MJPEGDbg_DispStatus;      //0x0CF8
    MS_U8 u8MJPEGDbg_ReadFbIdx;         //0x0CFC
    MS_U8 u8MJPEGDbg_WriteFbIdx;        //0x0CFD
    MS_U8 u8MJPEGDbg_SkipRepeat;        //0x0CFE
    MS_U8 u8MJPEGDbg_reserved8_1;       //0x0CFF
    MS_U32 u32MJPEGDbg_SysTime;         //0x0D00
    MS_U32 u32MJPEGDbg_VideoPts;        //0x0D04
    MS_U32 u32MJPEGDbg_SkipRepeatTime;  //0x0D08

    MS_U32 u32DSbufferAddr;             //0x0D0C
    MS_U32 u32DispRepeatCnt;            //0x0D10

    MS_U32 u32ColocateBBUReadPtr;       //0x0D14 FW->HK
    MS_U32 u32ColocateBBUWritePtr;      //0x0D18 HK->FW
    MS_U8  u8BBUMode;                   //0x0D1C  0: driver auto bbu mode, 1: fw auto bbu mode(colocate bbu mode)
    MS_U8  bUseTSPInBBUMode;            //0x0D1D  0: disable, 1: enable
    MS_U8  bUseWbMvop;                  //0x0D1E  0: use original MVOP, 1: use WB MVOP (HK->FW)
    MS_U8  bHVDIMIEnable;               //0x0D1F

    MS_U32 u32DmxFrameRate;             //0x0D20 // Demuxer Prefered Input Frame Rate
    MS_U32 u32DmxFrameRateBase;         //0x0D24 // Demuxer Prefered Input Frame Rate Base
    MS_U32 u32PTSTblRd;                 //0x0D28 // PTS table read ptr
    MS_U32 u32PTSTblWr;                 //0x0D2C // PTS table write ptr
    MS_U32 u32PreSetControl;            //0x0D30 // PreSetControl
    MS_U32 u32IapGnBufAddr;             //0x0D34
    MS_U32 u32IapGnBufSize;             //0x0D38
    MS_U32 u32SeamlessTSStatus;         //0x0D3C
    MS_U32 u32FWStatusFlag;             //0x0D40
    MS_U32 u32ESBufLevel;               //0x0D44
    MS_U32 u32ESBuf2Level;              //0x0D48
    MS_U32 u32FrameBuf2Addr;            //0x0D4C
    MS_U32 u32FrameBuf2Size;            //0x0D50
    MS_U8  bCMA_Use;                    //0x0D54
    MS_U8  bCMA_AllocDone;              //0x0D55
    MS_U8  bCMA_TwoMIU;                 //0x0D56
    MS_U8  u8FrmPostProcSupport;        //0x0D57
    MS_U8  u8PpQueueSize;               //0x0D58
    MS_U8  u8PpQueueWPtr;               //0x0D59
    MS_U8  u8PpQueueRPtr;               //0x0D5A
    MS_U8  u8CodecFeature;              //0x0D5B // Bit0: For AP to force vdec allocate 8bit framebuffer even decoding 10bit stream Bit1: Enable MFCODEC Bit2: Force MFCODEC nncompress mode
    MS_U32 u32DISPQUEUE_EXT_ST_ADDR;    //0x0D5C
    MS_U64 u64SeamlessTargetPTS;        //0x0D60
    MS_U32 u32SeamlessTargetPOC;        //0x0D68
    MS_U32 u32CodecMiscInfo;            //0x0D6C //Bit0: video full range bit
    MS_U32 u32RDPTR_PTS_LOW;            //0x0D70
    MS_U32 u32RDPTR_PTS_HIGH;           //0x0D74
    MS_U32 u32WRPTR_PTS_LOW;            //0x0D78
    MS_U32 u32WRPTR_PTS_HIGH;           //0x0D7C
    MS_U32 u32DisplayColourVolume_addr;    //0x0D80
    MS_U32 u32HVD_DisplayColourVolume_SEI; //0x0D84
    MS_U32 u32ContentLightLevel_addr;    //0x0D88
    MS_U32 u32HVD_ContentLightLevel_SEI;    //0x0D8C
    MS_U32 u32AllocateCMABuffAddr[2];      //0x0D90
    MS_U32 u32AllocateCMABuffSize[2];      //0x0D98
    PENDING_RELEASE_QUEUE pending_release_queue[2][2]; //0x0DA0~0x0DD0
    MS_U32 u32MaxCMAFrameBufSize;          //0x0DD0
    MS_U32 u32MaxCMAFrameBuf2Size;         //0x0DD4
    MS_U32 u32DirectStcInMs;               //0x0DD8

    //vdec plus info
    MS_U32 u32VdecPlusDecCnt;           //0x0DDC
    MS_U32 u32VdecPlusDropCnt;          //0x0DE0
    MS_U32 u32VdecPlusDispPicCnt;       //0x0DE4
    MS_U8  u8VdecPlusDropRatio;         //0x0DE8
    MS_U8  u8LowMemMode;                //0x0DE9
    MS_U8  bIsTSPIn;                    //0x0DEA
    MS_U8  u8ExternalHeapIdx;           //0x0DEB
    MS_U8  bUseCorrectVlcAddr;          //0x0DEC
    MS_U8  bSingleLayer;                //0x0DED
    MS_U8  u8DVProfileFromDriverAPI;    //0x0DEE
    MS_U8  u8DVLevelFromDriverAPI;      //0x0DEF
    DynmcDispPath    stDynmcDispPath;   //0x0DF0
    MS_U8  u8DolbyMetaReorder;          //0x0DF3
    MS_U32 u32FrameWrtPtr;              //0x0DF4
    MS_U32 u32FrameReadPtr;             //0x0DF8
    MS_U8  u8FrameType[FRAMEQ_SIZE];    //0x0DFC
    MS_U32 u32DecTimeStamp[FRAMEQ_SIZE];  //0x0E0C
    MS_U32 u32NotSupportInfo;           //0x0E4C
    MS_U32 u32DolbyVisionXCShmSize;     //0x0E50
    MS_U8  u8InputTSP;                  //0x0E54
    MS_U8  u8VP9HDR10InfoVaild;         //0x0E55
    MS_U8  u8IsDoblyHDR10;              //0x0E56
    MS_U8  reserved8_6;                 //0x0E57
    MS_U32 u32CurMinTspDataSize;        //0x0E58 //byte
    HVD_Config_VP9HDR10 VP9HDR10Info;   //0x0E5C
    MS_U8  reserved8_9[0xF9C-0x0E88];   //0x0E88

    MS_U32 u32VDEC_MIU_SEL;             //0x0F9C
    MS_U32 u32MaxVideoWidth;            //0x0FA0 // for VDEC3_FB usage
    MS_U32 u32MaxVideoHeight;           //0x0FA4 // for VDEC3_FB usage
    CMD_QUEUE cmd_queue;                //0x0FA8~0x0FBC DISPCMDQ and normal CMDQ
    MS_U32 u32HVD_VBBU_DRAM_ST_ADDR;    //0x0FC0
    MS_U32 u32HVD_PTS_TABLE_ST_OFFSET;  //0x0FC4
    MS_U32 u32HVD_BBU_DRAM_ST_ADDR;     //0x0FC8
    MS_U32 u32HVD_BBU2_DRAM_ST_ADDR;    //0x0FCC
    MS_U32 u32HVD_DYNAMIC_SCALING_ADDR; //0x0FD0
    MS_U32 u32HVD_SCALER_INFO_ADDR;     //0x0FD4
    MS_U32 u32HVD_AVC_DTVINFO;          //0x0FD8
    MS_U32 u32HVD_AVC_INFO608;          //0x0FDC
    MS_U32 u32HVD_AVC_INFO708;          //0x0FE0
    MS_U32 u32HVD_AVC_USERDATA;         //0x0FE4
    MS_U32 u32HVD_AVC_FRAME_PACKING_SEI;//0x0FE8
    MS_U32 u32HVD_DBG_DUMP_ADDR;        //0x0FEC
    MS_U32 u32HVD_DUMMY_WRITE_ADDR;     //0x0FF0
    MS_U16 u16EngineClock;              //0x0FF4
    MS_U16 u16PPUClock;                 //0x0FF6
    MS_U32 u32COMPARE_INFO_ADDR;        //0x0FF8
    MS_U32 u32COMPARE_MD5_ADDR;         //0x0FFC
} HVD_ShareMem;

typedef struct
{
    MS_U32 u32LumaAddr0;                 ///< The start offset of luma data. Unit: byte.
    MS_U32 u32ChromaAddr0;               ///< The start offset of chroma data. Unit: byte.
    MS_U32 u32LumaAddr1;                 ///< The start offset of luma data. Unit: byte.
    MS_U32 u32ChromaAddr1;               ///< The start offset of chroma data. Unit: byte.
    MS_U32 u32PriData;                   ///< Index for SEC release frame buffer
    MS_U32 u32PriData1;                  ///< Index for SEC release frame buffer
    MS_U32 u32Status;
    MS_U16 u16Pitch;
    MS_U16 u16Width;
    MS_U16 u16Height;
    MS_U16 u16CropLeft;
    MS_U16 u16CropRight;
    MS_U16 u16CropBottom;
    MS_U16 u16CropTop;
    MS_U8  u1BottomFieldFirst:1;
    MS_U8  u1DSIndex1Valid:1;
    MS_U8  u2Reserved:6;
    MS_U8  u8FieldType;                 ///< HVD_Field_Type, none, top , bottom, both field
    MS_U8  u8Interlace;
    MS_U8  u8ColorFormat;               // 0 -> 420, 1 -> 422, 2 -> 420 10 bit
    MS_U8  u8FrameNum;                  // if 2, u32LumaAddr1 and u32ChromaAddr1 should be use
    MS_U8  u8RangeMapY;                 // for VC1 or 10 BIT frame, 2 bit Y depth
    MS_U8  u8RangeMapUV;                // for VC1 or 10 BIT frame, 2 bit UV depth
    MS_U8  u8TB_toggle;                 // 0 -> TOP then BOTTOM
    MS_U8  u8Tog_Time;
    MS_U8  u2Luma0Miu:2;
    MS_U8  u2Luma1Miu:2;
    MS_U8  u2Chroma0Miu:2;
    MS_U8  u2Chroma1Miu:2;
    MS_U8  u8FieldCtrl;                 // 0-> Normal, 1->always top, 2->always bot
    union {
        MS_U8 u8DSIndex;
        struct
        {
            MS_U8 u4DSIndex0:4;
            MS_U8 u4DSIndex1:4;         // it is DS index for sFrames[1] (HEVC Dolby EL frame)
        };
    };
    union {
        MS_U16 u16Pitch1;               // for 10 BIT, the 2 bit frame buffer pitch
        MS_U16 u16DispCnt;              // when this display queue is show finish, record the display conut for debug if frame repeat
    };
} DISP_FRM_INFO;

typedef struct
{
    // for vsync bridge dispQ bridge
    MS_U8  u8DispQueNum;
    MS_U8  u8McuDispSwitch;
    MS_U8  u8McuDispQWPtr;
    MS_U8  u8McuDispQRPtr;
    DISP_FRM_INFO McuDispQueue[MAX_VSYNC_BRIDGE_DISPQ_NUM];
    MS_U8  u8DisableFDMask;
    MS_U8  u8FdMaskField;
    MS_U8  u8ToggledTime;
    MS_U8  u8ToggleMethod;
    MS_U8  u8Reserve[2];
    MS_U8  u5FRCMode:5;
    MS_U8  u1FBLMode:1;
    MS_U8  u2MirrorMode:2;
    MS_U8  u8Reserve2;
} MCU_DISPQ_INFO;

typedef enum
{
    MS_DISP_FRM_INFO_EXT_TYPE_10BIT,
    MS_DISP_FRM_INFO_EXT_TYPE_INTERLACE = 1, // interlace bottom 8bit will share the same enum value
    MS_DISP_FRM_INFO_EXT_TYPE_DOLBY_EL = 1,  // with dolby enhance layer 8bit
    MS_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE = 2, // interlace bottom 2bit will share the same enum
    MS_DISP_FRM_INFO_EXT_TYPE_10BIT_DOLBY_EL = 2,  // value with dolby enhance layer 2bit
    MS_DISP_FRM_INFO_EXT_TYPE_10BIT_MVC,
    MS_DISP_FRM_INFO_EXT_TYPE_DOLBY_MODE = 4,
    MS_DISP_FRM_INFO_EXT_TYPE_INTERLACE_MVC = 4,
    MS_DISP_FRM_INFO_EXT_TYPE_10BIT_INTERLACE_MVC = 5, // MVC interlace R-View 2bit will share the
    MS_DISP_FRM_INFO_EXT_TYPE_DOLBY_META = 5,          // same enum with dolby meta data
    MS_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN,
    MS_DISP_FRM_INFO_EXT_TYPE_MFCBITLEN_MVC,
    MS_DISP_FRM_INFO_EXT_TYPE_MAX,
} DISP_FRM_INFO_EXT_TYPE;

/****************************************************************************************
                                  MFCodecInfo
          +---------------------------------------------------------------+
          |Uncompress|BitLen Miu Select|  Pitch  |   MFCodec Version      |
          |   4 bits |      4 bits     |  8 bits |       16 bits          |
          +---------------------------------------------------------------+
***************************************************************************************/
typedef struct
{
    MS_U32 u32LumaAddrExt[MS_DISP_FRM_INFO_EXT_TYPE_MAX];
    MS_U32 u32ChromaAddrExt[MS_DISP_FRM_INFO_EXT_TYPE_MAX];
    MS_U32 MFCodecInfo;
    MS_U16 u16Width;      // the width of second frame
    MS_U16 u16Height;     // the height of second frame
    MS_U16 u16Pitch[2];   // the pitch of second frame
} DISP_FRM_INFO_EXT;

typedef struct
{
    MS_U8 u8Pattern[4];
    MS_U32 u32Version;
    MS_U32 u32Debug;
    MS_U16 u16VsyncCnt;
    MS_U16 u16Debug;
    DISP_FRM_INFO_EXT McuDispQueue[MAX_VSYNC_BRIDGE_DISPQ_NUM];
} MCU_DISPQ_INFO_EXT;

typedef struct
{
    MS_U32 u32ByteCnt;
    MS_U32 u32PTS;
    MS_U32 u32ID_L;
    MS_U32 u32ID_H;
} HVD_PTS_Entry;

// enum
typedef enum
{
    E_MSTAR_CHIP_NONE = 0,
    E_MSTAR_CHIP_U3,        //remove
    E_MSTAR_CHIP_T3,        //remove
    E_MSTAR_CHIP_T4,        //remove
    E_MSTAR_CHIP_JANUS,     //remove
    E_MSTAR_CHIP_U4,        //remove
    E_MSTAR_CHIP_T8,        //remove
    E_MSTAR_CHIP_T9,        //remove
    E_MSTAR_CHIP_M10,       //remove
    E_MSTAR_CHIP_T12,       //remove
    E_MSTAR_CHIP_T13,       //remove
    E_MSTAR_CHIP_J2,        //remove
    E_MSTAR_CHIP_K1,
    E_MSTAR_CHIP_A1,        //remove
    E_MSTAR_CHIP_A5,        //remove
    E_MSTAR_CHIP_A7,        //remove
    E_MSTAR_CHIP_K2,
    E_MSTAR_CHIP_A3,        //remove
    E_MSTAR_CHIP_A7P,       //remove
    E_MSTAR_CHIP_AGATE,     //remove
    E_MSTAR_CHIP_M12,
    E_MSTAR_CHIP_EAGLE,
    E_MSTAR_CHIP_EMERALD,
    E_MSTAR_CHIP_EDISON,
    E_MSTAR_CHIP_EIFFEL,
    E_MSTAR_CHIP_CEDRIC,
    E_MSTAR_CHIP_NUGGET,
    E_MSTAR_CHIP_KAISER,
    E_MSTAR_CHIP_NIKE,
    E_MSTAR_CHIP_KENYA,
    E_MSTAR_CHIP_EINSTEIN,
    E_MSTAR_CHIP_NIKON,
    E_MSTAR_CHIP_NAPOLI,
    E_MSTAR_CHIP_MADISON,
    E_MSTAR_CHIP_MONACO,
    E_MSTAR_CHIP_KERES,
    E_MSTAR_CHIP_CLIPPERS,
    E_MSTAR_CHIP_MUJI,
    E_MSTAR_CHIP_MUNICH,
    E_MSTAR_CHIP_MONET,
    E_MSTAR_CHIP_MULAN,
    E_MSTAR_CHIP_MANHATTAN,
    E_MSTAR_CHIP_KRATOS,
    E_MSTAR_CHIP_KANO,
    E_MSTAR_CHIP_MESSI,
    E_MSTAR_CHIP_MILAN,
    E_MSTAR_CHIP_MASERATI,
    E_MSTAR_CHIP_MACAN,
    E_MSTAR_CHIP_KIWI,
    E_MSTAR_CHIP_CURRY,
    E_MSTAR_CHIP_KAYLA,
    E_MSTAR_CHIP_K6,
    E_MSTAR_CHIP_MUSTANG,
    E_MSTAR_CHIP_MAXIM,
    E_MSTAR_CHIP_K6LITE,
    E_MSTAR_CHIP_MATISSE,
    E_MSTAR_CHIP_MOONEY,
    E_MSTAR_CHIP_MARLON,
    E_MSTAR_CHIP_K5TN, // Kentucky
    E_MSTAR_CHIP_MAINZ,
    E_MSTAR_CHIP_OTHER = 0xFF,
} MSTAR_CHIP_ID;

typedef enum
{
    E_HVD_SRC_MODE_DTV = 0,
    E_HVD_SRC_MODE_TS_FILE,
    E_HVD_SRC_MODE_FILE,
    E_HVD_SRC_MODE_TS_FILE_DUAL_ES,
    E_HVD_SRC_MODE_FILE_DUAL_ES,
} HVD_SRC_MODE;

typedef enum
{
    E_VDEC_FORCE_8BITS_MASK          = BMASK(0:0),       ///< 8BITS YUV Mode
    E_VDEC_FORCE_8BITS_MODE          = BIT(0),
    E_VDEC_MFCODEC_MASK              = BMASK(2:1),       ///< MFCodec Mode
        E_VDEC_MFCODEC_DEFAULT       = BITS(2:1, 0),     ///< deflaut: 0x00
        E_VDEC_MFCODEC_FORCE_ENABLE  = BITS(2:1, 1),     ///< force enable: 0x01
        E_VDEC_MFCODEC_FORCE_DISABLE = BITS(2:1, 2),     ///< force disable:0x10
    E_VDEC_MFCODEC_UNCOMPRESS_MODE   = BIT(3),
    E_VDEC_FORCE_MAIN_PROFILE_MASK = BMASK(4:4),
    E_VDEC_FORCE_MAIN_PROFILE   = BIT(4),     // HEVC: Only Support Main profile even this chip support Main10
    E_VDEC_DYNAMIC_CMA_MODE     = BIT(5),     // Enable Dynamic CMA mechanism
    E_VDEC_TEMPORAL_SCALABILITY_MODE = BIT(6),   //Enable/Disable Temporal Scalability Mode
    E_VDEC_DOLBY_VISION_SINGLE_LAYER_MODE = BIT(7)   //Enable/Disable dolby vision single layer Mode
} VDEC_CODEC_FEATURE;

typedef enum
{
    E_HVD_DV_ENABLE_MASK             = BMASK(1:0),       /// Is Dolby vision enable
    E_HVD_DV_SINGLE_LAYER_MODE       = BIT(0),
    E_HVD_DV_DUAL_LAYER_MODE         = BIT(1),
    E_HVD_DV_CURRENT_LAYER           = BIT(2),           /// 1: enhance layer 0: base layer
    E_HVD_DV_METADATA_MIUSEL_MASK    = BMASK(4:3),       /// meta data miu select(2bits)
        E_HVD_DV_METADATA_MIU0       = BITS(4:3, 0),
        E_HVD_DV_METADATA_MIU1       = BITS(4:3, 1),
        E_HVD_DV_METADATA_MIU2       = BITS(4:3, 2),
} HVD_DOLBY_VISION_MODE;

typedef enum
{
    E_HVD_Codec_AVC = 0,
    E_HVD_Codec_AVS,
    E_HVD_Codec_RM,
    E_HVD_Codec_MVC,
    E_HVD_Codec_VP8,
    E_HVD_Codec_MJPEG,
    E_HVD_Codec_VP6,
    E_HVD_Codec_HEVC,
    E_HVD_Codec_VP9,
    E_HVD_Codec_HEVC_DV,
    E_HVD_Codec_UNKNOWN
} HVD_Codec_Type;

typedef enum
{
    E_HVD_PICT_TYPE_I,
    E_HVD_PICT_TYPE_P,
    E_HVD_PICT_TYPE_B,
} HVD_Picture_Type;

typedef enum
{
    E_HVD_FIELD_TYPE_NONE = 0,
    E_HVD_FIELD_TYPE_TOP,
    E_HVD_FIELD_TYPE_BOTTOM,
    E_HVD_FIELD_TYPE_BOTH,
} HVD_Field_Type;

typedef enum
{
    EVD_TOP_FIELD = 1,
    EVD_BOTTOM_FIELD = 2,
    EVD_TOP_BOTTOM_ORDER = 3,
    EVD_BOTTOM_TOP_ORDER = 4,
    EVD_TOP_WITH_PREV = 9,
    EVD_BOTTOM_WITH_PREV = 10,
    EVD_TOP_WITH_NEXT = 11,
    EVD_BOTTOM_WITH_NEXT = 12,
    EVD_UNKNOWN_TYPE = 0xFF,
} HEVC_PIC_STRUCT;

typedef enum
{
    E_HVD_DECODE_ALL,
    E_HVD_DECODE_I,
    E_HVD_DECODE_IP,
} HVD_Skip_Decode_Type;

typedef enum
{
    E_HVD_CMA_ALLOCATION_NONE,
    E_HVD_CMA_ALLOCATION_WAITING,
    E_HVD_CMA_ALLOCATION_DONE,
    E_HVD_CMA_ALLOCATION_FAILED,
} HVD_CMA_Allocation_Status;

typedef enum
{
    E_HVD_CMA_RELEASE_NONE,
    E_HVD_CMA_RELEASE_WAITING,
    E_HVD_CMA_RELEASE_DONE,
} HVD_CMA_Release_Status;

typedef enum
{
    E_HVD_DROP_DISP_AUTO = (1<<0),
    E_HVD_DROP_DISP_ONCE = (1<<1),
} HVD_Drop_Disp_Type;

typedef enum
{
    E_HVD_FRC_NORMAL = 0,
    E_HVD_FRC_32PULLDOWN,               //3:2 pulldown mode (ex. 24p a 60i or 60p)
    E_HVD_FRC_PAL2NTSC ,                //PALaNTSC conversion (50i a 60i)
    E_HVD_FRC_NTSC2PAL,                 //NTSCaPAL conversion (60i a 50i)
    E_HVD_FRC_DISP_2X,                  //output rate is twice of input rate (ex. 30p a 60p)
    E_HVD_FRC_24_50,                    //output rate 24P->50P 48I->50I
    E_HVD_FRC_50P_60P,                  //output rate 50P ->60P
    E_HVD_FRC_60P_50P,                  //output rate 60P ->50P
    E_HVD_FRC_HALF_I,                   //output rate 120i -> 60i, 100i -> 50i
    E_HVD_FRC_120I_50I,                 //output rate 120i -> 60i
    E_HVD_FRC_100I_60I,                 //output rate 100i -> 60i
    E_HVD_FRC_DISP_4X,                  //output rate is four times of input rate (ex. 15P a 60P)
    E_HVD_FRC_15_50,                    //output rate 15P->50P, 30i -> 50i
    E_HVD_FRC_30_50,                    //output rate  30p->50p, 60i->50i
    E_HVD_FRC_30_24,                    //output rate 30p->24p, 60i->24i
    E_HVD_FRC_60_24,                    //output rate 60p->24p, 120i -> 24i
    E_HVD_FRC_60_25,                    //output rate 60p->25p , 120i -> 50i
    E_HVD_FRC_HALF_P,                   //output rate 60p-> 30p, 50p -> 25p
    E_HVD_FRC_25_30,                    //output rate 25p->30p , 50 i-> 30i
    E_HVD_FRC_50_30,                    //output rate 25p->30p ,  100i -> 30i
    E_HVD_FRC_24_30,                    //output rate 24p->30p , 48i -> 30i
} HVD_FRC_Mode;

typedef enum
{
    E_HVD_FRC_DROP_FRAME = 0,
    E_HVD_FRC_DROP_FIELD = 1,
} HVD_FRC_Drop_Mode;

typedef enum
{
    E_HVD_DISP_SPEED_F_32X = 32,
    E_HVD_DISP_SPEED_F_16X = 16,
    E_HVD_DISP_SPEED_F_8X = 8,
    E_HVD_DISP_SPEED_F_4X = 4,
    E_HVD_DISP_SPEED_F_2X = 2,
    E_HVD_DISP_SPEED_1X = 1,
    E_HVD_DISP_SPEED_S_2X = -2,
    E_HVD_DISP_SPEED_S_4X = -4,
    E_HVD_DISP_SPEED_S_8X = -8,
    E_HVD_DISP_SPEED_S_16X = -16,
    E_HVD_DISP_SPEED_S_32X = -32,
} HVD_Disp_Speed;

typedef enum
{
    E_HVD_SYNC_TBL_TYPE_NON,
    E_HVD_SYNC_TBL_TYPE_PTS,
    E_HVD_SYNC_TBL_TYPE_DTS,
    E_HVD_SYNC_TBL_TYPE_STS,            //Sorted TimeStamp
} HVD_Sync_Tbl_Type;                    //only for file mode. Ts , ts file mode always has PTS table

typedef enum
{
    E_HVD_FIELD_CTRL_OFF=0,
    E_HVD_FIELD_CTRL_TOP,       // Always Show Top Field
    E_HVD_FIELD_CTRL_BOTTOM,    // Always Show Bottom Field
} HVD_Field_Ctrl;

typedef enum
{
    E_HVD_BURST_CNT_LV0 = 0,  // U3,T3:32 cycle  T4~U4: 16 cycle
    E_HVD_BURST_CNT_LV1 = 1,  // U3,T3:64 cycle  T4~U4: 32 cycle
    E_HVD_BURST_CNT_LV2 = 2,  // U3,T3:96 cycle  T4~U4: 48 cycle
    E_HVD_BURST_CNT_LV3 = 3,  // U3,T3:128 cycle  T4~U4: 64 cycle
    E_HVD_BURST_CNT_LV4 = 4,  // U3,T3:160 cycle  T4~U4: 80 cycle
    E_HVD_BURST_CNT_LV5 = 5,  // U3,T3:192 cycle  T4~U4: 96 cycle
    E_HVD_BURST_CNT_LV6 = 6,  // U3,T3:224 cycle  T4~U4: 112 cycle
    E_HVD_BURST_CNT_LV7 = 7,  // U3,T3:256 cycle  T4~U4: 128 cycle
    E_HVD_BURST_CNT_DISABLE = 0xFFFFFFFF,
} HVD_MIU_Burst_Cnt_Ctrl;

typedef enum
{
    E_HVD_DISPQ_STATUS_NONE = 0,            //FW
    E_HVD_DISPQ_STATUS_INIT,                //FW
    E_HVD_DISPQ_STATUS_VIEW,                //HK
    E_HVD_DISPQ_STATUS_DISP,                //HK
    E_HVD_DISPQ_STATUS_FREE,                //HK
} HVD_DISPQ_STATUS;

typedef enum
{
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVAV_PER    = 0x1,
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVAV_PEN    = 0x2,
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVHE_DER    = 0x4,
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVHE_DEN    = 0x8,
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVHE_DTR    = 0x10,
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVHE_STN    = 0x20,
    E_HVD_EX_DV_STREAM_PROFILE_ID_DVHE_DTH    = 0x40,
    E_HVD_EX_DV_STREAM_PROFILE_ID_UNSUPPORTED = 0x0,
} HVD_EX_DV_Stream_Profile;

typedef enum
{
    E_HVD_EX_DV_META_REORDER_DEFAULT,
    E_HVD_EX_DV_META_REORDER_FOLLOW_BL,
    E_HVD_EX_DV_META_REORDER_FOLLOW_EL,
} HVD_EX_DV_Metadata_Reorder;

typedef enum
{
    E_HVD_EX_DV_STREAM_LEVEL_ID_HD24 = 0,
    E_HVD_EX_DV_STREAM_LEVEL_ID_HD30,
    E_HVD_EX_DV_STREAM_LEVEL_ID_FHD24,
    E_HVD_EX_DV_STREAM_LEVEL_ID_FHD30,
    E_HVD_EX_DV_STREAM_LEVEL_ID_FHD60,
    E_HVD_EX_DV_STREAM_LEVEL_ID_UHD24,
    E_HVD_EX_DV_STREAM_LEVEL_ID_UHD30,
    E_HVD_EX_DV_STREAM_LEVEL_ID_UHD48,
    E_HVD_EX_DV_STREAM_LEVEL_ID_UHD60,
    E_HVD_EX_DV_STREAM_LEVEL_ID_UNSUPPORTED,
} HVD_EX_DV_Stream_Highest_Level;

typedef enum
{
    E_HVD_DECODER_FREQUENCY_DOWN = 0, // Scale decoder frequency down
    E_HVD_DECODER_FREQUENCY_UP,       // Scale decoder frequency up
    E_HVD_DECODER_FREQUENCY_MIN,      // Scale decoder frequency to minimum
    E_HVD_DECODER_FREQUENCY_MAX,      // Scale decoder frequency to maximum
} HVD_Decoder_Frequency;

typedef enum
{
    // invalid cmd
    E_HVD_CMD_INVALID_CMD = 0xFFFFFFFFUL,

    // SVD old cmd
    E_HVD_CMD_SVD_BASE = 0x00010000,
    /*0x10001*/E_HVD_CMD_PARSER_BYPASS,             // 1 : on :for raw file mode; AVCHVD_CMD_PARSER_BYPASS ; 0: off: TS file mode and live stream
    /*0x10002*/E_HVD_CMD_BBU_RESIZE,                // svd only;  AVCHVD_CMD_BBU_SIZE
    /*0x10003*/E_HVD_CMD_FRAME_BUF_RESIZE,          // svd only; AVCHVD_CMD_RESIZE_MEM
    /*0x10004*/E_HVD_CMD_IGNORE_ERR_REF,            // 1: ignore ref error, 0: enable ref error handle; AVCHVD_CMD_IGNORE_LIST + AVCHVD_CMD_OPEN_GOP
    /*0x10005*/E_HVD_CMD_ES_FULL_STOP,              // ES auto stop: 1: AVCHVD_CMD_ES_STOP; ES not stop 0: AVCHVD_CMD_HANDSHAKE
    /*0x10006*/E_HVD_CMD_DROP_DISP_AUTO,            // 1:on AVCHVD_CMD_DISP_DROP, 0:off AVCHVD_CMD_DIS_DISP_DROP
    /*0x10007*/E_HVD_CMD_DROP_DISP_ONCE,            // AVCHVD_CMD_DROP_CNT
    /*0x10008*/E_HVD_CMD_FLUSH_DEC_Q,               // AVCHVD_CMD_FLUSH_QUEUE

    // HVD new cmd
    E_HVD_CMD_NEW_BASE = 0x00020000,
    // Action
    E_HVD_CMD_TYPE_ACTION_MASK = (0x0100|E_HVD_CMD_NEW_BASE),

    // state machine action
    /*0x20101*/E_HVD_CMD_INIT ,                     // Init FW type: E_HVD_Codec_AVC ; E_HVD_Codec_AVS;  E_HVD_Codec_RM
    /*0x20102*/E_HVD_CMD_PLAY,                      // AVCHVD_CMD_GO
    /*0x20103*/E_HVD_CMD_PAUSE,                     // AVCHVD_CMD_PAUSE
    /*0x20104*/E_HVD_CMD_STOP,                      // AVCHVD_CMD_STOP
    // run-time action
    /*0x20105*/E_HVD_CMD_STEP_DECODE,               // AVCHVD_CMD_STEP
    /*0x20106*/E_HVD_CMD_FLUSH,                     // Arg: 1 show last decode, 0 show current diaplay.FW need to clear read pointer of PTS table under SYNC_PTS, SYNC_DTS. ; BBU: AVCHVD_CMD_DROP ,  DISP: AVCHVD_CMD_FLUSH_DISPLAY , AVCHVD_CMD_SKIPTOI
    /*0x20107*/E_HVD_CMD_BLUE_SCREEN,               // only for AVC. remove auto blue screen before show first frame on screen
    /*0x20108*/E_HVD_CMD_RESET_PTS,                 // reset PTS table for TS file mode. AVCHVD_CMD_RE_SYNC
    /*0x20109*/E_HVD_CMD_FREEZE_IMG,                // FW showes the same frame at every Vsync, but background decode process can not stop. 1: freeze image; 0: normal diaplay
    /*0x2010A*/E_HVD_CMD_JUMP_TO_PTS,               // Arg: PTS(unit: 90kHz). 0: disable this mode. Any not zero value: enable. FW decode to specified PTS by using full speed. During the decoding, FW need not show any decoded frames, just maitain the last frame before get this command.
    /*0x2010B*/E_HVD_CMD_SYNC_TOLERANCE,            // Arg: any not zero number(unit: 90kHz). AVCHVD_CMD_SLOW_SYNC
    /*0x2010C*/E_HVD_CMD_SYNC_VIDEO_DELAY,          // Arg: 0~MAX_VIDEO_DELAY(unit: 90kHz): use Arg of video delay. AVCHVD_CMD_AVSYNC
    /*0x2010D*/E_HVD_CMD_DISP_ONE_FIELD,            // for AVS, AVC only, Arg: HVD_Field_Ctrl. AVCH264_CMD_ONE_FIELD
    /*0x2010E*/E_HVD_CMD_FAST_DISP,                 // Arg: 0: disable, Any not zero value: enable. Always return first frame ready. Don't care the first frame av-sync.
    /*0x2010F*/E_HVD_CMD_SKIP_TO_PTS,               // Arg: PTS(unit: 90kHz). 0: disable this mode. Any not zero value: enable. FW decode to specified PTS by using full speed. FW need not to decode frame until the first I after the specified PTS.
    /*0x20110*/E_HVD_CMD_SYNC_THRESHOLD,            // Arg: 0x01~0xFF , frame repeat time. If arg == 0xFF, fw will always repeat last frame when PTS > STC.
    /*0x20111*/E_HVD_CMD_FREERUN_THRESHOLD,         // Arg: (unit: 90KHz) 0: use default 5 sec (90000 x 5).
    /*0x20112*/E_HVD_CMD_FLUSH_FRM_BUF,             // Arg: 1 show last decode frame, 0 show current diaplay frame. FW will clear all frame buffer then skip to next I frame.
    /*0x20113*/E_HVD_CMD_FORCE_INTERLACE,           // Arg: 0; Diable. Arg: 1; Force interlace only support DTV and TS file mode with framerate 25 or 30 (all resolution, under FHD)
                                                    // Arg: 2; Force interlace support DTV and TS file mode with framerate 25 or 30,but only works on 1080P (width large less 1920)
                                                    // Arg: 3; Force interlace support DTV and TS file mode with framerate 23 to 30 and all resolution.
                                                    // Arg: 4; Force interlace support DTV and TS file mode with under 30 fps and all resolution.
                                                    // Arg: 5; Force interlace support DTV and TS file mode with with framerate 25 or 30 (under FHD and width large less 720)
                                                    // Arg: 6: Force interlace support all mode with under all fps and all resolution.
    /*0x20114*/E_HVD_CMD_DUAL_NON_BLOCK_MODE,       // Arg: 0 disable Arg:1 For dual decode case, force switch to another task when current task is idle
    /*0x20115*/E_HVD_CMD_INPUT_PTS_FREERUN_MODE,    // Arg: 0 disable. Arg:1, video free run when the difference between input PTS and current STC is large than E_HVD_CMD_FREERUN_THRESHOLD + 1s;
    /*0x20116*/E_HVD_CMD_FREEZE_TO_CHASE,           // Arg: 1 enable, 0 disable. Freeze current image when PTS < STC and decode drop / skip frame to sync stc.

    // internal control action

    // FW settings ( only for driver init)
    E_HVD_CMD_SETTINGS_MASK = (0x0200|E_HVD_CMD_NEW_BASE),
    /*0x20201*/E_HVD_CMD_PITCH,                     // Arg:any non-zero number. AVCHVD_CMD_PITCH_1952, AVCHVD_CMD_PITCH_1984
    /*0x20202*/E_HVD_CMD_SYNC_EACH_FRM,             // 1: TS file mode: on ; 0: live mode: off AVCHVD_CMD_SYNC
    /*0x20203*/E_HVD_CMD_MAX_DEC_TICK,              // 0: off ; not 0 : in fw.h  new value AVCHVD_CMD_MAXT
    /*0x20204*/E_HVD_CMD_AUTO_FREE_ES,              // 1: on ; 0: off ; for live stream only AVCHVD_CMD_AUTO_FREE
    /*0x20205*/E_HVD_CMD_DIS_VDEAD,                 // 1: on :For PVR , file mode only ; 0 : off: AVCHVD_CMD_DIS_VDEAD
    /*0x20206*/E_HVD_CMD_MIN_FRAME_GAP,             // Arg: 0~n, 0xFFFFFFFF: don't care frame gap; For file mode only; AVCHVD_CMD_MIN_FRAME_GAP
    /*0x20207*/E_HVD_CMD_SYNC_TYPE,                 // Arg: HVD_Sync_Tbl_Type. //only for file mode. Ts , ts file mode always has PTS table
    /*0x20208*/E_HVD_CMD_TIME_UNIT_TYPE,            // Set Time unit: 0: 90Khz, 1: 1ms
    /*0x20209*/E_HVD_CMD_ISR_TYPE,                  // Add ISR trigger timing.
    /*0x2020A*/E_HVD_CMD_DYNAMIC_SCALE,             // 0: disable; 1: enable
    /*0x2020B*/E_HVD_CMD_SCALER_INFO_NOTIFY,
    /*0x2020C*/E_HVD_CMD_MIU_BURST_CNT,             // Arg 0~7 burst cnt level , 0xFFFFFFFF = Disable
    /*0x2020D*/E_HVD_CMD_FDMASK_DELAY_CNT,          // Arg: 0~0xFF, Fdmask delay count, arg >= 0xFF -> use default.
    /*0x2020E*/E_HVD_CMD_FRC_OUTPUT_FRAMERATE,      // unit: vsync cnt
    /*0x2020F*/E_HVD_CMD_FRC_OUTPUT_INTERLACE,      // 0: progressive; 1: interlace
    /*0x20210*/E_HVD_CMD_ENABLE_DISP_QUEUE,         // 0: Disable; 1:Enable
    /*0x20211*/E_HVD_CMD_FORCE_DTV_SPEC,            // 0: Disable; 1:Enable, Force to follow H264 DTV Spec, if res>720p && framerate>50, force progessive
                                                       // 2: Disable, if frame_mbs_only_flag == TRUE, it's progressive.
    /*0x20212*/E_HVD_CMD_SET_USERDATA_MODE,         // Arg: HVD_USER_DATA_MODE, use "OR", 0x00: Normal DVB user_data mode; 0x01: ATSC DirectTV CC mode
                                                    // 0x02: FPA CallBack, 0x04: ATSC_CC_RAW mode
    /*0x20213*/E_HVD_CMD_ENABLE_DISP_OUTSIDE,       // 0: Disable; 1:Enable
    /*0x20214*/E_HVD_CMD_SUPPORT_AVC_TO_MVC,        // Arg: 0: Disable AVC to MVC, 1: Enable AVC to MVC but non-support DS, 2:Enable AVC to MVC and support DS,
    /*0x20215*/E_HVD_CMD_ENABLE_NEW_SLOW_MOTION,    // Arg: 0: Disable New Slow Motion, 1: Enable New Slow Motion.
    /*0x20216*/E_HVD_CMD_FORCE_ALIGN_VSIZE,         // Arg: 0: Disable and 3D ouput is frame packing mode. 1: Enable VSIZE would be 4 align and Crop Botton would be additional size; 3D output would not be frame packing mode.
    /*0x20217*/E_HVD_CMD_PUSH_DISPQ_WITH_REF_NUM,   // Arg: 0: Disable; 1:Enable
    /*0x20218*/E_HVD_CMD_GET_MORE_FRM_BUF,          // Arg: 0: Disable; 1:Enable. If buffer size is enough, intial more frame buffer to use.
    /*0x20219*/E_HVD_CMD_RM_ENABLE_PTS_TBL,         // Arg, 0:disable, 1:enable. this command is only used by RM, when enable==1, RM will search pts table and return matched u32ID_L
    /*0x2021A*/E_HVD_CMD_DYNAMIC_SCALE_RESV_N_BUFFER,   // Arg, 0:disable, 1:enable. use init_dpb_and_frame_buffer_layout_3 to do dynamic layout other than fixed layout
    /*0x2021B*/E_HVD_CMD_DS_RESET_XC_DISP_WIN,      // Arg, 0: Disable, 1:enable. When Dynamic scaling enable, report the display information change and re-set XC display window.
    /*0x2021C*/E_HVD_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE,     /// Arg, 0: Disable; 1:enable. AVC support reference number is more than maximum DPB size when frame buffer size was enough.
    /*0x2021D*/E_HVD_CMD_FRAMERATE_HANDLING,        // Arg 0~60000, 0: Disable, 1000 ~ 60000: Used the arg to set frame rate when the sequence did not have frame rate info. and arg is not zero. (The frame unit is (arg/1000)fps, Exp: 30000 = 30.000 fps), others: Do not thing.
    /*0x2021E*/E_HVD_CMD_AUTO_EXHAUST_ES_MODE,      // Arg, 0: disable, [31:16]= Upper bound, [15:0] = Lower bound, Unit is 1KBytes, // Auto drop display to consume ES data as soon as possible when ES level is higher than upper bound
    /*0x2021F*/E_HVD_CMD_RETURN_INVALID_AFD,        // Arg, 0: Disable, 1:enable, return 0 when AFD is invalid
                                                    // Arg, 2: Enable, return 0 when AFD is invalid at I frame. (GOP)
    /*0x20220*/E_HVD_CMD_AVC_FORCE_BROKEN_BY_US,    // Arg, 0: Disable, 1:enable, force enable broken by us mode, FW does not need it anymore.
    /*0x20221*/E_HVD_CMD_EXTERNAL_DS_BUF,           // Arg, 0: Disable, 1:Enable.
    /*0x20222*/E_HVD_CMD_SHOW_FIRST_FRAME_DIRECT,   // Arg: 0: Disable; 1:Enable. Push first I frame to display queue directly..
    /*0x20223*/E_HVD_CMD_AVC_RESIZE_DOS_DISP_PEND_BUF,  //Arg: Resize disp pending buffer size for display outside mode(dos), default dos disp pending buf size = 4
    /*0x20224*/E_HVD_CMD_SET_MIN_TSP_DATA_SIZE,         //Arg: Resize HVD_FW_AVC_ES_MIN_TSP_DATA_SIZE
    /*0x20225*/E_HVD_CMD_DYNAMIC_SCALE_ENHANCE_SETTINGS,    //Arg: 0:None, 1:MHP,...
    /*0x20226*/E_HVD_CMD_ONE_PENDING_BUFFER_MODE,   // Arg: 0: Disable; 1:Enable. Use only one pending buffer instead of two.
    /*0x20227*/E_HVD_CMD_ENABLE_EXTERNAL_CC_608_BUF, // Arg: [7:0] =size, [31:8]= start address,  unit =1k bytes , Enable: length/address != 0, Disable: length/address == 0
    /*0x20228*/E_HVD_CMD_ENABLE_EXTERNAL_CC_708_BUF, // Arg: [7:0] =size, [31:8]= start address,  unit =1k bytes , Enable: length/address != 0, Disable: length/address == 0
    /*0x20229*/E_HVD_CMD_SET_DISP_ERROR_TOLERANCE,   // Arg: //[15:8]+[7:0] = (err_tolerance(0%~100%)+enable or disable)
    /*0x2022A*/E_HVD_CMD_SET_PTS_US_MODE,            // Arg: //0: Disable; 1:Enable. return micro seconds PTS in PTS mode
    /*0x2022B*/E_HVD_CMD_SET_DV_XC_SHM_ADDR,         // Arg, address for dolby vision xc DM/composer shm

    // Mode ( for AP run-time)
    E_HVD_CMD_MODE_MASK = (0x0300|E_HVD_CMD_NEW_BASE),
    /*0x20301*/E_HVD_CMD_SKIP_DEC,                  // E_HVD_DECODE_ALL ;E_HVD_DECODE_I;E_HVD_DECODE_IP; AVCHVD_CMD_DEC_I , AVCHVD_CMD_SKIP_NONREF
    /*0x20302*/E_HVD_CMD_DISP_SPEED,                // HVD_Disp_Speed  ;  AVCHVD_CMD_TRICKY  0,1: normal speed N(>0): show N times, slow motion Nx(-2,-4...) ; N(<0): FF speed Nx(2,4,...) AVCHVD_CMD_2X_SPEED
    /*0x20303*/E_HVD_CMD_DISP_ERR_FRM,              // True: display and error frame; FALSE: not show error frame ; AVCHVD_CMD_ERR_TH
    /*0x20304*/E_HVD_CMD_ERR_CONCEAL,               // 1: on ; 0: off ; AVCHVD_CMD_PASTE
    /*0x20305*/E_HVD_CMD_REPEAT_LAST_FIELD,         // 1: ON ; 0: OFF
    /*0x20306*/E_HVD_CMD_FRC_MODE,                  // Arg:HVD_FRC_Mode. AVCHVD_CMD_FRAME_CVT
    /*0x20307*/E_HVD_CMD_SYNC_ACTIVE,               // Arg: 0: sync off. AVCHVD_CMD_FREE_RUN ;  1: sync on. AVCHVD_CMD_AVSYNC
    /*0x20308*/E_HVD_CMD_PLAYBACK_FINISH,           // 1: no more input data, FW need to show frame by itself until all buffers being empty. 0: close this mode.
    /*0x20309*/E_HVD_CMD_BALANCE_BW,                // Arg: Byte0: Quarter Pixel Off Level, Byte1: Deblock Off Level >> 0: off, 1~255: count threshold to enter, Byte2: Upper Bound value. i.e.: Byte0: 1,Byte1: 10,Byte2: 20.
    /*0x2030A*/E_HVD_CMD_POWER_SAVING,              // Arg: 0: Power Saving Off, 1: Power Saving On
    /*0x2030B*/E_HVD_CMD_DIS_DBF,                   // Disable deblock, Arg: 0: off, 1: disable all frame, 2: only disable non-ref frame
    /*0x2030C*/E_HVD_CMD_DIS_QUART_PIXEL,           // Disable quarter pixel, Arg: 0: off, 1: disable for all frame, 2: only dsiable non-ref frame
    /*0x2030D*/E_HVD_CMD_DPO_CC,                    // Display Order User Data Command, Arg: 0: off, 1: on.
    /*0x2030E*/E_HVD_CMD_DISP_I_DIRECT,             // Display I directly, Arg: 0: off, 1: on
    /*0x2030F*/E_HVD_CMD_FORCE_RESET_HW,            // Arg, 0:disable, 1:enable. Force reset hw when frame start
    /*0x20310*/E_HVD_CMD_UPDATE_DISP_THRESHOLD,     // Arg, none
    /*0x20311*/E_HVD_CMD_FRC_DROP_MODE,             // Arg, E_HVD_FRC_DROP_FRAME (0), E_HVD_FRC_DROP_FIELD (1)
    /*0x20312*/E_HVD_CMD_UPDATE_DISPQ,              // Arg, none. Update Frame Status in Display Queue
    /*0x20313*/E_HVD_CMD_SHOW_DECODE_ORDER,         // Arg, 0:disable, 1:enable. Show decoder order or display order
    /*0x20314*/E_HVD_CMD_3DLR_VIEW_EXCHANGE,        // Arg, 0: off, do not thing. 1: on, exchange the L/R views
    /*0x20315*/E_HVD_CMD_DISP_IGNORE_CROP,          // Arg, 0:disable, 1:enable. Ignore crop information when set V-sync to display
    /*0x20316*/E_HVD_CMD_STOP_MVD_PARSER,           // Arg, 1:stop mvd parser
    /*0x20317*/E_HVD_CMD_SUSPEND_DYNAMIC_SCALE,     // Arg, 0:disable, 1:enable. Suspend dynamic scale and raise interrupt.
    /*0x20318*/E_HVD_CMD_AVOID_PTS_TBL_OVERFLOW,    // Arg, 0:disable, 1:enable. for hw tsp mode, mvd parser will stop when pts table is close to overflow and restart when enough pts is consumed.
    /*0x20319*/E_HVD_CMD_IGNORE_PIC_OVERRUN,        // Arg, 0:disable, 1:enable. Ignore hw error: PIC overrun error.
    /*0x2031A*/E_HVD_CMD_RVU_SETTING_MODE,          // Arg, 0:disable, 1:Drop B frame and force IDR.
    /*0x2031B*/E_HVD_CMD_RELEASE_DISPQ,             // Arg, none. Unlock frame status.
    /*0x2031C*/E_HVD_CMD_CTRL_SPEED_IN_DISP_ONLY,   // Arg, 0:disable, control in decoding and displaying time; 1:enable, control speed in displaying time only.
    /*0x2031D*/E_HVD_CMD_IGNORE_PIC_STRUCT_DISPLAY, // Arg, 0:disable, 1:Ignore Pic_struct when display progressive frame.
    /*0x2031E*/E_HVD_CMD_ERR_CONCEAL_SLICE_1ST_MB,  // Arg, 0:disable, Error concealment from current/last MB position; 1:enale, Error concealment from current slice first MB.(Need enable E_HVD_CMD_ERR_CONCEAL)
    /*0x2031F*/E_HVD_CMD_AUTO_DROP_ES_DATA,         // Arg, 0:disable, [31:16]= Upper bound, [15:0] = Lower bound; Unit is 1%~100%: Drop ES data when ES buffer threshold more than 1%~100%.
    /*0x20320*/E_HVD_CMD_AUTO_DROP_DISP_QUEUE,      // Arg, 0:disable, N = 1~16: Drop display queue when display queue above than N frames. It only support Display Queue mode. (bEnableDispQueue = TRUE)
    /*0x20321*/E_HVD_CMD_USE_CPB_REMOVAL_DEALY,     // Arg, 0:disable, 1:enable. Use Cpb_Removal_Delay of Picture timing SEI to control PTS.
    /*0x20322*/E_HVD_CMD_SKIP_N_FRAME,              // Arg, 0:disable, N = 1~63. Skip N frame.
    /*0x20323*/E_HVD_CMD_PVR_SEAMLESS_TIMESHIFT,    // Arg, 0:disable, 1:pause decode, 2:reset hw and wait for playback with target data, 3:seek_to_I after play
    /*0x20324*/E_HVD_CMD_STOP_PARSER_BY_PTS_TABLE_LEVEL,    // Arg, 0:disable, [31:16]= Upper bound, [15:0] = Lower bound; Stop parser when PTS table size is more than upper bound. Resume parser when PTS table size is less than low bound.
    /*0x20325*/E_HVD_CMD_INC_DISPQ_NUM,             // Arg, none. Increase DispQ Num (SW detile case)
    /*0x20326*/E_HVD_CMD_THUMBNAIL_MODE,            // Arg, 0:disable, 1:enable. Use small frame buffer to decode thumbnail
    /*0x20327*/E_HVD_CMD_CMA_FRMBUFF_ALLOCATE_STATUS,
    /*0x20328*/E_HVD_CMD_CMA_FRMBUFF_RELEASE_STATUS,
    /*0x20329*/E_HVD_CMD_FRC_ONLY_SHOW_TOP_FIELD,   // Arg, 0:disable, 1:enable. only show top filed for FRC mode
    /*0x2032A*/E_HVD_CMD_DIRECT_STC_MODE,           // Arg, 0:disable, 1:enable. vdec fw use g_shm->u32DirectStcInMs as stc
    /*0x2032B*/E_HVD_CMD_DROP_ONLY_FIELD_FRAME,     // Arg, 0:disable, 1:enable. Drop only field frame when insert to display queue and enable drop error frame.
    /*0x2032C*/E_HVD_CMD_SET_ENABLE_HDR,            // Arg, 0:disable, 1:enable. Suspend HDR decode/transfer HDR data.
    /*0x2032D*/E_HVD_CMD_DYNAMIC_CONNECT_DISP_PATH, // Arg, bit[0]: connect/disconnect, bit[4:1]: display path, [31:5]: reserved.
    /*0x2032E*/E_HVD_CMD_AVSYNC_DISP_AUTO_DROP,     // Arg, 0:disable, 1:enable.
    /*0x2032F*/E_HVD_CMD_SET_SLOW_SYNC,                  // Arg, bits[31:16]: reserved, bits[15:8]: slow repeat frequency (0: disable slow repeat), bits[7:0]: slow drop frequency (0: disable slow drop).
    /*0x20330*/E_HVD_CMD_ENABLE_QOS_INFO,           // Arg, 0:disable, 1:enable. report qos info
    /*0x20331*/E_HVD_CMD_ADJUST_DECODER_FREQUENCY,  // HVD_Decoder_Frequency;  E_HVD_DECODER_FREQUENCY_DOWN, E_HVD_DECODER_FREQUENCY_UP, E_HVD_DECODER_FREQUENCY_MAX, E_HVD_DECODER_FREQUENCY_MIN

    // test cmd
    E_HVD_CMD_TEST_MASK = (0x0400|E_HVD_CMD_NEW_BASE),
    /*0x20401*/E_HVD_CMD_INIT_STREAM,               // Initialize this stream
    /*0x20402*/E_HVD_CMD_RELEASE_STREAM,            // Release this stream

    // HVD new cmd Max
    E_HVD_CMD_NEW_MAX = (0xFFFF|E_HVD_CMD_NEW_BASE),


    // Dual Stream Command
    E_DUAL_CMD_BASE = 0x00030000,             // pass the DRAM offset from argument

    E_DUAL_CMD_MODE_MASK = (0x0100|E_DUAL_CMD_BASE),
    /*0x30101*/E_DUAL_CMD_TASK0_HVD_TSP,
    /*0x30102*/E_DUAL_CMD_TASK0_HVD_BBU,
    /*0x30103*/E_DUAL_CMD_TASK0_MVD_TSP,
    /*0x30104*/E_DUAL_CMD_TASK0_MVD_SLQ,

    /*0x30105*/E_DUAL_CMD_TASK1_HVD_TSP,
    /*0x30106*/E_DUAL_CMD_TASK1_HVD_BBU,
    /*0x30107*/E_DUAL_CMD_TASK1_MVD_TSP,
    /*0x30108*/E_DUAL_CMD_TASK1_MVD_SLQ,

    /*0x30109*/E_DUAL_CMD_SINGLE_TASK,      //argument: 0:multi(default) 1:single // first cmd

    /*0x3010A*/E_DUAL_CMD_MODE,             //argument: 0:normal(default) 1:3D wmv 2:Korea 3D 3:Korea 3D Progressive 4:sub view sync main STC
                                            //          5:switch target STC , main view sync sub stc and sub view sync main stc //first cmd

    /*0x3010B*/E_DUAL_BURST_MODE,           //argument: 0:normal(default) 1:burst command to controller(lots of cmd)

    /*0x3010C*/E_DUAL_VERSION,              //argument: 0:controller 1:mvd fw 2:hvd fw 3:mvd interface 4:hvd interface
    /*0x3010D*/E_DUAL_R2_CMD_EXIT,          //for WIN32 testing and let R2 FW return directly.
    /*0x3010E*/E_DUAL_R2_CMD_FBADDR,        //frame buffer address
    /*0x3010F*/E_DUAL_R2_CMD_FBSIZE,        //frame buffer size
    /*0x30110*/E_DUAL_R2_CMD_FB2ADDR,       //frame buffer2 address
    /*0x30111*/E_DUAL_R2_CMD_FB2SIZE,       //frame buffer2 size
    /*0x30112*/E_DUAL_CMD_COMMON,           //argument: 0:dymanic fb management
    /*0x30113*/E_DUAL_CMD_STC_MODE,         //set STC index
    E_DUAL_CMD_CTL_MASK = (0x0200|E_DUAL_CMD_BASE), // argument is the id : 0 or 1
    /*0x30201*/E_DUAL_CMD_DEL_TASK,

    // Dual Stream cmd Max
    E_DUAL_CMD_MAX = (0xFFFF|E_DUAL_CMD_BASE),

    // N Stream Command
    E_NST_CMD_BASE = 0x00040000,  // pass the DRAM offset from argument

    E_NST_CMD_MODE_MASK = (0x0100|E_NST_CMD_BASE),
#ifdef VDEC3
    /*0x40101*/E_NST_CMD_TASK_HVD_TSP,
    /*0x40102*/E_NST_CMD_TASK_HVD_BBU,
    /*0x40103*/E_NST_CMD_TASK_MVD_TSP,
    /*0x40104*/E_NST_CMD_TASK_MVD_SLQ,
#endif

    E_NST_CMD_CTL_MASK = (0x0200|E_NST_CMD_BASE),  // argument is the id : 0 ,1 or 2
    /*0x40201*/E_NST_CMD_DEL_TASK,

    E_NST_CMD_COMMON_MASK = (0x0300|E_NST_CMD_BASE),
    /*0x40301*/E_NST_CMD_COMMON_CMD1,
    /*0x40302*/E_NST_CMD_COMMON_CMD2,

    // N Stream cmd Max
    E_NST_CMD_MAX = (0xFFFF|E_NST_CMD_BASE),

    // CMD MASK
    E_CMD_MASK = 0x00FFFFFF,

    // TASK ID MASK
    E_ID_CMD_MASK = 0xFF000000,

} HVD_User_Cmd;

// Command
typedef enum
{
    // Invalid cmd
    E_JPD_CMD_INVALID                        = 0xffffffffUL,

    E_JPD_CMD_GO                             = 0x00, // Start to show
    E_JPD_CMD_SET_FRAME_BUFF_START_ADDR      = 0x01, // Set frame buffer address
    E_JPD_CMD_SET_FRAME_BUFF_UNIT_SIZE       = 0x02, // Set frame buffer size
    E_JPD_CMD_SET_FRAME_BUFF_TOTAL_NUM       = 0x03, // Set total number of frame buffer
    E_JPD_CMD_SET_FRAME_BUFF_IDX             = 0x04, // Set frame buffer index
    E_JPD_CMD_SET_FRAME_BUFF_IDX_READY       = 0x05, // Set frame buffer index ready for display
    E_JPD_CMD_SET_WIDTH                      = 0x06, // Set frame width
    E_JPD_CMD_SET_HEIGHT                     = 0x07, // Set frame height
    E_JPD_CMD_SET_PITCH                      = 0x08, // Set pitch
    E_JPD_CMD_SET_FRAME_ID_L                 = 0x09, // Set frame ID_L
    E_JPD_CMD_SET_FRAME_ID_H                 = 0x0A, // Set frame ID_H
    E_JPD_CMD_SET_TIMESTAMP                  = 0x0B, // Set Time Stamp
    E_JPD_CMD_SET_FRAMERATE                  = 0x0C, // Set FrameRate
    E_JPD_CMD_SET_FRAMERATE_BASE             = 0x0D, // Set FrameRate Base
    E_JPD_CMD_SET_FRAME_BUFF_IDX_VALID       = 0x0E, // Set frame buffer index available
    E_JPD_CMD_SET_CHIP_ID                    = 0x0F, // Set Chip ID

    E_JPD_CMD_PLAY                           = 0x20, // Play
    E_JPD_CMD_PAUSE                          = 0x21, // Pause
    E_JPD_CMD_RESUME                         = 0x22, // Resume
    E_JPD_CMD_STEP_PLAY                      = 0x23, // Step play
    E_JPD_CMD_SET_SPEED_TYPE                 = 0x24, // Set play speed type: default, fast, slow
    E_JPD_CMD_SET_SPEED                      = 0x25, // Set play speed
    E_JPD_CMD_FLUSH_DISP_QUEUE               = 0X26, // Flush display queue
    E_JPD_CMD_FREEZE_DISP                    = 0x27, // Freeze display
    E_JPD_CMD_ENABLE_AVSYNC                  = 0x28, // Enable AV Sync
    E_JPD_CMD_SET_AVSYNC_DELAY               = 0x29, // Set AV sync delay
    E_JPD_CMD_SET_AVSYNC_TOLERENCE           = 0x2A, // Set AV sync tolerence
    E_JPD_CMD_SET_PTS_BASE                   = 0x2B, // Set PTS base
    E_JPD_CMD_SET_STC_BASE                   = 0x2C, // Set STC base
    E_JPD_CMD_SET_BLUE_SCREEN                = 0x2D, // Set Blue Screen
    E_JPD_CMD_PUSH_QUEUE_PARA_SETTING        = 0x2E,
    E_JPD_CMD_SET_DISPLAY_OUTSIDE_MODE       = 0x2F,

    E_JPD_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX   = 0x40, // Get next free frame buffer index
    E_JPD_CMD_COMPENSATE_PTS                 = 0x41, // Ask firmware to compensate PTS


#ifdef VDEC3
    E_JPD_CMD_GET_FRAME_BUFFER               = 0x50, // Ask firmware to get frame buffer
    E_JPD_CMD_FREE_FRAME_BUFFER              = 0x51, // Ask firmware to free frame buffer
#endif
    E_JPD_CMD_DYNAMIC_CONNECT_DISP_PATH      = 0x61, // Arg, bit[0]: connect/disconnect, bit[4:1]: display path, [31:5]: reserved.

    // Display Command Queue
    E_JPD_CMD_ENABLE_DISP_CMD_QUEUE          = 0x80, // Enable Display Command Queue
    E_JPD_CMD_PUSH_DISP_CMD                  = 0x81, // Push Display Command
    E_JPD_CMD_GET_DISP_CMD_Q_VACANCY         = 0x82, // Check if the display command queue full or not

    E_JPD_CMD_IS_STEP_PLAY_DONE              = 0xFF, //
    E_JPD_CMD_IS_DISP_FINISH                 = 0xFE, //
    E_JPD_CMD_IS_PLAYING                     = 0xFC, //
    E_JPD_CMD_IS_DISPLAY_QUEUE_FULL          = 0xFB, //
    E_JPD_CMD_IS_AVSYNC_ON                   = 0xFA, //
    E_JPD_CMD_IS_REACH_AVSYNC                = 0xF9, //
    E_JPD_CMD_IS_FLUSH_DONE                  = 0xF8, // Check if flush done

} JPD_User_Cmd;

// Firmware State
typedef enum
{
    E_JPD_FW_STATE_MASK = 0xF000,
} JPD_FW_State;

// Error Code
typedef enum
{
    // Error code base
    E_JPD_ERR_BASE= 0x01000000,
} JPD_Err_Code;

typedef enum
{
    E_HVD_FW_STATE_MASK = 0xF000,

    // state: INIT
    E_HVD_FW_INIT = 0x1000,
    E_HVD_FW_INIT_START,
    E_HVD_FW_INIT_DONE,

    // state: PLAY
    E_HVD_FW_PLAY = 0x2000,
    E_HVD_FW_PLAY_TYPE_MASK = 0x0C00,

    // AVC
    E_HVD_FW_PLAY_AVC = (0x0000|E_HVD_FW_PLAY),
    E_HVD_FW_AVC_READ_NAL,
    E_HVD_FW_AVC_READ_NEW_SLICE,
    E_HVD_FW_AVC_PREPARE_SLICE_HEADER,
    E_HVD_FW_AVC_DECODE_ONE_SLICE,
    E_HVD_FW_AVC_EXIT_PICTURE,

    // AVS
    E_HVD_FW_PLAY_AVS = (0x0400|E_HVD_FW_PLAY),

    // RM
    E_HVD_FW_PLAY_RM = (0x0800|E_HVD_FW_PLAY),

    // state: PAUSE
    E_HVD_FW_PAUSE = 0x3000,

    // state: STOP
    E_HVD_FW_STOP = 0x4000,
    E_HVD_FW_STOP_START,
    E_HVD_FW_STOP_DONE,
} HVD_FW_State;


typedef enum
{
    // Error code base
    E_HVD_ERR_BASE = 0x0000,

    // General
    E_HVD_ERR_GENERAL_BASE = (0x0000|E_HVD_ERR_BASE),
    E_HVD_ERR_OUT_OF_SPEC,
    E_HVD_ERR_UNKNOW_ERR,
    E_HVD_ERR_HW_BREAK_DOWN,
    // TIMEOUT
    E_HVD_ERR_HW_DEC_TIMEOUT,
    // NOT SUPPORT
    E_HVD_ERR_OUT_OF_MEMORY,        // required memory size is over frame buffer size.
    E_HVD_ERR_UNKNOWN_CODEC,        // unknown media codec
    E_HVD_ERR_CMA_FAILED,
    E_HVD_ERR_RES_NOT_SUPPORT,      // out of supported resolution

    // AVC
    E_HVD_ERR_AVC_BASE = (0x1000|E_HVD_ERR_BASE),
    // decode error
    E_HVD_ERR_AVC_SPS_BROKEN,           // SPS is not valid
    E_HVD_ERR_AVC_SPS_NOT_IN_SPEC,
    E_HVD_ERR_AVC_SPS_NOT_ENOUGH_FRM,   // DPB size at specified level is smaller than the specified number of reference frames. This is not allowed
    E_HVD_ERR_AVC_PPS_BROKEN,           // PPS is not valid
    E_HVD_ERR_AVC_REF_LIST,
    E_HVD_ERR_AVC_NO_REF,
    E_HVD_ERR_AVC_RES,                  // out of supported resolution

    // AVS
    E_HVD_ERR_AVS_BASE = (0x2000|E_HVD_ERR_BASE),
    E_HVD_ERR_AVS_RES,                  // out of supported resolution

    // RM
    E_HVD_ERR_RM_BASE = (0x3000|E_HVD_ERR_BASE),
    E_HVD_ERR_RM_PACKET_HEADER,
    E_HVD_ERR_RM_FRAME_HEADER,
    E_HVD_ERR_RM_SLICE_HEADER,
    E_HVD_ERR_RM_BYTE_CNT,
    E_HVD_ERR_RM_DISP_TIMEOUT,
    E_HVD_ERR_RM_NO_REF,
    E_HVD_ERR_RM_RES,                   // out of supported resolution
    E_HVD_ERR_RM_VLC,
    E_HVD_ERR_RM_SIZE_OUT_FB_LAYOUT,

    // VP8
    E_HVD_ERR_VP8_BASE = (0x4000|E_HVD_ERR_BASE),
    E_HVD_ERR_VP8_RES,                  // out of supported resolution

    // HEVC
    E_HVD_ERR_HEVC_BASE = (0x5000|E_HVD_ERR_BASE),
    E_HVD_ERR_HEVC_RES,                  // out of supported resolution

    // VP9
    E_HVD_ERR_VP9_BASE = (0x6000|E_HVD_ERR_BASE),
    E_HVD_ERR_VP9_RES,                  // out of supported resolution

    // Display
    E_HVD_ERR_DISPLAY_BASE = (0x7000|E_HVD_ERR_BASE),
    E_HVD_ERR_DISLPAY_MVOP_WITHOUT_MFDEC,    // [K6] MVOP and corresponding MFDEC share sram.
                                             //      MVOP cannot function if corresponding MFDEC is being used by DIP.
} HVD_Err_Code;

typedef enum
{
    E_HVD_ES_BUF_STATUS_UNKNOWN   = 0,
    E_HVD_ES_BUF_STATUS_UNDERFLOW = 1,
    E_HVD_ES_BUF_STATUS_OVERFLOW  = 2,
    E_HVD_ES_BUF_STATUS_NORMAL    = 3,

}HVD_ES_Buf_Status;

typedef enum {
    E_PVR_SEAMLESS_TIMESHIFT_NONE = 0,
    E_PVR_SEAMLESS_TIMESHIFT_PAUSE_DECODE,          // initialize timeshift record, pause decode and set target POC/pts
    E_PVR_SEAMLESS_TIMESHIFT_RESET_AND_FINE_TARGET, // resume and try to find picture with target POC/pts, drop before we find it
    E_PVR_SEAMLESS_TIMESHIFT_SEEK_TO_I              // stop finding target, decode from next I
} HVD_Seamless_Mode;

typedef enum
{
    E_HVD_FREEZE_AT_CUR_PIC = 1,
    E_HVD_FREEZE_AT_LAST_PIC = 2,
    E_HVD_FREEZE_AT_CUR_PIC_AND_CLEAR_DECODE_INFO = 3,
}HVD_Flush_Mode;

typedef enum
{
    E_HVD_SEAMLESS_PAUSE_DECODE      = BIT(0),
    E_HVD_SEAMLESS_DISPLAY_REPEATING = BIT(1),
    E_HVD_SEAMLESS_RESET_HW_DONE     = BIT(2),
    E_HVD_SEAMLESS_TARGET_FRM_FOUND  = BIT(3),
    E_HVD_SEAMLESS_DISPLAY_RESUME    = BIT(4),
}HVD_Seamless_Status;

typedef enum
{
    E_HVD_POST_PROC_NONE     = 0,
    E_HVD_POST_PROC_DETILE   = BIT(0),
} HVD_Post_Process;

typedef enum
{
    E_HVD_CHIP_U01   = 0,
    E_HVD_CHIP_U02   = 1,
} HVD_CHIP_ECO_NUM;

typedef enum
{
    // unknown sequence change info
    VDEC_SEQ_CHANGE_NONE              = 0x00,
    // sequence chagne first time
    VDEC_SEQ_CHANGE_FIRST_TIME        = BIT(0),
    // sequence chagne due to resolution
    VDEC_SEQ_CHANGE_RESOLUTION        = BIT(1),
    // sequence chagne due to picture type
    VDEC_SEQ_CHANGE_PICTURE_TYPE      = BIT(2),
    // sequence chagne due to aspect ratio
    VDEC_SEQ_CHANGE_ASPECT_RATIO      = BIT(3),
    // sequence chagne due to frame rate
    VDEC_SEQ_CHANGE_FRAME_RATE        = BIT(4),
    // sequence chagne due to HDR info
    VDEC_SEQ_CHANGE_HDR_INFO          = BIT(5),
} VDEC_SeqChangeInfo;

typedef enum
{
    // Not support profile
    E_HVD_NOT_SUPPORT_PROFILE      = BIT(0),
    // Not support SPS ID
    E_HVD_NOT_SUPPORT_SPS_ID        = BIT(1),
    // Not support chroma format
    E_HVD_NOT_SUPPORT_CHROMA_FORMAT = BIT(2),
    // Mot support max frame number
    E_HVD_NOT_SUPPORT_MAX_FRAME_NUM = BIT(3),
} HVD_NOT_SUPPORT_INFO;

typedef enum
{
  E_VDEC_TILE_NONE = 0,
  E_VDEC_TILE_16X16,
  E_VDEC_TILE_16X32,
  E_VDEC_TILE_32X16,
  E_VDEC_TILE_32X32,
  E_VDEC_TILE_MAX,
} VDEC_TILE_MODE;

#endif // _FW_HVD_IF_H_

