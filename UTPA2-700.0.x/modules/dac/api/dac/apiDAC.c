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
/// file   apiDAC.c
/// @author MStar Semiconductor,Inc.
/// @brief  DAC Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "mdrv_dac_tbl.h"
#include "drvDAC.h"
#include "drvMMIO.h"
#include "apiDAC.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDAC_private.h"
#include "apiDAC_v2.h"
#include "UFO.h"

static MSIF_Version _api_dac_version = {
    .DDI = { DAC_API_VERSION },
};

// for Utopia 2.0 to Utopia 1.0 compatibility.
void* g_pDACInst;
void* g_pDACRes;

MS_U32 MDrv_DAC_Get_Semaphore(void* pInstance)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if(UtopiaResourceObtain(pModule, E_DAC_POOL_ID_INTERNAL_VARIABLE, &g_pDACRes) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}

MS_U32 MDrv_DAC_Release_Semaphore(void)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    u32Return = UtopiaResourceRelease(g_pDACRes);
    return u32Return;
}

MS_BOOL _MApi_DAC_VideoTiming_ReMapping(E_OUTPUT_VIDEO_TIMING_TYPE eApiTiming, EN_OUTPUT_VIDEO_TIMING_TYPE* eDrvTiming)
{
    if (eApiTiming == E_RES_720x480I_60Hz)
    {
        *eDrvTiming = RES_720x480I_60Hz;
    }
    else if (eApiTiming == E_RES_720x480P_60Hz)
    {
        *eDrvTiming = RES_720x480P_60Hz;
    }
    else if (eApiTiming == E_RES_720x576I_50Hz)
    {
        *eDrvTiming = RES_720x576I_50Hz;
    }
    else if (eApiTiming == E_RES_720x576P_50Hz)
    {
        *eDrvTiming = RES_720x576P_50Hz;
    }
    else if (eApiTiming == E_RES_1280x720P_50Hz)
    {
        *eDrvTiming = RES_1280x720P_50Hz;
    }
    else if (eApiTiming == E_RES_1280x720P_60Hz)
    {
        *eDrvTiming = RES_1280x720P_60Hz;
    }
    else if (eApiTiming == E_RES_1920x1080I_50Hz)
    {
        *eDrvTiming = RES_1920x1080I_50Hz;
    }
    else if (eApiTiming == E_RES_1920x1080I_60Hz)
    {
        *eDrvTiming = RES_1920x1080I_60Hz;
    }
    else if (eApiTiming == E_RES_1920x1080P_24Hz)
    {
        *eDrvTiming = RES_1920x1080P_24Hz;
    }
    else if (eApiTiming == E_RES_1920x1080P_25Hz)
    {
        *eDrvTiming = RES_1920x1080P_25Hz;
    }
    else if (eApiTiming == E_RES_1920x1080P_30Hz)
    {
        *eDrvTiming = RES_1920x1080P_30Hz;
    }
    else if (eApiTiming == E_RES_1920x1080P_50Hz)
    {
        *eDrvTiming = RES_1920x1080P_50Hz;
    }
    else if (eApiTiming == E_RES_1920x1080P_60Hz)
    {
        *eDrvTiming = RES_1920x1080P_60Hz;
    }
    else if (eApiTiming == E_RES_VGA640x480P_60Hz)
    {
        *eDrvTiming = RES_VGA640x480P_60Hz;
    }
    else if (eApiTiming == E_RES_640x480P_60Hz)
    {
        *eDrvTiming = RES_640x480P_60Hz;
    }
    else if (eApiTiming == E_RES_TTL480X272P_60Hz)
    {
        *eDrvTiming = RES_TTL480X272P_60Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P_24Hz)
    {
        *eDrvTiming = RES_3840x2160P_24Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P_25Hz)
    {
        *eDrvTiming = RES_3840x2160P_25Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P_30Hz)
    {
        *eDrvTiming = RES_3840x2160P_30Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P_50Hz)
    {
        *eDrvTiming = RES_3840x2160P_50Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P_60Hz)
    {
        *eDrvTiming = RES_3840x2160P_60Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P_24Hz)
    {
        *eDrvTiming = RES_4096x2160P_24Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P_25Hz)
    {
        *eDrvTiming = RES_4096x2160P_25Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P_30Hz)
    {
        *eDrvTiming = RES_4096x2160P_30Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P_50Hz)
    {
        *eDrvTiming = RES_4096x2160P_50Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P_60Hz)
    {
        *eDrvTiming = RES_4096x2160P_60Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P420_30Hz)
    {
        *eDrvTiming = RES_3840x2160P420_30Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P420_50Hz)
    {
        *eDrvTiming = RES_3840x2160P420_50Hz;
    }
    else if (eApiTiming == E_RES_3840x2160P420_60Hz)
    {
        *eDrvTiming = RES_3840x2160P420_60Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P420_30Hz)
    {
        *eDrvTiming = RES_4096x2160P420_30Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P420_50Hz)
    {
        *eDrvTiming = RES_4096x2160P420_50Hz;
    }
    else if (eApiTiming == E_RES_4096x2160P420_60Hz)
    {
        *eDrvTiming = RES_4096x2160P420_60Hz;
    }
    else if (eApiTiming == E_RES_1600x1200P_60Hz)
    {
        *eDrvTiming = RES_1600x1200P_60Hz;
    }
    else if (eApiTiming == E_RES_1440x900P_60Hz)
    {
        *eDrvTiming = RES_1440x900P_60Hz;
    }
    else if (eApiTiming == E_RES_1280x1024P_60Hz)
    {
        *eDrvTiming = RES_1280x1024P_60Hz;
    }
    else if (eApiTiming == E_RES_1024x768P_60Hz)
    {
        *eDrvTiming = RES_1024x768P_60Hz;
    }
    else if (eApiTiming == E_RES_1920x2205P_24Hz)
    {
        *eDrvTiming = RES_1920x2205P_24Hz;
    }
    else if (eApiTiming == E_RES_1280x1470P_50Hz)
    {
        *eDrvTiming = RES_1280x1470P_50Hz;
    }
    else if (eApiTiming == E_RES_1280x1470P_60Hz)
    {
        *eDrvTiming = RES_1280x1470P_60Hz;
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

APIDAC_Result MApi_DAC_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_APIDAC_FAIL;
    }

    *ppVersion = &_api_dac_version;
    return E_APIDAC_OK;
}

APIDAC_Result MApi_DAC_GetLibVer(const MSIF_Version **ppVersion)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIDAC_FAIL;
    }

    stDAC_GET_LIBVER DACArgs;
    DACArgs.ppVersion = ppVersion;
    DACArgs.eReturnValue = E_APIDAC_FAIL;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_LIBVER, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.eReturnValue;
}

const DAC_ApiInfo * MApi_DAC_GetInfo_U2(void* pInstance)          ///< Get info from driver
{
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_Release_Semaphore();
    return &pDACResourcePrivate->stapiDAC._cstDac_ApiInfo;
}

const DAC_ApiInfo * MApi_DAC_GetInfo(void)          ///< Get info from driver
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return NULL;
    }

    stDAC_GET_INFO DACArgs;
    DACArgs.stReturnValue = NULL;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_INFO, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.stReturnValue;
}

#ifdef UFO_XC_GETOUTPUTINTELACETIMING
MS_BOOL MApi_DAC_GetOutputInterlaceTiming_U2(void* pInstance)
{
    MS_BOOL bRet = FALSE;
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    bRet = MDrv_DAC_GetOutputInterlaceTiming();
    MDrv_DAC_Release_Semaphore();
    return bRet;
}

MS_BOOL MApi_DAC_GetOutputInterlaceTiming(void)          ///< Get interlace or not from driver
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_GET_OUTPUTINTERLACETIMING DACArgs;
    DACArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_GETOUTPUTINTERLACETIMING, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}
#endif

MS_BOOL MApi_DAC_GetStatus_U2(void* pInstance, DAC_ApiStatus *pDacStatus)
{
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    pDacStatus->bDac_Initialized    = pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_Initialized;
    pDacStatus->bDac_VE_enabled     = pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_VE_enabled;
    pDacStatus->bDac_YPbPr_enabled  = pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_YPbPr_enabled;
    MDrv_DAC_Release_Semaphore();
    return TRUE;
}

MS_BOOL MApi_DAC_GetStatus(DAC_ApiStatus *pDacStatus)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_GET_STATUS DACArgs;
    DACArgs.pDacStatus = pDacStatus;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_STATUS, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

MS_BOOL MApi_DAC_SetDbgLevel_U2(void* pInstance, MS_U16 u16DbgSwitch)
{
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    pDACResourcePrivate->stapiDAC._u16DbgSwitch = u16DbgSwitch;
    MDrv_DAC_Release_Semaphore();
    return TRUE;
}

MS_BOOL MApi_DAC_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_SET_DBG_LEVEL DACArgs;
    DACArgs.u16DbgSwitch = u16DbgSwitch;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_DBG_LEVEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}


MS_BOOL MApi_DAC_Init_U2(void* pInstance)
{
    MS_VIRT virtPMBaseAddr = 0;
    MS_PHY phyPMBaseSize = 0;

    if(!MDrv_MMIO_GetBASE(&virtPMBaseAddr, &phyPMBaseSize, MS_MODULE_PM))
    {
        printf("[MAPI_DAC]MApi_DAC_Init Get PM BASE failure!\n");
        return FALSE;
    }

    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_SetIOMapBase(virtPMBaseAddr, virtPMBaseAddr);
    MDrv_DAC_Init();

    pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u32NPMBase = virtPMBaseAddr;
    pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u32PMBase = virtPMBaseAddr;
    pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_Initialized = TRUE;

    // Check DAC table type number
    if(DAC_TABLE_TYPE_NUM != DAC_TABTYPE_NUMS)
    {
        printf("Please check DAC table type number setting!\n");
    }

    MDrv_DAC_Release_Semaphore();
    return TRUE;
}

MS_BOOL MApi_DAC_Init(void)
{
    if (g_pDACInst == NULL)
    {
        if(UtopiaOpen(MODULE_DAC, &g_pDACInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen DAC failed\n");
    		return FALSE;
        }
    }

    stDAC_INIT DACArgs;
    DACArgs.bReturnValue = FALSE;
    if(UtopiaIoctl(g_pDACInst, E_DAC_INIT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}


void MApi_DAC_Enable_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    MDrv_DAC_Get_Semaphore(pInstance);
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_Enable(bEnable, bIsYPbPr);

    if(bIsYPbPr)
    {
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.bHDDACEnable = bEnable;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.bDACIsYPbPr = bIsYPbPr;
    }
    else
    {
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.bSDDACEnable = bEnable;
    }

    /*
     * DAC enable/disable should not do MDrv_DAC_SetOutputSource or
     * MDrv_DAC_SetOutputLevel. In order to make this API downward compatible,
     * MDrv_DAC_SetOutputSource and MDrv_DAC_SetOutputLevel are called
     * only when the first time that MApi_DAC_Enable is called. (HD and SD)
     */
    if (bIsYPbPr && !MDrv_DAC_GetDACState(TRUE)) {
        MDrv_DAC_SetOutputSource(DAC_TO_HDGEN, bIsYPbPr);
        MDrv_DAC_SetOutputLevel(DAC_OUTPUT_LEVEL_LOW, bIsYPbPr);
        //Can not call MApi because of mutex
        //MApi_DAC_SetOutputSwapSel(E_DAC_R_B_G, bIsYPbPr);
        MDrv_DAC_SetDACState(TRUE, TRUE);
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDOutputType = DAC_TO_HDGEN;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDMaxOutputLevel = DAC_OUTPUT_LEVEL_LOW;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDSwapSEL = E_DAC_R_B_G;
    }
    if (!bIsYPbPr && !MDrv_DAC_GetDACState(FALSE)) {
        MDrv_DAC_SetOutputSource(DAC_TO_VE, bIsYPbPr);
        MDrv_DAC_SetOutputLevel(DAC_OUTPUT_LEVEL_HIGH, bIsYPbPr);
        //Can not call MApi because of mutex
        //MApi_DAC_SetOutputSwapSel(E_DAC_R_G_B, bIsYPbPr);
        MDrv_DAC_SetDACState(TRUE, FALSE);
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDOutputType = DAC_TO_VE;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDMaxOutputLevel = DAC_OUTPUT_LEVEL_HIGH;
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDSwapSEL = E_DAC_R_G_B;
    }

    if (bIsYPbPr)
    {
        pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_YPbPr_enabled = bEnable;
    }
    else
    {
        pDACResourcePrivate->stapiDAC._stDac_ApiStatus.bDac_VE_enabled = bEnable;
    }
    MDrv_DAC_Release_Semaphore();
}

void MApi_DAC_Enable(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_ENABLE DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ENABLE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetOutputSource_U2(void* pInstance, E_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetOutputSource((EN_DAC_OUTPUT_TYPE)enOutputType,bIsYPbPr);
    return;
}

void MApi_DAC_SetOutputSource(E_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_OUTPUT_SOURCE DACArgs;
    DACArgs.enOutputType = enOutputType;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_OUTPUT_SOURCE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetOutputLevel_U2(void* pInstance, E_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetOutputLevel((EN_DAC_MAX_OUTPUT_LEVEL)enLevel,bIsYPbPr);
}

void MApi_DAC_SetOutputLevel(E_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_OUTPUT_LEVEL DACArgs;
    DACArgs.enLevel = enLevel;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_OUTPUT_LEVEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetOutputSwapSel_U2(void* pInstance, E_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_SetOutputSwapSel((EN_DAC_SWAP_SEL)enSwap, bIsYPbPr);

    if(bIsYPbPr)
    {
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDSwapSEL = enSwap;
    }
    else
    {
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDSwapSEL = enSwap;
    }

    MDrv_DAC_Release_Semaphore();
}

void MApi_DAC_SetOutputSwapSel(E_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_OUTPUT_SWAPSEL DACArgs;
    DACArgs.enSwap = enSwap;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_OUTPUT_SWAPSEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_SetClkInv_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetClkInv(bEnable, bIsYPbPr);
}

void MApi_DAC_SetClkInv(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_CLKINV DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_CLKINV, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_OnOffSD_U2(void* pInstance, E_DAC_SD_ENABLE_CTRL enBit)
{
    MDrv_DAC_OnOffSD((EN_DAC_SD_ENABLE_CTRL)enBit);
}

void MApi_DAC_OnOffSD(E_DAC_SD_ENABLE_CTRL enBit)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_ONOFF_SD DACArgs;
    DACArgs.enBit = enBit;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ONOFF_SD, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

E_DAC_SD_ENABLE_CTRL MApi_DAC_GetSDStatus_U2(void* pInstance)
{
    E_DAC_SD_ENABLE_CTRL enBit = 0;
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    enBit = MDrv_DAC_GetSDStatus();
    MDrv_DAC_Release_Semaphore();
    return enBit;
}

E_DAC_SD_ENABLE_CTRL MApi_DAC_GetSDStatus(void)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_GET_SD_STATUS DACArgs;
    DACArgs.enBit = E_DAC_SD_OFF;
    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_SD_STATUS, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.enBit;
}

void MApi_DAC_OnOffHD_U2(void* pInstance, E_DAC_HD_ENABLE_CTRL enBit)
{
    MDrv_DAC_OnOffHD((EN_DAC_HD_ENABLE_CTRL)enBit);
}

void MApi_DAC_OnOffHD(E_DAC_HD_ENABLE_CTRL enBit)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_ONOFF_HD DACArgs;
    DACArgs.enBit = enBit;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ONOFF_HD, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

E_DAC_HD_ENABLE_CTRL MApi_DAC_GetHDStatus_U2(void* pInstance)
{
    MS_BOOL bRet = FALSE;
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    bRet = MDrv_DAC_GetHDStatus();
    MDrv_DAC_Release_Semaphore();
    return bRet;
}

E_DAC_HD_ENABLE_CTRL MApi_DAC_GetHDStatus(void)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_GET_HD_STATUS DACArgs;
    DACArgs.enBit = E_DAC_HD_OFF;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_HD_STATUS, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.enBit;
}

//set output timing for YPbPr (hdgen)
void MApi_DAC_SetYPbPrOutputTiming_U2(void* pInstance, E_OUTPUT_VIDEO_TIMING_TYPE eTiming)
{
    EN_OUTPUT_VIDEO_TIMING_TYPE enTimingType;
    MS_BOOL bRet = FALSE;

    bRet = _MApi_DAC_VideoTiming_ReMapping(eTiming, &enTimingType);
    if(!bRet)
    {
        printf("[MAPI_DAC]Error timing type %d!\n", eTiming);
        return;
    }

    MDrv_DAC_SetOutputTiming(enTimingType);

    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_now = enTimingType;
    MDrv_DAC_Release_Semaphore();
}

void MApi_DAC_SetYPbPrOutputTiming(E_OUTPUT_VIDEO_TIMING_TYPE eTiming)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_YPBPR_OUTPUTTIMING DACArgs;
    DACArgs.eTiming = eTiming;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_YPBPR_OUTPUTTIMING, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}


void MApi_DAC_ClkSel_U2(void* pInstance, E_OUTPUT_VIDEO_TIMING_TYPE eTiming, E_OUTPUT_BIT_TYPE ebits)
{
    EN_OUTPUT_VIDEO_TIMING_TYPE enTimingType;
    EN_OUTPUT_BIT_TYPE enBits;
    MS_BOOL bRet = FALSE;

    bRet = _MApi_DAC_VideoTiming_ReMapping(eTiming, &enTimingType);
    if(!bRet)
    {
        printf("[MAPI_DAC]Error timing type %d!\n", eTiming);
        return;
    }

    switch(ebits)
    {
        case E_COLOR_8_BIT:
            enBits = COLOR_8_BIT;
            break;
        case E_COLOR_10_BIT:
            enBits = COLOR_10_BIT;
            break;
        case E_COLOR_12_BIT:
            enBits = COLOR_12_BIT;
            break;
        default:
            printf("[MAPI_CLKSEL]Error bit type %d!\n", ebits);
            return;
    }
    MDrv_ClkSel_Set(enTimingType, enBits);
}

void MApi_DAC_ClkSel(E_OUTPUT_VIDEO_TIMING_TYPE eTiming, E_OUTPUT_BIT_TYPE ebits)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_CLKSEL DACArgs;
    DACArgs.eTiming = eTiming;
    DACArgs.ebits = ebits;

    if(UtopiaIoctl(g_pDACInst, E_DAC_CLKSEL, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;

}

void MApi_DAC_Exit_U2(void* pInstance)
{
    MDrv_DAC_Exit();
    if(UtopiaClose(g_pDACInst))
    {
        g_pDACRes = NULL;
        g_pDACInst = NULL;
    }
    else
    {
        printf("Close DAC engine fail\n");
    }
}

void MApi_DAC_Exit(void)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(g_pDACInst, E_DAC_EXIT, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

void MApi_DAC_DumpTable_U2(void* pInstance, MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));
    MDrv_DAC_DumpTable(pDACTable, u8DACtype);

    pDACResourcePrivate->stapiDAC._stDac_DacPowerState.DACTable[u8DACtype] = (MS_VIRT)pDACTable;
    pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u8DACtype[u8DACtype] = 1;

    if((u8DACtype == DAC_TABTYPE_SC1_INIT) || (u8DACtype == DAC_TABTYPE_SC1_INIT_SC))
    {
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_SC1 = pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_now;
    }
    else
    {
        pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_SC0 = pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_now;
    }

    MDrv_DAC_Release_Semaphore();
}

void MApi_DAC_DumpTable(MS_U8 *pDACTable, MS_U8 u8DACtype)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_DUMP_TABLE DACArgs;
    DACArgs.pDACTable = pDACTable;
    DACArgs.u8DACtype = u8DACtype;

    if(UtopiaIoctl(g_pDACInst, E_DAC_DUMP_TABLE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// DAC - set half output current
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetIHalfOutput_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetIHalfOutput(bEnable, bIsYPbPr);
}

void MApi_DAC_SetIHalfOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_IHALF_OUTPUT DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_IHALF_OUTPUT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// DAC - set quart output current
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetQuartOutput_U2(void* pInstance, MS_BOOL bEnable,MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetQuartOutput(bEnable, bIsYPbPr);
}

void MApi_DAC_SetQuartOutput(MS_BOOL bEnable,MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_QUART_OUTPUT DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_QUART_OUTPUT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// DAC - SetDacState
// When DAC is already enabled in the MBoot, we can use this function to
// mark the DAC init state as "enabled".
// @bInited: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: HD, FALSE: SD
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetDacState_U2(void* pInstance, MS_BOOL bEnabled, MS_BOOL bIsYPbPr)
{
    MDrv_DAC_SetDACState(bEnabled, bIsYPbPr);
}

void MApi_DAC_SetDacState(MS_BOOL bEnabled, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_DAC_STATE DACArgs;
    DACArgs.bEnabled = bEnabled;
    DACArgs.bIsYPbPr = bIsYPbPr;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_DAC_STATE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

//----------------------------------------------------------------
// MApi_DAC_HotPlugDetect - Get DAC HotPlug state
// @param: SelDAC: DAC_DETECT_HD, DAC_DETECT_SD
// @param: DetectType: DAC_DETECT_PLUGIN, DAC_DETECT_PLUGOUT
// @param: bIsSignIn: Report signal is in/out
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MApi_DAC_HotPlugDetect_U2(void* pInstance, E_DAC_DETECT SelDAC,E_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    return MDrv_DAC_HotPlugDetect((EN_DAC_DETECT) SelDAC, (EN_DAC_DETECT_TYPE)DetectType, State);
}

MS_BOOL MApi_DAC_HotPlugDetect(E_DAC_DETECT SelDAC,E_DAC_DETECT_TYPE DetectType, MS_BOOL *State)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_HOTPLUG_DETECT DACArgs;
    DACArgs.SelDAC = SelDAC;
    DACArgs.DetectType = DetectType;
    DACArgs.State = State;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_HOTPLUG_DETECT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

#ifdef CONFIG_MSTAR_CLKM
#include "drvCLKM.h"
#endif
MS_U32 Mapi_DAC_SetPowerState_U2(void* pInstance, E_DAC_POWER_MODE ePowerState)
{
    static EN_POWER_MODE prev_PowerState = E_DAC_POWER_MECHANICAL;
    MS_U32 u32Ret = FALSE;

    MDrv_DAC_Get_Semaphore(pInstance);
    DAC_RESOURCE_PRIVATE* pDACResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pDACRes,(void**)(&pDACResourcePrivate));

    if(ePowerState == E_DAC_POWER_SUSPEND)
    {
        prev_PowerState = ePowerState;
        u32Ret = TRUE; //SUSPEND_OK;
#ifdef CONFIG_MSTAR_CLKM
        MS_U32 u32DacHandle = Drv_Clkm_Get_Handle("g_clk_hdgen");
        Drv_Clkm_Clk_Gate_Disable(u32DacHandle);
        u32DacHandle = Drv_Clkm_Get_Handle("g_clk_hdgen_fir");
        Drv_Clkm_Clk_Gate_Disable(u32DacHandle);
        u32DacHandle = Drv_Clkm_Get_Handle("g_clk_hdgen_in");
        Drv_Clkm_Clk_Gate_Disable(u32DacHandle);
#endif
    }
    else if(ePowerState == E_DAC_POWER_RESUME)
    {
        if(prev_PowerState == E_POWER_SUSPEND)
        {
            MDrv_DAC_SetIOMapBase(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u32NPMBase, pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u32PMBase);
            MDrv_DAC_Init();
            MDrv_DAC_SetClkInv(TRUE, TRUE);

            // Setting HD DAC
            MDrv_DAC_Enable(TRUE, TRUE);
            MDrv_DAC_SetOutputSource(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDOutputType, TRUE);
            MDrv_DAC_SetOutputSwapSel(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDSwapSEL, TRUE);
            MDrv_DAC_SetOutputLevel(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.HDMaxOutputLevel, TRUE);

            // Setting SD DAC
            MDrv_DAC_Enable(TRUE, FALSE);
            MDrv_DAC_SetOutputSource(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDOutputType, FALSE);
            MDrv_DAC_SetOutputSwapSel(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDSwapSEL, FALSE);
            MDrv_DAC_SetOutputLevel(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.SDMaxOutputLevel, FALSE);

            int i = 0;
            MS_U8 *DacTbl = NULL;
            MS_U8 DacType = 0;
            for(i = 0; i < DAC_TABLE_TYPE_NUM; i++)
            {
                if(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.u8DACtype[i] == 1)
                {
                    if((i == DAC_TABTYPE_SC1_INIT) || (i == DAC_TABTYPE_SC1_INIT_SC))
                    {
                        MDrv_DAC_SetOutputTiming(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_SC1);
                    }
                    else
                    {
                        MDrv_DAC_SetOutputTiming(pDACResourcePrivate->stapiDAC._stDac_DacPowerState.OutputVideoTimingType_SC0);
                    }
                    DacTbl = (MS_U8*)pDACResourcePrivate->stapiDAC._stDac_DacPowerState.DACTable[i];
                    DacType = i;
                    MDrv_DAC_DumpTable(DacTbl, DacType);
                }
            }

            prev_PowerState = ePowerState;
            u32Ret = TRUE;
        }
        else
        {
            printf("[%s,%5d] It is not suspended yet. We shouldn't resume\n", __FUNCTION__, __LINE__);
            u32Ret = FALSE;
        }
    }
    else
    {
        printf("[%s,%5d] Do Nothing: %u\n", __FUNCTION__, __LINE__, ePowerState);
        u32Ret = FALSE;
    }

    MDrv_DAC_Release_Semaphore();
    return u32Ret;
}

MS_U32 Mapi_DAC_SetPowerState(E_DAC_POWER_MODE ePowerState)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_POWER_MODE DACArgs;
    DACArgs.ePowerState = ePowerState;
    DACArgs.stReturnValue = 0;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_POWER_STATE, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.stReturnValue;
}

//----------------------------------------------------------------
// MApi_DAC_SetWSSOnOFF
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MApi_DAC_SetWSSOnOff_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (!bIsYPbPr)
    {
        // CVBS VBI
        printf("CVBS VBI is not implement at here! \n");
        return FALSE;
    }
    else
    {
        MDrv_DAC_SetWSSOnOff(bEnable);
    }

    return TRUE;
}

MS_BOOL MApi_DAC_SetWSSOnOff(MS_BOOL bEnable, MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_SET_WSSONOFF DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_WSSONOFF, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

//----------------------------------------------------------------
// MApi_DAC_GetWSSStatus
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @pEnabled:       \b OUT: the pointer to WSS OnOff. TRUE: enabled, FALSE: not enabled
///@pSetWSS_Data:   \b OUT: the pointer to DAC_SETWSS_INFO structure
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MApi_DAC_GetWSSStatus_U2(void* pInstance, MS_BOOL bIsYPbPr, MS_BOOL *pEnable, DAC_SETWSS_INFO *pSetWSS_Data)
{
    if (!bIsYPbPr)
    {
        // CVBS VBI
        printf("CVBS VBI is not implement at here! \n");
        return FALSE;
    }
    else
    {
        *pEnable = MDrv_DAC_GetWSSStatus();

        //TBD, set defalut value first.
        if(pSetWSS_Data != NULL)
        {
            pSetWSS_Data->u32DAC_SetWSS_version = 0;
            pSetWSS_Data->u32DAC_SetWSS_Length = sizeof(DAC_SETWSS_INFO);
            pSetWSS_Data->eOutput_Timing = E_RES_720x480I_60Hz;
            pSetWSS_Data->eVBI_WSS_Type = E_DAC_VIDEO_VBI_WSS_START;
        }
    }

    return TRUE;
}

MS_BOOL MApi_DAC_GetWSSStatus(MS_BOOL bIsYPbPr, MS_BOOL *pEnable, DAC_SETWSS_INFO *pSetWSS_Data)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_GET_WSSSTATUS DACArgs;
    DACArgs.bIsYPbPr = bIsYPbPr;
    DACArgs.pEnable = pEnable;
    DACArgs.pSetWSS_Data = pSetWSS_Data;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_GET_WSSSTATUS, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

//----------------------------------------------------------------
// MApi_DAC_ResetWSSData
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MApi_DAC_ResetWSSData_U2(void* pInstance, MS_BOOL bIsYPbPr)
{
    if (!bIsYPbPr)
    {
        // CVBS VBI
        printf("CVBS VBI is not implement at here! \n");
        return FALSE;
    }
    else
    {
        MDrv_DAC_ResetWSSData();
    }

    return TRUE;
}

MS_BOOL MApi_DAC_ResetWSSData(MS_BOOL bIsYPbPr)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_RESET_WSSDATA DACArgs;
    DACArgs.bIsYPbPr = bIsYPbPr;
    DACArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(g_pDACInst, E_DAC_RESET_WSSDATA, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

//----------------------------------------------------------------
// MApi_DAC_SetWSSOutput
// @bEnabled: TRUE: enabled, FALSE: not enabled
// @bIsYPbPr: TRUE: component, FALSE: CVBS
// @DAC_SETWSS_INFO: video vbi data struct
// @return: TRUE is working successful
//----------------------------------------------------------------
MS_BOOL MApi_DAC_SetWSSOutput_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr, DAC_SETWSS_INFO SetWSS_Data)
{
    MS_U16 u16WSS_Type = 0x00;
    EN_OUTPUT_VIDEO_TIMING_TYPE eVideo_Timing = RES_720x480I_60Hz;
    MS_BOOL bRet = FALSE;

    if (!bIsYPbPr)
    {
        // CVBS VBI
        printf("CVBS VBI is not implement at here! \n");
        return FALSE;
    }
    else
    {
        bRet = _MApi_DAC_VideoTiming_ReMapping(SetWSS_Data.eOutput_Timing, &eVideo_Timing);
        if(!bRet)
        {
            printf("[MAPI_DAC]Error timing type %d!\n", SetWSS_Data.eOutput_Timing);
            return FALSE;
        }

        if(((SetWSS_Data.eVBI_WSS_Type >= E_DAC_VIDEO_VBI_WSS_AS_4x3_FULL) && (SetWSS_Data.eVBI_WSS_Type <= E_DAC_VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC))
             ||((SetWSS_Data.eVBI_WSS_Type >= E_DAC_VIDEO_VBI_WSS525_A_AS_4x3_NORMAL) && (SetWSS_Data.eVBI_WSS_Type <= E_DAC_VIDEO_VBI_WSS525_A_AS_RESERVED)))
        {
            switch(SetWSS_Data.eVBI_WSS_Type)
            {
                default:
                case E_DAC_VIDEO_VBI_WSS_AS_4x3_FULL:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_4x3_FULL;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_CENTER:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_CENTER;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_TOP:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_14x9_LETTERBOX_TOP;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_CENTER:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_CENTER;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_TOP:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_16x9_LETTERBOX_TOP;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_ABOVE16x9_LETTERBOX_CENTER:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_ABOVE16x9_LETTERBOX_CENTER;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_14x9_FULL_CENTER:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_14x9_FULL_CENTER;
                    break;
                case E_DAC_VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_AS_16x9_ANAMORPHIC;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_AS_4x3_NORMAL:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_AS_4x3_NORMAL;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_AS_4x3_LETTERBOX:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_AS_4x3_LETTERBOX;
                    break;
                //case E_DAC_VIDEO_VBI_WSS525_A_AS_16x9_ANAMORPHIC:
                case E_DAC_VIDEO_VBI_WSS525_A_AS_16x9_NORMAL:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_AS_16x9_NORMAL;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_AS_RESERVED:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_AS_RESERVED;
                    break;
            }

            bRet = MDrv_DAC_WSS_SetAS(bEnable, eVideo_Timing, u16WSS_Type);
        }
        else if(((SetWSS_Data.eVBI_WSS_Type >= E_DAC_VIDEO_VBI_WSS_CGMS_COPYRIGHT) && (SetWSS_Data.eVBI_WSS_Type <= E_DAC_VIDEO_VBI_WSS_CGMS_COPY_PROTECTION))
                  ||((SetWSS_Data.eVBI_WSS_Type >= E_DAC_VIDEO_VBI_WSS_CGMS_COPY_FREELY) && (SetWSS_Data.eVBI_WSS_Type <= E_DAC_VIDEO_VBI_WSS_CGMS_COPY_NEVER))
                  ||((SetWSS_Data.eVBI_WSS_Type >= E_DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY) && (SetWSS_Data.eVBI_WSS_Type <= E_DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER)))
        {
            switch(SetWSS_Data.eVBI_WSS_Type)
            {
                default:
                case E_DAC_VIDEO_VBI_WSS_CGMS_COPYRIGHT:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_CGMS_COPYRIGHT;
                    break;
                case E_DAC_VIDEO_VBI_WSS_CGMS_COPY_PROTECTION:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_CGMS_COPY_PROTECTION;
                    break;
                case E_DAC_VIDEO_VBI_WSS_CGMS_COPY_FREELY:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_CGMS_COPY_FREELY;
                    break;
                case E_DAC_VIDEO_VBI_WSS_CGMS_COPY_ONCE:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_CGMS_COPY_ONCE;
                    break;
                case E_DAC_VIDEO_VBI_WSS_CGMS_COPY_NO_MORE:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_CGMS_COPY_NO_MORE;
                    break;
                case E_DAC_VIDEO_VBI_WSS_CGMS_COPY_NEVER:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS_CGMS_COPY_NEVER;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_FREELY;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_ONCE;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NO_MORE;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_CGMS_COPY_NEVER;
                    break;
            }

            bRet = MDrv_DAC_WSS_SetCGMS(bEnable, eVideo_Timing, u16WSS_Type);
        }
        else if((SetWSS_Data.eVBI_WSS_Type >= E_DAC_VIDEO_VBI_WSS525_A_APS_NO_APS) && (SetWSS_Data.eVBI_WSS_Type <= E_DAC_VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS))
        {
            switch(SetWSS_Data.eVBI_WSS_Type)
            {
                default:
                case E_DAC_VIDEO_VBI_WSS525_A_APS_NO_APS:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_APS_NO_APS;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_APS_PSP_CS_OFF:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_APS_PSP_CS_OFF;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_APS_PSP_2_LINE_CS:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_APS_PSP_2_LINE_CS;
                    break;
                case E_DAC_VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS:
                    u16WSS_Type = DAC_VIDEO_VBI_WSS525_A_APS_PSP_4_LINE_CS;
                    break;
            }

            bRet = MDrv_DAC_WSS_SetAPS(bEnable, eVideo_Timing, u16WSS_Type);
        }

        if(!bRet)
        {
            printf("[MAPI_DAC]The DAC WSS Timing %d Set Type %d Error!\n", SetWSS_Data.eOutput_Timing, SetWSS_Data.eVBI_WSS_Type);
            return FALSE;
        }
    }

    return TRUE;
}

MS_BOOL MApi_DAC_SetWSSOutput(MS_BOOL bEnable, MS_BOOL bIsYPbPr, DAC_SETWSS_INFO SetWSS_Data)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_SET_WSSOUTPUT DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bIsYPbPr = bIsYPbPr;
    memcpy(&DACArgs.SetWSS_Data, &SetWSS_Data, sizeof(DAC_SETWSS_INFO));
    DACArgs.bReturnValue = 0;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_WSSOUTPUT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

//----------------------------------------------------------------
// MApi_DAC_EnableICT
// Enable hdgen ICT (Image Constriant Token) function
// @bEnable: TRUE: enable, FALSE: disable
// @return: TRUE is working successful
//----------------------------------------------------------------

MS_BOOL MApi_DAC_EnableICT_U2(void* pInstance, MS_BOOL bEnable)
{
    MDrv_DAC_EnableICT(bEnable);
    return TRUE;
}

MS_BOOL MApi_DAC_EnableICT(MS_BOOL bEnable)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stDAC_ENABLE_ICT DACArgs;
    DACArgs.bEnable = bEnable;
    DACArgs.bReturnValue = 0;

    if(UtopiaIoctl(g_pDACInst, E_DAC_ENABLE_ICT, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return DACArgs.bReturnValue;
}

//----------------------------------------------------------------
// DAC - set VGA Hsync Vsync
// @return none
//----------------------------------------------------------------
void MApi_DAC_SetVGAHsyncVsync_U2(void* pInstance, MS_BOOL bEnable)
{
    MDrv_DAC_SetVGAHsyncVsync(bEnable);
}

void MApi_DAC_SetVGAHsyncVsync(MS_BOOL bEnable)
{
    if (g_pDACInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_DAC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDAC_SET_VGA_HsyncVsync DACArgs;
    DACArgs.bEnable = bEnable;

    if(UtopiaIoctl(g_pDACInst, E_DAC_SET_VGA_HSYNCVSYNC, (void*)&DACArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain DAC engine fail\n");
    }
    return;
}

EXPORT_SYMBOL(MApi_DAC_DumpTable);
EXPORT_SYMBOL(MApi_DAC_SetYPbPrOutputTiming);
