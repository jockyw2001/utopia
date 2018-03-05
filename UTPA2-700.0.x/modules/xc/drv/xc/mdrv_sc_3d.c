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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    Mdrv_sc_3d.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define DRV_SC_3D_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#endif
#include "UFO.h"
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
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
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drv_sc_ip.h"

#include "mhal_pip.h"
#include "mhal_mux.h"
#include "mhal_ip.h"

#include "halCHIP.h"
#include "drv_sc_scaling.h"
#include "xc_hwreg_utility2.h"
#include "mhal_hdmi.h"
#include "mdrv_sc_dynamicscaling.h"
#include "mhal_dynamicscaling.h"
#include "mhal_menuload.h"

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#ifndef DISABLE_3D_FUNCTION
//----------------------------------------
//--------3D macro define-----------------
//----------------------------------------

#define MS_IPM_BASE0(pInstance, eWin) (MDrv_XC_GetIPMBase(pInstance, 0, eWin))
#define MS_IPM_BASE1(pInstance, eWin) (MDrv_XC_GetIPMBase(pInstance, 1, eWin))

#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MED
#define MED(a,b,c)  (a+b+c-MAX3(a,b,c)-MIN3(a,b,c))
#endif
#define ABS(x)                    (((x) > 0) ? (x) : (-(x)))

#ifdef  MSOS_TYPE_LINUX
#include <assert.h>
#else
#define assert(b)   \
    do {    \
        if (!(b)){  \
            printf("assert at [%s][%d] \n", __FUNCTION__, __LINE__);   \
            *((int*)0) = 0; \
        }   \
    } while(0)
#endif

#ifdef ANDROID
#include <cutils/log.h>
#ifndef LOGE
#define printf(format, args...)          ALOGE(format, ##args);
#else
#define printf(format, args...)          LOGE(format, ##args);
#endif
#endif

#define WIDTH_4K2KTIMING     (3840)
#define HEIGHT_4K2KTIMING    (2160)
#define OFFSET_4K2KTIMING    (10)

//hw 2d to 3d macro
#define HW_2DTO3D_DEFAULT_CONCAVE                       0x0006
#define HW_2DTO3D_DEFAULT_ARTIFICIALGAIN                0x0000
#define HW_2DTO3D_DEFAULT_GAIN                          0x001E
#if (HW_2DTO3D_VER >= 4)
#define HW_2DTO3D_DEFAULT_OFFSET                        0x0080
#else
#define HW_2DTO3D_DEFAULT_OFFSET                        0x00FE
#endif
#define HW_2DTO3D_DEFAULT_ELESEL                        0x0002
#define HW_2DTO3D_DEFAULT_MODSEL                        0x0003
#define HW_2DTO3D_DEFAULT_EDGEBLACKWIDTH                0x0000

//detect nine lattice macro
#define DETECTNL_PIXEL_STEP                    16
#define DETECTNL_COLORTHRESHOLD                63
#define DETECTNL_TRY_TIMES                     4
#define DETECTNL_NON_VALUE                     255
#define DETECTNL_LINEAR_DIFF                   32
#define DETECTNL_COMPUTE_TIME                  0
#define DETECTNL_LATTICE_NUM                   9

//detect 3d format macro
#define DETECT3DFORMAT_DEFAULT_HORSEARCHRANGE           60
#define DETECT3DFORMAT_DEFAULT_VERSEARCHRANGE           1
#define DETECT3DFORMAT_DEFAULT_GYPIXELTHRESHOLD         14
#define DETECT3DFORMAT_DEFAULT_RCRPIXELTHRESHOLD        14
#define DETECT3DFORMAT_DEFAULT_BCBPIXELTHRESHOLD        14
#define DETECT3DFORMAT_DEFAULT_HORSAMPLECOUNT           8
#define DETECT3DFORMAT_DEFAULT_VERSAMPLECOUNT           8
#define DETECT3DFORMAT_DEFAULT_MAXCHECKINGFRAMECOUNT    1
#define DETECT3DFORMAT_DEFAULT_HITPIXELPERCENTAGE       85
#define DETECT3DFORMAT_DEFAULT_HITFRAMEPERCENTAGE       06
#define DETECT3DFORMAT_DEFAULT_HORPUREPIXEL             10
#define DETECT3DFORMAT_DEFAULT_VERPUREPIXEL             10
#define DETECT3DFORMAT_DEFAULT_PUREPIXELPERCENTAGE      85

#ifndef SUPPORT_SC0_SUB_WIN
#define SUPPORT_SC0_SUB_WIN        TRUE
#endif
#ifndef HW_6TAP_MODE_SUPPORT
#define HW_6TAP_MODE_SUPPORT        TRUE
#endif

#define ENABLE_T3D_DEBUG        FALSE //debug 2Dto3D and auto detect 3D format
//--------------------------------------------
//--------3D struct define-----------------
//--------------------------------------------

//--------------------------------------------
//--------3D static variables-----------------
//--------------------------------------------

//----------------------------------------
//--------3D internal interface-----------
//----------------------------------------
void MDrv_SC_3D_Variable_Init(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW]   = E_XC_3D_INPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._eInput3DMode[SUB_WINDOW]    = E_XC_3D_INPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._eOutput3DMode               = E_XC_3D_OUTPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._e3DPanelType                = E_XC_3D_PANEL_NONE;
    pXCResourcePrivate->stdrvXC_3D._b3DMainFirst                = TRUE;
    pXCResourcePrivate->stdrvXC_3D._u163DHShift = 0;
    pXCResourcePrivate->stdrvXC_3D._ePreUsedInput3DMode[MAIN_WINDOW]   = E_XC_3D_INPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._ePreUsedInput3DMode[SUB_WINDOW]    = E_XC_3D_INPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._ePreUsedOutput3DMode               = E_XC_3D_OUTPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._bNeedRestore3DFormat               = FALSE;
#if HW_2DTO3D_SUPPORT
    pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf = 0;
    pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf = 0;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u32Hw2dTo3dPara_Version = HW2DTO3DPARA_VERSION;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Concave = HW_2DTO3D_DEFAULT_CONCAVE;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ArtificialGain = HW_2DTO3D_DEFAULT_ARTIFICIALGAIN;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain = HW_2DTO3D_DEFAULT_GAIN;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset = HW_2DTO3D_DEFAULT_OFFSET;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel = HW_2DTO3D_DEFAULT_ELESEL;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ModSel = HW_2DTO3D_DEFAULT_MODSEL;
    pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EdgeBlackWidth = HW_2DTO3D_DEFAULT_EDGEBLACKWIDTH;
    pXCResourcePrivate->stdrvXC_3D._bIsHW2Dto3DPatchEnabled = FALSE;
#endif
#if (HW_DESIGN_3D_VER < 2)
    pXCResourcePrivate->stdrvXC_3D._bFrmpackToLinealterOpt      = FALSE;
#endif
    pXCResourcePrivate->stdrvXC_3D._bIsLR_Sbs2Line = FALSE;
    pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag = FALSE;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u32Detect3DFormatPara_Version = DETECT3DFORMATPARA_VERSION;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange             = DETECT3DFORMAT_DEFAULT_HORSEARCHRANGE;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSearchRange             = DETECT3DFORMAT_DEFAULT_VERSEARCHRANGE;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16GYPixelThreshold           = DETECT3DFORMAT_DEFAULT_GYPIXELTHRESHOLD;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16RCrPixelThreshold          = DETECT3DFORMAT_DEFAULT_RCRPIXELTHRESHOLD;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16BCbPixelThreshold          = DETECT3DFORMAT_DEFAULT_BCBPIXELTHRESHOLD;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount             = DETECT3DFORMAT_DEFAULT_HORSAMPLECOUNT;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount             = DETECT3DFORMAT_DEFAULT_VERSAMPLECOUNT;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount      = DETECT3DFORMAT_DEFAULT_MAXCHECKINGFRAMECOUNT;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HitPixelPercentage         = DETECT3DFORMAT_DEFAULT_HITPIXELPERCENTAGE;
    pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.bEnableOverscan               = TRUE;
    pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u32FPInfoPara_Version = XC_3D_FPINFOPARA_VERSION;
    pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting = FALSE;
    pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank = 0;
    pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D    =  TRUE;
    pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod     =  E_XC_3D_AUTODETECT_HW_COMPATIBLE;
    if(pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod == E_XC_3D_AUTODETECT_HW)
    {
        pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D =  pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D =  FALSE;
    }

    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes       =  E_XC_3D_INPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._bResetFrameCount       =  FALSE;
    pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount   =  0;
    pXCResourcePrivate->stdrvXC_3D._bDoubleAlign           = FALSE;
    pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank = TRUE;
    pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA           = FALSE;
    pXCResourcePrivate->stdrvXC_3D._u16DDHSize = 0; //DD H size
    pXCResourcePrivate->stdrvXC_3D._u16DDVSize = 0; //DD V size
    pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;//FRC 3D input format
    pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;//FRC 3D output format
    pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
    pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
    pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
}

// call this function to know whether current 3D status is 48hz required.
// if it is 48hz output, we need to adjust the logic of framelock judgement.
MS_BOOL MDrv_SC_3D_IsOutputVfreq48hz(void *pInstance, SCALER_WIN eWindow, MS_U16 u16InputVFreq)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "@@ gSrcInfo[eWindow].stCapWin.height=%u, Input Vfreq=%u\n",gSrcInfo[eWindow].stCapWin.height, u16InputVFreq)
    if(IS_OUTPUT_FRAME_ALTERNATIVE()
       && (u16InputVFreq >= 230)
       && (u16InputVFreq <= 245)
      )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MDrv_SC_3D_AdjustPreVerSrcForFramepacking(void *pInstance, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

#if (HW_DESIGN_3D_VER >= 2)
    return FALSE;
#else
    if(IS_INPUT_FRAME_PACKING(eWindow)
           && IS_OUTPUT_LINE_ALTERNATIVE()
           && pXCResourcePrivate->stdrvXC_3D._bFrmpackToLinealterOpt)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

MS_BOOL MDrv_SC_3D_Is2TapModeSupportedFormat(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return FALSE;
    }

    //LBL to LBL, TB to LBL, FP to LBL can use 2 tap mode to avoid line buffer limitation
    //SBS to LBL don't need use 2 tap mode, since it's op only fetch 960 pixel
    //FI to LBL to and sw 2d to 3d have fclk issue, need prescaling down, so not use 2 tap mode
    //interlace signal can not use 2 tap mode
    //if capture.width<=lblength, no need to use 2 tap mode
    //if have custom v prescaling, can not use 2 tap mode
#if (HW_DESIGN_3D_VER >= 2)
    return  ((((pSrcInfo->stCapWin.width > SCALER_LINE_BUFFER_MAX) // 4K2K or 2560x720 SBS-FULL
              && (IS_INPUT_SIDE_BY_SIDE_FULL(eWindow) || IS_INPUT_SIDE_BY_SIDE_HALF(eWindow))
              && (IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_TOP_BOTTOM()))
            || ((pSrcInfo->stCapWin.width > MS_3D_LINE_BFF_MAX)
              && (!((pSrcInfo->Status2.u16PreVCusScalingSrc > pSrcInfo->Status2.u16PreVCusScalingDst)))
              && (((IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
                  || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
                  || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_TOP_BOTTOM())
                  || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_TOP_BOTTOM())
                  || (IS_INPUT_CHECK_BOARD(eWindow))
                  || (IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
                  || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE()))
                  || ((IS_INPUT_FRAME_PACKING(eWindow)
                        || (IS_INPUT_TOP_BOTTOM(eWindow))
                        || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow))
                        || (IS_INPUT_SIDE_BY_SIDE_FULL(eWindow)))
                      && (IS_OUTPUT_FRAME_PACKING())))))
    #if (HW_2LINEMODE_DEINTERLACE_SUPPORT == FALSE)
              && (!pSrcInfo->bInterlace)
    #endif
                );
#else
    return  ((pSrcInfo->stCapWin.width > MS_3D_LINE_BFF_MAX)
              && (!((pSrcInfo->Status2.u16PreVCusScalingSrc > pSrcInfo->Status2.u16PreVCusScalingDst)))
              && ((IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_TOP_BOTTOM())
                   || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_TOP_BOTTOM()))
              && MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)
    #if (HW_2LINEMODE_DEINTERLACE_SUPPORT == FALSE)
              && (!pSrcInfo->bInterlace)
    #endif
               );
#endif
}

MS_BOOL MDrv_SC_3D_Set2TapMode(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, pSrcInfo, eWindow))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(5), BIT(5)); //Enable HD side by side line buffer mode
#if HW_2LINEMODE_DEINTERLACE_SUPPORT
        if(pSrcInfo->bInterlace)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, BIT(5), BIT(5)); //Force DI 2-line mode at sc2lb mode
        }
#endif

        // When main and sub is start from 0, it means the main&sub win don't need Fill the sub windows line buffer in vertical blanking
        MDrv_XC_FilLineBuffer(pInstance, FALSE, MAIN_WINDOW);

        //The starting address of f1 stored at line buffer
        //Main and Sub window will use the (1920+960) in 2Tap st the same time
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK20_13_L, pSrcInfo->u16H_SizeAfterPreScaling, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK20_1D_L,(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF) || ((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8),DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
        }
        else
#endif
        {
        //The starting address of f1 stored at line buffer
        //Main and Sub window will use the (1920+960) in 2Tap st the same time
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_13_L, pSrcInfo->u16H_SizeAfterPreScaling, 0x1FFF);

        //LB offset should be even when 2 tap mode
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset &= ~0x1;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
    }
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x0, BIT(5)); //Disable HD side by side line buffer mode
#if HW_2LINEMODE_DEINTERLACE_SUPPORT
        if(pSrcInfo->bInterlace)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0, BIT(5)); //Force DI 2-line mode at sc2lb mode
        }
#endif
    }
    return TRUE;
}

MS_BOOL MDrv_SC_3D_PostPQSetting(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return FALSE;
    }

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, pSrcInfo, eWindow))
    {
        //Set SRAM to linear mode
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x0303);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_2B_L, 0x0303);
        //disable snr_vertical vertical c low pass and spike NR
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK26_50_L, 0x0000, 0x00FF);
        //disable v cti
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK26_31_L, 0x0000, 0xF000);
        //disable anti pal
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_6A_L, 0x0000, BIT(0));
        //spf bypass
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0C_70_L, BIT(0), BIT(0));
    }
    return TRUE;
}

MS_BOOL MDrv_SC_3D_IsIPMFetchNoLimitCase(void *pInstance, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

#if (HW_DESIGN_3D_VER >= 2)
    return FALSE;
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((IS_INPUT_FRAME_PACKING(eWindow)
          && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_720X2P_VSIZE))
       ||
       IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
       ||
       MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, &gSrcInfo[eWindow], eWindow)
       )
    {
        //no any limits
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

void MDrv_SC_3D_Adjust_FillLBMode(void *pInstance, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);

#if (HW_DESIGN_3D_VER >= 2)
    if(IS_OUTPUT_TOP_BOTTOM()
       || IS_OUTPUT_SIDE_BY_SIDE_HALF()
       || IS_OUTPUT_LINE_ALTERNATIVE()
       || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()
       || IS_OUTPUT_TOP_BOTTOM_HW()
      )
    {
        MDrv_XC_FilLineBuffer(pInstance, TRUE, SUB_WINDOW);
    }
    else
    {
        MDrv_XC_FilLineBuffer(pInstance, FALSE, SUB_WINDOW);
    }
#else
    MS_BOOL bEnable = FALSE;
    if ( IS_OUTPUT_MODE_NONE() )
    {
        bEnable = TRUE;  // RD suggestion.
    }
    else
    {
        if ( (IS_OUTPUT_SIDE_BY_SIDE_HALF() || IS_OUTPUT_TOP_BOTTOM()
              || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW() || IS_OUTPUT_TOP_BOTTOM_HW())
            && (pXCResourcePrivate->stdrvXC_3D._b3DMainFirst ^ (eWindow == MAIN_WINDOW))) //only trigger the later one
            bEnable = TRUE;
        else
            bEnable = FALSE;
    }
    if (eWindow == SUB_WINDOW)
    {
        MDrv_XC_FilLineBuffer(pInstance, bEnable, SUB_WINDOW);
    }
#endif
    // Designer Jeff.Lin: disable fill line buffer of main_window forever.
    if (eWindow == MAIN_WINDOW)
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        /*sc1 fill main line buffer ahead*/
        if (Hal_SC_3D_IsDualViewMode(pInstance, eWindow) && (psXCInstPri->u32DeviceID == 1))
        {
            MDrv_XC_FilLineBuffer(pInstance, ENABLE, MAIN_WINDOW);
        }
        else
#endif
#if (((HW_DESIGN_4K2K_VER == 7) && (MS_3D_LINE_BFF_MAX == 2048))||((HW_DESIGN_4K2K_VER == 7) && (MS_3D_LINE_BFF_MAX == 960)))
        //By Jeff.Lin: Kano case can enable fill line buffer of main_window
        //Because of Kano is not share line PIP 3D mode
        if(IS_OUTPUT_TOP_BOTTOM()
           || IS_OUTPUT_SIDE_BY_SIDE_HALF()
           || IS_OUTPUT_LINE_ALTERNATIVE()
           || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()
           || IS_OUTPUT_TOP_BOTTOM_HW()
          )
        {
            MDrv_XC_FilLineBuffer(pInstance, ENABLE, MAIN_WINDOW);
        }
        else
#endif

        {
            MDrv_XC_FilLineBuffer(pInstance, DISABLE, MAIN_WINDOW);
        }
    }

    _XC_RETURN(pInstance);
}

void MDrv_SC_3D_Adjust_PreVLength(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                  XC_InternalStatus *pSrcInfo,
                                  SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }

#if (HW_DESIGN_3D_VER >= 2)
    return;
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreVLength(%s): from %u\n", eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16V_Length);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && (!pSrcInfo->bInterlace))
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
       || IS_INPUT_NORMAL_2D_INTERLACE(eWindow)
       || IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow)
       || IS_INPUT_NORMAL_2D(eWindow)) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pSrcInfo->u16V_Length *= 2;
    }
    else if((IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
               ||
               (IS_INPUT_NORMAL_2D(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
               ||
               (IS_INPUT_NORMAL_2D_HW(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM_HW())
               ||
               (IS_INPUT_TOP_BOTTOM(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
               ||
               (IS_INPUT_TOP_BOTTOM(eWindow) &&
               IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        pSrcInfo->u16V_Length /= 2;
    }
    else if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_TOP_BOTTOM())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_L())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_R())
            )
    {
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            u16VactVideo *= 2; //field size from MDrv_SC_3D_GetFP_Info
        }
        pSrcInfo->u16V_Length = u16VactVideo;
    }
    else if((IS_INPUT_LINE_ALTERNATIVE(eWindow) || IS_INPUT_TOP_BOTTOM(eWindow))
            && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
    {
        pSrcInfo->u16V_Length /= 2;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
               IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pSrcInfo->u16V_Length = pSrcInfo->stDispWin.height;
    }
    else if ((IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_LINE_ALTERNATIVE(eWindow)) &&
        (IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R()))
    {
        pSrcInfo->u16V_Length /= 2;
    }
    else if (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pSrcInfo->u16V_Length *= 2;
    }
    if((SUB_WINDOW == eWindow)
       && MDrv_SC_3D_IsUseSameBuffer(pInstance)
       && (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
           || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       )
    {
        pSrcInfo->u16V_Length = gSrcInfo[MAIN_WINDOW].u16V_Length;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreVLength(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16V_Length);
#endif
}

void MDrv_SC_3D_Adjust_PreHorDstSize(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                     XC_InternalStatus *pSrcInfo,
                                     SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_3D._bDoubleAlign = FALSE;

#if HW_2DTO3D_SUPPORT
#define HW_2DTO3DBUFFERSIZE  0x200000
    MS_PHY u32HW2DTO3D_DD_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf;
    MS_PHY u32HW2DTO3D_DR_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf;
    MS_BOOL bOnSameMIU = FALSE;
    if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf >= HAL_MIU1_BASE)
    {
        u32HW2DTO3D_DD_Buf -= HAL_MIU1_BASE;
    }

    if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf >= HAL_MIU1_BASE)
    {
        u32HW2DTO3D_DR_Buf -= HAL_MIU1_BASE;
    }


    if(((pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf >= HAL_MIU1_BASE)
            && (pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf >= HAL_MIU1_BASE)
            && (pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] >= HAL_MIU1_BASE))
        ||((pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf < HAL_MIU1_BASE)
            && (pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf < HAL_MIU1_BASE)
            && (pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] < HAL_MIU1_BASE)))
    {
        bOnSameMIU = TRUE;
    }
    else
    {
        bOnSameMIU = FALSE;
    }
    if(IS_INPUT_NORMAL_2D_HW(eWindow) //hw 2d to 3d
       || ((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
            && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D) //hw auto detect 3d
       || ((E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
            && pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D) //hw auto detect 3d compatible way with sw method
       )
    {
        if(bOnSameMIU
            && (((u32HW2DTO3D_DD_Buf >= (MS_IPM_BASE0(pInstance, eWindow) * BYTE_PER_WORD))
                    && (u32HW2DTO3D_DD_Buf < ((MS_IPM_BASE0(pInstance, eWindow) * BYTE_PER_WORD) + MDrv_XC_GetDNRBufSize(pInstance, eWindow))))
                || ((u32HW2DTO3D_DR_Buf >= (MS_IPM_BASE0(pInstance, eWindow) * BYTE_PER_WORD))
                    && (u32HW2DTO3D_DR_Buf < ((MS_IPM_BASE0(pInstance, eWindow) * BYTE_PER_WORD) + MDrv_XC_GetDNRBufSize(pInstance, eWindow)))))
          )
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "scaler dnr buffer is overflow with hw 2d to 3d buffer\n");
            //E_MMAP_ID_XC_2DTO3D_DD_BUF_LEN+E_MMAP_ID_XC_2DTO3D_DR_BUF_LEN:max:HW_2DTO3DBUFFERSIZE
            pXCResourcePrivate->stdrvXC_3D.u32OriginalDNRSize = MDrv_XC_GetDNRBufSize(pInstance, eWindow);
            MDrv_XC_SetDNRBufSize(pInstance, MDrv_XC_GetDNRBufSize(pInstance, eWindow) - HW_2DTO3DBUFFERSIZE, eWindow);
            pXCResourcePrivate->stdrvXC_3D.bAdjustDNRSize = TRUE;
        }
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, eWindow,
                                                           2,
                                                           pSrcInfo->u16V_SizeAfterPreScaling);

        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
    else if(pXCResourcePrivate->stdrvXC_3D.bAdjustDNRSize
            && ((MDrv_XC_GetDNRBufSize(pInstance, eWindow) + HW_2DTO3DBUFFERSIZE) <= pXCResourcePrivate->stdrvXC_3D.u32OriginalDNRSize))
    {
        MDrv_XC_SetDNRBufSize(pInstance, MDrv_XC_GetDNRBufSize(pInstance, eWindow) + HW_2DTO3DBUFFERSIZE, eWindow);
        pXCResourcePrivate->stdrvXC_3D.bAdjustDNRSize = FALSE;
    }

    if(IS_INPUT_NORMAL_2D_HW(eWindow)
       && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 1200))
    {
        if(pSrcInfo->u16H_SizeAfterPreScaling > 1100)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = 1100;
        }
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       2,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
#endif

#if (HW_DESIGN_3D_VER == 4)
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "PreHorDstSize(%s): %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16H_SizeAfterPreScaling);
    return;
#elif (HW_DESIGN_3D_VER >= 2)
    if(eWindow == MAIN_WINDOW)
    {
        if (pSrcInfo->u16H_SizeAfterPreScaling > MST_LINE_BFF_MAX)
            pSrcInfo->u16H_SizeAfterPreScaling = MST_LINE_BFF_MAX;
    }
    else
    {
        // sub window
        if(PIP_SUPPORTED)
        {
            if (pSrcInfo->u16H_SizeAfterPreScaling > SUB_MST_LINE_BFF_MAX)
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE] SUB LINE BFF LIMIT : HSrc=%u, HDst=%lu\n",pSrcInfo->u16H_SizeAfterPreScaling, SUB_MST_LINE_BFF_MAX);
                pSrcInfo->u16H_SizeAfterPreScaling = SUB_MST_LINE_BFF_MAX;
            }
            // Avoid Post HSD in PIP/POP case
            if(pSrcInfo->u16H_SizeAfterPreScaling >= pSrcInfo->stDispWin.width)
            {
                #if FRC_INSIDE
                if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC) &&
                    ((pSrcInfo->stCropWin.x != 0) || (pSrcInfo->stCropWin.width != pSrcInfo->stCapWin.width)))
                {
                    // Consider Crop case, the post HSD clk is not enough for FRC enable case
                    pSrcInfo->u16H_SizeAfterPreScaling  = ((pSrcInfo->stCapWin.width*pSrcInfo->stDispWin.width)+(pSrcInfo->stCropWin.width/2))/pSrcInfo->stCropWin.width;
                }
                else
                #endif
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"To avoid Post HSD, HDst=%u\n",pSrcInfo->u16H_SizeAfterPreScaling);
            }
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "PreHorDstSize(%s): %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16H_SizeAfterPreScaling);
    return;
#else

    //need do prescaling for 3D case especially when pip use same memory
    if(!(IS_INPUT_MODE_NONE(eWindow) && IS_OUTPUT_MODE_NONE()))
    {
        if(pSrcInfo->u16H_SizeAfterPreScaling > pSrcInfo->stDispWin.width)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreHorDstSize(%s): from %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16H_SizeAfterPreScaling);

    if(IS_INPUT_FRAME_PACKING(eWindow) &&
        IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 1200)
           && ((gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
               || (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
        {
            if(pSrcInfo->u16H_SizeAfterPreScaling > 900)
            {
                pSrcInfo->u16H_SizeAfterPreScaling = 900;
            }
        }

        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       2,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
    else if(((IS_INPUT_LINE_ALTERNATIVE(eWindow) || IS_INPUT_TOP_BOTTOM(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) || IS_INPUT_FRAME_PACKING(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow))
            && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_L())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_R())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_TOP_BOTTOM())
            )
    {
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       4 /* 4 frame mode */,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);

        if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_TOP_BOTTOM())
        {
            //do more prescaling down for fclk issue
            MS_U16 u16Temp = 0;
            if(MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)) //output line by line
            {
                u16Temp = 1300;
            }
            else
            {
                u16Temp = 720;
            }
            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->u16H_SizeAfterPreScaling > u16Temp ?
                    (u16Temp & ~(OFFSET_PIXEL_ALIGNMENT -1)) : pSrcInfo->u16H_SizeAfterPreScaling;
        }

        // for memory reason, need support 4 frame mode
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        //for best quality of side by side in, side by side out
        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;
    }

    if(((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF() && (pSrcInfo->stCapWin.width != pSrcInfo->stDispWin.width))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_TOP_BOTTOM())
       ||
#if (HW_DESIGN_3D_VER < 1)
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_FRAME_ALTERNATIVE())
       ||
#endif
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && (IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       (IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R()))
       ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
       IS_OUTPUT_FRAME_ALTERNATIVE())
       ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
       IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
       )
    {
        MS_U16 u16AlignedWidth = (pSrcInfo->u16H_SizeAfterPreScaling) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
        pSrcInfo->u16H_SizeAfterPreScaling = u16AlignedWidth;
        pXCResourcePrivate->stdrvXC_3D._bDoubleAlign = TRUE;
    }

    if(MDrv_SC_3D_IsIPMFetchNoLimitCase(pInstance, eWindow))
    {
    }
    else if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
            || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        // line buffer check
        if (pSrcInfo->u16H_SizeAfterPreScaling > MS_3D_LINE_BFF_MAX)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = MS_3D_LINE_BFF_MAX;
        }
    }
    else
    {
        // line buffer check
        if(eWindow == MAIN_WINDOW)
        {
            if (pSrcInfo->u16H_SizeAfterPreScaling > MST_LINE_BFF_MAX)
                pSrcInfo->u16H_SizeAfterPreScaling = MST_LINE_BFF_MAX;
        }
        else
        {
            // sub window
            if(PIP_SUPPORTED)
            {
                if (pSrcInfo->u16H_SizeAfterPreScaling > SUB_MST_LINE_BFF_MAX)
                    pSrcInfo->u16H_SizeAfterPreScaling = SUB_MST_LINE_BFF_MAX;
            }
            // Avoid Post HSD in PIP/POP case
            if(pSrcInfo->u16H_SizeAfterPreScaling >= pSrcInfo->stDispWin.width)
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE] To avoid HSD : HSrc=%u, HDst=%u\n",pSrcInfo->u16H_SizeAfterPreScaling, pSrcInfo->stDispWin.width);
                pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
            }
        }
    }

    if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_TOP_BOTTOM())
       || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
       || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_L())
       || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_R())
       )
    {
        if(IS_INPUT_FRAME_PACKING(eWindow)
           && IS_OUTPUT_TOP_BOTTOM()
           && ((gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
               || (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
        {
            //do more prescaling down to fclk issue for 1080p fp:the following formula is from experiment
            MS_U16 u16TempWidth = 0;
            if(MDrv_SC_3D_Is_LR_Sbs2Line(pInstance)) //output line by line
            {
                u16TempWidth = 1920 * (MS_U32)pSrcInfo->stDispWin.height * 13 / 10800;
            }
            else
            {
                u16TempWidth = 640;
            }
            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->u16H_SizeAfterPreScaling > u16TempWidth ?
                    u16TempWidth : pSrcInfo->u16H_SizeAfterPreScaling;
        }
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       2,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
#if (HW_DESIGN_3D_VER == 0)
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        if(pSrcInfo->stCapWin.width > MST_LINE_BFF_MAX / 2)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = (MST_LINE_BFF_MAX / 2) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
        else
        {
            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
    }
#endif
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        MS_U32 u32TempWidth;
        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;
        u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                4,
                                                pSrcInfo->u16V_SizeAfterPreScaling);
        // for memory reason, need support 4 frame mode
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth;
        }
#if (HW_DESIGN_3D_VER == 0)
        if(pSrcInfo->u16H_SizeAfterPreScaling > MST_LINE_BFF_MAX / 2)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = MST_LINE_BFF_MAX / 2;
        }
#endif
        pSrcInfo->u16H_SizeAfterPreScaling &= ~(OFFSET_PIXEL_ALIGNMENT - 1);
    }
    else if((IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
             || (IS_INPUT_TOP_BOTTOM_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        MS_U32 u32TempWidth;
        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;
        u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                2,
                                                pSrcInfo->u16V_SizeAfterPreScaling);
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth;
        }

        if (pSrcInfo->u16H_SizeAfterPreScaling > SUB_MST_LINE_BFF_MAX)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = SUB_MST_LINE_BFF_MAX;
        }

        pSrcInfo->u16H_SizeAfterPreScaling &= ~(OFFSET_PIXEL_ALIGNMENT - 1);
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow)
            && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       2,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
#if (HW_DESIGN_3D_VER == 1)
  else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
            IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        MS_U16 u16AlignedWidth = (pSrcInfo->stCapWin.width) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
        pSrcInfo->u16H_SizeAfterPreScaling = u16AlignedWidth;
        pXCResourcePrivate->stdrvXC_3D._bDoubleAlign = TRUE;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 1200)
           && (pSrcInfo->u16H_SizeAfterPreScaling > 900))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = 900;
        }
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       4 /* 4 frame mode */,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        // for memory reason, need support 4 frame mode
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
    else if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 1200))
    {
        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->u16H_SizeAfterPreScaling > 1200 ? 1200 : pSrcInfo->u16H_SizeAfterPreScaling;
    }
#endif

#if ENABLE_2_FRAME_SIZE_PROTECTION
    //do a rough check after all, for 2 frame case.
    //if it's 4 or other frame case, need detail check in the upper code.
    {
        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                       2,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        if((pSrcInfo->u16H_SizeAfterPreScaling > u32TempWidth)&&(u32TempWidth!=0))
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }
#endif

    if((SUB_WINDOW == eWindow)
       && (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
           || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       )
    {
        pSrcInfo->u16H_SizeAfterPreScaling = gSrcInfo[MAIN_WINDOW].u16H_SizeAfterPreScaling;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreHorDstSize(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16H_SizeAfterPreScaling);
#endif
}

MS_BOOL MDrv_SC_3D_IsDoubleAlign(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_3D._bDoubleAlign;
}

void MDrv_SC_3D_Adjust_SubLineBufferOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                           XC_InternalStatus *pSrcInfo,
                                           SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

#if (HW_DESIGN_3D_VER >= 2)
    return;
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //Setting Line Buffer Start offset for Sub Win
    if((eWindow == SUB_WINDOW)
        && (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
            || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       )
    {
        // we can extend sub window line buffer by setting this register
        HAL_SC_SetSubLineBufferOffset(pInstance, MS_3D_LINE_BFF_MAX);
    }
#endif
}

MS_U16 MDrv_SC_3D_Adjust_PreVerDstSize(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType,
                                       XC_InternalStatus *pSrcInfo,
                                       MS_U16 u16ScaleDst,
                                       SCALER_WIN eWindow)
{
    MS_U16 u16ScaleDstOriginal = u16ScaleDst;
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return u16ScaleDstOriginal;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return u16ScaleDstOriginal;
    }
#if (HW_DESIGN_3D_VER >= 2)
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"MDrv_SC_3D_Adjust_PreVerDstSize(%s)= %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleDstOriginal)
    return u16ScaleDstOriginal;
#else
    MS_U16 u16ScaleSrc;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u16ScaleSrc = pSrcInfo->stCapWin.height;
    u16ScaleDst = pSrcInfo->stDispWin.height;

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreVerSrcSize(%s): from %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleSrc);

    if(((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && (!pSrcInfo->bInterlace))
         || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
         || IS_INPUT_NORMAL_2D(eWindow)
         || IS_INPUT_NORMAL_2D_INTERLACE(eWindow)) &&
            IS_OUTPUT_LINE_ALTERNATIVE())
    {
        if(u16ScaleSrc >= u16ScaleDst/2)
        {
             u16ScaleDst = u16ScaleDst/2;
             XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: pre scaling: source V size is bigger than u16ScaleDst/2(%u)\n",u16ScaleDst);
        }
        else
        {
            u16ScaleDst = u16ScaleSrc;
             XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: pre scaling: source V size is smaller than u16ScaleDst/2(%u)\n",u16ScaleDst);
        }
    }
    else if((IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
            || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() && pSrcInfo->bInterlace)
            || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE()))
    {
        if (u16ScaleSrc > u16ScaleDst)
        {
            u16ScaleDst = u16ScaleDst;
        }
        else
        {
            u16ScaleDst = u16ScaleSrc;
        }
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        //do noting, just not follow into statement:else
    }
    else if((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
            (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
            ||
            (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && (IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
            )
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
            (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))//only for T8 new reg method
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow) &&
            (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow)
            && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        if((u16ScaleSrc == DOUBLEHD_720X2P_VSIZE) //720 centering opt
           && (pSrcInfo->stDispWin.height == DOUBLEHD_720X2P_FRAME_VSIZE))
        {
            u16ScaleDst = 735;
        }
        else
        {
            // we can merge 1080p and 720p frame packing, since
            // 1080(one frame size)/45(garbage band) = 720(one frame size)/30(garbage band)
            MS_U16 u16Temp = DOUBLEHD_1080X2P_GARBAGE_VSIZE * u16ScaleDst / 2;
            // if can not mod, need take more 1 lines to do opt
            if(u16Temp % DOUBLEHD_1080X2P_FRAME_VSIZE)
            {
                pXCResourcePrivate->stdrvXC_3D._bFrmpackToLinealterOpt = TRUE;
                u16Temp = u16Temp / DOUBLEHD_1080X2P_FRAME_VSIZE + 1;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_3D._bFrmpackToLinealterOpt = FALSE;
                u16Temp = u16Temp / DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            u16ScaleDst = u16ScaleDst + u16Temp;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: pre scaling: frame packing, V down to %u\n",u16ScaleDst);
    }
    else if((IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
       (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_LINE_ALTERNATIVE()))
       ||
       (IS_INPUT_TOP_BOTTOM(eWindow) &&
       IS_OUTPUT_TOP_BOTTOM())
       ||
       (IS_INPUT_TOP_BOTTOM(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if (IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
            IS_OUTPUT_FRAME_L())
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_TOP_BOTTOM())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
         IS_OUTPUT_TOP_BOTTOM())
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow) &&
            (IS_OUTPUT_TOP_BOTTOM()
             || IS_OUTPUT_SIDE_BY_SIDE_HALF()
             || IS_OUTPUT_LINE_ALTERNATIVE()))
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() )
    {
        u16ScaleDst = pSrcInfo->stDispWin.height / 2;
    }
    else if (IS_INPUT_FRAME_PACKING(eWindow) &&
            (IS_OUTPUT_FRAME_L()||IS_OUTPUT_FRAME_R()))
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        //output line by line case or small video
        if((u16ScaleSrc < pSrcInfo->stDispWin.height) || MDrv_SC_3D_Is_LR_Sbs2Line(pInstance))
        {
            u16ScaleDst = u16ScaleSrc;
        }
        else
        {
            u16ScaleDst = pSrcInfo->stDispWin.height;
        }
    }
    else if(IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        u16ScaleDst = u16ScaleDst / 2;
    }
    else if((IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
             || (IS_INPUT_TOP_BOTTOM_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
            IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        u16ScaleDst = u16ScaleSrc;
    }
    else if((IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_TOP_BOTTOM())
            || (IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW()))
    {
        if(u16ScaleSrc < pSrcInfo->stDispWin.height)
        {
            u16ScaleDst = u16ScaleSrc;
        }
        else
        {
            u16ScaleDst = pSrcInfo->stDispWin.height;
        }
    }
    else
    {
        u16ScaleDst = u16ScaleDstOriginal;
    }

    if((SUB_WINDOW == eWindow)
       && MDrv_SC_3D_IsUseSameBuffer(pInstance)
       && (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
           || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       )
    {
        pSrcInfo->u16V_SizeAfterPreScaling = gSrcInfo[MAIN_WINDOW].u16V_SizeAfterPreScaling;
        u16ScaleDst = gSrcInfo[MAIN_WINDOW].u16V_SizeAfterPreScaling;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreVerDstSize(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleDst);
    return u16ScaleDst;
#endif
}

MS_U16 MDrv_SC_3D_Adjust_PostHorSrcSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                        MS_U16 u16ScaleSrc,
                                        SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return u16ScaleSrc;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return u16ScaleSrc;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "PostHorSrcSize(%s): %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16H_SizeAfterPreScaling);
    return u16ScaleSrc;
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PostHorSrcSize(%s): from %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleSrc);
    if((((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && (!pSrcInfo->bInterlace))
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF() && (pSrcInfo->stCapWin.width != pSrcInfo->stDispWin.width))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_TOP_BOTTOM())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && (IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
        )
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.width/2;
    }
    else if (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
        (IS_OUTPUT_FRAME_L()||IS_OUTPUT_FRAME_R()))
    {
        u16ScaleSrc = u16ScaleSrc/2;
    }
#if (HW_DESIGN_3D_VER == 0)
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
            && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        u16ScaleSrc = u16ScaleSrc*2;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.width * 2;
    }
#endif
    else if((IS_INPUT_NORMAL_2D_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
         ||(IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
         ||(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
         ||(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
         ||(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE()))
    {
        gSrcInfo[eWindow].ScaledCropWin.width -= pXCResourcePrivate->stdrvXC_3D._u163DHShift;
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.width;
    }
    else if((IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
             || (IS_INPUT_TOP_BOTTOM_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.width * 2;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.width/2;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() && pSrcInfo->bInterlace)
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.width/2;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PostHorSrcSize(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleSrc);

    return u16ScaleSrc;
#endif
}

MS_U16 MDrv_SC_3D_Adjust_PostHorDstSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                        MS_U16 u16ScaleDst,
                                        SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return u16ScaleDst;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return u16ScaleDst;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (HW_DESIGN_3D_VER >= 2)
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "PostHorDstSize(%s): %u\n",eWindow?"Subwindow":"Mainwindow",pSrcInfo->u16H_SizeAfterPreScaling);
    return u16ScaleDst;
#else

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PostHorDstSize(%s): from %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleDst);
    if(((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF() && (pSrcInfo->stCapWin.width != pSrcInfo->stDispWin.width))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_TOP_BOTTOM())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && (IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
        )
    {
        if (u16ScaleDst > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width)
        {
            u16ScaleDst = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PostHorDstSize(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleDst);

    return u16ScaleDst;
#endif
}

MS_U16 MDrv_SC_3D_Adjust_PostVerSrcSize(void *pInstance, XC_InternalStatus *pSrcInfo,
                                        MS_U16 u16ScaleSrc,
                                        SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return u16ScaleSrc;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return u16ScaleSrc;
    }

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)
    return u16ScaleSrc;
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PostVerSrcSize(%s): from %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleSrc);
    if(((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && (!pSrcInfo->bInterlace))
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
       || IS_INPUT_NORMAL_2D_INTERLACE(eWindow)
       || IS_INPUT_NORMAL_2D(eWindow)) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.height * 2;
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        u16ScaleSrc = pSrcInfo->stDispWin.height - gSrcInfo[eWindow].ScaledCropWin.y*2;
    }
    else if((IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
               ||
               (IS_INPUT_TOP_BOTTOM(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
               ||
               (IS_INPUT_TOP_BOTTOM(eWindow) &&
               IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.height/2;//E_XC_3D_INPUT_LINE_ALTERNATIVE cannot do v-scaling.
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
               IS_OUTPUT_FRAME_L())
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.height/2;//E_XC_3D_INPUT_LINE_ALTERNATIVE cannot do v-scaling.
    }
    else if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_TOP_BOTTOM())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_L())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_R())
           )
    {
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            u16VactVideo *= 2; //field size from MDrv_SC_3D_GetFP_Info
        }
        u16ScaleSrc = (gSrcInfo[eWindow].ScaledCropWin.height-u16VactSpace)/2;
    }
    else if((IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
            || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()))
    {
        u16ScaleSrc = u16ScaleSrc/2;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
               IS_OUTPUT_SIDE_BY_SIDE_HALF() && (pSrcInfo->stCapWin.width != pSrcInfo->stDispWin.width))
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.height;//E_XC_3D_INPUT_LINE_ALTERNATIVE cannot do v-scaling.
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.height;//E_XC_3D_INPUT_LINE_ALTERNATIVE cannot do v-scaling.
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
                  IS_OUTPUT_TOP_BOTTOM())
    {
        u16ScaleSrc = pSrcInfo->ScaledCropWin.height;
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow) &&
             (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        u16ScaleSrc = gSrcInfo[eWindow].ScaledCropWin.height / 2;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        u16ScaleSrc = pSrcInfo->stDispWin.height - gSrcInfo[eWindow].ScaledCropWin.y*2;
    }
    else if (IS_INPUT_TOP_BOTTOM(eWindow) &&
        (IS_OUTPUT_FRAME_L()||IS_OUTPUT_FRAME_R()))
    {
        u16ScaleSrc = u16ScaleSrc/2;
    }
    else if((IS_INPUT_LINE_ALTERNATIVE(eWindow) || IS_INPUT_TOP_BOTTOM(eWindow))
            && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
    {
        u16ScaleSrc = u16ScaleSrc/2;
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
            && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        u16ScaleSrc = u16ScaleSrc/2;
    }
    else if(IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        u16ScaleSrc = pSrcInfo->stDispWin.height;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PostVerSrcSize(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",u16ScaleSrc);

    return u16ScaleSrc;
#endif
}

void MDrv_SC_3D_Adjust_PostVLength(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)
    return;
#else
    if(IS_INPUT_FRAME_PACKING(eWindow)
       && IS_OUTPUT_LINE_ALTERNATIVE()
       && pXCResourcePrivate->stdrvXC_3D._bFrmpackToLinealterOpt)
    {
        pSrcInfo->u16V_Length -= 2; //to handle odd garbage line scaling
    }
#endif
}

XC_FRAME_STORE_NUMBER MDrv_SC_3D_Adjust_FrameNum(void *pInstance, XC_FRAME_STORE_NUMBER u8FBNumType,
                                       SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow >= MAX_WINDOW)
    {
        return u8FBNumType;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER == 2)
    if((IS_INPUT_FRAME_ALTERNATIVE(eWindow) && (!gSrcInfo[eWindow].bInterlace))||
    (IS_INPUT_FRAME_PACKING(eWindow) && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForXc)
        {
            u8FBNumType = IMAGE_STORE_5_FRAMES;
        }
        else
        {
            u8FBNumType = IMAGE_STORE_4_FRAMES;
        }
    }
    return u8FBNumType;
#elif (HW_DESIGN_3D_VER > 2)
    if(IS_INPUT_FRAME_PACKING(eWindow) && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForXc)
        {
            u8FBNumType = IMAGE_STORE_5_FRAMES;
        }
        else
        {
            u8FBNumType = IMAGE_STORE_4_FRAMES;
        }
    }
#ifdef TBP_1920X2160_OUTPUT
    else if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
            &&(gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
            &&(MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FRAME_PACKING)
            &&(MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM))
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForXc)
        {
            u8FBNumType = IMAGE_STORE_5_FRAMES;
        }
        else
        {
            u8FBNumType = IMAGE_STORE_4_FRAMES;
        }
    }
#endif
    return u8FBNumType;
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "_Mdrv_SC_3D_Adjust_FBNum(%s): from %u\n",eWindow?"Subwindow":"Mainwindow",u8FBNumType);
    if(
#if (HW_DESIGN_3D_VER == 0)
        ((IS_INPUT_TOP_BOTTOM(eWindow) ||
        (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && (!gSrcInfo[eWindow].bInterlace)) ||
        IS_INPUT_FRAME_PACKING(eWindow) ) &&
          IS_OUTPUT_LINE_ALTERNATIVE())
        ||
       (IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
        (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_LINE_ALTERNATIVE()))
        ||
       (IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
        IS_OUTPUT_FRAME_L())
        ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
        IS_OUTPUT_TOP_BOTTOM())
        ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
        IS_OUTPUT_SIDE_BY_SIDE_HALF())
        ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
        IS_OUTPUT_FRAME_ALTERNATIVE())
        ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
        IS_OUTPUT_FRAME_L())
        ||
       (IS_INPUT_FRAME_PACKING(eWindow) &&
        IS_OUTPUT_FRAME_R())
        ||
       (IS_INPUT_TOP_BOTTOM(eWindow) &&
        IS_OUTPUT_TOP_BOTTOM())
        ||
       (IS_INPUT_TOP_BOTTOM(eWindow) &&
        IS_OUTPUT_SIDE_BY_SIDE_HALF())
        ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
        IS_OUTPUT_FRAME_ALTERNATIVE())
        ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF())
        ||
       (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
       IS_OUTPUT_TOP_BOTTOM())
       ||
       (IS_INPUT_FRAME_PACKING_OPT(eWindow) &&
       IS_OUTPUT_TOP_BOTTOM())
       ||
       (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) &&
       IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())// 2 frames = 4 fields
       ||
       (IS_INPUT_NORMAL_2D_INTERLACE(eWindow) &&
       IS_OUTPUT_LINE_ALTERNATIVE())// 2 frames = 4 fields
       ||
       (IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow) &&
       IS_OUTPUT_LINE_ALTERNATIVE())// 2 frames = 4 fields
       ||
       (IS_INPUT_FRAME_PACKING_OPT(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF())
       ||
       (IS_INPUT_TOP_BOTTOM_OPT(eWindow) &&
       IS_OUTPUT_SIDE_BY_SIDE_HALF())
       ||
       (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
       ||
#endif
       (IS_INPUT_FRAME_PACKING(eWindow)
        && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
        ||
        (IS_OUTPUT_FRAME_ALTERNATIVE() && (!IS_INPUT_FRAME_ALTERNATIVE(eWindow)))
       )
    {
        u8FBNumType = IMAGE_STORE_2_FRAMES;
    }
    else if((IS_INPUT_FRAME_ALTERNATIVE(eWindow) && (IS_OUTPUT_TOP_BOTTOM()|| IS_OUTPUT_LINE_ALTERNATIVE()))
            || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_L())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_R())
            || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
            || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
    {
        u8FBNumType = IMAGE_STORE_4_FRAMES;
    }

    if((!IS_INPUT_MODE_NONE(eWindow)) && (SUB_WINDOW == eWindow))
    {
        u8FBNumType = gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber;
        gSrcInfo[eWindow].bLinearMode = gSrcInfo[MAIN_WINDOW].bLinearMode;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "_Mdrv_SC_3D_Adjust_FBNum(%s): to %u\n",eWindow?"Subwindow":"Mainwindow",u8FBNumType);

    return u8FBNumType;
#endif
}

void MDrv_SC_3D_Adjust_DNRBase(void *pInstance, MS_PHY *pu32DNRBase0,
                               MS_PHY *pu32DNRBase1,
                               SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pu32DNRBase0);
    MS_ASSERT(pu32DNRBase1);

    if(eWindow >= MAX_WINDOW)
    {
        return;
    }

    if(pu32DNRBase0 == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D:pu32DNRBase0 is NULL\n");
        return;
    }

    if(pu32DNRBase1 == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D:pu32DNRBase1 is NULL\n");
        return;
    }

#if (HW_DESIGN_3D_VER >= 2)
    return;
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_DNRBase(%s): from pu32DNRBase0:0x%lx, pu32DNRBase1:0x%lx\n",eWindow?"Subwindow":"Mainwindow",*pu32DNRBase0,*pu32DNRBase1);
    MS_U16  u16DNROffset = (gSrcInfo[eWindow].u16IPMFetch + OFFSET_PIXEL_ALIGNMENT - 1)
                                                          & ~(OFFSET_PIXEL_ALIGNMENT-1);
    MS_U8   u8BytesPer2Pixel = gSrcInfo[eWindow].u8BitPerPixel * 2 / 8;
    if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) + (((gSrcInfo[eWindow].u16V_SizeAfterPreScaling)*
                        (MS_U32)u16DNROffset * ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
    else if((IS_INPUT_FRAME_ALTERNATIVE(eWindow)
             || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
             || IS_INPUT_NORMAL_2D_INTERLACE(eWindow))
            && IS_OUTPUT_LINE_ALTERNATIVE()
                 )
    {
        if (IsVMirrorMode(eWindow))
        {
            *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) - (((1)*(MS_U32)u16DNROffset *
                            ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        }
        else
        {
            *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) + (((1)*(MS_U32)(u16DNROffset)*
                            ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() && gSrcInfo[eWindow].bInterlace)
    {
        if (IsVMirrorMode(eWindow))
        {
            *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) - (((1)*(MS_U32)u16DNROffset *
                            ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        }
        else
        {
            *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) + (((1)*(MS_U32)u16DNROffset *
                            ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        if (IsVMirrorMode(eWindow))
        {
            *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) - (((1)*(MS_U32)u16DNROffset *
                            ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        }
        else
        {
            *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) + (((1)*(MS_U32)u16DNROffset *
                            ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_TOP_BOTTOM() && (eWindow == SUB_WINDOW))
    {
        Hal_SC_subwindow_disable(pInstance);
        *pu32DNRBase0 = MS_IPM_BASE0(MAIN_WINDOW) + (((gSrcInfo[eWindow].stDispWin.height / 2)
                      * (MS_U32)u16DNROffset * ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
    else if(((IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
             || (IS_INPUT_TOP_BOTTOM_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
             && (SUB_WINDOW == eWindow))
    {
        Hal_SC_subwindow_disable(pInstance);
        *pu32DNRBase0 = MS_IPM_BASE0(MAIN_WINDOW) + (((1)*(MS_U32)u16DNROffset * ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) + (((1)*(MS_U32)u16DNROffset * ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
#if (HW_DESIGN_3D_VER == 0)
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        *pu32DNRBase1 = MS_IPM_BASE0(pInstance, eWindow) + (((1)*(MS_U32)u16DNROffset * ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_DNRBase(%s): to pu32DNRBase0:0x%lx, pu32DNRBase1:0x%lx\n",eWindow?"Subwindow":"Mainwindow",*pu32DNRBase0,*pu32DNRBase1);
#endif
}

void MDrv_SC_3D_Adjust_OPMBase(void *pInstance, MS_PHY *pu32OPMBase0,
                               MS_PHY *pu32OPMBase1,
                               MS_PHY *pu32OPMBase2,
                               MS_PHY u32Offset,
                               SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow == MAX_WINDOW)
    {
        return;
    }

    if(pu32OPMBase0 == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "pu32OPMBase0 is NULL\n");
        return;
    }

    if(pu32OPMBase1 == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "pu32OPMBase1 is NULL\n");
        return;
    }

    if(pu32OPMBase2 == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "pu32OPMBase2 is NULL\n");
        return;
    }

#if (HW_DESIGN_3D_VER >= 2)
    return;
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_PHY  u32DNRBase0 = MS_IPM_BASE0(pInstance, eWindow);
    MS_PHY  u32DNRBase1 = u32DNRBase0 + ((u32Offset / BYTE_PER_WORD + 0x01) & ~0x01L);
    MS_PHY  u32DNRBase0Main = MS_IPM_BASE0(MAIN_WINDOW);
    MS_PHY  u32DNRBase1Main = 0;
    MS_PHY  u32DNRBase2Main = 0;
    MS_U8   u8BytesPer2Pixel = gSrcInfo[eWindow].u8BitPerPixel * 2 / 8;
    MS_BOOL bInterlace = gSrcInfo[eWindow].bInterlace;
    MS_U16  u16DNROffset = (gSrcInfo[eWindow].u16IPMFetch + OFFSET_PIXEL_ALIGNMENT - 1)
                                                          & ~(OFFSET_PIXEL_ALIGNMENT-1);
    MS_U32  u32Framesize = (MS_U32)gSrcInfo[eWindow].u16V_SizeAfterPreScaling *
                                  (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    MS_U8   u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pu32OPMBase0);
    MS_ASSERT(pu32OPMBase1);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_OPMBase(%s): from pu32OPMBase0:0x%lx, pu32OPMBase1:0x%lx, pu32OPMBase2:0x%lx\n",eWindow?"Subwindow":"Mainwindow",*pu32OPMBase0,*pu32OPMBase1,*pu32OPMBase2);

    if( gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES )
    {
        u32DNRBase1Main = u32DNRBase0Main + ((MDrv_XC_GetDNRBufSize(pInstance, MAIN_WINDOW) / 3 / BYTE_PER_WORD + 0x01) & ~0x01L);
        u32DNRBase2Main = u32DNRBase1Main + ((MDrv_XC_GetDNRBufSize(pInstance, MAIN_WINDOW) / 3 / BYTE_PER_WORD + 0x01) & ~0x01L);
    }
    else
    {
        u32DNRBase1Main = u32DNRBase0Main + ((MDrv_XC_GetDNRBufSize(pInstance, MAIN_WINDOW) / 2 / BYTE_PER_WORD + 0x01) & ~0x01L);
        u32DNRBase2Main = u32DNRBase0Main;
    }

    MDrv_SC_3D_Adjust_DNRBase(pInstance,
                              &u32DNRBase0,
                              &u32DNRBase1,
                              eWindow);

    //default setting: sub window use mainwindow's scaler buffer
    if((!IS_INPUT_MODE_NONE(eWindow)) && (SUB_WINDOW == eWindow))
    {
        *pu32OPMBase0 = (u32DNRBase0Main + 0x01) & ~0x01L;
        *pu32OPMBase1 = (u32DNRBase1Main + 0x01) & ~0x01L;
        *pu32OPMBase2 = (u32DNRBase2Main + 0x01) & ~0x01L;
    }

    if((IS_INPUT_TOP_BOTTOM(eWindow) ||
        (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && (!gSrcInfo[eWindow].bInterlace)) ||
        IS_INPUT_FRAME_ALTERNATIVE(eWindow)||
        IS_INPUT_FRAME_PACKING(eWindow)||
        IS_INPUT_NORMAL_2D(eWindow)) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
    {
        MS_U32 u32DoubleHDMidBandSize = 0;
        if((gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_720X2P_VSIZE)
            || (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2P_VSIZE))
        {
            u32DoubleHDMidBandSize = (MS_U32)(gSrcInfo[eWindow].u16V_SizeAfterPreScaling - gSrcInfo[eWindow].stDispWin.height) * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u32DoubleHDMidBandSize=%u\n", gSrcInfo[eWindow].u16V_SizeAfterPreScaling - gSrcInfo[eWindow].stDispWin.height);
        }

        if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
        {
            *pu32OPMBase1 = u32DNRBase0;
        }
        else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                && (!gSrcInfo[eWindow].bInterlace)
                && IS_OUTPUT_LINE_ALTERNATIVE())
        {
            *pu32OPMBase1 = *pu32OPMBase0 + ((((MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
        else if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
        {
            if (IsVMirrorMode(eWindow))
            {
                *pu32OPMBase1 = *pu32OPMBase0 + ((((MS_U32)1 * (MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                    (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
            }
            else
            {
                *pu32OPMBase1 = *pu32OPMBase0;
                *pu32OPMBase0 = *pu32OPMBase1 + ((((MS_U32)1 * (MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                    (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
            }
        }
        else if(IS_INPUT_TOP_BOTTOM(eWindow) &&
                IS_OUTPUT_LINE_ALTERNATIVE())
        {
            *pu32OPMBase1 = *pu32OPMBase0 + (((u32Framesize)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
        else if(IS_INPUT_FRAME_PACKING(eWindow) &&
                IS_OUTPUT_LINE_ALTERNATIVE())
        {
            MS_U32 u32SecondFrameOffset = 0;
            if(gSrcInfo[eWindow].stDispWin.height == DOUBLEHD_720X2P_FRAME_VSIZE) //720p centering opt
            {
                u32SecondFrameOffset = (MS_U32)(DOUBLEHD_720X2P_FRAME_VSIZE / 2) * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel +
                                             (MS_U32)15 * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel;
            }
            else
            {
                u32SecondFrameOffset = (MS_U32)(gSrcInfo[eWindow].stDispWin.height / 2) * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel +
                                             u32DoubleHDMidBandSize;
            }
            *pu32OPMBase1 = *pu32OPMBase0 + ((u32SecondFrameOffset/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_L())
    {
        *pu32OPMBase2 = *pu32OPMBase1 = *pu32OPMBase0 = u32DNRBase0;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_R())
    {
        *pu32OPMBase2 = *pu32OPMBase1 = *pu32OPMBase0 = u32DNRBase1;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
           && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        *pu32OPMBase1 = u32DNRBase0 + ((((MS_U32)1 * (MS_U32)u16DNROffset / 2 * (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
           && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        *pu32OPMBase1 = u32DNRBase0 + ((((MS_U32)1 * (MS_U32)u16DNROffset / 2 *
                            (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() && gSrcInfo[eWindow].bInterlace)
    {
        *pu32OPMBase1 = u32DNRBase0 + ((((MS_U32)1 * (MS_U32)u16DNROffset / 2 *
                            (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_NORMAL_2D_INTERLACE(eWindow)
           && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        if (IsVMirrorMode(eWindow))
        {
            *pu32OPMBase1 = *pu32OPMBase0 + ((((MS_U32)1 * (MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
        else
        {
            *pu32OPMBase1 = *pu32OPMBase0;
            *pu32OPMBase0 = *pu32OPMBase1 + ((((MS_U32)1 * (MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if((IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow))
           && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        if (IsVMirrorMode(eWindow))
        {
            *pu32OPMBase1 = *pu32OPMBase0 + ((((MS_U32)1 * (MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
        else
        {
            *pu32OPMBase1 = *pu32OPMBase0;
            *pu32OPMBase0 = *pu32OPMBase1 + ((((MS_U32)1 * (MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
        IS_OUTPUT_TOP_BOTTOM() &&
        (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        *pu32OPMBase0 = u32DNRBase1;
        *pu32OPMBase1 = u32DNRBase1;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
        IS_OUTPUT_TOP_BOTTOM() &&
        (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst != FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        *pu32OPMBase0 = u32DNRBase0;
        *pu32OPMBase1 = u32DNRBase0;
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow) &&
            (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()) &&
            (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == TRUE)? SUB_WINDOW:MAIN_WINDOW)))
    {
        *pu32OPMBase0 = ((u32DNRBase0Main + 0x01) & ~0x01L)
                        + (((gSrcInfo[eWindow].u16V_SizeAfterPreScaling/4)*
                        (MS_U32)u16DNROffset * ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
        *pu32OPMBase1 = ((u32DNRBase1Main + 0x01) & ~0x01L)
                        + (((gSrcInfo[eWindow].u16V_SizeAfterPreScaling/4)*(MS_U32)u16DNROffset *
                        ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        *pu32OPMBase1 = u32DNRBase1 + (((gSrcInfo[eWindow].u16V_SizeAfterPreScaling/2)*(MS_U32)u16DNROffset *
                          ((MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD)+ 0x01) & ~0x01L);
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
           (IS_OUTPUT_TOP_BOTTOM()
#if (HW_DESIGN_3D_VER == 1)
           || IS_OUTPUT_SIDE_BY_SIDE_HALF()
#endif
           ) &&
           (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        *pu32OPMBase0 = u32DNRBase0Main + 1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
        *pu32OPMBase1 = u32DNRBase1Main + 1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
        *pu32OPMBase2 = u32DNRBase2Main + 1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
            && IS_OUTPUT_FRAME_L()
            && gSrcInfo[eWindow].bInterlace)
    {
        *pu32OPMBase1 = (((*pu32OPMBase0 + 1*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD)) + 0x01) & ~0x01L);
    }
    else if(((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_TOP_BOTTOM())
           || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
           && (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        MS_U16 u16SkipLine_OpmBase0 = 0;
        MS_U16 u16SkipLine_OpmBase1 = 0;
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MDrv_SC_3D_GetFP_Info(pInstance, &gSrcInfo[eWindow], &u16VactVideo, &u16VactSpace, FALSE);

        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            u16SkipLine_OpmBase0 = u16VactVideo + DOUBLEHD_1080X2I_VACT_SPACE1;
            u16SkipLine_OpmBase1 = gSrcInfo[eWindow].stCapWin.height - u16VactVideo;
        }
        else
        {
            u16SkipLine_OpmBase0 = gSrcInfo[eWindow].stCapWin.height - u16VactVideo;
        }

        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D----------garbage band----------------u16SkipLine_OpmBase0=%u, u16SkipLine_OpmBase1=%u\n", u16SkipLine_OpmBase0, u16SkipLine_OpmBase1);
        *pu32OPMBase0 = u32DNRBase0Main + ((u16SkipLine_OpmBase0*u8FrameNum*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2) + 0x01) & ~0x01L);
        *pu32OPMBase1 = u32DNRBase1Main + ((u16SkipLine_OpmBase0*u8FrameNum*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2) + 0x01) & ~0x01L);
        *pu32OPMBase2 = u32DNRBase2Main + ((u16SkipLine_OpmBase0*u8FrameNum*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2) + 0x01) & ~0x01L);
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {//this format need use madi
            *pu32OPMBase1 = u32DNRBase0Main + ((u16SkipLine_OpmBase1*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if(((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_TOP_BOTTOM())
           || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
           && (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? SUB_WINDOW:MAIN_WINDOW)))
    {
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            MS_U16 u16SkipLine_OpmBase1 = DOUBLEHD_1080X2I_FIELD_VSIZE*2+DOUBLEHD_1080X2I_VACT_SPACE1+DOUBLEHD_1080X2I_VACT_SPACE2;
            *pu32OPMBase1 = u32DNRBase0Main + ((u16SkipLine_OpmBase1*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) &&
           (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()) &&
           (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        MS_U16 u16SkipLine = gSrcInfo[eWindow].u16V_SizeAfterPreScaling / 2;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D----------garbage band----------------u16SkipLine=%u\n", u16SkipLine);
        *pu32OPMBase0 = u32DNRBase0Main + u16SkipLine/(1 + (TRUE == bInterlace))*u8FrameNum*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
        *pu32OPMBase1 = u32DNRBase1Main + u16SkipLine/(1 + (TRUE == bInterlace))*u8FrameNum*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
        *pu32OPMBase2 = u32DNRBase2Main + u16SkipLine/(1 + (TRUE == bInterlace))*u8FrameNum*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
           IS_OUTPUT_SIDE_BY_SIDE_HALF()&&
           (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)) && (gSrcInfo[eWindow].stCapWin.width != gSrcInfo[eWindow].stDispWin.width))
    {
        *pu32OPMBase0 = u32DNRBase0Main + u8FrameNum*(MS_U32)u16DNROffset/2 * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
        *pu32OPMBase1 = u32DNRBase1Main + u8FrameNum*(MS_U32)u16DNROffset/2 * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
        *pu32OPMBase2 = u32DNRBase2Main + u8FrameNum*(MS_U32)u16DNROffset/2 * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
           IS_OUTPUT_TOP_BOTTOM() &&
           (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        *pu32OPMBase0 = (u32DNRBase0Main + (MS_U32)u16DNROffset/2 * u8FrameNum * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2) + 0x01) & ~0x01L;
        *pu32OPMBase1 = (u32DNRBase1Main + (MS_U32)u16DNROffset/2 * u8FrameNum * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2) + 0x01) & ~0x01L;
        *pu32OPMBase2 = (u32DNRBase2Main + (MS_U32)u16DNROffset/2 * u8FrameNum * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2) + 0x01) & ~0x01L;
    }
#if (HW_DESIGN_3D_VER < 1)
    else if((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
           (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
           ||(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && (IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())))

    {
        *pu32OPMBase0 = u32DNRBase0;
        *pu32OPMBase1 = *pu32OPMBase0 + (MS_U32)(u16DNROffset) * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD*2);
    }
    else if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
    {
        if (IsVMirrorMode(eWindow))
        {
            *pu32OPMBase1 = *pu32OPMBase1 + ((((MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
        else
        {
            *pu32OPMBase0 = *pu32OPMBase0 + ((((MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                                (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow) &&
           (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
    {
        MS_U16 u16SkipLine = 0;
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MDrv_SC_3D_GetFP_Info(pInstance, &gSrcInfo[eWindow], &u16VactVideo, &u16VactSpace, FALSE);
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            u16VactVideo *= 2; //field size from MDrv_SC_3D_GetFP_Info
        }

        u16SkipLine = gSrcInfo[eWindow].stCapWin.height - u16VactVideo;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D----------garbage band----------------u16SkipLine=%u\n", u16SkipLine);
        *pu32OPMBase0 = u32DNRBase0;
        *pu32OPMBase1 = *pu32OPMBase0 + (MS_U32)u16DNROffset *  u16SkipLine * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD);
    }
#endif
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
            IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
    {
        *pu32OPMBase1 = *pu32OPMBase0 + (((MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/BYTE_PER_WORD + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
    {
        *pu32OPMBase1 = *pu32OPMBase0 + ((u32Framesize/BYTE_PER_WORD + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        if(gSrcInfo[eWindow].bInterlace)
        {
            *pu32OPMBase1 = *pu32OPMBase0 + ((u32Framesize/2/BYTE_PER_WORD + 0x01) & ~0x01L);//only handle one field
        }
        else
        {
            *pu32OPMBase1 = *pu32OPMBase0 + ((u32Framesize/BYTE_PER_WORD + 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        *pu32OPMBase1 = *pu32OPMBase0 + ((((MS_U32)(pXCResourcePrivate->stdrvXC_3D._u163DHShift)  *
                           (MS_U32)u8BytesPer2Pixel)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
                IS_OUTPUT_LINE_ALTERNATIVE())
    {
        *pu32OPMBase1 = *pu32OPMBase0 + ((1*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow) && (IS_OUTPUT_FRAME_L()))
    {
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            MS_U16 u16SkipLine_OpmBase1 = DOUBLEHD_1080X2I_FIELD_VSIZE*2+DOUBLEHD_1080X2I_VACT_SPACE1+DOUBLEHD_1080X2I_VACT_SPACE2;
            *pu32OPMBase0 = u32DNRBase0Main;
            *pu32OPMBase1 = u32DNRBase0Main + ((u16SkipLine_OpmBase1*(MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow) &&
               (IS_OUTPUT_FRAME_R()))
    {
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MDrv_SC_3D_GetFP_Info(pInstance, &gSrcInfo[eWindow], &u16VactVideo, &u16VactSpace, FALSE);
        if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            MS_U16 u16SkipLine_OpmBase0 = u16VactSpace+DOUBLEHD_1080X2I_VACT_SPACE1;
            MS_U16 u16SkipLine_OpmBase1 = gSrcInfo[eWindow].stCapWin.height - u16VactVideo;
            *pu32OPMBase0 = u32DNRBase0Main + ((u16SkipLine_OpmBase0*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD) + 0x01) & ~0x01L);
            *pu32OPMBase1 = u32DNRBase0Main + ((u16SkipLine_OpmBase1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        }
        else
        {
            MS_U16 u16SkipLine = 0;
            u16SkipLine = gSrcInfo[eWindow].stCapWin.height - u16VactVideo;
            *pu32OPMBase0 = u32DNRBase0Main + u16SkipLine*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD);
            *pu32OPMBase1 = u32DNRBase1Main + u16SkipLine*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD);
            *pu32OPMBase2 = u32DNRBase2Main + u16SkipLine*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD);
        }
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) &&
               (IS_OUTPUT_FRAME_R()))
    {
        *pu32OPMBase0 = u32DNRBase0Main + (((u32Framesize/2) * u8FrameNum / (1 + (TRUE == bInterlace)) /(BYTE_PER_WORD) + 0x01) & ~0x01L);
        *pu32OPMBase1 = u32DNRBase1Main + (((u32Framesize/2) * u8FrameNum / (1 + (TRUE == bInterlace)) /(BYTE_PER_WORD) + 0x01) & ~0x01L);
        *pu32OPMBase2 = u32DNRBase2Main + (((u32Framesize/2) * u8FrameNum / (1 + (TRUE == bInterlace)) /(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
               (IS_OUTPUT_FRAME_R()))
    {
        *pu32OPMBase0 = u32DNRBase0Main + ((((MS_U32)u16DNROffset/2 * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        *pu32OPMBase1 = u32DNRBase1Main + ((((MS_U32)u16DNROffset/2 * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
        *pu32OPMBase2 = u32DNRBase2Main + ((((MS_U32)u16DNROffset/2 * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2)/(BYTE_PER_WORD) + 0x01) & ~0x01L);
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
            && IS_OUTPUT_FRAME_R())
    {
        *pu32OPMBase0 = (((u32DNRBase0Main + 1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD)) + 0x01) & ~0x01L);
        *pu32OPMBase1 = (((u32DNRBase1Main + 1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD)) + 0x01) & ~0x01L);
        *pu32OPMBase2 = (((u32DNRBase2Main + 1*(MS_U32)u16DNROffset * u8FrameNum * (MS_U32)u8BytesPer2Pixel/2/(BYTE_PER_WORD)) + 0x01) & ~0x01L);
    }
#if (HW_DESIGN_3D_VER == 0)
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        *pu32OPMBase1 = *pu32OPMBase0 = u32DNRBase0;
    }
#else
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
        IS_OUTPUT_SIDE_BY_SIDE_HALF() &&
        (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst == FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        *pu32OPMBase0 = u32DNRBase0;
        *pu32OPMBase1 = u32DNRBase0;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
        IS_OUTPUT_SIDE_BY_SIDE_HALF() &&
        (eWindow == ((pXCResourcePrivate->stdrvXC_3D._b3DMainFirst != FALSE)? MAIN_WINDOW:SUB_WINDOW)))
    {
        *pu32OPMBase0 = u32DNRBase1;
        *pu32OPMBase1 = u32DNRBase1;
    }
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_OPMBase(%s): to pu32OPMBase0:0x%lx, pu32OPMBase1:0x%lx, pu32OPMBase2:0x%lx\n",eWindow?"Subwindow":"Mainwindow",*pu32OPMBase0,*pu32OPMBase1,*pu32OPMBase2);
#endif
}



void MDrv_SC_3D_Adjust_FetchOffset(void *pInstance, XC_InternalStatus *pSrcInfo,
                                   SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(eWindow >= MAX_WINDOW)
    {
        return;
    }

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: pSrcInfo is NULL\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (HW_DESIGN_3D_VER >= 2)

    if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow)
       && (!pSrcInfo->bInterlace)
       && (IS_OUTPUT_FRAME_ALTERNATIVE()||IS_OUTPUT_TOP_BOTTOM()))
    {

        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch  = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset/2;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch/2;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: adjust u16OPMOffset(%u) and u16OPMFetch(%u) to half for E_XC_3D_INPUT_SIDE_BY_SIDE\n",
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset,pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            // Fixme: need refine dnr offset fetch
            //pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize;
            //pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch  = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch/2;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: adjust u16DualOPMOffset(%u) and u16DualOPMFetch(%u) to half for E_XC_3D_INPUT_SIDE_BY_SIDE\n",
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset,pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch);
        }
#endif

    }
    else  if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
           && (!IS_OUTPUT_MODE_NONE()))
    {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset/2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
            if (IsEnableDualMode(eWindow))
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2;
            }
#endif
    }
    return;
#else
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
       && (!pSrcInfo->bInterlace)
       && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  /= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  /= 2;
        }
#endif
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch /=2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset *= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch /=2;
        }
#endif

    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch /=2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch /=2;
        }
#endif
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() && gSrcInfo[eWindow].bInterlace)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch /=2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch /=2;
        }
#endif
    }
    else if(IS_INPUT_NORMAL_2D_INTERLACE(eWindow) &&
       IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset *= 2;
        }
#endif
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
       (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_FRAME_L()
       || IS_OUTPUT_FRAME_R()))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset *=2;
        }
#endif

    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) &&
            (IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        //change nothing
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow)
            && IS_OUTPUT_LINE_ALTERNATIVE()
            && pSrcInfo->bInterlace)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset *= 2;
        }
#endif
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
               IS_OUTPUT_SIDE_BY_SIDE_HALF() && (pSrcInfo->stCapWin.width != pSrcInfo->stDispWin.width))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch
                                                                            - pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset)/2
                                                                          + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch +1) & ~1;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            // Under Miu dual mode, dual opm fetch need not set linebuffer offset
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  /= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch +1) & ~1;

        }
#endif

    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
               IS_OUTPUT_TOP_BOTTOM())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch
                                                                            - pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset)/2
                                                                          + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch +1) & ~1;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            // Under Miu dual mode, dual opm fetch need not set linebuffer offset
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  /= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch +1) & ~1;

        }
#endif
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() )
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;

#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset *= 2;

        }
#endif
    }
    else if((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
                (IS_OUTPUT_FRAME_ALTERNATIVE() || IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
           ||(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && (IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC()))
            )
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch
                                                                            - pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset)/2
                                                                          + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch +1) & ~1;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            // Under Miu dual mode, dual opm fetch need not set linebuffer offset
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  /= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch +1) & ~1;

        }
#endif
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
               (IS_OUTPUT_FRAME_L()||IS_OUTPUT_FRAME_R()))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch
                                                                            - pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset)/2
                                                                          + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch +1) & ~1;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            // Under Miu dual mode, dual opm fetch need not set linebuffer offset
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  /= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch +1) & ~1;

        }
#endif
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
            IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset *= 2;

        }
#endif
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOffset *= 2;
        }
#endif
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
            && (IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset  *= 2;
        }
#endif
#if (HW_DESIGN_3D_VER == 0)
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  *= 2;
#endif
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
            && IS_OUTPUT_FRAME_L()
            && gSrcInfo[eWindow].bInterlace)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset *= 2;
        }
#endif
    }
#if (HW_DESIGN_3D_VER == 0)
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset *= 2;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  *= 2;
    }
#endif
    else if((IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
             || (IS_INPUT_TOP_BOTTOM_OPT(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset *= 2;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset *= 2;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  *= 2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset *= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset *= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  *= 2;
        }
#endif
    }

#if (HW_DESIGN_3D_VER < 1)
    //set free_md
    Hal_XC_Set_FreeFRCMD(pInstance, (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
                IS_OUTPUT_FRAME_ALTERNATIVE()) ||
                (IS_INPUT_FRAME_PACKING(eWindow) &&
                IS_OUTPUT_FRAME_ALTERNATIVE()));
#elif (HW_DESIGN_3D_VER == 1)
    //set free_md
    Hal_XC_Set_FreeFRCMD(pInstance, (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
                         || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()));
#endif

#if (HW_DESIGN_3D_VER == 1)
    {
        if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) &&
                   IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch /= 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset + (OFFSET_PIXEL_ALIGNMENT*2 -1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset + (OFFSET_PIXEL_ALIGNMENT*2 -1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
            Hal_XC_H3D_Enable(TRUE);
            Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_SIDE_BY_SIDE_FULL);
            Hal_XC_H3D_Breakline_Enable(TRUE);
            Hal_XC_H3D_HDE((pSrcInfo->u16H_SizeAfterPreScaling+OFFSET_PIXEL_ALIGNMENT-1) & ~(OFFSET_PIXEL_ALIGNMENT-1));
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
        }
        else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) &&
                    IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset*2 + (OFFSET_PIXEL_ALIGNMENT*2 -1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset*2 + (OFFSET_PIXEL_ALIGNMENT*2 -1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
            Hal_XC_H3D_Enable(TRUE);
            Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_LINE_ALTERNATIVE);
            Hal_XC_H3D_Breakline_Enable(FALSE);
            Hal_XC_H3D_HDE(0);
            HAL_XC_H3D_OPM_SBYS_PIP_Enable(FALSE);
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
        }
        else if(IS_INPUT_FRAME_PACKING(eWindow) &&
                    IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset*2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset*2;
            if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
            {
                Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
            }
            else
            {
                Hal_XC_H3D_HBLANK(0x33E);
            }
            Hal_XC_H3D_HDE(DOUBLEHD_1080X2P_HSIZE);//0x780
            Hal_XC_H3D_INIT_VBLANK(0x02);
            Hal_XC_H3D_VDE_F0(DOUBLEHD_1080X2P_FRAME_VSIZE);//0x438
            Hal_XC_H3D_VBLANK0(DOUBLEHD_1080X2P_GARBAGE_VSIZE);//0x2D
            Hal_XC_H3D_VBLANK1(DOUBLEHD_1080X2P_GARBAGE_VSIZE);//0x2D
            Hal_XC_H3D_VDE_F2(DOUBLEHD_1080X2P_FRAME_VSIZE);//0x438
            Hal_XC_H3D_VBLANK2(DOUBLEHD_1080X2P_GARBAGE_VSIZE);//0x2D
            Hal_XC_H3D_VSYNC_WIDTH(0x01);
            Hal_XC_H3D_VSYNC_POSITION(0x0400);
            Hal_XC_H3D_Enable(TRUE);
            Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_FRAME_PACKING);
            Hal_XC_H3D_SELECT_REGEN_TIMING(TRUE);
            Hal_XC_H3D_Breakline_Enable(FALSE);
            HAL_XC_H3D_OPM_SBYS_PIP_Enable(TRUE);
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);

            if ((gSrcInfo[eWindow].stCapWin.width == DOUBLEHD_1080X2I_HSIZE) && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
            {
                if ((pSrcInfo->u16InputVFreq >= 240) && (pSrcInfo->u16InputVFreq <= 260))
                {
                    if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
                    {
                        Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
                    }
                    else
                    {
                        Hal_XC_H3D_HBLANK(0x2D0);//1080i@50Hz
                    }
                }
                else if ((pSrcInfo->u16InputVFreq >= 290) && (pSrcInfo->u16InputVFreq <= 310))
                {
                    if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
                    {
                        Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
                    }
                    else
                    {
                        Hal_XC_H3D_HBLANK(0x118);//1080i@60Hz
                    }
                }
                Hal_XC_H3D_HDE(DOUBLEHD_1080X2I_HSIZE);//0x780
                Hal_XC_H3D_INIT_VBLANK(0x02);
                Hal_XC_H3D_VDE_F0(DOUBLEHD_1080X2I_FRAME_VSIZE/2);//0x21C
                Hal_XC_H3D_VBLANK0(0x17);//0x17
                Hal_XC_H3D_VBLANK1(0x16);//0x16
                Hal_XC_H3D_VDE_F2(DOUBLEHD_1080X2I_FRAME_VSIZE/2);//0x21C
                Hal_XC_H3D_VBLANK2(0x17);//0x17
                Hal_XC_H3D_VSYNC_WIDTH(0x01);
                Hal_XC_H3D_VSYNC_POSITION(0x0400);
                Hal_XC_H3D_Enable(TRUE);
                Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_FRAME_PACKING);
                Hal_XC_H3D_SELECT_REGEN_TIMING(TRUE);
                Hal_XC_H3D_Breakline_Enable(FALSE);
                HAL_XC_H3D_OPM_SBYS_PIP_Enable(TRUE);
                Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
            }
            else if ((gSrcInfo[eWindow].stCapWin.width == DOUBLEHD_720X2P_HSIZE) && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_720X2P_VSIZE))
            {
                if ((pSrcInfo->u16InputVFreq >= 470) && (pSrcInfo->u16InputVFreq <= 520))
                {
                    if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
                    {
                        Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
                    }
                    else
                    {
                        Hal_XC_H3D_HBLANK(0x2BC);//50Hz=700
                    }
                }
                else if ((pSrcInfo->u16InputVFreq >= 570) && (pSrcInfo->u16InputVFreq <= 620))
                {
                    if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
                    {
                        Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
                    }
                    else
                    {
                        Hal_XC_H3D_HBLANK(0x172);//59.94/60Hz=370
                    }
                }

                Hal_XC_H3D_HDE(DOUBLEHD_720X2P_HSIZE);//1280
                Hal_XC_H3D_INIT_VBLANK(0x02);
                Hal_XC_H3D_VDE_F0(DOUBLEHD_720X2P_FRAME_VSIZE);//720
                Hal_XC_H3D_VBLANK0(DOUBLEHD_720X2P_GARBAGE_VSIZE);//0x1E
                Hal_XC_H3D_VBLANK1(DOUBLEHD_720X2P_GARBAGE_VSIZE);//0x1E
                Hal_XC_H3D_VDE_F2(DOUBLEHD_720X2P_FRAME_VSIZE);//0x2D0
                Hal_XC_H3D_VBLANK2(DOUBLEHD_720X2P_GARBAGE_VSIZE);//0x1E
                Hal_XC_H3D_VSYNC_WIDTH(0x01);
                Hal_XC_H3D_VSYNC_POSITION(0x0400);
                Hal_XC_H3D_Enable(TRUE);
                Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_FRAME_PACKING);
                Hal_XC_H3D_SELECT_REGEN_TIMING(TRUE);
                Hal_XC_H3D_Breakline_Enable(FALSE);
                HAL_XC_H3D_OPM_SBYS_PIP_Enable(TRUE);
                Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
            }
        }
        else if(IS_INPUT_FRAME_PACKING(eWindow) &&
                    IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset*2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset*2;
            if ((pSrcInfo->u16InputVFreq >= 470) && (pSrcInfo->u16InputVFreq <= 520))
            {
                if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
                {
                    Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
                }
                else
                {
                    Hal_XC_H3D_HBLANK(0x2BC);//50Hz=700
                }
            }
            else if ((pSrcInfo->u16InputVFreq >= 570) && (pSrcInfo->u16InputVFreq <= 620))
            {
                if(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.bEnableUserSetting)
                {
                    Hal_XC_H3D_HBLANK(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara.u16HBlank);
                }
                else
                {
                    Hal_XC_H3D_HBLANK(0x172);//59.94/60Hz=370
                }
            }
            Hal_XC_H3D_HDE(DOUBLEHD_720X2P_HSIZE);//1280
            Hal_XC_H3D_INIT_VBLANK(0x02);
            Hal_XC_H3D_VDE_F0(DOUBLEHD_720X2P_FRAME_VSIZE);//720
            Hal_XC_H3D_VBLANK0(DOUBLEHD_720X2P_GARBAGE_VSIZE);//0x1E
            Hal_XC_H3D_VBLANK1(DOUBLEHD_720X2P_GARBAGE_VSIZE);//0x1E
            Hal_XC_H3D_VDE_F2(DOUBLEHD_720X2P_FRAME_VSIZE);//0x2D0
            Hal_XC_H3D_VBLANK2(DOUBLEHD_720X2P_GARBAGE_VSIZE);//0x1E no need in 1280x1470
            Hal_XC_H3D_VSYNC_WIDTH(0x01);
            Hal_XC_H3D_VSYNC_POSITION(0x0400);
            Hal_XC_H3D_Enable(TRUE);
            Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_FRAME_PACKING);
            Hal_XC_H3D_SELECT_REGEN_TIMING(TRUE);
            Hal_XC_H3D_Breakline_Enable(FALSE);
            HAL_XC_H3D_OPM_SBYS_PIP_Enable(TRUE);
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
        }
        else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
                    IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset*2 + (OFFSET_PIXEL_ALIGNMENT*2 -1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset*2 + (OFFSET_PIXEL_ALIGNMENT*2 -1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 -1);
            Hal_XC_H3D_Enable(FALSE);
            Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_FRAME_PACKING);
            Hal_XC_H3D_Breakline_Enable(FALSE);
            Hal_XC_H3D_HDE(0);
            HAL_XC_H3D_OPM_SBYS_PIP_Enable(FALSE);
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
            Hal_XC_H3D_SELECT_REGEN_TIMING(FALSE);
        }
        else
        {
            Hal_XC_H3D_Enable(FALSE);
            Hal_XC_H3D_Input3DType(E_XC_3D_INPUT_FRAME_PACKING);
            Hal_XC_H3D_Breakline_Enable(FALSE);
            Hal_XC_H3D_HDE(0);
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, FALSE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);
            Hal_XC_H3D_SELECT_REGEN_TIMING(FALSE);
            Hal_HDMI_Set_YUV422to444_Bypass(TRUE);
        }

        // Version = 1. 3D mode enable
        if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_70_L, (BIT(15))) )
        {
            Hal_HDMI_Set_YUV422to444_Bypass(FALSE);
        }
    }
#endif
    //  IPM offset / IPM fetch /OPM offset: All project --> 32pix align-----Fix me: Need consider DNR offset and DNR/OPM base
    //  OPM fetch:
    //            Janus /T7 --> 32pix align
    //            Others ( T8/T9/T12/T13/m10/J2 ) --> 2pix align
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = ( pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch + 1 ) & ~1;  //  pixels  alignment;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch *= ( pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1 ) & ~1;
        }
#endif

#endif
}

MS_BOOL MDrv_SC_3D_IsFramePaking(void *pInstance, MS_U16 u16Width, MS_U16 u16Height, MS_BOOL bInterlace)
{
    MS_BOOL bFramePacking = (((u16Width == DOUBLEHD_720X2P_HSIZE)
                                && (u16Height == DOUBLEHD_720X2P_VSIZE))
                              || ((u16Width == DOUBLEHD_576X2P_HSIZE)
                                && (u16Height == DOUBLEHD_576X2P_VSIZE))
                              || ((u16Width == DOUBLEHD_480X2P_HSIZE)
                                && (u16Height == DOUBLEHD_480X2P_VSIZE))
                              || ((u16Width == DOUBLEHD_640_480X2P_HSIZE)
                                && (u16Height == DOUBLEHD_640_480X2P_VSIZE))
                              || ((u16Width == DOUBLEHD_1080X2P_HSIZE)
                                && (u16Height == DOUBLEHD_1080X2P_VSIZE))
                              || ((u16Width == DOUBLEHD_1080X2I_HSIZE)
                                && (u16Height == DOUBLEHD_1080X2I_VSIZE))
                                ) && (!bInterlace);
    return bFramePacking;
}

MS_BOOL MDrv_SC_3D_FormatValidation(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_BOOL bRet = TRUE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_BOOL bFramePacking = MDrv_SC_3D_IsFramePaking(pInstance, pSrcInfo->stCapWin.width, pSrcInfo->stCapWin.height, pSrcInfo->bInterlace);

    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

    if( IS_INPUT_MODE_NONE(eWindow)^IS_OUTPUT_MODE_NONE() )
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if( IS_INPUT_MODE_NONE(eWindow) )
        {
            bRet = Hal_SC_3D_IsDualViewMode(pInstance, eWindow);
        }
#else
        bRet = FALSE;
#endif
    }
    else
    {
        if( IS_INPUT_MODE_NONE(eWindow))
        {
            return TRUE;
        }
    }


#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if(!Hal_SC_3D_IsDualViewMode(pInstance, eWindow))  //do not check FP content while in dual view mode
#endif
    {
        bRet = !(IS_INPUT_FRAME_PACKING(eWindow) ^ bFramePacking);
    }

    if(!bRet)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: format is invalid,3D Mode=%u !\n", pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow]);
    }

#ifdef UFO_XC_FB_LEVEL
    if((pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FBL) || (pSrcInfo->bR_FBL == E_XC_FB_LEVEL_RFBL_DI))
#else
    if(pSrcInfo->bFBL || pSrcInfo->bR_FBL)
#endif
    {
        if(!(MDrv_SC_3D_IsSupportFBL3D()
             && ((IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
                  || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE()))))
        {
            bRet = FALSE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: not support this 3d format under fbl\n");
        }
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow)
       || IS_INPUT_CHECK_BOARD(eWindow)
       || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
    {
        if(pSrcInfo->bInterlace)
        {
            bRet = FALSE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: only progresive timing is supported\n");
        }
    }
#if (HW_DESIGN_3D_VER < 2)
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow))
    {
        if(!pSrcInfo->bInterlace)
        {
            bRet = FALSE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: only interlace timing is supported\n");
        }
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow))
    {
        if(pSrcInfo->bInterlace)
        {
            bRet = FALSE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: only progresive timing is supported\n");
        }
    }
#endif
#if (HW_DESIGN_3D_VER!= 4)
    if(IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 3))//special use only for napoli
        if ( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal> 3000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal> 2000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500)) // 4K2K 60 out
        {
            if (!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ))
            {
                bRet = FALSE;

                printf("3D: need special flag E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ for 3D format E_XC_3D_OUTPUT_FRAME_ALTERNATIVE\n");
            }
        }
        else
        #endif
        if((pSrcInfo->u16InputVFreq > 350)
           &&
           ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 500)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 600)))
        {

            bRet = FALSE;

            printf("3D: need double framerate for 3D formatE_XC_3D_OUTPUT_FRAME_ALTERNATIVE\n");
        }
    }
#endif

    if (IS_OUTPUT_FRAME_PACKING())
    {
        //3D FP ouput ,display window always full screen
        pSrcInfo->stDispWin.x = 0;
        pSrcInfo->stDispWin.width = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
        pSrcInfo->stDispWin.y = 0;
        pSrcInfo->stDispWin.height = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }

    if (IS_INPUT_PIXEL_ALTERNATIVE(eWindow) || IS_INPUT_CHECK_BOARD(eWindow))
    {
        if (IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType))
        {
            _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
            MS_HDMI_COLOR_FORMAT eFormat = MDrv_HDMI_Get_ColorFormat();
            _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
            if ((eFormat == MS_HDMI_COLOR_YUV_422) || (eFormat == MS_HDMI_COLOR_YUV_420))
            {
               bRet = FALSE;
               printf("3D: YUV422/YUV420 do not support HDMI PA/CB 3D input.\n");
            }
        }
        else
        {
            bRet = FALSE;
            printf("3D: This source do not support PA/CB 3D input.\n");
        }
    }

    if(!bRet)
    {
        printf("MDrv_SC_3D_FormatValidation(): invalid format, set to 3d_none\n");
        _XC_ENTRY(pInstance);
        MDrv_XC_Set_3D_HShift(pInstance, 0);
        Hal_SC_subwindow_disable(pInstance);
        _XC_RETURN(pInstance);
        pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] = E_XC_3D_INPUT_MODE_NONE;

        if ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
            && !(pXCResourcePrivate->stdrvXC_3D._bFAToTB)
            && !(pXCResourcePrivate->stdrvXC_3D._bLAToTB))
        {
            pSrcInfo->stDispWin.y *= 2;
        }
        else if ((pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
            && !(pXCResourcePrivate->stdrvXC_3D._bFAToTB)
            && !(pXCResourcePrivate->stdrvXC_3D._bLAToTB))
        {
            pSrcInfo->stDispWin.x *= 2;
        }

#if (HW_DESIGN_3D_VER < 2)
        if (pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_BOTTOM)
        {
            pSrcInfo->stDispWin.height *= 2;
        }

        if (pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
        {
            pSrcInfo->stDispWin.width *= 2;
        }
#endif
        pXCResourcePrivate->stdrvXC_3D._eOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
        pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;
        pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
        pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
        pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
        pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
#if (HW_DESIGN_3D_VER < 2)
        MDrv_XC_Enable_3D_LR_Sbs2Line(pInstance, DISABLE);
#endif
    }
    return bRet;
}

void MDrv_SC_3D_AdjustHShift(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER < 2)
    if((IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_TOP_BOTTOM())
       ||  (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()))
    {
        pXCResourcePrivate->stdrvXC_3D._u163DHShift = (pXCResourcePrivate->stdrvXC_3D._u163DHShift + 1) & ~1;
        pSrcInfo->stCropWin.width -= pXCResourcePrivate->stdrvXC_3D._u163DHShift;
        if(MAIN_WINDOW == eWindow)
        {
            pSrcInfo->stCropWin.x += pXCResourcePrivate->stdrvXC_3D._u163DHShift;
        }
    }
#if (HW_2DTO3D_SUPPORT == FALSE)
    else
    {
        pXCResourcePrivate->stdrvXC_3D._u163DHShift = (pXCResourcePrivate->stdrvXC_3D._u163DHShift + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
    }
#endif
#endif
}

//disable sub window write dnr buffer when no needed
void MDrv_SC_3D_SetWriteMemory(void *pInstance, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    if(eWindow >= MAX_WINDOW)
    {
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);

#if (HW_DESIGN_3D_VER >= 2)
    if(IS_OUTPUT_TOP_BOTTOM()
       || IS_OUTPUT_SIDE_BY_SIDE_HALF()
       || IS_OUTPUT_LINE_ALTERNATIVE()
       || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()
       || IS_OUTPUT_TOP_BOTTOM_HW()
      )
    {
        Hal_SC_IP_Memory_Write_Request(pInstance, FALSE, SUB_WINDOW);
        Hal_SC_IP_Memory_Read_Request(pInstance, FALSE, SUB_WINDOW);
    }
    else
    {
        Hal_SC_IP_Memory_Write_Request(pInstance, TRUE, SUB_WINDOW);
        Hal_SC_IP_Memory_Read_Request(pInstance, TRUE, SUB_WINDOW);
    }
#else
    if(SUB_WINDOW == eWindow)
    {
        if(IS_INPUT_MODE_NONE(eWindow)
           || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF()
                && (gSrcInfo[eWindow].stCapWin.width == gSrcInfo[eWindow].stDispWin.width))
           || IS_INPUT_FRAME_PACKING_OPT(eWindow)
           || IS_INPUT_TOP_BOTTOM_OPT(eWindow)
           || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_TOP_BOTTOM() && (!MDrv_SC_3D_IsUseSameBuffer(pInstance)))
           || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF() && (!MDrv_SC_3D_IsUseSameBuffer(pInstance)))
           || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_TOP_BOTTOM() && (!MDrv_SC_3D_IsUseSameBuffer(pInstance)))
           || (IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF_HW() && (!MDrv_SC_3D_IsUseSameBuffer(pInstance)))
           || (IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW() && (!MDrv_SC_3D_IsUseSameBuffer(pInstance)))
#if (HW_DESIGN_3D_VER == 1)
           || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF() && (!MDrv_SC_3D_IsUseSameBuffer(pInstance)))
#endif
           )
        {// 2D format, sbs in, sbs out centering, framepacking opt,
         // or frame alternative in, top bottom out
            Hal_SC_IP_Memory_Write_Request(pInstance, TRUE, eWindow);
            Hal_SC_IP_Memory_Read_Request(pInstance, TRUE, eWindow);
        }
        else
        {
            Hal_SC_IP_Memory_Write_Request(pInstance, FALSE, eWindow);
            Hal_SC_IP_Memory_Read_Request(pInstance, FALSE, eWindow);
        }
    }
#endif

    _XC_RETURN(pInstance);
}

MS_BOOL MDrv_SC_3D_Is2Dto3DCase(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                                E_XC_3D_OUTPUT_MODE e3dOutputMode)
{
    MS_BOOL bRet = FALSE;

#if (HW_DESIGN_3D_VER <= 1)
    if(((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D) &&
        (e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
       ||
       ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_INTERLACE) &&
        (e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
       ||
       ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_INTERLACE_PTP) &&
        (e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
       ||
       ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D) &&
        (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC))
      )
    {
        bRet = TRUE;
    }
#endif

    return bRet;
}

void MDrv_SC_3D_CloneMainSettings(void *pInstance, XC_InternalStatus *pSrcInfo,
                                  SCALER_WIN eWindow)
{

    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER <= 1)
    if(SUB_WINDOW == eWindow)
    {
        if(IS_OUTPUT_TOP_BOTTOM()
           || IS_OUTPUT_SIDE_BY_SIDE_HALF()
           || IS_OUTPUT_TOP_BOTTOM_HW()
           || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()
           || IS_OUTPUT_FRAME_R())
        {
            MS_U8 u8Data_Mux = 0;
            MS_U8 u8Clk_Mux = 0;
            MDrv_XC_EnableCLK_for_SUB(pInstance, TRUE);
            Hal_SC_mux_get_mainwin_ip_mux(pInstance, &u8Data_Mux, &u8Clk_Mux);
            Hal_SC_set_subwin_ip_mux(pInstance, u8Data_Mux, u8Clk_Mux);
            if(MDrv_SC_3D_IsUseSameBuffer(pInstance))
            {
                if(pSrcInfo->bInterlace)
                {
                    pSrcInfo->eDeInterlaceMode = gSrcInfo[MAIN_WINDOW].eDeInterlaceMode;
                }
                pSrcInfo->u8BitPerPixel = gSrcInfo[MAIN_WINDOW].u8BitPerPixel;
            }
        }
    }
#else
    if(IS_OUTPUT_TOP_BOTTOM()
       || IS_OUTPUT_SIDE_BY_SIDE_HALF()
       || IS_OUTPUT_TOP_BOTTOM_HW()
       || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()
       || IS_OUTPUT_LINE_ALTERNATIVE()
       || IS_OUTPUT_FRAME_R()
      )
    {
        MS_U8 u8Data_Mux = 0;
        MS_U8 u8Clk_Mux = 0;
        MDrv_XC_EnableCLK_for_SUB(pInstance, TRUE);
        Hal_SC_mux_get_mainwin_ip_mux(pInstance, &u8Data_Mux, &u8Clk_Mux);
        Hal_SC_set_subwin_ip_mux(pInstance, u8Data_Mux, u8Clk_Mux);
    }
    else if(!IS_INPUT_MODE_NONE(MAIN_WINDOW))
    {
        if (eWindow == MAIN_WINDOW)
            MDrv_XC_EnableCLK_for_SUB(pInstance, FALSE);
        else
            MDrv_XC_EnableCLK_for_SUB(pInstance, TRUE);
    }
#endif
}

MS_BOOL MDrv_SC_3D_IsUseSameBuffer(void *pInstance)
{
    MS_BOOL bRet = FALSE;

#if (HW_DESIGN_3D_VER <= 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return (MS_IPM_BASE0(MAIN_WINDOW) == MS_IPM_BASE0(SUB_WINDOW));
#endif

    return bRet;
}

void MDrv_SC_3D_Adjust_MirrorDNROffset(void *pInstance, XC_InternalStatus *pSrcInfo,
                                       MS_U16 *pu16V_SizeAfterPreScaling,
                                       MS_U16 *pu16DNROffset,
                                       MS_U16 *pu16Ratio,
                                       SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)
    if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
       || IS_INPUT_TOP_BOTTOM(eWindow))
    {
        *pu16V_SizeAfterPreScaling /= 2;
        *pu16Ratio *= 2;
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow))
    {
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MS_U8 u8Ratio = 2;
        if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            u8Ratio = 4;
        }
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
    #if (HW_DESIGN_3D_VER == 2)
        *pu16V_SizeAfterPreScaling = (*pu16V_SizeAfterPreScaling - u16VactSpace) / u8Ratio;
    #else
        *pu16V_SizeAfterPreScaling /= u8Ratio;
    #endif
        *pu16Ratio *= 2;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
            || IS_INPUT_CHECK_BOARD(eWindow)
            || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
           )
    {
        *pu16DNROffset = ((*pu16DNROffset / 2) + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
        *pu16Ratio *= 2;
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
    {
        *pu16DNROffset *= 2;
        *pu16Ratio *= 1;
    }
#else
    if((IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
         ||(IS_INPUT_NORMAL_2D_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
         ||(IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
         ||(IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE() && gSrcInfo[eWindow].bInterlace)
         ||(IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
       )
    {
        *pu16V_SizeAfterPreScaling *= 2;
    }
#if (HW_DESIGN_3D_VER == 1)
    else if(IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        MS_U16 u16VactVideo = 0;
        MS_U16 u16VactSpace = 0;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
        *pu16V_SizeAfterPreScaling = (*pu16V_SizeAfterPreScaling - u16VactSpace) / 2;
        *pu16Ratio *= 2;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        *pu16DNROffset = ((*pu16DNROffset / 2) + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
        *pu16Ratio *= 2;
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        *pu16V_SizeAfterPreScaling /= 2;
        *pu16Ratio *= 2;
    }
#endif
#endif
}

void MDrv_SC_3D_AdjustMirrorCrop(void *pInstance, XC_InternalStatus *pSrcInfo,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#ifndef DISABLE_3D_FUNCTION
    #if (HW_DESIGN_3D_VER >= 1)
        if(IS_INPUT_TOP_BOTTOM(eWindow)
         || IS_INPUT_FRAME_PACKING(eWindow)
         || IS_INPUT_FRAME_ALTERNATIVE(eWindow)
         || IS_INPUT_LINE_ALTERNATIVE(eWindow))
         {
             //app set overscan'value that is L crop value,but L and R is two crop'value.
              gSrcInfo[eWindow].ScaledCropWin.y = gSrcInfo[eWindow].ScaledCropWin.y * 2;
        }
         if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
            || IS_INPUT_CHECK_BOARD(eWindow)
            || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
         {
             gSrcInfo[eWindow].ScaledCropWin.x =gSrcInfo[eWindow].ScaledCropWin.x * 2;
        }
    #endif
 #endif
        if( pSrcInfo->u16H_SizeAfterPreScaling > (gSrcInfo[eWindow].ScaledCropWin.x + gSrcInfo[eWindow].ScaledCropWin.width) )
        {
             gSrcInfo[eWindow].ScaledCropWin.x = pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x + gSrcInfo[eWindow].ScaledCropWin.width);
             // select nearest even number
        }
        else
        {
             gSrcInfo[eWindow].ScaledCropWin.x = 0;
            if(gSrcInfo[eWindow].ScaledCropWin.width > pSrcInfo->u16H_SizeAfterPreScaling)
            {
                 gSrcInfo[eWindow].ScaledCropWin.width = pSrcInfo->u16H_SizeAfterPreScaling;
             }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[2.a] Force x=%u, width=%u\n",gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.width);
        }

     // for interlace, keep y in multiple of 2
     if( pSrcInfo->u16V_SizeAfterPreScaling > (gSrcInfo[eWindow].ScaledCropWin.y + gSrcInfo[eWindow].ScaledCropWin.height) )
     {
#if SUPPORT_SEAMLESS_ZAPPING
        if( gSrcInfo[eWindow].bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#elif (HW_DESIGN_4K2K_VER == 4)
        if(gSrcInfo[eWindow].bInterlace
           &&
           (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
             ||(psXCInstPri->u32DeviceID == 1)))
#else
        if(gSrcInfo[eWindow].bInterlace )
#endif
         {
             gSrcInfo[eWindow].ScaledCropWin.y = (pSrcInfo->u16V_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.y + gSrcInfo[eWindow].ScaledCropWin.height) + 1) & ~0x1;
         }
         else
         {
             gSrcInfo[eWindow].ScaledCropWin.y = (pSrcInfo->u16V_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.y + gSrcInfo[eWindow].ScaledCropWin.height));
         }
     }
     else
     {
         gSrcInfo[eWindow].ScaledCropWin.y = 0;
         if(gSrcInfo[eWindow].ScaledCropWin.height > pSrcInfo->u16V_SizeAfterPreScaling)
         {
             gSrcInfo[eWindow].ScaledCropWin.height = pSrcInfo->u16V_SizeAfterPreScaling;
         }
         XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[2.b] Force y=%u, height=%u\n",gSrcInfo[eWindow].ScaledCropWin.y, gSrcInfo[eWindow].ScaledCropWin.height);
     }
#ifndef DISABLE_3D_FUNCTION
    #if (HW_DESIGN_3D_VER >= 1)
         if(IS_INPUT_TOP_BOTTOM(eWindow)
            || IS_INPUT_FRAME_PACKING(eWindow)
            || IS_INPUT_FRAME_ALTERNATIVE(eWindow)
            || IS_INPUT_LINE_ALTERNATIVE(eWindow))
        {
             gSrcInfo[eWindow].ScaledCropWin.y /= 2;
        }
        if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
            || IS_INPUT_CHECK_BOARD(eWindow)
            || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
            || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
        {
            gSrcInfo[eWindow].ScaledCropWin.x /= 2;
        }
    #endif
 #endif
}

// In Side by Side input case,  DNROffset will be divided into half.
void MDrv_SC_3D_Adjust_DNROffset(void *pInstance, MS_U16 *pu16DNROffsetTemp,MS_U16 *pu16DNROffset, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
        || IS_INPUT_CHECK_BOARD(eWindow)
        || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
    {
        *pu16DNROffsetTemp = ((*pu16DNROffsetTemp / 2) + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
        *pu16DNROffset = *pu16DNROffsetTemp*2;
        //In Side by Side input case, main and sub set alignment in same way.But in different
        //timing, alignment is different.
        //If we just care DNROffset's alignment.HW will set main DNROffset/2 and sub DNROffset/2,
        //maybe main's DNROffset isn't alignment.
        //eg. 800x600 case:
        //if we don't do *pu16DNROffset = *pu16DNROffsetTemp*2.DNROffset is 800,
        //HW will give main 400 and sub 400.
        //But DNROffsetTemp is 416,so it was croped 16*cropwin.y .
    }
#endif
}

// In Side by Side input case, pixelOffset will be divided into half.
void MDrv_SC_3D_Adjust_PixelOffset(void *pInstance, XC_InternalStatus *pSrcInfo,MS_PHY *pu32FRCM_RPixelOffset,MS_U16 *pu16FRCM_ROffset,MS_U16 *pu16FRCM_ROffset3D, MS_BOOL bLeftSide, SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#ifdef UFO_XC_SUPPORT_DUAL_MIU
    MS_U16 u16Offset = pSrcInfo->u16H_SizeAfterPreScaling;
    MS_U32 u32LeftFactor = pSrcInfo->Status2.stMiuDualModCfg.u32LeftFactor;
    MS_U32 u32RightFactor = pSrcInfo->Status2.stMiuDualModCfg.u32RightFactor;
    if (IsEnableDualMode(eWindow))
    {
        if (IsHMirrorMode(eWindow))
        {
            u16Offset = u16Offset*u32RightFactor /(u32RightFactor + u32LeftFactor);
            u16Offset = (u16Offset + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
            if (bLeftSide == TRUE)
            {
               u16Offset = pSrcInfo->u16H_SizeAfterPreScaling - u16Offset;
            }
        }
        else
        {
            u16Offset = u16Offset*u32LeftFactor /(u32RightFactor + u32LeftFactor);
            u16Offset = (u16Offset + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
            if (bLeftSide == FALSE)
            {
               u16Offset = pSrcInfo->u16H_SizeAfterPreScaling - u16Offset;
            }
        }
    }
#endif

#if (HW_DESIGN_3D_VER >= 2)
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
        || IS_INPUT_CHECK_BOARD(eWindow)
        || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
    {
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            *pu32FRCM_RPixelOffset += (*pu16FRCM_ROffset3D - u16Offset/2);
        }
        else
#endif
        {
            if (Hal_SC_get_pre_align_pixel(pInstance, eWindow)== TRUE)
            {

                *pu32FRCM_RPixelOffset += (gSrcInfo[eWindow].u16IPMFetch - pSrcInfo->u16H_SizeAfterPreScaling)/2;
            }
            else // disable pre-align
            {
                *pu32FRCM_RPixelOffset += (*pu16FRCM_ROffset3D - pSrcInfo->u16H_SizeAfterPreScaling/2);
            }
        }
    }
    else
    {
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            *pu32FRCM_RPixelOffset += (*pu16FRCM_ROffset3D - u16Offset);
        }
        else
#endif
        {
            if (Hal_SC_get_pre_align_pixel(pInstance, eWindow)== TRUE)
            {
                  *pu32FRCM_RPixelOffset += (gSrcInfo[eWindow].u16IPMFetch - pSrcInfo->u16H_SizeAfterPreScaling);
            }
            else // disable pre-align
            {
                 *pu32FRCM_RPixelOffset += (*pu16FRCM_ROffset - pSrcInfo->u16H_SizeAfterPreScaling);
            }
        }
    }
#endif
}
// In Side by Side input case,main and sub's Cropwinwidth are 1/2;
void MDrv_SC_3D_Adjust_CropWinWidth(void *pInstance, MS_U16 *pu16CropWinWidth,SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
      || IS_INPUT_CHECK_BOARD(eWindow)
      || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
      || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
      || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
      || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
    {
        *pu16CropWinWidth /= 2;
        if((*pu16CropWinWidth & 0x1)!= 0)
        {
            *pu16CropWinWidth = (*pu16CropWinWidth+1)& ~1;
        }
    }
#endif
}

// In Side by Side input case,main and sub's Fetch are 1/2;
void MDrv_SC_3D_Adjust_DualOPMFetch(void *pInstance, MS_U16 *pu16Fetch,SCALER_WIN eWindow)
{
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER >= 2)
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
      || IS_INPUT_CHECK_BOARD(eWindow)
      || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
      || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
      || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
      || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
    {
        *pu16Fetch /= 2;
        if((*pu16Fetch & 0x1)!= 0)
        {
            *pu16Fetch = (*pu16Fetch+1)& ~1;
        }
    }
#endif
}


// If 3d is enable for (HW_DESIGN_3D_VER >= 2),
// mem unit will split into L and R, so bytesPerPixel should be doubled.
void MDrv_SC_3D_Adjust_BytesPer2Pixel(void *pInstance, MS_U8 *u8BytesPer2PixelTemp,SCALER_WIN eWindow)
{
     MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow == MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
 #if (HW_DESIGN_3D_VER >= 2)
    //if REG_SC_BK12_30_L bit0 is enabled, we should add into this case.
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
        || IS_INPUT_CHECK_BOARD(eWindow)
        || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
        || IS_INPUT_TOP_BOTTOM(eWindow)
        || IS_INPUT_FRAME_PACKING(eWindow)
        || IS_INPUT_FRAME_ALTERNATIVE(eWindow)
        || IS_INPUT_LINE_ALTERNATIVE(eWindow)
        || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow))
    {
        *u8BytesPer2PixelTemp *= 2;
    }

#endif

}

MS_BOOL MDrv_SC_3D_Is_LR_Sbs2Line(void *pInstance)
{
    MS_BOOL bSbs2Line = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bSbs2Line = Hal_SC_3D_Is_LR_Sbs2Line(pInstance);
    if(bSbs2Line != pXCResourcePrivate->stdrvXC_3D._bIsLR_Sbs2Line)
    {
        printf("[%s,%5d] Attention: the variable _bIsLR_Sbs2Line(%u) is different from reg's value(%u)",
               __FUNCTION__,__LINE__, pXCResourcePrivate->stdrvXC_3D._bIsLR_Sbs2Line, bSbs2Line);
    }
    return bSbs2Line;
}

MS_BOOL MDrv_SC_3D_IsSupportFBL3D()
{
    return HW_3D_SUPPORT_FBL;
}
#endif
MS_BOOL MApi_SC_3D_Is_Skip_Default_LR_Flag_U2(void* pInstance)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#ifndef DISABLE_3D_FUNCTION
    bReturn = pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag;
#else
    bReturn = FALSE;
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_SC_3D_Is_Skip_Default_LR_Flag(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_3D_SKIP_DEFAULT_LR_FLAG XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_3D_SKIP_DEFAULT_LR_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


MS_BOOL MApi_XC_3D_Enable_Skip_Default_LR_Flag_U2(void* pInstance, MS_BOOL bEnable)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#ifndef DISABLE_3D_FUNCTION
    pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag = bEnable;
    bReturn = TRUE;
#else
    UNUSED(bEnable);
    bReturn = FALSE;
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_3D_Enable_Skip_Default_LR_Flag(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_SKIP_DEFAULT_LR_FLAG XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_SKIP_DEFAULT_LR_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#ifndef DISABLE_3D_FUNCTION
//------------------------------------------------------------------------------
//--------HW 2D to 3D CPU Depth Refine Part Begin-------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// the following part codes is copied from ursa5
#if HW_2DTO3D_SUPPORT
#define PIXELNUMOFBLOCK 32
#define MAXFRAME_3DDETECT_COMPATIBLE 64
#define MAXFRAME_3DDETECT            80
#define PERCENTAGE_3DDETECT          88
#define DRBYTENUM       64
#define DDBYTENUM       32
#define MAXVALUE        255

static void _MDrv_SC_3D_2DTo3D_DepthRefine(SC_INT_SRC eIntNum, void *pParam);

#if (HW_2DTO3D_VER < 3) //old 2d to 3d architecture

#define NBLK_H ((MIN(gSrcInfo[MAIN_WINDOW].stDispWin.width, 1920) + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //60 for full hd panel, 43 for sd panel
#define NBLK_V ((MIN(gSrcInfo[MAIN_WINDOW].stDispWin.height, 1080) + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK) //34 for full hd panel, 24 for sd panel

static MS_U32 minmax(MS_U32 u32X, MS_U32 u32Min, MS_U32 u32Max)
{
   return (u32X <= u32Min) ? u32Min :
          (u32X >= u32Max) ? u32Max : u32X;
}

static MS_U32 max(MS_U32 u32A, MS_U32 u32B)
{
   return (u32A <= u32B) ? u32B : u32A;
}

static MS_U32 min(MS_U32 u32A, MS_U32 u32B)
{
   return (u32A <= u32B) ? u32A : u32B;
}

static MS_U32 absdiff(MS_S32 a, MS_S32 b)
{
    return abs(a - b);
}

static MS_U32 slope_gen(void *pInstance, MS_U8 i,
                        MS_U8 u8Slope,
                        MS_U8 u8Element,
                        MS_U8 u8ModSel,
                        MS_U8 u8Motion)
{
    MS_U32 u32Rnd = 1;
    MS_U32 u32Tmp = (MS_U32)i * u8Slope;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (IsVMirrorMode(MAIN_WINDOW))
    {
       u32Tmp = (MS_U32)(VBLOCKNUM - 1 - i) * u8Slope;
    }

    if(u8Motion != 0)
    {
        u32Rnd = (u8ModSel == 0) ? u8Element >> 4:
                 (u8ModSel == 1) ? u8Element >> 3:
                 (u8ModSel == 2) ? u8Element >> 2:
                                   u8Element >> 1;
        u32Tmp = u32Tmp + u32Rnd;
        u32Tmp = (u32Tmp > MAXVALUE) ? MAXVALUE : u32Tmp;
    }
    return u32Tmp;
}

static MS_U8 remap(void *pInstance, MS_U8 u8Cls,
                   MS_U8 u8BlkHght,
                   MS_U8 u8BlkWdth,
                   MS_U8 i,
                   MS_U8 j,
                   MS_U8 u8NrmlEn)//i: block line number, j: block idx
{
    MS_U32 u32Idx = 0;
    MS_U8 u8Shift = 0;
    MS_U32 u32MapIdx = 0;
    MS_U8 u8Result = 0;
    MS_U8 u8Bln_u = 0;
    MS_U8 u8Bln_d = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_3D.s_u8flg == 0)
    {
        MS_U16 u16TotalBlock = HBLOCKNUM * VBLOCKNUM;
        for(u32Idx = 0; u32Idx < MAXINDEX; u32Idx++)
        {
            pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurArray[u32Idx] = 0;
            pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseArray[u32Idx] = 0;
        }
        for(u32Idx = 0; u32Idx < u16TotalBlock; u32Idx++)
        {
            pXCResourcePrivate->stdrvXC_3D._u8RemapLpfArray[u32Idx] = 0;
        }
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurSum = 0;
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseSum = 0;
        pXCResourcePrivate->stdrvXC_3D.s_u8flg = 1;
    }
    pXCResourcePrivate->stdrvXC_3D.s_u8Ptr = pXCResourcePrivate->stdrvXC_3D.s_u8Ptr + 1;
    u8Shift = (i == 5) ? u8BlkWdth-1 :
            (i == 6) ? u8BlkWdth-2 :
            (i == 7) ? u8BlkWdth-3 :
            (i >= 8) ? u8BlkWdth-4 : 0;

    u32MapIdx = i * u8BlkWdth + ( (j + u8Shift) % u8BlkWdth );
    u8Result = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u8Cls][u32MapIdx];

    // noise reduction start
    if(i == 0 && j == 0)
    {
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur = pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur / ((u8BlkHght - 1) * u8BlkWdth);
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse = pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse / u8BlkWdth;
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurSum = pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurSum
                                                           + pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur
                                                           - pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurArray[pXCResourcePrivate->stdrvXC_3D.s_u8Ptr];
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurArray[pXCResourcePrivate->stdrvXC_3D.s_u8Ptr] = pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur;
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseSum = pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseSum
                                                           + pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse
                                                           - pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseArray[pXCResourcePrivate->stdrvXC_3D.s_u8Ptr];
        pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseArray[pXCResourcePrivate->stdrvXC_3D.s_u8Ptr] = pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse;
        pXCResourcePrivate->stdrvXC_3D._u8RemapAvgBias = (MS_U8)( (pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCurSum >> 8)
                                                                 - (pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBseSum >> 8));
    }

    pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur = (i == 1 && j == 0) ? u8Result :
                      (i != 0) ? pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur + (MS_U32)u8Result : pXCResourcePrivate->stdrvXC_3D._u32RemapAvgCur;

    pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse = (i == 0 && j == 0) ? u8Result :
                      (i == 0) ? pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse + (MS_U32)u8Result : pXCResourcePrivate->stdrvXC_3D._u32RemapAvgBse;

    u8Result = ((u8NrmlEn & 0x02) == 2) ? pXCResourcePrivate->stdrvXC_3D._u8RemapAvgBias :
                ( (u8NrmlEn & 0x01) == 1 && i != 0) ? u8Result - pXCResourcePrivate->stdrvXC_3D._u8RemapAvgBias : u8Result;

    pXCResourcePrivate->stdrvXC_3D._u8RemapLpfArray[i * u8BlkWdth + j] = u8Result;
    u8Bln_u = (i == 0) ? 0 : i - 1;
    u8Bln_d = (i == u8BlkHght - 1) ? u8BlkHght - 1 : i + 1;
    u8Bln_u = pXCResourcePrivate->stdrvXC_3D._u8RemapLpfArray[u8Bln_u * u8BlkWdth + j];
    u8Bln_d = pXCResourcePrivate->stdrvXC_3D._u8RemapLpfArray[u8Bln_d * u8BlkWdth + j];

    u8Result = ( (u8NrmlEn & 0x04) == 4) ? ((u8Bln_u + (u8Result  << 1) + u8Bln_d) >> 2 ) : u8Result;
    // noise reduction end

    return u8Result;
}

static MS_U32 lpf3x3(void *pInstance, MS_U32 u32YY, MS_U32 u32XX, MS_U32 u32Index)
{
    MS_U32 d_r = 0, d_l = 0, d_c = 0;
    MS_U32 u_r = 0, u_l = 0, u_c = 0;
    MS_U32 c_r = 0, c_l = 0, c_c = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    c_l = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][u32YY * HBLOCKNUM + u32XX - 1];
    c_c = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][u32YY * HBLOCKNUM + u32XX];
    c_r = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][u32YY * HBLOCKNUM + u32XX + 1];
    u_l = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][(u32YY - 1) * HBLOCKNUM + u32XX - 1];
    u_c = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][(u32YY - 1) * HBLOCKNUM + u32XX];
    u_r = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][(u32YY - 1) * HBLOCKNUM + u32XX + 1];
    d_l = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][(u32YY + 1) * HBLOCKNUM + u32XX - 1];
    d_c = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][(u32YY + 1) * HBLOCKNUM + u32XX];
    d_r = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[u32Index][(u32YY + 1) * HBLOCKNUM + u32XX + 1];

    c_l = (u32XX == 0) ? c_c : c_l;
    c_r = (u32XX == HBLOCKNUM - 1) ? c_c : c_r;
    u_l = ((u32YY == 0) || (u32XX == 0)) ? c_c : u_l;
    u_r = ((u32YY == 0) || (u32XX == HBLOCKNUM - 1)) ? c_c : u_r;
    u_c = (u32YY == 0) ? c_c : u_c;
    d_l = ((u32YY == VBLOCKNUM - 1) || (u32XX == 0)) ? c_c : d_l;
    d_r = ((u32YY == VBLOCKNUM - 1) || (u32XX == HBLOCKNUM - 1)) ? c_c : d_r;
    d_c = (u32YY == VBLOCKNUM - 1) ? c_c : d_c;
    c_c = (u_r + u_c + u_l +
           c_r + c_c + c_l +
           d_r + d_c + d_l) >> 4;

    return c_c;
}

#if 0
static MS_U32 yslp_func(MS_U32 u32VDirEn,
                        MS_U32 u32VDirPn,
                        MS_U32 u32VDirStrd,
                        MS_U32 u32BuildFlag,
                        MS_U32 u32YAxis,
                        MS_U32 u32Result)
{
    MS_U32 u32Slope = (u32VDirPn) ? (VBLOCKNUM - u32YAxis) : u32YAxis;
    MS_U32 u32YResult = (u32BuildFlag) ? u32Result : (u32Slope * u32VDirStrd + ( u32Result - 128 ) / 4 );
    return u32Result = (u32VDirEn) ? u32YResult : u32Result;
}

static MS_U32 vlpf1x3(MS_U32 u32YY, MS_U32 u32XX, MS_U32 u32Index, MS_U32 u32FltrEn)
{
    MS_U32 d_c = 0;
    MS_U32 u_c = 0;
    MS_U32 c_c = 0;

    c_c = _u8FieldArray[u32Index][u32YY * HBLOCKNUM + u32XX];
    u_c = _u8FieldArray[u32Index][(u32YY - 1) * HBLOCKNUM + u32XX];
    d_c = _u8FieldArray[u32Index][(u32YY + 1) * HBLOCKNUM + u32XX];

    if(u32FltrEn)
    {
        u_c = (u32YY == 0)?c_c:u_c;
        d_c = (u32YY == 33)?c_c:d_c;
        c_c = (u_c + (c_c << 1) + d_c) >> 2;
    }
    return c_c;
}
#endif

#define NBLK_PITCH 60 //pitch block for 2d to 3d hw engine
#define COEF (100 * (NBLK_H / 2 - 4) * (NBLK_V - 4) / ((HBLOCKNUM / 2 - 4) * (VBLOCKNUM - 4))) //derived from full hd panel

static void _MDrv_SC_3D_SourceDetection(void *pInstance)
{
    MS_U16 i = 0, j = 0;
    MS_U16 u16C_Index  = 0;
    MS_U8 u8Detect_Con = 0;
    MS_U8 u8Diff_Th = 8;
    MS_U8 u8Reg_A = 0;
    MS_U8 u8Reg_B1 = 0, u8Reg_B2 = 0, u8Reg_B3 = 0, u8Reg_B4 = 0, u8Reg_B5 = 0;
    MS_U8 u8Reg_C1 = 0;
    MS_U8 u8Diff_Frame = 0;
    MS_U16 u16Sum_Tb = 0;
    MS_U16 u16Sum_Ss = 0;
    MS_U16 u16Sum_Ss_Mir = 0;
    MS_U16 u16Sum_Tb_1 = 0;
    MS_U16 u16Sum_Tb_2 = 0;
    MS_U16 u16Sum_Tb_3 = 0;
    MS_U16 u16Sum_Tb_4 = 0;
    MS_U16 u16Sum_Tb_5 = 0;
    MS_U16 u16Sum_Ss_1 = 0;
    MS_U16 u16Sum_Ss_2 = 0;
    MS_U16 u16Sum_Ss_3 = 0;
    MS_U16 u16Sum_Ss_4 = 0;
    MS_U16 u16Sum_Ss_5 = 0;
    MS_U16 u16Diff_Ss  = 0;
    MS_U16 u16HBlockShift = 0;
    MS_U16 u16VBlockShift = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.bEnableOverscan)
    {
        if(gSrcInfo[MAIN_WINDOW].u16HCusScalingSrc > 0)
        {
            u16HBlockShift = (MS_U32)gSrcInfo[MAIN_WINDOW].ScaledCropWin.x
                                 * gSrcInfo[MAIN_WINDOW].u16HCusScalingDst
                                 / gSrcInfo[MAIN_WINDOW].u16HCusScalingSrc / PIXELNUMOFBLOCK;
        }

        if(gSrcInfo[MAIN_WINDOW].u16VCusScalingSrc > 0)
        {
            u16VBlockShift = (MS_U32)gSrcInfo[MAIN_WINDOW].ScaledCropWin.y
                                 * gSrcInfo[MAIN_WINDOW].u16VCusScalingDst
                                 / gSrcInfo[MAIN_WINDOW].u16VCusScalingSrc / PIXELNUMOFBLOCK;
        }
    }

    if(u16HBlockShift > 2)
    {
        u16HBlockShift = 2;
    }

    if(u16VBlockShift > 2)
    {
        u16VBlockShift = 2;
    }

    if(pXCResourcePrivate->stdrvXC_3D._bResetFrameCount)
    {
        pXCResourcePrivate->stdrvXC_3D._u16SBSCount = 0;
        pXCResourcePrivate->stdrvXC_3D._u16TBCount = 0;
        pXCResourcePrivate->stdrvXC_3D._u162DCount = 0;
        pXCResourcePrivate->stdrvXC_3D._u16TotalCount = 0;
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = FALSE;
    }

    // Detect side by side pattern ================================================
    for (i = 2-u16VBlockShift; i < NBLK_V-2-u16VBlockShift; i++)
    {
        u16Sum_Ss_1 = 0;
        u16Sum_Ss_2 = 0;
        u16Sum_Ss_3 = 0;
        u16Sum_Ss_4 = 0;
        u16Sum_Ss_5 = 0;
        for (j = 2-u16HBlockShift; j < NBLK_H/2-2-u16HBlockShift; j++)
        {
            u16C_Index = i*NBLK_PITCH + j;
            u8Reg_A = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index];
            u8Reg_B1 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + NBLK_H / 2 - 2 + u16HBlockShift];
            u8Reg_B2 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + NBLK_H / 2 - 1 + u16HBlockShift];
            u8Reg_B3 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + NBLK_H / 2 + u16HBlockShift];
            u8Reg_B4 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + NBLK_H / 2 + 1 + u16HBlockShift];
            u8Reg_B5 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + NBLK_H / 2 + 2 + u16HBlockShift];
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B1);

            u16Sum_Ss_1 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_1 + 1 : u16Sum_Ss_1;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B2);
            u16Sum_Ss_2 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_2 + 1 : u16Sum_Ss_2;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B3);
            u16Sum_Ss_3 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_3 + 1 : u16Sum_Ss_3;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B4);
            u16Sum_Ss_4 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_4 + 1 : u16Sum_Ss_4;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B5);
            u16Sum_Ss_5 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_5 + 1 : u16Sum_Ss_5;

            if(j < NBLK_H / 2)
            {
                u16C_Index = i*NBLK_PITCH - j;
                u8Reg_C1 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + NBLK_H - 1];
                u8Diff_Frame = absdiff( u8Reg_A, u8Reg_C1);
                u16Sum_Ss_Mir = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_Mir + 1 : u16Sum_Ss_Mir; // for mirror case
            }
        }
        u16Sum_Ss_1 = min(u16Sum_Ss_1, u16Sum_Ss_2);
        u16Sum_Ss_1 = min(u16Sum_Ss_1, u16Sum_Ss_3);
        u16Sum_Ss_1 = min(u16Sum_Ss_1, u16Sum_Ss_4);
        u16Sum_Ss_1 = min(u16Sum_Ss_1, u16Sum_Ss_5);
        u16Sum_Ss = u16Sum_Ss + u16Sum_Ss_1;
    }
    // ==========================================================================

    u16Sum_Ss_1 = 0;
    // Detect top down pattern ================================================
    for (i = 2-u16VBlockShift; i < NBLK_V/2-2-u16VBlockShift; i++)
    {
        u16Sum_Tb_1 = 0;
        u16Sum_Tb_2 = 0;
        u16Sum_Tb_3 = 0;
        u16Sum_Tb_4 = 0;
        u16Sum_Tb_5 = 0;
        for (j = 2-u16HBlockShift; j < NBLK_H-2-u16HBlockShift; j++)
        {
            u16C_Index = i*NBLK_PITCH + j;
            u8Reg_A = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index];
            u16C_Index = u16C_Index + NBLK_V / 2 * NBLK_PITCH;
            u8Reg_B1 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index - 2 + u16HBlockShift];
            u8Reg_B2 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index - 1 + u16HBlockShift];
            u8Reg_B3 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + u16HBlockShift];
            u8Reg_B4 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + 1 + u16HBlockShift];
            u8Reg_B5 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][u16C_Index + 2 + u16HBlockShift];
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B1);
            u16Sum_Tb_1 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Tb_1 + 1 : u16Sum_Tb_1;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B2);
            u16Sum_Tb_2 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Tb_2 + 1 : u16Sum_Tb_2;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B3);
            u16Sum_Tb_3 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Tb_3 + 1 : u16Sum_Tb_3;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B4);
            u16Sum_Tb_4 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Tb_4 + 1 : u16Sum_Tb_4;
            u8Diff_Frame = absdiff( u8Reg_A, u8Reg_B5);
            u16Sum_Tb_5 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Tb_5 + 1 : u16Sum_Tb_5;

            if(i < NBLK_V / 2)
            {
                u8Reg_C1 = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][(NBLK_V - 1 - i)*NBLK_PITCH + j];
                u8Diff_Frame = absdiff( u8Reg_A, u8Reg_C1);
                u16Sum_Ss_1 = (u8Diff_Frame > u8Diff_Th) ? u16Sum_Ss_1 + 1 : u16Sum_Ss_1; // for mirror case
            }
        }
        u16Sum_Tb_1 = min(u16Sum_Tb_1, u16Sum_Tb_2);
        u16Sum_Tb_1 = min(u16Sum_Tb_1, u16Sum_Tb_3);
        u16Sum_Tb_1 = min(u16Sum_Tb_1, u16Sum_Tb_4);
        u16Sum_Tb_1 = min(u16Sum_Tb_1, u16Sum_Tb_5);
        u16Sum_Tb= u16Sum_Tb + u16Sum_Tb_1;
    }
    // ==========================================================================

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16Sum_Tb=%u, u16Sum_Ss=%u, mirror_ss=%u, mirror_tb=%u\n", u16Sum_Tb, u16Sum_Ss, u16Sum_Ss_Mir, u16Sum_Ss_1);

    // Make decision
    if(absdiff(u16Sum_Tb, u16Sum_Ss)>=100*COEF/100)
    {
        if((u16Sum_Ss < 50*COEF/100) && ((u16Sum_Ss*8) < u16Sum_Tb))
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if((u16Sum_Ss < 100*COEF/100) && ((u16Sum_Ss*3) < u16Sum_Tb))
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if((u16Sum_Ss < 150*COEF/100) && ((u16Sum_Ss*12/5) < u16Sum_Tb))
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if((u16Sum_Ss < 200*COEF/100) && ((u16Sum_Ss*9/4) < u16Sum_Tb))
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if((u16Sum_Ss < 250*COEF/100) && ((u16Sum_Ss*2) < u16Sum_Tb))
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if(u16Sum_Ss*2 < u16Sum_Tb)
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if((u16Sum_Tb < 50*COEF/100) && ((u16Sum_Tb*8) < u16Sum_Ss))
        {
            u8Detect_Con = 2; // Source : TB
        }
        else if((u16Sum_Tb < 100*COEF/100) && ((u16Sum_Tb*3) < u16Sum_Ss))
        {
            u8Detect_Con = 2; // Source : TB
        }
        else if((u16Sum_Tb < 150*COEF/100) && ((u16Sum_Tb*9/4) < u16Sum_Ss))
        {
            u8Detect_Con = 2; // Source : TB
        }
        else if((u16Sum_Tb < 220*COEF/100) && ((u16Sum_Tb*2) < u16Sum_Ss))
        {
            u8Detect_Con = 2; // Source : TB
        }
        else if((u16Sum_Tb < 250*COEF/100) && ((u16Sum_Tb*9/5) < u16Sum_Ss))
        {
            u8Detect_Con = 2; // Source : TB
        }
        else if(u16Sum_Tb*2 < u16Sum_Ss)
        {
            u8Detect_Con = 2; // Source : TB
        }
        else
        {
            u8Detect_Con = 3; // Source : 2D
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d 1\n");
        }
    }
    else
    {
        if((u16Sum_Ss < 50*COEF/100) && ((u16Sum_Ss*8) < u16Sum_Tb))
        {
            u8Detect_Con = 1; // Source : SS
        }
        else if((u16Sum_Tb < 50*COEF/100) && ((u16Sum_Tb*8) < u16Sum_Ss))
        {
            u8Detect_Con = 2; // Source : TB
        }
        else
        {
            u8Detect_Con = 3; // Source : 2D
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d 2\n");
        }
    }

    // Correct some wrong decision
    u16Diff_Ss = absdiff(u16Sum_Ss, u16Sum_Ss_Mir);

    if((u8Detect_Con == 2) && (u16Sum_Ss_1 <= u16Sum_Tb*9/5))
    {
        u8Detect_Con = 3; // Source : 2D
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d 3\n");
    }
    else if((u8Detect_Con == 1) && (u16Diff_Ss < 40*COEF/100) && (u16Sum_Ss_Mir < 250*COEF/100))
    {
        u8Detect_Con = 3; // Source : 2D
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d 4\n");
    }
    else if((u8Detect_Con==3) && (u16Sum_Ss_Mir > u16Sum_Ss*3/2) && (u16Sum_Ss*3/2 < u16Sum_Tb))
    {
        u8Detect_Con = 1; // Source : SS
    }
    else if((u8Detect_Con==3) && (u16Sum_Ss_1 > u16Sum_Tb*3/2) && (u16Sum_Tb*3/2 < u16Sum_Ss))
    {
        u8Detect_Con = 2; // Source : TB
    }

    ///frame statistics
    pXCResourcePrivate->stdrvXC_3D._u16TotalCount++;
    if(u8Detect_Con == 1)
    {
        pXCResourcePrivate->stdrvXC_3D._u16SBSCount++;
    }
    else if(u8Detect_Con == 2)
    {
        pXCResourcePrivate->stdrvXC_3D._u16TBCount++;
    }
    else if(u8Detect_Con == 3)
    {
        pXCResourcePrivate->stdrvXC_3D._u162DCount++;
    }

    if(E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
    {
        if(pXCResourcePrivate->stdrvXC_3D._u16TotalCount >= MAXFRAME_3DDETECT)
        {
            pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d_cnt=%u,ss_cnt=%u,tb_cnt=%u\n"
                                                , pXCResourcePrivate->stdrvXC_3D._u162DCount
                                                , pXCResourcePrivate->stdrvXC_3D._u16SBSCount
                                                , pXCResourcePrivate->stdrvXC_3D._u16TBCount);
            if((pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u16TBCount)
                && (pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                if(pXCResourcePrivate->stdrvXC_3D._u16SBSCount >= PERCENTAGE_3DDETECT * pXCResourcePrivate->stdrvXC_3D._u16TotalCount / 100)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "side by side\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
                }
            }
            else if((pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u16SBSCount)
                    && (pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                if(pXCResourcePrivate->stdrvXC_3D._u16TBCount >= PERCENTAGE_3DDETECT * pXCResourcePrivate->stdrvXC_3D._u16TotalCount / 100)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "top bottom\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_TOP_BOTTOM;
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
            }
        }
    }
    else if(E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
    {
        if(pXCResourcePrivate->stdrvXC_3D._u16TotalCount >= MAXFRAME_3DDETECT_COMPATIBLE)
        {
            pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d_cnt=%u,ss_cnt=%u,tb_cnt=%u\n"
                                              , pXCResourcePrivate->stdrvXC_3D._u162DCount
                                              , pXCResourcePrivate->stdrvXC_3D._u16SBSCount
                                              , pXCResourcePrivate->stdrvXC_3D._u16TBCount);
            if((pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u16TBCount)
                && (pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "side by side\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
            }
            else if((pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u16SBSCount)
                && (pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "top bottom\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_TOP_BOTTOM;
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
            }
            pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount++;
        }
    }
}

static MS_U32 _MDrv_SC_3D_2DTo3D_Art_Detect(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 i = 0;
    MS_U32 u32HisgrmArray[16];
    MS_U32 u32XStart =  512;
    MS_U32 u32XEnd   =  1408;    //896
    MS_U32 u32YStart =  256;     //640
    MS_U32 u32YEnd   =  896;
    MS_U32 u32PureColorBlkCnt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_3D_L); //16bit 3d => 8bit 7a,7b
    MS_U32 u32GradLevelBlkCnt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_3E_L); //16bit 3e => 8bit 7c,7d
    MS_U32 u32HighFreqBlkCnt  = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_3F_L); //16bit 3f => 8bit 7e,7f
    MS_U32 u32HisWeightCur = 0;
    MS_U32 u32MinRatio = 0;
    MS_U32 u32NonSmallHistogramCnt = 0;
    MS_U32 u32NonzeroHistogramCnt = 0;
    MS_U32 u32BiggestHistogram = 0;
    MS_U32 u32BiggestHistogramIdx = 0;
    MS_U32 u32SecondHistogram = 0;
    MS_U32 u32SecondHistogramIdx = 0;
    MS_U32 u32ThirdHistogram = 0;
    MS_U32 u32TempRatio1 = 0;
    MS_U32 u32TempRatio2 = 0;
    MS_U32 u32ArtWeightCurHF = 0;    // HF weight
    MS_U32 u32ArtIndex = 0;
    MS_U32 u32ArtWeightCurAll = 0;
    MS_U32 u32ArtWeightCur = 0;
    MS_U32 u32X1        = 0;
    MS_U32 u32DBit0     =  0;
    MS_U32 u32DBit1     =  0;
    MS_U32 u32DBit2     =  0;
    MS_U32 u32DBit3     =  0;
    MS_U32 u32DitherOut =  0;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // read histogram
    u32HisgrmArray[0] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_30_L, 0xFF00) >> 8;
    u32HisgrmArray[1] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_30_L, 0x00FF);
    u32HisgrmArray[2] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_31_L, 0xFF00) >> 8;
    u32HisgrmArray[3] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_31_L, 0x00FF);
    u32HisgrmArray[4] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_32_L, 0xFF00) >> 8;
    u32HisgrmArray[5] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_32_L, 0x00FF);
    u32HisgrmArray[6] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_33_L, 0xFF00) >> 8;
    u32HisgrmArray[7] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_33_L, 0x00FF);
    u32HisgrmArray[8] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_34_L, 0xFF00) >> 8;
    u32HisgrmArray[9] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_34_L, 0x00FF);
    u32HisgrmArray[10] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_35_L, 0xFF00) >> 8;
    u32HisgrmArray[11] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_35_L, 0x00FF);
    u32HisgrmArray[12] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_36_L, 0xFF00) >> 8;
    u32HisgrmArray[13] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_36_L, 0x00FF);
    u32HisgrmArray[14] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_37_L, 0xFF00) >> 8;
    u32HisgrmArray[15] = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_37_L, 0x00FF);

    //------------- Set Detection Window --------------------------
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_28_L, u32XStart);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_29_L, u32XEnd);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_2A_L, u32YStart);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_2B_L, u32YEnd);

    u32X1 = ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x01000000) >> 24) ^ ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00001000) >> 12);
    u32DBit0 =  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00000008) >> 3)   ^  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00001000) >> 12);  // bit 3    ^  bit 12
    u32DBit1 =  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00000040) >> 6)   ^  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00040000) >> 18);  // bit 6    ^  bit 18
    u32DBit2 =  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x01000000) >> 24)  ^  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x40000000) >> 30);  // bit 24   ^  bit 30
    u32DBit3 =  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00000200) >> 9)   ^  ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x00100000) >> 20);  // bit  9   ^  bit  20
    u32DitherOut =  (u32DBit3 << 3) + (u32DBit2 << 2) + (u32DBit1 << 1) + u32DBit0;

    pXCResourcePrivate->stdrvXC_3D._u32X = ((pXCResourcePrivate->stdrvXC_3D._u32X & 0x7fffffff) << 1) + u32X1;

    u32HisWeightCur = 0;
    u32SecondHistogram = 0;
    u32BiggestHistogram = 0;

    for(i = 0; i < 16; i++)
    {
        if(u32HisgrmArray[i] >= 0x3)
        {
            u32NonSmallHistogramCnt++;
        }

        if(u32HisgrmArray[i] > 0)
        {
            u32NonzeroHistogramCnt++;
        }

        if(u32HisgrmArray[i] == 0x0)
        {
            u32HisgrmArray[i] = 0x1;
        }

        if(u32HisgrmArray[i] > u32BiggestHistogram)
        {
            u32BiggestHistogram = u32HisgrmArray[i];
            u32BiggestHistogramIdx = i;
        }
    }

    for(i = 0; i < 16; i++)
    {
        if(i != u32BiggestHistogramIdx && u32HisgrmArray[i] >= u32SecondHistogram)
        {
            u32SecondHistogram = u32HisgrmArray[i];
            u32SecondHistogramIdx = i;
        }
    }

    for(i = 0; i < 16; i++)
    {
        if(i != u32BiggestHistogramIdx && i != u32SecondHistogramIdx && u32HisgrmArray[i] >= u32ThirdHistogram)
        {
            u32ThirdHistogram = u32HisgrmArray[i];
        }
    }

    for(i = 0; i < 16; i++)
    {
        if(i == 0)
        {
            u32MinRatio = min(u32HisgrmArray[0] / (u32HisgrmArray[1] + u32HisgrmArray[2]), 8);
            if(u32HisgrmArray[0] > 2 * u32HisgrmArray[1])
            {
                u32HisWeightCur = u32HisWeightCur + min((u32HisgrmArray[0] * u32MinRatio)/64, 8);
            }
        }
        else if (i == 15)
        {
            u32MinRatio = min(u32HisgrmArray[15] / (u32HisgrmArray[14] + u32HisgrmArray[13]), 8);
            if(u32HisgrmArray[15] > 2 * u32HisgrmArray[14])
            {
                u32HisWeightCur = u32HisWeightCur + min((u32HisgrmArray[15] * u32MinRatio) / 64, 8);
            }
        }
        else
        {
            u32MinRatio = min((u32HisgrmArray[i] / ((u32HisgrmArray[i + 1]) + (u32HisgrmArray[i - 1]))), 8);
            if((u32HisgrmArray[i] > 2 * u32HisgrmArray[i + 1]) && (u32HisgrmArray[i] > 2 * u32HisgrmArray[i - 1]))
            {
                u32HisWeightCur = u32HisWeightCur + min((u32HisgrmArray[i] * u32MinRatio) / 64, 8);
            }
        }
    }

    u32TempRatio1 = (u32BiggestHistogram / (u32SecondHistogram+u32ThirdHistogram + (u32NonzeroHistogramCnt + u32NonSmallHistogramCnt) / 3));
    u32TempRatio2 = ((u32BiggestHistogram + u32SecondHistogram)/(u32ThirdHistogram + u32NonzeroHistogramCnt + u32NonSmallHistogramCnt) );

    u32TempRatio1 = min(u32TempRatio1, 12);
    u32TempRatio2 = min(u32TempRatio2, 8);

    if(u32TempRatio1 <= 2)
    {
        u32TempRatio1 = 0;
    }
    if(u32TempRatio2 <= 3)
    {
        u32TempRatio2 = 0;
    }

    u32HisWeightCur = u32HisWeightCur + u32TempRatio1 + u32TempRatio2;

    if((u32BiggestHistogram + u32SecondHistogram + u32ThirdHistogram) * 18000 > u32PureColorBlkCnt * 128 * 4)
    {
        u32HisWeightCur = u32HisWeightCur / 3;
    }
    else if((u32BiggestHistogram + u32SecondHistogram + u32ThirdHistogram) * 18000 > u32PureColorBlkCnt * 128 * 3)
    {
       u32HisWeightCur = u32HisWeightCur / 2;
    }
    else if((u32BiggestHistogram + u32SecondHistogram + u32ThirdHistogram) * 18000 > u32PureColorBlkCnt * 128 * 2)
    {
       u32HisWeightCur = u32HisWeightCur * 2 / 3;
    }

    if(u32HisWeightCur <= 5)
    {
        u32HisWeightCur = 0;
    }

    u32HisWeightCur = min(u32HisWeightCur, 15);

    pXCResourcePrivate->stdrvXC_3D._u32HisWeight = (u32HisWeightCur * 1 + pXCResourcePrivate->stdrvXC_3D._u32HisWeight * 15 + u32DitherOut) / 16;

    pXCResourcePrivate->stdrvXC_3D._u32HisWeight     = min(pXCResourcePrivate->stdrvXC_3D._u32HisWeight    , 15);

    if(u32HighFreqBlkCnt >= 450 )
    {
        u32ArtWeightCurHF = 15;
    }
    else if(u32HighFreqBlkCnt > 300 )
    {
        u32ArtWeightCurHF = 8;
    }
    else
    {
        u32ArtWeightCurHF = 0;
    }

    // All weight
    u32ArtIndex = (u32PureColorBlkCnt / 4 * (min(u32HisWeightCur, 8) / 8)) + u32GradLevelBlkCnt;
    u32ArtWeightCurAll = 0;
    if(u32ArtIndex >= 3500)
    {
        u32ArtWeightCurAll = 15;
    }
    else if(u32ArtIndex <= 2600)
    {
        u32ArtWeightCurAll = 0;
    }
    else
    {
        u32ArtWeightCurAll = (u32ArtIndex - 2600) * 15 / 900;
    }

    u32ArtWeightCur =max(u32ArtWeightCurHF, u32ArtWeightCurAll);
    u32ArtWeightCur = min(u32ArtWeightCur, 15);
    pXCResourcePrivate->stdrvXC_3D._u32ArtWeight = (u32ArtWeightCur * 1 + pXCResourcePrivate->stdrvXC_3D._u32ArtWeight * 15 + u32DitherOut) / 16;
    pXCResourcePrivate->stdrvXC_3D._u32ArtWeight     = min(pXCResourcePrivate->stdrvXC_3D._u32ArtWeight, 15);
    pXCResourcePrivate->stdrvXC_3D._u32ReturnWeightCur = max(pXCResourcePrivate->stdrvXC_3D._u32HisWeight, pXCResourcePrivate->stdrvXC_3D._u32ArtWeight);
    pXCResourcePrivate->stdrvXC_3D._u32ReturnWeightCur = min(pXCResourcePrivate->stdrvXC_3D._u32ReturnWeightCur, 15);
    pXCResourcePrivate->stdrvXC_3D._u32ReturnWeight     = (pXCResourcePrivate->stdrvXC_3D._u32ReturnWeightCur + pXCResourcePrivate->stdrvXC_3D._u32ReturnWeight * 7 + (u32DitherOut >> 1)) / 8;

    return pXCResourcePrivate->stdrvXC_3D._u32ReturnWeight;
}

static void _MDrv_SC_3D_2DTo3D_DepthRefine(SC_INT_SRC eIntNum, void *pParam)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 i = 0, j = 0;
    MS_VIRT u32DDBase = MsOS_PA2KSEG1(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf);
    if(0 == u32DDBase)
    {
        printf("%s :u32DDBase = 0!,error!!!\n", __FUNCTION__);
        assert(u32DDBase != 0);
    }
    MS_VIRT u32DRBase = (MS_U32)MsOS_PA2KSEG1(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf);
    if(0 == u32DRBase)
    {
        printf("%s :u32DRBase = 0!,error!!!\n", __FUNCTION__);
        assert(u32DRBase != 0);
    }
    MS_U8  u8DepthMode = 0;
    MS_VIRT u32Base =  u32DDBase;
    MS_U32 u32ArtWt = 0;
    MS_U32 u32NatWt = 0;
    MS_U8  u8ArtWt_t = 0;
    MS_U8  u8YCtr_Cur = 0;
    MS_U8  u8LoopVIndex = VBLOCKNUM;
    MS_U8  u8LoopHIndex = HBLOCKNUM;

    eIntNum = eIntNum;
    pParam = pParam;

    if(IS_INPUT_NORMAL_2D_HW(MAIN_WINDOW))
    {
        if(pXCResourcePrivate->stdrvXC_3D.bFirstFlag) //initial
        {
            MS_U16 u16TotalBlock = HBLOCKNUM * VBLOCKNUM;
            for(j = 0; j < u16TotalBlock; j++)
            {
                pXCResourcePrivate->stdrvXC_3D._32YSumArray[j] = 0;
            }

            for(i = 0; i < MAXINDEX; i++)
            {
                for(j = 0; j < u16TotalBlock; j++)
                {
                    pXCResourcePrivate->stdrvXC_3D._u8YCtrArray[i][j] = 0;
                }
            }
        }

        for(i = 0; i < VBLOCKNUM; i++)
        {
            for(j = 0; j < HBLOCKNUM; j++)
            {
                pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YMAX][i * HBLOCKNUM + j]  = *((volatile MS_U8*)(( i * HBLOCKNUM * DDBYTENUM + j * DDBYTENUM + YMAXBUFINDEX) + u32Base));//y max
                pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YMIN][i * HBLOCKNUM + j]  = *((volatile MS_U8*)(( i * HBLOCKNUM * DDBYTENUM + j * DDBYTENUM + YMINBUFINDEX) + u32Base));//y min
                pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YCTRS][i * HBLOCKNUM + j]  = minmax ((pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YMAX][i * HBLOCKNUM + j] - pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YMIN][i * HBLOCKNUM + j]), 0, MAXVALUE);
#if 0
                _u8FieldArray[YAVG][i * HBLOCKNUM + j]  = *((volatile MS_U8*)(( i * HBLOCKNUM * DDBYTENUM + j * DDBYTENUM + YAVGBUFINDEX) + u32Base));//y avg
                _u8FieldArray[HEDGE][i * HBLOCKNUM + j]  = *((volatile MS_U8*)(( i * HBLOCKNUM * DDBYTENUM + j * DDBYTENUM + YHEDGEBUFINDEX) + u32Base));//prog cnt 0
                _u8FieldArray[VEDGE][i * HBLOCKNUM + j]  = *((volatile MS_U8*)(( i * HBLOCKNUM * DDBYTENUM + j * DDBYTENUM + YVEDGEBUFINDEX) + u32Base));//prog cnt 1
                _u8FieldArray[BUILDING][i * HBLOCKNUM + j]  = max(_u8FieldArray[HEDGE][i * HBLOCKNUM + j], _u8FieldArray[VEDGE][i * HBLOCKNUM + j]);
#endif
                if(pXCResourcePrivate->stdrvXC_3D._bIsHW2Dto3DPatchEnabled)
                {
                    u8YCtr_Cur = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YCTRS][i * HBLOCKNUM + j];
                    pXCResourcePrivate->stdrvXC_3D._32YSumArray[i * HBLOCKNUM + j] = pXCResourcePrivate->stdrvXC_3D._32YSumArray[i * HBLOCKNUM + j] - pXCResourcePrivate->stdrvXC_3D._u8YCtrArray[pXCResourcePrivate->stdrvXC_3D.u8Ptr][i * HBLOCKNUM + j] + u8YCtr_Cur;
                    pXCResourcePrivate->stdrvXC_3D._u8FieldArray[BLUE][i * HBLOCKNUM + j] = pXCResourcePrivate->stdrvXC_3D._32YSumArray[i * HBLOCKNUM + j] >> 8;
                    pXCResourcePrivate->stdrvXC_3D._u8YCtrArray[pXCResourcePrivate->stdrvXC_3D.u8Ptr][i * HBLOCKNUM + j]= u8YCtr_Cur;
                }
                else
                {
#if 1
                    u8YCtr_Cur = pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YCTRS][i * HBLOCKNUM + j];
                    pXCResourcePrivate->stdrvXC_3D._32YSumArray[i * HBLOCKNUM + j] = pXCResourcePrivate->stdrvXC_3D._32YSumArray[i * HBLOCKNUM + j] - pXCResourcePrivate->stdrvXC_3D._u8YCtrArray[pXCResourcePrivate->stdrvXC_3D.u8Ptr][i * HBLOCKNUM + j] + u8YCtr_Cur;
                    pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YCTRS][i * HBLOCKNUM + j] = pXCResourcePrivate->stdrvXC_3D._32YSumArray[i * HBLOCKNUM + j] >> 8;
                    pXCResourcePrivate->stdrvXC_3D._u8YCtrArray[pXCResourcePrivate->stdrvXC_3D.u8Ptr][i * HBLOCKNUM + j]= u8YCtr_Cur;
#else
                    _u8YCtrArray[u8Ptr][i * HBLOCKNUM + j] = _u8FieldArray[YCTRS][i * HBLOCKNUM + j];
//                    u8YCtr_Cur = _u8FieldArray[YCTRS][i * HBLOCKNUM + j];
//                    _32YSumArray[i * HBLOCKNUM + j] = _32YSumArray[i * HBLOCKNUM + j] - _u8YCtrArray[u8Ptr][i * HBLOCKNUM + j] + u8YCtr_Cur;
//                    _u8FieldArray[YCTRS][i * HBLOCKNUM + j] = _32YSumArray[i * HBLOCKNUM + j] >> 8;
//                    _u8YCtrArray[u8Ptr][i * HBLOCKNUM + j]= u8YCtr_Cur;
#endif
                }
            }
        }
        pXCResourcePrivate->stdrvXC_3D.u8Ptr++;
        pXCResourcePrivate->stdrvXC_3D.bFirstFlag = FALSE;

        if (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ArtificialGain != 0)
        {
            u8ArtWt_t         = (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ArtificialGain == MAXARTGAIN) ? MAXARTGAIN + 1 : pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ArtificialGain;
            u32ArtWt = _MDrv_SC_3D_2DTo3D_Art_Detect(pInstance);
            u32NatWt    = MAXARTGAIN - min((u32ArtWt * u8ArtWt_t) >> 4, MAXARTGAIN);
        }
        else
        {
            u32NatWt    = MAXARTGAIN;
        }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//left depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//right depth offset
#if (HW_2DTO3D_VER < 2)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, ((pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain*u32NatWt)>>4)<<8, 0x3F00);//left depth gain:6bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, ((pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain*u32NatWt)>>4)<<8, 0x3F00);//right depth gain:6bits
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, ((pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain*u32NatWt)>>4)<<8, 0x7F00);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, ((pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain*u32NatWt)>>4)<<8, 0x7F00);//right depth gain:7bits
#endif
        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_6C_L,BIT(1)))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, 0x7F00, 0x7F00);//left depth offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, 0x3F00, 0x7F00);//right depth offset
        }
        if(pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EdgeBlackWidth > 0)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_72_L, BIT(7), BIT(7));                               //draw left/right black enable
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_72_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EdgeBlackWidth, 0x007F);  //draw left/right black width
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_72_L, 0x0000, 0x00FF);  //draw left/right black enable, draw left/right black width
        }

        if(IS_OUTPUT_TOP_BOTTOM_HW())
        {
            u8LoopVIndex = u8LoopVIndex / 2;
        }
        else if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            u8LoopHIndex = u8LoopHIndex / 2;
        }

        for(i = 0; i < u8LoopVIndex; i++)
        {
            for(j = 0; j < u8LoopHIndex; j++)
            {
                MS_U32 u32Result = 0;
                MS_U8  u8Res = 0;
                if(pXCResourcePrivate->stdrvXC_3D._bIsHW2Dto3DPatchEnabled)
                {
                    u8Res = (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel == 0) ? remap(pInstance, YCTRS, VBLOCKNUM, HBLOCKNUM, i, j, 0x05) :
                            (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel == 1) ? remap(pInstance, YAVG,  VBLOCKNUM, HBLOCKNUM, i, j, 0x05) :
                            (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel == 2) ? remap(pInstance, BLUE,  VBLOCKNUM, HBLOCKNUM, i, j, 0x05) : 0;
                }
                else
                {
                    pXCResourcePrivate->stdrvXC_3D._u8FieldArray[BLUE][i * HBLOCKNUM + j]    = lpf3x3(pInstance, i,j,YCTRS);
                    u8Res = (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel == 0) ? pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YCTRS][i*HBLOCKNUM + j]:
                            (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel == 1) ? pXCResourcePrivate->stdrvXC_3D._u8FieldArray[YAVG ][i*HBLOCKNUM + j]:
                            (pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel == 2) ? pXCResourcePrivate->stdrvXC_3D._u8FieldArray[BLUE ][i*HBLOCKNUM + j]: 0;
                }
                switch(u8DepthMode)
                {
                    case 0:
                        u32Result = slope_gen(pInstance, i, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Concave, u8Res, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ModSel, 1);// V gradual pattern test
                        break;
                    case 1:
                        u32Result = MAXVALUE - (7 * i);// V gradual pattern test (inverse)
                        break;
                    case 2:
                        u32Result = 4 * j;// H gradual pattern test
                        break;
                    case 3:
                        u32Result = MAXVALUE - (4 * j);// H gradual pattern test (inverse)
                        break;
                    case 4:
                        u32Result = (abs(i - VBLOCKNUM / 2) + abs(j - HBLOCKNUM / 2)) <<3;// center gradual pattern test
                        break;
                    default:
                        u32Result = 7 * i;// V gradual pattern test
                        break;
                }
#if 0
                u32Result = minmax(u32Result, 0, MAXVALUE);
                u32Result = (j == 0 || j >= (((gSrcInfo[MAIN_WINDOW].stDispWin.width + PIXELNUMOFBLOCK - 1)/PIXELNUMOFBLOCK)-1)) ? _st3DHw2DTo3DPara.u16Offset : u32Result;//left/rightest side no 2d3d effect
                (*((volatile MS_U8*)((i * DRBYTENUM + j) + u32DRBase))) = u32Result;
#else
                u32Result = minmax(u32Result, 0, MAXVALUE);
                u32Result = ((j == 0) || (j >= (u8LoopHIndex-1))) ? pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset : u32Result;
                (*((volatile MS_U8*)((i * DRBYTENUM + j) + u32DRBase))) = u32Result;
                if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()&&(j == (u8LoopHIndex-1)))   //hardware SBS patch for disorder
                {
                    (*((volatile MS_U8*)((i * DRBYTENUM + j+1) + u32DRBase))) = u32Result;
                }
#endif
                if(IS_OUTPUT_TOP_BOTTOM_HW())
                {
                    (*((volatile MS_U8*)(((i + u8LoopVIndex) * DRBYTENUM + j) + u32DRBase))) = MAXVALUE - u32Result;
                }
                else if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
                {
                    (*((volatile MS_U8*)((i * DRBYTENUM + j + u8LoopHIndex) + u32DRBase))) = MAXVALUE - u32Result;
                }
            }
        }
        MsOS_FlushMemory();
    }
    else if(((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
                && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D)
            || ((E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
                && pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D))
    {
        if(!pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank)
        {
            u32Base += DDBANKOFFSET;
        }

        if(pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank)//write bank0 currently, let hw to write bank1 for next frame
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_41_L, 0x0001, 0x000F);   // DD force bank1
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_41_L, 0x0000, 0x000F);   // DD force bank0
        }
        pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank = !pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank;

        for(i = 0; i < VBLOCKNUM; i++)
        {
            for(j = 0; j < HBLOCKNUM; j++)
            {
                pXCResourcePrivate->stdrvXC_3D._u8FieldArray[VEDGE][i * HBLOCKNUM + j]  = *((volatile MS_U8*)(( i * HBLOCKNUM * DDBYTENUM + j * DDBYTENUM + YVEDGEBUFINDEX) + u32Base));//prog cnt 1
            }
        }
        _MDrv_SC_3D_SourceDetection(pInstance);
    }
}

#else //new 2d to 3d architecture

#define MINMAX(v,a,b)             (((v)<(a))? (a) : ((v)>(b)) ? (b) : (v))
#define MAX3(a,b,c) MAX(a,MAX(b,c))
#define MIN3(a,b,c) MIN(a,MIN(b,c))
#define BLEND128( a, b, r )  ((a*(128-r)+b*r+64)>>7)
#define LBOX_BLKCOLOR 3
#define BLKCOLOR 32
#if (HW_2DTO3D_VER == 4)
#define DISPLAY_DD_H (pXCResourcePrivate->stdrvXC_3D._u16DDHSize)
#define DISPLAY_DD_V (pXCResourcePrivate->stdrvXC_3D._u16DDVSize)
#define NBLK_DD_H ((DISPLAY_DD_H + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dd h block
#define NBLK_DD_V ((DISPLAY_DD_V + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dd v block
#define NBLK_DD_H_MAX ((1920 + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dd max h block
#define NBLK_DD_V_MAX ((1080 + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dd max v block
#define NBLK_DR_H (pXCResourcePrivate->stdrvXC_3D._u8NBLK_DR_H)
#define NBLK_DR_V (pXCResourcePrivate->stdrvXC_3D._u8NBLK_DR_V)
#define PANEL_WIDTH (pXCResourcePrivate->stdrvXC_3D._u16PanelWidth)
#define PANEL_HEIGHT (pXCResourcePrivate->stdrvXC_3D._u16PanelHeight)

//#define NBLK_DR_H ((gSrcInfo[MAIN_WINDOW].stDispWin.width + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dr h block
//#define NBLK_DR_V ((gSrcInfo[MAIN_WINDOW].stDispWin.height + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK) //dr v block
#else

static MS_U16 _GetCurDispWinWidth(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U16 u16MainDispWidth = gSrcInfo[MAIN_WINDOW].stDispWin.width;
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDispWin);
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        u16MainDispWidth = stDispWin.width;
    }
    return u16MainDispWidth;
}
static MS_U16 _GetCurDispWinHeight(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U16 u16MainDispHeight = gSrcInfo[MAIN_WINDOW].stDispWin.height;
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDispWin);
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        u16MainDispHeight = stDispWin.height;
    }
    return u16MainDispHeight;
}

#define DISPLAY_DD_H MIN(_GetCurDispWinWidth(pInstance), 1920)
#define DISPLAY_DD_V MIN(_GetCurDispWinHeight(pInstance), 1080)
#define NBLK_DD_H ((DISPLAY_DD_H + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dd h block
#define NBLK_DD_V ((DISPLAY_DD_V + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dd v block
#define NBLK_DR_H ((_GetCurDispWinWidth(pInstance) + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)  //dr h block
#define NBLK_DR_V ((_GetCurDispWinHeight(pInstance) + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK) //dr v block
#endif
#define NBLK_DD_PITCH  60
#define FHD_W 1920
#define FHD_H 1080
#define DVIEW_WEI 0
#define DVIEW_WEI2 1
#define ENABLE_CrDEPTH 1
#define FRAME_WIDTH_DLC 1920 //need DLC information
#define FRAME_HEIGHT_DLC 1080 //need DLC information
#define HW_2DTO3D_3DDetect_THRESHOLD 20
#define HW_2DTO3D_3DDetect_PERCENTAGE 40
#define PRINTINFO 0
#define BOUNDARY_MODE 1
#define AUTO_ADJUST 0
#define BACKLIGHTIIR 1
#define PATTERN_THRESLD 10
#if (HW_2DTO3D_VER == 4)
//Ursa9 new
#define DUMYGAINCTRL  0//LG use it

//DepthDectectionBLK
#define MOTIONDETECTION 1
#define OPTIMIZE 1
#endif
typedef enum
{
    E_3D_FORMAT_2D,
    E_3D_FORMAT_TB,
    E_3D_FORMAT_SS,
    E_3D_FORMAT_SS_PATTERN,
    E_3D_FORMAT_TB_PATTERN,
}EN_3D_FORMAT;

PIXEL RGB2YUV(PIXEL stIn)
{
    MS_S32 s32Y = 0, s32U = 0, s32V = 0;
    PIXEL stOut;

    stOut.R = 0;
    stOut.G = 0;
    stOut.B = 0;

    s32Y = ( ( 257*stIn.R + 504*stIn.G +  98*stIn.B +  16000)*4+500 ) / 1000;
    s32U = ( (-148*stIn.R - 291*stIn.G + 439*stIn.B + 128000)*4+500 ) / 1000;
    s32V = ( ( 439*stIn.R - 368*stIn.G -  71*stIn.B + 128000)*4+500 ) / 1000;

    stOut.R = MINMAX(s32U,0,1023);
    stOut.G = MINMAX(s32Y,0,1023);
    stOut.B = MINMAX(s32V,0,1023);

    return stOut;
}
#if (HW_2DTO3D_VER == 4)
PIXEL _MDrv_SC_3D_2DTo3D_YUV2RGB(PIXEL pin)
{

    PIXEL pout;
    // YUV888 -> RGB888
    MS_S16 r = (1164*(pin.G-16) + 1596*(pin.R-128) + 500 )/1000;
    MS_S16 g = (1164*(pin.G-16) -  813*(pin.R-128) - 391*(pin.B-128) + 500 )/1000;
    MS_S16 b = (1164*(pin.G-16) + 2018*(pin.B-128) + 500 )/1000;

    pout.R = MINMAX(r,0,1023);
    pout.G = MINMAX(g,0,1023);
    pout.B = MINMAX(b,0,1023);

    return pout;
}
#endif
#if (HW_2DTO3D_VER == 3)
void _MDrv_SC_3D_2DTo3D_HWInfo_Read(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_S32 i = 0, j = 0, i_dummy = 0, j_dummy = 0;
    MS_U8 u8Y_Avg_Edge_H = 0;
    MS_U8 u8R_Cr_Avg = 0;
    MS_U8 u8G_Y_Avg = 0;
    MS_U8 u8B_Cb_Avg = 0;
    MS_VIRT u32DDBase = MsOS_PA2KSEG1(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf);
    if(0 == u32DDBase)
    {
        printf("%s :u32DDBase = 0!,error!!!\n", __FUNCTION__);
        assert(u32DDBase != 0);
    }
    MS_U16 u16DDBlock_DramSize = DDBYTENUM;
    MS_U32 u32DDBlockLinePitch = (NBLK_DD_PITCH*DDBYTENUM);
    MS_U8 u8Idx = 0;

    for (i = 0; i < NBLK_DD_V; i++)
    {
        i_dummy = i * u32DDBlockLinePitch;

        for (j = 0; j < NBLK_DD_H; j++)
        {
            j_dummy = j* u16DDBlock_DramSize;

            u8Y_Avg_Edge_H = *((volatile MS_U8*)((MS_U64)( i_dummy + j_dummy + 1) + u32DDBase));

            u8R_Cr_Avg = *((volatile MS_U8*)((MS_U64)( i_dummy + j_dummy + 17) + u32DDBase));
            u8G_Y_Avg = *((volatile MS_U8*)((MS_U64)( i_dummy + j_dummy + 15) + u32DDBase));
            u8B_Cb_Avg = *((volatile MS_U8*)((MS_U64)( i_dummy + j_dummy + 16) + u32DDBase));


            u8R_Cr_Avg = MINMAX((((u8R_Cr_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);
            u8G_Y_Avg = MINMAX((((u8G_Y_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);
            u8B_Cb_Avg = MINMAX((((u8B_Cb_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);

            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[i][j].R = u8R_Cr_Avg; // avg color value
            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[i][j].G = u8G_Y_Avg; // avg color value
            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[i][j].B = u8B_Cb_Avg; // avg color value

            if(u8Y_Avg_Edge_H>0x1A)
            {
                pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[i][j] = u8Y_Avg_Edge_H-0x1A; // edge info
            }
            else
            {
                pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[i][j] = 0;
            }

        }
    }

    for(u8Idx = 0 ; u8Idx < 32 ; u8Idx++)
    {
        pXCResourcePrivate->stdrvXC_3D._u32Histogram[u8Idx] = 0;
    }
}
#elif (HW_2DTO3D_VER == 4)
void _MDrv_SC_3D_2DTo3D_HWInfo_Read_U9(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 i = 0, j = 0, i_dummy = 0, j_dummy = 0;
    MS_U8 u8Y_Avg_Edge_H = 0;
    MS_U8 u8R_Cr_Avg = 0;
    MS_U8 u8G_Y_Avg = 0;
    MS_U8 u8B_Cb_Avg = 0;
    MS_U8 u8_Y_Max = 0;//u9
    MS_U8 u8_Y_Min = 0;//u9
    MS_U8 u8_Motion = 0;//u9
    MS_U32 u32_Mot_Cnt = 0;
    MS_VIRT u32DDBase = MsOS_PA2KSEG1(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf);
    if(0 == u32DDBase)
    {
        printf("%s :u32DDBase = 0!,error!!!\n", __FUNCTION__);
        assert(u32DDBase != 0);
    }
    //if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x0010)==0x10)
    //printf("[DDBase:%8x, DD_Buf:%x]\r\n",u32DDBase,pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf);
    MS_U16 u16DDBlock_DramSize = DDBYTENUM;
    MS_U32 u32DDBlockLinePitch = (NBLK_DD_PITCH*DDBYTENUM);
    MS_U8 u8Idx = 0;

    /*if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x0001)==1)
    {
        printf("[%d %d]\r\n",gSrcInfo[MAIN_WINDOW].stDispWin.width,NBLK_DD_H);
    }*/
    //H-Upscaling
    //if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK15_01_L)&0x07ff)==0x3C0)
    if(!IsVMirrorMode(MAIN_WINDOW))
    {
        for (i = 0; i < NBLK_DD_V; i++)
        {
            i_dummy = i * u32DDBlockLinePitch;

            for (j = 0; j < NBLK_DD_H/2; j++)
            {
                j_dummy = j* u16DDBlock_DramSize;

                u8Y_Avg_Edge_H = *((volatile MS_U8*)(( i_dummy + j_dummy + 1UL) + u32DDBase));
#if 0
                u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 16) + u32DDBase));
                u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 15) + u32DDBase));
                u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 17) + u32DDBase));
#else
                u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 17UL) + u32DDBase));
                u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 15UL) + u32DDBase));
                u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 16UL) + u32DDBase));
#endif
                //u9 status
                u8_Y_Max =  *((volatile MS_U8*)(( i_dummy + j_dummy + 9UL) + u32DDBase));
                u8_Y_Min =  *((volatile MS_U8*)(( i_dummy + j_dummy + 12UL) + u32DDBase));
                u8_Motion =  *((volatile MS_U8*)(( i_dummy + j_dummy + 18UL) + u32DDBase));

     pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2].R = u8R_Cr_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2+1].R = u8R_Cr_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2].G = u8G_Y_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2+1].G = u8G_Y_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2].B = u8B_Cb_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2+1].B = u8B_Cb_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[i][j*2] = _MDrv_SC_3D_2DTo3D_YUV2RGB(pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2]);
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[i][j*2+1] = _MDrv_SC_3D_2DTo3D_YUV2RGB(pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j*2+1]);

                pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[i][j*2] = u8Y_Avg_Edge_H;
                pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[i][j*2+1] = u8Y_Avg_Edge_H;
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[i][j*2] = u8_Motion;
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[i][j*2+1] = u8_Motion;
                u32_Mot_Cnt = u32_Mot_Cnt + u8_Motion;
                //basic depth
                pXCResourcePrivate->stdrvXC_3D._field[i*NBLK_DD_H + j*2] = MAX( u8_Y_Max - u8_Y_Min, 0);
                pXCResourcePrivate->stdrvXC_3D._field[i*NBLK_DD_H + j*2+1] = MAX( u8_Y_Max - u8_Y_Min, 0);
            }
        }
    }
    else
    {
        for (i = 0; i < NBLK_DD_V; i++)
        {
            i_dummy = i * u32DDBlockLinePitch;

            for (j = 0; j < NBLK_DD_H/2; j++)
            {
                j_dummy = j* u16DDBlock_DramSize;

                u8Y_Avg_Edge_H = *((volatile MS_U8*)(( i_dummy + j_dummy + 1UL) + u32DDBase));

                u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 17UL) + u32DDBase));
                u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 15UL) + u32DDBase));
                u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 16UL) + u32DDBase));

                //u9 status
                u8_Y_Max =  *((volatile MS_U8*)(( i_dummy + j_dummy + 9UL) + u32DDBase));
                u8_Y_Min =  *((volatile MS_U8*)(( i_dummy + j_dummy + 12UL) + u32DDBase));
                u8_Motion =  *((volatile MS_U8*)(( i_dummy + j_dummy + 18UL) + u32DDBase));

                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i - 1][j*2].R = u8R_Cr_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i - 1][j*2+1].R = u8R_Cr_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i - 1][j*2].G = u8G_Y_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i - 1][j*2+1].G = u8G_Y_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i - 1][j*2].B = u8B_Cb_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i - 1][j*2+1].B = u8B_Cb_Avg; // avg color value
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V - i - 1][j*2] = _MDrv_SC_3D_2DTo3D_YUV2RGB(pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i][j*2]);
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V - i - 1][j*2+1] = _MDrv_SC_3D_2DTo3D_YUV2RGB(pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[NBLK_DD_V - i][j*2+1]);
                pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[NBLK_DD_V - i - 1][j*2] = u8Y_Avg_Edge_H;
                pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[NBLK_DD_V - i - 1][j*2+1] = u8Y_Avg_Edge_H;
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[NBLK_DD_V - i - 1][j*2] = u8_Motion;
                pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[NBLK_DD_V - i - 1][j*2+1] = u8_Motion;
                u32_Mot_Cnt = u32_Mot_Cnt + u8_Motion;
                //basic depth
                pXCResourcePrivate->stdrvXC_3D._field[(NBLK_DD_V - i - 1)*NBLK_DD_H + j*2] = MAX( u8_Y_Max - u8_Y_Min, 0);
                pXCResourcePrivate->stdrvXC_3D._field[(NBLK_DD_V - i - 1)*NBLK_DD_H + j*2+1] = MAX( u8_Y_Max - u8_Y_Min, 0);
            }
        }
    }
/*
            else
            {
    for (i = 0; i < NBLK_DD_V; i++)
    {
        i_dummy = i * u32DDBlockLinePitch;

        for (j = 0; j < NBLK_DD_H; j++)
        {
            j_dummy = j* u16DDBlock_DramSize;

            u8Y_Avg_Edge_H = *((volatile MS_U8*)(( i_dummy + j_dummy + 1) + u32DDBase));
#if 0
            u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 16) + u32DDBase));
            u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 15) + u32DDBase));
            u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 17) + u32DDBase));
#else
            u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 17) + u32DDBase));
            u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 15) + u32DDBase));
            u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 16) + u32DDBase));
#endif
            //u9 status
            u8_Y_Max =  *((volatile MS_U8*)(( i_dummy + j_dummy + 9) + u32DDBase));
            u8_Y_Min =  *((volatile MS_U8*)(( i_dummy + j_dummy + 12) + u32DDBase));
            u8_Motion =  *((volatile MS_U8*)(( i_dummy + j_dummy + 18) + u32DDBase));

            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j].R = u8R_Cr_Avg; // avg color value
            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j].G = u8G_Y_Avg; // avg color value
            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j].B = u8B_Cb_Avg; // avg color value

            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[i][j] = _MDrv_SC_3D_2DTo3D_YUV2RGB(pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[i][j]);

            pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[i][j] = u8Y_Avg_Edge_H;
            pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[i][j] = u8_Motion;
            u32_Mot_Cnt = u32_Mot_Cnt + u8_Motion;
            //basic depth
            pXCResourcePrivate->stdrvXC_3D._field[i*NBLK_DD_H + j] = MAX( u8_Y_Max - u8_Y_Min, 0);
            }
        }
    }
*/
    for(u8Idx = 0 ; u8Idx < 32 ; u8Idx++)
    {
        pXCResourcePrivate->stdrvXC_3D._u32Histogram[u8Idx] = 0;
    }
}
#endif

#if (HW_2DTO3D_VER >= 3)
static MS_BOOL  _MDrv_Is_T3DDE_UseMainDisplayWindow(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL  bUseMainDE = FALSE;

    MS_U16 u16MainDispWidth = gSrcInfo[MAIN_WINDOW].stDispWin.width;
    MS_U16 u16MainDispHeight = gSrcInfo[MAIN_WINDOW].stDispWin.height;
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDispWin);

    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        u16MainDispWidth = stDispWin.width;
        u16MainDispHeight = stDispWin.height;
    }

    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == u16MainDispWidth)
        && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == u16MainDispHeight))
    {
        bUseMainDE = FALSE;
    }
    else if (FALSE == MDrv_XC_Is_SubWindowEanble(pInstance))
    {
        bUseMainDE = TRUE;
    }
    else //PIP, not full-frame mode
    {
        bUseMainDE = FALSE;
    }

    return bUseMainDE;
}
#endif

static MS_U16 _MDrv_SC_3D_GetDispWinWidthForT3D(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U16 u16MainDispWidth = 0;
    MS_U16 u16MainDispHStart = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L);
    MS_U16 u16MainDispHEnd = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L);

    //T3D 2P engine, get two piexl a time,start must be even,end must be odd
    u16MainDispHStart = u16MainDispHStart - (u16MainDispHStart & 0x01);
    u16MainDispHEnd = u16MainDispHEnd + ((u16MainDispHEnd&0x1)^0x1);
    u16MainDispWidth = u16MainDispHEnd - u16MainDispHStart + 1;

    return u16MainDispWidth;
}

static void _MDrv_SC_3D_2DTo3D_HW_Info_Write(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if defined (__aarch64__)
    MS_U64 x = 0, y = 0;
#else
    MS_S16 x = 0, y = 0;
#endif
    MS_U8 s16BLKWeight=0,s16BLKDepth=0;//S16->U8
    MS_U8 u8DRBLKLinePitch = 0, u8DRPixBLKLinePitch = 0;
    MS_U8 u8BLKDepth_NoDC =0;
    MS_U8 u8BLKDepth_2D3D = 0;
    MS_U8 u8Auto_Adjust_En = 0;
    MS_VIRT u32DRBase = MsOS_PA2KSEG1(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf);
    MS_S32 s32Depth = 0;
    MS_U8 u8BLKDepth_Mod = 0;
    MS_U8 **u8QueueDataBLKWeight;
    MS_U8 **u8QueueDataBLKDepth;
    MS_S16 x_0 = 0, x_1 = 0, x_2 = 0, x_3 = 0;

    u8QueueDataBLKWeight = (MS_U8 **)malloc(sizeof(MS_U8 *) * NBLK_DR_V);
    u8QueueDataBLKDepth = (MS_U8 **)malloc(sizeof(MS_U8 *) * NBLK_DR_V);
    for( y = 0;y < NBLK_DR_V ;y++)
    {
        u8QueueDataBLKWeight[y]=(MS_U8 *)malloc(sizeof(MS_U8) * NBLK_DR_H);
        u8QueueDataBLKDepth[y]=(MS_U8 *)malloc(sizeof(MS_U8) * NBLK_DR_H);
    }

    if(0 == u32DRBase)
    {
        printf("%s :u32DRBase = 0!,error!!!\n", __FUNCTION__);
        assert(u32DRBase != 0);
    }

    u8DRBLKLinePitch    =  SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, 0x00ff)     *32;//256 bit alignment = 32 byte
    u8DRPixBLKLinePitch = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, 0xff00)>>8 )*32;//256 bit alignment = 32 byte

#if AUTO_ADJUST
    MS_S32 s32Total = 0;
    MS_S32 s32Count = 0;
    MS_S32 s32Diff = 0;
    MS_S32 s32Depth_Control_Var = 0;
    MS_S32 s32Mean_Depth = 0;
    u8Auto_Adjust_En = 1;

    for ( y = 5; y < NBLK_DR_V - 5; y ++ )
    {
        for ( x = 5; x < NBLK_DR_H - 5; x ++ )
        {
            u8BLKDepth_Mod = pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x];
            s32Total += u8BLKDepth_Mod;
            s32Count++;
        }
    }
    s32Count=MAX(s32Count,1);
    s32Mean_Depth=s32Total/s32Count;

    for ( y = 5; y < NBLK_DR_V-5; y ++ )
    {
        for ( x = 5; x < NBLK_DR_H-5; x ++ )
        {
            u8BLKDepth_Mod = pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x];
            s32Diff += abs(u8BLKDepth_Mod-s32Mean_Depth);
        }
    }
    s32Depth_Control_Var=s32Diff/s32Count;
    s32Depth_Control_Var=MAX(s32Depth_Control_Var,12);
    pXCResourcePrivate->stdrvXC_3D._s32Depth_Control_Var_IIR = (pXCResourcePrivate->stdrvXC_3D._s32Depth_Control_Var_IIR*112+s32Depth_Control_Var*(128-112) + 64 )>>7;
    pXCResourcePrivate->stdrvXC_3D._s32Mean_Depth_IIR = (pXCResourcePrivate->stdrvXC_3D._s32Mean_Depth_IIR*112+s32Mean_Depth*(128-112) + 64 )>>7;
#endif


    for ( y = 0; y < NBLK_DR_V; y ++ )
    for ( x = 0; x < NBLK_DR_H; x ++ )
    {
        u8BLKDepth_Mod = pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x];
#if AUTO_ADJUST
        if(pXCResourcePrivate->stdrvXC_3D._s32Depth_Control_Var_IIR == 0)
        {
            s32Depth = (u8BLKDepth_Mod - pXCResourcePrivate->stdrvXC_3D._s32Mean_Depth_IIR)*pXCResourcePrivate->stdrvXC_3D._s8Multi_Gain + 160-pXCResourcePrivate->stdrvXC_3D._s8Shift_Value;
        }
        else
        {
            s32Depth = (u8BLKDepth_Mod - pXCResourcePrivate->stdrvXC_3D._s32Mean_Depth_IIR)*pXCResourcePrivate->stdrvXC_3D._s8Multi_Gain/pXCResourcePrivate->stdrvXC_3D._s32Depth_Control_Var_IIR + 160-pXCResourcePrivate->stdrvXC_3D._s8Shift_Value;
        }
        s32Depth = MINMAX(s32Depth,160-pXCResourcePrivate->stdrvXC_3D._s8Clamp_Value-pXCResourcePrivate->stdrvXC_3D._s8Shift_Value,160+pXCResourcePrivate->stdrvXC_3D._s8Clamp_Value-pXCResourcePrivate->stdrvXC_3D._s8Shift_Value);
#endif
        s32Depth = MINMAX(s32Depth,0,MAXVALUE);
        s16BLKDepth = s32Depth;


        u8BLKDepth_2D3D = pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x];
        u8BLKDepth_NoDC = u8BLKDepth_2D3D;
        s16BLKDepth = u8Auto_Adjust_En ? s16BLKDepth: u8BLKDepth_NoDC ;
        s16BLKWeight = pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x];
        u8QueueDataBLKWeight[y][x] = s16BLKWeight;
        u8QueueDataBLKDepth[y][x] = s16BLKDepth;
    }

    //========Write Queue Data, each round 64bit ==========
    if(!IsVMirrorMode(MAIN_WINDOW))
    {
        for ( y = 0; y < NBLK_DR_V; y++ )
        {
            for ( x = 0; x < NBLK_DR_H; x += 4 )
            {
                x_0 = MIN(x, NBLK_DR_H-1);
                x_1 = MIN(x+1, NBLK_DR_H-1);
                x_2 = MIN(x+2, NBLK_DR_H-1);
                x_3 = MIN(x+3, NBLK_DR_H-1);

                (*((volatile MS_U32*)((y*u8DRBLKLinePitch + x) + u32DRBase))) = (((MS_U32)u8QueueDataBLKWeight[y][x_3] << 24)
                                                                  |((MS_U32)u8QueueDataBLKWeight[y][x_2] << 16)
                                                                  |((MS_U32)u8QueueDataBLKWeight[y][x_1] << 8)
                                                                  |((MS_U32)u8QueueDataBLKWeight[y][x_0])) ;          //dr pixel base address
                (*((volatile MS_U32*)((y*u8DRBLKLinePitch + x) + u32DRBase + HW_2DTO3D_BLOCK_DR_BUF_SIZE))) =(((MS_U32)u8QueueDataBLKDepth[y][x_3] << 24)
                                                                            |((MS_U32)u8QueueDataBLKDepth[y][x_2] << 16)
                                                                                |((MS_U32)u8QueueDataBLKDepth[y][x_1] << 8)
                                                                                |((MS_U32)u8QueueDataBLKDepth[y][x_0])) ; //dr block base address

            }
            MsOS_FlushMemory();
        }
    }
    else
    {
        for ( y = NBLK_DR_V-1; y >= 0; y-- )
        {
            for ( x = 0; x < NBLK_DR_H; x += 4 )
            {
                x_0 = MIN(x, NBLK_DR_H-1);
                x_1 = MIN(x+1, NBLK_DR_H-1);
                x_2 = MIN(x+2, NBLK_DR_H-1);
                x_3 = MIN(x+3, NBLK_DR_H-1);

                (*((volatile MS_U32*)((y*u8DRBLKLinePitch + x) + u32DRBase))) = (((MS_U32)u8QueueDataBLKWeight[NBLK_DR_V - y - 1][x_3] << 24)
                                                                  |((MS_U32)u8QueueDataBLKWeight[NBLK_DR_V - y - 1][x_2] << 16)
                                                                  |((MS_U32)u8QueueDataBLKWeight[NBLK_DR_V - y - 1][x_1] << 8)
                                                                  |((MS_U32)u8QueueDataBLKWeight[NBLK_DR_V - y - 1][x_0])) ;          //dr pixel base address
                  (*((volatile MS_U32*)((y*u8DRBLKLinePitch + x) + u32DRBase + HW_2DTO3D_BLOCK_DR_BUF_SIZE))) = (((MS_U32)u8QueueDataBLKDepth[NBLK_DR_V - y - 1][x_3] << 24)
                                                                            |((MS_U32)u8QueueDataBLKDepth[NBLK_DR_V - y - 1][x_2] << 16)
                                                                            |((MS_U32)u8QueueDataBLKDepth[NBLK_DR_V - y - 1][x_1] << 8)
                                                                            |((MS_U32)u8QueueDataBLKDepth[NBLK_DR_V - y - 1][x_0])) ; //dr block base address

            }
            MsOS_FlushMemory();
        }
    }
    for( y = 0;y < NBLK_DR_V ;y++)
    {
        free(u8QueueDataBLKWeight[y]);
        free(u8QueueDataBLKDepth[y]);
    }
    free(u8QueueDataBLKWeight);
    free(u8QueueDataBLKDepth);
    u8QueueDataBLKWeight = NULL;
    u8QueueDataBLKDepth = NULL;
}

#if (HW_2DTO3D_VER == 3)
static MS_U16 _MDrv_SC_3D_2DTo3D_HorizontalDetection(void *pInstance)
{
    MS_U16 x = 0, y = 0;
    MS_U16 u16Cplx_Sum = 0;
    MS_U16 u16Cplx = 0;
    MS_U8 u8Cplx_Idx = 0;
    MS_U32 u32CplxHist[NBLK_V_MAX];
    PIXEL stPP;
    MS_U16 u16Y_Sep_Out = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memset(&stPP, 0, sizeof(PIXEL));

    for( x =0; x< NBLK_DD_V; x++)
        u32CplxHist[x] = 0;

    for (  y = 0; y < NBLK_DD_V; y ++ )
    {
        u16Cplx_Sum = 0;
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            //to avoid dark letter box
            stPP = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            if(stPP.R<=LBOX_BLKCOLOR&&stPP.G<=LBOX_BLKCOLOR&&stPP.B<=LBOX_BLKCOLOR)
                continue;

            u16Cplx = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
            u8Cplx_Idx = MINMAX(u16Cplx/4,0,63);//for 32 level histogram
            u16Cplx_Sum = u8Cplx_Idx + u16Cplx_Sum ;

        }
        u32CplxHist[y] = u16Cplx_Sum;
    }

    u16Y_Sep_Out = 0;

    for (  y = 0; y < NBLK_DD_V; y ++)
    {
        if (u32CplxHist[y]  > 250 && u32CplxHist[(y)+1]  > 250)
        {
            u16Y_Sep_Out =y;
            break;
        }
    }

    u16Y_Sep_Out = u16Y_Sep_Out*pXCResourcePrivate->stdrvXC_3D._u8Vertical_Step;
    if (u16Y_Sep_Out >= DISPLAY_DD_V)
        u16Y_Sep_Out =0;

    return u16Y_Sep_Out;
}
#endif
#if (HW_2DTO3D_VER == 3)
static void _MDrv_SC_3D_2DTo3D_BGColorDetection(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32CplxHistoTp[64];
    MS_U32 u32CplxHistoDn[64];
    MS_U32 u32CplxHistoALL1[64];
    MS_S16 x = 0, y = 0;
    PIXEL stPP;
    MS_S16 s16MaxEdgeRGB = 0;
    MS_S16 s16Cplx = 0;
    MS_S16 s16Cplx_Idx = 0;
    MS_S16 s16CpxThrdTp = 0;
    MS_S16 s16CpxThrdDn = 0;
    MS_S16 blkSum_bgTp = 0, blkSum_bgDn = 0;
    MS_S32 s32RSum = 0, s32GSum = 0, s32BSum = 0;
    MS_S16 s16TBB_CplxBinThrd = 0, s16FlatAccuTp = 0, s16FlatAccuDn =  0;
    MS_BOOL bBGCOLOR_Tp = 0, bBGCOLOR_Dn = 0;
    MS_S16 s16Tp14_BGCOLORCnt =  0, s16Dn14_BGCOLORCnt = 0, s16AntiTp14_BGCOLORCnt = 0, s16AntiDn14_BGCOLORCnt = 0;
    MS_S16 s16ValidTpCnt = 0, s16ValidDnCnt = 0;
    MS_U32 u32ScalingRatio = DISPLAY_DD_H*DISPLAY_DD_V*1000/FHD_W/FHD_H;

    memset(&stPP, 0, sizeof(PIXEL));

    //--------Cplx histogram initialize start-------------//
    for(x =0; x<64; x++)
    {
        u32CplxHistoTp[x] = 0;
        u32CplxHistoDn[x] = 0;
        u32CplxHistoALL1[x] = 0;
    }
    //--------Cplx histogram initialize end-------------//


    //--------blur background weight estimate start-----//
    for ( y = 0; y < NBLK_DD_V; y ++ )
    {
        for ( x = 0; x < NBLK_DD_H; x++ )
        {
            //to avoid dark letter box
            stPP = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            if(stPP.R<=LBOX_BLKCOLOR&&stPP.G<=LBOX_BLKCOLOR&&stPP.B<=LBOX_BLKCOLOR)
                continue;

            s16Cplx = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
            s16Cplx_Idx = MINMAX(s16Cplx/4,0,63);//for 32 level histogram

            u32CplxHistoALL1[s16Cplx_Idx] = u32CplxHistoALL1[s16Cplx_Idx] +1 ;

            if( (y>= 0) && (y <= NBLK_DD_V/4))
                u32CplxHistoTp[s16Cplx_Idx] = u32CplxHistoTp[s16Cplx_Idx] +1 ;

            if( (y>= NBLK_DD_V*3/4) && (y <= NBLK_DD_V))
                u32CplxHistoDn[s16Cplx_Idx] = u32CplxHistoDn[s16Cplx_Idx] +1 ;
        }
    }

    if(u32ScalingRatio == 0)
    {
        u32ScalingRatio = 1;
    }

    //--------blur background weight estimate end-----//

    //try to useconstant value
    s16CpxThrdTp =3;
    s16CpxThrdDn =3;
    //--------estimate s16Cplx thrd end-----//


    //--------estimate Table B background color select weight start-----//
    s16TBB_CplxBinThrd = MIN(s16CpxThrdDn, s16CpxThrdTp);
    s16FlatAccuTp = 0;
    s16FlatAccuDn = 0;
    for(x = 0; x<63;x++)
    {
        s16FlatAccuTp = s16FlatAccuTp +u32CplxHistoTp[x];
        s16FlatAccuDn = s16FlatAccuDn +u32CplxHistoDn[x];
        if(x>=s16TBB_CplxBinThrd)
            break;
    }

    //**********************************************************************************************//
    pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei = MINMAX((s16FlatAccuTp-s16FlatAccuDn)*1000/u32ScalingRatio,-32,32);
    pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei = (pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei+32)/2;// range: 0~32

    pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei = MINMAX((s16FlatAccuTp-s16FlatAccuDn)*1000/u32ScalingRatio,-32,32);
    pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei = (pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei+32)/2;// range: 0~32

#if PRINTINFO
    printf("FlatTp=%u, FlatDn=%u\n",s16FlatAccuTp,s16FlatAccuDn);
#endif

    //**********************************************************************************************//
    //--------estimate Table B background color select weight end-----//

    //--------estimate Top 1/4 & Bottom 1/4 background color start-----//

    //Top Quarter Avg Color
    blkSum_bgTp=0;
    //PIXEL _stBgPixelTp;
    s32RSum = 0;
    s32GSum = 0;
    s32BSum = 0;
    for ( y = 0; y < NBLK_DD_V/4; y ++ )
    {
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            stPP = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            s16Cplx = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
            if ( s16Cplx/4 <= s16CpxThrdTp && !(stPP.R<=LBOX_BLKCOLOR&&stPP.G<=LBOX_BLKCOLOR&&stPP.B<=LBOX_BLKCOLOR)) //if ( s16Cplx < 32 )
            {
                s32RSum += stPP.R;
                s32GSum += stPP.G;
                s32BSum += stPP.B;
                blkSum_bgTp ++;
            }

        }
    }
    if(blkSum_bgTp==0)
    {
        pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.R= 0 ;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.G= 0 ;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.B= 0 ;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.R=s32RSum/blkSum_bgTp;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.G=s32GSum/blkSum_bgTp;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.B=s32BSum/blkSum_bgTp;
    }

    //last Quarter Avg Color
    blkSum_bgDn=0;
    //PIXEL _stBgPixelDn;
    s32RSum = 0;
    s32GSum = 0;
    s32BSum = 0;
    for ( y = NBLK_DD_V*3/4; y < NBLK_DD_V; y ++ )
    {
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            stPP = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            s16Cplx = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
            if ( s16Cplx/4 <= s16CpxThrdDn && !(stPP.R<=LBOX_BLKCOLOR&&stPP.G<=LBOX_BLKCOLOR&&stPP.B<=LBOX_BLKCOLOR)) //if ( s16Cplx < 32 )
            {
                s32RSum += stPP.R;
                s32GSum += stPP.G;
                s32BSum += stPP.B;
                blkSum_bgDn ++;
            }
        }
    }
    if(blkSum_bgDn==0)
    {
        pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.R= 0;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.G= 0;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.B= 0;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.R= s32RSum/blkSum_bgDn;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.G= s32GSum/blkSum_bgDn;
        pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.B= s32BSum/blkSum_bgDn;
    }

    pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC = RGB2YUV(pXCResourcePrivate->stdrvXC_3D._stBgPixelTp);
    pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC = RGB2YUV(pXCResourcePrivate->stdrvXC_3D._stBgPixelDn);

    //--------estimate Top 1/4 & Bottom 1/4 background color end-----//

    for ( y = 0; y < NBLK_DD_V ; y ++ )
    for ( x = 0; x < NBLK_DD_H; x ++ )
    {
        stPP = RGB2YUV(pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x]);
        s16MaxEdgeRGB   = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];

        bBGCOLOR_Tp = (s16MaxEdgeRGB < 32) && abs(stPP.B - pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.B)/4<16 && abs(stPP.G - pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.G)/4<16 && abs(stPP.R - pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.R)/4<16;
        bBGCOLOR_Dn = (s16MaxEdgeRGB < 32) && abs(stPP.B - pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC.B)/4<16 && abs(stPP.G - pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC.G)/4<16 && abs(stPP.R - pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC.R)/4<16;

        if( bBGCOLOR_Tp )
            pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x] = true;
        else
            pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x] = false;

        if( bBGCOLOR_Dn )
            pXCResourcePrivate->stdrvXC_3D._bDnBGColorMarker[y][x] = true;
        else
            pXCResourcePrivate->stdrvXC_3D._bDnBGColorMarker[y][x] = false;

    }

    s16Tp14_BGCOLORCnt = 0;
    s16Dn14_BGCOLORCnt = 0;
    s16AntiTp14_BGCOLORCnt = 0;
    s16AntiDn14_BGCOLORCnt = 0;

    s16ValidTpCnt = 0;
    s16ValidDnCnt = 0;

    for ( y = 0; y < NBLK_DD_V; y ++ )
    for ( x = 0; x < NBLK_DD_H; x ++ )
    {
        s16MaxEdgeRGB = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
        stPP = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];

        bBGCOLOR_Tp = pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x];
        bBGCOLOR_Dn = pXCResourcePrivate->stdrvXC_3D._bDnBGColorMarker[y][x];

        if(y>=0&&y<=NBLK_DD_V/4&&(s16MaxEdgeRGB/4)<3&&bBGCOLOR_Tp)
            s16ValidTpCnt++;
        if(y>=NBLK_DD_V*3/4&&y<=NBLK_DD_V&&(s16MaxEdgeRGB/4)<3&&bBGCOLOR_Dn)
            s16ValidDnCnt++;

        if(y>=0&&y<=NBLK_DD_V/4)
        {
            if(bBGCOLOR_Tp)
                s16Tp14_BGCOLORCnt++;
            if(bBGCOLOR_Dn)
                s16AntiDn14_BGCOLORCnt++;
        }

        if(y>=NBLK_DD_V*3/4&&y<=NBLK_DD_V)
        {
            if(bBGCOLOR_Dn)
                s16Dn14_BGCOLORCnt++;
            if(bBGCOLOR_Tp)
                s16AntiTp14_BGCOLORCnt++;
        }
    }

#if PRINTINFO
    printf("TpBGC R = %u, G = %d, B = %u\n",pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.R,pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.G,pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.B);
    printf("BtBGC R = %u, G = %d, B = %u\n",pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.R,pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.G,pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.B);
#endif

    pXCResourcePrivate->stdrvXC_3D._s16TpBGCOLOR_ValidWei = MINMAX((s16ValidTpCnt*1000/u32ScalingRatio-32)/2,0,32);
    pXCResourcePrivate->stdrvXC_3D._s16DnBGCOLOR_ValidWei = MINMAX((s16ValidDnCnt*1000/u32ScalingRatio-32)/2,0,32);

#if PRINTINFO
    printf("s16Tp14_BGCOLORCnt = %u, s16AntiDn14_BGCOLORCnt = %u\n",s16Tp14_BGCOLORCnt,s16AntiDn14_BGCOLORCnt);
    printf("s16Dn14_BGCOLORCnt = %u, s16AntiTp14_BGCOLORCnt = %u\n",s16Dn14_BGCOLORCnt,s16AntiTp14_BGCOLORCnt);
#endif

    s16Tp14_BGCOLORCnt     = MINMAX(s16Tp14_BGCOLORCnt/4*1000/u32ScalingRatio-16,0,48);//Tp color on top
    s16AntiDn14_BGCOLORCnt = MINMAX((s16AntiDn14_BGCOLORCnt/4*1000/u32ScalingRatio-4),0,48);//Dn color on Top
    s16Dn14_BGCOLORCnt     = MINMAX(s16Dn14_BGCOLORCnt/4*1000/u32ScalingRatio-16,0,48);//Dn color at bottom
    s16AntiTp14_BGCOLORCnt = MINMAX((s16AntiTp14_BGCOLORCnt/4*1000/u32ScalingRatio-4),0,48);//Tp color at bottom

#if PRINTINFO
    printf("Tp14_BGCOLOR_Wei = %u, AntiDn14_BGCOLOR_Wei = %u\n",s16Tp14_BGCOLORCnt,s16AntiDn14_BGCOLORCnt);
    printf("Dn14_BGCOLOR_Wei = %u, AntiTp14_BGCOLOR_Wei = %u\n",s16Dn14_BGCOLORCnt,s16AntiTp14_BGCOLORCnt);
#endif

    pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei   = MINMAX(s16Tp14_BGCOLORCnt-MAX(s16AntiDn14_BGCOLORCnt,s16AntiTp14_BGCOLORCnt), 0,32);
    pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei_Bottom = MINMAX(s16Dn14_BGCOLORCnt-MAX(s16AntiDn14_BGCOLORCnt,s16AntiTp14_BGCOLORCnt), 0,32);

    pXCResourcePrivate->stdrvXC_3D._s16TBA_BGColorWei = MINMAX(s16Tp14_BGCOLORCnt,0,32);
    pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorWei = MINMAX(s16Dn14_BGCOLORCnt,0,32);
}
#elif (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_BGColorDetection_U9(void* pInstance)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U32 CbHistoTp[32];
    MS_U32 CrHistoTp[32];
    MS_U32 CbHistoBn[32];
    MS_U32 CrHistoBn[32];
    MS_U32 YHistoTp[32];
    MS_U32 YHistoBn[32];

    int x,y;
    PIXEL pp, ppYCC;
    MS_U32 cplx;
    MS_U32 CbTpMaxidx=0,CrTpMaxidx=0,CbBnMaxidx=0,CrBnMaxidx=0,CbMaxidx=0,CrMaxidx=0;
    MS_U32 CbTpMaxcnt=0,CrTpMaxcnt=0,CbBnMaxcnt=0,CrBnMaxcnt=0,CbMaxcnt=0,CrMaxcnt=0;
    MS_U32 YTpMaxidx=0,YBnMaxidx=0,YTpMaxcnt=0,YBnMaxcnt=0;

    MS_U32 CbTpMaxinBncnt = 0, CrTpMaxinBncnt = 0;
    MS_U32 u32ScalingRatio = DISPLAY_DD_H*DISPLAY_DD_V*1000/FHD_W/FHD_H;
    for(x =0; x<32; x++)
    {
        CbHistoTp[x] = 0;
        CrHistoTp[x] = 0;
        CbHistoBn[x] = 0;
        CrHistoBn[x] = 0;

        YHistoTp[x] = 0;
        YHistoBn[x] = 0;
    }

    for ( y = 0; y < NBLK_DD_V/4; y ++ )
        for ( x = 0; x < NBLK_DD_H; x++ )
        {
            //to avoid dark letter box
            pp = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            ppYCC = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[y][x];

            //cplx = field[YCTRS][(y)*NBLK_DD_H+(x)];
            cplx = pXCResourcePrivate->stdrvXC_3D._field[(y)*NBLK_DD_H+(x)];
            //if ( cplx <= 24 && ppYCC.G>LBOX_BLKCOLOR)
            //if ( cplx <= 32 && ppYCC.G>LBOX_BLKCOLOR)
            if ( cplx <= 32 && ppYCC.G>BLKCOLOR)
            {
                CbHistoTp[ppYCC.B/8]++;
                CrHistoTp[ppYCC.R/8]++;
                YHistoTp[ppYCC.G/8]++;
            }
        }

    for ( y = NBLK_DD_V*3/4; y < NBLK_DD_V; y ++ )
        for ( x = 0; x < NBLK_DD_H; x++ )
        {
            //to avoid dark letter box
            pp = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            ppYCC = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[y][x];

            //cplx = field[YCTRS][(y)*NBLK_DD_H+(x)];
            cplx = pXCResourcePrivate->stdrvXC_3D._field[(y)*NBLK_DD_H+(x)];
            //if ( cplx <= 24 && ppYCC.G>LBOX_BLKCOLOR)
            //if ( cplx <= 32 && ppYCC.G>LBOX_BLKCOLOR)
            if ( cplx <= 32 && ppYCC.G>32)
            {
                CbHistoBn[ppYCC.B/8]++;
                CrHistoBn[ppYCC.R/8]++;
                YHistoBn[ppYCC.G/8]++;
            }
        }


    for(x =0; x<32; x++)
    {
        CbTpMaxidx = (CbHistoTp[x]>CbHistoTp[CbTpMaxidx])?x:CbTpMaxidx;
        CrTpMaxidx = (CrHistoTp[x]>CrHistoTp[CrTpMaxidx])?x:CrTpMaxidx;
        CbBnMaxidx = (CbHistoBn[x]>CbHistoBn[CbBnMaxidx])?x:CbBnMaxidx;
        CrBnMaxidx = (CrHistoBn[x]>CrHistoBn[CrBnMaxidx])?x:CrBnMaxidx;

        CbMaxidx = (CbHistoTp[x]+CbHistoBn[x]>CbHistoTp[CbMaxidx]+CbHistoBn[CbMaxidx])?x:CbMaxidx;
        CrMaxidx = (CrHistoTp[x]+CrHistoBn[x]>CrHistoTp[CrMaxidx]+CrHistoBn[CrMaxidx])?x:CrMaxidx;

        YTpMaxidx = (YHistoTp[x]>YHistoTp[YTpMaxidx])?x:YTpMaxidx;
        YBnMaxidx = (YHistoBn[x]>YHistoBn[YBnMaxidx])?x:YBnMaxidx;
    }

    if(CbTpMaxidx==0)
        CbTpMaxcnt=CbHistoTp[0]+CbHistoTp[1];
    else if(CbTpMaxidx==31)
        CbTpMaxcnt=CbHistoTp[30]+CbHistoTp[31];
    else
        CbTpMaxcnt=CbHistoTp[CbTpMaxidx-1]+CbHistoTp[CbTpMaxidx]+CbHistoTp[CbTpMaxidx+1];

    if(CrTpMaxidx==0)
        CrTpMaxcnt=CrHistoTp[0]+CrHistoTp[1];
    else if(CrTpMaxidx==31)
        CrTpMaxcnt=CrHistoTp[30]+CrHistoTp[31];
    else
        CrTpMaxcnt=CrHistoTp[CrTpMaxidx-1]+CrHistoTp[CrTpMaxidx]+CrHistoTp[CrTpMaxidx+1];

    if(CbBnMaxidx==0)
        CbBnMaxcnt=CbHistoBn[0]+CbHistoBn[1];
    else if(CbBnMaxidx==31)
        CbBnMaxcnt=CbHistoBn[30]+CbHistoBn[31];
    else
        CbBnMaxcnt=CbHistoBn[CbBnMaxidx-1]+CbHistoBn[CbBnMaxidx]+CbHistoBn[CbBnMaxidx+1];

    if(CrBnMaxidx==0)
        CrBnMaxcnt=CrHistoBn[0]+CrHistoBn[1];
    else if(CrBnMaxidx==31)
        CrBnMaxcnt=CrHistoBn[30]+CrHistoBn[31];
    else
        CrBnMaxcnt=CrHistoBn[CrBnMaxidx-1]+CrHistoBn[CrBnMaxidx]+CrHistoBn[CrBnMaxidx+1];

    if(CbMaxidx==0)
        CbMaxcnt=CbHistoTp[0]+CbHistoTp[1]+CbHistoBn[0]+CbHistoBn[1];
    else if(CbMaxidx==31)
        CbMaxcnt=CbHistoTp[30]+CbHistoTp[31]+CbHistoBn[30]+CbHistoBn[31];
    else
        CbMaxcnt=CbHistoTp[CbMaxidx-1]+CbHistoTp[CbMaxidx]+CbHistoTp[CbMaxidx+1]+CbHistoBn[CbMaxidx-1]+CbHistoBn[CbMaxidx]+CbHistoBn[CbMaxidx+1];

    if(CrMaxidx==0)
        CrMaxcnt=CrHistoTp[0]+CrHistoTp[1]+CrHistoBn[0]+CrHistoBn[1];
    else if(CrMaxidx==31)
        CrMaxcnt=CrHistoTp[30]+CrHistoTp[31]+CrHistoBn[30]+CrHistoBn[31];
    else
        CrMaxcnt=CrHistoTp[CrMaxidx-1]+CrHistoTp[CrMaxidx]+CrHistoTp[CrMaxidx+1]+CrHistoBn[CrMaxidx-1]+CrHistoBn[CrMaxidx]+CrHistoBn[CrMaxidx+1];

    if(YTpMaxidx==0)
        YTpMaxcnt=YHistoTp[0]+YHistoTp[1];
    else if(YTpMaxidx==31)
        YTpMaxcnt=YHistoTp[30]+YHistoTp[31];
    else
        YTpMaxcnt=YHistoTp[YTpMaxidx-1]+YHistoTp[YTpMaxidx]+YHistoTp[YTpMaxidx+1];

    if(YBnMaxidx==0)
        YBnMaxcnt=YHistoBn[0]+YHistoBn[1];
    else if(YBnMaxidx==31)
        YBnMaxcnt=YHistoBn[30]+YHistoBn[31];
    else
        YBnMaxcnt=YHistoBn[YBnMaxidx-1]+YHistoBn[YBnMaxidx]+YHistoBn[YBnMaxidx+1];

    MS_U32 CntL2,CntL1,CntCur,CntR1,CntR2;

    CntL2 = (CbTpMaxidx<2)?0:CbHistoBn[CbTpMaxidx-2];
    CntL1 = (CbTpMaxidx<1)?0:CbHistoBn[CbTpMaxidx-1];
    CntCur= CbHistoBn[CbTpMaxidx];
    CntR1 = (CbTpMaxidx+1>=32)?0:CbHistoBn[CbTpMaxidx+1];
    CntR2 = (CbTpMaxidx+2>=32)?0:CbHistoBn[CbTpMaxidx+2];

    CbTpMaxinBncnt = CntL2+CntL1+CntCur+CntR1+CntR2;

    CntL2 = (CrTpMaxidx<2)?0:CrHistoBn[CrTpMaxidx-2];
    CntL1 = (CrTpMaxidx<1)?0:CrHistoBn[CrTpMaxidx-1];
    CntCur= CbHistoBn[CbTpMaxidx];
    CntR1 = (CrTpMaxidx+1>=32)?0:CrHistoBn[CrTpMaxidx+1];
    CntR2 = (CrTpMaxidx+2>=32)?0:CrHistoBn[CrTpMaxidx+2];

    CrTpMaxinBncnt = CntL2+CntL1+CntCur+CntR1+CntR2;


    pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp = CbTpMaxidx*8;
    pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp = CrTpMaxidx*8;
    pXCResourcePrivate->stdrvXC_3D.BGColor_CbBn = CbBnMaxidx*8;
    pXCResourcePrivate->stdrvXC_3D.BGColor_CrBn = CrBnMaxidx*8;

    pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntforTp = MIN(CbTpMaxcnt,CrTpMaxcnt)*1000/u32ScalingRatio;
    pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntforBn = MIN(CbBnMaxcnt,CrBnMaxcnt)*1000/u32ScalingRatio;


    //printf("\nTpCb %d TpCr %d\n",BGColor_CbTp,BGColor_CrTp);
    //printf("\nCbrCnt: %d %d / %d %d",CbTpMaxcnt,CrTpMaxcnt,CbTpMaxinBncnt,CrTpMaxinBncnt);
    //printf("\nCbrIdx: %d %d",CbTpMaxidx*8,CrTpMaxidx*8);

    //printf("\nCbrCnt: %d %d / %d %d / %d %d\n",CbTpMaxcnt,CrTpMaxcnt,CbBnMaxcnt,CrBnMaxcnt,CbMaxcnt,CrMaxcnt);
    //printf("CbrIdx: %d %d / %d %d / %d %d\n\n",CbTpMaxidx*8,CrTpMaxidx*8,CbBnMaxidx*8,CrBnMaxidx*8,CbMaxidx*8,CrMaxidx*8);

#if 1
    //2014.02.06 add by CL, for LG subtitle pattern
    MS_BOOL isBGCOLOR_Tp;
    //U8 u8tempval1EF8 = MFC_ReadByte(REG_1EF8);

    pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp = 0;
    pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntBn = 0;
    for ( y = 0; y < NBLK_DD_V ; y ++ )
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            ppYCC = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[y][x];
            cplx = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];

            //isBGCOLOR_Tp = (cplx <= 32) && abs((ppYCC.R>>3)-(BGColor_CrTp>>3))<=2 && abs((ppYCC.B>>3)-(BGColor_CbTp>>3))<=2;
            //LGE patch
            //if((u8tempval1EF8&0x04)==0x04)
            isBGCOLOR_Tp = (cplx <= 32) && abs((ppYCC.R>>3)-(pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp>>3))<=1 && abs((ppYCC.B>>3)-(pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp>>3))<=1&&ppYCC.G>=48;
            //else
            //isBGCOLOR_Tp = (cplx <= 32) && abs((ppYCC.R>>2)-(BGColor_CrTp>>2))<=3 && abs((ppYCC.B>>2)-(BGColor_CbTp>>2))<=3&&ppYCC.G>=48;

            if( isBGCOLOR_Tp )
                pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x] = true;
            else
                pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x] = false;

            if(y>=0&&y<(NBLK_DD_V>>2)&&isBGCOLOR_Tp)
                pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp++;
            if(y>=((NBLK_DD_V*3)>>2)&&y<NBLK_DD_V&&isBGCOLOR_Tp)
                pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntBn++;
        }

    pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp = pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp*1000/u32ScalingRatio;
    pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntBn = pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntBn*1000/u32ScalingRatio;
    //BGColorBlkCntTp = MIN(CbTpMaxcnt,CrTpMaxcnt)/sizeratio;
    //BGColorBlkCntBn = MIN(CbTpMaxinBncnt,CrTpMaxinBncnt)/sizeratio;

    //printf("\nTpBGCnt %d nDnBGCnt %d\n",BGColorBlkCntTp,BGColorBlkCntBn);
#endif
}
#endif
#if 0
static MS_U16 _MDrv_SC_3D_2DTo3D_BackLightDetection(void)
{
    MS_S32 s32Low_Thrd = 6;
    MS_S32 s32Hi_Thrd  = 20;
    MS_S32 s32Low_Sum_Thrd  = 200;
    MS_S32 s32High_Sum_Thrd = 11;
    MS_S32 s32Mean_Thrd = 170;
    MS_S32 s32H_Diff_Thrd = 200 ;
    MS_S32 s32H_Mean_Thrd = 490 ;
    MS_S32 s32Contrast_Thrd = 160; //180
    MS_S32 s32Hist_Shift = 32;
    MS_S32 s32TotalPix = FRAME_HEIGHT_DLC*FRAME_WIDTH_DLC/s32Hist_Shift;

    MS_S32 s32Hist[32];
    MS_S32 s32Contrast = 0;
    MS_S32 s32Hist_Sum = 0;
    MS_S32 s32Hist_Temp = 0;
    MS_S32 s32Hist_Mean = 0;
    MS_S32 s32H_Mean = 0, s32H_Diff = 0, s32H_Twice = 0, s32H_Half = 0;
    MS_S32 s32Hist_Mean_Twice =0;
    MS_S32 s32Hist_Mean_Half =0;
    MS_S32 s32Index_BL = 0, s32Index_Y_Low = 0, s32Index_Y_Hi = 0;
    MS_S32 s32Index_H_Mean = 0, s32Index_H_Diff= 0, s32Index_Contrast = 0, s32Index_Mean = 0;
    MS_S32 s32HisData = 0;
    MS_S32 x = 0;
    MS_S32 s32Low_Sum = 0, s32Hi_Sum = 0;
    MS_S32 s32Hist_Count = 0;

    //Histogram Check
    for(x = 0 ; x < 32 ; x++)
    {
        s32HisData = _u32Histogram[x];
        s32Hist_Count = s32Hist_Count + s32HisData;
    }

    if(s32Hist_Count!=0)
        s32TotalPix = s32Hist_Count;

    for( x = 31; x >= 0; x-- )
    {
        s32Hist_Sum = x* _u32Histogram[x] + s32Hist_Sum;
        s32Hist_Temp = _u32Histogram[x] + s32Hist_Temp;
        s32Hist[x] = (s32Hist_Temp*1000/s32TotalPix);  //100
    }

    s32Hist_Mean = s32Hist_Sum /s32TotalPix;
    s32Hist_Mean_Twice = MIN(31, s32Hist_Mean +4);
    s32Hist_Mean_Half = MAX(0, s32Hist_Mean -4 );

    s32H_Mean = s32Hist[s32Hist_Mean];
    s32H_Twice =  s32Hist[s32Hist_Mean_Twice];
    s32H_Half =  s32Hist[s32Hist_Mean_Half];
    s32H_Diff = MIN(s32H_Mean- s32H_Twice, s32H_Half - s32H_Mean);

    s32Low_Sum =0;
    s32Hi_Sum =0;

    //X =0;
    for( x=2; x< s32Low_Thrd ; x++)
        s32Low_Sum = s32Low_Sum + _u32Histogram[x];

    //31
    for( x=31; x > s32Hi_Thrd ; x--)
        s32Hi_Sum = s32Hi_Sum + _u32Histogram[x];

    s32Contrast = ((s32Low_Sum- s32Hi_Sum)*1000/s32TotalPix);

    if (s32Hi_Sum*1000/s32TotalPix > s32High_Sum_Thrd)
        s32Index_Y_Hi =10;
    else
        s32Index_Y_Hi = (MINMAX(10 - s32High_Sum_Thrd + s32Hi_Sum*1000/s32TotalPix, 0, 10));

    if (s32Low_Sum*1000/s32TotalPix > s32Low_Sum_Thrd)
        s32Index_Y_Low =10;
    else
        s32Index_Y_Low = (MINMAX(10 - s32Low_Sum_Thrd + s32Low_Sum*1000/s32TotalPix, 0, 10));

    if (s32H_Mean > s32H_Mean_Thrd)
        s32Index_H_Mean =10;
    else
        s32Index_H_Mean = MINMAX( 10- s32H_Mean_Thrd + s32H_Mean, 0, 10);

    if (s32H_Diff < s32H_Diff_Thrd)
        s32Index_H_Diff =10;
    else
        s32Index_H_Diff = MINMAX( 10- s32H_Diff + s32H_Diff_Thrd, 0, 10);

    if (s32Contrast > s32Contrast_Thrd)
        s32Index_Contrast = 10;
    else
        s32Index_Contrast = MINMAX( 10 - s32Contrast_Thrd + s32Contrast , 0, 10);

    s32Hist_Mean = s32Hist_Sum*20 /s32TotalPix;
    if (s32Hist_Mean > s32Mean_Thrd)
        s32Index_Mean = 10;
    else
        s32Index_Mean = MINMAX( 10 - s32Mean_Thrd + s32Hist_Mean , 0, 10);

    s32Index_BL = MIN(MIN3(s32Index_Y_Low, s32Index_Y_Hi, s32Index_Mean),  MIN3(s32Index_H_Diff, s32Index_H_Mean, s32Index_Contrast));

    return s32Index_BL;
}
#endif

#if (HW_2DTO3D_VER == 3)
static void _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT(void *pInstance, MS_U16 u16Index_BL)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8RIdx = 0,u8GIdx = 0, u8BIdx = 0;
    MS_S16 s16R = 0, s16G = 0, s16B = 0, s16Depth = 0, s16Depth2 = 0;
    MS_S16 s16Y = 0, s16Cb = 0, s16Cr = 0;
    MS_S16 s16CrDepth = 0, s16CbDepth = 0, s16Sat = 0, s16YDepth = 0, s16CrCbDepth = 0;
    MS_S16 s16Sat_Wei = 0;
#if (DVIEW_WEI || DVIEW_WEI2)
    MS_S16 s16DViewDepth = 0;
#endif
    MS_S16 s16YDark = 0;
    PIXEL stRGB, stYCC;
    MS_S16 s16BGDiff3 = 0, s16BGdepthWei3 = 0;
    //Only in TableB
    MS_S16 s16BG_R = 0, s16BG_G = 0, s16BG_B = 0;
    PIXEL stRGBBG, stYCCBG;
    MS_S16 s16BGColor_ValidWei = 0;

    memset(&stRGB, 0, sizeof(PIXEL));
    memset(&stYCC, 0, sizeof(PIXEL));
    memset(&stRGBBG, 0, sizeof(PIXEL));
    memset(&stYCCBG, 0, sizeof(PIXEL));

    // Table A for Sky ( default : white in back )
    {
        for ( u8RIdx = 0; u8RIdx < 8; u8RIdx++ )
        for ( u8GIdx = 0; u8GIdx < 8; u8GIdx++ )
        for ( u8BIdx = 0; u8BIdx < 8; u8BIdx++ )
        {
            s16R = (u8RIdx<<5)*8/7; // u8RIdx<<5;
            s16G = (u8GIdx<<5)*8/7; // u8GIdx<<5;
            s16B = (u8BIdx<<5)*8/7; // u8BIdx<<5;
            s16Depth = 0;

#if ENABLE_CrDEPTH
            s16Y = ( ( 257*s16R + 504*s16G + 98*s16B + 16000)+500 ) / 1000;
            s16Cb = ( (-148*s16R - 291*s16G + 439*s16B + 128000)+500 ) / 1000;
            s16Cr = ( ( 439*s16R - 368*s16G -  71*s16B + 128000)+500 ) / 1000;

            s16CrDepth = MINMAX((s16Cr-20), 0, MAXVALUE) ;
            //int s16YDepth = MAX((s16Y-128), 0);
            s16CbDepth = MINMAX((MAXVALUE- s16Cb), 0, MAXVALUE) ;
            s16Sat = (abs(s16Cr-128)+ abs(s16CbDepth-128));
            s16YDepth = MIN( MAXVALUE-s16Y, s16Y );

            s16Sat_Wei = MINMAX(s16Sat,0,32);
            //int s16CrCbDepth = (2*s16Cr + s16CbDepth)/3;
            s16CrCbDepth = MINMAX((s16CrDepth + s16CbDepth)/2, 0, MAXVALUE);
            s16CrCbDepth= MINMAX(s16CrCbDepth*s16CrCbDepth/MAXVALUE, 0, MAXVALUE);
            s16Depth = (s16CrCbDepth*s16Sat_Wei + s16YDepth *(32-s16Sat_Wei)+16)/32;
            //s16Depth = MINMAX(((s16Depth-128)*3/2)+128,0,MAXVALUE);
#else
            s16Y = ( ( 257*s16R + 504*s16G + 98*s16B + 16000)*4+500 ) / 4000;
            s16Depth = MAX( s16R,s16G ) - MIN3( s16R, s16G, s16B );
            s16Depth = s16Depth - MINMAX( s16B - MAX( s16R, s16G ), 0, MAXVALUE)  + MINMAX(128 - s16B,0,MAXVALUE)/2;
            s16Depth = MAX( s16Depth, MIN3( 180, s16R*3 - s16G*3, s16B*6 - s16G*6 ) );
#endif

            //method3
            stRGB.R = s16R; stRGB.G = s16G; stRGB.B = s16B;
            stYCC = RGB2YUV(stRGB);
            s16BGDiff3 = MAX3( abs(stYCC.G-pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.G)*2/3/4, abs(stYCC.B-pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.B)/4, abs(stYCC.R-pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.R)/4 );
            s16BGdepthWei3 = MINMAX(s16BGDiff3*2/3,0,32);

            s16Depth2 = (s16BGdepthWei3*s16Depth+(32-s16BGdepthWei3)*16+16)>>5;//method3
            s16Depth2 = (pXCResourcePrivate->stdrvXC_3D._s16TpBGCOLOR_ValidWei*s16Depth2+(32-pXCResourcePrivate->stdrvXC_3D._s16TpBGCOLOR_ValidWei)*s16Depth+16)>>5;
            s16Depth2 = (pXCResourcePrivate->stdrvXC_3D._s16TBA_BGColorWei*s16Depth2+(32-pXCResourcePrivate->stdrvXC_3D._s16TBA_BGColorWei)*s16Depth+16)>>5;//normal BG color case

            s16Depth = (s16Depth*pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt+s16Depth2*(32-pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt)+16)>>5;//for blue sky color == BG color

#if DVIEW_WEI
            //for DView
            s16DViewDepth = MINMAX(s16Depth-64,0,MAXVALUE);
            //int s16DViewDepth = MINMAX(s16Depth-100,0,MAXVALUE);
            s16Depth = (s16DViewDepth*pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3+s16Depth*(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)+16)>>5;
#elif DVIEW_WEI2
            //int s16DViewDepth = MINMAX(s16Depth-64,0,MAXVALUE);// Ver 1
            //int s16DViewDepth = MINMAX(s16Depth-100,0,MAXVALUE);// Ver 2
            s16DViewDepth = MINMAX(s16Depth-50,0,MAXVALUE);// Ver 3

            s16Depth = (s16DViewDepth*pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3+s16Depth*(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)+16)>>5;
#endif

            {
                s16YDark = MIN(150, (80-s16Y)*6 );
                s16Depth = ( MINMAX( MAX( s16YDark, s16Depth), 0, MAXVALUE )*u16Index_BL + s16Depth*(10 - u16Index_BL))/10;
            }

            s16Depth = MINMAX( s16Depth, 0, MAXVALUE );
            pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8RIdx][u8GIdx][u8BIdx] = s16Depth;
            //_u8RGB3DLUT_A[u8RIdx][u8GIdx][u8BIdx] = u8GIdx*32;
        }
    }

    // Table B for Non-Sky
    {
        for ( u8RIdx = 0; u8RIdx < 8; u8RIdx++ )
        for ( u8GIdx = 0; u8GIdx < 8; u8GIdx++ )
        for ( u8BIdx = 0; u8BIdx < 8; u8BIdx++ )
        {
            s16R = (u8RIdx<<5)*8/7; // u8RIdx<<5;
            s16G = (u8GIdx<<5)*8/7; // u8GIdx<<5;
            s16B = (u8BIdx<<5)*8/7; // u8BIdx<<5;
            s16Depth = 0;

#if ENABLE_CrDEPTH
            s16Y = ( ( 257*s16R + 504*s16G + 98*s16B + 16000)+500 ) / 1000;
            s16Cb = ( (-148*s16R - 291*s16G + 439*s16B + 128000)+500 ) / 1000;
            s16Cr = ( ( 439*s16R - 368*s16G -  71*s16B + 128000)+500 ) / 1000;

            s16CrDepth = MINMAX((s16Cr-30), 0, MAXVALUE) ;
            s16CbDepth = MINMAX((MAXVALUE- s16Cb), 0, MAXVALUE) ;
            //int s16CrCbDepth = MINMAX((2*s16Cr + s16CbDepth)/3, 0, MAXVALUE);
            s16CrCbDepth =  MINMAX((s16Cr + s16CbDepth)/2, 0, MAXVALUE);
            //int s16Sat = MAX(abs(s16Cr-128), abs(s16CbDepth-128)) + MIN(abs(s16Cr-128), abs(s16CbDepth-128))/2;
            s16Sat =(abs(s16Cr-128)+ abs(s16CbDepth-128));
            s16YDepth = MIN( 180, s16Y);

            s16Sat_Wei = MINMAX(s16Sat-5,0,32);
            s16Depth = (s16CrCbDepth*s16Sat_Wei + s16YDepth *(32-s16Sat_Wei)+16)/32;
            //s16Depth = MINMAX(((s16Depth-128)*3/2)+128,0,MAXVALUE);
#else
            s16Y = ( ( 257*s16R + 504*s16G + 98*s16B + 16000)*4+500 ) / 4000;
            s16Depth = MAX( s16R,s16G ) - MIN3( s16R, s16G, s16B );
            s16Depth = s16Depth - MINMAX( s16B - MAX( s16R, s16G ), 0, MAXVALUE)  + MINMAX(128 - s16B,0,MAXVALUE)/2;
            s16Depth = MAX( s16Depth, MIN3( 180, s16R*3 - s16G*3, s16B*6 - s16G*6 ) );
#endif

#if ENABLE_CrDEPTH
            s16Depth = s16Depth;
#else
            s16Sat = (MAX3(s16R,s16G,s16B)==0)?0:((MAX3(s16R,s16G,s16B)-MIN3(s16R,s16G,s16B))*256)/MAX3(s16R,s16G,s16B);
            s16YDepth = MIN( 200, s16Y )-( MAX(s16Sat-32,0)*5/4 );
            s16Depth = MINMAX( MAX( s16YDepth, s16Depth), 0, MAXVALUE );
#endif

            //Tp/Dn BG color blending
            stRGB.R = s16R; stRGB.G = s16G; stRGB.B = s16B;
            stYCC = RGB2YUV(stRGB);

            s16BG_R = (pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei*pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.R+(32-pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei)*pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.R+16)>>5;
            s16BG_G = (pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei*pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.G+(32-pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei)*pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.G+16)>>5;
            s16BG_B = (pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei*pXCResourcePrivate->stdrvXC_3D._stBgPixelTp.B+(32-pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei)*pXCResourcePrivate->stdrvXC_3D._stBgPixelDn.B+16)>>5;

            stRGBBG.R = s16BG_R; stRGBBG.G = s16BG_G; stRGBBG.B = s16BG_B;
            stYCCBG = RGB2YUV(stRGBBG);

            s16BGDiff3 = MAX3( abs(stYCC.G-stYCCBG.G)*2/3/4, abs(stYCC.B-stYCCBG.B)/4, abs(stYCC.R-stYCCBG.R)/4 );
            s16BGdepthWei3 = MINMAX(s16BGDiff3*2/3,0,32);

            //BG color back off s16Depth
            s16Depth2 = (s16BGdepthWei3*s16Depth+(32-s16BGdepthWei3)*32+16)>>5;//method3

            s16BGColor_ValidWei = MAX(pXCResourcePrivate->stdrvXC_3D._s16TpBGCOLOR_ValidWei,pXCResourcePrivate->stdrvXC_3D._s16DnBGCOLOR_ValidWei);
            s16Depth2 = (s16BGColor_ValidWei*s16Depth2+(32-s16BGColor_ValidWei)*s16Depth+16)>>5;
            s16Depth2 = (pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorWei*s16Depth2+(32-pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorWei)*s16Depth+16)>>5;//normal BG color case
            s16Depth2 = (pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3*s16Depth+(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)*s16Depth2+16)>>5;// if Distant View, not apply BG color to TBB
            s16Depth = (s16Depth*pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt+s16Depth2*(32-pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt)+16)>>5;//for blue sky color == BG color

#if DVIEW_WEI
            //for DView
            s16DViewDepth = MINMAX((s16Depth*4/5)+160,0,MAXVALUE);
            s16Depth = (s16DViewDepth*pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3+s16Depth*(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)+16)>>5;
#elif DVIEW_WEI2
            s16DViewDepth = MINMAX((s16Depth*4/5)+100,0,MAXVALUE); //ver 2
            s16Depth = (s16DViewDepth*pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3+s16Depth*(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)+16)>>5;
#endif
            //backlight
            s16YDark = (MAXVALUE-s16Y);
            s16Depth = (MINMAX( MAX( s16Depth*4/5, s16YDark), 0, MAXVALUE )*u16Index_BL + s16Depth*(10 - u16Index_BL))/10;

            s16Depth = MINMAX( s16Depth, 0, MAXVALUE );
            pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8RIdx][u8GIdx][u8BIdx] = s16Depth;
            //_u8RGB3DLUT_B[u8RIdx][u8GIdx][u8BIdx] = gIdx*32;
        }
    }

#if PRINTINFO
    printf("======Table A======\n");
    printf("_s16TpBGCOLOR_ValidWei = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16TpBGCOLOR_ValidWei );
    printf("_s16TBA_BGColorWei = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16TBA_BGColorWei );
    printf("_s16BothTpBGSKY_COLORCnt = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt );
    printf("_s16DistantViewRatio3 = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3 );
    printf("u16Index_BL = 0x%X\n", u16Index_BL  );
    printf("=====Table B======\n");
    printf("_s16TBB_BGColorSelWei  = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorSelWei  );
    printf("s16BGColor_ValidWei = 0x%X\n", s16BGColor_ValidWei );
    printf("_s16TBB_BGColorWei = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16TBB_BGColorWei );
    printf("_s16BothDnBGSKY_COLORCnt = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt );
    printf("_s16DistantViewRatio3 = 0x%X\n", pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3 );
    printf("u16Index_BL = 0x%X\n", u16Index_BL );
#endif
}
#elif (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT2_U9(void *pInstance)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 rIdx,gIdx,bIdx;
    MS_S16 r, g, b,depth,depth2;
    MS_S16 y,Cb,Cr;
    MS_S16 CrDepth,CbDepth,sat,yDepth,CrCbDepth;
    MS_S16 sat_wei;
    MS_S16 DViewDepth;
//  MS_S16 yDark;

    PIXEL pRGB,pYCC;
    MS_S16 BGDiff3,BGdepthWei3;

    //Only in TableB
//  MS_S16 BG_R,BG_G,BG_B;
//  PIXEL pRGBBG,pYCCBG;
//  MS_S16 BGCOLOR_ValidWei;
    MS_U8 BGColorTrustWei;
    MS_U8 BGDepth;

//  MS_U8 BGColorTrustWei_forTBABGColor;
    MS_U8 BGDepth_forTpBGColor;
    MS_S16 BGDiff3_forTpBGColor,BGdepthWei3_forTpBGColor, depth2_forTpBGColor;

    MS_S16 Cbtmp,Crtmp;


    MS_S16 u8testval1 = 0x20;//MFC_ReadByte(REG_0EB6);//0x20
    MS_S16 u8testval2 = 0x40;//MFC_ReadByte(REG_0EB7);//0x40
    MS_S16 u8testval3 = 0x20;//MFC_ReadByte(REG_0EB8);//0x20
    MS_S16 u8testval4 = 0x20;//MFC_ReadByte(REG_0EB9);//0x20
#if DUMYGAINCTRL
    MS_U8 u8tmpval = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5A_L, 0xff));//MFC_ReadByte(REG_1EFA);
    MS_U8 u8tmpval2 = 0x00;

    MS_S16 u8testval7 = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5C_L, 0xff)); //for edge depth
    MS_S16 u8testval8 = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5D_L, 0xff00))>>8;//0x07; //for [0]:cbcr enable  [1]:y enable [2]:edge enable
    MS_S16 u8testval9 = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5D_L, 0xff));//0x10; //for sat_wei
    MS_S16 u8testvalA = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5E_L, 0xff00))>>8;//0x10; //for y_depth gain
#else
    MS_U8 u8tmpval = 0x00;// = MFC_ReadByte(REG_1EFA);
    MS_U8 u8tmpval2 = 0x00;// = MFC_ReadByte(REG_1EFB);

    MS_S16 u8testval7 = 0x30; //for edge depth
    MS_S16 u8testval8 = 0x07; //for [0]:cbcr enable  [1]:y enable [2]:edge enable
    MS_S16 u8testval9 = 0x10; //for sat_wei
    MS_S16 u8testvalA = 0x10; //for y_depth gain
#endif



    //U8 u8tmpval1EF8 = MFC_ReadByte(REG_1EF8);
    MS_S16 u8tmpval1EF9 = 0x50;//MFC_ReadByte(REG_1EF9);

    // Table A for Sky ( default : white in back )
    {
        for ( rIdx = 0; rIdx < 8; rIdx++ )
            for ( gIdx = 0; gIdx < 8; gIdx++ )
                for ( bIdx = 0; bIdx < 8; bIdx++ )
                {
                    r = (rIdx<<5)*8/7; // rIdx<<5;
                    g = (gIdx<<5)*8/7; // gIdx<<5;
                    b = (bIdx<<5)*8/7; // bIdx<<5;
                    depth = 0;

                    y = ( ( 257*r + 504*g + 98*b + 16000)+500 ) / 1000;
                    Cb = ( (-148*r - 291*g + 439*b + 128000)+500 ) / 1000;
                    Cr = ( ( 439*r - 368*g -  71*b + 128000)+500 ) / 1000;

                    CrDepth = MINMAX((Cr)*16/15, 0, 255) ;
                    CbDepth = MINMAX((256- Cb)*16/15, 0, 255) ;

                    //enhance red/yellow color depth to green color
                    CrDepth = MAX(CrDepth-u8tmpval1EF9,0);
                    CrDepth = MIN((CrDepth*u8testval4)>>4,255);

                    sat = (abs(Cr-128)+ abs(Cb-128));


                    yDepth = MIN( 255-y, y );


                    MS_U32 black_base_depth;
                    black_base_depth = 0x60;//MFC_ReadByte(REG_1EFE);

                    //2013.06.27 make white color depth could be lower than black color in TBA, and black color depth limited to 0x60, for Sky pattern
                    //if(1)
                    {
                        MS_U32 ydepth_base;
                        ydepth_base = 255;

                        yDepth = MAX(MIN( ydepth_base-y, y ),0);

                        if((u8tmpval&0x20)==0x00)
                        {
                            if(y>(ydepth_base-y))
                                yDepth = yDepth;//make white go more back, not be limited
                            else
                                yDepth = MAX(yDepth,black_base_depth);
                        }
                        else
                            yDepth = yDepth;
                    }

                    sat_wei = MINMAX(sat-16,0,32);

                    //make blue color go back more
                    {
                        MS_U8 blueback_item;
                        //blueback_item = MAX(b-MAX(r,g)+MIN(r,g),0);
                        if(0)
                        {
                            Cbtmp = MAX(Cb-128,0);
                            Crtmp = Cr-128;
                            if(Crtmp>=0)
                                blueback_item = MAX(Cbtmp-2*Crtmp,0);
                            else
                                blueback_item = MAX(Cbtmp+Crtmp/2,0);
                        }
                        else
                            blueback_item = MAX(b-MIN(r,g*3/2),0);
#if 0
                        if((u8tmpval&0x10)==0x00)
                            CrCbDepth = MINMAX((CrDepth + CbDepth)/2 - blueback_item, 0, 255);
                        else
                            CrCbDepth = (CrDepth + CbDepth)/2;
#else
                        CrCbDepth = MINMAX((CrDepth + CbDepth)/2- blueback_item, 0, 255);
#endif
                    }
#if 0
                    if((u8tmpval&0x03)==0x03)
                        depth = 0;
                    else if((u8tmpval&0x03)==0x01)
                        depth = CrCbDepth;
                    else if((u8tmpval&0x03)==0x02)
                        depth = yDepth;
                    else if((u8tmpval&0x03)==0x00)
                        depth = (CrCbDepth*sat_wei + yDepth *(32-sat_wei)+16)/32;
                    else
                        depth = 0;
#else
                    sat_wei = MIN((sat_wei*u8testval9)>>4,32);//0~32
                    yDepth = (yDepth*u8testvalA)>>4;//0~32
                    if((u8testval8&0x03)==0x01)
                        depth = CrCbDepth;
                    else if((u8testval8&0x03)==0x02)
                        depth = yDepth;
                    else if((u8testval8&0x03)==0x03)
                        depth = (CrCbDepth*sat_wei + yDepth *(32-sat_wei)+16)/32;
#endif
                    //method3
                    pRGB.R = r;
                    pRGB.G = g;
                    pRGB.B = b;
                    pYCC = RGB2YUV(pRGB);
                    if((u8tmpval&0x40)==0x00)
                    {
                        BGColorTrustWei = MINMAX(pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntforTp/4,0,32);
                        BGDepth = ( MIN(16,depth)*BGColorTrustWei+depth*(32-BGColorTrustWei))>>5;

                        //BGDiff3 = MAX( abs(pYCC.B/4-BGColor_CbTp), abs(pYCC.R/4-BGColor_CrTp) );
                        BGDiff3 = MAX( abs(Cb-pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp), abs(Cr-pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp) );
                        BGdepthWei3 = MINMAX(BGDiff3,0,32);

                        depth2 = (BGdepthWei3*depth+(32-BGdepthWei3)*BGDepth+16)>>5;//method3
                    }
                    else
                    {
                        BGDiff3 = MAX3( abs(pYCC.G-pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.G)*2/3/4, abs(pYCC.B-pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.B)/4, abs(pYCC.R-pXCResourcePrivate->stdrvXC_3D._stBgPixelTpYCC.R)/4 );
                        BGdepthWei3 = MINMAX(BGDiff3*2/3,0,32);
                        depth2 = (BGdepthWei3*depth+(32-BGdepthWei3)*16+16)>>5;//method3
                    }


                    if((u8tmpval&0x04)==0x00)
                        depth = MIN(depth,depth2);



                    DViewDepth = MINMAX(depth,0,255);

                    depth = (DViewDepth*pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3+depth*(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)+16)>>5;


                    //LOCALDEPTH_ENHANCE
                    MS_U32 skyRatio1, skyRatio2;
                    MS_U32 skyRatioDiff_wei;

                    MS_U32 gainU=0, gainD=0;
                    MS_U32 offset;

                    skyRatio1 = (MS_U32)pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1;
                    skyRatio2 = (MS_U32)MIN(pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1,pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2);

                    skyRatioDiff_wei = skyRatio1 - skyRatio2;//0~32
                    {
                        gainU = 4;
                        gainD = 3;
                    }

                    offset = MAX(128*gainU/gainD - 128,0);
                    if((u8tmpval&0x08)==0x00)
                        depth = depth + ( depth* (gainU-gainD)/gainD * skyRatioDiff_wei/32 - offset * skyRatioDiff_wei/32);


                    //for edge enhance in TBA
                    MS_U32 egde_depth;
                    if((u8testval8&0x04)==0x04)
                        egde_depth = ((u8testval3)*(u8testval7))>>4;// 8.0*2.4
                    else
                        egde_depth = 0;

                    depth = depth - egde_depth;


                    depth = MINMAX( depth, 0, 255 );
                    /*if((SC_R2BYTE(REG_SC_BK23_59_L)&0x00f0)==0x00)
                    {
                    pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[rIdx][gIdx][bIdx] = depth;
                    }
                    else*/
                    {
                        pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[rIdx][gIdx][bIdx] = depth;
                        //pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[rIdx][gIdx][bIdx] = depth;

                    }
                }
    }


    // Table B for Non-Sky
    {
        for ( rIdx = 0; rIdx < 8; rIdx++ )
            for ( gIdx = 0; gIdx < 8; gIdx++ )
                for ( bIdx = 0; bIdx < 8; bIdx++ )
                {
                    r = (rIdx<<5)*8/7; // rIdx<<5;
                    g = (gIdx<<5)*8/7; // gIdx<<5;
                    b = (bIdx<<5)*8/7; // bIdx<<5;
                    depth = 0;

                    y = ( ( 257*r + 504*g + 98*b + 16000)+500 ) / 1000;
                    Cb = ( (-148*r - 291*g + 439*b + 128000)+500 ) / 1000;
                    Cr = ( ( 439*r - 368*g -  71*b + 128000)+500 ) / 1000;

                    CrDepth = MINMAX((Cr)*16/15, 0, 255) ;
                    CbDepth = MINMAX((256- Cb)*16/15, 0, 255) ;

                    //enhance red/yellow color depth to green color
                    CrDepth = MAX(CrDepth-u8tmpval1EF9,0);
                    CrDepth = MIN((CrDepth*u8testval4)>>4,255);


                    //make blue color go back more
                    {
                        MS_U8 blueback_item;
                        //blueback_item = MAX(b-MAX(r,g)+MIN(r,g),0);
                        if(0)
                        {
                            Cbtmp = MAX(Cb-128,0);
                            Crtmp = Cr-128;
                            if(Crtmp>=0)
                                blueback_item = MAX(Cbtmp-2*Crtmp,0);
                            else
                                blueback_item = MAX(Cbtmp+Crtmp/2,0);
                        }
                        else
                            blueback_item = MAX(b-MIN(r,g*3/2),0);
#if 0
                        if((u8tmpval&0x10)==0x00)
                            CrCbDepth = MINMAX((CrDepth + CbDepth)/2 - blueback_item, 0, 255);
                        else
                            CrCbDepth = (CrDepth + CbDepth)/2;
#else
                        CrCbDepth = MINMAX(CrDepth, 0, 255);
#endif
                    }

                    sat =(abs(Cr-128)+ abs(Cb-128));

                    yDepth = MIN( 180, y);

                    {
                        MS_U32 black_base_depth;
                        black_base_depth = (MS_U32)u8testval2;//48//u8tmpval2;//

                        if((u8tmpval&0x20)==0x00)
                            yDepth = MAX(yDepth,black_base_depth);
                        else
                            yDepth = yDepth;

                        yDepth = yDepth + u8testval1;
                    }




                    sat_wei = MINMAX(sat-16,0,32);
#if 0
                    if((u8tmpval&0x03)==0x03)
                        depth = 0;
                    else if((u8tmpval&0x03)==0x01)
                        depth = CrCbDepth;
                    else if((u8tmpval&0x03)==0x02)
                        depth = yDepth;
                    else if((u8tmpval&0x03)==0x00)
                        depth = (CrCbDepth*sat_wei + yDepth *(32-sat_wei)+16)/32;
                    else
                        depth = 0;
#else
                    sat_wei = MIN((sat_wei*u8testval9)>>4,32);//0~32
                    yDepth = (yDepth*u8testvalA)>>4;//0~32
                    if((u8testval8&0x03)==0x01)
                        depth = CrCbDepth;
                    else if((u8testval8&0x03)==0x02)
                        depth = yDepth;
                    else if((u8testval8&0x03)==0x03)
                        depth = (CrCbDepth*sat_wei + yDepth *(32-sat_wei)+16)/32;
#endif
                    //method3
                    pRGB.R = r;
                    pRGB.G = g;
                    pRGB.B = b;
                    pYCC = RGB2YUV(pRGB);
                    if((u8tmpval&0x40)==0x00)
                    {
                        BGColorTrustWei = MINMAX(pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntforBn/4,0,32);
                        BGDepth = ( MIN(32,depth)*BGColorTrustWei+depth*(32-BGColorTrustWei))>>5;

                        //BGDiff3 = MAX( abs(pYCC.B/4-BGColor_CbBn), abs(pYCC.R/4-BGColor_CrBn) );
                        BGDiff3 = MAX( abs(Cb-pXCResourcePrivate->stdrvXC_3D.BGColor_CbBn), abs(Cr-pXCResourcePrivate->stdrvXC_3D.BGColor_CrBn) );
                        BGdepthWei3 = MINMAX(BGDiff3,0,32);

                        //BG color back off depth
                        depth2 = (BGdepthWei3*depth+(32-BGdepthWei3)*BGDepth+16)>>5;//method3

                        //for the strength of TpBGcolor in TBB
                        BGDepth_forTpBGColor = ( MIN(16,depth)*pXCResourcePrivate->stdrvXC_3D._u8TBABGColorWei+depth*(32-pXCResourcePrivate->stdrvXC_3D._u8TBABGColorWei))>>5;
                        BGDiff3_forTpBGColor = MAX( abs(Cb-pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp), abs(Cr-pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp) );
                        BGdepthWei3_forTpBGColor = MINMAX(BGDiff3_forTpBGColor,0,32);
                        //TpBG color back off depth
                        depth2_forTpBGColor = (BGdepthWei3_forTpBGColor*depth+(32-BGdepthWei3_forTpBGColor)*BGDepth_forTpBGColor+16)>>5;//method3

                        //if use TpBGColor
                        if((u8tmpval2&0x01)==0x00)
                            depth2 = MIN(depth2,depth2_forTpBGColor);

                    }
                    else
                    {
                        BGDiff3 = MAX3( abs(pYCC.G-pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC.G)*2/3/4, abs(pYCC.B-pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC.B)/4, abs(pYCC.R-pXCResourcePrivate->stdrvXC_3D._stBgPixelDnYCC.R)/4 );
                        BGdepthWei3 = MINMAX(BGDiff3*2/3,0,32);
                        //BG color back off depth
                        depth2 = (BGdepthWei3*depth+(32-BGdepthWei3)*32+16)>>5;//method3
                    }


                    if((u8tmpval&0x04)==0x00)
                        depth = MIN(depth,depth2);


                    DViewDepth = MINMAX(depth,0,255); //ver 2
                    depth = (DViewDepth*pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3+depth*(32-pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3)+16)>>5;


                    //LOCALDEPTH_ENHANCE
                    MS_U32 skyRatio1, skyRatio2;
                    MS_U32 skyRatioDiff_wei;

                    MS_U32 gainU=0, gainD=0;
                    MS_U32 offset;

                    skyRatio1 = (MS_U32)pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1;
                    skyRatio2 = (MS_U32)MIN(pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1,pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2);

                    skyRatioDiff_wei = skyRatio1 - skyRatio2;//0~32
                    {
                        gainU = 4;
                        gainD = 3;
                    }

                    offset = MAX(128*gainU/gainD - 128,0);
                    if((u8tmpval&0x08)==0x00)
                        depth = depth + ( depth* (gainU-gainD)/gainD * skyRatioDiff_wei/32 - offset * skyRatioDiff_wei/32);


                    //for edge enhance in TBB
                    MS_U32 egde_depth;
                    if((u8testval8&0x04)==0x04)
                        egde_depth = ((u8testval3)*(u8testval7))>>4;// 8.0*2.4
                    else
                        egde_depth = 0;

                    depth = depth + egde_depth;

                    depth = MINMAX( depth, 0, 255 );

                    pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[rIdx][gIdx][bIdx] = depth;


                }
    }


}

#endif
static MS_S32 _MDrv_SC_3D_2DTo3D_InterpolationDepthFromRGB_BLK(void *pInstance, MS_U16 u16X, MS_U16 u16Y, MS_U16 u16R, MS_U16 u16G, MS_U16 u16B)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_S32 s32DepthA = 0;
    MS_S32 s32DepthB = 0;
    MS_S32 s32BlendWeight = 0;
    MS_S32 s32Depth = 0;
    MS_S32 s32R00 = 0, s32R01 = 0, s32R10 = 0, s32R11 = 0, s32G0 = 0, s32G1 = 0;

    MS_S32 s32R = 0, s32G = 0, s32B = 0;
    MS_S32 s32RIdx = 0, s32GIdx = 0, s32BIdx = 0;
    MS_S32 s32RIdxN = 0, s32GIdxN = 0, s32BIdxN = 0;
    MS_S32 s32RRatio = 0, s32GRatio = 0, s32BRatio = 0;

    // Check RGB 3D LUT
    s32R = MINMAX( u16R * 7 / 8, 0, 224*4 );
    s32G = MINMAX( u16G * 7 / 8, 0, 224*4 );
    s32B = MINMAX( u16B * 7 / 8, 0, 224*4 );
    s32RIdx = s32R>>7; // u16R>>5;
    s32GIdx = s32G>>7; // u16G>>5;
    s32BIdx = s32B>>7; // u16B>>5;
    s32RIdxN = MIN( s32RIdx+1, 7 ); //MIN( s32RIdx+1, 8 );
    s32GIdxN = MIN( s32GIdx+1, 7 ); //MIN( s32GIdx+1, 8 );
    s32BIdxN = MIN( s32BIdx+1, 7 ); //MIN( s32BIdx+1, 8 );
    s32RRatio = s32R & 0x7f;
    s32GRatio = s32G & 0x7f;
    s32BRatio = s32B & 0x7f;

    //RGB3DLUT_A
    {
        // Interpolate in R domain
        s32R00 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdx][s32GIdx ][s32BIdx],  pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdxN][s32GIdx ][s32BIdx], s32RRatio );
        s32R01 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdx][s32GIdxN][s32BIdx],  pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdxN][s32GIdxN][s32BIdx], s32RRatio );
        s32R10 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdx][s32GIdx ][s32BIdxN], pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdxN][s32GIdx ][s32BIdxN], s32RRatio );
        s32R11 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdx][s32GIdxN][s32BIdxN], pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[s32RIdxN][s32GIdxN][s32BIdxN], s32RRatio );
        // Interpolate in G domain
        s32G0 = BLEND128( s32R00, s32R01, s32GRatio );
        s32G1 = BLEND128( s32R10, s32R11, s32GRatio );
        // Interpolate in B domain
        s32DepthA = BLEND128( s32G0, s32G1, s32BRatio );
    }

    //RGB3DLUT_B
    {
        // Interpolate in R domain
        s32R00 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdx][s32GIdx ][s32BIdx],  pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdxN][s32GIdx ][s32BIdx], s32RRatio );
        s32R01 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdx][s32GIdxN][s32BIdx],  pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdxN][s32GIdxN][s32BIdx], s32RRatio );
        s32R10 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdx][s32GIdx ][s32BIdxN], pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdxN][s32GIdx ][s32BIdxN], s32RRatio );
        s32R11 = BLEND128( pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdx][s32GIdxN][s32BIdxN], pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[s32RIdxN][s32GIdxN][s32BIdxN], s32RRatio );
        // Interpolate in G domain
        s32G0 = BLEND128( s32R00, s32R01, s32GRatio );
        s32G1 = BLEND128( s32R10, s32R11, s32GRatio );
        // Interpolate in B domain
        s32DepthB = BLEND128( s32G0, s32G1, s32BRatio );
    }

#if 0
    // blend s32DepthA & s32DepthB
    {
        // bilinear interpolation regionWeight
        wUL = _u8RegionBlkWeightFrame[MINMAX(u16Y/VERTICAL_STEP_OUT,0,NBLK_V_OUT-1)][MINMAX(u16X/HORIZONTAL_STEP_OUT,0,NBLK_H_OUT-1)];
        wUR = _u8RegionBlkWeightFrame[MINMAX(u16Y/VERTICAL_STEP_OUT,0,NBLK_V_OUT-1)][MINMAX(u16X/HORIZONTAL_STEP_OUT+1,0,NBLK_H_OUT-1)];
        wDL = _u8RegionBlkWeightFrame[MINMAX(u16Y/VERTICAL_STEP_OUT+1,0,NBLK_V_OUT-1)][MINMAX(u16X/HORIZONTAL_STEP_OUT,0,NBLK_H_OUT-1)];
        wDR = _u8RegionBlkWeightFrame[MINMAX(u16Y/VERTICAL_STEP_OUT+1,0,NBLK_V_OUT-1)][MINMAX(u16X/HORIZONTAL_STEP_OUT+1,0,NBLK_H_OUT-1)];
        xRatio = u16X%HORIZONTAL_STEP_OUT;
        wU = ( (HORIZONTAL_STEP_OUT-xRatio)*wUL + xRatio*wUR ) / HORIZONTAL_STEP_OUT;
        wD = ( (HORIZONTAL_STEP_OUT-xRatio)*wDL + xRatio*wDR ) / HORIZONTAL_STEP_OUT;
        yRatio = u16Y%VERTICAL_STEP_OUT;
        s32BlendWeight = ( (VERTICAL_STEP_OUT-yRatio)*wU + yRatio*wD ) / VERTICAL_STEP_OUT;
    }
    s32Depth = ( s32DepthA * ( 256 - s32BlendWeight ) + s32DepthB * s32BlendWeight ) >> 8;
#else
    s32BlendWeight = pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[u16Y][u16X];
    s32Depth = ( s32DepthA * ( 256 - s32BlendWeight ) + s32DepthB * s32BlendWeight ) >> 8;
#endif
    if(pXCResourcePrivate->stdrvXC_3D._u8DepthAB == 1)
        s32Depth = s32DepthA;
    else if (pXCResourcePrivate->stdrvXC_3D._u8DepthAB == 2)
        s32Depth = s32DepthB;
    else if (pXCResourcePrivate->stdrvXC_3D._u8DepthAB == 3)
        s32Depth = s32BlendWeight;

    return s32Depth;
}

static void _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT_IIR(void *pInstance)
{
    MS_S16 s16CurDepthA = 0, s16CurDepthB = 0;
    MS_S16 s16PreDepthA = 0, s16PreDepthB = 0;
    MS_U8 u8RIdx = 0, u8GIdx = 0, u8BIdx = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( u8RIdx = 0; u8RIdx < 8; u8RIdx++ )
    for ( u8GIdx = 0; u8GIdx < 8; u8GIdx++ )
    for ( u8BIdx = 0; u8BIdx < 8; u8BIdx++ )
    {
        s16CurDepthA = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8RIdx][u8GIdx][u8BIdx];
        s16CurDepthB = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8RIdx][u8GIdx][u8BIdx];
        s16PreDepthA = pXCResourcePrivate->stdrvXC_3D.u16Pre_RGB3DLUT_A[u8RIdx][u8GIdx][u8BIdx];
        s16PreDepthB = pXCResourcePrivate->stdrvXC_3D.u16Pre_RGB3DLUT_B[u8RIdx][u8GIdx][u8BIdx];

        s16CurDepthA = s16CurDepthA<<4;
        s16CurDepthB = s16CurDepthB<<4;

        s16CurDepthA = (s16PreDepthA*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+s16CurDepthA*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;
        s16CurDepthB = (s16PreDepthB*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+s16CurDepthB*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;

        pXCResourcePrivate->stdrvXC_3D.u16Pre_RGB3DLUT_A[u8RIdx][u8GIdx][u8BIdx] = s16CurDepthA;
        pXCResourcePrivate->stdrvXC_3D.u16Pre_RGB3DLUT_B[u8RIdx][u8GIdx][u8BIdx] = s16CurDepthB;

        pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8RIdx][u8GIdx][u8BIdx] = (s16CurDepthA+8)>>4;
        pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8RIdx][u8GIdx][u8BIdx] = (s16CurDepthB+8)>>4;
    }
}

static void _MDrv_SC_3D_2DTo3D_PrepareBLKDepth_IIR(void *pInstance)
{
    MS_S16 s16X = 0, s16Y = 0;
    MS_U16 u16CurDepth = 0, u16PreDepth = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for ( s16Y = 0; s16Y < NBLK_DR_V; s16Y ++ )
    for ( s16X = 0; s16X < NBLK_DR_H; s16X ++ )
    {
        u16CurDepth = pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[s16Y][s16X];
        u16PreDepth = pXCResourcePrivate->stdrvXC_3D.u16Pre_depthBlkBased[s16Y][s16X];
        u16CurDepth = u16CurDepth<<4;
        u16CurDepth = (u16PreDepth*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+u16CurDepth*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;
        pXCResourcePrivate->stdrvXC_3D.u16Pre_depthBlkBased[s16Y][s16X] = u16CurDepth;
        pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[s16Y][s16X] = (u16CurDepth+8)>>4;
    }
}
#if (HW_2DTO3D_VER == 3)
static void _MDrv_SC_3D_2DTo3D_UpdateRGB3DLUT(void *pInstance)
{
    MS_U8 u8AddrIdx = 0;
    MS_U8 u8IdxR = 0, u8IdxG = 0, u8IdxB = 0;
    MS_U8 u8Idx = 0;
    MS_U8 u8SRAMDataA[8][64];
    MS_U8 u8SRAMDataB[8][64];
    MS_U8 u8SRAMDataIn0 = 0;
    MS_U8 u8SRAMDataIn1 = 0;
    MS_U8 u8SRAMDataIn2 = 0;
    MS_U8 u8SRAMDataIn3 = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(u8AddrIdx =0; u8AddrIdx<64; u8AddrIdx++)
    {
        u8IdxR = u8AddrIdx&0x03;
        u8IdxG = (u8AddrIdx>>2)&0x03;
        u8IdxB = (u8AddrIdx>>4)&0x03;
        u8SRAMDataA[0][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataA[1][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataA[2][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataA[3][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataA[4][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataA[5][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataA[6][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+1];
        u8SRAMDataA[7][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+1];

        u8SRAMDataB[0][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataB[1][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataB[2][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataB[3][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataB[4][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataB[5][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataB[6][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+1];
        u8SRAMDataB[7][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+1];
    }

    //select no SRAM & Enable SRAM write
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x0000);//Choose in TBA,Choose in TBB
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x00,0x80);//SRAM write disable

    for(u8Idx =0; u8Idx<8; u8Idx++)
    for(u8AddrIdx =0; u8AddrIdx<16; u8AddrIdx++)
    {
        u8SRAMDataIn0 = u8SRAMDataA[u8Idx][u8AddrIdx*4+0];
        u8SRAMDataIn1 = u8SRAMDataA[u8Idx][u8AddrIdx*4+1];
        u8SRAMDataIn2 = u8SRAMDataA[u8Idx][u8AddrIdx*4+2];
        u8SRAMDataIn3 = u8SRAMDataA[u8Idx][u8AddrIdx*4+3];

        //Write SRAM A
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, u8AddrIdx<<8, 0xff00);//SRAM Addr
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, (0x1<<u8Idx)&0xFF, 0xff);//Choose SRAM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, (u8SRAMDataIn1<<8) | u8SRAMDataIn0);//SRAM Data
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_75_L, (u8SRAMDataIn3<<8)|u8SRAMDataIn2);//SRAM Data
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x80,0x80);//SRAM write enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x00,0x80);//SRAM write disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x00, 0xff);//Choose no SRAM
    }

    for(u8Idx =0; u8Idx<8; u8Idx++)
    for(u8AddrIdx =0; u8AddrIdx<16; u8AddrIdx++)
    {
        u8SRAMDataIn0 = u8SRAMDataB[u8Idx][u8AddrIdx*4+0];
        u8SRAMDataIn1 = u8SRAMDataB[u8Idx][u8AddrIdx*4+1];
        u8SRAMDataIn2 = u8SRAMDataB[u8Idx][u8AddrIdx*4+2];
        u8SRAMDataIn3 = u8SRAMDataB[u8Idx][u8AddrIdx*4+3];

        //Write SRAM B
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, u8AddrIdx<<8, 0xff00);//SRAM Addr
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, ((0x1<<u8Idx)&0xFF)<<8, 0xff00);//Choose SRAM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, (u8SRAMDataIn1<<8) | u8SRAMDataIn0);//SRAM Data
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_75_L, (u8SRAMDataIn3<<8)|u8SRAMDataIn2);//SRAM Data
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x80,0x80);//SRAM write enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x00,0x80);//SRAM write disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x0000, 0xff00);//Choose no SRAM
    }
}
#elif (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_PreSetting(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

//#define NBLK_DR_H (pXCResourcePrivate->stdrvXC_3D._u8NBLK_DR_H);
//#define NBLK_DR_V (pXCResourcePrivate->stdrvXC_3D._u8NBLK_DR_V);
//#define PANEL_WIDTH (pXCResourcePrivate->stdrvXC_3D._u16PanelWidth)
//#define PANEL_HEIGHT (pXCResourcePrivate->stdrvXC_3D._u16PanelHeight)

    //Get panel size output width
    MS_BOOL bT3DDE_Use_MainDisplayWindow = _MDrv_Is_T3DDE_UseMainDisplayWindow(pInstance);
    if(bT3DDE_Use_MainDisplayWindow)
    {
        PANEL_WIDTH = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L) + 1;
        PANEL_HEIGHT= SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L) + 1;
    }
    else
    {
        PANEL_WIDTH = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L) + 1;
        PANEL_HEIGHT= SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L) + 1;
    }
    if(PANEL_WIDTH==0)
        PANEL_WIDTH = 0xf00;
    if(PANEL_HEIGHT==0)
        PANEL_HEIGHT = 0x870;

    NBLK_DR_H = ((PANEL_WIDTH + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK);  //dr h block
    NBLK_DR_V = ((PANEL_HEIGHT + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK); //dr v block

}

static void _MDrv_SC_3D_2DTo3D_UpdateRGB3DLUT_U9(void *pInstance)
{
    MS_U8 u8AddrIdx = 0;
    MS_U8 u8IdxR = 0, u8IdxG = 0, u8IdxB = 0;
    MS_U8 u8Idx = 0;
    MS_U8 u8SRAMDataA[8][64];
    MS_U8 u8SRAMDataB[8][64];
    MS_U8 u8SRAMDataIn0 = 0;
    MS_U8 u8SRAMDataIn1 = 0;
    MS_U8 u8SRAMDataIn2 = 0;
    MS_U8 u8SRAMDataIn3 = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(u8AddrIdx =0; u8AddrIdx<64; u8AddrIdx++)
    {
        u8IdxR = u8AddrIdx&0x03;
        u8IdxG = (u8AddrIdx>>2)&0x03;
        u8IdxB = (u8AddrIdx>>4)&0x03;
        u8SRAMDataA[0][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataA[1][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataA[2][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataA[3][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataA[4][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataA[5][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataA[6][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+1];
        u8SRAMDataA[7][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_A[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+1];

        u8SRAMDataB[0][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataB[1][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+0];
        u8SRAMDataB[2][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataB[3][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+0];
        u8SRAMDataB[4][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataB[5][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+0][u8IdxB*2+1];
        u8SRAMDataB[6][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+0][u8IdxG*2+1][u8IdxB*2+1];
        u8SRAMDataB[7][u8AddrIdx] = pXCResourcePrivate->stdrvXC_3D._u8RGB3DLUT_B[u8IdxR*2+1][u8IdxG*2+1][u8IdxB*2+1];
    }

    //select no SRAM & Enable SRAM write
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x0000);//Choose in TBA,Choose in TBB
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x00,0x80);//SRAM write disable

    for(u8Idx =0; u8Idx<8; u8Idx++)
    for(u8AddrIdx =0; u8AddrIdx<16; u8AddrIdx++)
    {
        u8SRAMDataIn0 = u8SRAMDataA[u8Idx][u8AddrIdx*4+0];
        u8SRAMDataIn1 = u8SRAMDataA[u8Idx][u8AddrIdx*4+1];
        u8SRAMDataIn2 = u8SRAMDataA[u8Idx][u8AddrIdx*4+2];
        u8SRAMDataIn3 = u8SRAMDataA[u8Idx][u8AddrIdx*4+3];

        //Write SRAM A
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, u8AddrIdx<<8, 0xff00);//SRAM Addr
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, (0x1<<u8Idx)&0xFF, 0xff);//Choose SRAM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, (u8SRAMDataIn1<<8) | u8SRAMDataIn0);//SRAM Data
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_75_L, (u8SRAMDataIn3<<8)|u8SRAMDataIn2);//SRAM Data
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x80,0x80);//SRAM write enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x00,0x80);//SRAM write disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x00, 0xff);//Choose no SRAM
    }

    for(u8Idx =0; u8Idx<8; u8Idx++)
    for(u8AddrIdx =0; u8AddrIdx<16; u8AddrIdx++)
    {
        u8SRAMDataIn0 = u8SRAMDataB[u8Idx][u8AddrIdx*4+0];
        u8SRAMDataIn1 = u8SRAMDataB[u8Idx][u8AddrIdx*4+1];
        u8SRAMDataIn2 = u8SRAMDataB[u8Idx][u8AddrIdx*4+2];
        u8SRAMDataIn3 = u8SRAMDataB[u8Idx][u8AddrIdx*4+3];

        //Write SRAM B
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, u8AddrIdx<<8, 0xff00);//SRAM Addr
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, ((0x1<<u8Idx)&0xFF)<<8, 0xff00);//Choose SRAM
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, (u8SRAMDataIn1<<8) | u8SRAMDataIn0);//SRAM Data
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_75_L, (u8SRAMDataIn3<<8)|u8SRAMDataIn2);//SRAM Data
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x80,0x80);//SRAM write enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L,0x00,0x80);//SRAM write disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x0000, 0xff00);//Choose no SRAM
    }
}
#endif
#if (HW_2DTO3D_VER == 4)
static void MainObjectDetection_U9(void)//only for bule sky pattern (Golf pattern)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    int x,y;
    MS_U8 MaxPosDiff_SkyColor=0;
    MS_U8 MaxNegDiff_SkyColor=0;
    MS_U32 AvgDiff_SkyColor=0;
    MS_U32 Avg_NonSkyColorIdx=0;

    MS_U8 MaxPosDiff_TpBGColor=0;
    MS_U8 MaxNegDiff_TpBGColor=0;
    MS_U32 AvgDiff_TpBGColor=0;
    MS_U32 Avg_NonTpBGColorIdx=0;

    int Diff;

    MS_U8 MainObjWei_SkyColor;
    MS_U8 MainObjWei_TpBGColor;

    MS_U8 BLK_cnt=0;

    MS_U8 tmpColorIdx[NBLK_H_MAX];



    BLK_cnt = 0;
    //mark the idx of the start of non-sky BLK
    for ( x = 0; x < NBLK_DD_H; x ++ )
    {
        pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x]=0;
        pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x]=0;

        BLK_cnt = 0;
        //for ( y = 0; y < NBLK_DD_V ; y ++ )
        for ( y = 1; y < NBLK_DD_V ; y ++ )
        {
            if(pXCResourcePrivate->stdrvXC_3D._bSkyMarker[y][x])
                BLK_cnt++;
        }
        pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x] = BLK_cnt;

        BLK_cnt = 0;
        //for ( y = 0; y < NBLK_DD_V ; y ++ )
        for ( y = 1; y < NBLK_DD_V ; y ++ )
        {
            if(pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x])
                BLK_cnt++;
        }
        pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x] = BLK_cnt;
    }

    //2014.02.10 for high building pattern, median filter for stable statistic data
    for ( x = 0; x < NBLK_DD_H; x ++ )
        tmpColorIdx[x]=pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x];
    for ( x = 1; x < NBLK_DD_H-1; x ++ )
        pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x] = MED(pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x-1],pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x],pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x+1]);

    for ( x = 0; x < NBLK_DD_H; x ++ )
        tmpColorIdx[x]=pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x];
    for ( x = 1; x < NBLK_DD_H-1; x ++ )
        pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x] = MED(pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x-1],pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x],pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x+1]);


    BLK_cnt=0;
    //for ( x = NBLK_DD_H/4; x < NBLK_DD_H*3/4; x ++ )
    for ( x = NBLK_DD_H/3; x < NBLK_DD_H*2/3; x ++ )
    {
        BLK_cnt++;

        Diff = pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x] -pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x+3];
        AvgDiff_SkyColor = AvgDiff_SkyColor + abs(Diff);

        if( Diff>=0 && abs(Diff)>MaxPosDiff_SkyColor )
            MaxPosDiff_SkyColor = abs(Diff);
        if( Diff<=0 && abs(Diff)>MaxNegDiff_SkyColor )
            MaxNegDiff_SkyColor = abs(Diff);
    }
    AvgDiff_SkyColor = AvgDiff_SkyColor;///BLK_cnt;


    BLK_cnt=0;
    //for ( x = NBLK_DD_H/4; x < NBLK_DD_H*3/4; x ++ )
    for ( x = NBLK_DD_H/3; x < NBLK_DD_H*2/3; x ++ )
    {
        BLK_cnt++;

        Diff = pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x] -pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x+3];
        AvgDiff_TpBGColor = AvgDiff_TpBGColor + abs(Diff);
        if( Diff>=0 && abs(Diff)>MaxPosDiff_TpBGColor )
            MaxPosDiff_TpBGColor = abs(Diff);
        if( Diff<=0 && abs(Diff)>MaxNegDiff_TpBGColor )
            MaxNegDiff_TpBGColor = abs(Diff);
    }
    AvgDiff_TpBGColor = AvgDiff_TpBGColor;///BLK_cnt;

    BLK_cnt=0;
    for ( x = NBLK_DD_H/4; x < NBLK_DD_H*3/4; x ++ )
        BLK_cnt++;
    for ( x = NBLK_DD_H/4; x < NBLK_DD_H*3/4; x ++ )
        Avg_NonSkyColorIdx=Avg_NonSkyColorIdx+pXCResourcePrivate->stdrvXC_3D._u8NonSkyColorIdx[x];
    for ( x = NBLK_DD_H/4; x < NBLK_DD_H*3/4; x ++ )
        Avg_NonTpBGColorIdx=Avg_NonTpBGColorIdx+pXCResourcePrivate->stdrvXC_3D._u8NonTpBGColorIdx[x];

    Avg_NonSkyColorIdx = Avg_NonSkyColorIdx/BLK_cnt;
    Avg_NonTpBGColorIdx=Avg_NonTpBGColorIdx/BLK_cnt;

#if 0
    MainObjWei_SkyColor = MaxPosDiff_SkyColor + MaxNegDiff_SkyColor;
    MainObjWei_TpBGColor= MaxPosDiff_TpBGColor+ MaxNegDiff_TpBGColor;
#else
    MainObjWei_SkyColor = MAX(MaxPosDiff_SkyColor, MaxNegDiff_SkyColor)*2;
    MainObjWei_TpBGColor= MAX(MaxPosDiff_TpBGColor, MaxNegDiff_TpBGColor)*2;
#endif
    //printf("\nSk %d %d %d",MaxPosDiff_SkyColor,MaxNegDiff_SkyColor,Avg_NonSkyColorIdx);
    //printf("\nBG %d %d %d",MaxPosDiff_TpBGColor,MaxNegDiff_TpBGColor,Avg_NonTpBGColorIdx);

    //MainObjWei = MIN( MAX(MainObjWei_SkyColor-MAX(4,Avg_NonSkyColorIdx/4),0), 32);
    //2014.02.06 add by CL, for LG subtitle pattern
    //MainObjWei = MIN( max3(MainObjWei_SkyColor-MAX(4,Avg_NonSkyColorIdx/4),MainObjWei_TpBGColor-MAX(4,Avg_NonTpBGColorIdx/4),0), 32);

    MS_U8 MainObjWeiSky = MIN( MAX(MainObjWei_SkyColor*2-MAX(8,Avg_NonSkyColorIdx),0), 32);
    MS_U8 MainObjWeiBGC = MIN( MAX(MainObjWei_TpBGColor*2-MAX(8,Avg_NonTpBGColorIdx),0), 32);

    MS_BOOL isBlue = (pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp>=132&&pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp<=192)&&(pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp>=56&&pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp<=124);
    MS_BOOL isWCloud =    (pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp>=124&&pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp<=152)&&(pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp>=104&&pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp<=132);

    //MainObjWei = MIN( max3(MainObjWei_SkyColor*2-MAX(8,Avg_NonSkyColorIdx),MainObjWei_TpBGColor*2-MAX(8,Avg_NonTpBGColorIdx),0), 32);
    if(isBlue||isWCloud)
        pXCResourcePrivate->stdrvXC_3D._u8MainObjWei = MainObjWeiSky;
    else
        pXCResourcePrivate->stdrvXC_3D._u8MainObjWei = MAX(MainObjWeiSky,MainObjWeiBGC);


}
#endif
#if (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_TrendDetection(void *pInstance)
{
    //for Global Variable

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_S16 x,y;
    //int OUTIN_Ratio_Ver,OUTIN_Ratio_Hor;
    MS_S32 HorCplxSum,VerCplxSum;
    MS_S16 CplxTimes;

    CplxTimes = 2;

    MS_U8 VerTrend_1D[NBLK_V_MAX]={0};
    MS_U8 HorTrend_1D[NBLK_H_MAX]={0};

    MS_S32 VerTrendDiff_Max=0;
    MS_S32 VerTrendDiff=0;
    MS_S32 VerTrendDiff_Sum1=0;
    MS_S32 VerTrendDiff_Sum2=0;
    MS_S32 VerTrendDiff_Sum =0;

    MS_S32 VerTrendCplx_min=255;
    MS_S32 VerTrendCplx_Max=0;

    MS_S32 HorTrendCplx_min=255;
    MS_S32 HorTrendCplx_Max=0;
    memset(HorTrend_1D,0,(sizeof(MS_U8)*NBLK_H_MAX));

    for ( y = 0; y < NBLK_DD_V ; y ++ )
    {
        HorCplxSum = 0;
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            HorCplxSum = HorCplxSum + pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
        }
        HorCplxSum = HorCplxSum/NBLK_DD_H;
        HorCplxSum = MINMAX(HorCplxSum*CplxTimes,0,255);

        VerTrend_1D[y]=HorCplxSum;
    }

    for ( x = 0; x < NBLK_DD_H ; x ++ )
    {
        VerCplxSum = 0;
        for ( y = 0; y < NBLK_DD_V; y ++ )
        {
            VerCplxSum = VerCplxSum + pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
        }
        VerCplxSum = VerCplxSum/NBLK_DD_V;
        VerCplxSum = MINMAX(VerCplxSum*CplxTimes,0,255);

        HorTrend_1D[x] = VerCplxSum;
    }


    for( y = NBLK_DD_V/8; y < NBLK_DD_V*7/8 ; y ++ )
    {
        VerTrendDiff = VerTrend_1D[y]-VerTrend_1D[y+1];

        VerTrendDiff_Sum1 = VerTrendDiff_Sum1 + VerTrendDiff;
        VerTrendDiff_Max = MAX(VerTrendDiff_Max,abs(VerTrendDiff));

        if(y >= NBLK_DD_V/2)
            VerTrendDiff_Sum2 = VerTrendDiff_Sum2 + VerTrendDiff;
    }

    //VerTrendDiff_Sum = MAX(VerTrendDiff_Sum1,VerTrendDiff_Sum2);
    VerTrendDiff_Sum = MAX(VerTrendDiff_Sum1,0)+MAX(VerTrendDiff_Sum2,0);

    for( y = NBLK_DD_V/8; y < NBLK_DD_V*7/8 ; y ++ )
    {
        VerTrendCplx_min = MIN(VerTrendCplx_min,VerTrend_1D[y]);
        VerTrendCplx_Max = MAX(VerTrendCplx_Max,VerTrend_1D[y]);
    }

    for( x = NBLK_DD_H/8; x < NBLK_DD_H*7/8 ; x ++ )
    {
        HorTrendCplx_min = MIN(HorTrendCplx_min,HorTrend_1D[x]);
        HorTrendCplx_Max = MAX(HorTrendCplx_Max,HorTrend_1D[x]);
    }

    /*
    if((SC_R2BYTE(REG_SC_BK23_20_L)&0x0008)==8)
    {
        printf("[%d %d !%d @%d]\r\n",HorTrendCplx_min,HorTrendCplx_Max,MINMAX((2*HorTrendCplx_min-HorTrendCplx_Max-12),0,32),MINMAX(((HorTrendCplx_min-HorTrendCplx_Max)+50)*MINMAX(HorTrendCplx_min,0,40)/40,0,32));
    }
    */

    MS_S32 VerTrendDiff_Sum_Wei;
    MS_S32 VerTrendDiff_Max_Wei;

    MS_S32 VerTrendCplx_Max_Wei;
    MS_S32 VerTrendCplx_min_Wei;

    MS_S32 HorTrendCplx_Max_Wei;
    MS_S32 HorTrendCplx_min_Wei;

    MS_S32 VerTrendCplx_Range_Wei;

    MS_S32 times,timesH,timesV;
    //times = 0x56;//MFC_ReadByte(REG_1EFB);
    times = 0x52;
    timesH = times&0x0F;
    timesV = (times&0xF0)>>4;

    VerTrendDiff_Sum_Wei = MINMAX(VerTrendDiff_Sum-4,0,16);//>>4
    VerTrendDiff_Max_Wei = MINMAX(32-VerTrendDiff_Max, 0, 16);//8~16 >>4

    //VerTrendCplx_Max_Wei = MINMAX((VerTrendCplx_min*4+32) - VerTrendCplx_Max, 0, 32);// >>5
    //VerTrendCplx_min_Wei = MINMAX(VerTrendCplx_min,0,16);//>>4
    VerTrendCplx_Max_Wei = MINMAX((VerTrendCplx_min*timesV+32) - VerTrendCplx_Max, 0, 32);// >>5//timesV=5
    VerTrendCplx_min_Wei = MINMAX(VerTrendCplx_min+4,0,16);//>>4

    //HorTrendCplx_Max_Wei = MINMAX((HorTrendCplx_min*timesH/2+32) - HorTrendCplx_Max, 0, 32);//>>5//timesH=5
    //2014.02.05 for seafood outdoor false detection
    //HorTrendCplx_Max_Wei = MINMAX((HorTrendCplx_min*timesH-12) - HorTrendCplx_Max, 0, 32);//>>5//timesH=5
    //2014.05.30 for lm14 pic mode
    HorTrendCplx_Max_Wei = MINMAX(((HorTrendCplx_min-HorTrendCplx_Max)+50)*MINMAX(HorTrendCplx_min,0,40)/40,0,32);
    HorTrendCplx_min_Wei = MINMAX(HorTrendCplx_min,0,16);//>>4

    VerTrendCplx_Range_Wei = MINMAX((VerTrendCplx_min_Wei+VerTrendDiff_Sum+32)-VerTrendCplx_Max,0,32);

    //VerGLB_Wei = (VerTrendDiff2_Max_Wei*HorTrendCplx_min_Wei*VerTrendDiff_Sum_Wei*VerTrendDiff_Max_Wei*VerTrendCplx_min_Wei)>>14;//8,16,16,16,16
    pXCResourcePrivate->stdrvXC_3D.u8VerGLB_Wei = (VerTrendDiff_Sum_Wei*VerTrendCplx_Max_Wei*VerTrendCplx_min_Wei*HorTrendCplx_Max_Wei*HorTrendCplx_min_Wei)>>17;//16,32,16,32,16
    /*if((SC_R2BYTE(REG_SC_BK23_20_L)&0x0001)==1)
        printf("[%d %d %d %d %d]\r\n",VerTrendDiff_Sum_Wei,VerTrendCplx_Max_Wei,VerTrendCplx_min_Wei,HorTrendCplx_Max_Wei,HorTrendCplx_min_Wei);
    */
    //VerGLB_Wei = (VerTrendCplx_Range_Wei*VerTrendDiff_Sum_Wei*VerTrendCplx_Max_Wei*VerTrendCplx_min_Wei*HorTrendCplx_Max_Wei*HorTrendCplx_min_Wei)>>22;//32,16,32,16,32,16
#if 0
    {
        printf("\nVS %d, VM %d, Vm %d, HM %d, Hm %d",VerTrendDiff_Sum_Wei,VerTrendCplx_Max_Wei,VerTrendCplx_min_Wei,HorTrendCplx_Max_Wei,HorTrendCplx_min_Wei);
        printf("\nGLB=%d",VerGLB_Wei);
    }
#endif


}
#endif


#if (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_DepthDetectionTmp(void *pInstance)
{
    //for Global Variable

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if 1
    MS_U16 x,y,idx_x,idx_y;
    MS_U8 BLKWeight,BLKDepth;
    MS_U16 RVal,GVal,BVal,YVal;
    MS_U16 OUTIN_Ratio_Ver,OUTIN_Ratio_Hor;
    MS_U16 EDGEVal,YMinMaxDiff;


    MS_U8 u8testval_1EFE = 128;//MFC_ReadByte(REG_1EFE);


    if(NBLK_DR_V>NBLK_DD_V)
    {
        OUTIN_Ratio_Ver = NBLK_DR_V/NBLK_DD_V;
    }
    else
    {
        OUTIN_Ratio_Ver = NBLK_DD_V/NBLK_DR_V;
    }

    if(NBLK_DR_H>NBLK_DD_H)
    {
        OUTIN_Ratio_Hor = NBLK_DR_H/NBLK_DD_H;
    }
    else
    {
        OUTIN_Ratio_Hor = NBLK_DD_H/NBLK_DR_H;
    }
    for ( y = 0; y < NBLK_DR_V ; y ++ )
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {
            if(NBLK_DR_H>NBLK_DD_H)
            {
                idx_x = x/OUTIN_Ratio_Hor;
            }
            else
            {
                idx_x = x*OUTIN_Ratio_Hor;
            }

            if(NBLK_DR_V>NBLK_DD_V)
            {
                idx_y = y/OUTIN_Ratio_Ver;
            }
            else
            {
                idx_y = y*OUTIN_Ratio_Ver;
            }

            RVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[idx_y][idx_x].R;
            GVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[idx_y][idx_x].G;
            BVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[idx_y][idx_x].B;
            EDGEVal = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[idx_y][idx_x];
            YVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[idx_y][idx_x].G;
            YMinMaxDiff = pXCResourcePrivate->stdrvXC_3D._field[(idx_y)*NBLK_DD_H+(idx_x)];

            if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x01)
            {
                BLKWeight = MINMAX((RVal+GVal+BVal)/3,0,255);
                BLKDepth = MINMAX((RVal+GVal+BVal)/3,0,255);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x02)
            {
                BLKWeight = MINMAX(RVal,0,255);
                BLKDepth = MINMAX(RVal,0,255);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x03)
            {
                BLKWeight = MINMAX(GVal,0,255);
                BLKDepth = MINMAX(GVal,0,255);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x04)
            {
                BLKWeight = MINMAX(BVal,0,255);
                BLKDepth = MINMAX(BVal,0,255);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x05)
            {
                BLKWeight = MINMAX(EDGEVal,0,255);
                BLKDepth = MINMAX(EDGEVal,0,255);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x06)
            {
                BLKWeight = ((x+y)*5);
                BLKDepth = ((x+y)*5);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x07)
            {
                BLKWeight = ((x+y)<<2);
                BLKDepth = ((x+y)<<2);
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x08)
            {
                BLKWeight = (pXCResourcePrivate->stdrvXC_3D._bSkyMarker[idx_y][idx_x])?200:0;
                BLKDepth  = (pXCResourcePrivate->stdrvXC_3D._bSkyMarker[idx_y][idx_x])?200:0;
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x09)
            {
                BLKWeight = (pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[idx_y][idx_x])?200:0;
                BLKDepth  = (pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[idx_y][idx_x])?200:0;
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0A)
            {
                BLKWeight =(EDGEVal>=u8testval_1EFE)?200:0;
                BLKDepth = (EDGEVal>=u8testval_1EFE)?200:0;
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0B)
            {
                BLKWeight =(YVal>=u8testval_1EFE)?200:0;
                BLKDepth = (YVal>=u8testval_1EFE)?200:0;
            }
            else if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0C)
            {
                BLKWeight =(YMinMaxDiff>=u8testval_1EFE)?200:0;
                BLKDepth = (YMinMaxDiff>=u8testval_1EFE)?200:0;
            }
            else// if(DBG_En==0x05)
            {
                BLKWeight = 0;
                BLKDepth = 0;
            }
            //BLKWeight = MINMAX(RVal,0,255);//MINMAX(x+y,0,255);
            //BLKDepth = MINMAX(GVal,0,255);//MINMAX(x+y,0,255);

            pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x] = BLKWeight;
            pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = BLKDepth;
        }
#if ENABLE_T3D_DEBUG
    if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0F)
    {
        MS_U32 dummy_x = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_5A_L)&0x00FF);
        MS_U32 dummy_y = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_5A_L)&0xFF00)>>8;
        RVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor].R;
        GVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor].G;
        BVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor].B;
        EDGEVal = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor];
        YVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor].G;
        MS_U16 CbVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor].B;
        MS_U16 CrVal = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[dummy_y/OUTIN_Ratio_Ver][dummy_x/OUTIN_Ratio_Hor].R;
        printf("\n");
        printf("DDH:%d, DDV:%d\n",NBLK_DD_H,NBLK_DD_V);
        printf("DRH:%d, DRV:%d\n",NBLK_DR_H,NBLK_DR_V);
        printf("BSR1=%d, BSR2=%d\n",pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1,pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2);
        printf("BGCb:%d, BGCr:%d\n",pXCResourcePrivate->stdrvXC_3D.BGColor_CbTp,pXCResourcePrivate->stdrvXC_3D.BGColor_CrTp);
        printf("[RGB]=[%d,%d,%d]\n",RVal,GVal,BVal);
        printf("[edge]=[%d]\n",EDGEVal);
        printf("[YCbCr]=[%d,%d,%d]\n",YVal,CbVal,CrVal);

        printf("\n");
    }
#endif
#endif


#if 0
    //S16 CurDepthA,CurDepthB;
    U8 rIdx,gIdx,bIdx;

    for ( rIdx = 0; rIdx < 8; rIdx++ )
        for ( gIdx = 0; gIdx < 8; gIdx++ )
            for ( bIdx = 0; bIdx < 8; bIdx++ )
            {
                _RGB3DLUT_A[rIdx][gIdx][bIdx] = rIdx*32;
                _RGB3DLUT_B[rIdx][gIdx][bIdx] = rIdx*32;
            }
#endif

}
#endif

#if (HW_2DTO3D_VER == 3)
static void _MDrv_SC_3D_2DTo3D_DepthDetectionPIX(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_S16 s16BlueAndFlatBlkCnt = 0;
    MS_S16 s16BlueAndFlatBlkCnt_Dn = 0;
    MS_S16 x = 0, y = 0;
    PIXEL stPP;
    MS_S16 s16MaxEdgeRGB = 0, s16BlkSat = 0;
    MS_BOOL bBlue =  FALSE, bWCloud = FALSE;
    MS_BOOL bBGCOLOR_Tp = FALSE, bBGCOLOR_Dn = FALSE;
    MS_S16 s16BlueSkyRatio1 = 0, s16BlueSkyRatio2 = 0;
    MS_S16 s16NonSkyRate = 0;
    MS_S8 i = 0, j = 0;
    MS_U32 u32SumEdge = 0;
    //IIR related
    MS_S16 s16DistantViewRatio1 = 0, s16DistantViewRatio2 = 0;
    MS_S16 s16Index_BL = 0;
    MS_U8 u8RegionBlkWeightFrame_Tmp[NBLK_V_MAX][NBLK_H_MAX];
    MS_S16 s16OutIn_Ratio_Ver = 0, s16OutIn_Ratio_Hor = 0;
    MS_S16 s16Cur_YSep = 0;
    MS_U32 u32RegionBlkWei = 0;
    MS_U32 u32PreRegionBlkWei = 0;
    MS_U32 u32ScalingRatio = DISPLAY_DD_H*DISPLAY_DD_V*1000/FHD_W/FHD_H;
    MS_U32 u32VRatio = gSrcInfo[MAIN_WINDOW].stDispWin.height*10/DISPLAY_DD_V;
    MS_U32 u32HRatio = gSrcInfo[MAIN_WINDOW].stDispWin.width*10/DISPLAY_DD_H;

    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDispWin);
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        u32VRatio = stDispWin.height*10/DISPLAY_DD_V;
        u32HRatio = stDispWin.width*10/DISPLAY_DD_H;
    }

    memset(&stPP, 0, sizeof(PIXEL));
    if(u32ScalingRatio == 0)
    {
        u32ScalingRatio = 1;
    }

    _MDrv_SC_3D_2DTo3D_BGColorDetection(pInstance);

    s16BlueAndFlatBlkCnt = 0;
    s16BlueAndFlatBlkCnt_Dn = 0;

    pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt = 0;
    pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt = 0;

#if BACKLIGHTIIR
    //inital IIR buffer
    if(pXCResourcePrivate->stdrvXC_3D.s_bFirst)
    {
        MS_U8 u8IdxR = 0, u8IdxG = 0, u8IdxB = 0;
        MS_U8 x = 0, y = 0;
        for(u8IdxR=0;u8IdxR<8;u8IdxR++)
        for(u8IdxG=0;u8IdxG<8;u8IdxG++)
        for(u8IdxB=0;u8IdxB<8;u8IdxB++)
        {
            pXCResourcePrivate->stdrvXC_3D.u16Pre_RGB3DLUT_A[u8IdxR][u8IdxG][u8IdxB] = 128<<4;//.4
            pXCResourcePrivate->stdrvXC_3D.u16Pre_RGB3DLUT_B[u8IdxR][u8IdxG][u8IdxB] = 128<<4;//.4
        }

        for(y=0;y<NBLK_DR_V;y++)
        for(x=0;x<NBLK_DR_H;x++)
        {
            pXCResourcePrivate->stdrvXC_3D.u16Pre_depthBlkBased[y][x] = 128<<4;//.4
            pXCResourcePrivate->stdrvXC_3D.u16Pre_RegionBlkWeightFrame[y][x] = 128<<4;//.4
        }
        pXCResourcePrivate->stdrvXC_3D.s_bFirst = FALSE;
    }

    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        s16Cur_YSep = (_MDrv_SC_3D_2DTo3D_HorizontalDetection(pInstance))<<4;
        pXCResourcePrivate->stdrvXC_3D._u16Y_Sep = ( pXCResourcePrivate->stdrvXC_3D.u16Pre_YSep * pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight + s16Cur_YSep * (128 -pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) +64)>>7; // do IIR
        pXCResourcePrivate->stdrvXC_3D.u16Pre_YSep = pXCResourcePrivate->stdrvXC_3D._u16Y_Sep;
        pXCResourcePrivate->stdrvXC_3D._u16Y_Sep = (pXCResourcePrivate->stdrvXC_3D._u16Y_Sep+8)>>4;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._u16Y_Sep = _MDrv_SC_3D_2DTo3D_HorizontalDetection(pInstance);
    }
#else
    pXCResourcePrivate->stdrvXC_3D._u16Y_Sep = _MDrv_SC_3D_2DTo3D_HorizontalDetection(pInstance);
#endif

    pXCResourcePrivate->stdrvXC_3D._u16Y_Sep_OUT = pXCResourcePrivate->stdrvXC_3D._u16Y_Sep*u32VRatio/10;

#if PRINTINFO
    printf("=====_u16Y_Sep=====\n");
    printf("_u16Y_Sep_OUT = %u\n", pXCResourcePrivate->stdrvXC_3D._u16Y_Sep_OUT);
#endif
    for ( y = 0; y < NBLK_DD_V ; y ++ )
    {
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            s16MaxEdgeRGB = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];

            stPP = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            //bBlue = s16MaxEdgeRGB < 10 && stPP.B > 100 && stPP.B > stPP.R*1.25 && stPP.B > stPP.G && stPP.G > stPP.R;  //latest by CL
            // bBlue = s16MaxEdgeRGB < 16 && stPP.B > 100 && stPP.B > stPP.R*1.25 && stPP.B > stPP.G && stPP.G > stPP.R;
            //bBlue = s16MaxEdgeRGB < 10 && stPP.B > 100  && stPP.B > stPP.G && stPP.G > stPP.R;  //by tomato
            bBlue = (s16MaxEdgeRGB < 10) && (stPP.B > 100) && (stPP.B > (stPP.R*5/4)) && (stPP.B > stPP.G) && (stPP.G > stPP.R);  //latest by CL

            s16BlkSat = (MAX3(stPP.R,stPP.G,stPP.B)==0)?0:((MAX3(stPP.R,stPP.G,stPP.B)-MIN3(stPP.R,stPP.G,stPP.B))*256)/MAX3(stPP.R,stPP.G,stPP.B);
            //bWCloud =  (s16MaxEdgeRGB < 10) && MIN3(stPP.R,stPP.G,stPP.B)>210 && s16BlkSat<24 && (y <= NBLK_V/4);
            //bWCloud =  (s16MaxEdgeRGB < 10) && MIN3(stPP.R,stPP.G,stPP.B)>200 && s16BlkSat<24 && (y <= NBLK_V/4);
            //bWCloud =  (s16MaxEdgeRGB < 10) && MIN3(stPP.R,stPP.G,stPP.B)>190 && s16BlkSat<52 && (y <= NBLK_V/4);
            bWCloud =  (s16MaxEdgeRGB < 10) && MIN(stPP.R,stPP.G)>170&& stPP.B>190 && s16BlkSat<52 && (y <= NBLK_DD_V/4);

            bBlue = (bBlue||bWCloud);

            if ( bBlue ) // upper & white
            {
                //if(y<=NBLK_V/4)// 2012.11.30 modify by CL
                    if(y<=NBLK_DD_V/4&& x>=NBLK_DD_H/4 && x<=NBLK_DD_H*3/4 )// 2012.11.30 modify by CL
                        s16BlueAndFlatBlkCnt=s16BlueAndFlatBlkCnt+2;
                    else
                        s16BlueAndFlatBlkCnt ++;
            }

            if(bBlue&&y>=NBLK_DD_V*3/4)
                s16BlueAndFlatBlkCnt_Dn++;
            //for both BlueSky & BG color
            bBGCOLOR_Tp = pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x];
            bBGCOLOR_Dn = pXCResourcePrivate->stdrvXC_3D._bDnBGColorMarker[y][x];

            if(bBGCOLOR_Tp && bBlue)
                pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt++;

            if(bBGCOLOR_Dn && bBlue)
                pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt++;
        }
    }

    pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueAndFlatBlkCnt = s16BlueAndFlatBlkCnt;
    pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueAndFlatBlkCnt_Dn = s16BlueAndFlatBlkCnt_Dn;

    //_s16BothTpBGSKY_COLORCnt = MINMAX(_s16BothTpBGSKY_COLORCnt/(FRAME_WIDTH*FRAME_HEIGHT/FHD_W/FHD_H),0,32);
    //_s16BothDnBGSKY_COLORCnt = MINMAX(_s16BothDnBGSKY_COLORCnt/(FRAME_WIDTH*FRAME_HEIGHT/FHD_W/FHD_H),0,32);
    pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt = MINMAX(pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt*1000/u32ScalingRatio/2,0,32);
    pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt = MINMAX(pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt*1000/u32ScalingRatio/2,0,32);

    s16BlueSkyRatio1 = MINMAX( s16BlueAndFlatBlkCnt*1000/u32ScalingRatio/2, 0, 32 );
    s16BlueSkyRatio2 = MINMAX( s16BlueAndFlatBlkCnt_Dn*1000/u32ScalingRatio/2, 0, 32 );

#if PRINTINFO
    printf("Cnt =%u, Rat1=%u, PR1=%u\n", s16BlueAndFlatBlkCnt,s16BlueSkyRatio1,pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1);
    printf("CntD=%u, Rat2=%u, PR2=%u\n", s16BlueAndFlatBlkCnt_Dn,s16BlueSkyRatio2,pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2);
#endif

    s16BlueSkyRatio1 = MAX(MAX(s16BlueSkyRatio1,pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei),0);
    s16BlueSkyRatio2 = MAX(s16BlueSkyRatio2,0);

    s16BlueSkyRatio1 = MINMAX(s16BlueSkyRatio1,0,32);
    s16BlueSkyRatio2 = MINMAX(s16BlueSkyRatio2,0,32);

    // keeps sky ratio stable
    {
        if(abs(s16BlueSkyRatio1-pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1)<=3 &&abs(s16BlueSkyRatio2-pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2)<=3)
        {
            s16BlueSkyRatio1 = pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1;
            s16BlueSkyRatio2 = pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2;
        }

        if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
        {
            s16BlueSkyRatio1 = (pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+s16BlueSkyRatio1*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;
            s16BlueSkyRatio2 = (pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+s16BlueSkyRatio2*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;
        }

        pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1 = s16BlueSkyRatio1;
        pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2 = s16BlueSkyRatio2;
    }

    pXCResourcePrivate->stdrvXC_3D._s16BlueSkyRatio = s16BlueSkyRatio1;

#if PRINTINFO
    printf("BSCnt =%u\n", pXCResourcePrivate->stdrvXC_3D._s16BothTpBGSKY_COLORCnt);
    printf("BSCntD=%u\n", pXCResourcePrivate->stdrvXC_3D._s16BothDnBGSKY_COLORCnt);
    printf("DistantViewRatio1_Cur = %u, DistantViewRatio2_Cur = %u\n",s16BlueSkyRatio1,s16BlueSkyRatio2);
#endif

#if BACKLIGHTIIR
    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
#if PRINTINFO
        printf("In IIR Processing\n");
        printf("DistantViewRatio1_Cur = %u, DistantViewRatio2_Cur = %u\n",s16BlueSkyRatio1,s16BlueSkyRatio2);
        printf("DistantViewRatio1_Pre = %u, DistantViewRatio2_Pre = %u\n",pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio1>>4,pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio2>>4);
#endif
        s16DistantViewRatio1 = s16BlueSkyRatio1<<4;
        s16DistantViewRatio2 = s16BlueSkyRatio2<<4;

        s16DistantViewRatio1 = (pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio1*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+s16DistantViewRatio1*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;
        s16DistantViewRatio2 = (pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio2*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+s16DistantViewRatio2*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;

        pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio1 = s16DistantViewRatio1;
        pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio2 = s16DistantViewRatio2;

        s16DistantViewRatio1 = (s16DistantViewRatio1+8)>>4;
        s16DistantViewRatio2 = (s16DistantViewRatio2+8)>>4;
#if PRINTINFO
        printf("DistantViewRatio1_new = %u, DistantViewRatio2_new = %u\n",s16DistantViewRatio1,s16DistantViewRatio2);
#endif

    }
    else
    {
        s16DistantViewRatio1 = s16BlueSkyRatio1;
        s16DistantViewRatio2 = s16BlueSkyRatio2;
    }
#else
    s16DistantViewRatio1 = s16BlueSkyRatio1;
    s16DistantViewRatio2 = s16BlueSkyRatio2;
#endif

    pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3 = MINMAX(s16DistantViewRatio1-s16DistantViewRatio2,0,32);//DView weight
    s16OutIn_Ratio_Ver = MAX(u32VRatio, 10);
    s16OutIn_Ratio_Hor = MAX(u32HRatio, 10);

    for ( y = 0; y < NBLK_DD_V; y ++ )
    {
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            // ---------default method ----------------//
            //int s16NonSkyRate = _u16BlockStatus_Edge[y/_u8Vertical_Step][x/_u8Horizontal_Step]; // edge
            //s16NonSkyRate = MAX( s16NonSkyRate, 256 - s16BlueSkyRatio1*8 );
            //s16NonSkyRate = MAX( s16NonSkyRate, y*256/FRAME_HEIGHT ); // vertical position
            //s16NonSkyRate = MINMAX(s16NonSkyRate- s16BlueSkyRatio2*4,0,MAXVALUE);// for full blue sky background
            //--------------end of default method ----------------//

            s16NonSkyRate = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x]; // edge
            s16NonSkyRate = MAX( s16NonSkyRate, 256 - s16BlueSkyRatio1*8 );

            if(NBLK_DD_V == 0)
            {
                s16NonSkyRate = MAX( s16NonSkyRate, y*256); // vertical position
            }
            else
            {
                s16NonSkyRate = MAX( s16NonSkyRate, (y*256)/NBLK_DD_V ); // vertical position
            }

            //s16NonSkyRate = MAX( s16NonSkyRate, new_y*256/(FRAME_HEIGHT-_u16Y_Sep)); // vertical position
            s16NonSkyRate = MINMAX(s16NonSkyRate- s16BlueSkyRatio2*8,0,MAXVALUE);// for full blue sky background
            s16NonSkyRate = MINMAX( s16NonSkyRate, 0, MAXVALUE );

            u8RegionBlkWeightFrame_Tmp[y][x] = s16NonSkyRate;
        }
    }

#if PRINTINFO
    printf("_s16DistantViewWei = %u, _s16DistantViewWei_Bottom = %u\n",pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei,pXCResourcePrivate->stdrvXC_3D._s16DistantViewWei_Bottom);
#endif

    // LPF of _u8RegionBlkWeightFrame
    for ( y = 0; y < NBLK_DR_V; y ++ )
    {
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {
            // 5x5 LPF
            u32SumEdge = MAXVALUE;

            if(x>=1&&x<NBLK_DR_H-1&&y>=1&&y<NBLK_DR_V-1)//boundary case
            {
                for ( i = -1; i <= 1; i ++ )
                for ( j = -1; j <= 1; j ++ )
                {
                    u32SumEdge = MIN( u32SumEdge, u8RegionBlkWeightFrame_Tmp[(y+j)*10/s16OutIn_Ratio_Ver][(x+i)*10/s16OutIn_Ratio_Hor]);
                    //u32SumEdge += u8RegionBlkWeightFrame_Tmp[y+j][x+i];
                }
            }
            else//boundary case
            {
                for ( i = -1; i <= 1; i ++ )
                for ( j = -1; j <= 1; j ++ )
                {
                    u32SumEdge = MIN( u32SumEdge, u8RegionBlkWeightFrame_Tmp[MINMAX(y+j,0,NBLK_DR_V-1)*10/s16OutIn_Ratio_Ver][MINMAX(x+i,0,NBLK_DR_H-1)*10/s16OutIn_Ratio_Hor]);
                    //u32SumEdge += u8RegionBlkWeightFrame_Tmp[y+j][x+i];
                }
            }
            u32SumEdge = MINMAX( u32SumEdge, 0, MAXVALUE );
            //u32SumEdge = MINMAX( u32SumEdge/9, 0, MAXVALUE );

            pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x] = u32SumEdge;


        }
    }

#if BACKLIGHTIIR
    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        for ( y = 0; y < NBLK_DR_V; y ++ )
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {
            u32RegionBlkWei = (pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x])<<4;
            u32PreRegionBlkWei = pXCResourcePrivate->stdrvXC_3D.u16Pre_RegionBlkWeightFrame[y][x];
            u32RegionBlkWei = ( u32PreRegionBlkWei * pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight + u32RegionBlkWei * (128 -pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) +64)>>7; // doBL_IIR
            pXCResourcePrivate->stdrvXC_3D.u16Pre_RegionBlkWeightFrame[y][x] = u32RegionBlkWei;
            pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x] = (u32RegionBlkWei+8) >>4;
        }
    }
#endif

    // Prepare RGB 3D LUT 8x8x8, SW based!!!
    _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT(pInstance, s16Index_BL );
#if BACKLIGHTIIR
    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT_IIR(pInstance);
    }
#endif
}
#elif (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_DepthDetectionPIX_U9(void *pInstance)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_S16 x,y;
    MS_S16 blueSkyRatio1, blueSkyRatio2;
    MS_S16 nonSkyRate;

    MS_S8 i,j;
    MS_U32 sumEdge;
    MS_S8 mapping_yidx;
    MS_S8 mapping_xidx;

    //IIR related
    MS_S16 DistantViewRatio1, DistantViewRatio2;

    MS_U8 **_RegionBlkWeightFrame_tmp;
    MS_S16 OUTIN_Ratio_Ver,OUTIN_Ratio_Hor;
    MS_U32 RegionBlkWei = 0;
    MS_U32 PreRegionBlkWei = 0;

    MS_S16 BGColorBlkCnt =0;
    MS_S16 BGColorBlkCnt_Dn =0;

    MS_U8 BGColorRatio1,BGColorRatio2;

    //Blue SKy Detection
    PIXEL pp,ppYCC;
    MS_U8 maxedgeRGB;
    MS_U8 BlkSat, notSkyFlat;
    MS_S16 blueAndFlatBlkCnt = 0;
    MS_S16 blueAndFlatBlkCnt_Dn = 0;

    MS_BOOL isBlue,isWCloud,isSky,isSky_ful;
    MS_U8 id_v=0,id_h=0;
    MS_U8 u8tmpval = 0x18;//16;//

    MS_U32 u32ScalingRatio = DISPLAY_DD_H*DISPLAY_DD_V*1000/FHD_W/FHD_H;
    if(u32ScalingRatio == 0)
    {
        u32ScalingRatio = 1;
    }

#if DUMYGAINCTRL
    MS_U8 u8tempval1EFC = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_59_L)&0x00F0)>>4;
#else
    MS_U8 u8tempval1EFC = 0;//
#endif
    _RegionBlkWeightFrame_tmp = (MS_U8 **)malloc(sizeof(MS_U8 *) * NBLK_V_MAX);
    for( y = 0;y < NBLK_V_MAX ;y++)
        _RegionBlkWeightFrame_tmp[y]=(MS_U8 *)malloc(sizeof(MS_U8) * NBLK_H_MAX);
    for ( y = 0; y < NBLK_DD_V ; y ++ )
        for ( x = 0; x < NBLK_DD_H; x ++ )
            //for ( x = NBLK_DD_H>>3; x < (NBLK_DD_H*7)>>3; x ++ )//2014.02.11 for LG seafood dish
        {
            maxedgeRGB = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x];
            pp = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
            ppYCC = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[y][x];

            //isBlue = (maxedgeRGB < u8tmpval) && (pp.B > 100) && (pp.B > (pp.R*9/8)) && (pp.B > pp.G) && (pp.G > pp.R);  //latest by CL
            isBlue = (maxedgeRGB < u8tmpval)&&(ppYCC.B>=132&&ppYCC.B<=192)&&(ppYCC.R>=56&&ppYCC.R<=124)&& (ppYCC.G>=56) && pp.B>=0;//ppYCC.B>=140;//ppYCC.G>=52;//&& ppYCC.G>=96

            BlkSat = (MAX3(pp.R,pp.G,pp.B)<=32)?0:((MAX3(pp.R,pp.G,pp.B)-MIN3(pp.R,pp.G,pp.B))*256)/MAX3(pp.R,pp.G,pp.B);

            //isWCloud =  (maxedgeRGB < u8tmpval) && MIN(pp.R,pp.G)>170&& pp.B>190 && BlkSat<52 && (y <= NBLK_DD_V/4 && x>=NBLK_DD_H/4 && x<=NBLK_DD_H*3/4);
            //isWCloud =  (maxedgeRGB < u8tmpval) && (ppYCC.B>=124&&ppYCC.B<=140)&&(ppYCC.R>=116&&ppYCC.R<=132)&& ppYCC.G>=160;
            isWCloud =  (maxedgeRGB < u8tmpval) && (ppYCC.B>=124&&ppYCC.B<=152)&&(ppYCC.R>=104&&ppYCC.R<=132)&& (ppYCC.G>=160);
            //isWCloud = false;

            isSky = (isBlue||isWCloud);
            notSkyFlat = (maxedgeRGB < u8tmpval)&&(!isSky)&&(BlkSat>200);

            if(x >= (NBLK_DD_H>>3) && x <= ((NBLK_DD_H*7)>>3))
            {
                if ( isSky ) // upper & white
                {
                    if(y<=NBLK_DD_V>>2&& x>=NBLK_DD_H>>2 && x<=(NBLK_DD_H*3)>>2 )// 2012.11.30 modify by CL
                        blueAndFlatBlkCnt=blueAndFlatBlkCnt+2;
                    else if(y<=NBLK_DD_V>>2 && ( x<NBLK_DD_H>>2 || x>(NBLK_DD_H*3)>>2))
                        blueAndFlatBlkCnt ++;
                }
                else if(notSkyFlat)
                    blueAndFlatBlkCnt--;

                if(isSky&&y>=(NBLK_DD_V*3)>>2)
                    blueAndFlatBlkCnt_Dn++;
                else if(notSkyFlat && y>=(NBLK_DD_V*3)>>2)
                    blueAndFlatBlkCnt_Dn--;
            }

            //isWCloud is the same formula with upper one, but no spatial limit
            //isWCloud  = (maxedgeRGB < u8tmpval) && MIN(pp.R,pp.G)>170&& pp.B>190 && BlkSat<52;
            //isWCloud =  (maxedgeRGB < u8tmpval) && (ppYCC.B>=116&&ppYCC.B<=140)&&(ppYCC.R>=116&&ppYCC.R<=140)&& ppYCC.G>=160;
            isWCloud =  (maxedgeRGB < u8tmpval) && (ppYCC.B>=124&&ppYCC.B<=152)&&(ppYCC.R>=104&&ppYCC.R<=132)&& ppYCC.G>=160;
            isSky_ful = (isBlue||isWCloud);//detect Sky without spatial limit

            if(isSky_ful)
                pXCResourcePrivate->stdrvXC_3D._bSkyMarker[y][x] = true;
            else
                pXCResourcePrivate->stdrvXC_3D._bSkyMarker[y][x] = false;

        }
    if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0e)
    {
        printf("\nskycnt1:%d, Dn:%d, scalingR:%td",blueAndFlatBlkCnt,blueAndFlatBlkCnt_Dn,(ptrdiff_t)u32ScalingRatio);
        printf("\nDisDD_H:%d, DisDD_V:%d",DISPLAY_DD_H,DISPLAY_DD_V);
    }

    //printf("\nBFC:%d, BFCDn:%d",blueAndFlatBlkCnt,blueAndFlatBlkCnt_Dn);

    //blueSkyRatio1 = MINMAX( blueAndFlatBlkCnt/sizeratio/2, 0, 32 );
    //blueSkyRatio2 = MINMAX( blueAndFlatBlkCnt_Dn/sizeratio/2, 0, 32 );
    blueSkyRatio1 = MINMAX( (MS_S16)((blueAndFlatBlkCnt-32)*1000/u32ScalingRatio)/2, 0, 32 );
    blueSkyRatio2 = MINMAX( (MS_S16)((blueAndFlatBlkCnt_Dn-32)*1000/u32ScalingRatio)/2, 0, 32 );

    BGColorBlkCnt = MIN(pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp,128);
    BGColorBlkCnt_Dn = MIN( MIN(pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntTp,pXCResourcePrivate->stdrvXC_3D.BGColorBlkCntBn), 128);

    BGColorRatio1 = MINMAX( (BGColorBlkCnt-16)/4, 0, 32 );
    BGColorRatio2 = MINMAX( (BGColorBlkCnt_Dn-16)/4, 0, 32 );

    //printf("\nBG1:%d, BG2:%d",BGColorRatio1,BGColorRatio2);
    //printf("\nsky1:%d, sky2:%d",blueSkyRatio1,blueSkyRatio2);
    if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0e)
    {
        printf("\nBG1:%d, BG2:%d",BGColorRatio1,BGColorRatio2);
        printf("\nsky1:%d, sky2_d:%d",blueSkyRatio1,blueSkyRatio2);
    }

    MS_S16 s16Skyratiodiff = blueSkyRatio1 - blueSkyRatio2;
    MS_S16 s16BGCratiodiff = BGColorRatio1 - BGColorRatio2;


    blueSkyRatio1 = MIN( blueSkyRatio2+MAX(s16Skyratiodiff,s16BGCratiodiff), 32 );
    blueSkyRatio2 = blueSkyRatio2;

    pXCResourcePrivate->stdrvXC_3D._u8TBABGColorWei = MINMAX(BGColorRatio2-blueSkyRatio2,0,32);//When TBB is affect by BGColor more than Sky color, use TBA BG color in TBB;


    //printf("\nfnsky1:%d, fnsky2:%d",blueSkyRatio1,blueSkyRatio2);
    if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0e)
    {
        printf("\nfnsky1:%d, fnsky2:%d",blueSkyRatio1,blueSkyRatio2);
    }
//----- TRENDDETECTON -----
    blueSkyRatio1 = MAX(blueSkyRatio1,pXCResourcePrivate->stdrvXC_3D.u8VerGLB_Wei);
    //printf("\nftsky1:%d, ftsky2:%d",blueSkyRatio1,blueSkyRatio2);
    if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0e)
    {
        printf("\nftsky1:%d, ftsky2:%d",blueSkyRatio1,blueSkyRatio2);
    }



//----- MAINOBJDETECTON ----
    MainObjectDetection_U9();
    blueSkyRatio1 = MAX(blueSkyRatio1- pXCResourcePrivate->stdrvXC_3D._u8MainObjWei,0);
    blueSkyRatio2 = MAX(blueSkyRatio2- pXCResourcePrivate->stdrvXC_3D._u8MainObjWei,0);
    //printf("\nfmsky1:%d, fmsky2:%d\n",blueSkyRatio1,blueSkyRatio2);
    if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En==0x0e)
    {
        printf("\nfmsky1:%d, fmsky2:%d\n",blueSkyRatio1,blueSkyRatio2);
    }


    //DBG
    MS_U8 force_TBAB = 0;

    if(force_TBAB==0x1)//all TBA
    {
        blueSkyRatio1 = 32;
        blueSkyRatio2 = 32;
    }
    else if(force_TBAB==0x2)//all TBB
    {
        blueSkyRatio1 = 0;
        blueSkyRatio2 = 0;
    }
    else if(force_TBAB==0x3)//all TBA->TBB
    {
        blueSkyRatio1 = 32;
        blueSkyRatio2 = 0;
    }

    // keeps sky ratio stable
    {
        if(abs(blueSkyRatio1-pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1)<=3 &&abs(blueSkyRatio2-pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2)<=3)
        {
            blueSkyRatio1 = pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1;
            blueSkyRatio2 = pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2;
        }
        pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio1 = blueSkyRatio1;
        pXCResourcePrivate->stdrvXC_3D._u8Pre_BlueSkyRatio2 = blueSkyRatio2;
    }




    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        DistantViewRatio1 = blueSkyRatio1<<4;
        DistantViewRatio2 = blueSkyRatio2<<4;

        DistantViewRatio1 = (pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio1*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+DistantViewRatio1*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;
        DistantViewRatio2 = (pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio2*pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight+DistantViewRatio2*(128-pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) + 64 )>>7;

        pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio1 = DistantViewRatio1;
        pXCResourcePrivate->stdrvXC_3D.u16PreDistantViewRatio2 = DistantViewRatio2;

        DistantViewRatio1 = (DistantViewRatio1+8)>>4;
        DistantViewRatio2 = (DistantViewRatio2+8)>>4;
#if PRINTINFO
        printf(" \r\nDistantViewRatio1_new = %d, DistantViewRatio2_new = %d \r\n",DistantViewRatio1,DistantViewRatio2);
#endif

    }
    else
    {
        DistantViewRatio1 = blueSkyRatio1;
        DistantViewRatio2 = blueSkyRatio2;
    }


    pXCResourcePrivate->stdrvXC_3D._s16DistantViewRatio3 = MINMAX(DistantViewRatio1-DistantViewRatio2,0,32);//DView weight

    if(NBLK_DR_V>NBLK_DD_V)
    {
        OUTIN_Ratio_Ver = NBLK_DR_V/NBLK_DD_V;
    }
    else
    {
        OUTIN_Ratio_Ver = NBLK_DD_V/NBLK_DR_V;
    }
    if(NBLK_DR_H>NBLK_DD_H)
    {
        OUTIN_Ratio_Hor = NBLK_DR_H/NBLK_DD_H;
    }
    else
    {
        OUTIN_Ratio_Hor = NBLK_DD_H/NBLK_DR_H;
    }

    for(id_v=0; OUTIN_Ratio_Ver>1; id_v++)
    {
        OUTIN_Ratio_Ver=OUTIN_Ratio_Ver>>1;
    }
    for(id_h=0; OUTIN_Ratio_Hor>1; id_h++)
    {
        OUTIN_Ratio_Hor=OUTIN_Ratio_Hor>>1;
    }

    pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1 = blueSkyRatio1;
    pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2 = blueSkyRatio2;
    MS_U8 BLK_Y;
    MS_BOOL BGSkyCheck;

    for ( y = 0; y < NBLK_DD_V; y ++ )
    {
        for ( x = 0; x < NBLK_DD_H; x ++ )
        {
            // ---------default method ----------------//
            //int nonSkyRate = _blockStatus2[y/VERTICAL_STEP][x/HORIZONTAL_STEP]; // edge
            //nonSkyRate = MAX( nonSkyRate, 256 - blueSkyRatio1*8 );
            //nonSkyRate = MAX( nonSkyRate, y*256/FRAME_HEIGHT ); // vertical position
            //nonSkyRate = MINMAX(nonSkyRate- blueSkyRatio2*4,0,255);// for full blue sky background
            //--------------end of default method ----------------//
            BGSkyCheck = pXCResourcePrivate->stdrvXC_3D._bSkyMarker[y][x]||pXCResourcePrivate->stdrvXC_3D._bTpBGColorMarker[y][x];
            BLK_Y = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_YCCAvg[y][x].G;
            maxedgeRGB = pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x]; // edge
            nonSkyRate = maxedgeRGB;
            nonSkyRate = MAX( nonSkyRate, 256 - blueSkyRatio1*8 );

            nonSkyRate = MAX( nonSkyRate, (y*256)/NBLK_DD_V ); // vertical position

            //    nonSkyRate = MAX( nonSkyRate, new_y*256/(FRAME_HEIGHT-y_sep)); // vertical position
            nonSkyRate = MINMAX(nonSkyRate- blueSkyRatio2*8,0,255);// for full blue sky background

            nonSkyRate = MAX(nonSkyRate,pXCResourcePrivate->stdrvXC_3D._u16BlockStatus_Edge[y][x]);

            nonSkyRate = MINMAX( nonSkyRate, 0, 255 );

            //nonSkyRate = MIN(nonSkyRate,(maxedgeRGB<<4));
            if((BGSkyCheck)&&maxedgeRGB<=16&&(u8tempval1EFC)==0x08)
            {
                if(maxedgeRGB>8)
                    nonSkyRate = (MIN(nonSkyRate,maxedgeRGB)+nonSkyRate)>>1;
                else
                    nonSkyRate = MIN(nonSkyRate,maxedgeRGB);
            }

            _RegionBlkWeightFrame_tmp[y][x] = nonSkyRate;

        }
    }


    // LPF of _RegionBlkWeightFrame
    for ( y = 0; y < NBLK_DR_V; y ++ )
    {
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {

            if(NBLK_DR_V>NBLK_DD_V)
            {
                mapping_yidx = y>>id_v;
            }
            else
            {
                mapping_yidx = y<<id_v;
            }

            if(NBLK_DR_H>NBLK_DD_H)
            {
                mapping_xidx = x>>id_h;
            }
            else
            {
                mapping_xidx = x<<id_h;
            }
            sumEdge = _RegionBlkWeightFrame_tmp[mapping_yidx][mapping_xidx];
            for ( i = -2; i <= 2; i ++ )
                for ( j = -2; j <= 2; j ++ )
                {
                    sumEdge = sumEdge+  _RegionBlkWeightFrame_tmp[MINMAX(mapping_yidx+j,0,NBLK_DR_V-1)][MINMAX(mapping_xidx+i,0,NBLK_DR_H-1)];
                }
            //sumEdge = MINMAX( sumEdge, 0, 255 );
            sumEdge = MINMAX( sumEdge/25, 0, 255 );

            pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x] = sumEdge;
        }
    }

    // add new backlight detection


    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        for ( y = 0; y < NBLK_DR_V; y ++ )
            for ( x = 0; x < NBLK_DR_H; x ++ )
            {

                RegionBlkWei = (pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x])<<4;
                PreRegionBlkWei = pXCResourcePrivate->stdrvXC_3D.u16Pre_RegionBlkWeightFrame[y][x];
                RegionBlkWei = ( PreRegionBlkWei * pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight + RegionBlkWei * (128 -pXCResourcePrivate->stdrvXC_3D.u16BL_IIRweight) +64)>>7; // doBL_IIR
                pXCResourcePrivate->stdrvXC_3D.u16Pre_RegionBlkWeightFrame[y][x] = RegionBlkWei;
                pXCResourcePrivate->stdrvXC_3D._u8RegionBlkWeightFrame[y][x] = (RegionBlkWei+8) >>4;
            }
    }

        for( y = 0;y < NBLK_V_MAX ;y++)
            free(_RegionBlkWeightFrame_tmp[y]);
        free(_RegionBlkWeightFrame_tmp);

    }
#endif

#if (HW_2DTO3D_VER == 3)
static void _MDrv_SC_3D_2DTo3D_DepthDetectionBLK(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_S16 x = 0, y = 0, xx = 0, yy = 0, xj = 0, yj = 0;
    MS_S16 s16XRange = 0;
    MS_S16 s16Yrange = 0;
    MS_S16 s16Num = 1;
    PIXEL stAvgPixel, stCurPixel, stJPixel;
    MS_S16 s16Depth = 0;
    MS_U16 u16OutIn_Ratio_Ver = 0,u16OutIn_Ratio_Hor = 0;
    MS_U8 u8DepthBlkBased_Tmp[NBLK_V_MAX][NBLK_H_MAX];
    MS_U8 u8DepthBlkBased_Tmp2[NBLK_V_MAX][NBLK_H_MAX];
    MS_S32 s32YDiff = 0;
    MS_S32 s32WRatio = 0;
    MS_U16 u16Weight = 0;
    MS_U8  u8YVar = 24;
    MS_S32 s32WSum = 0, s32DSum = 0;
    MS_U16 u16Diff_Table[32] ={1000, 368, 135, 50, 18, 7, 2, 1,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    MS_U32 u32VRatio = gSrcInfo[MAIN_WINDOW].stDispWin.height*10/DISPLAY_DD_V;
    MS_U32 u32HRatio = gSrcInfo[MAIN_WINDOW].stDispWin.width*10/DISPLAY_DD_H;

    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDispWin);

    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        u32VRatio = stDispWin.height*10/DISPLAY_DD_V;
        u32HRatio = stDispWin.width*10/DISPLAY_DD_H;
    }

    memset(&stAvgPixel, 0, sizeof(PIXEL));
    memset(&stCurPixel, 0, sizeof(PIXEL));
    memset(&stJPixel, 0, sizeof(PIXEL));

#if BOUNDARY_MODE
    MS_U8 u8BlkOffset = 0;
#endif

    // Use Pixel based RGB3D LUT
    u16OutIn_Ratio_Ver = MAX(u32VRatio, 10);
    u16OutIn_Ratio_Hor = MAX(u32HRatio, 10);

    for ( y = 0; y < NBLK_DD_V; y ++ )
    for ( x = 0; x < NBLK_DD_H; x ++ )
    {
        stAvgPixel = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];

        s16Depth = _MDrv_SC_3D_2DTo3D_InterpolationDepthFromRGB_BLK(pInstance, x, y, stAvgPixel.R*4, stAvgPixel.G*4, stAvgPixel.B*4 );

        u8DepthBlkBased_Tmp[y][x] = s16Depth;

    }

    //lowpass setting
    if (pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En == 1)
    {
        s16XRange =1;
        s16Yrange =1;
        s16Num = 9;
    }
    else if(pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En == 2)
    {
        s16XRange =2;
        s16Yrange =2;
        s16Num =25;
    }
    else if (pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En == 3)
    {
        s16XRange =3;
        s16Yrange =3;
        s16Num = 49;
    }
    else
    {
        s16XRange =0;
        s16Yrange =0;
        s16Num = 1;
    }

    //lowpass or bilateral filter
    for(y = 0; y < NBLK_DD_V; y ++)
    for(x = 0; x < NBLK_DD_H; x ++)
    {
        if(pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En != 0 && pXCResourcePrivate->stdrvXC_3D._u8Bilateral_En ==0)// lowpass
        {
            s16Depth = 0;
            if(x>=s16XRange && x<NBLK_DD_H-s16XRange && y>=s16Yrange && y<NBLK_DD_V-s16Yrange)//normal case
            {
                for ( yy = -s16Yrange; yy <= s16Yrange; yy ++ )
                for ( xx = -s16XRange; xx <= s16XRange; xx ++ )
                {
                    yj = y+yy;
                    xj = x+xx;
                    s16Depth += u8DepthBlkBased_Tmp[yj][xj];
                }
            }
            else //  boundary case
            {
                for ( yy = -s16Yrange; yy <= s16Yrange; yy ++ )
                for ( xx = -s16XRange; xx <= s16XRange; xx ++ )
                {
                    yj = MINMAX(y+yy, 0,NBLK_DD_V-1);
                    xj = MINMAX(x+xx, 0,NBLK_DD_H-1);
                    s16Depth += u8DepthBlkBased_Tmp[yj][xj];
                }
            }
            u8DepthBlkBased_Tmp2[y][x]= s16Depth/s16Num;
        }
        else if (pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En != 0 && pXCResourcePrivate->stdrvXC_3D._u8Bilateral_En ==1)  //bilateral
        {
            s32WSum = 0;
            s32DSum = 0;
            s32YDiff = 0;
            s32WRatio = 0;
            u16Weight = 0;
            stCurPixel = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];

            if(x>=s16XRange && x<NBLK_DD_H-s16XRange && y>=s16Yrange && y<NBLK_DD_V-s16Yrange) // normal case
            {
                for ( yy = -s16Yrange; yy <= s16Yrange; yy ++ )
                for ( xx = -s16XRange; xx <= s16XRange; xx ++ )
                {
                    xj = x+xx;
                    yj = y+yy;
                    stJPixel = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[yj][xj];
                    s32YDiff = (abs( stCurPixel.R - stJPixel.R) + abs( stCurPixel.G - stJPixel.G) + abs( stCurPixel.B - stJPixel.B));
                    s32WRatio = s32YDiff/u8YVar;

                    u16Weight = u16Diff_Table[s32WRatio];
                    s32WSum = u16Weight + s32WSum;
                    s32DSum = u16Weight* u8DepthBlkBased_Tmp[yj][xj] + s32DSum ;
                }
            }
            else // boundary case
            {
                for ( yy = -s16Yrange; yy <= s16Yrange; yy ++ )
                for ( xx = -s16XRange; xx <= s16XRange; xx ++ )
                {
                    xj = MINMAX(x+xx, 0,NBLK_DD_H-1);
                    yj = MINMAX(y+yy, 0,NBLK_DD_V-1);
                    stJPixel = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[yj][xj];
                    s32YDiff = (abs( stCurPixel.R - stJPixel.R) + abs( stCurPixel.G - stJPixel.G) + abs( stCurPixel.B - stJPixel.B));
                    s32WRatio = s32YDiff/u8YVar;

                    u16Weight = u16Diff_Table[s32WRatio];
                    s32WSum = u16Weight + s32WSum;
                    s32DSum = u16Weight* u8DepthBlkBased_Tmp[yj][xj] + s32DSum ;
                }
            }

            if(s32WSum >0)
                u8DepthBlkBased_Tmp2[y][x] = s32DSum/s32WSum;
            else
                u8DepthBlkBased_Tmp2[y][x] = u8DepthBlkBased_Tmp[y][x];

        }
        else
        {
            u8DepthBlkBased_Tmp2[y][x]= u8DepthBlkBased_Tmp[y][x];
        }
    }

    // bilinear interpolation to scale up
    for ( y = 0; y < NBLK_DR_V; y ++ )
    for ( x = 0; x < NBLK_DR_H; x ++ )
    {
        MS_U8 idx_x,idx_y;
        idx_x =x*10/u16OutIn_Ratio_Hor;
        idx_y =y*10/u16OutIn_Ratio_Ver;
        s16Depth = 0;
        if(idx_x>=1 && idx_x<(NBLK_DD_H-1) && idx_y>=1 && idx_y< (NBLK_DD_V-1)) //normal case
        {
            if (x%2 == 0 && y%2 == 0)
                s16Depth=u8DepthBlkBased_Tmp2[idx_y][idx_x];
            else if (x%2 == 1 && y%2 == 0)
                s16Depth=(u8DepthBlkBased_Tmp2[idx_y][idx_x]+u8DepthBlkBased_Tmp2[idx_y][idx_x+1])/2;
            else if (x%2 == 0 && y%2 == 1)
                s16Depth=(u8DepthBlkBased_Tmp2[idx_y][idx_x]+u8DepthBlkBased_Tmp2[idx_y+1][idx_x])/2;
            else
                s16Depth=(u8DepthBlkBased_Tmp2[idx_y][idx_x]+u8DepthBlkBased_Tmp2[idx_y][idx_x+1]+u8DepthBlkBased_Tmp2[idx_y+1][idx_x]+u8DepthBlkBased_Tmp2[idx_y+1][idx_x+1])/4;
        }
        else //boundary case
        {
            if (x%2 == 0 && y%2 == 0)
                s16Depth=u8DepthBlkBased_Tmp2[idx_y][idx_x];
            else if (x%2 == 1 && y%2 == 0)
                s16Depth=(u8DepthBlkBased_Tmp2[idx_y][idx_x]+u8DepthBlkBased_Tmp2[idx_y][MINMAX(idx_x+1,0,NBLK_DD_H-1)])/2;
            else if (x%2 == 0 && y%2 == 1)
                s16Depth=(u8DepthBlkBased_Tmp2[idx_y][idx_x]+u8DepthBlkBased_Tmp2[MINMAX(idx_y,0,NBLK_DD_V-1)][idx_x])/2;
            else
                s16Depth=(u8DepthBlkBased_Tmp2[idx_y][idx_x]+u8DepthBlkBased_Tmp2[idx_y][MINMAX(idx_x+1,0,NBLK_DD_H-1)]+u8DepthBlkBased_Tmp2[MINMAX(idx_y+1,0,NBLK_DD_V-1)][idx_x]+u8DepthBlkBased_Tmp2[MINMAX(idx_y+1,0,NBLK_DD_V-1)][MINMAX(idx_x+1,0,NBLK_DD_H-1)])/4;
        }

        if( pXCResourcePrivate->stdrvXC_3D._u8Bilinear_En == 0)
            pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = u8DepthBlkBased_Tmp2[y*10/u16OutIn_Ratio_Ver][x*10/u16OutIn_Ratio_Hor];
        else
            pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = s16Depth;
    }

#if BOUNDARY_MODE
    u8BlkOffset = pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset;
    for ( y = 0; y < NBLK_DR_V; y ++ )
    {
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {
           if(x==0||x==NBLK_DR_H-1)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = u8BlkOffset;
            else if(x==1)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = ( u8BlkOffset+ 2*pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] + pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x+1] + 2)>>2;
            else if(x==NBLK_DR_H-2)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = ( pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x-1] + 2*pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] + u8BlkOffset              + 2)>>2;
        }
    }
#endif

#if BACKLIGHTIIR
    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        _MDrv_SC_3D_2DTo3D_PrepareBLKDepth_IIR(pInstance);
    }
#endif
}
#elif (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_DepthDetectionBLK_TEST(void *pInstance)
{
    //for Global Variable

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bT3DDE_Use_MainDisplayWindow = _MDrv_Is_T3DDE_UseMainDisplayWindow(pInstance);
    PIXEL avgPixel;
    MS_S16 x,y,xx,yy, xj, yj;
    MS_S16 s16Xrange =0;
    MS_S16 s16Yrange =0;
    MS_S16 s16Num = 1;
    //PIXEL curPixel, jPixel;
    MS_S16 depth=0;

    MS_U16 u16OutIn_Ratio_Ver,u16OutIn_Ratio_Hor;

    MS_U8 **_depthBlkBased_tmp;
    MS_U8 **_depthBlkBased_tmp2;

    MS_U8 id_v=0,id_h=0;

    MS_U32 u32VRatio = PANEL_HEIGHT*10/DISPLAY_DD_V;
    MS_U32 u32HRatio = PANEL_WIDTH*10/DISPLAY_DD_H;


#if MOTIONDETECTION
    MS_S16 blk_mot;
    MS_S16 mot_depth;

    //MS_S16 GMvSXsize,GMvSYsize,GMvsize;
    MS_U8 GMvWei;

    //GMvSXsize = abs(MFC_ReadByte(REG_29CC)-0x80);
    //GMvSYsize = abs((MFC_ReadByte(REG_29CD)&0x7F)-0x40);
    //GMvsize = (GMvSXsize+GMvSYsize);

    GMvWei = 0x10;//MINMAX(24 - GMvsize,0,16);

#endif

    _depthBlkBased_tmp = (MS_U8 **)malloc(sizeof(MS_U8 *) * NBLK_V_MAX);
    for( y = 0;y < NBLK_V_MAX ;y++)
        _depthBlkBased_tmp[y]=(MS_U8 *)malloc(sizeof(MS_U8) * NBLK_H_MAX);
    _depthBlkBased_tmp2 = (MS_U8 **)malloc(sizeof(MS_U8 *) * NBLK_V_MAX);
    for( y = 0;y < NBLK_V_MAX ;y++)
        _depthBlkBased_tmp2[y]=(MS_U8 *)malloc(sizeof(MS_U8) * NBLK_H_MAX);

    // Use Pixel based RGB3D LUT
    u16OutIn_Ratio_Ver = MAX(u32VRatio, 10);
    u16OutIn_Ratio_Hor = MAX(u32HRatio, 10);

#if DUMYGAINCTRL
    MS_U8 u8testval_1EFC = 0x00;//MFC_ReadByte(REG_1EFC);

    MS_U8 u8testval_0EBA = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5E_L, 0xff);//MFC_ReadByte(REG_0EBA);//default:0x00 for LGE 0x18
    MS_U8 u8testval_0EBB = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5F_L, 0xff00)>>8;//MFC_ReadByte(REG_0EBB);//default:0x20 for LGE 0x20
#else
    MS_U8 u8testval_1EFC = 0x00;//MFC_ReadByte(REG_1EFC);

    MS_U8 u8testval_0EBA = 0x00;//default:0x00 for LGE 0x18
    MS_U8 u8testval_0EBB = 0x20;//default:0x20 for LGE 0x20
#endif

    //S16 BLKnonSkyRate;
    MS_S16 BLKblueSkyRatioDiff;
    MS_S16 DepthSlope;

#if BOUNDARY_MODE
    MS_U8 u8BlkOffset = 0;
#endif

    MS_U16 u16DispWidth = gSrcInfo[MAIN_WINDOW].stDispWin.width;
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDispWin);
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW] == E_XC_3D_INPUT_MODE_NONE)
    {
        u16DispWidth = stDispWin.width;
    }

    pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2 = MIN(pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1,pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2);
    //BLKblueSkyRatioDiff = MAX(BLKblueSkyRatio1 - BLKblueSkyRatio2, GLB_Depth_Min);
    BLKblueSkyRatioDiff = MINMAX(pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio1 - pXCResourcePrivate->stdrvXC_3D._s16BLKblueSkyRatio2, u8testval_0EBA,u8testval_0EBB);

    DepthSlope = BLKblueSkyRatioDiff*256/32;
    MS_S16 BLKnum_V = NBLK_DD_V;

    if((u8testval_1EFC&0x20)==0x20)
    {
        for ( y = 0; y < NBLK_DD_V; y ++ )
            for ( x = 0; x < NBLK_DD_H; x ++ )
            {
                avgPixel = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
                depth = _MDrv_SC_3D_2DTo3D_InterpolationDepthFromRGB_BLK(pInstance, x, y, avgPixel.R*4, avgPixel.G*4, avgPixel.B*4 );

                if( (u8testval_1EFC&0x10)==0x10)
                {
                    //int blk_mot;
                    //int mot_depth;
                    blk_mot = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[y][x];
                    //mot_depth = 0x80;//MFC_ReadByte(REG_1EFE);//max additional mot depth
                    mot_depth = blk_mot>>1;//MIN( (mot_depth*blk_mot)>>8, 255);
                    mot_depth = (mot_depth*GMvWei)>>4;

                    depth = depth + mot_depth;
                }


                _depthBlkBased_tmp[y][x] = MINMAX(depth,0,255);

            }
    }
    else
    {
        for ( y = 0; y < NBLK_DD_V; y ++ )
        {
            for ( x = 0; x < NBLK_DD_H; x ++ )
            {
                if(((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK62_69_L)&0x80)==0x00))
                {
                    depth = (DepthSlope*(y-(BLKnum_V>>1))/(BLKnum_V) )+128;
#if MOTIONDETECTION
                    //add motion cue
                    if( (u8testval_1EFC&0x10)==0x10)
                    {

                        blk_mot = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_mot[y][x];
                        //mot_depth = 0x80;//MFC_ReadByte(REG_1EFE);//max additional mot depth
                        mot_depth = blk_mot>>1;//min( (mot_depth*blk_mot)>>8, 255);
                        mot_depth = (mot_depth*GMvWei)>>4;

                        depth = depth + mot_depth;
                    }
#endif
                 _depthBlkBased_tmp[y][x] = MINMAX(depth,0,255);
                }
                else
                {
                    DepthSlope = MAX(DepthSlope, 200);
                    //Depth Slope 0~255
                    if(y < NBLK_DD_V/4)
                        depth = (DepthSlope*(y)/(BLKnum_V) )+128;
                    else if(y < NBLK_DD_V*3/4)
                        depth = (DepthSlope*((BLKnum_V>>1)-y)/(BLKnum_V) )+128;
                    else
                        depth = (DepthSlope*(y-BLKnum_V)/(BLKnum_V) )+128;
                    _depthBlkBased_tmp[y][x] = MINMAX(depth,0,255);
                }
            }
        }
    }

    //lowpass setting
    if (pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En == 1)
    {
        s16Xrange =1;
        s16Yrange =1;
        s16Num = 9;
    }
    else if(pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En == 2)
    {
        s16Xrange =2;
        s16Yrange =2;
        s16Num =25;
    }
    else if (pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En == 3)
    {
        s16Xrange =3;
        s16Yrange =3;
        s16Num = 49;
    }
    else
    {
        s16Xrange =0;
        s16Yrange =0;
        s16Num = 1;
    }

    //lowpass or bilateral filter
    for(y = 0; y < NBLK_DD_V; y ++)
        for(x = 0; x < NBLK_DD_H; x ++)
        {

            if(pXCResourcePrivate->stdrvXC_3D._u8Lowpass_En != 0  )         // lowpass
            {
                depth = 0;
                if(x>=s16Xrange && x<NBLK_DD_H-s16Xrange && y>=s16Yrange && y<NBLK_DD_V-s16Yrange) //normal case
                {
                    for ( yy = -s16Yrange; yy <= s16Yrange; yy ++ )
                        for ( xx = -s16Xrange; xx <= s16Xrange; xx ++ )
                        {
                            yj = y+yy;
                            xj = x+xx;
                            depth += _depthBlkBased_tmp[yj][xj];
                        }
                }
                else //  boundary case
                {
                    for ( yy = -s16Yrange; yy <= s16Yrange; yy ++ )
                        for ( xx = -s16Xrange; xx <= s16Xrange; xx ++ )
                        {
                            yj = MINMAX(y+yy, 0,NBLK_DD_V-1);
                            xj = MINMAX(x+xx, 0,NBLK_DD_H-1);
                            depth += _depthBlkBased_tmp[yj][xj];
                        }
                }
                _depthBlkBased_tmp2[y][x]= depth/s16Num;
            }
            else
            {
                _depthBlkBased_tmp2[y][x]= _depthBlkBased_tmp[y][x];
            }

        }

    u16OutIn_Ratio_Ver = u16OutIn_Ratio_Ver/10;
    u16OutIn_Ratio_Hor = u16OutIn_Ratio_Hor/10;
#if OPTIMIZE
    for(id_v=0; u16OutIn_Ratio_Ver>1; id_v++)
    {
        u16OutIn_Ratio_Ver=u16OutIn_Ratio_Ver>>1;
    }
    for(id_h=0; u16OutIn_Ratio_Hor>1; id_h++)
    {
        u16OutIn_Ratio_Hor=u16OutIn_Ratio_Hor>>1;
    }
#endif
    // bilinear interpolation to scale up
    for ( y = 0; y < NBLK_DR_V; y ++ )
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {
            MS_U8 idx_x,idx_y;
#if OPTIMIZE
            idx_x =x>>id_h;//OUTIN_Ratio_Hor;
            idx_y =y>>id_v;//OUTIN_Ratio_Ver;
#else
            idx_x =x/u16OutIn_Ratio_Hor;
            idx_y =y/u16OutIn_Ratio_Ver;
#endif
            depth = 0;
            if(idx_x>=1 && idx_x<(NBLK_DD_H-1) && idx_y>=1 && idx_y< (NBLK_DD_V-1)) //normal case
            {
                if (x%2 == 0 && y%2 == 0)
                    depth=_depthBlkBased_tmp2[idx_y][idx_x];
                else if (x%2 == 1 && y%2 == 0)
                    depth=(_depthBlkBased_tmp2[idx_y][idx_x]+_depthBlkBased_tmp2[idx_y][idx_x+1])>>1;
                else if (x%2 == 0 && y%2 == 1)
                    depth=(_depthBlkBased_tmp2[idx_y][idx_x]+_depthBlkBased_tmp2[idx_y+1][idx_x])>>1;
                else
                    depth=(_depthBlkBased_tmp2[idx_y][idx_x]+_depthBlkBased_tmp2[idx_y][idx_x+1]+_depthBlkBased_tmp2[idx_y+1][idx_x]+_depthBlkBased_tmp2[idx_y+1][idx_x+1])>>2;
            }
            else //boundary case
            {

                if (x%2 == 0 && y%2 == 0)
                    depth=_depthBlkBased_tmp2[idx_y][idx_x];
                else if (x%2 == 1 && y%2 == 0)
                    depth=(_depthBlkBased_tmp2[idx_y][idx_x]+_depthBlkBased_tmp2[idx_y][MINMAX(idx_x+1,0,NBLK_DD_H-1)])>>1;
                else if (x%2 == 0 && y%2 == 1)
                    depth=(_depthBlkBased_tmp2[idx_y][idx_x]+_depthBlkBased_tmp2[MINMAX(idx_y,0,NBLK_DD_V-1)][idx_x])>>1;
                else
                    depth=(_depthBlkBased_tmp2[idx_y][idx_x]+_depthBlkBased_tmp2[idx_y][MINMAX(idx_x+1,0,NBLK_DD_H-1)]+_depthBlkBased_tmp2[MINMAX(idx_y+1,0,NBLK_DD_V-1)][idx_x]+_depthBlkBased_tmp2[MINMAX(idx_y+1,0,NBLK_DD_V-1)][MINMAX(idx_x+1,0,NBLK_DD_H-1)])>>2;
            }




            if( pXCResourcePrivate->stdrvXC_3D._u8Bilinear_En == 0)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = _depthBlkBased_tmp2[idx_y][idx_x];
            else
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = depth;

        }

#if BOUNDARY_MODE
        if(E_XC_3D_OUTPUT_TOP_BOTTOM_HW != MDrv_XC_Get_3D_Output_Mode(pInstance))
        {
            u8BlkOffset = pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset;
        }
        else
        {
            u8BlkOffset = 0x80;
        }
    for ( y = 0; y < NBLK_DR_V; y ++ )
    {
        for ( x = 0; x < NBLK_DR_H; x ++ )
        {
            if(bT3DDE_Use_MainDisplayWindow)//((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x2) == 0x2) //flag for MainDEonly mode
            {
                if(x==0||(x>=((u16DispWidth + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)-1))
                    pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = u8BlkOffset;
                else if(x==1)
                    pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = ( u8BlkOffset+ 2*pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] + pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x+1] + 2)>>2;
                else if(x==((u16DispWidth + PIXELNUMOFBLOCK - 1) / PIXELNUMOFBLOCK)-2)
                    pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = ( pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x-1] + 2*pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] + u8BlkOffset              + 2)>>2;
            }
            else
            {
            if(x==0||x==NBLK_DR_H-1)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = u8BlkOffset;
            else if(x==1)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = ( u8BlkOffset+ 2*pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] + pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x+1] + 2)>>2;
            else if(x==NBLK_DR_H-2)
                pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] = ( pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x-1] + 2*pXCResourcePrivate->stdrvXC_3D._u8DepthBlkBased[y][x] + u8BlkOffset              + 2)>>2;
        }
    }
    }
#endif

#if BACKLIGHTIIR
    if(pXCResourcePrivate->stdrvXC_3D.u8IIR_En==1)
    {
        _MDrv_SC_3D_2DTo3D_PrepareBLKDepth_IIR(pInstance);
    }

#endif

    for( y = 0;y < NBLK_V_MAX ;y++)
        free(_depthBlkBased_tmp[y]);
    free(_depthBlkBased_tmp);
    for( y = 0;y < NBLK_V_MAX ;y++)
        free(_depthBlkBased_tmp2[y]);
    free(_depthBlkBased_tmp2);

}
#endif

static MS_BOOL _MDrv_SC_3D_IsPattern(void *pInstance, MS_U16 x, MS_U16 y, MS_BOOL bHonrizantol)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bRet = FALSE;
    PIXEL Pixel1Quadrant = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H - 1 - x];
    PIXEL Pixel2Quadrant = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
    PIXEL Pixel3Quadrant = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V - 1 - y][x];
    PIXEL Pixel4Quadrant = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V - 1 - y][NBLK_DD_H - 1 - x];
    if (bHonrizantol)
    {
        MS_U16 u16Diff1_2 = ((abs( Pixel1Quadrant.R - Pixel2Quadrant.R) + abs( Pixel1Quadrant.G - Pixel2Quadrant.G) + abs( Pixel1Quadrant.B - Pixel2Quadrant.B)));
        MS_U16 u16Diff3_4 = ((abs( Pixel3Quadrant.R - Pixel4Quadrant.R) + abs( Pixel3Quadrant.G - Pixel4Quadrant.G) + abs( Pixel3Quadrant.B - Pixel4Quadrant.B)));
        if (MAX(u16Diff1_2, u16Diff3_4) < PATTERN_THRESLD)
        {
            bRet = TRUE;
        }
        else
        {
            bRet = FALSE;
        }
    }
    else
    {
        MS_U16 u16Diff2_3 = ((abs( Pixel3Quadrant.R - Pixel2Quadrant.R) + abs( Pixel3Quadrant.G - Pixel2Quadrant.G) + abs( Pixel3Quadrant.B - Pixel2Quadrant.B)));
        MS_U16 u16Diff4_1 = ((abs( Pixel1Quadrant.R - Pixel4Quadrant.R) + abs( Pixel1Quadrant.G - Pixel4Quadrant.G) + abs( Pixel1Quadrant.B - Pixel4Quadrant.B)));
        if (MAX(u16Diff2_3, u16Diff4_1) < PATTERN_THRESLD)
        {
            bRet = TRUE;
        }
        else
        {
            bRet = FALSE;
        }
    }
    return bRet;
}

PIXEL stblockStatusEven[NBLK_DD_V_MAX][NBLK_DD_H_MAX]; // avg color value for even lines
PIXEL stblockStatusOdd[NBLK_DD_V_MAX][NBLK_DD_H_MAX];  // avg color value for odd lines
static void _MDrv_SC_3D_SourceDetection(void *pInstance)
{
    MS_U16 x = 0, y = 0;
    PIXEL Pixel1;
    PIXEL Pixel10, Pixel11, Pixel12, Pixel13, Pixel14, Pixel15, Pixel16;
    MS_U16 u16YDiff0 = 0, u16YDiff1 = 0, u16YDiff2 = 0, u16YDiff3 = 0;
    MS_U16 u16YDiff4 = 0, u16YDiff5 = 0, u16YDiff6 = 0;
    EN_3D_FORMAT enFormat = E_3D_FORMAT_2D;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_3D._bResetFrameCount)
    {
        pXCResourcePrivate->stdrvXC_3D._u16SBSCount = 0;
        pXCResourcePrivate->stdrvXC_3D._u16TBCount = 0;
        pXCResourcePrivate->stdrvXC_3D._u162DCount = 0;
        pXCResourcePrivate->stdrvXC_3D._u16TotalCount = 0;
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = FALSE;
    }

    if((IS_INPUT_FRAME_PACKING(MAIN_WINDOW)
          || IS_INPUT_TOP_BOTTOM(MAIN_WINDOW)
          || IS_INPUT_SIDE_BY_SIDE_HALF(MAIN_WINDOW))
       && IS_OUTPUT_LINE_ALTERNATIVE())
    {
#if defined (__aarch64__)
        MS_U64 i = 0, j = 0, i_dummy = 0, j_dummy = 0;
#else
        MS_S32 i = 0, j = 0, i_dummy = 0, j_dummy = 0;
#endif
        MS_U8 u8R_Cr_Avg = 0;
        MS_U8 u8G_Y_Avg = 0;
        MS_U8 u8B_Cb_Avg = 0;
        MS_VIRT u32DDBase = MsOS_PA2KSEG1(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf);
        if(0 == u32DDBase)
        {
            printf("%s :u32DDBase = 0!,error!!!\n", __FUNCTION__);
            assert(u32DDBase != 0);
        }
        MS_U16 u16DDBlock_DramSize = DDBYTENUM;
        MS_U32 u32DDBlockLinePitch = (NBLK_DD_PITCH*DDBYTENUM);
        MS_U16 u16MinDiff = 0;
        MS_U16 u16MaxDiff = 0;
        MS_U32 u32MatchCount = 0;

        for (i = 0; i < NBLK_DD_V; i++)
        {
            i_dummy = i * u32DDBlockLinePitch;
            for (j = 0; j < NBLK_DD_H; j++)
            {
                j_dummy = j* u16DDBlock_DramSize;

                u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 6) + u32DDBase));
                u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 7) + u32DDBase));
                u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 8) + u32DDBase));

                u8R_Cr_Avg = MINMAX((((u8R_Cr_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);
                u8G_Y_Avg = MINMAX((((u8G_Y_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);
                u8B_Cb_Avg = MINMAX((((u8B_Cb_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);

                stblockStatusEven[i][j].R = u8R_Cr_Avg; // avg color value
                stblockStatusEven[i][j].G = u8G_Y_Avg; // avg color value
                stblockStatusEven[i][j].B = u8B_Cb_Avg; // avg color value

                u8G_Y_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 22) + u32DDBase));
                u8B_Cb_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 23) + u32DDBase));
                u8R_Cr_Avg = *((volatile MS_U8*)(( i_dummy + j_dummy + 24) + u32DDBase));

                u8R_Cr_Avg = MINMAX((((u8R_Cr_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);
                u8G_Y_Avg = MINMAX((((u8G_Y_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);
                u8B_Cb_Avg = MINMAX((((u8B_Cb_Avg-16)*MAXVALUE)/219), 0, MAXVALUE);

                stblockStatusOdd[i][j].R = u8R_Cr_Avg; // avg color value
                stblockStatusOdd[i][j].G = u8G_Y_Avg; // avg color value
                stblockStatusOdd[i][j].B = u8B_Cb_Avg; // avg color value
            }
        }

        for(y = 0; y < NBLK_DD_V; y ++)
        {
            for(x = 3; x < NBLK_DD_H - 3; x ++)
            {
                //even target pixel
                Pixel1 = stblockStatusEven[y][x];
                //odd pixels
                Pixel10 = stblockStatusOdd[y][x];
                Pixel11 = stblockStatusOdd[y][x+1];
                Pixel12 = stblockStatusOdd[y][x+2];
                Pixel13 = stblockStatusOdd[y][x+3];
                Pixel14 = stblockStatusOdd[y][x-1];
                Pixel15 = stblockStatusOdd[y][x-2];
                Pixel16 = stblockStatusOdd[y][x-3];

                //check s32Diff
                u16YDiff0 = ((abs( Pixel1.R - Pixel10.R) + abs( Pixel1.G - Pixel10.G) + abs( Pixel1.B - Pixel10.B)));
                u16YDiff1 = ((abs( Pixel1.R - Pixel11.R) + abs( Pixel1.G - Pixel11.G) + abs( Pixel1.B - Pixel11.B)));
                u16YDiff2 = ((abs( Pixel1.R - Pixel12.R) + abs( Pixel1.G - Pixel12.G) + abs( Pixel1.B - Pixel12.B)));
                u16YDiff3 = ((abs( Pixel1.R - Pixel13.R) + abs( Pixel1.G - Pixel13.G) + abs( Pixel1.B - Pixel13.B)));
                u16YDiff4 = ((abs( Pixel1.R - Pixel14.R) + abs( Pixel1.G - Pixel14.G) + abs( Pixel1.B - Pixel14.B)));
                u16YDiff5 = ((abs( Pixel1.R - Pixel15.R) + abs( Pixel1.G - Pixel15.G) + abs( Pixel1.B - Pixel15.B)));
                u16YDiff6 = ((abs( Pixel1.R - Pixel16.R) + abs( Pixel1.G - Pixel16.G) + abs( Pixel1.B - Pixel16.B)));

                u16MinDiff = MIN(MIN3(u16YDiff0, u16YDiff1, u16YDiff2), MIN(u16YDiff4, u16YDiff5));
                u16MaxDiff = MAX(MAX3(u16YDiff0, u16YDiff1, u16YDiff2), MAX(u16YDiff4, u16YDiff5));

#if 0
                if(x == 20)
                {
                    printf("u16MinDiff=%u, u16MaxDiff=%u\n", u16MinDiff, u16MaxDiff);
                }
#endif

                if (u16MinDiff <= HW_2DTO3D_3DDetect_THRESHOLD)
                {
                    u32MatchCount++;
                }
            }

            if(u32MatchCount < (NBLK_DD_V * (NBLK_DD_H - 6) * HW_2DTO3D_3DDetect_PERCENTAGE / 100))
            {
                enFormat = E_3D_FORMAT_2D;
            }
            else
            {
                //keep pre detect 3d result
                if(IS_INPUT_SIDE_BY_SIDE_HALF(MAIN_WINDOW))
                {
                    enFormat = E_3D_FORMAT_SS;
                }
                else if(IS_INPUT_TOP_BOTTOM(MAIN_WINDOW))
                {
                    enFormat = E_3D_FORMAT_TB;
                }
                else
                {
                    enFormat = E_3D_FORMAT_2D;
                }
            }
        }
#if PRINTINFO
        printf("u32MatchCount=%lu, treshold s32Count=%u, enFormat=%u\n", u32MatchCount, (NBLK_DD_V * (NBLK_DD_H - 6) * HW_2DTO3D_3DDetect_PERCENTAGE / 100), enFormat);
#endif
    }
    else
    {
        PIXEL Pixel3;
        PIXEL Pixel30, Pixel31, Pixel32, Pixel33, Pixel34, Pixel35, Pixel36;
        PIXEL Pixel2_0, Pixel2_1, Pixel2_2, Pixel2_3, Pixel2_4, Pixel2_5, Pixel2_6;
        PIXEL Pixel3_0, Pixel3_1, Pixel3_2, Pixel3_3, Pixel3_4, Pixel3_5, Pixel3_6;
        MS_U16 u16YDiff7 = 0, u16YDiff8 = 0, u16YDiff9 = 0, u16YDiff10 = 0, u16YDiff11 = 0;
        MS_U16 u16YDiff12 = 0, u16YDiff13 = 0;
        MS_U16 u16YDiff30 = 0, u16YDiff31 = 0, u16YDiff32 = 0, u16YDiff33 = 0;
        MS_U16 u16YDiff34 = 0, u16YDiff35 = 0, u16YDiff36 = 0, u16YDiff37 = 0;
        MS_U16 u16YDiff38 = 0, u16YDiff39 = 0, u16YDiff310 = 0, u16YDiff311 = 0;
        MS_U16 u16YDiff312 = 0, u16YDiff313 = 0;
        MS_U16 u16YDiff2_0 = 0, u16YDiff2_1 = 0, u16YDiff2_2 = 0, u16YDiff2_3 = 0;
        MS_U16 u16YDiff2_4 = 0, u16YDiff2_5 = 0, u16YDiff2_6 = 0, u16YDiff2_7 = 0;
        MS_U16 u16YDiff2_8 = 0, u16YDiff2_9 = 0, u16YDiff2_10 = 0, u16YDiff2_11 = 0;
        MS_U16 u16YDiff2_12 = 0, u16YDiff2_13 = 0;
        MS_U16 u16MinTB = 0, u16MinSS = 0, u16MinTB2 = 0, u16MinSS2 = 0;
        MS_U16 u16MaxTB_1 = 0, u16MaxSS_1 = 0, u16MaxTB = 0, u16MaxSS = 0, u16MaxTB2 = 0, u16MaxSS2 = 0;
        MS_U16 u16SSCount = 0, u16TBCount = 0, u162DCount = 0, u16HLinesymmetryCount = 0, u16VLinesymmetryCount = 0;
        MS_U16 u16HBlockShift = 0;
        PIXEL Pixel4;
        MS_U16 u16YDiff4_0 = 0, u16YDiff4_1 = 0, u16YDiff4_2 = 0, u16YDiff4_3 = 0;
        MS_U16 u16YDiff4_4 = 0, u16YDiff4_5 = 0, u16YDiff4_6 = 0, u16YDiff4_7 = 0;
        MS_U16 u16YDiff4_8 = 0, u16YDiff4_9 = 0, u16YDiff4_10 = 0, u16YDiff4_11 = 0;
        MS_U16 u16YDiff4_12 = 0, u16YDiff4_13 = 0;
        MS_U16 u16MaxTB_2 = 0, u16MaxSS_2 = 0;
        MS_U16 u16MinTB_1 = 0;
        MS_U16 u16MinTB_2 = 0;
        MS_U16 u16MinSS_1 = 0;
        MS_U16 u16MinSS_2 = 0;

        if(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.bEnableOverscan)
        {
            if(gSrcInfo[MAIN_WINDOW].u16HCusScalingSrc > 0)
            {
                u16HBlockShift = (MS_U32)gSrcInfo[MAIN_WINDOW].ScaledCropWin.x
                                     * gSrcInfo[MAIN_WINDOW].u16HCusScalingDst
                                     / gSrcInfo[MAIN_WINDOW].u16HCusScalingSrc / PIXELNUMOFBLOCK;
            }
        }

        if(u16HBlockShift > 3)
        {
            u16HBlockShift = 3;
        }
    #if (HW_2DTO3D_VER == 3)
        _MDrv_SC_3D_2DTo3D_HWInfo_Read(pInstance);
    #elif (HW_2DTO3D_VER == 4)
        _MDrv_SC_3D_2DTo3D_HWInfo_Read_U9(pInstance);
    #endif
        for(y = 0; y < NBLK_DD_V/2; y ++)
        {
            for(x = 3-u16HBlockShift; x < NBLK_DD_H/2-3-u16HBlockShift; x ++)
            {
                MS_BOOL bIsPattern = FALSE;
                Pixel1 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x];
                Pixel3 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x+(NBLK_DD_H/2)+u16HBlockShift];
                Pixel4 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][NBLK_DD_H-x];

                //horizontal postion pixels
                Pixel10 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)+u16HBlockShift];
                Pixel11 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)+1+u16HBlockShift];
                Pixel12 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)+2+u16HBlockShift];
                Pixel13 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)+3+u16HBlockShift];
                Pixel14 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)-1+u16HBlockShift];
                Pixel15 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)-2+u16HBlockShift];
                Pixel16 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][x+(NBLK_DD_H/2)-3+u16HBlockShift];

                //horizontal opposite position pixels
                Pixel2_0 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x];
                Pixel2_1 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x+1];
                Pixel2_2 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x+2];
                Pixel2_3 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x+3];
                Pixel2_4 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x-1];
                Pixel2_5 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x-2];
                Pixel2_6 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y][NBLK_DD_H-x-3];

                //vertical postion pixels
                Pixel30 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x+u16HBlockShift];
                Pixel31 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x+1+u16HBlockShift];
                Pixel32 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x+2+u16HBlockShift];
                Pixel33 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x+3+u16HBlockShift];
                Pixel34 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x-1+u16HBlockShift];
                Pixel35 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x-2+u16HBlockShift];
                Pixel36 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[y+(NBLK_DD_V/2)][x-3+u16HBlockShift];

                // vertical opposite position pixels
                Pixel3_0 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x];
                Pixel3_1 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x+1];
                Pixel3_2 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x+2];
                Pixel3_3 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x+3];
                Pixel3_4 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x-1];
                Pixel3_5 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x-2];
                Pixel3_6 = pXCResourcePrivate->stdrvXC_3D._stBlockStatus_Avg[NBLK_DD_V- y][x-3];

                //SS
                u16YDiff0 = ((abs( Pixel1.R - Pixel10.R) + abs( Pixel1.G - Pixel10.G) + abs( Pixel1.B - Pixel10.B)));
                u16YDiff1 = ((abs( Pixel1.R - Pixel11.R) + abs( Pixel1.G - Pixel11.G) + abs( Pixel1.B - Pixel11.B)));
                u16YDiff2 = ((abs( Pixel1.R - Pixel12.R) + abs( Pixel1.G - Pixel12.G) + abs( Pixel1.B - Pixel12.B)));
                u16YDiff3 = ((abs( Pixel1.R - Pixel13.R) + abs( Pixel1.G - Pixel13.G) + abs( Pixel1.B - Pixel13.B)));
                u16YDiff4 = ((abs( Pixel1.R - Pixel14.R) + abs( Pixel1.G - Pixel14.G) + abs( Pixel1.B - Pixel14.B)));
                u16YDiff5 = ((abs( Pixel1.R - Pixel15.R) + abs( Pixel1.G - Pixel15.G) + abs( Pixel1.B - Pixel15.B)));
                u16YDiff6 = ((abs( Pixel1.R - Pixel16.R) + abs( Pixel1.G - Pixel16.G) + abs( Pixel1.B - Pixel16.B)));

                //TB
                u16YDiff7 = ((abs( Pixel1.R - Pixel30.R) + abs( Pixel1.G - Pixel30.G) + abs( Pixel1.B - Pixel30.B)));
                u16YDiff8 = ((abs( Pixel1.R - Pixel31.R) + abs( Pixel1.G - Pixel31.G) + abs( Pixel1.B - Pixel31.B)));
                u16YDiff9 = ((abs( Pixel1.R - Pixel32.R) + abs( Pixel1.G - Pixel32.G) + abs( Pixel1.B - Pixel32.B)));
                u16YDiff10 = ((abs( Pixel1.R - Pixel33.R) + abs( Pixel1.G - Pixel33.G) + abs( Pixel1.B - Pixel33.B)));
                u16YDiff11 = ((abs( Pixel1.R - Pixel34.R) + abs( Pixel1.G - Pixel34.G) + abs( Pixel1.B - Pixel34.B)));
                u16YDiff12 = ((abs( Pixel1.R - Pixel35.R) + abs( Pixel1.G - Pixel35.G) + abs( Pixel1.B - Pixel35.B)));
                u16YDiff13 = ((abs( Pixel1.R - Pixel36.R) + abs( Pixel1.G - Pixel36.G) + abs( Pixel1.B - Pixel36.B)));


                //TB
                u16YDiff30 = ((abs( Pixel3.R - Pixel10.R) + abs( Pixel3.G - Pixel10.G) + abs( Pixel3.B - Pixel10.B)));
                u16YDiff31 = ((abs( Pixel3.R - Pixel11.R) + abs( Pixel3.G - Pixel11.G) + abs( Pixel3.B - Pixel11.B)));
                u16YDiff32 = ((abs( Pixel3.R - Pixel12.R) + abs( Pixel3.G - Pixel12.G) + abs( Pixel3.B - Pixel12.B)));
                u16YDiff33 = ((abs( Pixel3.R - Pixel13.R) + abs( Pixel3.G - Pixel13.G) + abs( Pixel3.B - Pixel13.B)));
                u16YDiff34 = ((abs( Pixel3.R - Pixel14.R) + abs( Pixel3.G - Pixel14.G) + abs( Pixel3.B - Pixel14.B)));
                u16YDiff35 = ((abs( Pixel3.R - Pixel15.R) + abs( Pixel3.G - Pixel15.G) + abs( Pixel3.B - Pixel15.B)));
                u16YDiff36 = ((abs( Pixel3.R - Pixel16.R) + abs( Pixel3.G - Pixel16.G) + abs( Pixel3.B - Pixel16.B)));

                //SS
                u16YDiff37 = ((abs( Pixel3.R - Pixel30.R) + abs( Pixel3.G - Pixel30.G) + abs( Pixel3.B - Pixel30.B)));
                u16YDiff38 = ((abs( Pixel3.R - Pixel31.R) + abs( Pixel3.G - Pixel31.G) + abs( Pixel3.B - Pixel31.B)));
                u16YDiff39 = ((abs( Pixel3.R - Pixel32.R) + abs( Pixel3.G - Pixel32.G) + abs( Pixel3.B - Pixel32.B)));
                u16YDiff310 = ((abs( Pixel3.R - Pixel33.R) + abs( Pixel3.G - Pixel33.G) + abs( Pixel3.B - Pixel33.B)));
                u16YDiff311 = ((abs( Pixel3.R - Pixel34.R) + abs( Pixel3.G - Pixel34.G) + abs( Pixel3.B - Pixel34.B)));
                u16YDiff312 = ((abs( Pixel3.R - Pixel35.R) + abs( Pixel3.G - Pixel35.G) + abs( Pixel3.B - Pixel35.B)));
                u16YDiff313 = ((abs( Pixel3.R - Pixel36.R) + abs( Pixel3.G - Pixel36.G) + abs( Pixel3.B - Pixel36.B)));

                // opposite position difference
                //SS
                u16YDiff2_0 = ((abs( Pixel1.R - Pixel2_0.R) + abs( Pixel1.G - Pixel2_0.G) + abs( Pixel1.B - Pixel2_0.B)));
                u16YDiff2_1 = ((abs( Pixel1.R - Pixel2_1.R) + abs( Pixel1.G - Pixel2_1.G) + abs( Pixel1.B - Pixel2_1.B)));
                u16YDiff2_2 = ((abs( Pixel1.R - Pixel2_2.R) + abs( Pixel1.G - Pixel2_2.G) + abs( Pixel1.B - Pixel2_2.B)));
                u16YDiff2_3 = ((abs( Pixel1.R - Pixel2_3.R) + abs( Pixel1.G - Pixel2_3.G) + abs( Pixel1.B - Pixel2_3.B)));
                u16YDiff2_4 = ((abs( Pixel1.R - Pixel2_4.R) + abs( Pixel1.G - Pixel2_4.G) + abs( Pixel1.B - Pixel2_4.B)));
                u16YDiff2_5 = ((abs( Pixel1.R - Pixel2_5.R) + abs( Pixel1.G - Pixel2_5.G) + abs( Pixel1.B - Pixel2_5.B)));
                u16YDiff2_6 = ((abs( Pixel1.R - Pixel2_6.R) + abs( Pixel1.G - Pixel2_6.G) + abs( Pixel1.B - Pixel2_6.B)));

                //TB
                u16YDiff2_7 = ((abs( Pixel1.R - Pixel3_0.R) + abs( Pixel1.G - Pixel3_0.G) + abs( Pixel1.B - Pixel3_0.B)));
                u16YDiff2_8 = ((abs( Pixel1.R - Pixel3_1.R) + abs( Pixel1.G - Pixel3_1.G) + abs( Pixel1.B - Pixel3_1.B)));
                u16YDiff2_9 = ((abs( Pixel1.R - Pixel3_2.R) + abs( Pixel1.G - Pixel3_2.G) + abs( Pixel1.B - Pixel3_2.B)));
                u16YDiff2_10 = ((abs( Pixel1.R - Pixel3_3.R) + abs( Pixel1.G - Pixel3_3.G) + abs( Pixel1.B - Pixel3_3.B)));
                u16YDiff2_11 = ((abs( Pixel1.R - Pixel3_4.R) + abs( Pixel1.G - Pixel3_4.G) + abs( Pixel1.B - Pixel3_4.B)));
                u16YDiff2_12 = ((abs( Pixel1.R - Pixel3_5.R) + abs( Pixel1.G - Pixel3_5.G) + abs( Pixel1.B - Pixel3_5.B)));
                u16YDiff2_13 = ((abs( Pixel1.R - Pixel3_6.R) + abs( Pixel1.G - Pixel3_6.G) + abs( Pixel1.B - Pixel3_6.B)));

                //SS
                u16YDiff4_0 = ((abs( Pixel4.R - Pixel3_0.R) + abs( Pixel4.G - Pixel3_0.G) + abs( Pixel4.B - Pixel3_0.B)));
                u16YDiff4_1 = ((abs( Pixel4.R - Pixel3_1.R) + abs( Pixel4.G - Pixel3_1.G) + abs( Pixel4.B - Pixel3_1.B)));
                u16YDiff4_2 = ((abs( Pixel4.R - Pixel3_2.R) + abs( Pixel4.G - Pixel3_2.G) + abs( Pixel4.B - Pixel3_2.B)));
                u16YDiff4_3 = ((abs( Pixel4.R - Pixel3_3.R) + abs( Pixel4.G - Pixel3_3.G) + abs( Pixel4.B - Pixel3_3.B)));
                u16YDiff4_4 = ((abs( Pixel4.R - Pixel3_4.R) + abs( Pixel4.G - Pixel3_4.G) + abs( Pixel4.B - Pixel3_4.B)));
                u16YDiff4_5 = ((abs( Pixel4.R - Pixel3_5.R) + abs( Pixel4.G - Pixel3_5.G) + abs( Pixel4.B - Pixel3_5.B)));
                u16YDiff4_6 = ((abs( Pixel4.R - Pixel3_6.R) + abs( Pixel4.G - Pixel3_6.G) + abs( Pixel4.B - Pixel3_6.B)));

                //TB
                u16YDiff4_7 = ((abs( Pixel4.R - Pixel2_0.R) + abs( Pixel4.G - Pixel2_0.G) + abs( Pixel4.B - Pixel2_0.B)));
                u16YDiff4_8 = ((abs( Pixel4.R - Pixel2_1.R) + abs( Pixel4.G - Pixel2_1.G) + abs( Pixel4.B - Pixel2_1.B)));
                u16YDiff4_9 = ((abs( Pixel4.R - Pixel2_2.R) + abs( Pixel4.G - Pixel2_2.G) + abs( Pixel4.B - Pixel2_2.B)));
                u16YDiff4_10 = ((abs( Pixel4.R - Pixel2_3.R) + abs( Pixel4.G - Pixel2_3.G) + abs( Pixel4.B - Pixel2_3.B)));
                u16YDiff4_11 = ((abs( Pixel4.R - Pixel2_4.R) + abs( Pixel4.G - Pixel2_4.G) + abs( Pixel4.B - Pixel2_4.B)));
                u16YDiff4_12 = ((abs( Pixel4.R - Pixel2_5.R) + abs( Pixel4.G - Pixel2_5.G) + abs( Pixel4.B - Pixel2_5.B)));
                u16YDiff4_13 = ((abs( Pixel4.R - Pixel2_6.R) + abs( Pixel4.G - Pixel2_6.G) + abs( Pixel4.B - Pixel2_6.B)));

                u16MaxTB = MAX3(MAX3(u16YDiff7, u16YDiff8, u16YDiff9), MAX3(u16YDiff10, u16YDiff11, u16YDiff12), u16YDiff13);
                u16MaxSS = MAX(MAX3(u16YDiff0, u16YDiff1, u16YDiff2), MAX(u16YDiff4, u16YDiff5));
                u16MaxTB2 = MAX3(MAX3( u16YDiff30, u16YDiff31, u16YDiff32), MAX3(u16YDiff33, u16YDiff34, u16YDiff35), u16YDiff36);
                u16MaxSS2 = MAX(MAX3(u16YDiff37, u16YDiff38, u16YDiff39), MAX(u16YDiff311, u16YDiff312));

                u16MaxTB = (u16MaxTB + u16MaxTB2)/2;
                u16MaxSS = (u16MaxSS + u16MaxSS2)/2;

                u16MinTB = MIN3(MIN3(u16YDiff7, u16YDiff8, u16YDiff9), MIN3(u16YDiff10, u16YDiff11, u16YDiff12), u16YDiff13);
                u16MinSS = MIN(MIN3(u16YDiff0, u16YDiff1, u16YDiff2), MIN(u16YDiff4, u16YDiff5));
                u16MinTB2 = MIN3(MIN3( u16YDiff30, u16YDiff31, u16YDiff32), MIN3(u16YDiff33, u16YDiff34, u16YDiff35), u16YDiff36);
                u16MinSS2 = MIN(MIN3(u16YDiff37, u16YDiff38, u16YDiff39), MIN(u16YDiff311, u16YDiff312));

                u16MinTB = u16MinTB + u16MinTB2;
                u16MinSS = u16MinSS + u16MinSS2;

                u16MaxTB_1 = MAX3(MAX3(u16YDiff2_7, u16YDiff2_8, u16YDiff2_9), MAX3(u16YDiff2_10, u16YDiff2_11, u16YDiff2_12), u16YDiff2_13);
                u16MaxSS_1 = MAX3(MAX3(u16YDiff2_0, u16YDiff2_1, u16YDiff2_2), MAX3(u16YDiff2_3,  u16YDiff2_4,  u16YDiff2_5),  u16YDiff2_6);
                u16MaxTB_2 = MAX3(MAX3(u16YDiff4_7, u16YDiff4_8, u16YDiff4_9), MAX3(u16YDiff4_10, u16YDiff4_11, u16YDiff4_12), u16YDiff4_13);
                u16MaxSS_2 = MAX3(MAX3(u16YDiff4_0, u16YDiff4_1, u16YDiff4_2), MAX3(u16YDiff4_3,  u16YDiff4_4,  u16YDiff4_5),  u16YDiff4_6);


                u16MaxTB_1 = (u16MaxTB_1 + u16MaxTB_2)/2;
                u16MaxSS_1 = (u16MaxSS_1 + u16MaxSS_2)/2;


                u16MinTB_1 = MIN3(MIN3(u16YDiff2_7, u16YDiff2_8, u16YDiff2_9), MIN3(u16YDiff2_10, u16YDiff2_11, u16YDiff2_12), u16YDiff2_13);
                u16MinSS_1 = MIN3(MIN3(u16YDiff2_0, u16YDiff2_1, u16YDiff2_2), MIN3(u16YDiff2_3,  u16YDiff2_4,  u16YDiff2_5),  u16YDiff2_6);
                u16MinTB_2 = MIN3(MIN3(u16YDiff4_7, u16YDiff4_8, u16YDiff4_9), MIN3(u16YDiff4_10, u16YDiff4_11, u16YDiff4_12), u16YDiff4_13);
                u16MinSS_2 = MIN3(MIN3(u16YDiff4_0, u16YDiff4_1, u16YDiff4_2), MIN3(u16YDiff4_3,  u16YDiff4_4,  u16YDiff4_5),  u16YDiff4_6);

                u16MinTB_1 = u16MinTB_1 + u16MinTB_2;
                u16MinSS_1 = u16MinSS_1 + u16MinSS_2;


                if (_MDrv_SC_3D_IsPattern(pInstance, x, y, TRUE))
                {
                    u16HLinesymmetryCount++;
                    bIsPattern = TRUE;
                }


                if (_MDrv_SC_3D_IsPattern(pInstance, x, y, FALSE))
                {
                    u16VLinesymmetryCount++;
                    bIsPattern = TRUE;
                }

                if (!bIsPattern)
                {
                    if ( MAX(u16MinSS, u16MinTB) <= 20 )
                    {
                        //may consider special pattern in future
#if 0
                        if( u16MaxTB < MIN3((4*u16MaxSS/5), u16MaxTB_1, 140) && (u16MinTB!= u16MinSS) && (u16MaxTB!= u16MaxSS))
                        {
#if 1
                            if(x == 10)
                            {
                                printf("tb\n");
                            }
#endif
                            u16TBCount = u16TBCount+1;
                        }
                        else if( u16MaxSS < MIN3((3*u16MaxTB/4), u16MaxSS_1, 60) && (u16MinTB!= u16MinSS) && (u16MaxTB!= u16MaxSS))
                        {
#if 1
                            if(x == 10)
                            {
                                printf("ss\n");
                            }
#endif
                            u16SSCount = u16SSCount+1;
                        }

                        if(x == 10)
                        {
                            printf("eeemaxTB %u, u16MaxSS %u, u16MinTB %u, u16MinSS %u, max1TB %u, max1SS %u\n", u16MaxTB, u16MaxSS, u16MinTB, u16MinSS, u16MaxTB_1, u16MaxSS_1);
                        }
#endif
                    }
                    else if (( MIN(u16MaxTB_1, u16MaxSS_1) > 25 ) && (u16MinTB!= u16MinSS))
                    {
                        if( u16MinTB < MIN3((4*u16MinSS/5), u16MaxTB_1, 140) && (u16MinTB < u16MaxTB))
                        {
#if 0
                            if(x == 10)
                            {
                                printf("tb\n");
                            }
#endif
                            u16TBCount = u16TBCount+1;
                        }
                        else if( u16MinSS < MIN3((3*u16MinTB/4), u16MaxSS_1, 80) && (u16MinSS < u16MaxSS))
                        {
#if 0
                            if(x == 10)
                            {
                                printf("ss\n");
                            }
#endif
                            u16SSCount = u16SSCount+1;
                        }
                        else if( MIN(u16MinTB, u16MinSS)!=0 )
                        {
#if 0
                            if(x == 10)
                            {
                                printf("2d\n");
                            }
#endif
                            u162DCount = u162DCount+1;
                        }
#if 0
                        if(x == 10)
                        {
                            printf("fffmaxTB %u, u16MaxSS %u, u16MinTB %u, u16MinSS %u, max1TB %u, max1SS %u, u16MinTB_1 %u, u16MinSS_1 %u\n", u16MaxTB, u16MaxSS, u16MinTB, u16MinSS, u16MaxTB_1, u16MaxSS_1, u16MinTB_1, u16MinSS_1);
                        }
#endif
                    }
                }
            }
        }
        if (u16HLinesymmetryCount < NBLK_DD_V*(NBLK_DD_H/4-4))
        {
            if (u16VLinesymmetryCount <= (NBLK_DD_H - 12)*(NBLK_DD_V-4)/4)
            {
                if(u16HBlockShift > 0)//overscan case
                {
                    if ( MAX3(u16TBCount, u16SSCount, u162DCount)<= 15)//pure image handle
                    {
                        enFormat = E_3D_FORMAT_2D;
                    }
                    else if((u16SSCount > u16TBCount) && (u16SSCount > u162DCount))//ss predict
                    {
                        if(u16SSCount < 50)//small image case
                        {
                            if(u16SSCount>6*u162DCount)
                            {
                                enFormat = E_3D_FORMAT_SS;
                            }
                            if((u16SSCount>3*u162DCount) && (u16SSCount>6*u16TBCount))
                            {
                                enFormat = E_3D_FORMAT_SS;
                            }
                            else
                            {
                                enFormat = E_3D_FORMAT_2D;
                            }
                        }
#if (HW_2DTO3D_VER == 4)
                        else if(2*u16SSCount>5*u162DCount)//general case
#else
                        else if(u16SSCount>4*u162DCount)//general case
#endif
                        {
                            enFormat = E_3D_FORMAT_SS;
                        }
                        //else if((u16SSCount>2*u162DCount) && (u16SSCount>4*u16TBCount))
                        else if((u16SSCount>5*u162DCount/2) && (u16SSCount>4*u16TBCount))
                        {
                            enFormat = E_3D_FORMAT_SS;
                        }
                        else
                        {
                            enFormat = E_3D_FORMAT_2D;
                        }
                    }
                    else if((u16TBCount > u16SSCount) && (u16TBCount > u162DCount))//tb predict
                    {
                        if(u16TBCount < 50)//small image case
                        {
                            if(u16TBCount > 6*u162DCount)
                            {
                                enFormat = E_3D_FORMAT_TB;
                            }
                            else if((u16TBCount > 3*u162DCount) && (u16TBCount > 6*u16SSCount))
                            {
                                enFormat = E_3D_FORMAT_TB;
                            }
                            else
                            {
                                enFormat = E_3D_FORMAT_2D;
                            }
                        }
                        else if(u16TBCount > 3*u162DCount)//general case
                        {
                            enFormat = E_3D_FORMAT_TB;
                        }
                        //else if((u16TBCount>2*u162DCount) && (u16TBCount>4*u16SSCount))
                        //else if((u16TBCount > 100) && (u16TBCount>9*u162DCount/5) && (u16TBCount>13*u16SSCount/2))
                        else if((u16TBCount>5*u162DCount/2) && (u16TBCount>4*u16SSCount))
                        {
                            enFormat = E_3D_FORMAT_TB;
                        }
                        else
                        {
                            enFormat = E_3D_FORMAT_2D;
                        }
                    }
                    else//2d predict
                    {
                        enFormat = E_3D_FORMAT_2D;
                    }
                }
                else
                {
                    if ( MAX3(u16TBCount, u16SSCount, u162DCount)< 6)//pure image handle
                    {
                        enFormat = E_3D_FORMAT_2D;
                    }
                    else if((u16SSCount > u16TBCount) && (u16SSCount > u162DCount))//ss predict
                    {
                        if(u16SSCount < 50)//small image case
                        {
                            if(u16SSCount>7*u162DCount)
                            {
                                enFormat = E_3D_FORMAT_SS;
                            }
                            else
                            {
                                enFormat = E_3D_FORMAT_2D;
                            }
                        }
#if (HW_2DTO3D_VER == 4)
                        else if(2*u16SSCount>5*u162DCount)//general case
#else
                        else if(u16SSCount>4*u162DCount)//general case
#endif
                        {
                            enFormat = E_3D_FORMAT_SS;
                        }
                        else
                        {
                            enFormat = E_3D_FORMAT_2D;
                        }
                    }
                    else if((u16TBCount > u16SSCount) && (u16TBCount > u162DCount))//tb predict
                    {
                        if(u16TBCount < 50)//small image case
                        {
                            if(u16TBCount > 7*u162DCount)
                            {
                                enFormat = E_3D_FORMAT_TB;
                            }
                            else
                            {
                                enFormat = E_3D_FORMAT_2D;
                            }
                        }
                        else if(u16TBCount > 4*u162DCount)//general case
                        {
                            enFormat = E_3D_FORMAT_TB;
                        }
                        else
                        {
                            enFormat = E_3D_FORMAT_2D;
                        }
                    }
                    else//2d predict
                    {
                        enFormat = E_3D_FORMAT_2D;
                    }
                }
            }
            else
            {
                enFormat = E_3D_FORMAT_TB_PATTERN;
            }
        }
        else
        {
            enFormat = E_3D_FORMAT_SS_PATTERN;
        }
#if PRINTINFO
    printf("u16TBCount=%u, u16SSCount=%u, u162DCount=%u, u16HLinesymmetryCount=%u, u16VLinesymmetryCount=%u, u8Format=%u\n", u16TBCount, u16SSCount, u162DCount, u16HLinesymmetryCount, u16VLinesymmetryCount, enFormat);
#endif
    }


    ///frame statistics
    pXCResourcePrivate->stdrvXC_3D._u16TotalCount++;
    if(enFormat == E_3D_FORMAT_SS)
    {
        pXCResourcePrivate->stdrvXC_3D._u16SBSCount++;
    }
    else if(enFormat == E_3D_FORMAT_TB)
    {
        pXCResourcePrivate->stdrvXC_3D._u16TBCount++;
    }
    else if(enFormat == E_3D_FORMAT_2D || enFormat == E_3D_FORMAT_SS_PATTERN || enFormat == E_3D_FORMAT_TB_PATTERN)
    {
        pXCResourcePrivate->stdrvXC_3D._u162DCount++;
    }

    if(E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
    {
        if(pXCResourcePrivate->stdrvXC_3D._u16TotalCount >= MAXFRAME_3DDETECT)
        {
            pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d_cnt=%u,ss_cnt=%u,tb_cnt=%u\n"
                                              , pXCResourcePrivate->stdrvXC_3D._u162DCount
                                              , pXCResourcePrivate->stdrvXC_3D._u16SBSCount
                                              , pXCResourcePrivate->stdrvXC_3D._u16TBCount);
            if((pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u16TBCount)
                && (pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                if(pXCResourcePrivate->stdrvXC_3D._u16SBSCount >= PERCENTAGE_3DDETECT * pXCResourcePrivate->stdrvXC_3D._u16TotalCount / 100)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "side by side\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
                }
            }
            else if((pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u16SBSCount)
                    && (pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                if(pXCResourcePrivate->stdrvXC_3D._u16TBCount >= PERCENTAGE_3DDETECT * pXCResourcePrivate->stdrvXC_3D._u16TotalCount / 100)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "top bottom\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_TOP_BOTTOM;
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                    pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
            }
        }
    }
    else if(E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
    {
        if(pXCResourcePrivate->stdrvXC_3D._u16TotalCount >= MAXFRAME_3DDETECT_COMPATIBLE)
        {
            pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d_cnt=%u,ss_cnt=%u,tb_cnt=%u\n"
                                              , pXCResourcePrivate->stdrvXC_3D._u162DCount
                                              , pXCResourcePrivate->stdrvXC_3D._u16SBSCount
                                              , pXCResourcePrivate->stdrvXC_3D._u16TBCount);
            if((pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u16TBCount)
                && (pXCResourcePrivate->stdrvXC_3D._u16SBSCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "side by side\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
            }
            else if((pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u16SBSCount)
                    && (pXCResourcePrivate->stdrvXC_3D._u16TBCount > pXCResourcePrivate->stdrvXC_3D._u162DCount))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "top bottom\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_TOP_BOTTOM;
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "2d image\n");
                pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
            }
            pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount++;
        }
    }
}
#if (HW_2DTO3D_VER == 3)
static void _MDrv_SC_3D_2DTo3D_DepthRefine(SC_INT_SRC eIntNum, void *pParam)
{
    void *pInstance = pu32XCInst_private;
    eIntNum = eIntNum;
    pParam = pParam;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(IS_INPUT_NORMAL_2D_HW(MAIN_WINDOW))
    {
        _MDrv_SC_3D_2DTo3D_UpdateRGB3DLUT(pInstance);
        _MDrv_SC_3D_2DTo3D_HWInfo_Read(pInstance);
        _MDrv_SC_3D_2DTo3D_DepthDetectionPIX(pInstance);
        _MDrv_SC_3D_2DTo3D_DepthDetectionBLK(pInstance);
        _MDrv_SC_3D_2DTo3D_HW_Info_Write(pInstance);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//left depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//right depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//right depth gain:7bits
    }

    if(((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D)
        || ((E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D))
    {
        _MDrv_SC_3D_SourceDetection(pInstance);
    }
}
#elif (HW_2DTO3D_VER == 4)
static void _MDrv_SC_3D_2DTo3D_DepthRefine(SC_INT_SRC eIntNum, void *pParam)
{
    void *pInstance = pu32XCInst_private;
    eIntNum = eIntNum;
    pParam = pParam;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));



    if(IS_INPUT_NORMAL_2D_HW(MAIN_WINDOW))
    {
#if ENABLE_T3D_DEBUG
        pXCResourcePrivate->stdrvXC_3D.u8DBG_En = (SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x0F00)>>8;
#else
        pXCResourcePrivate->stdrvXC_3D.u8DBG_En = 0;
#endif

        _MDrv_SC_3D_2DTo3D_PreSetting(pInstance);
        //if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x0001)==0)
        {
            //startTime = MsOS_GetSystemTime();
            //if(period_too_short == 0)
                _MDrv_SC_3D_2DTo3D_UpdateRGB3DLUT_U9(pInstance);
                //_MDrv_SC_3D_2DTo3D_UpdateRGB3DLUT(); //INTERRUPT
        }
#if ENABLE_T3D_DEBUG
        if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x0001)==0)
#endif
        {
        _MDrv_SC_3D_2DTo3D_HWInfo_Read_U9(pInstance);
        }
            _MDrv_SC_3D_2DTo3D_TrendDetection(pInstance);
            _MDrv_SC_3D_2DTo3D_BGColorDetection_U9(pInstance); //New, move out
            _MDrv_SC_3D_2DTo3D_DepthDetectionPIX_U9(pInstance); //New


        //if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x0010)==0)
        {
            _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT2_U9(pInstance);
            _MDrv_SC_3D_2DTo3D_PrepareRGB3DLUT_IIR(pInstance);
        }

            _MDrv_SC_3D_2DTo3D_DepthDetectionBLK_TEST(pInstance);

        if(pXCResourcePrivate->stdrvXC_3D.u8DBG_En!=0x00)
        {
            _MDrv_SC_3D_2DTo3D_DepthDetectionTmp(pInstance);
        }
#if ENABLE_T3D_DEBUG
        if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK23_58_L)&0x1000)==0)
#endif
        {
        _MDrv_SC_3D_2DTo3D_HW_Info_Write(pInstance);
        }



        //Global and Local depth gain tuning
#if 0//DUMYGAINCTRL
        MS_U8 u8tmpval_FE = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5B_L,0xff));//0EB8
        MS_U8 u8tmpval_FF = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5C_L,0xff00))>>8;//0EB9
        MS_BOOL LR_SWAP = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L,0x8000))>>15;
        MS_U16 Ggain, Pgain, Genable, Penable;
        Ggain = u8tmpval_FE&0x7F;
        Genable = (u8tmpval_FE&0x80)>>7;
        Pgain = u8tmpval_FF&0x7F;
        Penable = (u8tmpval_FF&0x80)>>7;

        if(Genable==1)
        {
            Ggain = MINMAX(Ggain,0,0x7F);
            if(LR_SWAP)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, Ggain<<8 ,0xff00);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, (Ggain+0x80)<<8,0xff00);

            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, Ggain<<8 ,0xff00);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, (Ggain+0x80)<<8,0xff00);
                //MFC_WriteByte(REG_13B1D,Ggain);
           //MFC_WriteByte(REG_13B0D,Ggain+0x80);
            }
        }
        else
        {
            if(E_XC_3D_OUTPUT_TOP_BOTTOM_HW != MDrv_XC_Get_3D_Output_Mode(pInstance))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//left depth offset
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//right depth offset
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//left depth gain:7bits
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//right depth gain:7bits
            }
        }

        if(Penable==1)
        {
            if(E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW != MDrv_XC_Get_3D_Output_Mode(pInstance))
            {
                Pgain = MINMAX(Pgain*8,0,0xFF);

                    if (LR_SWAP)
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_65_L, (~Pgain)+1 ,0x01ff);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_66_L, Pgain, 0x01ff);
                    }
                    else
                    {
                       SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_66_L, (~Pgain)+1 ,0x01ff);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_65_L, Pgain, 0x01ff);
                        //MFC_Write2BytesMask(REG_13BCC,(~Pgain)+1,0x01FF);
                       //MFC_Write2BytesMask(REG_13BCA,Pgain,0x01FF);
                    }
                }
            }
#else

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//left depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//right depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<9, 0x7F00);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<9, 0x7F00);//right depth gain:7bits

        MS_U16 Pgain_L =  pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain <<2;
        if((SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK62_06_L)&0x8000) == 0)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_65_L, (~Pgain_L) + 1, 0x01FF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_66_L, Pgain_L, 0x01FF);
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_65_L, Pgain_L, 0x01FF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_66_L, (~Pgain_L) + 1, 0x01FF);
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_67_L, 0x80, 0x00FF);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_67_L, 0x8000, 0xFF00);//right depth gain:7bits

#if ENABLE_T3D_DEBUG
        //enable black edge
        Hal_SC_3D_enable_black_edge(pInstance, TRUE);
#endif

#endif
    }

    if(((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D)
        || ((E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D))
    {
        _MDrv_SC_3D_SourceDetection(pInstance);
    }
}
#endif //(HW_2DTO3D_VER == 3)
#endif //(HW_2DTO3D_VER < 3)
#endif //HW_2DTO3D_SUPPORT
#endif //DISABLE_3D_FUNCTION
//------------------------------------------------------------------------------
//--------HW 2D to 3D CPU Depth Refine Part End---------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



//----------------------------------------
//--------3D external interface-----------
//----------------------------------------
//-------------------------------------------------------------------------------------------------
/// get 3D hw version
/// @return MS_U16                      \b OUT: 3d hw version
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_XC_Get_3D_HW_Version(void)
{
// Note: HW_DESIGN_3D_VER
// 0: T3, Janus. non 3d chip, use opm, madi etc to make 3d
// 1: T12, T13, A1: it has some 3d type specific registers. It can send out LR flag by hw
//        Yet A1 has hw 2d-->3d, but we use HW_2DTO3D_SUPPORT to indicate it.
// 2: M10, J2: based on version 1, it has all the 3d type registers. input and output 3d registers are independent.
// 3: A6: based on version 2, it removes black space before reading into memory.
// 4: A3, A5, A5P: based on version3, main sub window can both use 1920+960 line buffer, so no H prescaling is needed.
//        It has FRC-inside feature also, but we use FRC_INSIDE to indicate this feature.
    return HW_DESIGN_3D_VER;
}

MS_U16 MApi_XC_Get_3D_HW_Version_U2(void* pInstance)
{
    MS_U16 u16Return = 0;
    u16Return = MDrv_XC_Get_3D_HW_Version();
    return u16Return;
}

MS_U16 MApi_XC_Get_3D_HW_Version(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_3D_HWVERSION XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_HWVERSION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Is Supported HW 2D To 3D
/// @return MS_BOOL                      \b OUT: TRUE: HW 2D to 3D FALSE: SW 2D to 3D
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_XC_Get_3D_IsSupportedHW2DTo3D(void)
{
    return HW_2DTO3D_SUPPORT;
}

MS_BOOL MApi_XC_Get_3D_IsSupportedHW2DTo3D_U2(void* pInstance)
{
    return MDrv_XC_Get_3D_IsSupportedHW2DTo3D();
}

MS_BOOL MApi_XC_Get_3D_IsSupportedHW2DTo3D(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_3D_SUPPORT_HW2DTO3D XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_3D_SUPPORT_HW2DTO3D, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// set 3D convert format type
/// @param  e3dInputMode                \b IN: the input format
/// @param  e3dOutputMode               \b IN: the format we want to convert to
/// @param  e3dPanelType                \b IN: the panel type.
/// @param  eWindow                     \b IN: which window we are going to set
/// @return MS_BOOL                     \b OUT: TRUE:Success FALSE:Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_Mode_U2(void* pInstance,
                            E_XC_3D_INPUT_MODE e3dInputMode,
                            E_XC_3D_OUTPUT_MODE e3dOutputMode,
                            E_XC_3D_PANEL_TYPE e3dPanelType,
                            SCALER_WIN eWindow)
{
#ifndef DISABLE_3D_FUNCTION
    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_BOOL bRet = TRUE;
    E_XC_3D_INPUT_MODE eMain_3dInputMode = E_XC_3D_INPUT_MODE_NONE;
    E_XC_3D_OUTPUT_MODE eMain_3dOutputMode = E_XC_3D_OUTPUT_MODE_NONE;

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    Hal_SC_3D_InvertMemsyncInterlaceMode(pInstance);

#if((HW_DESIGN_3D_VER >= 5) || (HW_DESIGN_3D_VER == 3)) //manhattan and muji mode
    if(E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR == e3dOutputMode)
    {
        pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA = TRUE;
        e3dOutputMode = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE;
    }
    else if(E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW == e3dOutputMode)
    {
        pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA = TRUE;
        e3dOutputMode = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA = FALSE;
    }
#endif

    #if FRC_INSIDE
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal> 3000) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal> 2000) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500) &&  // 4K2K panel
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ) &&
        (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == e3dOutputMode))
    {
        // For FRC inside chip, ex.Einstein/Napoli
        // 4K1K FA output case, FRC only can handle TB in to FA out
        // so SC need to convert 3D output as TB mode
        e3dOutputMode = E_XC_3D_OUTPUT_TOP_BOTTOM;
    }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6)) //maserati 3D use FRC
     if (MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW)
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
         ||((e3dInputMode == E_XC_3D_INPUT_MODE_NONE)  // add dual view type
         &&((e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)
         ||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)))
#endif
        )
    {
        switch(e3dOutputMode)
        {
            case E_XC_3D_OUTPUT_FRAME_L:
            case E_XC_3D_OUTPUT_FRAME_R:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
                break;
            case E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF:
            case E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
                break;
            case E_XC_3D_OUTPUT_TOP_BOTTOM:
            case E_XC_3D_OUTPUT_TOP_BOTTOM_HW:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
                break;
            case E_XC_3D_OUTPUT_LINE_ALTERNATIVE:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = TRUE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = TRUE;
                e3dOutputMode = E_XC_3D_OUTPUT_TOP_BOTTOM;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_TOP_BOTTOM;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_LINE_ALTERNATIVE;
                break;
            case E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = TRUE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = TRUE;
                e3dOutputMode = E_XC_3D_OUTPUT_TOP_BOTTOM_HW;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_TOP_BOTTOM;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_LINE_ALTERNATIVE;
                break;
            case E_XC_3D_OUTPUT_FRAME_ALTERNATIVE:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = TRUE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = TRUE;
                e3dOutputMode = E_XC_3D_OUTPUT_TOP_BOTTOM;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_TOP_BOTTOM;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE;
                break;
            case E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = TRUE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = TRUE;
                e3dOutputMode = E_XC_3D_OUTPUT_TOP_BOTTOM_HW;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_TOP_BOTTOM;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE;
                break;
            default:
                pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
                pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
                pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;
                pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
                break;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "e3dOutputMode = %d, FRC Input 3D Type = %d, FRC output 3D Type = %d\n", e3dOutputMode,\
            pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode, pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode);
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._bFAToTB = FALSE;
        pXCResourcePrivate->stdrvXC_3D._bLAToTB = FALSE;
        pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled = FALSE;
        pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode = E_XC_3D_INPUT_MODE_NONE;
        pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode = E_XC_3D_OUTPUT_MODE_NONE;
    }
#endif
    #endif

#if (!(HW_2DTO3D_SUPPORT && (HW_2DTO3D_VER >= 3)))
    if(E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
    {
        if((e3dInputMode != E_XC_3D_INPUT_MODE_NONE) && (e3dOutputMode != E_XC_3D_OUTPUT_MODE_NONE))
        {
            pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D = FALSE;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D = pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D;
        }
    }
#endif

    pXCResourcePrivate->stdrvXC_3D._ePreUsedInput3DMode[eWindow]   = E_XC_3D_INPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._ePreUsedOutput3DMode           = E_XC_3D_OUTPUT_MODE_NONE;
    pXCResourcePrivate->stdrvXC_3D._bNeedRestore3DFormat = FALSE;

    if(eWindow == MAIN_WINDOW)
    {
        bRet = MDrv_XC_Is3DFormatSupported(pInstance, e3dInputMode, e3dOutputMode);
    }
    else
    {
        /// When 3D output is FI, the VOP window don't need to enable Sub
        if(e3dOutputMode != E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
        {
            eMain_3dInputMode = MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW);
            eMain_3dOutputMode = MDrv_XC_Get_3D_Output_Mode(pInstance);
            if((eMain_3dInputMode != e3dInputMode)||(eMain_3dOutputMode != e3dOutputMode))
            {
                printf("Please check the 3D combination of sub's\n");
                bRet = FALSE;
            }
        }
    }

    if(bRet == TRUE)
    {
        pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] = e3dInputMode;
        pXCResourcePrivate->stdrvXC_3D._eOutput3DMode         = e3dOutputMode;
        pXCResourcePrivate->stdrvXC_3D._e3DPanelType          = e3dPanelType;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "e3dInputMode=%u, e3dOutputMode=%u\n", e3dInputMode, e3dOutputMode);
    }
    else //if not supported, display as 2D
    {
        printf("MApi_XC_Set_3D_Mode(): invalid format, set to 3d_none\n");
        _XC_ENTRY(pInstance);
        Hal_SC_subwindow_disable(pInstance);
        _XC_RETURN(pInstance);
        pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] = E_XC_3D_INPUT_MODE_NONE;
        gSrcInfo[eWindow].stDispWin.x = 0;
        gSrcInfo[eWindow].stDispWin.y = 0;
        gSrcInfo[eWindow].stDispWin.width = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
        gSrcInfo[eWindow].stDispWin.height = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }

#if (HW_DESIGN_3D_VER == 4)
    if(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
        if(e3dPanelType == E_XC_3D_PANEL_PELLICLE)
        {
            if((e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
            ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD))
            {
#ifdef FA_1920X540_OUTPUT
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x540;
#else
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_960x1080;
#endif
            }
            else if((e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
            ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL)
            ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE))
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x540;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x1080;
            }
        }
        else if(e3dPanelType == E_XC_3D_PANEL_SHUTTER)
        {
            if((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING))
            {
                if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
                {
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                    {
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x540;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x1080;
                    }
                }
                else if(gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_720X2P_VSIZE)
                {
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                    {
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x540;
                    }
                    else
                    {
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1280x720;
                    }
                }
            }
        }
    }
    else
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_MODE_NONE;

    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC && (IS_INPUT_NORMAL_2D_HW(eWindow)))
    {
        // Enable CLK_ICLK_FRC
        W2BYTEMSK(L_CLKGEN1(0x35), 0x0004, 0x0004); // clk_idclk_frc
        W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0002); // Not Invert
        W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0001); // Enable clock
    }
    else
    {
        // Enable CLK_ICLK_FRC
        W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0004); // clk_fdclk_frc
        W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0002); // Not Invert
        W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0001); // Enable clock
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u83D_FI_out=%x\n",pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out);
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    MS_BOOL bRet = FALSE;
    UNUSED(e3dInputMode);
    UNUSED(e3dOutputMode);
    UNUSED(e3dPanelType);
    UNUSED(eWindow);

    printf("Not support!!");
#endif
    return bRet;
}

MS_BOOL MApi_XC_Set_3D_Mode(E_XC_3D_INPUT_MODE e3dInputMode,
                            E_XC_3D_OUTPUT_MODE e3dOutputMode,
                            E_XC_3D_PANEL_TYPE e3dPanelType,
                            SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_MODE XCArgs;
    XCArgs.e3dInputMode = e3dInputMode;
    XCArgs.e3dOutputMode = e3dOutputMode;
    XCArgs.e3dPanelType = e3dPanelType;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This API return 3D input format
/// @param  eWindow                \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
E_XC_3D_INPUT_MODE MDrv_XC_Get_3D_Input_Mode(void *pInstance, SCALER_WIN eWindow)
{
#ifndef DISABLE_3D_FUNCTION
    MS_ASSERT(eWindow < MAX_WINDOW);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(eWindow >= MAX_WINDOW)
    {
        return E_XC_3D_INPUT_MODE_NONE;
    }
    return pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow];
#else
    UNUSED(eWindow);
    return E_XC_3D_INPUT_MODE_NONE;
#endif
}

E_XC_3D_INPUT_MODE MApi_XC_Get_3D_Input_Mode_U2(void* pInstance, SCALER_WIN eWindow)
{
   E_XC_3D_INPUT_MODE  eInputMode = E_XC_3D_INPUT_MODE_NONE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
   _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
   eInputMode = MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow);
   _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
   return eInputMode;
}

E_XC_3D_INPUT_MODE MApi_XC_Get_3D_Input_Mode(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_XC_3D_INPUT_MODE_NONE;
        }
    }

    stXC_GET_3D_INPUT_MODE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_XC_3D_INPUT_MODE_NONE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_INPUT_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_INPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This API return 3D output format
//-------------------------------------------------------------------------------------------------
E_XC_3D_OUTPUT_MODE MDrv_XC_Get_3D_Output_Mode(void *pInstance)
{
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_3D._eOutput3DMode;
#else
    return E_XC_3D_OUTPUT_MODE_NONE;
#endif
}

E_XC_3D_OUTPUT_MODE MApi_XC_Get_3D_Output_Mode_U2(void* pInstance)
{
    E_XC_3D_OUTPUT_MODE eOutputMode = E_XC_3D_OUTPUT_MODE_NONE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    eOutputMode = MDrv_XC_Get_3D_Output_Mode(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eOutputMode;
}

E_XC_3D_OUTPUT_MODE MApi_XC_Get_3D_Output_Mode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_XC_3D_OUTPUT_MODE_NONE;
    }

    stXC_GET_3D_OUTPUT_MODE XCArgs;
    XCArgs.eReturnValue = E_XC_3D_OUTPUT_MODE_NONE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_OUTPUT_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_OUTPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

#ifdef UFO_XC_GET_3D_FORMAT
MS_BOOL MDrv_XC_Get3DFormat(void* pInstance, E_XC_3D_ATTRIBUTE_TYPE e3DAttrType, void* para, void* p3DFormat)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((NULL == para) || (NULL == p3DFormat))
    {
        printf(" \n Attention %s : %d, ERROR : (NULL == para) || (NULL == p3DFormat). \n ",__FUNCTION__,__LINE__);
        return FALSE;
    }

    E_XC_3D_OSD_BLENDING_POINT eOSDBlendingPoint = E_XC_3D_OSD_BLENDING_NUM;
    if(E_XC_3D_ATTRIBUTE_OSD == e3DAttrType)
    {
        eOSDBlendingPoint = *((E_XC_3D_OSD_BLENDING_POINT*)(para));
        if(E_XC_3D_OSD_BLENDING_SC_IP == eOSDBlendingPoint)
        {
            *((E_XC_3D_INPUT_MODE*)p3DFormat) = pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW];
            bRet = TRUE;
        }
        else if(E_XC_3D_OSD_BLENDING_SC_OP == eOSDBlendingPoint)
        {
            if(pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA && !pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled)
            {
                if(E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW == pXCResourcePrivate->stdrvXC_3D._eOutput3DMode)
                {
                    *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW;
                }
                else
                {
                    *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR;
                }
            }
            else
            {
                *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = pXCResourcePrivate->stdrvXC_3D._eOutput3DMode;
            }
            bRet = TRUE;
        }
        else if(E_XC_3D_OSD_BLENDING_FRC_IP == eOSDBlendingPoint)
        {
            *((E_XC_3D_INPUT_MODE*)p3DFormat) = pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode;
            bRet = TRUE;
        }
        else if(E_XC_3D_OSD_BLENDING_FRC_OP == eOSDBlendingPoint)
        {
            if(pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA  && pXCResourcePrivate->stdrvXC_3D._bFAToTB)
            {
                if(E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW == pXCResourcePrivate->stdrvXC_3D._eOutput3DMode)
                {
                    *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW;
                }
                else
                {
                    *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR;
                }
            }
            else
            {
                *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode;
            }
            bRet = TRUE;
        }
        else
        {
            *((E_XC_3D_OUTPUT_MODE*)p3DFormat) = E_XC_3D_OUTPUT_MODE_NONE;
            bRet = FALSE;
        }
    }
#endif
    return bRet;
}

MS_BOOL MApi_XC_Get3DFormat_U2(void* pInstance, E_XC_3D_ATTRIBUTE_TYPE e3DAttrType, void* para, void* p3DFormat)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bRet = MDrv_XC_Get3DFormat(pInstance, e3DAttrType, para, p3DFormat);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bRet;
}

MS_BOOL MApi_XC_Get3DFormat(E_XC_3D_ATTRIBUTE_TYPE e3DAttrType, void* para, void* p3DFormat)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET3DFORMAT XCArgs;
    XCArgs.e3DAttrType = e3DAttrType;
    XCArgs.para = para;
    XCArgs.p3DFormat = p3DFormat;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET3DFORMAT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("\n %s : %d, Obtain XC engine fail \n ",__FUNCTION__,__LINE__);
        return E_XC_3D_OUTPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}
#endif
//-------------------------------------------------------------------------------------------------
/// This API return 3D panel type
//-------------------------------------------------------------------------------------------------
E_XC_3D_PANEL_TYPE MDrv_XC_Get_3D_Panel_Type(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_3D._e3DPanelType;
}

E_XC_3D_PANEL_TYPE MApi_XC_Get_3D_Panel_Type_U2(void* pInstance)
{
#ifndef DISABLE_3D_FUNCTION
    E_XC_3D_PANEL_TYPE eType = E_XC_3D_PANEL_NONE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    eType = MDrv_XC_Get_3D_Panel_Type(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eType;
#else
    return E_XC_3D_PANEL_NONE;
#endif
}

E_XC_3D_PANEL_TYPE MApi_XC_Get_3D_Panel_Type(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_XC_3D_PANEL_MAX;
    }

    stXC_GET_3D_PANELTYPE XCArgs;
    XCArgs.eReturnValue = E_XC_3D_PANEL_MAX;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_PANELTYPE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_PANEL_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// do Ip Sync to make main and sub window same relation to OP, thus framelock could be applied to
/// both main and sub window
/// @return MS_BOOL                \b OUT: TRUE:Success FALSE:Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_3DMainSub_IPSync_U2(void* pInstance)
{
#ifndef DISABLE_3D_FUNCTION
    _XC_ENTRY(pInstance);
    HAL_SC_ip_3DMainSub_IPSync(pInstance);
    _XC_RETURN(pInstance);
    return TRUE;
#else
    return FALSE;
#endif
}

MS_BOOL MApi_XC_3DMainSub_IPSync(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_3D_MAINSUB_IPSYNC XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_3D_MAINSUB_IPSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// When we do output: line_alter, we can decide the odd line coming from which part of the input
/// (left or right, up or down, first frame or second frame, etc)
/// @param  bMainFirst                \b IN: True: Main first; False: Sub first
/// @return MS_BOOL                   \b OUT: TRUE:Success FALSE:Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_MainWin_FirstMode_U2(void* pInstance, MS_BOOL bMainFirst)
{
#ifndef DISABLE_3D_FUNCTION
    //True: Main first; False: Sub first
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_3D._b3DMainFirst = bMainFirst;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange main-sub win sequence! (%u)\n", bMainFirst);
    return TRUE;
#else
    UNUSED(bMainFirst);
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Set_3D_MainWin_FirstMode(MS_BOOL bMainFirst)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_MAINWIN_FIRST XCArgs;
    XCArgs.bMainFirst = bMainFirst;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_MAINWIN_FIRST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
//-------------------------------------------------------------------------------------------------
/// the API return the relation that odd line is coming from which part of the input content.
/// @param  bMainFirst                \b IN: True: Main first; False: Sub first
/// @return MS_BOOL                   \b OUT: TRUE:Success FALSE:Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Get_3D_MainWin_First_U2(void* pInstance)
{
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->stdrvXC_3D._b3DMainFirst;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
#else
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Get_3D_MainWin_First(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_3D_MAINWIN_FIRST XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_MAINWIN_FIRST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Some mm file maybe have unofficial format,
/// eg.
/// --------------------------
/// |     black_L            |
/// --------------------------
/// |                        |
/// |          L             |
/// |                        |
/// --------------------------
/// |                        |
/// |          R             |
/// |                        |
/// --------------------------
/// |      black+R           |
/// --------------------------
/// this format cannot direct convert to 3D output. Because black_L will cover with R
/// so we define the black area height, then handle it in 3D process.
/// But the height must be told by AP layer
/// @param  bMainFirst                \b IN: True: Main first; False: Sub first
/// @return MS_BOOL                   \b OUT: TRUE:Success FALSE:Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_VerVideoOffset_U2(void* pInstance, MS_U16 u163DVerVideoOffset)
{
    printf("MApi_XC_Set_3D_VerVideoOffset:this function is obsolete\n");
    return TRUE;
}

MS_BOOL MApi_XC_Set_3D_VerVideoOffset(MS_U16 u163DVerVideoOffset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_VERTICALVIDEO_OFFSET XCArgs;
    XCArgs.u163DVerVideoOffset = u163DVerVideoOffset;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_VERTICALVIDEO_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This API return Top Bottom's black area height
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_Get_3D_VerVideoOffset_U2(void* pInstance)
{
    printf("MApi_XC_Get_3D_VerVideoOffset:this function is obsolete\n");
    return 0;
}

MS_U16 MApi_XC_Get_3D_VerVideoOffset(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_3D_VERTICALVIDEO_OFFSET XCArgs;
    XCArgs.u16ReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_VERTICALVIDEO_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

#ifndef DISABLE_3D_FUNCTION

MS_BOOL MDrv_XC_Set_3D_LR_Frame_Exchg_burst(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

#if (HW_DESIGN_3D_VER < 2)

    if((IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
#if (HW_DESIGN_3D_VER == 0)
        || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
#endif
        )
    {
        MS_PHY u32Temp1 = 0, u32Temp2 = 0;
        //exchange the ipm base0 and ipm base1

        _MLOAD_ENTRY(pInstance);
        u32Temp1 = Hal_SC_Get_DNRBase0(pInstance, MAIN_WINDOW);
        u32Temp2 = Hal_SC_Get_DNRBase1(pInstance, MAIN_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_08_L, (MS_U16)(u32Temp2>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_09_L, (MS_U16)(u32Temp2>>16), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_0A_L, (MS_U16)(u32Temp1>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_0B_L, (MS_U16)(u32Temp1>>16), 0xFFFF);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange dnr base\n");
    }
    else if(((IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                  || IS_INPUT_LINE_ALTERNATIVE(eWindow))
               && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())//old way
             || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()) //old way
             || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
            )
    {
        MS_PHY u32Temp1 = 0, u32Temp2 = 0;
        //exchange the dnr base0 of main & sub

        _MLOAD_ENTRY(pInstance);
        u32Temp1 = Hal_SC_Get_OPMBase0(MAIN_WINDOW);
        u32Temp2 = Hal_SC_Get_OPMBase1(MAIN_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_10_L, (MS_U16)(u32Temp2>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_11_L, (MS_U16)(u32Temp2>>16), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_12_L, (MS_U16)(u32Temp1>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_13_L, (MS_U16)(u32Temp1>>16), 0xFFFF);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange opm base\n");
    }
    else if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()))
    {
        MS_U16 u16Temp = MOD_R2BYTE(REG_MOD_BK01_78_L);
        u16Temp = u16Temp^BIT(15);
#ifdef SUPPORT_MOD_ADBANK_SEPARATE

#else
        MOD_W2BYTEMSK(REG_MOD_BK01_78_L, u16Temp, BIT(15));
#endif
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange 3d ver1's L/R flag\n");
    }
    else if(IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        MS_PHY u32Temp1 = 0, u32Temp2 = 0;
        //exchange the dnr base0 of main & sub

        _MLOAD_ENTRY(pInstance);
        u32Temp1 = Hal_SC_Get_DNRBase0(pInstance, MAIN_WINDOW);
        u32Temp2 = Hal_SC_Get_DNRBase0(pInstance, SUB_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_08_L, (MS_U16)(u32Temp2>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_09_L, (MS_U16)(u32Temp2>>16), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_48_L, (MS_U16)(u32Temp1>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_49_L, (MS_U16)(u32Temp1>>16), 0xFFFF);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange main/sub dnr base0\n");
    }
    else if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
            || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        MS_PHY u32Temp1 = 0, u32Temp2 = 0;
        MS_U16 u16Temp = 0;
        //exchange the opm base0/base1 of main & sub

        _MLOAD_ENTRY(pInstance);
        u32Temp1 = Hal_SC_Get_OPMBase0(MAIN_WINDOW);
        u32Temp2 = Hal_SC_Get_OPMBase0(SUB_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_10_L, (MS_U16)(u32Temp2>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_11_L, (MS_U16)(u32Temp2>>16), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_50_L, (MS_U16)(u32Temp1>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_51_L, (MS_U16)(u32Temp1>>16), 0xFFFF);

        u32Temp1 = Hal_SC_Get_OPMBase1(MAIN_WINDOW);
        u32Temp2 = Hal_SC_Get_OPMBase1(SUB_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_12_L, (MS_U16)(u32Temp2>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_13_L, (MS_U16)(u32Temp2>>16), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_52_L, (MS_U16)(u32Temp1>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_53_L, (MS_U16)(u32Temp1>>16), 0xFFFF);

        u32Temp1 = Hal_SC_Get_OPMBase2(MAIN_WINDOW);
        u32Temp2 = Hal_SC_Get_OPMBase2(SUB_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_14_L, (MS_U16)(u32Temp2>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_15_L, (MS_U16)(u32Temp2>>16), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_54_L, (MS_U16)(u32Temp1>>00), 0xFFFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_55_L, (MS_U16)(u32Temp1>>16), 0xFFFF);

        u16Temp = Hal_SC_get_opm_fetch(MAIN_WINDOW);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_17_L, Hal_SC_get_opm_fetch(SUB_WINDOW), 0x1FFF);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_57_L, u16Temp, 0x1FFF);

        u16Temp = (Hal_SC_Get_LBOffset(MAIN_WINDOW)<<8)|(Hal_SC_Get_LBOffset(SUB_WINDOW));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1D_L, u16Temp , 0xFFFF);

        if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
        {
            MS_U16 u16MainRWBank, u16SubRWBank;
            u16MainRWBank = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L);
            u16SubRWBank = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L);

            u16MainRWBank = u16MainRWBank^BIT(14);
            u16SubRWBank = u16SubRWBank^BIT(14);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_07_L, u16MainRWBank, BIT(14));
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_47_L, u16SubRWBank, BIT(14));
        }

        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
        bRet = TRUE;
    }
#elif (HW_DESIGN_3D_VER >= 2)
    if((IS_INPUT_NORMAL_2D(eWindow) || IS_INPUT_NORMAL_2D_INTERLACE(eWindow)) &&
        IS_OUTPUT_LINE_ALTERNATIVE())
    {
        if(pXCResourcePrivate->stdrvXC_3D._u163DHShift & 0xFF)
        {
            pXCResourcePrivate->stdrvXC_3D._u163DHShift <<= 8; //Switch shift to sub window
        }
        else
        {
            pXCResourcePrivate->stdrvXC_3D._u163DHShift >>= 8; //Switch shift to Main window
        }
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_1C_L, pXCResourcePrivate->stdrvXC_3D._u163DHShift, 0xFFFF);//Main/sub display window offset in right direction
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
        bRet = TRUE;
    }
    else if(!(IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE())
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW()))
#if(HW_DESIGN_3D_VER >= 4)
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_PACKING))
#endif
            )
    {
#if(HW_DESIGN_3D_VER >= 5)
        MS_U16 u16FRCM_W,u16FRCM_R;
        u16FRCM_W = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L);
        u16FRCM_R = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L);
        if(u16FRCM_R & BIT(7))
        {
            //Recommend to use OP L/R switch as designer's advice
            u16FRCM_R = u16FRCM_R^BIT(8);
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK32_33_L, u16FRCM_R, BIT(8));//F2 OPM 3D LR invert
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
            bRet = TRUE;
        }
        else if(u16FRCM_W & BIT(0))
        {
            u16FRCM_W = u16FRCM_W^BIT(4);
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK32_30_L, u16FRCM_R, BIT(4));//F2 IPM 3D LR invert
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
            bRet = TRUE;
        }
        else
        {
            printf("----------------3D L/R switch: UnSupported format I: %u->O: %u\n", pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow], pXCResourcePrivate->stdrvXC_3D._eOutput3DMode);
        }
#else
        MS_U16 u16IPM, u16OPM;
        u16IPM = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L);
        u16OPM = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L);
        if(u16OPM & BIT(7))
        {
            //Recommend to use OP L/R switch as designer's advice
            u16OPM = u16OPM^BIT(8);
            _MLOAD_ENTRY(pInstance);

            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_33_L, u16OPM, BIT(8));//F2 OPM 3D LR invert

            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
            bRet = TRUE;
        }
        else if(u16IPM & BIT(0))
        {
            u16IPM = u16IPM^BIT(4);
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_30_L, u16IPM, BIT(4));//F2 IPM 3D LR invert
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
            bRet = TRUE;
        }
        else
        {
            printf("----------------3D L/R switch: UnSupported format I: %u->O: %u\n", pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow], pXCResourcePrivate->stdrvXC_3D._eOutput3DMode);
        }
#endif
    }
#endif

#if HW_2DTO3D_SUPPORT
    if((IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE_HW())
#if (HW_2DTO3D_VER >= 4)
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_PACKING())
#endif
       )
    {
#if (HW_2DTO3D_VER >= 3)
        MS_U16 u16LeftGain = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L);
        MS_U16 u16RightGain = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L);
        u16LeftGain = u16LeftGain^BIT(15);
        u16RightGain = u16RightGain^BIT(15);
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK62_06_L, u16LeftGain, BIT(15));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK62_0E_L, u16RightGain, BIT(15));
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
        bRet = TRUE;
#else
        MS_U16 u16OutputFormat = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0007);
        if(0x0001 == u16OutputFormat)
        {
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK15_67_L, 0x0004, 0x0007);
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
            bRet = TRUE;
        }
        else if(0x0004 == u16OutputFormat)
        {
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK15_67_L, 0x0001, 0x0007);
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
            bRet = TRUE;
        }
        else
        {
            printf("----------------3D L/R switch: UnSupported hw 2d to 3d output format\n");
        }
#endif
    }
    else if(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW())
    {
    #ifdef SUPPORT_MOD_ADBANK_SEPARATE

    #else
        MS_U16 u16Temp = MOD_R2BYTE(REG_MOD_BK01_78_L);
        u16Temp = u16Temp^BIT(15);
        MOD_W2BYTEMSK(REG_MOD_BK01_78_L, u16Temp, BIT(15));
    #endif
        bRet = TRUE;
    }
#endif
    return bRet;
}

MS_BOOL MDrv_XC_Set_3D_LR_Frame_Exchg(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_ASSERT(eWindow < MAX_WINDOW);

    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

#if (HW_DESIGN_3D_VER < 2)

    if((IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
#if (HW_DESIGN_3D_VER == 0)
        || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
#endif
        )
    {
        MS_PHY u32Temp = 0;
        //exchange the ipm base0 and ipm base1
        u32Temp = Hal_SC_Get_DNRBase0(pInstance, MAIN_WINDOW) * BYTE_PER_WORD;
        Hal_SC_set_DNRBase0(pInstance, Hal_SC_Get_DNRBase1(pInstance, MAIN_WINDOW) * BYTE_PER_WORD, MAIN_WINDOW);
        Hal_SC_set_DNRBase1(pInstance, u32Temp, MAIN_WINDOW);
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange dnr base\n");
    }
    else if(((IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                  || IS_INPUT_LINE_ALTERNATIVE(eWindow))
               && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())//old way
             || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()) //old way
             || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
            )
    {
        MS_PHY u32Temp = 0;
        u32Temp = Hal_SC_Get_OPMBase0(MAIN_WINDOW) * BYTE_PER_WORD;
        Hal_SC_set_OPMBase0(pInstance, Hal_SC_Get_OPMBase1(pInstance, MAIN_WINDOW) * BYTE_PER_WORD, MAIN_WINDOW);
        Hal_SC_set_OPMBase1(pInstance, u32Temp, MAIN_WINDOW);
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange opm base\n");
    }
    else if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()))
    {
    #ifdef SUPPORT_MOD_ADBANK_SEPARATE

    #else
        MS_U16 u16Temp = MOD_R2BYTE(REG_MOD_BK01_78_L);
        u16Temp = u16Temp^BIT(15);
        MOD_W2BYTEMSK(REG_MOD_BK01_78_L, u16Temp, BIT(15));
    #endif
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange 3d ver1's L/R flag\n");
    }
    else if(IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        MS_U32 u32Temp = 0;
        //exchange the dnr base0 of main & sub
        u32Temp = Hal_SC_Get_DNRBase0(pInstance, MAIN_WINDOW) * BYTE_PER_WORD;
        Hal_SC_set_DNRBase0(pInstance, Hal_SC_Get_DNRBase0(pInstance, SUB_WINDOW) * BYTE_PER_WORD, MAIN_WINDOW);
        Hal_SC_set_DNRBase0(pInstance, u32Temp, SUB_WINDOW);
        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange main/sub dnr base0\n");

    }
    else if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
            || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        MS_PHY u32Temp = 0;
        //exchange the opm base0/base1 of main & sub
        u32Temp = Hal_SC_Get_OPMBase0(pInstance, MAIN_WINDOW) * BYTE_PER_WORD;
        Hal_SC_set_OPMBase0(pInstance, Hal_SC_Get_OPMBase0(pInstance, SUB_WINDOW) * BYTE_PER_WORD, MAIN_WINDOW);
        Hal_SC_set_OPMBase0(pInstance, u32Temp, SUB_WINDOW);

        u32Temp = Hal_SC_Get_OPMBase1(pInstance, MAIN_WINDOW) * BYTE_PER_WORD;
        Hal_SC_set_OPMBase1(pInstance, Hal_SC_Get_OPMBase1(pInstance, SUB_WINDOW) * BYTE_PER_WORD, MAIN_WINDOW);
        Hal_SC_set_OPMBase1(pInstance, u32Temp, SUB_WINDOW);

        u32Temp = Hal_SC_Get_OPMBase2(pInstance, MAIN_WINDOW) * BYTE_PER_WORD;
        Hal_SC_set_OPMBase2(pInstance, Hal_SC_Get_OPMBase2(pInstance, SUB_WINDOW) * BYTE_PER_WORD, MAIN_WINDOW);
        Hal_SC_set_OPMBase2(pInstance, u32Temp, SUB_WINDOW);

        u32Temp = Hal_SC_get_opm_fetch(pInstance, MAIN_WINDOW);
        Hal_SC_set_opm_fetch(pInstance, MAIN_WINDOW, Hal_SC_get_opm_fetch(pInstance, SUB_WINDOW));
        Hal_SC_set_opm_fetch(pInstance, SUB_WINDOW, u32Temp);

        u32Temp = Hal_SC_Get_LBOffset(pInstance, MAIN_WINDOW);
        Hal_SC_Set_LBOffset(pInstance, Hal_SC_Get_LBOffset(pInstance, SUB_WINDOW), MAIN_WINDOW);
        Hal_SC_Set_LBOffset(pInstance, u32Temp, SUB_WINDOW);

        if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
        {
            MS_U16 u16MainRWBank, u16SubRWBank;
            u16MainRWBank = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L);
            u16SubRWBank = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L);

            u16MainRWBank = u16MainRWBank^BIT(14);
            u16SubRWBank = u16SubRWBank^BIT(14);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, u16MainRWBank, BIT(14));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, u16SubRWBank, BIT(14));
        }

        bRet = TRUE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: exchange main/sub opm base\n");
    }
#elif (HW_DESIGN_3D_VER >= 2)
    if((IS_INPUT_NORMAL_2D(eWindow) || IS_INPUT_NORMAL_2D_INTERLACE(eWindow)) &&
        (!IS_OUTPUT_FRAME_ALTERNATIVE()))
    {
        if(pXCResourcePrivate->stdrvXC_3D._u163DHShift & 0xFF)
        {
            pXCResourcePrivate->stdrvXC_3D._u163DHShift <<= 8; //Switch shift to sub window
        }
        else
        {
            pXCResourcePrivate->stdrvXC_3D._u163DHShift >>= 8; //Switch shift to Main window
        }
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, pXCResourcePrivate->stdrvXC_3D._u163DHShift);//Main/sub display window offset in right direction
        bRet = TRUE;
    }
    else if(!(IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE())
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW()))
#if(HW_DESIGN_3D_VER >= 4)
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_PACKING()))
#endif
            )
    {
#if(HW_DESIGN_3D_VER >= 5)
        MS_U16 u16FRCM_W,u16FRCM_R;
        u16FRCM_W = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L);
        u16FRCM_R = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L);
        if(u16FRCM_R & BIT(7))
        {
            //Recommend to use OP L/R switch as designer's advice
            u16FRCM_R = u16FRCM_R^BIT(8);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L, u16FRCM_R, BIT(8));//F2 OPM 3D LR invert
            bRet = TRUE;
        }
        else if(u16FRCM_W & BIT(0))
        {
            u16FRCM_W = u16FRCM_W^BIT(4);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L, u16FRCM_R, BIT(4));//F2 IPM 3D LR invert
            bRet = TRUE;
        }
        else
        {
            printf("----------------3D L/R switch: UnSupported format I: %u->O: %u\n"
                   , pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow]
                   , pXCResourcePrivate->stdrvXC_3D._eOutput3DMode);
        }
#else
        MS_U16 u16IPM, u16OPM;
        u16IPM = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L);
        u16OPM = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L);
        if(u16OPM & BIT(7))
        {
            //Recommend to use OP L/R switch as designer's advice
            u16OPM = u16OPM^BIT(8);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, u16OPM, BIT(8));//F2 OPM 3D LR invert
            bRet = TRUE;
        }
        else if(u16IPM & BIT(0))
        {
            u16IPM = u16IPM^BIT(4);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, u16IPM, BIT(4));//F2 IPM 3D LR invert
            bRet = TRUE;
        }
        else
        {
            printf("----------------3D L/R switch: UnSupported format I: %u->O: %u\n"
                   , pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow]
                   , pXCResourcePrivate->stdrvXC_3D._eOutput3DMode);
        }
#endif
    }
#endif

#if HW_2DTO3D_SUPPORT
    if((IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE_HW())
#if (HW_2DTO3D_VER >= 4)
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_PACKING())
#endif
        )
    {
#if (HW_2DTO3D_VER >= 3)
        MS_U16 u16LeftGain = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L);
        MS_U16 u16RightGain = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L);
        u16LeftGain = u16LeftGain^BIT(15);
        u16RightGain = u16RightGain^BIT(15);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, u16LeftGain, BIT(15));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, u16RightGain, BIT(15));
        bRet = TRUE;
#else
        MS_U16 u16OutputFormat = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0007);
        if(0x0001 == u16OutputFormat)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0004, 0x0007);
            bRet = TRUE;
        }
        else if(0x0004 == u16OutputFormat)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0001, 0x0007);
            bRet = TRUE;
        }
        else
        {
            printf("----------------3D L/R switch: UnSupported hw 2d to 3d output format\n");
        }
#endif
    }
    else if(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW())
    {
    #ifdef SUPPORT_MOD_ADBANK_SEPARATE

    #else
        MS_U16 u16Temp = MOD_R2BYTE(REG_MOD_BK01_78_L);
        u16Temp = u16Temp^BIT(15);
        MOD_W2BYTEMSK(REG_MOD_BK01_78_L, u16Temp, BIT(15));
    #endif
        bRet = TRUE;
    }
#endif

    return bRet;
}

MS_BOOL MDrv_XC_3D_Is_LR_Frame_Exchged(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;

    MS_ASSERT(eWindow < MAX_WINDOW);
    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER < 2)
    if((IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
#if (HW_DESIGN_3D_VER == 0)
        || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_SIDE_BY_SIDE_HALF())
#endif
        )
    {
        bRet = (Hal_SC_Get_DNRBase0(pInstance, MAIN_WINDOW) > Hal_SC_Get_DNRBase1(pInstance, MAIN_WINDOW));
        bRet ^= IsVMirrorMode(eWindow);
    }
    else if(((IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                  || IS_INPUT_LINE_ALTERNATIVE(eWindow))
               && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())//old way
             || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()) //old way
             || (IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_NOFRC())
             || (IS_INPUT_FIELD_ALTERNATIVE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
            )
    {
        bRet = (Hal_SC_Get_OPMBase0(MAIN_WINDOW) > Hal_SC_Get_OPMBase1(MAIN_WINDOW));
        bRet ^= IsVMirrorMode(eWindow);
    }
    else if((IS_INPUT_FRAME_PACKING(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_LINE_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_FRAME_ALTERNATIVE(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE())
             || (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE()))
    {
#if ENABLE_REGISTER_SPREAD
        bRet = (PWM_R2BYTEMSK(REG_MOD_BK01_78_L, BIT(15)) == BIT(15));
#else
        bRet = (MOD_R2BYTEMSK(REG_MOD_BK01_78_L, BIT(15)) == BIT(15));
#endif
        bRet ^= IsVMirrorMode(eWindow);
    }
    else if(IS_INPUT_FRAME_PACKING_OPT(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        bRet = (Hal_SC_Get_DNRBase0(pInstance, MAIN_WINDOW) > Hal_SC_Get_DNRBase0(pInstance, SUB_WINDOW));
        bRet ^= IsVMirrorMode(eWindow);
    }
    else if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_SIDE_BY_SIDE_HALF()
            || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        bRet = (Hal_SC_Get_OPMBase0(MAIN_WINDOW) > Hal_SC_Get_OPMBase0(SUB_WINDOW));
        bRet ^= IsVMirrorMode(eWindow);
    }
#elif (HW_DESIGN_3D_VER >= 2)
    if((IS_INPUT_NORMAL_2D(eWindow) || IS_INPUT_NORMAL_2D_INTERLACE(eWindow)) &&
        (!IS_OUTPUT_FRAME_ALTERNATIVE()))
    {
        bRet = ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, 0xFF00) >> 8) > 0);
    }
    else if(!(IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE())
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE_HW()))
            && (!(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW()))
            )
    {
#if(HW_DESIGN_3D_VER >= 5)
        MS_U16 u16FRCM_W,u16FRCM_R;
        u16FRCM_W = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L);
        u16FRCM_R = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L);
        if(u16FRCM_R & BIT(7))
        {
            bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L, BIT(8)) == BIT(8));
        }
        else if(u16FRCM_W & BIT(0))
        {
            bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L, BIT(4)) == BIT(4));
        }
#else
        MS_U16 u16IPM, u16OPM;
        u16IPM = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L);
        u16OPM = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L);
        if(u16OPM & BIT(7))
        {
            bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(8)) == BIT(8));
        }
        else if(u16IPM & BIT(0))
        {
            bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(4)) == BIT(4));
        }
#endif
        // if mvop mirror, main/sub window will get switched frame, so need do LR exchange
        // 1 field alternative and frame alternative input don't need do this, since main/sub
        //   window still get the original frame;
        // 2 side by side/top bottom outpu also don't need do this, because main/sub window
        //   will assign the specified display window
        if(IS_OUTPUT_LINE_ALTERNATIVE()
           && (!IS_INPUT_FIELD_ALTERNATIVE(eWindow))
           && (!IS_INPUT_FRAME_ALTERNATIVE(eWindow)))
        {
            bRet ^= (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1)));
        }
    }
#endif

#if HW_2DTO3D_SUPPORT
    if((IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE_HW())
#if(HW_2DTO3D_VER >= 4)
     || (IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_TOP_BOTTOM_HW())
#endif
        )
    {
#if (HW_2DTO3D_VER >= 3)
        bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, BIT(15)) == BIT(15));
        bRet ^= (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))));
#else
        bRet = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0007) == 0x0001);
        bRet ^= (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))));
#endif
    }
    else if(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_ALTERNATIVE_HW())
    {
#if ENABLE_REGISTER_SPREAD
        bRet = (PWM_R2BYTEMSK(REG_MOD_BK01_78_L, BIT(15)) == BIT(15));
#else
        bRet = (MOD_R2BYTEMSK(REG_MOD_BK01_78_L, BIT(15)) == BIT(15));
#endif
    }
#endif

    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Is Support HW Depth Adjustment For Real 3d Or Not
/// @param  e3dInputMode                 \b IN: input  3d format
/// @param  e3dOutputMode                \b IN: output 3d format
/// @param  eWindow                      \b IN: which window
/// @return MS_BOOL                      \b OUT: TRUE: support FALSE: not support
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_3D_IsHWDepthAdjSupported(void *pInstance, E_XC_3D_INPUT_MODE en3dInputMode,
                                         E_XC_3D_OUTPUT_MODE en3dOutputMode,
                                         SCALER_WIN enWindow)
{
    MS_ASSERT(enWindow < MAX_WINDOW);

    if(enWindow >= MAX_WINDOW)
    {
        return FALSE;
    }

#if 0 //may support when top bottom/side by side output later
    return (MDrv_XC_Get_3D_IsSupportedHW2DTo3D()
            && (E_XC_3D_INPUT_MODE_NONE != en3dInputMode)
            && (E_XC_3D_INPUT_NORMAL_2D != en3dInputMode)
            && ((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == en3dOutputMode)
                || (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == en3dOutputMode)
                || (E_XC_3D_OUTPUT_TOP_BOTTOM == en3dOutputMode)
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == en3dOutputMode)));
#else
    return (MDrv_XC_Get_3D_IsSupportedHW2DTo3D()
            && (E_XC_3D_INPUT_MODE_NONE != en3dInputMode)
            && (E_XC_3D_INPUT_NORMAL_2D != en3dInputMode)
            && ((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == en3dOutputMode)
#if(HW_2DTO3D_VER >= 4)
                || (E_XC_3D_OUTPUT_TOP_BOTTOM == en3dOutputMode)
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == en3dOutputMode)
                || (E_XC_3D_OUTPUT_FRAME_PACKING== en3dOutputMode)
#endif
                || ((E_XC_3D_OUTPUT_TOP_BOTTOM == en3dOutputMode) && MDrv_SC_3D_Is_LR_Sbs2Line(pInstance))//3d ver1 line alternative output
                || (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == en3dOutputMode)));
#endif
}

#if HW_2DTO3D_SUPPORT
#if (HW_2DTO3D_VER >= 4)
static void _MDrv_SC_3D_HWDepthAdj_TBOut(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{

    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }

    //Load T3d setting (same as 2d3d, in regardless of DD, and RGBLUT SRAM)
    //Get panel size output width
    MS_U32 u32OutputWidth = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L) + 1;
    MS_U32 PanelHeight= SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L) + 1;
    if(u32OutputWidth==0)
    {
        u32OutputWidth = 0xf00;
    }
    if(PanelHeight==0)
    {
        PanelHeight = 0x870;
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4A_L, BIT(10),  BIT(10)); //disable YUV to RGB
    MDrv_XC_wait_output_vsync(pInstance, 1, 100, eWindow);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, (u32OutputWidth+31)/32, 0x00FF);//horizontal number of 32x32 block
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, BIT(8), BIT(8));//eco en
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_57_L, BIT(1), BIT(1));//dr miu eco en
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_42_L, BIT(7), BIT(7));//reg_clrmiurdy_n
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_48_L, BIT(2), BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6));//reg_blk_dpthptr_en,reg_blk_dpthptr_sel

    if(u32OutputWidth==0xf00)
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, 0x0808); //reg_dpcach_req_ctr,reg_dpcach_last_ctr
    else
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, 0x0404); //reg_dpcach_req_ctr,reg_dpcach_last_ctr

    //fill sram
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0xFFFF);//reg_fill_in
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, BIT(7), BIT(7));//reg_fill_w

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0000, 0x0F00);//reg_fill_add 0
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0100, 0x0F00);//reg_fill_add 1
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0200, 0x0F00);//reg_fill_add 2
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0300, 0x0F00);//reg_fill_add 3
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0400, 0x0F00);//reg_fill_add 4
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0500, 0x0F00);//reg_fill_add 5
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0600, 0x0F00);//reg_fill_add 6
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0700, 0x0F00);//reg_fill_add 7
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0800, 0x0F00);//reg_fill_add 8
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0900, 0x0F00);//reg_fill_add 9
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0A00, 0x0F00);//reg_fill_add 10
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0B00, 0x0F00);//reg_fill_add 11
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0C00, 0x0F00);//reg_fill_add 12
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0D00, 0x0F00);//reg_fill_add 13
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0E00, 0x0F00);//reg_fill_add 14
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0F00, 0x0F00);//reg_fill_add 15
    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x80808080);//reg_fill_sram_1,reg_fill_sram_2

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x0000);//reg_fill_in
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x00, BIT(7));//reg_fill_w

    //dr pixel
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_60_L, 0x07, 0x3FFF);//reg_pipe_guard_cycle_pb
    if((pSrcInfo->stDispWin.width > (WIDTH_4K2KTIMING - OFFSET_4K2KTIMING))
       && (pSrcInfo->stDispWin.width < (WIDTH_4K2KTIMING + OFFSET_4K2KTIMING)))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, BIT(4)|BIT(5), BIT(4)|BIT(5));//dd h 2:1 scaling down
    }
    if((pSrcInfo->stDispWin.height > (HEIGHT_4K2KTIMING - OFFSET_4K2KTIMING))
        && (pSrcInfo->stDispWin.height < (HEIGHT_4K2KTIMING + OFFSET_4K2KTIMING)))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, BIT(0), BIT(0));//dd v 2:1 scaling down
    }
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_68_L, BIT(0)|BIT(2), BIT(0)|BIT(1)|BIT(2));//reg_render_control,reg_lr_control_l,reg_lr_control_r
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_08_L, BIT(5), BIT(5));//rgb_to_y
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_67_L, 0x8080);//reg_pixelsub_l,reg_pixelsub_r
    if(u32OutputWidth==0xf00)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(4), BIT(4));//DD v-sacling dn mode
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(7), BIT(7));//TB mode
}

static void _MDrv_SC_3D_DD_setting(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U16 u16DDHSize = 0; //DD H size
    MS_U16 u16DDVSize = 0; //DD V size
    MS_U16 u16PanelWidth = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
    MS_U16 u16PanelHeight = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    MS_BOOL bT3DDE_Use_MainDisplayWindow = _MDrv_Is_T3DDE_UseMainDisplayWindow(pInstance);
    //T3D 2P engine, get two piexl a time,start must be even,end must be odd
    MS_U16 u16MainDispWidth = _MDrv_SC_3D_GetDispWinWidthForT3D(pInstance);
    MS_U16 u16MainDispHeight = gSrcInfo[eWindow].stDispWin.height;
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, eWindow, &stDispWin);
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_MODE_NONE)
    {
        u16MainDispHeight = stDispWin.height;
    }

    if(bT3DDE_Use_MainDisplayWindow)
    {
        u16DDHSize = MIN(u16MainDispWidth>>1, 1920);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, u16DDHSize, 0x07FF);//src width
        if(u16MainDispHeight >= 1080)
        {
            u16DDVSize =  MIN(u16MainDispHeight>>1, 1080);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(4), BIT(4));//DD v-sacling dn mode
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, u16DDVSize, 0x0FFF);//src height
        }
        else
        {
            u16DDVSize = MIN(u16MainDispHeight, 1080);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, 0, BIT(4));//DD v-sacling dn mode
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, u16DDVSize, 0x0FFF);//src height
        }
    }
    else
    {
        u16DDHSize = MIN(u16PanelWidth>>1, 1920);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, u16DDHSize, 0x07FF);//src width
        if(u16PanelHeight >= HEIGHT_4K2KTIMING)
        {
            u16DDVSize = u16PanelHeight>>1;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(4), BIT(4));//DD v-sacling dn mode
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, u16DDVSize, 0x0FFF);//src height
        }
        else
        {
            u16DDVSize = MIN(u16PanelHeight, 1080);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, 0, BIT(4));//DD v-sacling dn mode
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, u16DDVSize, 0x0FFF);//src height
        }
    }

    //save DD size
    pXCResourcePrivate->stdrvXC_3D._u16DDHSize = u16DDHSize;
    pXCResourcePrivate->stdrvXC_3D._u16DDVSize = u16DDVSize;
}
#endif

static void _MDrv_SC_3D_DR_setting(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //T3D 2P engine, get two piexl a time,start must be even,end must be odd
    MS_U16 u16MainDispWidth = _MDrv_SC_3D_GetDispWinWidthForT3D(pInstance);
    MS_U16 u16MainDispHeight = gSrcInfo[MAIN_WINDOW].stDispWin.height;
    MS_U16 u16PanelWidth = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
    MS_U16 u16PanelHeight = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    MS_BOOL bT3DDE_Use_MainDisplayWindow = _MDrv_Is_T3DDE_UseMainDisplayWindow(pInstance);
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, eWindow, &stDispWin);
    if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] == E_XC_3D_INPUT_MODE_NONE)
    {
        u16MainDispHeight = stDispWin.height;
    }

    if(bT3DDE_Use_MainDisplayWindow)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_58_L, BIT(0), BIT(0));//Use MainDE
        if(IS_OUTPUT_SIDE_BY_SIDE_HALF() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            //DR V size needs be 4-align.
            u16MainDispHeight = (u16MainDispHeight + 1) & ~0x01;

            //dr bilinear size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, u16MainDispWidth / 2, 0x1FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, u16MainDispHeight * 2, 0x1FFF);//reg_col_height
            //dr pixel size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, u16MainDispWidth / 2, 0x3FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, u16MainDispHeight * 2, 0x1FFF);//reg_col_height
            //dr block size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, u16MainDispWidth / 2, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_66_L, u16MainDispHeight * 2, 0x1FFF);//reg_col_height
        }
        else
        {
            //DR V size needs be 4-align.
            u16MainDispHeight = (u16MainDispHeight + 3) & ~0x03;

            //dr bilinear size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, u16MainDispWidth, 0x1FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, u16MainDispHeight, 0x1FFF);//reg_col_height
            //dr pixel size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, u16MainDispWidth, 0x3FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, u16MainDispHeight, 0x1FFF);//reg_col_height
            //dr block size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, u16MainDispWidth, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_66_L, u16MainDispHeight, 0x1FFF);//reg_col_height
        }
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_58_L, 0x0, BIT(0));//Full-frame use Full-dispDE
        if(IS_OUTPUT_SIDE_BY_SIDE_HALF() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            //DR V size needs be 4-align.
            u16PanelHeight = (u16PanelHeight + 1) & ~0x01;

            //dr bilinear size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, u16PanelWidth / 2, 0x1FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, u16PanelHeight * 2, 0x1FFF);//reg_col_height
            //dr pixel size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, u16PanelWidth / 2, 0x3FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, u16PanelHeight * 2, 0x1FFF);//reg_col_height
            //dr block size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, u16PanelWidth / 2, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_66_L, u16PanelHeight * 2, 0x1FFF);//reg_col_height
        }
        else
        {
            //DR V size needs be 4-align.
            u16PanelHeight = (u16PanelHeight + 3) & ~0x03;

            //dr bilinear size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, u16PanelWidth, 0x1FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, u16PanelHeight, 0x1FFF);//reg_col_height
            //dr pixel size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, u16PanelWidth, 0x3FFF);//reg_ln_width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, u16PanelHeight, 0x1FFF);//reg_col_height
            //dr block size
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, u16PanelWidth, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_66_L, u16PanelHeight, 0x1FFF);//reg_col_height
        }
    }
}

#if (HW_2DTO3D_VER >= 3)
static void _MDrv_SC_3D_Load2DTo3DReg(void *pInstance, MS_BOOL bEnable, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{

    MS_ASSERT(eWindow < MAX_WINDOW);
    MS_ASSERT(pSrcInfo);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16DRWidth = 0;
    MS_U16 u16DRBlockNumber = 0;
    MS_U16 u16DRRequestNumber = 0;
    MS_U8 u8T3DMIUSel = 0;
    MS_WINDOW_TYPE stDispWin;
    memset(&stDispWin, 0, sizeof(MS_WINDOW_TYPE));
    MDrv_XC_get_dispwin_from_reg(pInstance, eWindow, &stDispWin);

    if(pSrcInfo == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: pSrcInfo is null pointer!\n");
        return;
    }

    if(eWindow >= MAX_WINDOW)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: eWindow is wrong!\n");
        return;
    }

    if(bEnable)
    {
        MS_PHY u32HW2DTO3D_DD_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf;
        MS_PHY u32HW2DTO3D_DR_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf;

#ifdef HAL_MIU2_BASE
      if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf >= HAL_MIU2_BASE)
        {
            u32HW2DTO3D_DD_Buf -= HAL_MIU2_BASE;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, 0x2000, 0x3000);//DD and DR both use miu2
            u8T3DMIUSel = 2;
        }
        else
#endif
        if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf >= HAL_MIU1_BASE)
        {
            u32HW2DTO3D_DD_Buf -= HAL_MIU1_BASE;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, 0x1000, 0x3000);//DD and DR both use miu1
            u8T3DMIUSel = 1;
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, 0x0000, 0x3000);//DD and DR both use miu0
            u8T3DMIUSel = 0;
        }
        Hal_SC_Set_T3D_MiuSelectExternal(pInstance, u8T3DMIUSel);
#ifdef HAL_MIU2_BASE
        if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf >= HAL_MIU2_BASE)
        {
            u32HW2DTO3D_DR_Buf -= HAL_MIU2_BASE;
        }
        else
#endif
        if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf >= HAL_MIU1_BASE)
        {
            u32HW2DTO3D_DR_Buf -= HAL_MIU1_BASE;
        }

        //=======================================================//
        //=====2015.04.24 patch for 2d3d under not full-screen by Ace.Chang //
        //==  Issue: Left-side has data from right-side (repeated data)   ==//
        //==  Solution:T3d DE use MainDE instead of Frame-DE              ==//
        //==  Note: DD & DR input size use corresponding ratio instead    ==//
        //=======================================================//
        MS_BOOL bT3DDE_Use_MainDisplayWindow = _MDrv_Is_T3DDE_UseMainDisplayWindow(pInstance);

        //Get panel size output width
        MS_U32 u32OutputWidth = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L) + 1;
        MS_U32 PanelHeight= SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L) + 1;
        if(u32OutputWidth==0)
           u32OutputWidth = 0xf00;
        if(PanelHeight==0)
            PanelHeight = 0x870;
        //clk
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, BIT(6)); //reg_ckg_ficlk_3d
        //dd
        //y/cb/cr mim/max initialize
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_09_L, 0x3f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_0A_L, 0x5f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_0B_L, 0x7f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_0C_L, 0x9f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_0D_L, 0xbf00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_0E_L, 0x5a1e);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_0F_L, 0xff00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_11_L, 0x3f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_12_L, 0x5f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_13_L, 0x7f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_14_L, 0x9f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_15_L, 0xbf00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_16_L, 0x2800);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_17_L, 0xc0a0);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_19_L, 0x3f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_1A_L, 0x5f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_1B_L, 0x7f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_1C_L, 0x9f00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_1D_L, 0xbf00);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_1E_L, 0xfa78);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_1F_L, 0x6040);
        //coef
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_20_L, 0x4d34);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_21_L, 0x540f);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_22_L, 0x5133);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_23_L, 0x171f);
        //mode select
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x00fc, 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x9f00, 0xDF00);
        //filter program
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_25_L, 0x8888);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_26_L, 0x10ff);
        //color quality initialize
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_38_L, 0xffa4);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_39_L, 0x0210);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_3A_L, 0x1210);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_3B_L, 0x1002);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_3C_L, 0x0108);
        #if (HW_2DTO3D_VER == 3)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, MIN(u32OutputWidth, 1920), 0x07FF);//src width
        if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] != E_XC_3D_INPUT_MODE_NONE)
        {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, MIN(pSrcInfo->stDispWin.height, 1080), 0x0FFF);//src height
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, MIN(stDispWin.height, 1080), 0x0FFF);//src height
        }
        #elif (HW_2DTO3D_VER == 4)
        _MDrv_SC_3D_DD_setting(pInstance, eWindow);
        #endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, 0x7000, 0xF000);//dd shift enable ext
#if(HW_2DTO3D_BYTE_PER_WORD == 16)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x7878, 0x7F7F);//dram request number
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x3c3c/*0x7878*/, 0x7F7F);//dram request number
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, (u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD)>>16, 0x07FF);//dd buffer high 11bit
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_07_L, u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD);//dd buffer low 2 bytes
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK63_44_L, (u32HW2DTO3D_DR_Buf + HW_2DTO3D_BLOCK_DR_BUF_SIZE)/HW_2DTO3D_BYTE_PER_WORD);//dr buffer:block base address
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK63_46_L, u32HW2DTO3D_DR_Buf/HW_2DTO3D_BYTE_PER_WORD);//dr buffer:pixel base address
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4A_L, BIT(10),  BIT(10)); //disable YUV to RGB
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_40_L, BIT(6)|BIT(4),  BIT(6)|BIT(4)); // DD force bank enable,high priorty threshold
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_41_L, 0x0000, 0x000F);     // DD force bank0
        //after DD address setted,should wait 2 vsync for pix counter finished
        MDrv_XC_wait_output_vsync(pInstance, 2, 100, eWindow);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu

        //DR setting
        _MDrv_SC_3D_DR_setting(pInstance, eWindow);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, (u32OutputWidth+31)/32, 0x00FF);//horizontal number of 32x32 block
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, BIT(8), BIT(8));//eco en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_57_L, BIT(1), BIT(1));//dr miu eco en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_42_L, BIT(7), BIT(7));//reg_clrmiurdy_n
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_48_L, BIT(2), BIT(2)|BIT(3)|BIT(4)|BIT(5)|BIT(6));//reg_blk_dpthptr_en,reg_blk_dpthptr_sel
        //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, 0x3C3C); //reg_dpcach_req_ctr,reg_dpcach_last_ctr
        //u16DRRequestNumber = _MDrv_SC_3D_2DTo3D_DRRequestNumber(pInstance);
        if(bT3DDE_Use_MainDisplayWindow)
        {
            u16DRWidth = _MDrv_SC_3D_GetDispWinWidthForT3D(pInstance);
        }
        else
        {
            u16DRWidth = u32OutputWidth;
        }
        u16DRBlockNumber = (u16DRWidth + 31)/32;
        u16DRRequestNumber = (u16DRBlockNumber + HW_2DTO3D_BYTE_PER_WORD - 1) / HW_2DTO3D_BYTE_PER_WORD;
        if(u16DRRequestNumber<4)
        {
            u16DRRequestNumber = 4; //always set req >=4, has problem when 1/2/3, HW limitation. Ace note
        }
        u16DRRequestNumber |= (u16DRRequestNumber<<8);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, u16DRRequestNumber,0xFFFF); //reg_dpcach_req_ctr,reg_dpcach_last_ctr
        //fill sram
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0xFFFF);//reg_fill_in
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, BIT(7), BIT(7));//reg_fill_w

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0000, 0x0F00);//reg_fill_add 0
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x5BFE5BFE);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0100, 0x0F00);//reg_fill_add 1
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xA884A884);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0200, 0x0F00);//reg_fill_add 2
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x38543854);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0300, 0x0F00);//reg_fill_add 3
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xA487A487);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0400, 0x0F00);//reg_fill_add 4
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x32123212);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0500, 0x0F00);//reg_fill_add 5
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x57265726);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0600, 0x0F00);//reg_fill_add 6
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xA823A823);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0700, 0x0F00);//reg_fill_add 7
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xA878A878);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0800, 0x0F00);//reg_fill_add 8
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xA826A826);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0900, 0x0F00);//reg_fill_add 9
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x12841284);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0A00, 0x0F00);//reg_fill_add 10
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x48484848);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0B00, 0x0F00);//reg_fill_add 11
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x79237923);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0C00, 0x0F00);//reg_fill_add 12
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x8A428A42);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0D00, 0x0F00);//reg_fill_add 13
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0x38423842);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0E00, 0x0F00);//reg_fill_add 14
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xFF22FF22);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x0F00, 0x0F00);//reg_fill_add 15
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_74_L, 0xAA88AA88);//reg_fill_sram_1,reg_fill_sram_2

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_73_L, 0x0000);//reg_fill_in
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_71_L, 0x00, BIT(7));//reg_fill_w

        //dr pixel
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_60_L, 0x07, 0x3FFF);//reg_pipe_guard_cycle_pb
        if(pXCResourcePrivate->stdrvXC_3D._eInput3DMode[eWindow] != E_XC_3D_INPUT_MODE_NONE)
        {
        if((pSrcInfo->stDispWin.width > (WIDTH_4K2KTIMING - OFFSET_4K2KTIMING))
           && (pSrcInfo->stDispWin.width < (WIDTH_4K2KTIMING + OFFSET_4K2KTIMING)))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, BIT(4)|BIT(5), BIT(4)|BIT(5));//dd h 2:1 scaling down
        }
        if((pSrcInfo->stDispWin.height > (HEIGHT_4K2KTIMING - OFFSET_4K2KTIMING))
            && (pSrcInfo->stDispWin.height < (HEIGHT_4K2KTIMING + OFFSET_4K2KTIMING)))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, BIT(0), BIT(0));//dd v 2:1 scaling down
        }
        }
        else
        {
            if((stDispWin.width > (WIDTH_4K2KTIMING - OFFSET_4K2KTIMING))
               && (stDispWin.width < (WIDTH_4K2KTIMING + OFFSET_4K2KTIMING)))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, BIT(4)|BIT(5), BIT(4)|BIT(5));//dd h 2:1 scaling down
            }
            if((stDispWin.height > (HEIGHT_4K2KTIMING - OFFSET_4K2KTIMING))
                && (stDispWin.height < (HEIGHT_4K2KTIMING + OFFSET_4K2KTIMING)))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, BIT(0), BIT(0));//dd v 2:1 scaling down
            }
        }
#if (HW_2DTO3D_VER >= 4)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_63_L, 0x00, BIT(0)|BIT(1));//reg_srclb_en,reg_depthlb_en
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_63_L, BIT(0)|BIT(1), BIT(0)|BIT(1));//reg_srclb_en,reg_depthlb_en
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_68_L, BIT(0)|BIT(2), BIT(0)|BIT(1)|BIT(2));//reg_render_control,reg_lr_control_l,reg_lr_control_r
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_08_L, BIT(5), BIT(5));//rgb_to_y
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_67_L, 0x8080);//reg_pixelsub_l,reg_pixelsub_r
        #if (HW_2DTO3D_VER == 4)
        //Tuning initial value
#if ENABLE_T3D_DEBUG
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5B_L, 0x0010, 0x00ff);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5C_L, 0x8630, 0xffff);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5D_L, 0x0710, 0xffff);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5E_L, 0x1000, 0xffff);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5F_L, 0x2000, 0xff00);
        #endif
        #endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_50_L, BIT(3), BIT(3));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_62_L, (u32HW2DTO3D_DD_Buf + HW_2DTO3D_DD_BUF_SIZE) * 4 / HW_2DTO3D_BYTE_PER_WORD, 0xFFFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_63_L, ((u32HW2DTO3D_DD_Buf + HW_2DTO3D_DD_BUF_SIZE) * 4 /HW_2DTO3D_BYTE_PER_WORD) >> 16, 0x0FFF);
    }
    else
    {
#if (HW_2DTO3D_VER == 4)
        //=======================================================//
        //=====2017.01.17 patch for 3d under not full-screen by Ace.Chang //
        //==  Issue: Left-side has datadistortion                                     ==//
        //==  Solution:T3d DE use MainDE instead of Frame-DE              ==//
        //==  Note: DD & DR input size use corresponding ratio instead    ==//
        //=======================================================//
        _MDrv_SC_3D_DR_setting(pInstance, eWindow);
        _MDrv_SC_3D_DD_setting(pInstance, eWindow);
#endif
        MS_U32 u32OutputWidth = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, 0x0000, BIT(13)|BIT(15));//src domain select&start to write miu
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, 0x0780, 0x07FF);//src width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, 0x0438, 0x0FFF);//src height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_55_L, u32OutputWidth, 0x1FFF);//pixel width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_66_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height, 0x1FFF);//reg_col_height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_51_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, 0x1FFF);//reg_ln_width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_52_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height, 0x1FFF);//reg_col_height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, 0x0000, 0x00FF);//horizontal number of 32x32 block
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, 0x0000, BIT(8));//eco en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_57_L, 0x0000, BIT(1));//dr miu eco en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4A_L, 0x0000, BIT(10));//enable YUV to RGB
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK63_50_L, 0x8080); //reg_dpcach_req_ctr,reg_dpcach_last_ctr
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_61_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, 0x3FFF);//reg_ln_width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_62_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height, 0x1FFF);//reg_col_height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_68_L, 0x0000, BIT(0)|BIT(1)|BIT(2));//reg_render_control,reg_lr_control_l,reg_lr_control_r
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_67_L, 0x0000);//reg_pixelsub_l,reg_pixelsub_r
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_6B_L, 0x0000, BIT(0)|BIT(4)|BIT(5));//dd h/v 2:1 scaling down
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_63_L, 0x0000, BIT(0));// disable T3D SRAM
#if (HW_2DTO3D_VER >= 4)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, 0x0000, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_63_L, 0x0000, BIT(1));// disable T3D SRAM
        //muji, monaco
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_30_L, 0x00, BIT(14));
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_50_L, 0, BIT(3));
    }
}
#endif
#endif

#endif
//-------------------------------------------------------------------------------------------------
/// When we do PIP 3D output, maybe users want to adjust the main and wub window for 3D effect.
/// We call this api to exchange once, and call it again to return to original status.
/// @param  eWindow                \b IN: which window we are going to set
/// @return MS_BOOL                \b OUT: TRUE:Success FALSE:Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_LR_Frame_Exchg_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        bRet = MDrv_XC_Set_3D_LR_Frame_Exchg_burst(pInstance, eWindow);
    }
    else
    {
        MDrv_XC_wait_output_vsync(pInstance, 2, 100, eWindow);
        bRet = MDrv_XC_Set_3D_LR_Frame_Exchg(pInstance, eWindow);
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    UNUSED(eWindow);
#endif
    return bRet;
}

MS_BOOL MApi_XC_Set_3D_LR_Frame_Exchg(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_LR_EXCHANGE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_LR_EXCHANGE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Is L/R flag Exchanged or not
/// @param  eWindow                \b IN: which window we are going to set
/// @return MS_BOOL                \b OUT: TRUE:L/R exchanged FALSE:L/R not exchanged
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_3D_Is_LR_Frame_Exchged_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_3D_Is_LR_Frame_Exchged(pInstance, eWindow);
    _XC_RETURN(pInstance);
#else
    UNUSED(eWindow);
#endif
    return bRet;
}

MS_BOOL MApi_XC_3D_Is_LR_Frame_Exchged(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_3D_LR_EXCHANGED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_3D_LR_EXCHANGED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_Set_3D_HShift(void* pInstance, MS_U16 u16HShift)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, pXCResourcePrivate->stdrvXC_3D._eInput3DMode[MAIN_WINDOW]
                                        , pXCResourcePrivate->stdrvXC_3D._eOutput3DMode, MAIN_WINDOW))
    {
        bRet = TRUE;
        pXCResourcePrivate->stdrvXC_3D._u163DHShift = u16HShift&0xff;

#if (HW_2DTO3D_SUPPORT && (HW_2DTO3D_VER >= 3))
        MS_U16 u16ABSLevel = ((u16HShift > 0x80)?(u16HShift - 0x80):(0x80 - u16HShift));
        {
            if((u16HShift == 0x80) || (u16HShift == 0x00))
            {
                //original 3d depth
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, 0x0000); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, 0x0000); //2  // 2nd offset R
            }
            else if(u16HShift > 0x80)
            {
                // |L-->      <--R|
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, (0xFF - (u16ABSLevel-1))<<8); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, (u16ABSLevel)<<8); //2  // 2nd offset R
            }
            else
            {
                // |L<--      -->R|
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, (u16ABSLevel)<<8); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, (0xFF - (u16ABSLevel-1))<<8); //2  // 2nd offset R
            }
        }
#if ENABLE_T3D_DEBUG
        //enable black edge
        Hal_SC_3D_enable_black_edge(pInstance, FALSE);
#endif
#else
        if((u16HShift == 0x80) || (u16HShift == 0x00))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x0000, 0x8000); //2  // 2nd offset enable
        }
        else if(u16HShift > 0x80)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x8000, 0x8000); //2  // 2nd offset enable
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5E_L, 0x0000); //2  // 2nd offset L
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5F_L, (u16HShift-0x80)<<8); //2  // 2nd offset R
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x8000, 0x8000); //2  // 2nd offset enable
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5E_L, (0x80-u16HShift)<<8); //2  // 2nd offset L
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5F_L, 0x0000); //2  // 2nd offset R
        }
#endif
    }
    else if(MApi_XC_Get_3D_HW_Version() < 2)
    {
        if(u16HShift & 0x01)
        {
            pXCResourcePrivate->stdrvXC_3D._u163DHShift = u16HShift + 1;
            printf("horizontal shift pixel should be even: adjust to %u\n", pXCResourcePrivate->stdrvXC_3D._u163DHShift);
        }
        else
        {
            bRet = TRUE;
            pXCResourcePrivate->stdrvXC_3D._u163DHShift = u16HShift;
        }
    }
    else
    {
        bRet = TRUE;
        //if 2P mode is enabled, the shift must be even.
#if (XC_SUPPORT_2P_MODE == TRUE)
        pXCResourcePrivate->stdrvXC_3D._u163DHShift = u16HShift*2 & 0xFF; //Offset is 8 bit
#else
        pXCResourcePrivate->stdrvXC_3D._u163DHShift = u16HShift & 0xFF; //Offset is 8 bit
#endif
    }
#else
    UNUSED(u16HShift);
#endif
    return bRet;
}

//------------------------------------------------------------------------------------------------------
/// Set 3D horizontal shift pixel
/// @param  u16HShift                \b IN: input  horizontal shift pixel
/// @return MS_BOOL                  \b OUT: TRUE: set is ok FALSE: set value is odd, need align to even
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_HShift_U2(void* pInstance, MS_U16 u16HShift)
{
    MS_BOOL bRet = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
     _XC_ENTRY(pInstance);

    bRet = MDrv_XC_Set_3D_HShift(pInstance, u16HShift);

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bRet;
}

MS_BOOL MApi_XC_Set_3D_HShift(MS_U16 u16HShift)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_HORIZONTAL_SHIFT XCArgs;
    XCArgs.u16HShift = u16HShift;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_HORIZONTAL_SHIFT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Set 3D reg_3dlr_side2line_en, for line_alternative full screen output
/// @param  u16HShift                \b IN: input  horizontal shift pixel
/// @return MS_BOOL                  \b OUT: TRUE: set is ok FALSE: set value is odd, need align to even
//------------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_Enable_3D_LR_Sbs2Line(void *pInstance, MS_BOOL bEnable)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
#if (HW_DESIGN_3D_VER >= 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_53_L, (bEnable << 15), 0x8000);//dnr offset
    pXCResourcePrivate->stdrvXC_3D._bIsLR_Sbs2Line = bEnable;
    bRet = TRUE;
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
#else
    UNUSED(bEnable);
#endif
    return bRet;
}

MS_BOOL MApi_XC_Enable_3D_LR_Sbs2Line_U2(void* pInstance, MS_BOOL bEnable)
{
    MS_BOOL bReturn = FALSE;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if(!Hal_SC_3D_IsDualViewMode(pInstance,MAIN_WINDOW))
 #endif
    {
        bReturn = MDrv_XC_Enable_3D_LR_Sbs2Line(pInstance, bEnable);
    }
    return bReturn;
}

MS_BOOL MApi_XC_Enable_3D_LR_Sbs2Line(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_LR_SBS2LINE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_LR_SBS2LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Is 3D reg_3dlr_side2line_en enabled or not
/// @return MS_BOOL                  \b OUT: TRUE: enabled FALSE: is not enabled
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_3D_Is_LR_Sbs2Line_U2(void* pInstance)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    bRet = MDrv_SC_3D_Is_LR_Sbs2Line(pInstance);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
    return bRet;
}

MS_BOOL MApi_XC_3D_Is_LR_Sbs2Line(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_3D_LR_SBS2LINE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_3D_LR_SBS2LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
//-------------------------------------------------------------------------------------------------
/// Get 3D horizontal shift pixel
/// @param  void
/// @return MS_U16                            \b OUT: need horizontal shift pixel value
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_XC_Get_3D_HShift(void *pInstance)
{
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    return pXCResourcePrivate->stdrvXC_3D._u163DHShift;
#else
    return 0;
#endif
}

MS_U16 MApi_XC_Get_3D_HShift_U2(void* pInstance)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16Return = MDrv_XC_Get_3D_HShift(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u16Return;
}

MS_U16 MApi_XC_Get_3D_HShift(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_3D_HORIZONTAL_SHIFT XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_HORIZONTAL_SHIFT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Set HW 2D to 3D para
/// @param  st3DHw2DTo3DPara          \b IN: parameters
/// @return MS_BOOL                   \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_HW2DTo3D_Parameters_U2(void* pInstance, MS_XC_3D_HW2DTO3D_PARA st3DHw2DTo3DPara)
{
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bRet = TRUE;
#if HW_2DTO3D_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(st3DHw2DTo3DPara.u16Concave > 15)
    {
        printf("u16Concave only have 4 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Concave = HW_2DTO3D_DEFAULT_CONCAVE;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Concave = st3DHw2DTo3DPara.u16Concave;
    }

    if(st3DHw2DTo3DPara.u16ArtificialGain > 15)
    {
        printf("u16ArtificialGain only have 4 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ArtificialGain = HW_2DTO3D_DEFAULT_ARTIFICIALGAIN;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ArtificialGain = st3DHw2DTo3DPara.u16ArtificialGain;
    }
#if (HW_2DTO3D_VER < 2)
    if(st3DHw2DTo3DPara.u16Gain > 63)
    {
        printf("u16Gain only have 6 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain = HW_2DTO3D_DEFAULT_GAIN;
    }
#else
    if(st3DHw2DTo3DPara.u16Gain > 127)
    {
        printf("u16Gain only have 7 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain = HW_2DTO3D_DEFAULT_GAIN;
    }
#endif
    else
    {
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain = st3DHw2DTo3DPara.u16Gain;
    }

    if(st3DHw2DTo3DPara.u16Offset > 255)
    {
        printf("u16Offset only have 8 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset = HW_2DTO3D_DEFAULT_OFFSET;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset = st3DHw2DTo3DPara.u16Offset;
    }

    if(st3DHw2DTo3DPara.u16EleSel > 3)
    {
        printf("u16Offset only have 2 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel = HW_2DTO3D_DEFAULT_ELESEL;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EleSel = st3DHw2DTo3DPara.u16EleSel;
    }

    if(st3DHw2DTo3DPara.u16ModSel > 3)
    {
        printf("u16Offset only have 2 bits\n");
        bRet = FALSE;
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ModSel = HW_2DTO3D_DEFAULT_MODSEL;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ModSel = st3DHw2DTo3DPara.u16ModSel;
    }

    if(st3DHw2DTo3DPara.u32Hw2dTo3dPara_Version < 2)
    {
        st3DHw2DTo3DPara.u16EdgeBlackWidth = 0;
    }
    else
    {
        if(st3DHw2DTo3DPara.u16EdgeBlackWidth > 127)
        {
            printf("u16EdgeBlackWidth only have 7 bits\n");
            bRet = FALSE;
            pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16ModSel = HW_2DTO3D_DEFAULT_EDGEBLACKWIDTH;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EdgeBlackWidth = st3DHw2DTo3DPara.u16EdgeBlackWidth;
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    printf("this function is only supported by HW_2DTO3D_SUPPORT=TRUE\n");
    bRet = FALSE;
#endif
    return bRet;
#else
    MS_BOOL bRet = FALSE;
    UNUSED(st3DHw2DTo3DPara);
    printf("Not support!!");
    return bRet;
#endif
}

MS_BOOL MApi_XC_Set_3D_HW2DTo3D_Parameters(MS_XC_3D_HW2DTO3D_PARA st3DHw2DTo3DPara)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_HW2DTO3D_PARAMETERS XCArgs;
    XCArgs.st3DHw2DTo3DPara = st3DHw2DTo3DPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_HW2DTO3D_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Get HW 2D to 3D para
/// @param  pst3DHw2DTo3DPara          \b IN: pointer to parameters
/// @return MS_BOOL                    \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Get_3D_HW2DTo3D_Parameters_U2(void* pInstance, MS_XC_3D_HW2DTO3D_PARA *pst3DHw2DTo3DPara)
{
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bRet = TRUE;

    MS_ASSERT(pst3DHw2DTo3DPara);
    if(pst3DHw2DTo3DPara == NULL)
    {
        printf("3D: pst3DHw2DTo3DPara is null pointer!\n");
        return FALSE;
    }

#if HW_2DTO3D_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16CopiedLength = sizeof(MS_XC_3D_HW2DTO3D_PARA);
    //new AP + old lib, only the length corresponding to old lib has meaning.
    if(pst3DHw2DTo3DPara->u32Hw2dTo3dPara_Version > HW2DTO3DPARA_VERSION)
    {
        //We cosider compatible operation from version1 , so reject the info init when version invalid
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Get_3D_HW2DTo3D_Parameters: this old version XC lib has only length:%lu !!\n",
                     sizeof(MS_XC_3D_HW2DTO3D_PARA))
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Get_3D_HW2DTo3D_Parameters: this old version XC lib has only length:%u !!\n",
                     sizeof(MS_XC_3D_HW2DTO3D_PARA))
#endif
        //we still copy the min size of both structure, but AP should take care of it.
        u16CopiedLength = sizeof(MS_XC_3D_HW2DTO3D_PARA);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if(pst3DHw2DTo3DPara->u32Hw2dTo3dPara_Version == 1)
    {
        //We cosider compatible operation form version1 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MApi_XC_Get_3D_HW2DTo3D_Parameters: new version XC lib shouldn't access to the space which doesn't exist in old structure!!\n")
        u16CopiedLength = sizeof(MS_XC_3D_HW2DTO3D_PARA) - sizeof(pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16EdgeBlackWidth);
    }

    memcpy(pst3DHw2DTo3DPara, &(pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara), u16CopiedLength);
    bRet = TRUE;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    printf("this function is only supported by HW_2DTO3D_SUPPORT=TRUE\n");
    memset(pst3DHw2DTo3DPara, 0, sizeof(MS_XC_3D_HW2DTO3D_PARA));
    bRet = FALSE;
#endif
    return bRet;
#else
    MS_BOOL bRet = FALSE;
    UNUSED(pst3DHw2DTo3DPara);
    return bRet;
#endif
}

MS_BOOL MApi_XC_Get_3D_HW2DTo3D_Parameters(MS_XC_3D_HW2DTO3D_PARA *pst3DHw2DTo3DPara)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_3D_HW2DTO3D_PARAMETERS XCArgs;
    XCArgs.pst3DHw2DTo3DPara = pst3DHw2DTo3DPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_HW2DTO3D_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Set HW 2D to 3D dd buffer&dr buffer
/// @param  MS_PHYADDR                \b IN: dd buffer address
/// @param  MS_PHYADDR                \b IN: dr buffer address
/// @return MS_BOOL                   \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_HW2DTo3D_Buffer_U2(void* pInstance, MS_PHY u32HW2DTO3D_DD_Buf, MS_PHY u32HW2DTO3D_DR_Buf)
{
#ifndef DISABLE_3D_FUNCTION
#if HW_2DTO3D_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf = u32HW2DTO3D_DD_Buf;
    pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf = u32HW2DTO3D_DR_Buf;

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
#else
    printf("this function is only supported by HW_2DTO3D_SUPPORT=TRUE\n");
    return FALSE;
#endif
#else
    UNUSED(u32HW2DTO3D_DD_Buf);
    UNUSED(u32HW2DTO3D_DR_Buf);
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Set_3D_HW2DTo3D_Buffer(MS_PHY u32HW2DTO3D_DD_Buf, MS_PHY u32HW2DTO3D_DR_Buf)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_HW2DTO3D_BUFFER XCArgs;
    XCArgs.u32HW2DTO3D_DD_Buf = u32HW2DTO3D_DD_Buf;
    XCArgs.u32HW2DTO3D_DR_Buf = u32HW2DTO3D_DR_Buf;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_HW2DTO3D_BUFFER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Set Color Registers after PQ
/// @param  eWindow                   \b IN: which window to do post pq setting
/// @return MS_BOOL                   \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_3D_PostPQSetting_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION
    MS_ASSERT(eWindow < MAX_WINDOW);
    if(eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    bRet = MDrv_SC_3D_PostPQSetting(pInstance, &gSrcInfo[eWindow], eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    UNUSED(eWindow);
#endif

    return bRet;
}

MS_BOOL MApi_XC_3D_PostPQSetting(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_POST_PQSETTING XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_POST_PQSETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Set Detect 3D Format Para
/// @param  pstDetect3DFormatPara       \b IN: pointer to parameters
/// @return MS_BOOL                     \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_Detect3DFormat_Parameters_U2(void* pInstance, MS_XC_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara)
{
#ifndef DISABLE_3D_FUNCTION

    MS_ASSERT(pstDetect3DFormatPara);
    if(pstDetect3DFormatPara == NULL)
    {
        printf("3D: pstDetect3DFormatPara is null pointer!\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(&(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara), pstDetect3DFormatPara, sizeof(MS_XC_3D_DETECT3DFORMAT_PARA));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
#else
    UNUSED(pstDetect3DFormatPara);
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Set_3D_Detect3DFormat_Parameters(MS_XC_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_DETECT_3DFORMAT_PARAMETERS XCArgs;
    XCArgs.pstDetect3DFormatPara = pstDetect3DFormatPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_DETECT_3DFORMAT_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Get Detect 3D Format Para
/// @param  pstDetect3DFormatPara       \b IN: pointer to parameters
/// @return MS_BOOL                     \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Get_3D_Detect3DFormat_Parameters_U2(void* pInstance, MS_XC_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara)
{
#ifndef DISABLE_3D_FUNCTION
    MS_ASSERT(pstDetect3DFormatPara);
    if(pstDetect3DFormatPara == NULL)
    {
        printf("3D: pstDetect3DFormatPara is null pointer!\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(pstDetect3DFormatPara, &(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara), sizeof(MS_XC_3D_DETECT3DFORMAT_PARA));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
#else
    UNUSED(pstDetect3DFormatPara);
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Get_3D_Detect3DFormat_Parameters(MS_XC_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    static MS_XC_3D_DETECT3DFORMAT_PARA tmp_para;
    memcpy(&tmp_para,pstDetect3DFormatPara,sizeof(MS_XC_3D_DETECT3DFORMAT_PARA));
    stXC_GET_3D_DETECT_3DFORMAT_PARAMETERS XCArgs;
    XCArgs.pstDetect3DFormatPara = &tmp_para;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_DETECT_3DFORMAT_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        memcpy(pstDetect3DFormatPara,&tmp_para,sizeof(MS_XC_3D_DETECT3DFORMAT_PARA));
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EnableAutoDetect3D_U2(void* pInstance, MS_BOOL bEnable, E_XC_3D_AUTODETECT_METHOD enDetectMethod)
{
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bRet = TRUE;

    MS_ASSERT(enDetectMethod < E_XC_3D_AUTODETECT_MAX);
    if(enDetectMethod >= E_XC_3D_AUTODETECT_MAX)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod     = enDetectMethod;
    pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D    = bEnable;
    if(pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod == E_XC_3D_AUTODETECT_HW)
    {
        pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D = pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D = FALSE;
    }

#if (HW_2DTO3D_SUPPORT == FALSE)
    if((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
       || (E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod))
    {
        printf("Attention, not support hw auto detect 3d, change to sw method!\n");
        pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod = E_XC_3D_AUTODETECT_SW;
        bRet = FALSE;
    }
#endif

#if HW_2DTO3D_SUPPORT
    if((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
        && (!pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D))//if disable hw auto detect 3d
    {
        _XC_ENTRY(pInstance);
        //detach vsync interrupt
        if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
        _XC_RETURN(pInstance);
    }
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    MS_BOOL bRet = FALSE;
    UNUSED(bEnable);
    UNUSED(enDetectMethod);
#endif
    return bRet;
}

MS_BOOL MApi_XC_EnableAutoDetect3D(MS_BOOL bEnable, E_XC_3D_AUTODETECT_METHOD enDetectMethod)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_AUTODETECT XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.enDetectMethod = enDetectMethod;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_AUTODETECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_GetAutoDetect3DFlag_U2(void* pInstance, E_XC_3D_AUTODETECT_METHOD *penDetectMethod, MS_BOOL *pbEnable)
{
#ifndef DISABLE_3D_FUNCTION
    if((penDetectMethod == NULL) || (pbEnable == NULL))
    {
        printf("3D: penDetectMethod or pbEnable is null pointer!\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    *penDetectMethod = pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod;
    if(pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod == E_XC_3D_AUTODETECT_HW)
    {
        *pbEnable = pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D;
    }
    else
    {
        *pbEnable = pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
#else
    UNUSED(penDetectMethod);
    UNUSED(pbEnable);
    return FALSE;
#endif
}

MS_BOOL MApi_XC_GetAutoDetect3DFlag(E_XC_3D_AUTODETECT_METHOD *penDetectMethod, MS_BOOL *pbEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_3D_AUTODETECT_3DFLAG XCArgs;
    XCArgs.penDetectMethod = penDetectMethod;
    XCArgs.pbEnable = pbEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_AUTODETECT_3DFLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Detect Nine Lattice By Content
/// @param  eWindow                   \b IN: which window view we are going to detect
/// @param  pstDetectNLatticePara     \b IN: parameters for detecting
/// @return MS_BOOL                   \b OUT: true->detect nine lattice succeed; false->detect fail
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_DetectNL_U2(void* pInstance, SCALER_WIN eWindow, ST_DETECTNL_PARA* pstDetectNLatticePara)
{
#ifndef DISABLE_3D_FUNCTION
    MS_U16 u16LatticeIdx = 0;
    MS_U16 u16H_PixelIdx = 0;
    MS_U16 u16V_PixelIdx = 0;
    MS_U16 i = 0;
    MS_WINDOW_TYPE stSrcRect = {0,0,0,0};
    MS_PIXEL_24BIT* pstSource = NULL;
    MS_PIXEL_24BIT* pstCurrentPoint = NULL;
    MS_U8* pu8Buf = NULL;
    MS_U8  u8FieldNum = 0;
    MS_U8* pu8BufReg = NULL;
    MS_BOOL bRet = TRUE;
    MS_U16 u16HCenterIdx[DETECTNL_LATTICE_NUM] = {0,0,0,0,0,0,0,0,0};
    MS_U16 u16VCenterIdx[DETECTNL_LATTICE_NUM] = {0,0,0,0,0,0,0,0,0};
    MS_U8  u8TryTimes = 0;
    MS_U16 u16PixelStep = DETECTNL_PIXEL_STEP;
    MS_U8  u8ColorThreshold = DETECTNL_COLORTHRESHOLD;
    MS_U8  u8MaxTryTimes = DETECTNL_TRY_TIMES;
#if 0 // no need
    MS_U16 u16LinearDiff = DETECTNL_LINEAR_DIFF;
#endif
#if DETECTNL_COMPUTE_TIME
    MS_U32 u32StartTime=0;
    u32StartTime = MsOS_GetSystemTime();
#endif
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    if (NULL != pstDetectNLatticePara)
    {
        u16PixelStep = pstDetectNLatticePara->u16PixelStep;
        u8ColorThreshold = pstDetectNLatticePara->u8ColorThreshold;
        u8MaxTryTimes = pstDetectNLatticePara->u8TryTimes;
#if 0 // no need
        u16LinearDiff = pstDetectNLatticePara->u16LinearDiff;
#endif
    }
    if (0 == u8MaxTryTimes || eWindow >= MAX_WINDOW)
    {
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    stSrcRect.width = gSrcInfo[eWindow].u16H_SizeAfterPreScaling/3;
    stSrcRect.height = gSrcInfo[eWindow].u16V_SizeAfterPreScaling/3;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    pstSource = (MS_PIXEL_24BIT*)malloc(stSrcRect.width * stSrcRect.height * sizeof(MS_PIXEL_24BIT));
    if (pstSource == NULL)
    {
        return FALSE;
    }

    pu8BufReg = (MS_U8*)malloc(stSrcRect.width/u16PixelStep * (stSrcRect.height/u16PixelStep) * sizeof(MS_U8));
    if (pu8BufReg == NULL)
    {
        free(pstSource);
        pstSource = NULL;
        return FALSE;
    }

    for (u16LatticeIdx = 0; u16LatticeIdx < DETECTNL_LATTICE_NUM; u16LatticeIdx++)  //check 9 lattices
    {
        MS_U16 u16LatticeLeft = 0; //record the lattice rect
        MS_U16 u16LatticeRight = 0;
        MS_U16 u16LatticeTop = 0;
        MS_U16 u16LatticeBottom = 0;
        MS_BOOL bForceHCheck = FALSE;

        bRet = TRUE;

        //locate the lattice
        stSrcRect.x = (u16LatticeIdx%3) * stSrcRect.width;
        stSrcRect.y = (u16LatticeIdx/3) * stSrcRect.height;

        //get the lattice rect
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        MApi_XC_Get_BufferData_WithoutMutex(pInstance, E_XC_OUTPUTDATA_RGB8BITS, &stSrcRect, (MS_U8 *)pstSource, eWindow);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        //For edge effect, we have to multi-times, every time has a shift.
        for (u8TryTimes = 0; u8TryTimes < u8MaxTryTimes; u8TryTimes++)
        {
            bRet = TRUE;
            pu8Buf = pu8BufReg;
            *pu8Buf = 0;
            u8FieldNum = 0;
            //V direction scan
            for (u16V_PixelIdx = (u16PixelStep/u8MaxTryTimes) * u8TryTimes; u16V_PixelIdx < stSrcRect.height - u16PixelStep; u16V_PixelIdx += u16PixelStep)
            {
                //H direction scan
                for (u16H_PixelIdx = (u16PixelStep/u8MaxTryTimes) * u8TryTimes; u16H_PixelIdx < stSrcRect.width - u16PixelStep; u16H_PixelIdx += u16PixelStep)
                {
                    //locate source point
                    pstCurrentPoint = pstSource + u16V_PixelIdx * stSrcRect.width + u16H_PixelIdx;
                    //locate tag of source point
                    pu8Buf = pu8BufReg + (u16V_PixelIdx/u16PixelStep)* (stSrcRect.width/u16PixelStep) + u16H_PixelIdx/u16PixelStep;
                    //check whether current point is similar to the point in last line in same column
                    if (u16V_PixelIdx != ((u16PixelStep/u8MaxTryTimes) * u8TryTimes)) //not first line
                    {
                        MS_PIXEL_24BIT* pstLastLinePoint = NULL;
                        //Y comparing with last line
                        pstLastLinePoint = pstCurrentPoint - stSrcRect.width*u16PixelStep;
                        if (abs(pstLastLinePoint->G_Y - pstCurrentPoint->G_Y) < u8ColorThreshold)
                        {
                            *pu8Buf = *(pu8Buf - stSrcRect.width/u16PixelStep);
                            if (1 == *pu8Buf)
                            {
                                if (u16H_PixelIdx == u16LatticeLeft)
                                {
                                    u16LatticeBottom = u16V_PixelIdx;
                                    continue;
                                }
                                else
                                {
                                    bForceHCheck = TRUE;
                                }
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else
                        {
                            *pu8Buf = DETECTNL_NON_VALUE;
                        }
                    }
                    //check whether current point is simaler to the point in last column with same line
                    if (u16H_PixelIdx != ((u16PixelStep/u8MaxTryTimes) * u8TryTimes))
                    {
                        MS_PIXEL_24BIT* pstLastColumnPoint = NULL;
                        //Y comparing with last column
                        pstLastColumnPoint = pstCurrentPoint - u16PixelStep;
                        if (abs(pstLastColumnPoint->G_Y - pstCurrentPoint->G_Y) < u8ColorThreshold)
                        {
                            *pu8Buf = *(pu8Buf - 1);
                            if (1 == *pu8Buf)
                            {
                                if (u16V_PixelIdx == u16LatticeTop)
                                {
                                    u16LatticeRight = u16H_PixelIdx;
                                    continue;
                                }
                                else
                                {
                                    if ((FALSE == bForceHCheck)
                                        && (u16V_PixelIdx != u16LatticeTop - u16PixelStep)  //ignore some points near top/right edge
                                        && (u16V_PixelIdx != u16LatticeTop + u16PixelStep)
                                        && (u16V_PixelIdx != u16LatticeTop + u16PixelStep)
                                        && (u16H_PixelIdx != u16LatticeRight - u16PixelStep)
                                        && (u16H_PixelIdx != u16LatticeRight + u16PixelStep))
                                    {
                                        bRet = FALSE;
                                        break;
                                    }
                                }
                            }
                            if (TRUE == bForceHCheck)
                            {
                                bForceHCheck = FALSE;
                                if ((1 != *pu8Buf)
                                    && (u16H_PixelIdx != u16LatticeLeft)  //ignore some points near left edge
                                    && (u16H_PixelIdx != u16LatticeLeft - u16PixelStep)
                                    && (u16H_PixelIdx != u16LatticeLeft + u16PixelStep)
                                    && (u16V_PixelIdx != u16LatticeBottom)
                                    && (u16V_PixelIdx != u16LatticeBottom - u16PixelStep)
                                    && (u16V_PixelIdx != u16LatticeBottom + u16PixelStep)
                                    && (u16H_PixelIdx != u16LatticeRight)
                                    && (u16H_PixelIdx != u16LatticeRight + u16PixelStep)
                                    && (u16H_PixelIdx != u16LatticeRight - u16PixelStep))
                                {
                                    bRet = FALSE;
                                    break;
                                }
                            }
                            continue;
                        }
                        else
                        {
                            *pu8Buf = DETECTNL_NON_VALUE;
                        }
                        bForceHCheck = FALSE;
                    }
                    if (DETECTNL_NON_VALUE == *pu8Buf)
                    {
                        if ((1 == u8FieldNum)
                            && ((u16H_PixelIdx == u16LatticeLeft - u16PixelStep)  //ignore some points near left/right/bottom edge
                            || (u16H_PixelIdx == u16LatticeRight - u16PixelStep)
                            || (u16H_PixelIdx == u16LatticeRight + u16PixelStep)
                            || (u16V_PixelIdx == u16LatticeBottom - u16PixelStep)
                            || (u16V_PixelIdx == u16LatticeBottom + u16PixelStep)))
                        {
                            *pu8Buf = 1;
                        }
                        else
                        {
                            u8FieldNum++;
                            *pu8Buf = u8FieldNum;
                            if (1 == *pu8Buf) // catch first "1" field position
                            {
                                u16LatticeLeft = u16H_PixelIdx;
                                u16LatticeRight = u16H_PixelIdx;
                                u16LatticeTop = u16V_PixelIdx;
                                u16LatticeBottom = u16V_PixelIdx;
                            }

                            if (u8FieldNum > 1)
                            {
                                break;
                            }
                        }
                    }
                }
                if (u8FieldNum > 1) // 3 fields
                {
                    break;
                }
            }

            if (u8FieldNum != 1) // 0 and 1 fields
            {
                bRet = FALSE;
            }
            if (TRUE == bRet)
            {
                u16HCenterIdx[u16LatticeIdx] = (u16LatticeLeft + u16LatticeRight) / 2;
                u16VCenterIdx[u16LatticeIdx] = (u16LatticeTop + u16LatticeBottom) / 2;
                break;
            }
        }
        if (FALSE == bRet)
        {
            break;
        }
    }
    if (TRUE == bRet) //check linear
    {
        for (i = 0; i < DETECTNL_LATTICE_NUM/3; i++)
        {
            if ((abs(u16VCenterIdx[i*3]-u16VCenterIdx[i*3+1]) > u8MaxTryTimes + u16PixelStep/2)
                || (abs(u16VCenterIdx[i*3+1]-u16VCenterIdx[i*3+2]) > 32+u16PixelStep/2))
            {
                bRet = FALSE;
                break;
            }
            if ((abs(u16HCenterIdx[i]-u16HCenterIdx[i+3]) > u8MaxTryTimes + u16PixelStep/2)
                || (abs(u16HCenterIdx[i+3]-u16HCenterIdx[i+6]) > 32+u16PixelStep/2))
            {
                bRet = FALSE;
                break;
            }
        }
    }
    free(pu8BufReg);
    free(pstSource);
    pstSource = NULL;
    pu8BufReg = NULL;
#if DETECTNL_COMPUTE_TIME
    printf("*time*%ld\n",MsOS_GetSystemTime()-u32StartTime);
#endif
    return bRet;
#else
    MS_BOOL bRet = FALSE;
    UNUSED(pstDetectNLatticePara);
    UNUSED(eWindow);
    printf("Not support!!");
    return bRet;
#endif

}

MS_BOOL MApi_XC_DetectNL(SCALER_WIN eWindow, ST_DETECTNL_PARA* pstDetectNLatticePara)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_NINELATTICE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.pstDetectNLatticePara = pstDetectNLatticePara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_NINELATTICE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Detect 3D Format By Content
/// @param  eWindow                   \b IN: which window view we are going to detect
/// @return E_XC_3D_INPUT_MODE        \b OUT: detected 3d format
//------------------------------------------------------------------------------------------------------
E_XC_3D_INPUT_MODE MApi_XC_Detect3DFormatByContent_U2(void* pInstance, SCALER_WIN eWindow)
{
    E_XC_3D_INPUT_MODE e3DInput = E_XC_3D_INPUT_MODE_NONE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifndef DISABLE_3D_FUNCTION
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(MDrv_XC_IsInputSourceDisabled(pInstance, eWindow))
    {
        printf("Attention! Should enable input source when do auto detect 3d\n");
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return E_XC_3D_INPUT_MODE_NONE;
    }

#if (HW_2DTO3D_SUPPORT && (HW_2DTO3D_VER >= 3))
    if((IS_INPUT_FRAME_PACKING(MAIN_WINDOW)
          || IS_INPUT_TOP_BOTTOM(MAIN_WINDOW)
          || IS_INPUT_SIDE_BY_SIDE_HALF(MAIN_WINDOW))
        && IS_OUTPUT_LINE_ALTERNATIVE())
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, BIT(13), BIT(13));//reg_det3d_la_en
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x00, BIT(13));//reg_det3d_la_en
    }
#else
    if((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_MODE_NONE)
       || (MDrv_XC_Get_3D_Output_Mode(pInstance) != E_XC_3D_OUTPUT_MODE_NONE))
    {
        printf("Attention! Should disable 3d when do auto detect 3d\n");
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return E_XC_3D_INPUT_MODE_NONE;
    }
#endif

    if((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D)
    {
#if (HW_2DTO3D_SUPPORT == FALSE)
        printf("Attention! hw auto detect mode is not supported by this chip!\n");
#endif
        e3DInput = pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes;
    }
    else if((E_XC_3D_AUTODETECT_HW_COMPATIBLE == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D)
    {
#if HW_2DTO3D_SUPPORT
        MS_U32 u32Time = 0;
        pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D = TRUE;

        _XC_ENTRY(pInstance);

#if (HW_2DTO3D_VER < 3)

        MDrv_XC_get_dispwin_from_reg(pInstance, eWindow, &gSrcInfo[eWindow].stDispWin);
        MS_PHY u32HW2DTO3D_DD_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf;
        MS_PHY u32HW2DTO3D_DR_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf;

        if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf >= HAL_MIU1_BASE)
        {
            u32HW2DTO3D_DD_Buf -= HAL_MIU1_BASE;
        }

        if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf >= HAL_MIU1_BASE)
        {
            u32HW2DTO3D_DR_Buf -= HAL_MIU1_BASE;
        }

        //clk
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, BIT(6)); //reg_ckg_ficlk_3d
        //dd
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, gSrcInfo[eWindow].stDispWin.width, 0x07FF);//src width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, gSrcInfo[eWindow].stDispWin.height, 0x07FF);//src height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x7878, 0x7F7F);//dram request number
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, (u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD)>>16, 0x07FF);//dd buffer high 9bit
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_07_L, u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD);//dd buffer low 2 bytes
        MDrv_XC_wait_output_vsync(pInstance, 1, 100, eWindow);
#if (HW_2DTO3D_VER < 2)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(13)|BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu
#endif
#if (HW_2DTO3D_VER > 0)
        MDrv_WriteByteMask(REG_MIU0_BASE+0x2B, BIT(6), BIT(6)); //enable memory write
#endif
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_21_L, 0x540f);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x0430);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_40_L, BIT(6),  BIT(6));   // DD force bank enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_41_L, 0x0000, 0x000F);     // DD force bank0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, 0, BIT(12));        //disable dynamic gating
#else
        _MDrv_SC_3D_Load2DTo3DReg(pInstance, TRUE, &gSrcInfo[eWindow], eWindow);
#endif

        pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank = TRUE;
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
        pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount = 0;
        pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
        _XC_RETURN(pInstance);


        u32Time = MsOS_GetSystemTime();
        while((E_XC_3D_INPUT_MODE_NONE == pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes)
              && (pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount)
              && ((MsOS_GetSystemTime() - u32Time) < 4000))
        {
            // because other thread wiill update _enCurDetectedRes, _u16CheckedFrameCount, u16MaxCheckingFrameCount
            // so we need to unlock first so that other thread(a.k.a ISR) can retrieve semaphore and update variables
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            MsOS_DelayTask(100);
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        }

        _XC_ENTRY(pInstance);
        //detach vsync interrupt
        if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }

#if (HW_2DTO3D_VER < 3)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, 0x0000, BIT(13)|BIT(15));//src domain select&start to write miu
        //dd
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, 0x0780, 0x07FF);//src width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, 0x0438, 0x07FF);//src height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x7878, 0x7F7F);//dram request number
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_21_L, 0x0000);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x0000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, BIT(12), BIT(12)); //enable dynamic gating
#else
        _MDrv_SC_3D_Load2DTo3DReg(pInstance, FALSE, &gSrcInfo[eWindow], eWindow);
#endif
        _XC_RETURN(pInstance);
        pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D = FALSE;
#else
        printf("Attention! hw auto detect compatible mode is not supported by this chip!\n");
#endif
        e3DInput = pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes;
    }
    else if((E_XC_3D_AUTODETECT_SW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bEnableAutoDetect3D)
    {
        e3DInput = E_XC_3D_INPUT_MODE_NONE;
        printf("\n Attention! SW auto detect mode is not supported. return value 3D_INPUT_MODE_NONE !\n");
#if 0
        MS_PIXEL_24BIT stSource = {0,0,0};
        MS_U16 u16SBSMatchedPixelCount = 0;
        MS_U16 u16TBMatchedPixelCount = 0;
        MS_PIXEL_24BIT stDest[pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange*pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSearchRange];
        MS_WINDOW_TYPE stSrcRect;
        MS_WINDOW_TYPE stDestRect;
        MS_U16 i = 0;
        MS_U16 j = 0;
        MS_U16 k = 0;
        MS_U16 m = 0;
        MS_U16 u16Temp = 0;
        MS_U16 u16SBSMatchedFrameCount = 0;
        MS_U16 u16TBMatchedFrameCount = 0;
        MS_BOOL bExit = FALSE;
        MS_U16 u16FrameCount = 0;
        MS_U16 u16Array[3][32];
        MS_U16 u16MaxSimCount[3];
        MS_U16 u16PurePixel = 0;
        MS_U16 u16PureFrameCount = 0;

        MS_ASSERT(eWindow < MAX_WINDOW);

        if(eWindow >= MAX_WINDOW)
        {
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return E_XC_3D_INPUT_MODE_NONE;
        }

        if(gSrcInfo[eWindow].bFBL
           || gSrcInfo[eWindow].bR_FBL
           || (gSrcInfo[eWindow].u16H_SizeAfterPreScaling == 0)
           || (gSrcInfo[eWindow].u16V_SizeAfterPreScaling == 0)
           )
        {
            printf("Attention: invalid case of MApi_XC_Detect3DFormatByContent!\n");
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return E_XC_3D_INPUT_MODE_NONE;
        }

        memset(&stDest, 0 , sizeof(stDest));

        _XC_ENTRY(pInstance);

        do
        {
            u16SBSMatchedPixelCount = 0;
            u16TBMatchedPixelCount = 0;
            MDrv_XC_wait_input_vsync(pInstance, 1, 100, eWindow);
            Hal_SC_set_freezeimg(pInstance, ENABLE, eWindow);

            ///skip pure frame
            u16PurePixel = 0;
            memset(u16Array, 0, sizeof(u16Array));
            memset(u16MaxSimCount, 0, sizeof(u16MaxSimCount));
            for(j = 0; j < DETECT3DFORMAT_DEFAULT_VERPUREPIXEL; j++)
            {
                for(i = 0; i < DETECT3DFORMAT_DEFAULT_HORPUREPIXEL; i++)
                {
                    stSrcRect.x = gSrcInfo[eWindow].u16H_SizeAfterPreScaling*(i+1)/(DETECT3DFORMAT_DEFAULT_HORPUREPIXEL+1);
                    stSrcRect.y = gSrcInfo[eWindow].u16V_SizeAfterPreScaling*(j+1)/(DETECT3DFORMAT_DEFAULT_VERPUREPIXEL+1);
                    stSrcRect.width = 1;
                    stSrcRect.height = 1;

                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "====================pure frame check Attention! i=%u, j=%u=======================================================\n", i, j);
                    MApi_XC_Get_BufferData_WithoutMutex(pInstance, E_XC_OUTPUTDATA_RGB8BITS, &stSrcRect, (MS_U8 *)&stSource, eWindow);
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "===================x=%u, y=%u==============================\n", stSrcRect.x, stSrcRect.y);
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "y=%x, cb=%x, cr=%x\n", stSource.G_Y, stSource.B_Cb, stSource.R_Cr);

                    u16Array[0][stSource.G_Y / pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16GYPixelThreshold]++;
                    u16Array[1][stSource.B_Cb / pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16BCbPixelThreshold]++;
                    u16Array[2][stSource.R_Cr / pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16RCrPixelThreshold]++;
                }
            }

            u16MaxSimCount[0] = u16Array[0][0] + u16Array[0][1];
            u16MaxSimCount[1] = u16Array[1][0] + u16Array[1][1];
            u16MaxSimCount[2] = u16Array[2][0] + u16Array[2][1];
            for(j = 0; j < 3; j++)
            {
                for(i = 1; i < 31; i++)
                {
                    MS_U16 u16Temp = u16Array[j][i] + u16Array[j][i + 1];
                    if(u16Temp > u16MaxSimCount[j])
                    {
                        u16MaxSimCount[j] = u16Temp;
                    }
                }
            }

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16MaxSimCount[0]=%u, u16MaxSimCount[1]=%u, u16MaxSimCount[2]=%u\n", u16MaxSimCount[0], u16MaxSimCount[1], u16MaxSimCount[2]);

            u16PurePixel = MIN(MIN(u16MaxSimCount[0], u16MaxSimCount[1]), u16MaxSimCount[2]);

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16PurePixel=%u\n", u16PurePixel);

            if(u16PurePixel >= (DETECT3DFORMAT_DEFAULT_HORPUREPIXEL*DETECT3DFORMAT_DEFAULT_VERPUREPIXEL*DETECT3DFORMAT_DEFAULT_PUREPIXELPERCENTAGE/100))
            {
                u16PureFrameCount++;
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "pure frame detected!\n");
                Hal_SC_set_freezeimg(pInstance, DISABLE, eWindow);
                continue;
            }

            ///sbs check
            for(j = 0; j < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount; j++)
            {
                for(i = 0; i < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount; i++)
                {
                    stSrcRect.x = gSrcInfo[eWindow].u16H_SizeAfterPreScaling*(i+1)/2/(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount+1);
                    stSrcRect.y = gSrcInfo[eWindow].u16V_SizeAfterPreScaling*(j+1)/(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount+1);
                    stSrcRect.width = 1;
                    stSrcRect.height = 1;

                    stDestRect.x = stSrcRect.x + gSrcInfo[eWindow].u16H_SizeAfterPreScaling/2 - pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange/2;
                    stDestRect.y = stSrcRect.y;
                    stDestRect.width = pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange;
                    stDestRect.height = pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSearchRange;

                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "====================SBS Attention! i=%u, j=%u=======================================================\n", i, j);
                    MApi_XC_Get_BufferData_WithoutMutex(pInstance, E_XC_OUTPUTDATA_RGB8BITS, &stSrcRect, (MS_U8 *)&stSource, eWindow);
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "===================x=%u, y=%u==============================\n", stSrcRect.x, stSrcRect.y);
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "y=%x, cb=%x, cr=%x\n", stSource.G_Y, stSource.B_Cb, stSource.R_Cr);
                    MApi_XC_Get_BufferData_WithoutMutex(pInstance, E_XC_OUTPUTDATA_RGB8BITS, &stDestRect, (MS_U8 *)stDest, eWindow);
                    bExit = FALSE;
                    for(m = 0; (!bExit) && (m < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSearchRange); m++)
                    {
                        for(k = 0; k < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange; k++)
                        {
                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "===================x=%u, y=%u==============================\n", stDestRect.x+k, stDestRect.y);
                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "y=%x, cb=%x, cr=%x\n", stDest[i].G_Y, stDest[i].B_Cb, stDest[i].R_Cr);
                            u16Temp = k + m * pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange;
                            if((abs((MS_S16)stDest[u16Temp].G_Y-stSource.G_Y) < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16GYPixelThreshold)    //find the matched pixel
                               && (abs((MS_S16)stDest[u16Temp].B_Cb-stSource.B_Cb) < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16BCbPixelThreshold)
                               && (abs((MS_S16)stDest[u16Temp].R_Cr-stSource.R_Cr) < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16RCrPixelThreshold)
                               )
                            {
                                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "SBS hit: i=%u, j=%u\n", i, j);
                                u16SBSMatchedPixelCount++;
                                bExit = TRUE;
                                break;
                            }
                        }
                    }
                }
            }

            ///tb check
            for(j = 0; j < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount; j++)
            {
                for(i = 0; i < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount; i++)
                {
                    stSrcRect.x = gSrcInfo[eWindow].u16H_SizeAfterPreScaling*(i+1)/(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount+1);
                    stSrcRect.y = gSrcInfo[eWindow].u16V_SizeAfterPreScaling*(j+1)/2/(pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount+1);
                    stSrcRect.width = 1;
                    stSrcRect.height = 1;

                    stDestRect.x = stSrcRect.x - pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange/2;
                    stDestRect.y = stSrcRect.y + gSrcInfo[eWindow].u16V_SizeAfterPreScaling/2;
                    stDestRect.width = pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange;
                    stDestRect.height = pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSearchRange;

                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "====================TB Attention! i=%u, j=%u=======================================================\n", i, j);
                    MApi_XC_Get_BufferData_WithoutMutex(pInstance, E_XC_OUTPUTDATA_RGB8BITS, &stSrcRect, (MS_U8 *)&stSource, eWindow);
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "===================x=%u, y=%u==============================\n", stSrcRect.x, stSrcRect.y);
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "y=%x, cb=%x, cr=%x\n", stSource.G_Y, stSource.B_Cb, stSource.R_Cr);
                    MApi_XC_Get_BufferData_WithoutMutex(pInstance, E_XC_OUTPUTDATA_RGB8BITS, &stDestRect, (MS_U8 *)stDest, eWindow);
                    bExit = FALSE;
                    for(m = 0; (!bExit) && (m < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSearchRange); m++)
                    {
                        for(k = 0; k < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange; k++)
                        {
                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "===================x=%u, y=%u==============================\n", stDestRect.x+k, stDestRect.y);
                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "y=%x, cb=%x, cr=%x\n", stDest[i].G_Y, stDest[i].B_Cb, stDest[i].R_Cr);
                            u16Temp = k + m * pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSearchRange;
                            if((abs((MS_S16)stDest[u16Temp].G_Y-stSource.G_Y) < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16GYPixelThreshold)    //find the matched pixel
                               && (abs((MS_S16)stDest[u16Temp].B_Cb-stSource.B_Cb) < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16BCbPixelThreshold)
                               && (abs((MS_S16)stDest[u16Temp].R_Cr-stSource.R_Cr) < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16RCrPixelThreshold)
                               )
                            {
                                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "TB hit: i=%u, j=%u\n", i, j);
                                u16TBMatchedPixelCount++;
                                bExit = TRUE;
                                break;
                            }
                        }
                    }
                }
            }

            Hal_SC_set_freezeimg(pInstance, DISABLE, eWindow);

            if(u16SBSMatchedPixelCount >=
                (pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount*pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount*pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HitPixelPercentage/100))
            {
                u16SBSMatchedFrameCount++;
            }

            if(u16TBMatchedPixelCount >=
                (pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HorSampleCount*pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16VerSampleCount*pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16HitPixelPercentage/100))
            {
                u16TBMatchedFrameCount++;
            }

            u16FrameCount++;

            MsOS_DelayTask(100);

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16SBSMatchedPixelCount=%u, u16TBMatchedPixelCount=%u\n", u16SBSMatchedPixelCount, u16TBMatchedPixelCount);
        }while((u16FrameCount < pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount) && (u16PureFrameCount < 256));

        _XC_RETURN(pInstance);

        if((u16SBSMatchedFrameCount > pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount*DETECT3DFORMAT_DEFAULT_HITFRAMEPERCENTAGE/10)
           && (u16TBMatchedFrameCount > pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount*DETECT3DFORMAT_DEFAULT_HITFRAMEPERCENTAGE/10)
           )
        {
            e3DInput = E_XC_3D_INPUT_MODE_NONE;
        }
        else if(u16SBSMatchedFrameCount > pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount*DETECT3DFORMAT_DEFAULT_HITFRAMEPERCENTAGE/10)
        {
            e3DInput = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
        }
        else if(u16TBMatchedFrameCount > pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount*DETECT3DFORMAT_DEFAULT_HITFRAMEPERCENTAGE/10)
        {
            e3DInput = E_XC_3D_INPUT_TOP_BOTTOM;
        }
        else
        {
            e3DInput = E_XC_3D_INPUT_MODE_NONE;
        }
#endif
    }
    else
    {
        printf("Attention, not enable auto detect 3d!\n");
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    UNUSED(eWindow);
#endif
    return e3DInput;
}

E_XC_3D_INPUT_MODE MApi_XC_Detect3DFormatByContent(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_3D_FORMAT_DETECTED_BY_CONTENT XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_3D_FORMAT_DETECTED_BY_CONTENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Set 3D Frame Packing Info
/// @param  pstFPInfoPara             \b IN: pointer to struct MS_XC_3D_FPINFO_PARA
/// @return MS_BOOL                   \b OUT: TRUE: set is ok FALSE: set value is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_FPInfo_U2(void* pInstance, MS_XC_3D_FPINFO_PARA *pstFPInfoPara)
{
#ifndef DISABLE_3D_FUNCTION

    MS_ASSERT(pstFPInfoPara);
    if(pstFPInfoPara == NULL)
    {
        printf("3D: pstFPInfoPara is null pointer!\n");
        return FALSE;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(&(pXCResourcePrivate->stdrvXC_3D._stFPInfoPara), pstFPInfoPara, sizeof(MS_XC_3D_FPINFO_PARA));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
#else
    UNUSED(pstFPInfoPara);
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Set_3D_FPInfo(MS_XC_3D_FPINFO_PARA *pstFPInfoPara)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_FRAMEPACKING_INFO XCArgs;
    XCArgs.pstFPInfoPara = pstFPInfoPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_FRAMEPACKING_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//------------------------------------------------------------------------------------------------------
/// Enable Sub Window Clock
/// @return MS_BOOL                   \b OUT: TRUE: set is ok FALSE: set is wrong
//------------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Set_3D_SubWinClk_U2(void* pInstance)
{
#ifndef DISABLE_3D_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_SC_3D_CloneMainSettings(pInstance, &gSrcInfo[SUB_WINDOW], SUB_WINDOW);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
#else
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Set_3D_SubWinClk(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_3D_SUBWINCLOCK XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_3D_SUBWINCLOCK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Query the input 3D format is supported or not
/// @param  E_XC_3D_INPUT_MODE                \b IN: input  3d format
/// @param  E_XC_3D_INPUT_MODE                \b IN: output 3d format
/// @return MS_BOOL                           \b OUT: TRUE:Supported FALSE:NotSupported
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_Is3DFormatSupported(void *pInstance, E_XC_3D_INPUT_MODE e3dInputMode,
                               E_XC_3D_OUTPUT_MODE e3dOutputMode)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_3D_FUNCTION

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "==============================================================\n");
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D:this 3D format is e3dInputMode:0x%x;e3dOutputMode:%x\n",
                     e3dInputMode, e3dOutputMode);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "==============================================================\n");

//-------------------------------------------------------------------------------------------------
/// special case
//-------------------------------------------------------------------------------------------------
    if(((e3dInputMode == E_XC_3D_INPUT_MODE_NONE) &&
        (e3dOutputMode == E_XC_3D_OUTPUT_MODE_NONE))
        ||
//-------------------------------------------------------------------------------------------------
///obsolete case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE)
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D))
        &&(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC))
        ||
        (((e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT)
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_INTERLACE)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE)
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_INTERLACE_PTP))
        &&(e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING_OPT) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
        ||
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING_OPT)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM_OPT))
        &&(e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF))
        ||
//-------------------------------------------------------------------------------------------------
///frame alternative output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
#if (HW_DESIGN_3D_VER == 4)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL)
#endif
#if (HW_DESIGN_3D_VER >= 2)
        ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
        ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D))
        &&((e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
             ||(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR)))
#if HW_2DTO3D_SUPPORT
#if ((HW_2DTO3D_VER  != 3))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_LLRR_HW))
#endif
#endif
        ||
//-------------------------------------------------------------------------------------------------
///line alternative output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D)
#if (HW_DESIGN_3D_VER >= 2)
        ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
        ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
        )
        &&(e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
#if (HW_2DTO3D_SUPPORT || (HW_DESIGN_3D_VER == 4))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE_HW))
#endif
        ||
//-------------------------------------------------------------------------------------------------
/// top bottom output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D)
#if (HW_DESIGN_3D_VER >= 2)
        ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
        ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
#if (HW_DESIGN_3D_VER == 4)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL)
#endif
        )
        &&(e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
#if HW_2DTO3D_SUPPORT
#if (HW_2DTO3D_VER >= 4)
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM_HW))
#endif
#endif
        ||
//-------------------------------------------------------------------------------------------------
/// side by side output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D)
#if (HW_DESIGN_3D_VER >= 2)
        ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
        ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
        )
        &&(e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF))
#if HW_2DTO3D_SUPPORT
#if (HW_2DTO3D_VER >= 4)
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW))
#endif
#endif
        ||
//-------------------------------------------------------------------------------------------------
/// LL RR TT BB output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        )
        &&((e3dOutputMode == E_XC_3D_OUTPUT_TOP_TOP)
        ||(e3dOutputMode == E_XC_3D_OUTPUT_BOTTOM_BOTTOM)
        ||(e3dOutputMode == E_XC_3D_OUTPUT_LEFT_LEFT)
        ||(e3dOutputMode == E_XC_3D_OUTPUT_RIGHT_RIGHT)))
        ||
//-------------------------------------------------------------------------------------------------
/// framepacking output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
         ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_FULL)
         ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
         ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
         ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
 #if ( HW_SUPPORT_3D_CB_AND_PA  == TRUE)
         ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
         ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
         ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
         ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW)
         ||(e3dInputMode == E_XC_3D_INPUT_NORMAL_2D)
         )
        &&(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_PACKING))
#if HW_2DTO3D_SUPPORT
#if (HW_2DTO3D_VER >= 4)
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_PACKING))
#endif
#endif
        ||
//-------------------------------------------------------------------------------------------------
/// 3d to 2d Lonly output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
#if (HW_DESIGN_3D_VER >= 2)
        ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
        ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
        )
        &&(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L))
#if (HW_2DTO3D_SUPPORT || (HW_DESIGN_3D_VER == 4))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_L_HW))
#endif
        ||
//-------------------------------------------------------------------------------------------------
/// 3d to 2d Ronly output case
//-------------------------------------------------------------------------------------------------
        (((e3dInputMode == E_XC_3D_INPUT_FRAME_PACKING)
        ||(e3dInputMode == E_XC_3D_INPUT_LINE_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_TOP_BOTTOM)
        ||(e3dInputMode == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)
        ||(e3dInputMode == E_XC_3D_INPUT_FRAME_ALTERNATIVE)
#if (HW_DESIGN_3D_VER >= 2)
        ||(e3dInputMode == E_XC_3D_INPUT_FIELD_ALTERNATIVE)
        ||(e3dInputMode == E_XC_3D_INPUT_CHECK_BORAD)
        ||(e3dInputMode == E_XC_3D_INPUT_PIXEL_ALTERNATIVE)
#endif
        )
        &&(e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R))
#if (HW_2DTO3D_SUPPORT || (HW_DESIGN_3D_VER == 4))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_R_HW))
#endif
//-------------------------------------------------------------------------------------------------
/// pixel alternative output case
//-------------------------------------------------------------------------------------------------
#if (HW_2DTO3D_SUPPORT || (HW_DESIGN_3D_VER == 4))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_PIXEL_ALTERNATIVE_HW))
#endif
//-------------------------------------------------------------------------------------------------
/// check board alternative output case
//-------------------------------------------------------------------------------------------------
#if (HW_2DTO3D_SUPPORT || (HW_DESIGN_3D_VER == 4))
        ||
        ((e3dInputMode == E_XC_3D_INPUT_NORMAL_2D_HW) &&
         (e3dOutputMode == E_XC_3D_OUTPUT_CHECKBOARD_HW))
#endif
//-------------------------------------------------------------------------------------------------
/// 3d dual view
//-------------------------------------------------------------------------------------------------
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        ||
        ((e3dInputMode == E_XC_3D_INPUT_MODE_NONE) &&
         ((e3dOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)
         || (e3dOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
         || (e3dOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM)))
#endif
    )
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
#else
    UNUSED(e3dInputMode);
    UNUSED(e3dOutputMode);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "is3dformatsupported = %d ,e3dInputMode:%u;e3dOutputMode:%u\n",
                         bRet,e3dInputMode, e3dOutputMode);
    return bRet;
}

MS_BOOL MApi_XC_Is3DFormatSupported_U2(void* pInstance,
                                      E_XC_3D_INPUT_MODE e3dInputMode,
                                      E_XC_3D_OUTPUT_MODE e3dOutputMode)
{
    MS_BOOL bReturn = FALSE;
    bReturn = MDrv_XC_Is3DFormatSupported(pInstance, e3dInputMode, e3dOutputMode);

    return bReturn;
}

MS_BOOL MApi_XC_Is3DFormatSupported(E_XC_3D_INPUT_MODE e3dInputMode,
                                    E_XC_3D_OUTPUT_MODE e3dOutputMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_3D_FORMAT_SUPPORTED XCArgs;
    XCArgs.e3dInputMode = e3dInputMode;
    XCArgs.e3dOutputMode = e3dOutputMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_3D_FORMAT_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

#ifndef DISABLE_3D_FUNCTION
void MDrv_SC_3D_GetFP_Info(void *pInstance, XC_InternalStatus *pSrcInfo, MS_U16 *pu16VactVideo, MS_U16 *pu16VactSpace, MS_BOOL bPreVscalingEna)
{
    if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
    {
        *pu16VactVideo = DOUBLEHD_1080X2P_FRAME_VSIZE;
        *pu16VactSpace = DOUBLEHD_1080X2P_GARBAGE_VSIZE;
    }
    else if(pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)
    {
        *pu16VactVideo = DOUBLEHD_720X2P_FRAME_VSIZE;
        *pu16VactSpace = DOUBLEHD_720X2P_GARBAGE_VSIZE;
    }
    else if(pSrcInfo->stCapWin.height == DOUBLEHD_576X2P_VSIZE)
    {
        *pu16VactVideo = DOUBLEHD_576X2P_FRAME_VSIZE;
        *pu16VactSpace = DOUBLEHD_576X2P_GARBAGE_VSIZE;
    }
    else if((pSrcInfo->stCapWin.height == DOUBLEHD_480X2P_VSIZE) && (pSrcInfo->stCapWin.width == DOUBLEHD_480X2P_HSIZE))
    {
        *pu16VactVideo = DOUBLEHD_480X2P_FRAME_VSIZE;
        *pu16VactSpace = DOUBLEHD_480X2P_GARBAGE_VSIZE;
    }
    else if((pSrcInfo->stCapWin.height == DOUBLEHD_640_480X2P_VSIZE) && (pSrcInfo->stCapWin.width== DOUBLEHD_640_480X2P_HSIZE))
    {
        *pu16VactVideo = DOUBLEHD_640_480X2P_FRAME_VSIZE;
        *pu16VactSpace = DOUBLEHD_640_480X2P_GARBAGE_VSIZE;
    }
    else if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
    {
        *pu16VactVideo = DOUBLEHD_1080X2I_FIELD_VSIZE;
        *pu16VactSpace = (DOUBLEHD_1080X2I_VACT_SPACE1*2+DOUBLEHD_1080X2I_VACT_SPACE2);
    }
    else if(pSrcInfo->stCapWin.height == FIELD_INTERLEAVE_1080X2I_VISZE)
    {
        *pu16VactVideo = FIELD_INTERLEAVE_1080X2I_FIELD_VISZE;
        *pu16VactSpace = FIELD_INTERLEAVE_1080X2I_VACT_SPACE;
    }
    else
    {
        printf("MDrv_SC_3D_GetFP_Info: it's not regular framepacking!\n");
        *pu16VactVideo = 0;
        *pu16VactSpace = 0;
    }
//Because the u16VactSpace is took off first at BK2 <New design!!>
#if ((HW_DESIGN_3D_VER < 3) || (HW_DESIGN_3D_VER == 4))
    if(bPreVscalingEna)
    {
        MS_U8 u8Ratio;
        if(pSrcInfo->stCapWin.height > pSrcInfo->u16V_SizeAfterPreScaling)//Avoid divide overflow
        {
            u8Ratio = pSrcInfo->stCapWin.height/pSrcInfo->u16V_SizeAfterPreScaling;
            *pu16VactVideo /= u8Ratio;
            *pu16VactSpace /= u8Ratio;
        }
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "FP_Info: bPreVEna=%u, VactVideo=%u, VactSpace=%u, Vsrc=%u, Vdst=%u\n",
                 bPreVscalingEna, *pu16VactVideo, *pu16VactSpace, pSrcInfo->stCapWin.height, pSrcInfo->u16V_SizeAfterPreScaling);
}

#if (HW_DESIGN_3D_VER >= 2)
// This function is used to calculate exact pre V scaling dst, to let u16VactSpace can be totally divided
void MDrv_SC_3D_Adjust_FP_PreVerDstSize(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                                    E_XC_3D_OUTPUT_MODE eOutputMode,
                                    XC_InternalStatus *pSrcInfo,
                                    MS_U16 *pu16ScaleDst,
                                    SCALER_WIN eWindow)
{
    #define MAX_ACCEPTABLE_VRATIO 5 //Notes: 10 means scaling down to 1/5 of Vsrc
    MS_U16 u16VactVideo, u16VactSpace;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);//Get the original frame packing V act info
//A5: the u16VactSpace is took off first at BK2 <New design!!>
#if (HW_DESIGN_3D_VER > 3)

    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&(pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
       (eOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_PELLICLE)
            ||(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_NONE))
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x540;
        else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_SHUTTER)
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1920x540;
            }
            else
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out = E_XC_3D_OUTPUT_FI_1280x720;
            }
        }
    }

    if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
    {
        if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R())
        {
            *pu16ScaleDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_OUTPUT_SIDE_BY_SIDE_HALF())
        {
            *pu16ScaleDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&(pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE))
            {

                if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_PELLICLE)
                    ||(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_NONE))
                //FPI input, no prescaling down
                    *pu16ScaleDst = pSrcInfo->stDispWin.height/2;  // target for output 1920x540
                else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_SHUTTER)
                {
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                    {
                        *pu16ScaleDst = pSrcInfo->stDispWin.height/2;
                    }
                    else
                    {
                        *pu16ScaleDst = pSrcInfo->stDispWin.height;
                    }
                }
            }
            else if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE))
            {
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_SHUTTER)
                    *pu16ScaleDst = pSrcInfo->stDispWin.height/2;
                else
                    *pu16ScaleDst = pSrcInfo->stDispWin.height;
            }
            else
            {
                *pu16ScaleDst = pSrcInfo->stDispWin.height;
            }
        }
        else
        {
            //Default disable prescaling
            *pu16ScaleDst = pSrcInfo->stCapWin.height-u16VactSpace;
        }
        *pu16ScaleDst = *pu16ScaleDst & (~0x01);
    }
    else
    {
        *pu16ScaleDst = pSrcInfo->Status2.u16PreVCusScalingDst & (~0x01);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "Adjust_FP_PreVerDstSize:bPreVCusScaling=%s, dst=%u, eOutputMode=%u\n",
                 (pSrcInfo->Status2.bPreVCusScaling?"Yes":"No"),*pu16ScaleDst, eOutputMode);
#elif (HW_DESIGN_3D_VER < 3)
    MS_U8 i = 0;

    if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
    {
        //FPI input, no prescaling down
        *pu16ScaleDst = pSrcInfo->stCapWin.height;
        return;
    }
    else if(pSrcInfo->Status2.bPreVCusScaling == TRUE)
    {
        //check if can use AP assigned pre V Customer scaling
        if((pSrcInfo->Status2.u16PreVCusScalingSrc % pSrcInfo->Status2.u16PreVCusScalingDst) == 0)
        {
            i = pSrcInfo->Status2.u16PreVCusScalingSrc/pSrcInfo->Status2.u16PreVCusScalingDst;
            if((u16VactSpace%i) == 0)
            {
                //Vact space can be divided with no remainder, so this customer pre V scaling is ok.
                return;
            }
        }
    }

    for(i=1; i<=MAX_ACCEPTABLE_VRATIO; i++)
    {
        if((u16VactSpace%i) != 0)
        {
            // The u16VactSpace must be divisible by the prescaling ratio
            continue;
        }
        if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R())
        {
            if((pSrcInfo->stCapWin.height-u16VactSpace)/i <= pSrcInfo->stDispWin.height)
            {
                *pu16ScaleDst = pSrcInfo->stCapWin.height/i;
                 break;
            }
            else
            {
                continue;
            }
        }
        else if(IS_OUTPUT_SIDE_BY_SIDE_HALF())
        {
            if((pSrcInfo->stCapWin.height-u16VactSpace)/2/i <= pSrcInfo->stDispWin.height)
            {
                *pu16ScaleDst = pSrcInfo->stCapWin.height/i;
                break;
            }
            else
            {
                continue;
            }
        }
        else if(IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            if((pSrcInfo->stCapWin.height-u16VactSpace)/2/i <= pSrcInfo->stDispWin.height)
            {
                *pu16ScaleDst = pSrcInfo->stCapWin.height/i;
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            //Default disable prescaling
            *pu16ScaleDst = pSrcInfo->stCapWin.height;
            break;
        }
    }
    if(i > MAX_ACCEPTABLE_VRATIO)
    {
        *pu16ScaleDst = pSrcInfo->stCapWin.height; //Disable prescaling
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "Adjust_FP_PreVerDstSize:i=%u, dst=%u, eOutputMode=%u\n",
                 i, *pu16ScaleDst, eOutputMode);
#else
    if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
    {
        if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R())
        {
            *pu16ScaleDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_OUTPUT_SIDE_BY_SIDE_HALF() || (IS_OUTPUT_FRAME_ALTERNATIVE()))
        {
            *pu16ScaleDst = pSrcInfo->stDispWin.height<<1;
        }
        else
        {
            //Default disable prescaling
            *pu16ScaleDst = pSrcInfo->stCapWin.height-u16VactSpace;
        }
        *pu16ScaleDst = *pu16ScaleDst & (~0x01);
    }
    else
    {
        if(pSrcInfo->Status2.u16PreVCusScalingDst & BIT(0))
        {
            *pu16ScaleDst = pSrcInfo->Status2.u16PreVCusScalingDst & (~0x01);
        }
        else
        {
            *pu16ScaleDst = pSrcInfo->Status2.u16PreVCusScalingDst;
        }
    }
#endif
}

void MDrv_SC_3D_Adjust_PreScaling(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                                    E_XC_3D_OUTPUT_MODE eOutputMode,
                                    XC_InternalStatus *pSrcInfo,
                                    SCALER_WIN eWindow)
{
    MS_BOOL b3DPreVScaling=FALSE, b3DPreHScaling=FALSE;
    MS_U32 u32H_PreScalingRatio, u32V_PreScalingRatio, u32TempWidth;
    MS_BOOL bForcePreVScalingDown = FALSE;
    MS_BOOL bForcePreHScalingDown = FALSE;
    MS_U8 u8FrameNum = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    MS_U16 u16PreHCusScalingDstAlign = 0;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_3D_VER != 4)
    if(IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R())
    {
        //for best quality, don't do prescaling when not necessary
        if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
        }

        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;
        }
    }
#endif

    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) ||  IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow))
    {
        if(IS_OUTPUT_TOP_BOTTOM())
        {
            if(pSrcInfo->bInterlace)
            {
                if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height;
                }
            }
            else
            {
                if(pSrcInfo->Status2.bPreVCusScaling == FALSE)
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
#if(HW_DESIGN_3D_VER == 5) //monaco mode
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
#else
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
#endif
                }
            }
        }
#ifdef FA_1920X540_OUTPUT
        else if(IS_OUTPUT_FRAME_ALTERNATIVE()&&(!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC))
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;

            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height/2;
        }
#endif
        else if(IS_OUTPUT_FRAME_PACKING())
        {
            b3DPreHScaling = TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;

            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
        }
    }
    else if(IS_OUTPUT_SIDE_BY_SIDE_HALF()
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ))
    {
        b3DPreHScaling=TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;

        b3DPreVScaling=TRUE;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
    }
    else if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
    {
        b3DPreHScaling=TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width/2;
        pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;

        b3DPreVScaling=TRUE;
        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow))
    {
        MS_U16 u16VactVideo, u16VactSpace, u16ScaleDst = 0xFFFF;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);//Get the original frame packing V act info
        MDrv_SC_3D_Adjust_FP_PreVerDstSize(pInstance, eInputMode, eOutputMode, pSrcInfo, &u16ScaleDst, eWindow);//calculate exact pre V scaling dst, to let u16VactSpace can be totally divided

        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            if(IS_OUTPUT_SIDE_BY_SIDE_HALF() && (pSrcInfo->Status2.u16PreHCusScalingDst > pSrcInfo->stDispWin.width/2))
            {
                b3DPreHScaling=TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            }
        }

        if(u16ScaleDst != 0xFFFF)
        {
#if (HW_DESIGN_3D_VER == 4)
            if(1)//((g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_PELLICLE)||(g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_NONE))
            {
                if((pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
                  (eOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = u16VactVideo; //pSrcInfo->stCapWin.height - u16VactSpace;
                    pSrcInfo->Status2.u16PreVCusScalingDst = u16ScaleDst;
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
                         (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = u16VactVideo; //pSrcInfo->stCapWin.height - u16VactSpace;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
                         (eOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF))
                {
                   b3DPreVScaling=TRUE;
                   pSrcInfo->Status2.u16PreVCusScalingSrc = u16VactVideo; //pSrcInfo->stCapWin.height - u16VactSpace;
                   pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height;
                }
  #ifdef TBP_1920X2160_OUTPUT
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)&&
                         (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height - u16VactSpace;
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)&&
                         (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace);
                    pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCapWin.height - u16VactSpace);
                }
                else
                {
                    b3DPreVScaling=TRUE;
                    //pSrcInfo->Status2.u16PreVCusScalingSrc = u16ScaleDst;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
                }
  #else
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)&&
                      ((eOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(eOutputMode == E_XC_3D_OUTPUT_FRAME_R)))
                {
                  b3DPreVScaling=TRUE;
                  pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace);
                  pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCapWin.height - u16VactSpace)/2;
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
                      ((eOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(eOutputMode == E_XC_3D_OUTPUT_FRAME_R)))
                {
                  b3DPreVScaling=TRUE;
                  pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace);
                  pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCapWin.height - u16VactSpace);
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)&&
                      ((eOutputMode == E_XC_3D_OUTPUT_FRAME_L)||(eOutputMode == E_XC_3D_OUTPUT_FRAME_R)))
                {
                  b3DPreVScaling=TRUE;
                  pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace);
                  pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCapWin.height - u16VactSpace)/2;
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)&&
                        (eOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF))
                {
                    b3DPreVScaling = TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace);
                    pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCapWin.height - u16VactSpace);
                }
                else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)&&
                       (eOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
                {
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                    {
                        b3DPreVScaling=TRUE;
                        pSrcInfo->Status2.u16PreVCusScalingSrc = u16VactVideo;//pSrcInfo->stCapWin.height - u16VactSpace;
                        pSrcInfo->Status2.u16PreVCusScalingDst = u16VactVideo/2;
                    }
                    else
                    {
                        b3DPreVScaling=TRUE;
                        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
                        pSrcInfo->Status2.u16PreVCusScalingDst = u16VactVideo;
                    }
                }
                else
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
                }
  #endif
            }
            else
            {
                if(eOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
                {
                    if(pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)
                    {
                        b3DPreVScaling=TRUE;
                        pSrcInfo->Status2.u16PreVCusScalingSrc = u16VactVideo;
                        pSrcInfo->Status2.u16PreVCusScalingDst = u16VactVideo;
                    }
                    else if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
                    {
                        b3DPreVScaling=TRUE;
                        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
                        pSrcInfo->Status2.u16PreVCusScalingDst = u16VactVideo;
                        printf("SG FPP-FA 2205\n");
                    }
                }
            }
#elif (HW_DESIGN_3D_VER > 2)
            b3DPreVScaling = TRUE;

            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
            //don't do v prescaling
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
            if(IS_INPUT_FRAME_PACKING(eWindow))
            {
                if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
                    && (eOutputMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))
                {

                    b3DPreHScaling=TRUE;
                    pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                    pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;

                    b3DPreVScaling = TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace);
                    pSrcInfo->Status2.u16PreVCusScalingDst = (pSrcInfo->stCapWin.height - u16VactSpace);
                }
            }
#else
             b3DPreVScaling=TRUE;

             pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
             //don't do v prescaling
             pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
#endif
         }
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_LINE_ALTERNATIVE(eWindow))
    {
        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            if((IS_OUTPUT_SIDE_BY_SIDE_HALF())
                ||(IS_INPUT_LINE_ALTERNATIVE(eWindow)
                       && ((pSrcInfo->stCapWin.height >= 1440)
                              ||((pSrcInfo->stCapWin.width >= 3840) && (pSrcInfo->stCapWin.height >= 1080)))))
            {
                b3DPreHScaling=TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
#if(HW_DESIGN_3D_VER == 5) //monaco mode
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;
#else
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
#endif
            }
            else if (IS_OUTPUT_FRAME_PACKING())
            {
                b3DPreHScaling = TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;
            }
        }
        // for better quality src don't use pr scaling down, but post scaling
        if(IS_INPUT_LINE_ALTERNATIVE(eWindow))
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
        }
        else if(IS_INPUT_TOP_BOTTOM(eWindow)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ))
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;

            b3DPreVScaling = TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
        }
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
    {
        if(pSrcInfo->Status2.bPreHCusScaling == FALSE)
        {
            if(IS_OUTPUT_SIDE_BY_SIDE_HALF())
            {
                b3DPreHScaling=TRUE;
                pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            }
            else if(IS_OUTPUT_TOP_BOTTOM())
            {
                b3DPreVScaling=TRUE;
                pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                if(pSrcInfo->stCapWin.height <= pSrcInfo->stDispWin.height/2)
                {
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
                }
                else
                {
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
                }
            }
            else if(IS_OUTPUT_LINE_ALTERNATIVE())
            {
                if(pSrcInfo->stCapWin.height > pSrcInfo->stDispWin.height)
                {
                    b3DPreVScaling=TRUE;
                    pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
                    pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height;
                }
            }
        }
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow))
    {
        MS_U16 u16VactVideo, u16VactSpace;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);//Get the original frame packing V act info
#if (HW_DESIGN_3D_VER >= 4)
        if(IS_OUTPUT_FRAME_ALTERNATIVE())
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_OUTPUT_TOP_BOTTOM())
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = (pSrcInfo->stCapWin.height - u16VactSpace)/2;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
#elif (HW_DESIGN_3D_VER >= 3)
        b3DPreHScaling=TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;

        b3DPreVScaling=TRUE;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height - u16VactSpace;
        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
#elif (HW_DESIGN_3D_VER >= 2)
        b3DPreHScaling=TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;

        b3DPreVScaling=TRUE;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
#endif
    }
    else if(IS_INPUT_CHECK_BOARD(eWindow) || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
    {
        if(IS_OUTPUT_TOP_BOTTOM())
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;

            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
        }
        else if(IS_OUTPUT_LINE_ALTERNATIVE())
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;

            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
        }
        else if(IS_OUTPUT_SIDE_BY_SIDE_HALF())
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->Status2.u16PreHCusScalingSrc;

            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingSrc;
        }
    }
    else if(IS_INPUT_NORMAL_2D(eWindow))
    {
        b3DPreHScaling=TRUE;
        b3DPreVScaling=TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;

        if (IS_OUTPUT_SIDE_BY_SIDE_HALF())
        {
            if (pSrcInfo->stCapWin.width <= pSrcInfo->stDispWin.width/2)
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;
            }
            else
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            }
        }
        else
        {
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            if(pSrcInfo->stCapWin.height <= pSrcInfo->stDispWin.height/2)
            {
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
            }
            else
            {
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
        }
    }
    else if(IS_INPUT_NORMAL_2D_HW(eWindow))
    {
        b3DPreHScaling = TRUE;
        b3DPreVScaling = TRUE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;

        if (IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            if (pSrcInfo->stCapWin.width <= pSrcInfo->stDispWin.width/2)
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;
            }
            else
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            }
        }
        else
        {
            pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stDispWin.width/2;
            if(pSrcInfo->stCapWin.height <= pSrcInfo->stDispWin.height/2)
            {
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
            }
            else
            {
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
        }
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    else if(IS_INPUT_MODE_NONE(eWindow))
    {
        if(Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
        {
            b3DPreHScaling=TRUE;
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
            if(pSrcInfo->stCapWin.width > MS_3D_LINE_BFF_MAX)//line buffer limit for main and sub use same sc0 line buffer under dual view
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[Dual View] Do PreHCusScaling for UHD input\n");
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width/2; //1920;
            }
            else
            {
                pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->stCapWin.width;
            }
            if(pSrcInfo->stCapWin.height > XC_FHD_HIGH_MAX)//dual view main and sub height should equal 1k into frc
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[Dual View] Do PreVCusScaling for UHD input\n");
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height/2; //1080;
            }
            else
            {
                pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->stCapWin.height;
            }
        }
    }
#endif

    if(MDrv_SC_GetPQHSDFlag(pInstance) == TRUE)
    {
        b3DPreHScaling = FALSE;
        pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
        pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->u16H_SizeAfterPreScaling;
    }

    //For double HD topbottom, like 1920x2160, 1280x1440
    if((IS_INPUT_TOP_BOTTOM(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE()) && (pSrcInfo->stCapWin.height >= 1440))
    {
        b3DPreVScaling = FALSE;
        pSrcInfo->Status2.u16PreVCusScalingSrc = pSrcInfo->stCapWin.height;
        pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->u16V_SizeAfterPreScaling;
    }

    //handle 3d with small display window,just roughly check
    Hal_XC_IsForcePrescaling_3D(pInstance, pSrcInfo, &bForcePreVScalingDown, &bForcePreHScalingDown, eWindow);

    //special case:1080i/1080p frame packing have fclk issue
    Hal_SC_3D_Adjust_PreHVscaling_SaveBW(pInstance, pSrcInfo, &b3DPreHScaling,&b3DPreVScaling,bForcePreHScalingDown,bForcePreVScalingDown,eWindow);

    u16PreHCusScalingDstAlign = (pSrcInfo->Status2.u16PreHCusScalingDst + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);

    //(1)protect HCusscaling size according to the line buffer
    if(!(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
       || IS_INPUT_NORMAL_2D_HW(eWindow)
       || IS_INPUT_CHECK_BOARD(eWindow)
       || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
       || IS_OUTPUT_FRAME_ALTERNATIVE()
#if (HW_DESIGN_3D_VER >= 4)
       || IS_OUTPUT_TOP_BOTTOM()
#endif
       || MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, pSrcInfo, eWindow)
       )
       ||(IS_INPUT_FRAME_ALTERNATIVE(eWindow)&&IS_OUTPUT_TOP_BOTTOM())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow)&&IS_OUTPUT_TOP_BOTTOM_HW())
       ||(IS_INPUT_NORMAL_2D_HW(eWindow)&&IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
       )
    {
        //These foramts will use subwindow, need check if the linebuff is enough
        //MS_3D_LINE_BFF_MAX is DI line buffer max
        if(u16PreHCusScalingDstAlign > MS_3D_LINE_BFF_MAX)
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = MS_3D_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "For output line buffer limit => H :PreHScaling=%s, src=%u, dst=%u\n",
                         (b3DPreHScaling?"Yes":"No"), pSrcInfo->Status2.u16PreHCusScalingSrc, pSrcInfo->Status2.u16PreHCusScalingDst);

        }
    }

    //(2)Below will check: if FB is enough or not
    if(IS_INPUT_FRAME_ALTERNATIVE(eWindow) &&
       (!pSrcInfo->bInterlace))
    {
#if (HW_DESIGN_3D_VER == 5)//monaco/muji mode
        u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
        u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                u8FrameNum,
                                                pSrcInfo->Status2.u16PreVCusScalingDst);
        // for input fi, memory store double piece data
        if((u16PreHCusScalingDstAlign*2 > u32TempWidth)&&(u32TempWidth!=0))
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = (u32TempWidth/2) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
#elif (HW_DESIGN_3D_VER > 2)
        u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
        u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                u8FrameNum,
                                                pSrcInfo->Status2.u16PreVCusScalingDst);
        // for input fi, memory store double piece data
        if((u16PreHCusScalingDstAlign*2 > u32TempWidth)&&(u32TempWidth!=0))
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = (u32TempWidth/2) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
#else
        u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                   4 /* 4 frame mode */,
                                                   pSrcInfo->Status2.u16PreVCusScalingDst);
        // for memory reason, need support 4 frame mode
        if((u16PreHCusScalingDstAlign > u32TempWidth)&&(u32TempWidth!=0))
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
#endif
    }
    else
    {
        MS_U8 u8FrameNum = 0;
        if(IS_INPUT_FRAME_PACKING(eWindow) && (gSrcInfo[eWindow].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
        {
            u8FrameNum = 4;//need 4 frame,pls check MDrv_SC_3D_Adjust_FrameNum
        }
#ifdef TBP_1920X2160_OUTPUT
#if (HW_DESIGN_3D_VER > 2)
    else if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
            && (gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
            && (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FRAME_PACKING)
            && (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM))
        {
            u8FrameNum = 4;//need 4 frame,pls check MDrv_SC_3D_Adjust_FrameNum
        }
#endif
#endif
        else
        {
            u8FrameNum = 2;
            if(IS_INPUT_NORMAL_2D(eWindow))
            {
                u8FrameNum = 3;
            }
        }

        u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                u8FrameNum,
                                                pSrcInfo->Status2.u16PreVCusScalingDst);
        if((u16PreHCusScalingDstAlign > u32TempWidth)&&(u32TempWidth!=0))
        {
            b3DPreHScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingSrc = pSrcInfo->stCapWin.width;
            pSrcInfo->Status2.u16PreHCusScalingDst = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
        }
    }

    //(3)alignment check
    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
       || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
       || IS_INPUT_CHECK_BOARD(eWindow)
       || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
       )
    {
       //For SBS input, should make sure the Hsize/2 is even to avoid the color mismatch issue in 422 mode
        if(pSrcInfo->Status2.u16PreHCusScalingDst & (BIT(1)))
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreHCusScalingDst = (pSrcInfo->Status2.u16PreHCusScalingDst>>2)<<2;
        }
    }

    if(IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_LINE_ALTERNATIVE(eWindow))
    {
        //For TPB/LAP input, should make sure the Vsize is even
        if(pSrcInfo->Status2.u16PreVCusScalingDst & BIT(0))
        {
            b3DPreVScaling=TRUE;
            pSrcInfo->Status2.u16PreVCusScalingDst = pSrcInfo->Status2.u16PreVCusScalingDst & ~0x01;
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreScaling for H :PreHScaling=%s, src=%u, dst=%u\n",
                 (b3DPreHScaling?"Yes":"No"), pSrcInfo->Status2.u16PreHCusScalingSrc, pSrcInfo->Status2.u16PreHCusScalingDst);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_SC_3D_Adjust_PreScaling for V :PreVScaling=%s, src=%u, dst=%u\n",
                 (b3DPreVScaling?"Yes":"No"), pSrcInfo->Status2.u16PreVCusScalingSrc, pSrcInfo->Status2.u16PreVCusScalingDst);

    if(b3DPreVScaling || b3DPreHScaling)
    {
        if(b3DPreHScaling && MDrv_SC_GetPQHSDFlag(pInstance))
        {
            MDrv_SC_SetPQHSDFlag(pInstance, FALSE);//Scaling is changed, ignore PQ setting
        }
        pSrcInfo->Status2.bPreHCusScaling = TRUE;//If enable, then need Enable customer scaling both
        pSrcInfo->Status2.bPreVCusScaling = TRUE;

        // this _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        // and _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        // are unavoidable case under current scaler architecture
        // please be awared and do not remove these two unless
        // refactor the archiecture
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        MDrv_SC_set_prescaling_ratio(pInstance, pSrcInfo->enInputSourceType, pSrcInfo, eWindow);
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        _XC_ENTRY(pInstance);
        if(!pSrcInfo->bPreV_ScalingDown)
        {
            Hal_SC_set_ficlk(pInstance, FALSE, eWindow );
        }
        u32H_PreScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L) & 0x40000000;
        u32H_PreScalingRatio |= (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PreScalingRatio & ~0x40000000);
        u32V_PreScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L) & 0x40000000;
        u32V_PreScalingRatio |= (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & ~0x40000000);
        if ( pSrcInfo->bPreV_ScalingDown )
        {
            Hal_SC_set_ficlk(pInstance, TRUE, eWindow );
        }

        if (eWindow == MAIN_WINDOW)
        {
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L, u32H_PreScalingRatio);   // H pre-scaling
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L, u32V_PreScalingRatio);   // V pre-scaling
        }
        else // SUB_WINDOW
        {
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_04_L, u32H_PreScalingRatio);   // H pre-scaling
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_08_L, u32V_PreScalingRatio);   // V pre-scaling
        }
        _XC_RETURN(pInstance);
    }
}

void MDrv_SC_3D_Adjust_PstScaling(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                                    E_XC_3D_OUTPUT_MODE eOutputMode,
                                    XC_InternalStatus *pSrcInfo,
                                    SCALER_WIN eWindow)
{
    MS_U16 u16VactVideo=0, u16VactSpace=0;
    MS_BOOL b3DPstVScaling=FALSE, b3DPstHScaling=FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(IS_INPUT_FRAME_PACKING(eWindow))
    {
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
    }

    if(IS_OUTPUT_LINE_ALTERNATIVE() || IS_OUTPUT_TOP_BOTTOM() ||
       IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R())
    {
        if(IS_INPUT_FRAME_PACKING(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;

            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

#if (HW_DESIGN_3D_VER >= 3)
  #ifdef TBP_1920X2160_OUTPUT
            if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)
            {
                pSrcInfo->u16VCusScalingSrc = u16VactVideo;//pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
            }
            else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)&&
                    (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
            {
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
            }
  #else
            if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)&&
               (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
            {
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
            else if((pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
               (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
            {
            #if (HW_DESIGN_3D_VER == 4)
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height);
            #else
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;
            #endif
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
            else if((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)&&
               (eOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM))
            {
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
            else if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE &&
                   (IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R()))
            {
                 pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;
                 pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
            else if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE &&
                   (IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R()))
            {
#if 0
                pSrcInfo->u16VCusScalingSrc = u16VactVideo*2;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
#else
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/4;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/4;
#endif
            }
            else if(pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE &&
                  (IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R()))
            {
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
            else
            {
                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height)/2;
                pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
            }
  #endif
#else
            pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height-u16VactSpace)/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
#endif
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "##u16V_SizeAfterPreScaling_%d, u16VCusScalingSrc=%d\n", pSrcInfo->u16V_SizeAfterPreScaling, pSrcInfo->u16VCusScalingSrc);
        }
        else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
        {

            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_TOP_BOTTOM(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_LINE_ALTERNATIVE())
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width + (pXCResourcePrivate->stdrvXC_3D._u163DHShift&0xFF);

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_TOP_BOTTOM())
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width + (pXCResourcePrivate->stdrvXC_3D._u163DHShift&0xFF);

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_LINE_ALTERNATIVE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_CHECK_BOARD(eWindow) || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        else if(Hal_SC_3D_IsDualViewMode(pInstance,eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
#endif
        if(IS_OUTPUT_FRAME_L() || IS_OUTPUT_FRAME_R())
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16VCusScalingDst <<= 1; //For 3D->2D L/R frame output, set to scaling up twice
        }
    }
    else if(IS_OUTPUT_TOP_BOTTOM_HW())
    {
        if(IS_INPUT_NORMAL_2D_HW(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
    }
    else if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        if(IS_INPUT_NORMAL_2D_HW(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
    }
    else if(IS_OUTPUT_SIDE_BY_SIDE_HALF())
    {
        if(IS_INPUT_FRAME_PACKING(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;

            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_LINE_ALTERNATIVE(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_CHECK_BOARD(eWindow) || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow))
        {

            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2;

            b3DPstVScaling=TRUE;
#if (HW_DESIGN_3D_VER >= 3)
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
#else
            MS_U16 u16VactVideo, u16VactSpace;
            MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);
            pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height-u16VactSpace)/4;
#endif
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_NORMAL_2D(eWindow))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width/2 + (pXCResourcePrivate->stdrvXC_3D._u163DHShift&0xFF);

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ))
        {
            b3DPstVScaling=TRUE;
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->ScaledCropWin.width;

            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }

    }
    else if(IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        if(IS_INPUT_FRAME_PACKING(eWindow))
        {
#if (HW_DESIGN_3D_VER >= 3)
            if((pSrcInfo->stCapWin.height == DOUBLEHD_720X2P_VSIZE)&&
                        (eOutputMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
            {
                if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_PELLICLE)
                    ||(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8FRC3DPanelType == E_XC_3D_PANEL_NONE))
                {
                    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
                    {
                        b3DPstHScaling=TRUE;
                        pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                        pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                        b3DPstVScaling=TRUE;
                        pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
                        pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
                    }
                    else
                    {
                        b3DPstHScaling=TRUE;
                        pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                        pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                        b3DPstVScaling=TRUE;
                        pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height / 2;
                        pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
                    }
                }
                else
                {
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                    {
                        b3DPstHScaling=TRUE;
                        pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                        pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                        b3DPstVScaling=TRUE;
                        pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                        pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
                    }
                    else
                    {
                        b3DPstHScaling=TRUE;
                        pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                        pSrcInfo->u16HCusScalingDst = pSrcInfo->ScaledCropWin.width;

                        b3DPstVScaling=TRUE;
                        pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                        pSrcInfo->u16VCusScalingDst = pSrcInfo->ScaledCropWin.height;
                    }
                }
            }
            else
#endif
            {
                b3DPstVScaling=TRUE;
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

#if (HW_DESIGN_3D_VER >= 3)
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                {
                    pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                }
                else
                {
                    pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
                }
#else

                pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height-u16VactSpace)/2;
#endif
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                {
                    pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
                }
                else
                {
                    pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
                }
            }
        }
        else if(IS_INPUT_LINE_ALTERNATIVE(eWindow)
#if ((HW_DESIGN_3D_VER <= 3) ||(HW_DESIGN_3D_VER == 5))
                || IS_INPUT_TOP_BOTTOM(eWindow)
#endif
                )
        {
            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
        else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow))
        {
#if (HW_DESIGN_3D_VER >= 4)
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
#else
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
#if (HW_DESIGN_3D_VER >= 3)
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
#else
            MS_U16 u16VactVideo, u16VactSpace;
            MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, FALSE);
            pSrcInfo->u16VCusScalingSrc = (pSrcInfo->ScaledCropWin.height-u16VactSpace)/4;
#endif
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
#endif
        }
        else if(IS_INPUT_CHECK_BOARD(eWindow) || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
        else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
#ifdef FA_1920X540_OUTPUT  // for 1920x540x120Hz output
        else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)&&(!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height/2;
        }
#else
        else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow))
        {
            b3DPstHScaling=TRUE;
            pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
            pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

            b3DPstVScaling=TRUE;
            pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
            pSrcInfo->u16VCusScalingDst = pSrcInfo->stDispWin.height;
        }
#endif

    }
    else if(IS_OUTPUT_FRAME_PACKING())
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == DOUBLEHD_1080X2P_VSIZE)
        {
            if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_TOP_BOTTOM(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_FRAME_PACKING(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = u16VactVideo;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_LINE_ALTERNATIVE(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_CHECK_BOARD(eWindow) || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_NORMAL_2D(eWindow) || IS_INPUT_NORMAL_2D_HW(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_1080X2P_FRAME_VSIZE;
            }
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == DOUBLEHD_720X2P_VSIZE)
        {
            if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_TOP_BOTTOM(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_FRAME_PACKING(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = u16VactVideo;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_LINE_ALTERNATIVE(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height/2;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_CHECK_BOARD(eWindow) || IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width/2;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_FRAME_VSIZE;
            }
            else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_VSIZE;
            }
            else if(IS_INPUT_NORMAL_2D(eWindow) || IS_INPUT_NORMAL_2D_HW(eWindow))
            {
                b3DPstHScaling=TRUE;
                pSrcInfo->u16HCusScalingSrc = pSrcInfo->ScaledCropWin.width;
                pSrcInfo->u16HCusScalingDst = pSrcInfo->stDispWin.width;

                b3DPstVScaling=TRUE;
                pSrcInfo->u16VCusScalingSrc = pSrcInfo->ScaledCropWin.height;
                pSrcInfo->u16VCusScalingDst = DOUBLEHD_720X2P_VSIZE;
            }
        }

    }

    if((!MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, eInputMode, eOutputMode, eWindow))
       && ((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == eOutputMode)
           || (E_XC_3D_OUTPUT_TOP_BOTTOM == eOutputMode)
           || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == eOutputMode)
           || (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == eOutputMode)))
    {
        pSrcInfo->u16HCusScalingDst += (pXCResourcePrivate->stdrvXC_3D._u163DHShift&0xFF);
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16HCusScalingSrc=%d, u16HCusScalingDst=%d\n", pSrcInfo->u16HCusScalingSrc, pSrcInfo->u16HCusScalingDst)
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "u16VCusScalingSrc=%d, u16VCusScalingDst=%d\n", pSrcInfo->u16VCusScalingSrc, pSrcInfo->u16VCusScalingDst)

    if(b3DPstVScaling || b3DPstHScaling)
    {
        pSrcInfo->bVCusScaling = TRUE;
        pSrcInfo->bHCusScaling = TRUE;

        // this _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        // and _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        // are unavoidable case under current scaler architecture
        // please be awared and do not remove these two unless
        // refactor the archiecture
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        MDrv_SC_set_postscaling_ratio(pInstance, pSrcInfo, eWindow);
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

        //HVSP
        if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        {
            _MLOAD_ENTRY(pInstance);
            if (eWindow == MAIN_WINDOW)
            {
                //H post-scaling
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_07_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio>>00), 0xFFFF);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_08_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio>>16), 0xFFFF);
                //V post-scaling
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_09_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio>>00), 0xFFFF);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_0A_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio>>16), 0xFFFF);
            }
            else // SUB_WINDOW
            {
                //H post-scaling
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_27_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio>>00), 0xFFFF);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio>>16), 0xFFFF);
                //V post-scaling
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio>>00), 0xFFFF);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK23_2A_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio>>16), 0xFFFF);
            }
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
        }
        else
        {
            _XC_ENTRY(pInstance);
            if (eWindow == MAIN_WINDOW)
            {
                SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio);   // H pre-scaling
                SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio);   // V pre-scaling
            }
            else // SUB_WINDOW
            {
                SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_27_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio);   // H pre-scaling
                SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio);   // V pre-scaling
            }
            _XC_RETURN(pInstance);
        }
    }
    }

//------------------------------------------------------------------------------------------------------
/// config sub win for 3d
/// @param  bEn                \b IN: input  enable sub window or disable
/// @return MS_BOOL                  \b OUT: TRUE: set is ok FALSE: set value is odd, need align to even
//------------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_3D_CfgSubWin(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(ENABLE == bEn)
    {
        MS_U32 u32tmp;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x0002, 0x0002);//Enable sub window shown on the screen
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0002, 0x0002);//Force F1 use F2's register setting
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, 0x0000, 0x0080);//source sync enable
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(MAIN_WINDOW))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L, BIT(2), BIT(2));
        }
#endif
        //MDrv_ACE_3DClonePQMap(FALSE, FALSE);
        MS_U32 u32H_PreScalingRatio, u32V_PreScalingRatio;
        u32H_PreScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L) & 0x40000000;
        u32H_PreScalingRatio |= (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PreScalingRatio & ~0x40000000);
        u32V_PreScalingRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L) & 0x40000000;
        u32V_PreScalingRatio |= (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & ~0x40000000);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_04_L, u32H_PreScalingRatio);   // H pre-scaling
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_08_L, u32V_PreScalingRatio);   // V pre-scaling
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_27_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio);  // H post-scaling
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio);  // V post-scaling

        u32tmp = (MS_U32)(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_59_L, (MS_U16)u32tmp);//frame number and opm/ipm seperate.

        u32tmp = (MS_U32)(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, (MS_U16)u32tmp);//madi format

        u32tmp = (MS_U32)(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_42_L, (MS_U16)u32tmp);

        u32tmp = (MS_U32)(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L, (MS_U16)u32tmp, 0xCFFF);//don't copy h and v mirror

        u32tmp = (MS_U32)(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, (MS_U16)u32tmp, 0xBFFF);

        u32tmp = (MS_U32)(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_45_L, (MS_U16)u32tmp, 0xFDEF);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x0000, 0x0002);//Enable sub window shown on the screen
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0000, 0x0002);//Force F1 use F2's register setting
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, 0x0080, 0x0080);//source sync disable
        //MDrv_ACE_3DClonePQMap(FALSE, FALSE);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(MAIN_WINDOW))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L, 0, BIT(2));
        }
#endif
    }

    return TRUE;
}
#endif

void MDrv_SC_3D_Disable_Crop(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
#if(HW_DESIGN_3D_VER < 1)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //only 2d to 3d can do overscan for all chips
    if((!IS_INPUT_MODE_NONE(eWindow)) && (!IS_OUTPUT_MODE_NONE())
       && (!IS_INPUT_NORMAL_2D(eWindow)) && (!IS_INPUT_NORMAL_2D_HW(eWindow))
       && (!IS_INPUT_NORMAL_2D_INTERLACE(eWindow))
       && (!IS_INPUT_NORMAL_2D_INTERLACE_PTP(eWindow)))
    {
        pSrcInfo->ScaledCropWin.x     = 0;
        pSrcInfo->ScaledCropWin.y     = 0;
        pSrcInfo->ScaledCropWin.width = pSrcInfo->u16H_SizeAfterPreScaling;
        pSrcInfo->ScaledCropWin.height= pSrcInfo->u16V_SizeAfterPreScaling;
    }
#endif
}

static void _XC_Set_3D_MiuDualModeFetchOffset(XC_RESOURCE_PRIVATE* pXCResourcePrivate, MS_U32 u32DeviceID, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (IsEnableDualMode(eWindow))
    {
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), 0x1FFF);  //F2_1 IPM offset
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), 0x1FFF);  //F2_1 IPM fetch number
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset), 0x1FFF);  //F2_1 OPM offset
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch), 0x0FFF);  //F2_1 OPM fetch number
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset), 0x1FFF);  //F2_1 OPM offset
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch), 0x0FFF);  //F2_1 OPM fetch number
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), 0x1FFF);  //F2 IPM total fetch
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
        SC_W2BYTEMSK(u32DeviceID, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
    }
#endif

}

MS_BOOL MDrv_XC_3D_LoadReg(void *pInstance, E_XC_3D_INPUT_MODE eInputMode,
                           E_XC_3D_OUTPUT_MODE eOutputMode,
                           XC_InternalStatus *pSrcInfo,
                           SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    // SC1 no 3D
    if (psXCInstPri->u32DeviceID == 1)
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (!Hal_SC_3D_IsDualViewMode(pInstance,eWindow)) // dual view mode cannot skip
#endif
        {
            return TRUE;
        }
    }
#endif
    //Get panel size output width
    MS_U32 u32OutputWidth = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L) + 1;
    MS_U32 PanelHeight= SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) - SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L) + 1;
    if(u32OutputWidth==0)
        u32OutputWidth = 0xf00;
    if(PanelHeight==0)
        PanelHeight = 0x870;

#if ((HW_DESIGN_3D_VER == 1))
    _XC_ENTRY(pInstance);
    MDrv_SC_3D_Set2TapMode(pInstance, pSrcInfo, eWindow);
    _XC_RETURN(pInstance);
#elif (HW_DESIGN_3D_VER >= 2)
    MS_BOOL b3Dstate =
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                       Hal_SC_3D_IsDualViewMode(pInstance,eWindow)  ||
#endif
                       ((!IS_INPUT_MODE_NONE(eWindow))
                       && (!IS_OUTPUT_MODE_NONE())
                       && ((!IS_INPUT_NORMAL_2D_HW(eWindow)) || IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW() ||IS_OUTPUT_FRAME_PACKING()));

    _XC_ENTRY(pInstance);
#if(HW_DESIGN_3D_VER == 5) //monaco mode
    static MS_BOOL _bCopyMainOPBase = FALSE;
    static MS_U32 _u32SubBaseAddr1 = 0x00000000;
    static MS_U32 _u32SubBaseAddr2 = 0x00000000;
#endif

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_BOOL bChangeDisplaySize = FALSE;
    MS_U16 u16VMainDisStart = 0;
    MS_U16 u16VMainDisEnd = 0;
    MS_U16 u16VSubDisStart = 0;
    MS_U16 u16VSubDisEnd = 0;
    MS_U16 u16VDispwinOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
#endif

    //First init all control register to HW init value
    if((pXCResourcePrivate->stdrvXC_3D.ePreOutputMode != E_XC_3D_OUTPUT_MODE_NONE)
       && ((eInputMode != pXCResourcePrivate->stdrvXC_3D.ePreInputMode) || (eOutputMode != pXCResourcePrivate->stdrvXC_3D.ePreOutputMode)))
    {
#if (HW_DESIGN_3D_VER == 5)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_29_L, 0x0404, 0xFFFF);//[4:0]reg_ini_max_bot_6tap,[12:8]reg_ini_max_top_6tap,should be 0x0404 in monaco 2D mode
        //Hal_SC_set_fmclk(psXCInstPri, DISABLE);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L, 0x0000, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L, 0x0000, 0x0082);//HDMI 3D field select toggle enable, OP 3D mode enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L, 0x0000, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L, 0, BIT(4));//F2 IPM 3D LR invert
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L, 0, BIT(11)|BIT(8)|BIT(1)|BIT(0));//[11]opm3d: OPM SBS using PIP

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_19_L, 0x0000, 0x8800);//clear opm/ipm frame number separate control
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L, 0x0, BIT(8));//Enable free run 3D LR flag
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_06_L, 0x0000, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0000, 0x0003);//Enable free run 3D LR flag
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_22_L, 0x0, BIT(4));//reg_ext_lr_en

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0801, 0x0013);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_18_L, 0x0000, 0x0002);//Force F1 use F2's register setting
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x0000, 0x8000);//reg_sw_tg_en
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK57_1C_L, 0x00); //Offset main/sub display window in right direction
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0, BIT(5)); //Enable HD side by side line buffer mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_18_L, 0x0, BIT(9));//fbl 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_64_L, 0x0000, BIT(5));   //pixel base active 3d
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_50_L, 0x0000, BIT(14)); //block base active 3d

        if(_bCopyMainOPBase)
        {
            _bCopyMainOPBase = FALSE;
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_50_L, _u32SubBaseAddr1);
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_52_L, _u32SubBaseAddr2);
        }
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x0000, 0x0002);//Enable sub window shown on the screen
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0000, 0x0002);//Force F1 use F2's register setting
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0000, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0000, 0x0082);//HDMI 3D field select toggle enable, OP 3D mode enable
        Hal_XC_H3D_LR_Toggle_Enable(pInstance, FALSE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);//HDMI 3D field select toggle disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_53_L, 0x0000, 0x8000);// 3D LR side-by-side to line-to-line enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_62_L, 0x0000, 0x0130);//external lr signal enable and swap
#if SUPPORT_SEAMLESS_ZAPPING
        if(Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow))
        {
            if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, BIT(0)|BIT(1)) == (BIT(0)|BIT(1)))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0000, 0xFFFC);//hw 3d TYPE, L/R split etc.
            }
        }
        else
#endif
        {
            //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0000);//hw 3d TYPE, L/R split etc.
            Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_OFF);
            Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
            Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
        }
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0000);//FPP, FPI, FIdA related control
        Hal_SC_3D_SetActiveVideoHeight(pInstance, 0x0000);
        Hal_SC_3D_SetActiveBlankSize0(pInstance, 0x0000);
        Hal_SC_3D_SetActiveBlankSize1(pInstance, 0x0000);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, 0x00, BIT(0)|BIT(2)); //[0]pix_sep_en,[2]LR_Alt_line
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6F_L, 0x00, 0x010F); //resample, [8]enable

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0, BIT(4));//F2 IPM 3D LR invert
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(11)|BIT(8)|BIT(1)|BIT(0));//[11]opm3d: OPM SBS using PIP
                                                                //[8] F2 OPM 3D LR invert
                                                                //[1] bsel_r_toggle_en
        #if 1                                                        //[0] bsel_r_inv
        ///for 3D film issue
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L, 0x0000);
        #endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x0200, 0x0300);//reg_bk_field_sel_f2

#if((HW_DESIGN_3D_VER == 3) || (HW_DESIGN_3D_VER == 5))//monaco/monet/manhattan mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,0x00, BIT(2));//reg_force_fe2_en
#endif

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_05_L, 0x0200, BIT(9));//Enable OPM F1 register
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L, 0x0000, 0x8800);//clear opm/ipm frame number separate control
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, 0x00); //Offset main/sub display window in right direction
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x0, BIT(5)); //Enable HD side by side line buffer mode
#if HW_2LINEMODE_DEINTERLACE_SUPPORT
        if(pSrcInfo->bInterlace)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0, BIT(5)); //Force DI 2-line mode at sc2lb mode
        }
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0000, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
        Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_FCLK, ENABLE, MAIN_WINDOW);//normal 2D and 3D format except checkboard should enable fclock
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0, BIT(9));//fbl 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_34_L, 0x0, BIT(15));//fbl 3d eco
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0, BIT(8));//Enable free run 3D LR flag
        Hal_SC_3D_enable_FALLRR_out(pInstance, FALSE);//disable FA LLRR out for 4k0.5k@240
#if(HW_2DTO3D_VER== 4) //monet
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, 0x0000, BIT(14)|BIT(13)|BIT(7));//TB out 3D depth enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, 0x0000, BIT(3));//SBS out 3D depth enable
#endif
#if(SUPPORT_SC0_SUB_WIN == FALSE)
        //monet top/bot 3d mode using the same buffer
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_23_L, 0, BIT(11));
        //monet has no pip
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(11), BIT(11));
#endif

#if ENABLE_T3D_DEBUG
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_65_L, 0x0000, 0x8000);//disable  principal black-bar
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4D_L, 0x0000, 0x8000);//disable  left black-bar
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4B_L, 0x0000, 0x0002);//disable Right black-bar
#endif

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        // restore sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_12_L, 0x1FFF, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_13_L, 0x1FFF, 0x1FFF);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_10_L, 0x3FFF, 0x3FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_11_L, 0x3FFF, 0x3FFF);
#endif

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        Hal_SC_3D_Set_DualView(pInstance,FALSE);
#endif

    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Windows[%u] eInputMode=%u, eOutputMode=%u, b3Dstate=%u\n", eWindow, eInputMode, eOutputMode, b3Dstate);

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
    {
        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
        {
            // force 4k2k to FSC case, need to make post-scaling to displayV*2 first
            gSrcInfo[eWindow].stDispWin.y = gSrcInfo[eWindow].stDispWin.y*2;
            gSrcInfo[eWindow].stDispWin.height = gSrcInfo[eWindow].stDispWin.height*2;
            bChangeDisplaySize = TRUE;

            // for custom scaling case
            if(gSrcInfo[eWindow].bVCusScaling)
            {
                gSrcInfo[eWindow].u16VCusScalingDst = gSrcInfo[eWindow].u16VCusScalingDst*2;
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"DispWin x: %4u  W: %4u  y: %4u  H: %4u ,u16VCusScalingDst : %u\n",
                                   gSrcInfo[eWindow].stDispWin.x,
                                   gSrcInfo[eWindow].stDispWin.width,
                                   gSrcInfo[eWindow].stDispWin.y,
                                   gSrcInfo[eWindow].stDispWin.height,
                                   gSrcInfo[eWindow].u16VCusScalingDst);
        }
    }
#endif

    if(IS_INPUT_MODE_NONE(eWindow)
       || IS_OUTPUT_MODE_NONE()
       || (IS_INPUT_NORMAL_2D_HW(eWindow) && (!IS_OUTPUT_TOP_BOTTOM_HW()) && (!IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()))
       )
    {
        //Close 3d if previous 3d state is enabled
        if(   (pXCResourcePrivate->stdrvXC_3D.ePreInputMode != E_XC_3D_INPUT_MODE_NONE)
           && (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode != E_XC_3D_OUTPUT_MODE_NONE)
           && ((pXCResourcePrivate->stdrvXC_3D.ePreInputMode != E_XC_3D_INPUT_NORMAL_2D_HW)
              || (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)
              || (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW)))
        {
            MDrv_XC_EnableCLK_for_SUB(pInstance, FALSE); //Disable 3D, so disable sub window clk here
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);


            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_MODE_NONE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1), VOP_DE_VEND_MASK);
            }

            // in 3d-->pip case, becuase sub window has hw limit, we cannot disable sub input source, there is some sw patch here.
            // make sub window back to init status. copy actions from MApi_XC_Init();
            MDrv_XC_init_fbn_win(pInstance, SUB_WINDOW);
            MDrv_XC_FilLineBuffer(pInstance, ENABLE, SUB_WINDOW);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            Hal_SC_3D_Set_DualView(pInstance,FALSE);
#endif
        }
    }
    else
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (!Hal_SC_3D_IsDualViewMode(pInstance,eWindow)) // dual view mode need to skip
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x0000, BIT(9));//Enable OPM F1 register
        }
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        if(_bCopyMainOPBase == FALSE)
        {
            _bCopyMainOPBase = TRUE;
            _u32SubBaseAddr1 = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_50_L);
            _u32SubBaseAddr2 = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_52_L);

            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_50_L,SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_10_L));
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_52_L,SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_12_L));
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_05_L, 0x0000, BIT(9));//Enable OPM F1 register
#endif
    }
    _XC_RETURN(pInstance);

    if(b3Dstate)
    {
        //input part

        //because prescaling and postscaling function will use pq functions
        //and pq will use mutex protect, so we should release mutex for these two cases.
        MDrv_SC_3D_Adjust_PreScaling(pInstance, eInputMode, eOutputMode, pSrcInfo, eWindow);
        MDrv_SC_set_crop_window(pInstance, pSrcInfo, eWindow);
        //monaco/muji 3D mode need control sub for some output format, or else the edge of object in the image may shake
#if (HW_DESIGN_4K2K_VER == 4)
        Hal_SC_set_opm_lcnt_inv(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.bOpmLcntInv_En, eWindow);
        if(IS_OUTPUT_LINE_ALTERNATIVE()
            || IS_OUTPUT_TOP_BOTTOM_HW()|| IS_OUTPUT_TOP_BOTTOM()
            || IS_OUTPUT_SIDE_BY_SIDE_HALF()|| IS_OUTPUT_SIDE_BY_SIDE_HALF_HW()
            || IS_OUTPUT_FRAME_PACKING())
        {
            Hal_SC_set_opm_lcnt_inv(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.bOpmLcntInv_En, SUB_WINDOW);
        }
#endif
        MDrv_SC_3D_Adjust_PstScaling(pInstance, eInputMode, eOutputMode, pSrcInfo, eWindow);
    }

    _XC_ENTRY(pInstance);

    if(!IS_INPUT_MODE_NONE(eWindow)
       || !IS_OUTPUT_MODE_NONE())
    {
#if(HW_6TAP_MODE_SUPPORT == FALSE)
        // sram bypass for maserati in 3D mode. PQ is 6 tap mode. Line buffer is not enough because 12 line mode is disabled in 3D mode.
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x303);
#endif
    }

    //patch of muji U01 HW bug, disable compress mode when input is interlace and 3D output is not none
#if(HW_DESIGN_3D_VER == 5) //monaco mode
    if (MDrv_ReadByte(REG_CHIP_REVISION) < 1)//muji U02 fix
    {
        if((E_XC_3D_OUTPUT_MODE_NONE != MDrv_XC_Get_3D_Output_Mode(pInstance))
            && (TRUE == pSrcInfo->bInterlace))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_01_L, 0x03, 0x03);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L, 0x03, 0x03);
        }
    }
    else
    {
        //ECO bit
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(2), BIT(2));
    }

    //patch for muji U01, other output mode don't need enable black bar when setting depth
    if (MDrv_ReadByte(REG_CHIP_REVISION) < 1)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_65_L, 0x0000, 0x8000);//principal black-bar enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4D_L, 0x0000, 0x8000);//Left black-bar enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_4B_L, 0x0000, 0x0002);//Right black-bar enable
    }
#endif

    //// The BWS mode need to enabled both Main and Sub
    if(IS_INPUT_NORMAL_2D(eWindow) && IS_OUTPUT_TOP_BOTTOM())
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_30_L, BIT(14), BIT(14));

        if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_01_L, BIT(0)))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_01_L, BIT(1), BIT(1));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_01_L, 0x00, BIT(1));
        }
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK08_30_L, 0x00, BIT(14));
    }

    if(b3Dstate)
    {
        if(pSrcInfo->bHCusScaling || pSrcInfo->bVCusScaling || pSrcInfo->Status2.bPreVCusScaling || pSrcInfo->Status2.bPreHCusScaling)
        {
        #if(HW_DESIGN_3D_VER == 5) //monaco mode
            MDrv_SC_set_frcm_fetch_number_limit(pInstance, pSrcInfo, eWindow);
        #else
            MDrv_SC_set_fetch_number_limit(pInstance, pSrcInfo, eWindow);
        #endif
        }

        if (eWindow == MAIN_WINDOW)
        {
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0);//DNR OPM base set the same
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_10_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
            if (IsEnableDualMode(eWindow))
            {
                Hal_SC_set_Dual_DNRBase0(pInstance, (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase0*BYTE_PER_WORD),  eWindow); //F2_1 IPM base addr
                Hal_SC_set_Dual_OPMBase0(pInstance, (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0*BYTE_PER_WORD), eWindow);  //F2_1 OPM base addr
            }
#endif
#if(HW_DESIGN_3D_VER == 5) //monaco mode
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase0);//DNR OPM base set the same
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_10_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0);//frcm_rbase0 for main
            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0);//frcm_rbase0 for sub
#endif
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_12_L, 0x0000, 0x1FFF);//The starting address of f2 stored at line buffer
        }
#if (HW_DESIGN_3D_VER == 4)
        if((IS_INPUT_FRAME_PACKING(eWindow)&&IS_OUTPUT_TOP_BOTTOM())||
            (IS_INPUT_FRAME_PACKING(eWindow)&&IS_OUTPUT_FRAME_L())||
            (IS_INPUT_FRAME_PACKING(eWindow)&&IS_OUTPUT_FRAME_R())||
            (IS_INPUT_LINE_ALTERNATIVE(eWindow)&&IS_OUTPUT_TOP_BOTTOM())||
            (IS_INPUT_FIELD_ALTERNATIVE(eWindow)&&IS_OUTPUT_TOP_BOTTOM())||
            (IS_INPUT_FRAME_ALTERNATIVE(eWindow)&&IS_OUTPUT_TOP_BOTTOM())||
            (IS_INPUT_TOP_BOTTOM(eWindow)&&IS_OUTPUT_TOP_BOTTOM()&&(pSrcInfo->u16H_SizeAfterPreScaling > MS_3D_LINE_BFF_MAX))
           )
        {
            //The starting address of f1 stored at line buffer
            //in A5, Main and Sub window will use the (1920+960) in 2Tap st the same time
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_13_L, 0x00, 0x1FFF);
            // When main and sub is start from 0, it means the sub win don't need Fill the sub windows line buffer in vertical blanking
            // in case the bandwidth of sub window is not enough. we will let sub window to work on v blanking area.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x00, BIT(10)); //[10]reg_vblank_main

            if (IsVMirrorMode(eWindow))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(11), BIT(11)); //[11]reg_vblank_sub
            }
            else //non mirror
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0 , BIT(11)); //[11]reg_vblank_sub
            }
        }
        else
#endif
        if(MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, pSrcInfo, eWindow))
        {
            MDrv_SC_3D_Set2TapMode(pInstance, pSrcInfo, eWindow);
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x0, BIT(5)); //Disable HD side by side line buffer mode
#if HW_2LINEMODE_DEINTERLACE_SUPPORT
            if(pSrcInfo->bInterlace)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, 0x0, BIT(5)); //Force DI 2-line mode at sc2lb mode
            }
#endif
            HAL_SC_SetSubLineBufferOffset(pInstance, MS_3D_LINE_BFF_MAX);
        }
    }

    if((!MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, eInputMode, eOutputMode, eWindow))
       && ((E_XC_3D_OUTPUT_LINE_ALTERNATIVE == eOutputMode)
           || (E_XC_3D_OUTPUT_TOP_BOTTOM == eOutputMode)
           || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == eOutputMode)
           || (E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == eOutputMode)
           || (E_XC_3D_OUTPUT_FRAME_PACKING== eOutputMode)))
    {
        #if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, pXCResourcePrivate->stdrvXC_3D._u163DHShift & ~0x01);//Main/sub display window offset in right direction
        #else
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK20_1C_L, pXCResourcePrivate->stdrvXC_3D._u163DHShift);//Main/sub display window offset in right direction
        #endif
    }

    //normal 2d or hw 2d to 3d top bottom output or hw 2d to 3d side by side output
    if(IS_INPUT_NORMAL_2D(eWindow)
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
       || Hal_SC_3D_IsDualViewMode(pInstance,eWindow)
#endif
       || (IS_INPUT_NORMAL_2D_HW(eWindow) && (IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW() || IS_OUTPUT_FRAME_PACKING())))
    {
        if(IS_INPUT_NORMAL_2D(eWindow))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is 2D\n");
#if(HW_DESIGN_3D_VER >= 3) //mooney/monet mode
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
        }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (!Hal_SC_3D_IsDualViewMode(pInstance,eWindow)) // dual view mode need to skip
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0800, 0x0800);//opm3d: OPM SBS using PIP
        }
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);
/////------------------
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//F1 OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L, 0x0800, 0x0800);//opm3d: OPM SBS using PIP
#endif
    }
    else if(IS_INPUT_FRAME_PACKING(eWindow))
    {
        MS_U16 u16VactVideo, u16VactSpace;
#if (HW_DESIGN_3D_VER >= 3) // for A5
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, DISABLE);
#else
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//F1 OPM FETCH

        if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
        {
            //interlace framepacking

            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0113);//reg_lr_chg_mode[1:0],reg_ini_lr[4],reg_split_half[8]
            Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
            Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
            Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0237, 0xff37);//Blanking number in one vsync,enable gen pseudo vsync in 3d
            Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
            Hal_SC_3D_SetActiveBlankSize0(pInstance,DOUBLEHD_1080X2I_VACT_SPACE1);
            Hal_SC_3D_SetActiveBlankSize1(pInstance, DOUBLEHD_1080X2I_VACT_SPACE2);
            Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, u16VactVideo);
            Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/2);
            Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(u16VactVideo*2 - 1));
            Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,u16VactVideo*2);
            Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(4*u16VactVideo - 1));

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, u16VactVideo*2, 0x0FFF);//active_hight  after VSD
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, u16VactVideo*2, 0x0FFF);//active_hight  after VSD
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x0100, 0x0100);//reg_bk_field_sel_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0A00, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0002, 0x0003);//reg_lr_chg_mode
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0000, 0x0010);//reg_lr_chg_mode

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0003, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en,[11]:reg_vblank_sub

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, BIT(15), BIT(15));//reg_vlen_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x0010, 0x3FFF);//reg_htot
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x000a, 0x3FFF);//reg_hend_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x0002, 0x3FFF);//reg_hst_f1
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x000a, 0x3FFF);//reg_hend_f1
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
        }
        else
        {
            //progressive framepacking

            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0113);//hw 3d TYPE, L/R split
            Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
            Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
            Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x011, 0x0037);//Blanking number in one vsync,enable gen pseudo vsync in 3d
            Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
            Hal_SC_3D_SetActiveBlankSize0(pInstance,u16VactSpace);
            Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, u16VactVideo);
            Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/2);
            Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(u16VactVideo - 1));
            Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,u16VactVideo);
            Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(2*u16VactVideo - 1));

                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, u16VactVideo, 0x0FFF);//active_hight  after VSD
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, u16VactVideo, 0x0FFF);//active_hight  after VSD
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0002, 0x0013);//[2:0]reg_lr_chg_mode,[4]:reg_ini_lr

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0003, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, BIT(15), BIT(15));//reg_vlen_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x0010, 0x3FFF);//reg_htot
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x000a, 0x3FFF);//reg_hend_f2
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x0002, 0x3FFF);//reg_hst_f1
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x000a, 0x3FFF);//reg_hend_f1
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
        }
#elif (HW_DESIGN_3D_VER >= 3)
            if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
            {
                //interlace framepacking

                Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
                Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
                MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0237, 0xff37);//Blanking number in one vsync,enable gen pseudo vsync in 3d
                Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
                Hal_SC_3D_SetActiveBlankSize0(pInstance,DOUBLEHD_1080X2I_VACT_SPACE1);
                Hal_SC_3D_SetActiveBlankSize1(pInstance, DOUBLEHD_1080X2I_VACT_SPACE2);
                Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
                Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, u16VactVideo);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x0100, 0x0100);//reg_bk_field_sel_f2
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0A00, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
#if(HW_DESIGN_3D_VER == 3) //monet mode
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is FPP\n");
                //progressive framepacking

                //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0103);//hw 3d TYPE, L/R split
                Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
                Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0011, 0x0037);//Blanking number in one vsync
                #if (HW_DESIGN_3D_VER < 4)
                MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
                #endif
                Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
                Hal_SC_3D_SetActiveBlankSize0(pInstance,u16VactSpace);
                Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);

                #if (HW_DESIGN_3D_VER >= 4)
                    Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen);
                #endif
                    Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, u16VactVideo);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0900, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
#if(HW_DESIGN_3D_VER == 3) //monet mode
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
            }
#else
            if(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
            {
                //interlace framepacking

                //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0103);//hw 3d TYPE, L/R split
                Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
                Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0237, 0xff37);//Blanking number in one vsync,enable gen pseudo vsync in 3d
                Hal_SC_3D_SetActiveVideoHeight(pInstance,DOUBLEHD_1080X2I_FIELD_VSIZE);
                Hal_SC_3D_SetActiveBlankSize0(pInstance,DOUBLEHD_1080X2I_VACT_SPACE1);
                Hal_SC_3D_SetActiveBlankSize1(pInstance, DOUBLEHD_1080X2I_VACT_SPACE2);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x0100, 0x0100);//reg_bk_field_sel_f2
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0A00, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
            }
            else
            {
                //progressive framepacking

                //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0103);//hw 3d TYPE, L/R split
                Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
                Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0011, 0x0037);//Blanking number in one vsync
                Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
                Hal_SC_3D_SetActiveBlankSize0(pInstance,u16VactSpace);

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0900, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
            }
#endif
    }
    else if(IS_INPUT_FIELD_ALTERNATIVE(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is FA\n");
        MS_U16 u16VactVideo, u16VactSpace;

        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, DISABLE);
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0011, 0x0037);//Blanking number in one vsync
        Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
        Hal_SC_3D_SetActiveBlankSize0(pInstance,u16VactSpace);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
        Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen);
        Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, u16VactVideo);
        Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(u16VactVideo - 1));
        Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,u16VactVideo);
        Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(2*u16VactVideo - 1));

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, u16VactVideo, 0x0FFF);//active_hight  after VSD
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, u16VactVideo, 0x0FFF);//active_hight  after VSD

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0x0000, 0x0100);//reg_local_field
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0002, 0x0003);//reg_lr_chg_mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0003, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, BIT(15), BIT(15));//reg_vlen_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x0010, 0x3FFF);//reg_htot
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x000a, 0x3FFF);//reg_hend_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x0002, 0x3FFF);//reg_hst_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x000a, 0x3FFF);//reg_hend_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
#else
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0103);//hw 3d TYPE, L/R split
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x0011, 0x0037);//Blanking number in one vsync
        Hal_SC_3D_SetActiveVideoHeight(pInstance,u16VactVideo);
        Hal_SC_3D_SetActiveBlankSize0(pInstance,u16VactSpace);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);

        #if (HW_DESIGN_3D_VER >= 3)
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen);
        #endif

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_06_L, 0x0A00, 0x0F00);//[11]Enable, [10:8]F2 IPM user define read
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#endif
    }
    else if(IS_INPUT_FRAME_ALTERNATIVE(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is FA\n");

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_62_L, 0x0030, 0x0030);// input Vsync exist between L and R frame

        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0001, 0x0013);// hw 3d TYPE, L/R split=0
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_FRAME);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, pSrcInfo->u16V_SizeAfterPreScaling, 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, pSrcInfo->u16V_SizeAfterPreScaling, 0x0FFF);//active_hight  after VSD

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));//F2 IPM 3D input enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));//OP 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L, BIT(7), BIT(7));

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_22_L, BIT(4), BIT(4));//reg_ext_lr_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0001, 0x0013);//reg_lr_chg_mode

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0001, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x0000, 0x1000);//reg_sw_tg_en

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//F1 OPM FETCH
#elif (HW_DESIGN_3D_VER >= 3)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0));// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_62_L, 0x0030, 0x0030);// input Vsync exist between L and R frame

        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0001, 0x0103);// hw 3d TYPE, L/R split=0
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_FRAME);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#else
        if(pSrcInfo->bInterlace == TRUE)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);// 3d enable
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0083, 0x0080);//[8] F2 OPM 3D LR invert
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_62_L, 0x0010, 0x0010);// input Vsync exist between L and R frame

            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0001, 0x0103);// hw 3d TYPE, L/R split=0
            Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_FRAME);
            Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
        }
        else
        {
            //for progressive frame alternative case, we should disable ip 3d, enable op 3d
            //to let main,sub engine read opm frame data alternatively
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0083, 0x0080);//[8] F2 OPM 3D LR invert
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L, 0x8804);//input 4 frame, output 2 frame with 3d
        }
#endif
    }
    else if(IS_INPUT_TOP_BOTTOM(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is TBP\n");

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK32_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0002, 0x0113);//hw 3d TYPE, L/R split
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
        Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->stCapWin.height);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);

        if(pSrcInfo->bInterlace == FALSE)
        {
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/2);
        }
        else
        {
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/4);
        }
        Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/2);
        Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling/2 - 1));
        Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,pSrcInfo->u16V_SizeAfterPreScaling/2);
        Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, pSrcInfo->u16V_SizeAfterPreScaling/2, 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, pSrcInfo->u16V_SizeAfterPreScaling/2, 0x0FFF);//active_hight  after VSD

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0002, 0x0013);//reg_lr_chg_mode

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0003, 0x0C03);//reg_hst_f2, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x8010, 0xBFFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x000a, 0x3FFF);//reg_hend_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x0002, 0x3FFF);//reg_hst_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x000a, 0x3FFF);//reg_hend_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));

#elif (HW_DESIGN_3D_VER >= 3)

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);
        Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->stCapWin.height);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);

        if(pSrcInfo->bInterlace == FALSE)
        {
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/2);
        }
        else
        {
            Hal_SC_3D_SetActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling/4);
        }

        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);

        if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, BIT(5)) == BIT(5))
        {
            // If use 2Tap mode, need to disable PQ scaling filter
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x303);
        }
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);

        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_BLOCK);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
        if(pSrcInfo->bInterlace == FALSE)
        {
            Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->u16V_SizeAfterPreScaling/2);
        }
        else
        {
            Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->u16V_SizeAfterPreScaling/4);
        }


#endif
    }
    else if(IS_INPUT_LINE_ALTERNATIVE(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is LA\n");
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        _XC_Set_3D_MiuDualModeFetchOffset(pXCResourcePrivate, psXCInstPri->u32DeviceID, eWindow);
#if(HW_DESIGN_3D_VER == 5) //monaco mode

        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0003, 0x0113);//hw 3d TYPE, L/R idx
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetSplitHalf(pInstance, FALSE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
        Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling);
        Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling/2 - 1));
        Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,0x0000);
        Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling/2 - 1));

        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, (pSrcInfo->u16V_SizeAfterPreScaling/2), 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, (pSrcInfo->u16V_SizeAfterPreScaling/2), 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);//OP 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0003, 0x0013);//reg_lr_chg_mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0C03, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_29_L, 0x0000, 0xFFFF);//[4:0]reg_ini_max_bot_6tap,[12:8]reg_ini_max_top_6tap,should be 0 in monaco 3D mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x8012, 0xBFFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x0009, 0x3FFF);//reg_hend_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x000a, 0x3FFF);//reg_hst_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x0011, 0x3FFF);//reg_hend_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_02_L,BIT(3), BIT(3));//reg_op1_frcm_cnnet_spthalf_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_18_L, pSrcInfo->stCapWin.height*2, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch, 0x1FFF);//F1 OPM FETCH

#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);// 3d enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);//OP 3D ENABLE

        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0003, 0x0013);//hw 3d TYPE, L/R idx
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width);
#if (HW_DESIGN_3D_VER >= 3)
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#endif
#endif
    }
    else if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow) || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is SBSH\n");

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset/2 + 1) & ~0x1;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//F1 OPM OFFSET
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
#ifdef UFO_XC_SUPPORT_3D_DS
            if(MDrv_XC_Is_SupportSWDS(pInstance))
            {
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
            }
            else
#endif
            {
            SC_W2BYTEMSK(0, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), 0x1FFF);  //F2_1 IPM offset
            SC_W2BYTEMSK(0, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), 0x1FFF);  //F2_1 IPM fetch number
            SC_W2BYTEMSK(0, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(0, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
        }
        }
#endif
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pSrcInfo->stCapWin.height*2, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, 0x1FFF);//F1 OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset/2 + 1) & ~0x1, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_18_L, pSrcInfo->stCapWin.height*2, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset/2 + 1) & ~0x1, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch+ 1) & ~0x1, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch+ 1) & ~0x1, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK32_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch+ 1) & ~0x1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0103, 0x0113);//hw 3d TYPE
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetSplitHalf(pInstance, TRUE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
            Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->stCapWin.height*2);
        }
        Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling);
        Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));
        Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,0x0000);
        Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, (pSrcInfo->u16V_SizeAfterPreScaling), 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, (pSrcInfo->u16V_SizeAfterPreScaling), 0x0FFF);//active_hight  after VSD

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);//IPM 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);//OP 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0003, 0x0013);//reg_lr_chg_mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0C03, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_29_L, 0x0000, 0xFFFF);//[4:0]reg_ini_max_bot_6tap,[12:8]reg_ini_max_top_6tap,should be 0 in monaco 3D mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x8012, 0xBFFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x0009, 0x3FFF);//reg_hend_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x000a, 0x3FFF);//reg_hst_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x0011, 0x3FFF);//reg_hend_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_02_L,BIT(3), BIT(3));//reg_op1_frcm_cnnet_spthalf_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
#else

#ifdef UFO_XC_FB_LEVEL
        if((pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FBL) || (pSrcInfo->bR_FBL == E_XC_FB_LEVEL_RFBL_DI))
#else
        if(pSrcInfo->bFBL || pSrcInfo->bR_FBL)
#endif
        {
            if(MDrv_SC_3D_IsSupportFBL3D())
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = pSrcInfo->u16H_SizeAfterPreScaling / 2;

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pSrcInfo->u16H_SizeAfterPreScaling / 2, 0x1FFF);//OPM FETCH
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pSrcInfo->u16H_SizeAfterPreScaling / 2, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(REG_SC_BK12_57_L, pSrcInfo->u16H_SizeAfterPreScaling / 2), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
                if(IsEnableDualMode(eWindow))
                {
#ifdef UFO_XC_SUPPORT_3D_DS
                    if(MDrv_XC_Is_SupportSWDS(pInstance))
                    {
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 IPM total fetch
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_29_L, (MS_U16)((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch)/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_69_L, (MS_U16)((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch)/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 Dual sub OPM total fetch

                    }
                    else
#endif
                    {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch/2 + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch/2 + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), 0x1FFF);  //F2 IPM total fetch
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_29_L, (MS_U16)((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch)/2 + 1) & ~0x1, 0x1FFF);  //F2 OPM total fetch
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_69_L, (MS_U16)((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch)/2 + 1) & ~0x1, 0x1FFF);  //F2 Dual sub OPM total fetch
                }
                }
#endif
#ifdef UFO_XC_SUPPORT_3D_DS
                if(MDrv_XC_Is_SupportSWDS(pInstance))
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK20_13_L, pSrcInfo->u16H_SizeAfterPreScaling/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                }
                else
#endif
                {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_13_L, pSrcInfo->u16H_SizeAfterPreScaling / 2, 0x1FFF);
                }
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, BIT(9), BIT(9));//fbl 3d enable
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_34_L, BIT(15), BIT(15));//fbl 3d eco
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"3D: not support 3d under FBL!\n");
            }
        }
        else
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1;

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, 0x1FFF);//OPM FETCH
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
            if(IsEnableDualMode(eWindow))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), 0x1FFF);  //F2 IPM total fetch
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch + 1) & ~0x1, 0x1FFF);  //F2 OPM total fetch
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch + 1) & ~0x1, 0x1FFF);  //F2 Dual sub OPM total fetch
            }
#endif

            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0103, 0x0103);//hw 3d TYPE
            Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
            Hal_SC_3D_SetSplitHalf(pInstance, TRUE);
            Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);//IPM 3D ENABLE
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);//OP 3D ENABLE
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, 0x00, BIT(15));
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, BIT(15), BIT(15));
#if (HW_DESIGN_3D_VER >= 3)
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
#endif
        }
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#endif
    }
    else if(IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is SBSF\n");

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset + 1) & ~0x1, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset + 1) & ~0x1, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
#ifdef UFO_XC_SUPPORT_3D_DS
            if(MDrv_XC_Is_SupportSWDS(pInstance))
            {
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 IPM total fetch
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch

            }
            else
#endif
            {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), 0x1FFF);  //F2_1 IPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), 0x1FFF);  //F2_1 IPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), 0x1FFF);  //F2 IPM total fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
        }
        }
#endif

        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0103, 0x0103);//hw 3d TYPE
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetSplitHalf(pInstance, TRUE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, 0x0001, 0x0001);//IPM 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x0080, 0x0080);//OP 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, 0x00, BIT(15));
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, BIT(15), BIT(15));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
#if (HW_DESIGN_3D_VER >= 3)

        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
#endif
    }
    else if(IS_INPUT_CHECK_BOARD(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is CKB\n");

        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset/2 + 1) & ~0x1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  + 1) & ~0x1;

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
#ifdef UFO_XC_SUPPORT_3D_DS
            if(MDrv_XC_Is_SupportSWDS(pInstance))
            {
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 IPM total fetch
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch

            }
            else
#endif
            {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), 0x1FFF);  //F2_1 IPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), 0x1FFF);  //F2_1 IPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), 0x1FFF);  //F2 IPM total fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
        }
        }
#endif

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0103, 0x0113);//hw 3d TYPE
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetSplitHalf(pInstance, TRUE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
            Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->stCapWin.height*2);
        }
        Hal_SC_3D_SetPixelSeparationWidth(pInstance, pSrcInfo->stCapWin.width/2);
        Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling);
        Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));
        Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,0x0000);
        Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, (pSrcInfo->u16V_SizeAfterPreScaling), 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, (pSrcInfo->u16V_SizeAfterPreScaling), 0x0FFF);//active_hight  after VSD

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, 0x0005, 0x0005); //[0]pix_sep_en,[2]LR_Alt_line
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6F_L, 0x0109, 0x010F); //resample line phase, [8]enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6D_L, 0x8000, 0xFFFF); //resample phase0 coef0/1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6E_L, 0x4000, 0xFFFF); //resample phase1 coef0/1

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0)); //IPM 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7)); //OP 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pSrcInfo->stCapWin.height*2, 0x1FFF);
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset/2 + 1) & ~0x1;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch+ 1) & ~0x1, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch+ 1) & ~0x1, 0x1FFF);//F1 OPM FETCH

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0003, 0x0013);//reg_lr_chg_mode

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0C03, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_29_L, 0x0000, 0xFFFF);//[4:0]reg_ini_max_bot_6tap,[12:8]reg_ini_max_top_6tap,should be 0 in monaco 3D mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x8012, 0xBFFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x0009, 0x3FFF);//reg_hend_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x000a, 0x3FFF);//reg_hst_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x0011, 0x3FFF);//reg_hend_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_02_L,BIT(3), BIT(3));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);

#else

#if (HW_DESIGN_3D_VER >= 3)
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
#if(HW_DESIGN_3D_VER == 3) //monet mode
        //chcckboard use DI linebuffer, need disable fclock
        Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_IDCLK2, ENABLE, MAIN_WINDOW);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#endif
        Hal_SC_3D_SetPixelSeparationWidth(pInstance, pSrcInfo->stCapWin.width/2);
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetSplitHalf(pInstance, TRUE);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, BIT(0)|BIT(2), BIT(0)|BIT(2)); //[0]pix_sep_en,[2]LR_Alt_line


        // TODO: Need to check how to setup resample
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_6D_L, 0x8000); //resample phase0 coef0/1
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_6E_L, 0x4000); //resample phase1 coef0/1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6F_L, 0x0109, 0x010F); //resample line phase, [8]enable

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0)); //IPM 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7)); //OP 3D ENABLE
        Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_IDCLK2, ENABLE, MAIN_WINDOW);//chcckboard use DI linebuffer, need disable fclock
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, 0x00, BIT(15));
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, BIT(15), BIT(15));

#endif
    }
    else if(IS_INPUT_PIXEL_ALTERNATIVE(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is pixel alternative\n");

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset/2 + 1) & ~0x1;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch + 1) & ~0x1, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch + 1) & ~0x1, 0x1FFF);//F1 OPM FETCH
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK12_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch + 1) & ~0x1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
        }
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
#ifdef UFO_XC_SUPPORT_3D_DS
            if(MDrv_XC_Is_SupportSWDS(pInstance))
            {
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 IPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM offset
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2_1 OPM fetch number
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 IPM total fetch
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), DS_IP,DS_XC,&PSTXC_DS_CMDCNT);  //F2 OPM total fetch

            }
            else
#endif
            {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_0E_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset), 0x1FFF);  //F2_1 IPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_0F_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch), 0x1FFF);  //F2_1 IPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_16_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_17_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_56_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset/2 + 1) & ~0x1, 0x1FFF);  //F2_1 OPM offset
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK52_57_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + 1) & ~0x1, 0x0FFF);  //F2_1 OPM fetch number
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_28_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch), 0x1FFF);  //F2 IPM total fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_29_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_69_L, (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch + pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch), 0x1FFF);  //F2 OPM total fetch
        }
        }
#endif

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0103, 0x0113);//hw 3d TYPE
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetInitialLRIndex(pInstance, FALSE);
        Hal_SC_3D_SetSplitHalf(pInstance, TRUE);
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
            Hal_SC_3D_SetActiveVideoHeight(pInstance,pSrcInfo->stCapWin.height*2);
        }

        Hal_SC_3D_SetPixelSeparationWidth(pInstance, pSrcInfo->stCapWin.width/2);
        Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(pInstance, pSrcInfo->u16V_SizeAfterPreScaling);
        Hal_SC_3D_SetSoftware_F2VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));
        Hal_SC_3D_SetSoftware_F1VBottomStartPosition(pInstance,0x0000);
        Hal_SC_3D_SetSoftware_F1VBottomEndPosition(pInstance,(pSrcInfo->u16V_SizeAfterPreScaling - 1));

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, (pSrcInfo->u16V_SizeAfterPreScaling), 0x0FFF);//active_hight  after VSD
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_16_L, (pSrcInfo->u16V_SizeAfterPreScaling), 0x0FFF);//active_hight  after VSD

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, 0x0001, 0x0005); //[0]pix_sep_en,[2]LR_Alt_line

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6F_L, 0x0105, 0x010F); //resample line phase, [8]enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6D_L, 0x8000, 0xFFFF); //resample phase0 coef0/1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6E_L, 0x4000, 0xFFFF); //resample phase1 coef0/1

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0)); //IPM 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7)); //OP 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pSrcInfo->stCapWin.height*2, 0x1FFF);
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0E_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset, 0x1FFF);//dnr offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_0F_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch, 0x1FFF);//dnr fetch
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset/2 + 1) & ~0x1;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//OPM offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_56_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset, 0x1FFF);//F1 OPM OFFSET
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_17_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch + 1) & ~0x1, 0x1FFF);//OPM FETCH
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_57_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch+ 1) & ~0x1, 0x1FFF);//F1 OPM FETCH

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_23_L, 0x0003, 0x0013);//reg_lr_chg_mode

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_10_L, 0x0C03, 0x0C03);//[0]:reg_main_en, [1]:reg_sw_sub_en, [11]:reg_vblank_sub
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_29_L, 0x0000, 0xFFFF);//[4:0]reg_ini_max_bot_6tap,[12:8]reg_ini_max_top_6tap,should be 0 in monaco 3D mode

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_47_L, 0x8012, 0xBFFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_48_L, 0x0002, 0x3FFF);//reg_hst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_49_L, 0x0009, 0x3FFF);//reg_hend_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4A_L, 0x000a, 0x3FFF);//reg_hst_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4B_L, 0x0011, 0x3FFF);//reg_hend_f1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_4C_L, 0x0000, 0x0FFF);//reg_vst_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_02_L,BIT(3), BIT(3));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_30_L,BIT(0), BIT(0));//reg_ipm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L,BIT(7), BIT(7));//reg_opm_3d_en
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_41_L,BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_18_L, pSrcInfo->stCapWin.height*2, 0x1FFF);

#else
        Hal_SC_3D_SetActiveVideoWidth(pInstance, pSrcInfo->stCapWin.width/2);
        Hal_SC_3D_SetPixelSeparationWidth(pInstance, pSrcInfo->stCapWin.width/2);
#if (HW_DESIGN_3D_VER >= 3)
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(12), BIT(12));
#if(HW_DESIGN_3D_VER == 3) //monet mode
        //chcckboard use DI linebuffer, need disable fclock
        Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_IDCLK2, ENABLE, MAIN_WINDOW);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#endif

        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_63_L, 0x0103, 0x0103);//hw 3d TYPE
        Hal_SC_3D_SetLRChgMode(pInstance, E_HAL_SC_3D_LRCHGMODE_LINE);
        Hal_SC_3D_SetSplitHalf(pInstance, TRUE);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, BIT(0), BIT(0)|BIT(2)); //[0]pix_sep_en,[2]LR_Alt_line


        // TODO: Need to check how to setup resample
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_6D_L, 0x8000); //resample phase0 coef0/1
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_6E_L, 0x4000); //resample phase1 coef0/1
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6F_L, 0x0105, 0x010F); //resample line phase, [8]enable

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(0), BIT(0)); //IPM 3D ENABLE
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(7), BIT(7)); //OP 3D ENABLE

        Hal_SC_set_ficlk2(pInstance, CKG_FICLK2_F2_IDCLK2, ENABLE, MAIN_WINDOW);//chcckboard use DI linebuffer, need disable fclock;

        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = pSrcInfo->stCapWin.height*2;
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, 0x00, BIT(15));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_18_L, BIT(15), BIT(15));

#endif
    }

    //below is output part
    if(IS_OUTPUT_FRAME_ALTERNATIVE())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is FA\n");
        {
#if (HW_DESIGN_3D_VER == 4)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_960x1080)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                               +(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd-pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart)/2, VOP_DISPLAY_HEND_MASK);   // Display H end
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 - 1), VOP_DE_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1), VOP_DE_VEND_MASK);
            }
            else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1920x540)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd-pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart)/2, VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2 - 1), VOP_DE_VEND_MASK);
            }
            else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1280x720)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1), VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1), VOP_DE_VEND_MASK);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1), VOP_DE_VEND_MASK);
            }
#else
            //if output is FP, do not need sub window
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
#endif
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_30_L, BIT(8), BIT(8));//Enable free run 3D LR flag
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## stDBreg.u16VLen =%x\n", pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen);
#if (HW_DESIGN_3D_VER == 4)
        if(IS_INPUT_TOP_BOTTOM(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen/2;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for second channel
        }
        else
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for second channel
        }
        Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, FALSE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);//HDMI 3D field select toggle enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
        // TODO: need to double check
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, FALSE, BIT(0)); // False: L flag is first, True: R flag is first
        //Set SRAM to linear mode
        if (IsVMirrorMode(eWindow))
        {
           SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x303);
        }

        if(pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA)//output FA_LLRR for 4k0.5k@240Hz
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is FA LLRR  \n");
            Hal_SC_3D_enable_FALLRR_out(pInstance, TRUE);
#if (HW_DESIGN_3D_VER == 3) && (HW_DESIGN_4K2K_VER != 7) //manhattan mode
        // 4k2k needs more time to train data so we need to delay the display time
        pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = VALUE_AUTO_TUNE_AREA_TRIG_4K540P;
        pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG] = VALUE_DISP_AREA_TRIG_4K540P;
        pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = TRUE;
        pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] = TRUE;
#endif
        }

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        Hal_SC_set_fmclk(psXCInstPri, ENABLE);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(6), BIT(6));//[6]:reg_opm_ext_lr_sel
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, BIT(1), BIT(1));//reg_force_f2
#endif
#if(HW_2DTO3D_VER == 4)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_64_L, BIT(5), BIT(5));   //pixel base active 3d
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_50_L, BIT(14), BIT(14)); //block base active 3d
#endif

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
#endif

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if (Hal_SC_3D_IsDualViewMode(pInstance,eWindow))
    {
        Hal_SC_3D_Set_DualView(pInstance,TRUE);
    }
#endif
    }
    else if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_TOP_BOTTOM_HW())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is TBP\n");
        MS_U16 u16VactVideo, u16VactSpace;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, DISABLE);

#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            if (IsVMirrorMode(eWindow))
            {
#ifdef TBP_1920X2160_OUTPUT
                if((IS_INPUT_FRAME_PACKING(eWindow))&&
                ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
                (pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd*2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height-1, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd*2-1, MAIN_WINDOW);
                    }
#endif
                }
                else
#endif
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VSubDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                            u16VSubDisEnd = u16VSubDisStart + pSrcInfo->stDispWin.height/2-1;
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, u16VSubDisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, u16VSubDisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                        else
                        {
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                    }
                    else
#endif
                    {
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    }

                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart
                                                                                     + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2 + pSrcInfo->stDispWin.height/2) - 1;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2 \
                                                      + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
                            u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height/2 -1;
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, u16VMainDisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, u16VMainDisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                        else if(IS_OUTPUT_4K2K_60HZ_PANEL())
                        {
                            if(pXCResourcePrivate->stdrvXC_3D._bLAToTB)
                            {
                                u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                                u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height -1;
                                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, u16VMainDisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                                if (u16VMainDisStart == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
                                {
                                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, u16VMainDisEnd+1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); //"+1" is hw bug
                                }
                                else
                                {
                                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, u16VMainDisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                                }
                            }
                        }
                        else
                        {
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                    }
                    else
#endif
                    {
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                        {
                            if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                            {
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, u16VSubDisStart, u16VSubDisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, u16VMainDisStart, u16VMainDisEnd, MAIN_WINDOW);
                            }
                            else
                            {
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd + pSrcInfo->stDispWin.height/2-1, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                            }
                        }
                        else
                        {
                            MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                            MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd + pSrcInfo->stDispWin.height/2-1, SUB_WINDOW);
                            MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                            MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                        }
                    }
#endif
                }

            }
            else
            {
#ifdef TBP_1920X2160_OUTPUT
                if((IS_INPUT_FRAME_PACKING(eWindow))&&
                ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
                (pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd*2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd*2-1, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height-1, MAIN_WINDOW);
                    }
#endif
                }
                else
#endif
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart
                                                                                     + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2 + pSrcInfo->stDispWin.height/2) - 1;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VSubDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2 \
                                                      + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height -1;
                            u16VSubDisEnd = u16VSubDisStart + pSrcInfo->stDispWin.height/2 -1;
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, u16VSubDisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, u16VSubDisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                        else
                        {
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                    }
                    else
#endif
                    {
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    }

                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                            u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height/2-1;
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, u16VMainDisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, u16VMainDisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                        else if(IS_OUTPUT_4K2K_60HZ_PANEL())
                        {
                            if(pXCResourcePrivate->stdrvXC_3D._bLAToTB)
                            {
                                u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                                u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height -1;
                                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, u16VMainDisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                                if (u16VMainDisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
                                {
                                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, u16VMainDisEnd+1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); //"+1" is hw bug
                                }
                                else
                                {
                                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, u16VMainDisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                                }
                            }
                        }
                        else
                        {
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                        }
                    }
                    else
#endif
                    {
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                        Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                        {
                            if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                            {
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, u16VSubDisStart, u16VSubDisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, u16VMainDisStart, u16VMainDisEnd, MAIN_WINDOW);
                            }
                            else
                            {
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                                MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, MAIN_WINDOW);
                            }
                        }
                        else
                        {
                            MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                            MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                            MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                            MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, MAIN_WINDOW);
                        }
                    }
#endif
                }
            }
        }
        else
#endif
        {
            if (IsVMirrorMode(eWindow))
            {
#ifdef TBP_1920X2160_OUTPUT
                if((IS_INPUT_FRAME_PACKING(eWindow))&&
                ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
                (pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF); // Display H start
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);   // Display H end
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//Sub window display window
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height-1, 0x1FFF);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK);//Main window display window
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height, VOP_DISPLAY_VSTART_MASK);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd*2-1, VOP_DISPLAY_VEND_MASK);
                }
                else
#endif
                {

                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF); // Display H start
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);   // Display H end
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VSubDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                            u16VSubDisEnd = u16VSubDisStart + pSrcInfo->stDispWin.height/2-1;
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, u16VSubDisStart, VOP_DISPLAY_VSTART_MASK);//Sub window display window
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, u16VSubDisEnd, VOP_DISPLAY_VEND_MASK);
                        }
                        else
                        {
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//Sub window display window
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, 0x1FFF);
                        }
                    }
                    else
#endif
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//Sub window display window
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, 0x1FFF);
                    }

                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK);//Main window display window
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart
                                                                                     + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2 + pSrcInfo->stDispWin.height/2) - 1;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2 \
                                                      + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
                            u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height/2 -1;
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, u16VMainDisStart, 0x1FFF);
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16VMainDisEnd, 0x1FFF);
                        }
                        else if(IS_OUTPUT_4K2K_60HZ_PANEL())
                        {
                            if(pXCResourcePrivate->stdrvXC_3D._bLAToTB)
                            {
                                u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                                u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height -1;
                                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, u16VMainDisStart, VOP_DISPLAY_VEND_MASK);
                                if (u16VMainDisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
                                {
                                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16VMainDisEnd+1, VOP_DISPLAY_VEND_MASK); //"+1" is hw bug
                                }
                                else
                                {
                                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16VMainDisEnd, VOP_DISPLAY_VEND_MASK);
                                }
                            }
                        }
                        else
                        {
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, VOP_DISPLAY_VSTART_MASK);
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
                        }
                    }
                    else
#endif
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, VOP_DISPLAY_VSTART_MASK);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
                    }
                }

            }
            else
            {
#ifdef TBP_1920X2160_OUTPUT
                if((IS_INPUT_FRAME_PACKING(eWindow))&&
                ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
                (pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);//Sub window display window
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height, 0x1FFF);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd*2-1, 0x1FFF);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height-1, VOP_DISPLAY_VEND_MASK);
                }
                else
#endif
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);//Sub window display window
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart
                                                                                     + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2 + pSrcInfo->stDispWin.height/2) -1;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VSubDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2 \
                                                      + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
                            u16VSubDisEnd = u16VSubDisStart + pSrcInfo->stDispWin.height/2 -1;
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, u16VSubDisStart, 0x1FFF);
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, u16VSubDisEnd, 0x1FFF);
                        }
                        else
                        {
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, 0x1FFF);
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
                        }
                    }
                    else
#endif
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2, 0x1FFF);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
                    }

                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end

                    if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_TOP_TOP)
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(10), BIT(10));
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(8));
                    }
                    else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_BOTTOM_BOTTOM)
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(10), BIT(10));
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(8), BIT(8));
                    }
                    else
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(10));
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(8));                    
                    }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    {
                        if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
                        {
                            u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                            u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height/2-1;
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, u16VMainDisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16VMainDisEnd, VOP_DISPLAY_VEND_MASK);
                        }
                        else if(IS_OUTPUT_4K2K_60HZ_PANEL())
                        {
                            if(pXCResourcePrivate->stdrvXC_3D._bLAToTB)
                            {
                                u16VMainDisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart +  u16VDispwinOffset*2;
                                u16VMainDisEnd = u16VMainDisStart + pSrcInfo->stDispWin.height -1;
                                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, u16VMainDisStart, VOP_DISPLAY_VEND_MASK);
                                if (u16VMainDisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
                                {
                                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16VMainDisEnd+1, VOP_DISPLAY_VEND_MASK); //"+1" is hw bug
                                }
                                else
                                {
                                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, u16VMainDisEnd, VOP_DISPLAY_VEND_MASK);
                                }
                            }
                        }
                        else
                        {
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, VOP_DISPLAY_VEND_MASK);
                        }
                    }
                    else
#endif
                    {
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + pSrcInfo->stDispWin.height/2-1, VOP_DISPLAY_VEND_MASK);
                    }
                }
            }
        }
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        Hal_SC_set_fmclk(psXCInstPri, ENABLE);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, BIT(1), BIT(1));//reg_force_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_13_L, BIT(10), BIT(10));//pip
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset &= ~0x1;
#else
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (!Hal_SC_3D_IsDualViewMode(pInstance,eWindow)) // dual view mode need to skip
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
        }
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
            Hal_SC_set_Dual_OPMBase0(pInstance, (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0*BYTE_PER_WORD), SUB_WINDOW);
        }
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for second channel
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for first channel
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_13_L, BIT(10), BIT(10));//pip
#endif
#endif
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
        {
            if(psXCInstPri->u32DeviceID == 0)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
            }
        }
        else
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
        }
        //Set SRAM to linear mode
        if (IsVMirrorMode(eWindow))
        {
           SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x303);
        }

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
       if(!Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
#endif
        {
            MDrv_XC_3D_CfgSubWin(pInstance, ENABLE);
        }
#if(SUPPORT_SC0_SUB_WIN == FALSE)
        MS_U16 u16SubLinebufferStart = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_13_L, 0x1FFF);//The starting address of F1 stored at line buffer
        MS_U16 u16MainLinebufferStart = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_12_L, 0x1FFF);//The starting address of F2 stored at line buffer
        if(!MDrv_SC_3D_Is2TapModeSupportedFormat(pInstance, pSrcInfo, eWindow) && (u16SubLinebufferStart == u16MainLinebufferStart))
        {
            //monet top/bot 3d mode using the same buffer
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_23_L, BIT(11), BIT(11));
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(!Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
#endif
        {
            //monet has no pip
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0, BIT(11));
        }
#endif

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, SUB_WINDOW);

        if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
        {
            if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
            {
#if(HW_2DTO3D_VER == 4)
                //masearti manhattan mode FRC output FA
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_64_L, BIT(5), BIT(5));   //pixel base active 3d
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_50_L, BIT(14), BIT(14)); //block base active 3d
#endif
                if(bChangeDisplaySize)//restore dispwin
                {
                    // force 4k2k to FSC case, but bk10 display window need to be original size 4k1k
                    gSrcInfo[eWindow].stDispWin.y = gSrcInfo[eWindow].stDispWin.y/2;
                    gSrcInfo[eWindow].stDispWin.height = gSrcInfo[eWindow].stDispWin.height/2;
                    bChangeDisplaySize = FALSE;

                    // for custom scaling case
                    if(gSrcInfo[eWindow].bVCusScaling)
                    {
                        gSrcInfo[eWindow].u16VCusScalingDst = gSrcInfo[eWindow].u16VCusScalingDst/2;
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"change DispWin x: %4u  W: %4u  y: %4u  H: %4u ,u16VCusScalingDst : %u\n",
                                               gSrcInfo[eWindow].stDispWin.x,
                                               gSrcInfo[eWindow].stDispWin.width,
                                               gSrcInfo[eWindow].stDispWin.y,
                                               gSrcInfo[eWindow].stDispWin.height,
                                               gSrcInfo[eWindow].u16VCusScalingDst);
                }
            }
        }
#endif
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (Hal_SC_3D_IsDualViewMode(pInstance,eWindow))
        {
            Hal_SC_3D_Set_DualView(pInstance,TRUE);
        }
#endif
    }
    else if(IS_OUTPUT_LINE_ALTERNATIVE())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is LA\n");
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);

            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
            {
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd+1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); //"+1" is hw bug
            }
            else
            {
                Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
            {
                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
                {
                    MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd+1, MAIN_WINDOW);
                }
                else
                {
                    MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                }
            }
#endif
        }
        else
#endif
        {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);//Sub window display window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd+1, VOP_DISPLAY_VEND_MASK); //"+1" is hw bug
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
        }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef UFO_XC_SUPPORT_3D_DS
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
            {
                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);

                if (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1))
                {
                    MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd+1, MAIN_WINDOW);
                }
                else
                {
                    MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                }
            }
#endif
#endif
        }
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        Hal_SC_set_fmclk(psXCInstPri, ENABLE);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, BIT(1), BIT(1));//reg_force_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_13_L, BIT(10), BIT(10));//pip
#endif
#if((HW_DESIGN_3D_VER == 3) || (HW_DESIGN_3D_VER == 5))//monaco/monet/manhattan mode
        // 4k2k needs more time to train data so we need to delay the display time
        // this is special in line alternative out
        if (   (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 3800)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 2100))
        {
#if (SUPPORT_3D_DS == 0)
            if ( MDrv_XC_GetDynamicScalingStatus(pInstance) == FALSE)
#endif
            {
                pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = 0x14;
                pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG] = 0x18;
                pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = TRUE;
                pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] = TRUE;
            }
        }
#endif
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_13_L, BIT(10), BIT(10));//pip
#endif
        #if 0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
        #else
        ///for 3D film issue
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x00, BIT(11)); //h3d_opm, sub win follow main's read bank
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (!Hal_SC_3D_IsDualViewMode(pInstance,eWindow)) // dual view mode need to skip
#endif
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L, 0x8000);
        }
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x00, BIT(11)); // Sub ahead need to disable when memory allocate is SBS
        #endif
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0);//F1 OPM BASE0 sub
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
            Hal_SC_set_Dual_OPMBase0(pInstance, (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0*BYTE_PER_WORD), SUB_WINDOW);
        }
#endif

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for second channel, added for protect
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for first channel

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_53_L, 0x8000, 0x8000);// 3D LR side-by-side to line-to-line enable
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset &= ~0x1;
#endif
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
        {
            //Main and Sub should load different LB offset at dual view mode
            if(psXCInstPri->u32DeviceID == 0)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
            }
        }
        else
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(!Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
#endif
        {
            MDrv_XC_3D_CfgSubWin(pInstance, ENABLE);
        }
        //if mvop mirror, main/sub window will get switched frame, so need do LR exchange
        //field alternative and frame alternative input don't need do this, since main/sub
        //window still get the original frame
        if((!IS_INPUT_FIELD_ALTERNATIVE(eWindow))
           && (!IS_INPUT_FRAME_ALTERNATIVE(eWindow)))
        {
            if(MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1)))
            {
                if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
                {
                    MDrv_XC_Set_3D_LR_Frame_Exchg_burst(pInstance, eWindow);
                }
                else
                {
                    MDrv_XC_Set_3D_LR_Frame_Exchg(pInstance, eWindow);
                }
            }
        }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, SUB_WINDOW);
#endif

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (Hal_SC_3D_IsDualViewMode(pInstance,eWindow))
        {
            Hal_SC_3D_Set_DualView(pInstance,TRUE);
        }
#endif
    }
    else if(IS_OUTPUT_FRAME_L())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is L\n");
        MS_U16 u16VactVideo, u16VactSpace;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, DISABLE);

        MDrv_XC_3D_CfgSubWin(pInstance, DISABLE);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1, VOP_DE_VEND_MASK);

        if(IS_INPUT_FRAME_PACKING(eWindow)&&(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen*2, 0x0FFF);//Set the maximum request lines for second channel, added for protect
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for second channel, added for protect
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
#endif
    }
    else if(IS_OUTPUT_FRAME_R())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is R\n");
        MS_U16 u16VactVideo, u16VactSpace;
        MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16VactVideo, &u16VactSpace, DISABLE);

#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
        }
        else
#endif
        {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);//Sub window display window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1, VOP_DE_VEND_MASK);
        }
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0);//F1 FRCM_R BASE0
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0);//F1 OPM BASE0
#endif

#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
            Hal_SC_set_Dual_OPMBase0(pInstance, (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0*BYTE_PER_WORD), SUB_WINDOW);
        }
#endif

        if(IS_INPUT_FRAME_PACKING(eWindow)&&(pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen*2, 0x0FFF);//Set the maximum request lines for second channel, added for protect
            if (IsVMirrorMode(eWindow))
            {
               SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_64_L, 0x00, 0x30); //Patch
            }
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for first channel
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
        MDrv_XC_3D_CfgSubWin(pInstance, ENABLE);

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, SUB_WINDOW);
#endif
    }
    else if(IS_OUTPUT_SIDE_BY_SIDE_HALF() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is SBSH\n");
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            if (IsVMirrorMode(eWindow))
            {
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                                                                     + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 + pSrcInfo->stDispWin.width/2)-1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, MAIN_WINDOW);
                    }
#endif
                }
                else
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                    }
#endif
                }
            }
            else
            {
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd= pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                                                                    + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 + pSrcInfo->stDispWin.width/2)-1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, MAIN_WINDOW);
                    }
#endif
                }
                else
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd= pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                                                                    + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 + pSrcInfo->stDispWin.width/2)-1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                    }
#endif
                }
            }

        }
        else
#endif
        {
        if (IsVMirrorMode(eWindow))
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//sub window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, VOP_DISPLAY_HSTART_MASK);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                                                                 + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 + pSrcInfo->stDispWin.width/2)-1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2)-1, VOP_DE_VEND_MASK);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//sub window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2, VOP_DISPLAY_HSTART_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
            }
        }
        else
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, 0x3FFF);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd= pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                                                                + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 + pSrcInfo->stDispWin.width/2)-1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart+ pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd)/2, VOP_DISPLAY_VEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2)-1, VOP_DE_VEND_MASK);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2, 0x3FFF);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd= pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart
                                                                                + (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 + pSrcInfo->stDispWin.width/2)-1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart + pSrcInfo->stDispWin.width/2-1, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
            }
        }
        }

#if 0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
#else
        ///for 3D film issue
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0x00, BIT(11)); //h3d_opm, sub win follow main's read bank
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L, 0x8000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_10_L, 0x00, BIT(11)); // Sub ahead need to disable when memory allocate is SBS
#endif

        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
            Hal_SC_set_Dual_OPMBase0(pInstance, (MS_PHY)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0*(BYTE_PER_WORD), SUB_WINDOW);
        }
#endif

#if(HW_DESIGN_3D_VER == 5) //monaco mode
        Hal_SC_set_fmclk(psXCInstPri, ENABLE);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_18_L, BIT(1), BIT(1));//reg_force_f2
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_13_L, BIT(10), BIT(10));//pip
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset &= ~0x1;
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_13_L, BIT(10), BIT(10));//pip
#endif
#endif

        if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_LEFT_LEFT)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(10), BIT(10));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(8));
        }
        else if(pXCResourcePrivate->stdrvXC_3D._eOutput3DMode == E_XC_3D_OUTPUT_RIGHT_RIGHT)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(10), BIT(10));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(8), BIT(8));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(10));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, 0, BIT(8));        
        }

#if((HW_DESIGN_3D_VER == 3) || (HW_DESIGN_3D_VER == 5))//monaco/monet/manhattan mode
        // 4k2k needs more time to train data so we need to delay the display time
        // this is special in line alternative out
        if (   (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 3800)
            && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 2100))
        {
#if (SUPPORT_3D_DS == 0)
            if ( MDrv_XC_GetDynamicScalingStatus(pInstance) == FALSE)
#endif
            {
                pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = 0x14;
                pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG] = 0x18;
                pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = TRUE;
                pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] = TRUE;
            }
        }
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
        //Set SRAM to linear mode
        if (IsVMirrorMode(eWindow))
        {
           SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x303);
        }

        MDrv_XC_3D_CfgSubWin(pInstance, ENABLE);

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, SUB_WINDOW);
#endif
    }
    else if (IS_OUTPUT_FRAME_PACKING())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is FPP\n");
#ifdef UFO_XC_SUPPORT_3D_DS
        if(MDrv_XC_Is_SupportSWDS(pInstance))
        {
            if (IsVMirrorMode(eWindow))
            {
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_1080X2P_VSIZE)
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE + DOUBLEHD_1080X2P_GARBAGE_VSIZE, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_VSIZE -1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE + DOUBLEHD_1080X2P_GARBAGE_VSIZE, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                    }
#endif
                }
                else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_720X2P_VSIZE)
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE + DOUBLEHD_720X2P_GARBAGE_VSIZE, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_VSIZE -1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE + DOUBLEHD_720X2P_GARBAGE_VSIZE, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, MAIN_WINDOW);
                    }
#endif
                }

            }
            else
            {
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_1080X2P_VSIZE)
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE + DOUBLEHD_1080X2P_GARBAGE_VSIZE, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_VSIZE -1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE + DOUBLEHD_1080X2P_GARBAGE_VSIZE, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE-1, MAIN_WINDOW);
                    }
#endif
                }
                else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_720X2P_VSIZE)
                {
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Sub window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE + DOUBLEHD_720X2P_GARBAGE_VSIZE, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_VSIZE -1;
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT); // Display H start
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);   // Display H end
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);//Main window display window
                    Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE-1, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE + DOUBLEHD_720X2P_GARBAGE_VSIZE, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, SUB_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndX(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, MAIN_WINDOW);
                        MHal_SWDS_AddTGENMdeStartEndY(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE-1, MAIN_WINDOW);
                    }
#endif
                }
            }
        }
        else
#endif
        {
        if (IsVMirrorMode(eWindow))
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_1080X2P_VSIZE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF); // Display H start
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);   // Display H end
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//Sub window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK);//Main window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE + DOUBLEHD_1080X2P_GARBAGE_VSIZE, VOP_DISPLAY_VSTART_MASK);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_VSIZE -1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
            }
            else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_720X2P_VSIZE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF); // Display H start
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);   // Display H end
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, 0x1FFF);//Sub window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK);//Main window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE + DOUBLEHD_720X2P_GARBAGE_VSIZE, VOP_DISPLAY_VSTART_MASK);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_VSIZE -1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
            }

        }
        else
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_1080X2P_VSIZE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);//Sub window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE + DOUBLEHD_1080X2P_GARBAGE_VSIZE, 0x1FFF);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_VSIZE -1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_1080X2P_FRAME_VSIZE-1, VOP_DISPLAY_VEND_MASK);
            }
            else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height ==DOUBLEHD_720X2P_VSIZE)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, 0x3FFF);//Sub window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, 0x3FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE + DOUBLEHD_720X2P_GARBAGE_VSIZE, 0x1FFF);
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_VSIZE -1;
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, 0x1FFF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart + DOUBLEHD_720X2P_FRAME_VSIZE-1, VOP_DISPLAY_VEND_MASK);
            }

        }
        }
#if (HW_DESIGN_3D_VER == 5)
        Hal_SC_set_fmclk(psXCInstPri, ENABLE);
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_33_L, BIT(11), BIT(11)); //h3d_opm, sub win follow main's read bank
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(IsEnableDualMode(eWindow))
        {
            Hal_SC_set_Dual_OPMBase0(pInstance, (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0*BYTE_PER_WORD), SUB_WINDOW);
        }
#endif

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for second channel
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_16_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen, 0x0FFF);//Set the maximum request lines for first channel
#if(HW_DESIGN_3D_VER == 5) //monaco mode
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset &= ~0x1;
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF), 0xFF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset&0xFF)<<8, 0xFF00);
        //Set SRAM to linear mode
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0B_L, 0x303);
        MDrv_XC_3D_CfgSubWin(pInstance, ENABLE);

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, SUB_WINDOW);
#endif
    }

    _XC_RETURN(pInstance);
#endif

    if(((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type >= E_XC_PNL_LPLL_VBY1_10BIT_4LANE)
        &&((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type <= E_XC_PNL_LPLL_VBY1_8BIT_8LANE))
    {
    #ifdef SUPPORT_MOD_ADBANK_SEPARATE

    #else
        if((E_XC_3D_OUTPUT_FRAME_ALTERNATIVE == eOutputMode)
            ||(E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_NOFRC == eOutputMode)
            ||(E_XC_3D_OUTPUT_FRAME_ALTERNATIVE_HW == eOutputMode))

        {
            MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, BIT(15), BIT(15));
        }
        else
        {
            MOD_W2BYTEMSK(REG_MOD_BK00_5A_L, 0, BIT(15));
        }
    #endif
    }

#if HW_2DTO3D_SUPPORT
    MS_PHY u32HW2DTO3D_DD_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf;
    MS_PHY u32HW2DTO3D_DR_Buf = pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf;

    if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DD_Buf >= HAL_MIU1_BASE)
    {
        u32HW2DTO3D_DD_Buf -= HAL_MIU1_BASE;
    }

    if(pXCResourcePrivate->stdrvXC_3D._u32HW2DTO3D_DR_Buf >= HAL_MIU1_BASE)
    {
        u32HW2DTO3D_DR_Buf -= HAL_MIU1_BASE;
    }
#if (HW_2DTO3D_VER < 3) //old 2d to 3d architecture
    _XC_ENTRY(pInstance);
    //First init all control register to HW init value
    if((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod) && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D)
    {
        //clk
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, BIT(6)); //reg_ckg_ficlk_3d
        //dd
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, pSrcInfo->stDispWin.width, 0x07FF);//src width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, pSrcInfo->stDispWin.height, 0x07FF);//src height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x7878, 0x7F7F);//dram request number
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, (u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD)>>16, 0x07FF);//dd buffer high 9bit
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_07_L, u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD);//dd buffer low 2 bytes
        MDrv_XC_wait_output_vsync(pInstance, 1, 100, eWindow);
#if (HW_2DTO3D_VER < 2)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(13)|BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu
#endif
#if (HW_2DTO3D_VER > 0)
        MDrv_WriteByteMask(REG_MIU0_BASE+0x2B, BIT(6), BIT(6)); //enable memory write
#endif
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_21_L, 0x540f);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x0430);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_40_L, BIT(6),  BIT(6));   // DD force bank enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_41_L, 0x0000, 0x000F);     // DD force bank0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, 0, BIT(12));        //disable dynamic gating
        pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank = TRUE;
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
        pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount = 0;
        pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
    }
    else
    {
        //detach vsync interrupt
        if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, 0x0000, BIT(13)|BIT(15));//src domain select&start to write miu
        //dd
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, 0x0780, 0x07FF);//src width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, 0x0438, 0x07FF);//src height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x7878, 0x7F7F);//dram request number
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_21_L, 0x0000);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_24_L, 0x0000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK00_03_L, BIT(12), BIT(12)); //enable dynamic gating
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
        pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount = pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount;
        pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
    }

    if(   (pXCResourcePrivate->stdrvXC_3D.ePreInputMode != E_XC_3D_INPUT_MODE_NONE)
       && (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode != E_XC_3D_OUTPUT_MODE_NONE)
       && ((eInputMode != pXCResourcePrivate->stdrvXC_3D.ePreInputMode) || (eOutputMode != pXCResourcePrivate->stdrvXC_3D.ePreOutputMode)))
    {
        //dr
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_6C_L, 0x0000, 0x0001);//Enable Depth Render
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_64_L, 0x0000, 0x1FFF);//pixel width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_60_L, 0x0000, 0x07FF);//MV width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_62_L, 0x0000, 0x00FF);//horizontal number of 32x32 block
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_61_L, 0x0000, 0x007F);//vertical number of 32x32 block
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_62_L, 0x0F00, 0xFF00);//TOTAL FRAME
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, 0x0000, 0x00FF);//left depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, 0x0000, 0x00FF);//right depth offset
#if (HW_2DTO3D_VER < 2)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, 0x0000, 0x3F00);//left depth gain:6bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, 0x0000, 0x3F00);//right depth gain:6bits
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, 0x0000, 0x7F00);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, 0x0000, 0x7F00);//right depth gain:7bits
#endif
#if (HW_2DTO3D_VER == 0)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_73_L, 0x8000, 0xBF3F);//dp info software reset, last number,request number
#else
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_73_L, 0x0000);          //last number,request number
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_71_L, BIT(11), BIT(11)); //dp info software reset
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0000, 0xC007);//Active mode, Passive mode Select,1:Line alternative
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_72_L, 0x0000, 0x00FF);  //draw left/right black enable, draw left/right black width
        if((IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE()))
        {
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, FALSE, TRUE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);//enable op active 2d to 3d
        }
    }

    if((IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE()))
    {
        //Close 3D, just need restore Mainwindow here
        if(   (pXCResourcePrivate->stdrvXC_3D.ePreInputMode != E_XC_3D_INPUT_MODE_NONE)
           && (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode != E_XC_3D_OUTPUT_MODE_NONE))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
        }
        _XC_RETURN(pInstance);
        pXCResourcePrivate->stdrvXC_3D.ePreInputMode = eInputMode;
        pXCResourcePrivate->stdrvXC_3D.ePreOutputMode = eOutputMode;
        return TRUE;
    }

    //input
    if(IS_INPUT_NORMAL_2D_HW(eWindow))
    {
        //clk
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, BIT(6)); //reg_ckg_ficlk_3d
        //dd
        if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, pSrcInfo->stDispWin.width/2, 0x07FF);//src width
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_01_L, pSrcInfo->stDispWin.width, 0x07FF);//src width
        }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_02_L, pSrcInfo->stDispWin.height, 0x07FF);//src height
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_05_L, 0x7878, 0x7F7F);//dram request number
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, (u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD)>>16, 0x07FF);//dd buffer high 9bit
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_07_L, u32HW2DTO3D_DD_Buf/HW_2DTO3D_BYTE_PER_WORD);//dd buffer low 2 bytes
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_74_L, u32HW2DTO3D_DR_Buf/HW_2DTO3D_BYTE_PER_WORD);//dr buffer low 2 bytes
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_75_L, (u32HW2DTO3D_DR_Buf/HW_2DTO3D_BYTE_PER_WORD)>>16, 0x07FF);//dr buffer high 9bit
        MDrv_XC_wait_output_vsync(pInstance, 1, 100, eWindow);
#if (HW_2DTO3D_VER < 2)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(13)|BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_06_L, BIT(15), BIT(13)|BIT(15));//src domain select&start to write miu
#endif
        //dr
        if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_64_L, pSrcInfo->stDispWin.width/2, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_60_L, ((pSrcInfo->stDispWin.width+3)/4)/2, 0x07FF);//MV width
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_64_L, pSrcInfo->stDispWin.width, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_60_L, (pSrcInfo->stDispWin.width+3)/4, 0x07FF);//MV width
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_62_L, (pSrcInfo->stDispWin.width+31)/32, 0x00FF);//horizontal number of 32x32 block
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_61_L, (pSrcInfo->stDispWin.height+31)/32, 0x007F);//vertical number of 32x32 block
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_62_L, 0x0F00, 0xFF00);//TOTAL FRAME
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//left depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//right depth offset
#if (HW_2DTO3D_VER < 2)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x3F00);//left depth gain:6bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x3F00);//right depth gain:6bits
        if(IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, 0x0000, BIT(14));//sign bit:bit14
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, BIT(14), BIT(14));//sign bit:bit14
        }
#else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_66_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//right depth gain:7bits
        if(IS_OUTPUT_TOP_BOTTOM_HW() || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, 0x0000, BIT(15));//sign bit:bit15
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_65_L, BIT(15), BIT(15));//sign bit:bit15
        }
#endif

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x0000, 0x8000); // 2nd offset enable
#if (HW_2DTO3D_VER == 0)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_73_L, 0x0404, 0xBF3F);//dp info software reset, last number,request number
#else
        MDrv_WriteByteMask(REG_MIU0_BASE+0x2B, BIT(6), BIT(6)); //enable memory write
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_71_L, 0x0, BIT(11));           //dp info software reset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_73_L, 0x0404);                    //last number,request number
#endif
        if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_6C_L, 0x0821, 0x0821);//Enable Depth Render
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_6C_L, 0x0001, 0x0001);//Enable Depth Render
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, BIT(12),  BIT(12)); // DR force bank enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_40_L, BIT(6),  BIT(6));   // DD force bank enable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_41_L, 0x0000, 0x000F);     // DD force bank0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0000, 0x0F00);     // DR force bank0
#if HW_2DTO3D_PATCH
        pXCResourcePrivate->stdrvXC_3D._bIsHW2Dto3DPatchEnabled = Hal_SC_IsHW2Dto3DPatch_Enable();
#endif
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
    }
    else if((!IS_INPUT_MODE_NONE(eWindow))
            && (!IS_OUTPUT_MODE_NONE())
            && (!IS_OUTPUT_FRAME_L())
            && (!IS_OUTPUT_FRAME_R())
            && (!IS_INPUT_NORMAL_2D(eWindow))) //real 3d adjust depth using hw 2d to 3d function
    {
#if (HW_DESIGN_3D_VER >= 2)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_64_L, pSrcInfo->stDispWin.width, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_60_L, (pSrcInfo->stDispWin.width+3)/4, 0x07FF);//MV width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_62_L, (pSrcInfo->stDispWin.width+31)/32, 0x00FF);//horizontal number of 32x32 block
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_61_L, (pSrcInfo->stDispWin.height+31)/32, 0x007F);//vertical number of 32x32 block
#else
            //for 3d ver<2,vop2 display window or height may not same as main or sub window's display size
            MS_U16 u16Width = pSrcInfo->stDispWin.width;
            MS_U16 u16Height = pSrcInfo->stDispWin.height;
            if(IS_OUTPUT_SIDE_BY_SIDE_HALF()
               || IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
            {
                u16Width = u16Width * 2;
            }
            else if(IS_OUTPUT_LINE_ALTERNATIVE()
                    || IS_OUTPUT_LINE_ALTERNATIVE_HW()
                    || IS_OUTPUT_TOP_BOTTOM()
                    || IS_OUTPUT_TOP_BOTTOM_HW())
            {
                u16Height = u16Height * 2;
            }
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_64_L, u16Width, 0x1FFF);//pixel width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_60_L, (u16Width+3)/4, 0x07FF);//MV width
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_62_L, (u16Width+31)/32, 0x00FF);//horizontal number of 32x32 block
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_61_L, (u16Height+31)/32, 0x007F);//vertical number of 32x32 block
#endif
        if(MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, eInputMode, eOutputMode, eWindow)) //real 3d adjust depth using hw 2d to 3d function
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_6C_L, 0x0001, 0x0001);//Enable Depth Render
            if((pXCResourcePrivate->stdrvXC_3D._u163DHShift == 0x80) || (pXCResourcePrivate->stdrvXC_3D._u163DHShift == 0x00))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x0000, 0x8000); //2  // 2nd offset enable
            }
            else if(pXCResourcePrivate->stdrvXC_3D._u163DHShift > 0x80)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x8000, 0x8000); //2  // 2nd offset enable
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5E_L, 0x0000); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5F_L, (pXCResourcePrivate->stdrvXC_3D._u163DHShift-0x80)<<8); //2  // 2nd offset R
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_5D_L, 0x8000, 0x8000); //2  // 2nd offset enable
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5E_L, (0x80-pXCResourcePrivate->stdrvXC_3D._u163DHShift)<<8); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK15_5F_L, 0x0000); //2  // 2nd offset R
            }
        }
    }

    //output
    if(IS_OUTPUT_CHECKBOARD_HW())
    {
        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0000, 0x0007);//Passive mode Select,0:Check Board, R first
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0003, 0x0007);//Passive mode Select,3:Check Board, L first
        }
    }
    else if(IS_OUTPUT_LINE_ALTERNATIVE_HW())
    {
        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0001, 0x0007);//Passive mode Select,1:Line Alternative, R first
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0004, 0x0007);//Passive mode Select,4:Line Alternative, L first
        }
    }
    else if(IS_OUTPUT_FRAME_ALTERNATIVE_HW())
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, BIT(14), BIT(14));//active mode
        Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, TRUE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);//enable op active 2d to 3d
    }
    else if(IS_OUTPUT_PIXEL_ALTERNATIVE_HW())
    {
        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0002, 0x0007);//Passive mode Select,2:Pixel Alternative, R first
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0005, 0x0007);//Passive mode Select,5:Pixel Alternative, L first
        }
    }
    else if(IS_OUTPUT_FRAME_L_HW())
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0006, 0x0007);//Passive mode Select,6:L Only
    }
    else if(IS_OUTPUT_FRAME_R_HW())
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0007, 0x0007);//Passive mode Select,7:R Only
    }
    else if(IS_OUTPUT_FRAME_ALTERNATIVE()) //real 3d adjust depth using hw 2d to 3d function:frame alternative output
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, BIT(14), BIT(14)); //enable op2t3d_lr
    }
    else if(IS_OUTPUT_LINE_ALTERNATIVE() || (IS_OUTPUT_TOP_BOTTOM() && MDrv_SC_3D_Is_LR_Sbs2Line(pInstance))) //real 3d adjust depth using hw 2d to 3d function:line alternative output
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK15_67_L, 0x0001, 0x0007);//Passive mode Select,1:Line Alternative
    }

    _XC_RETURN(pInstance);

#else //new 2d to 3d architecture
    _XC_ENTRY(pInstance);
    //First init all control register to HW init value
    if((E_XC_3D_AUTODETECT_HW == pXCResourcePrivate->stdrvXC_3D._enAutoDetectMethod)
        && pXCResourcePrivate->stdrvXC_3D._bCurEnableAutoDetect3D)
    {
        _MDrv_SC_3D_Load2DTo3DReg(pInstance, TRUE, pSrcInfo, eWindow);
        pXCResourcePrivate->stdrvXC_3D._bAutoDetect3DReadFirstBank = TRUE;
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
        pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount = 0;
        pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
    }
    else
    {
        //detach vsync interrupt
        if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
        _MDrv_SC_3D_Load2DTo3DReg(pInstance, FALSE, pSrcInfo, eWindow);
        pXCResourcePrivate->stdrvXC_3D._bResetFrameCount = TRUE;
        pXCResourcePrivate->stdrvXC_3D._u16CheckedFrameCount = pXCResourcePrivate->stdrvXC_3D._stDetect3DFormatPara.u16MaxCheckingFrameCount;
        pXCResourcePrivate->stdrvXC_3D._enCurDetectedRes = E_XC_3D_INPUT_MODE_NONE;
    }

    if(   (pXCResourcePrivate->stdrvXC_3D.ePreInputMode != E_XC_3D_INPUT_MODE_NONE)
       && (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode != E_XC_3D_OUTPUT_MODE_NONE)
       && ((eInputMode != pXCResourcePrivate->stdrvXC_3D.ePreInputMode)
            || (eOutputMode != pXCResourcePrivate->stdrvXC_3D.ePreOutputMode)))
    {
        _MDrv_SC_3D_Load2DTo3DReg(pInstance, FALSE, pSrcInfo, eWindow);

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, 0x0000);//left gain/offset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, 0x0000);//right gain/offset
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, 0x0000); //2  // 2nd offset L
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, 0x0000); //2  // 2nd offset R
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_65_L, 0x0000 ,0x01ff);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_66_L, 0x0000, 0x01ff);

        if((IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE()))
        {
            Hal_XC_H3D_LR_Toggle_Enable(pInstance, FALSE, TRUE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);//enable op active 2d to 3d
        }
    }

    if((IS_INPUT_MODE_NONE(eWindow) || IS_OUTPUT_MODE_NONE()))
    {
        //Close 3D, just need restore Mainwindow here
        if(   (pXCResourcePrivate->stdrvXC_3D.ePreInputMode != E_XC_3D_INPUT_MODE_NONE)
           && (pXCResourcePrivate->stdrvXC_3D.ePreOutputMode != E_XC_3D_OUTPUT_MODE_NONE))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart, VOP_DISPLAY_HSTART_MASK); // Display H start
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd, VOP_DISPLAY_HEND_MASK);   // Display H end
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart, VOP_DISPLAY_VSTART_MASK);//Main window display window
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd, VOP_DISPLAY_VEND_MASK);
        }
        _XC_RETURN(pInstance);
        pXCResourcePrivate->stdrvXC_3D.ePreInputMode = eInputMode;
        pXCResourcePrivate->stdrvXC_3D.ePreOutputMode = eOutputMode;
        return TRUE;
    }

    //input
    if(IS_INPUT_NORMAL_2D_HW(eWindow))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D input is 2D_HW\n");
        _MDrv_SC_3D_Load2DTo3DReg(pInstance, TRUE, pSrcInfo, eWindow);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//left depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Offset, 0x00FF);//right depth offset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain<<8, 0x7F00);//right depth gain:7bits

#if(HW_DESIGN_3D_VER == 5)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK57_15_L, pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCMVLen, 0x0FFF);//Set the maximum request lines for second channel
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
#if(HW_DESIGN_3D_VER == 3) //monet mode
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_03_L,BIT(2), BIT(2));//reg_force_fe2_en
#endif
        if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL))
        {
            MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MDrv_SC_3D_2DTo3D_DepthRefine, NULL);
        }
    }
    else if((!IS_INPUT_MODE_NONE(eWindow))
            && (!IS_OUTPUT_MODE_NONE())
            && (!IS_OUTPUT_FRAME_L())
            && (!IS_OUTPUT_FRAME_R())
            && (!IS_INPUT_NORMAL_2D(eWindow))) //real 3d adjust depth using hw 2d to 3d function
    {
        if(MDrv_XC_3D_IsHWDepthAdjSupported(pInstance, eInputMode, eOutputMode, eWindow)) //real 3d adjust depth using hw 2d to 3d function
        {
            MS_U16 u16ABSLevel = ((pXCResourcePrivate->stdrvXC_3D._u163DHShift > 0x80)?(pXCResourcePrivate->stdrvXC_3D._u163DHShift - 0x80):(0x80 - pXCResourcePrivate->stdrvXC_3D._u163DHShift));
            //DR setting
            _MDrv_SC_3D_DR_setting(pInstance, eWindow);

#if (HW_2DTO3D_VER >= 4)
            if(IS_OUTPUT_TOP_BOTTOM() || IS_OUTPUT_FRAME_PACKING())
            {
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
                if(!((MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
                    && !IsVBY1_16LANE((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)
                    &&  (pXCResourcePrivate->stdrvXC_3D._bLAToTB || pXCResourcePrivate->stdrvXC_3D._bFAToTB)))// non-maserati manhattan mode FRC 3D
#endif
                {
                    _MDrv_SC_3D_HWDepthAdj_TBOut(pInstance, pSrcInfo, eWindow); //T3d settings & sram depth force to 0x80
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(14)|BIT(13)|BIT(7), BIT(14)|BIT(13)|BIT(7));//TB out 3D depth enable
                }
            }
#endif

            if(IS_OUTPUT_SIDE_BY_SIDE_HALF())
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(3), BIT(3));//SBS out 3D depth enable
            }
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_49_L, (u32OutputWidth+31)/32, 0x00FF);//horizontal number of 32x32 block

            if((pXCResourcePrivate->stdrvXC_3D._u163DHShift == 0x80) || (pXCResourcePrivate->stdrvXC_3D._u163DHShift == 0x00))
            {
                //original 3d depth
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, 0x0000); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, 0x0000); //2  // 2nd offset R
            }
            else if(pXCResourcePrivate->stdrvXC_3D._u163DHShift > 0x80)
            {
                // |L-->      <--R|
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, (0xFF - (u16ABSLevel-1))<<8); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, (u16ABSLevel)<<8); //2  // 2nd offset R
            }
            else
            {
                // |L<--      -->R|
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_07_L, (u16ABSLevel)<<8); //2  // 2nd offset L
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK62_0F_L, (0xFF - (u16ABSLevel-1))<<8); //2  // 2nd offset R
            }

#if ENABLE_T3D_DEBUG
            //enable black edge
            Hal_SC_3D_enable_black_edge(pInstance, FALSE);
#endif
        }
    }

    //output
    if(IS_OUTPUT_LINE_ALTERNATIVE_HW())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is LA_HW\n");
        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, BIT(15), BIT(15)); //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, 0x0000, BIT(15));   //sign bit:bit15

        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, 0x0000, BIT(15));  //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, BIT(15), BIT(15)); //sign bit:bit15
        }
#if (HW_2DTO3D_VER >= 4)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_65_L, (~(pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain*4) + 1), 0x01FF);//left depth gain:7bits
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_66_L, pXCResourcePrivate->stdrvXC_3D._st3DHw2DTo3DPara.u16Gain*4, 0x01FF);//right depth gain:7bits
#endif
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
#endif
    }
    else if(IS_OUTPUT_FRAME_ALTERNATIVE_HW())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is FA_HW\n");
        if(pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA) //output FA_LLRR_HW for 4k0.5k@240Hz
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is FA_LLRR_HW \n");
            Hal_SC_3D_enable_FALLRR_out(pInstance, TRUE);
#if (HW_DESIGN_3D_VER == 3) && (HW_DESIGN_4K2K_VER != 7)//manhattan mode
        // 4k2k needs more time to train data so we need to delay the display time
        pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = VALUE_AUTO_TUNE_AREA_TRIG_4K540P;
        pXCResourcePrivate->sthal_SC.u16NewValue[E_STORE_VALUE_DISP_AREA_TRIG] = VALUE_DISP_AREA_TRIG_4K540P;
        pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_AUTO_TUNE_AREA_TRIG] = TRUE;
        pXCResourcePrivate->sthal_SC.bUsingNewValue[E_STORE_VALUE_DISP_AREA_TRIG] = TRUE;
#endif
        }

        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, BIT(15), BIT(15)); //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, 0x0000, BIT(15));   //sign bit:bit15

        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, 0x0000, BIT(15));  //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, BIT(15), BIT(15)); //sign bit:bit15
        }
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_64_L, BIT(5), BIT(5));   //pixel base active 3d
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_50_L, BIT(14), BIT(14)); //block base active 3d
        Hal_XC_H3D_LR_Toggle_Enable(pInstance, TRUE, TRUE, pXCResourcePrivate->stdrvXC_3D._bSkipDefaultLRFlag);//enable op active 2d to 3d

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        Hal_SC_3D_CopyDispWinToSTGEN(pInstance, MAIN_WINDOW);
#endif
    }
#if (HW_2DTO3D_VER >= 4)
    else if(IS_OUTPUT_TOP_BOTTOM_HW()
        || (IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_PACKING()))
    {
        if(IS_OUTPUT_TOP_BOTTOM_HW())
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is TB_HW\n");
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is FPP_HW\n");
        }
        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, BIT(15), BIT(15)); //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, 0x0000, BIT(15));   //sign bit:bit15

        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, 0x0000, BIT(15));  //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, BIT(15), BIT(15)); //sign bit:bit15
        }
#if (HW_DESIGN_3D_VER == 3)
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
         if(!((MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
            && !IsVBY1_16LANE((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)
            && (pXCResourcePrivate->stdrvXC_3D._bLAToTB || pXCResourcePrivate->stdrvXC_3D._bFAToTB)))// non-maserati manhattan mode FRC 3D
#endif
        {
            //tb out mode for monet.this must be setted for monaco eco,fix me
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(14)|BIT(13)|BIT(7), BIT(14)|BIT(13)|BIT(7));
        }
#else
        //turn off for TB PQ
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(7), BIT(7));
#endif
#if ENABLE_T3D_DEBUG
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_5C_L, 0x0000, 0x8000);
#endif
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, 0x0080, 0x00FF);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, 0x0080, 0x00FF);
        //muji, monaco
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_30_L, BIT(14), BIT(14));

        //HW 2Dto3D FP out
        if(IS_INPUT_NORMAL_2D_HW(eWindow) && IS_OUTPUT_FRAME_PACKING())
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_56_L, BIT(7), BIT(7)); //TB out Vsync reset enabel
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_56_L, 0x40, 0x7F);  //FP out need more delay,default value is 0x20
        }
    }
    else if(IS_OUTPUT_SIDE_BY_SIDE_HALF_HW())
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 3D output is SBS_HW\n");
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_69_L, BIT(3), BIT(3));//SS out depth enable bit
        if (IsVMirrorMode(eWindow) || (MDrv_ReadRegBit(REG_MVOP_MIRROR, (BIT(0)|BIT(1))) == (BIT(0)|BIT(1))))
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, BIT(15), BIT(15)); //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, 0x0000, BIT(15));   //sign bit:bit15

        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_06_L, 0x0000, BIT(15));  //sign bit:bit15
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_0E_L, BIT(15), BIT(15)); //sign bit:bit15
        }
    }
#endif

    _XC_RETURN(pInstance);

#endif //(HW_2DTO3D_VER < 3)
#endif //HW_2DTO3D_SUPPORT

    _XC_ENTRY(pInstance);
    //below is moved from hal sw db
    if(Hal_SC_is_extra_req_en(pInstance, NULL, NULL, NULL, NULL))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, BIT(15), BIT(15));
    }
    else
    {
         SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_11_L, 0, BIT(15));
    }
    _XC_RETURN(pInstance);

    pXCResourcePrivate->stdrvXC_3D.ePreInputMode = eInputMode;
    pXCResourcePrivate->stdrvXC_3D.ePreOutputMode = eOutputMode;

    return TRUE;
}
#endif

MS_BOOL MDrv_XC_EnableT3D(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#ifdef ENABLE_GOP_T3DPATCH
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK63_61_L, bEnable, BIT(0));
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK62_08_L, bEnable<<4, BIT(4));
        Hal_SC_set_T3D_setting(pInstance, bEnable);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK63_61_L, bEnable, BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK62_08_L, bEnable<<4, BIT(4));
        Hal_SC_set_T3D_setting(pInstance, bEnable);
    }
#endif
    return TRUE;
}

