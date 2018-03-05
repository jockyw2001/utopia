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

#ifndef _FW_JPD_IF_H_
#define _FW_JPD_IF_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------
#define JPD_DISP_QUEUE_MAX_SIZE         32

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#if (!defined( _MS_TYPES_H_))
/// data type unsigned char, data length 1 byte
typedef unsigned char               MS_U8;                              // 1 byte
/// data type unsigned short, data length 2 byte
typedef unsigned short              MS_U16;                             // 2 bytes
/// data type unsigned int, data length 4 byte
typedef unsigned long               MS_U32;                             // 4 bytes
/// data type signed char, data length 1 byte
typedef signed char                 MS_S8;                              // 1 byte
/// data type signed short, data length 2 byte
typedef signed short                MS_S16;                             // 2 bytes
/// data type signed int, data length 4 byte
typedef signed long                 MS_S32;                             // 4 bytes
#endif


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_JPD_DISPQ_STATUS_NONE = 0,            //FW
    E_JPD_DISPQ_STATUS_INIT,                //FW
    E_JPD_DISPQ_STATUS_VIEW,                //HK
    E_JPD_DISPQ_STATUS_DISP,                //HK
    E_JPD_DISPQ_STATUS_FREE,                //HK
} JPD_DISPQ_STATUS;

/// frame information
typedef struct
{
    /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_U32 u32LumaAddr;
    /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_U32 u32ChromaAddr;
    /// Time stamp(DTS, PTS) of current displayed frame. Unit: ms (todo: 90khz)
    MS_U32 u32TimeStamp;
    /// low part of ID number
    MS_U32 u32ID_L;
    /// high part of ID number
    MS_U32 u32ID_H;
    /// pitch
    MS_U16 u16Pitch;
    /// width
    MS_U16 u16Width;
    /// hight
    MS_U16 u16Height;
    ///< Frame type: I, P, B frame
    MS_U8 u8FrameType;
    /// 4-byte alignment
    MS_U8 u8Reserved;
    ///< 0:None, 1:Init, 2:View, 3:Disp, 4:Free
    MS_U32 u32Status;
} MJPEG_FrameInfo;

// Share Memory
typedef struct
{
    // HK -> FW
//    MS_U32 u32FrameBufAddr;
//    MS_U32 u32FrameBufSize;

    // FW -> HK
    MS_U32 u32FWVersionID;              // FW version ID

    //
//    MS_U32 u32DispBufAddr;
    MJPEG_FrameInfo infoDispFrame;      // Information of currently displayed frame


    // PTS table
//    MS_U32 u32TimeStamp;            // current PTS
//    MS_U32 u32PTStableWptrAddr;     // The address of PTS table write pointer.
//    MS_U32 u32PTStableRptrAddr;     // The address of PTS table read pointer.

    MS_U32 u16DispQSize;
    MS_U16 u16DispQNumb;
    MS_U16 u16DispQPtr;
    MJPEG_FrameInfo DispQueue[JPD_DISP_QUEUE_MAX_SIZE];
} JPD_ShareMem;

// PTS table entry
typedef struct
{
    MS_U32 u32ByteCnt;
    MS_U32 u32PTS;
    MS_U32 u32ID_L;
    MS_U32 u32ID_H;
} JPD_PTS_Entry;

// Command
typedef enum
{
    // Invalid cmd
    E_JPD_CMD_INVALID = 0xffffffffUL,

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

    E_JPD_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX   = 0x40, // Get next free frame buffer index
    E_JPD_CMD_COMPENSATE_PTS                 = 0x41, // Ask firmware to compensate PTS

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


//#define HVD_PTS_TABLE_ST_OFFSET     0x50000
#define JPD_PTS_TABLE_ST_OFFSET     0x70000
//#define HVD_SHARE_MEM_ST_OFFSET     0x79000
#define JPD_SHARE_MEM_ST_OFFSET     0x80000
#define JPD_DEBUG_MEM_ST_OFFSET     0x81000

#define JPD_PTS_TABLE_ENTRY_NUM     8

#endif // _FW_JPD_IF_H_

