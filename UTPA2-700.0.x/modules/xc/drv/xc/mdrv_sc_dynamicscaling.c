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
//==============================================================================
// [mdrv_sc_dynamicscaling.c]
// Date: 2012/10/29
// Descriptions: dynamic scaling related functions
//==============================================================================
#ifndef  _MDRV_SC_DYNAMICSCALING_C_
#define  _MDRV_SC_DYNAMICSCALING_C_

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#if (SUPPORT_KERNEL_DS == 1)
#include "mdrv_xc_st.h"
#include "mdrv_xc_io.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#endif
#endif

//#include "Debug.h"
#include "drvXC_IOPort.h"
#include "MsTypes.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_dynamicscaling.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "xc_hwreg_utility2.h"
#include "mdrv_sc_dynamicscaling.h"

#if (SUPPORT_KERNEL_DS == 1)
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
static MS_S32   _s32FdScaler = -1;
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
extern E_K_APIXC_ReturnValue KApi_XC_Get_DSForceIndexSupported(EN_KDRV_SC_DEVICE u32DeviceID, EN_KDRV_WINDOW  eWindow);
extern E_K_APIXC_ReturnValue KApi_XC_Set_DSIndexSourceSelect(EN_KDRV_SC_DEVICE u32DeviceID, E_K_XC_DS_INDEX_SOURCE eDSIdxSrc, EN_KDRV_WINDOW  eWindow);
extern MS_BOOL KApi_XC_SetDynamicScaling(EN_KDRV_SC_DEVICE u32DeviceID, K_XC_DynamicScaling_Info *pstDSInfo,MS_U32 u32DSInfoLen, EN_KDRV_WINDOW  eWindow);
extern void KApi_XC_Set_DynamicScalingFlag(EN_KDRV_SC_DEVICE u32DeviceID,MS_BOOL bEnable);
extern MS_BOOL KDrv_XC_GetDynamicScalingStatus(EN_KDRV_SC_DEVICE u32DeviceID);
extern MS_BOOL KDrv_XC_EnableIPMTuneAfterDS(EN_KDRV_SC_DEVICE u32DeviceID, MS_BOOL bEnable);
extern MS_BOOL KApi_XC_GetDynamicScalingIndex(EN_KDRV_SC_DEVICE u32DeviceID, EN_KDRV_WINDOW eWindow, MS_U8* pu8DSIndex);
extern MS_BOOL KApi_XC_FireDynamicScalingIndex(EN_KDRV_SC_DEVICE u32DeviceID, EN_KDRV_WINDOW eWindow);
extern MS_BOOL MDrv_XC_SetDSHDRInfo(ST_KDRV_XC_DS_HDRInfo *pstDSHDRInfo);
#endif
#endif
E_APIXC_ReturnValue MDrv_XC_Get_DSForceIndexSupported(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
	bReturn = MHal_XC_Get_DSForceIndexSupported(pInstance, eWindow);
    return bReturn?E_APIXC_RET_OK:E_APIXC_RET_FAIL;
}

E_APIXC_ReturnValue MDrv_XC_Set_DSIndexSourceSelect(void *pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
	MHal_XC_Set_DSIndexSourceSelect(pInstance, eDSIdxSrc, eWindow);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MDrv_XC_Set_DSForceIndex(void *pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    MHal_XC_Set_DSForceIndex(pInstance, bEnable, u8Index, eWindow);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[eWindow] = bEnable;

    return E_APIXC_RET_OK;
}

MS_BOOL MDrv_XC_Is_DSForceIndexEnabled(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Dynamicscaling._bDSForceIndexEnable[eWindow];
}

E_APIXC_ReturnValue MApi_XC_Get_DSForceIndexSupported_U2(void* pInstance, SCALER_WIN eWindow)
{
#if (SUPPORT_KERNEL_DS == 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }
    return KApi_XC_Get_DSForceIndexSupported(u32DeviceID, eWindow);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);
    }

    if (0 > _s32FdScaler)
    {
        printf("\nUnable to open /dev/scaler\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        ST_KDRV_DS_GET_DSForceIndexSupported k_ds_get_ForceIndexSupported;
        if(psXCInstPri->u32DeviceID == 0)
            k_ds_get_ForceIndexSupported.u32DeviceID = E_KDRV_XC_SC0;
        else
            k_ds_get_ForceIndexSupported.u32DeviceID = E_KDRV_XC_SC1;
        k_ds_get_ForceIndexSupported.eWindow = eWindow;
        k_ds_get_ForceIndexSupported.eReturn = E_APIXC_RET_FAIL;
        if (-1 == ioctl(_s32FdScaler, MDRV_XC_IOC_DS_GET_DSFORCE_INDEX_SUPPORT,&k_ds_get_ForceIndexSupported))
        {
            perror("ioctl");
            return E_APIXC_RET_FAIL;
        }

        return k_ds_get_ForceIndexSupported.eReturn;
    }
#else
    return E_APIXC_RET_FAIL;
#endif
#endif
#else
    return MDrv_XC_Get_DSForceIndexSupported(pInstance, eWindow);
#endif
}

E_APIXC_ReturnValue MApi_XC_Get_DSForceIndexSupported(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_DS_FORCE_INDEX_SUPPORTED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DS_FORCE_INDEX_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_DSIndexSourceSelect_U2(void* pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
#if (SUPPORT_KERNEL_DS == 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }
    return KApi_XC_Set_DSIndexSourceSelect(u32DeviceID,eDSIdxSrc,eWindow);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);
    }

    if (0 > _s32FdScaler)
    {
        printf("\nUnable to open /dev/scaler\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        ST_KDRV_DS_SET_DSIndexSourceSelect k_ds_set_IndexSourceSelect;
        if(psXCInstPri->u32DeviceID == 0)
            k_ds_set_IndexSourceSelect.u32DeviceID = E_KDRV_XC_SC0;
        else
            k_ds_set_IndexSourceSelect.u32DeviceID = E_KDRV_XC_SC1;
        k_ds_set_IndexSourceSelect.eDSIdxSrc = eDSIdxSrc;
        k_ds_set_IndexSourceSelect.eWindow = eWindow;
        k_ds_set_IndexSourceSelect.eReturn = E_APIXC_RET_FAIL;
        if (-1 == ioctl(_s32FdScaler, MDRV_XC_IOC_DS_SET_DSINDEX_SOURCE_SELECT,&k_ds_set_IndexSourceSelect))
        {
            perror("ioctl");
            return E_APIXC_RET_FAIL;
        }

        return k_ds_set_IndexSourceSelect.eReturn;
    }
#else
    return E_APIXC_RET_FAIL;
#endif
#endif
#else
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    _XC_ENTRY(pInstance);
    eReturn = MDrv_XC_Set_DSIndexSourceSelect(pInstance, eDSIdxSrc, eWindow);
    _XC_RETURN(pInstance);
    return eReturn;
#endif
}

E_APIXC_ReturnValue MApi_XC_Set_DSIndexSourceSelect(E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_INDEX_SOURCE_SELECT XCArgs;
    XCArgs.eDSIdxSrc = eDSIdxSrc;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DS_INDEX_SOURCE_SELECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_Set_DSForceIndex_U2(void* pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    eReturn = MDrv_XC_Set_DSForceIndex(pInstance, bEnable, u8Index, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_Set_DSForceIndex(MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_FORCE_INDEX XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Index = u8Index;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DS_FORCE_INDEX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get Dynamic Scaling Status
/// @return @ref MS_BOOL TRUE: dynamic scaling is enabled; FALSE: dynamic scaling is not enabled.
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetDynamicScalingStatus_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = (MDrv_XC_Is_DSForceIndexEnabled(pInstance,MAIN_WINDOW)| MDrv_XC_GetDynamicScalingStatus(pInstance));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_GetDynamicScalingStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_DS_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DS_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_GetDynamicScalingStatus(void *pInstance)
{
#if (SUPPORT_KERNEL_DS == 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }
    return KDrv_XC_GetDynamicScalingStatus(u32DeviceID);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);
    }

    if (0 > _s32FdScaler)
    {
        printf("\nUnable to open /dev/scaler\n");
        return FALSE;
    }
    else
    {
        ST_KDRV_DS_GetDynamicScalingStatus k_ds_get_DynamicScalingStatus;
        if(psXCInstPri->u32DeviceID == 0)
            k_ds_get_DynamicScalingStatus.u32DeviceID = E_KDRV_XC_SC0;
        else
            k_ds_get_DynamicScalingStatus.u32DeviceID = E_KDRV_XC_SC1;
        k_ds_get_DynamicScalingStatus.bReturn = FALSE;
        if (-1 == ioctl(_s32FdScaler, MDRV_XC_IOC_DS_GET_DS_STATUS,&k_ds_get_DynamicScalingStatus))
        {
            perror("ioctl");
            return FALSE;
        }

        return k_ds_get_DynamicScalingStatus.bReturn;
    }
#else
    return FALSE;
#endif
#endif
#else
    return MHAL_SC_Get_DynamicScaling_Status(pInstance);
#endif
}

MS_BOOL MDrv_XC_EnableIPMTuneAfterDS(void *pInstance, MS_BOOL bEnable)
{
#if (SUPPORT_KERNEL_DS == 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }
    return KDrv_XC_EnableIPMTuneAfterDS(u32DeviceID,bEnable);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);
    }

    if (0 > _s32FdScaler)
    {
        printf("\nUnable to open /dev/scaler\n");
        return FALSE;
    }
    else
    {
        ST_KDRV_DS_EnableIPMTuneAfterDS k_ds_EnableIPMTuneAfterDS;
        if(psXCInstPri->u32DeviceID == 0)
            k_ds_EnableIPMTuneAfterDS.u32DeviceID = E_KDRV_XC_SC0;
        else
            k_ds_EnableIPMTuneAfterDS.u32DeviceID = E_KDRV_XC_SC1;
        k_ds_EnableIPMTuneAfterDS.bEnable = bEnable;
        k_ds_EnableIPMTuneAfterDS.bReturn = FALSE;
        if (-1 == ioctl(_s32FdScaler, MDRV_XC_IOC_DS_ENABLE_IPM_TUNE_AFTER_DS,&k_ds_EnableIPMTuneAfterDS))
        {
            perror("ioctl");
            return FALSE;
        }

        return k_ds_EnableIPMTuneAfterDS.bReturn;
    }
#else
    return FALSE;
#endif
#endif
#else
    return MHAL_SC_Enable_IPMTuneAfterDS(pInstance, bEnable);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Set Dynamic Scaling
/// @param  pstDSInfo              \b IN: the information of Dynamic Scaling
/// @param  u32DSInforLen          \b IN: the length of the pstDSInfo
/// @param  eWindow                \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetDynamicScaling_U2(void* pInstance, XC_DynamicScaling_Info *pstDSInfo,MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
{
#if (SUPPORT_KERNEL_DS == 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    K_XC_DynamicScaling_Info stKDrvDsInfo;
    memset(&stKDrvDsInfo, 0, sizeof(K_XC_DynamicScaling_Info));

    stKDrvDsInfo.u64DS_Info_BaseAddr = pstDSInfo->u32DS_Info_BaseAddr;
    stKDrvDsInfo.u8MIU_Select = pstDSInfo->u8MIU_Select;
    stKDrvDsInfo.u8DS_Index_Depth = pstDSInfo->u8DS_Index_Depth;
    stKDrvDsInfo.bOP_DS_On = pstDSInfo->bOP_DS_On;
    stKDrvDsInfo.bIPS_DS_On = pstDSInfo->bIPS_DS_On;
    stKDrvDsInfo.bIPM_DS_On = pstDSInfo->bIPM_DS_On;

#ifdef MSOS_TYPE_LINUX_KERNEL
    //**************update enable in shm*********************//
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16OnOff = 0;
    u16OnOff = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L) & 0x1000;

    u16OnOff |= ((pstDSInfo->bOP_DS_On << 9) | (pstDSInfo->bIPS_DS_On << 10) | (pstDSInfo->bIPM_DS_On << 11));
    if(u16OnOff & 0x0E00)
        pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = TRUE;
    else
        pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = FALSE;

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    //********************************************************
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
        u32DeviceID = E_KDRV_XC_SC0;
    else
        u32DeviceID = E_KDRV_XC_SC1;
    return KApi_XC_SetDynamicScaling(u32DeviceID, (K_XC_DynamicScaling_Info*)&stKDrvDsInfo,u32DSInfoLen,eWindow);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);
    }

    if (0 > _s32FdScaler)
    {
        printf("\nUnable to open /dev/scaler\n");
        return FALSE;
    }
    else
    {
        ST_KDRV_DS_SetDynamicScaling k_ds_set_DynamicScaling;
        if(psXCInstPri->u32DeviceID == 0)
            k_ds_set_DynamicScaling.u32DeviceID = E_KDRV_XC_SC0;
        else
            k_ds_set_DynamicScaling.u32DeviceID = E_KDRV_XC_SC1;
        k_ds_set_DynamicScaling.pstDSInfo = (K_XC_DynamicScaling_Info*)&stKDrvDsInfo;
        k_ds_set_DynamicScaling.u32DSInfoLen = u32DSInfoLen;
        k_ds_set_DynamicScaling.eWindow = eWindow;
        k_ds_set_DynamicScaling.bReturn = FALSE;

        if (-1 == ioctl(_s32FdScaler, MDRV_XC_IOC_DS_SET_DYNAMICSCALING, &k_ds_set_DynamicScaling))
        {
            perror("ioctl");
            return FALSE;
        }

        //**************update enable in shm*********************//
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        _XC_ENTRY(pInstance);

        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        MS_U16 u16OnOff = 0;
        u16OnOff = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L) & 0x1000;

        u16OnOff |= ((pstDSInfo->bOP_DS_On << 9) | (pstDSInfo->bIPS_DS_On << 10) | (pstDSInfo->bIPM_DS_On << 11));
        if(u16OnOff & 0x0E00)
            pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = TRUE;
        else
            pXCResourcePrivate->sthal_SC.bDynamicScalingEnable = FALSE;

        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        //********************************************************
        return k_ds_set_DynamicScaling.bReturn;
    }
#else
    return FALSE;
#endif
#endif
#else
    MS_BOOL bStatus;
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Entry\n");
    if(u32DSInfoLen != sizeof(XC_DynamicScaling_Info))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"It's dynamic scaling case(%u,%u,%u)\n",
            pstDSInfo->bOP_DS_On,pstDSInfo->bIPS_DS_On,pstDSInfo->bIPM_DS_On);

#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d]SLZ[%s] use DS memory base addr = %lx, MIU_sel =%x, Depth=%u\n",
            __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", pstDSInfo->u32DS_Info_BaseAddr, pstDSInfo->u8MIU_Select, pstDSInfo->u8DS_Index_Depth);
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SEAMLESSZAP, "[%s][%06d]SLZ[%s] use DS memory base addr = %tx, MIU_sel =%x, Depth=%u\n",
            __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", (ptrdiff_t)pstDSInfo->u32DS_Info_BaseAddr, pstDSInfo->u8MIU_Select, pstDSInfo->u8DS_Index_Depth);
#endif

    bStatus = MDrv_XC_Set_DynamicScaling(
        pInstance,
        pstDSInfo->u32DS_Info_BaseAddr,
        pstDSInfo->u8MIU_Select,
        pstDSInfo->u8DS_Index_Depth,
        pstDSInfo->bOP_DS_On,
        pstDSInfo->bIPS_DS_On,
        pstDSInfo->bIPM_DS_On,
        eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
    return bStatus;
#endif
}

MS_BOOL MApi_XC_SetDynamicScaling(XC_DynamicScaling_Info *pstDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_DYNAMIC_SCALING XCArgs;
    XCArgs.pstDSInfo = pstDSInfo;
    XCArgs.u32DSInfoLen = u32DSInfoLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DYNAMIC_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_Set_DynamicScaling(void *pInstance,MS_PHY u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On,SCALER_WIN eWindow)
{
    return MHAL_SC_Set_DynamicScaling(pInstance,u32MemBaseAddr, u8MIU_Select,u8IdxDepth, bOP_On, bIPS_On, bIPM_On,eWindow);
}

void MApi_XC_Set_DynamicScalingFlag_U2(void *pInstance,MS_BOOL bEnable)
{
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MHAL_SC_Set_DynamicScalingFlag(pInstance, bEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#if (SUPPORT_KERNEL_DS == 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }
    KApi_XC_Set_DynamicScalingFlag(u32DeviceID,bEnable);
    return;
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);
    }

    if (0 > _s32FdScaler)
    {
        printf("\nUnable to open /dev/scaler\n");
        return ;
    }
    else
    {
        ST_KDRV_DS_SET_DynamicScalingFlag k_ds_set_DynamicScalingFlag;
        if(psXCInstPri->u32DeviceID == 0)
            k_ds_set_DynamicScalingFlag.u32DeviceID = E_KDRV_XC_SC0;
        else
            k_ds_set_DynamicScalingFlag.u32DeviceID = E_KDRV_XC_SC1;
        k_ds_set_DynamicScalingFlag.bEnable = bEnable;
        if (-1 == ioctl(_s32FdScaler, MDRV_XC_IOC_DS_SET_DYNAMICSCALING_FLAG,&k_ds_set_DynamicScalingFlag))
        {
            perror("ioctl");
            return ;
        }

        return ;
    }
#endif
#endif
#endif
}

void MApi_XC_Set_DynamicScalingFlag(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }
    stXC_SET_DYNAMIC_SCALING_FLAG XCArgs;
    XCArgs.bEnable = bEnable;
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DYNAMIC_SCALING_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

MS_BOOL MDrv_XC_Is_SupportSWDS(void *pInstance)
{
#if ((HW_DESIGN_4K2K_VER == 4)||(HW_DESIGN_4K2K_VER == 6))
#if 0
    #if (defined (ANDROID))
        return TRUE;
    #else
        return FALSE;
    #endif
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // SC0 main would enable DS, may call by AN and SN
    // SC1 always call by SN
    if((psXCInstPri->u32DeviceID==0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IS_ANDROID))
        return TRUE;
    else
        return FALSE;
#endif

#elif (HW_DESIGN_4K2K_VER == 7)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //only consider sc0 case
    if((psXCInstPri->u32DeviceID==0))
    {
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IS_ANDROID))
        {
            if (pXCResourcePrivate->sthal_SC.bDynamicScalingEnable == TRUE)
            {
                //AN call this functoin, force return TRUE
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
#ifdef UFO_XC_SET_DSINFO_V0
            //If UFO SET_DSINFO_V0, support MCU DS, return by bUpdate_DS_CMD
            //(bUpdate_DS_CMD means write DS command, do not sw_db)
            return gSrcInfo[MAIN_WINDOW].Status2.bUpdate_DS_CMD;
#else
            //If no support MCU DS, force return FALSE (raw case)
            return FALSE;
#endif
        }
    }
    else
    {
        return FALSE;
    }

#else
    return FALSE;
#endif
}

#if (SUPPORT_KERNEL_DS == 1)
MS_U8 MDrv_XC_GetSWDSIndex(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }

    MS_U8 u8DSIndex = 0;

    KApi_XC_GetDynamicScalingIndex(u32DeviceID, eWindow, &u8DSIndex);

    return u8DSIndex;
#else
    ST_KDRV_DS_GET_INDEX_INFO stIndexInfo;
    memset(&stIndexInfo, 0, sizeof(ST_KDRV_DS_GET_INDEX_INFO));
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return 0;
        }
    }

    if (eWindow == MAIN_WINDOW)
        stIndexInfo.eWindow = E_KDRV_MAIN_WINDOW;
    else if (eWindow == SUB_WINDOW)
        stIndexInfo.eWindow = E_KDRV_SUB_WINDOW;
    else
        stIndexInfo.eWindow = E_KDRV_MAX_WINDOW;

    if (psXCInstPri->u32DeviceID == 0)
        stIndexInfo.u32DeviceID = E_KDRV_XC_SC0;
    else
        stIndexInfo.u32DeviceID = E_KDRV_XC_SC1;

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_DS_GET_INDEX, &stIndexInfo))
    {
        printf("MDRV_XC_IOC_DS_GET_INDEX fail!!!!\n");
        return 0;
    }
#endif

    return stIndexInfo.u8DSIndex;
#endif
}

MS_BOOL MDrv_XC_FireSWDSIndex(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#ifdef MSOS_TYPE_LINUX_KERNEL
    EN_KDRV_SC_DEVICE u32DeviceID;
    if(psXCInstPri->u32DeviceID == 0)
    {
        u32DeviceID = E_KDRV_XC_SC0;
    }
    else
    {
        u32DeviceID = E_KDRV_XC_SC1;
    }

    return KApi_XC_FireDynamicScalingIndex(u32DeviceID, eWindow);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_WRONLY | O_NDELAY);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return FALSE;
        }
    }

    ST_KDRV_DS_FIRE_INDEX_INFO stFireIndexInfo;
    memset(&stFireIndexInfo, 0, sizeof(ST_KDRV_DS_FIRE_INDEX_INFO));
    if (eWindow == MAIN_WINDOW)
        stFireIndexInfo.eWindow = E_KDRV_MAIN_WINDOW;
    else if (eWindow == SUB_WINDOW)
        stFireIndexInfo.eWindow = E_KDRV_SUB_WINDOW;
    else
        stFireIndexInfo.eWindow = E_KDRV_MAX_WINDOW;

    if (psXCInstPri->u32DeviceID == 0)
        stFireIndexInfo.u32DeviceID = E_KDRV_XC_SC0;
    else
        stFireIndexInfo.u32DeviceID = E_KDRV_XC_SC1;

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_DS_FIRE_INDEX, &stFireIndexInfo))
    {
        printf("MDRV_XC_IOC_DS_FIRE_INDEX fail!!!!\n");
        return FALSE;
    }
#endif
    return TRUE;
#endif
}
#endif

MS_BOOL MApi_XC_Is_SupportSWDS_U2(void *pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
#if (HW_DESIGN_4K2K_VER == 7)
    bReturn = TRUE;
#else
    bReturn = MDrv_XC_Is_SupportSWDS(pInstance);
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MApi_XC_Is_SupportSWDS(void)
#else
 __attribute__((weak)) MS_BOOL MApi_XC_Is_SupportSWDS(void)
#endif
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }
#if (defined (ANDROID))
    static XC_INITMISC tmp_Init_Misc;
    stXC_GET_MISC_STATUS XCArgs1;
    XCArgs1.pXC_Init_Misc = &tmp_Init_Misc;
    XCArgs1.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MISC_STATUS, (void*)&XCArgs1) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }

    if(!(tmp_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IS_ANDROID))
    {
        tmp_Init_Misc.u32MISC_A |= E_XC_INIT_MISC_A_IS_ANDROID;

        stXC_INIT_MISC XCArgs2;
        XCArgs2.pXC_Init_Misc = &tmp_Init_Misc;
        XCArgs2.u32InitMiscDataLen = sizeof(XC_INITMISC);
        XCArgs2.eReturnValue = E_APIXC_RET_FAIL;

        if(UtopiaIoctl(pu32XCInst, E_XC_CMD_INIT_MISC, (void*)&XCArgs2) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Obtain XC engine fail\n");
            return E_APIXC_RET_FAIL;
        }
    }
#else
    static XC_INITMISC tmp_Init_Misc;
    stXC_GET_MISC_STATUS XCArgs1;
    XCArgs1.pXC_Init_Misc = &tmp_Init_Misc;
    XCArgs1.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_MISC_STATUS, (void*)&XCArgs1) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }

    if(tmp_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IS_ANDROID)
    {
        tmp_Init_Misc.u32MISC_A |= E_XC_INIT_MISC_A_IS_ANDROID;

        stXC_INIT_MISC XCArgs2;
        XCArgs2.pXC_Init_Misc = &tmp_Init_Misc;
        XCArgs2.u32InitMiscDataLen = sizeof(XC_INITMISC);
        XCArgs2.eReturnValue = E_APIXC_RET_FAIL;

        if(UtopiaIoctl(pu32XCInst, E_XC_CMD_INIT_MISC, (void*)&XCArgs2) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Obtain XC engine fail\n");
            return E_APIXC_RET_FAIL;
        }
    }
#endif
    stXC_GET_Is_SupportSWDS XCArgs;
    XCArgs.bEnable = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_IS_SupportSWDS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)  //E_XC_CMD_GET_SWDSIndex
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.bEnable;
    }

}

#ifdef UFO_XC_SET_DSINFO_V0
void MApi_XC_SetDSInfo_U2(void *pInstance,XC_DS_INFO *pstSetDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
{
    XC_DS_INFO DrvDSInfo;
    MS_U16 u16CopiedLength = sizeof(XC_DS_INFO);
    if((NULL == pstSetDSInfo) || (pstSetDSInfo->u16ApiDSInfo_Length == 0))
    {
        //We cosider compatible operation form version2 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_SetDSInfo: Null paramter or Wrong u16ApiDSInfo_Length!!\n");

        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");
        return ;
    }
    // the version control is coming in with version 1
    if(pstSetDSInfo->u32ApiDSInfo_Version < 1)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_SetDSInfo: please check your u32ApiDSInfo_Version, it should not set to 0!!\n");
        XC_LOG_TRACE(XC_DBGLEVEL_FUNCTION_TRACE, "Exit\n");

        return ;
    }

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //new AP + old lib, only the length corresponding to old lib has meaning.
    if(pstSetDSInfo->u32ApiDSInfo_Version > API_DS_INFO_VERSION)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_SetDSInfo: this old version XC lib has only length:%lu driver status!!\n",
                     sizeof(XC_DS_INFO))
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_SetDSInfo: this old version XC lib has only length:%u driver status!!\n",
                     sizeof(XC_DS_INFO))
#endif
        //we still copy the min size of both structure, but AP should take care of it.
        u16CopiedLength = sizeof(XC_DS_INFO);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if((pstSetDSInfo->u32ApiDSInfo_Version < API_DS_INFO_VERSION) || (pstSetDSInfo->u16ApiDSInfo_Length < sizeof(XC_DS_INFO)))
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_GetStatusEx: new version XC lib shouldn't access to the space which doesn't exist in old structure!!\n")
        u16CopiedLength = pstSetDSInfo->u16ApiDSInfo_Length;
    }

    pstSetDSInfo->u32ApiDSInfo_Version = API_DS_INFO_VERSION; //Return the actual version of the returned value for APP to use
    pstSetDSInfo->u16ApiDSInfo_Length = u16CopiedLength; //Return the actual length of the returned value for APP to know whick info is valid

    memcpy(&DrvDSInfo, pstSetDSInfo, u16CopiedLength);

    gSrcInfo[eWindow].Status2.u32MFCodecInfo = DrvDSInfo.u32MFCodecInfo;
    memcpy(&gSrcInfo[eWindow].Status2.stHDRInfo, &(DrvDSInfo.stHDRInfo), sizeof(XC_DS_HDRInfo));
    gSrcInfo[eWindow].Status2.bUpdate_DS_CMD = DrvDSInfo.bUpdate_DS_CMD[eWindow];
    gSrcInfo[eWindow].Status2.bEnableDNR = DrvDSInfo.bEnableDNR[eWindow];
    pXCResourcePrivate->sthal_SC.u32DynamicScalingBufSize = DrvDSInfo.u32DSBufferSize;

#if (SUPPORT_KERNEL_DS == 1)
#if (defined UFO_XC_HDR) && (UFO_XC_HDR_VERSION == 2)
#if (defined MSOS_TYPE_LINUX_KERNEL) || (defined ANDROID) || (HW_DESIGN_4K2K_VER == 7)
    if (pstSetDSInfo->u32ApiDSInfo_Version >= 2)
    {
        ST_KDRV_XC_DS_HDRInfo stKDrvDSHDRInfo;
        memset(&stKDrvDSHDRInfo, 0, sizeof(ST_KDRV_XC_DS_HDRInfo));
        stKDrvDSHDRInfo.u8CurrentIndex = DrvDSInfo.stHDRInfo.u8CurrentIndex;
        stKDrvDSHDRInfo.phyRegAddr = DrvDSInfo.stHDRInfo.phyRegAddr;
        stKDrvDSHDRInfo.u32RegSize = DrvDSInfo.stHDRInfo.u32RegSize;
        stKDrvDSHDRInfo.phyLutAddr = DrvDSInfo.stHDRInfo.phyLutAddr;
        stKDrvDSHDRInfo.u32LutSize = DrvDSInfo.stHDRInfo.u32LutSize;
        stKDrvDSHDRInfo.bDMEnable = DrvDSInfo.stHDRInfo.bDMEnable;
        stKDrvDSHDRInfo.bCompEnable = DrvDSInfo.stHDRInfo.bCompEnable;

#ifdef MSOS_TYPE_LINUX_KERNEL
        MDrv_XC_SetDSHDRInfo(&stKDrvDSHDRInfo);
#else
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
            }
        }

        if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_DSHDRINFO, &stKDrvDSHDRInfo))
        {
            printf("MDRV_XC_IOC_SET_DSHDRINFO fail!!!!\n");
        }
#endif
#endif
    }
#endif
#endif
#endif

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

#ifdef MSOS_TYPE_LINUX_KERNEL
void MApi_XC_SetDSInfo(XC_DS_INFO *pstSetDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
#else
__attribute__((weak)) void MApi_XC_SetDSInfo(XC_DS_INFO *pstSetDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow)
#endif
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stXC_SET_DSINFO XCArgs;
    XCArgs.pstSetDSInfo = pstSetDSInfo;
    XCArgs.u32DSInfoLen = u32DSInfoLen;
    XCArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DSINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}
#endif
#undef _MDRV_SC_DYNAMICSCALING_C_
#endif //_MDRV_SC_DYNAMICSCALING_C_
