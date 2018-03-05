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
/// @file   drvMJPEG.h
/// @brief  MJPEG Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MJPEG_H_
#define _DRV_MJPEG_H_

//#include "njpeg_def.h"


#ifdef __cplusplus
extern "C"
{
#endif

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

#ifdef VDEC3
    E_MJPEG_CMD_GET_FRAME_BUFFER               = 0x50, // Ask firmware to get frame buffer
    E_MJPEG_CMD_FREE_FRAME_BUFFER              = 0x51, // Ask firmware to free frame buffer
#endif
    E_MJPEG_CMD_DYNAMIC_CONNECT_DISP_PATH      = 0x61, // Arg, bit[0]: connect/disconnect, bit[4:1]: display path, [31:5]: reserved

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

// MJPEG debug level enum
typedef enum
{
    E_MJPEG_DEBUG_DRV_NONE = 0x0
  , E_MJPEG_DEBUG_DRV_MSG  = 0x02
  , E_MJPEG_DEBUG_DRV_ERR  = 0x08
} MJPEG_DrvDbgLevel;

typedef enum
{
    E_MJPEG_FAILED        = 0
  , E_MJPEG_OK            = 1
  , E_MJPEG_INVALID_PARAM = 2
} MJPEG_Return;

/// frame information
typedef struct
{
    MS_PHYADDR u32LumaAddr;     /// frame buffer base + the start offset of current displayed luma data. Unit: byte.
    MS_PHYADDR u32ChromaAddr;   /// frame buffer base + the start offset of current displayed chroma data. Unit: byte.
    MS_U32 u32TimeStamp;        /// Time stamp(DTS, PTS) of current displayed frame. Unit: ms (todo: 90khz)
    MS_U32 u32ID_L;              /// low part of ID number
    MS_U32 u32ID_H;              /// high part of ID number
    MS_U16 u16Pitch;             /// pitch
    MS_U16 u16Width;             /// width
    MS_U16 u16Height;            /// hight
    MS_U32 eFrameType;///< Frame type: I, P, B frame
} MJPEG_DRV_FrameInfo;

typedef struct
{
    MS_U32 u32Size;
    MS_PHY u32Addr;
    MS_U32 u32BinSize;
    MS_PHY u32BinAddr;
    MS_U8 u8FwSrcType;
    MS_PHY u32MWCAddr;
    MS_U32 u32MWCSize;
#ifdef VDEC3
    MS_BOOL bFWdecideFB;
#endif
} MJPEG_FW_Info;

typedef enum
{
    E_MJPEG_DRV_STREAM_NONE = 0,
    E_MJPEG_DRV_MAIN_STREAM,
    E_MJPEG_DRV_SUB_STREAM,
#ifdef VDEC3
    E_MJPEG_DRV_N_STREAM,
#endif
} MJPEG_DRV_StreamType;


MS_BOOL MDrv_MJPEG_Init_Share_Mem(void);
void MDrv_MJPEG_SetFrameBuffStAddr(MS_U8 u8DrvId, MS_VIRT u32Value);
MS_VIRT MDrv_MJPEG_GetFrameBuffStAddr(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameBuffTotalSize(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameBuffTotalSize(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameBuffUnitSize(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameBuffUnitSize(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameBuffTotalNum(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameBuffTotalNum(MS_U8 u8DrvId);
void MDrv_MJPEG_SetWidth(MS_U8 u8DrvId, MS_U16 u16Value);
MS_U16 MDrv_MJPEG_GetWidth(MS_U8 u8DrvId);
void MDrv_MJPEG_SetHeight(MS_U8 u8DrvId, MS_U16 u16Value);
MS_U16 MDrv_MJPEG_GetHeight(MS_U8 u8DrvId);
void MDrv_MJPEG_SetPitch(MS_U8 u8DrvId, MS_U16 u16Value);
MS_U16 MDrv_MJPEG_GetPitch(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameCnt(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameCnt(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameRate(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameRate(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameRateBase(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameRateBase(MS_U8 u8DrvId);
void MDrv_MJPEG_SetFrameTime(MS_U8 u8DrvId, MS_U32 u32Value);
MS_U32 MDrv_MJPEG_GetFrameTime(MS_U8 u8DrvId);

void MDrv_MJPEG_SetDbgLevel(MS_U8 u8DbgLevel);

/* =============================== MJPEG =====================================*/
MS_U8 MDrv_MJPEG_GetDrvId(MS_U32 u32Id);
void MDrv_MJPEG_StopVPU(void);
//MS_BOOL MDrv_MJPEG_RstVPU(void);
void MDrv_MJPEG_ReleaseVPU(void);
//void MDrv_MJPEG_ConfigVPU(MS_PHYADDR u32StAddr, MS_U8 u8dlend_en);
MS_BOOL MDrv_MJPEG_IsFirmwareReady(void);

#if 0
MS_BOOL MDrv_MJPEG_SetVPUCmd(MS_U32 u32Cmd);
MS_BOOL MDrv_MJPEG_SetVPUCmdArg(MS_U32 u32Arg);
MS_U32 MDrv_MJPEG_CheckVPUStatus(MS_U32 u32Cmd);
MS_U32 MDrv_MJPEG_GetNextFreeFrameBuffIdx(void);
#endif
MS_U32 MDrv_MJPEG_SendVPUCommand(MS_U32 u32Id, MJPEG_User_Cmd eCmd, MS_VIRT u32Arg);

//MS_BOOL MDrv_MJPEG_IsVPUOnMiu1(void);
//MS_U32 MDrv_MJPEG_GetMiuBase(void);
MS_VIRT MDrv_MJPEG_GetShareMemAddr(MS_U32 u32Id);
void MDrv_MJPEG_ClearShareMemory(MS_U32 u32Id);
MS_VIRT MDrv_MJPEG_GetPtsTableAddr(MS_U32 u32Id);

MS_BOOL MDrv_MJPEG_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase);
MS_U32 MDrv_MJPEG_GetPTS(MS_U32 u32Id);
void MDrv_MJPEG_GetDispFrameInfo(MS_U32 u32Id, void *pInfo);

void MDrv_MJPEG_DispFrame(MS_U32 u32Id, MS_U32 u32DispIdx);
void MDrv_MJPEG_FreeFrame(MS_U32 u32Id, MS_U32 u32DispIdx);
void* MDrv_MJPEG_GetNextDispFrame(MS_U32 u32Id);
MS_U32 MDrv_MJPEG_GetDispFrameNum(MS_U32 u32Id);
MS_U32 MDrv_MJPEG_GetNextDispQPtr(MS_U32 u32Id);

void MDrv_MJPEG_EnableTurboFWMode(MS_BOOL bEnable);

MJPEG_Return MDrv_MJPEG_InitFw(MS_U32 u32Id, MJPEG_FW_Info FWInfo);
MJPEG_MSTAR_CHIP_ID MDrv_MJPEG_Get_Chip_ID(void);

// for Dual Decode
MJPEG_Return MDrv_MJPEG_GetFreeStream(MS_U32 *pu32Id, MJPEG_DRV_StreamType eStreamType);
MJPEG_Return MDrv_MJPEG_DeInit(MS_U32 u32Id);
void MDrv_MJPEG_InitRegBase(MS_VIRT u32RegBase);
void MDrv_MJPEG_SetPushQPara(MS_U32 u32Id, MS_U32 u32TimeStamp, MS_U32 u32ID_L, MS_U32 u32ID_H, MS_U32 u32NextFrameBuffIdx);
void MDrv_MJPEG_SetStepDispPara(MS_U32 u32Id);
void MDrv_MJPEG_SetMIUInfo(MS_U32 u32Id, MS_U8 u8MIU);
void MDrv_MJPEG_PrintShareInfo(MS_U32 u32Id);
MS_U32 MDrv_MJPEG_GetNextFrameBufferIdx(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsStepPlayDone(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsPlaying(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsAVSyncOn(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsReachAVSync(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsFlushDone(MS_U32 u32Id);
void MDrv_MJPEG_InitFlush(MS_U32 u32Id);
void MDrv_MJPEG_Init_IsDispFinish(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsDispFinish(MS_U32 u32Id);
void MDrv_MJPEG_Init_IsQueueFull(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsQueueFull(MS_U32 u32Id);
void MDrv_MJPEG_SetConnectStatus(MS_U32 u32Id, MS_U8 u8Status);
MS_U8 MDrv_MJPEG_GetConnectStatus(MS_U32 u32Id);
#ifdef VDEC3
void MDrv_MJPEG_Init_GetFrameBuffer(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_GetFrameBuffer(MS_U32 u32Id);
MS_BOOL MDrv_MJPEG_IsFreeFinish(MS_U32 u32Id);
#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
void MDrv_MJPEG_GetCMAInformation(MS_U32 u32Id, MS_PHY* u32Addr,MS_U32* u32Size,MS_BOOL* bAllocDone);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_MJPEG_H_

