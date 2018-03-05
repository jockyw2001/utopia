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
/// file  apiXC_Adc.c
/// brief  Scaler API layer Interface
/// author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_DWIN_C_
#define _API_XC_DWIN_C_
#endif
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsOS.h"
#ifdef UTOPIA_V2
#include "utopia.h"
#include "utopia_driver_id.h"
#include "apiXC_DWIN_v2.h"
#endif
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
#include "mvideo_context.h"
#include "drv_sc_DIP_scaling.h"
#include "mhal_dip.h"
#ifdef GOPD_DIP
#include "apiGOP.h"
#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif
#endif

// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL)
#ifndef DIP_UTOPIA2K
#define DIP_UTOPIA2K
#endif
#endif

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif
//#define TRACE_ADC_INPUT(x) //x
#define msg_video(x) //x

#define DIPDRIVER_BASE 0
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef UTOPIA_V2
void* pInstantDip[MAX_DIP_WINDOW] = {NULL};
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//MS_XC_MEM_FMT s_DIPeMemFmt[MAX_WINDOW] = {E_MS_XC_MEM_FMT_AUTO, E_MS_XC_MEM_FMT_AUTO};
#ifdef GOPD_DIP
GOP_DwinProperty GopD_property;
EN_XC_DIP_DATA_FMT DataFmt = DIP_DATA_FMT_YUV422;
extern SCALER_DIP_WIN Gopd_dip;
static InterruptCb pGopd_CB;
#ifdef MSOS_TYPE_LINUX
#define FRAME_DELAY 15
static MS_BOOL bDeAttached = TRUE;
static pthread_t id;
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

/////////////////////////////////////////
// Configuration
/////////////////////////////////////////
#if 0
// Path
typedef struct s_Data_Path
{

    //////////////////////////////
    // Path Type
    //////////////////////////////
    E_PATH_TYPE e_PathType;

    //////////////////////////////
    // Path Status
    //////////////////////////////

    /*
        Make the signal go through the path. If this flag is not enabled,
        synchronous_event_handler and source_monitor are useless.
     */
    #define DATA_PATH_ENABLE BIT(0)

    /*
        Used to identify a path is created or not. ( 1: path created )
     */
    #define DATA_PATH_USING BIT(1)

    /*
        Used to identify the priodic handler enabled or disabled
     */
    #define ENABLE_PERIODIC_HANDLER BIT(2)

    MS_U8 u8PathStatus;

    //////////////////////////////
    // A path has a format like Src -> Dest
    //////////////////////////////
    INPUT_SOURCE_TYPE_t src;
    E_DEST_TYPE dest;

    //////////////////////////////
    // thread for source.
    //////////////////////////////
    void (*path_thread)(INPUT_SOURCE_TYPE_t src, MS_BOOL bRealTimeMonitorOnly);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed automatically when synchronous event triggered.
    ///////////////////////////////////////
    void (* synchronous_event_handler )(INPUT_SOURCE_TYPE_t src, void* para);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed automatically when destination on/off event triggered.
    ///////////////////////////////////////
    void (* dest_on_off_event_handler )(INPUT_SOURCE_TYPE_t src, void* para);

    ///////////////////////////////////////
    // Customize function.
    // This will be executed periodically after enabled destinatio period handler
    ///////////////////////////////////////
    void (* dest_periodic_handler )(INPUT_SOURCE_TYPE_t src, MS_BOOL bRealTimeMonitorOnly);
}MUX_DATA_PATH;
#endif

#if 0
// Path Collection
typedef struct s_Mux_InputSource_Attr
{
    // The number of pathes which is used right now.
    MS_U8  u8PathCount;

    void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count );
    // Pathes.
    // Path array in MAPP_MUX is constructed [Sync path, Sync path, ..., Async path, Async path]
    MUX_DATA_PATH Mux_DataPaths[MAX_DATA_PATH_SUPPORTED];

    // indicate how many times the src created
    MS_U16 u16SrcCreateTime[INPUT_SOURCE_NUM];
}InputSource_Mux_Attr;

#endif
E_APIXC_ReturnValue MApi_XC_DIP_QueryResource(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            XC_ERR("Open DIP fail\n");
            utopia_free(pResource);
            return E_APIXC_RET_FAIL;
        }
    }
    //Query Resource
    pResource->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_QUERY_RESOURCE,(void* )pResource) == UTOPIA_STATUS_NO_RESOURCE)
    {
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else

#endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

E_APIXC_ReturnValue MApi_XC_DIP_GetResource(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            XC_ERR("Open DIP fail\n");
            utopia_free(pResource);
            return E_APIXC_RET_FAIL;
        }
    }
    //Get Resource
    pResource->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_GET_RESOURCE,(void*)pResource) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_GET_RESOURCE fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else

#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MApi_XC_DIP_ReleaseResource(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_RESOURCE* pResource = NULL;
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            XC_ERR("Open DIP fail\n");
            utopia_free(pResource);
            return E_APIXC_RET_FAIL;
        }
    }
    //Free Resource
    pResource->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_RELEASE_RESOURCE,(void*)pResource) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_GET_RESOURCE fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else

#endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

E_APIXC_ReturnValue MApi_XC_DIP_CMDQ_SetAction(EN_XC_DIP_CMDQ_ACTION eAction, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_CMDQ_SET_ACTION* pResource = NULL;
    pResource = (DIP_CMDQ_SET_ACTION*)utopia_malloc(sizeof(DIP_CMDQ_SET_ACTION));
    if (pResource == NULL)
    {
        printf("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }

    if(pInstantDip[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open DIP fail\n");
            utopia_free(pResource);
            return E_APIXC_RET_FAIL;
        }
    }

    pResource->eWindow = eWindow;
    pResource->eAction = eAction;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_CMDQ_SETACTION,(void*)pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Ioctl MDrv_CMD_DIP_GET_RESOURCE fail\n");
        utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pResource);
#else
    MDrv_XC_DIP_CMDQ_SetAction(NULL, eAction, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
E_APIXC_ReturnValue MApi_XC_DIP_CMDQ_SetOperations(cmd_mload_utopia_interface *pOps, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
        DIP_CMDQ_SET_OPERATIONS* pResource = NULL;
        pResource = (DIP_CMDQ_SET_OPERATIONS*)utopia_malloc(sizeof(DIP_CMDQ_SET_OPERATIONS));
        if (pResource == NULL)
        {
            printf("%s alloc fail\n",__FUNCTION__);
            return E_APIXC_RET_FAIL;
        }

        if(pInstantDip[eWindow] == NULL)
        {
            if(UtopiaOpen(MODULE_DIP,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Open DIP fail\n");
                utopia_free(pResource);
                return E_APIXC_RET_FAIL;
            }
        }

        pResource->pOps = pOps;
        pResource->bEnable = bEnable;
        pResource->eWindow = eWindow;
        if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_CMDQ_SETOPERATIONS,(void*)pResource) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Ioctl MDrv_CMD_DIP_GET_RESOURCE fail\n");
            utopia_free(pResource);
            return E_APIXC_RET_FAIL;
        }
        utopia_free(pResource);
#else
        //MDrv_XC_DIP_CMDQ_SetAction(NULL, eAction, eWindow);
        MDrv_XC_DIP_CMDQ_SetOperations(NULL, pOps, bEnable, eWindow);
#endif
        return E_APIXC_RET_OK;
#else
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

//MS_BOOL PreScaling = FALSE;
//-------------------------------------------------------------------------------------------------
/// Reset DIP for restart capture from buffer 0
/// @return E_APIXC_RET_OK - Success
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SWReset(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        return E_APIXC_RET_OK;
    }
#endif
    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pTrigMode->eTrigMode = DIP_TRIG_RESET;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(void*)pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_RESET fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
	MDrv_XC_DIP_SWReset(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}


//-------------------------------------------------------------------------------------------------
/// Init the status for DIP path
/// @return E_APIXC_RET_OK - Success
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_InitByDIP(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_RESOURCE* pResource = NULL;
#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_Init();
        return E_APIXC_RET_OK;
    }
#endif
    pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
    if (pResource == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    if(pInstantDip[eWindow] == NULL)
    {
        if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            XC_ERR("Open DIP fail\n");
            utopia_free(pResource);
            return E_APIXC_RET_FAIL;
        }
    }
	//init
	pResource->eWindow = eWindow;
	if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INIT,(void*)pResource) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_INIT fail\n");
		utopia_free(pResource);
        return E_APIXC_RET_FAIL;
    }

    utopia_free(pResource);
#else
	MDrv_XC_DIP_Init(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
///Set the frame buffer address for specific window
/// @param  u32FBAddress      \b IN: the frame buffer address
/// @param  u32FBSize           \b IN: the frame buffer size
/// @param  eWindow             \b IN: set address for which window(DIP_WINDOW)
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetFrameBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_Disable_IPM_ReadWriteRequest(NULL,DISABLE, eWindow);
    MDrv_XC_DWIN_SetFrameBufferAddress(NULL,u32FBAddress, u32FBSize, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for specify input source type for DIP path
/// @param enInputSourceType                  \b IN: the inputsource type
/// @param  eWindow                                \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetInputSource(INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
	MDrv_XC_DWIN_SetInputSource(NULL,enInputSourceType, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for get input source type for DIP path
/// @param enInputSourceType                  \b OUT: the inputsource type
/// @param  eWindow                                \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_GetInputSource(INPUT_SOURCE_TYPE_t *enInputSourceType, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_GetInputSource(NULL,enInputSourceType,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable input source for DIP path
/// @param bEnable                  \b IN: enable or disable inputsource
/// @param  eWindow               \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_DisableInputSource(MS_BOOL bDisable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_DisableInputSource(NULL,bDisable, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the specific window
/// @param  pstXC_SetWin_Info      \b IN: the information of the window setting
/// @param  u32InitDataLen             \b IN: the length of the pstXC_SetWin_Info
/// @param  eWindow                     \b IN: which window we are going to set
/// @return @ref E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetWindow(XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_WINDOW* pSetWindow = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        GopD_property.u16x = pstXC_SetWin_Info->stCapWin.x;
        GopD_property.u16y = pstXC_SetWin_Info->stCapWin.y;
        if( pstXC_SetWin_Info->u16PreHCusScalingSrc != pstXC_SetWin_Info->u16PreHCusScalingDst ){
            XC_ERR("eWindow[%d] doesnt support Hscaling src[%d] dst[%d]\n",eWindow,pstXC_SetWin_Info->u16PreHCusScalingSrc,pstXC_SetWin_Info->u16PreHCusScalingDst);
            return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
        if( pstXC_SetWin_Info->u16PreVCusScalingSrc != pstXC_SetWin_Info->u16PreVCusScalingDst ){
            XC_ERR("eWindow[%d] doesnt support Vscaling src[%d] dst[%d]\n",eWindow,pstXC_SetWin_Info->u16PreVCusScalingSrc,pstXC_SetWin_Info->u16PreVCusScalingDst);
            return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
        return E_APIXC_RET_OK;
    }
#endif
    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetWindow = (DIP_SET_WINDOW*)utopia_malloc(sizeof(DIP_SET_WINDOW));
    if (pSetWindow == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetWindow->u32DataLen = u32InitDataLen;
    memcpy(&(pSetWindow->WinInfo),pstXC_SetWin_Info, sizeof(XC_SETWIN_INFO));
	pSetWindow->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_WINDOW,(void*)pSetWindow) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_WINDOW fail\n");
		utopia_free(pSetWindow);
        return E_APIXC_RET_FAIL;
    }
    pstXC_SetWin_Info->u16PreHCusScalingDst=pSetWindow->WinInfo.u16PreHCusScalingDst;
    utopia_free(pSetWindow);

    return E_APIXC_RET_OK;
#else
	MDrv_XC_DIP_SetWindow(NULL,pstXC_SetWin_Info,u32InitDataLen,eWindow);
#endif
	return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API to get DE window for DIP path
/// @param psWin                  \b OUT: the DE window info to get
/// @param  eWindow             \b IN:  for which window(DIP_WINDOW)
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_GetDEWindow(MS_WINDOW_TYPE *psWin, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_get_DE_Window(NULL,psWin, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//---------------------------------

//---- Aileen
//-------------------------------------------------------------------------------------------------
/// Set the Vertical start of capture window for specific window
/// @param  u16Vstart        \b IN: the window info to get
/// @param  eWindow          \b IN: for which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVstart(MS_U16 u16Vstart , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_v_start(NULL,u16Vstart, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the horizontal start of capture window for specific window
/// @param  u16Hstart        \b IN: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHstart(MS_U16 u16Hstart , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_h_start(NULL,u16Hstart, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the Vertical size of capture window for specific window
/// @param  u16Vsize         \b IN: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowVsize(MS_U16 u16Vsize , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_v_size(NULL,u16Vsize, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set the Horizontal size of capture window for specific window
/// @param  u16Hsize         \b IN: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetCaptureWindowHsize(MS_U16 u16Hsize , SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_set_capture_h_size(NULL,u16Hsize, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Get the capture window for specific window
/// @param  capture_win      \b OUT: the window info to get
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to get
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_GetCaptureWindow(MS_WINDOW_TYPE* capture_win, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2

#else
    MDrv_XC_DWIN_get_capture_window(NULL,capture_win, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable RGB to YUV color space convertion
/// @param bEnable \b IN: enable or disable
/// @param  eWindow          \b IN: which window(DIP_WINDOW) to set
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_EnableR2Y(MS_BOOL benable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_EnableR2YCSC(benable);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bR2Y = benable;
    pSetProperty->eFlag = DIP_PRO_R2Y;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_R2Y fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DWIN_EnableR2YCSC(NULL,benable, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//=============== DIP =====================//
//-------------------------------------------------------------------------------------------------
/// Set  FrameRateCtrl
/// @param  bEnable           \b IN: FrameRateCtrl enable or not
/// @param  u16Src             \b IN: Src Frame
/// @param  u16Dst            \b IN: Dst Frame
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_FrameRateCtrl(MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support frame rate cnt\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stFrameRateControl.bFrameRateCtrl = bEnable;
    pSetProperty->Property.stFrameRateControl.u16In = u16In;
    pSetProperty->Property.stFrameRateControl.u16Out = u16Out;
    pSetProperty->eFlag = DIP_PRO_FRC;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_FRC fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetFRC(NULL,bEnable,u16In,u16Out,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set DIP win property
/// @param  pstDIPWinProperty     \b IN: @ref ST_DIP_WINPROPERTY
/// @param  eWindow         \b IN: the window
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetDIPWinProperty(ST_XC_DIP_WINPROPERTY *pstDIPWinProperty, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_WIN_PROPERTY* pSetWinProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        GopD_property.u16w = pstDIPWinProperty->u16Width;
        GopD_property.u16h = pstDIPWinProperty->u16Height;
        if( (DataFmt == DIP_DATA_FMT_YUV422) || (DataFmt == DIP_DATA_FMT_RGB565))
            GopD_property.u16fbw = pstDIPWinProperty->u16Pitch/2;
        else if(DataFmt == DIP_DATA_FMT_ARGB8888)
            GopD_property.u16fbw = pstDIPWinProperty->u16Pitch/4;
        GopD_property.u32fbaddr0 = pstDIPWinProperty->u32BufStart;
        GopD_property.u32fbaddr1 = pstDIPWinProperty->u32BufEnd;

        if(pstDIPWinProperty->enSource == SCALER_DIP_SOURCE_TYPE_MAIN)
            MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_IP);
        else if(pstDIPWinProperty->enSource == SCALER_DIP_SOURCE_TYPE_OP_CAPTURE)
            MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);
        else{
            XC_ERR("eWindow[%d] doesnt support capture source[%d]\n",eWindow,pstDIPWinProperty->enSource);
            return E_APIXC_RET_FAIL_ENUM_NOT_SUPPORT;
        }
        MApi_GOP_DWIN_SetWinProperty(&GopD_property);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetWinProperty = (DIP_SET_WIN_PROPERTY*)utopia_malloc(sizeof(DIP_SET_WIN_PROPERTY));
    if (pSetWinProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    memcpy(&(pSetWinProperty->WinProperty),pstDIPWinProperty, sizeof(ST_XC_DIP_WINPROPERTY));
	pSetWinProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_WINPROPERTY,(void*)pSetWinProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_WINPROPERTY fail\n");
		utopia_free(pSetWinProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetWinProperty);
#else
	MDrv_XC_DIP_SetWinProperty(NULL,pstDIPWinProperty, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Get DWIN GetBufferInfo
//-------------------------------------------------------------------------------------------------
BUFFER_INFO MApi_XC_DIP_GetBufInfo(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	BUFFER_INFO BufInfo;
    DIP_BUF_INFO* pBufInfo = NULL;
	DIP_RESOURCE* pResource = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        BufInfo.u8BufCnt = 1;
        BufInfo.u32YBuf[0] = GopD_property.u32fbaddr0;
        return BufInfo;
    }
#endif

    memset(&BufInfo, 0, sizeof(BUFFER_INFO));
    if(pInstantDip[eWindow] == NULL)
    {
    	pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
		if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
		{
			XC_ERR("Open DIP fail\n");
			utopia_free(pResource);
			return BufInfo;
		}
		utopia_free(pResource);
    }

    pBufInfo = (DIP_BUF_INFO*)utopia_malloc(sizeof(DIP_BUF_INFO));
    if (pBufInfo == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return BufInfo;
    }
	memset(&(pBufInfo->BufInfo), 0, sizeof(BUFFER_INFO));
	pBufInfo->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_GET_BUFINFO,(void*)pBufInfo) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_GET_BUFINFO fail\n");
		utopia_free(pBufInfo);
        return BufInfo;
    }
    memcpy(&BufInfo,&(pBufInfo->BufInfo), sizeof(BUFFER_INFO));
    utopia_free(pBufInfo);
    return BufInfo;
#else
	return MDrv_XC_DIP_GetBufferInfo(NULL,eWindow);
#endif
#else
    BUFFER_INFO BufInfo;
    memset(&BufInfo, 0, sizeof(BUFFER_INFO));
	return BufInfo;
#endif

}

/********************************************************************************/
/// Set gop dwin capture one frame
/// @return E_APIXC_RET_OK - Success
/********************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_CapOneFrame(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_CaptureOneFrame();
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pTrigMode->eTrigMode = DIP_TRIG_ONESHOT;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(void*)pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_ONESHOT fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
    MDrv_XC_DIP_CaptureOneFrame(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

/********************************************************************************/
/// Set XC dwin capture one frame
/// @return E_APIXC_RET_OK - Success
/********************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_CapOneFrameFast(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_CaptureOneFrame2();
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pTrigMode->eTrigMode = DIP_TRIG_ONESHOTFAST;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(void*)pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_ONESHOTFAST fail\n");
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
    MDrv_XC_DIP_CaptureOneFrame2(NULL,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// clear DWIN interrupt
/// @param u8mask \b IN: interrup mask
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_ClearInt(MS_U16 u8mask, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_INTER_STUS* pIntrStus = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_ClearIntr(u8mask);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_OK;
    }

    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrStus->IntrStautus = u8mask;
    pIntrStus->eFlag = DIP_INTR_CLEAR;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT,(void*)pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_CLEAR fail\n");
		utopia_free(pIntrStus);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrStus);
#else
    MDrv_XC_DIP_ClearIntr(NULL,u8mask,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable DWIN
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_Ena(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_TRIGGER_MODE* pTrigMode = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_Enable(bEnable);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    pTrigMode = (DIP_TRIGGER_MODE*)utopia_malloc(sizeof(DIP_TRIGGER_MODE));
    if (pTrigMode == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    if(bEnable)
        pTrigMode->eTrigMode = DIP_TRIG_ON;
    else
        pTrigMode->eTrigMode = DIP_TRIG_OFF;
	pTrigMode->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_TRIGGER,(void*)pTrigMode) != UTOPIA_STATUS_SUCCESS)
    {
        if(bEnable)
        {
            XC_ERR("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_ON fail\n");
        }
        else
        {
            XC_ERR("Ioctl MDrv_CMD_DIP_TRIGGER DIP_TRIG_OFF fail\n");
        }
		utopia_free(pTrigMode);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pTrigMode);
#else
	MDrv_XC_DIP_EnableCaptureStream(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

/******************************************************************************/
/// Set interrupt mask of XC DWIN.
/// @param u8mask \b IN bitmask of interrupt type
///   - bit7 DWIN VSYNC interrupt mask
///   - bit6 Dump Window interlace Bottom Field ACK Interrupt Mask
///   - bit5 Dump Window interlace Top Field ACK Interrupt Mask
///   - bit4 Dump Window Progressive ACK Interrupt Mask
///   - bit3:1 Reserved
///   - bit0 GWIN VSYNC Interrupt Mask
/// @param bEnable \b IN
///   - # TRUE enable interrupts specified by u8mask
///   - # FALSE disable interrupts specified by u8mask
/// @return E_APIXC_RET_OK - Success
/******************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_EnaInt(MS_U16 u8mask, MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_INTER_STUS* pIntrStus = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_EnableIntr(u8mask,bEnable);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_OK;
    }

    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrStus->IntrStautus = u8mask;
    if(bEnable)
        pIntrStus->eFlag = DIP_INTR_ENABLE;
    else
        pIntrStus->eFlag = DIP_INTR_DISABLE;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT,(void*)pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        if(bEnable)
        {
            XC_ERR("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_ENABLE fail\n");
        }
        else
        {
            XC_ERR("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_DISABLE fail\n");
        }
		utopia_free(pIntrStus);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrStus);
#else
    MDrv_XC_DIP_EnableIntr(NULL,u8mask, bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

/********************************************************************************/
/// Get XC Dwin interrupt status
/// @return XC Dwin interrupt status
/********************************************************************************/
MS_U16 MApi_XC_DIP_GetIntStatus(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	MS_U16 u16IntrStus = 0;
    DIP_INTER_STUS* pIntrStus = NULL;
	DIP_RESOURCE* pResource = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        u16IntrStus = MApi_GOP_DWIN_GetIntrStatus();
        return u16IntrStus;
    }
#endif
	if(pInstantDip[eWindow] == NULL)
	{
		pResource = (DIP_RESOURCE*)utopia_malloc(sizeof(DIP_RESOURCE));
		if(UtopiaOpen(MODULE_DIP|DIPDRIVER_BASE,(void**)&pInstantDip[eWindow], 0, NULL) != UTOPIA_STATUS_SUCCESS)
		{
			XC_ERR("Open DIP fail\n");
			utopia_free(pResource);
			return u16IntrStus;
		}

		utopia_free(pResource);
	}
    pIntrStus = (DIP_INTER_STUS*)utopia_malloc(sizeof(DIP_INTER_STUS));
    if (pIntrStus == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return u16IntrStus;
    }
    pIntrStus->eFlag = DIP_INTR_GET;
	pIntrStus->IntrStautus=0;
	pIntrStus->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT,(void*)pIntrStus) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_GET fail\n");
		utopia_free(pIntrStus);
        return u16IntrStus;
    }
    u16IntrStus = pIntrStus->IntrStautus;
    utopia_free(pIntrStus);

    return u16IntrStus;
#else
    return MDrv_XC_DIP_GetIntrStatus(NULL,eWindow);
#endif
#else
    MS_U16 u16IntrStus = 0;
	return u16IntrStus;
#endif

}

//******************************************************************************
/// Set DWIN data source scan type
/// @param enScanMode \b IN: EN_GOP_DWIN_SCAN_MODE
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetSourceScanType(EN_XC_DIP_SCAN_MODE enScanMode, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_SelectSourceScanType((EN_GOP_DWIN_SCAN_MODE)enScanMode);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.eScanMode = enScanMode;
    pSetProperty->eFlag = DIP_PRO_SCANMODE;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SCANMODE fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SelectSourceScanType(NULL,(EN_XC_DWIN_SCAN_TYPE)enScanMode,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for enable or disable DWIN Interlace write
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_EnaInterlaceWrite(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support interlace write\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bInterlaceW = bEnable;
    pSetProperty->eFlag = DIP_PRO_INTERLACE_W;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_INTERLACE_W fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetInterlaceWrite(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN Y2R
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_EnableY2R(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support Y2R\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bY2R = bEnable;
    pSetProperty->eFlag = DIP_PRO_Y2R;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_Y2R fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetY2R(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// Set DWIN Capture format
/// @param fmt \b IN: buffer format to store DWIN content
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetOutputDataFmt(EN_XC_DIP_DATA_FMT fmt, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        DataFmt = fmt;
        if(fmt == DIP_DATA_FMT_YUV422)
            MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_UV8Y8);
        else if(fmt == DIP_DATA_FMT_RGB565)
            MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_RGB565);
        else if(fmt == DIP_DATA_FMT_ARGB8888)
            MApi_GOP_DWIN_SetDataFmt(DWIN_DATA_FMT_ARGB8888);
        else{
            XC_ERR("eWindow[%d] doesnt support fmt[%d]\n",eWindow,fmt);
            return E_APIXC_RET_FAIL_ENUM_NOT_SUPPORT;
        }
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.eDataFmt = fmt;
    pSetProperty->eFlag = DIP_PRO_DATAFMT;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_DATAFMT fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetDataFmt(NULL,(EN_DRV_XC_DWIN_DATA_FMT)fmt,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
/*****************************************************************************/
/// Set DWIN Capture constant alpha value for format ARGB8888
/// @param u8AlphaVal \b IN: constant alpha value
/// @return E_APIXC_RET_OK - Success
/*****************************************************************************/
E_APIXC_ReturnValue MApi_XC_DIP_SetAlpha(MS_U8 u8AlphaVal, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_SetAlphaValue(u8AlphaVal);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.u8Alpha = u8AlphaVal;
    pSetProperty->eFlag = DIP_PRO_ALPHA;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_ALPHA fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetAlphaValue(NULL,u8AlphaVal,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN UV SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SwapUV(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        MApi_GOP_DWIN_SetUVSwap(bEnable);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bSwapUV = bEnable;
    pSetProperty->eFlag = DIP_PRO_SWAPUV;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SWAPUV fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetUVSwap(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN YC SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SwapYC(MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support YC swap\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bSwapYC = bEnable;
    pSetProperty->eFlag = DIP_PRO_SWAPYC;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SWAPYC fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetYCSwap(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for enable or disable DWIN RGB SWAP
/// @param bEnable \b IN DWIN enable or not
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SwapRGB(MS_BOOL bEnable, EN_XC_DIP_RGB_SWAP_TYPE enSwapType, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support RGB swap\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stSwapRGB.bSwap = bEnable;
    pSetProperty->Property.stSwapRGB.eSwapType = enSwapType;
    pSetProperty->eFlag = DIP_PRO_SWAPRGB;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_SWAPRGB fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetRGBSwap(NULL,bEnable,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for set output capture mux
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetOutputCapture(MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        if( eOpCapture == E_XC_DIP_OP2 )
            MApi_XC_OP2VOPDESel(E_OP2VOPDE_WHOLEFRAME_WITHOSD);
        else
            MApi_XC_OP2VOPDESel(E_OP2VOPDE_WHOLEFRAME);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stOpCapture.bOpCapture = bEnable;
    pSetProperty->Property.stOpCapture.eOpCapture = eOpCapture;
    pSetProperty->eFlag = DIP_PRO_OP_CAPTURE;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_OP_CAPTURE fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetOutputCapture(NULL,bEnable,eOpCapture,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for set DIPW H & V Mirror
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetMirror(MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
	DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support Mirror\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stMirror.bHMirror = bHMirror;
    pSetProperty->Property.stMirror.bVMirror = bVMirror;
    pSetProperty->eFlag = DIP_PRO_MIRROR;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_MIRROR fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
    MDrv_XC_DIP_SetMirror(NULL,bHMirror,bVMirror,eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for set DIPR Property
/// @return E_APIXC_RET_OK - Success
//******************************************************************************

E_APIXC_ReturnValue MApi_XC_DIP_SetDIPRProperty(ST_XC_DIPR_PROPERTY *pstDIPRProperty,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_SET_DIPR_PROPERTY_EX* pSetDIPRProperty_ex = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support DIPR\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetDIPRProperty_ex = (DIP_SET_DIPR_PROPERTY_EX*)utopia_malloc(sizeof(DIP_SET_DIPR_PROPERTY_EX));
    if (pSetDIPRProperty_ex == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
        return E_APIXC_RET_FAIL;
    }
    memcpy(&(pSetDIPRProperty_ex->stDIPRProperty_ex.stDIPRProperty),pstDIPRProperty, sizeof(ST_XC_DIPR_PROPERTY));
    pSetDIPRProperty_ex->enDIPRFlag = E_DIP_SET_DIPR_NORMAL;
    pSetDIPRProperty_ex->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_DIPR_PROPERTY,(void*)pSetDIPRProperty_ex) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_DIPR_PROPERTY fail\n");
        utopia_free(pSetDIPRProperty_ex);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetDIPRProperty_ex);
#else
    MDrv_XC_DIP_SetDIPRProperty(NULL,pstDIPRProperty, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

E_APIXC_ReturnValue MApi_XC_DIP_SetDIPRProperty_EX(ST_XC_DIPR_PROPERTY_EX *pstDIPRProperty_ex, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
        DIP_SET_DIPR_PROPERTY_EX* pSetDIPRProperty_ex = NULL;

#ifdef GOPD_DIP
        if(eWindow == Gopd_dip){
            XC_ERR("eWindow[%d] doesnt support DIPR\n",eWindow);
            return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
#endif

        if(pInstantDip[eWindow] == NULL)
        {
            XC_ERR("DIP not Opened\n");
            return E_APIXC_RET_FAIL;
        }
        pSetDIPRProperty_ex = (DIP_SET_DIPR_PROPERTY_EX*)utopia_malloc(sizeof(DIP_SET_DIPR_PROPERTY_EX));
        if (pSetDIPRProperty_ex == NULL)
        {
            XC_ERR("%s alloc fail\n",__FUNCTION__);
            return E_APIXC_RET_FAIL;
        }
        memcpy(&(pSetDIPRProperty_ex->stDIPRProperty_ex),pstDIPRProperty_ex, sizeof(ST_XC_DIPR_PROPERTY_EX));
        pSetDIPRProperty_ex->enDIPRFlag = E_DIP_SET_DIPR_EX;
        pSetDIPRProperty_ex->eWindow = eWindow;
        if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_DIPR_PROPERTY,(void*)pSetDIPRProperty_ex) != UTOPIA_STATUS_SUCCESS)
        {
            XC_ERR("Ioctl MDrv_CMD_DIP_SET_DIPR_PROPERTY_EX fail\n");
            utopia_free(pSetDIPRProperty_ex);
            return E_APIXC_RET_FAIL;
        }
        utopia_free(pSetDIPRProperty_ex);
#else
        MDrv_XC_DIP_SetDIPRProperty_EX(NULL,pstDIPRProperty_ex, eWindow);
#endif
        return E_APIXC_RET_OK;
#else
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

    }


//******************************************************************************
/// API for MApi_XC_DIP_Rotation
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_Rotation(MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,SCALER_DIP_WIN eTmpWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eTmpWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support Rotate\n",eTmpWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eTmpWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stRotation.bRotation = bRotation;
    pSetProperty->Property.stRotation.eRoDirection = eRoDirection;
    pSetProperty->eFlag = DIP_PRO_ROTATION;
	pSetProperty->eWindow = eTmpWindow;
    if(UtopiaIoctl(pInstantDip[eTmpWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_ROTATION fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
	MDrv_XC_DIP_SetRotation(NULL,bRotation,eRoDirection,eTmpWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for MApi_XC_DIP_SetPinpon
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetPinpon(MS_BOOL bPinpon,MS_PHY u32PinponAddr,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
    DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support Pinpon\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.stPinpon.bPinpon = bPinpon;
    pSetProperty->Property.stPinpon.u32PinponAddr = u32PinponAddr;
    pSetProperty->eFlag = DIP_PRO_PINPON;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_PINPON fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);
#else
		MDrv_XC_DIP_SetPinpon(NULL,bPinpon,u32PinponAddr,eWindow);
#endif
		return E_APIXC_RET_OK;
#else
		return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

#ifdef DIP_UTOPIA2K
static InterruptCb pAPI_DipIntCb;
static InterruptCb pAPI_Dwin0IntCb;
static InterruptCb pAPI_Dwin1IntCb;
static MS_BOOL bAPI_DipIsr = FALSE,bAPI_Dwin0Isr = FALSE,bAPI_Dwin1Isr = FALSE,bAPI_Attached = FALSE;

static void _MApi_XC_DIP_CallBack(InterruptNum eIntNum)
{
    if( (MApi_XC_DIP_GetIntStatus(DIP_WINDOW) > 0) && (pAPI_DipIntCb != NULL) )
        (void)(pAPI_DipIntCb)(eIntNum);
    if( (MApi_XC_DIP_GetIntStatus(DWIN0_WINDOW) > 0) && (pAPI_Dwin0IntCb != NULL) )
        (void)(pAPI_Dwin0IntCb)(eIntNum);
    if( (MApi_XC_DIP_GetIntStatus(DWIN1_WINDOW) > 0) && (pAPI_Dwin1IntCb != NULL) )
        (void)(pAPI_Dwin1IntCb)(eIntNum);

    MsOS_EnableInterrupt(E_INT_IRQ_DIPW);
}
#endif
//******************************************************************************
/// API for MApi_XC_InterruptAttach
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
#ifdef GOPD_DIP
#ifdef MSOS_TYPE_LINUX
void dip_thread(void)
{
    while(1)
    {
        if( MApi_GOP_DWIN_GetIntrStatus() )
            (void)(pGopd_CB)(E_INT_IRQ_DIPW);
        MsOS_DelayTask(FRAME_DELAY);
        if(bDeAttached)
            break;
    }
}
#endif
#endif

E_APIXC_ReturnValue MApi_XC_DIP_InterruptAttach(InterruptCb pIntCb,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
#ifdef MSOS_TYPE_LINUX
        pGopd_CB = pIntCb;
        if(bDeAttached == TRUE)
        {
            pthread_create(&id,NULL,(void *) dip_thread,NULL);
            bDeAttached = FALSE;
        }
        return E_APIXC_RET_OK;
#else
    	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
    }
#endif

#ifndef DIP_UTOPIA2K
	DIP_INTER_CB* pIntrCb = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_OK;
    }

    pIntrCb = (DIP_INTER_CB*)utopia_malloc(sizeof(DIP_INTER_CB));
    if (pIntrCb == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrCb->pIntCb = pIntCb;
    pIntrCb->eFlag = DIP_INTR_ATTACH;
	pIntrCb->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT_CB,(void*)pIntrCb) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_ATTACH fail\n");
		utopia_free(pIntrCb);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrCb);
#else
    if(bAPI_Attached == FALSE)
    {
        MsOS_AttachInterrupt(E_INT_IRQ_DIPW,_MApi_XC_DIP_CallBack);
    }
    bAPI_Attached = TRUE;

    if( (bAPI_DipIsr == FALSE) && (bAPI_Dwin0Isr == FALSE) && (bAPI_Dwin1Isr == FALSE))
    {
        MsOS_EnableInterrupt(E_INT_IRQ_DIPW);
    }

    if( eWindow == DIP_WINDOW )
    {
        pAPI_DipIntCb = pIntCb;
        bAPI_DipIsr = TRUE;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        pAPI_Dwin0IntCb = pIntCb;
        bAPI_Dwin0Isr = TRUE;
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        pAPI_Dwin1IntCb = pIntCb;
        bAPI_Dwin1Isr = TRUE;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }
#endif
#else
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
	MDrv_XC_DIP_InterruptAttach(NULL,pIntCb,eWindow);
	_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
	return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
//******************************************************************************
/// API for MApi_XC_InterruptDetach
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_InterruptDetach(SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

#ifdef UTOPIA_V2
#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
#ifdef MSOS_TYPE_LINUX
        bDeAttached = TRUE;
        if( pthread_join(id, NULL) != 0 )
        {
            XC_DBUG("[DIP] InterruptDetach fail \n");
        }
        return E_APIXC_RET_OK;
#else
    	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
    }
#endif

#ifndef DIP_UTOPIA2K
	DIP_INTER_CB* pIntrCb = NULL;
    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_OK;
    }

    pIntrCb = (DIP_INTER_CB*)utopia_malloc(sizeof(DIP_INTER_CB));
    if (pIntrCb == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pIntrCb->eFlag = DIP_INTR_DETACH;
	pIntrCb->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_INTERRUPT_CB,(void*)pIntrCb) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_INTERRUPT DIP_INTR_DETACH fail\n");
		utopia_free(pIntrCb);
        return E_APIXC_RET_OK;
    }
    utopia_free(pIntrCb);
#else
    if( eWindow == DIP_WINDOW )
    {
        bAPI_DipIsr = FALSE;
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        bAPI_Dwin0Isr = FALSE;
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        bAPI_Dwin1Isr = FALSE;
    }
    else
    {
        return E_APIXC_RET_FAIL;
    }

    if( (bAPI_DipIsr == FALSE) && (bAPI_Dwin0Isr == FALSE) && (bAPI_Dwin1Isr == FALSE))
    {
    MsOS_DisableInterrupt(E_INT_IRQ_DIPW);
    //MsOS_DetachInterrupt(E_INT_IRQ_DIPW); // do not deattach, because deattach will take 100 ms to wait pthread join
    }
#endif
#else
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
	MDrv_XC_DIP_InterruptDetach(NULL,eWindow);
	_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
	return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}

//******************************************************************************
/// API for MApi_XC_DIP_SetHVSP
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_SetHVSP(MS_BOOL bSelect,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION
    DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support HVSP swap\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.bHVSP = bSelect;
    pSetProperty->eFlag = DIP_PRO_HVSP;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_PINPON fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);

    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

//******************************************************************************
/// API for MApi_XC_DIP_Set420TileBlock
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
E_APIXC_ReturnValue MApi_XC_DIP_Set420TileBlock(EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION
    DIP_SET_PROPERTY* pSetProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        XC_ERR("eWindow[%d] doesnt support 420 tile block\n",eWindow);
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetProperty = (DIP_SET_PROPERTY*)utopia_malloc(sizeof(DIP_SET_PROPERTY));
    if (pSetProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    pSetProperty->Property.eTileBlock = eTileBlock;
    pSetProperty->eFlag = DIP_PRO_TILEBLOCK;
	pSetProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_PROPERTY,(void*)pSetProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_PROPERTY DIP_PRO_PINPON fail\n");
		utopia_free(pSetProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetProperty);

    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

//******************************************************************************
/// API for MApi_XC_DIP_SetPowerState
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
MS_U32 MApi_XC_DIP_SetPowerState(EN_POWER_MODE u16PowerState)
{
#ifndef DISABLE_DIP_FUNCTION
#ifdef UTOPIA_V2

#else

#endif
#endif
    return UTOPIA_STATUS_SUCCESS;
}

//******************************************************************************
/// API for MApi_XC_DIP_SetDbgLevel
/// @return E_APIXC_RET_OK - Success
//******************************************************************************
MS_U32 MApi_XC_DIP_SetDbgLevel(EN_XC_DIP_DEBUG_LEVEL u8DbgLevel)
{
#ifndef DISABLE_DIP_FUNCTION
    if(pInstantDip[DIP_WINDOW] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }

    if(UtopiaIoctl(pInstantDip[DIP_WINDOW], MDrv_CMD_DIP_SET_DBG_LEVEL, (void*)(&u8DbgLevel)) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_DBG_LEVEL fail\n");
        return E_APIXC_RET_FAIL;
    }
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}
//================================ Obsolete Function Start =======================================
//-------------------------------------------------------------------------------------------------
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Alert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  Alert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// !!! below function will be "REMOVED" or "REFACTORING" later, please do not use it !!!!!!!!!
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Set DWIN base0 base 1 address
/// @param  u32FBAddress0,u32FBAddress1     \b IN: the framebuffer address
/// @param  eWindow         \b IN: the window
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_DIP_SetWinProperty_Ex(MS_U8 u8BufCnt ,MS_U16 u16Width, MS_U16 u16Height, MS_PHY u32BufStart ,MS_PHY u32BufEnd, SCALER_DIP_SOURCE_TYPE eSource, SCALER_DIP_WIN eWindow)
{
#ifndef DISABLE_DIP_FUNCTION

    ST_XC_DIP_WINPROPERTY stDIPWinProperty;
    stDIPWinProperty.u8BufCnt    = u8BufCnt;
    stDIPWinProperty.u16Width    = u16Width;
    stDIPWinProperty.u16Pitch    = 0x0;
    stDIPWinProperty.u16Height   = u16Height;
    stDIPWinProperty.u32BufStart = u32BufStart;
    stDIPWinProperty.u32BufEnd   = u32BufEnd;
    stDIPWinProperty.enSource     = eSource;
#ifdef UTOPIA_V2
	DIP_SET_WIN_PROPERTY* pSetWinProperty = NULL;

#ifdef GOPD_DIP
    if(eWindow == Gopd_dip){
        GopD_property.u16w = u16Width;
        GopD_property.u16h = u16Height;
        GopD_property.u16fbw = u16Width;
        GopD_property.u32fbaddr0 = u32BufStart;
        GopD_property.u32fbaddr1 = u32BufEnd;
        if(eSource == SCALER_DIP_SOURCE_TYPE_MAIN)
            MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_IP);
        else if(eSource == SCALER_DIP_SOURCE_TYPE_OP_CAPTURE)
            MApi_GOP_DWIN_SetSourceSel(DWIN_SRC_OP);
        else{
            XC_ERR("eWindow[%d] doesnt support capture source[%d]\n",eWindow,eSource);
            return E_APIXC_RET_FAIL_ENUM_NOT_SUPPORT;
        }
        MApi_GOP_DWIN_SetWinProperty(&GopD_property);
        return E_APIXC_RET_OK;
    }
#endif

    if(pInstantDip[eWindow] == NULL)
    {
        XC_ERR("DIP not Opened\n");
        return E_APIXC_RET_FAIL;
    }
    pSetWinProperty = (DIP_SET_WIN_PROPERTY*)utopia_malloc(sizeof(DIP_SET_WIN_PROPERTY));
    if (pSetWinProperty == NULL)
    {
        XC_ERR("%s alloc fail\n",__FUNCTION__);
    	return E_APIXC_RET_FAIL;
    }
    memcpy(&(pSetWinProperty->WinProperty),&stDIPWinProperty, sizeof(ST_XC_DIP_WINPROPERTY));
	pSetWinProperty->eWindow = eWindow;
    if(UtopiaIoctl(pInstantDip[eWindow],MDrv_CMD_DIP_SET_WINPROPERTY,(void*)pSetWinProperty) != UTOPIA_STATUS_SUCCESS)
    {
        XC_ERR("Ioctl MDrv_CMD_DIP_SET_WINPROPERTY fail\n");
		utopia_free(pSetWinProperty);
        return E_APIXC_RET_FAIL;
    }
    utopia_free(pSetWinProperty);
#else
    MDrv_XC_DIP_SetWinProperty(NULL,&stDIPWinProperty, eWindow);
#endif
    return E_APIXC_RET_OK;
#else
	return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif

}
MS_U32 MDrv_XC_DIP_GetResourceByPipeID(MS_U32 u32PipeID, MS_U32*  U32RetResNum, DIPRESOURCE* pResource)
{
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
}

MS_U32 MDrv_XC_DIP_ConfigPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    SCALER_DIP_WIN eWindow=DIP_WINDOW;
    DIP_CONFIG_PIPE DIP_Config_Pipe;
    MS_U32 i=0;

    DIP_Config_Pipe.u32PipeID = u32PipeID;
    DIP_Config_Pipe.u32SecureDMA = u32SecureDMA;
    DIP_Config_Pipe.u32OperationMode = u32OperationMode;
    DIP_Config_Pipe.u32ReturnValue = UTOPIA_STATUS_FAIL;

    for(i=0;i<MAX_DIP_WINDOW;i++)
    {
        if (pInstantDip[(SCALER_DIP_WIN)i] == NULL)
        {
            continue;
        }
        else
        {
            eWindow = (SCALER_DIP_WIN)i;
            break;
        }
        if( i == (MAX_DIP_WINDOW-1))
        {
            XC_ERR("DIP not Opened\n");
            return UTOPIA_STATUS_FAIL;
        }
    }

    if(UtopiaIoctl(pInstantDip[eWindow], MDrv_CMD_DIP_CONFIG_PIPE, (void*)&DIP_Config_Pipe) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        return DIP_Config_Pipe.u32ReturnValue;
    }
}

MS_U32 MDrv_XC_DIP_CheckPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
}

//================================ Obsolete Function End =======================================

//=============== DIP =====================//
