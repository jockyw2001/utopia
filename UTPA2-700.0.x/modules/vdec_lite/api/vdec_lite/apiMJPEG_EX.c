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
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)


#include "MsCommon.h"
// JPEG
#include "drvMJPEG.h"
#include "apiJPEG.h"
#include "../../../njpd/api/njpeg_ex/apiJPEG_priv.h"
#include "apiMJPEG_EX.h"
// RIU Base
#include "drvMMIO.h"
#include "halCHIP.h"

#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
#include "drvCMAPool.h"
#include "msos/linux/ion_uapi.h"
#include "msos/linux/mdrv_cma_pool_st.h"
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#undef printf
#define printf(format, args...)          printk(format, ##args);
#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

#define ENABLE_MJPEG_FIRMWARE           1

#define ENABLE_AVSYNC_ID_CHECK          0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define JPEGBASEWIDTHMAX    1920UL//1600
#define JPEGBASEHEIGHTMAX   1080UL//1200
#define JPEGPROWIDTHMAX     1024UL
#define JPEGPROHEIGHTMAX    768UL

#define MJPEG_SD_WIDTH      640UL
#define MJPEG_SD_HEIGHT     480UL

#define MJPEG_HD_800x600_WIDTH      800UL
#define MJPEG_HD_800x600_HEIGHT     600UL

#define MJPEG_MAX_SUPPORT_WIDTH     1920UL
#define MJPEG_MAX_SUPPORT_HEIGHT     1088UL

#define MAX_SUPPORTED_FRAME_RATE_1080P   30000UL

#define DISPLAY_QUEUE_NUM           8UL
#define MJPEG_INTERNAL_SIZE                         0x20000UL

#define SUPPORT_ASF_MJPEG_MODE                      TRUE
#define MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE       (0x400000UL*2)
#define VDEC_EX_MVD_PIC_START_FLAG                  0x40000000UL

#define SUPPORT_SKIP_ERROR_FRAME                    TRUE
#define SKIP_ERROR_FRAME_THRESHOLD                  1000UL //60UL
#define printf_red(args...)           do{ printf("\033[1;31m"); printf(args); printf("\033[m"); }while(0)
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
#define JPEG_memcpy(pDstAddr, pSrcAddr, u32Size) memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define JPEG_memset(pDstAddr, u8value, u32Size)  memset((pDstAddr), (u8value), (u32Size))

#ifndef UNUSED
#define UNUSED(x) (void)(x)
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


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL bEnableDispCmdQ[E_MJPEG_MJPEG_TOTAL];

//static MS_U32 __u32FwAddr = 0xFFFFFFFF;           // Address to load firmware
static MS_PHY __u32MRCBufAddr[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32MRCBufSize[E_MJPEG_MJPEG_TOTAL];
static MS_PHY __u32MWCBufAddr[E_MJPEG_MJPEG_TOTAL];
#if SUPPORT_ASF_MJPEG_MODE
static MS_PHY __u32CollectBufAddr[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bAsfMjpegMode[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bAsfFirstFrame[E_MJPEG_MJPEG_TOTAL];
static MS_BOOL __bAsfFirstPacketInFrame[E_MJPEG_MJPEG_TOTAL];
static MS_PHY __u32CollectBufWptr[E_MJPEG_MJPEG_TOTAL];
static MS_PHY __u32PrevBufAddr[E_MJPEG_MJPEG_TOTAL];
static MJPEG_COLLECT_BUF_STATUS __eColBufStatus[E_MJPEG_MJPEG_TOTAL][2];
#endif

static MS_PHY __u32InternalBufAddr[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32InternalBufSize[E_MJPEG_MJPEG_TOTAL];

static MS_PHY __u32ReadPtr[E_MJPEG_MJPEG_TOTAL];
static MS_PHY __u32WritePtr[E_MJPEG_MJPEG_TOTAL];


static MS_BOOL __bIsFirstFrameRdy[E_MJPEG_MJPEG_TOTAL];

static MS_U16 PreVIdx[E_MJPEG_MJPEG_TOTAL];
static MS_U16 ReCheckTime[E_MJPEG_MJPEG_TOTAL];

static MS_BOOL __bIsDecodeDone[E_MJPEG_MJPEG_TOTAL];
static MS_U32 __u32DropFrameCnt[E_MJPEG_MJPEG_TOTAL];
static MS_U32 u32NextFrameBuffIdx[E_MJPEG_MJPEG_TOTAL];
static MS_PHY u32EndAddr[E_MJPEG_MJPEG_TOTAL];

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
static MS_U32 _u32StartDecodeTime[2];
#endif

#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
static MS_BOOL bCMAUsed;
static MS_BOOL bCMAAllocDone;
static struct CMA_Pool_Init_Param cmaInitParam;
static struct CMA_Pool_Free_Param cmaFreeParam[E_MJPEG_MJPEG_TOTAL];
//static MS_BOOL bCMATwoMIU[E_MJPEG_MJPEG_TOTAL];
#endif

#ifdef VDEC3
static MS_BOOL __bFWdecideFB[E_MJPEG_MJPEG_TOTAL];
#endif

static MS_BOOL bNJPDNum = 0;
static MS_U8 u8MJPEGCnt = 0;
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

    MDrv_MJPEG_SetFrameCnt(u8DrvId, 0);
    MDrv_MJPEG_SetFrameTime(u8DrvId, 0);
    MDrv_MJPEG_SetFrameRate(u8DrvId, 600UL);
    MDrv_MJPEG_SetFrameRateBase(u8DrvId, 20UL);

    MDrv_MJPEG_SetWidth(u8DrvId, 0);
    MDrv_MJPEG_SetHeight(u8DrvId, 0);
    MDrv_MJPEG_SetPitch(u8DrvId, 0);

    MDrv_MJPEG_SetFrameBuffTotalNum(u8DrvId, 0);
    MDrv_MJPEG_SetFrameBuffUnitSize(u8DrvId, 0);

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
    _u32StartDecodeTime[bNJPDNum] = 0;
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

    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        _MApi_JPEG_SetNJPDInstance(bNJPDNum);
    }

    if (__bIsFirstFrameRdy[u8DrvId])
    {
        JPEG_Event reg_val = _MApi_JPEG_GetJPDEventFlag();
        MJPEG_DEBUG_API_MSG("        EventFlag = %d\n", reg_val);

        if(MApi_JPEG_IsNJPD()==FALSE)
        {
            MS_U16 cur_vidx = 0;
            //For H/W bug, some cases can not exit after decode done. Check Vidx to exit.
            if(PreVIdx[u8DrvId] != (cur_vidx = _MApi_JPEG_GetCurVidx()))
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
                    MJPEG_DEBUG_API_MSG("Set frame buffer index %td as Valid", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
                    __bIsDecodeDone[u8DrvId] = TRUE;
                    __u32DropFrameCnt[u8DrvId]++;

                    MJPEG_DEBUG_API_ERR("Decode timeout!!!!\n");
                    reg_val = E_JPEG_EVENT_DEC_ERROR_MASK;
                    return E_MJPEG_RET_FAIL;
                }
            }
        }

        if((E_JPEG_EVENT_DEC_DONE | E_JPEG_EVENT_DEC_ERROR_MASK) ==reg_val)
        {
            return E_MJPEG_RET_FAIL;
        }
        else if(E_JPEG_EVENT_DEC_DONE & reg_val)
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
                    MJPEG_DEBUG_API_MSG("Compensate PTS for %td dropped frames", (ptrdiff_t)__u32DropFrameCnt[u8DrvId]);
                }
                __u32DropFrameCnt[u8DrvId] = 0;

                MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY, u32NextFrameBuffIdx[u8DrvId]);
                MJPEG_DEBUG("[MJPEG]Decode done...Frame Buffer Index %td ready\n", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
                __bIsDecodeDone[u8DrvId] = TRUE;
            }
#endif
            MJPEG_DEBUG_API_MSG("\n");
            //__u32ReadPtr = u32EndAddr;
            MJPEG_DEBUG_API_MSG("Count %td\n", (ptrdiff_t)MDrv_MJPEG_GetFrameCnt(u8DrvId));
        }
        else
        if(E_JPEG_EVENT_DEC_ERROR_MASK & reg_val)
        {
            MJPEG_DEBUG_API_ERR("   JPD Decode Error\n");
            //MApi_MJPEG_Debug();

            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %td as Valid", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            _u32ErrCnt[u8DrvId]++;
            _MApi_JPEG_Rst();
            _MApi_JPEG_Exit();
            return E_MJPEG_RET_DECODE_ERROR;
        }
        else
        {
            MJPEG_DEBUG_API_MSG("   Still decoding previous JPEG ......\n");
#if SUPPORT_SKIP_ERROR_FRAME
            if(MsOS_GetSystemTime() - _u32StartDecodeTime[bNJPDNum] > SKIP_ERROR_FRAME_THRESHOLD)
            {
                MJPEG_DEBUG_API_ERR("%s[%d]: JPEG decode frame timeout!!!!!!!!!!\n", __FUNCTION__, u8DrvId);
                MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
                __u32DropFrameCnt[u8DrvId]++;
                _u32SkipCnt[u8DrvId]++;
                _u32ErrCnt[u8DrvId]++;
                // Repeat next frame because previous frame decode timeout
                u16NullCnt[u8DrvId]++;
                //MApi_MJPEG_Debug();
                MJPEG_DEBUG_API_MSG("%s: Reset JPD!!!!!!!!!!\n", __FUNCTION__);
                _MApi_JPEG_Rst();
                _MApi_JPEG_Exit();
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
    MJPEG_DEBUG_API_MSG("is able to push?.....%tx, %tx, %tx, %tx, %tx\n", (ptrdiff_t)__eColBufStatus[u8DrvId][0], (ptrdiff_t)__eColBufStatus[u8DrvId][1],
        (ptrdiff_t)__u32CollectBufWptr[u8DrvId], (ptrdiff_t)__u32CollectBufAddr[u8DrvId],
        (ptrdiff_t)(__u32CollectBufAddr[u8DrvId] + MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE/2));
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

static void _MApi_MJPEG_PushToLowBuffer(MS_U8 u32Id, MS_VIRT u32SrcAddrVA, MS_U32 u32Size)
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

static void _MApi_MJPEG_PushToHighBuffer(MS_U8 u32Id, MS_VIRT u32SrcAddrVA, MS_U32 u32Size)
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
    MS_VIRT u32SrcAddrVA = MsOS_PA2KSEG1(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]);

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
#if 0   // do not drop frame in all case (handled by vdplayer)
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

        if(_MApi_JPEG_GetWidth()<=MJPEG_SD_WIDTH && _MApi_JPEG_GetHeight()<=MJPEG_SD_HEIGHT)
        {
            return FALSE;
        }
        else if(_MApi_JPEG_GetWidth()<=MJPEG_MAX_SUPPORT_WIDTH && _MApi_JPEG_GetHeight()<=MJPEG_MAX_SUPPORT_HEIGHT
            && MDrv_MJPEG_GetFrameRate(u8DrvId) <= (MAX_SUPPORTED_FRAME_RATE_1080P))
        {
            return FALSE;
        }
        return TRUE;
    }
    else
#endif
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
///@return TRUE or FALSE
/******************************************************************************/
#ifdef VDEC3
MJPEG_Result MApi_MJPEG_Init(MS_U32 u32Id, MJPEG_INIT_PARAM *pInitParam, MS_BOOL bFWdecideFB)//, PJPEG_FILE_FileSystem_t pStream)
#else
MJPEG_Result MApi_MJPEG_Init(MS_U32 u32Id, MJPEG_INIT_PARAM *pInitParam)//, PJPEG_FILE_FileSystem_t pStream)
#endif
{
    JPEG_InitParam jpegInitParam;
    memset(&jpegInitParam,0x00,sizeof(JPEG_InitParam)); //CID 51825

    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

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
    MJPEG_FW_Info FWInfo;

    FWInfo.u32Size = pInitParam->u32FwSize;
    FWInfo.u32Addr = pInitParam->u32FwAddr;
    FWInfo.u32BinSize = pInitParam->u32FwBinSize;
    FWInfo.u32BinAddr = pInitParam->u32FwBinAddr;
    FWInfo.u8FwSrcType = pInitParam->eFwSrcType;
#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
    if(bCMAUsed && u8DrvId == 0)
    {
        FWInfo.u32MWCAddr = cmaInitParam.heap_miu_start_offset;
        FWInfo.u32MWCSize = cmaInitParam.heap_length;
        MJPEG_DEBUG("CMA used, reset address/size to [0x%tx, 0x%tx]\n", (ptrdiff_t)FWInfo.u32MWCAddr, (ptrdiff_t)FWInfo.u32MWCSize);
#ifdef VDEC3
        bFWdecideFB = TRUE;
#endif
    }
    else
#endif
    {
        FWInfo.u32MWCAddr = pInitParam->u32MWCBufAddr;
        FWInfo.u32MWCSize = pInitParam->u32MWCBufSize;
    }
#ifdef VDEC3
    FWInfo.bFWdecideFB = bFWdecideFB;
#endif
    if(E_MJPEG_OK != MDrv_MJPEG_InitFw(u32Id, FWInfo))
    {
        MJPEG_DEBUG_API_ERR("MDrv_MJPEG_InitFw Failed~~~~\n");
    }
    //__u32FwAddr = pInitParam->u32FwAddr;
    isDecFrameInfoValid[u8DrvId] = FALSE;
    bStepDec[u8DrvId] = FALSE;

    MJPEG_DEBUG_API_MSG("clear error code due to call MApi_MJPEG_Init()\n");
    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERROR_CODE_NONE);
    // Check if firmware is ready or not
    if (MDrv_MJPEG_IsFirmwareReady(u32Id) == FALSE)
    {
        MDrv_MJPEG_DeInit(u32Id);
        return E_MJPEG_RET_FAIL;
    }

#ifdef VDEC3
    __bFWdecideFB[u8DrvId] = bFWdecideFB;
#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
    if(bFWdecideFB || (bCMAUsed && u8DrvId == 0))
#else
    if(bFWdecideFB)
#endif
    {
        MJPEG_DEBUG("%s...FW decide FB!!! [%tdx%td]\n", __FUNCTION__, (ptrdiff_t)pInitParam->u32Width, (ptrdiff_t)pInitParam->u32Height);
        if(pInitParam->u32Width*pInitParam->u32Height ==0)
        {
            pInitParam->u32Width = MJPEG_MAX_SUPPORT_WIDTH;
            pInitParam->u32Height = MJPEG_MAX_SUPPORT_HEIGHT;
        }
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_WIDTH, pInitParam->u32Width);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_HEIGHT, pInitParam->u32Height);

        if(MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_GET_FRAME_BUFFER, 0))
        {
            pInitParam->u32MWCBufAddr = MDrv_MJPEG_GetFrameBuffStAddr(u8DrvId);
            pInitParam->u32MWCBufSize= MDrv_MJPEG_GetFrameBuffTotalSize(u8DrvId);
#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
            if(bCMAUsed && u8DrvId == 0)
            {
                if(cmaInitParam.miu==1)
                {
                    MJPEG_DEBUG("[MJPEG]CMA in MIU1!!!\n");
                    pInitParam->u32MWCBufAddr += HAL_MIU1_BASE;
                }
#ifdef HAL_MIU2_BASE
                else if(cmaInitParam.miu==2)
                {
                    MJPEG_DEBUG("[MJPEG]CMA in MIU2!!!\n");
                    pInitParam->u32MWCBufAddr += HAL_MIU2_BASE;
                }
#endif
            }
#endif
        }
        else
        {
            MJPEG_DEBUG_API_ERR("MDrv_MJPEG_GetFrameBuffer Failed~~~~\n");
            return E_MJPEG_RET_FAIL;
        }
#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
        if(bCMAUsed && u8DrvId == 0)
        {
            MS_PHY u32Addr;
            MS_U32 u32Size;
            MS_BOOL bAllocateDone;
            struct CMA_Pool_Alloc_Param alloc_param;
            MDrv_MJPEG_GetCMAInformation(u32Id,&u32Addr,&u32Size,&bAllocateDone);
            MJPEG_DEBUG("MDrv_MJPEG_GetCMAInformation() u32Addr=0x%tx, u32Size=0x%tx, bAllocateDone=%d!!!\n", (ptrdiff_t)u32Addr, (ptrdiff_t)u32Size, bAllocateDone);

            if(u32Addr == 0 || u32Size == 0)
            {
                return FALSE;
            }
            else
            {
                alloc_param.pool_handle_id = cmaInitParam.pool_handle_id;
                alloc_param.offset_in_pool = u32Addr - cmaInitParam.heap_miu_start_offset;
                alloc_param.length = u32Size;
                alloc_param.flags = CMA_FLAG_VIRT_ADDR;

                if (MApi_CMA_Pool_GetMem(&alloc_param) == FALSE)
                {
                    MJPEG_DEBUG("MApi_CMA_Pool_GetMem() fail!!!\n");
                    return FALSE;
                }

                VPRINTF("[VDEC][%d]MApi_CMA_Pool_GetMem in mjpeg: alloc_param.pool_handle_id=%x, alloc_param.flags=%x, alloc_param.offset_in_pool=%llx, alloc_param.length=%x\n",
                                (unsigned int)u8DrvId,
                                (unsigned int)alloc_param.pool_handle_id,
                                (unsigned int)alloc_param.flags,
                                (unsigned long long int)alloc_param.offset_in_pool,
                                (unsigned int)alloc_param.length);

                cmaFreeParam[u8DrvId].pool_handle_id = alloc_param.pool_handle_id;
                cmaFreeParam[u8DrvId].offset_in_pool = alloc_param.offset_in_pool;
                cmaFreeParam[u8DrvId].length = alloc_param.length;

                bCMAAllocDone = TRUE;
            }
        }
#endif
    }
#endif

    if((pInitParam->u32MWCBufAddr)%8)
    {
        MJPEG_DEBUG_API_MSG("u32MWCBufAddr need 8 byte alignment~~~~0x%tx\n", (ptrdiff_t)pInitParam->u32MWCBufAddr);
    }
    else
    {
        MJPEG_DEBUG_API_MSG("u32MWCBufAddr don't need 8 byte alignment~~~~0x%tx\n", (ptrdiff_t)pInitParam->u32MWCBufAddr);
    }

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR, pInitParam->u32MWCBufAddr);

#if SUPPORT_ASF_MJPEG_MODE
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        if((pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)
            >= MJPEG_MAX_SUPPORT_WIDTH*MJPEG_MAX_SUPPORT_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            MDrv_MJPEG_SetFrameBuffTotalNum(u8DrvId, DISPLAY_QUEUE_NUM);
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
            MDrv_MJPEG_SetFrameBuffTotalNum(u8DrvId, DISPLAY_QUEUE_NUM);
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for asf-mjpeg mode support!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    MDrv_MJPEG_SetFrameBuffUnitSize(u8DrvId, (pInitParam->u32MWCBufSize-MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE)/MDrv_MJPEG_GetFrameBuffTotalNum(u8DrvId));
#else
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        if(pInitParam->u32MWCBufSize >= MJPEG_MAX_SUPPORT_WIDTH*MJPEG_MAX_SUPPORT_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            MDrv_MJPEG_SetFrameBuffTotalNum(u8DrvId, DISPLAY_QUEUE_NUM);
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for FHD mjpeg!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    else
    {
        if(pInitParam->u32MWCBufSize >= MJPEG_SD_WIDTH*MJPEG_SD_HEIGHT*DISPLAY_QUEUE_NUM)
        {
            MDrv_MJPEG_SetFrameBuffTotalNum(u8DrvId, DISPLAY_QUEUE_NUM);
        }
        else
        {
            MJPEG_DEBUG_API_ERR("Not enough framebuffer for mjpeg!!\n");
            return E_MJPEG_RET_FAIL;
        }
    }
    MDrv_MJPEG_SetFrameBuffUnitSize(u8DrvId, pInitParam->u32MWCBufSize/MDrv_MJPEG_GetFrameBuffTotalNum(u8DrvId));
#endif
    MDrv_MJPEG_SetFrameBuffUnitSize(u8DrvId, (MDrv_MJPEG_GetFrameBuffUnitSize(u8DrvId) >> 3) << 3);//8 byte alignment

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE, MDrv_MJPEG_GetFrameBuffUnitSize(u8DrvId));
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM, MDrv_MJPEG_GetFrameBuffTotalNum(u8DrvId));
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAMERATE, pInitParam->u32FrameRate);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAMERATE_BASE, pInitParam->u32FrameRateBase);
//    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_CHIP_ID, MDrv_MJPEG_Get_Chip_ID());

    if(pInitParam->u32FrameRate ==0)
    {
        MJPEG_DEBUG_API_ERR("Error!!! MJPEG Frame rate is 0\n");
        pInitParam->u32FrameRate =30000;
    }
    MDrv_MJPEG_SetFrameRate(u8DrvId, pInitParam->u32FrameRate);
    MDrv_MJPEG_SetFrameRateBase(u8DrvId, pInitParam->u32FrameRateBase);

    MDrv_MJPEG_SetFrameTime(u8DrvId, MDrv_MJPEG_GetFrameRateBase(u8DrvId) * 1000 / MDrv_MJPEG_GetFrameRate(u8DrvId));
#endif // ENABLE_MJPEG_FIRMWARE

    MDrv_MJPEG_SetFrameCnt(u8DrvId, 0);
    __u32DropFrameCnt[u8DrvId] = 0;

    __u32MRCBufAddr[u8DrvId] = pInitParam->u32MRCBufAddr;
    __u32MRCBufSize[u8DrvId] = pInitParam->u32MRCBufSize;
    __u32MWCBufAddr[u8DrvId] = pInitParam->u32MWCBufAddr;
#if SUPPORT_ASF_MJPEG_MODE
    __u32CollectBufAddr[u8DrvId] = pInitParam->u32MWCBufAddr+pInitParam->u32MWCBufSize - MJPEG_RESERVED_MJPEG_COLLECT_BUF_SIZE;
    __u32CollectBufWptr[u8DrvId] = __u32CollectBufAddr[u8DrvId];
#endif

    __u32InternalBufAddr[u8DrvId] = pInitParam->u32MRCBufAddr;
    __u32InternalBufSize[u8DrvId] = pInitParam->u32MRCBufSize;


    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        if(u8MJPEGCnt==0)
        {
            _MApi_JPEG_SetNJPDInstance(0xCC);
        }
    }
    else
    {
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
        // This is used to init memory
        if (_MApi_JPEG_Init(&jpegInitParam) == E_JPEG_FAILED)
        {
    #if ENABLE_MJPEG_FIRMWARE
            MDrv_MJPEG_DeInit(u32Id);
    #endif
            return E_MJPEG_RET_FAIL;
        }

        // Make sure it's exited correctly
        _MApi_JPEG_Exit();
   }

    MJPEG_DEBUG("MJPEG init================================\n");
    MJPEG_DEBUG("[MRC]addr=0x%tx, size=0x%tx\n", (ptrdiff_t)__u32MRCBufAddr[u8DrvId], (ptrdiff_t)pInitParam->u32MRCBufSize);
    MJPEG_DEBUG("[MWC]addr=0x%tx, size=0x%tx\n", (ptrdiff_t)__u32MWCBufAddr[u8DrvId], (ptrdiff_t)pInitParam->u32MWCBufSize);
    MJPEG_DEBUG("[INTER]addr=0x%tx, size=0x%tx\n", (ptrdiff_t)__u32InternalBufAddr[u8DrvId], (ptrdiff_t)__u32InternalBufSize[u8DrvId]);
    MJPEG_DEBUG("[MRC VA]addr=0x%tx, size=0x%tx\n", (ptrdiff_t)MS_PA2KSEG1(__u32MRCBufAddr[u8DrvId]), (ptrdiff_t)pInitParam->u32MRCBufSize);
    MJPEG_DEBUG("[MWC VA]addr=0x%tx, size=0x%tx\n", (ptrdiff_t)MS_PA2KSEG1(__u32MWCBufAddr[u8DrvId]), (ptrdiff_t)pInitParam->u32MWCBufSize);
    MJPEG_DEBUG("[INTER VA]addr=0x%tx, size=0x%tx\n", (ptrdiff_t)MS_PA2KSEG1(__u32InternalBufAddr[u8DrvId]), (ptrdiff_t)__u32InternalBufSize[u8DrvId]);
    MJPEG_DEBUG("===========================================\n");

    // Init Local Variables
    PreVIdx[u8DrvId] = 0;
    ReCheckTime[u8DrvId] = 0;
    bLstFrame[u8DrvId] = FALSE;

    u8MJPEGCnt++;
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Reset JPD
/******************************************************************************/
MJPEG_Result MApi_MJPEG_Rst(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    __bIsFirstFrameRdy[u8DrvId] = FALSE;

#if 0
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        _MApi_JPEG_SetNJPDInstance(0);
    }
    _MApi_JPEG_Rst();
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        _MApi_JPEG_SetNJPDInstance(1);
    }
    _MApi_JPEG_Rst();
#endif
    MJPEG_DEBUG_API_MSG("clear error code due to call MApi_MJPEG_Rst()\n");
    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERROR_CODE_NONE);

    // CHECK : reset firmware???

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Turn off JPD clock
/******************************************************************************/
//extern void _MApi_JPEG_DisableAddressConvert(void); // defined in apiJPEG.c
MJPEG_Result MApi_MJPEG_Stop(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

    __bIsFirstFrameRdy[u8DrvId] = FALSE;
    isDecFrameInfoValid[u8DrvId] = FALSE;
    bStepDec[u8DrvId] = FALSE;

#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
    if (bCMAUsed && u8DrvId == 0)
    {
        if (MApi_CMA_Pool_PutMem(&cmaFreeParam[u8DrvId]) == FALSE)
        {
            return E_MJPEG_RET_FAIL;
        }

        VPRINTF("[VDEC][%d]MApi_CMA_Pool_PutMem in mjpeg: cmaFreeParam.pool_handle_id=%x, cmaFreeParam.offset_in_pool=%llx, cmaFreeParam.length=%x\n",
                                (unsigned int)u8DrvId,
                                (unsigned int)cmaFreeParam[u8DrvId].pool_handle_id,
                                (unsigned long long int)cmaFreeParam[u8DrvId].offset_in_pool,
                                (unsigned int)cmaFreeParam[u8DrvId].length);
        MJPEG_DEBUG("[MJPEG][0x%tx]MApi_CMA_Pool_PutMem: offset=0x%tx, len=0x%tx\n", (ptrdiff_t)u32Id, (ptrdiff_t)cmaFreeParam[u8DrvId].offset_in_pool, (ptrdiff_t)cmaFreeParam[u8DrvId].length);
    }
#endif

#ifdef VDEC3
    if(__bFWdecideFB[u8DrvId])
    {
        MJPEG_DEBUG("%s...release FB by FW!!!\n", __FUNCTION__);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_FREE_FRAME_BUFFER, 0);
    }
#endif
    MDrv_MJPEG_StopVPU(u32Id);

    MDrv_MJPEG_DeInit(u32Id);

#if 0
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        _MApi_JPEG_SetNJPDInstance(0);
    }

    _MApi_JPEG_DisableAddressConvert();

    //_MApi_JPEG_PowerOff() is called in _MApi_JPEG_Exit()
    _MApi_JPEG_Exit();

    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        _MApi_JPEG_SetNJPDInstance(1);
    }

    _MApi_JPEG_DisableAddressConvert();

    //_MApi_JPEG_PowerOff() is called in _MApi_JPEG_Exit()
    _MApi_JPEG_Exit();
#endif

    if(u8MJPEGCnt)
    {
        u8MJPEGCnt--;
    }
    else
    {
        MJPEG_DEBUG_API_ERR("Warning!!! u8MJPEGCnt is 0 and cannot be decreased!!\n");
    }
    if(MApi_JPEG_IsNJPD()==TRUE && u8MJPEGCnt==0)
    {
        MJPEG_DEBUG("MJPEG all close!!!!!!!!!!!!!!!!!!\n");
        _MApi_JPEG_SetNJPDInstance(0xdd);
        _MApi_JPEG_Exit();
    }
    else if(MApi_JPEG_IsNJPD()==FALSE)
    {
        MJPEG_DEBUG("[OJPD]_MApi_JPEG_Exit()\n");
        _MApi_JPEG_Exit();
    }
    // CHECK : stop firmware???
    MJPEG_DEBUG("MApi_MJPEG_Stop()...done\n");

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if first frame has been decoded
///@return TRUE if first picture is decoded, else return FALSE
/******************************************************************************/
MJPEG_Result MApi_MJPEG_CheckDispInfoRdy(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    if ((TRUE == __bIsFirstFrameRdy[u8DrvId])
    && (E_JPEG_EVENT_DEC_DONE & _MApi_JPEG_GetJPDEventFlag()))
    {
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
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_PLAY, 0xFFFFFFFFUL) == 0)
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
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_PAUSE, 0xFFFFFFFFUL) == 0)
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
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_RESUME, 0xFFFFFFFFUL) == 0)
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
    MDrv_MJPEG_SetStepDispPara(u32Id);
    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_STEP_PLAY, 0xFFFFFFFFUL) == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_StepPlay : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }

    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_StepPlay : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Check if step play is done or not
///@return TRUE or FALSE
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsStepPlayDone(MS_U32 u32Id)
{
    return (!MDrv_MJPEG_IsStepPlayDone(u32Id));
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
        _MApi_JPEG_SetNJPDInstance(bNJPDNum);
    }

    MJPEG_DEBUG("[MJPEG]MApi_MJPEG_PushQueue() pts=%td\n", (ptrdiff_t)pInfo->u32TimeStamp);
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
        _MApi_JPEG_Rst();
        _MApi_JPEG_Exit();
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
            MJPEG_DEBUG("\033[1;31m[MWC][%dx%d.yuyv]addr=0x%tx, size=0x%tx, [r:0x%tx, w:0x%tx]\033[m\n",
                _MApi_JPEG_GetWidth(), _MApi_JPEG_GetHeight(), (ptrdiff_t)infoDecFrame[u8DrvId].u32LumaAddr,
                (ptrdiff_t)(_MApi_JPEG_GetWidth()*_MApi_JPEG_GetHeight()*2),
                (ptrdiff_t)__u32ReadPtr[u8DrvId], (ptrdiff_t)__u32WritePtr[u8DrvId]);
        }
        else
        {
            MJPEG_DEBUG("[MWC][%dx%d.yuyv]addr=0x%tx, size=0x%tx, [r:0x%tx, w:0x%tx]\n",
                _MApi_JPEG_GetWidth(), _MApi_JPEG_GetHeight(), (ptrdiff_t)infoDecFrame[u8DrvId].u32LumaAddr,
                (ptrdiff_t)(_MApi_JPEG_GetWidth()*_MApi_JPEG_GetHeight()*2),
                (ptrdiff_t)__u32ReadPtr[u8DrvId], (ptrdiff_t)__u32WritePtr[u8DrvId]);
        }
        if(_u8MJPEG_ApiDbgLevel & E_JPEG_DEBUG_MJPEG)
        {
            MDrv_MJPEG_PrintShareInfo(u32Id);
        }
#endif

        __u32ReadPtr[u8DrvId] = u32EndAddr[u8DrvId];
        MJPEG_DEBUG_API_MSG("%s: Count %td, __bSkipToPts=%td, __bSeekToPts=%td, __u32Skip2PtsBase=%td, __u32TargetPts=%td\n",
            __FUNCTION__, (ptrdiff_t)MDrv_MJPEG_GetFrameCnt(u8DrvId), (ptrdiff_t)__bSkipToPts[u8DrvId], (ptrdiff_t)__bSeekToPts[u8DrvId], (ptrdiff_t)__u32Skip2PtsBase[u8DrvId], (ptrdiff_t)__u32TargetPts[u8DrvId]);


        __u32WritePtr[u8DrvId] = pInfo->u32StAddr + pInfo->u32Size - 1;


        MJPEG_DEBUG("[MJPEG]Pushed done, [r:0x%tx, w:0x%tx, u8DrvId=%td]\n", (ptrdiff_t)__u32ReadPtr[u8DrvId], (ptrdiff_t)__u32WritePtr[u8DrvId], (ptrdiff_t)u8DrvId);

        // Skip/Seek to PTS
        if (__bSkipToPts[u8DrvId] || __bSeekToPts[u8DrvId])
        {
            if (__u32Skip2PtsBase[u8DrvId] >= __u32TargetPts[u8DrvId])
            {
                MJPEG_DEBUG_API_MSG("Skip/Seek to PTS done -- current PTS %td\n", (ptrdiff_t)__u32Skip2PtsBase[u8DrvId]);
                __bSkipToPts[u8DrvId] = FALSE;
                __bSeekToPts[u8DrvId] = FALSE;
                // Need to update firmware PTS base
                MDrv_MJPEG_ResetPTS(u32Id, __u32Skip2PtsBase[u8DrvId]);
            }
            else
            {
                MJPEG_DEBUG_API_MSG("Skip Frame -- PTS %td\n", (ptrdiff_t)__u32Skip2PtsBase[u8DrvId]);
                __u32Skip2PtsBase[u8DrvId] += MDrv_MJPEG_GetFrameTime(u8DrvId);
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
                if ((MDrv_MJPEG_GetFrameCnt(u8DrvId) - __u32SpeedCtrlFrameCntBase[u8DrvId]) % eCurDispSpeed[u8DrvId])
                {
                    MJPEG_DEBUG_API_MSG("       Skip frame %td\n", (ptrdiff_t)(MDrv_MJPEG_GetFrameCnt(u8DrvId) - __u32SpeedCtrlFrameCntBase[u8DrvId]));
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
                __u32SpeedCtrlFrameCntBase[u8DrvId] = MDrv_MJPEG_GetFrameCnt(u8DrvId);
                MJPEG_DEBUG_API_MSG("   Update Frame Count Base %td\n", (ptrdiff_t)__u32SpeedCtrlFrameCntBase[u8DrvId]);
            }
        }


        _MApi_JPEG_SetMaxDecodeResolution(JPEGBASEWIDTHMAX, JPEGBASEHEIGHTMAX);


        MJPEG_DEBUG_API_MSG("pInfo->u32StAddr=0x%tx, __u32MRCBufAddr=0x%tx\n", (ptrdiff_t)pInfo->u32StAddr, (ptrdiff_t)__u32MRCBufAddr[u8DrvId]);
        MJPEG_DEBUG_API_MSG("pInfo->u32Size=0x%tx\n", (ptrdiff_t)pInfo->u32Size);
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
            MS_U8 u8Cnt;
            for(u8Cnt=0; u8Cnt<(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr); u8Cnt++)
            {
                if(*((MS_U8*)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr+u8Cnt))==0xff && *((MS_U8*)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr+u8Cnt+1))==0xd9)
                {
                     *((MS_U8*)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr+u8Cnt+1)) = 0xff;
                }
            }
//            memset((void *)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr), 0, (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr));
            jpegInitParam.u32MRCBufSize = ((pInfo->u32Size+(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr)+0xf) & ~0x0f)*2;
        }
#else
        jpegInitParam.u32MRCBufAddr = (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]) & ~0x0f;
        // Tricky code: let JPD decode only L buffer, then decode dne
        MS_U8 u8Cnt;
        for(u8Cnt=0; u8Cnt<(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr); u8Cnt++)
        {
            if(*((MS_U8*)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr+u8Cnt))==0xff && *((MS_U8*)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr+u8Cnt+1))==0xd9)
            {
                 *((MS_U8*)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr+u8Cnt+1)) = 0xff;
            }
        }
//        memset((void *)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr), 0, (pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr));
        jpegInitParam.u32MRCBufSize = ((pInfo->u32Size+(pInfo->u32StAddr + __u32MRCBufAddr[u8DrvId]-jpegInitParam.u32MRCBufAddr)+0xf) & ~0x0f)*2;
#endif
        if (jpegInitParam.u32MRCBufSize < JPEG_DEFAULT_EXIF_SIZE)
        {   // Meet size check in _MApi_JPEG_Init()
            jpegInitParam.u32MRCBufSize = JPEG_DEFAULT_EXIF_SIZE;
        }

        // Ask firmware if there is any frame buffer available
        u32NextFrameBuffIdx[u8DrvId] = MDrv_MJPEG_GetNextFrameBufferIdx(u32Id);

        MJPEG_DEBUG("[MJPEG]              E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX %td\n", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
        if (u32NextFrameBuffIdx[u8DrvId] >= 0xFFUL)// It may be 0xFF, 0xFFFF, 0xFFFFFF, or 0xFFFFFFFF
        {
            MJPEG_DEBUG_API_ERR("   !!! No frame buffer available !!!, u32NextFrameBuffIdx=0x%tx\n", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
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
        MDrv_MJPEG_SetPushQPara(u32Id, pInfo->u32TimeStamp, pInfo->u32ID_L, pInfo->u32ID_H, u32NextFrameBuffIdx[u8DrvId]);
        MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_PUSH_QUEUE_PARA_SETTING, (MS_VIRT)NULL);

        jpegInitParam.u32MWCBufAddr = __u32MWCBufAddr[u8DrvId] + MDrv_MJPEG_GetFrameBuffUnitSize(u8DrvId)*u32NextFrameBuffIdx[u8DrvId];
        MJPEG_DEBUG_API_MSG("   Set Frame buffer addr 0x%tx\n", (ptrdiff_t)jpegInitParam.u32MWCBufAddr);
        jpegInitParam.u32MWCBufSize = MDrv_MJPEG_GetFrameBuffUnitSize(u8DrvId);
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
        printf ("ID 0x%08lx%08lx    PTS %d\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp);
        #endif

        // Make sure it's exited correctly
        _MApi_JPEG_Exit();


        u32EndAddr[u8DrvId] = pInfo->u32StAddr + pInfo->u32Size - 1;
        MJPEG_DEBUG("[MWC]: PA=0x%tx, size=0x%tx, VA=0x%tx\n", (ptrdiff_t)jpegInitParam.u32MWCBufAddr, (ptrdiff_t)jpegInitParam.u32MWCBufSize, (ptrdiff_t)MS_PA2KSEG1(jpegInitParam.u32MWCBufAddr));
        MJPEG_DEBUG("[MRC]: PA=0x%tx, size=0x%tx, VA=0x%tx\n", (ptrdiff_t)jpegInitParam.u32MRCBufAddr, (ptrdiff_t)jpegInitParam.u32MRCBufSize, (ptrdiff_t)MS_PA2KSEG1(jpegInitParam.u32MRCBufAddr));
        MJPEG_DEBUG("[INTER]: PA=0x%tx, size=0x%tx, VA=0x%tx\n", (ptrdiff_t)jpegInitParam.u32InternalBufAddr, (ptrdiff_t)jpegInitParam.u32InternalBufSize, (ptrdiff_t)MS_PA2KSEG1(jpegInitParam.u32InternalBufAddr));
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

        if (_MApi_JPEG_Init(&jpegInitParam) == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("_MApi_JPEG_Init() FAIL\n");
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %td as Valid", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            _u32ErrCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;
            _MApi_JPEG_Rst();
            _MApi_JPEG_Exit();
            return E_MJPEG_RET_FAIL;
        }

        // Get width, height, & pitch of 1st frame
        if (__bIsFirstFrameRdy[u8DrvId] == FALSE)
        {
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_WIDTH, _MApi_JPEG_GetWidth());
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_HEIGHT, _MApi_JPEG_GetHeight());
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_PITCH, _MApi_JPEG_GetAlignedPitch());
            MJPEG_DEBUG_API_MSG("       width %d, height %d, pitch %d\n", MDrv_MJPEG_GetWidth(u8DrvId),
                MDrv_MJPEG_GetHeight(u8DrvId), MDrv_MJPEG_GetPitch(u8DrvId));

            if(MApi_JPEG_IsNJPD()==TRUE)
            {
                if((MDrv_MJPEG_GetWidth(u8DrvId) > MJPEG_MAX_SUPPORT_WIDTH) ||
                    (MDrv_MJPEG_GetHeight(u8DrvId)>MJPEG_MAX_SUPPORT_HEIGHT))
                {
                    MJPEG_DEBUG_API_MSG("Set MJPEG error code=E_MJPEG_ERR_CODE_RES \n");
                    _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERR_CODE_RES);
                }
            }
            else
            {
                if((MDrv_MJPEG_GetWidth(u8DrvId) > MJPEG_SD_WIDTH) ||
                    (MDrv_MJPEG_GetHeight(u8DrvId)>MJPEG_SD_HEIGHT))
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
        infoDecFrame[u8DrvId].u16Pitch = MDrv_MJPEG_GetPitch(u8DrvId);
        infoDecFrame[u8DrvId].u16Width = MDrv_MJPEG_GetWidth(u8DrvId);
        infoDecFrame[u8DrvId].u16Height = MDrv_MJPEG_GetHeight(u8DrvId);
        isDecFrameInfoValid[u8DrvId] = TRUE;

        // Decode header
        if (_MApi_JPEG_DecodeHdr() == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("_MApi_JPEG_DecodeHdr() FAIL\n");
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %td as Valid", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            _u32ErrCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;

            JPEG_ErrCode jpeg_errcode = _MApi_JPEG_GetErrorCode();
            if((E_JPEG_UNSUPPORTED_COLORSPACE == jpeg_errcode)
            || (E_JPEG_UNSUPPORTED_SAMP_FACTORS == jpeg_errcode)
            || (E_JPEG_UNSUPPORTED_HUFF_DECODE == jpeg_errcode))
            {
                MJPEG_DEBUG_API_ERR("Set MJPEG error code=E_MJPEG_ERR_CODE_RES for unsupport!!\n");
                _MApi_MJPEG_SetErrCode(u32Id, E_MJPEG_ERR_CODE_RES);
            }
            _MApi_JPEG_Rst();
            _MApi_JPEG_Exit();

            return E_MJPEG_RET_DECODE_HDR_ERROR;
        }

        bStepDec[u8DrvId] = TRUE;

        // Decode
        if (_MApi_JPEG_Decode() == E_JPEG_FAILED)
        {
            MJPEG_DEBUG_API_ERR("_MApi_JPEG_Decode() FAIL\n");
            MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID, u32NextFrameBuffIdx[u8DrvId]);
            MJPEG_DEBUG_API_MSG("Set frame buffer index %td as Valid", (ptrdiff_t)u32NextFrameBuffIdx[u8DrvId]);
            __bIsDecodeDone[u8DrvId] = TRUE;
            __u32DropFrameCnt[u8DrvId]++;
            u16NullCnt[u8DrvId]=0;
            return E_MJPEG_RET_DECODE_ERROR;
        }

        if(_MApi_NJPD_JPDCount()==2)
        {
            bNJPDNum = !bNJPDNum ;
            MJPEG_DEBUG("bNJPDNum=%d\n", bNJPDNum);
        }

        if(MApi_JPEG_IsNJPD()==TRUE)
        {
            _MApi_JPEG_SetNJPDInstance(bNJPDNum);
        }

#if SUPPORT_SKIP_ERROR_FRAME
        _u32StartDecodeTime[bNJPDNum] = MsOS_GetSystemTime();
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
            MJPEG_DEBUG_API_MSG("Wait for Fill decode frame info u32LumaAddr=0x%tx, u32ChromaAddr=0x%tx\n",
                (ptrdiff_t)infoDecFrame[u8DrvId].u32LumaAddr, (ptrdiff_t)infoDecFrame[u8DrvId].u32ChromaAddr);
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
            if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL, 0xFFFFFFFFUL) == 1)
            {
                MJPEG_DEBUG_API_MSG("[MJPEG]            Display queue is full\n");
                MJPEG_DEBUG_API_MSG("[MJPEG]           MApi_MJPEG_GetQueueVacancy : 0\n");
                return 0;
            }

            if(MDrv_MJPEG_GetNextFrameBufferIdx(u32Id)>=0xff)
            {
                MJPEG_DEBUG_API_MSG("[MJPEG]           MApi_MJPEG_GetQueueVacancy : 0\n");
                return 0;
            }
            else
            {
                MJPEG_DEBUG_API_MSG("[MJPEG]           MApi_MJPEG_GetQueueVacancy : 1\n");
                return 1;
            }
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
    MS_U32 u32Ret;
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
    u32Ret = MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_FLUSH_DISP_QUEUE, 0xFFFFFFFFUL);
    if (u32Ret == 0)
    {
        MJPEG_DEBUG_API_MSG("           MApi_MJPEG_FlushQueue : E_MJPEG_RET_FAIL\n");
        return E_MJPEG_RET_FAIL;
    }
    else if(u32Ret == 2)
    {
        MJPEG_DEBUG_API_MSG("MApi_MJPEG_FlushQueue return SUCCESS\n");
        return E_MJPEG_RET_SUCCESS;
    }

    while(MDrv_MJPEG_IsFlushDone(u32Id)==0);
    MJPEG_DEBUG_API_MSG("           MApi_MJPEG_FlushQueue : E_MJPEG_RET_SUCCESS\n");
    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_PHY MApi_MJPEG_GetESWritePtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    // return end of data
    return __u32WritePtr[u8DrvId];
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_PHY MApi_MJPEG_GetESReadPtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
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
    bLstFrame[u8DrvId] = bEnable;
    // This function always returns TRUE.
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

    if (MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_IS_DISP_FINISH, 0xFFFFFFFFUL) == 0)
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
    eCurSpeedType[u8DrvId] = eSpeedType;
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_SPEED_TYPE, (MS_U32)eSpeedType);

    eCurDispSpeed[u8DrvId] = eDispSpeed;
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_SPEED, (MS_U32)eDispSpeed);

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_STC_BASE, 0xFFFFFFFFUL);

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
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_BLUE_SCREEN, (MS_U32)bOn);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Reset PTS value
///@param u32Value \b IN new PTS value
/******************************************************************************/
MJPEG_Result MApi_MJPEG_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase)
{
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
    return MDrv_MJPEG_IsAVSyncOn(u32Id);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsReachSync(MS_U32 u32Id)
{
    return MDrv_MJPEG_IsReachAVSync(u32Id);
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_BOOL MApi_MJPEG_GetStatus(MS_U32 u32Id, MJPEG_Status *pStatus)
{
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
    MJPEG_DEBUG_API_MSG("[MJPEG] _u32ErrCnt[u8DrvId]=%td\n", (ptrdiff_t)_u32ErrCnt[u8DrvId]);

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
    return MDrv_MJPEG_GetFrameCnt(u8DrvId);
}

/******************************************************************************/
///Return video information
///@param pInfo \b OUT video information: width & height
/******************************************************************************/
MJPEG_Result MApi_MJPEG_GetDispInfo(MS_U32 u32Id, MJPEG_DISP_INFO *pInfo)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    // APP wants pitch to setup display
    // This should be used with crop information

    MJPEG_DEBUG_API_MSG("MApi_MJPEG_GetDispInfo() with u16Pitch=%d, u16Height=%d, u16Width=%d\n",
    MDrv_MJPEG_GetPitch(u8DrvId), MDrv_MJPEG_GetHeight(u8DrvId), MDrv_MJPEG_GetWidth(u8DrvId));
    pInfo->u16Width = MDrv_MJPEG_GetPitch(u8DrvId);
    pInfo->u16Height = MDrv_MJPEG_GetHeight(u8DrvId);
    pInfo->u16Pitch = MDrv_MJPEG_GetPitch(u8DrvId);
    if(MDrv_MJPEG_GetFrameRateBase(u8DrvId) !=0)
    {
        pInfo->u32FrameRate = MDrv_MJPEG_GetFrameRate(u8DrvId) * 1000 / MDrv_MJPEG_GetFrameRateBase(u8DrvId);
    }
    else
    {
        MJPEG_DEBUG_API_ERR("Error!!! u32FrameRateBase==0!!!!!\n");
    }

    pInfo->u16CropLeft = 0;
    pInfo->u16CropRight = pInfo->u16Width - MDrv_MJPEG_GetWidth(u8DrvId) - pInfo->u16CropLeft;
    pInfo->u16CropTop = (pInfo->u16Height - MDrv_MJPEG_GetHeight(u8DrvId))/2;
    pInfo->u16CropBottom = pInfo->u16Height - MDrv_MJPEG_GetHeight(u8DrvId) - pInfo->u16CropTop;
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
    return MDrv_MJPEG_IsPlaying(u32Id);
}

/******************************************************************************/
///Return TRUE if idle, including decode done & error
///@return idle or not
/******************************************************************************/
MS_BOOL MApi_MJPEG_IsIdle(MS_U32 u32Id)
{
    if(MApi_JPEG_IsNJPD()==TRUE)
    {
        _MApi_JPEG_SetNJPDInstance(bNJPDNum);
    }
    JPEG_Event u16Value = _MApi_JPEG_GetJPDEventFlag();
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
    UNUSED(u32Id);
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
    UNUSED(u32Id);
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
    pFrmInfo->u8FrmType = (MS_U8)E_MJPEG_FRM_TYPE_I;

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
    pFrmInfo->u16Pitch = MDrv_MJPEG_GetPitch(u8DrvId);
    pFrmInfo->u16Width = MDrv_MJPEG_GetWidth(u8DrvId);
    pFrmInfo->u16Height = MDrv_MJPEG_GetHeight(u8DrvId);
    pFrmInfo->u8FrmType = (MS_U8)E_MJPEG_FRM_TYPE_I;

    #if ENABLE_AVSYNC_ID_CHECK
    if (u32OldID_H != pFrmInfo->u32ID_H || u32OldID_L != pFrmInfo->u32ID_L
        || u32OldTimeStamp != pFrmInfo->u32TimeStamp)
    {
        printf ("ID 0x%08lx%08lx    PTS %d     addr 0x%lx\n",
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

    _MApi_JPEG_SetDbgLevel(u8Level);
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
    UNUSED(u32Addr);
    UNUSED(u32Data);
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
    UNUSED(u32Addr);
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
    MJPEG_DEBUG_API_MSG("MApi_MJPEG_DispFrame() with index=%td\n", (ptrdiff_t)u32DispIdx);
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
    // Need to call __MApi_MJPEG_CheckDecodeDone to update decode status
    // When decode done, change the display queue status to E_HVD_DISPQ_STATUS_INIT,
    // thus we can get the updated next display frame.
    MJPEG_Result ret;
    ret = __MApi_MJPEG_CheckDecodeDone(u32Id);
    UNUSED(ret);
    MJPEG_FrameInfo *pSrc = (MJPEG_FrameInfo*)MDrv_MJPEG_GetNextDispFrame(u32Id);
    if (pSrc != NULL)
    {
        _MJPEG_Memcpy(pFrmInfo, pSrc, sizeof(MJPEG_FrameInfo));
        pFrmInfo->u16Pitch = MDrv_MJPEG_GetPitch(u8DrvId);
        pFrmInfo->u16Width = MDrv_MJPEG_GetPitch(u8DrvId);
        pFrmInfo->u16Height = MDrv_MJPEG_GetHeight(u8DrvId);
        pFrmInfo->u8FrmType = (MS_U8)E_MJPEG_FRM_TYPE_I;

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
    __bSeekToPts[u8DrvId] = TRUE;
    __u32Skip2PtsBase[u8DrvId] = MApi_MJPEG_GetPTS(u32Id);
    __u32TargetPts[u8DrvId] = u32PTS;
    MJPEG_DEBUG_API_MSG("   Seek from PTS %td to %td\n", (ptrdiff_t)__u32Skip2PtsBase[u8DrvId], (ptrdiff_t)__u32TargetPts[u8DrvId]);

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
    __bSkipToPts[u8DrvId] = TRUE;
    __u32Skip2PtsBase[u8DrvId] = MApi_MJPEG_GetPTS(u32Id);
    __u32TargetPts[u8DrvId] = u32PTS;
    MJPEG_DEBUG_API_MSG("   Skip from PTS %td to %td\n", (ptrdiff_t)__u32Skip2PtsBase[u8DrvId], (ptrdiff_t)__u32TargetPts[u8DrvId]);

    return E_MJPEG_RET_SUCCESS;
}

/******************************************************************************/
///Description : Enable/Disable FW turbo mode
///@param bEnable \b IN enable or not.
///@return NONE
/******************************************************************************/
void MApi_MJPEG_EnableTurboFWMode(MS_U32 u32Id, MS_BOOL bEnable)
{
    MDrv_MJPEG_EnableTurboFWMode(u32Id, bEnable);
}

static MJPEG_Result _MApi_MJPEG_SetDisplayOutSideMode(MS_U32 u32Id, MS_BOOL bOn)
{
    MJPEG_DEBUG_API_MSG("MJPEG _VDEC_EX_DISP_OUTSIDE_MODE!!!!!! with %d\n", bOn);
    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_DISPLAY_OUTSIDE_MODE, (MS_U32)bOn);

    return E_MJPEG_RET_SUCCESS;
}


MJPEG_Result MApi_MJPEG_SetControl(MS_U32 u32Id, MJPEG_VDEC_Cmd cmd_id, MS_U32 param)
{
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

#ifdef VDEC3
    if ((eStreamType >=  E_MJPEG_API_N_STREAM) && (eStreamType < E_MJPEG_API_N_STREAM + E_MJPEG_MJPEG_TOTAL))
    {
        if (E_MJPEG_OK == MDrv_MJPEG_GetFreeStream(pu32Id, E_MJPEG_DRV_N_STREAM + eStreamType-E_MJPEG_API_N_STREAM))
            eRet = E_MJPEG_RET_SUCCESS;
        else
            eRet = E_MJPEG_RET_FAIL;
    }
#else
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
#endif



    return eRet;
}

#if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
void MApi_MJPEG_SetCMAInformation(void* pcmaInitParam)
{
    bCMAUsed = TRUE;
    JPEG_memcpy((void*)(&cmaInitParam),pcmaInitParam,sizeof(struct CMA_Pool_Init_Param));
    MJPEG_DEBUG("[MJPEG!!] set bCMAUsed=TRUE, cmaInitParam.heap_miu_start_offset=0x%tx, cmaInitParam.heap_length=0x%tx\n", (ptrdiff_t)cmaInitParam.heap_miu_start_offset, (ptrdiff_t)cmaInitParam.heap_length);
}
#endif
#endif
