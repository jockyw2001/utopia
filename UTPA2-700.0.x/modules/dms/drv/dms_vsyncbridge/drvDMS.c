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

#include "halCHIP.h"
#include "fwHVD_if.h"

void* g_pDMSRes = NULL;

#define DMS_ENABLE_MVOP_INT

#if 1
#include "ULog.h"

#define DMS_PRINTF(format,args...)  ULOGD("DMS", format, ##args)
#define DMS_PRINTI(format,args...)  ULOGI("DMS", format, ##args)
#define DMS_PRINTD(format,args...)  ULOGD("DMS", format, ##args)
#define DMS_ERR(format,args...) ULOGE("DMS", format, ##args)

#else
#ifndef ANDROID
//#define DMS_PRINTF
#define DMS_PRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define DMS_PRINTF ALOGD
#endif
#endif

// FIXME: it needs XC/MVOP to add it to header file.
MS_U8   __attribute__((weak))   MApi_XC_GetSWDSIndex(SCALER_WIN eWindow);
void    __attribute__((weak))   MApi_SWDS_Fire(SCALER_WIN eWindow);
MS_U8   __attribute__((weak))   MDrv_MVOP_GetIntStatus(void);
MS_BOOL __attribute__((weak))   MDrv_MVOP_EnableInterrupt(MS_U8 eIntType);
MS_U8   __attribute__((weak))   MDrv_MVOP_SubGetIntStatus(void);
MS_BOOL __attribute__((weak))   MDrv_MVOP_SubEnableInterrupt(MS_U8 eIntType);

E_APIXC_ReturnValue __attribute__((weak)) MApi_XC_GetPixelShift(MS_S8 *ps8HOffset, MS_S8 *ps8VOffset);

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DMS_FHD_Hszie           1920
#define DMS_FHD_Vszie           1080
#define DMS_4K_Hsize            4096
#define DMS_4K_Vsize            2160

#define NEW_FLIP        1
#define ENABLE_FRAME_CONVERT 1

//------------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------------
#define DNRFrameNUM       5
#define DMS_INVALID_FRAME_ID 0xFF
#define FRAME_DURATION(x) (1000000 / ((x) ? (x) : 30000))
#define WAIT_MVOP_TIME 100      // 0.1 ms
#define DISPQ_FIRST_R_INDEX         1

static MS_S32 s32DMSEventID[DMS_MAX_WINDOW_NUM] = {-1, -1};
static MS_S32 s32DMSTaskID[DMS_MAX_WINDOW_NUM] = {-1, -1};
static MS_S32 s32DMSVsyncEventID[DMS_MAX_WINDOW_NUM] = {-1, -1};

static DMS_DRV_CTRL gDMSCtrl[DMS_MAX_WINDOW_NUM];

static MS_BOOL bMsLogON = FALSE;

//Marco to simplize resource usage
#define DMS_GET_RES_PRI DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;\
                        UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri))

#define RES_DMS pDMSResPri->stDrvDMS
//#define RES_DMSCAPTURE pDMSResPri->stDrvDMSCapMode

#define ENABLE_DMS_MUTEX
#ifdef ENABLE_DMS_MUTEX
static  MS_S32 s32DMSMutexID[DMS_MAX_WINDOW_NUM] = {-1, -1};

#define _DRV_DMS_MutexCreate(WinID)                             \
    do                                                          \
    {                                                           \
        if (s32DMSMutexID[WinID] < 0)                           \
        {                                                       \
            if (WinID) {                                        \
                s32DMSMutexID[WinID] = MsOS_CreateMutex(E_MSOS_FIFO, "DMS_Drv_0", MSOS_PROCESS_SHARED); \
            } else {                                            \
                s32DMSMutexID[WinID] = MsOS_CreateMutex(E_MSOS_FIFO, "DMS_Drv_1", MSOS_PROCESS_SHARED); \
            }                                                   \
        }                                                       \
    } while (0)

#define _DRV_DMS_MutexDelete(WinID)                                  \
    do                                                          \
    {                                                           \
        if (s32DMSMutexID[WinID] >= 0)                          \
        {                                                       \
            MsOS_DeleteMutex(s32DMSMutexID[WinID]);             \
            s32DMSMutexID[WinID] = -1;                          \
        }                                                       \
    } while (0)

#define  _DRV_DMS_Entry(WinID)                                                       \
    do                                                                          \
    {                                                                           \
        if (s32DMSMutexID[WinID] >= 0)                                          \
        {                                                                       \
            if (!MsOS_ObtainMutex(s32DMSMutexID[WinID], MSOS_WAIT_FOREVER))     \
            {                                                                   \
                DMS_PRINTF("[DMS][%06d] Mutex taking timeout\n", __LINE__);     \
            }                                                                   \
        }                                                                       \
    } while (0)

#define _DRV_DMS_Return(WinID, _ret_)                                  \
    do                                                          \
    {                                                           \
        if (s32DMSMutexID[WinID] >= 0)                          \
        {                                                       \
            MsOS_ReleaseMutex(s32DMSMutexID[WinID]);            \
        }                                                       \
        return _ret_;                                           \
    } while(0)

#define _DRV_DMS_Release(WinID)                                 \
    do                                                          \
    {                                                           \
        if (s32DMSMutexID[WinID] >= 0)                          \
        {                                                       \
            MsOS_ReleaseMutex(s32DMSMutexID[WinID]);            \
        }                                                       \
    } while (0)

#else

#define _DRV_DMS_MutexCreate(WinID)
#define _DRV_DMS_MutexDelete(WinID)
#define _DRV_DMS_Entry(WinID)
#define _DRV_DMS_Return(WinID, _ret)      {return _ret;}
#define _DRV_DMS_Release(WinID)

#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U16 gDMS_externDebugFlag = 0;
//MS_U16 gDMSMVOPForceLookVDEC = 0;
#define sync_print(fmt, args...) do { if(0) DMS_PRINTF(fmt, ##args);  } while (0)
MS_U16 gDMSDebug = 0;
void* pTaskInstance = NULL;

#define DBG_DISABLE_SEMAPHORE 0
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
        DMS_PRINTF("[%s, %d]No instance existed, please get an instance by calling MApi_DMS_Init() first\n", __FUNCTION__, __LINE__);
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
static void _DMS_Wait_MVOP_Vsync(unsigned char Overlay_ID, const ST_DMS_DISPFRAMEFORMAT *dff)
{
#ifdef DMS_ENABLE_MVOP_INT
    MVOP_Handle stMvopHd;
    MS_U16 u16Vcnt = 0;
    MS_BOOL bEnable = FALSE;
    MS_U32 u32WaitEventFlag = E_MVOP_INT_VSYNC;
    MS_U32 u32Events = 0;

    if (Overlay_ID == 0) {
        MDrv_MVOP_GetIsEnable(&bEnable);
    } else {
        MDrv_MVOP_SubGetIsEnable(&bEnable);
    }

    if (!bEnable || !gDMSCtrl[Overlay_ID].bVsyncIntAlive) {
        if (bMsLogON)
        DMS_PRINTF("MVOP not enable bVsyncIntAlive = %d !!\n", gDMSCtrl[Overlay_ID].bVsyncIntAlive);
        return;
    }

    if (s32DMSVsyncEventID[Overlay_ID] == -1) {
        DMS_PRINTF("s32DMSVsyncEventID[%d] = %d !!\n", Overlay_ID, (int)s32DMSVsyncEventID[Overlay_ID]);
        return;
    }

    MsOS_WaitEvent(s32DMSVsyncEventID[Overlay_ID], u32WaitEventFlag, &u32Events, E_OR_CLEAR, FRAME_DURATION(dff->u32FrameRate));
    MDrv_MVOP_GetCommand(&stMvopHd, E_MVOP_CMD_GET_VCOUNT, &u16Vcnt, sizeof(u16Vcnt));

    if (bMsLogON)
    DMS_PRINTF("[%d] Wait mvop timing end u16Vcnt = %d !!\n", Overlay_ID, u16Vcnt);
#endif
}

static MS_BOOL _DMS_Need_XC_HDR_DS(const ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
#if defined(UFO_XC_SET_DSINFO_V0) && (API_XCDS_INFO_VERSION >= 2)
    if (pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_HEVC_DV
        || (pstDispFrameFormat->stHDRInfo.u32FrmInfoExtAvail &
        (E_DMS_HDR_METADATA_TCH + E_DMS_HDR_METADATA_HDR10_PER_FRAME + E_DMS_HDR_METATYPE_DYNAMIC))) {

        if ((pstDispFrameFormat->stHDRInfo.u32FrmInfoExtAvail & E_DMS_HDR_METATYPE_DYNAMIC)
            && !pstDispFrameFormat->stHDRInfo.stDolbyHDRInfo.u8DMEnable) {
            return FALSE;
        }

        return TRUE;
    } else
#endif
    {
        return FALSE;
    }
}

static void _DMS_Update_DS_Table(void* pInstance, unsigned char Overlay_ID, ST_DMS_DISPFRAMEFORMAT *dff, MS_U8 u8FrameIndex, MS_BOOL bSizeChange)
{
    MS_U32 u32Time = MsOS_GetSystemTime();
    MS_U16 u16Width = dff->stFrames[u8FrameIndex].u32Width
                      - dff->stFrames[u8FrameIndex].u32CropLeft
                      - dff->stFrames[u8FrameIndex].u32CropRight;
    MS_U16 u16Height = dff->stFrames[u8FrameIndex].u32Height
                       - dff->stFrames[u8FrameIndex].u32CropTop
                       - dff->stFrames[u8FrameIndex].u32CropBottom;
    XC_SETWIN_INFO stXC_SetWin_Info;
    memset(&stXC_SetWin_Info, 0, sizeof(XC_SETWIN_INFO));


    ST_DMS_WINDOW CropWin = {0, 0, 0, 0};
    ST_DMS_WINDOW DispWin = {0, 0, 0, 0};;

    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes, (void**)(&pDMSResPri));
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    if (pDMSResPri->stDrvDMS.bWindowInfoReady[Overlay_ID]) {
        DispWin.u32x = pDMSResPri->stDrvDMS.stDisplayWin[Overlay_ID].u32x;
        DispWin.u32y = pDMSResPri->stDrvDMS.stDisplayWin[Overlay_ID].u32y;
        DispWin.u32width = pDMSResPri->stDrvDMS.stDisplayWin[Overlay_ID].u32width;
        DispWin.u32height = pDMSResPri->stDrvDMS.stDisplayWin[Overlay_ID].u32height;

        CropWin.u32x = pDMSResPri->stDrvDMS.stOutputWinInfo[Overlay_ID].stCropWin.u32x;
        CropWin.u32y = pDMSResPri->stDrvDMS.stOutputWinInfo[Overlay_ID].stCropWin.u32y;
        CropWin.u32width = pDMSResPri->stDrvDMS.stOutputWinInfo[Overlay_ID].stCropWin.u32width;
        CropWin.u32height = pDMSResPri->stDrvDMS.stOutputWinInfo[Overlay_ID].stCropWin.u32height;

        // clear the outside crop win
        memset(&pDMSResPri->stDrvDMS.stOutputWinInfo[Overlay_ID].stCropWin, 0, sizeof(ST_DMS_WINDOW));
    }

    if (CropWin.u32x || CropWin.u32y
        || CropWin.u32width || CropWin.u32height) {
        // outside crop win assigned
        gDMSCtrl[Overlay_ID].stOutSideCropWin = CropWin;
        gDMSCtrl[Overlay_ID].u32WidthForOutsideCrop = u16Width;
        gDMSCtrl[Overlay_ID].u32HeightForOutsideCrop = u16Height;

    } else if (gDMSCtrl[Overlay_ID].stOutSideCropWin.u32x || gDMSCtrl[Overlay_ID].stOutSideCropWin.u32y
        || gDMSCtrl[Overlay_ID].stOutSideCropWin.u32width || gDMSCtrl[Overlay_ID].stOutSideCropWin.u32height) {
        // apply the saved crop ratio

        CropWin.u32x = (u16Width * gDMSCtrl[Overlay_ID].stOutSideCropWin.u32x) / gDMSCtrl[Overlay_ID].u32WidthForOutsideCrop;
        CropWin.u32y = (u16Height * gDMSCtrl[Overlay_ID].stOutSideCropWin.u32y) / gDMSCtrl[Overlay_ID].u32HeightForOutsideCrop;
        CropWin.u32width = (u16Width * gDMSCtrl[Overlay_ID].stOutSideCropWin.u32width) / gDMSCtrl[Overlay_ID].u32WidthForOutsideCrop;
        CropWin.u32height = (u16Height * gDMSCtrl[Overlay_ID].stOutSideCropWin.u32height) / gDMSCtrl[Overlay_ID].u32HeightForOutsideCrop;

    } else {
        // no out side crop win, use source video crop win
        CropWin.u32x = dff->stFrames[u8FrameIndex].u32CropLeft;
        CropWin.u32y = dff->stFrames[u8FrameIndex].u32CropTop;
        CropWin.u32width = u16Width;
        CropWin.u32height = u16Height;
    }

    pDMSResPri->stDrvDMS.bWinChanged[Overlay_ID] = FALSE;

    if (!pDMSResPri->stDrvDMS.bWindowInfoReady[Overlay_ID]) {
        // no window info, only trigger MApi_XC_SetDSInfo / MApi_SWDS_Fire / MApi_XC_GetSWDSIndex
        bSizeChange = FALSE;
    }

    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

    if (!MApi_XC_GetDynamicScalingStatus()) {
        DMS_PRINTF("MApi_XC_GetDynamicScalingStatus() is FALSE\n");
        return;
    }

    // apple panel ratio
    // ....

#ifdef UFO_XC_SETWINDOW_LITE
    ST_XC_APISTATUSNODELAY stXCStatus;
#else
    XC_ApiStatus stXCStatus;
#endif

    MVOP_Timing stMVOPTiming;

    if (Overlay_ID) {
        MDrv_MVOP_SubGetOutputTiming(&stMVOPTiming);
    } else {
        MDrv_MVOP_GetOutputTiming(&stMVOPTiming);
    }

    // get scaler information
#ifdef UFO_XC_SETWINDOW_LITE
    if (MApi_XC_GetStatusNodelay(&stXCStatus, (SCALER_WIN) Overlay_ID) == FALSE) {
        DMS_PRINTF("MApi_XC_GetStatusNodealy failed.\n");
    }
#else
    if (MApi_XC_GetStatus(&stXCStatus, (SCALER_WIN) Overlay_ID) == FALSE) {
        DMS_PRINTF("MApi_XC_GetStatus failed.\n");
    }
#endif

    stXC_SetWin_Info.enInputSourceType = stXCStatus.enInputSourceType;

    if (Overlay_ID) {
        stXC_SetWin_Info.stCapWin.x = MDrv_MVOP_SubGetHStart();
        stXC_SetWin_Info.stCapWin.y = MDrv_MVOP_SubGetVStart();
    } else {
        stXC_SetWin_Info.stCapWin.x = MDrv_MVOP_GetHStart();
        stXC_SetWin_Info.stCapWin.y = MDrv_MVOP_GetVStart();
    }

    stXC_SetWin_Info.stCapWin.width = u16Width;
    stXC_SetWin_Info.stCapWin.height = u16Height;

    stXC_SetWin_Info.stDispWin.width = stXCStatus.stDispWin.width;
    stXC_SetWin_Info.stDispWin.height = stXCStatus.stDispWin.height;

    // Timing
    stXC_SetWin_Info.bHDuplicate = stMVOPTiming.bHDuplicate;
    stXC_SetWin_Info.u16InputVTotal = stMVOPTiming.u16V_TotalCount;
    stXC_SetWin_Info.bInterlace = stMVOPTiming.bInterlace;
    if (stXC_SetWin_Info.bInterlace) {
        stXC_SetWin_Info.u16InputVFreq = (stMVOPTiming.u16ExpFrameRate * 2) / 100;
    } else {
        stXC_SetWin_Info.u16InputVFreq = stMVOPTiming.u16ExpFrameRate / 100;
    }

    if (gDMSCtrl[Overlay_ID].u8ForcePMode) {
        DMS_PRINTF("set real interlace mode to XC %d\n", dff->u8Interlace);
        stXC_SetWin_Info.bInterlace = dff->u8Interlace;
    }

    #ifndef UFO_XC_SETWINDOW_LITE
    stXC_SetWin_Info.u16DefaultHtotal = stXCStatus.u16DefaultHtotal;
    stXC_SetWin_Info.u8DefaultPhase = stXCStatus.u8DefaultPhase;

    // customized scaling //post
    stXC_SetWin_Info.bHCusScaling = stXCStatus.bHCusScaling;
    stXC_SetWin_Info.u16HCusScalingSrc = stXCStatus.u16HCusScalingSrc; //stXC_SetWin_Info.stCapWin.width
    stXC_SetWin_Info.u16HCusScalingDst = stXCStatus.u16HCusScalingDst;
    stXC_SetWin_Info.bVCusScaling = stXCStatus.bVCusScaling;
    stXC_SetWin_Info.u16VCusScalingSrc = stXCStatus.u16VCusScalingSrc; //stXC_SetWin_Info.stCapWin.height
    stXC_SetWin_Info.u16VCusScalingDst = stXCStatus.u16VCusScalingDst;
    // 9 lattice
    stXC_SetWin_Info.bDisplayNineLattice = stXCStatus.bDisplayNineLattice;
    #else
    stXC_SetWin_Info.bHCusScaling = 0;
    stXC_SetWin_Info.bVCusScaling = 0;
    #endif

    //add // force disable prescaling
    stXC_SetWin_Info.bPreHCusScaling = TRUE;
    stXC_SetWin_Info.bPreVCusScaling = TRUE;
    stXC_SetWin_Info.u16PreHCusScalingSrc = stXC_SetWin_Info.stCapWin.width;
    if (gDMSCtrl[Overlay_ID].u8SaveBandwidthMode && (u16Width >= 3840)) {
        // for saving BW, 4K video H prescaling down 1/2
        stXC_SetWin_Info.u16PreHCusScalingDst = stXC_SetWin_Info.stCapWin.width / 2;
    } else if (u16Width >= 3840) {
        // exceed 3840, prescaling down to 3840
        stXC_SetWin_Info.u16PreHCusScalingDst = 3840;
    } else {
        stXC_SetWin_Info.u16PreHCusScalingDst = stXC_SetWin_Info.stCapWin.width;
    }
    stXC_SetWin_Info.u16PreVCusScalingSrc = stXC_SetWin_Info.stCapWin.height;
    // height exceed 2160, prescaling down to 2160
    if (u16Height >= 2160) {
        stXC_SetWin_Info.u16PreVCusScalingDst = 2160;
    } else {
        stXC_SetWin_Info.u16PreVCusScalingDst = stXC_SetWin_Info.stCapWin.height;
    }

    if (bSizeChange) {
#if 0
        DMS_PRINTF("[%s,%5d] DispW:%d DispH:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.stDispWin.width, stXC_SetWin_Info.stDispWin.height);

        DMS_PRINTF("[%s,%5d] Cap.x:%d Cap.y:%d CapW:%d CapH:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.stCapWin.x, stXC_SetWin_Info.stCapWin.y,
            stXC_SetWin_Info.stCapWin.width, stXC_SetWin_Info.stCapWin.height);
        DMS_PRINTF("[%s,%5d] enInputSourceType:%d \n",__func__,__LINE__,stXC_SetWin_Info.enInputSourceType);
        DMS_PRINTF("[%s,%5d] u16InputVTotal:%d  u16InputVFreq:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.u16InputVTotal, stXC_SetWin_Info.u16InputVFreq);
        DMS_PRINTF("[%s,%5d] bInterlace:%d  u16DefaultHtotal:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.bInterlace, stXC_SetWin_Info.u16DefaultHtotal);
        //prescaling
        DMS_PRINTF("[%s,%5d] bPreHCusScaling:%d  bPreVCusScaling:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.bPreHCusScaling, stXC_SetWin_Info.bPreVCusScaling);
        DMS_PRINTF("[%s,%5d] u16PreHCusScalingSrc:%d  u16PreHCusScalingDst:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.u16PreHCusScalingSrc, stXC_SetWin_Info.u16PreHCusScalingDst);
        DMS_PRINTF("[%s,%5d] u16PreVCusScalingSrc:%d  u16PreVCusScalingSrc:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.u16PreVCusScalingSrc, stXC_SetWin_Info.u16PreVCusScalingDst);
        //postscaling
        DMS_PRINTF("[%s,%5d] bHCusScaling:%d  bVCusScaling:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.bHCusScaling, stXC_SetWin_Info.bVCusScaling);
        DMS_PRINTF("[%s,%5d] u16HCusScalingSrc:%d  u16HCusScalingDst:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.u16HCusScalingSrc, stXC_SetWin_Info.u16HCusScalingDst);
        DMS_PRINTF("[%s,%5d] u16VCusScalingSrc:%d  u16VCusScalingDst:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.u16VCusScalingSrc, stXC_SetWin_Info.u16VCusScalingDst);
#endif

#ifdef PORTING_MMSDK
        if ((OutSideCropWin[Overlay_ID].x != 0)
            || (OutSideCropWin[Overlay_ID].y != 0)
            || (OutSideCropWin[Overlay_ID].width != 0)
            || (OutSideCropWin[Overlay_ID].height != 0)) {

            vInfo.u16CropLeft = OutSideCropWin[Overlay_ID].x;
            vInfo.u16CropTop = OutSideCropWin[Overlay_ID].y;
            vInfo.u16CropRight = vInfo.u16HorSize
                                - OutSideCropWin[Overlay_ID].x
                                - OutSideCropWin[Overlay_ID].width;
            vInfo.u16CropBottom = vInfo.u16VerSize
                                - OutSideCropWin[Overlay_ID].y
                                - OutSideCropWin[Overlay_ID].height;

            DMS_PRINTF("update ds table src w:%d h:%d, **apply** crop w:%d h:%d, disp w:%d h:%d\n",
                u16SrcWidth[Overlay_ID], u16SrcHeight[Overlay_ID],
                OutSideCropWin[Overlay_ID].width, OutSideCropWin[Overlay_ID].height,
                OutputWin[Overlay_ID].width, OutputWin[Overlay_ID].height);
            DMS_PRINTF("apply dff crop x:%d y:%d w:%d h:%d\n", vInfo.u16CropLeft, vInfo.u16CropTop,
                vInfo.u16HorSize - vInfo.u16CropLeft - vInfo.u16CropRight,
                vInfo.u16VerSize - vInfo.u16CropTop - vInfo.u16CropBottom);

            OutSideCropWinRatio[Overlay_ID].x = (float)OutSideCropWin[Overlay_ID].x / (float)u16Width;
            OutSideCropWinRatio[Overlay_ID].y =  (float)OutSideCropWin[Overlay_ID].y / (float)u16Height;
            OutSideCropWinRatio[Overlay_ID].width = (float)OutSideCropWin[Overlay_ID].width / (float)u16Width;
            OutSideCropWinRatio[Overlay_ID].height = (float)OutSideCropWin[Overlay_ID].height / (float)u16Height;

            DMS_PRINTF("save crop ratio x:%f y:%f w:%f h:%f\n", OutSideCropWinRatio[Overlay_ID].x,
                OutSideCropWinRatio[Overlay_ID].y,
                OutSideCropWinRatio[Overlay_ID].width,
                OutSideCropWinRatio[Overlay_ID].height);
        } else if ((OutSideCropWinRatio[Overlay_ID].x != 0)
            || (OutSideCropWinRatio[Overlay_ID].y != 0)
            || (OutSideCropWinRatio[Overlay_ID].width != 0)
            || (OutSideCropWinRatio[Overlay_ID].height != 0)) {

            MS_WINDOW newCropWin;

            newCropWin.x = u16Width * OutSideCropWinRatio[Overlay_ID].x;
            newCropWin.y = u16Height * OutSideCropWinRatio[Overlay_ID].y;
            newCropWin.width = u16Width * OutSideCropWinRatio[Overlay_ID].width;
            newCropWin.height = u16Height * OutSideCropWinRatio[Overlay_ID].height;

            vInfo.u16CropLeft = newCropWin.x;
            vInfo.u16CropTop = newCropWin.y;
            vInfo.u16CropRight = vInfo.u16HorSize
                                - newCropWin.x
                                - newCropWin.width;
            vInfo.u16CropBottom = vInfo.u16VerSize
                                - newCropWin.y
                                - newCropWin.height;

            DMS_PRINTF("apply saved crop ratio x:%d y:%d w:%d h:%d\n", vInfo.u16CropLeft, vInfo.u16CropTop,
                vInfo.u16HorSize - vInfo.u16CropLeft - vInfo.u16CropRight,
                vInfo.u16VerSize - vInfo.u16CropTop - vInfo.u16CropBottom);
        }
#endif

#ifndef PORTING_MMSDK
        //vsync_bridge_CalAspectRatio(Overlay_ID, &DispWin, &CropWin, dff, TRUE);
#endif

#if 0
        if (bSizeChange) {
          Wrapper_SurfaceVideoSizeChange(Overlay_ID, DispWin.x,
              DispWin.y, DispWin.width, DispWin.height, u16SrcWidth[Overlay_ID],
              u16SrcHeight[Overlay_ID]);
        }
#endif

        if (gDMSCtrl[Overlay_ID].u8MirrorMode & E_VOPMIRROR_HORIZONTALL) {
            DispWin.u32x = gDMSCtrl[Overlay_ID].panelWidth - (DispWin.u32x + DispWin.u32width);
            CropWin.u32x = u16Width - (CropWin.u32x + CropWin.u32width);
        }
        if (gDMSCtrl[Overlay_ID].u8MirrorMode & E_VOPMIRROR_VERTICAL) {
            DispWin.u32y = gDMSCtrl[Overlay_ID].panelHeight - (DispWin.u32y + DispWin.u32height);
            CropWin.u32y = u16Height - (CropWin.u32y + CropWin.u32height);
        }
        if (bSizeChange && (gDMSCtrl[Overlay_ID].u8MirrorMode & E_VOPMIRROR_HVBOTH)) {
            DMS_PRINTF("DS MVOP Mirror %d [%d %d %d %d] [%d %d %d %d]\n", (int)gDMSCtrl[Overlay_ID].u8MirrorMode,
                (int)DispWin.u32x, (int)DispWin.u32y, (int)DispWin.u32width, (int)DispWin.u32height,
                (int)CropWin.u32x, (int)CropWin.u32y, (int)CropWin.u32width, (int)CropWin.u32height);
        }

        stXC_SetWin_Info.stCropWin.x = CropWin.u32x;
        stXC_SetWin_Info.stCropWin.y = CropWin.u32y;
        stXC_SetWin_Info.stCropWin.width = CropWin.u32width;
        stXC_SetWin_Info.stCropWin.height = CropWin.u32height;

        stXC_SetWin_Info.stDispWin.x = DispWin.u32x;
        stXC_SetWin_Info.stDispWin.y = DispWin.u32y;
        stXC_SetWin_Info.stDispWin.width = DispWin.u32width;
        stXC_SetWin_Info.stDispWin.height = DispWin.u32height;

        if (!stXC_SetWin_Info.stDispWin.width || !stXC_SetWin_Info.stDispWin.height) {
            DMS_PRINTF("[%s,%5d] Disp Win is 0, set a default value\n", __func__, __LINE__);
            stXC_SetWin_Info.stDispWin.x = 0;
            stXC_SetWin_Info.stDispWin.y = 0;
            stXC_SetWin_Info.stDispWin.width = g_IPanel.Width();
            stXC_SetWin_Info.stDispWin.height = g_IPanel.Height();
        }

        // disable force index !!!!!!!!!!!!!!!!!
        MApi_XC_Set_DSForceIndex(0, 0, (SCALER_WIN)Overlay_ID);
        DMS_PRINTF("[%s,%5d] SW DS !!! u8ForceInterlace = %d\n", __func__, __LINE__, gDMSCtrl[Overlay_ID].u8ForceInterlace);
        DMS_PRINTF("[%s,%5d] Cap.x:%d Cap.y:%d Cap.W:%d Cap.H:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.stCapWin.x, stXC_SetWin_Info.stCapWin.y,
            stXC_SetWin_Info.stCapWin.width, stXC_SetWin_Info.stCapWin.height);
        DMS_PRINTF("[%s,%5d] Disp.x:%d Disp.y:%d Disp.W:%d Disp.H:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.stDispWin.x, stXC_SetWin_Info.stDispWin.y,
            stXC_SetWin_Info.stDispWin.width, stXC_SetWin_Info.stDispWin.height);
        DMS_PRINTF("[%s,%5d] Crop.x:%d Crop.y:%d Crop.W:%d Crop.H:%d \n", __func__, __LINE__,
            stXC_SetWin_Info.stCropWin.x, stXC_SetWin_Info.stCropWin.y,
            stXC_SetWin_Info.stCropWin.width, stXC_SetWin_Info.stCropWin.height);
        DMS_PRINTF("[%s,%5d] MApi_XC_GetDynamicScalingStatus():%d \n", __func__, __LINE__, MApi_XC_GetDynamicScalingStatus());
    }

    if (MApi_XC_GetDynamicScalingStatus()) {

#ifdef UFO_XC_SWDS_SW_FILM_ENABLE
        if ((gDMSCtrl[Overlay_ID].u8ForceInterlace || gDMSCtrl[Overlay_ID].u8ForcePMode) && MApi_XC_Set_DS_ForceI_DeInterlaceMode) {
            DMS_PRINTF("[%s,%5d] SW DS !!! \n", __func__, __LINE__);
            DMS_PRINTF("[%s,%5d] MApi_XC_Set_DS_ForceI_DeInterlaceMode %d %d\n", __func__, __LINE__, dff->u8Interlace, dff->u32FrameRate);
            MApi_XC_Set_DS_ForceI_DeInterlaceMode(dff->u8Interlace ? 0 : 1, TRUE, dff->u32FrameRate, (SCALER_WIN)Overlay_ID);
        }
#endif

#ifdef UFO_XC_SET_DSINFO_V0
        // Inform xc to update the ds info in mcu mode
        if (MApi_XC_SetDSInfo)
        {
            XC_DS_INFO stXCDSInfo = {0, };

            stXCDSInfo.u32ApiDSInfo_Version = API_XCDS_INFO_VERSION;
            stXCDSInfo.u16ApiDSInfo_Length = sizeof(XC_DS_INFO);
            stXCDSInfo.bUpdate_DS_CMD[(SCALER_WIN) Overlay_ID] = TRUE;

#if (API_XCDS_INFO_VERSION == 1)
            if (sizeof(XC_DS_HDRInfo) == sizeof(MS_HDRInfo)) {
                memcpy(&stXCDSInfo.stHDRInfo, &dff->sHDRInfo, sizeof(XC_DS_HDRInfo));
            } else {
                DMS_PRINTF("MS_HDRInfo/XC_DS_HDRInfo size is not match\n");
            }
#elif (API_XCDS_INFO_VERSION >= 2)
            stXCDSInfo.stHDRInfo.u32FrmInfoExtAvail = dff->stHDRInfo.u32FrmInfoExtAvail;

            stXCDSInfo.stHDRInfo.stColorDescription.u8ColorPrimaries = dff->stHDRInfo.stColorDescription.u8ColorPrimaries;
            stXCDSInfo.stHDRInfo.stColorDescription.u8MatrixCoefficients = dff->stHDRInfo.stColorDescription.u8MatrixCoefficients;
            stXCDSInfo.stHDRInfo.stColorDescription.u8TransferCharacteristics = dff->stHDRInfo.stColorDescription.u8TransferCharacteristics;

            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u32MaxLuminance = dff->stHDRInfo.stMasterColorDisplay.u32MaxLuminance;
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u32MinLuminance = dff->stHDRInfo.stMasterColorDisplay.u32MinLuminance;
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[0][0] = dff->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[0][0];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[0][1] = dff->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[0][1];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[1][0] = dff->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[1][0];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[1][1] = dff->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[1][1];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[2][0] = dff->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[2][0];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[2][1] = dff->stHDRInfo.stMasterColorDisplay.u16DisplayPrimaries[2][1];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16WhitePoint[0] = dff->stHDRInfo.stMasterColorDisplay.u16WhitePoint[0];
            stXCDSInfo.stHDRInfo.stMasterColorDisplay.u16WhitePoint[1] = dff->stHDRInfo.stMasterColorDisplay.u16WhitePoint[1];

            stXCDSInfo.stHDRInfo.u8CurrentIndex = dff->stHDRInfo.stDolbyHDRInfo.u8CurrentIndex;
            stXCDSInfo.stHDRInfo.phyRegAddr = dff->stHDRInfo.stDolbyHDRInfo.phyHDRRegAddr;
            stXCDSInfo.stHDRInfo.u32RegSize = dff->stHDRInfo.stDolbyHDRInfo.u32HDRRegSize;
            stXCDSInfo.stHDRInfo.phyLutAddr = dff->stHDRInfo.stDolbyHDRInfo.phyHDRLutAddr;
            stXCDSInfo.stHDRInfo.u32LutSize = dff->stHDRInfo.stDolbyHDRInfo.u32HDRLutSize;
            stXCDSInfo.stHDRInfo.bDMEnable = dff->stHDRInfo.stDolbyHDRInfo.u8DMEnable;
            stXCDSInfo.stHDRInfo.bCompEnable = dff->stHDRInfo.stDolbyHDRInfo.u8CompEnable;
#endif


#if (API_XCDS_INFO_VERSION >= 3)
            stXCDSInfo.bEnableDNR[(SCALER_WIN) Overlay_ID] = (gDMSCtrl[Overlay_ID].u8DNRFrameCnt == DNRFrameNUM);
#endif

#if (API_XCDS_INFO_VERSION >= 4)
            stXCDSInfo.bEnable_ForceP[(SCALER_WIN) Overlay_ID] = gDMSCtrl[Overlay_ID].u8ForcePMode;
#endif

            MApi_XC_SetDSInfo(&stXCDSInfo, sizeof(stXCDSInfo), (SCALER_WIN) Overlay_ID);
        }
#endif


#ifdef UFO_XC_SETWINDOW_LITE
        if (!bSizeChange) {
            MApi_SWDS_Fire((SCALER_WIN)Overlay_ID);
        } else
#endif
        if (MApi_XC_SetWindow(&stXC_SetWin_Info, sizeof(XC_SETWIN_INFO), (SCALER_WIN)Overlay_ID) == FALSE) {
            DMS_PRINTF("MApi_XC_SetWindow failed!\n");
        }

#ifdef UFO_XC_SWDS_SW_FILM_ENABLE
        if ((gDMSCtrl[Overlay_ID].u8ForceInterlace || (gDMSCtrl[Overlay_ID].u8ForcePMode && dff->u8Interlace))
            && MApi_XC_Set_DS_ForceI_DeInterlaceMode) {

            gDMSCtrl[Overlay_ID].ds_curr_index.u4DSIndex0 = MApi_XC_GetSWDSIndex((SCALER_WIN)Overlay_ID);
            DMS_PRINTF("[%s,%5d] MApi_XC_Set_DS_ForceI_DeInterlaceMode %d %d\n", __func__, __LINE__, dff->u8Interlace, dff->u32FrameRate);

            MApi_XC_Set_DS_ForceI_DeInterlaceMode(dff->u8Interlace ? 0 : 1, FALSE, dff->u32FrameRate, (SCALER_WIN) Overlay_ID);

            if (MApi_XC_SetWindow(&stXC_SetWin_Info, sizeof(XC_SETWIN_INFO), (SCALER_WIN) Overlay_ID) == FALSE) {
                DMS_PRINTF("[%s,%5d] MApi_XC_SetWindow failed!\n", __func__, __LINE__);
            }

            gDMSCtrl[Overlay_ID].ds_curr_index.u4DSIndex1 = MApi_XC_GetSWDSIndex((SCALER_WIN)Overlay_ID);
        } else
#endif
        {
            gDMSCtrl[Overlay_ID].ds_curr_index.u8DSIndex = MApi_XC_GetSWDSIndex((SCALER_WIN)Overlay_ID);
        }
    }

    if (bMsLogON) {
        DMS_PRINTF("[%s,%5d] [%d].u8DSIndex = 0x%x, time = %d ms\n", __func__,
            __LINE__, Overlay_ID, gDMSCtrl[Overlay_ID].ds_curr_index.u8DSIndex, (int)(MsOS_GetSystemTime() - u32Time));
    }
}

void _DMS_Ddecide_SW_FRC_Mode(MS_U32 u32WindowID, const ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat, MS_U32 u32OutputFrmRate)
{
    MS_U32 u32FrameRate = pstDispFrameFormat->u32FrameRate;
    MS_U8 u8NextFrcMode = E_HVD_FRC_NORMAL;

    if (u32OutputFrmRate) {
        if (pstDispFrameFormat->u8Interlace) {   // Only Interlace Mode
            if ((u32FrameRate << 1) == u32OutputFrmRate) {   // Ex: 30x2 = 60i
                u8NextFrcMode = E_HVD_FRC_NORMAL;
            } else if (u32FrameRate == 60000) {   // Input 120i
                if (u32OutputFrmRate == 50000) {
                    // 120i -> 50i
                    u8NextFrcMode = E_HVD_FRC_120I_50I;
                } else if (u32OutputFrmRate == 60000) {
                    // 120i -> 60i
                    u8NextFrcMode = E_HVD_FRC_HALF_I;
                }
            } else if (u32FrameRate == 50000) {   // Input 100i
                if (u32OutputFrmRate == 60000) {
                    // 100i -> 60i
                    u8NextFrcMode = E_HVD_FRC_100I_60I;
                } else if (u32OutputFrmRate == 50000) {
                    // 100i -> 50i
                    u8NextFrcMode = E_HVD_FRC_HALF_I;
                }
            } else if (u32FrameRate >= 29970
                       && u32FrameRate <= 30000) {
                // 60i -> 50i
                if (u32OutputFrmRate == 50000) {
                    u8NextFrcMode = E_HVD_FRC_NTSC2PAL;
                } else if (u32OutputFrmRate == 30000) {
                    u8NextFrcMode = E_HVD_FRC_HALF_I;
                } else if (u32OutputFrmRate == 25000) {
                    u8NextFrcMode = E_HVD_FRC_120I_50I; // 30 fps interlace = 60i, 60i to 25i == 120i to 50i
                } else if (u32OutputFrmRate == 24000) {
                    u8NextFrcMode = E_HVD_FRC_30_24;
                }
            } else if (u32FrameRate == 25000) {   // Input 50i
                if (u32OutputFrmRate == 60000) {   // 50i -> 60i
                    u8NextFrcMode = E_HVD_FRC_PAL2NTSC;
                } else if ((u32OutputFrmRate == 30000)) {
                    u8NextFrcMode = E_HVD_FRC_25_30;
                }
            } else if (u32FrameRate >= 23970
                    && u32FrameRate <= 24000) {

                if (u32OutputFrmRate == 60000) {   // 48i -> 60i
                    u8NextFrcMode = E_HVD_FRC_32PULLDOWN;
                } else if (u32OutputFrmRate == 50000) {   // 48i -> 50i
                    u8NextFrcMode = E_HVD_FRC_NORMAL;
                } else if (u32OutputFrmRate == 30000) {   // 48i -> 30i
                     u8NextFrcMode = E_HVD_FRC_24_30;
                }
            }
        } else {   // Only Progressive Mode
            if (u32FrameRate == u32OutputFrmRate) {   // Ex: 25p = 25p
                u8NextFrcMode = E_HVD_FRC_NORMAL;
            } else if ((u32FrameRate<<1) == u32OutputFrmRate) {   // Ex: 25p x 2 = 50p
                u8NextFrcMode = E_HVD_FRC_DISP_2X;
            } else if (u32FrameRate == 60000) {
                if (u32OutputFrmRate == 50000) {   // 60p -> 50p
                    u8NextFrcMode = E_HVD_FRC_60P_50P;
                }
            } else if (u32FrameRate == 50000) {
                if (u32OutputFrmRate == 60000) {   // 50p -> 60p
                    u8NextFrcMode = E_HVD_FRC_50P_60P;
                }
            } else if (u32FrameRate >= 29970
                       && u32FrameRate <= 30000) {
                // 30p -> 50p
                if (u32OutputFrmRate == 50000) {
                    u8NextFrcMode = E_HVD_FRC_NTSC2PAL;
                } else if ((u32OutputFrmRate == 60000)) {
                    u8NextFrcMode = E_HVD_FRC_DISP_2X;
                } else if (u32OutputFrmRate == 25000) {
                    u8NextFrcMode = E_HVD_FRC_60P_50P;  // 30p -> 25p, 60p -> 50p, the same rule
                } else if (u32OutputFrmRate == 24000) {
                    u8NextFrcMode = E_HVD_FRC_30_24;
                }
            } else if (u32FrameRate == 25000) {
                // 25p -> 60p
                if (u32OutputFrmRate == 60000) {
                    u8NextFrcMode = E_HVD_FRC_PAL2NTSC;
                } else if ((u32OutputFrmRate == 30000)) {
                    u8NextFrcMode = E_HVD_FRC_25_30;
                }
            } else if (u32FrameRate >= 23970
                    && u32FrameRate <= 24000) {

                if (u32OutputFrmRate == 60000) {
                    u8NextFrcMode = E_HVD_FRC_32PULLDOWN;
                } else if (u32OutputFrmRate == 50000) {
                    u8NextFrcMode = E_HVD_FRC_24_50;
                } else if (u32OutputFrmRate == 30000) {   // 24p -> 30p
                    u8NextFrcMode = E_HVD_FRC_24_30;
                }
            }
        }
    }

    gDMSCtrl[u32WindowID].u8FrcMode = u8NextFrcMode;

    if (pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_RV8
        || pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_RV9) {
        // rmvb is variable framerate, don't do FRC
        gDMSCtrl[u32WindowID].u8FrcMode = E_HVD_FRC_NORMAL;
    }

    DMS_PRINTF("u32FrameRate=%d, u32OutputFrmRate=%d, u8FrcMode[%d] = %d",
        (int)u32FrameRate, (int)u32OutputFrmRate, (int)u32WindowID, gDMSCtrl[u32WindowID].u8FrcMode);
}

static void _DMS_Update_SW_FRC_Mode(MS_U32 u32WindowID, const ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
    // Get is XC gen timing or not, if XC gen timing, enable SW FRC
    MVOP_Handle stMvopHd;
    MS_BOOL bIsXCGenTiming = FALSE;

    if (u32WindowID == 0) {
        stMvopHd.eModuleNum = E_MVOP_MODULE_MAIN;
    } else {
        stMvopHd.eModuleNum = E_MVOP_MODULE_SUB;
    }

    MDrv_MVOP_GetCommand(&stMvopHd, E_MVOP_CMD_GET_IS_XC_GEN_TIMING, &bIsXCGenTiming, sizeof(bIsXCGenTiming));

    if (bIsXCGenTiming) {
        _DMS_Ddecide_SW_FRC_Mode(u32WindowID, pstDispFrameFormat, MApi_XC_GetOutputVFreqX100() * 10);
    } else {
        gDMSCtrl[u32WindowID].u8FrcMode = E_HVD_FRC_NORMAL;
    }
}

static MS_BOOL _DMS_VDEC_Frame_AddRef(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
#if NEW_FLIP
    VDEC_StreamId VdecStreamId;
    VDEC_EX_DispFrame VdecDispFrm;
    VdecStreamId.u32Version = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
    VdecStreamId.u32Id = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
    VdecDispFrm.u32Idx = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex;
    VdecDispFrm.u32PriData = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData;
    VDEC_EX_Result eResult;
    eResult = MApi_VDEC_EX_DisplayFrame((VDEC_StreamId*)&VdecStreamId, &VdecDispFrm);
    if (eResult != E_VDEC_EX_OK)
    {
        DMS_PRINTF("[%s] %d %d, %d %d\n", __FUNCTION__, (int)VdecStreamId.u32Version, (int)VdecStreamId.u32Id,
            (int)VdecDispFrm.u32Idx, (int)VdecDispFrm.u32PriData);
        DMS_PRINTF("[%s] window %d, BufID %d, MApi_VDEC_EX_DisplayFrame return %d\n",__FUNCTION__, (int)u32Window,u16BufID, eResult);
        return FALSE;
    }
    //DMS_PRINTF("[AddRef] u16BufID[%d] Frame[0x%x], PriData[0x%x]\n", u16BufID, VdecDispFrm.u32Idx, VdecDispFrm.u32PriData);
    pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Window][u16BufID]++;
#endif
    return TRUE;
}

static MS_BOOL _DMS_VDEC_Frame_Release(MS_U32 u32Window, MS_U16 u16BufID)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
#if NEW_FLIP
    if(pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Window][u16BufID] == 0)
    {
        DMS_PRINTF("[%s] window %d, BufID %d, Release an invalid buffer\n",__FUNCTION__, (int)u32Window, u16BufID);
        return TRUE;
    }
    pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Window][u16BufID]--;
    if(pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Window][u16BufID] == 0)
    {
        VDEC_StreamId VdecStreamId;
        VDEC_EX_DispFrame VdecDispFrm;
        VdecStreamId.u32Version = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamVersion;
        VdecStreamId.u32Id = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32VDECStreamID;
        VdecDispFrm.u32Idx = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32FrameIndex;
        VdecDispFrm.u32PriData = pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32Window][u16BufID].u32PriData;

        VDEC_EX_Result eResult;
        eResult = MApi_VDEC_EX_ReleaseFrame((VDEC_StreamId *) &VdecStreamId, &VdecDispFrm);
        if (eResult != E_VDEC_EX_OK)
        {
            DMS_PRINTF("[%s] %d %d, %d %d\n",__FUNCTION__, (int)VdecStreamId.u32Version, (int)VdecStreamId.u32Id,
                (int)VdecDispFrm.u32Idx, (int)VdecDispFrm.u32PriData);
            DMS_PRINTF("[%s] window %d, BufID %d, MApi_VDEC_EX_ReleaseFrame return %d\n",__FUNCTION__, (int)u32Window, u16BufID, eResult);
            return FALSE;
        }
        //DMS_PRINTF("[DecRef] u16BufID[%d] Frame[0x%x], PriData[0x%x]\n", u16BufID, VdecDispFrm.u32Idx, VdecDispFrm.u32PriData);
    }
#endif
    return TRUE;
}

static void _DMS_MVOP_ISR(InterruptNum eIntNum)
{
    MS_U8 u8IntStatus;
    MS_U8 u8MvopId = ((E_INT_IRQ_DCSUB == eIntNum) ? 1 : 0);

#ifndef MSOS_TYPE_LINUX_KERNEL

    if (E_INT_IRQ_DC == eIntNum) {
        u8IntStatus = MDrv_MVOP_GetIntStatus();
    } else {
        u8IntStatus = MDrv_MVOP_SubGetIntStatus();
    }
#endif

    //if (bMsLogON)
    //    DMS_PRINTF("_DMS_MVOP_ISR eIntNum %d mvop %d int, status = 0x%x\n", eIntNum, u8MvopId, u8IntStatus);

    if (u8MvopId) {
        MDrv_MVOP_SubEnableInterrupt(0);
        MDrv_MVOP_SubEnableInterrupt(E_MVOP_INT_VSYNC);
        MsOS_EnableInterrupt(E_INT_IRQ_DCSUB);
    } else {
        MDrv_MVOP_EnableInterrupt(0);
        MDrv_MVOP_EnableInterrupt(E_MVOP_INT_VSYNC);
        MsOS_EnableInterrupt(E_INT_IRQ_DC);
    }
    //if (u8IntStatus & E_MVOP_INT_VSYNC) {
        MsOS_SetEvent(s32DMSEventID[u8MvopId], E_MVOP_INT_VSYNC);
        MsOS_SetEvent(s32DMSVsyncEventID[u8MvopId], E_MVOP_INT_VSYNC);
    //}
}

static void _Task_DMS_Mointor(MS_U32 u32Id)
{
    MS_U32 u32WaitEventFlag = E_MVOP_INT_VSYNC;
    MS_U32 u32Events = 0;
    void *pInstance = pTaskInstance;

    while(1) {

#ifdef DMS_ENABLE_MVOP_INT
        MsOS_WaitEvent(s32DMSEventID[u32Id], u32WaitEventFlag, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
#else
        MsOS_WaitEvent(s32DMSEventID[u32Id], u32WaitEventFlag, &u32Events, E_OR_CLEAR, 16);
#endif
        _DRV_DMS_Entry(u32Id);
        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
        UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

        gDMSCtrl[u32Id].bVsyncIntAlive = TRUE;
        MCU_DISPQ_INFO *pSHM = (MCU_DISPQ_INFO *)MsOS_PA2KSEG1(pDMSResPri->stDrvDMS.phyVsyncbridgeAddr[u32Id] + ((MS_U32)u32Id * sizeof(MCU_DISPQ_INFO)));
#if 0
        DMS_PRINTF("  _Task_DMS_Mointor ---> [%d %d %d %d %d %d %d %d] %d %d \n",
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][0],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][1],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][2],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][3],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][4],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][5],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][6],
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][7],
            pSHM->u8McuDispQWPtr,
            pSHM->u8McuDispQRPtr);
#endif
        int i;
        for (i = 0; i < MAX_VSYNC_BRIDGE_DISPQ_NUM; i++) {

            if (pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32Id][i]) {
                // reference count is not 0
                if(pSHM->u8McuDispQRPtr != i && pSHM->McuDispQueue[i].u8Tog_Time == 0) {
                    _DMS_VDEC_Frame_Release(u32Id, i);
                    if (bMsLogON)
                        DMS_PRINTF("  _Task_DMS_Mointor Overlay[%d] W %d, R %d, Cur %d, disp_cnt = %d\n", (int)u32Id,
                              pSHM->u8McuDispQWPtr, pSHM->u8McuDispQRPtr, i,
                              pSHM->McuDispQueue[i].u16DispCnt);
                }
            }
        }

        // display win changed in pause state
        if (pSHM->u8McuDispSwitch
            && (pSHM->u8McuDispQWPtr == pSHM->u8McuDispQRPtr)
            && !pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u8Tog_Time
            && pDMSResPri->stDrvDMS.bWinChanged[u32Id]) {

                MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

                _DMS_Update_DS_Table(pInstance, u32Id, &gDMSCtrl[u32Id].lastdff, 0, TRUE);
                pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u16CropLeft = gDMSCtrl[u32Id].stCurrentCropWin.u32x;
                pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u16CropTop = gDMSCtrl[u32Id].stCurrentCropWin.u32y;
                pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u16CropRight = pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u16Width
                                                            - gDMSCtrl[u32Id].stCurrentCropWin.u32width
                                                            - gDMSCtrl[u32Id].stCurrentCropWin.u32x;
                pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u16CropBottom = pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u16Height
                                                            - gDMSCtrl[u32Id].stCurrentCropWin.u32height
                                                            - gDMSCtrl[u32Id].stCurrentCropWin.u32y;

                if (pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u8Interlace) {
                    MS_U8 u8WRBankMappingNum = MApi_XC_GetWRBankMappingNum((SCALER_WIN)u32Id) + 1;
                    DMS_PRINTF("u8WRBankMappingNum = %d , FRAME_DURATION = %d", u8WRBankMappingNum, FRAME_DURATION(gDMSCtrl[u32Id].u16FrameRate));
                    pSHM->u8ToggleMethod = 1;
                    pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u8Tog_Time = u8WRBankMappingNum;
                    pSHM->u8ToggledTime = u8WRBankMappingNum;
                }

                pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u8DSIndex = gDMSCtrl[u32Id].ds_curr_index.u8DSIndex;

                DMS_PRINTF("  _Task_DMS_Mointor DS_Idx=0x%x, DS_Idx1=0x%x, u8DSIndex = 0x%x, MVOP ID=%d",
                    gDMSCtrl[u32Id].ds_curr_index.u4DSIndex0 , gDMSCtrl[u32Id].ds_curr_index.u4DSIndex1,
                    pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u8DSIndex, (int)u32Id);

        }
        else
        {
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        }

        _DRV_DMS_Release(u32Id);
    }
}

MS_BOOL _MDrv_DMS_CreateTask(MS_U32 u32Id)
{
#ifndef MSOS_TYPE_NOS
    // Create Event
    if(s32DMSEventID[u32Id] < 0)
    {
        s32DMSEventID[u32Id] = MsOS_CreateEventGroup(u32Id ? "DMS_Sub_Event" : "DMS_Main_Event");
    }

    if(s32DMSVsyncEventID[u32Id] < 0)
    {
        s32DMSVsyncEventID[u32Id] = MsOS_CreateEventGroup(u32Id ? "DMS_Sub_Vsync_Event" : "DMS_Main_Vsync_Event");
    }

    if(s32DMSTaskID[u32Id] == -1)
    {
        s32DMSTaskID[u32Id] = MsOS_CreateTask((TaskEntry)_Task_DMS_Mointor,
                                    u32Id,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    NULL,
                                    0,
                                    "DMS_Task");
    }
    else
    {
        DMS_PRINTF("MsOS_ResumeTask(s32DMSTaskID) \n");
        MsOS_ResumeTask(s32DMSTaskID[u32Id]);
    }

#endif
    return TRUE;
}

void _MDrv_DMS_InvalidateAllQueue(void)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    MS_U8 u8WindowID = 0;
    MS_U8 u8BufferID = 0;

    for (u8WindowID = 0; u8WindowID < DMS_MAX_WINDOW_NUM; u8WindowID++)
    {
        for(u8BufferID=0;u8BufferID<DMS_DIP_QUEUEDEPTH;u8BufferID++)
        {
            pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].bValid = FALSE;
            pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u8WindowID][u8BufferID].u32FrameIndex = DMS_INVALID_FRAME_ID;
        }
    }
}

static void _MDrv_DMS_ZOrder_Init(void)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    MS_U8 u8Window = 0;
    memset(pDMSResPri->stDrvDMS.WindowZOrder, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM);
    memset(pDMSResPri->stDrvDMS.WindowScore, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM);

    for(u8Window = 0; u8Window < DMS_MAX_WINDOW_NUM; u8Window++)
    {
        pDMSResPri->stDrvDMS.WindowZOrder[u8Window] = u8Window;
    }
}

#define BPP_OF_YUV422 2
static MS_BOOL _MDrv_DMS_Setup_Memory(EN_DMS_MEMORY_TYPE enMemType, MS_PHY phyMemBufferAddr, MS_PHY u32MemBufferSize)
{
    MS_BOOL bRet = TRUE;
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    switch(enMemType)
    {
        case E_DMS_MEMORY_TYPE_GOP_DISPLAY:
        {
            break;
        }
        case E_DMS_MEMORY_TYPE_FREEZE:
        {
            break;
        }
        case E_DMS_MEMORY_TYPE_MVOP_DISPLAY:
        {
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

static MS_BOOL _MDrv_DMS_Init(void* pInstance)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    if(pDMSResPri->bDrvInit == TRUE)
    {
        DMS_PRINTF("Driver has inited!!!!!!!!\n");
        return TRUE;
    }

#ifdef DMS_ENABLE_MVOP_INT
    //MDrv_SYS_GlobalInit();
    MDrv_MVOP_Init();
    MDrv_MVOP_SubInit();
    MDrv_MVOP_SetMMIOMapBase();

    //XC_INITDATA XC_InitData;
    // xc not init.
    //memset((void*)&XC_InitData, 0, sizeof(XC_InitData));
    //MApi_XC_Init(&XC_InitData, 0);
    //MApi_PNL_IOMapBaseInit();
#endif

    #if 0
    // Create Event
    if(s32DMSEventID[0] < 0)
    {
        s32DMSEventID[0] = MsOS_CreateEventGroup("DMS_Main_Vsync_Event");
    }
    if(s32DMSEventID[1] < 0)
    {
        s32DMSEventID[1] = MsOS_CreateEventGroup("DMS_Sub_Vsync_Event");
    }
    _MDrv_DMS_CreateTask(0);
    _MDrv_DMS_CreateTask(1);
    #endif

    // Clear VDEC info
    memset(pDMSResPri->stDrvDMS._stXC_DIPPushInfo, 0x0, sizeof(DMS_VDECFRAME_INFO)*DMS_DIP_QUEUEDEPTH);

    _MDrv_DMS_ZOrder_Init();

    pTaskInstance = pInstance;

    _MDrv_DMS_InvalidateAllQueue();

    pDMSResPri->bDrvInit = TRUE;

    return TRUE;
}

static MS_BOOL _MDrv_DMS_SetWin(void* pInstance, MS_U32 u32Window, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    ST_DMS_WINDOW stCropWin = {0};
    ST_DMS_WINDOW stDispWin = {0};

    if(pstDMS_SetWin_Info == NULL)
    {
        DMS_PRINTF("[%s] error, Set window parameter cannot be NULL\n", __FUNCTION__);
        return FALSE;
    }

    if (u32Window >= DMS_MAX_WINDOW_NUM) {
        DMS_PRINTF("[%s] error, u32Window number exceed limit\n", __FUNCTION__);
        return FALSE;
    }

    memcpy(&pDMSResPri->stDrvDMS.stOutputWinInfo[u32Window].stCropWin ,
        &pstDMS_SetWin_Info->stOutputWinInfo.stCropWin, sizeof(stCropWin));
    memcpy(&pDMSResPri->stDrvDMS.stOutputWinInfo[u32Window].stOutputWin,
        &pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin, sizeof(stDispWin));
    memcpy(&pDMSResPri->stDrvDMS.stDisplayWin[u32Window],
        &pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin, sizeof(stDispWin));

    DMS_PRINTF("[%s] crop[%d %d %d %d] disp[%d %d %d %d]\n", __FUNCTION__ ,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32x,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32y,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32width,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stCropWin.u32height,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin.u32x,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin.u32y,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin.u32width,
        (int)pstDMS_SetWin_Info->stOutputWinInfo.stOutputWin.u32height);

    if (pstDMS_SetWin_Info->enARC != E_DMS_AR_DEFAULT) {
        // calculate ARC here

    }

    if (pstDMS_SetWin_Info->u32OnOutputLayer && pDMSResPri->stDrvDMS.stOutputLayer.u32width
        && pDMSResPri->stDrvDMS.stOutputLayer.u32height) {
        // calculate the real window to XC

        MS_U32 u32PanelWidth = g_IPanel.Width();
        MS_U32 u32PanelHeight = g_IPanel.Height();

        DMS_PRINTF("panel [%d %d] osd [%d %d] \n", (int)u32PanelWidth, (int)u32PanelHeight,
            (int)pDMSResPri->stDrvDMS.stOutputLayer.u32width,
            (int)pDMSResPri->stDrvDMS.stOutputLayer.u32height);
        if (pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32x < 0) {
            pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32x = 0;
        } else {
            pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32x =
                ((pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32x * u32PanelWidth)
                / pDMSResPri->stDrvDMS.stOutputLayer.u32width);
        }

        pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32width =
            ((pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32width * u32PanelWidth)
            / pDMSResPri->stDrvDMS.stOutputLayer.u32width);
        if (pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32width > u32PanelWidth) {
            pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32width = u32PanelWidth;
        }

        if (pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32y < 0) {
            pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32y = 0;
        } else {
            pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32y =
                ((pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32y * u32PanelHeight)
                / pDMSResPri->stDrvDMS.stOutputLayer.u32height);
        }

        pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32height =
            ((pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32height * u32PanelHeight)
            / pDMSResPri->stDrvDMS.stOutputLayer.u32height);
        if (pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32height > u32PanelHeight) {
            pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32height = u32PanelHeight;
        }

        DMS_PRINTF("apply ratio, disp_win[%d %d %d %d] crop_win[%d %d %d %d]\n",
              (int)pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32x,
              (int)pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32y,
              (int)pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32width,
              (int)pDMSResPri->stDrvDMS.stDisplayWin[u32Window].u32height,
              (int)pDMSResPri->stDrvDMS.stOutputWinInfo[u32Window].stCropWin.u32x,
              (int)pDMSResPri->stDrvDMS.stOutputWinInfo[u32Window].stCropWin.u32y,
              (int)pDMSResPri->stDrvDMS.stOutputWinInfo[u32Window].stCropWin.u32width,
              (int)pDMSResPri->stDrvDMS.stOutputWinInfo[u32Window].stCropWin.u32height);
    }

    pDMSResPri->stDrvDMS.bWindowInfoReady[u32Window] = TRUE;

    pDMSResPri->stDrvDMS.bWinChanged[u32Window] = TRUE;

    return TRUE;
}

static MS_BOOL _MDrv_DMS_Final(void* pInstance, MS_U32 u32Window)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));


    if (u32Window >= DMS_MAX_WINDOW_NUM) {
        return FALSE;
    }
    pDMSResPri->stDrvDMS.bWindowInfoReady[u32Window] = FALSE;
    pDMSResPri->stDrvDMS.bWinUsed[u32Window] = FALSE;
    // clear drv control
    memset(&gDMSCtrl[u32Window], 0, sizeof(DMS_DRV_CTRL));

    if (u32Window)
        MDrv_MVOP_SubEnableInterrupt(0);
    else
        MDrv_MVOP_EnableInterrupt(0);
    MsOS_DisableInterrupt(u32Window ? E_INT_IRQ_DCSUB : E_INT_IRQ_DC);
    MsOS_DetachInterrupt(u32Window ? E_INT_IRQ_DCSUB : E_INT_IRQ_DC);

    return TRUE;
}

EN_DMS_RESULT MDrv_DMS_Init(void* pInstance, ST_DMS_INITDATA *pstDMS_InitData)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    bRet = _MDrv_DMS_Init(pInstance);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_SetMemoryType(void* pInstance, ST_DMS_SET_MEMORY_TYPE* pstDMS_SetMemType)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    DMS_PRINTF("MDrv_DMS_SetMemoryType\n");
    bRet = _MDrv_DMS_Setup_Memory(pstDMS_SetMemType->eMemoryType, pstDMS_SetMemType->phyAddr,pstDMS_SetMemType->phySize);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_CreateWindow(void* pInstance, ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID)
{
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    MS_BOOL bRet = FALSE;

    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);

    if (*pu32WindowID >= DMS_MAX_WINDOW_NUM) {
        DMS_PRINTF("[%s %d] u32WindowID %d exceed limit\n", __FUNCTION__, __LINE__, (int)*pu32WindowID);
    }

#if 1
    pDMSResPri->stDrvDMS.bWinUsed[*pu32WindowID] = TRUE;
#else
    pDMSResPri->stDrvDMS.bWinUsed[*pu32WindowID] = TRUE;

    _DRV_DMS_MutexCreate(*pu32WindowID);
    // create monitor thread
    _MDrv_DMS_CreateTask(*pu32WindowID);
#endif

    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_SetDigitalDecodeSignalInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat)
{
    DMS_PRINTF("[%s %d] u32WindowID = %d\n", __FUNCTION__, __LINE__, (int)u32WindowID);

    if (u32WindowID >= DMS_MAX_WINDOW_NUM) {
        DMS_PRINTF("[%s %d] u32WindowID %d exceed limit\n", __FUNCTION__, __LINE__, (int)u32WindowID);
        return E_DMS_WINID_ILLEGAL;
    }

    if (pstDispFrameFormat) {
        DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
        UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

        VDEC_StreamId VdecStreamId;
        MS_PHY phyVsyncbridgeAddr, phyVsyncbridgeExtAddr;

        VdecStreamId.u32Version = pstDispFrameFormat->u32VdecStreamVersion;
        VdecStreamId.u32Id = pstDispFrameFormat->u32VdecStreamId;

        if (E_VDEC_EX_OK == MApi_VDEC_EX_GetControl(&VdecStreamId, E_VDEC_EX_USER_CMD_GET_VSYNC_BRIDGE_ADDR, (MS_U32 *)&phyVsyncbridgeAddr)) {
            DMS_PRINTF("phyVsyncbridgeAddr = 0x%x\n", (unsigned int)phyVsyncbridgeAddr);
        } else {
            DMS_PRINTF("E_VDEC_EX_USER_CMD_GET_VSYNC_BRIDGE_ADDR FAIL!!!\n");
        }

        if (E_VDEC_EX_OK == MApi_VDEC_EX_GetControl(&VdecStreamId, E_VDEC_EX_USER_CMD_GET_VSYNC_BRIDGE_EXT_ADDR, (MS_U32 *)&phyVsyncbridgeExtAddr)) {
            DMS_PRINTF("phyVsyncbridgeExtAddr = 0x%x\n", (unsigned int)phyVsyncbridgeExtAddr);
        } else {
            DMS_PRINTF("E_VDEC_EX_USER_CMD_GET_VSYNC_BRIDGE_EXT_ADDR FAIL!!!\n");
        }

        // clear drv control
        memset(&gDMSCtrl[u32WindowID], 0, sizeof(DMS_DRV_CTRL));

        pDMSResPri->stDrvDMS.bWinUsed[u32WindowID] = TRUE;

#if 1
        _DRV_DMS_MutexCreate(u32WindowID);
        // create monitor thread
        _MDrv_DMS_CreateTask(u32WindowID);
#endif

        MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
        pDMSResPri->stDrvDMS.phyVsyncbridgeAddr[u32WindowID] = phyVsyncbridgeAddr;
        pDMSResPri->stDrvDMS.phyVsyncbridgeExtAddr[u32WindowID] = phyVsyncbridgeExtAddr;


        int u8BufID;
        for(u8BufID = 0; u8BufID < DMS_DIP_QUEUEDEPTH; u8BufID++)
        {
            pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32WindowID][u8BufID] = 0;
        }

        MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);

        VDEC_EX_DynmcDispPath stDynmcDispPath;
        stDynmcDispPath.bConnect = TRUE;
        stDynmcDispPath.eMvopPath = u32WindowID ? E_VDEC_EX_DISPLAY_PATH_MVOP_SUB : E_VDEC_EX_DISPLAY_PATH_MVOP_MAIN;

        MApi_VDEC_EX_SetControl((VDEC_StreamId *)&VdecStreamId, E_VDEC_EX_USER_CMD_SET_DYNAMIC_DISP_PATH, (MS_U32)&stDynmcDispPath);

#ifdef DMS_ENABLE_MVOP_INT
        DMS_PRINTF("Attach and enable MVOP interrupt E_INT_IRQ_DC = %d\n", E_INT_IRQ_DC);
        MsOS_DisableInterrupt(u32WindowID ? E_INT_IRQ_DCSUB : E_INT_IRQ_DC);
        if (u32WindowID) {
            MDrv_MVOP_SubEnableInterrupt(0);
        } else {
            MDrv_MVOP_EnableInterrupt(0);
        }
        MsOS_AttachInterrupt(u32WindowID ? E_INT_IRQ_DCSUB : E_INT_IRQ_DC, _DMS_MVOP_ISR);
        MsOS_EnableInterrupt(u32WindowID ? E_INT_IRQ_DCSUB : E_INT_IRQ_DC);
#ifndef MSOS_TYPE_LINUX_KERNEL
        MsOS_CompleteInterrupt(u32WindowID ? E_INT_IRQ_DCSUB : E_INT_IRQ_DC);
#endif

        if (u32WindowID) {
            MDrv_MVOP_SubEnableInterrupt(E_MVOP_INT_VSYNC);
        } else {
            MDrv_MVOP_EnableInterrupt(E_MVOP_INT_VSYNC);
        }
#endif
    }

    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_ClearDigitalDecodeSignalInfo(void* pInstance, MS_U32 u32WindowID)
{
    //DMS_PRINTF("\033[32m [%s][%d] === Not Impl %d=== \033[0m\n",__FUNCTION__,__LINE__, (int)u32WindowID);
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    bRet = _MDrv_DMS_Final(pInstance, u32WindowID);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
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

EN_DMS_RESULT MDrv_DMS_SetWindow(void* pInstance, MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info)
{
    MS_BOOL bRet = FALSE;
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    bRet = _MDrv_DMS_SetWin(pInstance, u32WindowID, pstDMS_SetWin_Info);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return (bRet)? E_DMS_OK : E_DMS_FAIL;
}

EN_DMS_RESULT MDrv_DMS_GetWindowInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info)
{
    return E_DMS_OK;
}

E_DMS_VIDEO_SCAN_TYPE _DMS_GetDMSScanType(MS_U8 u8ScanType)
{
    // ProgressiveFrame = 0,   /** Picture is a progressive frame */
    // InterlacedFrame  = 2,    /** Picture is an interlaced frame */
    // InterlacedField  = 3,      /** Picture is two interlaced fields */
    // InterlacedField  = 1,      MVD bring 1 to push interlace frame
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

EN_DMS_RESULT MDrv_DMS_Video_Flip(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat)
{
    EN_DMS_RESULT eResult = E_DMS_FAIL;

    MCU_DISPQ_INFO *pSHM = 0;
    if (pstDispFrameFormat->u32OverlayID < 2) {
        pSHM = (MCU_DISPQ_INFO *)MsOS_PA2KSEG1(pstDispFrameFormat->phyVsyncBridgeAddr +
            ((MS_U32)pstDispFrameFormat->u32OverlayID * sizeof(MCU_DISPQ_INFO)));
    } else {
        pSHM = (MCU_DISPQ_INFO *)MsOS_PA2KSEG1(pstDispFrameFormat->phyVsyncBridgeAddr);
    }

    MS_U32 lumaAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr;
    MS_U32 chromAddr = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr;
    MS_U32 lumaAddr2 = pstDispFrameFormat->stFrames[1].stHWFormat.phyLumaAddr;
    MS_U32 chromAddr2 = pstDispFrameFormat->stFrames[1].stHWFormat.phyChromaAddr;
    MS_U16 u16Width = pstDispFrameFormat->stFrames[0].u32Width
                      - pstDispFrameFormat->stFrames[0].u32CropLeft
                      - pstDispFrameFormat->stFrames[0].u32CropRight;
    MS_U16 u16Height = pstDispFrameFormat->stFrames[0].u32Height
                       - pstDispFrameFormat->stFrames[0].u32CropTop
                       - pstDispFrameFormat->stFrames[0].u32CropBottom;


    if (u32WindowID >= DMS_MAX_WINDOW_NUM) {
        DMS_PRINTF("u32WindowID = %d exceed limit\n", (int)u32WindowID);
        return E_DMS_WINSIZE_ERROR;
    }

    if (pstDispFrameFormat->u8MCUMode) {
        if (bMsLogON) {
            DMS_PRINTF("MCU render_frame [%d] pts = %lld, luma=0x%x, chroma=0x%x, B_F = %d, Time = %d, Interlace = %d, Frc = %d\n",
                  (int)pstDispFrameFormat->u32OverlayID, pstDispFrameFormat->u64Pts,
                  (unsigned int)lumaAddr, (unsigned int)chromAddr, pstDispFrameFormat->u8BottomFieldFirst,
                  pstDispFrameFormat->u8ToggleTime, pstDispFrameFormat->u8Interlace, pstDispFrameFormat->u8FrcMode);
        }

        if (u32WindowID == 0) {
            MDrv_MVOP_SetBaseAdd(lumaAddr, chromAddr, 1, 0);
        } else {
            MDrv_MVOP_SubSetBaseAdd(lumaAddr, chromAddr, 1, 0);
        }
        return eResult;
    }

    if (pSHM) {

        MS_U8 u8WIndex = pSHM->u8McuDispQWPtr;
        MS_U32 u32Cnt = 10;

        _DRV_DMS_Entry(u32WindowID);

#if 1 //def DYNAMIC_DISP_PATH    // TODO
        VDEC_EX_DynmcDispPath stDynmcDispPath;
        VDEC_StreamId VdecStreamId;

        if (u32WindowID == 0) {
            if (gDMSCtrl[u32WindowID].lastdff.u32OverlayID != pstDispFrameFormat->u32OverlayID) {
                if (gDMSCtrl[u32WindowID].CodecType != pstDispFrameFormat->u32CodecType) {
                    //bVideoMute = TRUE;
                    //Wrapper_Video_setMute(Overlay_ID, 1);
                    // will seq_change latter
                }
                VdecStreamId.u32Version = gDMSCtrl[u32WindowID].lastdff.u32VdecStreamVersion;
                VdecStreamId.u32Id = gDMSCtrl[u32WindowID].lastdff.u32VdecStreamId;
                stDynmcDispPath.bConnect = FALSE;
                stDynmcDispPath.eMvopPath = E_VDEC_EX_DISPLAY_PATH_MVOP_MAIN;
                MApi_VDEC_EX_SetControl((VDEC_StreamId *)&VdecStreamId, E_VDEC_EX_USER_CMD_SET_DYNAMIC_DISP_PATH,(MS_U32)&stDynmcDispPath);

                VdecStreamId.u32Version = pstDispFrameFormat->u32VdecStreamVersion;
                VdecStreamId.u32Id = pstDispFrameFormat->u32VdecStreamId;
                stDynmcDispPath.bConnect = TRUE;
                MApi_VDEC_EX_SetControl((VDEC_StreamId *)&VdecStreamId, E_VDEC_EX_USER_CMD_SET_DYNAMIC_DISP_PATH, (MS_U32)&stDynmcDispPath);
                //bStreamChange = TRUE;
            }
        }
#endif

        if (pSHM->u8McuDispSwitch == 0) {
            // fill ctrl flags

            DMS_PRINTF("VSYNC_BRIDGE_INIT clear dispQ\n");
            memset(pSHM, 0, sizeof(MCU_DISPQ_INFO));

            u8WIndex = DISPQ_FIRST_R_INDEX - 1;
            gDMSCtrl[u32WindowID].u16SrcWidth = pstDispFrameFormat->stFrames[0].u32Width;
            gDMSCtrl[u32WindowID].u16SrcHeight = pstDispFrameFormat->stFrames[0].u32Height;
            gDMSCtrl[u32WindowID].u16FrameRate = (MS_U16)pstDispFrameFormat->u32FrameRate;
            gDMSCtrl[u32WindowID].CodecType = pstDispFrameFormat->u32CodecType;
            gDMSCtrl[u32WindowID].u8Interlace = pstDispFrameFormat->u8Interlace;
            gDMSCtrl[u32WindowID].u32AspectWidth = pstDispFrameFormat->u32AspectWidth;
            gDMSCtrl[u32WindowID].u32AspectHeight = pstDispFrameFormat->u32AspectHeight;

            pSHM->u2MirrorMode =  gDMSCtrl[u32WindowID].u8MirrorMode;

#if ENABLE_FRAME_CONVERT
            gDMSCtrl[u32WindowID].u8UpdateFrcMode = 1;
#endif
        }

        if (gDMSCtrl[u32WindowID].u16FrameRate != (MS_U16)pstDispFrameFormat->u32FrameRate) {
            gDMSCtrl[u32WindowID].u8UpdateFrcMode = 1;
            gDMSCtrl[u32WindowID].u16FrameRate = pstDispFrameFormat->u32FrameRate;
        }

#if ENABLE_FRAME_CONVERT
        if (gDMSCtrl[u32WindowID].u8UpdateFrcMode) {
            _DMS_Update_SW_FRC_Mode(u32WindowID, pstDispFrameFormat);
            gDMSCtrl[u32WindowID].u8UpdateFrcMode = 0;
        }
#endif

        if (bMsLogON)
            DMS_PRINTF("dms render_frame [%d] pts = %lld, luma=0x%x, chroma=0x%x, Interlace = %d, B_F = %d, TG = %d, Last_TG = %d\n",
                  (int)pstDispFrameFormat->u32OverlayID, pstDispFrameFormat->u64Pts,
                  (unsigned int)lumaAddr, (unsigned int)chromAddr, pstDispFrameFormat->u8Interlace,
                  pstDispFrameFormat->u8BottomFieldFirst, pstDispFrameFormat->u8ToggleTime,
                  pSHM->McuDispQueue[pSHM->u8McuDispQWPtr].u8Tog_Time);

        DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
        UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
        while (pDMSResPri->stDrvDMS._stXC_LocalFrameRefCount[u32WindowID][((u8WIndex + 1) % MAX_VSYNC_BRIDGE_DISPQ_NUM)]
            && u32Cnt--) {
        //while ((((u8WIndex + 1) % MAX_VSYNC_BRIDGE_DISPQ_NUM) == pSHM->u8McuDispQRPtr) && u32Cnt--) {
            _DRV_DMS_Release(u32WindowID);
            #if 0
            return E_DMS_OK;
            #else
            if (bMsLogON)
            DMS_PRINTF("DMS Display Queue Full !! Waiting... [u32SHMAddr = 0x%x]\n", (int)pstDispFrameFormat->phyVsyncBridgeAddr);
            _DMS_Wait_MVOP_Vsync(u32WindowID, pstDispFrameFormat);
            _DRV_DMS_Entry(u32WindowID);
            #endif
        }

        u8WIndex = (u8WIndex + 1)%MAX_VSYNC_BRIDGE_DISPQ_NUM;

        pSHM->McuDispQueue[u8WIndex].u32LumaAddr0 = lumaAddr;
        pSHM->McuDispQueue[u8WIndex].u32ChromaAddr0 = chromAddr;


        if (pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_MVC) {
            pSHM->McuDispQueue[u8WIndex].u16Width = (u16Width + 1) & ~1;
            pSHM->McuDispQueue[u8WIndex].u16Height = u16Height;
        } else {
            pSHM->McuDispQueue[u8WIndex].u16Width = (pstDispFrameFormat->stFrames[0].u32Width + 1) & ~1;
            pSHM->McuDispQueue[u8WIndex].u16Height = pstDispFrameFormat->stFrames[0].u32Height;
        }

        pSHM->McuDispQueue[u8WIndex].u16Pitch = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch;
        //pSHM->McuDispQueue[u8WIndex].u32CodecType = pstDispFrameFormat->u32CodecType;
        pSHM->McuDispQueue[u8WIndex].u8Interlace = pstDispFrameFormat->u8Interlace;

        if (gDMSCtrl[u32WindowID].u8FieldCtrl != pstDispFrameFormat->u8FieldCtrl) {
            if (u32WindowID) {
                // FIXME: wait MVOP implement sub API
                //MDrv_MVOP_SubSEL_OP_FIELD(TRUE);
            } else {
                DMS_PRINTF("MDrv_MVOP_SEL_OP_FIELD = 0x%d\n", (pstDispFrameFormat->u8FieldCtrl ? TRUE : FALSE));
                MDrv_MVOP_SEL_OP_FIELD(pstDispFrameFormat->u8FieldCtrl ? TRUE : FALSE);
            }
            // enable BOB mode to prevent FF/RF playback De-interlace garbage.
            DMS_PRINTF("MApi_XC_SetBOBMode = %d\n", (pstDispFrameFormat->u8FieldCtrl ? TRUE : FALSE));
            MApi_XC_SetBOBMode(pstDispFrameFormat->u8FieldCtrl ? TRUE : FALSE, u32WindowID);
            gDMSCtrl[u32WindowID].u8FieldCtrl = pstDispFrameFormat->u8FieldCtrl;
        }

        pSHM->McuDispQueue[u8WIndex].u8FieldCtrl = pstDispFrameFormat->u8FieldCtrl;

        if (pstDispFrameFormat->enColorFormat == E_DMS_COLOR_FORMAT_YUYV) {
            pSHM->McuDispQueue[u8WIndex].u8ColorFormat = 1; //422
        } else if (pstDispFrameFormat->enColorFormat == E_DMS_COLOR_FORMAT_10BIT_TILE) {

            pSHM->McuDispQueue[u8WIndex].u8ColorFormat = 2; //420 10 bits
            pSHM->McuDispQueue[u8WIndex].u16Pitch1 = pstDispFrameFormat->stFrames[0].stHWFormat.u32LumaPitch2Bit;
            pSHM->McuDispQueue[u8WIndex].u8RangeMapY = pstDispFrameFormat->stFrames[0].u8LumaBitdepth;
            pSHM->McuDispQueue[u8WIndex].u8RangeMapUV = pstDispFrameFormat->stFrames[0].u8ChromaBitdepth;
            pSHM->McuDispQueue[u8WIndex].u32LumaAddr1 = pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr2Bit;
            pSHM->McuDispQueue[u8WIndex].u32ChromaAddr1 = pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr2Bit;
        } else {
            pSHM->McuDispQueue[u8WIndex].u8ColorFormat = 0; //420
        }

        //if (u8Interlace[Overlay_ID] != dff->u8Interlace) {
        //    DMS_PRINTF("WARNING [%d] pts = %lld, luma=0x%x, chroma=0x%x, BoT_First = %d, TogTime = %d, Interlace = %d",
        //          dff->OverlayID, dff->u64Pts, (unsigned int)lumaAddr, (unsigned int)chromAddr, dff->u8BottomFieldFirst,
        //          dff->u8ToggleTime, dff->u8Interlace);
        //}

        if (pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_VC1_ADV
            || pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_VC1_MAIN) {
            pSHM->McuDispQueue[u8WIndex].u8RangeMapY = pstDispFrameFormat->stFrames[0].u8LumaBitdepth;
            pSHM->McuDispQueue[u8WIndex].u8RangeMapUV = pstDispFrameFormat->stFrames[0].u8ChromaBitdepth;
        }

        if (pstDispFrameFormat->u32CodecType == E_DMS_CODEC_TYPE_MVC) {
            pSHM->McuDispQueue[u8WIndex].u8FrameNum = 2;
            if ((gDMSCtrl[u32WindowID].u8MirrorMode == (MS_U8)E_VOPMIRROR_HVBOTH) && (pstDispFrameFormat->u83DMode == E_DMS_3DMODE_SIDEBYSIDE)) {
                // MVOP mirror and MVC side by side, swap L R
                pSHM->McuDispQueue[u8WIndex].u32LumaAddr0 = lumaAddr2;
                pSHM->McuDispQueue[u8WIndex].u32ChromaAddr0 = chromAddr2;
                pSHM->McuDispQueue[u8WIndex].u32LumaAddr1 = lumaAddr;
                pSHM->McuDispQueue[u8WIndex].u32ChromaAddr1 = chromAddr;
            } else {
                pSHM->McuDispQueue[u8WIndex].u32LumaAddr1 = lumaAddr2;
                pSHM->McuDispQueue[u8WIndex].u32ChromaAddr1 = chromAddr2;
            }
        }

        if (pstDispFrameFormat->u8ToggleTime == 0) {
            pSHM->McuDispQueue[u8WIndex].u8TB_toggle = 0;
            pSHM->McuDispQueue[u8WIndex].u8Tog_Time = 1;

        } else if (pstDispFrameFormat->u8Interlace) {   //interlace
            pSHM->McuDispQueue[u8WIndex].u8TB_toggle = pstDispFrameFormat->u8BottomFieldFirst ? 1 : 0;    //0, TOP then BOTTOM, 1 BOT then TOP
            pSHM->McuDispQueue[u8WIndex].u8Tog_Time = pstDispFrameFormat->u8ToggleTime;

            if ((pstDispFrameFormat->u8ToggleTime == 1)
                && (pstDispFrameFormat->stFrames[0].enFieldType == E_DMS_FIELD_TYPE_BOTH)) {
                // prevent de-interlace wrong...
                pSHM->McuDispQueue[u8WIndex].u8Tog_Time = 2;
            }
        } else {
                pSHM->McuDispQueue[u8WIndex].u8TB_toggle = 0;
                pSHM->McuDispQueue[u8WIndex].u8Tog_Time = 1;
        }

        pSHM->McuDispQueue[u8WIndex].u1BottomFieldFirst = pstDispFrameFormat->u8BottomFieldFirst;

        MS_PHY phyTemp;
        _phy_to_miu_offset(pSHM->McuDispQueue[u8WIndex].u2Luma0Miu, phyTemp, pstDispFrameFormat->stFrames[0].stHWFormat.phyLumaAddr);
        _phy_to_miu_offset(pSHM->McuDispQueue[u8WIndex].u2Chroma0Miu, phyTemp, pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr);
        _phy_to_miu_offset(pSHM->McuDispQueue[u8WIndex].u2Luma1Miu, phyTemp, pstDispFrameFormat->stFrames[1].stHWFormat.phyLumaAddr);
        _phy_to_miu_offset(pSHM->McuDispQueue[u8WIndex].u2Chroma1Miu, phyTemp, pstDispFrameFormat->stFrames[0].stHWFormat.phyChromaAddr);

#if ENABLE_FRAME_CONVERT
        pSHM->u5FRCMode = gDMSCtrl[u32WindowID].u8FrcMode;
#endif

        // TODO: implement force P
        //....

        if (pstDispFrameFormat->phyVsyncBridgeExtAddr) {
            // has vsync_bridge EXT addr
            MCU_DISPQ_INFO_EXT *pDispQ_Ext = 0;//kk

            if (pstDispFrameFormat->u32OverlayID < 2) {
                pDispQ_Ext = (MCU_DISPQ_INFO_EXT *)MsOS_PA2KSEG1(pstDispFrameFormat->phyVsyncBridgeExtAddr +
                    ((MS_U32)pstDispFrameFormat->u32OverlayID * sizeof(MCU_DISPQ_INFO_EXT)));
            } else {
                pDispQ_Ext = (MCU_DISPQ_INFO_EXT *)MsOS_PA2KSEG1(pstDispFrameFormat->phyVsyncBridgeExtAddr);
            }

            if (pDispQ_Ext /*&& (pDispQ_Ext->u32Version <= VSYNC_BRIDGE_EXT_VERSION)*/) {
                // vsync bridge extened header --> vbeh magic number
                pDispQ_Ext->u8Pattern[0] = 'v';
                pDispQ_Ext->u8Pattern[1] = 'b';
                pDispQ_Ext->u8Pattern[2] = 'e';
                pDispQ_Ext->u8Pattern[3] = 'h';
                pDispQ_Ext->u32Version = 0;
                #if 1
                int i = 0;
                for (i = 0; i < MS_DISP_FRM_INFO_EXT_TYPE_MAX; i++) {
                    pDispQ_Ext->McuDispQueue[u8WIndex].u32LumaAddrExt[i] = pstDispFrameFormat->stDispFrmInfoExt.u32LumaAddrExt[i];
                    pDispQ_Ext->McuDispQueue[u8WIndex].u32ChromaAddrExt[i] = pstDispFrameFormat->stDispFrmInfoExt.u32ChromaAddrExt[i];
                }
                pDispQ_Ext->McuDispQueue[u8WIndex].MFCodecInfo = pstDispFrameFormat->stFrames[0].stHWFormat.u32MFCodecInfo;
                pDispQ_Ext->McuDispQueue[u8WIndex].u16Width = pstDispFrameFormat->stDispFrmInfoExt.u16Width;
                pDispQ_Ext->McuDispQueue[u8WIndex].u16Height = pstDispFrameFormat->stDispFrmInfoExt.u16Height;
                pDispQ_Ext->McuDispQueue[u8WIndex].u16Pitch[0] = pstDispFrameFormat->stDispFrmInfoExt.u16Pitch[0];
                pDispQ_Ext->McuDispQueue[u8WIndex].u16Pitch[1] = pstDispFrameFormat->stDispFrmInfoExt.u16Pitch[1];
                #else
                memcpy(&pDispQ_Ext->McuDispQueue[u8WIndex], &pstDispFrameFormat->stDispFrmInfoExt, sizeof(DISP_FRM_INFO_EXT));
                #endif
            } else {
                DMS_PRINTF("pDispQ_Ext is NULL\n");
            }
        }


        // TODO: handle DS
        //if (gDMSCtrl[u32WindowID].bDS_Enable) {
        //if (u32WindowID ? MApi_XC_GetDynamicScalingStatus() : MApi_XC_GetDynamicScalingStatus()) {
        if (MApi_XC_GetDynamicScalingStatus()) {

            // source resolution change
            if ((gDMSCtrl[u32WindowID].u16SrcWidth != pstDispFrameFormat->stFrames[0].u32Width)
                || (gDMSCtrl[u32WindowID].u16SrcHeight != pstDispFrameFormat->stFrames[0].u32Height)
                || (gDMSCtrl[u32WindowID].u32AspectWidth != pstDispFrameFormat->u32AspectWidth
                    && gDMSCtrl[u32WindowID].u32AspectHeight != pstDispFrameFormat->u32AspectHeight)) {
                gDMSCtrl[u32WindowID].u32AspectWidth = pstDispFrameFormat->u32AspectWidth;
                gDMSCtrl[u32WindowID].u32AspectHeight = pstDispFrameFormat->u32AspectHeight;
                gDMSCtrl[u32WindowID].u16SrcWidth = pstDispFrameFormat->stFrames[0].u32Width;
                gDMSCtrl[u32WindowID].u16SrcHeight = pstDispFrameFormat->stFrames[0].u32Height;
                gDMSCtrl[u32WindowID].u8SizeChange = 1;

                #ifdef UFO_SWDS_NR_RESOLUTION_CHANGE
                MApi_XC_Set_DS_DNR(DISABLE, Overlay_ID);
                #endif
                gDMSCtrl[u32WindowID].u8DNRFrameCnt = 0;
            }

            if (!pSHM->u8McuDispSwitch
                || gDMSCtrl[u32WindowID].u8SizeChange
#ifdef UFO_XC_SWDS_DYMAMIC_CONTROL_DNR
                || (gDMSCtrl[u32WindowID].u8DNRFrameCnt == DNRFrameNUM)
#endif
                || _DMS_Need_XC_HDR_DS(pstDispFrameFormat)) {

                _DMS_Update_DS_Table(pInstance, u32WindowID, pstDispFrameFormat, 0,
                    !pSHM->u8McuDispSwitch || gDMSCtrl[u32WindowID].u8SizeChange
#ifndef UFO_XC_SETWINDOW_LITE
                    || _DMS_Need_XC_HDR_DS(pstDispFrameFormat)
#endif
#ifdef UFO_XC_SWDS_DYMAMIC_CONTROL_DNR
                    || (gDMSCtrl[u32WindowID].u8DNRFrameCnt == DNRFrameNUM)
#endif
                );

                gDMSCtrl[u32WindowID].u8SizeChange = 0;
            }

#ifdef UFO_XC_SWDS_SW_FILM_ENABLE
            if ((u8ForceInterlace[Overlay_ID] || (u8ForcePMode[Overlay_ID] && dff->u8Interlace))
                && MApi_XC_Set_DS_ForceI_DeInterlaceMode) {
                pSHM->McuDispQueue[u8WIndex].u1DSIndex1Valid = 1;
            }
#endif

            pSHM->McuDispQueue[u8WIndex].u8DSIndex = gDMSCtrl[u32WindowID].ds_curr_index.u8DSIndex;
        } else {
            gDMSCtrl[u32WindowID].u8SizeChange = 1;
        }

        pSHM->McuDispQueue[u8WIndex].u16CropLeft = gDMSCtrl[u32WindowID].stCurrentCropWin.u32x;
        pSHM->McuDispQueue[u8WIndex].u16CropTop = gDMSCtrl[u32WindowID].stCurrentCropWin.u32y;
        pSHM->McuDispQueue[u8WIndex].u16CropRight = pSHM->McuDispQueue[u8WIndex].u16Width
                                                    - gDMSCtrl[u32WindowID].stCurrentCropWin.u32width
                                                    - gDMSCtrl[u32WindowID].stCurrentCropWin.u32x;
        pSHM->McuDispQueue[u8WIndex].u16CropBottom = pSHM->McuDispQueue[u8WIndex].u16Height
                                                    - gDMSCtrl[u32WindowID].stCurrentCropWin.u32height
                                                    - gDMSCtrl[u32WindowID].stCurrentCropWin.u32y;

        // if render interval large than 500 ms, maybe pause, needs to wait timing
        if (pSHM->u8McuDispSwitch && ((MsOS_GetSystemTime() - gDMSCtrl[u32WindowID].u32LastTime) > 500)) {
            _DMS_Wait_MVOP_Vsync(u32WindowID, pstDispFrameFormat);
        }
        gDMSCtrl[u32WindowID].u32LastTime = MsOS_GetSystemTime();

        pSHM->u8McuDispQWPtr = u8WIndex;

        //pstDispFrameFormat->u8CurIndex = u8WIndex;

        if (pSHM->u8McuDispSwitch == 0) {
            pSHM->u8DispQueNum = MAX_VSYNC_BRIDGE_DISPQ_NUM;

            _DMS_Wait_MVOP_Vsync(u32WindowID, pstDispFrameFormat);

            pSHM->u8McuDispSwitch = 1;
        }
        gDMSCtrl[u32WindowID].u32FrameCount++;
        if (pstDispFrameFormat != &gDMSCtrl[u32WindowID].lastdff)
            memcpy(&gDMSCtrl[u32WindowID].lastdff, pstDispFrameFormat, sizeof(ST_DMS_DISPFRAMEFORMAT));

        MsOS_FlushMemory();

        if (gDMSCtrl[u32WindowID].u8Freeze) {
            gDMSCtrl[u32WindowID].u8Freeze--;
            if (gDMSCtrl[u32WindowID].u8Freeze == 0) {
                DMS_PRINTF("[%d]MApi_XC_UnFreezeImg\n", (int)u32WindowID);
                MApi_XC_FreezeImg(FALSE, (SCALER_WIN)(u32WindowID ? 1 : 0));
            }
        }

        _DRV_DMS_Release(u32WindowID);

        {
            MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
            DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
            UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
            pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32WindowID][u8WIndex].u32VDECStreamVersion = pstDispFrameFormat->u32VdecStreamVersion;
            pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32WindowID][u8WIndex].u32VDECStreamID = pstDispFrameFormat->u32VdecStreamId;
            pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32WindowID][u8WIndex].u32FrameIndex = pstDispFrameFormat->stFrames[0].u32Idx;
            pDMSResPri->stDrvDMS._stXC_DIPPushInfo[u32WindowID][u8WIndex].u32PriData = pstDispFrameFormat->stFrames[0].u32PriData;
            pDMSResPri->stDrvDMS.phyVsyncbridgeAddr[u32WindowID] = pstDispFrameFormat->phyVsyncBridgeAddr;
            pDMSResPri->stDrvDMS.phyVsyncbridgeExtAddr[u32WindowID] = pstDispFrameFormat->phyVsyncBridgeExtAddr;
            _DMS_VDEC_Frame_AddRef(u32WindowID, u8WIndex);
            MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
        }

        eResult = E_DMS_OK;
    } else {
        DMS_PRINTF("%s %d pSHM is NULL\n", __FUNCTION__, __LINE__);
        eResult = E_DMS_FAIL;
    }

    return eResult;
}

EN_DMS_RESULT MDrv_DMS_SetZOrder(void* pInstance, MS_U32 u32WindowID, MS_U32 u32ZOrder)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));

    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Video_Freeze(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    DMS_PRINTF("\033[32m [%s][%d] === u32Enable : %d === \033[0m\n", __FUNCTION__, __LINE__, (int)u32Enable);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return TRUE;
}

EN_DMS_RESULT MDrv_DMS_Video_Mute(void* pInstance, MS_U32 u32WindowID, MS_U32 u32Enable)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_RESOURCE_PRIVATE* pDMSResPri = NULL;
    UtopiaResourceGetPrivate(g_pDMSRes,(void**)(&pDMSResPri));
    //Hal_DMS_XC_SetMute(pInstance, u32Enable);
    MDrv_DMS_Release_Semaphore(pInstance, E_DMS_ID_VAR);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Set_MuteColor(void* pInstance, MS_U32 u32WindowID, ST_DMS_COLOR stMuteColor)
{
    DMS_PRINTF("\033[32m [%s][%d] === Not Impl %d=== \033[0m\n", __FUNCTION__, __LINE__, (int)u32WindowID);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_GetStatus(void* pInstance, MS_U32 *pu32Status)
{
    DMS_PRINTF("\033[32m [%s][%d] === Not Impl === \033[0m\n", __FUNCTION__, __LINE__);
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_GetCapability(void* pInstance, EN_DMS_CAPABILITY *peCapability)
{
    *peCapability = E_DMS_CAPABILITY_VSYNC_BRIDGE;
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_SetOutputLayer(void* pInstance, ST_DMS_WINDOW *pstLayer)
{
    MDrv_DMS_Get_Semaphore(pInstance, E_DMS_ID_VAR);
    DMS_GET_RES_PRI;
    memcpy(&RES_DMS.stOutputLayer, pstLayer,sizeof(ST_DMS_WINDOW));
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


EN_DMS_RESULT MDrv_DMS_Set_CaptureInfo(void* pInstance, MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo)
{
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Get_CaptureBuffer(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    return E_DMS_OK;
}

EN_DMS_RESULT MDrv_DMS_Release_CaptureBuffer(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo)
{
    return E_DMS_OK;
}
