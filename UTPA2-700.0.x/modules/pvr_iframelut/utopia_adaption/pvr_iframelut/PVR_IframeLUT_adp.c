//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file  PVR_IframeLUT_adp.c
/// @brief  PVR IframeLUT adaption API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/compat.h>

#include "drvPVR_IframeLUT_v2.h"
#include "PVR_IframeLUT_adp.h"

#include "utopia.h"
#include "utopia_adp.h"

UADP_STRUCT_POINTER_TABLE spt_PVR_IframeLUT_Open[3];
UADP_STRUCT_POINTER_TABLE spt_PVR_IframeLUT_SetVdecType[3];
UADP_STRUCT_POINTER_TABLE spt_PVR_IframeLUT_MS_U32[3];
UADP_STRUCT_POINTER_TABLE spt_PVR_IframeLUT_GetWritePtr[3];
UADP_STRUCT_POINTER_TABLE spt_PVR_IframeLUT_Close[3];

MS_U32 PVR_IFRAMELUT_adp_Init(FUtopiaIOctl *pIoctl)
{
    PVR_IFRAMELUT_GETWRITEPTR_PARAM *pPVR_IframeLUT_GetWritePtr;

    UADPBypassSetSPT(&spt_PVR_IframeLUT_Open[0], UADP_SPT_SELF_SIZE, 0, sizeof(PVR_IFRAMELUT_OPEN_PARAM));
    UADPBypassSetSPT(&spt_PVR_IframeLUT_Open[1], UADP_SPT_END, 0, 0);

    UADPBypassSetSPT(&spt_PVR_IframeLUT_SetVdecType[0], UADP_SPT_SELF_SIZE, 0, sizeof(PVR_IFRAMELUT_SETVDECTYPE_PARAM));
    UADPBypassSetSPT(&spt_PVR_IframeLUT_SetVdecType[1], UADP_SPT_END, 0, 0);

    UADPBypassSetSPT(&spt_PVR_IframeLUT_MS_U32[0], UADP_SPT_SELF_SIZE, 0, sizeof(MS_U32));
    UADPBypassSetSPT(&spt_PVR_IframeLUT_MS_U32[1], UADP_SPT_END, 0, 0);

    UADPBypassSetSPT(&spt_PVR_IframeLUT_GetWritePtr[0], UADP_SPT_SELF_SIZE, 0, sizeof(PVR_IFRAMELUT_GETWRITEPTR_PARAM));
    UADPBypassSetSPT(&spt_PVR_IframeLUT_GetWritePtr[1], UADP_SPT_POINTER_TO_NEXT, (unsigned int)&pPVR_IframeLUT_GetWritePtr->pu32WritePtr - (unsigned int)pPVR_IframeLUT_GetWritePtr, (unsigned int)spt_PVR_IframeLUT_MS_U32);
    UADPBypassSetSPT(&spt_PVR_IframeLUT_GetWritePtr[2], UADP_SPT_END, 0, 0);

    UADPBypassSetSPT(&spt_PVR_IframeLUT_Close[0], UADP_SPT_SELF_SIZE, 0, sizeof(PVR_IFRAMELUT_CLOSE_PARAM));
    UADPBypassSetSPT(&spt_PVR_IframeLUT_Close[1], UADP_SPT_END, 0, 0);

    *pIoctl= (FUtopiaIOctl)PVR_IFRAMELUT_adp_Ioctl;

    return 0;
}

MS_U32 PVR_IFRAMELUT_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char argBuffer[32];

    switch(u32Cmd)
    {
        case E_MDrv_CMD_PVR_IframeLUT_Init:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDrv_CMD_PVR_IframeLUT_Open:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_PVR_IframeLUT_Open, NULL, argBuffer, sizeof(argBuffer));
            break;
        case E_MDrv_CMD_PVR_IframeLUT_SetVdecType:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_PVR_IframeLUT_SetVdecType, NULL, argBuffer, sizeof(argBuffer));
            break;
        case E_MDrv_CMD_PVR_IframeLUT_GetWritePtr:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_PVR_IframeLUT_GetWritePtr, spt_PVR_IframeLUT_GetWritePtr, argBuffer, sizeof(argBuffer));
            break;
        case E_MDrv_CMD_PVR_IframeLUT_Close:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_PVR_IframeLUT_Close, NULL, argBuffer, sizeof(argBuffer));
            break;
        case E_MDrv_CMD_PVR_IframeLUT_Exit:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        default:
            break;
    }

    return u32Ret;
}
