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
/// file    drvHVD_EX.c
/// @brief  HVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "drvHVD_Common.h"
#include "drvHVD_EX.h"
#include "drvHVD_def.h"
#include "HVD_EX_Common.h"
#include "controller.h"
#include "fwHVD_if.h"
#include "halVPU_EX.h"
#include "halHVD_EX.h"
#include "drvSYS.h"

#if HVD_ENABLE_AUTO_SET_REG_BASE
#include "drvMMIO.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Functions Prototype
//-------------------------------------------------------------------------------------------------

#define IPAUTH_VIDEO_H264           (27)
#define IPAUTH_VIDEO_RM             (28)
#define IPAUTH_VIDEO_AVS            (33)
#define IPAUTH_VIDEO_MVC            (40)
#define IPAUTH_VIDEO_VP8            (61)
#define IPAUTH_VIDEO_HEVC           (75)
#define IPAUTH_VIDEO_VP9            (91)
#define IPAUTH_VIDEO_DOLBY_HDR      (118)
MS_BOOL bModeSelectHD = TRUE;
static MS_BOOL bAutoIpcheck = TRUE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);            //FROM #include "drvIPAUTH.h"
#endif

#define HVD_CODEC_INFO_MASK 0x0000000f

MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType)
{
    MS_BOOL bIPAUTHReady = FALSE;
    VDEC_EX_CODEC_CAP_INFO pCodecCapInfo;
#if !defined(SUPPORT_X_MODEL_FEATURE)
    u32CodecType = (u32CodecType & HVD_CODEC_INFO_MASK);
    bAutoIpcheck = FALSE;
    switch (u32CodecType)
    {
        case HVD_INIT_HW_AVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_H264) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady = TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_AVS:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_AVS) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady = TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_RM:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_RM) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_MVC:  /// SUPPORT_MVC
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MVC) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_VP8:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VP8) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_HEVC:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_HEVC) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_VP9:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VP9) == TRUE && HAL_VPU_EX_GetCodecCapInfo( E_HVD_EX_CODEC_TYPE_VP9, &pCodecCapInfo) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        case HVD_INIT_HW_HEVC_DV:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DOLBY_HDR) == TRUE)
            {
                bModeSelectHD =TRUE;
                bIPAUTHReady =TRUE;
                bAutoIpcheck =TRUE;
            }
            break;
        default:
            bIPAUTHReady = FALSE;
            break;
    }

    if(bIPAUTHReady == FALSE)
    {
        HVD_EX_MSG_ERR("[%s]User using invaild key !!!\n",__FUNCTION__);
    }
#endif
    return bIPAUTHReady;
}

#ifdef VDEC3
#define HVD_MAX_STREAMS         16
#else
#define HVD_MAX_STREAMS         2
#endif

#define VPU_MAIL_BOX_SEND_FAIL 1
#define CODEC_KEY_COMPARE_TIMEOUT 2
extern MS_U8 u8CodecCompare[HVD_MAX_STREAMS];

extern HVD_EX_Result _HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle);

#define _VDEC_GET_IDX(x)    (MS_U8)((x)>>24)

#if defined(CHIP_K6LITE) || defined(CHIP_K6)
MS_BOOL _MDrv_HVDSendVPUCMD(MS_U32 u32Id,MS_U32 u32CMD,MS_U32 u32Arg)
{
    MS_BOOL bRet = TRUE;
    MS_U8  u8Idx = _VDEC_GET_IDX(u32Id);

    HAL_VPU_EX_Mutex_Lock();
    if(HAL_HVD_EX_SetCmd(u32Id, u32CMD, u32Arg) != E_HVD_RETURN_SUCCESS)
    {
        bRet = FALSE;
    }
    HAL_VPU_EX_Mutex_UnLock();

    if(bRet == FALSE)
    {
        HVD_PRINT("[Info]Case1\n");
        u8CodecCompare[u8Idx] = VPU_MAIL_BOX_SEND_FAIL;
    }

    return bRet;
}

MS_BOOL _MDrv_HVDSendCodecKey(MS_U32 u32Id,MS_U8 u8Decoder,MS_U16 u16CodecKey)
{
    MS_U8 u8Idx = HAL_VPU_EX_GetOffsetIdx(u32Id);
    MS_U32 u32Timeout = 0;
    struct _ctl_info *ctl_ptr = (struct _ctl_info *) MsOS_PA2KSEG1(HAL_VPU_EX_GetFWCodeAddr(u32Id) + CTL_INFO_ADDR);

    if(_MDrv_HVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_CMD1,(((VDEC_TAG<<24)&0xff000000))|(((u8Decoder<<16)&0xff0000))|(u16CodecKey&0xffff)) == FALSE)
    {
        HAL_HVD_EX_DeInit(u32Id);
        return FALSE;
    }

    if(_MDrv_HVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_MASK,0) == FALSE)
    {
        HAL_HVD_EX_DeInit(u32Id);
        return FALSE;
    }

    MsOS_ReadMemory();
    u32Timeout = HVD_GetSysTime_ms() + 500; //timeout 500 ms
    while (CTL_TASK_CMD != ctl_ptr->task_statue[u8Idx])
    {
        if (HVD_GetSysTime_ms() > u32Timeout)
        {
            HVD_PRINT("[Info]Case2\n");
            u8CodecCompare[u8Idx] = CODEC_KEY_COMPARE_TIMEOUT;
            HAL_HVD_EX_DeInit(u32Id);
            return FALSE;
        }
        MsOS_ReadMemory();
    }

    return TRUE;
}
#endif


HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id,MS_U32 u32ModeFlag)
{
    HVD_EX_Result eRst = E_HVD_EX_FAIL;

    UNUSED(u32ModeFlag);
    if(bAutoIpcheck)
    {
        eRst = _HVD_EX_Rst(u32Id ,FALSE);
#ifndef CONFIG_MBOOT
#if defined(CHIP_K6LITE) || defined(CHIP_K6)
        MS_U32 u32CodecType = u32ModeFlag & HVD_CODEC_INFO_MASK;
        if(eRst == E_HVD_EX_OK)
        {
            if(u32CodecType != HVD_INIT_HW_MJPEG)
            {
                if(_MDrv_HVDSendCodecKey(u32Id,HVD_DECODER,u32CodecType) == FALSE)
                {
                    eRst = E_HVD_EX_FAIL;
                }
            }
        }
#endif
#endif
    }
    else
    {
        HVD_PRINT("[%s] .. STOP and Contact VDEC team!!!\n",__FUNCTION__);
    }

    return eRst;
}

#else

#include "drvHVD_def.h"
#include "HVD_EX_Common.h"
#include "controller.h"
#include "halHVD_EX.h"

MS_BOOL _MDrv_HVD_EX_TEE_AllocateEsBufHandle_(MS_U8 u8Idx, MS_PHY pVPUCodecAddr, MS_U32 u32ReqSize, MS_U32 u32BuffEnd, MS_U32 *pu32EsHandle)
{
    VDEC_ESMap_Table *pEsHandleQueue = (VDEC_ESMap_Table *) HAL_HVD_EX_GetVpuShmAddr(u8Idx, pVPUCodecAddr);
    const MS_U32 u32QueueSize = sizeof(pEsHandleQueue->stEntry) / sizeof(VDEC_ESMap_Entry);

    //HVD_ERR("allocate with read %d write %d\n", pEsHandleQueue->u32RdPtr, pEsHandleQueue->u32WrPtr);
    // queue is empty
    if (pEsHandleQueue->u32WrPtr == pEsHandleQueue->u32RdPtr)
    {
        pEsHandleQueue->stEntry[pEsHandleQueue->u32WrPtr].u32Offset = 0;
        pEsHandleQueue->stEntry[pEsHandleQueue->u32WrPtr].u32Length = u32ReqSize;
    }
    else
    {
        MS_U32 u32PrevWptr, u32Vacancy;
        MS_U32 u32WriteOffset, u32ReadOffset, u32WriteEndOffset;

        if (pEsHandleQueue->u32WrPtr == 0)
            u32PrevWptr = u32QueueSize - 1;
        else
            u32PrevWptr = pEsHandleQueue->u32WrPtr - 1;
        // check if queue is full
        if (pEsHandleQueue->u32WrPtr == pEsHandleQueue->u32RdPtr)
        {
            u32Vacancy = u32QueueSize;
        }
        else if (pEsHandleQueue->u32WrPtr > pEsHandleQueue->u32RdPtr)
        {
            u32Vacancy = u32QueueSize - (pEsHandleQueue->u32WrPtr - pEsHandleQueue->u32RdPtr);
        }
        else
        {
            // pEsHandleQueue->u32RdPtr > pEsHandleQueue->u32WrPtr
            u32Vacancy = pEsHandleQueue->u32RdPtr - pEsHandleQueue->u32WrPtr;
        }
        u32Vacancy--; // read write pointer should not be the same after inserting one entry
        if (u32Vacancy == 0)
            return FALSE;

        // check free size and if we need to wrap around in ES buffer
        u32WriteOffset = pEsHandleQueue->stEntry[u32PrevWptr].u32Offset + pEsHandleQueue->stEntry[u32PrevWptr].u32Length;
        u32ReadOffset = pEsHandleQueue->stEntry[pEsHandleQueue->u32RdPtr].u32Offset;

        u32WriteEndOffset = u32WriteOffset + u32ReqSize;

        if (u32ReadOffset <= u32WriteOffset)
        {
            if (u32WriteEndOffset >= u32BuffEnd)
            {
                u32WriteOffset = 0;
                u32WriteEndOffset = u32ReqSize; // start from 0

                if (u32WriteEndOffset > u32ReadOffset)
                {
                    return FALSE;
                }
            }
        }
        else
        {
            if (u32WriteEndOffset > u32ReadOffset)
            {
                return FALSE;
            }
        }

        pEsHandleQueue->stEntry[pEsHandleQueue->u32WrPtr].u32Offset = u32WriteOffset;
        pEsHandleQueue->stEntry[pEsHandleQueue->u32WrPtr].u32Length = u32ReqSize;
    }

    *pu32EsHandle = pEsHandleQueue->u32WrPtr;

    pEsHandleQueue->u32WrPtr++;
    if (pEsHandleQueue->u32WrPtr >= u32QueueSize)
        pEsHandleQueue->u32WrPtr = 0;

    MsOS_FlushMemory();
    return TRUE;
}

MS_BOOL _MDrv_HVD_EX_TEE_GetESBufByHandle_(MS_U8 u8Idx, MS_PHY pVPUCodecAddr, MS_U32 u32EsHandle, MS_PHY *pPhyAddr)
{
    VDEC_ESMap_Table *pEsHandleQueue = (VDEC_ESMap_Table *) HAL_HVD_EX_GetVpuShmAddr(u8Idx, pVPUCodecAddr);

    if (u32EsHandle >= sizeof(pEsHandleQueue->stEntry) / sizeof(VDEC_ESMap_Entry))
        return FALSE;

    *pPhyAddr = pEsHandleQueue->stEntry[u32EsHandle].u32Offset;
    return TRUE;
}

#endif
