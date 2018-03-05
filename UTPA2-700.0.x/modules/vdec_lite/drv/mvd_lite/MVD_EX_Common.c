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
// file   MVD_EX_Common.c
// @brief  MPEG Video Decoder (MVD) OSAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)
#ifndef CONFIG_MBOOT  //Add For GPL (content protection)

#include "MsCommon.h"
#include "drvMVD_EX.h"
#include "halVPU_EX.h"
#include "controller.h"
#include "halMVD_EX.h"
#include "fwHVD_if.h"
#include "../hvd_lite/drvHVD_def.h"
#include "drvSYS.h"


#define IPAUTH_VIDEO_MPEG2          (19)
#define IPAUTH_VIDEO_MPEG2_HD       (20)
#define IPAUTH_VIDEO_MPEG4          (21)
#define IPAUTH_VIDEO_MPEG4_SD       (22)
#define IPAUTH_VIDEO_MPEG4_HD       (23)
#define IPAUTH_VIDEO_DIVX_1080PHD   (24)
#define IPAUTH_VIDEO_DIVX_DRM       (25)
#define IPAUTH_VIDEO_DIVX_PLUS      (26)
#define IPAUTH_VIDEO_VC1            (29)
#define IPAUTH_VIDEO_WMV            (30)
#define IPAUTH_VIDEO_WMDRM_PD       (31)
#define IPAUTH_VIDEO_WMDRM_ND       (32)
#define IPAUTH_VIDEO_FLV            (34)
#define IPAUTH_VIDEO_DIVX_QMOBILE   (35)
#define IPAUTH_VIDEO_DIVX_MOBILE    (36)
#define IPAUTH_VIDEO_DIVX_HT        (37)
#define IPAUTH_VIDEO_DIVX_720PHD    (38)
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);
#endif

MS_BOOL MDrv_MVD_AUTH_IPCheck(MS_U32 u32CodecType,MS_BOOL* bSupportSDModeOnly)
{
    MS_BOOL bIPAUTHReady = FALSE;
    *bSupportSDModeOnly    = TRUE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
    switch (u32CodecType)
    {
        case E_MVD_CODEC_MPEG2:
           if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2_HD) == TRUE)
           {
               bIPAUTHReady = TRUE;
               *bSupportSDModeOnly = FALSE;
           }
           else if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2) == TRUE)
           {
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_MPEG4:
       case E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER:
           if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_HD) == TRUE)
           {
               bIPAUTHReady = TRUE;
               *bSupportSDModeOnly = FALSE;
           }
           else if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4) == TRUE) || (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_SD) == TRUE))
           {
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_DIVX311:
           if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_1080PHD) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_DRM) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HT) == TRUE))
           {
               bIPAUTHReady = TRUE;
               *bSupportSDModeOnly = FALSE;
           }
           else if((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_QMOBILE) == TRUE) || (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_MOBILE) == TRUE))
           {
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_FLV:
           if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_FLV) == TRUE)
           {
               *bSupportSDModeOnly = FALSE;
               bIPAUTHReady = TRUE;
           }
           break;
       case E_MVD_CODEC_VC1_ADV:
       case E_MVD_CODEC_VC1_MAIN:
           if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VC1) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMV) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMDRM_PD) == TRUE)
               ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMDRM_ND) == TRUE))
           {
               *bSupportSDModeOnly =FALSE;
               bIPAUTHReady = TRUE;
           }
           break;
       default:
             bIPAUTHReady = FALSE;;
    }

    if(bIPAUTHReady == FALSE)
    {
        HVD_PRINT("%s fail!!\r\n", __FUNCTION__);
    }
#endif
    return bIPAUTHReady;
}
#endif


#ifdef VDEC3
#define MVD_MAX_STREAMS         16
#else
#define MVD_MAX_STREAMS         2
#endif

#define VPU_MAIL_BOX_SEND_FAIL 1
#define CODEC_KEY_COMPARE_TIMEOUT 2
extern MS_U8 u8CodecCompare[MVD_MAX_STREAMS];
extern HVD_Return HAL_HVD_EX_SetCmd(MS_U32 u32Id, HVD_User_Cmd eUsrCmd, MS_U32 u32CmdArg);
#define _VDEC_GET_IDX(x)    (MS_U8)((x)>>24)

#if defined(CHIP_KANO)
MS_BOOL MDrv_MVDSendVPUCMD(MS_U32 u32Id,MS_U32 u32CMD,MS_U32 u32Arg)
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

MS_BOOL MDrv_MVDSendCodecKey(MS_U32 u32Id,MS_U8 u8Decoder,MS_U16 u16CodecKey)
{
    MS_U8 u8Idx = _VDEC_GET_IDX(u32Id);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetOffsetIdx(u32Id);
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U32 u32Timeout = 0;
    struct _ctl_info *ctl_ptr = (struct _ctl_info *) MsOS_PA2KSEG1(HAL_VPU_EX_GetFWCodeAddr(u32Id) + CTL_INFO_ADDR);

    if(MDrv_MVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_CMD1,(((VDEC_TAG<<24)&0xff000000))|(((u8Decoder<<16)&0xff0000))|(u16CodecKey&0xffff)) == FALSE)
    {
        HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MDrv_MVD_GetVpuStreamId(u32Id));
        return FALSE;
    }

    if(MDrv_MVDSendVPUCMD(u32Id,E_NST_CMD_COMMON_MASK,0) == FALSE)
    {
        HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MDrv_MVD_GetVpuStreamId(u32Id));
        return FALSE;
    }

    MsOS_ReadMemory();
    u32Timeout = MsOS_GetSystemTime() + 500; //timeout 500 ms
    while (CTL_TASK_CMD != ctl_ptr->task_statue[u8VPUIdx])
    {
        if (MsOS_GetSystemTime() > u32Timeout)
        {
            HVD_PRINT("[Info]Case2\n");
            u8CodecCompare[u8Idx] = CODEC_KEY_COMPARE_TIMEOUT;
            HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)MDrv_MVD_GetVpuStreamId(u32Id));
            return FALSE;
        }
        MsOS_ReadMemory();
    }
    return TRUE;
}
#endif

MS_BOOL _MVD_Init(MS_U32 u32Id, MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing)
{
    MS_U8 u8HalIdx = MVD_GetHalIdx(u32Id);
    MS_U8 u8ChipECORev = MDrv_SYS_GetChipRev();
    //MVD_PRINT("u8CodecType=0x%x\n", u8CodecType);
    //MVD_PRINT("u8BSProviderMode=0x%x\n", u8BSProviderMode);
    if (HAL_MVD_SetCodecInfo(u8HalIdx, u8CodecType, u8BSProviderMode, bDisablePESParsing, u8ChipECORev) == FALSE)
    {
        HVD_PRINT("%s fail!!\r\n", __FUNCTION__);
        return FALSE;
    }

    #ifndef CONFIG_MBOOT  //Add For GPL (content protection)
    #if defined(CHIP_KANO)
    if(MDrv_MVDSendCodecKey(u32Id,MVD_DECODER,u8CodecType) == FALSE)
    {
        return FALSE;
    }
    else
    #endif
    #endif
    {
        return TRUE;
    }
}

#endif
