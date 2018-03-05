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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
/// file    apiXC_PCMonitor.c
/// @brief  Scaler API layer Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_PC_MONITOR_C_
#define _API_XC_PC_MONITOR_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#define PCMSG(x)    //x

// Common
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/div64.h>
#else
#include <string.h>
#define do_div(x,y) ((x)/=(y))
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Driver
#include "drvXC_IOPort.h"
#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
#include "apiXC.h"
#include "drvXC_HDMI_Internal.h"
#include "drvXC_HDMI_if.h"
#include "mhal_hdmi.h"
#include "mhal_ip.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "mhal_xc_chip_config.h"
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
#include "drv_sc_scaling.h"
#include "apiXC_PCMonitor_v2.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define PC_POLLING_COUNTER          30  // (30/TIMER_PERIOD), unit ms

#define MD_TIMING_SOG_DETECT        20
#if 1//(INPUT_HDMI_VIDEO_COUNT > 0)
  #if 1
    // Change these count value just for HDMI fast to recover video,
    // But have to delay 200mSec to reduce flash blue screen when plug-in/out HDMI fast,
    #define MD_TIMING_STABLE_COUNT    (MDrv_XC_PCMonitor_GetStableCounter(pInstance, eWindow))
    #define MD_TIMING_NOSYNC_COUNT    (MDrv_XC_PCMonitor_GetNoSyncCounter(pInstance, eWindow))
  #else
    #define MD_TIMING_STABLE_COUNT    (((IsSrcTypeHDMI(psPCMONITOR_HK_Info[eWindow]->eCurrentSrc)?120:25))
    #define MD_TIMING_NOSYNC_COUNT    (((IsSrcTypeHDMI(psPCMONITOR_HK_Info[eWindow]->eCurrentSrc)?80:40))
  #endif
#else
    #define MD_TIMING_STABLE_COUNT    25 // input timing stable counter
    #define MD_TIMING_NOSYNC_COUNT    40
#endif

#define TIMING_CHANGE_DEBOUNCE_FOR_STR 3
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//*************************************************************************
//Function name:    _MApi_XC_PCMonitor_SetSyncStatusFlag
//Passing parameter:    MS_U8 u8TempSyncStatus:    b0: VSync polarity
//                                      b1: HSync polarity
//                                      b2: Hsync active
//                                      b3: Vsync active
//Return parameter: none
//Description:          Set signal sync status flags
//*************************************************************************
static void _MApi_XC_PCMonitor_SetSyncStatusFlag (void *pInstance, MS_U8 u8TempSyncStatus, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    stPCMonitorInfo.u8SyncStatus = u8TempSyncStatus;
    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
}

//*************************************************************************
//------------------------------ Warning ----------------------------------
//Attention: !!!!!!------ Don't add UI variable or unneeded code in this file
//
//           Don't modify this procedure !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//*************************************************************************
//*************************************************************************
//Function name:    _MApi_XC_PCMonitor_DetectSync
//Passing parameter:    none
//Return parameter: MS_BOOL:  true: sync loss    false: sync active
//Description:          Get signal sync status, true for sync loss, false for sync active
//*************************************************************************
static MS_BOOL _MApi_XC_PCMonitor_DetectSync (void *pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc , SCALER_WIN eWindow )
{
    PCMSG(static MS_BOOL bSyncLoss = TRUE);

    MS_BOOL bResult = FALSE;
    XC_IP_SYNC_STATUS sXC_Sync_Status;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    memset(&sXC_Sync_Status, 0, sizeof(XC_IP_SYNC_STATUS));
    _XC_ENTRY(pInstance);
    MDrv_XC_GetSyncStatus(pInstance, eCurrentSrc, &sXC_Sync_Status, eWindow);
    _XC_RETURN(pInstance);
    if ( sXC_Sync_Status.u8SyncStatus & XC_MD_SYNC_LOSS ) // check no signal
    {
        PCMSG(if(!bSyncLoss) printf("XC_MD_SYNC_LOSS 0x%x\n", sXC_Sync_Status.u8SyncStatus); bSyncLoss = TRUE);

        if( IsSrcTypeVga(eCurrentSrc) || IsSrcTypeYPbPr(eCurrentSrc))
        {
            MDrv_ADC_ISOG_SetDetectMode(pInstance, E_ADC_ISOG_STANDBY_MODE);
        }

        if (MDrv_XC_GetVGASogEn(pInstance))   // Daten FixMe -> Please refine VGA SOG flow
        {
            if( IsSrcTypeVga(eCurrentSrc)&&
                ((stPCMonitorInfo.ucInputTimingStableCounter % MD_TIMING_SOG_DETECT)==0))
            {
                Hal_SC_ip_sog_detect(pInstance); // toggle analog input and detect next time
            }
        }

        if (pXCResourcePrivate->stdrvXC_MVideo_Context._bEnableEuro)
        {
            if( IsSrcTypeYPbPr(eCurrentSrc) )
            {
                MApi_XC_EuroHdtvHandler(pInstance, eWindow);
            }
            else
            {
                MDrv_XC_HWAuto_SyncSeparate(pInstance, eWindow);
            }
        }
    }
    else // have signal
    {

        if (MDrv_XC_GetVGASogEn(pInstance))   // Daten FixMe -> Please refine VGA SOG flow
        {
            _XC_ENTRY(pInstance);
            if( IsSrcTypeVga(eCurrentSrc)&&
                ((stPCMonitorInfo.ucInputTimingStableCounter % MD_TIMING_SOG_DETECT)==0)
                 && (SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK01_02_L,0x70)!=0x70)
                 && (stPCMonitorInfo.ucInputTimingStableCounter <=0x14))
            {
                Hal_SC_ip_sog_detect(pInstance); // toggle analog input and detect next time
            }
            _XC_RETURN(pInstance);
        }

        bResult = TRUE;

        if( IsSrcTypeVga(eCurrentSrc) || IsSrcTypeYPbPr(eCurrentSrc))
        {
            MDrv_ADC_ISOG_SetDetectMode(pInstance, E_ADC_ISOG_NORMAL_MODE);
        }

        PCMSG(if(bSyncLoss) printf("MD_SYNC_DETECT 0x%x\n", sXC_Sync_Status.u8SyncStatus); bSyncLoss = FALSE);
    }

    if ( bResult ) // neet to update flag
    {
        _MApi_XC_PCMonitor_SetSyncStatusFlag(pInstance, sXC_Sync_Status.u8SyncStatus, eWindow);
    } // set global sync status flag

    return bResult;
}

// If IP1 detect P mode, but IPM set I mode for 3D FramePacking I 60/50hz.
// It is possible IPM detect wrong field from IP2. Only for A5/A3 serial scaler.
// Affected chips
//               Old Patch Method: Force interlace
//                                 A3/A5/A6/Agate U01/Eagle U01/J2
//               New Patch Method: Register REG_SC_BK12_05[10] to let HW auto force i
//                                 Agate U02/Edison/Eagle U02/Emerald/Kaiser/Macaw12
//
static void _MApi_XC_PCMonitor_FPi_Patch(void *pInstance, SCALER_WIN eWindow)
{
    MS_WINDOW_TYPE sWin;
    memset(&sWin, 0 , sizeof(MS_WINDOW_TYPE));

    if (MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow) == FALSE)
    {
        _XC_ENTRY(pInstance);
        MDrv_XC_GetDEWindow(pInstance, &sWin, eWindow);
        _XC_RETURN(pInstance);
    }
    else
    {
        MDrv_XC_GetDEWidthHeightInDEByPassMode(pInstance, &sWin.width, &sWin.height, eWindow);
    }

    // Check DE for framepacking I
    if ( ( sWin.height >  ( DOUBLEHD_1080X2I_VSIZE - 5) )
           && ( sWin.height <  ( DOUBLEHD_1080X2I_VSIZE + 5) )
           && ( sWin.width <  ( DOUBLEHD_1080X2I_HSIZE + 5) )
           && ( sWin.width >  ( DOUBLEHD_1080X2I_HSIZE - 5) ) )
    {
        MDrv_SC_ForceInterlaceInPMode(pInstance, ENABLE,eWindow);
        MsOS_DelayTask(150);
    }
}

static void _MApi_XC_PCMonitor_InitRegInTimingChange(void *pInstance, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    if(IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc) || IsSrcTypeDVI(stPCMonitorInfo.eCurrentSrc))
    {
        //ToDo HDMI
        Hal_HDMI_audio_output(DISABLE);
        if ( MDrv_SC_GetInterlaceInPModeStatus(pInstance, eWindow) )
        {
            MDrv_SC_ForceInterlaceInPMode(pInstance, DISABLE,eWindow);
        }
    }
    _XC_ENTRY(pInstance);
    if(eWindow == MAIN_WINDOW)
    {
        MDrv_XC_SoftwareReset(pInstance, REST_IP_F2, eWindow);
    }
    else
    {
        MDrv_XC_SoftwareReset(pInstance, REST_IP_F1, eWindow);
    }
    _XC_RETURN(pInstance);
// 20081226 - DVI+HDCP snow noise patch - start ...
//Note: Don't use rest function after HPD Hi/Low or it will cause snow image in a while.
//    MDrv_XC_ADC_reset(REST_DVI|REST_HDCP|REST_HDMI); // Bright20080918
// 20081226 - DVI+HDCP snow noise patch - end ...
    MApi_XC_TimingChangeInit(pInstance, eWindow);

    if(IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc))
        MApi_XC_VsyncPolarityDetectMode(pInstance, ENABLE, eWindow);
    else
        MApi_XC_VsyncPolarityDetectMode(pInstance, DISABLE, eWindow);

}

//*************************************************************************
//------------------------------ Warning ----------------------------------
//Attention: !!!!!!------ Don't add UI variable or unneeded code in this file
//
//           Don't modify this procedure !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//*************************************************************************
//*************************************************************************
//Function name:    _MApi_XC_PCMonitor_PrepareForTimingChange
//Passing parameter:    none
//Return parameter:     none
//Description:  This function will pre-set flags and registers for timing change
//*************************************************************************
static void _MApi_XC_PCMonitor_PrepareForTimingChange(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8TurnOffDestination = DISABLE;

    PCMSG(printf("PrepareForTimingChange\n"));
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);

    if ( stPCMonitorInfo.bInputTimingStable )
    {
        _MApi_XC_PCMonitor_InitRegInTimingChange(pInstance, eWindow);

    }

    if( IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc) || IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc) || IsSrcTypeDVI(stPCMonitorInfo.eCurrentSrc))
    {
        MApi_XC_InitRegInTimingChange_Post(pInstance, stPCMonitorInfo.u8SyncStatus, eWindow );
    }

    // reset input timing stable and wait for timing stable
    stPCMonitorInfo.bInputTimingStable = FALSE;

    // reset timing stable counter
    stPCMonitorInfo.ucInputTimingStableCounter = 0;

    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
}

/*
void _MApi_XC_PCMonitor_ModeParse(SCALER_WIN eWindow)
{


    // to match mode, we need these information
    // H/V freq., Vtt
    // Sync polarity
    // Interlaced or Progrssive
    psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.eSrcType = psPCMONITOR_HK_Info[eWindow]->eCurrentSrc;
    psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u16HFreqx10 = MApi_XC_CalculateHFreqx10(psPCMONITOR_HK_Info[eWindow]->u16Hperiod);
    psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u16VFreqx10 = MApi_XC_CalculateVFreqx10(psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u16HFreqx10, psPCMONITOR_HK_Info[eWindow]->u16Vtotal);

    if(psPCMONITOR_HK_Info[eWindow]->u8SyncStatus & XC_MD_INTERLACE_BIT)
    {
        psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u16VFreqx10 *=2;
    }

    psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u16Vtotal = psPCMONITOR_HK_Info[eWindow]->u16Vtotal;
    psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u8SyncStatus = psPCMONITOR_HK_Info[eWindow]->u8SyncStatus;
    //psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.eEuroHDTVStatus = EURO_AUS_HDTV_NORMAL;
    memcpy(&(psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.sDVI_HDMI_DE_Info), &(psPCMONITOR_HK_Info[eWindow]->sDVI_HDMI_DE_Info), sizeof(MS_WINDOW_TYPE));

#if (INPUT_HDMI_VIDEO_COUNT > 0)
    // search mode
    if(IsSrcTypeHDMI(psPCMONITOR_HK_Info[eWindow]->eCurrentSrc))
    {
        // check if HDMI or DVI
        MApi_XC_HDMI_CheckModeChanged(TRUE);
    }
#endif

    psPCMONITOR_HK_Info[eWindow]->eModeParseResult = MApi_XC_ModeParse_SearchMode(&(psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo));

    if(psPCMONITOR_HK_Info[eWindow]->sModeParseInputInfo.u8SyncStatus & XC_MD_USER_MODE_BIT)
    {
        // mode parse indicate this is a user mode
        psPCMONITOR_HK_Info[eWindow]->u8SyncStatus |= XC_MD_USER_MODE_BIT;
    }

    if(psPCMONITOR_HK_Info[eWindow]->eModeParseResult == XC_MODEPARSE_SUPPORT_MODE)
    {
        psPCMONITOR_HK_Info[eWindow]->eCurrentState = E_XC_PCMONITOR_STABLE_SUPPORT_MODE;
    }
    else
    {
        psPCMONITOR_HK_Info[eWindow]->eCurrentState = E_XC_PCMONITOR_STABLE_UN_SUPPORT;
    }
}
*/

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//*************************************************************************
//Function name:    MApi_XC_PCMonitor_InvalidTimingDetect
//Passing parameter:    none
//Return parameter: MS_BOOL : timing is valid or not
//Description:          Detect invalid timing. true: timing invalid   false : timing valid
//*************************************************************************
MS_BOOL MApi_XC_PCMonitor_InvalidTimingDetect_U2( void* pInstance, MS_BOOL bPollingOnly, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bReturn = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    _XC_ENTRY(pInstance);
    bReturn = MDrv_XC_PCMonitor_InvalidTimingDetect(pInstance, bPollingOnly, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_PCMonitor_InvalidTimingDetect(  MS_BOOL bPollingOnly, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_PCMONITOR_INVALIDTIMINGDETECT XCArgs;
    XCArgs.bPollingOnly = bPollingOnly;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_INVALIDTIMINGDETECT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_PCMonitor_Restart_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    MDrv_XC_PCMonitor_Restart(pInstance, eWindow);
    _XC_ENTRY(pInstance);
    MDrv_DVI_Accumulator_Monitor();
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_PCMonitor_Restart(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_PCMONITOR_RESTART XCArgs;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_RESTART, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_PCMonitor_Init_U2(void* pInstance, MS_U8 u8MaxWindowNum)
{
    MS_U8 i;
    SCALER_WIN eWindow = MAIN_WINDOW;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // initialize House keeping info
    MDrv_XC_PCMonitor_Init(pInstance, u8MaxWindowNum);
    for(i = 0; i < u8MaxWindowNum; i++)
    {
        switch(i)
        {
            case 0:
                MDrv_XC_PCMonitor_Restart(pInstance, MAIN_WINDOW);
                eWindow = MAIN_WINDOW;
                break;

            case 1:
                MDrv_XC_PCMonitor_Restart(pInstance, SUB_WINDOW);
                eWindow = SUB_WINDOW;
                break;

            default:
                // un-supported
                break;
        }

        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow] = MD_TIMING_STABLE_COUNT;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow] = MD_TIMING_NOSYNC_COUNT;
        pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc = INPUT_SOURCE_NONE;

    }
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_VGA = MD_TIMING_STABLE_COUNT;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_VGA = MD_TIMING_NOSYNC_COUNT;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_YPBPR = MD_TIMING_STABLE_COUNT;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_YPBPR = MD_TIMING_NOSYNC_COUNT;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_HDMI = MD_TIMING_STABLE_COUNT;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_HDMI = MD_TIMING_NOSYNC_COUNT;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
}

MS_BOOL MApi_XC_PCMonitor_Init(MS_U8 u8MaxWindowNum)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_PCMONITOR_INIT XCArgs;
    XCArgs.u8MaxWindowNum = u8MaxWindowNum;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_INIT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


void MApi_XC_PCMonitor_SetTimingCount_U2(void* pInstance, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_VGA = u16TimingStableCounter;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_VGA = u16TimingNosyncCounter;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_YPBPR = u16TimingStableCounter;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_YPBPR = u16TimingNosyncCounter;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_HDMI = u16TimingStableCounter;
    pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_HDMI = u16TimingNosyncCounter;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_PCMonitor_SetTimingCount(MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_PCMONITOR_SETTIMINGCOUNT XCArgs;
    XCArgs.u16TimingStableCounter = u16TimingStableCounter;
    XCArgs.u16TimingNosyncCounter = u16TimingNosyncCounter;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_SETTIMINGCOUNT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

E_APIXC_ReturnValue MApi_XC_PCMonitor_SetTimingCountEx_U2(void* pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter)
{
    E_APIXC_ReturnValue eResult = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (IsSrcTypeVga(eCurrentSrc))
    {
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_VGA = u16TimingStableCounter;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_VGA = u16TimingNosyncCounter;
        eResult = E_APIXC_RET_OK;
    }
    else if (IsSrcTypeYPbPr(eCurrentSrc))
    {
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_YPBPR= u16TimingStableCounter;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_YPBPR= u16TimingNosyncCounter;
        eResult = E_APIXC_RET_OK;
    }
    else if (IsSrcTypeHDMI(eCurrentSrc) || IsSrcTypeDVI(eCurrentSrc))
    {
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_HDMI= u16TimingStableCounter;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_HDMI= u16TimingNosyncCounter;
        eResult = E_APIXC_RET_OK;
    }
    else
    {
        printf("<ATTENTION> Not support this source type for timing count setting ! \n");
        eResult = E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eResult;
}

E_APIXC_ReturnValue MApi_XC_PCMonitor_SetTimingCountEx(INPUT_SOURCE_TYPE_t eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_PCMONITOR_SETTIMINGCOUNTEX XCArgs;
    XCArgs.eCurrentSrc = eCurrentSrc;
    XCArgs.u16TimingStableCounter = u16TimingStableCounter;
    XCArgs.u16TimingNosyncCounter = u16TimingNosyncCounter;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_SETTIMINGCOUNTEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_PCMONITOR_STATUS MApi_XC_PCMonitor_GetCurrentState_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    XC_PCMONITOR_STATUS eReturn = E_XC_PCMONITOR_STABLE_NOSYNC;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    eReturn = stPCMonitorInfo.eCurrentState;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eReturn;
}

XC_PCMONITOR_STATUS MApi_XC_PCMonitor_GetCurrentState(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }

    stXC_PCMONITOR_GETCURRENTSTATE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_XC_PCMONITOR_STABLE_NOSYNC;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GETCURRENTSTATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//*************************************************************************
//------------------------------ Warning ----------------------------------
//Attention: !!!!!!------ Don't add UI variable or unneeded code in this file
//
//           Don't modify this procedure !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//*************************************************************************
//*************************************************************************
//Function name:    MApi_XC_PCMonitor_SyncLoss
//Passing parameter:    none
//Return parameter: MS_BOOL:  true: sync loss    false: sync active
//Description:          Get signal sync status, true for sync loss, false for sync active
//*************************************************************************
MS_BOOL MApi_XC_PCMonitor_SyncLoss_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    bReturn = MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_PCMonitor_SyncLoss(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_PCMONITOR_SYNCLOSS XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_SYNCLOSS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U8 MApi_XC_PCMonitor_GetSyncStatus_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    u8Return = stPCMonitorInfo.u8SyncStatus;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u8Return;
}

MS_U8 MApi_XC_PCMonitor_GetSyncStatus(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_PCMONITOR_GETSYNCSTATUS XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GETSYNCSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U8 MDrv_XC_PCMonitor_GetVSyncTime(void *pInstance, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);

    if((stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_UNSTABLE) || (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        return 0;
    }
    else
    {
        return stPCMonitorInfo.u16VSyncTime;
    }
}
void MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info_U2(void* pInstance, SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(pInstance, eWindow,msWin);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    static MS_WINDOW_TYPE tmp_msWin;
    memcpy(&tmp_msWin,msWin,sizeof(MS_WINDOW_TYPE));
    stXC_PCMONITOR_GET_DVI_HDMI_DE_INFO XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.msWin = &tmp_msWin;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GET_DVI_HDMI_DE_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        memcpy(msWin,&tmp_msWin,sizeof(MS_WINDOW_TYPE));
        return;
    }
}

MS_U16 MApi_XC_PCMonitor_Get_HFreqx10_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    u16Return = MDrv_XC_PCMonitor_Get_HFreqx10(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u16Return;
}

MS_U16 MApi_XC_PCMonitor_Get_HFreqx10(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_PCMONITOR_GET_HFREQX10 XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GET_HFREQX10, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U32 MApi_XC_PCMonitor_Get_HFreqx1K_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    MS_U32 u32HFreq = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    if((stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_UNSTABLE) || (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        u32HFreq = 0;
    }
    else
    {
        u32HFreq = MDrv_XC_CalculateHFreqx1K(pInstance, stPCMonitorInfo.u16Hperiod);

    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32HFreq;
}

MS_U32 MApi_XC_PCMonitor_Get_HFreqx1K(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_PCMONITOR_GET_HFREQX1K XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GET_HFREQX1K, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}
MS_U16 MDrv_XC_PCMonitor_Get_VFreqx10(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16VFreq = 0, u16AccurateVFreq = 0;
    MS_U32 u32AccurateVPeriod = 0;
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);

    if((stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_UNSTABLE) || (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        // Original return value is 1
        // Change to 1 to prevent 4K2K home screen (4K2K 30Hz) force SetPanelTiming with no signal cause core dump issue.
        return 1;
    }
    else
    {
        if(FALSE == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFast_Get_VFreq))
        {
        u16VFreq = MDrv_XC_CalculateVFreqx10(pInstance, MDrv_XC_PCMonitor_Get_HFreqx10(pInstance, eWindow), stPCMonitorInfo.u16Vtotal);
        if(stPCMonitorInfo.u8SyncStatus& XC_MD_INTERLACE_BIT)
        {
            u16VFreq *= 2;
        }

        /// for more correct Vfreq to instead
        u32AccurateVPeriod = MDrv_SC_ip_get_verticalperiod(pInstance, eWindow);
        if(u32AccurateVPeriod != 0)
        {
              u16AccurateVFreq = (MS_U16)((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 10 +u32AccurateVPeriod /2) / u32AccurateVPeriod);
        }

        XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "InputVFreq=%u->%u, Tolerance=%u\n", u16VFreq, u16AccurateVFreq, (MS_U8)ACCURATE_VFREQ_TOLERANCE);
        if(abs(u16VFreq - u16AccurateVFreq) < ACCURATE_VFREQ_TOLERANCE)
        {
            u16VFreq = u16AccurateVFreq; //In tolerance range, using new accurate vfreq
        }
        }
        else
        {
            u16VFreq =(MS_U16)(stPCMonitorInfo.u32FrameRatex1K/100);
        }

        return u16VFreq;
    }
}

MS_U16 MApi_XC_PCMonitor_Get_VFreqx10_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    _XC_ENTRY(pInstance);
    u16Return = MDrv_XC_PCMonitor_Get_VFreqx10(pInstance, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u16Return;
}

MS_U16 MApi_XC_PCMonitor_Get_VFreqx10(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_PCMONITOR_GET_VFREQX10 XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GET_VFREQX10, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//comparing with MApi_XC_PCMonitor_Get_VFreqx1K, MDrv_XC_PCMonitor_Get_FrameRatex1K gets Frame rate without judging Input timing stable.
MS_U32 MDrv_XC_PCMonitor_Get_FrameRatex1K(void* pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32VFreq = 0, u32HFreq=0, u32AccurateVFreq = 0, u32AccurateVPeriod = 0 /*,u32PrevAccurateVPeriod = 0*/;
    MS_U64 u64XTALClk = 0;
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //MS_U8 u8VSyncTime=0, u8Count=0;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);

    u32HFreq = MDrv_XC_CalculateHFreqx1K(pInstance, stPCMonitorInfo.u16Hperiod);
    u32VFreq = MDrv_XC_CalculateVFreqx1K(pInstance, u32HFreq, stPCMonitorInfo.u16Vtotal);
    if(stPCMonitorInfo.u8SyncStatus& XC_MD_INTERLACE_BIT)
    {
        u32VFreq *= 2;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "u32VFreq = %tu\n", (ptrdiff_t)u32VFreq);
#if 0
    // for more correct VPeriod
    if (u32VFreq != 0)
        u8VSyncTime = stPCMonitorInfo.u16VSyncTime =((1000000)+(u32VFreq/2))/(u32VFreq);
    else
        u8VSyncTime = WAIT_VSYNC_TIME_MAX;

    while (u8Count < MAX_GET_VPERIOD_TIME)
    {
        u32AccurateVPeriod = HAL_SC_ip_get_verticalperiod(pInstance, eWindow);
        if (abs(u32AccurateVPeriod - u32PrevAccurateVPeriod) <= PERIOD_TOLARANCE)
        {
            break;
        }
        u32PrevAccurateVPeriod = u32AccurateVPeriod;
        MsOS_DelayTask(u8VSyncTime);
        u8Count++;
    }
#else
    u32AccurateVPeriod = HAL_SC_ip_get_verticalperiod(pInstance, eWindow);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "u32AccurateVPeriod = %tu\n", (ptrdiff_t)u32AccurateVPeriod);

    if(u32AccurateVPeriod != 0)
    {
        u64XTALClk = (MS_U64)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000;
        do_div(u64XTALClk,u32AccurateVPeriod);
        u32AccurateVFreq = (MS_U32)(u64XTALClk);
        //u32AccurateVFreq = (MS_U32)(u64XTALClk / u32AccurateVPeriod);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "u32AccurateVFreq = %tu\n", (ptrdiff_t)u32AccurateVFreq);

    XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "InputVFreq=%tu->%tu, Tolerance=%tu\n", (ptrdiff_t)u32VFreq, (ptrdiff_t)u32AccurateVFreq, (ptrdiff_t)ACCURATE_VFREQ_TOLERANCE * 100);
    if(abs(u32VFreq - u32AccurateVFreq) < ACCURATE_VFREQ_TOLERANCE * 100)
    {
        u32VFreq = u32AccurateVFreq; //In tolerance range, using new accurate vfreq
    }
    return u32VFreq;
}

MS_U32 MApi_XC_PCMonitor_Get_VFreqx1K_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32VFreq = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    u32VFreq = MDrv_XC_PCMonitor_Get_VFreqx1K(pInstance, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32VFreq;
}

MS_U32 MApi_XC_PCMonitor_Get_VFreqx1K(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_PCMONITOR_GET_VFREQX1K XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GET_VFREQX1K, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_BOOL MDrv_XC_PCMonitor_GetDEWidthHeightInDEByPassMode(void* pInstance, MS_U16* pu16Width, MS_U16* pu16Height, SCALER_WIN eWindow)
{
    if(MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow) == FALSE)
    {
        printf("[%s,%5d] Cannot get HV width and height under DE mode!! please check your xc lib!\n", __FUNCTION__,__LINE__);
        return FALSE;
    }
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);

    MDrv_XC_GetDEWidthHeightInDEByPassMode(pInstance, pu16Width, pu16Height, eWindow);
    stPCMonitorInfo.sDVI_HDMI_DE_Info.width  = *pu16Width;
    stPCMonitorInfo.sDVI_HDMI_DE_Info.height = *pu16Height;
    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
    return TRUE;
}

MS_U16 MApi_XC_PCMonitor_Get_Vtotal_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    u16Return = MDrv_XC_PCMonitor_Get_Vtotal(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return  u16Return;
}

MS_U16 MApi_XC_PCMonitor_Get_Vtotal(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_PCMONITOR_GET_VTOTAL XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD_GET_VTOTAL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void MDrv_XC_PCMonitor_Update_TimingCount(void *pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (IsSrcTypeVga(eCurrentSrc))
    {
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow] = pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_VGA;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow] = pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_VGA;
    }
    else if (IsSrcTypeYPbPr(eCurrentSrc))
    {
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow] = pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_YPBPR;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow] = pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_YPBPR;
    }
    else if (IsSrcTypeHDMI(eCurrentSrc) || IsSrcTypeDVI(eCurrentSrc))
    {
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow] = pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter_HDMI;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow] = pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter_HDMI;
    }
    else
    {
        printf("<ATTENTION> Not support timing count for this source, use default vaule \n");
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow] = MD_TIMING_STABLE_COUNT;
        pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow] = MD_TIMING_NOSYNC_COUNT;
    }
}

static void MDrv_XC_NewModeInterlacedDetect(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    // enable H/V sync status check
    MDrv_XC_SetHVSyncStatusCheck(pInstance, ENABLE, eWindow);
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    if((IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc)
        || IsSrcTypeDVI(stPCMonitorInfo.eCurrentSrc)
        || IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc))
        && (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_SYNC))
    {
        MS_WINDOW_TYPE stCapWin;
        MS_U16 u16HFreqx10, u16VFreqx10, u16Vtotal;
        MDrv_XC_GetCaptureWindow(pInstance, &stCapWin, eWindow);

        if((stCapWin.width > 1915) && (stCapWin.width < 1925)
            && (stCapWin.height > 535) && (stCapWin.height < 545)
            && ((u16HFreqx10 = MDrv_XC_PCMonitor_Get_HFreqx10(pInstance, eWindow)) > 305) && (u16HFreqx10 < 315)
            && ((u16VFreqx10 = MDrv_XC_PCMonitor_Get_VFreqx10(pInstance, eWindow)) > 498) && (u16VFreqx10 < 502)
            && ((u16Vtotal = MDrv_XC_PCMonitor_Get_Vtotal(pInstance, eWindow)) > 620) && (u16Vtotal < 630)
            && (Hal_SC_ip_get_interlace_status(pInstance, eWindow) == 0))
        {
            // enable new mode interlaced detect
            MDrv_XC_SetNewModeInterlacedDetect(pInstance, ENABLE, 2, ENABLE, TRUE, FALSE, eWindow);
            stPCMonitorInfo.eCurrentState = E_XC_PCMONITOR_UNSTABLE;
            pXCResourcePrivate->stapiXC_PCMonitor._bNewModeInterlacedDetect = TRUE;
        }
        else if((pXCResourcePrivate->stapiXC_PCMonitor._bNewModeInterlacedDetect == TRUE)
            && (!(((u16HFreqx10 = MDrv_XC_PCMonitor_Get_HFreqx10(pInstance, eWindow)) > 305) && (u16HFreqx10 < 315)
            && ((u16VFreqx10 = MDrv_XC_PCMonitor_Get_VFreqx10(pInstance, eWindow)) > 498) && (u16VFreqx10 < 502)
            && ((u16Vtotal = MDrv_XC_PCMonitor_Get_Vtotal(pInstance, eWindow)) > 1245) && (u16Vtotal < 1255)
            && (Hal_SC_ip_get_interlace_status(pInstance, eWindow) == 1))))
        {
            // disable new mode interlaced detect
            MDrv_XC_SetNewModeInterlacedDetect(pInstance, DISABLE, 0, DISABLE, FALSE, FALSE, eWindow);
            pXCResourcePrivate->stapiXC_PCMonitor._bNewModeInterlacedDetect = FALSE;
        }
    }
    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
}

//*************************************************************************
//Function name:    MApi_XC_PCMonitor
//Passing parameter:    none
//Return parameter: none
//Description:          Monitor signal timing from analog port
//
//Attention: !!!!!!------ Don't put UI/CC code in this function,
//           if need, you can use in "MApp_PCMode_PrepareForTimingChange" ------!!!!!!
//
//*************************************************************************
XC_PCMONITOR_STATUS MApi_XC_PCMonitor_U2(void* pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc, SCALER_WIN eWindow)
{
//#ifdef VGA_PATH_WITH_AUDIO
    //static bit bNoSyncStatus=0;
//#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    PCMSG(static MS_BOOL bNoSyncStatus = TRUE);
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_U8 u8Interlace = 0;

    if(MDrv_SC_Check_IP_Gen_Timing(pInstance))
    {
        //it ip gen timing case, we need to return previous state to AP to keep everything unchanged.
        return stPCMonitorInfo.eCurrentState;
    }

    if(stPCMonitorInfo.eCurrentSrc != eCurrentSrc)
    {
        PCMSG(printf("Restart src %d\n", eCurrentSrc));
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
        stPCMonitorInfo.eCurrentSrc = eCurrentSrc;
        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
        MDrv_XC_PCMonitor_Restart(pInstance, eWindow);             // source changed, restart monitor

        stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
        MDrv_XC_PCMonitor_Update_TimingCount(pInstance, eCurrentSrc, eWindow);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    }

    //printf("PCMonitor %d, %ld, %ld\n", psPCMONITOR_HK_Info[eWindow]->bInputTimingChange,
    //        psPCMONITOR_HK_Info[eWindow]->u32LastExecutedTime,
    //        OS_SYSTEM_TIME());

    if((FALSE == stPCMonitorInfo.bInputTimingChange) &&
        (MsOS_GetSystemTime() - stPCMonitorInfo.u32LastExecutedTime < PC_POLLING_COUNTER))
    {
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return stPCMonitorInfo.eCurrentState;
    }
    else
    {
        // update last executed time
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
        stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
        stPCMonitorInfo.u32LastExecutedTime = MsOS_GetSystemTime();
        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    }

    // DE-Bypass is applied in HDMI HV mode. It will ignore pixel repetition package from HDMI.
    // We use the software mointor to monitor this package.
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( (IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc) || IsSrcTypeDVI(eCurrentSrc) )
      &&(MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_HV)
      &&(eWindow == MAIN_WINDOW) )
    {
        MS_U8 u8CurHDMIPixelRep = MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x0F;

        MS_U8 u8IP_PixelRep = gSrcInfo[eWindow].Status2.u8IP_PixelRep;
        if( u8CurHDMIPixelRep != u8IP_PixelRep)
        {
            if( u8CurHDMIPixelRep )
            {
                //printf("daniel_test PC HV mode: Pixel-Rep=1 FIR=81\n");
                // If avi pkg report pixel repetition, IP1 should enable divider.
                MApi_XC_SetFIRDownSampleDivider(pInstance, ENABLE,0x01,eWindow);
            }
            else
            {
                //printf("daniel_test PC HV mode: Pixel-Rep=0 FIR=0\n");
                MApi_XC_SetFIRDownSampleDivider(pInstance, DISABLE,0x00,eWindow);
            }
            //_s_u32LastResetTime = MsOS_GetSystemTime();
            //MApi_XC_SoftwareReset(REST_IP_F2, eWindow);

            stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
            stPCMonitorInfo.bInputTimingChange = TRUE;
            MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);

            //printf("@@@--- HDMI Pixel-Rep change from [%u] to [%u]\n", gSrcInfo[eWindow].Status2.u8IP_PixelRep, u8CurHDMIPixelRep);
            gSrcInfo[eWindow].Status2.u8IP_PixelRep = u8CurHDMIPixelRep;
        }
    }

    //--------------------------------------------------
    //display active or no signal?
    if ( stPCMonitorInfo.bInputTimingChange == FALSE ) // check timing change flag have be setting
    {
        if ( MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow) ) // no sync
        {
            MDrv_XC_SetSogDetectStatus(pInstance, pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus);
            if ( _MApi_XC_PCMonitor_DetectSync(pInstance, stPCMonitorInfo.eCurrentSrc,eWindow) ) // check signal active
            {
                stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
                stPCMonitorInfo.bInputTimingChange = TRUE;
                MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                PCMSG(if(bNoSyncStatus) printf("NewSync\n"); bNoSyncStatus = FALSE);
            }
            else
            {
                PCMSG(if(!bNoSyncStatus) printf("NoSync\n"); bNoSyncStatus = TRUE);
            }
        }
        else //have sync?
        {
            _XC_ENTRY(pInstance);
            if(gSrcInfo[eWindow].bAutoNoSignalEnabled && pXCResourcePrivate->stapiXC_PCMonitor._bEnableAutoNoSignalDetect[eWindow])
            {
                // check invalid
                if ( MDrv_XC_PCMonitor_InvalidTimingDetect(pInstance, TRUE, eWindow) )
                {
                    ++pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[eWindow];

                    // PC monitor have to changed when auto nosignal enabled.
                    // If no signal bit is enabled, that's treaded as no-signal
                    // Here have to update variables for hw-detected-no-signal.
                    if ( pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[eWindow] > 3)
                    {
                        Hal_SC_ip_set_IPAutoNoSignal(pInstance, DISABLE,MAIN_WINDOW);
                        #if (HW_DESIGN_4K2K_VER >= 4 )
                        // if enable input source before video mute, it would shows temp garbage
                        // so need to mute video first before enable input source
                        MDrv_SC_GenerateBlackVideo(pInstance, TRUE, eWindow );
#ifndef DISABLE_3D_FUNCTION
                        if( (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)
                            || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM)
                            || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW)
                            || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
                            || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_PACKING)
                            || (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)
                        )
                        {
                        #if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                            MDrv_SC_GenerateBlackVideo(g_pDevice1Instance, TRUE,  MAIN_WINDOW);
                        #else
                            MDrv_SC_GenerateBlackVideo(pInstance, TRUE,  SUB_WINDOW);
                        #endif
                        }
 #endif

                        // Only SW patch for YPbPr timing change case, reset IP1 to avoid HW Auto No signal not work
                        if((MAIN_WINDOW == eWindow) && IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc))
                        {
                            Hal_SC_ip_software_reset(pInstance, REST_IP_F2, MAIN_WINDOW);
                        }

                        #endif
                        MDrv_XC_DisableInputSource(pInstance, DISABLE, eWindow); //enable the input source
                        MDrv_XC_PCMonitor_InvalidTimingDetect(pInstance, FALSE, eWindow);
                        stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
                        stPCMonitorInfo.u8SyncStatus |= XC_MD_SYNC_LOSS;
                        stPCMonitorInfo.bInputTimingChange = TRUE;
                        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                        pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[eWindow] = 0;
                    }
                }
                else // time stable
                {
                    if( gSrcInfo[eWindow].bAutoNoSignalEnabled )
                    {
                        if( MDrv_XC_IsInputSourceDisabled(pInstance, eWindow)&&(pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[eWindow]!=0)
                        &&(stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_SYNC) )
                        {
                            MDrv_XC_DisableInputSource(pInstance, DISABLE, eWindow); //enable the input source
                            pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[eWindow] = 0;
                        }
                        else
                        {
                            pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[eWindow]++;
                        }
                    }

                    if((pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[eWindow] ==20) && MDrv_XC_IsInputSourceDisabled(pInstance, eWindow)) // to check the input source status
                    {
                        MDrv_XC_DisableInputSource(pInstance, DISABLE, eWindow);
                        pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[eWindow]=0;
                    }

                    if(pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[eWindow] >=20)
                    {
                        pXCResourcePrivate->stapiXC_PCMonitor.u8DeStableCnt[eWindow] = 0;
                    }

                    pXCResourcePrivate->stapiXC_PCMonitor.u8DeNoiseCount[eWindow] = 0;
                }
            }
            else if ( MDrv_XC_PCMonitor_InvalidTimingDetect(pInstance, FALSE, eWindow) ) // check invalid sync
            {
                PCMSG(printf("InvalidTimingDetect GO TO UNSTABLE! \n" )) ;
                stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
                stPCMonitorInfo.u8SyncStatus |= XC_MD_SYNC_LOSS;
                stPCMonitorInfo.bInputTimingChange = TRUE;
                MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                MDrv_DVI_Accumulator_Monitor();
            }
            _XC_RETURN(pInstance);

            #if ENABLE_IP_AUTO_COAST
            if ( IsSrcTypeVideo(stPCMonitorInfo.eCurrentSrc) )
            {
                msAPI_Scaler_IPAutoCoastHandler();
            }
            #endif
        }
    }

    //is input timing changed and check debounce
    //--------------------------------------------------
    if ( stPCMonitorInfo.bInputTimingChange) // input timing is unstable
    {
        PCMSG(printf("TimingChg\n"));
        stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
        stPCMonitorInfo.bInputTimingChange = FALSE;
        stPCMonitorInfo.ucTimingChangeDebounce++;

        PCMSG(printf("Deb=%d\n", stPCMonitorInfo.ucTimingChangeDebounce));

        if ( !MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow) ) // have signal
        {
            if ( stPCMonitorInfo.ucTimingChangeDebounce <= 3 ) // prevent from SOG
            {
                stPCMonitorInfo.u8SyncStatus |= XC_MD_SYNC_LOSS;
                MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
                return stPCMonitorInfo.eCurrentState;
            }
        }
        else
        {
            if ( IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc ) ||
                 IsSrcTypeVga(stPCMonitorInfo.eCurrentSrc )  )
            {
                // System have to set free run as soon as possible when timing unstable.
                MDrv_XC_ADC_Set_Freerun(pInstance, TRUE);
            }
        }

        stPCMonitorInfo.eCurrentState = E_XC_PCMONITOR_UNSTABLE;
        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);

        _MApi_XC_PCMonitor_PrepareForTimingChange(pInstance, eWindow);

        stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
        // Already enter timing change, turn off AutoNoSignal
        if( gSrcInfo[eWindow].bAutoNoSignalEnabled )
        {
            //MApi_XC_EnableIPAutoNoSignal(FALSE, eWindow);
            pXCResourcePrivate->stapiXC_PCMonitor._bEnableAutoNoSignalDetect[eWindow] = FALSE;
            Hal_SC_ip_set_IPAutoNoSignal(pInstance, DISABLE,eWindow);
            // HW PIP architeucture
            // sub's input source and extra request should always enable/disable together
            if(MDrv_XC_IsInputSourceDisabled(pInstance, eWindow) && (eWindow == SUB_WINDOW))
            {
                MDrv_XC_Enable_Extra_Request(pInstance, FALSE);
            }
            //printf("daniel_test AutoNoSignal=0 t=%lu %s\n", MsOS_GetSystemTime(), __FUNCTION__);
        }
    }
    else // input timing is stable
    {
        stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
        stPCMonitorInfo.ucTimingChangeDebounce = 0; // reset timing change debounce counter

        stPCMonitorInfo.ucInputTimingStableCounter++; // timing stable counter

        if ( stPCMonitorInfo.ucInputTimingStableCounter == 0 )
            stPCMonitorInfo.ucInputTimingStableCounter = 1;

        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
        if ( !stPCMonitorInfo.bInputTimingStable )
        {
#ifndef MSOS_TYPE_LINUX_KERNEL
            MS_ASSERT(pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow] > 0);
            MS_ASSERT(pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow] > 0);
#endif
            if( ((!MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow)) && (stPCMonitorInfo.ucInputTimingStableCounter >= pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow])) ||
                (MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow) && (stPCMonitorInfo.ucInputTimingStableCounter >= pXCResourcePrivate->stapiXC_PCMonitor._u16TimingNosyncCounter[eWindow])) )
            {
                stPCMonitorInfo.bInputTimingStable = TRUE;
                MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                //g_PcadcModeSetting.u8ModeIndex = 0;  // reset mode index
                pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus = MDrv_XC_GetSogDetectStatus(pInstance);

                if (MDrv_XC_GetVGASogEn(pInstance))     // Daten FixMe -> Please refine VGA SOG flow
                {
                    if(IsSrcTypeVga(stPCMonitorInfo.eCurrentSrc) &&  // Is RGB in
                        (!MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow)) &&                     // Has signal
                        ((pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus & (0x0001) ) == (0x0001)) &&          // In detecting SOG
                        (!((pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus & (0x0002)) == (0x0002))))         // 1st time detecting SOG

                    {
                        // only if RGB input, has sync and in SOG detecting enter here
                        // force to check from other sync type again.
                        pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus |= (0x0002);
                        stPCMonitorInfo.u8SyncStatus ^= XC_MD_VSYNC_POR_BIT;

                        stPCMonitorInfo.bInputTimingStable = TRUE;
                        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                    }
                    else
                    {
                        if(!MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow))
                        {
                            // has sync, no matter what sync detected, clear the SOG_DETECTED_BIT
                            pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus &= ~(0x0002);
                        }
                        if((pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus & (0x0001)) &&
                             (MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow)))
                        {
                            // in detecting the SOG and no Sync, clear the SOG_DETECTED_BIT
                            pXCResourcePrivate->stapiXC_PCMonitor.g_u8RGBSOG_DetectStatus &= ~(0x0002);
                        }
                        stPCMonitorInfo.bInputTimingStable = TRUE;
                        MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                    }
                }

                if(MDrv_XC_PCMonitor_SyncLoss(pInstance, eWindow))
                {
                    stPCMonitorInfo.eCurrentState = E_XC_PCMONITOR_STABLE_NOSYNC;
                    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                }
                else
                {
                    stPCMonitorInfo.eCurrentState = E_XC_PCMONITOR_STABLE_SYNC;
                    // for get accurate v freq, need call msAPI_Scaler_Field_Detect and MDrv_XC_ADC_Set_Freerun
                    u8Interlace = Hal_SC_ip_get_interlace_status(pInstance, eWindow);
                    msAPI_Scaler_Field_Detect(pInstance, stPCMonitorInfo.eCurrentSrc, u8Interlace, eWindow);
                    if ( IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc ) ||
                        IsSrcTypeVga(stPCMonitorInfo.eCurrentSrc )  )
                    {
                        MDrv_XC_ADC_Set_Freerun(pInstance, FALSE);
                    }
                    // Update V sync time when timing stable.
                    stPCMonitorInfo.u16VSyncTime = MApi_XC_CalculateVsyncTime(pInstance, stPCMonitorInfo.u16Vtotal, stPCMonitorInfo.u16Hperiod);
                    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
                    if ( IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc ) )
                    {
                        _MApi_XC_PCMonitor_FPi_Patch(pInstance, eWindow);
                    }
                    // error flow: setwindow ->MDrv_XC_ADC_Reload -> enable auto nosignal -> htt change because ADC reload -> auto no signal disable input source(HW) -> load PQ -> unmute
                    // it's not flash buffer because load PQ in disable input source
                    // so here is not enable auto nosignal when video mute
                    if(gSrcInfo[eWindow].bAutoNoSignalEnabled)
                    {
                        MDrv_XC_DisableInputSource(pInstance, DISABLE, eWindow);
                        //YPbPr have some garbage when enable the auto signal detect.
                        if((!IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc)) || (!MDrv_SC_CheckMuteStatusByRegister(pInstance,  eWindow)))
                        {
                            pXCResourcePrivate->stapiXC_PCMonitor._bEnableAutoNoSignalDetect[eWindow] = TRUE;
                            Hal_SC_ip_set_IPAutoNoSignal(pInstance, ENABLE,eWindow);
                        }
                    }
                    //_MApi_XC_PCMonitor_ModeParse(eWindow);
                }

                PCMSG(printf("*** Do Mode In Analog Path ***\n"));
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
                return stPCMonitorInfo.eCurrentState;
            }
        }

        if(gSrcInfo[eWindow].bAutoNoSignalEnabled)
        {
			_XC_ENTRY(pInstance);
            if(( stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_SYNC)
                &&(stPCMonitorInfo.ucInputTimingStableCounter
                    >=pXCResourcePrivate->stapiXC_PCMonitor._u16TimingStableCounter[eWindow])
                &&(!Hal_SC_ip_get_IPAutoNoSignal(pInstance, eWindow ))&&(!MDrv_XC_IsInputSourceDisabled(pInstance, eWindow)))
            {
                // error flow: setwindow ->MDrv_XC_ADC_Reload -> enable auto nosignal -> htt change because ADC reload -> auto no signal disable input source(HW) -> load PQ -> unmute
                // it's not flash buffer because load PQ in disable input source
                // so here is not enable auto nosignal when video mute
                if((!IsSrcTypeYPbPr(stPCMonitorInfo.eCurrentSrc)) || (!MDrv_SC_CheckMuteStatusByRegister(pInstance,  eWindow)))
                {
                    //when signal stable, enable the auto signal detect.
                    pXCResourcePrivate->stapiXC_PCMonitor._bEnableAutoNoSignalDetect[eWindow] = TRUE;
                    Hal_SC_ip_set_IPAutoNoSignal(pInstance, ENABLE,eWindow);
                }
            }
			_XC_RETURN(pInstance);
        }

    }

    //Mantis issue of 0270665. Set the DE HV Mode when pre monitor state is E_XC_PCMONITOR_UNSTABLE .
    if((IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc) || IsSrcTypeDVI(stPCMonitorInfo.eCurrentSrc) )
        && ((stPCMonitorInfo.ePreMonitorState == E_XC_PCMONITOR_UNSTABLE)
            || (stPCMonitorInfo.ePreMonitorState == E_XC_PCMONITOR_STABLE_NOSYNC)))
    {
        MDrv_XC_Switch_DE_HV_Mode_By_Timing(pInstance, eWindow);
    }

    if (IsSrcTypeHDMI(stPCMonitorInfo.eCurrentSrc) || IsSrcTypeDVI(stPCMonitorInfo.eCurrentSrc))
    {
#if SUPPORT_IP_HDMI_FOR_HV_MODE
        MDrv_XC_Switch_DE_HV_Mode_By_Timing(pInstance, eWindow);
#else
        //MDrv_XC_Set_DE_Lock_Mode(pInstance, eWindow);
#endif
        MDrv_DVI_HF_adjust_internal();
    }

    _XC_ENTRY(pInstance);
    MDrv_XC_NewModeInterlacedDetect(pInstance, eWindow);
    _XC_RETURN(pInstance);

    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    stPCMonitorInfo.ePreMonitorState = stPCMonitorInfo.eCurrentState;
    MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(pXCResourcePrivate->stdrvXC_MVideo.bTimingUnstableForStr == TRUE)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.u8DebounceForStr < TIMING_CHANGE_DEBOUNCE_FOR_STR)
        {
            stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
            stPCMonitorInfo.eCurrentState = E_XC_PCMONITOR_UNSTABLE;
            stPCMonitorInfo.bInputTimingChange = TRUE;
            stPCMonitorInfo.ePreMonitorState = stPCMonitorInfo.eCurrentState;
            MDrv_XC_PCMonitor_SetStatus(pInstance, stPCMonitorInfo, eWindow);
            ++(pXCResourcePrivate->stdrvXC_MVideo.u8DebounceForStr);
        }
        else
        {
            pXCResourcePrivate->stdrvXC_MVideo.bTimingUnstableForStr = FALSE;
            pXCResourcePrivate->stdrvXC_MVideo.u8DebounceForStr = 0;
        }
    }
#endif
    return stPCMonitorInfo.eCurrentState;
}

XC_PCMONITOR_STATUS MApi_XC_PCMonitor(INPUT_SOURCE_TYPE_t eCurrentSrc, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }

    stXC_PCMONITOR XCArgs;
    XCArgs.eCurrentSrc = eCurrentSrc;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_XC_PCMONITOR_STABLE_NOSYNC;

    if(UtopiaIoctl(pu32XCInst, E_XC_PCMONITOR_CMD, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

#undef _API_XC_PC_MONITOR_C_
#endif  // _API_XC_PC_MONITOR_C_
