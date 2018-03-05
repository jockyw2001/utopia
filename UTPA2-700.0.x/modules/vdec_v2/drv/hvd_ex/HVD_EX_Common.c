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
#ifndef MSOS_TYPE_NUTTX

// Common Definition
#include <string.h>
#include "drvHVD_Common.h"
#include "drvHVD_EX.h"
#include "drvHVD_def.h"
#include "HVD_EX_Common.h"

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
MS_BOOL bModeSelectHD = FALSE;
static MS_BOOL bAutoIpcheck = FALSE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);            //FROM #include "drvIPAUTH.h"
#endif

MS_BOOL  _MDrv_HVD_AUTH_IPCheck(MS_U32 u32CodecType)
{
    MS_BOOL bIPAUTHReady = FALSE;
#if !defined(SUPPORT_X_MODEL_FEATURE)
    #define HVD_CODEC_INFO_MASK 0x00000007
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
        default:
            bIPAUTHReady = FALSE;
            break;
    }

    if(bIPAUTHReady == FALSE)
    {
        HVD_EX_MSG_ERR("[%s]User using invaild key !!!\n",__FUNCTION__);
    }
#else
    bModeSelectHD =TRUE;
    bIPAUTHReady =TRUE;
    bAutoIpcheck =TRUE;
#endif
    return bIPAUTHReady;
}

extern HVD_EX_Result _HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle);

HVD_EX_Result _HVD_EX_Init_(MS_U32 u32Id)
{
	HVD_EX_Result eRst = E_HVD_EX_FAIL;
	if(bAutoIpcheck)
	{
		eRst = _HVD_EX_Rst(u32Id ,FALSE);
	}else
	{
		printf("[%s] .. STOP and Contact VDEC team!!!\n",__FUNCTION__);
	}

	return eRst;
}

#endif

