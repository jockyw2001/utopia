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
#include <linux/slab.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"

// Internal Definition
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "PNL_adp.h"
#include "drvPNL.h"

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
UADP_STRUCT_POINTER_TABLE spt_stPNL_GetLibVer[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_GetInfo[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_GetStatus[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_GetStatusEx[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetDbgLevel[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_IOMapBaseInit[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_PreInit[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Init[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Init_Ex[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_GetConfig[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetOutput[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_ChangePanelType[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_TCONMAP_DumpTable[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_TCONMAP_Power_Sequence[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_TCON_Count_Reset[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_TCON_Init[10];
//UADP_STRUCT_POINTER_TABLE spt_stPNL_GetDstInfo[10];
UADP_SPT_NAMEnNXT_DEF(MS_PNL_DST_DispInfo,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetDstInfo,1);
UADP_STRUCT_POINTER_TABLE spt_stPNL_Control_Out_Swing[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetOutputPattern[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Mod_Calibration_Setting[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Mod_Do_Calibration[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_BD_LVDS_Output_Type[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetLPLLTypeExt[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Init_MISC[10];
UADP_SPT_NAMEnNXT_DEF(stPNL_Get_MISC_Status,0);
UADP_STRUCT_POINTER_TABLE spt_stPNL_MOD_OutputConfig_User[10];
UADP_SPT_NAMEnNXT_DEF(stPNL_MOD_OutputChannelOrder,0);
UADP_STRUCT_POINTER_TABLE spt_stPNL_HWLVDSReservedtoLRFlag[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_MOD_PVDD_Power_Setting[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetSSC_En[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetSSC_Fmodulation[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetSSC_Rdeviation[10];
UADP_SPT_NAMEnNXT_DEF(stPNL_SetOSDSSC_En,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_SetOSDSSC_Fmodulation,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_SetOSDSSC_Rdeviation,0);
UADP_STRUCT_POINTER_TABLE spt_stPNL_SkipTimingChange[10];
UADP_SPT_NAMEnNXT_DEF(stPNL_PreSetModeOn,0);
UADP_STRUCT_POINTER_TABLE spt_stPNL_OverDriver_Init[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_OverDriver_Enable[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Get_LPLL_Type[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_Get_TCON_Capability[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetPairSwap[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetExt_LPLL_Type[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_CalExtLPLLSETbyDClk[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetDiffSwingLevel[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetPowerState[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_ForceSetPanelDCLK[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_EnableInternalTermination[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_OutputDeviceHandshake[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_OutputDeviceOCHandshake[10];
UADP_STRUCT_POINTER_TABLE spt_stPNL_SetOutputInterlaceTiming[10];

UADP_SPT_NAMEnNXT_DEF(stPNL_GetOutputInterlaceTiming,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetPanelData,1);
UADP_SPT_NAMEnNXT_DEF(stPNL_SetSSC,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetPanelOnTiming,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetPanelOffTiming,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetPanelDimCtrl,0);

//gamma
UADP_SPT_NAMEnNXT_DEF(GammaTab,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_SetGammaTbl,5);
//
UADP_SPT_NAMEnNXT_DEF(stPNL_EnablePanel,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetLPLLMode,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetDefVFreq,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_ForceSetPanelHStart,0);
UADP_SPT_NAMEnNXT_DEF(PNLName,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetName,1);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetHstart,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetVstart,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetWidth,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetHeight,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetHtotal,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetVtotal,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetHsyncWidth,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetHsyncBackPorch,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetVsyncBackPorch,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetLPLLType,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetARC,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetMinSET,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetMaxSET,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetOutTimingMode,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_CheckVBY1HandshakeStatus,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_MOD_Video_HWTrainingMode,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_MOD_OSD_HWTrainingMode,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetVideoHWTraining_status,0);
UADP_SPT_NAMEnNXT_DEF(stPNL_GetOSDHWTraining_status,0);

//Bottom
UADP_STRUCT_POINTER_TABLE spt_MSIF_Version[10];
UADP_STRUCT_POINTER_TABLE spt_PNL_ApiInfo[10];
UADP_STRUCT_POINTER_TABLE spt_PNL_ApiStatus[10];
UADP_STRUCT_POINTER_TABLE spt_PNL_ApiExtStatus[10];
UADP_STRUCT_POINTER_TABLE spt_PanelType[10];
UADP_STRUCT_POINTER_TABLE spt_TCONTable[10];
//UADP_STRUCT_POINTER_TABLE spt_MS_PNL_DST_DispInfo[10];
UADP_STRUCT_POINTER_TABLE spt_MS_PNL_ModCaliInfo[10];
UADP_STRUCT_POINTER_TABLE spt_MS_PNL_OD_INITDATA[10];
UADP_STRUCT_POINTER_TABLE spt_PanelChar[10];

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 PNL_adp_Init(FUtopiaIOctl *pIoctl)
{

//set table
    UADP_SPT_BGN(&spt_stPNL_GetLibVer[0], sizeof(stPNL_GetLibVer));
    UADP_SPT_NXT(&spt_stPNL_GetLibVer[1], stPNL_GetLibVer, ppVersion, spt_MSIF_Version);
    UADP_SPT_FIN(&spt_stPNL_GetLibVer[2]);

    UADP_SPT_BGN(&spt_stPNL_GetInfo[0], sizeof(stPNL_GetInfo));
    UADP_SPT_NXT(&spt_stPNL_GetInfo[1], stPNL_GetInfo, pApiInfo, spt_PNL_ApiInfo);
    UADP_SPT_FIN(&spt_stPNL_GetInfo[2]);

    UADP_SPT_BGN(&spt_stPNL_GetStatus[0], sizeof(stPNL_GetStatus));
    UADP_SPT_NXT(&spt_stPNL_GetStatus[1], stPNL_GetStatus, pPnlStatus, spt_PNL_ApiStatus);
    UADP_SPT_FIN(&spt_stPNL_GetStatus[2]);

    UADP_SPT_BGN(&spt_stPNL_GetStatusEx[0], sizeof(stPNL_GetStatusEx));
    UADP_SPT_NXT(&spt_stPNL_GetStatusEx[1], stPNL_GetStatusEx, pPnlExtStatus, spt_PNL_ApiExtStatus);
    UADP_SPT_FIN(&spt_stPNL_GetStatusEx[2]);

    UADP_SPT_BGN(&spt_stPNL_SetDbgLevel[0], sizeof(stPNL_SetDbgLevel));
    UADP_SPT_FIN(&spt_stPNL_SetDbgLevel[1]);

    UADP_SPT_BGN(&spt_stPNL_IOMapBaseInit[0], sizeof(stPNL_IOMapBaseInit));
    UADP_SPT_FIN(&spt_stPNL_IOMapBaseInit[1]);

    UADP_SPT_BGN(&spt_stPNL_PreInit[0], sizeof(stPNL_PreInit));
    UADP_SPT_FIN(&spt_stPNL_PreInit[1]);

    UADP_SPT_BGN(&spt_stPNL_Init[0], sizeof(stPNL_Init));
    UADP_SPT_NXT(&spt_stPNL_Init[1], stPNL_Init, pSelPanelType, spt_PanelType);
    UADP_SPT_FIN(&spt_stPNL_Init[2]);

    UADP_SPT_BGN(&spt_stPNL_Init_Ex[0], sizeof(stPNL_Init_Ex));
    UADP_SPT_NXT(&spt_stPNL_Init_Ex[1], stPNL_Init_Ex, pSelPanelType, spt_PanelType);
    UADP_SPT_FIN(&spt_stPNL_Init_Ex[2]);

    UADP_SPT_BGN(&spt_stPNL_GetConfig[0], sizeof(stPNL_GetConfig));
    UADP_SPT_NXT(&spt_stPNL_GetConfig[1], stPNL_GetConfig, pSelPanelType, spt_PanelType);
    UADP_SPT_FIN(&spt_stPNL_GetConfig[2]);

    UADP_SPT_BGN(&spt_stPNL_SetOutput[0], sizeof(stPNL_SetOutput));
    UADP_SPT_FIN(&spt_stPNL_SetOutput[1]);

    UADP_SPT_BGN(&spt_stPNL_ChangePanelType[0], sizeof(stPNL_ChangePanelType));
    UADP_SPT_NXT(&spt_stPNL_ChangePanelType[1], stPNL_ChangePanelType, pSelPanelType, spt_PanelType);
    UADP_SPT_FIN(&spt_stPNL_ChangePanelType[2]);

    UADP_SPT_BGN(&spt_stPNL_TCONMAP_DumpTable[0], sizeof(stPNL_TCONMAP_DumpTable));
    UADP_SPT_NXT(&spt_stPNL_TCONMAP_DumpTable[1], stPNL_TCONMAP_DumpTable, pTCONTable, spt_TCONTable);
    UADP_SPT_FIN(&spt_stPNL_TCONMAP_DumpTable[2]);

    UADP_SPT_BGN(&spt_stPNL_TCONMAP_Power_Sequence[0], sizeof(stPNL_TCONMAP_Power_Sequence));
    UADP_SPT_NXT(&spt_stPNL_TCONMAP_Power_Sequence[1], stPNL_TCONMAP_Power_Sequence, pTCONTable, spt_TCONTable);
    UADP_SPT_FIN(&spt_stPNL_TCONMAP_Power_Sequence[2]);

    UADP_SPT_BGN(&spt_stPNL_TCON_Count_Reset[0], sizeof(stPNL_TCON_Count_Reset));
    UADP_SPT_FIN(&spt_stPNL_TCON_Count_Reset[1]);

    UADP_SPT_NAME0NXT(MS_PNL_DST_DispInfo,MS_PNL_DST_DispInfo);
    UADP_SPT_NAME1NXT(stPNL_GetDstInfo,stPNL_GetDstInfo,pDstInfo,MS_PNL_DST_DispInfo);
    //UADP_SPT_BGN(&spt_stPNL_GetDstInfo[0], sizeof(stPNL_GetDstInfo));
    //UADP_SPT_NXT(&spt_stPNL_GetDstInfo[1], stPNL_GetDstInfo, pDstInfo, spt_MS_PNL_DST_DispInfo);
    //UADP_SPT_FIN(&spt_stPNL_GetDstInfo[2]);

    UADP_SPT_BGN(&spt_stPNL_Control_Out_Swing[0], sizeof(stPNL_Control_Out_Swing));
    UADP_SPT_FIN(&spt_stPNL_Control_Out_Swing[1]);

    UADP_SPT_BGN(&spt_stPNL_SetOutputPattern[0], sizeof(stPNL_SetOutputPattern));
    UADP_SPT_FIN(&spt_stPNL_SetOutputPattern[1]);

    UADP_SPT_BGN(&spt_stPNL_Mod_Calibration_Setting[0], sizeof(stPNL_Mod_Calibration_Setting));
    UADP_SPT_NXT(&spt_stPNL_Mod_Calibration_Setting[1], stPNL_Mod_Calibration_Setting, pstModCaliInfo, spt_MS_PNL_ModCaliInfo);
    UADP_SPT_FIN(&spt_stPNL_Mod_Calibration_Setting[2]);

    UADP_SPT_BGN(&spt_stPNL_Mod_Do_Calibration[0], sizeof(stPNL_Mod_Do_Calibration));
    UADP_SPT_FIN(&spt_stPNL_Mod_Do_Calibration[1]);

    UADP_SPT_BGN(&spt_stPNL_BD_LVDS_Output_Type[0], sizeof(stPNL_BD_LVDS_Output_Type));
    UADP_SPT_FIN(&spt_stPNL_BD_LVDS_Output_Type[1]);

    UADP_SPT_BGN(&spt_stPNL_SetLPLLTypeExt[0], sizeof(stPNL_SetLPLLTypeExt));
    UADP_SPT_FIN(&spt_stPNL_SetLPLLTypeExt[1]);

    UADP_SPT_BGN(&spt_stPNL_Init_MISC[0], sizeof(stPNL_Init_MISC));
    UADP_SPT_FIN(&spt_stPNL_Init_MISC[1]);

    UADP_SPT_NAME0NXT(stPNL_Get_MISC_Status,stPNL_Get_MISC_Status);

    UADP_SPT_BGN(&spt_stPNL_Control_Out_Swing[0], sizeof(stPNL_Control_Out_Swing));
    UADP_SPT_FIN(&spt_stPNL_Control_Out_Swing[1]);

    UADP_SPT_BGN(&spt_stPNL_MOD_OutputConfig_User[0], sizeof(stPNL_MOD_OutputConfig_User));
    UADP_SPT_FIN(&spt_stPNL_MOD_OutputConfig_User[1]);

    UADP_SPT_NAME0NXT(stPNL_MOD_OutputChannelOrder,stPNL_MOD_OutputChannelOrder);

    UADP_SPT_BGN(&spt_stPNL_HWLVDSReservedtoLRFlag[0], sizeof(stPNL_HWLVDSReservedtoLRFlag));
    UADP_SPT_FIN(&spt_stPNL_HWLVDSReservedtoLRFlag[1]);

    UADP_SPT_BGN(&spt_stPNL_MOD_PVDD_Power_Setting[0], sizeof(stPNL_MOD_PVDD_Power_Setting));
    UADP_SPT_FIN(&spt_stPNL_MOD_PVDD_Power_Setting[1]);

    UADP_SPT_BGN(&spt_stPNL_SetSSC_En[0], sizeof(stPNL_SetSSC_En));
    UADP_SPT_FIN(&spt_stPNL_SetSSC_En[1]);

    UADP_SPT_BGN(&spt_stPNL_Control_Out_Swing[0], sizeof(stPNL_Control_Out_Swing));
    UADP_SPT_FIN(&spt_stPNL_Control_Out_Swing[1]);

    UADP_SPT_BGN(&spt_stPNL_SetSSC_Fmodulation[0], sizeof(stPNL_SetSSC_Fmodulation));
    UADP_SPT_FIN(&spt_stPNL_SetSSC_Fmodulation[1]);

    UADP_SPT_BGN(&spt_stPNL_SetSSC_Rdeviation[0], sizeof(stPNL_SetSSC_Rdeviation));
    UADP_SPT_FIN(&spt_stPNL_SetSSC_Rdeviation[1]);

    UADP_SPT_NAME0NXT(stPNL_SetOSDSSC_En,stPNL_SetOSDSSC_En);

    UADP_SPT_NAME0NXT(stPNL_SetOSDSSC_Fmodulation,stPNL_SetOSDSSC_Fmodulation);
    
    UADP_SPT_NAME0NXT(stPNL_SetOSDSSC_Rdeviation,stPNL_SetOSDSSC_Rdeviation);

    UADP_SPT_BGN(&spt_stPNL_Control_Out_Swing[0], sizeof(stPNL_Control_Out_Swing));
    UADP_SPT_FIN(&spt_stPNL_Control_Out_Swing[1]);

    UADP_SPT_BGN(&spt_stPNL_SkipTimingChange[0], sizeof(stPNL_SkipTimingChange));
    UADP_SPT_FIN(&spt_stPNL_SkipTimingChange[1]);

    UADP_SPT_NAME0NXT(stPNL_PreSetModeOn,stPNL_PreSetModeOn);

    UADP_SPT_BGN(&spt_stPNL_OverDriver_Init[0], sizeof(stPNL_OverDriver_Init));
    UADP_SPT_NXT(&spt_stPNL_OverDriver_Init[1], stPNL_OverDriver_Init, pPNL_ODInitData, spt_MS_PNL_OD_INITDATA);
    UADP_SPT_FIN(&spt_stPNL_OverDriver_Init[2]);

    UADP_SPT_BGN(&spt_stPNL_OverDriver_Enable[0], sizeof(stPNL_OverDriver_Enable));
    UADP_SPT_FIN(&spt_stPNL_OverDriver_Enable[1]);

    UADP_SPT_BGN(&spt_stPNL_Get_LPLL_Type[0], sizeof(stPNL_Get_LPLL_Type));
    UADP_SPT_FIN(&spt_stPNL_Get_LPLL_Type[1]);

    UADP_SPT_BGN(&spt_stPNL_Get_TCON_Capability[0], sizeof(stPNL_Get_TCON_Capability));
    UADP_SPT_FIN(&spt_stPNL_Get_TCON_Capability[1]);

    UADP_SPT_BGN(&spt_stPNL_SetPairSwap[0], sizeof(stPNL_SetPairSwap));
    UADP_SPT_FIN(&spt_stPNL_SetPairSwap[1]);

    UADP_SPT_BGN(&spt_stPNL_SetExt_LPLL_Type[0], sizeof(stPNL_SetExt_LPLL_Type));
    UADP_SPT_FIN(&spt_stPNL_SetExt_LPLL_Type[1]);

    UADP_SPT_BGN(&spt_stPNL_CalExtLPLLSETbyDClk[0], sizeof(stPNL_CalExtLPLLSETbyDClk));
    UADP_SPT_FIN(&spt_stPNL_CalExtLPLLSETbyDClk[1]);

    UADP_SPT_BGN(&spt_stPNL_SetDiffSwingLevel[0], sizeof(stPNL_SetDiffSwingLevel));
    UADP_SPT_FIN(&spt_stPNL_SetDiffSwingLevel[1]);

    UADP_SPT_BGN(&spt_stPNL_SetPowerState[0], sizeof(stPNL_SetPowerState));
    UADP_SPT_FIN(&spt_stPNL_SetPowerState[1]);

    UADP_SPT_BGN(&spt_stPNL_ForceSetPanelDCLK[0], sizeof(stPNL_ForceSetPanelDCLK));
    UADP_SPT_FIN(&spt_stPNL_ForceSetPanelDCLK[1]);

    UADP_SPT_BGN(&spt_stPNL_EnableInternalTermination[0], sizeof(stPNL_EnableInternalTermination));
    UADP_SPT_FIN(&spt_stPNL_EnableInternalTermination[1]);

    UADP_SPT_BGN(&spt_stPNL_OutputDeviceHandshake[0], sizeof(stPNL_OutputDeviceHandshake));
    UADP_SPT_FIN(&spt_stPNL_OutputDeviceHandshake[1]);

    UADP_SPT_BGN(&spt_stPNL_OutputDeviceOCHandshake[0], sizeof(stPNL_OutputDeviceOCHandshake));
    UADP_SPT_FIN(&spt_stPNL_OutputDeviceOCHandshake[1]);

    UADP_SPT_BGN(&spt_stPNL_SetOutputInterlaceTiming[0], sizeof(stPNL_SetOutputInterlaceTiming));
    UADP_SPT_FIN(&spt_stPNL_SetOutputInterlaceTiming[1]);

    UADP_SPT_NAME0NXT(stPNL_GetOutputInterlaceTiming,stPNL_GetOutputInterlaceTiming);

    UADP_SPT_NAME1NXT(stPNL_GetPanelData,stPNL_GetPanelData,pstPNLData,PanelType);
    UADP_SPT_NAME0NXT(stPNL_SetSSC,stPNL_SetSSC);
    UADP_SPT_NAME0NXT(stPNL_GetPanelOnTiming,stPNL_GetPanelOnTiming);
    UADP_SPT_NAME0NXT(stPNL_GetPanelOffTiming,stPNL_GetPanelOffTiming);
    UADP_SPT_NAME0NXT(stPNL_GetPanelDimCtrl,stPNL_GetPanelDimCtrl);

    //gamma
    UADP_SPT_BGN(&spt_GammaTab[0], sizeof(MS_U8)*386);
    UADP_SPT_FIN(&spt_GammaTab[1]);
    UADP_SPT_NAME3NXT(stPNL_SetGammaTbl,stPNL_SetGammaTbl,pu8GammaTab[0],GammaTab,pu8GammaTab[1],GammaTab,pu8GammaTab[2],GammaTab);
    //
    UADP_SPT_NAME0NXT(stPNL_EnablePanel,stPNL_EnablePanel);
    UADP_SPT_NAME0NXT(stPNL_GetLPLLMode,stPNL_GetLPLLMode);
    UADP_SPT_NAME0NXT(stPNL_GetDefVFreq,stPNL_GetDefVFreq);
    UADP_SPT_NAME0NXT(stPNL_ForceSetPanelHStart,stPNL_ForceSetPanelHStart);
    UADP_SPT_BGN(&spt_PNLName[0], sizeof(char)*20);
    UADP_SPT_FIN(&spt_PNLName[1]);
    UADP_SPT_NAME1NXT(stPNL_GetName,stPNL_GetName,pPNLName,PNLName);
    UADP_SPT_NAME0NXT(stPNL_GetHstart,stPNL_GetHstart);
    UADP_SPT_NAME0NXT(stPNL_GetVstart,stPNL_GetVstart);
    UADP_SPT_NAME0NXT(stPNL_GetWidth,stPNL_GetWidth);
    UADP_SPT_NAME0NXT(stPNL_GetHeight,stPNL_GetHeight);
    UADP_SPT_NAME0NXT(stPNL_GetHtotal,stPNL_GetHtotal);
    UADP_SPT_NAME0NXT(stPNL_GetVtotal,stPNL_GetVtotal);
    UADP_SPT_NAME0NXT(stPNL_GetHsyncWidth,stPNL_GetHsyncWidth);
    UADP_SPT_NAME0NXT(stPNL_GetHsyncBackPorch,stPNL_GetHsyncBackPorch);
    UADP_SPT_NAME0NXT(stPNL_GetVsyncBackPorch,stPNL_GetVsyncBackPorch);
    UADP_SPT_NAME0NXT(stPNL_GetLPLLType,stPNL_GetLPLLType);
    UADP_SPT_NAME0NXT(stPNL_GetARC,stPNL_GetARC);
    UADP_SPT_NAME0NXT(stPNL_GetMinSET,stPNL_GetMinSET);
    UADP_SPT_NAME0NXT(stPNL_GetMaxSET,stPNL_GetMaxSET);
    UADP_SPT_NAME0NXT(stPNL_GetOutTimingMode,stPNL_GetOutTimingMode);
    UADP_SPT_NAME0NXT(stPNL_CheckVBY1HandshakeStatus,stPNL_CheckVBY1HandshakeStatus);
    UADP_SPT_NAME0NXT(stPNL_MOD_Video_HWTrainingMode,stPNL_MOD_Video_HWTrainingMode);
    UADP_SPT_NAME0NXT(stPNL_MOD_OSD_HWTrainingMode,stPNL_MOD_OSD_HWTrainingMode);
    UADP_SPT_NAME0NXT(stPNL_GetVideoHWTraining_status,stPNL_GetVideoHWTraining_status);
    UADP_SPT_NAME0NXT(stPNL_GetOSDHWTraining_status,stPNL_GetOSDHWTraining_status);
    //bottom
    UADP_SPT_BGN(&spt_MSIF_Version[0], sizeof(MSIF_Version));
    UADP_SPT_FIN(&spt_MSIF_Version[1]);

    UADP_SPT_BGN(&spt_PNL_ApiInfo[0], sizeof(PNL_ApiInfo));
    UADP_SPT_FIN(&spt_PNL_ApiInfo[1]);

    UADP_SPT_BGN(&spt_PNL_ApiStatus[0], sizeof(PNL_ApiStatus));
    UADP_SPT_FIN(&spt_PNL_ApiStatus[1]);

    UADP_SPT_BGN(&spt_PNL_ApiExtStatus[0], sizeof(PNL_ApiExtStatus));
    UADP_SPT_FIN(&spt_PNL_ApiExtStatus[1]);

    UADP_SPT_BGN(&spt_PanelChar[0], sizeof(char));
    UADP_SPT_FIN(&spt_PanelChar[1]);

    UADP_SPT_BGN(&spt_PanelType[0], sizeof(PanelType));
    UADP_SPT_FIN(&spt_PanelType[1]);

    //UADP_SPT_BGN(&spt_MS_PNL_DST_DispInfo[0], sizeof(MS_PNL_DST_DispInfo));
    //UADP_SPT_FIN(&spt_MS_PNL_DST_DispInfo[1]);

    UADP_SPT_BGN(&spt_MS_PNL_ModCaliInfo[0], sizeof(MS_PNL_ModCaliInfo));
    UADP_SPT_FIN(&spt_MS_PNL_ModCaliInfo[1]);

    UADP_SDT_NAME2(MS_PNL_OD_INITDATA,MS_PNL_OD_INITDATA,UADP_SDT_AT,u32OD_MSB_Addr,MS_NULL,UADP_SDT_AT,u32OD_LSB_Size,MS_NULL)

    *pIoctl= (FUtopiaIOctl)PNL_adp_Ioctl;
    return 0;

}

MS_U32 PNL_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];
    pu32PNLInst_private = pInstanceTmp;
    //printk("\033[1;33m[%s:%d]CMD=%ld\033[m\n",__FUNCTION__,__LINE__,u32Cmd);
    switch(u32Cmd)
    {
        case E_PNL_CMD_GETLIBVER:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_GetLibVer, spt_stPNL_GetLibVer, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_GETINFO:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_GetInfo, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_GETSTATUS:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_GetStatus, spt_stPNL_GetStatus, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_GETSTATUSEX:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_GetStatusEx, spt_stPNL_GetStatusEx, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETDBGLEVEL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetDbgLevel, spt_stPNL_SetDbgLevel, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_IOMAPBASEINIT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_IOMapBaseInit, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_PREINIT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_PreInit, spt_stPNL_PreInit, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_INIT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_Init, spt_stPNL_Init, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_INIT_EX:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_Init_Ex, spt_stPNL_Init_Ex, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_GET_CONFIG:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_GetConfig, spt_stPNL_GetConfig, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETOUTPUT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetOutput, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_CHANGEPANELTYPE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_ChangePanelType, spt_stPNL_ChangePanelType, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_TCONMAP_DUMPTABLE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_TCONMAP_DumpTable, spt_stPNL_TCONMAP_DumpTable, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_TCONMAP_POWER_SEQUENCE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_TCONMAP_Power_Sequence, spt_stPNL_TCONMAP_Power_Sequence, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_TCON_COUNT_RESET:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_TCON_Count_Reset, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_TCON_INIT:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;

        case E_PNL_CMD_GETDSTINFO:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_GetDstInfo, spt_stPNL_GetDstInfo, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_CONTROL_OUT_SWING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_Control_Out_Swing, spt_stPNL_Control_Out_Swing, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SET_OUTPUT_PATTERN:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetOutputPattern, spt_stPNL_SetOutputPattern, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_MOD_CALIBRATION_SETTING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_Mod_Calibration_Setting, spt_stPNL_Mod_Calibration_Setting, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_MOD_DO_CALIBRATION:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_Mod_Do_Calibration, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_BD_LVDS_OUTPUT_TYPE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_BD_LVDS_Output_Type, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETLPLLTYPEEXT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetLPLLTypeExt, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_INIT_MISC:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_Init_MISC, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GET_MISC_STATUS:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_Get_MISC_Status, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_MOD_OUTPUTCONFIG_USER:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_MOD_OutputConfig_User, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_MOD_OUTPUT_CHANNEL_ORDER:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_MOD_OutputChannelOrder, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_HWLVDSRESERVEDTOLRFLAG:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_HWLVDSReservedtoLRFlag, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_MOD_PVDD_POWER_SETTING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_MOD_PVDD_Power_Setting, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETSSC_EN:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetSSC_En, spt_stPNL_SetSSC_En, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETSSC_FMODULATION:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetSSC_Fmodulation, spt_stPNL_SetSSC_Fmodulation, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETSSC_RDEVIATION:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetSSC_Rdeviation, spt_stPNL_SetSSC_Rdeviation, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETOSDSSC_EN:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetOSDSSC_En, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_SETOSDSSC_FMODULATION:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetOSDSSC_Fmodulation, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_SETOSDSSC_RDEVIATION:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetOSDSSC_Rdeviation, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_SKIPTIMINGCHANGE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SkipTimingChange, spt_stPNL_SkipTimingChange, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_PRE_SET_MODE_ON:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_PreSetModeOn, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_OVERDRIVER_INIT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_OverDriver_Init, spt_stPNL_OverDriver_Init, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_OVERDRIVER_ENABLE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_OverDriver_Enable, spt_stPNL_OverDriver_Enable, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_GET_LPLL_TYPE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_Get_LPLL_Type, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_GET_TCON_CAPABILITY:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_Get_TCON_Capability, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETPAIRSWAP:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetPairSwap, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SET_EXT_LPLL_TYPE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetExt_LPLL_Type, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_CALEXTLPLLSETBYDCLK:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_CalExtLPLLSETbyDClk, NULL, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETDIFFSWINGLEVEL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetDiffSwingLevel, spt_stPNL_SetDiffSwingLevel, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETPOWERSTATE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_SetPowerState, spt_stPNL_SetPowerState, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_FORCESETPANELDCLK:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_ForceSetPanelDCLK, spt_stPNL_ForceSetPanelDCLK, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_ENABLEINTERNALTERMINATION:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_EnableInternalTermination, spt_stPNL_EnableInternalTermination, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_OUTPUTDEVICEHANDSHAKE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_OutputDeviceHandshake, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_OUTPUTDEVICEOCHANDSHAKE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_OutputDeviceOCHandshake, buffer_arg, sizeof(buffer_arg));
            break;

        case E_PNL_CMD_SETOUTPUTINTERLACETIMING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_stPNL_SetOutputInterlaceTiming, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETOUTPUTINTERLACETIMING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_stPNL_GetOutputInterlaceTiming, spt_stPNL_SetOutputInterlaceTiming, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETPANELDATA:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_GetPanelData, spt_stPNL_GetPanelData, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_DUMPPANELDATA:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case E_PNL_CMD_SETSSC:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_SetSSC, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETPANELONTIMING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_GetPanelOnTiming, spt_stPNL_GetPanelOnTiming, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETPANELOFFTIMING:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_GetPanelOffTiming, spt_stPNL_GetPanelOffTiming, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETPANELDIMCTRL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_GetPanelDimCtrl, spt_stPNL_GetPanelDimCtrl, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETALLGAMMATBL:
            break;
        case E_PNL_CMD_ENABLEPANEL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_EnablePanel, spt_stPNL_EnablePanel, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_SETGAMMATBL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_SetGammaTbl, spt_stPNL_SetGammaTbl, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETGAMMATBL:
            break;
        case E_PNL_CMD_SETGAMMAVALUE:
            break;
        case E_PNL_CMD_GETLPLLMODE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetLPLLMode, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETDEFVFREQ:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetDefVFreq, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_FORCESETPANELHSTART:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_ForceSetPanelHStart, spt_stPNL_ForceSetPanelHStart, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETPANELNAME:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetName, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETHSTART:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetHstart, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETVSTART:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetVstart, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETWIDTH:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetWidth, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETHEIGHT:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetHeight, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETHTOTAL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetHtotal, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETVTOTAL:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetVtotal, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETHSYNCWIDTH:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetHsyncWidth, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETHSYNCBACKPORCH:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetHsyncBackPorch, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETVSYNCBACKPORCH:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetVsyncBackPorch, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETLPLLTYPE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetLPLLType, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETARC:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetARC, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETMINSET:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetMinSET, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETMAXSET:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetMaxSET, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GETOUTTIMINGMODE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetOutTimingMode, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_CHECKVBY1HANDSHAKESTATUS:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_CheckVBY1HandshakeStatus, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_SET_VIDEOHWTRAINING_MODE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_MOD_Video_HWTrainingMode, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_SET_OSDHWTRAINING_MODE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,spt_stPNL_MOD_OSD_HWTrainingMode, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GET_VIDEOHWTRAINING_MODE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetVideoHWTraining_status, buffer_arg, sizeof(buffer_arg));
            break;
        case E_PNL_CMD_GET_OSDHWTRAINING_MODE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs,NULL, spt_stPNL_GetOSDHWTraining_status, buffer_arg, sizeof(buffer_arg));
            break;

        default:
            break;

    }

    return u32Ret;

}

#endif
