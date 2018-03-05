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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMS.c
/// @author MStar Semiconductor,Inc.
/// @brief  DMS Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "drvDMS.h"
#include "apiDMS.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDMS_v2.h"
#include "drvDMS.h"
#include "halDMS.h"
#include "apiDMS_private.h"
#include "UFO.h"

// for Utopia 2.0 to Utopia 1.0 compatibility.
void* g_pDMSInst = NULL;

EN_DMS_RESULT MApi_DMS_Init_U2(void* pInstance, ST_DMS_INITDATA *pstDMS_InitData)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Init(pInstance,pstDMS_InitData);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Init(ST_DMS_INITDATA *pstDMS_InitData)
{
    if (g_pDMSInst == NULL)
    {
        if(UtopiaOpen(MODULE_DMS, &g_pDMSInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("[%s]UtopiaOpen DMS failed\n", __FUNCTION__);
            return E_DMS_FAIL;
        }
    }

    if(pstDMS_InitData == NULL)
    {
        printf("[%s,%5d] pstDMS_InitData is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_INIT DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.pstDMS_InitData = pstDMS_InitData;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_INIT, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_SetMemory_U2(void* pInstance, ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_SetMemoryType(pInstance,pstDMS_SetMemType);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_SetMemory(ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstDMS_SetMemType == NULL)
    {
        printf("[%s,%5d] pstDMS_SetMemType is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_SETMEMTYPE DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.pstDMS_SetMemType = pstDMS_SetMemType;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETMEMORY, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_CreateWindow_U2(void* pInstance, ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_CreateWindow(pInstance, pstOutputWin, 0, pu32WindowID);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_CreateWindow(ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_CREATEWIN DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.pstOutputWin = pstOutputWin;
    DMSArgs.pstCreateWin_Info = pstCreateWin_Info;
    DMSArgs.pu32WindowID = pu32WindowID;

    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_CREATEWINDOW, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_SetDigitalDecodeSignalInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_SetDigitalDecodeSignalInfo(pInstance, u32WindowID, pstDispFrameFormat);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_SetDigitalDecodeSignalInfo(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_SETDIGIDECINFO DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstDispFrameFormat = pstDispFrameFormat;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETDIGITALDECODESIGNALINFO, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_ClearDigitalDecodeSignalInfo_U2(void* pInstance, MS_U32 u32WindowID)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_ClearDigitalDecodeSignalInfo(pInstance, u32WindowID);
    return E_DMS_OK;
}

EN_DMS_RESULT MApi_DMS_ClearDigitalDecodeSignalInfo(MS_U32 u32WindowID)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_CLEARDIGIDECINFO DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_CLEARDIGITALDECODESIGNALINFO, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_DestroyWindow_U2(void* pInstance, MS_U32 u32WindowID)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_DestroyWindow(pInstance, u32WindowID);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_DestroyWindow(MS_U32 u32WindowID)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_DESTORYWIN DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_DESTORYWINDOW, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_SetWindow_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_SetWindow(pInstance,u32WindowID,pstDMS_SetWin_Info);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_SetWindow(MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstDMS_SetWin_Info == NULL)
    {
        printf("[%s,%5d] pstDMS_SetWin_Info is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_SETWIN DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstDMS_SetWin_Info = pstDMS_SetWin_Info;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETWINDOW, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_GetWindowInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info)
{
    printf("[%s,%5d] Not Impl\n",__FUNCTION__,__LINE__);
    return E_DMS_OK;
}

EN_DMS_RESULT MApi_DMS_GetWindowInfo(MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstDMS_GetWin_Info == NULL)
    {
        printf("[%s,%5d] pstDMS_GetWin_Info is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_GETWININFO DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstDMS_GetWin_Info = pstDMS_GetWin_Info;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_GETWINDOWINFO, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Video_Flip_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Video_Flip(pInstance, u32WindowID, pstDispFrameFormat);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Video_Flip(MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstDispFrameFormat == NULL)
    {
        printf("[%s,%5d] pstDispFrameFormat is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_FLIP DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstDispFrameFormat = pstDispFrameFormat;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_VIDEOFLIP, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_SetZOrder_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_ZORDER_INFO *pstZorderInfo)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_SetZOrder(pInstance, u32WindowID, pstZorderInfo->u32Zorder);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_SetZOrder(MS_U32 u32WindowID, ST_DMS_ZORDER_INFO *pstZorderInfo)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstZorderInfo == NULL)
    {
        printf("[%s,%5d] pstZorderInfo is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_ZORDER DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstZorderInfo = pstZorderInfo;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETZORDER, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Video_Freeze_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Video_Freeze(pInstance, u32WindowID, pstWindowFreeze->u32Enable);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Video_Freeze(MS_U32 u32WindowID, ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstWindowFreeze == NULL)
    {
        printf("[%s,%5d] pstWindowFreeze is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_FREEZE DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstWindowFreeze = pstWindowFreeze;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_VIDEOFREEZE, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Video_Mute_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_MUTE_WINDOW_INFO *pstWindowMute)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Video_Mute(pInstance, u32WindowID, pstWindowMute->u32Enable);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Video_Mute(MS_U32 u32WindowID, ST_DMS_MUTE_WINDOW_INFO *pstWindowMute)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstWindowMute == NULL)
    {
        printf("[%s,%5d] pstWindowMute is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_MUTE DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstWindowMute = pstWindowMute;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_VIDEOMUTE, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Set_MuteColor_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_MUTE_COLOR_INFO *pstMuteColor)
{
    printf("[%s,%5d] Not Impl\n",__FUNCTION__,__LINE__);
    return E_DMS_OK;
}

EN_DMS_RESULT MApi_DMS_Set_MuteColor(MS_U32 u32WindowID, ST_DMS_MUTE_COLOR_INFO *pstMuteColor)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstMuteColor == NULL)
    {
        printf("[%s,%5d] pstMuteColor is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_MUTECOLOR DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstMuteColor = pstMuteColor;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETMUTECOLOR, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_GetStatus_U2(void* pInstance, EN_DMS_STATUS *peStatus)
{
    printf("[%s,%5d] Not Impl\n",__FUNCTION__,__LINE__);
    return 0;
}

EN_DMS_RESULT MApi_DMS_GetStatus(EN_DMS_STATUS *peStatus)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_GETSTATUS DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.peStatus = peStatus;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_GETSTATUS, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_GetCapability_U2(void* pInstance, EN_DMS_CAPABILITY *peCapability)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_GetCapability(pInstance, peCapability);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_GetCapability(EN_DMS_CAPABILITY *peCapability)
{
    if (g_pDMSInst == NULL)
    {
        if(UtopiaOpen(MODULE_DMS, &g_pDMSInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("[%s]UtopiaOpen DMS failed\n", __FUNCTION__);
            return E_DMS_FAIL;
        }
    }

    ST_DMS_GETCAPS DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.peCapability = peCapability;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_GETCAPS, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_GetExistedWindows_U2(void* pInstance, MS_U32 *pu32WindowNums)
{
    printf("[%s,%5d] Not Impl\n",__FUNCTION__,__LINE__);
    return 0;
}

EN_DMS_RESULT MApi_DMS_GetExistedWindows(MS_U32 *pu32WindowNums)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pu32WindowNums == NULL)
    {
        printf("[%s,%5d] pu32WindowNums is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_GETEXISTWIN DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.pu32WindowNums = pu32WindowNums;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_GETEXISTEDWINDOW, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_SetOutputLayer_U2(void* pInstance, ST_DMS_WINDOW *pstLayer)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_SetOutputLayer(pInstance, pstLayer);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_SetOutputLayer(ST_DMS_WINDOW *pstLayer)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstLayer == NULL)
    {
        printf("[%s,%5d] pstLayer is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_SETOUTPUTLAYER DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.pstLayer = pstLayer;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETOUTPUTLAYER, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Set_3D_Mode_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Set_3D_Mode(pInstance, u32WindowID, pst3DInfo);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Set_3D_Mode(MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pst3DInfo == NULL)
    {
        printf("[%s,%5d] pstMuteColor is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_SET3DMODE DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pst3DInfo = pst3DInfo;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SET3DMODE, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Set_CaptureInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Set_CaptureInfo(pInstance, u32WindowID, pstSetCaptureInfo);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Set_CaptureInfo(MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstSetCaptureInfo == NULL)
    {
        printf("[%s,%5d] pstSetCaptureInfo is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_SETCAPTUREINFO DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstSetCaptureInfo = pstSetCaptureInfo;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_SETCAPTUREINFO, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Get_CaptureBuffer_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Get_CaptureBuffer(pInstance, u32WindowID, pstCaptureInfo);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Get_CaptureBuffer(MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstCaptureInfo == NULL)
    {
        printf("[%s,%5d] pstCaptureInfo is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_GETCAPTUREBUF DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstCaptureInfo = pstCaptureInfo;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_GETCAPTUREBUFFER, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

EN_DMS_RESULT MApi_DMS_Release_CaptureBuffer_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    EN_DMS_RESULT eRet = E_DMS_FAIL;
    eRet = MDrv_DMS_Release_CaptureBuffer(pInstance, u32WindowID, pstCaptureInfo);
    return eRet;
}

EN_DMS_RESULT MApi_DMS_Release_CaptureBuffer(MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    if (g_pDMSInst == NULL)
    {
        printf("[%s,%5d] No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    if(pstCaptureInfo == NULL)
    {
        printf("[%s,%5d] pstCaptureInfo is NULL\n",__FUNCTION__,__LINE__);
        return E_DMS_FAIL;
    }

    ST_DMS_RELEASECAPTUREBUF DMSArgs;
    DMSArgs.eReturnValue = E_DMS_FAIL;
    DMSArgs.u32WindowID = u32WindowID;
    DMSArgs.pstCaptureInfo = pstCaptureInfo;
    if(UtopiaIoctl(g_pDMSInst, E_DMS_CMD_RELEASECAPTUREBUFFER, (void*)&DMSArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Obtain DMS engine fail\n",__FUNCTION__,__LINE__);
    }
    return DMSArgs.eReturnValue;
}

