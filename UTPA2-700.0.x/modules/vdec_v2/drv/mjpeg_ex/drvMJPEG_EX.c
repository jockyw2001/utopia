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
/// file    drvMJPEG.c
/// @brief  MJPEG Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


#if !defined(MSOS_TYPE_NUTTX)

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "halCHIP.h"
// Internal Definition
#include "drvMJPEG.h"
#include "VirtualhalMJPEG.h"
#include "halVPU_EX.h"
#include "fwHVD_if.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//MJPEG driver debug macro
#ifndef ANDROID
#define MJPEG_DEBUG_DRV_MSG(format, args...)  do{if(u8MJPEG_DrvDbgLevel & E_MJPEG_DEBUG_DRV_MSG) printf(format, ##args);}while(0)
#define MJPEG_DEBUG_DRV_ERR(format, args...)  do{if(u8MJPEG_DrvDbgLevel & E_MJPEG_DEBUG_DRV_ERR) printf(format, ##args);}while(0)
#else
#define LOG_TAG "DRV_JPD"
#include <cutils/log.h>
#ifndef LOGD
#define MJPEG_DEBUG_DRV_MSG(format, args...)  do{if(u8MJPEG_DrvDbgLevel & E_MJPEG_DEBUG_DRV_MSG) ALOGI(format, ##args);}while(0)
#define MJPEG_DEBUG_DRV_ERR(format, args...)  do{if(u8MJPEG_DrvDbgLevel & E_MJPEG_DEBUG_DRV_ERR) ALOGE(format, ##args);}while(0)
#else
#define MJPEG_DEBUG_DRV_MSG(format, args...)  do{if(u8MJPEG_DrvDbgLevel & E_MJPEG_DEBUG_DRV_MSG) LOGI(format, ##args);}while(0)
#define MJPEG_DEBUG_DRV_ERR(format, args...)  do{if(u8MJPEG_DrvDbgLevel & E_MJPEG_DEBUG_DRV_ERR) LOGE(format, ##args);}while(0)
#endif

#endif

#define HVD_FW_MEM_OFFSET           0x100000UL  // 1M

#if ENABLE_INC_BUILD

static MS_U8 u8MJPEG_DrvDbgLevel = E_MJPEG_DEBUG_DRV_NONE;

static MS_U32 __u32FwAddr = 0xFFFFFFFF;

static MS_BOOL bDisplayOutSideMode[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U32 u32FrameBuffStAddr[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U32 u32FrameBuffUnitSize[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U32 u32FrameBuffTotalNum[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U16 u16Width[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U16 u16Height[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U16 u16Pitch[E_MJPEG_MJPEG_TOTAL] = {0, 0};
static MS_U32 u32FrameCnt[E_MJPEG_MJPEG_TOTAL] = {0, 0};

/********************************************************************/
///Set MJPEG Debug level
///@para MS_U8 u8DbgLevel \b IN The MJPEG debug level
///@return none
/********************************************************************/
void MDrv_MJPEG_SetDbgLevel(MS_U8 u8DbgLevel)
{
    u8MJPEG_DrvDbgLevel = u8DbgLevel;
}

/* =============================== MJPEG =====================================*/

MS_U8 MDrv_MJPEG_GetDrvId(MS_U32 u32Id)
{
    return (0xFF & (u32Id >> 16));
}
/******************************************************************************/
/// Stop VPU
/******************************************************************************/
void MDrv_MJPEG_StopVPU(void)
{
    VirtualHal_MJPEG_StopVPU();
}


/******************************************************************************/
/// Release VPU
/******************************************************************************/
void MDrv_MJPEG_ReleaseVPU(void)
{
    VirtualHal_MJPEG_ReleaseVPU();
}

/******************************************************************************/
///Check if MJPEG firmware ready or not
///@return TRUE or FALSE
/******************************************************************************/
MS_BOOL MDrv_MJPEG_IsFirmwareReady(void)
{
    MS_U32 u32FirmVer = 0, u32Timeout = 50000;

    // read VPU_RISC_MBOX0 to get firmware version
    while(u32Timeout)
    {
        if(VirtualHal_MJPEG_IsVPUMBoxReady(VPU_RISC_MBOX0))
        {
            VirtualHal_MJPEG_ReadVPUMBox(VPU_RISC_MBOX0, (MS_U32 *)&u32FirmVer);
            VirtualHal_MJPEG_ClearVPUMBox(VPU_RISC_MBOX0);
            break;
        }
        u32Timeout--;
    }
    if(u32Timeout > 0)
    {
        MJPEG_DEBUG_DRV_MSG("MJPEG firmware version 0x%lx\n", u32FirmVer);
    }
    else
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get MJPEG firmware version !! \n");
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
///Set VPU Command
///@return TRUE or FALSE
///@param u8Cmd \b IN: VPU Command
///     - MJPEG_CMD_SET_FRAME_BUFF_ADDR, Set frame buffer address
///     - MJPEG_CMD_SET_FRAME_BUFF_SIZE, Set frame buffer size
/******************************************************************************/
static MS_BOOL MDrv_MJPEG_SetVPUCmd(MS_U32 u32Id, MS_U32 u32Cmd)
{
    //currently we only use VPU_HI_MBOX0 to send command.
    //MS_U32 u32TimeOut = MsOS_GetSystemTime() + _u32FrameTime;
    MS_U32 u32TimeOut = 200000;
    MS_BOOL bResult = FALSE;

    MJPEG_DEBUG_DRV_MSG("Send CMD 0x%lx to MJPEG firmware\n", u32Cmd);
    //while(MsOS_GetSystemTime() < u32TimeOut)
    while(u32TimeOut)
    {
        if(VirtualHal_MJPEG_IsVPUMBoxReady(VPU_HI_MBOX0))
        {
            bResult = VirtualHal_MJPEG_SendVPUMBox(u32Id, VPU_HI_MBOX0, u32Cmd);
            break;
        }
        u32TimeOut--;
    }
    if(u32TimeOut <= 0)
    {
        MJPEG_DEBUG_DRV_ERR("MJPEG send command timeout !!....pc=0x%lx \n", HAL_VPU_EX_GetProgCnt());
        bResult = FALSE;
    }

    return bResult;
}

/******************************************************************************/
///Set VPU command argument
///@return TRUE or FALSE
///@param u32Arg \b IN: VPU command argument
/******************************************************************************/
static MS_BOOL MDrv_MJPEG_SetVPUCmdArg(MS_U32 u32Arg)
{
    // we use VPU_HI_MBOX1 to send command argument
    //U16 u16TimeOut = 0xFFFF;
    MS_U32 u32TimeOut = 200000;
    MS_BOOL bResult = FALSE;

    MJPEG_DEBUG_DRV_MSG("Send argument 0x%lx to VPU \n", u32Arg);
    while(--u32TimeOut)
    {
        if( VirtualHal_MJPEG_IsVPUMBoxReady(VPU_HI_MBOX0)&& VirtualHal_MJPEG_IsVPUMBoxReady(VPU_HI_MBOX1))
        {
            // the argument to be sent doesn't need to add task information in prefix
            bResult = VirtualHal_MJPEG_SendVPUMBox(0, VPU_HI_MBOX1, u32Arg);
            break;
        }
    }
    if(u32TimeOut <= 0)
    {
        MJPEG_DEBUG_DRV_ERR("MJPEG send command argument timeout !! \n");
        bResult = FALSE;
    }

    return bResult;
}

/******************************************************************************/
///Check VPU status
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
static MS_U32 MDrv_MJPEG_CheckVPUStatus(MS_U32 u32Id, MS_U32 u32Cmd)
{
//    MS_BOOL bResult = FALSE;
    MS_U32 u32Status = 0, u32Timeout = 200000;

    MJPEG_DEBUG_DRV_MSG("Firmware status check:  CMD 0x%lX\n", u32Cmd);
    while (u32Timeout) // Need timeout protection
    {
        if(VirtualHal_MJPEG_IsVPUMBoxReady(VPU_HI_MBOX0))
        {
//            bResult = VirtualHal_MJPEG_SendVPUMBox(VPU_HI_MBOX0, u32Cmd);
            VirtualHal_MJPEG_SendVPUMBox(u32Id, VPU_HI_MBOX0, u32Cmd);
            break;
        }
        u32Timeout--;
    }
    if(u32Timeout <= 0)
    {
        MJPEG_DEBUG_DRV_ERR("u32Cmd=0x%lX MJPEG check vpu status 1 timeout !! \n", u32Cmd);
    }

    // Get firmware return value
    u32Timeout = 200000;
    while(u32Timeout)
    {
        if(VirtualHal_MJPEG_IsVPUMBoxReady(VPU_RISC_MBOX0))
        {
            VirtualHal_MJPEG_ReadVPUMBox(VPU_RISC_MBOX0, (MS_U32 *)&u32Status);
            VirtualHal_MJPEG_ClearVPUMBox(VPU_RISC_MBOX0);
            break;
        }
        u32Timeout--;
    }
    if(u32Timeout <= 0)
    {
        MJPEG_DEBUG_DRV_ERR("u32Cmd=0x%lX MJPEG check vpu status 2 timeout !! \n", u32Cmd);
    }

    MJPEG_DEBUG_DRV_MSG("Firmware status : 0x%lX\n", u32Status);
    return u32Status;
}

/******************************************************************************/
///Set VPU command argument
///@return TRUE or FALSE
///@param u32Arg \b IN: VPU command argument
/******************************************************************************/
MS_U32 MDrv_MJPEG_SendVPUCommand(MS_U32 u32Id, MJPEG_User_Cmd eCmd, MS_U32 u32Arg)
{
    MS_U32 u32Ret = 0;

    MS_U32 idx = 0;
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

    if (bDisplayOutSideMode[u8DrvId])
    {
        volatile HVD_ShareMem *pShareMem = NULL;
        MsOS_ReadMemory();
        pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
        if((MS_U32)pShareMem==0xFFFFFFFF)
        {
            MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!![%d]\n", __LINE__);
            return 0xFFFFFFFF;
        }

        switch (eCmd)
        {
        case E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR:
            u32FrameBuffStAddr[u8DrvId] = u32Arg;
            pShareMem->DispQueue[0].u32LumaAddr = u32FrameBuffStAddr[u8DrvId];
            pShareMem->DispQueue[0].u32Status = E_HVD_DISPQ_STATUS_FREE;
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE:
            u32FrameBuffUnitSize[u8DrvId] = u32Arg;
            break;

        case E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM:
            if (u32FrameBuffTotalNum[u8DrvId] > HVD_DISP_QUEUE_MAX_SIZE)
                u32FrameBuffTotalNum[u8DrvId] = HVD_DISP_QUEUE_MAX_SIZE;
            else
                u32FrameBuffTotalNum[u8DrvId] = u32Arg;

            pShareMem->u16DispQSize = u32FrameBuffTotalNum[u8DrvId];

            for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]-1; idx++)
            {
                if (u32FrameBuffUnitSize[u8DrvId] != 0)
                {
                    pShareMem->DispQueue[idx+1].u32LumaAddr = pShareMem->DispQueue[idx].u32LumaAddr + u32FrameBuffUnitSize[u8DrvId];
                    pShareMem->DispQueue[idx+1].u32Status = E_HVD_DISPQ_STATUS_FREE;
                }
            }
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX:
            for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
            {
                if (pShareMem->DispQueue[idx].u32Status == E_HVD_DISPQ_STATUS_FREE)
                {
                    return idx;
                }
            }
            return (0xFFFFFFFF);
            break;

        case E_MJPEG_CMD_SET_FRAME_BUFF_IDX:
            if (u32Arg < u32FrameBuffTotalNum[u8DrvId])
            {
                pShareMem->DispQueue[u32Arg].u32Status = E_HVD_DISPQ_STATUS_NONE;
                pShareMem->u32MJPEG_NextFrameBuffIdx = 0xFFFF;
                MsOS_FlushMemory();
            }
            break;

        case E_MJPEG_CMD_PUSH_QUEUE_PARA_SETTING:

            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32Status = E_HVD_DISPQ_STATUS_NONE;
            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32TimeStamp =  pShareMem->u32MJPEGTimeStamp;
            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32ID_L = pShareMem->u32MJPEGID_L;
            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32ID_H = pShareMem->u32MJPEGID_H;
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_SET_DISPLAY_OUTSIDE_MODE:

            if(u32Arg==0)
            {
                bDisplayOutSideMode[u8DrvId] = FALSE;
            }
            MDrv_MJPEG_SetVPUCmdArg(u32Arg);
            MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_DISPLAY_OUTSIDE_MODE);
            break;

        case E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY:
            if (u32Arg < u32FrameBuffTotalNum[u8DrvId])
            {
                u32FrameCnt[u8DrvId]++;
                pShareMem->DispQueue[u32Arg].u32Status = E_HVD_DISPQ_STATUS_INIT;
                pShareMem->DispQueue[u32Arg].u32ChromaAddr = pShareMem->DispQueue[u32Arg].u32LumaAddr;
                pShareMem->DispQueue[u32Arg].u16Width = u16Width[u8DrvId];
                pShareMem->DispQueue[u32Arg].u16Height = u16Height[u8DrvId];
                pShareMem->DispQueue[u32Arg].u16Pitch = u16Pitch[u8DrvId];
                pShareMem->DispQueue[u32Arg].u32PrivateData = u32FrameCnt[u8DrvId];

                for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
                {
                    if (pShareMem->DispQueue[idx].u32Status == E_HVD_DISPQ_STATUS_FREE)
                    {
                        pShareMem->u32MJPEG_NextFrameBuffIdx = idx;
                        break;
                    }
                    else
                    {
                        pShareMem->u32MJPEG_NextFrameBuffIdx = 0xFFFF;
                    }
                }
            }
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID:
            if (u32Arg < u32FrameBuffTotalNum[u8DrvId])
            {
                pShareMem->DispQueue[u32Arg].u32Status = E_HVD_DISPQ_STATUS_FREE;
                MsOS_FlushMemory();
            }
            break;

        case E_MJPEG_CMD_FLUSH_DISP_QUEUE:

            for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
            {
                if (pShareMem->DispQueue[idx].u32Status != E_HVD_DISPQ_STATUS_VIEW)
                {
                    pShareMem->DispQueue[idx].u32Status = E_HVD_DISPQ_STATUS_FREE;
                    pShareMem->DispQueue[idx].u32TimeStamp = 0;
                    pShareMem->DispQueue[idx].u32ID_L = 0xFFFFFFFF;
                    pShareMem->DispQueue[idx].u32ID_H = 0xFFFFFFFF;
                }
            }
            pShareMem->u8MJPEG_bFlushQueue = 0;
            u32Ret = TRUE;
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL:
            for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
            {
                if (pShareMem->DispQueue[idx].u32Status == E_HVD_DISPQ_STATUS_FREE)
                {
                    return 0;
                }
            }
            MJPEG_DEBUG_DRV_MSG("Display Q is full[%d]\n", __LINE__);
            // return full
            return 1;
            break;

        case E_MJPEG_CMD_SET_WIDTH:
            u16Width[u8DrvId] = (MS_U16)u32Arg;
            break;

        case E_MJPEG_CMD_SET_HEIGHT:
            u16Height[u8DrvId] = (MS_U16)u32Arg;
            break;

        case E_MJPEG_CMD_SET_PITCH:
            u16Pitch[u8DrvId] = (MS_U16)u32Arg;
            break;

        case E_MJPEG_CMD_IS_DISP_FINISH:
            u32Ret = TRUE;
            break;

        case E_MJPEG_CMD_SET_FRAME_ID_L:
            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32ID_L = u32Arg;
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_SET_FRAME_ID_H:
            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32ID_H = u32Arg;
            MsOS_FlushMemory();
            break;

        case E_MJPEG_CMD_SET_TIMESTAMP:
            pShareMem->DispQueue[pShareMem->u32MJPEGFrameBuffIdx].u32TimeStamp = u32Arg;
            MsOS_FlushMemory();
            break;
        case E_MJPEG_CMD_GO:
        case E_MJPEG_CMD_SET_FRAMERATE:
        case E_MJPEG_CMD_SET_FRAMERATE_BASE:
        case E_MJPEG_CMD_SET_CHIP_ID:
        case E_MJPEG_CMD_PLAY:
        case E_MJPEG_CMD_PAUSE:
        case E_MJPEG_CMD_RESUME:
        case E_MJPEG_CMD_STEP_PLAY:
        case E_MJPEG_CMD_SET_SPEED_TYPE:
        case E_MJPEG_CMD_SET_SPEED:
        case E_MJPEG_CMD_FREEZE_DISP:
        case E_MJPEG_CMD_ENABLE_AVSYNC:
        case E_MJPEG_CMD_SET_AVSYNC_DELAY:
        case E_MJPEG_CMD_SET_AVSYNC_TOLERENCE:
        case E_MJPEG_CMD_SET_PTS_BASE:
        case E_MJPEG_CMD_SET_STC_BASE:
        case E_MJPEG_CMD_SET_BLUE_SCREEN:
        case E_MJPEG_CMD_COMPENSATE_PTS:
        case E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE:
        case E_MJPEG_CMD_PUSH_DISP_CMD:
        case E_MJPEG_CMD_GET_DISP_CMD_Q_VACANCY:
        case E_MJPEG_CMD_IS_STEP_PLAY_DONE:
        case E_MJPEG_CMD_IS_PLAYING:
        case E_MJPEG_CMD_IS_AVSYNC_ON:
        case E_MJPEG_CMD_IS_REACH_AVSYNC:
        case E_MJPEG_CMD_IS_FLUSH_DONE:
            MJPEG_DEBUG_DRV_MSG(" Unused MJPEG Command[0x%x] in display outside mode\n", eCmd);
            u32Ret = 0xFFFFFFFF;
            break;

        default:
            MJPEG_DEBUG_DRV_MSG("    !!! Unknown MJPEG Command[0x%x] !!!\n", eCmd);
            u32Ret = 0xFFFFFFFF;
            break;
        }

        return u32Ret;
    }

    switch (eCmd)
    {
    case E_MJPEG_CMD_GO:
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_GO);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_START_ADDR:
        u32FrameBuffStAddr[u8DrvId] = u32Arg;
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_START_ADDR);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_UNIT_SIZE:
        u32FrameBuffUnitSize[u8DrvId] = u32Arg;
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_UNIT_SIZE);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_TOTAL_NUM:
        if (u32FrameBuffTotalNum[u8DrvId] > HVD_DISP_QUEUE_MAX_SIZE)
            u32FrameBuffTotalNum[u8DrvId] = HVD_DISP_QUEUE_MAX_SIZE;
        else
            u32FrameBuffTotalNum[u8DrvId] = u32Arg;
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_TOTAL_NUM);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_IDX:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_IDX);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_IDX_READY:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_IDX_READY);
        break;

    case E_MJPEG_CMD_SET_WIDTH:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_WIDTH);
        break;

    case E_MJPEG_CMD_SET_HEIGHT:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_HEIGHT);
        break;

    case E_MJPEG_CMD_SET_PITCH:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_PITCH);
        break;

    case E_MJPEG_CMD_SET_FRAME_ID_L:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_ID_L);
        break;

    case E_MJPEG_CMD_SET_FRAME_ID_H:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_ID_H);
        break;

    case E_MJPEG_CMD_SET_TIMESTAMP:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_TIMESTAMP);
        break;

    case E_MJPEG_CMD_SET_FRAMERATE:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAMERATE);
        break;

    case E_MJPEG_CMD_SET_FRAMERATE_BASE:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAMERATE_BASE);
        break;

    case E_MJPEG_CMD_SET_FRAME_BUFF_IDX_VALID:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_IDX_VALID);
        break;

    case E_MJPEG_CMD_SET_CHIP_ID:
         MDrv_MJPEG_SetVPUCmdArg(u32Arg);
         MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_CHIP_ID);
         break;

    case E_MJPEG_CMD_PLAY:
        u32Ret = (MS_U32)MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_PLAY);
        break;

    case E_MJPEG_CMD_PAUSE:
        u32Ret = (MS_U32)MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_PAUSE);
        break;

    case E_MJPEG_CMD_RESUME:
        u32Ret = (MS_U32)MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_RESUME);
        break;

    case E_MJPEG_CMD_STEP_PLAY:
        u32Ret = (MS_U32)MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_STEP_PLAY);
        break;

    case E_MJPEG_CMD_SET_SPEED_TYPE:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_SPEED_TYPE);
        break;

    case E_MJPEG_CMD_SET_SPEED:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_SPEED);
        break;

    case E_MJPEG_CMD_FLUSH_DISP_QUEUE:
        u32Ret = (MS_U32)MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_FLUSH_DISP_QUEUE);
        break;

    case E_MJPEG_CMD_FREEZE_DISP:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_FREEZE_DISP);
        break;

    case E_MJPEG_CMD_ENABLE_AVSYNC:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_ENABLE_AVSYNC);
        break;

    case E_MJPEG_CMD_SET_AVSYNC_DELAY:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_AVSYNC_DELAY);
        break;

    case E_MJPEG_CMD_SET_AVSYNC_TOLERENCE:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_AVSYNC_TOLERENCE);
        break;

    case E_MJPEG_CMD_SET_PTS_BASE:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_PTS_BASE);
        break;

    case E_MJPEG_CMD_SET_STC_BASE:
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_STC_BASE);
        break;

    case E_MJPEG_CMD_SET_BLUE_SCREEN:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_BLUE_SCREEN);
        break;

    case E_MJPEG_CMD_PUSH_QUEUE_PARA_SETTING:
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_PUSH_QUEUE_PARA_SETTING);
        break;

    case E_MJPEG_CMD_SET_DISPLAY_OUTSIDE_MODE:

        if(u32Arg)
        {
            bDisplayOutSideMode[u8DrvId] = TRUE;
        }
        if (bDisplayOutSideMode[u8DrvId])
        {
            // set E_JPD_CMD_SET_FRAME_BUFF_TOTAL_NUM to 0 previent F/W write share mem
            MDrv_MJPEG_SetVPUCmdArg(0);
            MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_FRAME_BUFF_TOTAL_NUM);
            MDrv_MJPEG_SetVPUCmdArg(0);
            MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_ENABLE_DISP_CMD_QUEUE);

            MsOS_DelayTaskUs(10* 1000);
            u32FrameCnt[u8DrvId] = 0;

            volatile HVD_ShareMem *pShareMem = NULL;
            MsOS_ReadMemory();
            pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
            if((MS_U32)pShareMem==0xFFFFFFFF)
            {
                MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!![%d]\n", __LINE__);
                return 0xFFFFFFFF;
            }

            pShareMem->DispQueue[0].u32LumaAddr = u32FrameBuffStAddr[u8DrvId];
            pShareMem->DispQueue[0].u32Status = E_HVD_DISPQ_STATUS_FREE;
            pShareMem->u16DispQSize = u32FrameBuffTotalNum[u8DrvId];

            for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]-1; idx++)
            {
                if (u32FrameBuffUnitSize[u8DrvId] != 0)
                {
                    pShareMem->DispQueue[idx+1].u32LumaAddr = pShareMem->DispQueue[idx].u32LumaAddr + u32FrameBuffUnitSize[u8DrvId];
                    pShareMem->DispQueue[idx+1].u32Status = E_HVD_DISPQ_STATUS_FREE;
                }
            }

            for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
            {
                MJPEG_DEBUG_DRV_MSG("pShareMem->DispQueue[idx].u32Status : 0x%lX  %ld\n", pShareMem->DispQueue[idx].u32Status, idx);
            }
            MsOS_FlushMemory();
        }
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_SET_DISPLAY_OUTSIDE_MODE);
        break;

    case E_MJPEG_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_GET_NEXT_FREE_FRAME_BUFF_IDX);
        break;

    case E_MJPEG_CMD_COMPENSATE_PTS:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_COMPENSATE_PTS);
        break;

    // Display Command Queue -- BEGIN
    case E_MJPEG_CMD_ENABLE_DISP_CMD_QUEUE:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_ENABLE_DISP_CMD_QUEUE);
        break;

    case E_MJPEG_CMD_PUSH_DISP_CMD:
        MDrv_MJPEG_SetVPUCmdArg(u32Arg);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_PUSH_DISP_CMD);
        break;

    case E_MJPEG_CMD_GET_DISP_CMD_Q_VACANCY:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_GET_DISP_CMD_Q_VACANCY);
        break;
    // Display Command Queue -- END

    case E_MJPEG_CMD_IS_STEP_PLAY_DONE:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_STEP_PLAY_DONE);
        break;

    case E_MJPEG_CMD_IS_DISP_FINISH:
#if USE_MJPEG_SHAREMEMORY
        MDrv_MJPEG_Init_IsDispFinish(u32Id);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_IS_DISP_FINISH);
        u32Ret = (MS_U32) MDrv_MJPEG_IsDispFinish(u32Id);
#else
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_DISP_FINISH);
#endif
        break;

    case E_MJPEG_CMD_IS_PLAYING:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_PLAYING);
        break;

    case E_MJPEG_CMD_IS_DISPLAY_QUEUE_FULL:
#if USE_MJPEG_SHAREMEMORY
        MDrv_MJPEG_Init_IsQueueFull(u32Id);
        MDrv_MJPEG_SetVPUCmd(u32Id, E_JPD_CMD_IS_DISPLAY_QUEUE_FULL);
        u32Ret = (MS_U32) MDrv_MJPEG_IsQueueFull(u32Id);
#else
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_DISPLAY_QUEUE_FULL);
#endif
        break;

    case E_MJPEG_CMD_IS_AVSYNC_ON:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_AVSYNC_ON);
        break;

    case E_MJPEG_CMD_IS_REACH_AVSYNC:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_REACH_AVSYNC);
        break;

    case E_MJPEG_CMD_IS_FLUSH_DONE:
        u32Ret = MDrv_MJPEG_CheckVPUStatus(u32Id, E_JPD_CMD_IS_FLUSH_DONE);
        break;

    default:
        MJPEG_DEBUG_DRV_MSG("    !!! Unknown MJPEG Command !!!\n");
        u32Ret = 0xFFFFFFFF;
        break;
    }


    return u32Ret;
}

#if 0
/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_BOOL MDrv_MJPEG_IsVPUOnMiu1(void)
{
    return VirtualHal_MJPEG_IsVPUOnMiu1();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 MDrv_MJPEG_GetMiuBase(void)
{
    return VirtualHal_MJPEG_GetMiuBase();
}
#endif

/******************************************************************************/
///Get ShareMemory address
///@param u32Value \b IN new PTS value
/******************************************************************************/
MS_U32 MDrv_MJPEG_GetShareMemAddr(MS_U32 u32Id)
{
    MS_U32 u32ShareMemAddr = 0xFFFFFFFF;

    if (__u32FwAddr != 0xFFFFFFFF)
        u32ShareMemAddr = __u32FwAddr +(HAL_VPU_EX_GetTaskId(u32Id) * HVD_FW_MEM_OFFSET) + HVD_SHARE_MEM_ST_OFFSET;
    else
        return 0xFFFFFFFF;

    u32ShareMemAddr = MS_PA2KSEG1((MS_U32)u32ShareMemAddr); //PhysicalAddr to VirtualAddr

    return u32ShareMemAddr;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_MJPEG_ClearShareMemory(MS_U32 u32Id)
{
    MS_U32 u32Idx = 0, u32Size;
    volatile MS_U8 *u8Ptr = 0;

    u8Ptr = (volatile MS_U8 *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)u8Ptr==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }
    u32Size = sizeof(HVD_ShareMem);

    for (u32Idx = 0; u32Idx < u32Size; u32Idx++)
    {
        *u8Ptr = 0;
        u8Ptr++;
    }
    return;
}

/******************************************************************************/
///Get PTS table address
///@param u32Value \b IN new PTS value
/******************************************************************************/
MS_U32 MDrv_MJPEG_GetPtsTableAddr(MS_U32 u32Id)
{
    MS_U32 u32PtsAddr = 0xFFFFFFFF;
    #if defined(SUPPORT_NEW_MEM_LAYOUT)
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    MS_U32 u32HVD_PTS_TABLE_ST_OFFSET = pShareMem->u32HVD_PTS_TABLE_ST_OFFSET - HAL_VPU_EX_GetTaskId(u32Id) * HVD_FW_MEM_OFFSET;
    #else
    MS_U32 u32HVD_PTS_TABLE_ST_OFFSET = HVD_PTS_TABLE_ST_OFFSET;
    #endif

    if (__u32FwAddr != 0xFFFFFFFF)
        u32PtsAddr = __u32FwAddr + u32HVD_PTS_TABLE_ST_OFFSET;
    else
        return 0xFFFFFFFF;

    u32PtsAddr = MS_PA2KSEG1((MS_U32)u32PtsAddr); //PhysicalAddr to VirtualAddr

    return u32PtsAddr;
}

/******************************************************************************/
///Reset PTS value
///@param u32Value \b IN new PTS value
/******************************************************************************/
MS_BOOL MDrv_MJPEG_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase)
{

    MDrv_MJPEG_SendVPUCommand(u32Id, E_MJPEG_CMD_SET_PTS_BASE, u32PtsBase);

    return TRUE;
}

/******************************************************************************/
///Return PTS of current displayed frame
///@return PTS
/******************************************************************************/
MS_U32 MDrv_MJPEG_GetPTS(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();

    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return NULL;
    }

    return pShareMem->DispFrmInfo.u32TimeStamp;
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_MJPEG_DispFrame(MS_U32 u32Id, MS_U32 u32DispIdx)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    if (pShareMem->DispQueue[u32DispIdx].u32Status == E_HVD_DISPQ_STATUS_VIEW)
    {
        pShareMem->DispQueue[u32DispIdx].u32Status = E_HVD_DISPQ_STATUS_DISP;
    }
    MsOS_FlushMemory();
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_MJPEG_FreeFrame(MS_U32 u32Id, MS_U32 u32DispIdx)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    if (pShareMem->DispQueue[u32DispIdx].u32Status == E_HVD_DISPQ_STATUS_VIEW)
    {
        pShareMem->DispQueue[u32DispIdx].u32Status = E_HVD_DISPQ_STATUS_FREE;
    }
    MsOS_FlushMemory();
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
#define MJPEG_DISPQ_PREFETCH_COUNT        2

static MS_U16 _u16DispQPtr[E_MJPEG_MJPEG_TOTAL] = {0, 0};
void* MDrv_MJPEG_GetNextDispFrame(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return NULL;
    }

    HVD_Frm_Information *pFrm;

    if (bDisplayOutSideMode[u8DrvId])
    {
        MS_U32 idx = 0;
        MS_S32 found = -1;

        for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
        {
            if (pShareMem->DispQueue[idx].u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                MJPEG_DEBUG_DRV_MSG("MDrv_MJPEG_GetNextDispFrame, pShareMem->DispQueue[%ld].u32PrivateData = %ld", idx, pShareMem->DispQueue[idx].u32PrivateData);

                if (found >= 0)
                {
                    // find the older one
                    if (pShareMem->DispQueue[idx].u32PrivateData < pShareMem->DispQueue[found].u32PrivateData)
                        found = idx;
                }
                else
                {
                    found = idx;
                }
            }
        }

        if (found >= 0)
        {
            pFrm = (HVD_Frm_Information *)(MS_U32)&(pShareMem->DispQueue[found]);
            _u16DispQPtr[u8DrvId] = found;
            pFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW;
            MsOS_FlushMemory();
            MJPEG_DEBUG_DRV_MSG("bDisplayOutSideMode-->FrameDone: %ld, pFrm=0x%lx, timestamp=%ld\n", idx, (MS_U32)pFrm, pFrm->u32TimeStamp);
            return pFrm;
        }
        else
        {
            MJPEG_DEBUG_DRV_MSG("MDrv_MJPEG_GetNextDispFrame return NULL!!!\n");
            return NULL;
        }
    }
    else
    {

        MS_U16 u16QNum = pShareMem->u16DispQNumb;
        MS_U16 u16QPtr = pShareMem->u16DispQPtr;

#if 0
        if (u16QNum > MJPEG_DISPQ_PREFETCH_COUNT) u16QNum = MJPEG_DISPQ_PREFETCH_COUNT;
#endif

        while (u16QNum != 0)
        {
            pFrm = (HVD_Frm_Information*)(MS_U32)&(pShareMem->DispQueue[u16QPtr]);

            if (pFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                _u16DispQPtr[u8DrvId] = u16QPtr;
                pFrm->u32Status = E_HVD_DISPQ_STATUS_VIEW; /////Change its state!!
                MJPEG_DEBUG_DRV_MSG("FrameDone: %d, pFrm=0x%lx, timestamp=%ld\n", u16QPtr, (MS_U32)pFrm, pFrm->u32TimeStamp);
                return pFrm;
            }
            u16QNum--;
            u16QPtr++;
            if (u16QPtr == pShareMem->u16DispQSize) u16QPtr = 0; //wrap to the begin
        }
        MsOS_FlushMemory();
        return NULL;
    }
}

MS_U32 MDrv_MJPEG_GetDispFrameNum(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);

    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return NULL;
    }

    HVD_Frm_Information *pFrm;
    MS_U32 u32DispFrmNum = 0;

    if (bDisplayOutSideMode[u8DrvId])
    {
        MS_U32 idx = 0;
        for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
        {
            pFrm = (HVD_Frm_Information *)(MS_U32)&(pShareMem->DispQueue[idx]);

            if (pFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                u32DispFrmNum++;
            }
        }
        return u32DispFrmNum;
    }
    else
    {
        MS_U16 u16QNum = pShareMem->u16DispQNumb;
        MS_U16 u16QPtr = pShareMem->u16DispQPtr;

        while (u16QNum != 0)
        {
            pFrm = (HVD_Frm_Information *)(MS_U32)&(pShareMem->DispQueue[u16QPtr]);

            if (pFrm->u32Status == E_HVD_DISPQ_STATUS_INIT)
            {
                u32DispFrmNum++;
            }
            u16QNum--;
            u16QPtr++;
            if (u16QPtr == pShareMem->u16DispQSize)
            {
                u16QPtr = 0; //wrap to the begin
            }
        }
        return u32DispFrmNum;
    }
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
MS_U32 MDrv_MJPEG_GetNextDispQPtr(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    return _u16DispQPtr[u8DrvId];
}

/******************************************************************************/
///Description
///@param in \b IN in parameter
///@param out \b OUT out parameter
///@return status
/******************************************************************************/
void MDrv_MJPEG_GetDispFrameInfo(MS_U32 u32Id, void *pInfo)
{
    MJPEG_DRV_FrameInfo *pFrmInfo = (MJPEG_DRV_FrameInfo *)pInfo;

    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    pFrmInfo->u32LumaAddr = pShareMem->DispFrmInfo.u32LumaAddr;
    pFrmInfo->u32ChromaAddr = pShareMem->DispFrmInfo.u32ChromaAddr;
    pFrmInfo->u32TimeStamp = pShareMem->DispFrmInfo.u32TimeStamp;
    pFrmInfo->u32ID_L = pShareMem->DispFrmInfo.u32ID_L;
    pFrmInfo->u32ID_H = pShareMem->DispFrmInfo.u32ID_H;
}

void MDrv_MJPEG_EnableTurboFWMode(MS_BOOL bEnable)
{
//    gbEnableTurboFWMode = bEnable;
    HAL_VPU_EX_SetFWReload(!bEnable);
}

MJPEG_Return MDrv_MJPEG_InitFw(MS_U32 u32Id, MS_U32 u32Size, MS_PHYADDR u32Addr, MS_U32 u32BinSize, MS_PHYADDR u32BinAddr, MS_U8 u8FwSrcType)
{
    __u32FwAddr = u32Addr;

    MJPEG_DEBUG_DRV_MSG("MDrv_MJPEG_InitFw u32Size = 0x%lx, u32Addr = 0x%lx, u32BinSize = 0x%lx, u32BinAddr = 0x%lx, u8FwSrcType = %d\n", u32Size, u32Addr, u32BinSize, u32BinAddr, u8FwSrcType);
    // for wb
    //MDrv_MJPEG_ClearShareMemory(u32Id);

    bDisplayOutSideMode[MDrv_MJPEG_GetDrvId(u32Id)] = FALSE;

    // Set Codec Type for HVD Firmware
    volatile HVD_ShareMem *pHVDShareMem = NULL;
    pHVDShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pHVDShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return E_MJPEG_FAILED;
    }
    pHVDShareMem->u32CodecType = E_HVD_Codec_MJPEG;
    // u16ChipID is set in HAL_HVD_EX_InitShareMem()
    //pHVDShareMem->u16ChipID = E_MSTAR_CHIP_J2;
    MsOS_FlushMemory();

    // Create Task
    VPU_EX_FWCodeCfg       stVpuFWCfg;
    VPU_EX_TaskInfo        stTaskInfo;
    VPU_EX_VLCTblCfg       stVlcCfg;
    VPU_EX_NDecInitPara    stVpuInitPara;

    memset(&stVpuFWCfg,     0, sizeof(VPU_EX_FWCodeCfg));
    memset(&stTaskInfo,     0, sizeof(VPU_EX_TaskInfo));
    memset(&stVlcCfg,       0, sizeof(VPU_EX_VLCTblCfg));
    memset(&stVpuInitPara,  0, sizeof(VPU_EX_NDecInitPara));

    stVpuInitPara.pFWCodeCfg = &stVpuFWCfg;
    stVpuInitPara.pTaskInfo = &stTaskInfo;

    stVpuFWCfg.u32DstAddr = MsOS_PA2KSEG1(u32Addr);
    stVpuFWCfg.u32DstSize = u32Size;
    stVpuFWCfg.u32BinAddr = MsOS_PA2KSEG1(u32BinAddr);
    stVpuFWCfg.u32BinSize = u32BinSize;
    stVpuFWCfg.u8SrcType  = u8FwSrcType;


    stTaskInfo.u32Id = u32Id;
    stTaskInfo.eDecType = E_VPU_EX_DECODER_HVD;
    stTaskInfo.eVpuId = (HAL_VPU_StreamId) (0xFF & u32Id);
    stTaskInfo.eSrcType = E_VPU_EX_INPUT_FILE;

    HAL_VPU_EX_TaskCreate(u32Id, &stVpuInitPara);

    return E_MJPEG_OK;
}

MJPEG_MSTAR_CHIP_ID MDrv_MJPEG_Get_Chip_ID(void)
{
    return VirtualHal_MJPEG_Get_Chip_ID();
}

MJPEG_Return MDrv_MJPEG_GetFreeStream(MS_U32 *pu32Id, MJPEG_DRV_StreamType eStreamType)
{
    MJPEG_Return eRet = E_MJPEG_INVALID_PARAM;
    MS_U8 i = 0xff;
    MS_U32 u32VPUStreamId = 0;
    MS_U32 u32MJPEGStreamId = 0;

    MJPEG_DEBUG_DRV_MSG("%20s:%30s: Id=0x%lX\n", __FILE__, __FUNCTION__, (*pu32Id));

    switch(eStreamType)
    {
        case(E_MJPEG_DRV_MAIN_STREAM):
            i = 0;
            break;
        case(E_MJPEG_DRV_SUB_STREAM):
            i = 1;
            break;
        case(E_MJPEG_DRV_STREAM_NONE):
        default:
            i = 0xff;
            break;
    }

    if ( i == 0xff )
    {
        return E_MJPEG_FAILED;
    }

    if (E_MJPEG_DRV_MAIN_STREAM == eStreamType)
    {
    u32VPUStreamId   = (MS_U32) HAL_VPU_EX_GetFreeStream(E_HAL_VPU_MAIN_STREAM);
    u32MJPEGStreamId = (MS_U32) VirtualHal_MJPEG_GetFreeStream(E_HAL_MJPEG_MAIN_STREAM);

        if (u32VPUStreamId && u32MJPEGStreamId)
        {
            *pu32Id = (i << 16 | u32MJPEGStreamId << 8 | u32VPUStreamId);
            eRet = E_MJPEG_OK;
        }
    }
    else //if (E_MJPEG_DRV_SUB_STREAM== eStreamType)
    {
        u32VPUStreamId   = (MS_U32) HAL_VPU_EX_GetFreeStream(E_HAL_VPU_SUB_STREAM);
        u32MJPEGStreamId = (MS_U32) VirtualHal_MJPEG_GetFreeStream(E_HAL_MJPEG_SUB_STREAM);

        if (u32VPUStreamId && u32MJPEGStreamId)
        {
            *pu32Id = (i << 16 | u32MJPEGStreamId << 8 | u32VPUStreamId);
            eRet = E_MJPEG_OK;
        }
    }


    MJPEG_DEBUG_DRV_MSG("%20s:%30s: Id=0x%lX\n", __FILE__, __FUNCTION__, (*pu32Id));

    return eRet;
}

MJPEG_Return MDrv_MJPEG_DeInit(MS_U32 u32Id)
{
    MJPEG_Return eRet = E_MJPEG_INVALID_PARAM;

    VPU_EX_FWCodeCfg stVpuFWCfg;
    VPU_EX_TaskInfo stTaskInfo;
    VPU_EX_NDecInitPara stVpuInitPara;

    stVpuInitPara.pFWCodeCfg = &stVpuFWCfg;
    stVpuInitPara.pTaskInfo = &stTaskInfo;

    stVpuFWCfg.u32DstAddr = MsOS_PA2KSEG1(__u32FwAddr);
    stVpuFWCfg.u8SrcType  = E_HVD_FW_INPUT_SOURCE_NONE;

    stTaskInfo.u32Id = u32Id;
    stTaskInfo.eDecType = E_VPU_EX_DECODER_HVD;
    stTaskInfo.eVpuId = (HAL_VPU_StreamId) (0xFF & u32Id);
    stTaskInfo.eSrcType = E_VPU_EX_INPUT_FILE;

    if ( HAL_VPU_EX_TaskDelete(u32Id, &stVpuInitPara) ) eRet = E_MJPEG_OK;

    return eRet;
}

void MDrv_MJPEG_InitRegBase(MS_U32 u32RegBase)
{
    HAL_VPU_EX_InitRegBase(u32RegBase);
}

void MDrv_MJPEG_SetPushQPara(MS_U32 u32Id, MS_U32 u32TimeStamp, MS_U32 u32ID_L, MS_U32 u32ID_H, MS_U32 u32NextFrameBuffIdx)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    pShareMem->u32MJPEGFrameBuffIdx = u32NextFrameBuffIdx;
    pShareMem->u32MJPEGTimeStamp = u32TimeStamp;
    pShareMem->u32MJPEGID_L = u32ID_L;
    pShareMem->u32MJPEGID_H = u32ID_H;
    pShareMem->u32MJPEG_NextFrameBuffIdx = 0xffff;
    MsOS_FlushMemory();

}


void MDrv_MJPEG_SetStepDispPara(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }
    pShareMem->u8MJPEG_bStepPlay = TRUE;
    MsOS_FlushMemory();

}


void MDrv_MJPEG_PrintShareInfo(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }

    MJPEG_DEBUG_DRV_MSG("0x%08x, r:%d, w:%d, sys=%ld, v_pts=%ld, s/r=%ld(%c)\n",
        (int)pShareMem->u32MJPEGDbg_DispStatus,
        pShareMem->u8MJPEGDbg_ReadFbIdx, pShareMem->u8MJPEGDbg_WriteFbIdx,
        pShareMem->u32MJPEGDbg_SysTime, pShareMem->u32MJPEGDbg_VideoPts,
        pShareMem->u32MJPEGDbg_SkipRepeatTime, pShareMem->u8MJPEGDbg_SkipRepeat
        );
}

MS_U32 MDrv_MJPEG_GetNextFrameBufferIdx(MS_U32 u32Id)
{
    MS_U8 u8DrvId = MDrv_MJPEG_GetDrvId(u32Id);
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return NULL;
    }

    if (bDisplayOutSideMode[u8DrvId])
    {
        MS_U32 idx = 0;
        for (idx = 0; idx < u32FrameBuffTotalNum[u8DrvId]; idx++)
        {
            if (pShareMem->DispQueue[idx].u32Status == E_HVD_DISPQ_STATUS_FREE)    return idx;
        }
        MJPEG_DEBUG_DRV_ERR("MDrv_MJPEG_GetNextFrameBufferIdx is full!!!\n");
        return (0xFFFFFFFF);
    }
    else
    {
        return pShareMem->u32MJPEG_NextFrameBuffIdx;
    }
}

MS_BOOL MDrv_MJPEG_IsStepPlayDone(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    return (MS_BOOL)pShareMem->u8MJPEG_bStepPlay;
}

MS_BOOL MDrv_MJPEG_IsPlaying(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    return (MS_BOOL)pShareMem->u8MJPEG_bPlaying;
}

MS_BOOL MDrv_MJPEG_IsAVSyncOn(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    return (MS_BOOL)pShareMem->u8MJPEG_bIsAVSyncOn;
}

MS_BOOL MDrv_MJPEG_IsReachAVSync(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    return (MS_BOOL)pShareMem->u8MJPEG_bIsReachAVSync;
}

MS_BOOL MDrv_MJPEG_IsFlushDone(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    return (MS_BOOL)(!pShareMem->u8MJPEG_bFlushQueue);
}

void MDrv_MJPEG_InitFlush(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }
    pShareMem->u8MJPEG_bFlushQueue = 1;
    MsOS_FlushMemory();
}


void MDrv_MJPEG_Init_IsDispFinish(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }
    pShareMem->u8MJPEG_bIsDispFinish = 0xff;
    MsOS_FlushMemory();
}

MS_BOOL MDrv_MJPEG_IsDispFinish(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    MS_U32 u32Timer;

    u32Timer = MsOS_GetSystemTime();
    while(1)
    {
        if (MsOS_GetSystemTime()-u32Timer > 30)
        {
            MJPEG_DEBUG_DRV_ERR("MDrv_MJPEG_IsDispFinish() timeout...\n");
            break;
        }
        if(pShareMem->u8MJPEG_bIsDispFinish!=0xff)
            break;
    }
    return (MS_BOOL)(pShareMem->u8MJPEG_bIsDispFinish);
}


void MDrv_MJPEG_Init_IsQueueFull(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return;
    }
    pShareMem->u8MJPEG_bQueueFull = 0xff;
}

MS_BOOL MDrv_MJPEG_IsQueueFull(MS_U32 u32Id)
{
    volatile HVD_ShareMem *pShareMem = NULL;
    MsOS_ReadMemory();
    pShareMem = (HVD_ShareMem *)MDrv_MJPEG_GetShareMemAddr(u32Id);
    if((MS_U32)pShareMem==0xFFFFFFFF)
    {
        MJPEG_DEBUG_DRV_ERR("Cannot get pShareMem correctly!!!\n");
        return FALSE;
    }
    MS_U32 u32Timer;

    u32Timer = MsOS_GetSystemTime();
    while(1)
    {
        if (MsOS_GetSystemTime()-u32Timer > 1000)
        {
            MJPEG_DEBUG_DRV_ERR("MDrv_MJPEG_IsQueueFull() timeout...\n");
            break;
        }
        MsOS_ReadMemory();
        if(pShareMem->u8MJPEG_bQueueFull!=0xff)
        {
            MJPEG_DEBUG_DRV_ERR("[%ld]ms\n", MsOS_GetSystemTime()-u32Timer);
            break;
        }
    }
    return (MS_BOOL)(pShareMem->u8MJPEG_bQueueFull);

}
#endif
#endif

