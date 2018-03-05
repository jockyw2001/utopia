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
/// file    apiMJPEG.c
/// @brief  MJPEG API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
// JPEG
#include "drvMJPEG.h"
#include "apiJPEG.h"
#include "apiMJPEG_EX.h"
// RIU Base
#include "drvMMIO.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

#define ENABLE_MJPEG_FIRMWARE           1

#define ENABLE_AVSYNC_ID_CHECK          0

#define ENABLE_DOUBLE_FRAMERATE         0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define JPEGBASEWIDTHMAX    1920//1600
#define JPEGBASEHEIGHTMAX   1080//1200
#define JPEGPROWIDTHMAX     1024
#define JPEGPROHEIGHTMAX    768

#define MJPEG_SD_WIDTH      640
#define MJPEG_SD_HEIGHT     480

#define MJPEG_HD_800x600_WIDTH      800
#define MJPEG_HD_800x600_HEIGHT     600

#define MJPEG_MAX_SUPPORT_WIDTH     1368
#define MJPEG_MAX_SUPPORT_HEIGHT     768

#define MAX_SUPPORTED_FRAME_RATE_720P   30000

#define DISPLAY_QUEUE_NUM           8

#define SUPPORT_ASF_MJPEG_MODE                      TRUE
#define MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE       (0x400000*2)
#define VDEC_EX_MVD_PIC_START_FLAG                  0x40000000

#define SUPPORT_SKIP_ERROR_FRAME                    TRUE
#define SKIP_ERROR_FRAME_THRESHOLD                  60
#define printf_red(args...)           do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size) memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define JPEG_memset(pDstAddr, u8value, u32Size)  memset((pDstAddr), (u8value), (u32Size))

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define _MJPEG_Memcpy(pDstAddr, pSrcAddr, u32Size)               \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        volatile MS_U8 *src = (volatile MS_U8 *)(pSrcAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = src[i];                                    \
        }                                                       \
    } while (0)

#if SUPPORT_ASF_MJPEG_MODE
typedef enum
{
    E_MJPEG_BUF_AVAILABLE   = 0
  , E_MJPEG_BUF_USING     = 1
  , E_MJPEG_BUF_BUSY     = 2
} MJPEG_COLLECT_BUF_STATUS;
#endif

#if ENABLE_INC_BUILD

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL bEnableDispCmdQ[E_MJPEG_MJPEG_TOTAL];

//static MS_U32 __u32FwAddr = 0xFFFFFFFF;           // Address to load firmware
static MS_U32 __u32MRCBufAddr[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32MWCBufAddr[E_MJPEG_MJPEG_TOTAL];
#if SUPPORT_ASF_MJPEG_MODE
static MS_U32 __u32CollectBufAddr[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bAsfMjpegMode[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bAsfFirstFrame[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bAsfFirstPacketInFrame[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32CollectBufWptr[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32PrevBufAddr[E_MJPEG_MJPEG_TOTAL];
static MJPEG_COLLECT_BUF_STATUS __eColBufStatus[E_MJPEG_MJPEG_TOTAL][2];
#endif

static MS_U32 __u32InternalBufAddr[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32InternalBufSize[E_MJPEG_MJPEG_TOTAL];

static MS_U32 __u32ReadPtr[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32WritePtr[E_MJPEG_MJPEG_TOTAL];

static MS_U32 __u32FrameCnt[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32FrameTime[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32FrameRate[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32FrameRateBase[E_MJPEG_MJPEG_TOTAL];

static MS_U16 __u16Width[E_MJPEG_MJPEG_TOTAL];
static MS_U16 __u16Height[E_MJPEG_MJPEG_TOTAL];
static MS_U16 __u16Pitch[E_MJPEG_MJPEG_TOTAL];

static MS_U8 __u8FrameBuffTotalNum[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32FrameBuffUnitSize[E_MJPEG_MJPEG_TOTAL];

static MS_BOOL __bIsFirstFrameRdy[E_MJPEG_MJPEG_TOTAL];

static MS_U16 PreVIdx[E_MJPEG_MJPEG_TOTAL];
static MS_U16 ReCheckTime[E_MJPEG_MJPEG_TOTAL];

static MS_BOOL __bIsDecodeDone[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32DropFrameCnt[E_MJPEG_MJPEG_TOTAL];
static MS_U32 u32NextFrameBuffIdx[E_MJPEG_MJPEG_TOTAL];
static MS_U32 u32EndAddr[E_MJPEG_MJPEG_TOTAL];

// Speed Control
static MJPEG_SpeedType eCurSpeedType[E_MJPEG_MJPEG_TOTAL];
static MJPEG_DispSpeed eCurDispSpeed[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32SpeedCtrlFrameCntBase[E_MJPEG_MJPEG_TOTAL];

static MS_U8 _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;


MJPEG_FrameInfo infoDecFrame[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL isDecFrameInfoValid[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL bStepDec[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL bLstFrame[E_MJPEG_MJPEG_TOTAL];

static MS_BOOL __bSkipToPts[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bSeekToPts[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32TargetPts[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32Skip2PtsBase[E_MJPEG_MJPEG_TOTAL];

static MJPEG_ErrorCode eMJPEGErrCode[E_MJPEG_MJPEG_TOTAL];

static MS_U16 u16NullCnt[E_MJPEG_MJPEG_TOTAL];

#if SUPPORT_SKIP_ERROR_FRAME
static MS_U32 _u32SkipCnt[E_MJPEG_MJPEG_TOTAL];
static MS_U32 _u32ErrCnt[E_MJPEG_MJPEG_TOTAL];
static MS_U32 _u32StartDecodeTime[E_MJPEG_MJPEG_TOTAL];
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifndef ANDROID
#define MJPEG_DEBUG(format, args...)          do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_MJPEG) printf(format, ##args);}while(0)
#define MJPEG_DEBUG_API_MSG(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_API) printf(format, ##args);}while(0)
#define MJPEG_DEBUG_API_ERR(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) printf(format, ##args);}while(0)
#else
#define LOG_TAG "API_JPEG"
#include <cutils/log.h>
#ifndef LOGD
#define MJPEG_DEBUG(format, args...)          do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_MJPEG) ALOGI(format, ##args);}while(0)
#define MJPEG_DEBUG_API_MSG(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_API) ALOGI(format, ##args);}while(0)
#define MJPEG_DEBUG_API_ERR(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) ALOGE(format, ##args);}while(0)
#else
#define MJPEG_DEBUG(format, args...)          do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_MJPEG) LOGI(format, ##args);}while(0)
#define MJPEG_DEBUG_API_MSG(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_API) LOGI(format, ##args);}while(0)
#define MJPEG_DEBUG_API_ERR(format, args...)  do{if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_ERR) LOGE(format, ##args);}while(0)
#endif

#endif



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

void _MApi_MJPEG_InitPara(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    MJPEG_DEBUG("[MJPEG]_MApi_MJPEG_InitPara()!!!\n");

    bEnableDispCmdQ[u8DrvId] = TRUE;

    __u32MRCBufAddr[u8DrvId] = 0;
    __u32MWCBufAddr[u8DrvId] = 0;
#if SUPPORT_ASF_MJPEG_MODE
    __u32CollectBufAddr[u8DrvId] = 0;
    __u32CollectBufWptr[u8DrvId] = 0;
    __u32PrevBufAddr[u8DrvId] = 0;
    __bAsfMjpegMode[u8DrvId] = 0;
    __bAsfFirstFrame[u8DrvId] = 0;
    __bAsfFirstPacketInFrame[u8DrvId] = 0;
    __eColBufStatus[u8DrvId][0] = E_MJPEG_BUF_AVAILABLE;
    __eColBufStatus[u8DrvId][1] = E_MJPEG_BUF_AVAILABLE;
#endif

    __u32InternalBufAddr[u8DrvId] = 0;
    __u32InternalBufSize[u8DrvId] = 0;

    __u32ReadPtr[u8DrvId] = 0;
    __u32WritePtr[u8DrvId] = 0;

    __u32FrameCnt[u8DrvId] = 0;
    __u32FrameTime[u8DrvId] = 0;
    __u32FrameRate[u8DrvId] = 600;
    __u32FrameRateBase[u8DrvId] = 20;

    __u16Width[u8DrvId] = 0;
    __u16Height[u8DrvId] = 0;
    __u16Pitch[u8DrvId] = 0;

    __u8FrameBuffTotalNum[u8DrvId] = 0;
    __u32FrameBuffUnitSize[u8DrvId] = 0;

    __bIsFirstFrameRdy[u8DrvId] = FALSE;

    PreVIdx[u8DrvId] = 0;
    ReCheckTime[u8DrvId] = 0;

    __bIsDecodeDone[u8DrvId] = FALSE;
    __u32DropFrameCnt[u8DrvId] = 0;
    u32NextFrameBuffIdx[u8DrvId] = 0;
    u32EndAddr[u8DrvId] = 0;

// Speed Control
    eCurSpeedType[u8DrvId] = E_MJPEG_SPEED_DEFAULT;
    eCurDispSpeed[u8DrvId] = E_MJPEG_DISP_SPEED_1X;
    __u32SpeedCtrlFrameCntBase[u8DrvId] = 0;


    isDecFrameInfoValid[u8DrvId] = FALSE;
    bStepDec[u8DrvId] = FALSE;
    bLstFrame[u8DrvId] = FALSE;

    __bSkipToPts[u8DrvId] = FALSE;
    __bSeekToPts[u8DrvId] = FALSE;
    __u32TargetPts[u8DrvId] = 0;
    __u32Skip2PtsBase[u8DrvId] = 0;

    u16NullCnt[u8DrvId] = 0;

#if SUPPORT_SKIP_ERROR_FRAME
    _u32SkipCnt[u8DrvId] = 0;
    _u32ErrCnt[u8DrvId] = 0;
    _u32StartDecodeTime[u8DrvId] = 0;
#endif


}

void _MApi_MJPEG_SetErrCode(MS_U32 u32Id, MJPEG_ErrorCode eErrorCode)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    MJPEG_DEBUG_API_MSG("_MApi_MJPEG_SetErrCode() with eMJPEGErrCode=0x%x\n", eErrorCode);
    eMJPEGErrCode[u8DrvId] = eErrorCode;
}

/******************************************************************************/
///Check if JPEG decode done
///@return status
/******************************************************************************/
static MJPEG_Result __MApi_MJPEG_CheckDecodeDone(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);


    if (__bIsFirstFrameRdy[u8DrvId])
    {
        JPEG_Event reg_val = MApi_JPEG_GetJPDEventFlag();
        MJPEG_DEBUG_API_MSG("        EventFlag = %d\n", reg_val);

        if(MApi_JPEG_IsNJPD()==FALSE)
        {
            MS_U16 cur_vidx = 0;
            //For H/W bug, some cases can not exit after decode done. Check Vidx to exit.
            if(PreVIdx[u8DrvId] != (cur_vidx = MApi_JPEG_GetCurVidx()))
            {
                PreVIdx[u8DrvId] = cur_vidx;
                ReCheckTime[u8DrvId] = 0;
            }
            else
            {
                ReCheckTime[u8DrvId]++;
                if(ReCheckTime[u8DrvId] >= 1000)
                {
                    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
                    MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx[u8DrvId]);
                    __bIsDecodeDone[u8DrvId] = TRUE;
                    __u32DropFrameCnt[u8DrvId]++;

                    MJPEG_DEBUG_API_ERR("Decode timeout!!!!\n");
                    reg_val = E_JPEG_EVENT_DEC_ERROR_MASK;
                    return E_MJPEG_RET_FAIL;
                }
            }
        }

        if(E_JPEG_EVENT_DEC_DONE & reg_val)
        {
            ReCheckTime[u8DrvId] = 0;

            MJPEG_DEBUG_API_MSG("        Decode Done -- ");
#if ENABLE_MJPEG_FIRMWARE
            if (__bIsDecodeDone[u8DrvId] == FALSE)
            {
                if (__u32DropFrameCnt[u8DrvId] != 0)
                {
                    // Ask firmware to compensate PTS
                    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_COMPENSATE_PTS, __u32DropFrameCnt[u8DrvId]);
                    MJPEG_DEBUG_API_MSG("Compensate PTS for %ld dropped frames", __u32DropFrameCnt[u8DrvId]);
                }
                __u32DropFrameCnt[u8DrvId] = 0;

                MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY, u32NextFrameBuffIdx[u8DrvId]);
                MJPEG_DEBUG("[MJPEG]Frame Buffer Index %ld", u32NextFrameBuffIdx[u8DrvId]);
                __bIsDecodeDone[u8DrvId] = TRUE;
            }
#endif
            MJPEG_DEBUG_API_MSG("\n");
            //__u32FrameCnt++;
            //__u32ReadPtr = u32EndAddr;
            MJPEG_DEBUG_API_MSG("Count %ld\n", __u32FrameCnt[u8DrvId]);
        }
        else
        if(E_JPEG_EVENT_DEC_ERROR_MASK & reg_val)
        {
            MJPEG_DEBUG_API_ERR("   JPD Decode Error\n");
            //MApi_MJPEG_Debug();

            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            return E_MJPEG_RET_DECODE_ERROR;
        }
        else
        {
            MJPEG_DEBUG_API_MSG("   Still decoding previous JPEG ......\n");
#if SUPPORT_SKIP_ERROR_FRAME
            if(MsOS_GetSystemTime() - _u32StartDecodeTime[u8DrvId] > SKIP_ERROR_FRAME_THRESHOLD)
            {
               MJPEG_DEBUG_API_ERR("%s: JPEG decode frame timeout!!!!!!!!!!\n", __FUNCTION__);
                _u32SkipCnt[u8DrvId]++;
                _u32ErrCnt[u8DrvId]++;
                // Repeat next frame because previous frame decode timeout
                //MApi_MJPEG_Debug();
                MJPEG_DEBUG_API_MSG("%s: Reset JPD!!!!!!!!!!\n", __FUNCTION__);
                MApi_JPEG_Rst();
                MApi_JPEG_Exit();
                return E_MJPEG_RET_DECODE_ERROR;
            }
#endif
            return E_MJPEG_RET_DECODING;
        }
    }
    else
    {
        return E_MJPEG_RET_FAIL;
    }


    return E_MJPEG_RET_SUCCESS;
}

#if SUPPORT_ASF_MJPEG_MODE

static void _MApi_MJPEG_ASFParameterSetting(MS_U32 u32Id, MJPEG_Packet_Info *pInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(__bAsfMjpegMode[u8DrvId]==FALSE)
    {
        if(pInfo->u32StAddr & VDEC_EX_MVD_PIC_START_FLAG)
        {
            MJPEG_DEBUG_API_MSG("[ASF_MJPEG]Set to ASF-MJPEG mode!!!!!!!!!!!!!!!!\n");
            __bAsfMjpegMode[u8DrvId] = TRUE;
            __bAsfFirstFrame[u8DrvId] = TRUE;
            __bAsfFirstPacketInFrame[u8DrvId] = TRUE;
        }
    }
    else
    {
        __bAsfFirstFrame[u8DrvId] = FALSE;

        if(pInfo->u32StAddr & VDEC_EX_MVD_PIC_START_FLAG)
        {
            __bAsfFirstPacketInFrame[u8DrvId] = TRUE;
        }
        else
        {
            __bAsfFirstPacketInFrame[u8DrvId] = FALSE;
        }
    }
}

static MS_BOOL _MApi_MJPEG_IsColWPtrInLow(MS_U8 u8DrvId)
{
    if(__u32CollectBufWptr[u8DrvId] >= __u32CollectBufAddr[u8DrvId] + MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE/2)
        return FALSE;
    else
        return TRUE;
}

static MS_BOOL _MApi_MJPEG_IsAbleToPushNextFrame(MS_U8 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    MJPEG_DEBUG_API_MSG("is able to push?.....%x, %x, %lx, %lx, %lx\n", __eColBufStatus[u8DrvId][0], __eColBufStatus[u8DrvId][1],
        __u32CollectBufWptr[u8DrvId],__u32CollectBufAddr[u8DrvId],
        __u32CollectBufAddr[u8DrvId] + MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE/2);
    if(__MApi_MJPEG_CheckDecodeDone(u32Id) != E_MJPEG_RET_DECODING)
    {
        if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId) ==TRUE
            && __eColBufStatus[u8DrvId][1]==E_MJPEG_BUF_BUSY)
        {
            __eColBufStatus[u8DrvId][1] = E_MJPEG_BUF_AVAILABLE;
        }
        else if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)==FALSE
            && __eColBufStatus[u8DrvId][0]==E_MJPEG_BUF_BUSY)
        {
            __eColBufStatus[u8DrvId][0] = E_MJPEG_BUF_AVAILABLE;
        }
    }
    MJPEG_DEBUG_API_MSG("is able to push?.....[%x, %x]\n", __eColBufStatus[u8DrvId][0], __eColBufStatus[u8DrvId][1]);

    if(__eColBufStatus[u8DrvId][0] != E_MJPEG_BUF_AVAILABLE
        && __eColBufStatus[u8DrvId][1] != E_MJPEG_BUF_AVAILABLE)
        return FALSE;
    else
        return TRUE;
}

static void _MApi_MJPEG_PushToLowBuffer(MS_U8 u32Id, MS_U32 u32SrcAddrVA, MS_U32 u32Size)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId) == FALSE)
    {
        __u32PrevBufAddr[u8DrvId] = __u32CollectBufWptr[u8DrvId];
        MJPEG_DEBUG_API_MSG("[ASF_MJPEG]_MApi_MJPEG_PushToLowBuffer...............start\n");
        // Step 1: reset write ptr to start of low buffer
        __u32CollectBufWptr[u8DrvId] = __u32CollectBufAddr[u8DrvId];
        // Step 2: memcpy
        JPEG_memcpy((void *)MsOS_PA2KSEG1(__u32CollectBufWptr[u8DrvId]), (void *)u32SrcAddrVA, u32Size);
        // Step 3: update write ptr address
        __u32CollectBufWptr[u8DrvId] = __u32CollectBufWptr[u8DrvId] + u32Size;
    }
    else
    {
        MJPEG_DEBUG_API_MSG("[ASF_MJPEG]_MApi_MJPEG_PushToLowBuffer...............middle\n");
        JPEG_memcpy((void *)MsOS_PA2KSEG1(__u32CollectBufWptr[u8DrvId]), (void *)u32SrcAddrVA, u32Size);
        __u32CollectBufWptr[u8DrvId] = __u32CollectBufWptr[u8DrvId] + u32Size;
    }
}

static void _MApi_MJPEG_PushToHighBuffer(MS_U8 u32Id, MS_U32 u32SrcAddrVA, MS_U32 u32Size)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId) == TRUE)
    {
        __u32PrevBufAddr[u8DrvId] = __u32CollectBufWptr[u8DrvId];
        MJPEG_DEBUG_API_MSG("[ASF_MJPEG]_MApi_MJPEG_PushToHighBuffer...............start\n");
        // Step 1: reset write ptr to start of low buffer
        __u32CollectBufWptr[u8DrvId] = __u32CollectBufAddr[u8DrvId]+MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE/2;
        // Step 2: memcpy
        JPEG_memcpy((void *)MsOS_PA2KSEG1(__u32CollectBufWptr[u8DrvId]), (void *)u32SrcAddrVA, u32Size);
        // Step 3: update write ptr address
        __u32CollectBufWptr[u8DrvId] = __u32CollectBufWptr[u8DrvId] + u32Size;
    }
    else
    {
        MJPEG_DEBUG_API_MSG("[ASF_MJPEG]_MApi_MJPEG_PushToHighBuffer...............middle\n");
        JPEG_memcpy((void *)MsOS_PA2KSEG1(__u32CollectBufWptr[u8DrvId]), (void *)u32SrcAddrVA, u32Size);
        __u32CollectBufWptr[u8DrvId] = __u32CollectBufWptr[u8DrvId] + u32Size;
    }

}

static MS_BOOL _MApi_MJPEG_ASFBufferManagement(MS_U32 u32Id, MJPEG_Packet_Info *pInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    MS_BOOL _bDecodeInThisPush = FALSE;
    MS_U32 u32SrcAddrVA = MsOS_PA2KSEG1(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]);

    if(__bAsfFirstPacketInFrame[u8DrvId])
    {
        MJPEG_DEBUG_API_MSG("[ASF_MJPEG]detected first packet in frame!!!!!!!!!!!!!!!!\n");
        if(__bAsfFirstFrame[u8DrvId]==FALSE)
        {
            if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)
                && __eColBufStatus[u8DrvId][0]==E_MJPEG_BUF_USING)
            {
                MJPEG_DEBUG_API_MSG("[ASF_MJPEG]Push previous frame(L) to JPD!!!!!!!!!!!!!!!!\n");
                __eColBufStatus[u8DrvId][0] = E_MJPEG_BUF_BUSY;
                MsOS_FlushMemory();
                _bDecodeInThisPush = TRUE;
            }
            else if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)==FALSE
                && __eColBufStatus[u8DrvId][1]==E_MJPEG_BUF_USING)
            {
                MJPEG_DEBUG_API_MSG("[ASF_MJPEG]Push previous frame(H) to JPD!!!!!!!!!!!!!!!!\n");
                __eColBufStatus[u8DrvId][1] = E_MJPEG_BUF_BUSY;
                MsOS_FlushMemory();
                _bDecodeInThisPush = TRUE;
            }
        }

        if(_MApi_MJPEG_IsAbleToPushNextFrame(u32Id))
        {
            if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)
                && __eColBufStatus[u8DrvId][1] == E_MJPEG_BUF_AVAILABLE)
            {
                MJPEG_DEBUG_API_MSG("[ASF_MJPEG]First packet in frame(H)!!!!!!!!!!!!!!!!\n");
                _MApi_MJPEG_PushToHighBuffer(u32Id, u32SrcAddrVA, pInfo->u32Size);
                __eColBufStatus[u8DrvId][1] = E_MJPEG_BUF_USING;
            }
            else if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)==FALSE
                && __eColBufStatus[u8DrvId][0]==E_MJPEG_BUF_AVAILABLE)
            {
                MJPEG_DEBUG_API_MSG("[ASF_MJPEG]First packet in frame(L)!!!!!!!!!!!!!!!!\n");
                _MApi_MJPEG_PushToLowBuffer(u32Id, u32SrcAddrVA, pInfo->u32Size);
                __eColBufStatus[u8DrvId][0] = E_MJPEG_BUF_USING;
            }
        }
    }
    else
    {
        if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)
            && __eColBufStatus[u8DrvId][0] == E_MJPEG_BUF_USING)
        {
            MJPEG_DEBUG_API_MSG("[ASF_MJPEG]middle packet in frame(L)!!!!!!!!!!!!!!!!\n");
            _MApi_MJPEG_PushToLowBuffer(u32Id, u32SrcAddrVA, pInfo->u32Size);
        }
        else if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)==FALSE
            && __eColBufStatus[u8DrvId][1] == E_MJPEG_BUF_USING)
        {
            MJPEG_DEBUG_API_MSG("[ASF_MJPEG]middle packet in frame(H)!!!!!!!!!!!!!!!!\n");
            _MApi_MJPEG_PushToHighBuffer(u32Id, u32SrcAddrVA, pInfo->u32Size);
        }
    }
    return _bDecodeInThisPush;
}
#endif


#if 0//SUPPORT_SKIP_ERROR_FRAME
static MS_BOOL _MApi_MJPEG_IsDecodeTimeout(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(_u32StartDecodeTime[u8DrvId] ==0)
    {
        return FALSE;
    }
    if(MsOS_GetSystemTime() - _u32StartDecodeTime[u8DrvId] > SKIP_ERROR_FRAME_THRESHOLD)
    {
        if(E_MJPEG_RET_DECODING == __MApi_MJPEG_CheckDecodeDone(u32Id))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}
#endif

MS_BOOL _MApi_MJPEG_DropFrameWhenFF(MS_U8 u32Id)
{
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

        if(MApi_JPEG_GetWidth()<=MJPEG_SD_WIDTH && MApi_JPEG_GetHeight()<=MJPEG_SD_HEIGHT)
        {
            return FALSE;
        }
        else if(MApi_JPEG_GetWidth()<=MJPEG_HD_800x600_WIDTH && MApi_JPEG_GetHeight()<=MJPEG_HD_800x600_HEIGHT
            && __u32FrameRate[u8DrvId] <= MAX_SUPPORTED_FRAME_RATE_720P)
        {
            return FALSE;
        }
        else if(MApi_JPEG_GetWidth()<=MJPEG_MAX_SUPPORT_WIDTH && MApi_JPEG_GetHeight()<=MJPEG_MAX_SUPPORT_HEIGHT
            && __u32FrameRate[u8DrvId] <= (MAX_SUPPORTED_FRAME_RATE_720P/2))
        {
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
///Called only when video start
///@param pInitParam \b IN initial paramters
///@param pStream \b IN file system realted function pointers
///@return true or false
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Init(MS_U32 u32Id, MJPEG_INIT_PARAM *pInitParam)//, PJPEG_FILE_FileSystem_t pStream)
{
    JPEG_InitParam jpegInitParam;

    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    _MApi_MJPEG_InitPara(u32Id);
#if 0
    // For register read/write
    extern MS_U32 u32MJPEGRiuBaseAdd;
    MS_U32 u32NonPMBankSize = 0;
    if(FALSE == MDrv_MMIO_GetBASE( &u32MJPEGRiuBaseAdd, &u32NonPMBankSize, MS_MODULE_JPD))
    {
        MJPEG_DEBUG_API_ERR("MDrv_MMIO_GetBASE FAILED !! \n");
        return E_MJPEG_RET_FAIL;
    }
    else
    {
        MJPEG_DEBUG_API_MSG("DRVreg base = 0x%lX, length = %lu\n", u32MJPEGRiuBaseAdd, u32NonPMBankSize);
        MDrv_MJPEG_InitRegBase(u32MJPEGRiuBaseAdd);
    }
#endif

#if ENABLE_MJPEG_FIRMWARE
    // Init Firmware
    if(E_MJPEG_OK != MDrv_MJPEG_InitFw(u32Id, pInitParam->u32FwSize, pInitParam->u32FwAddr, pInitParam->u32FwBinSize, pInitParam->u32FwBinAddr, pInitParam->eFwSrcType))
    {
        MJPEG_DEBUG_API_ERR("MDrv_MJPEG_InitFw Failed~~~~\n");
    }
    //__u32FwAddr = pInitParam->u32FwAddr;
    isDecFrameInfoValid[u8DrvId] = FALSE;
    bStepDec[u8DrvId] = FALSE;

    MJPEG_DEBUG_API_MSG("clear error code due to call MApi_MJPEG_Init()\n");
    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERROR_CODE_NONE);
    // Check if firmware is ready or not
    if (MDrv_MJPEG_IsFirmwareReady() == FALSE)
    {
        MDrv_MJPEG_DeInit(u32Id);
        return E_MJPEG_RET_FAIL;
    }
    if((pInitParam->u32MWCBufAddr)%8)
    {
        MJPEG_DEBUG_API_MSG("u32MWCBufAddr need 8 byte alignment~~~~0x%lx\n", pInitParam->u32MWCBufAddr);
    }
    else
    {
        MJPEG_DEBUG_API_MSG("u32MWCBufAddr don't need 8 byte alignment~~~~0x%lx\n", pInitParam->u32MWCBufAddr);
    }

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR, pInitParam->u32MWCBufAddr);

#if SUPPORT_ASF_MJPEG_MODE
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        if((pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)
            >= MJPEG_MAX_SUPPORT_WIDTH*MJPEG_MAX_SUPPORT_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM;
        }
        else if((pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)
            >= MJPEG_MAX_SUPPORT_WIDTH*MJPEG_MAX_SUPPORT_HEIGHT*DISPLAY_QUEUE_NUM/2)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM/2;
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for 720p asf-mjpeg mode support!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    else
    {
        if((pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)
            >= MJPEG_SD_WIDTH*MJPEG_SD_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM;
        }
        else if((pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)
            >= MJPEG_SD_WIDTH*MJPEG_SD_HEIGHT*DISPLAY_QUEUE_NUM/2)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM/2;
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for asf-mjpeg mode support!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    __u32FrameBuffUnitSize[u8DrvId] = (pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)/__u8FrameBuffTotalNum[u8DrvId];
#else
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        if(pInitParam->u32MWCBufSize >= MJPEG_MAX_SUPPORT_WIDTH*MJPEG_MAX_SUPPORT_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM;
        }
        else if(pInitParam->u32MWCBufSize >= MJPEG_MAX_SUPPORT_WIDTH*MJPEG_MAX_SUPPORT_HEIGHT*DISPLAY_QUEUE_NUM/2)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM/2;
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for 720p mjpeg!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    else
    {
        if(pInitParam->u32MWCBufSize >= MJPEG_SD_WIDTH*MJPEG_SD_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM;
        }
        else if(pInitParam->u32MWCBufSize >= MJPEG_SD_WIDTH*MJPEG_SD_HEIGHT*DISPLAY_QUEUE_NUM/2)
        {
            __u8FrameBuffTotalNum[u8DrvId] = DISPLAY_QUEUE_NUM/2;
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for mjpeg!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    __u32FrameBuffUnitSize[u8DrvId] = pInitParam->u32MWCBufSize/__u8FrameBuffTotalNum[u8DrvId];
#endif
    __u32FrameBuffUnitSize[u8DrvId] = (__u32FrameBuffUnitSize[u8DrvId] >> 3) << 3;//8 byte alignment

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE, __u32FrameBuffUnitSize[u8DrvId]);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM, __u8FrameBuffTotalNum[u8DrvId]);
    #if ENABLE_DOUBLE_FRAMERATE
    MJPEG_DEBUG_API_MSG ("   frame rate base %ld\n", pInitParam->u32FrameRate);
    pInitParam->u32FrameRate /= 2;
    MJPEG_DEBUG_API_MSG ("   1/2 frame rate base %ld\n", pInitParam->u32FrameRate);
    #endif
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAMERATE, pInitParam->u32FrameRate);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAMERATE_BASE, pInitParam->u32FrameRateBase);
//    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_CHIP_ID, MDrv_MJPEG_Get_Chip_ID());

    if(pInitParam->u32FrameRate ==0)
    {
        MJPEG_DEBUG_API_ERR("Error!!! MJPEG Frame rate is 0\n");
        pInitParam->u32FrameRate =30000;
    }
    __u32FrameRate[u8DrvId] = pInitParam->u32FrameRate;
    __u32FrameRateBase[u8DrvId] = pInitParam->u32FrameRateBase;
    __u32FrameTime[u8DrvId] = __u32FrameRateBase[u8DrvId] * 1000 / __u32FrameRate[u8DrvId];
#endif // ENABLE_MJPEG_FIRMWARE

    __u32FrameCnt[u8DrvId] = 0;
    __u32DropFrameCnt[u8DrvId] = 0;

    __u32MRCBufAddr[u8DrvId] = pInitParam->u32MRCBufAddr;
    __u32MWCBufAddr[u8DrvId] = pInitParam->u32MWCBufAddr;
#if SUPPORT_ASF_MJPEG_MODE
    __u32CollectBufAddr[u8DrvId] = pInitParam->u32MWCBufAddr+pInitParam->u32MWCBufSize - MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE;
    __u32CollectBufWptr[u8DrvId] = __u32CollectBufAddr[u8DrvId];
#endif

    __u32InternalBufAddr[u8DrvId] = pInitParam->u32InternalBufAddr;
    __u32InternalBufSize[u8DrvId] = pInitParam->u32InternalBufSize;

    jpegInitParam.u32MRCBufAddr = pInitParam->u32MRCBufAddr;
    jpegInitParam.u32MRCBufSize = pInitParam->u32MRCBufSize;
    jpegInitParam.u32MWCBufAddr = pInitParam->u32MWCBufAddr;
#if SUPPORT_ASF_MJPEG_MODE
    jpegInitParam.u32MWCBufSize = pInitParam->u32MWCBufSize - MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE;
#else
    jpegInitParam.u32MWCBufSize = pInitParam->u32MWCBufSize;
#endif

    jpegInitParam.u32InternalBufAddr = pInitParam->u32InternalBufAddr;
    jpegInitParam.u32InternalBufSize = pInitParam->u32InternalBufSize;
    jpegInitParam.u32DecByteRead = 0;
    jpegInitParam.bEOF = TRUE;
    jpegInitParam.u8DecodeType = E_JPEG_TYPE_MJPEG;
    jpegInitParam.bInitMem = TRUE;
    jpegInitParam.pFillHdrFunc = NULL;

#if 0
    printf("MJPEG init================================\n");
    printf("[MRC]addr=0x%lx, size=0x%lx\n", __u32MRCBufAddr[u8DrvId], jpegInitParam.u32MRCBufSize);
    printf("[MWC]addr=0x%lx, size=0x%lx\n", __u32MWCBufAddr[u8DrvId], jpegInitParam.u32MWCBufSize);
    printf("[INTER]addr=0x%lx, size=0x%lx\n", __u32InternalBufAddr[u8DrvId], __u32InternalBufSize[u8DrvId]);
    printf("[MRC VA]addr=0x%lx, size=0x%lx\n", MS_PA2KSEG1(__u32MRCBufAddr[u8DrvId]), jpegInitParam.u32MRCBufSize);
    printf("[MWC VA]addr=0x%lx, size=0x%lx\n", MS_PA2KSEG1(__u32MWCBufAddr[u8DrvId]), jpegInitParam.u32MWCBufSize);
    printf("[INTER VA]addr=0x%lx, size=0x%lx\n", MS_PA2KSEG1(__u32InternalBufAddr[u8DrvId]), __u32InternalBufSize[u8DrvId]);
    printf("===========================================\n");
#endif

    // This is used to init memory
    if (MApi_JPEG_Init(&jpegInitParam) == E_JPEG_FAILED)
    {
#if ENABLE_MJPEG_FIRMWARE
        MDrv_MJPEG_DeInit(u32Id);
#endif
        return E_MJPEG_RET_FAIL;
    }

    // Make sure it's exited correctly
    MApi_JPEG_Exit();

    // Init Local Variables
    PreVIdx[u8DrvId] = 0;
    ReCheckTime[u8DrvId] = 0;
    bLstFrame[u8DrvId] = FALSE;

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Reset JPD
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Rst(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    //__u32FrameCnt = 0;
    __bIsFirstFrameRdy[u8DrvId] = FALSE;

    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MApi_JPEG_Rst();
    MJPEG_DEBUG_API_MSG("clear error code due to call MApi_MJPEG_Rst()\n");
    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERROR_CODE_NONE);

    // CHECK : reset firmware???

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Turn off JPD clock
/******************************************************************************/
//extern void MApi_JPEG_DisableAddressConvert(void); // defined in apiJPEG.c
MJPEG_Result MApi_MJPEG_Stop(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    //__u32FrameCnt = 0;
    __bIsFirstFrameRdy[u8DrvId] = FALSE;
    isDecFrameInfoValid[u8DrvId] = FALSE;
    bStepDec[u8DrvId] = FALSE;

    MDrv_MJPEG_StopVPU();

    MDrv_MJPEG_DeInit(u32Id);

    //MApi_JPEG_PowerOff() is called in MApi_JPEG_Exit()
    MApi_JPEG_Exit();

    MApi_JPEG_DisableAddressConvert();

    // CHECK : stop firmware???

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if first frame has been decoded
///@return TRUE if first picture is decoded, else return FALSE
/******************************************************************************/
MJPEG_Result MApi_MJPEG_CheckDispInfoRdy(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if ((TRUE == __bIsFirstFrameRdy[u8DrvId])
    && (E_JPEG_EVENT_DEC_DONE & MApi_JPEG_GetJPDEventFlag()))
    {
        // Should use the width & height of 1st frame
        //__u16Width = MApi_JPEG_GetWidth();
        //__u16Height = MApi_JPEG_GetHeight();

        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_CheckDispInfoRdy : E_MJPEG_RET_SUCCESS\n");
        return E_MJPEG_RET_SUCCESS;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_CheckDispInfoRdy : E_MJPEG_RET_FAIL\n");
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Play
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Play(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_PLAY, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Play : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Play : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Pause
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Pause(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_PAUSE, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Pause : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Pause : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Resume
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Resume(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_RESUME, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Resume : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_Resume : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Step Play
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_StepPlay(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MDrv_MJPEG_SetStepDispPara(u32Id);
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_STEP_PLAY, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_StepPlay : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_StepPlay : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if step play is done or not
///@return true or false
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsStepPlayDone(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }

#if USE_MJPEG_SHAREMEMORY
    return (!MDrv_MJPEG_IsStepPlayDone(u32Id));
#else
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_STEP_PLAY_DONE, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsStepPlayDone : TRUE\n");
        return TRUE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsStepPlayDone : FALSE\n");
    return FALSE;
#endif
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_StepDecode(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    bStepDec[u8DrvId] = FALSE;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsStepDecodeDone(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if(FALSE == bStepDec[u8DrvId])
        return FALSE;

    if (E_MJPEG_RET_SUCCESS == __MApi_MJPEG_CheckDecodeDone(u32Id))
        return TRUE;

    return FALSE;
}

/******************************************************************************/
///Issue full JPEG decode process
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
//MJPEG_Result MApi_MJPEG_PushQueue(MS_PHY u32StAddr, MS_U32 u32Size, MS_U32 u32TimeStamp)
MJPEG_Result MApi_MJPEG_PushQueue(MS_U32 u32Id, MJPEG_Packet_Info *pInfo)
{
    //
    // There's no queue
    //
    JPEG_InitParam jpegInitParam;
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }

    MJPEG_DEBUG("[MJPEG]MApi_MJPEG_PushQueue() pts=%ld\n", pInfo->u32TimeStamp);
#if 0   // force to asf mjpeg mode (move to another internal buffer)
    pInfo->u32StAddr |= VDEC_EX_MVD_PIC_START_FLAG;
#endif

    if(pInfo->u32Size == 0)
    {
        u16NullCnt[u8DrvId]++;
        MJPEG_DEBUG_API_MSG("%s: MJPEG NULL PACKET!!!!!!!!!!\n", __FUNCTION__);
        return E_MJPEG_RET_SUCCESS;
    }
#if 0//SUPPORT_SKIP_ERROR_FRAME
    if(_MApi_MJPEG_IsDecodeTimeout(u32Id))
    {
        MJPEG_DEBUG_API_ERR("%s: JPEG decode frame timeout!!!!!!!!!!\n", __FUNCTION__);
        _u32SkipCnt[u8DrvId]++;
        _u32ErrCnt[u8DrvId]++;
        // Repeat next frame because previous frame decode timeout
        u16NullCnt[u8DrvId]++;
        MJPEG_DEBUG_API_ERR("%s: Reset JPD!!!!!!!!!!\n", __FUNCTION__);
        MApi_JPEG_Rst();
        MApi_JPEG_Exit();
    }
#endif

#if SUPPORT_ASF_MJPEG_MODE
    if(__bAsfMjpegMode[u8DrvId]==TRUE)
    {
        if(_MApi_MJPEG_IsAbleToPushNextFrame(u32Id) ==FALSE
            && pInfo->u32StAddr & VDEC_EX_MVD_PIC_START_FLAG)
        {
            // At least one packet is available for pushing next frame
            return E_MJPEG_RET_DECODING;
        }
    }

    _MApi_MJPEG_ASFParameterSetting(u32Id, pInfo);
    pInfo->u32StAddr &= ~VDEC_EX_MVD_PIC_START_FLAG;

    if(__bAsfMjpegMode[u8DrvId]==TRUE)
    {
        if(_MApi_MJPEG_ASFBufferManagement(u32Id, pInfo)==FALSE)
        {
            return E_MJPEG_RET_SUCCESS;
        }
    }
#endif

    MS_U16 u16DecodeCnt;
    for(u16DecodeCnt=0; u16DecodeCnt<=u16NullCnt[u8DrvId]; u16DecodeCnt++)
    {
        if (E_MJPEG_RET_DECODING == __MApi_MJPEG_CheckDecodeDone(u32Id))
        {
            u16NullCnt[u8DrvId] -=u16DecodeCnt;
            MJPEG_DEBUG_API_MSG("%s: E_MJPEG_RET_DECODING with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
            return E_MJPEG_RET_DECODING;
        }
#if SUPPORT_ASF_MJPEG_MODE
        if(__bAsfMjpegMode[u8DrvId]==TRUE)
        {
            if(u16DecodeCnt == u16NullCnt[u8DrvId])
            {
                if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId) ==TRUE
                    && __eColBufStatus[u8DrvId][1]==E_MJPEG_BUF_BUSY)
                {
                    __eColBufStatus[u8DrvId][1] = E_MJPEG_BUF_AVAILABLE;
                }
                else if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId)==FALSE
                    && __eColBufStatus[u8DrvId][0]==E_MJPEG_BUF_BUSY)
                {
                    __eColBufStatus[u8DrvId][0] = E_MJPEG_BUF_AVAILABLE;
                }
            }
        }
#endif

#if 1
        if(__u32ReadPtr[u8DrvId] > __u32WritePtr[u8DrvId])
        {
            MJPEG_DEBUG("\033[1;31m[MWC][%dx%d.yuyv]addr=0x%lx, size=0x%x, [r:0x%lx, w:0x%lx]\033[m\n",
                MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), infoDecFrame[u8DrvId].u32LumaAddr,
                MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2,
                __u32ReadPtr[u8DrvId], __u32WritePtr[u8DrvId]);
        }
        else
        {
            MJPEG_DEBUG("[MWC][%dx%d.yuyv]addr=0x%lx, size=0x%x, [r:0x%lx, w:0x%lx]\n",
                MApi_JPEG_GetWidth(), MApi_JPEG_GetHeight(), infoDecFrame[u8DrvId].u32LumaAddr,
                MApi_JPEG_GetWidth()*MApi_JPEG_GetHeight()*2,
                __u32ReadPtr[u8DrvId], __u32WritePtr[u8DrvId]);
        }
        if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_MJPEG)
        {
            MDrv_MJPEG_PrintShareInfo(u32Id);
        }
#endif
       __u32FrameCnt[u8DrvId]++;
        __u32ReadPtr[u8DrvId] = u32EndAddr[u8DrvId];
        MJPEG_DEBUG_API_MSG("%s: Count %ld, __bSkipToPts=%d, __bSeekToPts=%d, __u32Skip2PtsBase=%ld, __u32TargetPts=%ld\n",
            __FUNCTION__, __u32FrameCnt[u8DrvId], __bSkipToPts[u8DrvId], __bSeekToPts[u8DrvId], __u32Skip2PtsBase[u8DrvId], __u32TargetPts[u8DrvId]);


        __u32WritePtr[u8DrvId] = pInfo->u32StAddr + pInfo->u32Size - 1;


        MJPEG_DEBUG("[MJPEG]Pushed done, [r:0x%lx, w:0x%lx, u8DrvId=%d]\n", __u32ReadPtr[u8DrvId], __u32WritePtr[u8DrvId], u8DrvId);

        // Skip/Seek to PTS
        if (__bSkipToPts[u8DrvId] || __bSeekToPts[u8DrvId])
        {
            if (__u32Skip2PtsBase[u8DrvId] >= __u32TargetPts[u8DrvId])
            {
                MJPEG_DEBUG_API_MSG("Skip/Seek to PTS done -- current PTS %ld\n", __u32Skip2PtsBase[u8DrvId]);
                __bSkipToPts[u8DrvId] = FALSE;
                __bSeekToPts[u8DrvId] = FALSE;
                // Need to update firmware PTS base
                MDrv_MJPEG_ResetPTS(u32Id, __u32Skip2PtsBase[u8DrvId]);
            }
            else
            {
                MJPEG_DEBUG_API_MSG("Skip Frame -- PTS %ld\n", __u32Skip2PtsBase[u8DrvId]);
                __u32Skip2PtsBase[u8DrvId] += __u32FrameTime[u8DrvId];
                __bIsDecodeDone[u8DrvId] = TRUE;
                u32EndAddr[u8DrvId] = pInfo->u32StAddr + pInfo->u32Size - 1;
                return E_MJPEG_RET_SUCCESS;
            }
        }

        // Speed Control
        if (__bIsFirstFrameRdy[u8DrvId])
        {
            if (eCurSpeedType[u8DrvId] == E_MJPEG_SPEED_FAST)
            {
                if ((__u32FrameCnt[u8DrvId] - __u32SpeedCtrlFrameCntBase[u8DrvId]) % eCurDispSpeed[u8DrvId])
                {
                    MJPEG_DEBUG_API_MSG("       Skip frame %ld\n", (__u32FrameCnt[u8DrvId] - __u32SpeedCtrlFrameCntBase[u8DrvId]));
                    //__u32FrameCnt++;
                    u16NullCnt[u8DrvId]=0;
                    if(_MApi_MJPEG_DropFrameWhenFF(u32Id)==TRUE)
                    {
                        __bIsDecodeDone[u8DrvId] = TRUE;
                        return E_MJPEG_RET_SUCCESS;
                    }
                }
            }
            else if (eCurSpeedType[u8DrvId] == E_MJPEG_SPEED_SLOW)
            {
            }
            else // if (eCurSpeedType == E_MJPEG_SPEED_DEFAULT)
            {
                __u32SpeedCtrlFrameCntBase[u8DrvId] = __u32FrameCnt[u8DrvId];
                MJPEG_DEBUG_API_MSG("   Update Frame Count Base %ld\n", __u32SpeedCtrlFrameCntBase[u8DrvId]);
            }
        }


        MApi_JPEG_SetMaxDecodeResolution(JPEGBASEWIDTHMAX, JPEGBASEHEIGHTMAX);


        MJPEG_DEBUG_API_MSG("pInfo->u32StAddr=0x%lx, __u32MRCBufAddr=0x%lx\n", pInfo->u32StAddr, __u32MRCBufAddr[u8DrvId]);
        MJPEG_DEBUG_API_MSG("pInfo->u32Size=0x%lx\n", pInfo->u32Size);
        // Init
#if SUPPORT_ASF_MJPEG_MODE
        if(__bAsfMjpegMode[u8DrvId]==TRUE)
        {
            if(_MApi_MJPEG_IsColWPtrInLow(u8DrvId))
            {
                jpegInitParam.u32MRCBufAddr = __u32CollectBufAddr[u8DrvId]+MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE/2;
            }
            else
            {
                jpegInitParam.u32MRCBufAddr = __u32CollectBufAddr[u8DrvId];
            }
            jpegInitParam.u32MRCBufSize = 2*(__u32PrevBufAddr[u8DrvId] - jpegInitParam.u32MRCBufAddr);
            //printf("[addr=0x%lx, size=0x%lx]\n", jpegInitParam.u32MRCBufAddr, jpegInitParam.u32MRCBufSize);
        }
        else
        {
            jpegInitParam.u32MRCBufAddr = (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]) & ~0x0f;
            // Tricky code: let JPD decode only L buffer, then decode dne
            memset((void *)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr), 0, (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr));
            jpegInitParam.u32MRCBufSize = ((pInfo->u32Size+(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr)+0xf) & ~0x0f)*2;
        }
#else
        jpegInitParam.u32MRCBufAddr = (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]) & ~0x0f;
        // Tricky code: let JPD decode only L buffer, then decode dne
        memset((void *)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr), 0, (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr));
        jpegInitParam.u32MRCBufSize = ((pInfo->u32Size+(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr)+0xf) & ~0x0f)*2;
#endif
        if (jpegInitParam.u32MRCBufSize < JPEG_DEFAULT_EXIF_SIZE)
        {   // Meet size check in MApi_JPEG_Init()
            jpegInitParam.u32MRCBufSize = JPEG_DEFAULT_EXIF_SIZE;
        }

        // Ask firmware if there is any frame buffer available
#if USE_MJPEG_SHAREMEMORY
        u32NextFrameBuffIdx[u8DrvId] = MDrv_MJPEG_GetNextFrameBufferIdx(u32Id);
#else
        u32NextFrameBuffIdx[u8DrvId] = MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX, 0xFFFFFFFF);
#endif

        MJPEG_DEBUG("[MJPEG]              E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX %ld\n", u32NextFrameBuffIdx[u8DrvId]);
        if (u32NextFrameBuffIdx[u8DrvId] >= 0xFF)// It may be 0xFF, 0xFFFF, 0xFFFFFF, or 0xFFFFFFFF
        {
            MJPEG_DEBUG_API_ERR("   !!! No frame buffer available !!!, u32NextFrameBuffIdx=0x%lx\n", u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;
            return E_MJPEG_RET_FAIL;
        }

        if (__bIsFirstFrameRdy[u8DrvId] == FALSE)
        {
            u32NextFrameBuffIdx[u8DrvId] = 0;
        }
        __bIsDecodeDone[u8DrvId] = FALSE;
#if USE_MJPEG_SHAREMEMORY
        MDrv_MJPEG_SetPushQPara(u32Id, pInfo->u32TimeStamp, pInfo->u32ID_L, pInfo->u32ID_H, u32NextFrameBuffIdx[u8DrvId]);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_PUSH_QUEUE_PARA_SETTING, (MS_U32)NULL);
#else
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX, u32NextFrameBuffIdx[u8DrvId]);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_TIMESTAMP, pInfo->u32TimeStamp);
        MJPEG_DEBUG_API_MSG("   u32TimeStamp %ld", pInfo->u32TimeStamp);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_ID_L, pInfo->u32ID_L);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_ID_H, pInfo->u32ID_H);
        MJPEG_DEBUG_API_MSG("Frame ID 0x%lx%lx", pInfo->u32ID_H, pInfo->u32ID_L);
#endif

        jpegInitParam.u32MWCBufAddr = __u32MWCBufAddr[u8DrvId] + __u32FrameBuffUnitSize[u8DrvId]*u32NextFrameBuffIdx[u8DrvId];
        MJPEG_DEBUG_API_MSG("   Set Frame buffer addr 0x%lx\n", jpegInitParam.u32MWCBufAddr);
        jpegInitParam.u32MWCBufSize = __u32FrameBuffUnitSize[u8DrvId];

        jpegInitParam.u32InternalBufAddr = __u32InternalBufAddr[u8DrvId];
        jpegInitParam.u32InternalBufSize = __u32InternalBufSize[u8DrvId];
#if SUPPORT_ASF_MJPEG_MODE
        if(__bAsfMjpegMode[u8DrvId]==TRUE)
        {
            jpegInitParam.u32DecByteRead = jpegInitParam.u32MRCBufSize;
        }
        else
        {
            jpegInitParam.u32DecByteRead = pInfo->u32Size;
        }
#else
        jpegInitParam.u32DecByteRead = pInfo->u32Size;
#endif
        jpegInitParam.bEOF = TRUE; // Set to TRUE to disable read function
        jpegInitParam.u8DecodeType = E_JPEG_TYPE_MAIN; // only set to E_JPEG_TYPE_MJPEG when 1st time
        jpegInitParam.bInitMem = TRUE;
        jpegInitParam.pFillHdrFunc = NULL;

        #if ENABLE_AVSYNC_ID_CHECK
        printf ("ID 0x%08lx%08lx    PTS %ld\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp);
        #endif

        // Make sure it's exited correctly
        MApi_JPEG_Exit();


        u32EndAddr[u8DrvId] = pInfo->u32StAddr + pInfo->u32Size - 1;
#if 0
        printf("[MWC]: addr=0x%lx, size=0x%lx\n", jpegInitParam.u32MWCBufAddr, jpegInitParam.u32MWCBufSize);
        printf("[MRC]: addr=0x%lx, size=0x%lx\n", jpegInitParam.u32MRCBufAddr, jpegInitParam.u32MRCBufSize);
        printf("[INTER]: addr=0x%lx, size=0x%lx\n", jpegInitParam.u32InternalBufAddr, jpegInitParam.u32InternalBufSize);
#endif
#if 0
        printf("bitstream addr[PA]: 0x%lx, size=0x%lx\n", jpegInitParam.u32MRCBufAddr, pInfo->u32Size);
        MS_U32 u32VA = MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr);
        MS_U16 u8i;
        for(u8i=0;  u8i<0x100; u8i++)
        {
            printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n",
                *((MS_U8*)(u32VA+u8i*16)), *((MS_U8*)(u32VA+u8i*16+1)), *((MS_U8*)(u32VA+u8i*16+2)), *((MS_U8*)(u32VA+u8i*16+3)),
                *((MS_U8*)(u32VA+u8i*16+4)), *((MS_U8*)(u32VA+u8i*16+5)), *((MS_U8*)(u32VA+u8i*16+6)), *((MS_U8*)(u32VA+u8i*16+7)),
                *((MS_U8*)(u32VA+u8i*16+8)), *((MS_U8*)(u32VA+u8i*16+9)), *((MS_U8*)(u32VA+u8i*16+10)), *((MS_U8*)(u32VA+u8i*16+11)),
                *((MS_U8*)(u32VA+u8i*16+12)), *((MS_U8*)(u32VA+u8i*16+13)), *((MS_U8*)(u32VA+u8i*16+14)), *((MS_U8*)(u32VA+u8i*16+15))
                );
        }
#endif

        if (MApi_JPEG_Init(&jpegInitParam) == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("MApi_JPEG_Init() FAIL\n");
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;
            return E_MJPEG_RET_FAIL;
        }

        // Get width, height, & pitch of 1st frame
        if (__bIsFirstFrameRdy[u8DrvId] == FALSE)
        {
            __u16Width[u8DrvId] = MApi_JPEG_GetWidth();
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_WIDTH, __u16Width[u8DrvId]);
            __u16Height[u8DrvId] = MApi_JPEG_GetHeight();
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_HEIGHT, __u16Height[u8DrvId]);
            __u16Pitch[u8DrvId] = MApi_JPEG_GetAlignedPitch();
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_PITCH, __u16Pitch[u8DrvId]);
            MJPEG_DEBUG_API_MSG("       width %d, height %d, pitch %d\n", __u16Width[u8DrvId], __u16Height[u8DrvId], __u16Pitch[u8DrvId]);

            if(MApi_JPEG_IsNJPD()==TRUE)
            {
                if((__u16Width[u8DrvId] > MJPEG_MAX_SUPPORT_WIDTH) ||
                    (__u16Height[u8DrvId]>MJPEG_MAX_SUPPORT_HEIGHT))
                {
                    MJPEG_DEBUG_API_MSG("Set MJPEG error code=E_MJPEG_ERR_CODE_RES \n");
                    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERR_CODE_RES);
                }
            }
            else
            {
                if((__u16Width[u8DrvId] > MJPEG_SD_WIDTH) ||
                    (__u16Height[u8DrvId]>MJPEG_SD_HEIGHT))
                {
                    MJPEG_DEBUG_API_MSG("Set MJPEG error code=E_MJPEG_ERR_CODE_RES \n");
                    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERR_CODE_RES);
                }

            }
        }

        // Fill decode frame info
        infoDecFrame[u8DrvId].u32LumaAddr = jpegInitParam.u32MWCBufAddr;
        infoDecFrame[u8DrvId].u32ChromaAddr = infoDecFrame[u8DrvId].u32LumaAddr;
        infoDecFrame[u8DrvId].u32TimeStamp = pInfo->u32TimeStamp;
        infoDecFrame[u8DrvId].u32ID_L = pInfo->u32ID_L;
        infoDecFrame[u8DrvId].u32ID_H = pInfo->u32ID_H;
        infoDecFrame[u8DrvId].u16Pitch = __u16Pitch[u8DrvId];
        infoDecFrame[u8DrvId].u16Width = __u16Width[u8DrvId];
        infoDecFrame[u8DrvId].u16Height = __u16Height[u8DrvId];
        isDecFrameInfoValid[u8DrvId] = TRUE;

        // Decode header
        if (MApi_JPEG_DecodeHdr() == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("MApi_JPEG_DecodeHdr() FAIL\n");
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;

            JPEG_ErrCode jpeg_errcode = MApi_JPEG_GetErrorCode();
            if((E_JPEG_UNSUPPORTED_COLORSPACE == jpeg_errcode)
            || (E_JPEG_UNSUPPORTED_SAMP_FACTORS == jpeg_errcode)
            || (E_JPEG_UNSUPPORTED_HUFF_DECODE == jpeg_errcode))
            {
                MJPEG_DEBUG_API_ERR("Set MJPEG error code=E_MJPEG_ERR_CODE_RES for unsupport!!\n");
                _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERR_CODE_RES);
            }

            return E_MJPEG_RET_DECODE_HDR_ERROR;
        }

        bStepDec[u8DrvId] = TRUE;

        // Decode
        if (MApi_JPEG_Decode() == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("MApi_JPEG_Decode() FAIL\n");
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %ld as Valid", u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;
            return E_MJPEG_RET_DECODE_ERROR;
        }

#if SUPPORT_SKIP_ERROR_FRAME
        _u32StartDecodeTime[u8DrvId] = MsOS_GetSystemTime();
#endif


        // Moved Up
        //u32EndAddr = pInfo->u32StAddr + pInfo->u32Size - 1;
        __bIsFirstFrameRdy[u8DrvId] = TRUE;

        // All done, wait for decoding finish
        if(u16NullCnt[u8DrvId] ==u16DecodeCnt)
        {
            if(u16NullCnt[u8DrvId]!=0)
            {
                MJPEG_DEBUG_API_MSG("%s: Handle MJPEG NULL PACKET with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
            }
            u16NullCnt[u8DrvId]=0;
            MJPEG_DEBUG_API_MSG("Wait for Fill decode frame info u32LumaAddr=0x%lx, u32ChromaAddr=0x%lx\n",
                infoDecFrame[u8DrvId].u32LumaAddr, infoDecFrame[u8DrvId].u32ChromaAddr);
            return E_MJPEG_RET_SUCCESS;
        }
        else
        {
            MJPEG_DEBUG_API_MSG("%s: Duplicate PACKET with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
        }
    }

    // Only for compile ok, the code should not run to this place.
    MJPEG_DEBUG_API_MSG("%s: Handle MJPEG NULL PACKET with u16DecodeCnt=%d\n", __FUNCTION__, u16DecodeCnt);
    u16NullCnt[u8DrvId]=0;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetQueueVacancy(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    #if ENABLE_AVSYNC_ID_CHECK
    MJPEG_FrameInfo info;
    MApi_MJPEG_GetDispFrameInfo(u32Id, &info);
    #endif


    if (__bIsFirstFrameRdy[u8DrvId] == FALSE)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 1\n");
        return 1;
    }
    else
    {
        if (__MApi_MJPEG_CheckDecodeDone(u32Id) != E_MJPEG_RET_DECODING)
        {
#if USE_MJPEG_SHAREMEMORY
            if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL, 0xFFFFFFFF) == 1)
            {
                MJPEG_DEBUG("[MJPEG]            Display queue is full\n");
                MJPEG_DEBUG("[MJPEG]           MApi_MJPEG_GetQueueVacancy : 0\n");
                return 0;
            }

            if(MDrv_MJPEG_GetNextFrameBufferIdx(u32Id)>=0xff)
            {
                MJPEG_DEBUG("[MJPEG]           MApi_MJPEG_GetQueueVacancy : 0\n");
                return 0;
            }
            else
            {
                MJPEG_DEBUG("[MJPEG]           MApi_MJPEG_GetQueueVacancy : 1\n");
                return 1;
            }
#else
            if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL, 0xFFFFFFFF) == 1)
            {
                MJPEG_DEBUG_API_MSG("            Display queue is full\n");
                MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 0\n");
                return 0;
            }

            MS_U32 u32tmpcounter = MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX, 0xFFFFFFFF);
            if ( u32tmpcounter>= 0xFF)
            {
                MJPEG_DEBUG_API_MSG("E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX %ld\n", u32tmpcounter);
                return 0;
            }
            MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 1\n");
            return 1;
#endif
        }
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_GetQueueVacancy : 0\n");
    return 0;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_FlushQueue(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    // Wait current frame decode done
    while (E_MJPEG_RET_DECODING == __MApi_MJPEG_CheckDecodeDone(u32Id))
        ;

    // Stop firmware display to avoid display queue status overwritten
    MApi_MJPEG_Pause(u32Id);

    isDecFrameInfoValid[u8DrvId] = FALSE;
    bStepDec[u8DrvId] = FALSE;

    MJPEG_DEBUG("MApi_MJPEG_FlushQueue()!!!!!!!!!\n");
    __u32WritePtr[u8DrvId] = 0;
    __u32ReadPtr[u8DrvId] = 0;
    u32EndAddr[u8DrvId] = 0;

    MDrv_MJPEG_InitFlush(u32Id);
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_FLUSH_DISP_QUEUE, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_FlushQueue : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }


#if USE_MJPEG_SHAREMEMORY
    while(MDrv_MJPEG_IsFlushDone(u32Id)==0);
#else
    while (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_FLUSH_DONE, 0xFFFFFFFF) == 0)
    {
        printf ("Wait for flush done\n");
    }
#endif
    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_FlushQueue : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetESWritePtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    // return end of data
    return __u32WritePtr[u8DrvId];
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetESReadPtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    // return end of data
    return __u32ReadPtr[u8DrvId];
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_EnableLastFrameShow(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    bLstFrame[u8DrvId] = bEnable;
    // This function always returns true.
    // Because MJPEG firmware does not reserve any frame.
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsDispFinish(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    // Check if the last frame decode finish,
    // Call this function to set E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY
    // when decode done.
//    MJPEG_Result ret;
    if (bLstFrame[u8DrvId] == 0)
    {
        return FALSE;
    }
//    ret = __MApi_MJPEG_CheckDecodeDone(u32Id);

    if(E_MJPEG_RET_SUCCESS != __MApi_MJPEG_CheckDecodeDone(u32Id))
    {
        MJPEG_DEBUG_API_MSG("MApi_MJPEG_IsDispFinish: MApi_MJPEG_CheckDecodeDone Fail!!\n");
        return FALSE;
    }

    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_DISP_FINISH, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsDispFinish : FALSE\n");
        return FALSE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsDispFinish : TRUE\n");
    return TRUE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SetSpeed(MS_U32 u32Id, MJPEG_SpeedType eSpeedType, MJPEG_DispSpeed eDispSpeed)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    eCurSpeedType[u8DrvId] = eSpeedType;
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_SPEED_TYPE, (MS_U32)eSpeedType);

    eCurDispSpeed[u8DrvId] = eDispSpeed;
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_SPEED, (MS_U32)eDispSpeed);

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_STC_BASE, 0xFFFFFFFF);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SetFreezeDisp(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_FREEZE_DISP, (MS_U32)bEnable);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SetBlueScreen(MS_U32 u32Id, MS_BOOL bOn)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_BLUE_SCREEN, (MS_U32)bOn);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Reset PTS value
///@param u32Value \b IN new PTS value
/******************************************************************************/
MJPEG_Result MApi_MJPEG_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if (MDrv_MJPEG_ResetPTS(u32Id, u32PtsBase))
        return E_MJPEG_RET_SUCCESS;

    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Return PTS of current displayed frame
///@return PTS
/******************************************************************************/
MS_U32 MApi_MJPEG_GetPTS(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return MDrv_MJPEG_GetPTS(u32Id);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_AVSyncOn(MS_U32 u32Id, MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_ENABLE_AVSYNC, (MS_U32)bOn);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_AVSYNC_DELAY, (MS_U32)u32SyncDelay);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_AVSYNC_TOLERENCE, (MS_U32)u16SyncTolerance);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsAVSyncOn(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
#if USE_MJPEG_SHAREMEMORY
    return MDrv_MJPEG_IsAVSyncOn(u32Id);
#else
    return MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_AVSYNC_ON, 0xFFFFFFFF);
#endif
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsReachSync(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
#if USE_MJPEG_SHAREMEMORY
    return MDrv_MJPEG_IsReachAVSync(u32Id);
#else
    return MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_REACH_AVSYNC, 0xFFFFFFFF);
#endif
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_GetStatus(MS_U32 u32Id, MJPEG_Status *pStatus)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    pStatus->bIdle = MApi_MJPEG_IsIdle(u32Id);
    //pStatus->bInit
    //pStatus->eStage = E_MJPEG_STAGE_PLAY;

    return FALSE;
}

/******************************************************************************/
///Return JPD error code
///@return JPD error code
/******************************************************************************/
MS_U32 MApi_MJPEG_GetErrCode(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    // Per XDemux's request, this only returns critical error.
    // Since no error would prevent JPD from decoding next JPEG,
    // it always return 0 here.
    MJPEG_DEBUG_API_MSG("MApi_MJPEG_GetErrCode() with eMJPEGErrCode=0x%x\n", eMJPEGErrCode[u8DrvId]);
    return eMJPEGErrCode[u8DrvId];
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetErrCnt(MS_U32 u32Id)
{
#if SUPPORT_SKIP_ERROR_FRAME
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return _u32ErrCnt[u8DrvId];
#else
    return 0;
#endif
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetSkipCnt(MS_U32 u32Id)
{
#if SUPPORT_SKIP_ERROR_FRAME
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return _u32SkipCnt[u8DrvId];
#else
    return 0;
#endif
}
/******************************************************************************/
///Return count of current displayed frame
///@return frame count
/******************************************************************************/
MS_U32 MApi_MJPEG_GetFrameCnt(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return __u32FrameCnt[u8DrvId];
}

/******************************************************************************/
///Return video information
///@param pInfo \b OUT video information: width & height
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDispInfo(MS_U32 u32Id, MJPEG_DISP_INFO *pInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    // APP wants pitch to setup display
    // This should be used with crop information

    MJPEG_DEBUG_API_MSG("MApi_MJPEG_GetDispInfo() with __u16Pitch=%d, __u16Height=%d, __u16Width=%d\n",
    __u16Pitch[u8DrvId], __u16Height[u8DrvId], __u16Width[u8DrvId]);
    pInfo->u16Width = __u16Pitch[u8DrvId];
    pInfo->u16Height = __u16Height[u8DrvId];
    pInfo->u16Pitch = __u16Pitch[u8DrvId];
    if(__u32FrameRateBase[u8DrvId] !=0)
    {
        pInfo->u32FrameRate = __u32FrameRate[u8DrvId] * 1000 / __u32FrameRateBase[u8DrvId];
    }
    else
    {
        MJPEG_DEBUG_API_ERR("Error!!! __u32FrameRateBase==0!!!!!\n");
    }

    pInfo->u16CropLeft = (pInfo->u16Width - __u16Width[u8DrvId])/2;
    pInfo->u16CropRight = pInfo->u16Width - __u16Width[u8DrvId] - pInfo->u16CropLeft;
    pInfo->u16CropTop = (pInfo->u16Height - __u16Height[u8DrvId])/2;
    pInfo->u16CropBottom = pInfo->u16Height - __u16Height[u8DrvId] - pInfo->u16CropTop;
    MJPEG_DEBUG_API_MSG("   Crop info right %d, left %d, bottom %d, top %d\n",
        pInfo->u16CropRight, pInfo->u16CropLeft, pInfo->u16CropBottom, pInfo->u16CropTop);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsPlaying(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
#if USE_MJPEG_SHAREMEMORY
    return MDrv_MJPEG_IsPlaying(u32Id);
#else
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_PLAYING, 0xFFFFFFFF) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsPlaying : FALSE\n");
        return FALSE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsPlaying : TRUE\n");
    return TRUE;
#endif
}

/******************************************************************************/
///Return true if idle, including decode done & error
///@return idle or not
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsIdle(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    JPEG_Event u16Value = MApi_JPEG_GetJPDEventFlag();
    if ((E_JPEG_EVENT_DEC_DONE & u16Value)
    || (E_JPEG_EVENT_DEC_ERROR_MASK & u16Value))
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsIdle : TRUE\n");
        return TRUE;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_IsIdle : FALSE\n");
    return FALSE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsIFrameFound(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return TRUE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsWithLowDelay(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return TRUE;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDecFrameInfo(MS_U32 u32Id, MJPEG_FrameInfo *pFrmInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if(FALSE == isDecFrameInfoValid[u8DrvId])
        return E_MJPEG_RET_FAIL;

    pFrmInfo->u32LumaAddr = infoDecFrame[u8DrvId].u32LumaAddr;
    pFrmInfo->u32ChromaAddr = infoDecFrame[u8DrvId].u32ChromaAddr;
    pFrmInfo->u32TimeStamp = infoDecFrame[u8DrvId].u32TimeStamp;
    pFrmInfo->u32ID_L = infoDecFrame[u8DrvId].u32ID_L;
    pFrmInfo->u32ID_H = infoDecFrame[u8DrvId].u32ID_H;
    pFrmInfo->u16Pitch = infoDecFrame[u8DrvId].u16Pitch;
    pFrmInfo->u16Width = infoDecFrame[u8DrvId].u16Width;
    pFrmInfo->u16Height = infoDecFrame[u8DrvId].u16Height;
    pFrmInfo->eFrameType = E_MJPEG_FRM_TYPE_I;

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDispFrameInfo(MS_U32 u32Id, MJPEG_FrameInfo *pFrmInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    #if ENABLE_AVSYNC_ID_CHECK
    static MS_U32 u32OldID_L = 0, u32OldID_H = 0, u32OldTimeStamp = 0;
    #endif
    MJPEG_FrameInfo info;
    MDrv_MJPEG_GetDispFrameInfo(u32Id, (void *)&info);

    pFrmInfo->u32LumaAddr = info.u32LumaAddr;
    pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
    pFrmInfo->u32TimeStamp = info.u32TimeStamp;
    pFrmInfo->u32ID_L = info.u32ID_L;
    pFrmInfo->u32ID_H = info.u32ID_H;
    pFrmInfo->u16Pitch = __u16Pitch[u8DrvId];
    pFrmInfo->u16Width = __u16Width[u8DrvId];
    pFrmInfo->u16Height = __u16Height[u8DrvId];
    pFrmInfo->eFrameType = E_MJPEG_FRM_TYPE_I;

    #if ENABLE_AVSYNC_ID_CHECK
    if (u32OldID_H != pFrmInfo->u32ID_H || u32OldID_L != pFrmInfo->u32ID_L
        || u32OldTimeStamp != pFrmInfo->u32TimeStamp)
    {
        printf ("ID 0x%08lx%08lx    PTS %ld     addr 0x%lx\n",
            pFrmInfo->u32ID_H, pFrmInfo->u32ID_L, pFrmInfo->u32TimeStamp, pFrmInfo->u32LumaAddr);
        u32OldID_H = pFrmInfo->u32ID_H;
        u32OldID_L = pFrmInfo->u32ID_L;
        u32OldTimeStamp = pFrmInfo->u32TimeStamp;
    }
    #endif

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MApi_MJPEG_DbgSetMsgLevel(MJPEG_DbgLevel eDbgLevel)
{
    MS_U8 u8Level;

    switch (eDbgLevel)
    {
    case E_MJPEG_DBG_LEVEL_ERR:
        u8Level = E_JPEG_DEBUG_ERR;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_INFO:
        u8Level = E_JPEG_DEBUG_ERR;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_MJPEG | E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_DBG:
        u8Level = E_JPEG_DEBUG_ALL;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_MJPEG | E_JPEG_DEBUG_API | E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_TRACE:
        u8Level = E_JPEG_DEBUG_ALL;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_MJPEG | E_JPEG_DEBUG_API | E_JPEG_DEBUG_ERR;
        break;
    case E_MJPEG_DBG_LEVEL_FW:
        u8Level = E_JPEG_DEBUG_NONE;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;
        break;
    case E_MJPEG_DBG_LEVEL_NONE:
    default:
        u8Level = E_JPEG_DEBUG_NONE;
        _u8MJPEG_ApiDbgLevel = E_JPEG_DEBUG_NONE;
        break;
    }

    MApi_JPEG_SetDbgLevel(u8Level);
    MDrv_MJPEG_SetDbgLevel(u8Level);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_DbgSetData(MS_U32 u32Id, MS_PHY u32Addr, MS_U32 u32Data)
{
    UNUSED(u32Id);
    u32Addr = u32Addr;
    u32Data = u32Data;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_DbgGetData(MS_U32 u32Id, MS_PHY u32Addr, MS_U32* u32Data)
{
    UNUSED(u32Id);
    u32Addr = u32Addr;
    *u32Data = 0;

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_DispFrame(MS_U32 u32Id, MS_U32 u32DispIdx)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MJPEG_DEBUG_API_MSG("MApi_MJPEG_DispFrame() with index=%ld\n", u32DispIdx);
    MDrv_MJPEG_DispFrame(u32Id, u32DispIdx);
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_FreeFrame(MS_U32 u32Id, MS_U32 u32DispIdx)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MDrv_MJPEG_FreeFrame(u32Id, u32DispIdx);
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetNextDispFrame(MS_U32 u32Id, MJPEG_FrameInfo *pFrmInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MJPEG_FrameInfo *pSrc = (MJPEG_FrameInfo*)MDrv_MJPEG_GetNextDispFrame(u32Id);
    if (pSrc != NULL)
    {
        _MJPEG_Memcpy(pFrmInfo, pSrc, sizeof(MJPEG_FrameInfo));
        pFrmInfo->u16Pitch = __u16Pitch[u8DrvId];
        pFrmInfo->u16Width = __u16Width[u8DrvId];
        pFrmInfo->u16Height = __u16Height[u8DrvId];
        pFrmInfo->eFrameType = E_MJPEG_FRM_TYPE_I;

        return E_MJPEG_RET_SUCCESS;
    }
    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetDispFrmNum(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return MDrv_MJPEG_GetDispFrameNum(u32Id);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetNextDispQPtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    return MDrv_MJPEG_GetNextDispQPtr(u32Id);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_EnableDispCmdQ(MS_U32 u32Id, MS_BOOL bEnable)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    bEnableDispCmdQ[u8DrvId] = bEnable;

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE, bEnable);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_PushDispCmdQ(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if (bEnableDispCmdQ[u8DrvId])
    {
    }

    return E_MJPEG_RET_FAIL;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MApi_MJPEG_GetDispCmdQVacancy( MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    if (bEnableDispCmdQ[u8DrvId])
    {
    }

    return 0;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SeekToPTS(MS_U32 u32Id, MS_U32 u32PTS)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    __bSeekToPts[u8DrvId] = TRUE;
    __u32Skip2PtsBase[u8DrvId] = MApi_MJPEG_GetPTS(u32Id);
    __u32TargetPts[u8DrvId] = u32PTS;
    MJPEG_DEBUG_API_MSG("   Seek from PTS %ld to %ld\n", __u32Skip2PtsBase[u8DrvId], __u32TargetPts[u8DrvId]);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_SkipToPTS(MS_U32 u32Id, MS_U32 u32PTS)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    __bSkipToPts[u8DrvId] = TRUE;
    __u32Skip2PtsBase[u8DrvId] = MApi_MJPEG_GetPTS(u32Id);
    __u32TargetPts[u8DrvId] = u32PTS;
    MJPEG_DEBUG_API_MSG("   Skip from PTS %ld to %ld\n", __u32Skip2PtsBase[u8DrvId], __u32TargetPts[u8DrvId]);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description : Enable/Disable FW turbo mode
///@param bEnable \b IN enable or not.
///@return NONE
/******************************************************************************/
void MApi_MJPEG_EnableTurboFWMode(MS_BOOL bEnable)
{
    MDrv_MJPEG_EnableTurboFWMode(bEnable);
}

static MJPEG_Result _MApi_MJPEG_SetDisplayOutSideMode(MS_U32 u32Id, MS_BOOL bOn)
{
    MJPEG_DEBUG_API_MSG("MJPEG _VDEC_EX_DISP_OUTSIDE_MODE!!!!!! with %d\n", bOn);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_DISPLAY_OUTSIDE_MODE, (MS_U32)bOn);

    return E_MJPEG_RET_SUCCESS;
}


MJPEG_Result MApi_MJPEG_SetControl(MS_U32 u32Id, MJPEG_VDEC_Cmd cmd_id, MS_U32 param)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MApi_JPEG_SetNJPDInstance(u8DrvId);
    }
    MJPEG_Result eRet = E_MJPEG_RET_INVALID_PARAMETER;
    switch(cmd_id)
    {
        case E_MJPEG_VDEC_CMD_SET_DISPLAY_OUTSIDE_MODE:
            eRet = _MApi_MJPEG_SetDisplayOutSideMode(u32Id, (MS_BOOL)param);
            break;
        default:
            break;

}
    return eRet;
}

/******************************************************************************/
///Description : GetFreeStream for Dual Decode
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetFreeStream(MS_U32 *pu32Id, MJPEG_API_StreamType eStreamType)
{
    MJPEG_Result eRet = E_MJPEG_RET_INVALID_PARAMETER;

    switch(eStreamType)
    {
        case E_MJPEG_API_MAIN_STREAM:
        {
            if (E_MJPEG_OK == MDrv_MJPEG_GetFreeStream(pu32Id, E_MJPEG_DRV_MAIN_STREAM))
                eRet = E_MJPEG_RET_SUCCESS;
            else
                eRet = E_MJPEG_RET_FAIL;

            break;
        }
        case E_MJPEG_API_SUB_STREAM:
        {
            if (E_MJPEG_OK == MDrv_MJPEG_GetFreeStream(pu32Id, E_MJPEG_DRV_SUB_STREAM))
                eRet = E_MJPEG_RET_SUCCESS;
            else
                eRet = E_MJPEG_RET_FAIL;

            break;
        }
        default:
            break;
    }

    return eRet;
}

#endif

