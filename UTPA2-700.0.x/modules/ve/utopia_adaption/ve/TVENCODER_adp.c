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

////////////////////////////////////////////////////////////////////////////////
/// file    TVENCODER_adp.c
/// @brief  TV encoder.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _TVENCODER_ADP_C_
#define _TVENCODER_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
/*#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif*/
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"

// Internal Definition
#include "tvencoder.h"
#include "TVENCODER_adp.h"
extern spt_MS_NULL;
//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Structurs
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------

//Top
UADP_STRUCT_POINTER_TABLE spt_TVE_GetInfo[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_GetStatus[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetDbgLevel[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SwitchInputSRC[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetInputSRC[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SwitchOutputDest[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetOutputCtrl[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetOutputVideoStd[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetMemoryBaseAddr[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_Init[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_Exit[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetWSSData[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_GetWSSData[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetMode[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetBlackSceen[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_IsBlackScreenEnabled[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetTtxBuffer[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_VE_GetTtxReadDoneStatus[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_EnableCcSw[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetCcRange[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SendCcData[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_set_display_window[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetOSD[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetRGBIn[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_Get_Output_Video_Std[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetCaptureMode[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_GetDstInfo[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_Set_TestPattern[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_DisableRegWrite[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_AdjustPositionBase[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_Set_Customer_Scaling[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_InitVECapture[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_EnaVECapture[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_Adjust_FrameStart[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_SetPowerState[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_ReloadSetting[5];
UADP_STRUCT_POINTER_TABLE spt_TVE_GetConfig[5];

//Bottom
UADP_STRUCT_POINTER_TABLE spt_VE_DrvInfo[5];
UADP_STRUCT_POINTER_TABLE spt_VE_VE_Caps[5];
UADP_STRUCT_POINTER_TABLE spt_VE_DrvStatus[5];
UADP_STRUCT_POINTER_TABLE spt_VE_SwithcSrc_info[5];
UADP_STRUCT_POINTER_TABLE spt_VE_InputSrcInfo[5];
UADP_STRUCT_POINTER_TABLE spt_VE_Switch_Dest_Info[5];
UADP_STRUCT_POINTER_TABLE spt_VE_Output_Ctrl[5];
UADP_STRUCT_POINTER_TABLE spt_VE_DrvSetMode[5];
UADP_STRUCT_POINTER_TABLE spt_VE_DST_DispInfo[5];
//UADP_STRUCT_POINTER_TABLE spt_VE_SetDisplayWin[5];
UADP_STRUCT_POINTER_TABLE spt_VE_CusScalingInfo[5];
UADP_STRUCT_POINTER_TABLE spt_VE_Output_CAPTURE[5];
UADP_STRUCT_POINTER_TABLE spt_VE_VECAPTURESTATE[5];
UADP_STRUCT_POINTER_TABLE spt_VE_WINDOW_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_VE_Config[5];
//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 TVENCODER_adp_Init(FUtopiaIOctl* pIoctl)
{

//set table
    MS_U32 temp=0;

    //Top
    PTVE_GETINFO          pGetInfo = (PTVE_GETINFO)temp;
	PTVE_GETSTATUS        pGetStatus = (PTVE_GETSTATUS)temp;
	//PTVE_SETDBGLEVEL      pDbgLevel  = (PTVE_SETDBGLEVEL)temp;
	PTVE_SWITCH_SRC_INFO  pVE_SwitchSrc_info   = (PTVE_SWITCH_SRC_INFO)temp;
	PTVE_SETINPUTSRC      pVE_SetInputSrc      = (PTVE_SETINPUTSRC)temp;
	PTVE_SWITCHOUTPUTDEST pSwitchOutputDest = (PTVE_SWITCHOUTPUTDEST)temp;
	PTVE_SETOUTPUTCTRL pSetOutputCtrl = (PTVE_SETOUTPUTCTRL)temp;
	//PTVE_SETOUTPUTVIDEOSTD pSetOutputVideoStd = (PTVE_SETOUTPUTVIDEOSTD)temp;
	//PTVE_INIT pInit = (PTVE_INIT)temp;
	//PTVE_EXIT pExit = (PTVE_EXIT)temp;
	//PTVE_SETWSSDATA pSetWSSdata = (PTVE_SETWSSDATA)temp;
	//PTVE_GETWSSDATA pGetWSSData = (PTVE_GETWSSDATA)temp;
	PTVE_SETMODE pSetMode = (PTVE_SETMODE)temp;
	//PTVE_SETBLACKSCREEN pSetBlackScreen = (PTVE_SETBLACKSCREEN)temp;
	//PTVE_ISBLACKSCREENENABLE pBlackScreenEnable = (PTVE_ISBLACKSCREENENABLE)temp;
	//PTVE_SETTTXBUFFER pSetTtxBuffer = (PTVE_SETTTXBUFFER)temp;
	//PTVE_GETTTXREADDONE pGetTtxReadDoneStatus = (PTVE_GETTTXREADDONE)temp;
	//PTVE_ENABLECCSW pEnableCcSw = (PTVE_ENABLECCSW)temp;
	//PTVE_SETCCRANGE pSetCcRange = (PTVE_SETCCRANGE)temp;
	//PTVE_SENDCCDATA pSendCcData = (PTVE_SENDCCDATA)temp;
	//PTVE_SETDISPLAYWIN pSetDisplayWin = (PTVE_SETDISPLAYWIN)temp;
	//PTVE_SETOSD pSetOsd = (PTVE_SETOSD)temp;
	//PTVE_SETRGBIN pSetRGBIn = (PTVE_SETRGBIN)temp;
	//PTVE_GETOUTPUTVIDEOSTD pOutputVideoSTD = (PTVE_GETOUTPUTVIDEOSTD)temp;
	//PTVE_SETCAPMODE pSetCaptureMode = (PTVE_SETCAPMODE)temp;
	PTVE_GETDSTINFO pGetDstInfo = (PTVE_GETDSTINFO)temp;
	//PTVE_SETTESTPATTERN pSetTestPattern = (PTVE_SETTESTPATTERN)temp;
	//PTVE_DISABLEREGWRITE pDiableRegWrite = (PTVE_DISABLEREGWRITE)temp;
	//PTVE_ADJPOSITIONBASE pAdjPositionBase = (PTVE_ADJPOSITIONBASE)temp;
	PTVE_SETCUSSCALING pSetCusScaling = (PTVE_SETCUSSCALING)temp;
	P_TVE_INITCAPTURE pInitCapture = (P_TVE_INITCAPTURE)temp;
	PTVE_ENACAPTURE pEnableCapture =(PTVE_ENACAPTURE)temp;
	PTVE_ADJFRAMESTART pAdjustFrameStart = (PTVE_ADJFRAMESTART)temp;
	//PTVE_SETPOWERSTATE pSetPowerState = (PTVE_SETPOWERSTATE)temp;
	PTVE_GETCONFIG        pGetConfig = (PTVE_GETCONFIG)temp;


	//Bottom
    //VE_DrvInfo* pVE_DrvInfo = (VE_DrvInfo*)temp;
    //VE_Caps* pVE_Caps = (VE_Caps*)temp;
	//VE_DrvStatus* pStaus = (VE_DrvStatus*)temp;
	//PMS_Switch_VE_Src_Info pSwithcSrc_info = (PMS_Switch_VE_Src_Info)temp;
	//PMS_VE_InputSrc_Info pInputSrcInfo = (PMS_VE_InputSrc_Info)temp;
	//PMS_Switch_VE_Dest_Info pSwitchInfo = (PMS_Switch_VE_Dest_Info)temp;
	//PMS_VE_Output_Ctrl pOutputCtrl = (PMS_VE_Output_Ctrl)temp;
	//MS_VE_Set_Mode_Type* pVESetMode = (MS_VE_Set_Mode_Type*)temp;
    //MS_VE_WINDOW_TYPE*	//(MS_VE_WINDOW_TYPE*)pVEstDispWin = (MS_VE_WINDOW_TYPE*) temp;
	//MS_VE_DST_DispInfo* pDstInfo = (MS_VE_DST_DispInfo*)temp;
	//MS_VE_CusScalingInfo* pstVECusScalingInfo = (MS_VE_CusScalingInfo*)temp;
	//PMS_VE_Output_CAPTURE pVECapture = (PMS_VE_Output_CAPTURE)temp;
	//PMS_VE_VECAPTURESTATE pstVECapState = (PMS_VE_VECAPTURESTATE)temp;
	//MS_VE_WINDOW_TYPE *stSrcWin = (MS_VE_WINDOW_TYPE*)temp;

//
    UADPBypassSetSPT(&spt_VE_VE_Caps[0],UADP_SPT_SELF_SIZE,0, sizeof(VE_Caps));
    UADPBypassSetSPT(&spt_VE_VE_Caps[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_DrvInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(VE_DrvInfo));
    //UADPBypassSetSPT(&spt_VE_DrvInfo[1],UADP_SPT_POINTER_TO_NEXT,
    //    ((unsigned long)(&(pVE_DrvInfo->stCaps))-(unsigned long)pVE_DrvInfo), &spt_VE_VE_Caps);
    UADPBypassSetSPT(&spt_VE_DrvInfo[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_DrvStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(VE_DrvStatus));
    UADPBypassSetSPT(&spt_VE_DrvStatus[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_SwithcSrc_info[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_Switch_VE_Src_Info));
    UADPBypassSetSPT(&spt_VE_SwithcSrc_info[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_InputSrcInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_InputSrc_Info));
    UADPBypassSetSPT(&spt_VE_InputSrcInfo[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_Switch_Dest_Info[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_Switch_VE_Dest_Info));
    UADPBypassSetSPT(&spt_VE_Switch_Dest_Info[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_Output_Ctrl[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_Output_Ctrl));
    UADPBypassSetSPT(&spt_VE_Output_Ctrl[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_DrvSetMode[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_Set_Mode_Type));
    UADPBypassSetSPT(&spt_VE_DrvSetMode[1],UADP_SPT_END , 0, 0);

    //UADPBypassSetSPT(&spt_VE_SetDisplayWin[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_WINDOW_TYPE));
    //UADPBypassSetSPT(&spt_VE_SetDisplayWin[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_DST_DispInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_DST_DispInfo));
    UADPBypassSetSPT(&spt_VE_DST_DispInfo[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_CusScalingInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_CusScalingInfo));
    UADPBypassSetSPT(&spt_VE_CusScalingInfo[1],UADP_SPT_END , 0, 0);

#ifdef BIFROST_32BIT_MODE
    UADP_SDT_NAME1(VE_Output_CAPTURE,MS_VE_Output_CAPTURE,UADP_SDT_AT,u32MiuBaseAddr,MS_NULL);
#else
    UADPBypassSetSPT(&spt_VE_Output_CAPTURE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_Output_CAPTURE));
    UADPBypassSetSPT(&spt_VE_Output_CAPTURE[1],UADP_SPT_END , 0, 0);
#endif

    UADPBypassSetSPT(&spt_VE_VECAPTURESTATE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_VECAPTURESTATE));
    UADPBypassSetSPT(&spt_VE_VECAPTURESTATE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_WINDOW_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_VE_WINDOW_TYPE));
    UADPBypassSetSPT(&spt_VE_WINDOW_TYPE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_VE_Config[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_PHYADDR));
    UADPBypassSetSPT(&spt_VE_Config[1],UADP_SPT_END , 0, 0);

//---
    UADPBypassSetSPT(&spt_TVE_GetInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETINFO));
    UADPBypassSetSPT(&spt_TVE_GetInfo[1],UADP_SPT_POINTER_TO_NEXT,
                      ((unsigned long)(&(pGetInfo->pVE_DrvInfo))-(unsigned long)pGetInfo), (unsigned long)&spt_VE_DrvInfo);
    UADPBypassSetSPT(&spt_TVE_GetInfo[2],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_TVE_GetStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETSTATUS));
    UADPBypassSetSPT(&spt_TVE_GetStatus[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pGetStatus->pStaus))-(unsigned long)pGetStatus), (unsigned long)&spt_VE_DrvStatus);
    UADPBypassSetSPT(&spt_TVE_GetStatus[2],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_TVE_SetDbgLevel[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETDBGLEVEL));
    UADPBypassSetSPT(&spt_TVE_SetDbgLevel[1],UADP_SPT_END , 0, 0);


	UADPBypassSetSPT(&spt_TVE_SwitchInputSRC[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SWITCH_SRC_INFO));
    UADPBypassSetSPT(&spt_TVE_SwitchInputSRC[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pVE_SwitchSrc_info->pSwithcSrc_info))-(unsigned long)pVE_SwitchSrc_info), (unsigned long)&spt_VE_SwithcSrc_info);
    UADPBypassSetSPT(&spt_TVE_SwitchInputSRC[2],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_TVE_SetInputSRC[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETINPUTSRC));
    UADPBypassSetSPT(&spt_TVE_SetInputSRC[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pVE_SetInputSrc->pInputSrcInfo))-(unsigned long)pVE_SetInputSrc), (unsigned long)&spt_VE_InputSrcInfo);
    UADPBypassSetSPT(&spt_TVE_SetInputSRC[2],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_TVE_SwitchOutputDest[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SWITCHOUTPUTDEST));
    UADPBypassSetSPT(&spt_TVE_SwitchOutputDest[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pSwitchOutputDest->pSwitchInfo))-(unsigned long)pSwitchOutputDest), (unsigned long)&spt_VE_Switch_Dest_Info);
    UADPBypassSetSPT(&spt_TVE_SwitchOutputDest[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_TVE_SetOutputCtrl[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETOUTPUTCTRL));
    UADPBypassSetSPT(&spt_TVE_SetOutputCtrl[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pSetOutputCtrl->pOutputCtrl)) - (unsigned long)pSetOutputCtrl), (unsigned long)&spt_VE_Output_Ctrl);
    UADPBypassSetSPT(&spt_TVE_SetOutputCtrl[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_TVE_SetOutputVideoStd[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETOUTPUTVIDEOSTD));
	UADPBypassSetSPT(&spt_TVE_SetOutputVideoStd[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetMemoryBaseAddr[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETMEMORYBASEADDR));
    UADPBypassSetSPT(&spt_TVE_SetMemoryBaseAddr[1],UADP_SPT_END , 0, 0);

    UADP_SDT_NAME1(TVE_Init,TVE_INIT,UADP_SDT_AT,u32MIUAddress,MS_NULL);

    UADPBypassSetSPT(&spt_TVE_Exit[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_EXIT));
	UADPBypassSetSPT(&spt_TVE_Exit[1],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_TVE_SetWSSData[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETWSSDATA));
	UADPBypassSetSPT(&spt_TVE_SetWSSData[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_GetWSSData[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETWSSDATA));
	UADPBypassSetSPT(&spt_TVE_GetWSSData[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetMode[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETMODE));
    UADPBypassSetSPT(&spt_TVE_SetMode[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pSetMode->pVESetMode)) - (unsigned long)pSetMode), (unsigned long)&spt_VE_DrvSetMode);
	UADPBypassSetSPT(&spt_TVE_SetMode[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetBlackSceen[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETBLACKSCREEN));
	UADPBypassSetSPT(&spt_TVE_SetBlackSceen[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_IsBlackScreenEnabled[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETBLACKSCREEN));
	UADPBypassSetSPT(&spt_TVE_IsBlackScreenEnabled[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetTtxBuffer[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETTTXBUFFER));
	UADPBypassSetSPT(&spt_TVE_SetTtxBuffer[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_VE_GetTtxReadDoneStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETTTXREADDONE));
	UADPBypassSetSPT(&spt_TVE_VE_GetTtxReadDoneStatus[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_EnableCcSw[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_ENABLECCSW));
	UADPBypassSetSPT(&spt_TVE_EnableCcSw[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetCcRange[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETCCRANGE));
	UADPBypassSetSPT(&spt_TVE_SetCcRange[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SendCcData[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SENDCCDATA));
	UADPBypassSetSPT(&spt_TVE_SendCcData[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_set_display_window[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETDISPLAYWIN));
	//UADPBypassSetSPT(&spt_TVE_set_display_window[1],UADP_SPT_POINTER_TO_NEXT,
    //    ((unsigned long)(&(pSetDisplayWin->stDispWin)) - (unsigned long)pSetDisplayWin), (unsigned long)&spt_VE_SetDisplayWin);
    UADPBypassSetSPT(&spt_TVE_set_display_window[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetOSD[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETOSD));
	UADPBypassSetSPT(&spt_TVE_SetOSD[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetRGBIn[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETRGBIN));
	UADPBypassSetSPT(&spt_TVE_SetRGBIn[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_Get_Output_Video_Std[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETOUTPUTVIDEOSTD));
	UADPBypassSetSPT(&spt_TVE_Get_Output_Video_Std[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetCaptureMode[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETCAPMODE));
	UADPBypassSetSPT(&spt_TVE_SetCaptureMode[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_GetDstInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETDSTINFO));
    UADPBypassSetSPT(&spt_TVE_GetDstInfo[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pGetDstInfo->pDstInfo)) - (unsigned long)pGetDstInfo), (unsigned long)&spt_VE_DST_DispInfo);
	UADPBypassSetSPT(&spt_TVE_GetDstInfo[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_Set_TestPattern[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETTESTPATTERN));
	UADPBypassSetSPT(&spt_TVE_Set_TestPattern[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_DisableRegWrite[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_DISABLEREGWRITE));
	UADPBypassSetSPT(&spt_TVE_DisableRegWrite[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_AdjustPositionBase[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_ADJPOSITIONBASE));
	UADPBypassSetSPT(&spt_TVE_AdjustPositionBase[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_Set_Customer_Scaling[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETCUSSCALING));
    UADPBypassSetSPT(&spt_TVE_Set_Customer_Scaling[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pSetCusScaling->pstVECusScalingInfo)) - (unsigned long)pSetCusScaling), (unsigned long)&spt_VE_CusScalingInfo);
	UADPBypassSetSPT(&spt_TVE_Set_Customer_Scaling[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_InitVECapture[0],UADP_SPT_SELF_SIZE,0, sizeof(_TVE_INITCAPTURE));
    UADPBypassSetSPT(&spt_TVE_InitVECapture[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pInitCapture->pVECapture)) - (unsigned long)pInitCapture), (unsigned long)&spt_VE_Output_CAPTURE);
	UADPBypassSetSPT(&spt_TVE_InitVECapture[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_EnaVECapture[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_ENACAPTURE));
    UADPBypassSetSPT(&spt_TVE_EnaVECapture[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pEnableCapture->pstVECapState)) - (unsigned long)pEnableCapture), (unsigned long)&spt_VE_VECAPTURESTATE);
	UADPBypassSetSPT(&spt_TVE_EnaVECapture[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_Adjust_FrameStart[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_ADJFRAMESTART));
    UADPBypassSetSPT(&spt_TVE_Adjust_FrameStart[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pAdjustFrameStart->stSrcWin)) - (unsigned long)pAdjustFrameStart), (unsigned long)&spt_VE_WINDOW_TYPE);
	UADPBypassSetSPT(&spt_TVE_Adjust_FrameStart[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_SetPowerState[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_SETPOWERSTATE));
    UADPBypassSetSPT(&spt_TVE_SetPowerState[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_ReloadSetting[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_RELOADSETTING));
    UADPBypassSetSPT(&spt_TVE_ReloadSetting[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_TVE_GetConfig[0],UADP_SPT_SELF_SIZE,0, sizeof(TVE_GETCONFIG));
    UADPBypassSetSPT(&spt_TVE_GetConfig[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pGetConfig->pMIUAddress))-(unsigned long)pGetConfig), (unsigned long)&spt_VE_Config);
    UADPBypassSetSPT(&spt_TVE_GetConfig[2],UADP_SPT_END , 0, 0);

	*pIoctl= (FUtopiaIOctl)TVENCODER_adp_Ioctl;
    return 0;
}

MS_U32 TVENCODER_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
	char buffer_arg[2048];
    switch(u32Cmd)
    {

		case E_MDRV_CMD_TVE_GetInfo :
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_TVE_GetInfo,buffer_arg,sizeof(buffer_arg));
			break;
        case E_MDRV_CMD_TVE_GetStatues:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_TVE_GetStatus,buffer_arg,sizeof(buffer_arg));
			break;
        case E_MDRV_CMD_TVE_SetDbgLevel:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetDbgLevel, spt_TVE_SetDbgLevel,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SwitchInputSRC:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SwitchInputSRC, spt_TVE_SwitchInputSRC,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetInputSRC:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetInputSRC, spt_TVE_SetInputSRC,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SwitchOutputDest:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SwitchOutputDest,spt_TVE_SwitchOutputDest,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetOutputCtrl:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SwitchOutputDest,spt_TVE_SwitchOutputDest,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetOutputVideoStd:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetOutputVideoStd,spt_TVE_SetOutputVideoStd,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetMemoryBaseAddr:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetMemoryBaseAddr,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_Init:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_Init,spt_TVE_Init,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_Exit:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_TVE_Exit,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetWSSData:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetWSSData,spt_TVE_SetWSSData,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_GetWSSData:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_TVE_GetWSSData,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetMode:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetMode,spt_TVE_SetMode,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetBlackSceen:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetBlackSceen,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_IsBlackScreenEnabled:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_TVE_IsBlackScreenEnabled,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetTtxBuffer:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetTtxBuffer,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_VE_GetTtxReadDoneStatus:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_TVE_VE_GetTtxReadDoneStatus,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_EnableCcSw:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_EnableCcSw,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetCcRange:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetCcRange,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SendCcData:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SendCcData,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_set_display_window:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_set_display_window,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetOSD:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetOSD,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_SetRGBIn:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetRGBIn,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_Get_Output_Video_Std:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_TVE_Get_Output_Video_Std,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_SetCaptureMode:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetCaptureMode,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_GetDstInfo:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_GetDstInfo,spt_TVE_GetDstInfo,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_Set_TestPattern:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_Set_TestPattern,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_DisableRegWrite:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_DisableRegWrite,spt_TVE_DisableRegWrite,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_AdjustPositionBase:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_AdjustPositionBase,spt_TVE_AdjustPositionBase,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_Set_Customer_Scaling:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_Set_Customer_Scaling,spt_TVE_Set_Customer_Scaling,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_InitVECapture:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_InitVECapture,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_EnaVECapture:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_EnaVECapture,spt_TVE_EnaVECapture,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_Adjust_FrameStart:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_Adjust_FrameStart,spt_TVE_Adjust_FrameStart,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_SetPowerState:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_SetPowerState,spt_TVE_SetPowerState,buffer_arg,sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_TVE_ReloadSetting:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_TVE_ReloadSetting,buffer_arg,sizeof(buffer_arg));
		    break;
        case E_MDRV_CMD_TVE_GetConfig:
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TVE_GetConfig, spt_TVE_GetConfig,buffer_arg,sizeof(buffer_arg));
		    break;
        default:
            break;

    }

	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif

