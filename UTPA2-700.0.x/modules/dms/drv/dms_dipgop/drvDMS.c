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
/// file    drvDMS.c
/// @author MStar Semiconductor Inc.
/// @brief  DMS Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#define  MDRV_DMS_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvDMS.h"
#include "halDMS.h"
#include "UFO.h"
#include "halCHIP.h"

// utopia DRV
#include "drvMVOP.h"
#include "drvSYS.h"
#include "apiXC.h"
#include "apiPNL.h"
#include "apiXC_DWIN.h"
#include "apiVDEC_EX.h"
#include "apiGOP.h"
#include "apiGFX.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDMS_private.h"

void* g_pDMSRes = NULL;

#ifndef ANDROID
#define DMS_PRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define DMS_PRINTF ALOGD
#endif

#if(DMS_DONT_USE_CMA == 1)
#ifdef MSOS_TYPE_LINUX_KERNEL
#include "drvCMAPool_v2.h"
#else
#include "drvCMAPool.h"
#endif
#include "msos/linux/mdrv_cma_pool_st.h"
#endif

// FIXME: it needs MVOP to add it to header file.
MS_BOOL __attribute__((weak))   MDrv_MVOP_EnableInterrupt(MS_U8 eIntType);

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
// DIP MultiView
#define DMS_DIP_NUM             1
#define DMS_FHD_Hszie           1920
#define DMS_FHD_Vszie           1080
#define DMS_Frame_Buffer_Width           3840
#define DMS_Frame_Buffer_Height           2160
#define DMS_Support_Max_Resolution_Hsize           3840
#define DMS_Support_Max_Resolution_Vsize           2160
#define GOP_NUM 2

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define DMS_DIP_TIMESLICE  6  //(ms)
#define DMS_DIP_Alignment(value, align)  ((value)/(align) * (align))
#define DMS_DIPR_Alignment(value, align)  ((value + (align-1))/(align) * (align))
#define DMS_DIP_TIMEOUT 500 //(ms)
#define DMS_DIP_BUFFER_COUNT  1
#define DMS_DIP_MULTIVIEW_TRANSPARENT_COLOR     (0x00000000)
#define MVOP_WINDOW 0
#define DMS_INVALID_FRAME_ID 0xFF

//Define for Develop
#define DMS_MVOP_FLOW 1
#define DBG_FPS 0
#define NEW_FLIP 1
#define DBG_DMS_LOG 0
#define DEV_PAUSE 1
#define DEV_IMODE_PAUSE 1
#define DEV_DI 1
#define DEV_DI_TIMER 0
#define DEV_DI_EVENT 0
#define DEV_MVOP_FORCE_IRQ 0
#define DEV_CAPMODE 1
#define DEV_NEW_NEXT 1
#define TEMP_LOG 0
#define TEMP_MAIN_CAPTURE 1


static MS_S32 XC_Vsync_Event_id = -1;
static MS_S32  s32DMSTaskID = -1;

#ifndef DMS_DIP_Support_3DDI
#define DMS_DIP_Support_3DDI 0
#endif

#ifndef DMS_DIP_NWindow_Support_2P5DDI
#define DMS_DIP_NWindow_Support_2P5DDI 0
#endif

#if DEV_CAPMODE
static MS_S32 CaptureMode_Event_id = -1;
static MS_S32 s32DIPTaskID_CaptureMode = -1;
#endif

#ifndef DMS_DIP_Support_IMI
#define DMS_DIP_Support_IMI 0
#endif

#if DMS_DIP_Support_IMI
static MS_S32 StartFRC_Event_id = -1;
#endif

#ifndef DMS_DIPWindow_NORMAL
#define DMS_DIPWindow_NORMAL 0
#endif

#ifndef DMS_DIPWindow_DI
#define DMS_DIPWindow_DI 0
#endif

#ifndef DMS_DIPWindow_CAPTURE
#define DMS_DIPWindow_CAPTURE 0
#endif

#ifndef DMS_MFDEC_CAPTURE
#define DMS_MFDEC_CAPTURE 0
#endif

#ifndef DMS_MFDEC_NORMAL
#define DMS_MFDEC_NORMAL 0
#endif

#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
static MS_S32  s32MVOP_Event_id = -1;
static MS_S32  s32MVOPTaskID = -1;
static MS_S32  s32DisableBob_Event_id = -1;
static MS_S32  s32DisableBobTaskID = -1;
#endif

#define TRACE_LOG 0
#if TRACE_LOG
#define print_trace(_fmt, _args...) DMS_PRINTF(_fmt, ##_args)
#else
#define print_trace(_fmt, _args...)
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#define USE_2ND_BUFFER 0x1
#define USE_XC_SCALING_DOWN 0x2
#define USE_XC_FBL 0x4

#define DMS_XC_RESET_WINDOW_SIZE        0x1
#define DMS_XC_BLACK_SCREEN_BY_WINUSED  0x2
#define DMS_XC_FREEZE                   0x4
#define DMS_XC_FREEZE_SETTING           0x8
#define DMS_XC_FIRST_TIME_SETWINDOW     0x10    //For do utopia setwin and DS wrtie scenario
#define DMS_XC_NO_FRAME                 0x100
#define DMS_XC_SEQ_CHANGE_PI            0x200
#define DMS_XC_SEQ_CHANGE_SIZE          0x400
#define DMS_XC_UPDATE_FROM_VDEC         0X1000
#define DMS_XC_FIRST_FRAME_FROM_VDEC    0X2000

#if DBG_FPS
static MS_S32  s32DMSFPSTaskID = -1;
#define PRI_PERIOD 5000
static MS_BOOL bFPS_Start[2] = {FALSE};
static MS_U32 u32FPS_Push_Counter[2] = {0};
static MS_U32 u32FPS_Pull_Counter[2] = {0};
static MS_U32 u32FPS_Pull_NotReady_Counter[2] = {0};
static MS_U32 u32FPS_Switch_Counter[2] = {0};
static MS_U32 u32FPS_Show_Counter[2] = {0};
static MS_U32 u32FPS_Add_Counter[2] = {0};
static MS_U32 u32FPS_Release_Counter[2] = {0};
static MS_U32 u32FPS_Di_Counter[2] = {0};
#endif
MS_U16 gDMS_externDebugFlag = 0;
MS_U16 gDMSMVOPForceLookVDEC = 0;
#define sync_print(fmt, args...) do { if(0) DMS_PRINTF(fmt, ##args);  } while (0)
MS_U16 gDMSDebug = 0;
void* pTaskInstance = NULL;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
#if DEV_IMODE_PAUSE
static MS_S32 s32DIPDiRePushTimerId = -1;
#endif
#endif

//Marco to simplize resource usage
#define DMS_GET_RES_PRI DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;\
                        UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri))

#define RES_DMS pDMSResPri->stDrvDMS
#define RES_DMSDI pDMSResPri->stDrvDMSDi
#define RES_DMSCAPTURE pDMSResPri->stDrvDMSCapMode

#define DBG_DISABLE_SEMAPHORE 0
static MS_BOOL _DMS_GetNextInfo(MS_U16 u16WindowID, MS_BOOL bResetCaptureID);

MS_U32 MDrv_DMS_Get_Semaphore(void* pInstance, EN_DMS_POOL_TYPE ePoolType)
{
#if DBG_DISABLE_SEMAPHORE
    static MS_BOOL bGetRes = FALSE;
    if(bGetRes)
    {
        return UTOPIA_STATUS_SUCCESS;
    }
#endif
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    //DMS_PRINTF("get SHM!!\n");
    if (pInstance == NULL)
    {
        DMS_PRINTF("[%s,%5d]No instance existed, please get an instance by calling MApi_DMS_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if(UtopiaResourceObtain(pModule, E_DMS_POOL_ID_INTERNAL_VARIABLE, &g_pDMSRes) != UTOPIA_STATUS_SUCCESS)
    {
        DMS_PRINTF("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    u32Return = UTOPIA_STATUS_SUCCESS;
#if DBG_DISABLE_SEMAPHORE
    printf("\033[1;31m Disable semaphore get!!!!!!!!!!!!!!!!\033[0m\n");
    bGetRes = TRUE;
#endif
    return u32Return;
}

MS_U32 MDrv_DMS_Release_Semaphore(void* pInstance, EN_DMS_POOL_TYPE ePoolType)
{
#if DBG_DISABLE_SEMAPHORE
    return UTOPIA_STATUS_SUCCESS;
#endif
    //DMS_PRINTF("release SHM!!\n");
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    u32Return = UtopiaResourceRelease(g_pDMSRes);
    return u32Return;
}

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
static void _DMS_Add_XC_Status(MS_U32 u32Window, MS_U32 u32Status)
{
    DMS_GET_RES_PRI;
    RES_DMS.gu8DMS_SetXC_Status[u32Window] |= (u32Status);
}

static void _DMS_Remove_XC_Status(MS_U32 u32Window, MS_U32 u32Status)
{
    DMS_GET_RES_PRI;
    RES_DMS.gu8DMS_SetXC_Status[u32Window] &= ~(u32Status);
}

static MS_BOOL _DMS_Query_XC_Status(MS_U32 u32Window, MS_U32 u32Status)
{
    DMS_GET_RES_PRI;
    return (RES_DMS.gu8DMS_SetXC_Status[u32Window] & (u32Status))? TRUE : FALSE;
}

static MS_BOOL _DMS_Is_MVOPFlow(MS_U32 u32Window)
{
    return ((DMS_MVOP_FLOW) && (u32Window == MVOP_WINDOW));
}

static MS_BOOL _DMS_Is_SourceInterlace(E_DMS_VIDEO_SCAN_TYPE eScanType)
{
    if((eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME) || (eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_Is_SourceFieldBaseInterlace(E_DMS_VIDEO_SCAN_TYPE eScanType)
{
    if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_Is_SourceFrameBaseInterlace(E_DMS_VIDEO_SCAN_TYPE eScanType, E_DMS_VIDEO_FIELD_TYPE eFieldType)
{
    if((eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME) && (eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTH))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_Is_CaptureMode_OutputTile(EN_DMS_DIP_TILE_BLOCK eTileBlock)
{
    if(eTileBlock != DMS_DIP_TILE_BLOCK_W_16_32)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_Is_ScalingConditionUse2ndBuffer(MS_U32 u32ScalingCondition)
{
    if(u32ScalingCondition & USE_2ND_BUFFER)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_Is_ScalingConditionUseFBL(MS_U32 u32ScalingCondition)
{
    if(u32ScalingCondition & USE_XC_FBL)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_DIP_GetFrameBaseInterlaceFieldType(DMS_VDECFRAME_INFO *pVDECFrameInfo)
{
    if(pVDECFrameInfo->bIs2ndField == FALSE)
    {
        if(pVDECFrameInfo->eFieldOrderType == E_DMS_VIDEO_FIELD_ORDER_TYPE_TOP)
        {
            pVDECFrameInfo->eFieldType = E_DMS_VIDEO_FIELD_TYPE_TOP;
        }
        else
        {
            pVDECFrameInfo->eFieldType = E_DMS_VIDEO_FIELD_TYPE_BOTTOM;
        }
    }
    else
    {
        if(pVDECFrameInfo->eFieldOrderType == E_DMS_VIDEO_FIELD_ORDER_TYPE_TOP)
        {
            pVDECFrameInfo->eFieldType = E_DMS_VIDEO_FIELD_TYPE_BOTTOM;
        }
        else
        {
            pVDECFrameInfo->eFieldType = E_DMS_VIDEO_FIELD_TYPE_TOP;
        }
    }

    return TRUE;
}

static MS_BOOL _DMS_Is_UVSwap(EN_DMS_CAPTURE_COLORFORMAT eColorFormat)
{
    if((eColorFormat == E_DMS_CAPTURE_COLOR_FORMAT_YVYU) || (eColorFormat == E_DMS_CAPTURE_COLOR_FORMAT_VYUY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _DMS_Is_YCSwap(EN_DMS_CAPTURE_COLORFORMAT eColorFormat)
{
    if((eColorFormat == E_DMS_CAPTURE_COLOR_FORMAT_UYVY) || (eColorFormat == E_DMS_CAPTURE_COLOR_FORMAT_VYUY))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#define DMS_FHD_MAX_H_SIZE 1920
#define DMS_FHD_MAX_V_SIZE 1088
static MS_BOOL _DMS_WindowSizeOverFHD(MS_U32 u32Width, MS_U32 u32Height)
{
    if((u32Width > DMS_FHD_MAX_H_SIZE) && (u32Height > DMS_FHD_MAX_V_SIZE))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static EN_DIP_DI_FIELD _DMS_DIP_GetDIPDiDataFMT(MS_U32 u32Window, E_DMS_VIDEO_SCAN_TYPE eScanType, E_DMS_VIDEO_FIELD_TYPE eFieldType)
{
    EN_DIP_DI_FIELD eDIField = E_DIP_3DDI_TOP_BOTH_FIELD_STAGGER;
#if DMS_DIP_Support_3DDI
    if(u32Window == MVOP_WINDOW)
    {
        if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME)
        {
            if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_TOP)
            {
                eDIField = E_DIP_3DDI_TOP_BOTH_FIELD_STAGGER;
            }
            else if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTTOM)
            {
                eDIField = E_DIP_3DDI_BOTTOM_BOTH_FIELD_STAGGER;
            }
        }
        else if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
        {
            if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_TOP)
            {
                eDIField = E_DIP_3DDI_TOP_SINGLE_FIELD;
            }
            else if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTTOM)
            {
                eDIField = E_DIP_3DDI_BOTTOM_SINGLE_FIELD;
            }
        }
        return eDIField;
    }
#else
    if(u32Window == MVOP_WINDOW)
    {
        if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME)
        {
            if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_TOP)
            {
                eDIField = E_DIP_MED_DI_TOP_BOTH_FIELD_STAGGER;
            }
            else if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTTOM)
            {
                eDIField = E_DIP_MED_DI_BOTTOM_BOTH_FIELD_STAGGER;
            }
        }
        else if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
        {
            if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_TOP)
            {
                eDIField = E_DIP_MED_DI_TOP_BOTH_FIELD_SEPARATE;
            }
            else if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTTOM)
            {
                eDIField = E_DIP_MED_DI_BOTTOM_BOTH_FIELD_SEPARATE;
            }
        }
        return eDIField;
    }
#endif
#if DMS_DIP_NWindow_Support_2P5DDI
    else
    {
        if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME)
        {
            if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_TOP)
            {
                eDIField = E_DIP_MED_DI_TOP_BOTH_FIELD_STAGGER;
            }
            else if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTTOM)
            {
                eDIField = E_DIP_MED_DI_BOTTOM_BOTH_FIELD_STAGGER;
            }
        }
        else if(eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
        {
            if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_TOP)
            {
                eDIField = E_DIP_MED_DI_TOP_BOTH_FIELD_SEPARATE;
            }
            else if(eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTTOM)
            {
                eDIField = E_DIP_MED_DI_BOTTOM_BOTH_FIELD_SEPARATE;
            }
        }
        return eDIField;
    }
#endif
    printf("\033[31m [%s][%d] CHECK CODE FLOW, SHOULD NOT REACH HERE !!\033[0m\n",__FUNCTION__,__LINE__);
    return eDIField;
}

static EN_XC_DIP_DATA_FMT _DMS_DIP_GetDIPRDataFMT(DMS_VDECFRAME_INFO *pVDECFrameInfo)
{
    EN_XC_DIP_DATA_FMT eFmt = E_DMS_VIDEO_CODEC_UNKNOWN;

    switch (pVDECFrameInfo->eCODEC)
    {
        case E_DMS_VIDEO_CODEC_HEVC:
            eFmt = DIP_DATA_FMT_YUV420_H265;
            break;
        case E_DMS_VIDEO_CODEC_H264:
        default :
            eFmt = DIP_DATA_FMT_YUV420;
            break;
    }
    return eFmt;
}

static MS_U16 _DMS_DIP_CalcPitch(EN_XC_DIP_DATA_FMT eFmt, MS_U16 u16Width)
{
    MS_U16 u16Pitch = 0;
    switch (eFmt)
    {
        case DIP_DATA_FMT_YUV420:
        case DIP_DATA_FMT_YUV420_H265:
        case DIP_DATA_FMT_YUV420_MED_DI_FIELDTYPE_TOP:
        case DIP_DATA_FMT_YUV420_MED_DI_FIELDTYPE_BOTTOM:
            u16Pitch = u16Width * 3 / 2; //*1.5
            break;
        case DIP_DATA_FMT_RGB565:
        case DIP_DATA_FMT_YUV422:
        case DIP_DATA_FMT_YUV422_MED_DI_FIELDTYPE_TOP:
        case DIP_DATA_FMT_YUV422_MED_DI_FIELDTYPE_BOTTOM:
            u16Pitch = u16Width << 1;
            break;
        case DIP_DATA_FMT_ARGB8888:
            u16Pitch = u16Width << 2;
            break;
        default :
            u16Pitch = 0;
            break;
    }
    return u16Pitch;
}

static EN_MFDEC_TILE_MODE _DMS_DIP_MFDECTileTypeMapping(E_DMS_VIDEO_TILE_MODE eTileMode)
{
    EN_MFDEC_TILE_MODE eTile;
    switch (eTileMode)
    {
        case E_DMS_VIDEO_TILE_MODE_16x32:
            eTile = E_DIP_MFEDC_TILE_16_32;
            break;
        case E_DMS_VIDEO_TILE_MODE_32x16:
            eTile = E_DIP_MFEDC_TILE_32_16;
            break;
        case E_DMS_VIDEO_TILE_MODE_32x32:
            eTile = E_DIP_MFEDC_TILE_32_32;
            break;
        default :
            eTile = E_DIP_MFEDC_TILE_16_32;
            break;
    }
    return eTile;
}

static EN_XC_DIP_TILE_BLOCK _DMS_DIP_TileTypeMapping(E_DMS_VIDEO_TILE_MODE eTileMode)
{
    EN_XC_DIP_TILE_BLOCK eTile;
    switch (eTileMode)
    {
        case E_DMS_VIDEO_TILE_MODE_16x32:
            eTile = DIP_TILE_BLOCK_R_16_32;
            break;
        case E_DMS_VIDEO_TILE_MODE_32x16:
            eTile = DIP_TILE_BLOCK_R_32_16;
            break;
        case E_DMS_VIDEO_TILE_MODE_32x32:
            eTile = DIP_TILE_BLOCK_R_32_32;
            break;
        default :
            eTile = DIP_TILE_BLOCK_R_16_32;
            break;
    }

    return eTile;
}

static EN_XC_DIP_TILE_BLOCK _DMS_DIP_Capture_TileMapping(EN_DMS_DIP_TILE_BLOCK eTileMode)
{
    EN_XC_DIP_TILE_BLOCK eTile;
    switch (eTileMode)
    {
        case DMS_DIP_TILE_BLOCK_R_NONE:
            eTile = DIP_TILE_BLOCK_R_NONE;
            break;
        case DMS_DIP_TILE_BLOCK_W_NONE:
            eTile = DIP_TILE_BLOCK_W_NONE;
            break;
        case DMS_DIP_TILE_BLOCK_R_16_32:
            eTile = DIP_TILE_BLOCK_R_16_32;
            break;
        case DMS_DIP_TILE_BLOCK_W_16_32:
            eTile = DIP_TILE_BLOCK_W_16_32;
            break;
        case DMS_DIP_TILE_BLOCK_R_32_16:
            eTile = DIP_TILE_BLOCK_R_32_16;
            break;
        case DMS_DIP_TILE_BLOCK_W_32_16:
            eTile = DIP_TILE_BLOCK_W_32_16;
            break;
        case DMS_DIP_TILE_BLOCK_R_32_32:
            eTile = DIP_TILE_BLOCK_R_32_32;
            break;
        case DMS_DIP_TILE_BLOCK_W_32_32:
            eTile = DIP_TILE_BLOCK_W_32_32;
            break;
        default :
            eTile = DIP_TILE_BLOCK_R_16_32;
            break;
    }

    return eTile;
}

static EN_XC_DIP_DATA_FMT _DMS_DIP_Capture_ColorFmtMapping(EN_DMS_DIP_FMT eColorFmt)
{
    EN_XC_DIP_DATA_FMT eFmt;
    switch (eColorFmt)
    {
        case E_DMS_DIP_FMT_YUV422:
            eFmt = DIP_DATA_FMT_YUV422;
            break;
        case E_DMS_DIP_FMT_RGB565:
            eFmt = DIP_DATA_FMT_RGB565;
            break;
        case E_DMS_DIP_FMT_ARGB8888:
            eFmt = DIP_DATA_FMT_ARGB8888;
            break;
        case E_DMS_DIP_FMT_YUV420:
            eFmt = DIP_DATA_FMT_YUV420;
            break;
        case E_DMS_DIP_FMT_YC422:
            eFmt = DIP_DATA_FMT_YC422;
            break;
        case E_DMS_DIP_FMT_YUV420_H265:
            eFmt = DIP_DATA_FMT_YUV420_H265;
            break;
        case E_DMS_DIP_FMT_YUV420_H265_10BITS:
            eFmt = DIP_DATA_FMT_YUV420_H265_10BITS;
            break;
        case E_DMS_DIP_FMT_YUV420_PLANER:
            eFmt = DIP_DATA_FMT_YUV420_PLANER;
            break;
        case E_DMS_DIP_FMT_YUV420_SEMI_PLANER:
            eFmt = DIP_DATA_FMT_YUV420_SEMI_PLANER;
            break;
        case E_DMS_DIP_FMT_YUV422_MED_DI_FIELDTYPE_TOP:
            eFmt = DIP_DATA_FMT_YUV422_MED_DI_FIELDTYPE_TOP;
            break;
        case E_DMS_DIP_FMT_YUV422_MED_DI_FIELDTYPE_BOTTOM:
            eFmt = DIP_DATA_FMT_YUV422_MED_DI_FIELDTYPE_BOTTOM;
            break;
        case E_DMS_DIP_FMT_YUV420_MED_DI_FIELDTYPE_TOP:
            eFmt = DIP_DATA_FMT_YUV420_MED_DI_FIELDTYPE_TOP;
            break;
        case E_DMS_DIP_FMT_YUV420_MED_DI_FIELDTYPE_BOTTOM:
            eFmt = DIP_DATA_FMT_YUV420_MED_DI_FIELDTYPE_BOTTOM;
            break;
        default :
            eFmt = DIP_DATA_FMT_MAX;
            break;
    }

    return eFmt;
}

static void _DMS_XC_SetCusScaling(XC_SETWIN_INFO* pXCSetWinInfo, MS_BOOL bPostScaling,MS_U16 u16Hsrc, MS_U16 u16Hdest, MS_U16 u16Vsrc, MS_U16 u16Vdest)
{
    if(pXCSetWinInfo != NULL)
    {
        if(bPostScaling)
        {
            pXCSetWinInfo->u16HCusScalingSrc = u16Hsrc;
            pXCSetWinInfo->u16HCusScalingDst = u16Hdest;
            pXCSetWinInfo->u16VCusScalingSrc = u16Vsrc;
            pXCSetWinInfo->u16VCusScalingDst = u16Vdest;
        }
        else
        {
            pXCSetWinInfo->u16PreHCusScalingSrc = u16Hsrc;
            pXCSetWinInfo->u16PreHCusScalingDst = u16Hdest;
            pXCSetWinInfo->u16PreVCusScalingSrc = u16Vsrc;
            pXCSetWinInfo->u16PreVCusScalingDst = u16Vdest;
        }
    }
}

static void _DMS_XC_WindowAlignto2(ST_DMS_WINDOW *pstDMSWindow)
{
    pstDMSWindow->u32x += (pstDMSWindow->u32x % 2);
    pstDMSWindow->u32y += (pstDMSWindow->u32y % 2);
    pstDMSWindow->u32width += (pstDMSWindow->u32width % 2);
    pstDMSWindow->u32height += (pstDMSWindow->u32height % 2);
}

static void _DMS_XC_DMSWintoXCWin(MS_WINDOW_TYPE *pstXCWindow, ST_DMS_WINDOW *pstDMSWindow)
{
    pstXCWindow->x = (pstDMSWindow->u32x);
    pstXCWindow->y = (pstDMSWindow->u32y);
    pstXCWindow->width = (pstDMSWindow->u32width);
    pstXCWindow->height = (pstDMSWindow->u32height);
}

static MS_U16 _GetPreviousBufferID(MS_U16 u16CurID)
{
    if (u16CurID == 0)
    {
        return (DMS_DIP_QUEUEDEPTH - 1);
    }
    else
    {
        return (u16CurID - 1);
    }
}

static MS_U16 _GetNextBufferID(MS_U16 u16CurID)
{
    return ((u16CurID + 1) % DMS_DIP_QUEUEDEPTH);
}

static MS_BOOL _IsFrameRefCountZero(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
    if((RES_DMS._stXC_LocalFrameRefCount[u32Window][u16BufID] == 0)
        && (RES_DMSCAPTURE._stXC_LocalFrameRefCountCaptureMode[u32Window][u16BufID] == 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



E_DMS_VIDEO_SCAN_TYPE _DMS_GetDMSScanType(MS_U8 u8ScanType)
{
    // ProgressiveFrame = 0,   /** Picture is a progressive frame */
    // InterlacedFrame  = 2,    /** Picture is an interlaced frame */
    // InterlacedField  = 3,      /** Picture is two interlaced fields */
    // InterlacedFrame  = 1,      MVD bring 1 to push interlace frame
    E_DMS_VIDEO_SCAN_TYPE eScanType = E_DMS_VIDEO_SCAN_TYPE_MAX;
    switch (u8ScanType)
    {
        case 0:
            eScanType = E_DMS_VIDEO_SCAN_TYPE_PROGRESSIVE;
            break;
        case 1:
        case 2:
            eScanType = E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FRAME;
            break;
        case 3:
            eScanType = E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD;
            break;
        default:
            eScanType = E_DMS_VIDEO_SCAN_TYPE_MAX;
            break;
    }
    return eScanType;
}

#if DEV_DI
static MS_BOOL _DMS_DIP_Fire(E_DMS_DIP_ACTION eAction);
static MS_BOOL _DMS_DIP_WaitWriteDone(E_DMS_DIP_ACTION eAction);
static MS_BOOL _DMS_SetDIPRWindow(E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID);
static MS_BOOL _DMS_SetDIPWWindow(E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID);
static MS_BOOL _DMS_SetDIPWindow(E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID);
static void _DMS_StoreVDECInfo2DIPQueue(void* pInstance, DMS_VDECFRAME_INFO *VDECDispFrame);
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static void _DMS_DIP_SyncStoredSetting(void);
#endif
static MS_S32 XC_Di_Push_Event_id = -1;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_S32  s32DIPTaskID_Interlace = -1;
#endif
#define di_sync_print(fmt, args...) do { if(0) DMS_PRINTF(fmt, ##args);  } while (0)
MS_PHY tempAddr = 0x0;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static void _DIBufferRingToNext(void)
{
    DMS_GET_RES_PRI;
    RES_DMSDI.u8DIRingBufferID++;
    if(RES_DMSDI.u8DIRingBufferID == DMS_DI_RING_BUFFER)
    {
        RES_DMSDI.u8DIRingBufferID = 0;
    }
}
#endif
static MS_PHY _GetDIRingBufferAddr(MS_U32 u32Window, MS_U8 u8RingBufID)
{
    DMS_GET_RES_PRI;
    if(u8RingBufID >= DMS_DI_RING_BUFFER)
    {
        DMS_PRINTF("_GetDIRingBufferAddr error, %d\n",u8RingBufID);
    }
    return RES_DMS._stXC_DIPWinInfo[u32Window].phyDIPRingBufMemAddr[u8RingBufID];
}
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_BOOL _DMS_VDEC_Frame_AddRef_Interlace(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
#if NEW_FLIP
    //DMS_PRINTF("[AddRef] bid[0x%x], ct[0x%x]\n",u16BufID,RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID]);
    VDEC_StreamId VdecStreamId;
    VDEC_EX_DispFrame VdecDispFrm;
    VdecStreamId.u32Version = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamVersion;
    VdecStreamId.u32Id = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamID;
    VdecDispFrm.u32Idx = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex;
    VdecDispFrm.u32PriData = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32PriData;
    VDEC_EX_Result eResult;
    eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
    if (eResult != E_VDEC_EX_OK)
    {
        DMS_PRINTF("[%s] %d %d, %d %d\n",__FUNCTION__,VdecStreamId.u32Version,VdecStreamId.u32Id,
            VdecDispFrm.u32Idx,VdecDispFrm.u32PriData);
        DMS_PRINTF("[%s] window %d, BufID %d, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__,u32Window,u16BufID,eResult);
        return FALSE;
    }
    if(_DMS_Is_SourceFieldBaseInterlace(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].eScanType))
    {
        VdecStreamId.u32Version = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex_2nd;
        VdecDispFrm.u32PriData = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32PriData_2nd;
        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %d %d, %d %d\n",__FUNCTION__,VdecStreamId.u32Version,VdecStreamId.u32Id,
                VdecDispFrm.u32Idx,VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %d, BufID %d, 2nd Field, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__,u32Window,u16BufID,eResult);
            return FALSE;
        }
    }
    //DMS_PRINTF("[AddRef] Frame[0x%x], PriData[0x%x]\n",RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex,RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32PriData);
    RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID]++;

#if DBG_FPS
    u32FPS_Add_Counter[u32Window]++;
#endif
#endif
    return TRUE;
}
#endif
static MS_BOOL _DMS_VDEC_Frame_Release_Interlace(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
#if NEW_FLIP
    //DMS_PRINTF("[Releas] bid[0x%x], ct[0x%x]\n",u16BufID,RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID]);

    if(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex == DMS_INVALID_FRAME_ID)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }
    if(RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID] == 0)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer (release a ref 0 frame)\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }
    RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID]--;
    if(RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID] == 0)
    {
        VDEC_StreamId VdecStreamId;
        VDEC_EX_DispFrame VdecDispFrm;
        VdecStreamId.u32Version = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex;
        VdecDispFrm.u32PriData = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32PriData;

        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %d, BufID %td, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
            return FALSE;
        }
        if(_DMS_Is_SourceFieldBaseInterlace(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].eScanType))
        {
            VdecStreamId.u32Version = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamVersion;
            VdecStreamId.u32Id = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32VDECStreamID;
            VdecDispFrm.u32Idx = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex_2nd;
            VdecDispFrm.u32PriData = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32PriData_2nd;

            VDEC_EX_Result eResult;
            eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
            if (eResult != E_VDEC_EX_OK)
            {
                DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                    (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
                DMS_PRINTF("[%s] window %td, BufID %d, 2nd Field, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
                return FALSE;
            }
        }
        //DMS_PRINTF("[Releas] Frame[0x%x], PriData[0x%x]\n",_stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex,_stXC_DIPDIInfo[u32Window][u16BufID].u32PriData);
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufID].u32FrameIndex = DMS_INVALID_FRAME_ID;
    }

#if DBG_FPS
    u32FPS_Release_Counter[u32Window]++;
#endif
#endif
    return TRUE;
}

static void _DMS_Clear_VDEC_Frame_Ref_Interlace(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8RefCounter = 0;
    for(u8RefCounter = 0; u8RefCounter < RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16BufID]; u8RefCounter++)
    {
        DMS_PRINTF("[%s] window %td release frame for WP %d ct %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,(u8RefCounter+1));
        _DMS_VDEC_Frame_Release_Interlace(u32Window,u16BufID);
    }
}

MS_BOOL _MDrv_DMS_DI_Init(void)
{
    DMS_GET_RES_PRI;
    MS_U32 u32DIPDISize = (4/2) * DMS_FHD_Hszie * DMS_FHD_Vszie * 4;
    ST_XC_DIPR_PROPERTY_EX DIPRWinPropertytoShow_EX;
    ST_DIP_DI_SETTING DIP3DDISetting;
    memset(&DIPRWinPropertytoShow_EX,0,sizeof(ST_XC_DIPR_PROPERTY_EX));
    memset(&DIP3DDISetting,0,sizeof(ST_DIP_DI_SETTING));
    ST_DIP_DI_NWAY_BUF_INFO stDINwayBuf[32];

    DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width = 1920;
    DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = 1080;
    DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch = 1920;
    DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr = _GetDIRingBufferAddr(MVOP_WINDOW, RES_DMSDI.u8DIRingBufferID);
    DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr = DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr + u32DIPDISize;
    DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt = DIP_DATA_FMT_YUV420;
    DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bMFDec_Enable = FALSE;

    DIP3DDISetting.bEnableDI = TRUE;
    DIP3DDISetting.bInitDI = TRUE;
    DIP3DDISetting.stInitInfo.phyDIInitAddr = RES_DMSDI.u32DIPDIAddress_pa;
    DIP3DDISetting.stInitInfo.u32DIInitSize = 0x23456;

    DIP3DDISetting.u8NwayIndex = 0;
    DIP3DDISetting.enDI_Field = E_DIP_3DDI_TOP_BOTH_FIELD_STAGGER;
    DIP3DDISetting.bEnaNwayInfoRefresh = TRUE;
    DIP3DDISetting.u8NwayBufCnt = 1;
    DIP3DDISetting.bEnableBob = TRUE;

    stDINwayBuf[0].phyDI_BufAddr = RES_DMSDI.u32DIPDIAddress_pa;
    stDINwayBuf[0].u32DI_BufSize = u32DIPDISize;
    stDINwayBuf[0].u8DIBufCnt = 4;

    DIP3DDISetting.stDINwayBufInfo = (void*)stDINwayBuf;

    DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32version=1;
    DIPRWinPropertytoShow_EX.stDIPR3DDISetting.pDIPR3DDISetting = (void*)&DIP3DDISetting;

    //weak symbol test
    if(MApi_XC_DIP_SetDIPRProperty_EX)
    {
        MApi_XC_DIP_SetDIPRProperty_EX(&DIPRWinPropertytoShow_EX, DIP_WINDOW);
    }
    else
    {
        MApi_XC_DIP_SetDIPRProperty(&DIPRWinPropertytoShow_EX.stDIPRProperty, DIP_WINDOW);
    }

    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DIStartAddr = RES_DMSDI.u32DIPDIAddress_pa;
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DIEndAddr= RES_DMSDI.u32DIPDIAddress_pa + u32DIPDISize;
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DISize = u32DIPDISize;
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u8DIBufCnt = 4; // 4 DI temp buffer

    return TRUE;
}
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_U16 _GetNextWritePointerInterlace(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    return _GetNextBufferID(RES_DMSDI._stXC_DIPWriteReadPointerInterlace[u32Window].u16WritePointer);
}

#if DEV_IMODE_PAUSE
static void _DIPDi_RePush_Handler(MS_U32 stTimer, MS_U32 u32TimerID)
{
    void *pInstance = pTaskInstance;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    if(RES_DMSDI._bSetwinToDiTask_DoubleBuffer)
    {
#if DEV_DI_EVENT
        MsOS_SetEvent(XC_Di_Push_Event_id, 0x01);
#else
        RES_DMSDI.bFlipToDiTask = TRUE;
#endif
        printf("[%s] trigger Flip PushedInterlace!\n",__FUNCTION__);
    }
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
}
#endif
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_BOOL _Task_CapturePushedInterlaceFrame(void)
{
    void *pInstance = pTaskInstance;
    MS_U8 u8WindowID = 0;
#if DEV_DI_EVENT
    MS_U32 u32WaitEventFlag = BIT(0);
    MS_U32 u32Events = 0;
#endif
    MS_U8  i = 0;
    MS_U8  u8FrameIdx = 0;
    MS_U8  u8FrameNum = 0;
    MS_U8  u8FieldNum = 0;
#if DEV_IMODE_PAUSE
    MS_BOOL  s32DIPDiRePushTimerFlag = FALSE;
#endif

    // coverity[no_escape]
    while(1)
    {
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        DMS_GET_RES_PRI;

        MS_U16 u16CheckDiBufferID = RES_DMSDI._u8DMS_DiSetEventBufferID;
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
#if DEV_IMODE_PAUSE
        if(u16CheckDiBufferID == DMS_Invalid_ReadPointer_ID)
        {
            if (s32DIPDiRePushTimerId >= 0)
            {
                MsOS_DeleteTimer(s32DIPDiRePushTimerId);
                s32DIPDiRePushTimerId = -1;
            }
            continue;
        }
        else
        {
            MS_U32 u32InputFrameRate = 100;//1000000/(_stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32FrameRate*2);
            if (s32DIPDiRePushTimerId < 0)
            {
                //printf("[%s][%d] Create timer with period 100ms !!\n", __FUNCTION__, __LINE__,u32InputFrameRate,_stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32FrameRate);
                s32DIPDiRePushTimerId = MsOS_CreateTimer(_DIPDi_RePush_Handler,
                                                    u32InputFrameRate,
                                                    u32InputFrameRate,
                                                    TRUE,
                                                     "DIPDiRePushTimer");
                s32DIPDiRePushTimerFlag = TRUE;
            }
            else if(s32DIPDiRePushTimerFlag == FALSE)
            {
                s32DIPDiRePushTimerFlag = TRUE;
                MsOS_StartTimer(s32DIPDiRePushTimerId);
            }
        }
#endif

#if DEV_DI_EVENT
        MsOS_WaitEvent(XC_Di_Push_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
#else
        MsOS_DelayTask(5);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        if(RES_DMSDI.bFlipToDiTask)
        {
            RES_DMSDI.bFlipToDiTask = FALSE;
        }
        else
        {
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
            continue;
        }
#endif

#if DEV_IMODE_PAUSE
        if (s32DIPDiRePushTimerFlag == TRUE)
        {
            MsOS_StopTimer(s32DIPDiRePushTimerId);
            s32DIPDiRePushTimerFlag = FALSE;
        }
#endif
        MS_U16 u16NextReadPointer = RES_DMSDI._u8DMS_DiSetEventBufferID;
        if(u16NextReadPointer == DMS_Invalid_ReadPointer_ID)
        {
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
            continue;
        }

        for(u8WindowID=0; u8WindowID < 1; u8WindowID++)
        {
            //Get Next frame info
            u8FrameNum = (RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)? 2 : 1;
            for(u8FrameIdx=0; u8FrameIdx<u8FrameNum; u8FrameIdx++)
            {
                u8FieldNum = (RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTH)? 2 : 1;
                _DMS_DIP_SyncStoredSetting();
                for(i = 0 ; i < u8FieldNum; i ++)
                {
                    MS_BOOL bSetwinToDiTask = RES_DMSDI._bSetwinToDiTask_DoubleBuffer;
                    RES_DMSDI.u8DIPReadytoCaptureIDInterlace[u8WindowID] = u16NextReadPointer;

                    if(gDMSDebug == 1)
                        gDMSDebug = 2;
                    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

                    while(MApi_XC_DIP_GetResource(DIP_WINDOW) == E_APIXC_RET_FAIL)
                    {
                        MsOS_DelayTask(1);
                    }

                    //Disable BK1336 reg3[8]
                    MApi_XC_W2BYTEMSK(REG_SC_BK36_03_L,0x0000,0x0100);
                    if(_DMS_Is_MVOPFlow(u8WindowID) == FALSE)
                    {
                        MApi_XC_DIP_SwapUV(TRUE ,DIP_WINDOW);
                    }
                    else
                    {
                        MApi_XC_DIP_SwapUV(FALSE ,DIP_WINDOW);
                    }
                    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);

                    _DIBufferRingToNext();

                    MApi_XC_DIP_SetOutputDataFmt(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].eDIPWFmt, DIP_WINDOW);
                    MApi_XC_DIP_EnableR2Y(FALSE,DIP_WINDOW);
                    MApi_XC_DIP_EnableY2R(FALSE,DIP_WINDOW);
                    // set DIP read/write window
                    _DMS_SetDIPWindow(E_DMS_DIP_DI, u8WindowID, u16NextReadPointer);
                    //di_sync_print("\033[1;35mDi+F %d: 0x%x, 0x%llx , MainQ %d \033[0m\n",MsOS_GetSystemTime(), RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32PriData,_GetDIRingBufferAddr(u8WindowID,RES_DMSDI.u8DIRingBufferID),RES_DMS._stXC_DIPWriteReadPointer[u8WindowID].u16WritePointer);
                    if(_GetDIRingBufferAddr(u8WindowID,RES_DMSDI.u8DIRingBufferID) == tempAddr)
                    {
                        DMS_PRINTF("\033[1;31mDI hit showing buffer!! Addr 0x%llx, MainQ %d\033[0m\n",tempAddr,RES_DMS._stXC_DIPWriteReadPointer[u8WindowID].u16WritePointer);
                    }

                    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
                    //printf("\033[1;31m[%d] Write Ring Buffer %d, DIP start \033[0m\n",MsOS_GetSystemTime(),RES_DMSDI.u8DIRingBufferID);
                    // enable DIP
                    //MApi_XC_W2BYTEMSK(0x3460,0x0000,0x0001);
                    //MApi_XC_W2BYTEMSK(0x34D8,0x0000,0x0600);
                    _DMS_DIP_Fire(E_DMS_DIP_DI);

                    // Wait DIP write done
                    _DMS_DIP_WaitWriteDone(E_DMS_DIP_DI);
                    //printf("\033[1;31m[%d] Write Ring Buffer %d, DIP end \033[0m\n",MsOS_GetSystemTime(),RES_DMSDI.u8DIRingBufferID);
                    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);

                    RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].bValid = FALSE;
                    if(gDMSDebug == 2)
                    {
                        DMS_PRINTF("\033[1;31m[%d] interlace Capture ID %d, Ring buffer ID %d\033[0m\n",MsOS_GetSystemTime(),RES_DMSDI.u8DIPReadytoCaptureIDInterlace[u8WindowID],RES_DMSDI.u8DIRingBufferID);
                        gDMSDebug++;
                    }

                    MApi_XC_DIP_Ena(FALSE,DIP_WINDOW);

                    MApi_XC_DIP_ReleaseResource(DIP_WINDOW);

                    if(RES_DMSDI._u8DMS_DiReleaseVDECFrameBufferID != DMS_Invalid_ReadPointer_ID &&
                        RES_DMSDI._u8DMS_DiReleaseVDECFrameBufferID != u16NextReadPointer)
                    {
                        _DMS_VDEC_Frame_Release_Interlace(u8WindowID,RES_DMSDI._u8DMS_DiReleaseVDECFrameBufferID);
                    }
                    RES_DMSDI._u8DMS_DiReleaseVDECFrameBufferID = u16NextReadPointer;

                    RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].bIsAfterDiTask = TRUE;
                    RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u8DiOutputRingBufferID = RES_DMSDI.u8DIRingBufferID;
                    RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].eFmt = E_DMS_VIDEO_DATA_FMT_YUV422;
                    MS_U32 tempFrameIndex = RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32FrameIndex;
                    RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32FrameIndex = DMS_INVALID_FRAME_ID;

                    _DMS_StoreVDECInfo2DIPQueue(pInstance, &RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer]);
                    RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32FrameIndex = tempFrameIndex;
                    di_sync_print("\033[1;35mDi W %d: 0x%x, 0x%llx , MainQ %d write done\033[0m\n",MsOS_GetSystemTime(),RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].u32PriData,_GetDIRingBufferAddr(u8WindowID,RES_DMSDI.u8DIRingBufferID),RES_DMS._stXC_DIPWriteReadPointer[u8WindowID].u16WritePointer);
                    if(i == 0 && bSetwinToDiTask)
                    {
                        _DMS_Add_XC_Status(u8WindowID, DMS_XC_RESET_WINDOW_SIZE);
                        RES_DMSDI._bSetwinToDiTask_DoubleBuffer = FALSE;
                    }

#if DBG_FPS
                    u32FPS_Di_Counter[u8WindowID]++;
#endif
                    if((RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTH) &&
                        (RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].bIs2ndField == FALSE))
                    {
                        MS_U16 u16NextID = _GetNextBufferID(u16NextReadPointer);
                        if(RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextID].bValid == TRUE)
                        {
                            u16NextReadPointer = u16NextID;
                        }
                        else
                        {
                            DMS_PRINTF("!!!!![%s][%d] Error!! can not get 2nd field when VIDEO_FIELD_TYPE_BOTH\n",__FUNCTION__,__LINE__);
                        }
                    }
                }
                if((RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD) &&
                    (RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextReadPointer].bIs2ndField == FALSE))
                {
                    MS_U16 u16NextID = _GetNextBufferID(u16NextReadPointer);
                    if(RES_DMSDI._stXC_DIPDIInfo[u8WindowID][u16NextID].bValid == TRUE)
                    {
                        u16NextReadPointer = u16NextID;
                    }
                    else
                    {
                        DMS_PRINTF("!!!!![%s][%d] Error!! can not get 2nd field when E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD\n",__FUNCTION__,__LINE__);
                    }
                }
            }
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        }
    }
    return TRUE;
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static void _DMS_StoreVDECInfo2DIPInterlaceQueue(DMS_VDECFRAME_INFO *VDECDispFrame)
{
    DMS_GET_RES_PRI;
    MS_U16 u16NextWritePointer = 0;
    MS_U32 u32Window = VDECDispFrame->u32Window;

    //Jump to Next Write Pointer
    u16NextWritePointer = _GetNextWritePointerInterlace(u32Window);
    //DMS_PRINTF("=====PUSH Frame=%d====",u16NextWritePointer);
#if NEW_FLIP
    if(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bValid == TRUE)
    {
        //VDEC push too fast
        DMS_PRINTF("[%s] release frame for WP %d ring back\n",__FUNCTION__,u16NextWritePointer);
        _DMS_Clear_VDEC_Frame_Ref_Interlace(u32Window,u16NextWritePointer);
    }
    if(RES_DMSDI._stXC_LocalFrameRefCountInterlace[u32Window][u16NextWritePointer] > 0)
    {
        DMS_PRINTF("[%s] LocalFrameRefCount check ring back\n",__FUNCTION__);
        _DMS_Clear_VDEC_Frame_Ref_Interlace(u32Window,u16NextWritePointer);
    }
#endif
    //DMS_PRINTF("[%s] push to Buffer %d\n",__FUNCTION__,u16NextWritePointer);

    if((VDECDispFrame->eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
        && (VDECDispFrame->bIs2ndField == TRUE))
    {
        // New Flip
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32FrameIndex = VDECDispFrame->u32FrameIndex_2nd;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32VDECStreamID = VDECDispFrame->u32VDECStreamID;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32VDECStreamVersion = VDECDispFrame->u32VDECStreamVersion;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32PriData = VDECDispFrame->u32PriData_2nd;
        _DMS_VDEC_Frame_AddRef_Interlace(u32Window,u16NextWritePointer);

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bValid = TRUE;

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16SrcWidth = VDECDispFrame->u16SrcWidth - VDECDispFrame->u16CropLeft - VDECDispFrame->u16CropRight;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16SrcHeight = VDECDispFrame->u16SrcHeight - VDECDispFrame->u16CropTop - VDECDispFrame->u16CropBottom;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16SrcPitch = VDECDispFrame->u16SrcPitch;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcLumaAddr = VDECDispFrame->u32SrcLumaAddrI;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcChromaAddr = VDECDispFrame->u32SrcChromaAddrI;

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32FrameRate = VDECDispFrame->u32FrameRate;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eFmt = VDECDispFrame->eFmt;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32Window = VDECDispFrame->u32Window;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eCODEC = VDECDispFrame->eCODEC;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eScanType = VDECDispFrame->eScanType;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eFieldOrderType = VDECDispFrame->eFieldOrderType;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eFieldType = VDECDispFrame->eFieldType_2nd;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eTileMode = VDECDispFrame->eTileMode;
        // 10 bits
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].b10bitData = VDECDispFrame->b10bitData;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16Src10bitPitch = VDECDispFrame->u16Src10bitPitch;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcLumaAddr_2bit = VDECDispFrame->u32SrcLumaAddrI_2bit;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcChromaAddr_2bit = VDECDispFrame->u32SrcChromaAddrI_2bit;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u8LumaBitdepth = VDECDispFrame->u8LumaBitdepth;

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bIsAfterDiTask = VDECDispFrame->bIsAfterDiTask;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bIs2ndField = VDECDispFrame->bIs2ndField;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u8DiOutputRingBufferID = VDECDispFrame->u8DiOutputRingBufferID;
    }
    else
    {
        // New Flip
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32FrameIndex = VDECDispFrame->u32FrameIndex;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32VDECStreamID = VDECDispFrame->u32VDECStreamID;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32VDECStreamVersion = VDECDispFrame->u32VDECStreamVersion;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32PriData = VDECDispFrame->u32PriData;
        _DMS_VDEC_Frame_AddRef_Interlace(u32Window,u16NextWritePointer);

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bValid= TRUE;

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16SrcWidth = VDECDispFrame->u16SrcWidth - VDECDispFrame->u16CropLeft - VDECDispFrame->u16CropRight;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16SrcHeight = VDECDispFrame->u16SrcHeight - VDECDispFrame->u16CropTop - VDECDispFrame->u16CropBottom;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16SrcPitch = VDECDispFrame->u16SrcPitch;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcLumaAddr = VDECDispFrame->u32SrcLumaAddr;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcChromaAddr = VDECDispFrame->u32SrcChromaAddr;

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32FrameRate = VDECDispFrame->u32FrameRate;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eFmt = VDECDispFrame->eFmt;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32Window = VDECDispFrame->u32Window;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eCODEC = VDECDispFrame->eCODEC;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eScanType = VDECDispFrame->eScanType;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eFieldOrderType = VDECDispFrame->eFieldOrderType;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eFieldType = VDECDispFrame->eFieldType;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].eTileMode = VDECDispFrame->eTileMode;
        // 10 bits
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].b10bitData = VDECDispFrame->b10bitData;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u16Src10bitPitch = VDECDispFrame->u16Src10bitPitch;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcLumaAddr_2bit = VDECDispFrame->u32SrcLumaAddr_2bit;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u32SrcChromaAddr_2bit = VDECDispFrame->u32SrcChromaAddr_2bit;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u8LumaBitdepth = VDECDispFrame->u8LumaBitdepth;

        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bIsAfterDiTask = VDECDispFrame->bIsAfterDiTask;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].bIs2ndField = VDECDispFrame->bIs2ndField;
        RES_DMSDI._stXC_DIPDIInfo[u32Window][u16NextWritePointer].u8DiOutputRingBufferID = VDECDispFrame->u8DiOutputRingBufferID;
    }
    RES_DMSDI._stXC_DIPWriteReadPointerInterlace[u32Window].u16WritePointer = u16NextWritePointer;
}
#endif
#endif //End of DEV_DI

static MS_BOOL _DMS_VDEC_Frame_AddRef(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
#if NEW_FLIP
    if(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex == DMS_INVALID_FRAME_ID)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Add an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }

    VDEC_StreamId VdecStreamId;
    VDEC_EX_DispFrame VdecDispFrm;
    VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
    VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
    VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex;
    VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData;
    VDEC_EX_Result eResult;
    eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
    if (eResult != E_VDEC_EX_OK)
    {
        DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
            (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
        DMS_PRINTF("[%s] window %td, BufID %d, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
        return FALSE;
    }
    if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].eScanType))
    {
        VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex_2nd;
        VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData_2nd;
        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %td, BufID %d, 2nd Field, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
            return FALSE;
        }
    }
    //DMS_PRINTF("[AddRef] Frame[0x%x], PriData[0x%x]\n",_stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex,_stXC_DIPPushInfo[u32Window][u16BufID].u32PriData);
    RES_DMS._stXC_LocalFrameRefCount[u32Window][u16BufID]++;

#if DBG_FPS
    u32FPS_Add_Counter[u32Window]++;
#endif
#endif
    return TRUE;
}

static MS_BOOL _DMS_VDEC_Frame_Release(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
#if NEW_FLIP
    if(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex == DMS_INVALID_FRAME_ID)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }
    if(RES_DMS._stXC_LocalFrameRefCount[u32Window][u16BufID] == 0)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }
    RES_DMS._stXC_LocalFrameRefCount[u32Window][u16BufID]--;
    if(RES_DMS._stXC_LocalFrameRefCount[u32Window][u16BufID] == 0)
    {
        VDEC_StreamId VdecStreamId;
        VDEC_EX_DispFrame VdecDispFrm;
        VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex;
        VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData;

        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %td, BufID %d, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
            return FALSE;
        }
        if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].eScanType))
        {
            VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
            VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
            VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex_2nd;
            VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData_2nd;

            VDEC_EX_Result eResult;
            eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
            if (eResult != E_VDEC_EX_OK)
            {
                DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                    (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
                DMS_PRINTF("[%s] window %td, BufID %d, 2nd Field, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
                return FALSE;
            }
        }
        //DMS_PRINTF("[Releas] Frame[0x%x], PriData[0x%x]\n",RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex,RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData);
        if(_IsFrameRefCountZero(u32Window, u16BufID))
        {
            RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex = DMS_INVALID_FRAME_ID;
        }
    }

#if DBG_FPS
    u32FPS_Release_Counter[u32Window]++;
#endif
#endif
    return TRUE;
}

#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
static MVOP_TileFormat _DMS_SetMVOPTileMode(E_DMS_VIDEO_TILE_MODE eTile)
{
    MS_U32 u32TileMode = 0;

    switch(eTile)
    {
        case E_DMS_VIDEO_TILE_MODE_16x32:
        {
            u32TileMode = 1;
            break;
        }
        case E_DMS_VIDEO_TILE_MODE_NONE:
        {
            u32TileMode = 2;
            break;
        }
        case E_DMS_VIDEO_TILE_MODE_32x16:
        {
            u32TileMode = 3;
            break;
        }
        case E_DMS_VIDEO_TILE_MODE_32x32:
        {
            u32TileMode = 4;
            break;
        }
        default:
        {
            printf("\033[031m[%s:%d]WARNING: NOT surport tile mode:%d\033[0m\n",__FUNCTION__, __LINE__, eTile);
        }
    }
    return u32TileMode;
}

static MS_BOOL _DMS_Is422Mode(E_DMS_VIDEO_DATA_FMT eFmt)
{
    MS_BOOL ret = 0;
    switch(eFmt)
    {
        case E_DMS_VIDEO_DATA_FMT_YUV420:
        case E_DMS_VIDEO_DATA_FMT_YUV420_H265:
        case E_DMS_VIDEO_DATA_FMT_YUV420_H265_10BITS:
        case E_DMS_VIDEO_DATA_FMT_YUV420_PLANER:
        case E_DMS_VIDEO_DATA_FMT_YUV420_SEMI_PLANER:
        {
            ret = 0;
            break;
        }
        default:
        {
            ret = 1;
            break;
        }
    }
    return ret;
}

static E_DMS_VIDEO_DATA_FMT _DMS_GetColorFormat(EN_DMS_COLORFORMAT eColorFormat)
{
    MS_U32 u32ColorFormat = 0;

    switch(eColorFormat)
    {
        case E_DMS_COLOR_FORMAT_HW_HVD:
        case E_DMS_COLOR_FORMAT_HW_MVD:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_YUV420;
            break;
        }
        case E_DMS_COLOR_FORMAT_SW_RGB565:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_RGB565;
            break;
        }
        case E_DMS_COLOR_FORMAT_SW_ARGB8888:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_ARGB8888;
            break;
        }
        case E_DMS_COLOR_FORMAT_YUYV:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_YUV422;
            break;
        }
        case E_DMS_COLOR_FORMAT_10BIT_TILE:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_YUV420_H265_10BITS;
            break;
        }
        case E_DMS_COLOR_FORMAT_SW_YUV420_SEMIPLANAR:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_YUV420_SEMI_PLANER;
            break;
        }
        case E_DMS_COLOR_FORMAT_HW_EVD:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_YUV420_H265;
            break;
        }
        default:
        {
            u32ColorFormat = E_DMS_VIDEO_DATA_FMT_MAX;
            printf("\033[031m[%s:%d]WARNING:DMS NOT SURPPORT color format:%d\033[0m\n",__FUNCTION__, __LINE__, eColorFormat);
            break;
        }
    }
    return u32ColorFormat;
}

static MS_BOOL _DMS_Is_SourceFieldBaseInterlaceBottom(E_DMS_VIDEO_SCAN_TYPE eScanType, MS_BOOL bIsVdecBot)
{
    if(_DMS_Is_SourceFieldBaseInterlace(eScanType) && bIsVdecBot)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
static void _Task_DMS_DisableBob_MainWin(void)
{
    void *pInstance = pTaskInstance;
    MS_U32 u32WaitEventFlag = BIT(0);
    MS_U32 u32Events = 0;
    MS_U32 u32BobDelay = 0;
    MS_U16 u16RP = 0;
    MS_U8 u8XCFrameNum = 0;
    VDEC_StreamId stVDEC_StreamId = {0};

    // coverity[no_escape]
    while(1)
    {
        MsOS_WaitEvent(s32DisableBob_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        DMS_GET_RES_PRI;

        u16RP = RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer;
        stVDEC_StreamId.u32Version = RES_DMS._stXC_DIPPushInfo[MAIN_WINDOW][u16RP].u32VDECStreamVersion;
        stVDEC_StreamId.u32Id = RES_DMS._stXC_DIPPushInfo[MAIN_WINDOW][u16RP].u32VDECStreamID;

        u8XCFrameNum = MApi_XC_Get_FrameNumFactor(MAIN_WINDOW);

        if(u32BobDelay == u8XCFrameNum)
        {
            MApi_XC_SetBOBMode(FALSE, MAIN_WINDOW);
            RES_DMS.bXCEnableBob = FALSE;
            u32BobDelay = 0;
        }
        u32BobDelay++;
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    }
}

static void _Task_DMS_SetMVOP(void)
{
    MS_U32 u32WaitEventFlag = BIT(0);
    MS_U32 u32Events = 0;
    void *pInstance = pTaskInstance;
    MS_BOOL bVdecTBflag = 0;
    MS_BOOL bMVOP_FDMask = FALSE;
    MS_BOOL bIsPause = FALSE;
    MS_BOOL bPauseIsBot = 0;
    MS_U32 u32CurReadPointer = 0;
    MS_U32 u32mfdecMiuSel = 0;
    MS_U32 i = 0;
    MS_U8 u8XCFrameNum = 0;
    MS_BOOL bXC_BobMode = FALSE;


    MVOP_Handle stHd = {E_MVOP_MODULE_MAIN};
    MVOP_Result ret = E_MVOP_FAIL;
    MVOP_DMSStreamINFO stMVOPStreamInfo;
    MVOP_DMSDisplaySize stMVOPDispSize;
    MVOP_DMSCropINFO stMVOPCropInfo;
    MVOP_DMSDisplayADD stMVOPDispAddr;

    memset(&stMVOPStreamInfo, 0, sizeof(MVOP_DMSStreamINFO));
    memset(&stMVOPDispSize, 0, sizeof(MVOP_DMSDisplaySize));
    memset(&stMVOPCropInfo, 0, sizeof(MVOP_DMSCropINFO));
    memset(&stMVOPDispAddr, 0, sizeof(MVOP_DMSDisplayADD));

    // coverity[no_escape]
    while(1)
    {
        MsOS_WaitEvent(s32MVOP_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        DMS_GET_RES_PRI;


        for(i = 0; i<DMS_DIP_QUEUEDEPTH ; i++)
        {
            if(RES_DMS.u32ReleaseState[i] >= 2)
            {
                 _DMS_VDEC_Frame_Release(MVOP_WINDOW, i);
                 RES_DMS.u32ReleaseState[i] = 0;
                 RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][i].bValid = FALSE;
            }
        }

        // make sure: 1. at least one frame is processed 2. DMS_SetMute is called.
        // than start MuteCounter
        u8XCFrameNum = MApi_XC_Get_FrameNumFactor(MAIN_WINDOW);
        if((RES_DMS.u32ReleaseState[RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer] >=1) && RES_DMS.bDisableBlackScreen)
        {
            RES_DMS.u32MuteCounter++;
            if(RES_DMS.u32MuteCounter == u8XCFrameNum)
            {
                if(RES_DMS.bFirstPlay)
                {
                   RES_DMS.bDisableBlackScreen = FALSE;
                   RES_DMS.bFirstPlay = FALSE;
                   MApi_XC_GenerateBlackVideoByMode(FALSE, MAIN_WINDOW,E_XC_BLACK_VIDEO_MEMSYNC);
                }
            }
        }

        // prevent from read ponter go through write pointer
        if(RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer == RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16WritePointer)
        {
           // pause: set FDmask to TRUE
            if(_DMS_Is_SourceInterlace(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][(RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer)].eScanType) && (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer].u8FieldCtrl == 0))
            {
                bIsPause = TRUE;
                stMVOPDispAddr.u8BotFlag = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer].eFieldOrderType^1);
                bPauseIsBot = stMVOPDispAddr.u8BotFlag;
                stMVOPDispAddr.u8FDMask = TRUE;
                ret = MDrv_MVOP_DMS_SetDispADDInfo(&stHd, E_MVOP_MAIN_WIN, &stMVOPDispAddr, NULL);
                if(ret != 0)
                {
                    DMS_PRINTF("\033[031m[%s:%d]MDrv_MVOP_DMS_SetDispADDInfo fail: %d\033[0m\n",__FUNCTION__, __LINE__, ret);
                }
            }
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
            continue;
        }

        if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][((RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer+1)%DMS_DIP_QUEUEDEPTH)].eScanType == E_DMS_VIDEO_SCAN_TYPE_PROGRESSIVE)
        {
            _DMS_GetNextInfo(MVOP_WINDOW, FALSE);
            u32CurReadPointer = RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer;
        }
        else
        {
            //Issue: Progressive data after FRC jump to interlace frame, and break while by continue due to interlace flag not match
            //Next round u16ReadPointer will be plus one cause previous one data not be process

            //Solution: Make sure every interlace data must be process, check condition as below:
            //Once u16ReadPointer's data is valid  &&  u32ReleaseState ==0, not to update u16ReadPointer
            if((RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer].bValid == FALSE) || (RES_DMS.u32ReleaseState[RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer] >= 1))
            {
               u32CurReadPointer = _GetNextBufferID(RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer);
            }
        }

        if((RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].bValid == TRUE) && (RES_DMS.u32ReleaseState[u32CurReadPointer] == 0))
        {
            if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eScanType == E_DMS_VIDEO_SCAN_TYPE_PROGRESSIVE)
            {
                if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].b10bitData)
                {
                    stMVOPDispAddr.u32LSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr_2bit;
                    stMVOPDispAddr.u32LSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr_2bit;
                    stMVOPDispAddr.u32LSB_FB_MIU[0] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr_2bit >= HAL_MIU1_BASE)?1:0 ;
                    stMVOPDispAddr.u32LSB_FB_MIU[1] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr_2bit >= HAL_MIU1_BASE)?1:0;
                    stMVOPDispAddr.u32BIT_DEPTH[0] = 10;
                    stMVOPDispAddr.u32BIT_DEPTH[1] = 10;
                }
                else
                {
                    stMVOPDispAddr.u32BIT_DEPTH[0] = 8;
                    stMVOPDispAddr.u32BIT_DEPTH[1] = 8;

                }
                stMVOPDispAddr.u32MSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr;
                stMVOPDispAddr.u32MSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr;
                stMVOPDispAddr.u8DMSB_FB_MIU[0] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr >= HAL_MIU1_BASE)? 1:0;
                stMVOPDispAddr.u8DMSB_FB_MIU[1] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr >= HAL_MIU1_BASE)? 1:0;
                if(_DMS_Is_ScalingConditionUse2ndBuffer(RES_DMS.gDMS_ScalingCondition))
                {
                    stMVOPDispAddr.u32MSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].phySrc2ndBufferLumaAddr;
                    stMVOPDispAddr.u32MSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].phySrc2ndBufferChromaAddr;
                    stMVOPDispAddr.u32BIT_DEPTH[0] = 8;
                    stMVOPDispAddr.u32BIT_DEPTH[1] = 8;
                }
                stMVOPDispAddr.bOutputIMode = 0;
            }
            else if(_DMS_Is_SourceInterlace(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eScanType))
            {
                if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].bIs2ndField)
                {
                    bVdecTBflag = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eFieldOrderType;
                }
                else
                {
                    bVdecTBflag = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eFieldOrderType ^ 0x0001;
                }

                // resume
                if(bIsPause)
                {
                    if(bVdecTBflag == bPauseIsBot)
                    {
                        bMVOP_FDMask = FALSE;
                        bIsPause = FALSE;
                    }
                    else
                    {
                        bMVOP_FDMask = TRUE;
                    }
                }

                if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u8FieldCtrl)  // ff , rw
                {
                    RES_DMS.bXCEnableBob = TRUE;
                    if(bVdecTBflag == (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eFieldOrderType^ 0x0001))
                    {
                         bMVOP_FDMask = FALSE;
                    }
                    else
                    {
                         bMVOP_FDMask = TRUE;
                    }

                    if(bXC_BobMode)
                    {
                        MApi_XC_SetBOBMode(TRUE, MAIN_WINDOW);
                    }
                    bXC_BobMode = TRUE;

                    bVdecTBflag =  (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eFieldOrderType^ 0x0001);

                }
                else
                {
                     if(RES_DMS.bXCEnableBob)
                     {
                        bMVOP_FDMask = FALSE;
                        bXC_BobMode = FALSE;
                        MsOS_SetEvent(s32DisableBob_Event_id, 0x01);
                     }
                }

                if(_DMS_Is_SourceFieldBaseInterlaceBottom(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eScanType, bVdecTBflag))
                {
                     if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].b10bitData)
                     {
                         stMVOPDispAddr.u32LSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddrI_2bit;
                         stMVOPDispAddr.u32LSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddrI_2bit;
                         stMVOPDispAddr.u32LSB_FB_MIU[0] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddrI_2bit >= HAL_MIU1_BASE)?1:0 ;
                         stMVOPDispAddr.u32LSB_FB_MIU[1] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddrI_2bit >= HAL_MIU1_BASE)?1:0;
                         stMVOPDispAddr.u32BIT_DEPTH[0] = 10;
                         stMVOPDispAddr.u32BIT_DEPTH[1] = 10;
                     }
                     else
                     {
                         stMVOPDispAddr.u32BIT_DEPTH[0] = 8;
                         stMVOPDispAddr.u32BIT_DEPTH[1] = 8;
                     }

                     stMVOPDispAddr.u32MSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddrI;
                     stMVOPDispAddr.u32MSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddrI;
                     stMVOPDispAddr.u8DMSB_FB_MIU[0] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddrI >= HAL_MIU1_BASE)? 1:0;
                     stMVOPDispAddr.u8DMSB_FB_MIU[1] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddrI >= HAL_MIU1_BASE)? 1:0;

               }
               else
               {
                    if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].b10bitData)
                    {
                        stMVOPDispAddr.u32LSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr_2bit;
                        stMVOPDispAddr.u32LSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr_2bit;
                        stMVOPDispAddr.u32LSB_FB_MIU[0] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr_2bit >= HAL_MIU1_BASE)?1:0 ;
                        stMVOPDispAddr.u32LSB_FB_MIU[1] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr_2bit >= HAL_MIU1_BASE)?1:0 ;
                        stMVOPDispAddr.u32BIT_DEPTH[0] = 10;
                        stMVOPDispAddr.u32BIT_DEPTH[1] = 10;
                    }
                    else
                    {
                         stMVOPDispAddr.u32BIT_DEPTH[0] = 8;
                         stMVOPDispAddr.u32BIT_DEPTH[1] = 8;
                     }

                     stMVOPDispAddr.u32MSB_FB_ADDR[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr;
                     stMVOPDispAddr.u32MSB_FB_ADDR[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr;
                     stMVOPDispAddr.u8DMSB_FB_MIU[0] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcLumaAddr >= HAL_MIU1_BASE)? 1:0;
                     stMVOPDispAddr.u8DMSB_FB_MIU[1] = (RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u32SrcChromaAddr >= HAL_MIU1_BASE)? 1:0;
               }
               stMVOPDispAddr.bOutputIMode = 1;
               stMVOPDispAddr.u8BotFlag = bVdecTBflag;
            }
            else
            {
                DMS_PRINTF("\033[031m[%s:%d]Error: undefine scanType:%d\033[0m\n",__FUNCTION__, __LINE__, RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eScanType);
            }
            // MVOP StreamInfo
            stMVOPStreamInfo.u32ApiDMSStream_Version = API_MVOP_DMS_STREAM_VERSION;
            stMVOPStreamInfo.u16ApiDMSStream_Length = sizeof(MVOP_DMSStreamINFO);
            stMVOPStreamInfo.eTileFormat = _DMS_SetMVOPTileMode(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eTileMode);
            stMVOPStreamInfo.bIs422Mode = _DMS_Is422Mode(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].eFmt);
            stMVOPStreamInfo.bIsDRAMCont = (stMVOPStreamInfo.bIs422Mode == TRUE)? TRUE : FALSE;
            stMVOPStreamInfo.bDDR4_REMAP = 0;
            if(_DMS_Is_ScalingConditionUse2ndBuffer(RES_DMS.gDMS_ScalingCondition))
            {
                 stMVOPStreamInfo.eTileFormat = _DMS_SetMVOPTileMode(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u8Src2ndBufferTileMode);
            }
            // MVOP DispSizeInfo
            stMVOPDispSize.u32ApiDMSSize_Version = API_MVOP_DMS_DISP_SIZE_VERSION;
            stMVOPDispSize.u16ApiDMSSize_Length = sizeof(MVOP_DMSDisplaySize);
            stMVOPDispSize.u16Width = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16SrcWidth;
            stMVOPDispSize.u16Height = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16SrcHeight;
            if(_DMS_Is_ScalingConditionUse2ndBuffer(RES_DMS.gDMS_ScalingCondition))
            {
                stMVOPDispSize.u16Pitch[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16Src2ndBufferPitch;
                stMVOPDispSize.u16Width = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16Src2ndBufferWidth ;
                stMVOPDispSize.u16Height = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16Src2ndBufferHeight ;
            }
            else
            {
                stMVOPDispSize.u16Pitch[0] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16SrcPitch;
            }
            if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].b10bitData)
            {
                stMVOPDispSize.u16Pitch[1] = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].u16Src10bitPitch;
            }
            stMVOPDispSize.U8DSIndex = 0xff;
            stMVOPDispSize.bDualDV_EN = 0;
            stMVOPDispSize.bHDup = 0;
            stMVOPDispSize.bVDup = 0;

            // MVOP SetDispADDInfo
            stMVOPDispAddr.u32ApiDMSADD_Version = API_MVOP_DMS_DIPS_ADD_VERSION;
            stMVOPDispAddr.u16ApiDMSADD_Length = sizeof(MVOP_DMSDisplayADD);
            stMVOPDispAddr.u8FRAME_ID = 0;  // no use
            if(_DMS_Is_ScalingConditionUse2ndBuffer(RES_DMS.gDMS_ScalingCondition))
            {
                RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.bMFDec_Enable = 0;
            }
            stMVOPDispAddr.bMFDEC_EN = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.bMFDec_Enable;
            stMVOPDispAddr.u8MFDEC_ID = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.u8MFDec_Select;
            stMVOPDispAddr.u32UNCOMPRESS_MODE = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.bUncompress_mode;
            stMVOPDispAddr.u32BITLEN_FB_ADDR = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.phyBitlen_Base;
            if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.phyBitlen_Base >= HAL_MIU1_BASE)
            {
                u32mfdecMiuSel = 1;
            }
            else
            {
                u32mfdecMiuSel = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.u8Bitlen_MiuSelect;
            }
            stMVOPDispAddr.u8BITLEN_FB_MIU = u32mfdecMiuSel;
            stMVOPDispAddr.u32BITLEN_FB_PITCH = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u32CurReadPointer].stMFdecInfo.u16Bitlen_Pitch;
            stMVOPDispAddr.bEN3D = 0;
            stMVOPDispAddr.u8FDMask = bMVOP_FDMask;
            stMVOPDispAddr.bXCBOB_EN = 0;

            ret = MDrv_MVOP_DMS_SetDispSizeInfo(&stHd, E_MVOP_MAIN_WIN, &stMVOPDispSize, NULL);
            if(ret != 0)
            {
                DMS_PRINTF("\033[031m[%s:%d]MDrv_MVOP_DMS_SetDispSizeInfo fail: %d\033[0m\n",__FUNCTION__, __LINE__, ret);
            }
            ret = MDrv_MVOP_DMS_SetStreamInfo(&stHd, E_MVOP_MAIN_WIN, &stMVOPStreamInfo, NULL);
            if(ret != 0)
            {
                DMS_PRINTF("\033[031m[%s:%d]MDrv_MVOP_DMS_SetStreamInfo fail: %d\033[0m\n",__FUNCTION__, __LINE__, ret);
            }
            ret = MDrv_MVOP_DMS_SetDispADDInfo(&stHd, E_MVOP_MAIN_WIN, &stMVOPDispAddr, NULL);
            if(ret != 0)
            {
                DMS_PRINTF("\033[031m[%s:%d]MDrv_MVOP_DMS_SetDispADDInfo fail: %d\033[0m\n",__FUNCTION__, __LINE__, ret);
            }

            for(i = 0; i<DMS_DIP_QUEUEDEPTH ; i++)
            {
                if(RES_DMS.u32ReleaseState[i] >= 1)
                {
                     RES_DMS.u32ReleaseState[i]++;
                }
            }
            RES_DMS.u32ReleaseState[u32CurReadPointer]++;
            RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer = u32CurReadPointer;
         } //if
         MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    }// while(1)
}


 static void _DMS_MVOP_ISR(void)
{
    MDrv_MVOP_EnableInterrupt(E_MVOP_INT_NONE);
    MsOS_SetEvent(s32MVOP_Event_id, 0x01);
    MDrv_MVOP_EnableInterrupt(E_MVOP_INT_VSYNC);
    MsOS_EnableInterrupt(E_INT_IRQ_DC);
}

#endif // DMS_MAIN_WINDOW_FLOW_VERSION == 0

static MS_BOOL _IsGwinAvailable(void)
{
    DMS_GET_RES_PRI;
    if(RES_DMS.u8GOPGWinNum == GWIN_NO_AVAILABLE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static MS_BOOL _DMS_IsWindowUsed (MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    return RES_DMS._stXC_DIPWinInfo[u32Window].bDIPwinUse;
}

static MS_BOOL _DMS_IsCaptureWindowEnable (MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    return RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Enable;
}

static MS_U16 _GetPreviousWritePointer(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    return _GetPreviousBufferID(RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer);
}

static MS_U16 _GetNextWritePointer(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    return _GetNextBufferID(RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer);
}

//For RP always Read (WP-1) principle, need marked valid false for drop case
//         W
//  o  x  v  o  o  o  o
//     R
//===next phase===
//                  W
//  o  x  x  x  x  v  o
//        R'      R

static void _MarkIgnorableReadBuffer(MS_U32 u32Window, MS_U16 u16TargetID)
{
    DMS_GET_RES_PRI;
    MS_U16 u16CurID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16ReadPointer;
    sync_print("MarkIg From%d to %d => ",u16CurID,u16TargetID);
    while (u16CurID != u16TargetID)
    {
        if(_DMS_Is_MVOPFlow(u32Window) && (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC))
        {
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
            if(u16CurID != RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer)
#else
            if(u16CurID != RES_DMS.gDMS_MVOPShowBufferID && u16CurID != RES_DMS.gDMS_MVOPRemoveBufferID)
#endif
            {
                _DMS_VDEC_Frame_Release(u32Window,u16CurID);
                RES_DMS._stXC_DIPPushInfo[u32Window][u16CurID].bValid = FALSE;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
                RES_DMS.u32ReleaseState[u16CurID] = 0;
#endif
                sync_print("MarkIg ID%d => ",u16CurID);
            }
        }
        else
        {
#if(DEV_NEW_NEXT == 0)
            MS_BOOL bOriginFrame = TRUE;
            if(bOriginFrame == TRUE)
            {
                bOriginFrame = FALSE;
            }
            else
#endif
            {
                _DMS_VDEC_Frame_Release(u32Window,u16CurID);
                RES_DMS._stXC_DIPPushInfo[u32Window][u16CurID].bValid = FALSE;
            }
            //sync_print("MarkIg ID%d => ",u16CurID);
        }
        u16CurID = _GetNextBufferID(u16CurID);
    }

}

//Get Nex Read Pointer and check if any buffer could be ignored
#define nrp_sync_print(fmt, args...) do { if(0) DMS_PRINTF(fmt, ##args);  } while (0)
static MS_U16 _GetNextReadPointer(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    MS_U16 u16NextID = 0;
    MS_BOOL bFrameBaseFlag = FALSE;
    if(_DMS_Is_MVOPFlow(u32Window))
    {
        sync_print("0.WP %d => ",RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer);
        if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
        {
            u16NextID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer;
        }
#if DEV_DI
        else if(DEV_DI_TIMER == 0 && (RES_DMS._stXC_DIPPushInfo[u32Window][RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer].bIsAfterDiTask == TRUE))
        {
            MS_U16 u16CurID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16ReadPointer;
            MS_U16 u16WP = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer;
            if(RES_DMS._stXC_DIPPushInfo[u32Window][u16WP].eFieldType == E_DMS_VIDEO_FIELD_TYPE_BOTH)
            {
                nrp_sync_print("[%td] Cur %d, WP %d, ",(ptrdiff_t)MsOS_GetSystemTime(),u16CurID,u16WP);
                static MS_U16 bForceBFlag = DMS_Invalid_ReadPointer_ID;
                MS_BOOL b2ndField = RES_DMS._stXC_DIPPushInfo[u32Window][u16WP].bIs2ndField;
                {
                    if(b2ndField == FALSE)
                    {
                        //Get a top field
                        if(bForceBFlag != DMS_Invalid_ReadPointer_ID)
                        {
                            if(bForceBFlag == u16WP)
                            {
                                u16NextID = _GetNextBufferID(u16CurID);
                                bFrameBaseFlag = TRUE;
                                nrp_sync_print("w_T_force_B.NRP %d => ",u16NextID);
                            }
                            else
                            {
                                u16NextID = u16WP;
                                nrp_sync_print("w_T_NewTo_%d.NRP %d => ",b2ndField,u16NextID);
                            }
                            bForceBFlag = DMS_Invalid_ReadPointer_ID;
                        }
                        else
                        {
                            u16NextID = u16WP;
                            nrp_sync_print("w_T.NRP %d => ",u16NextID);
                        }
                    }
                    else
                    {
                        //Get a bottom field
                        if(RES_DMS._stXC_DIPPushInfo[u32Window][_GetPreviousBufferID(u16WP)].bValid == TRUE)
                        {
                            u16NextID = _GetPreviousBufferID(u16WP);
                            nrp_sync_print("w_B_back_T.NRP %d => ",u16NextID);
                            bForceBFlag = u16WP;
                        }
                        else
                        {
                            u16NextID = u16WP;
                            nrp_sync_print("w_B.NRP %d => ",u16NextID);
                        }
                    }
                }
            }
            else
            {
                nrp_sync_print("xx.WP %d \n ",RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer);
                u16NextID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer;
            }
        }
        else if(DEV_DI_TIMER == 1 && (RES_DMS._stXC_DIPPushInfo[u32Window][RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer].bIsAfterDiTask == TRUE))
        {
            MS_U16 u16CurID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16ReadPointer;
            MS_U16 u16WP = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer;
            nrp_sync_print("Cur %d, WP %d, ",u16CurID,u16WP);
            nrp_sync_print("Timer.WP %d => ",RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer);
            u16NextID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer;
        }
#endif
        else
        {
            nrp_sync_print("b.WP %d => ",RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer);
            u16NextID = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer;
        }
    }
    else
    {
        u16NextID = _GetPreviousWritePointer(u32Window);
    }
    nrp_sync_print("Get Next ID%d => RBID %d => ",u16NextID,RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u16NextID].u8DiOutputRingBufferID);
    // Frame base interlace has two continous frame Buffer ID
    // MarkIgnorableReadBuffer is impossible.
    if(bFrameBaseFlag == FALSE)
    {
        _MarkIgnorableReadBuffer(u32Window, u16NextID);
    }

    return u16NextID;
}

static void _DMS_DisableByWin(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    if(RES_DMS._stXC_DIPWinInfo[u32Window].bDIPwinUse == TRUE)
    {
        //Window Count --
        RES_DMS.u8EnableWindowCount--;
        RES_DMS.u8UseDIPWindowCount--;

        //set as invalid
        RES_DMS._stXC_DIPWinInfo[u32Window].bDIPwinUse = FALSE;
        RES_DMS.u32WindowFrameRate[u32Window] = 0;

        //Raise the flag "need to clear buffer"
        RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_0][u32Window].bCleanBuf = TRUE;
        RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_1][u32Window].bCleanBuf = TRUE;

        //db_print(" _stXC_DispBuf_Flag[E_MSAPI_DIP_MULTIVIEW_BUF_ID_0][u32Window].bCleanBuf = %d \n", RES_DMS._stXC_DispBuf_Flag[E_MSAPI_DIP_MULTIVIEW_BUF_ID_0][u32Window].bCleanBuf);
        //db_print(" _stXC_DispBuf_Flag[E_MSAPI_DIP_MULTIVIEW_BUF_ID_1][u32Window].bCleanBuf = %d \n",RES_DMS. _stXC_DispBuf_Flag[E_MSAPI_DIP_MULTIVIEW_BUF_ID_1][u32Window].bCleanBuf);
    }
}

static void _DMS_InvalidateQueueByWin(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    int i = 0;
    for(i=0;i<DMS_DIP_QUEUEDEPTH;i++)
    {
       if(RES_DMS._stXC_DIPPushInfo[u32Window][i].u32Window == u32Window)
       {
#if DEV_DI
            _DMS_VDEC_Frame_Release_Interlace(u32Window,i);
#endif
            _DMS_VDEC_Frame_Release(u32Window,i);
            RES_DMS._stXC_DIPPushInfo[u32Window][i].bValid = FALSE;
       }
    }
}

static void _DMS_DeInitByWin(MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    int i = 0;
    MS_BOOL bDeInit = TRUE;

    for(i=0;i<DMS_MAX_WINDOW_NUM;i++)
    {
       if( RES_DMS._stXC_DIPWinInfo[i].bDIPwinUse == TRUE )
       {
           bDeInit = FALSE;
           break;
       }
    }
}

static void _DMS_DIP_ScoreSorting(void)
{
    DMS_GET_RES_PRI;
    MS_U8 i = 0;
    MS_U8 j = 0;
    MS_U8 temp = 0;

    for (i=0; i<DMS_MAX_WINDOW_NUM-1; i++)
    {
        for (j=1; j<DMS_MAX_WINDOW_NUM-i-1; j++)
        {
            if (RES_DMS.WindowScore[j] > RES_DMS.WindowScore[j+1])
            {
                temp = RES_DMS.WindowScore[j];
                RES_DMS.WindowScore[j]   = RES_DMS.WindowScore[j+1];
                RES_DMS.WindowScore[j+1] = temp;

                temp = RES_DMS.WindowZOrder[j];
                RES_DMS.WindowZOrder[j]   = RES_DMS.WindowZOrder[j+1];
                RES_DMS.WindowZOrder[j+1] = temp;
            }
        }
    }

}

static MS_BOOL _DMS_DIP_IsDispBufferValid(void* pInstance, E_DMS_DIP_CLIENT eDIPClient)
{
    DMS_GET_RES_PRI;
    if (RES_DMS.u32DIPAddress_pa[eDIPClient] == NULL)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static MS_PHY _DMS_DIP_GetDispBufferAddress(void* pInstance, E_DMS_DIP_CLIENT eDIPClient, MS_U16 u16BufID)
{
    MS_PHY phyBufAddr = NULL;
    MS_U32 BytesPerLine = 0;
    MS_U32 UHDBytesPerLine = 0;
    DMS_GET_RES_PRI;
    if (_DMS_DIP_IsDispBufferValid(pInstance, eDIPClient) == NULL)
    {
        return NULL;
    }

    BytesPerLine = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[eDIPClient].u32PanelWidth);
    UHDBytesPerLine = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt, DMS_Frame_Buffer_Width);

    if(eDIPClient == E_DMS_DIP_CLIENT_GOP)
    {
        phyBufAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + ( (UHDBytesPerLine * DMS_Frame_Buffer_Height) * u16BufID );
    }
    else
    {
        phyBufAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + ( (BytesPerLine * RES_DMS._stXC_DIPMemInfo[eDIPClient].u32PanelHeight) * u16BufID );
    }

    //DMS_PRINTF("\033[32m [%s][%d] === u32DIPAddress_pa : 0x%lx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS.u32DIPAddress_pa[eDIPClient]);
    //DMS_PRINTF("\033[32m [%s][%d] === u32DIPStartMemAddr : 0x%lx === \033[0m\n",__FUNCTION__,__LINE__,phyBufAddr);

    return phyBufAddr;
}

static MS_BOOL _DMS_GEBitblt(MS_PHY SrcAddress, MS_PHY DstAddress, GFX_DrawRect* stgfxDrawRect)
{
    DMS_GET_RES_PRI;
    GFX_BufferInfo stSrc, stDst;
    GFX_Point stgfxPt0, stgfxPt1;
    MS_U16 fbFmt = 0;

    switch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt)
    {
        case DIP_DATA_FMT_YUV422:
        default:
            fbFmt = E_MS_FMT_YUV422;
            break;
    }

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth;
    stgfxPt1.y = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);

    stSrc.u32ColorFmt =fbFmt;
    stSrc.u32Addr =SrcAddress;
    stSrc.u32Width = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth;
    stSrc.u32Height = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight;
    stSrc.u32Pitch =_DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);
    MApi_GFX_SetSrcBufferInfo(&stSrc, 0);
    stDst.u32ColorFmt = fbFmt;
    stDst.u32Addr = DstAddress;
    stDst.u32Width = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth;
    stDst.u32Height = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight;
    stDst.u32Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);
    MApi_GFX_SetDstBufferInfo(&stDst, 0);

    MApi_GFX_BitBlt(stgfxDrawRect, GFXDRAW_FLAG_SCALE);
    MApi_GFX_FlushQueue();

    return TRUE;
}

static MS_BOOL _DMS_GERendering(void* pInstance, MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    //db_print(" u8WindowID = %d need GERendering to update buffer\n", u8WindowID);
    GFX_DrawRect stgfxDrawRect;
    stgfxDrawRect.srcblk.x = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x;
    stgfxDrawRect.srcblk.y = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y;
    stgfxDrawRect.srcblk.width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
    stgfxDrawRect.srcblk.height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;

    stgfxDrawRect.dstblk.x = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x;
    stgfxDrawRect.dstblk.y = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y;
    stgfxDrawRect.dstblk.width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
    stgfxDrawRect.dstblk.height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
    if (RES_DMS.bDIPWriteFirstAddr)
    {
        _DMS_GEBitblt(_DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, E_DMS_DIP_MULTIVIEW_BUF_ID_1),
            _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, E_DMS_DIP_MULTIVIEW_BUF_ID_0),
            &stgfxDrawRect);
    }
    else
    {
        _DMS_GEBitblt(_DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, E_DMS_DIP_MULTIVIEW_BUF_ID_0),
            _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, E_DMS_DIP_MULTIVIEW_BUF_ID_1),
            &stgfxDrawRect);
    }

    return TRUE;
}

static MS_BOOL _DMS_GE_DrawRect(MS_PHY DstAddress, GFX_Block stRect, MS_U32 u32Data)
{
    DMS_GET_RES_PRI;
    GFX_Point stgfxPt0;
    GFX_Point stgfxPt1;
    GFX_BufferInfo stgfxDstBuf;
    GFX_RectFillInfo pGfxRectFillInfo;
    memset(&pGfxRectFillInfo, 0, sizeof(GFX_RectFillInfo));

    //Set Dst buffer
    stgfxDstBuf.u32ColorFmt = GFX_FMT_YUV422;
    stgfxDstBuf.u32Addr = DstAddress;
    stgfxDstBuf.u32Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);
    MApi_GFX_SetDstBufferInfo(&stgfxDstBuf, 0);

    stgfxPt0.x = 0;
    stgfxPt0.y = 0;
    stgfxPt1.x = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth;
    stgfxPt1.y = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight;
    MApi_GFX_SetClip(&stgfxPt0, &stgfxPt1);

    pGfxRectFillInfo.dstBlock.x = stRect.x;
    pGfxRectFillInfo.dstBlock.y = stRect.y;
    pGfxRectFillInfo.dstBlock.width= stRect.width;
    pGfxRectFillInfo.dstBlock.height= stRect.height;
    pGfxRectFillInfo.fmt = GFX_FMT_YUV422;

    pGfxRectFillInfo.colorRange.color_s.a = pGfxRectFillInfo.colorRange.color_e.a = 0;
    pGfxRectFillInfo.colorRange.color_s.r = pGfxRectFillInfo.colorRange.color_e.r = 0;
    pGfxRectFillInfo.colorRange.color_s.g = pGfxRectFillInfo.colorRange.color_e.g = 0;
    pGfxRectFillInfo.colorRange.color_s.b = pGfxRectFillInfo.colorRange.color_e.b = 0;

    pGfxRectFillInfo.flag = GFXRECT_FLAG_COLOR_CONSTANT;
    MApi_GFX_RectFill(&pGfxRectFillInfo);

    MApi_GFX_FlushQueue();
    return TRUE;
}


//Draw rectangle with transparent color
static MS_BOOL _DMS_GE_GetRectAreaByWin(MS_U32 u32Window, GFX_Block *stRect)
{
    DMS_GET_RES_PRI;
    stRect->x = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x;
    stRect->y= RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y;
    stRect->width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
    stRect->height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
    return TRUE;
}

//Draw rectangle with transparent color
static MS_BOOL _DMS_GE_GetPreviousRectAreaByWin(MS_U32 u32Window, GFX_Block *stRect)
{
    DMS_GET_RES_PRI;


    stRect->x = RES_DMS._stXC_PreDispWinInfo[u32Window].u32x;
    stRect->y= RES_DMS._stXC_PreDispWinInfo[u32Window].u32y;
    stRect->width = RES_DMS._stXC_PreDispWinInfo[u32Window].u32width;
    stRect->height = RES_DMS._stXC_PreDispWinInfo[u32Window].u32height;
    return TRUE;
}

//Draw rectangle with transparent color
static MS_BOOL _DMS_GE_ClearWin(void *pInstance, MS_U16 u16BufID, MS_U32 u32Window)
{
    GFX_Block stRect = {0};
    MS_PHY phyBaseAddr = NULL;

    //db_print(" u16BufID = %d, u32Window = %d \n", u16BufID, u32Window);

    //Get Base Address by Buffer ID
    phyBaseAddr = _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, u16BufID);

    if (phyBaseAddr == NULL)
    {
        DMS_PRINTF(" phyBaseAddr is invalid (NULL) !! \n");
        return FALSE;
    }

    //Get Rectangle Area by Window Number
    _DMS_GE_GetRectAreaByWin(u32Window, &stRect);

    //db_print(" stRect.x = %d, stRect.y = %d, stRect.width = %d, stRect.height = %d \n", stRect.x, stRect.y, stRect.width, stRect.height);

    //Draw rectangle with transparent color
    _DMS_GE_DrawRect(phyBaseAddr, stRect, DMS_DIP_MULTIVIEW_TRANSPARENT_COLOR);

    return TRUE;
}

//Draw rectangle with transparent color
static MS_BOOL _DMS_GE_ClearPreviousWin(void* pInstance, MS_U16 u16BufID, MS_U32 u32Window)
{
    GFX_Block stRect = {0};
    MS_PHY phyBaseAddr = NULL;

    //db_print(" u16BufID = %d, u32Window = %d \n", u16BufID, u32Window);

    //Get Base Address by Buffer ID
    phyBaseAddr = _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, u16BufID);

    //db_print(" phyBaseAddr = 0x%lx, \n", phyBaseAddr);

    //Get Rectangle Area by Window Number
    _DMS_GE_GetPreviousRectAreaByWin(u32Window, &stRect);

    //db_print(" stRect.x = %d, stRect.y = %d, stRect.width = %d, stRect.height = %d \n", stRect.x, stRect.y, stRect.width, stRect.height);
    //DMS_PRINTF(" stRect.x = %d, stRect.y = %d, stRect.width = %d, stRect.height = %d \n", stRect.x, stRect.y, stRect.width, stRect.height);

    //Draw rectangle with transparent color
    _DMS_GE_DrawRect(phyBaseAddr, stRect, DMS_DIP_MULTIVIEW_TRANSPARENT_COLOR);

    return TRUE;
}

static MS_U8 _DMS_DIP_GetBackgroundBufID(void *pInstance)
{
    DMS_GET_RES_PRI;
    if (RES_DMS.bDIPWriteFirstAddr == TRUE)
    {
        return E_DMS_DIP_MULTIVIEW_BUF_ID_0;
    }
    else
    {
        return E_DMS_DIP_MULTIVIEW_BUF_ID_1;
    }
}

static MS_BOOL _WinNeedToClear(void *pInstance, MS_U8 u8BufId, MS_U8 u8WindowId)
{
    DMS_GET_RES_PRI;
    return RES_DMS._stXC_DispBuf_Flag[u8BufId][u8WindowId].bCleanBuf;
}

static void _WinResetClearBufFlag(void* pInstance, MS_U8 u8BufId, MS_U8 u8WindowId)
{
    DMS_GET_RES_PRI;
    RES_DMS._stXC_DispBuf_Flag[u8BufId][u8WindowId].bCleanBuf = FALSE;
}

static MS_BOOL _DMS_HandleClearFlag(void* pInstance)
{
    MS_U8 u8BufId = 0;
    MS_U8 u8WindowId = 0;

    u8BufId = _DMS_DIP_GetBackgroundBufID(pInstance);
    //db_print(" trace, u8BufId = %d \n", u8BufId);

    for (u8WindowId= (DMS_MVOP_FLOW)? 1 : 0; u8WindowId < DMS_MAX_WINDOW_NUM; u8WindowId++)
    {
        if (_WinNeedToClear(pInstance, u8BufId, u8WindowId) == TRUE)
        {
            //Reset Buffer of Window
            _DMS_GE_ClearWin(pInstance, u8BufId, u8WindowId);

            //Reset Clear Flag
            _WinResetClearBufFlag(pInstance, u8BufId, u8WindowId);
        }
    }
    return TRUE;
}

static MS_BOOL _WinDispChange(void* pInstance, MS_U8 u8BufId, MS_U8 u8WindowId)
{
    DMS_GET_RES_PRI;
    return RES_DMS._stXC_DispBuf_Flag[u8BufId][u8WindowId].bDispChange;
}

static void _WinResetDispChangeFlag(void* pInstance, MS_U8 u8BufId, MS_U8 u8WindowId)
{
    DMS_GET_RES_PRI;
    RES_DMS._stXC_DispBuf_Flag[u8BufId][u8WindowId].bDispChange = FALSE;
}

static MS_BOOL _DMS_HandleDispChangeFlag(void* pInstance)
{
    MS_U8 u8BufId = 0;
    MS_U8 u8WindowId = 0;

    u8BufId = _DMS_DIP_GetBackgroundBufID(pInstance);

    //DMS_PRINTF(" [%s][%d] trace, u8BufId = %d \n", __FUNCTION__, __LINE__, u8BufId);
    for (u8WindowId=(DMS_MVOP_FLOW)? 1 : 0; u8WindowId < DMS_MAX_WINDOW_NUM; u8WindowId++)
    {
        if (_WinDispChange(pInstance, u8BufId, u8WindowId) == TRUE)
        {
            //db_print(" trace, u8BufId = %d , u8WindowId =%d \n", u8BufId, u8WindowId);

            //Reset Buffer of Window
            _DMS_GE_ClearPreviousWin(pInstance, u8BufId, u8WindowId);

            //Reset Clear Flag
            _WinResetDispChangeFlag(pInstance, u8BufId, u8WindowId);
        }
    }
    return TRUE;
}


//Check Display Buffer Flag and Fire
static MS_BOOL _DMS_CheckDispBufFlag(void* pInstance)
{
    DMS_GET_RES_PRI;
    //Handle if there any window need to be clear
    _DMS_HandleClearFlag(pInstance);

    //Handle if there any display window change
    _DMS_HandleDispChangeFlag(pInstance);

    //others
    //At new vsync start, we can remove marked buffer id safely for MVOP from VDEC
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
#if DEV_PAUSE
    if(_DMS_Is_MVOPFlow(MVOP_WINDOW))
#else
    if(_DMS_Is_MVOPFlow(MVOP_WINDOW) && (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC))
#endif
    {
        if(RES_DMS.bMVOPMarkRemoveBuffer)
        {
            sync_print(" RemoveID %d V:%d =>",RES_DMS.gDMS_MVOPRemoveBufferID,RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPRemoveBufferID].bValid);
            if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPRemoveBufferID].bValid == TRUE && RES_DMS.gDMS_MVOPRemoveBufferID != DMS_Invalid_ReadPointer_ID)
            {
                //sync_print(" Remove %d =>",gDMS_MVOPRemoveBufferID);
                RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPRemoveBufferID].bValid = FALSE;
                _DMS_VDEC_Frame_Release(MVOP_WINDOW,RES_DMS.gDMS_MVOPRemoveBufferID);
                RES_DMS.gDMS_MVOPRemoveBufferID = DMS_Invalid_ReadPointer_ID;
            }
            RES_DMS.bMVOPMarkRemoveBuffer = FALSE;
        }
    }

    if(_DMS_Is_MVOPFlow(MVOP_WINDOW))
    {
        if(RES_DMS.bDMS_Freeze_DoubleBuffer)
        {
            _DMS_Add_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE);
        }
        else
        {
            _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE);
        }
    }
#endif //#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    return TRUE;
}

//(5) Get Next round DIP window stage
#define START_FRC_ACCUMULE_FRAME_COUNT 3
static MS_BOOL _DMS_GetNextInfo(MS_U16 u16WindowID, MS_BOOL bResetCaptureID)
{
    DMS_GET_RES_PRI;
    //static MS_U16 u16WindowID = 0;
    MS_U16 u16NextReadPointer = 0;
    MS_U8 u8BufferID = DMS_Invalid_ReadPointer_ID;

    if(bResetCaptureID)
    {
        RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = DMS_Invalid_ReadPointer_ID;
    }

    if(DEV_NEW_NEXT)
    {
        MS_U32 u32OutputRate = RES_DMS.u32OutputRate;
        if(u32OutputRate > 0)
        {
            RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = DMS_Invalid_ReadPointer_ID;
            if(RES_DMS.bStartCountFRC[u16WindowID] != FALSE)
            {
                MS_U16 u16ReadPoint = RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer;
                MS_U32 u32InputRate = RES_DMS._stXC_DIPPushInfo[u16WindowID][u16ReadPoint].u32FrameRate;
                if(u32InputRate == 0)
                {
                    DMS_PRINTF("[%s] Input Frame rate is 0, can not move FRC\n",__FUNCTION__);
                }
#if TEMP_LOG
                DMS_PRINTF(" In:%d ,\n",u32InputRate);
#endif
                if(_DMS_Is_SourceInterlace(RES_DMS._stXC_DIPPushInfo[u16WindowID][u16ReadPoint].eScanType))
                {
                    u32InputRate *= 2;
                }
                MS_U32 u32ReadPointOffset = 0;
                RES_DMS.u32AccumuleInputRate[u16WindowID] += u32InputRate;
#if TEMP_LOG
                DMS_PRINTF("AccumuleInputRate=%d, u32OutputRate=%d\n",RES_DMS.u32AccumuleInputRate[u16WindowID],RES_DMS.u32OutputRate);
#endif
                if(RES_DMS.u32AccumuleInputRate[u16WindowID] >= u32OutputRate)
                {
                    u32ReadPointOffset = RES_DMS.u32AccumuleInputRate[u16WindowID] / u32OutputRate;
                    RES_DMS.u32AccumuleInputRate[u16WindowID] = RES_DMS.u32AccumuleInputRate[u16WindowID] % u32OutputRate;
                }
#if TEMP_LOG
                DMS_PRINTF("AccumuleInputRate=%d, u32ReadPointOffset=%d\n",RES_DMS.u32AccumuleInputRate[u16WindowID],u32ReadPointOffset);
#endif
                if(u32ReadPointOffset > 0)
                {
                    MS_U8 i = 0;
                    MS_U16 u16NextReadPointer = RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer;
                    for(i = 0 ; i < u32ReadPointOffset; i++)
                    {
                        u16NextReadPointer = _GetNextBufferID(u16NextReadPointer);
                    }
                    if (RES_DMS._stXC_DIPPushInfo[u16WindowID][u16NextReadPointer].bValid)
                    {
                        _MarkIgnorableReadBuffer(u16WindowID, u16NextReadPointer);
                        if(RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer != u16NextReadPointer)
                        {
                            RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = u16NextReadPointer;
                            RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer = u16NextReadPointer;
                        }
                    }
                    else
                    {
                        //DMS_PRINTF("[%s][%d] win %d, now %d, next %d NOT Valid\n",__FUNCTION__,__LINE__,u16WindowID,RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer,u16NextReadPointer);
                    }
                }
                else
                {
                    if(_DMS_Is_MVOPFlow(u16WindowID) == FALSE)
                    {
                        //No frame has been pushed, display the old frame
                        if(_DMS_Query_XC_Status(u16WindowID, DMS_XC_UPDATE_FROM_VDEC))
                        {
                            RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = u8BufferID;
                        }
                    }
                }
            }
            else
            {
                MS_U16 u16WritePoint = RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16WritePointer;
                if(RES_DMS._stXC_DIPPushInfo[u16WindowID][u16WritePoint].bValid)
                {
                        _MarkIgnorableReadBuffer(u16WindowID, u16WritePoint);
                        RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer = u16WritePoint;
                        RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = u16WritePoint;
                        RES_DMS.bStartCountFRC[u16WindowID] = TRUE;
                }
            }
        }

    }
    else
    {
        //Check Window is Enable or Not
        if (_DMS_IsWindowUsed(u16WindowID) == FALSE)
        {
            //Clean Pushframe Info in queue
            _DMS_InvalidateQueueByWin(u16WindowID);
            return TRUE;
        }

        //Find Next Read Pointer
        u16NextReadPointer = _GetNextReadPointer(u16WindowID);
        if((_DMS_Is_MVOPFlow(u16WindowID) == FALSE) ||
            ((_DMS_Is_MVOPFlow(u16WindowID) == TRUE) && (RES_DMS.eDMS_MVOPFlowCtrl != E_DMS_MVOP_FLOW_FROM_VDEC)))
        {
            u8BufferID = RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer;
            RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer = u16NextReadPointer;
        }

        //Set infomation to u8DIPReadytoCaptureID[]
        if (RES_DMS._stXC_DIPPushInfo[u16WindowID][u16NextReadPointer].bValid)
        {
            sync_print(" V, NRP %d => ",u16NextReadPointer);
            if(_DMS_Is_MVOPFlow(u16WindowID) && (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC))
            {
                if(RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer != u16NextReadPointer)
                {
                    RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = u16NextReadPointer;
                    RES_DMS._stXC_DIPWriteReadPointer[u16WindowID].u16ReadPointer = u16NextReadPointer;
                }
            }
            else
            {
                if(u8BufferID != u16NextReadPointer)
                {
                    //A new frame has been pushed
                    if(u8BufferID < DMS_Invalid_ReadPointer_ID)
                    {
                        RES_DMS._stXC_DIPPushInfo[u16WindowID][u8BufferID].bValid = FALSE;
                        _DMS_VDEC_Frame_Release(u16WindowID,u8BufferID);
                    }
                    RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = u16NextReadPointer;
                }
                else
                {
                    if(bResetCaptureID == FALSE)
                    {
                        //No frame has been pushed, display the old frame
                        if(_DMS_Query_XC_Status(u16WindowID, DMS_XC_UPDATE_FROM_VDEC))
                        {
                            RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = u8BufferID;
                        }
                        else
                        {
                            RES_DMS.u8DIPReadytoCaptureID[u16WindowID] = DMS_Invalid_ReadPointer_ID;
                        }
                    }
                }
            }
        }
    }

    return TRUE;
}

//(3) Prepare Next Frame to be Displayed
static MS_BOOL _DMS_SetDIPRWindow(E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[eAction];
    ST_XC_DIPR_PROPERTY_EX DIPRWinPropertytoShow_EX;
    memset(&DIPRWinPropertytoShow_EX,0,sizeof(ST_XC_DIPR_PROPERTY_EX));

    ST_DIP_DI_SETTING DIP3DDISetting;
#if DMS_DIP_Support_3DDI
    static MS_U8 u8count = 0;
    ST_DIP_DI_NWAY_BUF_INFO stDINwayBuf[32];
#endif
    memset(&DIP3DDISetting,0,sizeof(ST_DIP_DI_SETTING));

    if(eAction == E_DMS_DIP_DI)
    {
        //DIPR win setting
        DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt = _DMS_DIP_GetDIPRDataFMT(&RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID]);

        DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width = DMS_DIPR_Alignment(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u16SrcWidth, DMS_HAL_DIPR_Align_YUV420);

#if DMS_DIP_Support_3DDI
        if(_DMS_Is_SourceFrameBaseInterlace(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eScanType, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eFieldType))
        {
            DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u16SrcHeight;
        }
        else
        {
            DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u16SrcHeight / 2;
        }
#else
        DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u16SrcHeight;
#endif
        DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch = _DMS_DIP_CalcPitch(DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u16SrcPitch);
        DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcLumaAddr;
        DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcChromaAddr;
        DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr10Bits = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcLumaAddr;
        DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr10Bits = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcChromaAddr;
        //DMS_PRINTF("\033[34m  _DMS_SetDIPRWindowInterlace %d %d\033[0m\n",u16BufferID,DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt);

        if(_DMS_Is_SourceFrameBaseInterlace(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eScanType, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eFieldType))
        {
            _DMS_DIP_GetFrameBaseInterlaceFieldType(&RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID]);
        }
#if DMS_DIP_Support_3DDI
        if(u8count < 2)
        {
            DIP3DDISetting.bEnableDI = TRUE;
            DIP3DDISetting.bInitDI = FALSE;
            DIP3DDISetting.u8NwayIndex = u32Window;
            DIP3DDISetting.enDI_Field = _DMS_DIP_GetDIPDiDataFMT(u32Window, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eScanType, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eFieldType);

            //DIP3DDISetting.phy2ndFieldAddr = 0;
            DIP3DDISetting.bEnaNwayInfoRefresh = FALSE;
            DIP3DDISetting.u8NwayBufCnt = 1; // Fix me
            DIP3DDISetting.bEnableBob = TRUE;// Fix me

            stDINwayBuf[0].phyDI_BufAddr = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIStartAddr;
            stDINwayBuf[0].u32DI_BufSize = RES_DMS._stXC_DIPWinInfo[u32Window].u32DISize;
            stDINwayBuf[0].u8DIBufCnt = RES_DMS._stXC_DIPWinInfo[u32Window].u8DIBufCnt;

            DIP3DDISetting.stDINwayBufInfo = (void*)stDINwayBuf;

            DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32version=1;
            DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32size=sizeof(ST_DIP_DI_SETTING);
            DIPRWinPropertytoShow_EX.stDIPR3DDISetting.pDIPR3DDISetting = (void*)&DIP3DDISetting;
            u8count ++;
        }
        else
        {
            DIP3DDISetting.bEnableDI = TRUE;
            DIP3DDISetting.bInitDI = FALSE;
            DIP3DDISetting.u8NwayIndex = u32Window;
            DIP3DDISetting.enDI_Field = _DMS_DIP_GetDIPDiDataFMT(u32Window, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eScanType, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eFieldType);

            DIP3DDISetting.bEnaNwayInfoRefresh = FALSE;
            DIP3DDISetting.u8NwayBufCnt = 1; // Fix me
            DIP3DDISetting.bEnableBob = FALSE;// Fix me

            stDINwayBuf[0].phyDI_BufAddr = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIStartAddr;
            stDINwayBuf[0].u32DI_BufSize = RES_DMS._stXC_DIPWinInfo[u32Window].u32DISize;
            stDINwayBuf[0].u8DIBufCnt = RES_DMS._stXC_DIPWinInfo[u32Window].u8DIBufCnt;

            DIP3DDISetting.stDINwayBufInfo = (void*)stDINwayBuf;

            DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32version=1;
            DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32size=sizeof(ST_DIP_DI_SETTING);
            DIPRWinPropertytoShow_EX.stDIPR3DDISetting.pDIPR3DDISetting = (void*)&DIP3DDISetting;
#if 0
            DMS_PRINTF("\033[34m  width %d, Height %d, Pitch %d \033[0m\n",DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width, DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height, DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch);
            DMS_PRINTF("\033[35m bEnable %d, bInitDI %d, NwayIndex %d, enDiField %d, bEnaNwayInfoRefresh %d, u8NwayBufCnt %d, bEnableBob %d\033[0m\n",
                DIP3DDISetting.bEnableDI, DIP3DDISetting.bInitDI, DIP3DDISetting.u8NwayIndex, DIP3DDISetting.enDI_Field, DIP3DDISetting.bEnaNwayInfoRefresh,
                DIP3DDISetting.u8NwayBufCnt, DIP3DDISetting.bEnableBob);
#endif

        }
#else
        DIP3DDISetting.bEnableDI = TRUE;
        DIP3DDISetting.enDI_Field = _DMS_DIP_GetDIPDiDataFMT(u32Window, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eScanType, RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eFieldType);

        if(_DMS_Is_SourceFieldBaseInterlace(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eScanType))
        {
            DIP3DDISetting.phyBotYBufAddr = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcLumaAddrI;
            DIP3DDISetting.phyBotCBufAddr = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcChromaAddrI;
            DIP3DDISetting.phyBotYBufAddr10Bits = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcLumaAddrI;
            DIP3DDISetting.phyBotCBufAddr10Bits = RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].u32SrcChromaAddrI;
        }
        else
        {
            DIP3DDISetting.phyBotYBufAddr = DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr;
            DIP3DDISetting.phyBotCBufAddr = DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr;
            DIP3DDISetting.phyBotYBufAddr10Bits = DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr;
            DIP3DDISetting.phyBotCBufAddr10Bits = DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr;
        }
        DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32version=1;
        DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32size=sizeof(ST_DIP_DI_SETTING);
        DIPRWinPropertytoShow_EX.stDIPR3DDISetting.pDIPR3DDISetting = (void*)&DIP3DDISetting;
#endif
    }
    else
    {
        if(u16BufferID < DMS_Invalid_ReadPointer_ID)
        {
            RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt = _DMS_DIP_GetDIPRDataFMT(&RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID]);

            if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth,RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
            {
                DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width = DMS_DIPR_Alignment((RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth/2), DMS_HAL_DIPR_Align_YUV420);
                DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight/2;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16Src2ndBufferPitch);
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].phySrc2ndBufferLumaAddr;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].phySrc2ndBufferChromaAddr;
            }
            else
            {
                //DIPR win setting
                DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width = DMS_DIPR_Alignment(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth, DMS_HAL_DIPR_Align_YUV420);
                DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcPitch);
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcLumaAddr;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcChromaAddr;

                DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr10Bits = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcLumaAddr;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr10Bits = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcChromaAddr;
            }
#if DMS_DIP_NWindow_Support_2P5DDI
            if(_DMS_Is_SourceFrameBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eScanType, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eFieldType))
            {
                _DMS_DIP_GetFrameBaseInterlaceFieldType(&RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID]);
            }

            if(_DMS_Is_SourceInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eScanType))
            {
                DIP3DDISetting.bEnableDI = TRUE;
                DIP3DDISetting.enDI_Field = _DMS_DIP_GetDIPDiDataFMT(u32Window, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eScanType, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eFieldType);
                if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eScanType))
                {
                    DIP3DDISetting.phyBotYBufAddr = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcLumaAddrI;
                    DIP3DDISetting.phyBotCBufAddr = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcChromaAddrI;
                    DIP3DDISetting.phyBotYBufAddr10Bits = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcLumaAddrI;
                    DIP3DDISetting.phyBotCBufAddr10Bits = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcChromaAddrI;
                }
                else
                {
                    DIP3DDISetting.phyBotYBufAddr = DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr;
                    DIP3DDISetting.phyBotCBufAddr = DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr;
                    DIP3DDISetting.phyBotYBufAddr10Bits = DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr;
                    DIP3DDISetting.phyBotCBufAddr10Bits = DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr;
                }
                DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32version=1;
                DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32size=sizeof(ST_DIP_DI_SETTING);
                DIPRWinPropertytoShow_EX.stDIPR3DDISetting.pDIPR3DDISetting = (void*)&DIP3DDISetting;
            }
            else
#endif
            {
                DIP3DDISetting.bEnableDI = FALSE;
                DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32version=1;
                DIPRWinPropertytoShow_EX.stDIPR3DDISetting.stDipr3DDI_VerCtl.u32size=sizeof(ST_DIP_DI_SETTING);
                DIPRWinPropertytoShow_EX.stDIPR3DDISetting.pDIPR3DDISetting = (void*)&DIP3DDISetting;
            }

            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.stMFDec_VerCtl.u32version = 0;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.stMFDec_VerCtl.u32size = sizeof(ST_XC_DIPR_PROPERTY_EX);
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.enMFDec_tile_mode = _DMS_DIP_MFDECTileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eTileMode);
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.phyLuma_FB_Base = (RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcLumaAddr);
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.phyChroma_FB_Base = (RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u32SrcChromaAddr);
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16FB_Pitch = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcPitch;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16HSize = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16VSize = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;

            if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
            {
                DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bMFDec_Enable = FALSE;
            }
            else
            {
                DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bMFDec_Enable = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.bMFDec_Enable;
            }

            if(eAction == E_DMS_DIP_NORMAL)
            {
                DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u8MFDec_Select = DMS_MFDEC_NORMAL;
            }
            else if(eAction == E_DMS_DIP_CAPTURE)
            {
                DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u8MFDec_Select = DMS_MFDEC_CAPTURE;
            }
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bUncompress_mode = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.bUncompress_mode;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bBypass_codec_mode = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.bBypass_codec_mode;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.en_MFDecVP9_mode = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.en_MFDecVP9_mode;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16StartX = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.u16StartX;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16StartY = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.u16StartY;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.phyBitlen_Base = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.phyBitlen_Base;
            DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16Bitlen_Pitch = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].stMFdecInfo.u16Bitlen_Pitch;
        }
        else
        {
            //Case: DIP Replace GE Rendering
            RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt;

            //DIPR win setting
            DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt;
            DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
            DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
            DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPRFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);

            if(RES_DMS.bDIPWriteFirstAddr)
            {
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPDoubleBufStartMemAddr;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPDoubleBufStartMemAddr;
            }
            else
            {
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPStartMemAddr;
                DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPStartMemAddr;
            }
        }
    }

#if 0
    DMS_PRINTF("\033[34m bufID %d, Fmt %d, width %d, height %d, pitch %d, Yaddr 0x%llx, Caddr 0x%llx \033[0m\n",u16BufferID,DIPRWinPropertytoShow_EX.stDIPRProperty.enDataFmt,
        DIPRWinPropertytoShow_EX.stDIPRProperty.u16Width,DIPRWinPropertytoShow_EX.stDIPRProperty.u16Height,DIPRWinPropertytoShow_EX.stDIPRProperty.u16Pitch,DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr,DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr);
    //DMS_PRINTF("\033[34m  ========== u32YBufAddr10Bits[0x%llx] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPRProperty.u32YBufAddr10Bits);
    //DMS_PRINTF("\033[34m  ========== u32CBufAddr10Bits[0x%llx] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPRProperty.u32CBufAddr10Bits);
    //DMS_PRINTF("\033[34m  ========== u32version[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.stMFDec_VerCtl.u32version);
    //DMS_PRINTF("\033[34m  ========== u32size[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.stMFDec_VerCtl.u32size);
    //DMS_PRINTF("\033[34m  ========== enMFDec_tile_mode[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.enMFDec_tile_mode);
    //DMS_PRINTF("\033[34m  ========== phyLuma_FB_Base[0x%llx] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.phyLuma_FB_Base);
    //DMS_PRINTF("\033[34m  ========== phyChroma_FB_Base[0x%llx] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.phyChroma_FB_Base);
    //DMS_PRINTF("\033[34m  ========== u16FB_Pitch[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16FB_Pitch);
    //DMS_PRINTF("\033[34m  ========== u16HSize[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16HSize);
    //DMS_PRINTF("\033[34m  ========== u16VSize[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16VSize);
    //DMS_PRINTF("\033[34m  ========== bMFDec_Enable[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bMFDec_Enable);
    //DMS_PRINTF("\033[34m  ========== u8MFDec_Select[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u8MFDec_Select);
    //DMS_PRINTF("\033[34m  ========== bUncompress_mode[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bUncompress_mode);
    //DMS_PRINTF("\033[34m  ========== bBypass_codec_mode[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.bBypass_codec_mode);
    //DMS_PRINTF("\033[34m  ========== en_MFDecVP9_mode[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.en_MFDecVP9_mode);
    //DMS_PRINTF("\033[34m  ========== u16StartX[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16StartX);
    //DMS_PRINTF("\033[34m  ========== u16StartY[%d] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16StartY);
    //DMS_PRINTF("\033[34m  ========== phyBitlen_Base[0x%llx] ========== \033[0m\n",DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.phyBitlen_Base);
    //DMS_PRINTF("\033[34m  ========== u16Bitlen_Pitch[%d] ========== \033[0m\n", DIPRWinPropertytoShow_EX.stDIPR_MFDecInfo.u16Bitlen_Pitch);
#endif

    if(MApi_XC_DIP_SetDIPRProperty_EX)
    {
        MApi_XC_DIP_SetDIPRProperty_EX(&DIPRWinPropertytoShow_EX, u8DIPWindowID);
    }
    else
    {
        MApi_XC_DIP_SetDIPRProperty(&DIPRWinPropertytoShow_EX.stDIPRProperty, u8DIPWindowID);
    }

    return TRUE;
}

static MS_BOOL _DMS_SetDIPWWindow(E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[eAction];
    ST_XC_DIP_WINPROPERTY DIPWinPropertytoShow;
    memset(&DIPWinPropertytoShow,0,sizeof(ST_XC_DIP_WINPROPERTY));

    DIPWinPropertytoShow.enSource = SCALER_DIP_SOURCE_TYPE_DRAM;
    DIPWinPropertytoShow.u8BufCnt = DMS_DIP_BUFFER_COUNT;

    if(eAction == E_DMS_DIP_NORMAL)
    {
        if(_DMS_Is_MVOPFlow(u32Window))
        {
            DIPWinPropertytoShow.u16Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth);
            if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width)
            {
                //scaling up
                DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width;
            }
            else
            {
                //scaling down
                DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
            }

            if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height > RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height)
            {
                //scaling up
                DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height;
            }
            else
            {
                //scaling down
                DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
            }

            if(_DMS_Query_XC_Status(u32Window, DMS_XC_FREEZE))
            {
                DIPWinPropertytoShow.u32BufStart = RES_DMS.u32MVOPFreezeAddress_pa;
                DIPWinPropertytoShow.u32BufEnd = RES_DMS.u32MVOPFreezeAddress_pa + (RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr - RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr);
            }
            else
            {
                if(RES_DMS.bDIPMVOPWriteFirstAddr)
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr;
                }
                else
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufEndMemAddr;
                }
            }
        }
        else
        {
            DIPWinPropertytoShow.u16Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);
            if(u16BufferID < DMS_Invalid_ReadPointer_ID)
            {
                if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
                {
#if (DMS_DIP_Support_HVSP == 0)
                    if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth / 2)
                    {
                        DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth / 2;
                    }
                    else
#endif
                    {
                        DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
                    }

#if (DMS_DIP_Support_HVSP == 0)
                    if(RES_DMS._stXC_DIPWinInfo[ u32Window].stDstWin.u32height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight / 2)
                    {
                        DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight / 2;
                    }
                    else
#endif
                    {
                        DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
                    }
                }
                else
                {
#if (DMS_DIP_Support_HVSP == 0)
                    if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth)
                    {
                        DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
                    }
                    else
#endif
                    {
                        DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
                    }

#if (DMS_DIP_Support_HVSP == 0)
                    if(RES_DMS._stXC_DIPWinInfo[ u32Window].stDstWin.u32height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight)
                    {
                        DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
                    }
                    else
#endif
                    {
                        DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
                    }
                }

#if (DMS_DIP_Support_HVSP == 0)
                RES_DMS.bNeedGEstretch = FALSE;

                if(((RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPWinInfo[ u32Window].stCropWin.u32width) ||
                    (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height > RES_DMS._stXC_DIPWinInfo[ u32Window].stCropWin.u32height)))
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[u32Window].u32GEStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[u32Window].u32GEEndMemAddr;
                    RES_DMS.bNeedGEstretch = TRUE;
                }
                else if(RES_DMS.bDIPWriteFirstAddr)
#else
                if(RES_DMS.bDIPWriteFirstAddr)
#endif
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPEndMemAddr;
                }
                else
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPDoubleBufStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[ u32Window].u32DIPDoubleBufEndMemAddr;
                }
            }
            else
            {
                DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
                DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;

                if(RES_DMS.bDIPWriteFirstAddr)
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr;
                }
                else
                {
                    DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufEndMemAddr;
                }
            }
        }
    }
    else if(eAction == E_DMS_DIP_CAPTURE)
    {
        DIPWinPropertytoShow.u16Pitch = _DMS_DIP_CalcPitch(_DMS_DIP_Capture_ColorFmtMapping(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enDIPDataFmt), RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width);
        if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth,RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
        {
#if (DMS_DIP_Support_HVSP == 0)
            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth / 2)
            {
                DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth / 2;
            }
            else
#endif
            {
                DIPWinPropertytoShow.u16Width = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width;
            }
#if (DMS_DIP_Support_HVSP == 0)
            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight / 2)
            {
                DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight / 2;
            }
            else
#endif
            {
                DIPWinPropertytoShow.u16Height = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height;
            }
        }
        else
        {
#if (DMS_DIP_Support_HVSP == 0)
            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth)
            {
                DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
            }
            else
#endif
            {
                DIPWinPropertytoShow.u16Width = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width;
            }

#if (DMS_DIP_Support_HVSP == 0)
            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight)
            {
                DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
            }
            else
#endif
            {
                DIPWinPropertytoShow.u16Height = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height;
            }
        }
/*
#if (DMS_DIP_Support_HVSP == 0)
        RES_DMS.bNeedGEstretch = FALSE;

        if(((RES_DMS._stXC_DIPCaptureWinInfo[u32Window].u32Width > RES_DMS._stXC_DIPWinInfo[ u32Window].stCropWin.u32width) ||
            (RES_DMS._stXC_DIPCaptureWinInfo[u32Window].u32Height > RES_DMS._stXC_DIPWinInfo[ u32Window].stCropWin.u32height)))
        {
            DIPWinPropertytoShow.u32BufStart = RES_DMS._stXC_DIPWinInfo[u32Window].u32GEStartMemAddr;
            DIPWinPropertytoShow.u32BufEnd = RES_DMS._stXC_DIPWinInfo[u32Window].u32GEEndMemAddr;
            RES_DMS.bNeedGEstretch = TRUE;
        }
        else
#endif
*/
        {
#if(DMS_DIP_Support_IMI == 0)
            switch(RES_DMSCAPTURE._stXC_DIPCaptureWin_RingBufferPointer[u32Window].u16WritePointer)
            {
                case 0:
                    DIPWinPropertytoShow.u32BufStart = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32StartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32EndMemAddr;
                    break;
                case 1:
                    DIPWinPropertytoShow.u32BufStart = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32SecondBufStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32SecondBufEndMemAddr;
                    break;
                case 2:
                    DIPWinPropertytoShow.u32BufStart = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32ThirdBufStartMemAddr;
                    DIPWinPropertytoShow.u32BufEnd = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32ThirdBufEndMemAddr;
                    break;
            }
#else
            DIPWinPropertytoShow.u32BufEnd = 0x123456;  //Temp use
#endif
        }
    }
    else
    {
        DIPWinPropertytoShow.u16Pitch = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth);
        if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width)
        {//scaling up
            DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width;
        }
        else
        {//scaling down
            DIPWinPropertytoShow.u16Width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
        }

        if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height > RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height)
        {//scaling up
            DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height;
        }
        else
        {//scaling down
            DIPWinPropertytoShow.u16Height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
        }

        MS_PHY phyDiBufSize = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr - RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr;
        DIPWinPropertytoShow.u32BufStart = _GetDIRingBufferAddr(u32Window,RES_DMSDI.u8DIRingBufferID);
        DIPWinPropertytoShow.u32BufEnd = DIPWinPropertytoShow.u32BufStart + phyDiBufSize;
    }

#if 0
    DMS_PRINTF("\033[35m width %d, height %d, pitch %d, bufStart 0x%llx, bufEnd 0x%llx \033[0m\n",
        DIPWinPropertytoShow.u16Width,DIPWinPropertytoShow.u16Height,DIPWinPropertytoShow.u16Pitch,DIPWinPropertytoShow.u32BufStart,DIPWinPropertytoShow.u32BufEnd);
#endif

    MApi_XC_DIP_SetDIPWinProperty(&DIPWinPropertytoShow, u8DIPWindowID);
    return TRUE;
}

static MS_BOOL _DMS_SetDIPWindow(E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[eAction];

    XC_SETWIN_INFO xc_dip_wininfotoShow;
    memset(&xc_dip_wininfotoShow,0,sizeof(XC_SETWIN_INFO));

    if(eAction != E_DMS_DIP_DI)
    {
        if(_DMS_Is_MVOPFlow(u32Window))
        {
            if(RES_DMS.gDMS_ScalingCondition & USE_2ND_BUFFER)
            {
                RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth = DMS_FHD_Hszie;
                RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight = DMS_FHD_Vszie;
            }
        }
    }

    // set DIP read window
    _DMS_SetDIPRWindow(eAction, u32Window, u16BufferID);

    if(eAction == E_DMS_DIP_CAPTURE)
    {
        // set tile mode
        //MApi_XC_DIP_Set420TileBlock(_DMS_DIP_TileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eTileMode),u8DIPWindowID);
        if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
        {
            MApi_XC_DIP_Set420TileBlock(_DMS_DIP_TileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u8Src2ndBufferTileMode),u8DIPWindowID);
        }
        else
        {
            // set tile mode
            MApi_XC_DIP_Set420TileBlock(_DMS_DIP_TileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eTileMode),u8DIPWindowID);
        }
        // set Capture output tile
        if(_DMS_DIP_Capture_ColorFmtMapping(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enDIPDataFmt) != DIP_DATA_FMT_YUV422)
        {
            MApi_XC_DIP_Set420TileBlock(_DMS_DIP_Capture_TileMapping(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enTileBlock),u8DIPWindowID);
        }

        xc_dip_wininfotoShow.stCapWin.x = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32x;
        xc_dip_wininfotoShow.stCapWin.y = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32y;
        if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth,RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
        {
            xc_dip_wininfotoShow.stCapWin.width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth / 2;
            xc_dip_wininfotoShow.stCapWin.height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight / 2;
        }
        else
        {
            xc_dip_wininfotoShow.stCapWin.width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
            xc_dip_wininfotoShow.stCapWin.height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
        }
        xc_dip_wininfotoShow.bPreHCusScaling = TRUE;
        xc_dip_wininfotoShow.u16PreHCusScalingSrc = xc_dip_wininfotoShow.stCapWin.width;
        xc_dip_wininfotoShow.u16PreHCusScalingDst = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width;

        xc_dip_wininfotoShow.bPreVCusScaling = TRUE;
        xc_dip_wininfotoShow.u16PreVCusScalingSrc = xc_dip_wininfotoShow.stCapWin.height;
        xc_dip_wininfotoShow.u16PreVCusScalingDst = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height;

        if( (RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth) ||
            (RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight) )
        {
#if (DMS_DIP_Support_HVSP == 1)
            MApi_XC_DIP_SetHVSP(TRUE, u8DIPWindowID);
#else
            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth)
            {
                xc_dip_wininfotoShow.u16PreHCusScalingDst = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
            }
            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight)
            {
                xc_dip_wininfotoShow.u16PreVCusScalingDst = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
            }
#endif
        }
#if (DMS_DIP_Support_HVSP == 1)
        else
        {
            MApi_XC_DIP_SetHVSP(FALSE, u8DIPWindowID);
        }
#endif
    }
    else
    {
        if(_DMS_IsCaptureWindowEnable(u32Window) &&
            RES_DMS.u8DIP_Window[E_DMS_DIP_CAPTURE] == RES_DMS.u8DIP_Window[E_DMS_DIP_NORMAL])
        {
            if(_DMS_Is_CaptureMode_OutputTile(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enTileBlock))
            {
                //Restore DIPW status
                MApi_XC_DIP_Set420TileBlock(DIP_TILE_BLOCK_W_16_32, u8DIPWindowID);
            }
        }
        if(eAction == E_DMS_DIP_NORMAL)
        {
            if(u16BufferID < DMS_Invalid_ReadPointer_ID)
            {
                if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth, RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
                {
                    MApi_XC_DIP_Set420TileBlock(_DMS_DIP_TileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u8Src2ndBufferTileMode),u8DIPWindowID);
                }
                else
                {
                    // set tile mode
                    MApi_XC_DIP_Set420TileBlock(_DMS_DIP_TileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eTileMode),u8DIPWindowID);
                }
            }
            else
            {
                // set tile mode to NONE
                MApi_XC_W2BYTEMSK(0x3616,0x0000,0x8000);
            }
        }
        else
        {
            // case E_DMS_DIP_DI, set tile mode
            MApi_XC_DIP_Set420TileBlock(_DMS_DIP_TileTypeMapping(RES_DMSDI._stXC_DIPDIInfo[u32Window][u16BufferID].eTileMode),u8DIPWindowID);
        }

        //Capture window
        if(_DMS_Is_MVOPFlow(u32Window))
        {
            //consider two case
            //1. P mode scaling down
            //    a. scaling from crop to dest, xc not scaling
            //2. I mode scaling down/up
            //    a. scaling from crop to dest, xc not scaling
            //    b. keep ratio crop to dest, xc scaling up
            xc_dip_wininfotoShow.stCapWin.x = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32x;
            xc_dip_wininfotoShow.stCapWin.y = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32y;
            xc_dip_wininfotoShow.stCapWin.width = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width;
            xc_dip_wininfotoShow.stCapWin.height = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height;

            xc_dip_wininfotoShow.bPreHCusScaling = TRUE;
            xc_dip_wininfotoShow.bPreVCusScaling = TRUE;

            _DMS_XC_SetCusScaling(&xc_dip_wininfotoShow,FALSE,
                RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width,
                (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width)? RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width : RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width,
                RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height,
                (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height > RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height)? RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height : RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height);
        }
        else
        {
            if(eAction == E_DMS_DIP_NORMAL)
            {
                if(u16BufferID < DMS_Invalid_ReadPointer_ID)
                {
                    xc_dip_wininfotoShow.stCapWin.x = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32x;
                    xc_dip_wininfotoShow.stCapWin.y = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32y;
                    if(_DMS_WindowSizeOverFHD(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth,RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight))
                    {
                        xc_dip_wininfotoShow.stCapWin.width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth / 2;
                        xc_dip_wininfotoShow.stCapWin.height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight / 2;
                    }
                    else
                    {
                        xc_dip_wininfotoShow.stCapWin.width = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
                        xc_dip_wininfotoShow.stCapWin.height = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
                    }
                    xc_dip_wininfotoShow.bPreHCusScaling = TRUE;
                    xc_dip_wininfotoShow.u16PreHCusScalingSrc = xc_dip_wininfotoShow.stCapWin.width;
                    xc_dip_wininfotoShow.u16PreHCusScalingDst = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;

                    xc_dip_wininfotoShow.bPreVCusScaling = TRUE;
                    xc_dip_wininfotoShow.u16PreVCusScalingSrc = xc_dip_wininfotoShow.stCapWin.height;
                    xc_dip_wininfotoShow.u16PreVCusScalingDst = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;

                    if( (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth) ||
                        (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight) )
                    {
#if (DMS_DIP_Support_HVSP == 1)
                        MApi_XC_DIP_SetHVSP(TRUE, u8DIPWindowID);
#else
                        if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth)
                        {
                            xc_dip_wininfotoShow.u16PreHCusScalingDst = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcWidth;
                        }
                        if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height > RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight)
                        {
                            xc_dip_wininfotoShow.u16PreVCusScalingDst = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].u16SrcHeight;
                        }
#endif
                    }
#if (DMS_DIP_Support_HVSP == 1)
                    else
                    {
                        MApi_XC_DIP_SetHVSP(FALSE, u8DIPWindowID);
                    }
#endif
                }
                else
                {
                    //Capture window
                    xc_dip_wininfotoShow.stCapWin.x = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32x;
                    xc_dip_wininfotoShow.stCapWin.y = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32y;
                    xc_dip_wininfotoShow.stCapWin.width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
                    xc_dip_wininfotoShow.stCapWin.height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;

                    xc_dip_wininfotoShow.bPreHCusScaling = FALSE;
                    xc_dip_wininfotoShow.u16PreHCusScalingSrc = xc_dip_wininfotoShow.stCapWin.width;
                    xc_dip_wininfotoShow.u16PreHCusScalingDst = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;

                    xc_dip_wininfotoShow.bPreVCusScaling = FALSE;
                    xc_dip_wininfotoShow.u16PreVCusScalingSrc = xc_dip_wininfotoShow.stCapWin.height;
                    xc_dip_wininfotoShow.u16PreVCusScalingDst = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
                }
            }
        }
    }

#if 0
    if(eAction == E_DMS_DIP_DI)
    {
        DMS_PRINTF("\033[34m  ========== Set Cap[%d %d %d %d] ========== \033[0m\n",xc_dip_wininfotoShow.stCapWin.x,xc_dip_wininfotoShow.stCapWin.y,
            xc_dip_wininfotoShow.stCapWin.width,xc_dip_wininfotoShow.stCapWin.height);
        DMS_PRINTF("\033[34m  ========== Set Scaling[%d %d %d %d] ========== \033[0m\n",xc_dip_wininfotoShow.u16PreHCusScalingSrc,xc_dip_wininfotoShow.u16PreHCusScalingDst,
            xc_dip_wininfotoShow.u16PreVCusScalingSrc,xc_dip_wininfotoShow.u16PreVCusScalingDst);
    }
    else
    {
        DMS_PRINTF("\033[34m  ========== Set Cap[%d %d %d %d] Tile %d -> %d ========== \033[0m\n",xc_dip_wininfotoShow.stCapWin.x,xc_dip_wininfotoShow.stCapWin.y,
            xc_dip_wininfotoShow.stCapWin.width,xc_dip_wininfotoShow.stCapWin.height,
            RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eTileMode,
            _DMS_DIP_TileTypeMapping(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].eTileMode));
        DMS_PRINTF("\033[34m  ========== Set Scaling[%d %d %d %d] ========== \033[0m\n",xc_dip_wininfotoShow.u16PreHCusScalingSrc,xc_dip_wininfotoShow.u16PreHCusScalingDst,
            xc_dip_wininfotoShow.u16PreVCusScalingSrc,xc_dip_wininfotoShow.u16PreVCusScalingDst);
    }
#endif

    MApi_XC_SkipWaitVsync(MAIN_WINDOW,TRUE);
    MApi_XC_DIP_SetWindow(&xc_dip_wininfotoShow,sizeof(XC_SETWIN_INFO),u8DIPWindowID);
    MApi_XC_SkipWaitVsync(MAIN_WINDOW,FALSE);

    // set DIP write window
    _DMS_SetDIPWWindow(eAction, u32Window, u16BufferID);
    return TRUE;
}

////MVOP XC flow
#define FLOW_CONDITION_4K           0x01
#define FLOW_CONDITION_INTERLACE    0x02
#define FLOW_CONDITION_SHRINK       0x04
#define CONDITION_4K_WIDTH          3840
#define CONDITION_4K_HEIGHT         2160
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_U8 _DMS_MVOP_Set_Flow(MS_U8 u8Id)
{
    DMS_GET_RES_PRI;
    MS_U8 u8FlowCondition = 0;
    MS_BOOL bMVOPFromDIPDi = FALSE;
    if(RES_DMS._stXC_XCCropWinInfo.u32width >= CONDITION_4K_WIDTH && RES_DMS._stXC_XCCropWinInfo.u32height >= CONDITION_4K_HEIGHT)
    {
        u8FlowCondition |= FLOW_CONDITION_4K;
    }
    if((u8Id < DMS_Invalid_ReadPointer_ID) && (_DMS_Is_SourceInterlace(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u8Id].eScanType) == TRUE))
    {
        u8FlowCondition |= FLOW_CONDITION_INTERLACE;
    }
    if((RES_DMS._stXC_XCCropWinInfo.u32width > RES_DMS._stXC_XCDispWinInfo.u32width) &&
        (RES_DMS._stXC_XCCropWinInfo.u32height > RES_DMS._stXC_XCDispWinInfo.u32height))
    {
        u8FlowCondition |= FLOW_CONDITION_SHRINK;
    }

    if(u8FlowCondition & FLOW_CONDITION_4K)
    {
        //4K force from VDEC
        RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_VDEC;
    }
    else if(u8FlowCondition & FLOW_CONDITION_INTERLACE)
    {
        //Interlace force From DIP
        RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_DIP;
    }
    else
    {
        //P mode
        if(u8FlowCondition & FLOW_CONDITION_SHRINK)
        {
            //P mode shrink, from DIP
            //4K scaling down case contain in 4K case, use from VDEC
            RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_DIP;
        }
        else
        {
            //P mode enlarge or the same, from VDEC
            RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_VDEC;
        }
    }

    if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE))
    {
        DMS_PRINTF("[%s] freeze setting to DIP to MVOP flow\n",__FUNCTION__);
        RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_DIP;
    }

#if DEV_DI
    if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u8Id].bIsAfterDiTask == TRUE)
    {
        DMS_PRINTF("[%s] Get After Di Buffer\n",__FUNCTION__);
        RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_DIP;
        bMVOPFromDIPDi = TRUE;
    }
#endif

    RES_DMS.bDMS_MVOPFlowFromDi = bMVOPFromDIPDi;
    return u8FlowCondition;
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_BOOL _DMS_MVOP_SetConfig(MVOP_InputSel eInputCfg, MS_BOOL bYUV422, MS_U8 u8MIUSel)
{
    DMS_GET_RES_PRI;
    MVOP_VidStat stMvopVidSt;
    MVOP_InputCfg stMvopInputCfg;

    // Structure initialization
    memset(&stMvopVidSt, 0, sizeof(MVOP_VidStat));
    memset(&stMvopInputCfg, 0, sizeof(MVOP_InputCfg));

    MDrv_MVOP_Init();
    MDrv_MVOP_Enable(FALSE); // Turn off MVOP before setting it
    if(E_MVOP_OK != MDrv_MVOP_MiuSwitch(u8MIUSel))
    {
        DMS_PRINTF("Cannot set MVOP MIU selection!\n");
        return FALSE;
    }

    //For IFrame-Decoder
    if(eInputCfg == MVOP_INPUT_DRAM)
    {
        stMvopInputCfg.u16HSize = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth;
        stMvopInputCfg.u16VSize = 1080;
        stMvopInputCfg.u32YOffset = 0;
        stMvopInputCfg.u32UVOffset = 0;
        stMvopInputCfg.bProgressive = TRUE;
        stMvopInputCfg.bUV7bit = FALSE;
        stMvopInputCfg.bField = FALSE;
        stMvopInputCfg.u16StripSize = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth;
        if (bYUV422 == TRUE)
        {
            stMvopInputCfg.bSD = FALSE;
            stMvopInputCfg.bYUV422 = FALSE;
            stMvopInputCfg.bDramRdContd = FALSE;
            stMvopInputCfg.b422pack = FALSE;
            //For H264 IFrame.
            MDrv_MVOP_SetTileFormat(E_MVOP_TILE_16x32);
        }
        else
        {
            stMvopInputCfg.bSD = TRUE;
            stMvopInputCfg.bYUV422 = TRUE;
            stMvopInputCfg.bDramRdContd = TRUE;
            stMvopInputCfg.b422pack = TRUE;
        }
    }

    if(E_MVOP_OK != MDrv_MVOP_SetInputCfg(eInputCfg, &stMvopInputCfg))
    {
        DMS_PRINTF("Set MVOP input config failed!");
        return FALSE;
    }

    MS_BOOL bHS = TRUE;
    MS_BOOL bXCGen = TRUE;
    MVOP_Handle stMvopHd = { E_MVOP_MODULE_MAIN };
    MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_HANDSHAKE_MODE, &bHS);
    MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_XC_GEN_TIMING, &bXCGen);

    stMvopVidSt.u16HorSize = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth;
    stMvopVidSt.u16VerSize = RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth;
    stMvopVidSt.u16FrameRate = g_IPanel.DefaultVFreq()*100;
    stMvopVidSt.u8AspectRate = 0;
    stMvopVidSt.u8Interlace = FALSE;

    if(E_MVOP_OK != MDrv_MVOP_SetOutputCfg(&stMvopVidSt, FALSE))
    {
        DMS_PRINTF("Set MVOP output config failed!");
        return FALSE;
    }
    MDrv_MVOP_Enable(TRUE); // Enable MVOP, video pass to Scaler
    return TRUE;
}
#endif
static MS_BOOL _DMS_XC_SetConnect(INPUT_SOURCE_TYPE_t eInputSrcType)
{
    MS_S16 s16PathId;
    MS_U8 u8Count = 0;
    XC_MUX_PATH_INFO PathInfo;
    XC_MUX_PATH_INFO AllPathInfo[MAX_SYNC_DATA_PATH_SUPPORTED];

    memset(&PathInfo, 0, sizeof(XC_MUX_PATH_INFO));
    PathInfo.Path_Type = PATH_TYPE_SYNCHRONOUS;
    PathInfo.src = eInputSrcType;
    PathInfo.dest = OUTPUT_SCALER_MAIN_WINDOW;
    PathInfo.SyncEventHandler = NULL;
    PathInfo.DestOnOff_Event_Handler = NULL;
    PathInfo.path_thread = NULL;
    PathInfo.dest_periodic_handler = NULL; //[FIXME] PQ_Adaptive

    memset(AllPathInfo, 0, sizeof(XC_MUX_PATH_INFO)*MAX_SYNC_DATA_PATH_SUPPORTED);
    u8Count = MApi_XC_Mux_GetPathInfo(AllPathInfo);

    if(u8Count > 0)
    {
        do{
            u8Count = u8Count - 1;
            if(AllPathInfo[u8Count].dest == PathInfo.dest)
            {
                if(AllPathInfo[u8Count].src == (eInputSrcType))
                {
                    DMS_PRINTF("eInputSrc %u was already connected to %u.\n", eInputSrcType, PathInfo.dest);
                    return TRUE;
                }
                else
                {
                    if(-1 == MApi_XC_Mux_DeletePath(AllPathInfo[u8Count].src, AllPathInfo[u8Count].dest))
                    {
                        DMS_PRINTF(" Delete path fail: src = %d, dest = %d\n", AllPathInfo[u8Count].src, AllPathInfo[u8Count].dest);
                    }
                    break;
                }
            }
        }while(u8Count>0);
    }

    s16PathId = MApi_XC_Mux_CreatePath(&PathInfo, sizeof(XC_MUX_PATH_INFO));
    if (s16PathId == -1)
    {
        DMS_PRINTF(" Create path fail, src = %d, dest = %d, your structure has wrong size with library \n", PathInfo.src, PathInfo.dest );
    }
    else
    {
        MApi_XC_Mux_EnablePath((MS_U16)s16PathId);
    }

    MApi_XC_SetInputSource(eInputSrcType, MAIN_WINDOW);
    MApi_XC_DisableInputSource(DISABLE, MAIN_WINDOW);
    return TRUE;
}

static MS_BOOL _DMS_XC_SetDisconnect(INPUT_SOURCE_TYPE_t eInputSrcType)
{
    E_DEST_TYPE eDest = OUTPUT_SCALER_MAIN_WINDOW;

    if(-1 == MApi_XC_Mux_DeletePath(eInputSrcType, eDest))
    {
          DMS_PRINTF(" [%s][%d]Delete path fail\n", __FUNCTION__, __LINE__);
          return FALSE;
    }

    return TRUE;
}

//Wait Apple's API
static void _DMS_MVOP_MFDEC_Patch(MS_BOOL bMFDecEnable)
{
    DMS_GET_RES_PRI;
    if(bMFDecEnable)
    {
        MApi_XC_Write2ByteMask(0x1014A8, 0x01,0x01); //enable
        MApi_XC_Write2ByteMask(0x1014AE,(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].stMFdecInfo.phyBitlen_Base >> 3) & 0xFFFF,0xFFFF);
        MApi_XC_Write2ByteMask(0x1014B0,((RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].stMFdecInfo.phyBitlen_Base >> 3)>>16) & 0x0FFF,0x0FFF);
        MApi_XC_Write2ByteMask(0x1014BC,(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].stMFdecInfo.u16Bitlen_Pitch << 8), 0xFF00);
        MApi_XC_Write2ByteMask(0x101450, 0x20, 0x20); //h264 nfdec need 32x32
    }
    else
    {
        MApi_XC_Write2ByteMask(0x1014A8, 0x00,0x01); //disable
        MApi_XC_Write2ByteMask(0x101450, 0x00, 0x20); //undo h264 nfdec need 32x32
    }
}
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_U16 _DMS_MVOP_CalcPitch(MS_U16 u16Width)
{
    DMS_GET_RES_PRI;
    MS_U16 u16Pitch = 0;
    DMS_PRINTF("[%s] u16SrcWidth: %d\n",__FUNCTION__,u16Width);
    if(u16Width % 32 != 0)
    {
        u16Width = (((u16Width)/(32)+1) * (32));
        DMS_PRINTF("[%s] u16SrcWidth align to : %d\n",__FUNCTION__,u16Width);
    }
    if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
    {
        //YUV420 pitch is Width / 8
        u16Pitch = u16Width / 8;
    }
    else
    {
        //YUV422 pitch is Width / 4
        u16Pitch = u16Width / 4;
    }
    return u16Pitch;
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static void _DMS_MVOP_SetPitch(MS_U16 u16Width)
{
    MS_U16 u16Pitch = _DMS_MVOP_CalcPitch(u16Width);
    MApi_XC_Write2ByteMask(0x101440,((u16Pitch & 0xFF) << 8),0xFF00);
    MApi_XC_WriteByteMask(0x101430,(u16Pitch >> 8),0x03);
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static void _DMS_MVOP_SwitchPath(MS_U8 u8Id)
{
    DMS_GET_RES_PRI;
    MApi_XC_WriteByteMask(0x10144E,0x10,0x10);

    if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
    {
        //MVOP Pitch from VDEC need get vdec source size
        //_DMS_MVOP_SetPitch(_stXC_XCCropWinInfo.width);
        _DMS_MVOP_SetPitch(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u8Id].u16SrcPitch);
        MApi_XC_Write2ByteMask(0x101440,0x00,0xB0);
    }
    else
    {
        //MVOP Pitch is DIP write pitch = DIPWinInfo.stDstWin
        _DMS_MVOP_SetPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth);
        MApi_XC_Write2ByteMask(0x101440,0xB0,0xB0);
    }
    MApi_XC_WriteByteMask(0x10144E,0x00,0x10);
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static void _DMS_DIP_SyncStoredSetting(void)
{
    DMS_GET_RES_PRI;
    memcpy(&RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stCropWin,&RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stCropWin,sizeof(ST_DMS_WINDOW));
    memcpy(&RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stDstWin,&RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin,sizeof(ST_DMS_WINDOW));
    memcpy(&RES_DMS._stXC_XCDispWinInfo,&RES_DMS._stXC_XCDispWinInfo_DoubleBuffer,sizeof(ST_DMS_WINDOW));
    memcpy(&RES_DMS._stXC_XCCropWinInfo,&RES_DMS._stXC_XCCropWinInfo_DoubleBuffer,sizeof(ST_DMS_WINDOW));
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DIPStartMemAddr = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPStartMemAddr;
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DIPEndMemAddr = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPEndMemAddr;
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DIPDoubleBufStartMemAddr = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPDoubleBufStartMemAddr;
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32DIPDoubleBufEndMemAddr = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPDoubleBufEndMemAddr;
    MS_U8 i = 0;
    for(i = 0; i < DMS_DI_RING_BUFFER; i++)
    {
        RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].phyDIPRingBufMemAddr[i] = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.phyDIPRingBufMemAddr[i];
    }
    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].u32RingBufferSize = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32RingBufferSize;
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_BOOL _DMS_XC_SetWindow_MVOP_SetPath(void* pInstance, MS_U16 u16ID)
{
    DMS_GET_RES_PRI;
    _DMS_DIP_SyncStoredSetting();
    MS_WINDOW_TYPE stMVOPWindow; //decribe how to set MVOP crop
    XC_SETWIN_INFO stXCSetWinInfo;
    memset(&stXCSetWinInfo, 0, sizeof(XC_SETWIN_INFO));

    stXCSetWinInfo.enInputSourceType = INPUT_SOURCE_DTV;
    MVOP_Timing mvop_timing;
    memset(&mvop_timing, 0, sizeof(MVOP_Timing));
    if(E_MVOP_OK != MDrv_MVOP_GetOutputTiming(&mvop_timing))
        DMS_PRINTF("MDrv_MVOP_GetOutputTiming failed!!\n");

    stXCSetWinInfo.bHDuplicate    = mvop_timing.bHDuplicate;
    stXCSetWinInfo.u16InputVTotal = mvop_timing.u16V_TotalCount;
    stXCSetWinInfo.u16DefaultHtotal = mvop_timing.u16H_TotalCount;
    stXCSetWinInfo.bInterlace     = mvop_timing.bInterlace;

    stXCSetWinInfo.u16InputVFreq = g_IPanel.DefaultVFreq();
    //cap Window
    stXCSetWinInfo.stCapWin.x = 0;
    stXCSetWinInfo.stCapWin.y = 0;
    stXCSetWinInfo.stCapWin.width = RES_DMS._stXC_XCCropWinInfo.u32width;
    stXCSetWinInfo.stCapWin.height = RES_DMS._stXC_XCCropWinInfo.u32height;

    //Crop / disp Window
    stXCSetWinInfo.stCropWin.x = 0;
    stXCSetWinInfo.stCropWin.y = 0;
    stXCSetWinInfo.stCropWin.width = RES_DMS._stXC_XCCropWinInfo.u32width;
    stXCSetWinInfo.stCropWin.height = RES_DMS._stXC_XCCropWinInfo.u32height;

    stXCSetWinInfo.stDispWin.x = RES_DMS._stXC_XCDispWinInfo.u32x;
    stXCSetWinInfo.stDispWin.y = RES_DMS._stXC_XCDispWinInfo.u32y;
    stXCSetWinInfo.stDispWin.width = RES_DMS._stXC_XCDispWinInfo.u32width;
    stXCSetWinInfo.stDispWin.height = RES_DMS._stXC_XCDispWinInfo.u32height;

    //-------------------------
    // customized pre scaling
    //-------------------------
    stXCSetWinInfo.bPreHCusScaling = TRUE;
    stXCSetWinInfo.u16PreHCusScalingSrc = RES_DMS._stXC_XCCropWinInfo.u32width;
    stXCSetWinInfo.u16PreHCusScalingDst = RES_DMS._stXC_XCCropWinInfo.u32width;
    stXCSetWinInfo.bPreVCusScaling = TRUE;
    stXCSetWinInfo.u16PreVCusScalingSrc = RES_DMS._stXC_XCCropWinInfo.u32height;
    stXCSetWinInfo.u16PreVCusScalingDst = RES_DMS._stXC_XCCropWinInfo.u32height;

    //customized Post scaling
    stXCSetWinInfo.bHCusScaling = TRUE;
    stXCSetWinInfo.bVCusScaling = TRUE;
    RES_DMS.eDMS_PreMVOPFlowCtrl = RES_DMS.eDMS_MVOPFlowCtrl;
    MS_U8 u8FlowCondition = _DMS_MVOP_Set_Flow(u16ID);
    DMS_PRINTF("\033[1;36m[%s] 4K:%s(2ndBuf:%s) I:%s shrink:%s(XC90:%s), Flow %s, ID %d\033[0m\n",__FUNCTION__,(u8FlowCondition & FLOW_CONDITION_4K)? "YES" : "NO",
        (RES_DMS.gDMS_ScalingCondition & USE_2ND_BUFFER)? "YES" : "NO",
        (u8FlowCondition & FLOW_CONDITION_INTERLACE)? "YES" : "NO",
        (u8FlowCondition & FLOW_CONDITION_SHRINK)? "YES" : "NO",
        (RES_DMS.gDMS_ScalingCondition & USE_XC_SCALING_DOWN)? "YES" : "NO",
        (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)? "From VDEC" : "From DIP",
        u16ID);

    if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
    {
        //Enlarge or the same case or XC force scaling down 4K to 90%up
        stMVOPWindow.x = RES_DMS._stXC_XCCropWinInfo.u32x;
        stMVOPWindow.y = RES_DMS._stXC_XCCropWinInfo.u32y;
        stMVOPWindow.width = RES_DMS._stXC_XCCropWinInfo.u32width;
        stMVOPWindow.height = RES_DMS._stXC_XCCropWinInfo.u32height;
        _DMS_XC_SetCusScaling(&stXCSetWinInfo,TRUE,
            RES_DMS._stXC_XCCropWinInfo.u32width, RES_DMS._stXC_XCDispWinInfo.u32width,
            RES_DMS._stXC_XCCropWinInfo.u32height, RES_DMS._stXC_XCDispWinInfo.u32height);
    }
    else
    {
        //From DIP
        stMVOPWindow.x = stMVOPWindow.y = 0;
        if(u8FlowCondition & FLOW_CONDITION_SHRINK)
        {
            stMVOPWindow.width = RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stDstWin.u32width;
            stMVOPWindow.height = RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stDstWin.u32height;
            if(RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stDstWin.u32width < RES_DMS._stXC_XCDispWinInfo.u32width)
            {
                //Shrink but for DIP pixel alignment, need enlarge a little by XC case
                _DMS_XC_SetCusScaling(&stXCSetWinInfo,TRUE,
                    RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stDstWin.u32width, RES_DMS._stXC_XCDispWinInfo.u32width,
                    RES_DMS._stXC_XCDispWinInfo.u32height, RES_DMS._stXC_XCDispWinInfo.u32height);
            }
            else
            {
                _DMS_XC_SetCusScaling(&stXCSetWinInfo,TRUE,
                    RES_DMS._stXC_XCDispWinInfo.u32width, RES_DMS._stXC_XCDispWinInfo.u32width,
                    RES_DMS._stXC_XCDispWinInfo.u32height, RES_DMS._stXC_XCDispWinInfo.u32height);
            }
        }
        else
        {
            //From DIP but enlarge => DIP do Di and XC enlarge
            stMVOPWindow.width = RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stCropWin.u32width;
            stMVOPWindow.height = RES_DMS._stXC_DIPWinInfo[MVOP_WINDOW].stCropWin.u32height;
            _DMS_XC_SetCusScaling(&stXCSetWinInfo,TRUE,
                RES_DMS._stXC_XCCropWinInfo.u32width, RES_DMS._stXC_XCDispWinInfo.u32width,
                RES_DMS._stXC_XCCropWinInfo.u32height, RES_DMS._stXC_XCDispWinInfo.u32height);
        }
    }
#if DBG_DMS_LOG
    DMS_PRINTF("[%s] total: %d %d\n",__FUNCTION__,stXCSetWinInfo.u16DefaultHtotal,stXCSetWinInfo.u16InputVTotal);
    DMS_PRINTF("[%s] bInterlace: %d, VFreq %d\n",__FUNCTION__,stXCSetWinInfo.bInterlace,stXCSetWinInfo.u16InputVFreq);
    DMS_PRINTF("[%s] MVOP: %d %d, %d %d\n",__FUNCTION__,stMVOPWindow.x,stMVOPWindow.y,stMVOPWindow.width,stMVOPWindow.height);
    DMS_PRINTF("[%s] Capt: %d %d, %d %d\n",__FUNCTION__,stXCSetWinInfo.stCapWin.x,stXCSetWinInfo.stCapWin.y,stXCSetWinInfo.stCapWin.width,stXCSetWinInfo.stCapWin.height);
    DMS_PRINTF("[%s] Crop: %d %d, %d %d\n",__FUNCTION__,stXCSetWinInfo.stCropWin.x,stXCSetWinInfo.stCropWin.y,stXCSetWinInfo.stCropWin.width,stXCSetWinInfo.stCropWin.height);
    DMS_PRINTF("[%s] Disp: %d %d, %d %d\n",__FUNCTION__,stXCSetWinInfo.stDispWin.x,stXCSetWinInfo.stDispWin.y,stXCSetWinInfo.stDispWin.width,stXCSetWinInfo.stDispWin.height);
    DMS_PRINTF("[%s] Pre : %d -> %d, %d -> %d\n",__FUNCTION__,stXCSetWinInfo.u16PreHCusScalingSrc,stXCSetWinInfo.u16PreHCusScalingDst,stXCSetWinInfo.u16PreVCusScalingSrc,stXCSetWinInfo.u16PreVCusScalingDst);
    DMS_PRINTF("[%s] Post: %d -> %d, %d -> %d\n",__FUNCTION__,stXCSetWinInfo.u16HCusScalingSrc,stXCSetWinInfo.u16HCusScalingDst,stXCSetWinInfo.u16VCusScalingSrc,stXCSetWinInfo.u16VCusScalingDst);
#endif
    if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FIRST_TIME_SETWINDOW))
    {
        MDrv_MVOP_SetStartPos(stMVOPWindow.x, stMVOPWindow.y);
        //[FIXME] C2 MVOP Driver Bug, H need set Y + H, wait Apple Fix
        MDrv_MVOP_SetImageWidthHight(stMVOPWindow.width, stMVOPWindow.height + stMVOPWindow.y);

        MDrv_MVOP_SetRegSizeFromMVD(FALSE);
        _DMS_MVOP_SwitchPath(RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]);

        DMS_PRINTF("[%s] Setwin: %d %d %d %d %d\n",__FUNCTION__,stXCSetWinInfo.bHDuplicate,stXCSetWinInfo.u16InputVTotal,
            stXCSetWinInfo.u16DefaultHtotal,stXCSetWinInfo.bInterlace,stXCSetWinInfo.u16InputVFreq);

        DMS_PRINTF("[%s] Setwin: (%d %d %d %d) (%d %d %d %d) (%d %d %d %d)\n",__FUNCTION__,stXCSetWinInfo.stCapWin.x,stXCSetWinInfo.stCapWin.y,
            stXCSetWinInfo.stCapWin.width,stXCSetWinInfo.stCapWin.height,stXCSetWinInfo.stCropWin.x,stXCSetWinInfo.stCropWin.y,
            stXCSetWinInfo.stCropWin.width,stXCSetWinInfo.stCropWin.height,stXCSetWinInfo.stDispWin.x,stXCSetWinInfo.stDispWin.y,
            stXCSetWinInfo.stDispWin.width,stXCSetWinInfo.stDispWin.height);

        MApi_XC_SkipWaitVsync(MAIN_WINDOW,TRUE);
        MApi_XC_SetWindow(&stXCSetWinInfo, sizeof(XC_SETWIN_INFO), MAIN_WINDOW);
        MApi_XC_SkipWaitVsync(MAIN_WINDOW,FALSE);
///Set XC for Virtual Box size
        MApi_XC_W2BYTE(REG_SC_BK12_0F_L,0x1000);
        MApi_XC_W2BYTE(REG_SC_BK12_17_L,0x1000);
        _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_FIRST_TIME_SETWINDOW);
        HAL_DMS_XC_DSSetConfig(pInstance);
        if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
        {
            DMS_PRINTF("[%s] u16SrcWidth: %d %d\n",__FUNCTION__,u16ID,RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u16ID].u16SrcPitch);
            HAL_DMS_XC_DSSetMVOPPitch(pInstance, _DMS_MVOP_CalcPitch(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u16ID].u16SrcPitch));
            //HAL_DMS_XC_DSSetMVOPPitch(_DMS_MVOP_CalcPitch(stMVOPWindow.width));
        }
        else
        {
            HAL_DMS_XC_DSSetMVOPPitch(pInstance, _DMS_MVOP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth));
        }
        HAL_DMS_XC_DSSetWin(pInstance, &stMVOPWindow, &stXCSetWinInfo,RES_DMS.eDMS_MVOPFlowCtrl);
    }
    else
    {
        //DS setwin
        if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
        {
            if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_SIZE))
            {
                DMS_PRINTF("[%s] VDEC SrcWidth: %d (to id %d) %d\n",__FUNCTION__,u16ID,RES_DMS.gu8DMS_Seq_Change_ID,RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gu8DMS_Seq_Change_ID].u16SrcPitch);
                HAL_DMS_XC_DSSetMVOPPitch(pInstance, _DMS_MVOP_CalcPitch(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gu8DMS_Seq_Change_ID].u16SrcPitch));
            }
            else
            {
                DMS_PRINTF("[%s] SrcWidth: %d %d\n",__FUNCTION__,u16ID,RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u16ID].u16SrcPitch);
                HAL_DMS_XC_DSSetMVOPPitch(pInstance, _DMS_MVOP_CalcPitch(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u16ID].u16SrcPitch));
            }
        }
        else
        {
            DMS_PRINTF("[%s] DIP SrcWidth: %d (to id %d) %d\n",__FUNCTION__,u16ID,RES_DMS.gu8DMS_Seq_Change_ID,RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth);
            HAL_DMS_XC_DSSetMVOPPitch(pInstance, _DMS_MVOP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth));
        }
        HAL_DMS_XC_DSSetWin(pInstance, &stMVOPWindow, &stXCSetWinInfo,RES_DMS.eDMS_MVOPFlowCtrl);
    }

    return TRUE;
}
#endif
////MVOP XC flow
static MS_BOOL _DMS_DIP_Fire(E_DMS_DIP_ACTION eAction)
{
    DMS_GET_RES_PRI;
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[eAction];

    //enable DIP interrupt
    MApi_XC_DIP_EnaInt(BMASK((DMS_DIP_BUFFER_COUNT-1):0)&0xFF, TRUE, u8DIPWindowID);

    MApi_XC_DIP_ClearInt(BMASK((DMS_DIP_BUFFER_COUNT-1):0)&0xFF, u8DIPWindowID);

    // Enable DIPW
    MApi_XC_DIP_Ena(TRUE,u8DIPWindowID);

    // Enable DIPR softwave trigger
    MApi_XC_DIP_CapOneFrameFast(u8DIPWindowID);
    return TRUE;
}

#if (DMS_DIP_Support_HVSP == 0)
static MS_BOOL _DMS_NeedGEstretch(void* pInstance, MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    if(RES_DMS.bNeedGEstretch)
    {
        GFX_DrawRect stgfxDrawRect;
        stgfxDrawRect.srcblk.x = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32x;
        stgfxDrawRect.srcblk.y = RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32y;
        stgfxDrawRect.srcblk.width = (RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width> RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width)?
            RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width: RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32width;
        stgfxDrawRect.srcblk.height = (RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height> RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height)?
            RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height: RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin.u32height;

        stgfxDrawRect.dstblk.x = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x;
        stgfxDrawRect.dstblk.y = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y;
        stgfxDrawRect.dstblk.width = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
        stgfxDrawRect.dstblk.height = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
        _DMS_GEBitblt(RES_DMS._stXC_DIPWinInfo[u32Window].u32GEStartMemAddr,
            _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, _DMS_DIP_GetBackgroundBufID(pInstance)),
            &stgfxDrawRect);
    }
    return TRUE;
}
#endif

static MS_BOOL _DMS_DIP_WaitWriteDone(E_DMS_DIP_ACTION eAction)
{
    DMS_GET_RES_PRI;
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[eAction];

    MS_U32 u32StartTime = 0;
    u32StartTime = MsOS_GetSystemTime();
    while(MApi_XC_DIP_GetIntStatus(u8DIPWindowID) == 0)
    {
        if(MsOS_GetSystemTime() - u32StartTime > DMS_DIP_TIMEOUT)
        {
            DMS_PRINTF("\033[1;31m[%s] DIP %d Timeout!!!!!!!!!!!!!!!! \033[0m\n",__FUNCTION__, u8DIPWindowID);
            return FALSE;
        }
        MsOS_DelayTask(1);
    }
    return TRUE;
}

static void _DMS_DIP_Flow_PrepareNextFrame(void* pInstance, E_DMS_DIP_ACTION eAction, MS_U32 u32Window, MS_U16 u16BufferID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[eAction];

    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    while(MApi_XC_DIP_GetResource(u8DIPWindowID) == E_APIXC_RET_FAIL)
    {
        MsOS_DelayTask(1);
    }

    //Disable BK1336 reg3[8]
    MApi_XC_W2BYTEMSK(REG_SC_BK36_03_L,0x0000,0x0100);

    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);

    if(eAction == E_DMS_DIP_CAPTURE)
    {
        if(_DMS_Is_UVSwap(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enColorFormat))
        {
            MApi_XC_DIP_SwapUV(TRUE ,u8DIPWindowID);
        }
        else
        {
            MApi_XC_DIP_SwapUV(FALSE ,u8DIPWindowID);
        }

        if(_DMS_Is_YCSwap(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enColorFormat))
        {
            MApi_XC_DIP_SwapYC(TRUE ,u8DIPWindowID);
        }
        else
        {
            MApi_XC_DIP_SwapYC(FALSE ,u8DIPWindowID);
        }

        MApi_XC_DIP_SetOutputDataFmt(_DMS_DIP_Capture_ColorFmtMapping(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].enDIPDataFmt), u8DIPWindowID);
    }
    else
    {
        if(1)//_DMS_Is_MVOPFlow(u32Window) == FALSE)
        {
            MApi_XC_DIP_SwapUV(TRUE ,u8DIPWindowID);
        }
        else
        {
            MApi_XC_DIP_SwapUV(FALSE ,u8DIPWindowID);
        }

        if(_DMS_Is_MVOPFlow(u32Window) == FALSE)
        {
            MApi_XC_DIP_SetOutputDataFmt(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].eDIPWFmt, u8DIPWindowID);
        }
        else
        {
            MApi_XC_DIP_SetOutputDataFmt(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, u8DIPWindowID);
        }
    }

    MApi_XC_DIP_EnableR2Y(FALSE,u8DIPWindowID);
    MApi_XC_DIP_EnableY2R(FALSE,u8DIPWindowID);

    // set DIP read/write window
    _DMS_SetDIPWindow(eAction, u32Window, u16BufferID);

#if DMS_DIP_Support_IMI
    if(eAction == E_DMS_DIP_CAPTURE)
    {
        MS_BOOL bEnable = TRUE;
        MApi_XC_DIP_SetConfig(E_XC_DIP_IMI_ENABLE, &bEnable, u8DIPWindowID);
    }
#endif

    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    // enable DIP
    _DMS_DIP_Fire(eAction);

#if DMS_DIP_Support_IMI
    if(eAction == E_DMS_DIP_CAPTURE)
    {
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        RES_DMSCAPTURE._stXC_IMICaptureInfo[u32Window].bCaptureRet = TRUE;
        if(RES_DMSCAPTURE.bIsFrameValid[u32Window][u16BufferID] == TRUE)
        {
            if(RES_DMSCAPTURE.u8frameRepeatCnt[u32Window][u16BufferID] == 0)
            {
                RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u16PreviousWP = u16BufferID;
            }
        }
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    }
#endif

    // Wait DIP write done
    _DMS_DIP_WaitWriteDone(eAction);

    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
#if (DMS_DIP_Support_HVSP == 0)
    if(_DMS_Is_MVOPFlow(u32Window) == FALSE)
    {
        _DMS_NeedGEstretch(pInstance, u32Window);
    }
#endif

#if DEV_PAUSE
    //if((_DMS_Is_MVOPFlow(u32Window) == TRUE) && (eAction == E_DMS_DIP_NORMAL))
    if(_DMS_Is_MVOPFlow(u32Window) == TRUE)
    {
        if(RES_DMS.gDMS_MVOPShowBufferID != DMS_Invalid_ReadPointer_ID)
        {
            RES_DMS.gDMS_MVOPRemoveBufferID = RES_DMS.gDMS_MVOPShowBufferID;
        }
        RES_DMS.gDMS_MVOPShowBufferID = u16BufferID;
    }
#else
    //if((_DMS_Is_MVOPFlow(u32Window) == TRUE) && (eAction == E_DMS_DIP_NORMAL))
    if(_DMS_Is_MVOPFlow(u32Window) == TRUE)
    {
        RES_DMS._stXC_DIPPushInfo[u32Window][u16BufferID].bValid = FALSE;

        _DMS_VDEC_Frame_Release(u32Window,u16BufferID);
    }
#endif
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
#if (DMS_DIP_Support_IMI == 0)
    MApi_XC_DIP_Ena(FALSE,u8DIPWindowID);
#endif
    MApi_XC_DIP_ReleaseResource(u8DIPWindowID);
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
}

static void _DMS_VDEC_Flow_PrepareNextFrame(MS_U16 u16BufferID)
{
    DMS_GET_RES_PRI;
    sync_print("prepare ID%d -> %d => ",u16BufferID,RES_DMS.gDMS_MVOPShowBufferID);
    if(RES_DMS.gDMS_MVOPShowBufferID != DMS_Invalid_ReadPointer_ID)
    {
        RES_DMS.gDMS_MVOPRemoveBufferID = RES_DMS.gDMS_MVOPShowBufferID;
    }
    RES_DMS.gDMS_MVOPShowBufferID = u16BufferID;
    //Process MFDEC //patch
    _DMS_MVOP_MFDEC_Patch(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].stMFdecInfo.bMFDec_Enable);
}

//Load Setting for next sync (to sync out of task setting and CapturePushedFrame to the same)
static void _DMS_SyncSettingForNextFrame(void* pInstance)
{
    DMS_GET_RES_PRI;
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    if(_DMS_Is_MVOPFlow(MVOP_WINDOW))
    {
        //Sync XC/MVOP setting by gu8DMS_SetXC_Status
        MS_U8 u8ID = RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW];
#if (DEV_DI == 1)
        if(gDMSDebug >= 1)
        {
            DMS_PRINTF("[%d] Main loop sync when Ring ID %d, u8ID %d\n",MsOS_GetSystemTime(),RES_DMSDI._bDiNowShowRingBufID,u8ID);
        }
#endif
        if(u8ID < DMS_Invalid_ReadPointer_ID)
        {
            if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_SIZE | DMS_XC_RESET_WINDOW_SIZE | DMS_XC_SEQ_CHANGE_PI | DMS_XC_FREEZE_SETTING))
            {
#if DBG_DMS_LOG
                if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_SIZE) && (u8ID != RES_DMS.gu8DMS_Seq_Change_ID))
                {
                    DMS_PRINTF("gu8DMS_SetXC_Status & DMS_XC_SEQ_CHANGE_SIZE && (u8DIPReadytoCaptureID[MVOP_WINDOW] < gu8DMS_Seq_Change_ID)  %d %d\n",RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW], RES_DMS.gu8DMS_Seq_Change_ID);
                }
#endif
#if (DEV_DI == 1)
                if(gDMSDebug >= 1)
                {
                    DMS_PRINTF("[%d] Sync setting for after Di case: RBID %d, FBID %d, xc_status 0x%x\n",MsOS_GetSystemTime(),RES_DMSDI._bDiNowShowRingBufID,u8ID,RES_DMS.gu8DMS_SetXC_Status[MVOP_WINDOW]);
                }
#endif
                _DMS_XC_SetWindow_MVOP_SetPath(pInstance, u8ID);

                //If VDEC to DIP path change for MVOP, Need clean all valid except u8DIPReadytoCaptureID[MVOP_WINDOW]
#if DEV_PAUSE

#else
                if((RES_DMS.eDMS_PreMVOPFlowCtrl != RES_DMS.eDMS_MVOPFlowCtrl) &&
                    (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_DIP))
                {
                    MS_U8 i = 0;
                    for(i=0;i<DMS_DIP_QUEUEDEPTH;i++)
                    {
                       if(i != RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW])
                       {
                            if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][i].bValid == TRUE)
                            {
                                printf("Change source clear frame ref!! id %d\n",i);
                                _DMS_VDEC_Frame_Release(MVOP_WINDOW,i);
                            }
                            RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][i].bValid = FALSE;
                       }
                    }
                }
#endif
                if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_RESET_WINDOW_SIZE))
                {
                    _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_RESET_WINDOW_SIZE);
                }
                if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_PI))
                {
                    _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_PI);
                }
                if((_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE_SETTING)) && (FALSE == _DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE)))
                {
                    _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE_SETTING);
                }
                if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_SIZE))
                {
                    _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_SIZE);
                }
            }
        }
        else if(DEV_PAUSE)
        {
            if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_RESET_WINDOW_SIZE) && (_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_SEQ_CHANGE_SIZE) == FALSE)
                && (_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FIRST_TIME_SETWINDOW) == FALSE))
            {
                DMS_PRINTF("\033[1;35m###[River][%s][%d]### pause reset window case\033[0m\n",__FUNCTION__,__LINE__);
                if(_DMS_XC_SetWindow_MVOP_SetPath(pInstance, RES_DMS.gDMS_MVOPShowBufferID))
                {
                    _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_RESET_WINDOW_SIZE);
                    RES_DMS._bForceSwitchBufferFlag = TRUE;
                }
            }
        }
    }
#endif  // #if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    }

#define PREPARE_FRAME_CHECK_COUNT 2
#define DOUBLE_CHECK_LATENCY 2
static void _DMS_PrepareNextFrame(void* pInstance)
{
    DMS_GET_RES_PRI;
    MS_U8 u8CheckCount = 0;
    MS_U8 u8WindowID = 0;
    MS_U8 u8BufferID = DMS_Invalid_ReadPointer_ID;
    MS_U8 u8Flag = FALSE;
    MS_U8 u8Idx = 0;
    MS_BOOL bInOrder = TRUE;
    MS_U8 WindowZOrder_Local[DMS_MAX_WINDOW_NUM];
    MS_BOOL bWindowAlreadyUpdated[DMS_MAX_WINDOW_NUM];
    MS_BOOL bWindowReadyToDisp[DMS_MAX_WINDOW_NUM];

    for(u8Idx=0; u8Idx < DMS_MAX_WINDOW_NUM; u8Idx++)
    {
        WindowZOrder_Local[u8Idx] = RES_DMS.WindowZOrder[u8Idx];
        bWindowReadyToDisp[u8Idx] = FALSE;
        bWindowAlreadyUpdated[u8Idx] = FALSE;
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 0)
        if(u8Idx == MVOP_WINDOW)
              bWindowAlreadyUpdated[u8Idx] = TRUE;
#endif
    }

    for(u8CheckCount = 0; u8CheckCount < PREPARE_FRAME_CHECK_COUNT; u8CheckCount++)
    {
        if(u8CheckCount == 0)
        {
#if DEV_DI
            if(DEV_NEW_NEXT == 0 && RES_DMS.bDMS_MVOPFlowFromDi)
            {
                MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
                MsOS_DelayTask(10);
                MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
            }
#endif
            for(u8WindowID=0; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
            {
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
                if(u8WindowID != MVOP_WINDOW)
#endif
                _DMS_GetNextInfo(u8WindowID, TRUE);
            }
            //Sync setting to next frame
            if(DEV_NEW_NEXT == 1)
            {
                _DMS_SyncSettingForNextFrame(pInstance);
            }
            else if(RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW] < DMS_Invalid_ReadPointer_ID)
            {
                _DMS_SyncSettingForNextFrame(pInstance);
                u8Flag = TRUE;
            }
        }
        else
        {
            //To use get next info twice, MVOP case need a delay
            //For example: Only Main window case, if first getnextinfo do not set u8DIPReadytoCaptureID by timing issue
            //must delay little time to getnextinfo twice
            for(u8WindowID=0; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
            {
                if(DEV_NEW_NEXT)
                {
                    continue;
                }

                if((_DMS_IsWindowUsed(u8WindowID) == TRUE) && (bWindowAlreadyUpdated[u8WindowID] == FALSE))
                {
                    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
                    //DMS_PRINTF("MVOP window 2nd _DMS_GetNextInfo, delay 2ms\n");
                    MsOS_DelayTask(DOUBLE_CHECK_LATENCY);
                    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
                    break;
                }
            }

            for(u8WindowID=0; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
            {
                if(bWindowReadyToDisp[u8WindowID] == FALSE)
                {
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
                    if(u8WindowID != MVOP_WINDOW)
#endif
                    _DMS_GetNextInfo(u8WindowID, FALSE);
                }
            }

            //Sync setting to next frame
            if(u8Flag == FALSE)
            {
                _DMS_SyncSettingForNextFrame(pInstance);
            }
        }

        for(u8Idx=0; u8Idx < DMS_MAX_WINDOW_NUM; u8Idx++)
        {
            u8WindowID = WindowZOrder_Local[u8Idx];
            u8BufferID = RES_DMS.u8DIPReadytoCaptureID[u8WindowID];
            SKIP_DMS_MAIN_WINDOW_FLOW(u8WindowID);

            if(DEV_NEW_NEXT)
            {
                bWindowReadyToDisp[u8WindowID] = TRUE;
            }

            if(bWindowAlreadyUpdated[u8WindowID] == FALSE)
            {
                if(_DMS_IsCaptureWindowEnable(u8WindowID))
                {
                    if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].u32Visible == FALSE)
                    {
                        continue;
                    }
                }

                if(u8BufferID < DMS_Invalid_ReadPointer_ID)
                {
                    bWindowReadyToDisp[u8WindowID] = TRUE;
                    if(_DMS_Is_MVOPFlow(u8WindowID) == TRUE || bInOrder == TRUE)
                    {
                        bWindowAlreadyUpdated[u8WindowID] = TRUE;
                    }

                    if(_DMS_Is_MVOPFlow(u8WindowID) && _DMS_Query_XC_Status(u8WindowID, DMS_XC_FREEZE))
                    {
                        if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_FREEZE_SETTING))
                        {
                            _DMS_Remove_XC_Status(u8WindowID, DMS_XC_FREEZE_SETTING);
                            _DMS_DIP_Flow_PrepareNextFrame(pInstance,E_DMS_DIP_NORMAL,u8WindowID,u8BufferID);
                            DMS_PRINTF("Freeze By DIP capture!!!!!!!!!!!!!!!!!!!!!!\n");
                        }
                    }
                    else
                    {
                        if(_DMS_Is_MVOPFlow(u8WindowID))
                        {
                            if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
                            {
                                _DMS_VDEC_Flow_PrepareNextFrame(u8BufferID);
                            }
                            else if((RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][u8BufferID].bIsAfterDiTask == FALSE)
                                && (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_DIP))
                            {
                                _DMS_DIP_Flow_PrepareNextFrame(pInstance,E_DMS_DIP_NORMAL,u8WindowID,u8BufferID);
                            }
                        }
                        else
                        {
                            //First frame has been pushed since zorder or window was adjusted
                            if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_UPDATE_FROM_VDEC))
                            {
                                _DMS_Add_XC_Status(u8WindowID,  DMS_XC_FIRST_FRAME_FROM_VDEC);
                            }

                            if(RES_DMS.bIsWindowMute[u8WindowID] == FALSE)
                            {
                                if(u8CheckCount == 0)
                                {
                                    if(bInOrder == TRUE)
                                    {
                                        _DMS_DIP_Flow_PrepareNextFrame(pInstance,E_DMS_DIP_NORMAL,u8WindowID,u8BufferID);
                                    }
                                }
                                else
                                {
                                    _DMS_DIP_Flow_PrepareNextFrame(pInstance,E_DMS_DIP_NORMAL,u8WindowID,u8BufferID);
                                }
                            }

                        }
                    }
                }
                else if(u8BufferID == DMS_Invalid_ReadPointer_ID)
                {
                    if(DEV_NEW_NEXT)
                    {
                        if(RES_DMS.bIsWindowMute[u8WindowID] == FALSE)
                        {
                            //case: bNeedUpdateFromVDEC == FALSE
                            if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_UPDATE_FROM_VDEC) == FALSE)
                            {
                                if(_DMS_IsWindowUsed(u8WindowID) == TRUE)
                                {
                                    if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_FIRST_FRAME_FROM_VDEC))
                                    {
                                        _DMS_GERendering(pInstance, u8WindowID);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if(_DMS_Is_MVOPFlow(u8WindowID) == FALSE)
                        {
                            if(u8CheckCount == 0)
                            {
                                bInOrder = FALSE;
                            }
                            else
                            {
                                if(RES_DMS.bIsWindowMute[u8WindowID] == FALSE)
                                {
                                    //case: bNeedUpdateFromVDEC == FALSE
                                    if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_UPDATE_FROM_VDEC) == FALSE)
                                    {
                                        if(_DMS_IsWindowUsed(u8WindowID) == TRUE)
                                        {
                                            if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_FIRST_FRAME_FROM_VDEC))
                                            {
                                                _DMS_GERendering(pInstance, u8WindowID);
                                            }
                                        }
                                    }
                                }
                            }
                        }
#if DEV_PAUSE
                        else if(_DMS_Is_MVOPFlow(u8WindowID) && RES_DMS._bForceSwitchBufferFlag == TRUE)
                        {
                            u8BufferID = RES_DMS.gDMS_MVOPShowBufferID;
                            //pause case
                            MS_U8 u8WorkID = DMS_Invalid_ReadPointer_ID;
                            if(u8BufferID != DMS_Invalid_ReadPointer_ID)
                            {
                                bWindowAlreadyUpdated[u8WindowID] = TRUE;
                                if((RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_DIP))
                                {
                                    DMS_PRINTF("\033[1;35m###[River][%s][%d]### DIP pause reset window case %d\033[0m\n",__FUNCTION__,__LINE__,u8WorkID);
                                    _DMS_DIP_Flow_PrepareNextFrame(pInstance,E_DMS_DIP_NORMAL,u8WindowID,u8BufferID);
                                }
                                else if((RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC))
                                {
                                    DMS_PRINTF("\033[1;35m###[River][%s][%d]### VDEC pause reset window case, ID %d\033[0m\n",__FUNCTION__,__LINE__,RES_DMS.gDMS_MVOPShowBufferID);
                                    _DMS_VDEC_Flow_PrepareNextFrame(u8BufferID);
                                }
                            }
                            else
                            {
                                RES_DMS._bForceSwitchBufferFlag = FALSE;
                                DMS_PRINTF("\033[1;35m###[River][%s][%d]### pause reset window case, work ID %d error\033[0m\n",__FUNCTION__,__LINE__,u8BufferID);
                            }
                        }
#endif
                    }
                }
            }
        }
    }

    //DIP case only
    for(u8WindowID=1; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
    {
        if(_DMS_Query_XC_Status(u8WindowID, DMS_XC_FIRST_FRAME_FROM_VDEC) && _DMS_Query_XC_Status(u8WindowID, DMS_XC_UPDATE_FROM_VDEC))
        {
            _DMS_Remove_XC_Status(u8WindowID, DMS_XC_UPDATE_FROM_VDEC);
        }
    }
    nrp_sync_print(" End\n");
}

//Switch GOP Read Buffer
static MS_BOOL _DMS_SwitchGOPBuffer(void* pInstance)
{
    DMS_GET_RES_PRI;
    GOP_GwinInfo stGWin;
    if (_IsGwinAvailable() == FALSE)
    {
        return FALSE;
    }
    MApi_GOP_GWIN_GetWinInfo(RES_DMS.u8GOPGWinNum, &stGWin);
    MApi_GOP_GWIN_UpdateRegOnceEx2(GOP_NUM,TRUE,FALSE);
    //MApi_GOP_GWIN_UpdateRegOnce(TRUE);
    stGWin.u32DRAMRBlkStart = _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_GOP, _DMS_DIP_GetBackgroundBufID(pInstance));
    MApi_GOP_GWIN_SetWinInfo(RES_DMS.u8GOPGWinNum, &stGWin);
    MApi_GOP_GWIN_Enable(RES_DMS.u8GOPGWinNum, TRUE);//enable GWIN
    //MApi_GOP_GWIN_UpdateRegOnce(FALSE);
    MApi_GOP_GWIN_UpdateRegOnceEx2(GOP_NUM,FALSE,FALSE);
#if DBG_FPS
    u32FPS_Switch_Counter[1]++;
#endif
    RES_DMS.bDIPWriteFirstAddr = !RES_DMS.bDIPWriteFirstAddr;

    return TRUE;
}
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_BOOL _DMS_SwitchMVOPBufferByDIP(void* pInstance)
{
    DMS_GET_RES_PRI;
    if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE))
    {
        MS_PHY phyDIPoutAddr = RES_DMS.u32MVOPFreezeAddress_pa;
        HAL_DMS_XC_DSSetMVOPAddr(pInstance, phyDIPoutAddr,phyDIPoutAddr,TRUE);
        HAL_DMS_XC_DSFire(pInstance);
        return TRUE;
    }
    else
    {
        MS_PHY phyDIPoutAddr = 0;
#if (DEV_DI == 1)
        if(RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].bIsAfterDiTask == TRUE)
        {
            if(RES_DMSDI._bDiReleaseShowRingBufID != INVALID_DI_RING_BUFFER_ID)
            {
                RES_DMSDI._bDiReleaseShowRingBufID = INVALID_DI_RING_BUFFER_ID;
            }
            MS_U8 u8DiRingBufID = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].u8DiOutputRingBufferID;
            phyDIPoutAddr = _GetDIRingBufferAddr(MVOP_WINDOW,u8DiRingBufID);
            tempAddr = phyDIPoutAddr;
            di_sync_print("\033[1;34mBF R %d: 0x%x, 0x%llx , MainQ %d reading\033[0m\n",MsOS_GetSystemTime(),RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].u32PriData,phyDIPoutAddr,RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]);
            if(RES_DMSDI._bDiNowShowRingBufID == INVALID_DI_RING_BUFFER_ID)
            {
                RES_DMSDI._bDiNowShowRingBufID = u8DiRingBufID;
            }
            else
            {
                if(RES_DMSDI._bDiNowShowRingBufID != u8DiRingBufID)
                {
                    if(gDMSDebug >= 2)
                    {
                        DMS_PRINTF("[%d] switch Ring ID from %d to %d => ",MsOS_GetSystemTime(),RES_DMSDI._bDiNowShowRingBufID,u8DiRingBufID);
                    }

                    RES_DMSDI._bDiReleaseShowRingBufID = RES_DMSDI._bDiNowShowRingBufID;
                    //printf("\033[1;32m[%d] Free  Ring Buffer %d\033[0m\n",MsOS_GetSystemTime(),RES_DMSDI._bDiReleaseShowRingBufID);
                    RES_DMSDI._bDiNowShowRingBufID = u8DiRingBufID;
                }
            }
            if(gDMSDebug >= 2)
            {
                DMS_PRINTF("[%d] now show after set DS table %d\n",MsOS_GetSystemTime(),RES_DMSDI._bDiNowShowRingBufID);
            }
            RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].bValid = FALSE;
        }
        else
#endif
        {
            phyDIPoutAddr = _DMS_DIP_GetDispBufferAddress(pInstance, E_DMS_DIP_CLIENT_MVOP, (RES_DMS.bDIPMVOPWriteFirstAddr)? E_DMS_DIP_MULTIVIEW_BUF_ID_0 : E_DMS_DIP_MULTIVIEW_BUF_ID_1);
        }

        if(gDMSMVOPForceLookVDEC)
        {
            MS_PHY phyAddr0 = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].u32SrcLumaAddr;
            MS_PHY phyAddr1 = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].u32SrcChromaAddr;
            HAL_DMS_XC_DSSetMVOPAddr(pInstance, phyAddr0, phyAddr1,FALSE);
            HAL_DMS_XC_DSFire(pInstance);
        }
        else
        {
            HAL_DMS_XC_DSSetMVOPAddr(pInstance, phyDIPoutAddr,phyDIPoutAddr,TRUE);
            HAL_DMS_XC_DSFire(pInstance);
        }

#if DBG_FPS
        static MS_U8 u8DIP_FPSID = 0xFF;
        if(u8DIP_FPSID != RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW])
        {
            u32FPS_Switch_Counter[0]++;
        }
        u8DIP_FPSID = RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW];
#endif

#if DEV_PAUSE
        if((RES_DMS.gDMS_MVOPRemoveBufferID != DMS_Invalid_ReadPointer_ID) && (RES_DMS.gDMS_MVOPRemoveBufferID != RES_DMS.gDMS_MVOPShowBufferID))
        {
            RES_DMS.bMVOPMarkRemoveBuffer = TRUE;
        }
#endif

        RES_DMS.bDIPMVOPWriteFirstAddr = !RES_DMS.bDIPMVOPWriteFirstAddr;

        return TRUE;
    }
}
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
static MS_BOOL _DMS_SwitchMVOPBufferByVDEC(void* pInstance)
{
    DMS_GET_RES_PRI;
    //DMS_PRINTF(" setNexttoNow ");
    if(RES_DMS.gDMS_ScalingCondition & USE_2ND_BUFFER)
    {
        RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].u32SrcLumaAddr = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].phySrc2ndBufferLumaAddr;
        RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].u32SrcChromaAddr = RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].phySrc2ndBufferChromaAddr;
    }

    HAL_DMS_XC_DSSetMVOPAddr(pInstance, RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].u32SrcLumaAddr,RES_DMS._stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.gDMS_MVOPShowBufferID].u32SrcChromaAddr,FALSE);
    HAL_DMS_XC_DSFire(pInstance);

#if DBG_FPS
    static MS_U8 u8VDEC_FPSID = 0xFF;
    if(u8VDEC_FPSID != RES_DMS.gDMS_MVOPShowBufferID)
    {
        u32FPS_Switch_Counter[0]++;
    }
    u8VDEC_FPSID = RES_DMS.gDMS_MVOPShowBufferID;
#endif
    sync_print("set show ID%d, rm %d => ",RES_DMS.gDMS_MVOPShowBufferID, RES_DMS.gDMS_MVOPRemoveBufferID);

    //After Switch buffer, now we can remove raw buffer(set to valid false)
    //Mark remove only, do valid = FALSE in next vsync start to avoid MVOP double buffer timing issue
    if((RES_DMS.gDMS_MVOPRemoveBufferID != DMS_Invalid_ReadPointer_ID) && (RES_DMS.gDMS_MVOPRemoveBufferID != RES_DMS.gDMS_MVOPShowBufferID))
    {
        RES_DMS.bMVOPMarkRemoveBuffer = TRUE;
    }
    return TRUE;
}
#endif
static MS_BOOL _DMS_SwitchBuffer(void* pInstance)
{
    //Need Switch MVOP first
    DMS_GET_RES_PRI;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    if(_DMS_Is_MVOPFlow(MVOP_WINDOW))
    {
        MS_BOOL bRet = FALSE;
        if(RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW] < DMS_Invalid_ReadPointer_ID || (TRUE == RES_DMS._bForceSwitchBufferFlag))
        {
            if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FREEZE) || (RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_DIP))
            {
                bRet = _DMS_SwitchMVOPBufferByDIP(pInstance);
            }
            else if(RES_DMS.eDMS_MVOPFlowCtrl == E_DMS_MVOP_FLOW_FROM_VDEC)
            {
                bRet = _DMS_SwitchMVOPBufferByVDEC(pInstance);
            }
            RES_DMS._bForceSwitchBufferFlag = FALSE;

            if(bRet == TRUE && _DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_BLACK_SCREEN_BY_WINUSED))
            {
                MS_BOOL bDIPWinUsed = _DMS_IsWindowUsed(MVOP_WINDOW);
                Hal_DMS_XC_SetMute(pInstance, !bDIPWinUsed);
                _DMS_Remove_XC_Status(MVOP_WINDOW, DMS_XC_BLACK_SCREEN_BY_WINUSED);
            }
        }
        else
        {
            //DMS_PRINTF("No switch _DMS_SwitchBuffer\n");
        }
    }
#endif  //DMS_MAIN_WINDOW_FLOW_VERSION
    _DMS_SwitchGOPBuffer(pInstance);

    return TRUE;
}

#define DEV_PQ_FOD 0
#if DEV_PQ_FOD
#include "drvPQ.h"
#include "drvPQ_cus.h"
#endif
static MS_BOOL _Task_DMS_CapturePushedFrame(void)
{
    MS_U32 u32WaitEventFlag = BIT(0);
    MS_U32 u32Events = 0;
    void *pInstance = pTaskInstance;

    // coverity[no_escape]
    while(1)
    {
        MsOS_WaitEvent(XC_Vsync_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        DMS_GET_RES_PRI;

        //gDMSMVOPForceLookVDEC = gDMS_externDebugFlag;
#if DEV_PQ_FOD
        if(_DMS_Query_XC_Status(MVOP_WINDOW, DMS_XC_FIRST_TIME_SETWINDOW) == 0)
        {
            MDrv_PQ_AdaptiveTuning();
        }
#endif
        sync_print("||>> ");
#if DEV_DI
        //di_sync_print("\033[1;32mSHOW %d: 0x%x, 0x%llx , MainQ %d reading\033[0m\n",MsOS_GetSystemTime(),_stXC_DIPPushInfo[MVOP_WINDOW][RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]].u32PriData,tempAddr,RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW]);
#endif
        sync_print(" next buffer ID %d, now show ID%d => ",RES_DMS.u8DIPReadytoCaptureID[MVOP_WINDOW],RES_DMS.gDMS_MVOPShowBufferID);

        //(1) Check Display Buffer Flag
        _DMS_CheckDispBufFlag(pInstance);

        //(3) Prepare Next Frame to be Displayed
        _DMS_PrepareNextFrame(pInstance);

        //(5) Switch GOP Read Buffer
        _DMS_SwitchBuffer(pInstance);

        //clear Frame Ref
        //How to check release frame?

        sync_print("End \n");
        //DMS_PRINTF("End \n");
        if(0)//gDMS_externDebugFlag != 0)
        {
            DMS_PRINTF("#end\n");
            while(1);
        }

        if(gDMSDebug >= 3)
        {
            gDMSDebug++;
            if(gDMSDebug > 4)
                gDMSDebug = 0;
        }
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

#if DBG_FPS
        u32FPS_Show_Counter[0]++;
        u32FPS_Show_Counter[1]++;
#endif

    }
    return TRUE;
}

static MS_BOOL _DMS_VDEC_Frame_AddRef_CaptureMode(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
#if NEW_FLIP
    if(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex == DMS_INVALID_FRAME_ID)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }

    VDEC_StreamId VdecStreamId;
    VDEC_EX_DispFrame VdecDispFrm;
    VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
    VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
    VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex;
    VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData;
    VDEC_EX_Result eResult;
    eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
    if (eResult != E_VDEC_EX_OK)
    {
        DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
            (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
        DMS_PRINTF("[%s] window %td, BufID %d, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
        return FALSE;
    }
    if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].eScanType))
    {
        VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex_2nd;
        VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData_2nd;
        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %td, BufID %d, 2nd Field, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
            return FALSE;
        }
    }
    //DMS_PRINTF("[AddRef] Frame[0x%x], PriData[0x%x]\n",_stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex,_stXC_DIPPushInfo[u32Window][u16BufID].u32PriData);
    RES_DMSCAPTURE._stXC_LocalFrameRefCountCaptureMode[u32Window][u16BufID]++;

#if DBG_FPS
    u32FPS_Add_Counter[u32Window]++;
#endif
#endif
    return TRUE;
}

static MS_BOOL _DMS_VDEC_Frame_Release_CaptureMode(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
#if NEW_FLIP
    if(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex == DMS_INVALID_FRAME_ID)
    {
        //DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }
    if(RES_DMSCAPTURE._stXC_LocalFrameRefCountCaptureMode[u32Window][u16BufID] == 0)
    {
        //DMS_PRINTF("[%s] ~window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__,u32Window,u16BufID);
        return TRUE;
    }
    RES_DMSCAPTURE._stXC_LocalFrameRefCountCaptureMode[u32Window][u16BufID]--;
    if(RES_DMSCAPTURE._stXC_LocalFrameRefCountCaptureMode[u32Window][u16BufID] == 0)
    {
        VDEC_StreamId VdecStreamId;
        VDEC_EX_DispFrame VdecDispFrm;
        VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex;
        VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData;

        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %td, BufID %d, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
            return FALSE;
        }
        if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].eScanType))
        {
            VdecStreamId.u32Version = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
            VdecStreamId.u32Id = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
            VdecDispFrm.u32Idx = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex_2nd;
            VdecDispFrm.u32PriData = RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData_2nd;

            VDEC_EX_Result eResult;
            eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
            if (eResult != E_VDEC_EX_OK)
            {
                DMS_PRINTF("[%s] %td %td, %td %td\n",__FUNCTION__,(ptrdiff_t)VdecStreamId.u32Version,(ptrdiff_t)VdecStreamId.u32Id,
                    (ptrdiff_t)VdecDispFrm.u32Idx,(ptrdiff_t)VdecDispFrm.u32PriData);
                DMS_PRINTF("[%s] window %td, BufID %d, 2nd Field, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,eResult);
                return FALSE;
            }
        }
        //DMS_PRINTF("[Release] Frame[0x%x], PriData[0x%x]\n",RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex,RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData);
        if(_IsFrameRefCountZero(u32Window, u16BufID))
        {
            RES_DMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex = DMS_INVALID_FRAME_ID;
        }
    }

#if DBG_FPS
    u32FPS_Release_Counter[u32Window]++;
#endif
#endif
    return TRUE;
}

static void _DMS_Clear_VDEC_Frame_Ref_CaptureMode(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8RefCounter = 0;
    for(u8RefCounter = 0; u8RefCounter < RES_DMSCAPTURE._stXC_LocalFrameRefCountCaptureMode[u32Window][u16BufID]; u8RefCounter++)
    {
        DMS_PRINTF("[%s] CaptureMode window %td release frame for WP %d ct %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,(u8RefCounter+1));
        _DMS_VDEC_Frame_Release_CaptureMode(u32Window,u16BufID);
    }
}

#if DEV_CAPMODE

#if (DMS_DIP_Support_IMI == 0) //Fix with FrameBuffer mode variable
static MS_U16 _Get_CaptureMode_NextBufferID(MS_U32 u32WindowID, MS_U16 u16CurID)
{
    DMS_GET_RES_PRI;
    MS_U16 u16NextID = DMS_CAPTUREWIN_RING_BUFFER;

    u16NextID = (u16CurID + 1) % DMS_CAPTUREWIN_RING_BUFFER;
    while(RES_DMSCAPTURE._stXC_CaptureRingBufRefCount[u32WindowID][u16NextID] != 0)
    {
        u16NextID = (u16NextID + 1) % DMS_CAPTUREWIN_RING_BUFFER;
    }
    return u16NextID;
}

static MS_U16 _Get_CaptureMode_NextRingBufferWritePointer(MS_U32 u32WindowID)
{
    DMS_GET_RES_PRI;
    RES_DMSCAPTURE._stXC_DIPCaptureWin_RingBufferPointer[u32WindowID].u16PreWritePointer = RES_DMSCAPTURE._stXC_DIPCaptureWin_RingBufferPointer[u32WindowID].u16WritePointer;
    return _Get_CaptureMode_NextBufferID(u32WindowID, RES_DMSCAPTURE._stXC_DIPCaptureWin_RingBufferPointer[u32WindowID].u16WritePointer);
}
#endif

static void _MarkIgnorableCaptureBufferID(MS_U32 u32WindowID, MS_U16 u16TargetID)
{
    DMS_GET_RES_PRI;
    MS_U16 u16CurID = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u16PreviousWP;

    while (u16CurID != u16TargetID)
    {
        if(RES_DMSCAPTURE.bIsFrameValid[u32WindowID][u16CurID] == TRUE)
        {
            _DMS_VDEC_Frame_Release_CaptureMode(u32WindowID, u16CurID);
            RES_DMSCAPTURE.bIsFrameValid[u32WindowID][u16CurID] = FALSE;
            RES_DMSCAPTURE.u8frameRepeatCnt[u32WindowID][u16CurID] = 0;
        }
        u16CurID = _GetNextBufferID(u16CurID);
    }
}

static MS_BOOL _Task_DMS_CaptureMode_PushedFrame(void)
{
    MS_U32 u32WaitEventFlag = BIT(0)|BIT(1)|BIT(2)|BIT(3);
    MS_U32 u32Events = 0;
    MS_U8 u8WindowID = 0;
    MS_U8 u8BufferID = 0;
#if (DMS_DIP_Support_IMI == 0)
    MS_U8 u8RepeatCnt = 0;
    MS_U16 u16NextWriteBufID = DMS_CAPTUREWIN_RING_BUFFER;
#endif
    void *pInstance = pTaskInstance;

    // coverity[no_escape]
    while(1)
    {
        MsOS_WaitEvent(CaptureMode_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        DMS_GET_RES_PRI;

        for(u8WindowID=0; u8WindowID<DMS_MAX_WINDOW_NUM; u8WindowID++)
        {
            if(u32Events & BIT(u8WindowID))
            {
                u8BufferID = RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u8WindowID].u16ReadPointer;
                _MarkIgnorableCaptureBufferID(u8WindowID, u8BufferID);
#if (DMS_DIP_Support_IMI == 0)
                MS_U8 u8FrameRepeat = RES_DMSCAPTURE.u8frameRepeatCnt[u8WindowID][u8BufferID];
                for(u8RepeatCnt=0; u8RepeatCnt<u8FrameRepeat; u8RepeatCnt++)
#else
                if(RES_DMSCAPTURE.u8frameRepeatCnt[u8WindowID][u8BufferID] > 0)
#endif
                {
                    RES_DMSCAPTURE.u8frameRepeatCnt[u8WindowID][u8BufferID]--;
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].u32FrameCount++;
#if (DMS_DIP_Support_IMI == 0)
                    u16NextWriteBufID = _Get_CaptureMode_NextRingBufferWritePointer(u8WindowID);
                    //printf("\033[36m === windowID: %d, BufferID: %d [%d]=== \033[0m\n", u8WindowID, u8BufferID, u16NextWriteBufID);
                    RES_DMSCAPTURE._stXC_DIPCaptureWin_RingBufferPointer[u8WindowID].u16WritePointer = u16NextWriteBufID;
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].stCaptureRingBuf[u16NextWriteBufID].u64Pts = RES_DMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].u64Pts;
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].stCaptureRingBuf[u16NextWriteBufID].u32frameCount = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].u32FrameCount;
#else
                    //printf("\033[36m === windowID: %d, BufferID: %d, Cnt %d === \033[0m\n", u8WindowID, u8BufferID, RES_DMSCAPTURE.u8frameRepeatCnt[u8WindowID][u8BufferID]);
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].stCaptureRingBuf[0].u64Pts = RES_DMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].u64Pts;
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].stCaptureRingBuf[0].u32frameCount = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].u32FrameCount;
#endif
                    _DMS_DIP_Flow_PrepareNextFrame(pInstance, E_DMS_DIP_CAPTURE,u8WindowID,u8BufferID);
#if (DMS_DIP_Support_IMI == 0)
                    BUFFER_INFO stBufInfo;
                    stBufInfo = MApi_XC_DIP_GetBufInfo(RES_DMS.u8DIP_Window[E_DMS_DIP_CAPTURE]);
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].stCaptureRingBuf[u16NextWriteBufID].phyBufferCAddr = stBufInfo.u32CBuf[0];
                    RES_DMSCAPTURE.bIsRingBufAccessed[u16NextWriteBufID] = TRUE;
#endif
                }
#if (DMS_DIP_Support_IMI)
                if(RES_DMSCAPTURE.u8frameRepeatCnt[u8WindowID][u8BufferID] == 0)
#endif
                {
                    if(RES_DMSCAPTURE.bIsFrameValid[u8WindowID][u8BufferID] == TRUE)
                    {
                        RES_DMSCAPTURE.bIsFrameValid[u8WindowID][u8BufferID] = FALSE;
#if (DMS_DIP_Support_IMI == 0)
                        RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u8WindowID].u16PreviousWP = u8BufferID;
#endif
                        _DMS_VDEC_Frame_Release_CaptureMode(u8WindowID, u8BufferID);
                    }
                }
           }
        }
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    }
    return TRUE;
}
#endif

static MS_BOOL _DMS_TriggerToCaptureFrame(void* pInstance, MS_U16 u16WindowID)
{
    DMS_GET_RES_PRI;
    MS_U32 u32OutputRate = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32FrameRate;
    MS_U16 u16WritePointer = RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16WritePointer;
    MS_U32 u32InputRate = RES_DMS._stXC_DIPPushInfo[u16WindowID][u16WritePointer].u32FrameRate;

    if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].bStartCountCapModeFRC != FALSE)
    {
        if(u32InputRate > 0)
        {
            MS_U8 u8DisplayCount = 0;
            RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate += u32OutputRate;
            RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u8ReadPointerOffset++;

            if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate >= u32InputRate)
            {
                //DMS_PRINTF("\033[36m [%s] Out: %d, In: %d \033[0m\n",__FUNCTION__, RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate, u32InputRate);
                u8DisplayCount = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate / u32InputRate;
                RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate % u32InputRate;

                MS_U8 u8RP_Offset = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u8ReadPointerOffset;
                RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u8ReadPointerOffset = 0;
                MS_U8 i = 0;
                MS_U16 u16CurReadPointer = RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16ReadPointer;
                MS_U16 u16NextReadPointer = u16CurReadPointer;
                for(i=0; i < u8RP_Offset; i++)
                {
                    u16NextReadPointer = _GetNextBufferID(u16NextReadPointer);
                }
                if(RES_DMSCAPTURE.bIsFrameValid[u16WindowID][u16NextReadPointer])
                {
                    while(RES_DMSCAPTURE.u8frameRepeatCnt[u16WindowID][u16CurReadPointer] != 0)
                    {
                        if(_DMS_IsCaptureWindowEnable(u16WindowID) == FALSE)
                        {
                            return FALSE;
                        }
                        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
                        DMS_PRINTF("\033[31m [%s] win %d, CAPTURE Force block flip when %d RepeatCnt %d > 0\033[0m\n",__FUNCTION__, u16WindowID, u16CurReadPointer, RES_DMSCAPTURE.u8frameRepeatCnt[u16WindowID][u16CurReadPointer]);
                        MsOS_DelayTask(1);
                        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
                    }
                    RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16ReadPointer = u16NextReadPointer;
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u16NeedDeletePtr = u16NextReadPointer;
                    RES_DMSCAPTURE.u8frameRepeatCnt[u16WindowID][u16NextReadPointer] = u8DisplayCount;
                    //printf("\033[32m Read: %d, RepeatCnt: %d [%d]\033[0m\n", u16NextReadPointer, u8DisplayCount, u32InputRate);
                    return TRUE;
                }
                else
                {
                    DMS_PRINTF("win %d, %d (Cnt: %d), next %d NOT Valid\n",u16WindowID, RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16ReadPointer, RES_DMSCAPTURE.u8frameRepeatCnt[u16WindowID][u16CurReadPointer], u16NextReadPointer);
                }
            }
            else
            {
                MS_U16 u16DeletePtr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u16NeedDeletePtr;
                u16DeletePtr = _GetNextBufferID(u16DeletePtr);
                _DMS_VDEC_Frame_Release_CaptureMode(u16WindowID, u16DeletePtr);
                RES_DMSCAPTURE.bIsFrameValid[u16WindowID][u16DeletePtr] = FALSE;
                RES_DMSCAPTURE.u8frameRepeatCnt[u16WindowID][u16DeletePtr] = 0;
                RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u16NeedDeletePtr = u16DeletePtr;
            }
        }
    }
    else
    {
        MS_U16 u16WritePoint = RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16WritePointer;
        if(RES_DMSCAPTURE.bIsFrameValid[u16WindowID][u16WritePoint])
        {
            MS_U8 i = 0;
            MS_BOOL bValid = FALSE;
            for(i = 0 ; i < (START_FRC_ACCUMULE_FRAME_COUNT - 1); i++)
            {
                u16WritePoint = _GetPreviousBufferID(u16WritePoint);
                if(RES_DMSCAPTURE.bIsFrameValid[u16WindowID][u16WritePoint])
                {
                    bValid = TRUE;
                }
                else
                {
                    bValid = FALSE;
                    break;
                }
            }
            if(bValid)
            {
                if(u32OutputRate > 0)
                {
                    while(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate < u32InputRate)
                    {
                        RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate + u32OutputRate;
                    }
                    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u32AccumuleOutputRate % u32InputRate;
                }
                else
                {
                    DMS_PRINTF("\033[31m [%s] Error! Capture FrameRate <= 0 \033[0m\n",__FUNCTION__);
                }

                RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16ReadPointer = u16WritePoint;
                DMS_PRINTF("win %d, %d \n",u16WindowID, RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u16WindowID].u16ReadPointer);
                RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].u16PreviousWP = _GetPreviousBufferID(u16WritePoint);
                RES_DMSCAPTURE.u8frameRepeatCnt[u16WindowID][u16WritePoint] = 1;
                RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u16WindowID].bStartCountCapModeFRC = TRUE;
                DMS_PRINTF("FRC Start \n");
                return TRUE;
            }
        }
    }
    return FALSE;
}

#if DBG_FPS
static MS_BOOL _Task_DMS_FPS(void)
{
    MS_U32 t0[2] = {0};
    MS_U8 i = 0;
// coverity[no_escape]
    while(1)
    {
        for(i = 0 ; i < 2; i++)
        {
            if(bFPS_Start[i])
            {
                MS_U32 t1 = (MsOS_GetSystemTime() - t0[i]);
                if((t1 > PRI_PERIOD) || (t0[i] == 0))
                {
#if NEW_FLIP
                    DMS_PRINTF("FPS(win%d): in %d, flip %d, out %d, Add/Rel %d/%d (%d/%d), Di %d. (time: %d).\n",i,
#else
                    DMS_PRINTF("FPS(win%d): in %d, flip %d, out %d, pull (%d/%d). (time: %d)\n",i,
#endif
                        ((u32FPS_Push_Counter[i] * 1000) / PRI_PERIOD),
                        ((u32FPS_Switch_Counter[i] * 1000) / PRI_PERIOD),
                        ((u32FPS_Show_Counter[i] * 1000) / PRI_PERIOD),
#if NEW_FLIP
                        (u32FPS_Add_Counter[i]),
                        (u32FPS_Release_Counter[i]),
                        ((u32FPS_Add_Counter[i] * 1000) / PRI_PERIOD),
                        ((u32FPS_Release_Counter[i] * 1000) / PRI_PERIOD),
                        (u32FPS_Di_Counter[i]),

#else
                        ((u32FPS_Pull_Counter[i] * 1000) / PRI_PERIOD),
                        ((u32FPS_Pull_NotReady_Counter[i] * 1000) / PRI_PERIOD),
#endif
                        t1
                        );
                    if(((u32FPS_Push_Counter[i] * 1000) / PRI_PERIOD) > (((u32FPS_Switch_Counter[i] * 1000) / PRI_PERIOD) + 5))
                    {
                        //gDMS_externDebugFlag = 1;
                    }
                    else
                    {
                        //gDMS_externDebugFlag = 0;
                    }
                    t0[i] = MsOS_GetSystemTime();
                    u32FPS_Push_Counter[i] = 0;
                    u32FPS_Switch_Counter[i] = 0;
                    u32FPS_Show_Counter[i] = 0;
                    u32FPS_Pull_Counter[i] = 0;
                    u32FPS_Pull_NotReady_Counter[i] = 0;
                    u32FPS_Add_Counter[i] = 0;
                    u32FPS_Release_Counter[i] = 0;
                    u32FPS_Di_Counter[i] = 0;
                }
            }
        }
        MsOS_DelayTask(5);
    }
    return TRUE;
}
#endif

MS_BOOL _MDrv_DMS_CreateTask(void)
{
#ifndef MSOS_TYPE_NOS
    if(s32DMSTaskID == -1)
    {
        s32DMSTaskID = MsOS_CreateTask((TaskEntry)_Task_DMS_CapturePushedFrame,
                                    0,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_Task");
#if DBG_FPS
        s32DMSFPSTaskID = MsOS_CreateTask((TaskEntry)_Task_DMS_FPS,
                                    0,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_FPS_Task");
#endif
    }
    else
    {
        DMS_PRINTF("MsOS_ResumeTask(s32DMSTaskID) \n");
        MsOS_ResumeTask(s32DMSTaskID);
    }
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
#if DEV_DI
    if(s32DIPTaskID_Interlace == -1)
    {
        s32DIPTaskID_Interlace = MsOS_CreateTask((TaskEntry)_Task_CapturePushedInterlaceFrame,
                                    0,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_IModeFrame_Task");
    }
    else
    {
        DMS_PRINTF("MsOS_ResumeTask(s32DIPTaskID_Interlace) \n");
        MsOS_ResumeTask(s32DIPTaskID_Interlace);
    }
#endif
#endif
#if DEV_CAPMODE
    if(s32DIPTaskID_CaptureMode == -1)
    {
        s32DIPTaskID_CaptureMode = MsOS_CreateTask((TaskEntry)_Task_DMS_CaptureMode_PushedFrame,
                                    0,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_CaptureModeFrame_Task");
    }
    else
    {
        DMS_PRINTF("MsOS_ResumeTask(s32DIPTaskID_CaptureMode) \n");
        MsOS_ResumeTask(s32DIPTaskID_CaptureMode);
    }
#endif

#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
    if(s32MVOPTaskID == -1)
    {
        s32MVOPTaskID = MsOS_CreateTask((TaskEntry)_Task_DMS_SetMVOP,
                                    0,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_SetMVOP_Task");
    }
    else
    {
        DMS_PRINTF("MsOS_ResumeTask(s32MVOPTaskID) \n");
        MsOS_ResumeTask(s32MVOPTaskID);
    }
    if(s32DisableBobTaskID == -1)
    {
        s32DisableBobTaskID = MsOS_CreateTask((TaskEntry)_Task_DMS_DisableBob_MainWin,
                                    0,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_DisableBob_Task");
    }
    else
    {
        DMS_PRINTF("MsOS_ResumeTask(s32DisableBobTaskID) \n");
        MsOS_ResumeTask(s32DisableBobTaskID);
    }
#endif
#endif
    return TRUE;
}

void _MDrv_DMS_InvalidateAllQueue(void)
{
    DMS_GET_RES_PRI;
    MS_U8 u8WindowID = 0;
    MS_U8 u8BufferID = 0;

    for (u8WindowID = 0; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
    {
        for(u8BufferID=0;u8BufferID<DMS_DIP_QUEUEDEPTH;u8BufferID++)
        {
            RES_DMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].bValid = FALSE;
            RES_DMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].u32FrameIndex = DMS_INVALID_FRAME_ID;
            RES_DMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].eScanType = E_DMS_VIDEO_SCAN_TYPE_MAX;
        }
    }
}

static MS_BOOL _TimeToChangeGOPBuffer(void)
{
    DMS_GET_RES_PRI;
    MS_U16 u16OutVFreq = g_IPanel.DefaultVFreq() / 10;
    MS_U32 u32Count = 0;

    // Counter + 1 in each Vsync
    RES_DMS.u32VsyncCount++;

    u32Count = (RES_DMS.u32MaxOutputFrameRate * RES_DMS.u32VsyncCount) / u16OutVFreq;

    //reset loop
    if (RES_DMS.u32VsyncCount == u16OutVFreq)
    {
        RES_DMS.u32VsyncCount = 0;
        RES_DMS.u32GOPDispCount = 0;

        return TRUE;
    }

    if (u32Count > RES_DMS.u32GOPDispCount)
    {
        RES_DMS.u32GOPDispCount = u32Count;
        return TRUE;
    }
    else
    {
        return FALSE;
    }


}

static void _DMS_EmbedXCISR(SC_INT_SRC enIntNum, void *pParam)
{
    if (_TimeToChangeGOPBuffer() == TRUE)
    {
        MsOS_SetEvent (XC_Vsync_Event_id, 0x01);
    }
}

static MS_BOOL _MDrv_DMS_MemData_Init(MS_PHY Address, MS_U32 size, EN_XC_DIP_DATA_FMT eDIPWColorFmt)
{
    if(eDIPWColorFmt == DIP_DATA_FMT_YUV422)
    {
#if 0
        MApi_GFX_ClearFrameBufferByWord(Address, size, 0x80108010);
#else
        //#define TRANSPARENT_YUYV    0x10361097
        MApi_GFX_ClearFrameBufferByWord(Address, size, DMS_DIP_MULTIVIEW_TRANSPARENT_COLOR);

        // Enable YUV422 Transparent Color
        // TODO, wait utopia API ready
        #if 1
        if(!(MApi_XC_ReadByte(0x120200 + (GOP_NUM * 0x300))&0x20))
        {
            MApi_XC_Write2ByteMask(0x120200 + (GOP_NUM * 0x300), BIT(5), BIT(5));
            MApi_XC_Write2ByteMask(0x12024C + (GOP_NUM * 0x300), 0x0000, 0xFFFF);
            MApi_XC_Write2ByteMask(0x12024E + (GOP_NUM * 0x300), 0x00, 0xFF);
        }
        #else
        ST_Transparent_Color stColor = {0};

        //Data in Dram: (0x36109710)
        stColor.color = TRANSPARENT_YUYV;
        MApi_GOP_GWIN_SetTransparentClr(1, GOPTRANSCLR_FMT3, &stColor);
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT3, ENABLE);

        #endif
#endif
    }
    else
    {
        MApi_GFX_ClearFrameBuffer(Address, size, 0x00);
    }

    MApi_GFX_FlushQueue();
    return TRUE;
}

static MS_BOOL _MDrv_DMS_OSD_Init(MS_U8 u8GOP, MS_PHY Address, EN_XC_DIP_DATA_FMT eDIPWColorFmt)
{
    DMS_GET_RES_PRI;
    MS_U16 pitch = 0;
    MS_U16 fbFmt = 0;
    static MS_U32 fbID;

    if(RES_DMS.bIsGOPInit == FALSE)
    {
        fbID = 0;
        GOP_InitInfo stGopInit;
        GFX_Config stGFXcfg;
        memset(&stGopInit, 0, sizeof(GOP_InitInfo));
        memset(&stGFXcfg, 0, sizeof(GFX_Config));

        // -Initial GE
        stGFXcfg.u8Miu = 0;
        stGFXcfg.u8Dither = FALSE;
        stGFXcfg.u32VCmdQSize = 0;
        stGFXcfg.u32VCmdQAddr = 0;

        stGFXcfg.bIsCompt = TRUE;
        stGFXcfg.bIsHK = TRUE;
        MApi_GFX_Init(&stGFXcfg);
        DMS_PRINTF("driver GE init ok\n");

        stGopInit.u16PanelWidth = g_IPanel.Width();
        stGopInit.u16PanelHeight = g_IPanel.Height();
        stGopInit.u16PanelHStr = g_IPanel.HStart();

        MS_U32 u32DIPSize = (DMS_Frame_Buffer_Height * _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, DMS_Frame_Buffer_Width));
        MS_U32 u32DIPSizeWithDouble = u32DIPSize * 2; //double buffer

        stGopInit.u32GOPRBAdr = RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP];
        stGopInit.u32GOPRegdmaAdr = RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP];
        stGopInit.u32GOPRBLen = (MS_U32)u32DIPSizeWithDouble;
        stGopInit.u32GOPRegdmaLen = (MS_U32)u32DIPSizeWithDouble;
        stGopInit.bEnableVsyncIntFlip = FALSE;

        if( GOP_API_SUCCESS != MApi_GOP_InitByGOP(&stGopInit, u8GOP) )
        {
             DMS_PRINTF("[%s][%d] MApi_GOP_Init failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        MApi_GOP_GWIN_SwitchGOP(u8GOP);//use GOP 0
        MApi_GOP_GWIN_EnableTransClr(GOPTRANSCLR_FMT0, FALSE);

        if(GOP_API_SUCCESS != MApi_GOP_GWIN_SetGOPDst(u8GOP, E_GOP_DST_OP0))
        {
             printf("[%s][%d] HD MApi_GOP_GWIN_SetGOPDst failed", __FUNCTION__, __LINE__);
             return FALSE;
        }

        MApi_GOP_GWIN_Set_STRETCHWIN(u8GOP, E_GOP_DST_OP0, 0, 0, stGopInit.u16PanelWidth, stGopInit.u16PanelHeight);
        MApi_GOP_GWIN_OutputColor_EX(u8GOP, GOPOUT_YUV);

        pitch = _DMS_DIP_CalcPitch((EN_XC_DIP_DATA_FMT)eDIPWColorFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);

        switch(eDIPWColorFmt)
        {
            case DIP_DATA_FMT_YUV422:
            default:
                fbFmt = E_MS_FMT_YUV422;
                break;
        }

        if(RES_DMS.u8GOPGWinNum != 0xFF)
        {
            MApi_GOP_GWIN_DeleteWin (RES_DMS.u8GOPGWinNum);
        }
        //for Kano temporary use
        MApi_GOP_MIUSel(u8GOP, E_GOP_SEL_MIU0);

        if(MApi_GOP_GWIN_Create32FBFrom3rdSurf(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight, fbFmt, Address, pitch, &fbID) != GOP_API_SUCCESS)
        {
            DMS_PRINTF("[%s][%d] DMS, MApi_GOP_GWIN_Create32FBFrom3rdSurf: failed to create FB \n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        RES_DMS.u8GOPGWinNum = MApi_GOP_GWIN_CreateWin_Assign_32FB(u8GOP, fbID, 0, 0);

        if(_IsGwinAvailable() == FALSE)
        {
            return FALSE;
        }

        if( fbFmt == E_MS_FMT_ARGB8888 || fbFmt == E_MS_FMT_ARGB4444)
        {
            MApi_GOP_GWIN_SetBlending(RES_DMS.u8GOPGWinNum, true, 0xff);
        }
        else
        {
            MApi_GOP_GWIN_SetBlending(RES_DMS.u8GOPGWinNum, false, 0xff);
        }

        RES_DMS.bIsGOPInit = TRUE;
    }
    else
    {
        MS_U32 u32DIPSizeWithDouble = (DMS_Frame_Buffer_Height * _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, DMS_Frame_Buffer_Width)) * 2;
        _MDrv_DMS_MemData_Init(RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP], (MS_U32)u32DIPSizeWithDouble, DIP_DATA_FMT_YUV422);

        pitch = _DMS_DIP_CalcPitch((EN_XC_DIP_DATA_FMT)eDIPWColorFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth);

        switch(eDIPWColorFmt)
        {
            case DIP_DATA_FMT_YUV422:
            default:
                fbFmt = E_MS_FMT_YUV422;
                break;
        }

        //for Kano temporary use
        MApi_GOP_MIUSel(u8GOP, E_GOP_SEL_MIU0);

        MApi_GOP_GWIN_SetGWinShared(RES_DMS.u8GOPGWinNum, TRUE);

        if(MApi_GOP_GWIN_Create32FBFrom3rdSurf(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight, fbFmt, Address, pitch, &fbID) != GOP_API_SUCCESS)
        {
            DMS_PRINTF("[%s][%d] DMS, MApi_GOP_GWIN_Create32FBbyStaticAddr2: failed to create FB \n", __FUNCTION__, __LINE__);
            return FALSE;
        }
        //Use existing GWin to re-mapping
        MApi_GOP_GWIN_CreateStaticWin_Assign_32FB(RES_DMS.u8GOPGWinNum, u8GOP, fbID, 0, 0);

        if(_IsGwinAvailable() == FALSE)
        {
            return FALSE;
        }

        if( fbFmt == E_MS_FMT_ARGB8888 || fbFmt == E_MS_FMT_ARGB4444)
        {
            MApi_GOP_GWIN_SetBlending(RES_DMS.u8GOPGWinNum, true, 0xff);
        }
        else
        {
            MApi_GOP_GWIN_SetBlending(RES_DMS.u8GOPGWinNum, false, 0xff);
        }
    }

    return TRUE;
}

static void _MDrv_DMS_ZOrder_Init(void)
{
    DMS_GET_RES_PRI;
    MS_U8 u8Window = 0;
    memset(RES_DMS.WindowZOrder, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM);
    memset(RES_DMS.WindowScore, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM);

    for(u8Window = 0; u8Window < DMS_MAX_WINDOW_NUM; u8Window++)
    {
        RES_DMS.WindowZOrder[u8Window] = u8Window;
    }
}

#if(DMS_DONT_USE_CMA == 1)
struct CMA_Pool_Init_Param stDMSCMA_Pool_Init_PARAM[E_DMS_MEMORY_TYPE_MAX];
#endif
#define BPP_OF_YUV422 2
#define BPP_OF_ARGB8888 4
static MS_BOOL _MDrv_DMS_Setup_CMA(void* pInstance, EN_DMS_MEMORY_TYPE enMemType, MS_U32 u32CMAHeapID, MS_PHY* phyOutAddr)
{
#if(DMS_DONT_USE_CMA == 1)
    /* 1. do CMA_POOL_INIT */
    stDMSCMA_Pool_Init_PARAM[enMemType].heap_id     = u32CMAHeapID;
    stDMSCMA_Pool_Init_PARAM[enMemType].flags       = CMA_FLAG_MAP_VMA;
    if (stDMSCMA_Pool_Init_PARAM[enMemType].heap_id != DMS_INVALID_CMA_HEAP_ID)
    {
        MS_BOOL ret = MApi_CMA_Pool_Init(&stDMSCMA_Pool_Init_PARAM[enMemType]);
        if(ret == FALSE)
        {
            DMS_PRINTF("\033[35m   Function = %s, Line = %d, CMA_POOL_INIT ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            DMS_PRINTF("\033[35m  Function = %s, Line = %d, get pool_handle_id is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, stDMSCMA_Pool_Init_PARAM[enMemType].pool_handle_id);
            DMS_PRINTF("\033[35m  Function = %s, Line = %d, get miu is %u\033[m\n", __PRETTY_FUNCTION__,__LINE__, stDMSCMA_Pool_Init_PARAM[enMemType].miu);
            DMS_PRINTF("\033[35m  Function = %s, Line = %d, get heap_miu_start_offset is 0x%x\033[m\n", __PRETTY_FUNCTION__, __LINE__, (MS_U32)stDMSCMA_Pool_Init_PARAM[enMemType].heap_miu_start_offset);
            DMS_PRINTF("\033[35m  Function = %s, Line = %d, get heap_length is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, stDMSCMA_Pool_Init_PARAM[enMemType].heap_length);
        }
    }
    if(FALSE == HAL_DMS_Setup_CMA(pInstance,enMemType,stDMSCMA_Pool_Init_PARAM[enMemType].pool_handle_id))
    {
        DMS_PRINTF("\033[1;31m[%s][%d] error, HAL_DMS_Setup_CMA failed\033[0m\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        *phyOutAddr = stDMSCMA_Pool_Init_PARAM[enMemType].heap_miu_start_offset;
    }
#endif
    return TRUE;
}

static MS_BOOL _MDrv_DMS_Setup_Memory(void* pInstance, EN_DMS_MEMORY_TYPE enMemType, MS_PHY phyMemBufferAddr, MS_PHY u32MemBufferSize, MS_U32 u32CMAHeapID)
{
    MS_BOOL bRet = TRUE;
    DMS_GET_RES_PRI;
    switch(enMemType)
    {
        case E_DMS_MEMORY_TYPE_GOP_DISPLAY:
        {
            MS_U32 u32DIPSizeWithDouble = 0;
            if(u32CMAHeapID != DMS_INVALID_CMA_HEAP_ID)
            {
                if(FALSE == _MDrv_DMS_Setup_CMA(pInstance,enMemType,u32CMAHeapID,&phyMemBufferAddr))
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, _MDrv_DMS_Setup_CMA failed\033[0m\n",__FUNCTION__,__LINE__);
                    bRet = FALSE;
                }
                MS_U32 u32DIPSize = (DMS_Frame_Buffer_Height * _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, DMS_Frame_Buffer_Width));
                u32DIPSizeWithDouble = u32DIPSize * 2; //double buffer
            }
            else
            {
                MS_PHY phyMaxNeedSize = DMS_Frame_Buffer_Width * DMS_Frame_Buffer_Height * BPP_OF_YUV422 * 2;
#if (DMS_DIP_Support_HVSP == 0)
                phyMaxNeedSize += (DMS_Frame_Buffer_Width * DMS_Frame_Buffer_Height * BPP_OF_YUV422);
#endif
                if(phyMaxNeedSize > u32MemBufferSize)
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, buffer not enough u32MemBufferSize 0x%tx , need 0x%tx\033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32MemBufferSize, (ptrdiff_t)phyMaxNeedSize);
                    bRet = FALSE;
                }
                //DIP size
                MS_U32 u32DIPSize = (DMS_Frame_Buffer_Height * _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, DMS_Frame_Buffer_Width));
                u32DIPSizeWithDouble = u32DIPSize * 2; //double buffer
#if (DMS_DIP_Support_HVSP == 0)
                u32DIPSizeWithDouble += u32DIPSize;
#endif
            }

            if(bRet)
            {
                RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP] = phyMemBufferAddr;
                _MDrv_DMS_MemData_Init(RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP], (MS_U32)u32DIPSizeWithDouble, DIP_DATA_FMT_YUV422);
#if (DMS_DIP_Support_HVSP == 0)
                RES_DMS.u32GEAddress_pa = RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP] + u32DIPSizeWithDouble;
#endif
            }
            break;
        }
        case E_DMS_MEMORY_TYPE_FREEZE:
        {
            if(u32CMAHeapID != DMS_INVALID_CMA_HEAP_ID)
            {
                if(FALSE == _MDrv_DMS_Setup_CMA(pInstance,enMemType,u32CMAHeapID,&phyMemBufferAddr))
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, _MDrv_DMS_Setup_CMA failed\033[0m\n",__FUNCTION__,__LINE__);
                    bRet = FALSE;
                }
            }
            else
            {
                MS_PHY phyMaxNeedSize = DMS_FHD_Hszie * DMS_FHD_Vszie * BPP_OF_YUV422;
                if(phyMaxNeedSize > u32MemBufferSize)
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, buffer not enough u32MemBufferSize 0x%tx , need 0x%tx\033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32MemBufferSize, (ptrdiff_t)phyMaxNeedSize);
                    bRet = FALSE;
                }
            }
            if(bRet)
            {
                RES_DMS.u32MVOPFreezeAddress_pa = phyMemBufferAddr;
                _MDrv_DMS_MemData_Init(RES_DMS.u32MVOPFreezeAddress_pa, (MS_U32)u32MemBufferSize, DIP_DATA_FMT_YUV422);
            }
            break;
        }
        case E_DMS_MEMORY_TYPE_MVOP_DISPLAY:
        {
            if(u32CMAHeapID != DMS_INVALID_CMA_HEAP_ID)
            {
                if(FALSE == _MDrv_DMS_Setup_CMA(pInstance,enMemType,u32CMAHeapID,&phyMemBufferAddr))
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, _MDrv_DMS_Setup_CMA failed\033[0m\n",__FUNCTION__,__LINE__);
                    bRet = FALSE;
                }
            }
            else
            {
                MS_PHY phyMaxNeedSize = DMS_FHD_Hszie * DMS_FHD_Vszie * BPP_OF_YUV422 * DMS_DI_RING_BUFFER;
                if(phyMaxNeedSize > u32MemBufferSize)
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, buffer not enough u32MemBufferSize 0x%tx , need 0x%tx\033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32MemBufferSize, (ptrdiff_t)phyMaxNeedSize);
                    bRet = FALSE;
                }
            }
            if(bRet)
            {
                MS_U32 u32DIPMVOPSize = (RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelHeight * _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth));
                MS_U32 u32DIPMVOPSizeWithDouble = u32DIPMVOPSize * DMS_DI_RING_BUFFER; //double buffer
                RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_MVOP] = phyMemBufferAddr;
                _MDrv_DMS_MemData_Init(RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_MVOP], (MS_U32)u32DIPMVOPSizeWithDouble, DIP_DATA_FMT_YUV422);
            }
            break;
        }
        case E_DMS_MEMORY_TYPE_DIPDI:
        {
#if DMS_DIP_Support_3DDI
            if(u32CMAHeapID != DMS_INVALID_CMA_HEAP_ID)
            {
                if(FALSE == _MDrv_DMS_Setup_CMA(pInstance,enMemType,u32CMAHeapID,&phyMemBufferAddr))
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, _MDrv_DMS_Setup_CMA failed\033[0m\n",__FUNCTION__,__LINE__);
                    bRet = FALSE;
                }
            }
            else
            {
                MS_PHY phyMaxNeedSize = (4/2) * DMS_FHD_Hszie * DMS_FHD_Vszie * BPP_OF_ARGB8888;
                if(phyMaxNeedSize > u32MemBufferSize)
                {
                    DMS_PRINTF("\033[1;31m[%s][%d] error, buffer not enough u32MemBufferSize 0x%tx , need 0x%tx\033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32MemBufferSize, (ptrdiff_t)phyMaxNeedSize);
                    bRet = FALSE;
                }
            }
            if(bRet)
            {
                RES_DMSDI.u32DIPDIAddress_pa = phyMemBufferAddr;
                _MDrv_DMS_MemData_Init(RES_DMSDI.u32DIPDIAddress_pa, (MS_U32)u32MemBufferSize, DIP_DATA_FMT_YUV422);
            }

#if (DEV_DI == 1)
            _MDrv_DMS_DI_Init();
#endif
#endif
            break;
        }
        default:
        {
            DMS_PRINTF("\033[1;31m[%s][%d] unuse mem type %d\033[0m\n",__FUNCTION__,__LINE__,enMemType);
            break;
        }
    }
    return bRet;
}

static MS_BOOL _MDrv_DMS_Init(void* pInstance, MS_U8 u8GOP, EN_DMS_DIP_FMT eDIPWColorFmt, MS_PHY phyMemBufferAddr, MS_U32 u32MemBufferSize)
{
    DMS_GET_RES_PRI;
    if(pDMSResPri->bDrvInit == TRUE)
    {
        DMS_PRINTF("Driver has inited!!!!!!!!\n");
        return TRUE;
    }
    // Create Event
    if(XC_Vsync_Event_id < 0)
    {
        XC_Vsync_Event_id = MsOS_CreateEventGroup("DMS_XC_Vsync_Event");
    }
#if (DEV_DI == 1)
    // Create  Di Event
    if(XC_Di_Push_Event_id < 0)
    {
        XC_Di_Push_Event_id = MsOS_CreateEventGroup("DMS_XC_Di_Push_Event");
    }
#endif
#if DEV_CAPMODE
    if(CaptureMode_Event_id < 0)
    {
        CaptureMode_Event_id = MsOS_CreateEventGroup("DMS_CaptureMode_Event");
    }
#endif
#if DMS_DIP_Support_IMI
    if(StartFRC_Event_id < 0)
    {
        StartFRC_Event_id = MsOS_CreateEventGroup("DMS_IMI_StartFRC_Event");
    }
#endif
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
     // create MVOP event
     if(s32MVOP_Event_id < 0)
    {
        s32MVOP_Event_id = MsOS_CreateEventGroup("DMS_MVOP_Event");
    }
    // create Disable Bob Mode event
    if(s32DisableBob_Event_id < 0)
    {
        s32DisableBob_Event_id = MsOS_CreateEventGroup("DMS_DisableBob_Event");
    }
#endif
    // inital DIP
    MApi_XC_DIP_InitByDIP(DIP_WINDOW);
    MApi_XC_DIP_EnaInt(0xFF, FALSE, DIP_WINDOW);

    MApi_XC_DIP_InitByDIP(DWIN0_WINDOW);
    MApi_XC_DIP_EnaInt(0xFF, FALSE, DWIN0_WINDOW);

    MApi_XC_DIP_InitByDIP(DWIN1_WINDOW);
    MApi_XC_DIP_EnaInt(0xFF, FALSE, DWIN1_WINDOW);

    // Store DIP need data
    RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt = (EN_XC_DIP_DATA_FMT)eDIPWColorFmt;
    RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth = g_IPanel.Width();
    RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight = g_IPanel.Height();

    // Store DIP to MVOP need Data (force value)
    RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].eDIPWFmt = (EN_XC_DIP_DATA_FMT)eDIPWColorFmt;
    RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelWidth = DMS_FHD_Hszie;
    RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MVOP].u32PanelHeight = DMS_FHD_Vszie;

    MApi_XC_DIP_SetOutputDataFmt(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, DMS_DIPWindow_NORMAL);
    if(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt == E_DMS_DIP_FMT_YUV422)
    {
        MApi_XC_DIP_SwapUV(TRUE ,DMS_DIPWindow_NORMAL);
    }

    // Attach ISR and Set STB_DISP_IRQ
    MApi_XC_InterruptAttach(SC_INT_VSINT, _DMS_EmbedXCISR, NULL);

    // Clear VDEC info
    memset(RES_DMS._stXC_DIPPushInfo, 0x0, sizeof(DMS_VDECFRAME_INFO)*DMS_DIP_QUEUEDEPTH);
    memset(&RES_DMS._stXC_DIPWriteReadPointer, 0x0, sizeof(DMS_DIPWriteReadPointer));
    memset(RES_DMS._stXC_DIPWinInfo, 0x0, sizeof(DMS_DIPWIN_INFO)*DMS_MAX_WINDOW_NUM);
    memset(RES_DMS.u32WindowFrameRate, 0x0, sizeof(RES_DMS.u32WindowFrameRate));

    _MDrv_DMS_ZOrder_Init();

    pTaskInstance = pInstance;
    _MDrv_DMS_CreateTask();

    _MDrv_DMS_InvalidateAllQueue();

#if (DEV_DI == 1)
    memset(&RES_DMSDI._stXC_DIPWriteReadPointerInterlace, 0x0, sizeof(DMS_DIPWriteReadPointer));
#endif

    if(DMS_MVOP_FLOW)
    {
        _DMS_Add_XC_Status(MVOP_WINDOW, DMS_XC_BLACK_SCREEN_BY_WINUSED | DMS_XC_NO_FRAME | DMS_XC_FIRST_TIME_SETWINDOW);
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
        //set MVOP config
        _DMS_MVOP_SetConfig(MVOP_INPUT_DRAM,TRUE,0);

        _DMS_XC_SetConnect(INPUT_SOURCE_DTV);
        MApi_XC_EnableFrameBufferLess(TRUE);
#endif
        RES_DMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_FROM_VDEC;
    }
    pDMSResPri->bDrvInit = TRUE;

    return TRUE;
}

static void _DMS_Regen_CropWindow(MS_U16 u16NewSrcWidth, MS_U16 u16NewSrcHeight)
{
    DMS_GET_RES_PRI;
    ST_DMS_WINDOW* pstSrcCropWin = &RES_DMS._stXC_XCCropWinInfo_raw;
    ST_DMS_WINDOW stCropWin;
    stCropWin.u32x = pstSrcCropWin->u32x * u16NewSrcWidth/ RES_DMS.u16FirstSrcWidth;
    stCropWin.u32width = pstSrcCropWin->u32width * u16NewSrcWidth / RES_DMS.u16FirstSrcWidth;
    stCropWin.u32y = pstSrcCropWin->u32y * u16NewSrcHeight / RES_DMS.u16FirstSrcHeight;
    stCropWin.u32height = pstSrcCropWin->u32height * u16NewSrcHeight / RES_DMS.u16FirstSrcHeight;
    _DMS_XC_WindowAlignto2(&stCropWin);
    DMS_PRINTF("new crop size:%td %td, user crop size:%td %td, src is: %d %d, base on: %d %d\n",
        (ptrdiff_t)stCropWin.u32width,(ptrdiff_t)stCropWin.u32height,
        (ptrdiff_t)RES_DMS._stXC_XCCropWinInfo_raw.u32width,(ptrdiff_t)RES_DMS._stXC_XCCropWinInfo_raw.u32height,
        u16NewSrcWidth,u16NewSrcHeight,RES_DMS.u16FirstSrcWidth,RES_DMS.u16FirstSrcHeight);
    memcpy(&RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stCropWin,&stCropWin,sizeof(stCropWin));
    memcpy(&RES_DMS._stXC_XCCropWinInfo_DoubleBuffer,&stCropWin,sizeof(stCropWin));
}

static void _DMS_Clear_VDEC_Frame_Ref(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_GET_RES_PRI;
    MS_U8 u8RefCounter = 0;
    for(u8RefCounter = 0; u8RefCounter < RES_DMS._stXC_LocalFrameRefCount[u32Window][u16BufID]; u8RefCounter++)
    {
        DMS_PRINTF("[%s] window %td release frame for WP %d ct %d\n",__FUNCTION__,(ptrdiff_t)u32Window,u16BufID,(u8RefCounter+1));
        _DMS_VDEC_Frame_Release(u32Window,u16BufID);
    }
}

static void _DMS_StoreVDECInfo2DIPQueue(void* pInstance, DMS_VDECFRAME_INFO *VDECDispFrame)
{
    DMS_GET_RES_PRI;
    MS_U16 u16NextWritePointer = 0;
    MS_U32 u32Window = VDECDispFrame->u32Window;
    MS_U16 u16RP = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16ReadPointer;

    //Jump to Next Write Pointer
    u16NextWritePointer = _GetNextWritePointer(u32Window);
    //DMS_PRINTF("=====PUSH Frame=%d====",u16NextWritePointer);

    while(_GetNextBufferID(u16NextWritePointer) == u16RP)
    {
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        //DMS_PRINTF("\033[31m [%s] window %d Force block flip when Read Write Diff < 2 (%d) \033[0m\n",__FUNCTION__, u32Window, u16RP);
        MsOS_DelayTask(1);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        u16RP = RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16ReadPointer;
    }

#if NEW_FLIP
    if(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32FrameIndex != DMS_INVALID_FRAME_ID)
    {
        if(DEV_NEW_NEXT == 0)
        {
            if(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].bValid == TRUE)
            {
                //VDEC push too fast
                DMS_PRINTF("[%s] (Window, Buffer) (%td, %d) valid check ring back, Time %td \n",__FUNCTION__, (ptrdiff_t)u32Window, u16NextWritePointer, (ptrdiff_t)MsOS_GetSystemTime());
                _DMS_Clear_VDEC_Frame_Ref(u32Window,u16NextWritePointer);
            }
            if(RES_DMS._stXC_LocalFrameRefCount[u32Window][u16NextWritePointer] > 0)
            {
                DMS_PRINTF("[%s] LocalFrameRefCount check ring back\n",__FUNCTION__);
                _DMS_Clear_VDEC_Frame_Ref(u32Window,u16NextWritePointer);
            }
        }
        else
        {
            while(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].bValid == TRUE)
            {
                MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
                //DMS_PRINTF("[%s] win %d, Force block flip when bValid\n",__FUNCTION__, u32Window);
                MsOS_DelayTask(1);
                MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
            }
        }
    }
#endif

#if DEV_CAPMODE
    if((_DMS_Is_MVOPFlow(u32Window) == FALSE) && _DMS_IsCaptureWindowEnable(u32Window))
    {
        if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].u32Visible == FALSE)
        {
            // case: full speed decode
            MS_U16 u16Prev = u16NextWritePointer;
            u16Prev = _GetPreviousBufferID(u16Prev);
            u16Prev = _GetPreviousBufferID(u16Prev);
            if(RES_DMS._stXC_DIPPushInfo[u32Window][u16Prev].bValid == TRUE)
            {
                //DMS_PRINTF("\033[32m [%s] (Window, Buffer) (%d, %d) clear prev 2, Time %d \033[0m\n",__FUNCTION__, u32Window, u16Prev, MsOS_GetSystemTime());
                _DMS_Clear_VDEC_Frame_Ref(u32Window,u16Prev);
                RES_DMS._stXC_DIPPushInfo[u32Window][u16Prev].bValid = FALSE;
            }
        }
    }
#endif

    memcpy(&RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer], VDECDispFrame, sizeof(DMS_VDECFRAME_INFO));

    // New Flip
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32FrameIndex = VDECDispFrame->u32FrameIndex;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32VDECStreamID = VDECDispFrame->u32VDECStreamID;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32VDECStreamVersion = VDECDispFrame->u32VDECStreamVersion;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32PriData = VDECDispFrame->u32PriData;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u64Pts = VDECDispFrame->u64Pts;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eScanType = VDECDispFrame->eScanType;
    if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eScanType))
    {
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32FrameIndex_2nd = VDECDispFrame->u32FrameIndex_2nd;
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32PriData_2nd = VDECDispFrame->u32PriData_2nd;
    }
    _DMS_VDEC_Frame_AddRef(u32Window,u16NextWritePointer);

#if DMS_DIP_Support_IMI
    if(_DMS_IsCaptureWindowEnable(u32Window))
    {
        RES_DMSCAPTURE.bIsFrameValid[u32Window][u16NextWritePointer] = TRUE;
        _DMS_VDEC_Frame_AddRef_CaptureMode(u32Window,u16NextWritePointer);
        printf("\033[32m [%d] === win[%td], buf[%d]: %td === \033[0m\n",__LINE__,(ptrdiff_t)u32Window,u16NextWritePointer,(ptrdiff_t)MsOS_GetSystemTime());
        RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u32Window].u16WritePointer = u16NextWritePointer;
    }

    if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32Window].bStartGetCapture == TRUE)
    {
        if(_DMS_TriggerToCaptureFrame(pInstance, u32Window) && (RES_DMSCAPTURE._stXC_IMICaptureInfo[u32Window].bFirstCapFrame == FALSE))
        {
            MsOS_SetEvent(StartFRC_Event_id, BIT(u32Window));
        }
    }
    else
    {
        MS_U16 u16Prev = u16NextWritePointer;
        MS_U8 i;
        for(i = 0 ; i < (START_FRC_ACCUMULE_FRAME_COUNT-1); i++)
        {
            u16Prev = _GetPreviousBufferID(u16Prev);
        }
        if(RES_DMSCAPTURE.bIsFrameValid[u32Window][u16Prev])
        {
            DMS_PRINTF("\033[32m [%s] IMI flow, win %td, clear prev 2 (buf: %d) \033[0m\n",__FUNCTION__, (ptrdiff_t)u32Window, u16Prev);
            _DMS_VDEC_Frame_Release_CaptureMode(u32Window, u16Prev);
            RES_DMSCAPTURE.bIsFrameValid[u32Window][u16Prev] = FALSE;
        }
    }
#else
    if(_DMS_IsCaptureWindowEnable(u32Window))
    {
        RES_DMSCAPTURE.bIsFrameValid[u32Window][u16NextWritePointer] = TRUE;
        _DMS_VDEC_Frame_AddRef_CaptureMode(u32Window,u16NextWritePointer);
        RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u32Window].u16WritePointer = u16NextWritePointer;
        //printf("u16WritePointer (%d, %d)", u32Window, RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u32Window].u16WritePointer);
        if(_DMS_TriggerToCaptureFrame(pInstance, u32Window))
        {
            printf("\033[33m setEvent %d, time %d \033[0m\n", BIT(u32Window), MsOS_GetSystemTime());
            MsOS_SetEvent(CaptureMode_Event_id, BIT(u32Window));
        }
    }
#endif
    sync_print("#### Push %d ####",u16NextWritePointer);
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].bValid= TRUE;
    //printf("\033[1;31m[%d] Push to Main %d, RBID %d\033[0m\n",MsOS_GetSystemTime(),u16NextWritePointer, VDECDispFrame->u8DiOutputRingBufferID);

    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcWidth = VDECDispFrame->u16SrcWidth - VDECDispFrame->u16CropLeft - VDECDispFrame->u16CropRight;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcHeight = VDECDispFrame->u16SrcHeight - VDECDispFrame->u16CropTop - VDECDispFrame->u16CropBottom;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcPitch = VDECDispFrame->u16SrcPitch;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32SrcLumaAddr = VDECDispFrame->u32SrcLumaAddr;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32SrcChromaAddr = VDECDispFrame->u32SrcChromaAddr;

    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32FrameRate = VDECDispFrame->u32FrameRate;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFmt = VDECDispFrame->eFmt;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32Window = VDECDispFrame->u32Window;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eCODEC = VDECDispFrame->eCODEC;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldOrderType = VDECDispFrame->eFieldOrderType;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldType = VDECDispFrame->eFieldType;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eTileMode = VDECDispFrame->eTileMode;

    if(_DMS_Is_SourceFieldBaseInterlace(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eScanType))
    {
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32FrameIndex_2nd = VDECDispFrame->u32FrameIndex_2nd;
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32PriData_2nd = VDECDispFrame->u32PriData_2nd;
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32SrcLumaAddrI = VDECDispFrame->u32SrcLumaAddrI;
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32SrcChromaAddrI = VDECDispFrame->u32SrcChromaAddrI;
        if(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldOrderType == E_DMS_VIDEO_FIELD_ORDER_TYPE_TOP)
        {
            if(VDECDispFrame->bIs2ndField == TRUE)
            {
                RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldType = E_DMS_VIDEO_FIELD_TYPE_BOTTOM;
            }
            else
            {
                RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldType = E_DMS_VIDEO_FIELD_TYPE_TOP;
            }
        }
        else
        {
            if(VDECDispFrame->bIs2ndField == TRUE)
            {
                RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldType = E_DMS_VIDEO_FIELD_TYPE_TOP;
            }
            else
            {
                RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eFieldType = E_DMS_VIDEO_FIELD_TYPE_BOTTOM;
            }
        }
    }

    // MFDec info
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.bMFDec_Enable = VDECDispFrame->stMFdecInfo.bMFDec_Enable;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
    if(u32Window == MVOP_WINDOW)
    {
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.u8MFDec_Select = 0;
    }
    else
#endif
    {
        RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.u8MFDec_Select = VDECDispFrame->stMFdecInfo.u8MFDec_Select;
    }
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.bUncompress_mode = VDECDispFrame->stMFdecInfo.bUncompress_mode;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.bBypass_codec_mode = VDECDispFrame->stMFdecInfo.bBypass_codec_mode;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.en_MFDecVP9_mode = VDECDispFrame->stMFdecInfo.en_MFDecVP9_mode;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.u16StartX = VDECDispFrame->stMFdecInfo.u16StartX;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.u16StartY = VDECDispFrame->stMFdecInfo.u16StartY;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.phyBitlen_Base = VDECDispFrame->stMFdecInfo.phyBitlen_Base;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].stMFdecInfo.u16Bitlen_Pitch = VDECDispFrame->stMFdecInfo.u16Bitlen_Pitch;
    // 10 bits
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].b10bitData = VDECDispFrame->b10bitData;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16Src10bitPitch = VDECDispFrame->u16Src10bitPitch;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32SrcLumaAddr_2bit = VDECDispFrame->u32SrcLumaAddr_2bit;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u32SrcChromaAddr_2bit = VDECDispFrame->u32SrcChromaAddr_2bit;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u8LumaBitdepth = VDECDispFrame->u8LumaBitdepth;

    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].bIsAfterDiTask = VDECDispFrame->bIsAfterDiTask;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].bIs2ndField = VDECDispFrame->bIs2ndField;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u8DiOutputRingBufferID = VDECDispFrame->u8DiOutputRingBufferID;

    RES_DMS._stXC_DIPWriteReadPointer[u32Window].u16WritePointer = u16NextWritePointer;
    RES_DMS.u32WindowFrameRate[u32Window] = VDECDispFrame->u32FrameRate/1000;

    //2nd buffer
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].phySrc2ndBufferLumaAddr = VDECDispFrame->phySrc2ndBufferLumaAddr;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].phySrc2ndBufferChromaAddr = VDECDispFrame->phySrc2ndBufferChromaAddr;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16Src2ndBufferPitch = VDECDispFrame->u16Src2ndBufferPitch;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u8Src2ndBufferV7DataValid = VDECDispFrame->u8Src2ndBufferV7DataValid;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16Src2ndBufferWidth = VDECDispFrame->u16Src2ndBufferWidth;
    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16Src2ndBufferHeight = VDECDispFrame->u16Src2ndBufferHeight;

    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u8FieldCtrl = VDECDispFrame->u8FieldCtrl;


#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
    if(u32Window == MVOP_WINDOW)
    {
        RES_DMS.u32ReleaseState[u16NextWritePointer] = 0;
    }
#endif

    if(_DMS_Is_MVOPFlow(u32Window))
    {
        if(_DMS_Query_XC_Status(u32Window, DMS_XC_NO_FRAME))
        {
            _DMS_Remove_XC_Status(u32Window, DMS_XC_NO_FRAME);
            RES_DMS.u16FirstSrcWidth = RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcWidth;
            RES_DMS.u16FirstSrcHeight = RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcHeight;
        }
        else //if(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].bIsAfterDiTask == FALSE)
        {
            MS_U16 u16PreID = _GetPreviousWritePointer(u32Window);
            if(RES_DMS._stXC_DIPPushInfo[u32Window][u16PreID].eScanType != RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eScanType)
            {
                DMS_PRINTF("\033[1;35m###[%s][%d][%td]### SeqChange (eScanType change %d -> %d) !!!!!!\033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)MsOS_GetSystemTime(),RES_DMS._stXC_DIPPushInfo[u32Window][u16PreID].eScanType,RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].eScanType);
                _DMS_Add_XC_Status(u32Window, DMS_XC_SEQ_CHANGE_PI);
                RES_DMS.gu8DMS_Seq_Change_ID = u16NextWritePointer;
            }

            if((RES_DMS._stXC_DIPPushInfo[u32Window][u16PreID].u16SrcWidth != RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcWidth) ||
                (RES_DMS._stXC_DIPPushInfo[u32Window][u16PreID].u16SrcHeight != RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcHeight))
            {
                DMS_PRINTF("\033[1;35m###[%s][%d][%td]### SeqChange at WP:%d (Size change (%d,%d) -> (%d,%d)) !!!!!!\033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)MsOS_GetSystemTime(),u16NextWritePointer,RES_DMS._stXC_DIPPushInfo[u32Window][u16PreID].u16SrcWidth,RES_DMS._stXC_DIPPushInfo[u32Window][u16PreID].u16SrcHeight,RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcWidth,RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcHeight);
                _DMS_Add_XC_Status(u32Window, DMS_XC_SEQ_CHANGE_SIZE);
                RES_DMS.gu8DMS_Seq_Change_ID = u16NextWritePointer;
                printf("\033[1;35m###[%s][%d][%td]### %d %d %d %d %d %d !!!!!!\033[0m\n",__FUNCTION__,__LINE__,
                    (ptrdiff_t)MsOS_GetSystemTime(),
                    VDECDispFrame->u16SrcWidth, VDECDispFrame->u16SrcHeight,
                    VDECDispFrame->u16CropLeft, VDECDispFrame->u16CropRight,
                    VDECDispFrame->u16CropTop, VDECDispFrame->u16CropBottom);

                _DMS_Regen_CropWindow(RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcWidth,
                    RES_DMS._stXC_DIPPushInfo[u32Window][u16NextWritePointer].u16SrcHeight);
            }

        }
    }
    //DMS_PRINTF("scan %d, fieldType %d, fieldOrderType %d, LAddr : 0x%llx, CAddr : 0x%llx\n",VDECDispFrame->eScanType,VDECDispFrame->eFieldType,VDECDispFrame->eFieldOrderType,
    //    VDECDispFrame->u32SrcLumaAddr,VDECDispFrame->u32SrcChromaAddr);

}

static void _CalculateMaxOutputFrameRate(void)
{
    DMS_GET_RES_PRI;
    MS_U16 u16OutVFreq = g_IPanel.DefaultVFreq() /10;
    //if(RES_DMS.u8EnableWindowCount == 0)
    if(RES_DMS.u8UseDIPWindowCount == 0)
    {
        RES_DMS.u32MaxOutputFrameRate = u16OutVFreq;
    }
    else
    {
        //RES_DMS.u32MaxOutputFrameRate = 1000 / (DMS_DIP_TIMESLICE * RES_DMS.u8EnableWindowCount);
        RES_DMS.u32MaxOutputFrameRate = 1000 / (DMS_DIP_TIMESLICE * RES_DMS.u8UseDIPWindowCount);
    }

    //reset VsyncCount
    RES_DMS.u32VsyncCount = 0;
    RES_DMS.u32GOPDispCount = 0;
    RES_DMS.u32OutputRate = ((MS_U32)g_IPanel.DefaultVFreq()) * 100;

    //DMS_PRINTF("DIP_TimeSlice = %d \n", DMS_DIP_TIMESLICE);
    //DMS_PRINTF("u8Enablu32WindowCount = %d \n", u8Enablu32WindowCount);
    //DMS_PRINTF("u32MaxOutputFrameRate = %d \n", RES_DMS.u32MaxOutputFrameRate);

}

static MS_BOOL _MDrv_DMS_Final(void* pInstance, MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    if(_DMS_Is_MVOPFlow(u32Window))
    {
        Hal_DMS_XC_SetMute(pInstance, TRUE);
        RES_DMS.gDMS_MVOPShowBufferID = DMS_Invalid_ReadPointer_ID;
        RES_DMS.gDMS_MVOPRemoveBufferID = DMS_Invalid_ReadPointer_ID;
        RES_DMS.bMVOPMarkRemoveBuffer = FALSE;
        MS_U8 u8BufID = 0;
        for(u8BufID = 0; u8BufID < DMS_DIP_QUEUEDEPTH; u8BufID++)
        {
            _DMS_Clear_VDEC_Frame_Ref(u32Window, u8BufID);
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
            RES_DMS.u32ReleaseState[u8BufID] = 0;
#endif
        }
        _DMS_Add_XC_Status(u32Window, DMS_XC_RESET_WINDOW_SIZE | DMS_XC_BLACK_SCREEN_BY_WINUSED | DMS_XC_NO_FRAME | DMS_XC_FIRST_TIME_SETWINDOW);
        RES_DMSDI._u8DMS_DiSetEventBufferID = DMS_Invalid_ReadPointer_ID;
        RES_DMSDI._u8DMS_DiReleaseVDECFrameBufferID = DMS_Invalid_ReadPointer_ID;
        for(u8BufID = 0; u8BufID < DMS_DIP_QUEUEDEPTH; u8BufID++)
        {
            _DMS_Clear_VDEC_Frame_Ref_Interlace(u32Window, u8BufID);
        }
        RES_DMS.bDMS_MVOPFlowFromDi = FALSE;
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
        RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16ReadPointer = 0;
        RES_DMS._stXC_DIPWriteReadPointer[MVOP_WINDOW].u16WritePointer = 0;
#endif
    }
    RES_DMS.bStartCountFRC[u32Window] = FALSE;
    //Close Win and Show No Signal Color
    _DMS_DisableByWin(u32Window);

    //Clean Pushframe Info in queue;
    _DMS_InvalidateQueueByWin(u32Window);

    //Check if exist any window and DeInit when no window is open
    _DMS_DeInitByWin(u32Window);

    _CalculateMaxOutputFrameRate();

    _DMS_XC_SetDisconnect(INPUT_SOURCE_DTV);

    return TRUE;
}

//Set Display Window Change Flag
static MS_BOOL _DMS_IsDispWindowChange (MS_U32 u32Window, ST_DMS_WINDOW *ptDstWin)
{
    DMS_GET_RES_PRI;
    if (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x != ptDstWin->u32x)
        return TRUE;

    if (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y != ptDstWin->u32y)
        return TRUE;

    if (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width != ptDstWin->u32width)
        return TRUE;

    if (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height != ptDstWin->u32height)
        return TRUE;

    return FALSE;
}

static void _DMS_SetDispWindowChangeFlag (MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    //Raise the flag "Display Window Change"
    RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_0][u32Window].bDispChange = TRUE;
    RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_1][u32Window].bDispChange = TRUE;
}

//Set Display Window Change Flag
static MS_BOOL _DMS_GetDispWindowChangeFlag (MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    //Raise the flag "Display Window Change"
    if (RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_0][u32Window].bDispChange == TRUE)
    {
        return TRUE;
    }

    if (RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_1][u32Window].bDispChange == TRUE)
    {
        return TRUE;
    }

    return FALSE;
}

//Store Display Window Info as Previous one
static void _DMS_StorePreviousWindowInfo (MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    RES_DMS._stXC_PreDispWinInfo[u32Window].u32x = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x;
    RES_DMS._stXC_PreDispWinInfo[u32Window].u32y = RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y;
    RES_DMS._stXC_PreDispWinInfo[u32Window].u32width= RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width;
    RES_DMS._stXC_PreDispWinInfo[u32Window].u32height= RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height;
}

static MS_BOOL _DMS_CheckIfDispWindowChange (MS_U32 u32Window, ST_DMS_WINDOW *ptDstWin)
{
    if (_DMS_IsWindowUsed(u32Window) == TRUE)
    {
        if (_DMS_IsDispWindowChange(u32Window, ptDstWin) == TRUE)
        {
            //If (Window Change Flag != TRUE), Store Old Window info
            if (_DMS_GetDispWindowChangeFlag(u32Window) != TRUE)
            {
                _DMS_StorePreviousWindowInfo(u32Window);
            }

            //Set Display Window Change Flag
            _DMS_SetDispWindowChangeFlag(u32Window);
        }
    }
    return TRUE;
}

//Condition: (HAL Level function?)
//0:scaling down 90%             Use XC scaling,
//1:scaling down 90% ~ 2K,    Use 2nd buffer. change to XC enlarge/same case
//2:scaling 2Kdown ~ more,     Use 2nd buffer. DIP scaling down

static void _DMS_XC_Set_ScalingDown_Condition(ST_DMS_WINDOW *ptCropWin, ST_DMS_WINDOW *ptDstWin)
{
    //condition check
    DMS_GET_RES_PRI;
    RES_DMS.gDMS_ScalingCondition = 0;

    // Decide scaling Strategy Only if Crop window larger than FHD
    if(_DMS_WindowSizeOverFHD(ptCropWin->u32width,ptCropWin->u32height))
    {
        if(ptDstWin->u32width >= ptCropWin->u32width &&
           ptDstWin->u32height >= ptCropWin->u32height)
        {
            //4k in 4k out
            //3840 in 4096 out,scaling up, Use XC scaling
            DMS_PRINTF("[%s] H %td->%td, V %td->%td, enlarge case\n",__FUNCTION__,
                (ptrdiff_t)ptCropWin->u32width,(ptrdiff_t)ptDstWin->u32width,
                (ptrdiff_t)ptCropWin->u32height,(ptrdiff_t)ptDstWin->u32height);
            RES_DMS.gDMS_ScalingCondition |= USE_XC_FBL;
        }
        else if(ptDstWin->u32width >= ((ptCropWin->u32width * 9)/10) &&
            ptDstWin->u32height >= ((ptCropWin->u32height * 9)/10))
        {
            //scaling down 90%up, Use XC scaling
            DMS_PRINTF("[%s] H %td->%td, V %td->%td, 90up case, scaling by XC\n",__FUNCTION__,
                (ptrdiff_t)((ptCropWin->u32width * 9)/10),(ptrdiff_t)ptDstWin->u32width,
                (ptrdiff_t)((ptCropWin->u32height * 9)/10),(ptrdiff_t)ptDstWin->u32height);
            RES_DMS.gDMS_ScalingCondition |= USE_XC_SCALING_DOWN;
        }
        else if(ptDstWin->u32width >= (ptCropWin->u32width/2) &&
            ptDstWin->u32height >= (ptCropWin->u32height/2))
        {
            //scaling down 90% ~ 2K, Use 2nd buffer. change to XC enlarge/same case
            DMS_PRINTF("[%s] H %td->%td, V %td->%td, 90 to 2K, 2nd buffer scaling up by XC\n",__FUNCTION__,
                (ptrdiff_t)(ptCropWin->u32width/2),(ptrdiff_t)ptDstWin->u32width,
                (ptrdiff_t)(ptCropWin->u32height/2),(ptrdiff_t)ptDstWin->u32height);
            RES_DMS.gDMS_ScalingCondition |= USE_2ND_BUFFER;
        }
        else
        {
            DMS_PRINTF("[%s] H %td->%td, V %td->%td, 2K ~ more, 2nd buffer scaling down\n",__FUNCTION__,
                (ptrdiff_t)ptCropWin->u32width,(ptrdiff_t)ptDstWin->u32width,
                (ptrdiff_t)ptCropWin->u32height,(ptrdiff_t)ptDstWin->u32height);
            RES_DMS.gDMS_ScalingCondition |= USE_2ND_BUFFER;
        }
    }
}

static MS_BOOL _MDrv_DMS_SetWin(void* pInstance, ST_DMS_WINDOW *ptCropWin, ST_DMS_WINDOW *ptDstWin, MS_U32 u32Window)
{
    DMS_GET_RES_PRI;
    MS_U32 BytesPerLine = 0;
    MS_U32 UHDBytesPerLine = 0;
    ST_DMS_WINDOW stCropWin = {0};
    ST_DMS_WINDOW stDispWin = {0};
    E_DMS_DIP_CLIENT eDIPClient = E_DMS_DIP_CLIENT_GOP;

    if(ptCropWin == NULL || ptDstWin == NULL)
    {
        DMS_PRINTF("[%s] error, Set window parameter cannot be NULL\n",__FUNCTION__);
        return FALSE;
    }

    memcpy(&stCropWin,ptCropWin,sizeof(stCropWin));
    memcpy(&stDispWin,ptDstWin,sizeof(stDispWin));

    if(_DMS_Is_MVOPFlow(u32Window))
    {
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
        _DMS_XC_Set_ScalingDown_Condition(&stCropWin, &stDispWin);
        if(RES_DMS.gDMS_ScalingCondition & USE_2ND_BUFFER)
        {
            stCropWin.u32width /= 2;
            stCropWin.u32height /= 2;
        }
#endif
    }
    else
    {
        _DMS_Add_XC_Status(u32Window, DMS_XC_UPDATE_FROM_VDEC);
        _DMS_Remove_XC_Status(u32Window, DMS_XC_FIRST_FRAME_FROM_VDEC);
        RES_DMS.bIsWindowMute[u32Window] = FALSE;
    }

#if DBG_FPS
    bFPS_Start[u32Window] = TRUE;
#endif
    _DMS_XC_WindowAlignto2(&stCropWin);
    _DMS_XC_WindowAlignto2(&stDispWin);
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    if(_DMS_Is_MVOPFlow(u32Window))
    {
        eDIPClient = E_DMS_DIP_CLIENT_MVOP;
        memcpy(&RES_DMS._stXC_XCCropWinInfo_DoubleBuffer,&stCropWin,sizeof(stCropWin));
        memcpy(&RES_DMS._stXC_XCDispWinInfo_DoubleBuffer,&stDispWin,sizeof(stCropWin));
        _DMS_Add_XC_Status(u32Window, DMS_XC_RESET_WINDOW_SIZE);
        if(_DMS_Query_XC_Status(u32Window, DMS_XC_NO_FRAME))
        {
            memcpy(&RES_DMS._stXC_XCCropWinInfo_raw,&stCropWin,sizeof(stCropWin));
        }
#if DEV_DI
        if(_DMS_Query_XC_Status(u32Window, DMS_XC_NO_FRAME))
        {
            RES_DMSDI._bSetwinToDiTask_DoubleBuffer = TRUE;
        }
        else
        {
            if(RES_DMS.bDMS_MVOPFlowFromDi == TRUE)
            {
                RES_DMSDI._bSetwinToDiTask_DoubleBuffer = TRUE;
                _DMS_Remove_XC_Status(u32Window, DMS_XC_RESET_WINDOW_SIZE);
            }
        }
#endif
    }
#endif  //DMS_MAIN_WINDOW_FLOW_VERSION == 1

    MS_BOOL bResChange = FALSE;
    if(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth != g_IPanel.Width())
    {
        RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth = g_IPanel.Width();
        bResChange = TRUE;
    }
    if(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight != g_IPanel.Height())
    {
        RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight = g_IPanel.Height();
        bResChange = TRUE;
    }

    if(bResChange)
    {
        MApi_GOP_GWIN_Set_STRETCHWIN(GOP_NUM, E_GOP_DST_OP0, 0, 0, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelWidth, RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].u32PanelHeight);
        _MDrv_DMS_OSD_Init(GOP_NUM, RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP], DIP_DATA_FMT_YUV422);
    }

    BytesPerLine = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt, RES_DMS._stXC_DIPMemInfo[eDIPClient].u32PanelWidth);
    UHDBytesPerLine = _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt, DMS_Frame_Buffer_Width);

    // Handle Window Change
    _DMS_CheckIfDispWindowChange(u32Window, &stDispWin);

    if (RES_DMS._stXC_DIPWinInfo[u32Window].bDIPwinUse == FALSE)
    {
        // record enable window number
        RES_DMS.u8EnableWindowCount++;
        RES_DMS.u8UseDIPWindowCount++;
    }

    RES_DMS._stXC_DIPWinInfo[u32Window].bDIPwinUse = TRUE;

    if(_DMS_Is_MVOPFlow(u32Window))
    {
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
        memcpy(&RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stCropWin,&stCropWin,sizeof(stCropWin));
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32x = 0;
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32y = 0;
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32width = DMS_DIP_Alignment(stDispWin.u32width, DMS_HAL_DIP_Align);
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32height = stDispWin.u32height;

        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPStartMemAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + (BytesPerLine * RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32y) + ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt, RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32x));
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPEndMemAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + (BytesPerLine * (RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32y + RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32height)) +
        ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt,(RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32x + RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin.u32width)));
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPDoubleBufStartMemAddr = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPStartMemAddr + (BytesPerLine * RES_DMS._stXC_DIPMemInfo[eDIPClient].u32PanelHeight);
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPDoubleBufEndMemAddr = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPEndMemAddr + (BytesPerLine *  RES_DMS._stXC_DIPMemInfo[eDIPClient].u32PanelHeight);

        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.phyDIPRingBufMemAddr[0] = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPStartMemAddr;
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.phyDIPRingBufMemAddr[1] = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPDoubleBufStartMemAddr;
        MS_U8 i = 2;
        for(i = 2; i < DMS_DI_RING_BUFFER; i++)
        {
            RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.phyDIPRingBufMemAddr[i] = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.phyDIPRingBufMemAddr[i-1] + (BytesPerLine * RES_DMS._stXC_DIPMemInfo[eDIPClient].u32PanelHeight);
        }
        RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32RingBufferSize = RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPEndMemAddr - RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.u32DIPStartMemAddr;
#endif   //#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    }
    else
    {
        memcpy(&RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin,&stCropWin,sizeof(stCropWin));
        RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x = DMS_DIP_Alignment(stDispWin.u32x, DMS_HAL_DIP_Align);
        RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y = stDispWin.u32y;

        RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width = DMS_DIP_Alignment(stDispWin.u32width, DMS_HAL_DIP_Align);
        RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height = stDispWin.u32height;

        if(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y == 0)
        {
            RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt,RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x));
        }
        else
        {
            RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + (BytesPerLine * (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y-1)) + ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt,RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x));
        }
        RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr = RES_DMS.u32DIPAddress_pa[eDIPClient] + (BytesPerLine * (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y + RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height - 1)) +
        ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[eDIPClient].eDIPWFmt,(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x + RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width)));
        RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufStartMemAddr = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr + (UHDBytesPerLine * DMS_Frame_Buffer_Height);
        RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufEndMemAddr = RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr + (UHDBytesPerLine * DMS_Frame_Buffer_Height);
#if (DMS_DIP_Support_HVSP == 0)
        if(eDIPClient == E_DMS_DIP_CLIENT_GOP)
        {
            RES_DMS._stXC_DIPWinInfo[u32Window].u32GEStartMemAddr = RES_DMS.u32GEAddress_pa + (BytesPerLine * RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y) + ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt, RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x));
            RES_DMS._stXC_DIPWinInfo[u32Window].u32GEEndMemAddr = RES_DMS.u32GEAddress_pa + (BytesPerLine * (RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32y + RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32height)) +
                ( _DMS_DIP_CalcPitch(RES_DMS._stXC_DIPMemInfo[E_DMS_DIP_CLIENT_GOP].eDIPWFmt,(RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32x + RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin.u32width)));
        }
#endif
    }

    //Re-calculate Max Output Frame Rate
    _CalculateMaxOutputFrameRate();
#if DBG_DMS_LOG
    ST_DMS_WINDOW *pstCropLogWin = (_DMS_Is_MVOPFlow(u32Window))? &RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stCropWin : &RES_DMS._stXC_DIPWinInfo[u32Window].stCropWin;
    ST_DMS_WINDOW *pstDispLogWin = (_DMS_Is_MVOPFlow(u32Window))? &RES_DMS._stXC_DIPWinInfo_MVOPDoubleBuffer.stDstWin : &RES_DMS._stXC_DIPWinInfo[u32Window].stDstWin;
    DMS_PRINTF("\033[32m [%s][%d] === %s === \033[0m\n",__FUNCTION__,__LINE__,(DMS_MVOP_FLOW)? "MVOP flow!!!" : "DIP to GOP flow!!!");
    DMS_PRINTF("\033[32m [%s][%d] === Window : %d === \033[0m\n",__FUNCTION__,__LINE__,u32Window);
    DMS_PRINTF("\033[32m [%s][%d] === u32DIPAddress_pa : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS.u32DIPAddress_pa[eDIPClient]);
    DMS_PRINTF("\033[32m [%s][%d] === Crop_Window === \033[0m\n",__FUNCTION__,__LINE__);
    DMS_PRINTF("\033[32m [%s][%d] === X : %d, Y : %d, W : %d, H : %d  === \033[0m\n",__FUNCTION__,__LINE__,pstCropLogWin->u32x,pstCropLogWin->u32y,pstCropLogWin->u32width,pstCropLogWin->u32height);
    DMS_PRINTF("\033[32m [%s][%d] === DST_Window === \033[0m\n",__FUNCTION__,__LINE__);
    DMS_PRINTF("\033[32m [%s][%d] === X : %d, Y : %d, W : %d, H : %d === \033[0m\n",__FUNCTION__,__LINE__,pstDispLogWin->u32x,pstDispLogWin->u32y, pstDispLogWin->u32width,pstDispLogWin->u32height);
    DMS_PRINTF("\033[32m [%s][%d] === DIP Addr === \033[0m\n",__FUNCTION__,__LINE__);
    DMS_PRINTF("\033[32m [%s][%d] === u32DIPStartMemAddr : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPStartMemAddr);
    DMS_PRINTF("\033[32m [%s][%d] === u32DIPEndMemAddr : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPEndMemAddr);
    DMS_PRINTF("\033[32m [%s][%d] === u32DIPDoubleBufStartMemAddr : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufStartMemAddr);
    DMS_PRINTF("\033[32m [%s][%d] === u32DIPDoubleBufEndMemAddr : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS._stXC_DIPWinInfo[u32Window].u32DIPDoubleBufEndMemAddr);
    DMS_PRINTF("\033[32m [%s][%d] === u32GEStartMemAddr : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS._stXC_DIPWinInfo[u32Window].u32GEStartMemAddr);
    DMS_PRINTF("\033[32m [%s][%d] === u32GEEndMemAddr : 0x%llx === \033[0m\n",__FUNCTION__,__LINE__,RES_DMS._stXC_DIPWinInfo[u32Window].u32GEEndMemAddr);
#endif
    gDMSDebug = 0;

    return TRUE;
}

static MS_BOOL _MDrv_DMS_SetWindowZOrder(MS_U32 u32WindowID, MS_U32 u32ZOrder)
{
    DMS_GET_RES_PRI;
    MS_U8 u8WindowID = 0;

    if(_DMS_Is_MVOPFlow(u32WindowID) == TRUE)
    {
        RES_DMS.WindowScore[u32WindowID] = u32ZOrder;

        if(RES_DMS.WindowScore[u32WindowID] < RES_DMS.WindowScore[1])
        {
            MApi_XC_SetVideoOnOSD(E_VIDEO_ON_OSD_LAYER_0,MAIN_WINDOW);
        }
        else if(RES_DMS.WindowScore[u32WindowID] > RES_DMS.WindowScore[DMS_MAX_WINDOW_NUM-1])
        {
            MApi_XC_SetVideoOnOSD(E_VIDEO_ON_OSD_LAYER_4,MAIN_WINDOW);
        }
        else
        {
            MApi_XC_SetVideoOnOSD(E_VIDEO_ON_OSD_LAYER_0,MAIN_WINDOW);
            RES_DMS.WindowScore[u32WindowID] = 0;
        }
    }
    else
    {
        _DMS_Add_XC_Status(u8WindowID,  DMS_XC_UPDATE_FROM_VDEC);
        _DMS_Remove_XC_Status(u8WindowID,  DMS_XC_FIRST_FRAME_FROM_VDEC);

        for(u8WindowID=1; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
        {
            if(RES_DMS.WindowZOrder[u8WindowID] == u32WindowID)
            {
                RES_DMS.WindowScore[u8WindowID] = u32ZOrder;
                break;
            }
        }
        _DMS_DIP_ScoreSorting();
    }

    return TRUE;
}

EN_DMS_RESULT MDrv_DMS_Init(void* pInstance, ST_DMS_INITDATA *pstDMS_InitData)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    bRet = _MDrv_DMS_Init(pInstance, GOP_NUM, E_DMS_DIP_FMT_YUV422, 0, 0);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_SetMemoryType(void* pInstance, ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;

    printf("MDrv_DMS_SetMemoryType\n");
    if(pstDMS_SetMemType->eMemoryType == E_DMS_MEMORY_TYPE_GOP_DISPLAY)
    {
        _MDrv_DMS_OSD_Init(GOP_NUM, RES_DMS.u32DIPAddress_pa[E_DMS_DIP_CLIENT_GOP], DIP_DATA_FMT_YUV422);
    }
    bRet = _MDrv_DMS_Setup_Memory(pInstance, pstDMS_SetMemType->eMemoryType, pstDMS_SetMemType->phyAddr,pstDMS_SetMemType->phySize,pstDMS_SetMemType->u32CMAHeapID);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_CreateWindow(void* pInstance, ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID)
{
    DMS_GET_RES_PRI;
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    MS_U16 u16WindowID = 0;
    for(u16WindowID=0; u16WindowID<DMS_MAX_WINDOW_NUM; u16WindowID++)
    {
        if(_DMS_IsWindowUsed(u16WindowID) == FALSE)
        {
            // record enable window number
            RES_DMS.u8EnableWindowCount++;
            RES_DMS.u8UseDIPWindowCount++;

            RES_DMS._stXC_DIPWinInfo[u16WindowID].bDIPwinUse = TRUE;
            *pu32WindowID = u16WindowID;

            bRet = TRUE;
            break;
        }
    }
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 0)
static MS_BOOL _DMS_MVOP_SetConfig_From_Vdec(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
    DMS_GET_RES_PRI;

    MVOP_VidStat stMvopVidSt;
    MVOP_InputCfg stMvopInputCfg;
    MVOP_InputSel eInputCfg = MVOP_INPUT_DRAM;
    MS_U8 u8MIUSel = 0;
    MS_BOOL bHS = TRUE;
    MS_BOOL bXCGen = FALSE;
    MS_BOOL bEnableUvSwap = FALSE;
    MVOP_Handle stMvopHd = { E_MVOP_MODULE_MAIN };

    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);


    // Structure initialization
    memset(&stMvopVidSt, 0, sizeof(MVOP_VidStat));
    memset(&stMvopInputCfg, 0, sizeof(MVOP_InputCfg));

    RES_DMS.bDisableBlackScreen = FALSE;
    RES_DMS.bFirstPlay = TRUE;
    RES_DMS.u32MuteCounter = 0;

    MDrv_MVOP_Init();
    MDrv_MVOP_Enable(FALSE); // Turn off MVOP before setting it
    //u8MIUSel = (pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr >=0x80000000)?1:0;
    if(E_MVOP_OK != MDrv_MVOP_MiuSwitch(u8MIUSel))
    {
        DMS_PRINTF("Cannot set MVOP MIU selection!\n");
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return FALSE;
    }

    stMvopInputCfg.u16HSize = pstDispFrameFormat->stFrames[0].u32Width;
    stMvopInputCfg.u16VSize = pstDispFrameFormat->stFrames[0].u32Height;
    stMvopInputCfg.u32YOffset = 0;
    stMvopInputCfg.u32UVOffset = 0;
    stMvopInputCfg.bProgressive = (_DMS_Is_SourceInterlace(_DMS_GetDMSScanType(pstDispFrameFormat->u8Interlace)) == TRUE)?0:1;
    stMvopInputCfg.bUV7bit = FALSE;
    stMvopInputCfg.bField = (_DMS_GetDMSScanType(pstDispFrameFormat->u8Interlace) == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)? 1:0;
    stMvopInputCfg.u16StripSize = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch;
    stMvopInputCfg.bYUV422 = _DMS_Is422Mode(_DMS_GetColorFormat(pstDispFrameFormat->enColorFormat));
    stMvopInputCfg.bSD = ((pstDispFrameFormat->stFrames[0].u32Width <= 720) && (pstDispFrameFormat->stFrames[0].u32Height <= 576))?1:0;
    if (stMvopInputCfg.bYUV422 == TRUE)
    {
        stMvopInputCfg.bDramRdContd = TRUE;
        stMvopInputCfg.b422pack = TRUE;
    }
    else
    {
        stMvopInputCfg.bDramRdContd = FALSE;
        stMvopInputCfg.b422pack = FALSE;
    }
    if(E_MVOP_OK != MDrv_MVOP_SetInputCfg(eInputCfg, &stMvopInputCfg))
    {
        DMS_PRINTF("Set MVOP input config failed!");
        printf("Set MVOP input config failed!");
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return FALSE;
    }

    if(stMvopInputCfg.bProgressive == TRUE)
    {
        bXCGen = TRUE;
    }
    MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_HANDSHAKE_MODE, &bHS);
    MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_XC_GEN_TIMING, &bXCGen);

    MDrv_MVOP_SetCommand(&stMvopHd, E_MVOP_CMD_SET_UV_SWAP, &bEnableUvSwap);

    stMvopVidSt.u16HorSize = pstDispFrameFormat->stFrames[0].u32Width;
    stMvopVidSt.u16VerSize = pstDispFrameFormat->stFrames[0].u32Height;
    stMvopVidSt.u8AspectRate = pstDispFrameFormat->u8AspectRate;
    stMvopVidSt.u8Interlace = _DMS_GetDMSScanType(pstDispFrameFormat->u8Interlace);

    if(stMvopInputCfg.bProgressive == FALSE)
    {
        stMvopVidSt.u16FrameRate = pstDispFrameFormat->u32FrameRate;
    }
    else
    {
        //printf("[%s][%d]========= pstDispFrameFormat->u32FrameRate=%ld===========\n",__FUNCTION__,__LINE__, pstDispFrameFormat->u32FrameRate);
        //printf("[%s][%d]=========g_IPanel.DefaultVFreq()=%d===========\n",__FUNCTION__,__LINE__,g_IPanel.DefaultVFreq());
        stMvopVidSt.u16FrameRate = g_IPanel.DefaultVFreq() * 100;
    }

    if(E_MVOP_OK != MDrv_MVOP_SetOutputCfg(&stMvopVidSt, FALSE))
    {
        DMS_PRINTF("Set MVOP output config failed!");
        printf("Set MVOP output config failed!");
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return FALSE;
    }
    MDrv_MVOP_Enable(TRUE); // Enable MVOP, video pass to Scaler
    MsOS_DisableInterrupt(E_INT_IRQ_DC);
    MDrv_MVOP_EnableInterrupt(E_MVOP_INT_NONE);
    MsOS_AttachInterrupt(E_INT_IRQ_DC, (InterruptCb)_DMS_MVOP_ISR);
    MsOS_EnableInterrupt(E_INT_IRQ_DC);
    MDrv_MVOP_EnableInterrupt(E_MVOP_INT_VSYNC);
    _CalculateMaxOutputFrameRate();
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

    return TRUE;

}
#endif

EN_DMS_RESULT MDrv_DMS_SetDigitalDecodeSignalInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    DMS_PRINTF("\033[32m [%s][%d] === Not Impl %d=== \033[0m\n",__FUNCTION__,__LINE__,u32WindowID);
#else
    if(u32WindowID == MVOP_WINDOW)
    {
        MApi_XC_SetBOBMode(FALSE, MAIN_WINDOW);
        _DMS_MVOP_SetConfig_From_Vdec(pInstance, u32WindowID, pstDispFrameFormat);
    }
#endif
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_ClearDigitalDecodeSignalInfo(void* pInstance, MS_U32 u32WindowID)
{
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    DMS_PRINTF("\033[32m [%s][%d] === Not Impl %d=== \033[0m\n",__FUNCTION__,__LINE__,u32WindowID);
#else
    MVOP_Handle stHd = {E_MVOP_MODULE_MAIN};
    MVOP_Result ret = E_MVOP_FAIL;
    MVOP_DMSDisplayADD stMVOPDispAddr;

    memset(&stMVOPDispAddr, 0, sizeof(MVOP_DMSDisplayADD));

    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;

    if(u32WindowID == MVOP_WINDOW)
    {
        ret = MDrv_MVOP_DMS_SetDispADDInfo(&stHd, E_MVOP_MAIN_WIN, &stMVOPDispAddr, NULL);
        if(ret != 0)
        {
            DMS_PRINTF("\033[031m[%s:%d]MDrv_MVOP_DMS_SetDispADDInfo fail: %d\033[0m\n",__FUNCTION__, __LINE__, ret);
        }

        MDrv_MVOP_EnableInterrupt(E_MVOP_INT_NONE);
        MsOS_DisableInterrupt(E_INT_IRQ_DC);
        MsOS_DetachInterrupt(E_INT_IRQ_DC);
        MDrv_MVOP_Enable(FALSE);
        MDrv_MVOP_Exit();
    }
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
#endif
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_DestroyWindow(void* pInstance, MS_U32 u32WindowID)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    bRet = _MDrv_DMS_Final(pInstance, u32WindowID);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

//static MS_BOOL bOnce = FALSE;
EN_DMS_RESULT MDrv_DMS_SetWindow(void* pInstance, MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info)
{
#if 0
    if(bOnce)
    {
        return E_DMS_OK;
    }
    else
    {
        bOnce = TRUE;
    }
#endif
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
    if(pstDMS_SetWin_Info->u32XCWinInfoValid)
    {
        DMS_PRINTF("[%s][%d] Do setXC win by DMS!\n\n",__FUNCTION__,__LINE__);
        XC_SETWIN_INFO stXCSetWinInfo;
        memset(&stXCSetWinInfo, 0, sizeof(XC_SETWIN_INFO));

        stXCSetWinInfo.enInputSourceType = pstDMS_SetWin_Info->stXCWinInfo.u32InputSourceType;
        _DMS_XC_DMSWintoXCWin(&stXCSetWinInfo.stCapWin, &pstDMS_SetWin_Info->stXCWinInfo.stCapWin);
        _DMS_XC_DMSWintoXCWin(&stXCSetWinInfo.stDispWin, &pstDMS_SetWin_Info->stXCWinInfo.stDispWin);
        _DMS_XC_DMSWintoXCWin(&stXCSetWinInfo.stCropWin, &pstDMS_SetWin_Info->stXCWinInfo.stCropWin);

        stXCSetWinInfo.bInterlace = pstDMS_SetWin_Info->stXCWinInfo.u32Interlace;
        stXCSetWinInfo.bHDuplicate = pstDMS_SetWin_Info->stXCWinInfo.u32HDuplicate;
        stXCSetWinInfo.u16InputVFreq = pstDMS_SetWin_Info->stXCWinInfo.u32InputVFreq;
        stXCSetWinInfo.u16InputVTotal = pstDMS_SetWin_Info->stXCWinInfo.u32InputVTotal;
        stXCSetWinInfo.u16DefaultHtotal = pstDMS_SetWin_Info->stXCWinInfo.u32DefaultHtotal;
        stXCSetWinInfo.u16DefaultPhase = pstDMS_SetWin_Info->stXCWinInfo.u32DefaultPhase;

        stXCSetWinInfo.bHCusScaling = pstDMS_SetWin_Info->stXCWinInfo.u32HCusScaling;
        stXCSetWinInfo.bVCusScaling = pstDMS_SetWin_Info->stXCWinInfo.u32VCusScaling;
        _DMS_XC_SetCusScaling(&stXCSetWinInfo,TRUE,pstDMS_SetWin_Info->stXCWinInfo.u32HCusScalingSrc,pstDMS_SetWin_Info->stXCWinInfo.u32HCusScalingDst,
            pstDMS_SetWin_Info->stXCWinInfo.u32VCusScalingSrc,pstDMS_SetWin_Info->stXCWinInfo.u32VCusScalingDst);

        stXCSetWinInfo.bDisplayNineLattice = pstDMS_SetWin_Info->stXCWinInfo.u32DisplayNineLattice;        ///<used to indicate where to display in panel and where to put in frame buffer

        stXCSetWinInfo.bPreHCusScaling = pstDMS_SetWin_Info->stXCWinInfo.u32PreHCusScaling;
        stXCSetWinInfo.bPreVCusScaling = pstDMS_SetWin_Info->stXCWinInfo.u32PreVCusScaling;
        _DMS_XC_SetCusScaling(&stXCSetWinInfo,FALSE,pstDMS_SetWin_Info->stXCWinInfo.u32PreHCusScalingSrc,pstDMS_SetWin_Info->stXCWinInfo.u32PreHCusScalingDst,
            pstDMS_SetWin_Info->stXCWinInfo.u32PreVCusScalingSrc,pstDMS_SetWin_Info->stXCWinInfo.u32PreVCusScalingDst);

        MApi_XC_SetWindow(&stXCSetWinInfo, sizeof(XC_SETWIN_INFO), MAIN_WINDOW);
    }
#else
    if(u32WindowID == MVOP_WINDOW)
    {
        _DMS_XC_SetConnect(INPUT_SOURCE_DTV);
        DMS_PRINTF("[%s][%d] Do setXC win by DMS!\n\n",__FUNCTION__,__LINE__);
        DMS_GET_RES_PRI;
        XC_SETWIN_INFO stXCSetWinInfo;
        XC_DynamicScaling_Info stXCDsInfo;
        MS_U32 u32DSInfoLen;
        XC_OUTPUTFRAME_Info stOutFrameInfo;

        memset(&stXCSetWinInfo, 0, sizeof(XC_SETWIN_INFO));
        memset(&stXCDsInfo, 0, sizeof(XC_DynamicScaling_Info));
        memset(&stOutFrameInfo, 0, sizeof(XC_OUTPUTFRAME_Info));

        stXCSetWinInfo.enInputSourceType = INPUT_SOURCE_DTV;
        MVOP_Timing mvop_timing;
        MVOP_Handle stHd = { E_MVOP_MODULE_MAIN };
        MS_BOOL bIsXCGenTiming;
        memset(&mvop_timing, 0, sizeof(MVOP_Timing));
        if(E_MVOP_OK != MDrv_MVOP_GetOutputTiming(&mvop_timing))
           DMS_PRINTF("MDrv_MVOP_GetOutputTiming failed!!\n");

        if(E_MVOP_OK != MDrv_MVOP_GetCommand(&stHd , E_MVOP_CMD_GET_IS_XC_GEN_TIMING, &bIsXCGenTiming , sizeof(bIsXCGenTiming)))
        {
            DMS_PRINTF("MDrv_MVOP_Get_XC_GEN_TIMING failed!!\n");
        }

        stXCSetWinInfo.bInterlace     = mvop_timing.bInterlace;
        stXCSetWinInfo.bHDuplicate    = mvop_timing.bHDuplicate;
        stXCSetWinInfo.u16InputVFreq = (bIsXCGenTiming)?g_IPanel.DefaultVFreq():mvop_timing.u8Framerate;
        stXCSetWinInfo.u16InputVTotal = mvop_timing.u16V_TotalCount;
        stXCSetWinInfo.u16DefaultHtotal = mvop_timing.u16H_TotalCount;


        stOutFrameInfo.u16InVFreq = stXCSetWinInfo.u16InputVFreq;
        stOutFrameInfo.u16OutVFreq = g_IPanel.DefaultVFreq();
        stOutFrameInfo.bInterlace = stXCSetWinInfo.bInterlace;

        //-------------------------
        _DMS_XC_Set_ScalingDown_Condition(&pstDMS_SetWin_Info->stOutputWinInfo.stCropWin, &pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin);
        if(_DMS_Is_ScalingConditionUse2ndBuffer(RES_DMS.gDMS_ScalingCondition))
        {
            pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32width = pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32width / 2;
            pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32height = pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32height / 2;
        }

        _DMS_XC_DMSWintoXCWin(&stXCSetWinInfo.stCapWin, &pstDMS_SetWin_Info->stOutputWinInfo.stCropWin);
        _DMS_XC_DMSWintoXCWin(&stXCSetWinInfo.stDispWin, &pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin);
        _DMS_XC_DMSWintoXCWin(&stXCSetWinInfo.stCropWin, &pstDMS_SetWin_Info->stOutputWinInfo.stCropWin);

        //-------------------------
        // customized pre scaling
        //-------------------------
        stXCSetWinInfo.bPreHCusScaling = FALSE;
        stXCSetWinInfo.u16PreHCusScalingSrc = pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32width;
        stXCSetWinInfo.u16PreHCusScalingDst = pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32width;
        stXCSetWinInfo.bPreVCusScaling = FALSE;
        stXCSetWinInfo.u16PreVCusScalingSrc = pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32height;
        stXCSetWinInfo.u16PreVCusScalingDst = pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32height;

        //customized Post scaling
        stXCSetWinInfo.bHCusScaling = FALSE;
        stXCSetWinInfo.bVCusScaling = FALSE;

        MDrv_MVOP_SetStartPos(stXCSetWinInfo.stCropWin.x, stXCSetWinInfo.stCropWin.y);
        MDrv_MVOP_SetImageWidthHight(stXCSetWinInfo.stCropWin.width, stXCSetWinInfo.stCropWin.height);
        MDrv_MVOP_SetRegSizeFromMVD(FALSE);

        // ds patch
        stXCDsInfo.bOP_DS_On = FALSE;
        stXCDsInfo.bIPS_DS_On = FALSE;
        stXCDsInfo.bIPM_DS_On = FALSE;
        u32DSInfoLen = sizeof(XC_DynamicScaling_Info);
        MApi_XC_SetDynamicScaling(&stXCDsInfo, u32DSInfoLen, 0);

        MApi_XC_SkipWaitVsync(MAIN_WINDOW,TRUE);
        MApi_XC_SetWindow(&stXCSetWinInfo, sizeof(XC_SETWIN_INFO), MAIN_WINDOW);
        MApi_XC_SkipWaitVsync(MAIN_WINDOW,FALSE);

        MApi_XC_EnableFrameBufferLess(_DMS_Is_ScalingConditionUseFBL(RES_DMS.gDMS_ScalingCondition));
        MApi_XC_OutputFrameCtrl(TRUE, &stOutFrameInfo, MAIN_WINDOW);
    }
#endif
    bRet = _MDrv_DMS_SetWin(pInstance, &pstDMS_SetWin_Info->stOutputWinInfo.stCropWin, &pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin, u32WindowID);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_GetWindowInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info)
{
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Video_Flip(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat)
{
    DMS_VDECFRAME_INFO stFrameInfo;
    memset(&stFrameInfo, 0 , sizeof(stFrameInfo));
    stFrameInfo.u32Window = u32WindowID;

    stFrameInfo.u16SrcWidth = pstDispFrameFormat->stFrames[0].u32Width;
    stFrameInfo.u16SrcHeight = pstDispFrameFormat->stFrames[0].u32Height;
    stFrameInfo.u16SrcPitch = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch;
    stFrameInfo.u32SrcLumaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr;
    stFrameInfo.u32SrcChromaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr;
    stFrameInfo.eFieldType = pstDispFrameFormat->stFrames[0].enFieldType;

    stFrameInfo.u16CropRight = pstDispFrameFormat->stFrames[0].u32CropRight;
    stFrameInfo.u16CropLeft = pstDispFrameFormat->stFrames[0].u32CropLeft;
    stFrameInfo.u16CropBottom = pstDispFrameFormat->stFrames[0].u32CropBottom;
    stFrameInfo.u16CropTop = pstDispFrameFormat->stFrames[0].u32CropTop;

    stFrameInfo.u32FrameRate = pstDispFrameFormat->u32FrameRate;
    stFrameInfo.eCODEC = pstDispFrameFormat->u32CodecType;
    stFrameInfo.eTileMode = pstDispFrameFormat->u32TileMode;
    stFrameInfo.eFmt = _DMS_GetColorFormat(pstDispFrameFormat->enColorFormat);

    stFrameInfo.eScanType = _DMS_GetDMSScanType(pstDispFrameFormat->u8Interlace);
    stFrameInfo.eFieldOrderType  = !(pstDispFrameFormat->u8BottomFieldFirst);

    if(stFrameInfo.eScanType == E_DMS_VIDEO_SCAN_TYPE_INTERLACE_FIELD)
    {
        stFrameInfo.u32SrcLumaAddrI = pstDispFrameFormat->stFrames[1].stHWFormat.phyLumaAddr;
        stFrameInfo.u32SrcChromaAddrI = pstDispFrameFormat->stFrames[1].stHWFormat.phyChromaAddr;
        stFrameInfo.u32FrameIndex_2nd = pstDispFrameFormat->stFrames[1].u32Idx;
        stFrameInfo.u32PriData_2nd = pstDispFrameFormat->stFrames[1].u32PriData;
    }

    stFrameInfo.b10bitData = (pstDispFrameFormat->enColorFormat == E_DMS_COLOR_FORMAT_10BIT_TILE)? TRUE : FALSE;
    if(stFrameInfo.b10bitData)
    {
        stFrameInfo.u16Src10bitPitch = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch2Bit;
        stFrameInfo.u32SrcLumaAddr_2bit = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr2Bit;
        stFrameInfo.u32SrcChromaAddr_2bit = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr2Bit;
        stFrameInfo.u8LumaBitdepth = pstDispFrameFormat->stFrames[0].u8LumaBitdepth;
    }

    /// Frame index
    stFrameInfo.u32FrameIndex = pstDispFrameFormat->stFrames[0].u32Idx;
    stFrameInfo.u32VDECStreamID = pstDispFrameFormat->u32VdecStreamId;
    stFrameInfo.u32VDECStreamVersion = pstDispFrameFormat->u32VdecStreamVersion;
    stFrameInfo.u32PriData = pstDispFrameFormat->stFrames[0].u32PriData;
    stFrameInfo.u64Pts = pstDispFrameFormat->u64Pts;
    if(gDMSMVOPForceLookVDEC)
    {
        //DMS_PRINTF("Flip%d: Y 0x%llx, C 0x%llx, pri 0x%x\n",u32WindowID,stFrameInfo.u32SrcLumaAddr,stFrameInfo.u32SrcChromaAddr,stFrameInfo.u32PriData);
    }

    //mfcodec info
    stFrameInfo.stMFdecInfo.bMFDec_Enable = pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo & 0xFF ;
    if(stFrameInfo.stMFdecInfo.bMFDec_Enable != 0 && stFrameInfo.stMFdecInfo.bMFDec_Enable != 0xFF)
    {
        stFrameInfo.stMFdecInfo.bMFDec_Enable = 1;
        stFrameInfo.stMFdecInfo.bBypass_codec_mode = 0;
        stFrameInfo.stMFdecInfo.u8MFDec_Select = (pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo >> 8) & 0x3;     //Bit[8]      MFDec ID. 1: MFDec 1; 0: MFDec 0; 2 MFDec 2;
        stFrameInfo.stMFdecInfo.bUncompress_mode = (pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo >> 28) & 0x1;  //Bit[28]     1: MFCodec uncompress mode; 0: data is compressed
        stFrameInfo.stMFdecInfo.en_MFDecVP9_mode = (pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo >> 29) & 0x1;  //Bit[29]     1: MFCodec 3.0 vp9 mode; 0: MFCodec 3.0 h26x mode
        stFrameInfo.stMFdecInfo.u16Bitlen_Pitch = (pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo >> 16) & 0xFF; //Bits[23:16] MFCodec pitch setting

        stFrameInfo.stMFdecInfo.u16StartX = stFrameInfo.u16CropLeft;
        stFrameInfo.stMFdecInfo.u16StartY = stFrameInfo.u16CropTop;

        MS_U8 u8BitLenmiu = 0;
        u8BitLenmiu = (pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo >> 24) & 0xF ;
        stFrameInfo.stMFdecInfo.u8Bitlen_MiuSelect = u8BitLenmiu;
        stFrameInfo.stMFdecInfo.phyBitlen_Base = (u8BitLenmiu == 0) ? (MS_PHY)pstDispFrameFormat->stFrames[0].stHWFormat.phyMFCBITLEN : ((MS_PHY)pstDispFrameFormat->stFrames[0].stHWFormat.phyMFCBITLEN | HAL_MIU1_BASE);
    }
    else
    {
        stFrameInfo.stMFdecInfo.bMFDec_Enable = 0;
    }

    //set 2nd buffer (real case use a flag to control MVOP watch which buffer)
    if(pstDispFrameFormat->stFrames[0].stHWFormat.u8V7DataValid & BIT(0))
    {
        stFrameInfo.phySrc2ndBufferLumaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr_subsample;
        stFrameInfo.phySrc2ndBufferChromaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr_subsample;
        stFrameInfo.u16Src2ndBufferPitch = pstDispFrameFormat->stFrames[0].stHWFormat.u16Pitch_subsample;
        stFrameInfo.u8Src2ndBufferTileMode = pstDispFrameFormat->stFrames[0].stHWFormat.u8TileMode_subsample;
        stFrameInfo.u8Src2ndBufferV7DataValid = pstDispFrameFormat->stFrames[0].stHWFormat.u8V7DataValid;
        stFrameInfo.u16Src2ndBufferWidth = pstDispFrameFormat->stFrames[0].stHWFormat.u16Width_subsample;
        stFrameInfo.u16Src2ndBufferHeight = pstDispFrameFormat->stFrames[0].stHWFormat.u16Height_subsample;
    }
    else
    {
        //Patch for no 2nd buffer come
        if(stFrameInfo.u16SrcWidth >= CONDITION_4K_WIDTH && stFrameInfo.u16SrcHeight >= CONDITION_4K_HEIGHT)
        {
            static MS_BOOL v7verify = TRUE;
            if(v7verify)
            {
                VDEC_EX_FrameInfoExt_v7 stFrmInfo_v7;
                memset(&stFrmInfo_v7,0,sizeof(VDEC_EX_FrameInfoExt_v7));
                VDEC_EX_VerCtl *stVerCrl = (VDEC_EX_VerCtl *) &stFrmInfo_v7;
                stVerCrl->u32version = 7;
                stVerCrl->u32size = sizeof(VDEC_EX_FrameInfoExt_v7);
                VDEC_StreamId stVDEC_StreamId;
                stVDEC_StreamId.u32Id = stFrameInfo.u32VDECStreamID;
                stVDEC_StreamId.u32Version = stFrameInfo.u32VDECStreamVersion;
                if (E_VDEC_EX_OK == MApi_VDEC_EX_GetControl(&stVDEC_StreamId, E_VDEC_EX_USER_CMD_GET_NEXT_DISP_FRAME_INFO_EXT, (MS_U32 *)&stFrmInfo_v7))
                {
                    stFrameInfo.phySrc2ndBufferLumaAddr = stFrmInfo_v7.phyLumaAddr_subsample;
                    stFrameInfo.phySrc2ndBufferChromaAddr = stFrmInfo_v7.phyChromaAddr_subsample;
                    stFrameInfo.u16Src2ndBufferPitch = stFrmInfo_v7.u16Pitch_subsample;
                    if(stFrameInfo.u16Src2ndBufferPitch != 0)
                    {
                        static MS_BOOL bLogFlag = FALSE;
                        if(!bLogFlag)
                        {
                            DMS_PRINTF("[%s] Get 2nd Buffer in DMS! This is patch, need flip info for DMS!\n",__FUNCTION__);
                            bLogFlag = TRUE;
                        }
                    }
                    else
                    {
                        v7verify = FALSE;
                        DMS_PRINTF("[%s] Get 2nd Buffer in DMS! But pitch is 0, do not get any more!\n",__FUNCTION__);
                    }
                }
                else
                {
                    DMS_PRINTF("[%s] VDEC info ver7 not impl, get sub_buffer failed\n",__FUNCTION__);
                    v7verify = FALSE;
                }
            }
        }
    }

    //Field ctrl
    stFrameInfo.u8FieldCtrl = pstDispFrameFormat->u8FieldCtrl;

    //DMS_PRINTF("%d %d %d\n",stFrameInfo.eScanType,pstDispFrameFormat->u8Interlace,stFrameInfo.eTileMode);
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;

#if (DMS_MAIN_WINDOW_FLOW_VERSION == 0)
#if 0
    if(u32WindowID == MVOP_WINDOW)
    {
        MS_BOOL ret;
        MS_U16 NextWritePointer = 0;

        NextWritePointer = RES_DMS._stXC_DIPWriteReadPointer[stFrameInfo.u32Window].u16WritePointer;
        RES_DMS._stXC_DIPWriteReadPointer[stFrameInfo.u32Window].u16WritePointer = NextWritePointer;
        memcpy(&RES_DMS.stFrameInfo_mainWin[NextWritePointer], &stFrameInfo, sizeof(DMS_VDECFRAME_INFO));

        ret = _DMS_VDEC_Frame_AddRef_MVOP(pDMSResPri->stDrvDMSMvop.s32Frame_index);
        //if(pDMSResPri->stDrvDMSMvop.s32Release_Frame_index >= 0)
        //{
        //    ret = _DMS_VDEC_Frame_Release_MVOP(pDMSResPri->stDrvDMSMvop.s32Release_Frame_index);
        //}

        //MsOS_EnableInterrupt(E_INT_IRQ_DC);
        //MDrv_MVOP_EnableInterrupt(E_MVOP_INT_VSYNC);

    }
#endif
#else
#if (DEV_DI == 1)
    if(_DMS_Is_MVOPFlow(u32WindowID) && _DMS_Is_SourceInterlace(stFrameInfo.eScanType))
    {
        _DMS_StoreVDECInfo2DIPInterlaceQueue(&stFrameInfo);
        RES_DMSDI._u8DMS_DiSetEventBufferID = RES_DMSDI._stXC_DIPWriteReadPointerInterlace[u32WindowID].u16WritePointer;
#if DEV_DI_EVENT
        //MsOS_SetEvent(XC_Di_Push_Event_id, 0x01);
#else
        RES_DMSDI.bFlipToDiTask = TRUE;
#endif
        if(_DMS_Is_SourceFrameBaseInterlace(stFrameInfo.eScanType,stFrameInfo.eFieldType)
            || _DMS_Is_SourceFieldBaseInterlace(stFrameInfo.eScanType))
        {
            stFrameInfo.bIs2ndField = TRUE;
            _DMS_StoreVDECInfo2DIPInterlaceQueue(&stFrameInfo);
        }
    }
    else
#endif
#endif
    {
#if 0
        if(u32WindowID != MVOP_WINDOW)
#endif
        {
            _DMS_StoreVDECInfo2DIPQueue(pInstance, &stFrameInfo);
        }
#if (DMS_MAIN_WINDOW_FLOW_VERSION == 1)
        if(_DMS_Is_MVOPFlow(u32WindowID))
        {
            RES_DMSDI._u8DMS_DiSetEventBufferID = DMS_Invalid_ReadPointer_ID;
        }
#endif
#if 0
        if(u32WindowID != MVOP_WINDOW)
#endif
        {
            if(_DMS_Is_SourceFrameBaseInterlace(stFrameInfo.eScanType,stFrameInfo.eFieldType)
                || _DMS_Is_SourceFieldBaseInterlace(stFrameInfo.eScanType))
            {
                stFrameInfo.bIs2ndField = TRUE;
                _DMS_StoreVDECInfo2DIPQueue(pInstance, &stFrameInfo);
            }
        }
    }
#if DBG_FPS
    u32FPS_Push_Counter[u32WindowID]++;
#endif
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

    return TRUE;
}

EN_DMS_RESULT MDrv_DMS_SetZOrder(void* pInstance, MS_U32 u32WindowID, MS_U32 u32ZOrder)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    MS_BOOL bRet = FALSE;

    bRet = _MDrv_DMS_SetWindowZOrder(u32WindowID, u32ZOrder);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_Video_Freeze(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    DMS_PRINTF("\033[32m [%s][%d] === u32Enable : %td === \033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32Enable);
    if(u32Enable)
    {
        RES_DMS.bDMS_Freeze_DoubleBuffer = TRUE;
    }
    else
    {
        RES_DMS.bDMS_Freeze_DoubleBuffer = FALSE;
    }

    if(_DMS_Is_MVOPFlow(u32WindowID) == TRUE)
    {
        _DMS_Add_XC_Status(u32WindowID, DMS_XC_FREEZE_SETTING);
    }
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return TRUE;
}

static MS_BOOL _MDrv_DMS_SetMute(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable)
{
    DMS_GET_RES_PRI;
    if(_DMS_Is_MVOPFlow(u32WindowID) == TRUE)
    {
#if(DMS_MAIN_WINDOW_FLOW_VERSION == 1)
        Hal_DMS_XC_SetMute(pInstance, u32Enable);
#else
      if(u32Enable)
      {
          Hal_DMS_XC_SetMute(pInstance, u32Enable);
      }
      else
      {
          if(RES_DMS.bFirstPlay)
          {
              RES_DMS.bDisableBlackScreen = TRUE;
          }
          else
          {
              Hal_DMS_XC_SetMute(pInstance, u32Enable);
          }
       }
#endif
    }
    else
    {
        RES_DMS.bIsWindowMute[u32WindowID] = u32Enable;
        RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_0][u32WindowID].bCleanBuf = u32Enable;
        RES_DMS._stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_1][u32WindowID].bCleanBuf = u32Enable;
    }
    return TRUE;
}

EN_DMS_RESULT MDrv_DMS_Video_Mute(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    MS_BOOL bRet = FALSE;

    bRet = _MDrv_DMS_SetMute(pInstance, u32WindowID, u32Enable);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_Set_MuteColor(void* pInstance, MS_U32 u32WindowID, ST_DMS_COLOR stMuteColor)
{
    DMS_PRINTF("\033[32m [%s][%d] === Not Impl %td=== \033[0m\n",__FUNCTION__,__LINE__,(ptrdiff_t)u32WindowID);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_GetStatus(void* pInstance, MS_U32 *pu32Status)
{
    DMS_PRINTF("\033[32m [%s][%d] === Not Impl === \033[0m\n",__FUNCTION__,__LINE__);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_GetCapability(void* pInstance, EN_DMS_CAPABILITY *peCapability)
{
    HAL_DMS_XC_GetCaps(pInstance, peCapability);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_SetOutputLayer(void* pInstance, ST_DMS_WINDOW *pstLayer)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    memcpy(&RES_DMS.stOutputLayer,pstLayer,sizeof(ST_DMS_WINDOW));
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Set_3D_Mode(void* pInstance, MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    if(MApi_XC_Set_3D_Mode(pst3DInfo->en3DInputMode, pst3DInfo->en3DOutputMode, pst3DInfo->en3DPanelType, MAIN_WINDOW))
    {
        if(FALSE == MApi_XC_Set_3D_Mode(pst3DInfo->en3DInputMode, pst3DInfo->en3DOutputMode, pst3DInfo->en3DPanelType, SUB_WINDOW))
        {
            DMS_PRINTF("MApi_XC_Set_3D_Mode for Sub window failed\n");
            bRet = FALSE;
        }
    }
    else
    {
        DMS_PRINTF("MApi_XC_Set_3D_Mode for Main window failed\n");
        bRet = FALSE;
    }

    if(pst3DInfo->en3DInputMode == E_DMS_3D_INPUT_MODE_NONE || pst3DInfo->en3DOutputMode == E_DMS_3D_OUTPUT_MODE_NONE)
    {
        DMS_PRINTF("Disable 3D\n");
        MApi_XC_EnableFrameBufferLess(TRUE);
        MApi_XC_Set_DynamicScalingFlag(TRUE);
    }
    else
    {
        DMS_PRINTF("Enable 3D\n");
        MApi_XC_EnableFrameBufferLess(FALSE);
        MApi_XC_Set_DynamicScalingFlag(FALSE);
    }

    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_DIP_FMT _MDrv_DMS_CaptureMode_ColorFmtMapping(MS_U32 u32WindowID, EN_DMS_CAPTURE_COLORFORMAT enColorFormat)
{
    DMS_GET_RES_PRI;
    EN_DMS_DIP_FMT eFmt;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enTileBlock = DMS_DIP_TILE_BLOCK_W_16_32;
    switch(enColorFormat)
    {
        case E_DMS_CAPTURE_COLOR_FORMAT_16X32TILE:
            eFmt = E_DMS_DIP_FMT_YUV420;
            break;
        case E_DMS_CAPTURE_COLOR_FORMAT_32X16TILE:
            eFmt = E_DMS_DIP_FMT_YUV420;
            RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enTileBlock = DMS_DIP_TILE_BLOCK_W_32_16;
            break;
        case E_DMS_CAPTURE_COLOR_FORMAT_32X32TILE:
            eFmt = E_DMS_DIP_FMT_YUV420;
            RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enTileBlock = DMS_DIP_TILE_BLOCK_W_32_32;
            break;
        case E_DMS_CAPTURE_COLOR_FORMAT_YUYV:
        case E_DMS_CAPTURE_COLOR_FORMAT_YVYU:
        case E_DMS_CAPTURE_COLOR_FORMAT_UYVY:
        case E_DMS_CAPTURE_COLOR_FORMAT_VYUY:
            eFmt = E_DMS_DIP_FMT_YUV422;
            break;
        case E_DMS_CAPTURE_COLOR_FORMAT_NV12:
        case E_DMS_CAPTURE_COLOR_FORMAT_NV21:
            eFmt = E_DMS_DIP_FMT_YUV420_SEMI_PLANER;
            break;
        case E_DMS_CAPTURE_COLOR_FORMAT_MAX:
        default:
            eFmt = E_DMS_DIP_FMT_MAX;
            printf("\033[31m [%s][%d] === Wrong color fmt! === \033[0m\n",__FUNCTION__,__LINE__);
            break;
    }
    return eFmt;
}

EN_DMS_RESULT MDrv_DMS_Set_CaptureInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo)
{
#if DEV_CAPMODE
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;

    if(pstSetCaptureInfo->u32Enable == FALSE)
    {
        RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Enable = pstSetCaptureInfo->u32Enable;
        RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].bStartGetCapture = FALSE;
        MS_U8 u8BufID;
        for(u8BufID = 0; u8BufID < DMS_DIP_QUEUEDEPTH; u8BufID++)
        {
            _DMS_Clear_VDEC_Frame_Ref_CaptureMode(u32WindowID, u8BufID);
            RES_DMSCAPTURE.bIsFrameValid[u32WindowID][u8BufID] = FALSE;
        }
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return E_DMS_OK;
    }

    if(RES_DMS._stXC_DIPWinInfo[u32WindowID].bDIPwinUse == FALSE)
    {
        RES_DMS.u8UseDIPWindowCount++;
        _CalculateMaxOutputFrameRate();

        if(pstSetCaptureInfo->u32Visible == TRUE)
        {
            pstSetCaptureInfo->u32Visible = FALSE;
            DMS_PRINTF("\033[1;31m[%s][%d] error, Visible can't be TRUE before setting window info\033[0m\n",__FUNCTION__,__LINE__);
        }
    }
    RES_DMS.u8DIP_Window[E_DMS_DIP_CAPTURE] = DMS_DIPWindow_CAPTURE;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Enable = pstSetCaptureInfo->u32Enable;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Visible = pstSetCaptureInfo->u32Visible;
    if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Visible == FALSE)
    {
        _MDrv_DMS_SetMute(pInstance, u32WindowID, TRUE);
    }
    else
    {
        _MDrv_DMS_SetMute(pInstance, u32WindowID, FALSE);
    }
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32FrameRate = pstSetCaptureInfo->u32FrameRate;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Width = pstSetCaptureInfo->u32Width;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height = pstSetCaptureInfo->u32Height;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enColorFormat = pstSetCaptureInfo->enColorFormat;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enDIPDataFmt = _MDrv_DMS_CaptureMode_ColorFmtMapping(u32WindowID, pstSetCaptureInfo->enColorFormat);
    //RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enColorFormat = E_DMS_DIP_FMT_YUV422;
#if (DMS_DIP_Support_IMI == 0)
    MS_U32 BytesPerLine = 0;
    BytesPerLine = _DMS_DIP_CalcPitch(_DMS_DIP_Capture_ColorFmtMapping(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].enDIPDataFmt), RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Width);

    MS_PHY phyMaxNeedSize = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height * BytesPerLine * DMS_CAPTUREWIN_RING_BUFFER;
#if (DMS_DIP_Support_HVSP == 0)
    //phyMaxNeedSize += (RES_DMS._stXC_DIPCaptureWinInfo[u32WindowID].u32Height * BytesPerLine);
#endif
    if(phyMaxNeedSize > pstSetCaptureInfo->phySize)
    {
        DMS_PRINTF("\033[1;31m[%s][%d] error, buffer not enough u32MemBufferSize 0x%llx , need 0x%llx\033[0m\n",__FUNCTION__,__LINE__,pstSetCaptureInfo->phySize, phyMaxNeedSize);
        return E_DMS_FAIL;
    }

    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32StartMemAddr = pstSetCaptureInfo->phyAddr;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32EndMemAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32StartMemAddr + (BytesPerLine * RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height);
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32SecondBufStartMemAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32StartMemAddr + (BytesPerLine * RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height);
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32SecondBufEndMemAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32EndMemAddr + (BytesPerLine * RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height);
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32ThirdBufStartMemAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32SecondBufStartMemAddr + (BytesPerLine * RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height);
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32ThirdBufEndMemAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32SecondBufEndMemAddr + (BytesPerLine * RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32Height);

    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[0].phyBufferYAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32StartMemAddr;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[1].phyBufferYAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32SecondBufStartMemAddr;
    RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[2].phyBufferYAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32ThirdBufStartMemAddr;
#endif
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
#endif
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Get_CaptureBuffer(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
#if DEV_CAPMODE

#if (DMS_DIP_Support_IMI == 0)
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    MS_U16 u16CurRingBufID = RES_DMSCAPTURE._stXC_DIPCaptureWin_RingBufferPointer[u32WindowID].u16PreWritePointer;
    if(RES_DMSCAPTURE.bIsRingBufAccessed[u16CurRingBufID] == TRUE)
    {
        RES_DMSCAPTURE._stXC_CaptureRingBufRefCount[u32WindowID][u16CurRingBufID]++;

        pstCaptureInfo->phyBufferYAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[u16CurRingBufID].phyBufferYAddr;
        pstCaptureInfo->phyBufferCAddr = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[u16CurRingBufID].phyBufferCAddr;
        pstCaptureInfo->u64Pts = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[u16CurRingBufID].u64Pts;
        pstCaptureInfo->u32frameCount = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32FrameCount;
        RES_DMSCAPTURE.bIsRingBufAccessed[u16CurRingBufID] = FALSE;
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return E_DMS_OK;
    }
    else
    {
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return E_DMS_FAIL;
    }
#else
    MS_U32 u32WaitEventFlag = BIT(0)|BIT(1)|BIT(2)|BIT(3);
    MS_U32 u32Events = 0;

    DMS_GET_RES_PRI;
    if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].bStartGetCapture == FALSE)
    {
        RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].bStartGetCapture = TRUE;
        RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bCaptureRet = FALSE;
        MsOS_WaitEvent(StartFRC_Event_id, u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        if(u32Events == BIT(u32WindowID))
        {
            printf("\033[32m [%s][%d] Receive Event %td, %d, time %td !!!!!!\033[0m\n",__FUNCTION__,__LINE__, (ptrdiff_t)u32Events , BIT(u32WindowID), (ptrdiff_t)MsOS_GetSystemTime());
            RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bFirstCapFrame = TRUE;
        }
        else
        {
            printf("\033[31m [%s][%d] Receive Wrong Event %td, %d !\033[0m\n",__FUNCTION__,__LINE__, (ptrdiff_t)u32Events , BIT(u32WindowID));
        }
    }
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);

    if(RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bIsEventSent == FALSE)
    {
        RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bCaptureRet = FALSE;
        if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u16PreviousWP != RES_DMSCAPTURE._stXC_DIPCapWin_WriteReadPointer[u32WindowID].u16ReadPointer)
        {
            MsOS_SetEvent(CaptureMode_Event_id, BIT(u32WindowID));
            RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bIsEventSent = TRUE;
        }
        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        return E_DMS_FAIL;
    }
    else
    {
        if(RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bCaptureRet == TRUE)
        {
            RES_DMSCAPTURE._stXC_IMICaptureInfo[u32WindowID].bIsEventSent = FALSE;
            pstCaptureInfo->u64Pts = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[0].u64Pts;
            pstCaptureInfo->u32frameCount = RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].u32FrameCount;
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
            return E_DMS_OK;
        }
        else
        {
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
            return E_DMS_FAIL;
        }
    }
#endif

#endif
    return E_DMS_OK;
}

#if (DMS_DIP_Support_IMI == 0)
static MS_BOOL _MDrv_DMS_IsContentMatch(MS_U32 u32WindowID, MS_U8 u8BufferID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    DMS_GET_RES_PRI;
    if(RES_DMSCAPTURE._stXC_DIPCaptureWinInfo[u32WindowID].stCaptureRingBuf[u8BufferID].phyBufferYAddr == pstCaptureInfo->phyBufferYAddr)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

EN_DMS_RESULT MDrv_DMS_Release_CaptureBuffer(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
#if DEV_CAPMODE
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
#if (DMS_DIP_Support_IMI == 0)
    MS_U8 u8BufferID = 0;
    for(u8BufferID=0; u8BufferID<DMS_CAPTUREWIN_RING_BUFFER; u8BufferID++)
    {
        if(_MDrv_DMS_IsContentMatch(u32WindowID, u8BufferID, pstCaptureInfo) == TRUE)
        {
            RES_DMSCAPTURE._stXC_CaptureRingBufRefCount[u32WindowID][u8BufferID]--;
        }
    }
#else
    MS_U8 u8DIPWindowID = RES_DMS.u8DIP_Window[E_DMS_DIP_CAPTURE];
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    while(MApi_XC_DIP_GetResource(u8DIPWindowID) == E_APIXC_RET_FAIL)
    {
        MsOS_DelayTask(1);
    }
    MApi_XC_DIP_Ena(FALSE,u8DIPWindowID);
    MApi_XC_DIP_ReleaseResource(u8DIPWindowID);
    return E_DMS_OK;
#endif
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
#endif
    return E_DMS_OK;
}


