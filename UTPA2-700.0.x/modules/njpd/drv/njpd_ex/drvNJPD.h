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
/// @file   drvNJPD.h
/// @brief  NJPD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_NJPD_H_
#define _DRV_NJPD_H_

#include "njpeg_def.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_NJPD_LIB_CODE                     {'J','P','D','_'}                 ///<Lib code
#define MSIF_NJPD_LIBVER                       {'0','1'}                         ///<LIB version
#define MSIF_NJPD_BUILDNUM                     {'0','3'}                         ///<Build Number
#define MSIF_NJPD_CHANGELIST                   {'0','0','6','1','6','2','3','3'} ///<P4 ChangeList Number


//------------------------------------------------------------------------------
/// @brief \b NJPD_DRV_VERSION : NJPEG Version
//------------------------------------------------------------------------------
#define NJPD_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_NJPD_LIB_CODE,                  /* IP__                                             */  \
    MSIF_NJPD_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_NJPD_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_NJPD_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#if 0
#if defined (__aeon__)
#define AEON_NON_CACHE_MASK  0x80000000
#elif defined (MSOS_TYPE_LINUX)
#define AEON_NON_CACHE_MASK  0x0
#else
#define AEON_NON_CACHE_MASK  0xA0000000
#endif
#endif

#define NJPD_BIT(_bit_)      (1<<(_bit_))
#define NJPD_BITMASK(_bits_) (NJPD_BIT(((1)?_bits_)+1)-NJPD_BIT(((0)?_bits_)))
#define NJPD_OFFSET(x)       ((x)<<1)

/*****************Config Flag*********************/

// detail for reg: BK_NJPD_GLOBAL_SETTING00 (0x00)
#define NJPD_Y_HSF1                     NJPD_BIT(0)
#define NJPD_Y_HSF2                     NJPD_BIT(1)
#define NJPD_Y_VSF2                     NJPD_BIT(2)
#define NJPD_UV                         NJPD_BIT(3)
#define NJPD_SWRST                      NJPD_BIT(4)
#define NJPD_RST_EN                     NJPD_BIT(5)
#define NJPD_ROI_EN                     NJPD_BIT(6)
#define NJPD_SUVQ                       NJPD_BIT(7)
#define NJPD_SUVH                       NJPD_BIT(8)
#define NJPD_YC_SWAP                    NJPD_BIT(10)
#define NJPD_UV_SWAP                    NJPD_BIT(11)


// detail for reg: BK_NJPD_GLOBAL_SETTING01 (0x01)
#define NJPD_DOWN_SCALE                 (NJPD_BIT(0) | NJPD_BIT(1))
#define NJPD_SVLD                       NJPD_BIT(6)
#define NJPD_UV_7BIT                    NJPD_BIT(8)
#define NJPD_UV_MSB                     NJPD_BIT(9)
#define NJPD_GTABLE_RST                 NJPD_BIT(10)
#define NJPD_HTABLE_RELOAD_EN           NJPD_BIT(11)
#define NJPD_GTABLE_RELOAD_EN           NJPD_BIT(12)
#define NJPD_QTABLE_RELOAD_EN           NJPD_BIT(13)

// detail for reg: BK_NJPD_GLOBAL_SETTING02 (0x02)
#define NJPD_TBC_MODE                   NJPD_BIT(1)
#define NJPD_FFD9_EN                    NJPD_BIT(2)
#define NJPD_LITTLE_ENDIAN              NJPD_BIT(8)
#define NJPD_REMOVE_0xFF00              NJPD_BIT(9)
#define NJPD_REMOVE_0xFFFF              NJPD_BIT(10)
#define NJPD_HUFF_TABLE_ERROR           NJPD_BIT(11)
#define NJPD_HUFF_DATA_LOSS_ERROR       NJPD_BIT(12)
#define NJPD_BITSTREAM_LE               NJPD_BIT(13)
#define NJPD_SRAM_SD_EN                 NJPD_BIT(15)


// detail for reg: BK_NJPD_WRITE_ONE_CLEAR (0x08)
#define NJPD_DECODE_ENABLE              NJPD_BIT(0)
#define NJPD_TABLE_LOADING_START        NJPD_BIT(1)
#define NJPD_MIU_PARK                   NJPD_BIT(2)
#define NJPD_MRC0_VALID                 NJPD_BIT(3)
#define NJPD_MRC1_VALID                 NJPD_BIT(4)
#define NJPD_MRC_LAST                   NJPD_BIT(5)
#define NJPD_TBC_EN                     NJPD_BIT(6)
#define NJPD_TBC_DONE_CLR               NJPD_BIT(7)
#define NJPD_CLEAR_CRC                  NJPD_BIT(8)
#define NJPD_HANDSHAKE_SW_WOC           NJPD_BIT(9)


// detail for reg: BK_NJPD_MIU_READ_STATUS (0x0e)
#define NJPD_MIU_MRC0_STATUS            NJPD_BIT(0)
#define NJPD_MIU_MRC1_STATUS            NJPD_BIT(1)
#define NJPD_MIU_HTABLE_RDY             NJPD_BIT(2)
#define NJPD_MIU_GTABLE_RDY             NJPD_BIT(3)
#define NJPD_MIU_QTABLE_RDY             NJPD_BIT(4)


// detail for reg: BK_NJPD1_IBUF_READ_LENGTH (0x28)
#define NJPD_MIU_SEL_SHIFT              10
#define NJPD_MIU_SEL_1_SHIFT            11
#define NJPD_MIU_SEL                    NJPD_BIT(NJPD_MIU_SEL_SHIFT)    // this reg only supported in edison/einstein/eiffel
#define NJPD_MIU_SEL_1                  NJPD_BIT(NJPD_MIU_SEL_1_SHIFT)    // this reg only supported in einstein3

// detail for reg: BK_NJPD_IRQ_CLEAR (0x29)
//[0] Decode done event flag
//[1] Mini-code error event flag
//[2] Inverse scan error event flag
//[3] Restart marker error event flag
//[4] Restart marker index disorder error event flag
//[5] End image error event flag
//[6] Read buffer0 empty event flag
//[7] Read buffer1 empty event flag
//[8] MIU write protect event flag
//[9] Data lose error event flag
//[10] iBuf table load done flag
//[11] Huffman table error

#define NJPD_EVENT_DECODE_DONE          NJPD_BIT(0)
#define NJPD_EVENT_MINICODE_ERR         NJPD_BIT(1)
#define NJPD_EVENT_INV_SCAN_ERR         NJPD_BIT(2)
#define NJPD_EVENT_RES_MARKER_ERR       NJPD_BIT(3)
#define NJPD_EVENT_RMID_ERR             NJPD_BIT(4)
#define NJPD_EVENT_END_IMAGE_ERR        NJPD_BIT(5)
#define NJPD_EVENT_MRC0_EMPTY           NJPD_BIT(6)
#define NJPD_EVENT_MRC1_EMPTY           NJPD_BIT(7)
#define NJPD_EVENT_WRITE_PROTECT        NJPD_BIT(8)
#define NJPD_EVENT_DATA_LOSS_ERR        NJPD_BIT(9)
#define NJPD_EVENT_IBUF_LOAD_DONE       NJPD_BIT(10)
#define NJPD_EVENT_HUFF_TABLE_ERR       NJPD_BIT(11)
#define NJPD_EVENT_ALL                  (NJPD_EVENT_DECODE_DONE | NJPD_EVENT_MINICODE_ERR | \
            NJPD_EVENT_INV_SCAN_ERR | NJPD_EVENT_RES_MARKER_ERR | NJPD_EVENT_RMID_ERR |     \
            NJPD_EVENT_END_IMAGE_ERR | NJPD_EVENT_MRC0_EMPTY | NJPD_EVENT_MRC1_EMPTY |      \
            NJPD_EVENT_WRITE_PROTECT | NJPD_EVENT_DATA_LOSS_ERR |                           \
            NJPD_EVENT_IBUF_LOAD_DONE | NJPD_EVENT_HUFF_TABLE_ERR)

#define NJPD_EVENT_ERROR                  ( NJPD_EVENT_MINICODE_ERR | NJPD_EVENT_INV_SCAN_ERR | \
            NJPD_EVENT_RES_MARKER_ERR | NJPD_EVENT_RMID_ERR | NJPD_EVENT_END_IMAGE_ERR |    \
            NJPD_EVENT_DATA_LOSS_ERR | NJPD_EVENT_HUFF_TABLE_ERR)


// detail for reg: BK_NJPD_TBC (0x40)
#define NJPD_JPD_TBC_RW                 NJPD_BIT(0)
#define NJPD_JPD_TBC_SEL                (NJPD_BIT(1) | NJPD_BIT(2))
#define NJPD_JPD_TBC_TABLE_READ         NJPD_BIT(4)
#define NJPD_JPD_TBC_ADR                (NJPD_BIT(8) | NJPD_BIT(9) | NJPD_BIT(10) | NJPD_BIT(11) | NJPD_BIT(12) | NJPD_BIT(13) | NJPD_BIT(14) | NJPD_BIT(15))


// detail for reg: BK_NJPD2_MARB_SETTING_06 (0x56)
#define NJPD_JPD_MARB_MRPRIORITY_SW     (NJPD_BIT(0) | NJPD_BIT(1))

// detail for reg: BK_NJPD2_MARB_SETTING_07 (0x57)
#define NJPD_JPD_MARB_BURST_SPLIT       (NJPD_BIT(12) | NJPD_BIT(13))


// detail for reg: BK_NJPD_MARB_LBOUND_0_H (0x5b)
#define NJPD_MARB_MIU_BOUND_EN_0        NJPD_BIT(13)



// detail for reg: BK_NJPD_TOP_MARB_PORT_ENABLE (0x76)
#define NJPD_TOP_MARB_P0_ENABLE                     NJPD_BIT(0)
#define NJPD_TOP_MARB_P1_ENABLE                     NJPD_BIT(1)
#define NJPD_TOP_MARB_P2_ENABLE                     NJPD_BIT(2)
#define NJPD_TOP_MARB_P0_W_BYPASS_ENABLE            NJPD_BIT(4)
#define NJPD_TOP_MARB_P1_W_BYPASS_ENABLE            NJPD_BIT(5)
#define NJPD_TOP_MARB_P2_W_BYPASS_ENABLE            NJPD_BIT(6)
#define NJPD_TOP_MARB_P0_R_BYPASS_ENABLE            NJPD_BIT(7)
#define NJPD_TOP_MARB_P1_R_BYPASS_ENABLE            NJPD_BIT(0)     // second byte
#define NJPD_TOP_MARB_P2_R_BYPASS_ENABLE            NJPD_BIT(1)     // second byte

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_HVD_FW_INPUT_SOURCE_NONE,       ///< No input fw.
    E_HVD_FW_INPUT_SOURCE_DRAM,       ///< input source from DRAM.
    E_HVD_FW_INPUT_SOURCE_FLASH,      ///< input source from FLASH.
} HVD_FWInputSourceType;

// Chip Define
typedef enum
{
    E_MJPEG_MSTAR_CHIP_NONE = 0,
    E_MJPEG_MSTAR_CHIP_U3,
    E_MJPEG_MSTAR_CHIP_T3,
    E_MJPEG_MSTAR_CHIP_T4,
    E_MJPEG_MSTAR_CHIP_JANUS,
    E_MJPEG_MSTAR_CHIP_U4,
    E_MJPEG_MSTAR_CHIP_T8,
    E_MJPEG_MSTAR_CHIP_T9,
    E_MJPEG_MSTAR_CHIP_T13,
    E_MJPEG_MSTAR_CHIP_A1,
    E_MJPEG_MSTAR_CHIP_A6,
    E_MJPEG_MSTAR_CHIP_A7,
    E_MJPEG_MSTAR_CHIP_AMETHYST,
    E_MJPEG_MSTAR_CHIP_EAGLE,
    E_MJPEG_MSTAR_CHIP_J2,
    E_MJPEG_MSTAR_CHIP_EMERALD,
    E_MJPEG_MSTAR_CHIP_NUGGET,
    E_MJPEG_MSTAR_CHIP_NIKON,
} MJPEG_MSTAR_CHIP_ID;

typedef enum
{
    E_NJPD_EVENT_DEC_NONE    = 0x00
  , E_NJPD_EVENT_DEC_DONE    = 0x01
  , E_NJPD_EVENT_MINICODE_ERR   = 0x02
  , E_NJPD_EVENT_INV_SCAN_ERR    = 0x04
  , E_NJPD_EVENT_RES_MARKER_ERR   = 0x08
  , E_NJPD_EVENT_RMID_ERR   = 0x10

  , E_NJPD_EVENT_END_IMAGE_ERR   = 0x20
  , E_NJPD_EVENT_MRC0_EMPTY   = 0x40
  , E_NJPD_EVENT_MRC1_EMPTY   = 0x80
  , E_NJPD_EVENT_WRITE_PROTECT   = 0x100
  , E_NJPD_EVENT_DATA_LOSS_ERR   = 0x200

  , E_NJPD_EVENT_IBUF_LOAD_DONE   = 0x400
  , E_NJPD_EVENT_HUFF_TABLE_ERR   = 0x800
} NJPD_Event;


// NJPD Downscale Ratio
// Bellows are 1, 1/2, 1/4 and 1/8 in order
typedef enum
{
    E_NJPD_DOWNSCALE_ORG     = 0x00
  , E_NJPD_DOWNSCALE_HALF    = 0x01
  , E_NJPD_DOWNSCALE_FOURTH  = 0x02
  , E_NJPD_DOWNSCALE_EIGHTH  = 0x03
} NJPD_DownScale;

// Mimic fwNJPD_if.h
typedef enum
{
    // Invalid cmd
    E_MJPEG_CMD_INVALID = 0xffffffffUL,

    E_MJPEG_CMD_GO                             = 0x00, // Start to show
    E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR      = 0x01, // Set frame buffer address
    E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE       = 0x02, // Set frame buffer size
    E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM       = 0x03, // Set total number of frame buffer
    E_MJPEG_CMD_SET_FRAME_BUFF_IDX             = 0x04, // Set frame buffer index
    E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY       = 0x05, // Set frame buffer index ready for display
    E_MJPEG_CMD_SET_WIDTH                      = 0x06, // Set frame width
    E_MJPEG_CMD_SET_HEIGHT                     = 0x07, // Set frame height
    E_MJPEG_CMD_SET_PITCH                      = 0x08, // Set pitch
    E_MJPEG_CMD_SET_FRAME_ID_L                 = 0x09, // Set frame ID_L
    E_MJPEG_CMD_SET_FRAME_ID_H                 = 0x0A, // Set frame ID_H
    E_MJPEG_CMD_SET_TIMESTAMP                  = 0x0B, // Set Time Stamp
    E_MJPEG_CMD_SET_FRAMERATE                  = 0x0C, // Set FrameRate
    E_MJPEG_CMD_SET_FRAMERATE_BASE             = 0x0D, // Set FrameRate Base
    E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID       = 0x0E, // Set frame buffer index available
    E_MJPEG_CMD_SET_CHIP_ID                    = 0x0F, // Set frame buffer index available

    E_MJPEG_CMD_PLAY                           = 0x20, // Play
    E_MJPEG_CMD_PAUSE                          = 0x21, // Pause
    E_MJPEG_CMD_RESUME                         = 0x22, // Resume
    E_MJPEG_CMD_STEP_PLAY                      = 0x23, // Step play
    E_MJPEG_CMD_SET_SPEED_TYPE                 = 0x24, // Set play speed type: default, fast, slow
    E_MJPEG_CMD_SET_SPEED                      = 0x25, // Set play speed
    E_MJPEG_CMD_FLUSH_DISP_QUEUE               = 0X26, // Flush display queue
    E_MJPEG_CMD_FREEZE_DISP                    = 0x27, // Freeze display
    E_MJPEG_CMD_ENABLE_AVSYNC                  = 0x28, // Enable AV sync
    E_MJPEG_CMD_SET_AVSYNC_DELAY               = 0x29, // Set AV sync delay
    E_MJPEG_CMD_SET_AVSYNC_TOLERENCE           = 0x2A, // Set AV sync tolerence
    E_MJPEG_CMD_SET_PTS_BASE                   = 0x2B, // Set PTS base
    E_MJPEG_CMD_SET_STC_BASE                   = 0x2C, // Set STC base
    E_MJPEG_CMD_SET_BLUE_SCREEN                = 0x2D, // Set Blue Screen
    E_MJPEG_CMD_PUSH_QUEUE_PARA_SETTING        = 0x2E,
    E_MJPEG_CMD_SET_DISPLAY_OUTSIDE_MODE       = 0x2F,

    E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX   = 0x40, // Get next free frame buffer index
    E_MJPEG_CMD_COMPENSATE_PTS                 = 0x41, // Ask firmware to compensate PTS

    // Display Command Queue
    E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE          = 0x80, // Enable Display Command Queue
    E_MJPEG_CMD_PUSH_DISP_CMD                  = 0x81, // Push Display Command
    E_MJPEG_CMD_GET_DISP_CMD_Q_VACANCY         = 0x82, // Check if the display command queue full or not

    E_MJPEG_CMD_IS_STEP_PLAY_DONE              = 0xFF, //
    E_MJPEG_CMD_IS_DISP_FINISH                 = 0xFE, //
    E_MJPEG_CMD_IS_PLAYING                     = 0xFC, //
    E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL          = 0xFB, //
    E_MJPEG_CMD_IS_AVSYNC_ON                   = 0xFA, //
    E_MJPEG_CMD_IS_REACH_AVSYNC                = 0xF9, //
    E_MJPEG_CMD_IS_FLUSH_DONE                  = 0xF8, // Check if flush done

} MJPEG_User_Cmd;

// NJPD debug level enum
typedef enum
{
    E_NJPD_DEBUG_DRV_NONE = 0x0
  , E_NJPD_DEBUG_DRV_MSG  = 0x02
  , E_NJPD_DEBUG_DRV_ERR  = 0x08
} NJPD_DrvDbgLevel;

typedef enum
{
    E_NJPD_FAILED        = 0
  , E_NJPD_OK            = 1
  , E_NJPD_INVALID_PARAM = 2
} NJPD_Return;

typedef enum
{
    E_SCALING_ORG           = 0
  , E_SCALING_1_2           = 1
  , E_SCALING_1_4           = 2
  , E_SCALING_1_8           = 3
} NJPD_SCALING_DOWN_FACTOR;

/// frame information
typedef struct
{
    MS_PHY u32LumaAddr;     /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_PHY u32ChromaAddr;   /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_U32 u32TimeStamp;        /// Time stamp(DTS, PTS) of current displayed frame. Unit: ms (todo: 90khz)
    MS_U32 u32ID_L;              /// low part of ID number
    MS_U32 u32ID_H;              /// high part of ID number
    MS_U16 u16Pitch;             /// pitch
    MS_U16 u16Width;             /// width
    MS_U16 u16Height;            /// hight
    MS_U32 eFrameType;///< Frame type: I, P, B frame
} MJPEG_DRV_FrameInfo;

typedef enum
{
    E_MJPEG_DRV_STREAM_NONE = 0,
    E_MJPEG_DRV_MAIN_STREAM,
    E_MJPEG_DRV_SUB_STREAM
} MJPEG_DRV_StreamType;

typedef enum
{
    E_NJPD_NJPD1,
    E_NJPD_NJPD2,
    E_NJPD_NJPD_TOTAL,       // Do not remove this one
    E_NJPD_MJPEG_ON =0xcc,
    E_NJPD_MJPEG_OFF=0xdd,
} JPD_Number;

// NJPD Interrupt Register Function
typedef void (*NJPD_IsrFuncCb)(void);

//------------------------------------------------------------------------------
// Structure for buffer
typedef struct
{
    MS_PHY u32ThumbnailBufAddr;
    MS_U32 u32ThumbnailBufSize;
    MS_U32 u32ThumbnailBufOffset;
    MS_PHY u32MRCBufAddr; // MRC buffer address
    MS_U32 u32MRCBufSize; // MRC buffer size
    MS_U32 u32MRCBufOffset; // MRC access byte address offset in MRC buffer relative to MRC start address
    MS_PHY u32MWCBufAddr; // MWC buffer address
    MS_U16 u16MWCBufLineNum; // MWC Line number
    MS_BOOL bProgressive;
    MS_BOOL bThumbnailAccessMode;
} NJPD_BufCfg;
//------------------------------------------------------------------------------
// Structure for Grpinf of Table IndirectAccess
typedef struct
{
    MS_U8 *u8DcLumaValid;
    MS_U8 *u8DcLumaSymbol;
    MS_U16 *u16DcLumaCode;
    MS_U8 *u8DcChromaValid;
    MS_U8 *u8DcChromaSymbol;
    MS_U16 *u16DcChromaCode;
    MS_U8 *u8DcChroma2Valid;
    MS_U8 *u8DcChroma2Symbol;
    MS_U16 *u16DcChroma2Code;
    MS_U8 *u8AcLumaValid;
    MS_U8 *u8AcLumaSymbol;
    MS_U16 *u16AcLumaCode;
    MS_U8 *u8AcChromaValid;
    MS_U8 *u8AcChromaSymbol;
    MS_U16 *u16AcChromaCode;
    MS_U8 *u8AcChroma2Valid;
    MS_U8 *u8AcChroma2Symbol;
    MS_U16 *u16AcChroma2Code;
    MS_BOOL DHT;
    MS_BOOL bUVHuffman;
    MS_U8 u8DcLumaCnt;
    MS_U8 u8DcChromaCnt;
    MS_U8 u8DcChroma2Cnt;
    MS_U8 u8AcLumaCnt;
    MS_U8 u8AcChromaCnt;
    MS_U8 u8AcChroma2Cnt;
} NJPD_Grpinf;
//------------------------------------------------------------------------------
// Structure for Symidx of Table Indirect Access
typedef struct
{
    MS_U8 *u8DcLumaHuffVal;
    MS_U8 *u8DcChromaHuffVal;
    MS_U8 *u8DcChroma2HuffVal;
    MS_U8 *u8AcLumaHuffVal;
    MS_U8 *u8AcChromaHuffVal;
    MS_U8 *u8AcChroma2HuffVal;
    MS_BOOL DHT;
    MS_BOOL bUVHuffman;
} NJPD_Symidx;
//------------------------------------------------------------------------------
// Structure for Quantization Table
typedef struct
{
    MS_U16 s16Value[64];
    MS_BOOL bValid;
} NJPD_QuanTbl;
//------------------------------------------------------------------------------
// Structure for IQtbl of Table Indirect Access
typedef struct
{
    MS_U8 *u8CompQuant;
    NJPD_QuanTbl *QuantTables;
    MS_U8 u8CompNum;
    MS_BOOL DQT;
} NJPD_IQTbl;
//------------------------------------------------------------------------------
// Structure for NJPD capability
typedef struct
{
    MS_BOOL bBaseline;
    MS_BOOL bProgressive;
    MS_BOOL bMJPEG;
} NJPD_Cap;
//------------------------------------------------------------------------------
// Structure for NJPD Information
typedef struct
{
    MS_U8* pu8HalVer;
    MS_U8* pu8FwVer;
    NJPD_Cap stCap;
} NJPD_Info;
//------------------------------------------------------------------------------
// Structure for NJPD Status
typedef struct
{
    MS_U32  u32CurMRCAddr;
    MS_U16  u16CurVidx;
    MS_U16  u16CurRow;
    MS_U16  u16CurCol;
    MS_BOOL bIsBusy;
    MS_BOOL bIsrEnable;
} NJPD_Status;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//void MDrv_NJPD_Initialize(MS_U8 u8ChipType);
MS_BOOL MDrv_NJPD_InitBuf(NJPD_BufCfg in, MS_BOOL bMJPEG);
//void MDrv_NJPD_Reset(void);
//void MDrv_NJPD_SW_Pause_Reset(void);
void MDrv_NJPD_Rst(void);
//MS_U16 MDrv_NJPD_GetSWResetMask(void);
//void MDrv_NJPD_SetReadBuffer(MS_U32 u32BufAddr, MS_U32 u32BufSize);
//void MDrv_NJPD_SetMRCStartAddr(MS_U32 u32ByteOffset);
//void MDrv_NJPD_SetOutputFrameBuffer(MS_U32 u32BufAddr);
MS_U32 MDrv_NJPD_GetMWCStartAddr(void);
MS_U32 MDrv_NJPD_GetWritePtrAddr(void);
void MDrv_NJPD_SetPicDimension(MS_U16 u16Width, MS_U16 u16Height);
//MS_U16 MDrv_NJPD_ReadNJPDStatus(void);
MS_U16 MDrv_NJPD_GetEventFlag(void);
//void MDrv_NJPD_ClearNJPDStatus(MS_U16 status_bit);
void MDrv_NJPD_SetEventFlag(MS_U16 u16Value);
void MDrv_NJPD_SetROI(MS_U16 start_x, MS_U16 start_y, MS_U16 width, MS_U16 height);
//void MDrv_NJPD_SetClock(MS_U8 on);
void MDrv_NJPD_PowerOn(void);
void MDrv_NJPD_PowerOff(void);
#if 0 // unused function
MS_U32 MDrv_NJPD_ReadCurrentMRCAddr(void);
#endif
void MDrv_NJPD_SetRSTIntv(MS_U16 u16Value);
MS_U16 MDrv_NJPD_GetCurVidx(void);

void MDrv_NJPD_WriteGrpinf(NJPD_Grpinf in);
void MDrv_NJPD_WriteSymidx(NJPD_Symidx in);
void MDrv_NJPD_WriteIQTbl(NJPD_IQTbl in);

void MDrv_NJPD_SetDbgLevel(MS_U8 u8DbgLevel);
void MDrv_NJPD_GetInfo(NJPD_Info *pNJPD_Info);
NJPD_Status* MDrv_NJPD_GetStatus(void);
MS_BOOL MDrv_NJPD_GetLibVer(const MSIF_Version **ppVersion);

MS_BOOL MDrv_NJPD_EnableISR(NJPD_IsrFuncCb IsrCb);
MS_BOOL MDrv_NJPD_DisableISR(void);

MS_U32 MDrv_NJPD_GetCurMRCAddr(void);
MS_U16 MDrv_NJPD_GetCurRow(void);
MS_U16 MDrv_NJPD_GetCurCol(void);

void MDrv_NJPD_SetAutoProtect(MS_BOOL enable);
void MDrv_NJPD_SetWPENEndAddr(MS_PHY u32ByteOffset);
void MDrv_NJPD_SetWPENStartAddr(MS_PHY u32ByteOffset);

void MDrv_NJPD_SetSpare(MS_U16 u16Value);
MS_U16 MDrv_NJPD_GetSpare(void);
void MDrv_NJPD_SetMRC_Valid(MS_U16 u16Value);

void MDrv_NJPD_DecodeEnable(void);
void MDrv_NJPD_TableLoadingStart(void);
void MDrv_NJPD_ReadLastBuffer(void);
void MDrv_NJPD_SetScalingDownFactor(NJPD_SCALING_DOWN_FACTOR eScalingFactor);

void MDrv_NJPD_GTable_Rst(MS_BOOL bEnable);
void MDrv_NJPD_HTable_Reload_Enable(MS_BOOL bEnable);
void MDrv_NJPD_GTable_Reload_Enable(MS_BOOL bEnable);
void MDrv_NJPD_QTable_Reload_Enable(MS_BOOL bEnable);
void MDrv_NJPD_SetSoftwareVLD(MS_BOOL bEnable);
void MDrv_NJPD_SetDifferentQTable(MS_BOOL bEnable);
void MDrv_NJPD_SetDifferentHTable(MS_BOOL bEnable);
void MDrv_NJPD_Set_GlobalSetting00(MS_U16 u16Value);
MS_U16 MDrv_NJPD_Get_GlobalSetting00(void);
void MDrv_NJPD_SetHTableStartAddr(MS_PHY u32ByteOffset);
void MDrv_NJPD_SetQTableStartAddr(MS_PHY u32ByteOffset);
void MDrv_NJPD_SetGTableStartAddr(MS_PHY u32ByteOffset);
void MDrv_NJPD_SetVerificationMode(NJPD_VerificationMode VerificationMode);
NJPD_VerificationMode MDrv_NJPD_GetVerificationMode(void);
void MDrv_NJPD_Debug(void);
void MDrv_NJPD_EnablePowerSaving(void);
void MDrv_NJPD_SetNJPDInstance(JPD_Number JPDNum);
MS_BOOL MDrv_NJPD_IsNeedToPatch(NJPD_PATCH_INDEX eIndex);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_NJPD_Get_ISR(void);
void MDrv_NJPD_Set_ISR(MS_BOOL val);
#endif
MS_U8 MDrv_NJPD_JPDCount(void);


#ifdef __cplusplus
}
#endif

#endif // _DRV_NJPD_H_

