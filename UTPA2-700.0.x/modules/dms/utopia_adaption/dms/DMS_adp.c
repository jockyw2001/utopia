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
/// file    DMS_adp.c
/// @brief  DMS adaption API
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/compat.h>

#include "apiDMS_Type.h"
#include "apiDMS_v2.h"
#include "DMS_adp.h"

#include "utopia.h"
#include "utopia_adp.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/delay.h>
#endif

UADP_STRUCT_POINTER_TABLE spt_DMS_INIT[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SETMEMTYPE[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_CREATEWIN[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SETDIGIDECINFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_CLEARDIGIDECINFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_DESTORYWIN[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SETWIN[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_GETWININFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_FLIP[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_ZORDER[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_FREEZE[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_MUTE[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_MUTECOLOR[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_GETSTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_GETCAPS[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_GETEXISTWIN[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SETOUTPUTLAYER[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SET3DMODE[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SETCAPTUREINFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_GETCAPTUREBUF[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_RELEASECAPTUREBUF[5];

UADP_STRUCT_POINTER_TABLE spt_DMS_INITDATA[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SET_MEMORY_TYPE[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_WINDOW[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_CREATE_WIN_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_MS_U32[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_DISPFRAMEFORMAT[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SETWIN_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_WINDOW_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_ZORDER_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_FREEZE_WINDOW_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_MUTE_WINDOW_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_MUTE_COLOR_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_STATUS[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_CAPABILITY[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_3D_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_SET_CAPTURE_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_DMS_CAPTURE_INFO[5];

MS_U32 DMS_adp_Init(FUtopiaIOctl *pIoctl)
{
    unsigned long temp = 0;

    PST_DMS_INIT               pDMS_INIT               = (PST_DMS_INIT)temp;
    PST_DMS_SETMEMTYPE         pDMS_SETMEMTYPE         = (PST_DMS_SETMEMTYPE)temp;
    PST_DMS_CREATEWIN          pDMS_CREATEWIN          = (PST_DMS_CREATEWIN)temp;
    PST_DMS_SETDIGIDECINFO     pDMS_SETDIGIDECINFO     = (PST_DMS_SETDIGIDECINFO)temp;
    PST_DMS_SETWIN             pDMS_SETWIN             = (PST_DMS_SETWIN)temp;
    PST_DMS_GETWININFO         pDMS_GETWININFO         = (PST_DMS_GETWININFO)temp;
    PST_DMS_FLIP               pDMS_FLIP               = (PST_DMS_FLIP)temp;
    PST_DMS_ZORDER             pDMS_ZORDER             = (PST_DMS_ZORDER)temp;
    PST_DMS_FREEZE             pDMS_FREEZE             = (PST_DMS_FREEZE)temp;
    PST_DMS_MUTE               pDMS_MUTE               = (PST_DMS_MUTE)temp;
    PST_DMS_MUTECOLOR          pDMS_MUTECOLOR          = (PST_DMS_MUTECOLOR)temp;
    PST_DMS_GETSTATUS          pDMS_GETSTATUS          = (PST_DMS_GETSTATUS)temp;
    PST_DMS_GETCAPS            pDMS_GETCAPS            = (PST_DMS_GETCAPS)temp;
    PST_DMS_GETEXISTWIN        pDMS_GETEXISTWIN        = (PST_DMS_GETEXISTWIN)temp;
    PST_DMS_SETOUTPUTLAYER     pDMS_SETOUTPUTLAYER     = (PST_DMS_SETOUTPUTLAYER)temp;
    PST_DMS_SET3DMODE          pDMS_SET3DMODE          = (PST_DMS_SET3DMODE)temp;
    PST_DMS_SETCAPTUREINFO     pDMS_SETCAPTUREINFO     = (PST_DMS_SETCAPTUREINFO)temp;
    PST_DMS_GETCAPTUREBUF      pDMS_GETCAPTUREBUF      = (PST_DMS_GETCAPTUREBUF)temp;
    PST_DMS_RELEASECAPTUREBUF  pDMS_RELEASECAPTUREBUF  = (PST_DMS_RELEASECAPTUREBUF)temp;

    // Function parameter's pointer
    UADPBypassSetSPT(&spt_DMS_INITDATA[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_INITDATA));
    UADPBypassSetSPT(&spt_DMS_INITDATA[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_SET_MEMORY_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SET_MEMORY_TYPE));
    UADPBypassSetSPT(&spt_DMS_SET_MEMORY_TYPE[1],UADP_SPT_END , 0, 0); 

    UADPBypassSetSPT(&spt_DMS_WINDOW[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_WINDOW));
    UADPBypassSetSPT(&spt_DMS_WINDOW[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_CREATE_WIN_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_CREATE_WIN_INFO));
    UADPBypassSetSPT(&spt_DMS_CREATE_WIN_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_DISPFRAMEFORMAT[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_DISPFRAMEFORMAT));
    UADPBypassSetSPT(&spt_DMS_DISPFRAMEFORMAT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_SETWIN_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SETWIN_INFO));
    UADPBypassSetSPT(&spt_DMS_SETWIN_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_WINDOW_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_WINDOW_INFO));
    UADPBypassSetSPT(&spt_DMS_WINDOW_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_ZORDER_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_ZORDER_INFO));
    UADPBypassSetSPT(&spt_DMS_ZORDER_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_FREEZE_WINDOW_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_FREEZE_WINDOW_INFO));
    UADPBypassSetSPT(&spt_DMS_FREEZE_WINDOW_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_MUTE_WINDOW_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_MUTE_WINDOW_INFO));
    UADPBypassSetSPT(&spt_DMS_MUTE_WINDOW_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_MUTE_COLOR_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_MUTE_COLOR_INFO));
    UADPBypassSetSPT(&spt_DMS_MUTE_COLOR_INFO[1],UADP_SPT_END , 0, 0);
      
    UADPBypassSetSPT(&spt_DMS_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(EN_DMS_STATUS));
    UADPBypassSetSPT(&spt_DMS_STATUS[1],UADP_SPT_END , 0, 0);
      
    UADPBypassSetSPT(&spt_DMS_CAPABILITY[0],UADP_SPT_SELF_SIZE,0, sizeof(EN_DMS_CAPABILITY));
    UADPBypassSetSPT(&spt_DMS_CAPABILITY[1],UADP_SPT_END , 0, 0);
      
    UADPBypassSetSPT(&spt_DMS_WINDOW[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_WINDOW));
    UADPBypassSetSPT(&spt_DMS_WINDOW[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_3D_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_3D_INFO));
    UADPBypassSetSPT(&spt_DMS_3D_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_SET_CAPTURE_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SET_CAPTURE_INFO));
    UADPBypassSetSPT(&spt_DMS_SET_CAPTURE_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_CAPTURE_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_CAPTURE_INFO));
    UADPBypassSetSPT(&spt_DMS_CAPTURE_INFO[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DMS_MS_U32[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U32));
    UADPBypassSetSPT(&spt_DMS_MS_U32[1],UADP_SPT_END , 0, 0);

    // Function's parameter
    // MApi_DMS_Init
    UADPBypassSetSPT(&spt_DMS_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_INIT));
    UADPBypassSetSPT(&spt_DMS_INIT[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_INIT->pstDMS_InitData))-(unsigned long)pDMS_INIT), (unsigned long)spt_DMS_INITDATA);
    UADPBypassSetSPT(&spt_DMS_INIT[2],UADP_SPT_END , 0, 0);

    // MApi_DMS_SetMemory
    UADPBypassSetSPT(&spt_DMS_SETMEMTYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SETMEMTYPE));
    UADPBypassSetSPT(&spt_DMS_SETMEMTYPE[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_SETMEMTYPE->pstDMS_SetMemType))-(unsigned long)pDMS_SETMEMTYPE), (unsigned long)spt_DMS_SET_MEMORY_TYPE);
    UADPBypassSetSPT(&spt_DMS_SETMEMTYPE[2],UADP_SPT_END , 0, 0);    

    // MApi_DMS_CreateWindow
    UADPBypassSetSPT(&spt_DMS_CREATEWIN[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_CREATEWIN));
    UADPBypassSetSPT(&spt_DMS_CREATEWIN[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_CREATEWIN->pstOutputWin))-(unsigned long)pDMS_CREATEWIN), (unsigned long)spt_DMS_WINDOW);
    UADPBypassSetSPT(&spt_DMS_CREATEWIN[2],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_CREATEWIN->pstCreateWin_Info))-(unsigned long)pDMS_CREATEWIN), (unsigned long)spt_DMS_CREATE_WIN_INFO);
    UADPBypassSetSPT(&spt_DMS_CREATEWIN[3],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_CREATEWIN->pu32WindowID))-(unsigned long)pDMS_CREATEWIN), (unsigned long)spt_DMS_MS_U32);
    UADPBypassSetSPT(&spt_DMS_CREATEWIN[4],UADP_SPT_END , 0, 0);   
    
    // MApi_DMS_SetDigitalDecodeSignalInfo
    UADPBypassSetSPT(&spt_DMS_SETDIGIDECINFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SETDIGIDECINFO));
    UADPBypassSetSPT(&spt_DMS_SETDIGIDECINFO[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_SETDIGIDECINFO->pstDispFrameFormat))-(unsigned long)pDMS_SETDIGIDECINFO), (unsigned long)spt_DMS_DISPFRAMEFORMAT);
    UADPBypassSetSPT(&spt_DMS_SETDIGIDECINFO[2],UADP_SPT_END , 0, 0); 
    
    // MApi_DMS_ClearDigitalDecodeSignalInfo
    UADPBypassSetSPT(&spt_DMS_CLEARDIGIDECINFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_CLEARDIGIDECINFO));
    UADPBypassSetSPT(&spt_DMS_CLEARDIGIDECINFO[1],UADP_SPT_END , 0, 0); 
    
    // MApi_DMS_DestroyWindow
    UADPBypassSetSPT(&spt_DMS_DESTORYWIN[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_DESTORYWIN));
    UADPBypassSetSPT(&spt_DMS_DESTORYWIN[1],UADP_SPT_END , 0, 0); 
    
    // MApi_DMS_SetWindow
    UADPBypassSetSPT(&spt_DMS_SETWIN[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SETWIN));
    UADPBypassSetSPT(&spt_DMS_SETWIN[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_SETWIN->pstDMS_SetWin_Info))-(unsigned long)pDMS_SETWIN), (unsigned long)spt_DMS_SETWIN_INFO);
    UADPBypassSetSPT(&spt_DMS_SETWIN[2],UADP_SPT_END , 0, 0);    
    
    // MApi_DMS_GetWindowInfo
    UADPBypassSetSPT(&spt_DMS_GETWININFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_GETWININFO));
    UADPBypassSetSPT(&spt_DMS_GETWININFO[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_GETWININFO->pstDMS_GetWin_Info))-(unsigned long)pDMS_GETWININFO), (unsigned long)spt_DMS_WINDOW_INFO);
    UADPBypassSetSPT(&spt_DMS_GETWININFO[2],UADP_SPT_END , 0, 0);     
    
    // MApi_DMS_Video_Flip
    UADPBypassSetSPT(&spt_DMS_FLIP[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_FLIP));
    UADPBypassSetSPT(&spt_DMS_FLIP[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_FLIP->pstDispFrameFormat))-(unsigned long)pDMS_FLIP), (unsigned long)spt_DMS_DISPFRAMEFORMAT);
    UADPBypassSetSPT(&spt_DMS_FLIP[2],UADP_SPT_END , 0, 0);    
    
    // MApi_DMS_SetZOrder
    UADPBypassSetSPT(&spt_DMS_ZORDER[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_ZORDER));
    UADPBypassSetSPT(&spt_DMS_ZORDER[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_ZORDER->pstZorderInfo))-(unsigned long)pDMS_ZORDER), (unsigned long)spt_DMS_ZORDER_INFO);
    UADPBypassSetSPT(&spt_DMS_ZORDER[2],UADP_SPT_END , 0, 0);     
    
    // MApi_DMS_Video_Freeze
    UADPBypassSetSPT(&spt_DMS_FREEZE[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_FREEZE));
    UADPBypassSetSPT(&spt_DMS_FREEZE[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_FREEZE->pstWindowFreeze))-(unsigned long)pDMS_FREEZE), (unsigned long)spt_DMS_FREEZE_WINDOW_INFO);
    UADPBypassSetSPT(&spt_DMS_FREEZE[2],UADP_SPT_END , 0, 0);      
    
    // MApi_DMS_Video_Mute
    UADPBypassSetSPT(&spt_DMS_MUTE[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_MUTE));
    UADPBypassSetSPT(&spt_DMS_MUTE[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_MUTE->pstWindowMute))-(unsigned long)pDMS_MUTE), (unsigned long)spt_DMS_MUTE_WINDOW_INFO);
    UADPBypassSetSPT(&spt_DMS_MUTE[2],UADP_SPT_END , 0, 0);    
    
    // MApi_DMS_Set_MuteColor
    UADPBypassSetSPT(&spt_DMS_MUTECOLOR[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_MUTECOLOR));
    UADPBypassSetSPT(&spt_DMS_MUTECOLOR[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_MUTECOLOR->pstMuteColor))-(unsigned long)pDMS_MUTECOLOR), (unsigned long)spt_DMS_MUTE_COLOR_INFO);
    UADPBypassSetSPT(&spt_DMS_MUTECOLOR[2],UADP_SPT_END , 0, 0);  
    
    // MApi_DMS_GetStatus
    UADPBypassSetSPT(&spt_DMS_GETSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_GETSTATUS));
    UADPBypassSetSPT(&spt_DMS_GETSTATUS[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_GETSTATUS->peStatus))-(unsigned long)pDMS_GETSTATUS), (unsigned long)spt_DMS_STATUS);
    UADPBypassSetSPT(&spt_DMS_GETSTATUS[2],UADP_SPT_END , 0, 0);      
    
    // MApi_DMS_GetCapability
    UADPBypassSetSPT(&spt_DMS_GETCAPS[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_GETCAPS));
    UADPBypassSetSPT(&spt_DMS_GETCAPS[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_GETCAPS->peCapability))-(unsigned long)pDMS_GETCAPS), (unsigned long)spt_DMS_CAPABILITY);
    UADPBypassSetSPT(&spt_DMS_GETCAPS[2],UADP_SPT_END , 0, 0);      
    
    // MApi_DMS_GetExistedWindows
    UADPBypassSetSPT(&spt_DMS_GETEXISTWIN[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_GETEXISTWIN));
    UADPBypassSetSPT(&spt_DMS_GETEXISTWIN[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_GETEXISTWIN->pu32WindowNums))-(unsigned long)pDMS_GETEXISTWIN), (unsigned long)spt_DMS_MS_U32);
    UADPBypassSetSPT(&spt_DMS_GETEXISTWIN[2],UADP_SPT_END , 0, 0);        
    
    // MApi_DMS_SetOutputLayer
    UADPBypassSetSPT(&spt_DMS_SETOUTPUTLAYER[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SETOUTPUTLAYER));
    UADPBypassSetSPT(&spt_DMS_SETOUTPUTLAYER[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_SETOUTPUTLAYER->pstLayer))-(unsigned long)pDMS_SETOUTPUTLAYER), (unsigned long)spt_DMS_WINDOW);
    UADPBypassSetSPT(&spt_DMS_SETOUTPUTLAYER[2],UADP_SPT_END , 0, 0);   
    
    // MApi_DMS_Set_3D_Mode
    UADPBypassSetSPT(&spt_DMS_SET3DMODE[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SET3DMODE));
    UADPBypassSetSPT(&spt_DMS_SET3DMODE[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_SET3DMODE->pst3DInfo))-(unsigned long)pDMS_SET3DMODE), (unsigned long)spt_DMS_3D_INFO);
    UADPBypassSetSPT(&spt_DMS_SET3DMODE[2],UADP_SPT_END , 0, 0);       
    
    // MApi_DMS_Set_CaptureInfo
    UADPBypassSetSPT(&spt_DMS_SET_CAPTURE_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_SETCAPTUREINFO));
    UADPBypassSetSPT(&spt_DMS_SET_CAPTURE_INFO[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_SETCAPTUREINFO->pstSetCaptureInfo))-(unsigned long)pDMS_SETCAPTUREINFO), (unsigned long)spt_DMS_SET_CAPTURE_INFO);
    UADPBypassSetSPT(&spt_DMS_SET_CAPTURE_INFO[2],UADP_SPT_END , 0, 0);      
    
    // MApi_DMS_Get_CaptureBuffer
    UADPBypassSetSPT(&spt_DMS_GETCAPTUREBUF[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_GETCAPTUREBUF));
    UADPBypassSetSPT(&spt_DMS_GETCAPTUREBUF[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_GETCAPTUREBUF->pstCaptureInfo))-(unsigned long)pDMS_GETCAPTUREBUF), (unsigned long)spt_DMS_CAPTURE_INFO);
    UADPBypassSetSPT(&spt_DMS_GETCAPTUREBUF[2],UADP_SPT_END , 0, 0);     
    
    
    
    // MApi_DMS_Release_CaptureBuffer
    UADPBypassSetSPT(&spt_DMS_RELEASECAPTUREBUF[0],UADP_SPT_SELF_SIZE,0, sizeof(ST_DMS_RELEASECAPTUREBUF));
    UADPBypassSetSPT(&spt_DMS_RELEASECAPTUREBUF[1],UADP_SPT_POINTER_TO_NEXT,
                     ((unsigned long)(&(pDMS_RELEASECAPTUREBUF->pstCaptureInfo))-(unsigned long)pDMS_RELEASECAPTUREBUF), (unsigned long)spt_DMS_CAPTURE_INFO);
    UADPBypassSetSPT(&spt_DMS_RELEASECAPTUREBUF[2],UADP_SPT_END , 0, 0); 
    *pIoctl= (FUtopiaIOctl)DMS_adp_Ioctl;
    return 0;
}

MS_U32 DMS_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    switch(u32Cmd)
    {
        case E_DMS_CMD_INIT:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_INIT, spt_DMS_INIT, NULL,0);
            break;
        }
        case E_DMS_CMD_SETMEMORY:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_SETMEMTYPE, spt_DMS_SETMEMTYPE, NULL,0);
            break;
        }
        case E_DMS_CMD_CREATEWINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_CREATEWIN, spt_DMS_CREATEWIN, NULL,0);
            break;
        }
        case E_DMS_CMD_SETDIGITALDECODESIGNALINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_SETDIGIDECINFO, spt_DMS_SETDIGIDECINFO, NULL,0);
            break;
        }
        case E_DMS_CMD_CLEARDIGITALDECODESIGNALINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_CLEARDIGIDECINFO, spt_DMS_CLEARDIGIDECINFO, NULL,0);
            break;
        }
        case E_DMS_CMD_DESTORYWINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_DESTORYWIN, spt_DMS_DESTORYWIN, NULL,0);
            break;
        }
        case E_DMS_CMD_SETWINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_SETWIN, spt_DMS_SETWIN, NULL,0);
            break;
        }
        case E_DMS_CMD_GETWINDOWINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_GETWININFO, spt_DMS_GETWININFO, NULL,0);
            break;
        }
        case E_DMS_CMD_VIDEOFLIP:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_FLIP, spt_DMS_FLIP, NULL,0);
            break;
        }
        case E_DMS_CMD_SETZORDER:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_ZORDER, spt_DMS_ZORDER, NULL,0);
            break;
        }
        case E_DMS_CMD_VIDEOMUTE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_FREEZE, spt_DMS_FREEZE, NULL,0);
            break;
        }
        case E_DMS_CMD_SETMUTECOLOR:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_MUTE, spt_DMS_MUTE, NULL,0);
            break;
        }
        case E_DMS_CMD_VIDEOFREEZE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_MUTECOLOR, spt_DMS_MUTECOLOR, NULL,0);
            break;
        }
        case E_DMS_CMD_GETSTATUS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_GETSTATUS, spt_DMS_GETSTATUS, NULL,0);
            break;
        }
        case E_DMS_CMD_GETCAPS:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_GETCAPS, spt_DMS_GETCAPS, NULL,0);
            break;
        }
        case E_DMS_CMD_GETEXISTEDWINDOW:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_GETEXISTWIN, spt_DMS_GETEXISTWIN, NULL,0);
            break;
        }
        case E_DMS_CMD_SETOUTPUTLAYER:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_SETOUTPUTLAYER, spt_DMS_SETOUTPUTLAYER, NULL,0);
            break;
        }
        case E_DMS_CMD_SET3DMODE:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_SET3DMODE, spt_DMS_SET3DMODE, NULL,0);
            break;
        }
        case E_DMS_CMD_SETCAPTUREINFO:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_SETCAPTUREINFO, spt_DMS_SETCAPTUREINFO, NULL,0);
            break;
        }
        case E_DMS_CMD_GETCAPTUREBUFFER:
        {
             u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_GETCAPTUREBUF, spt_DMS_GETCAPTUREBUF, NULL,0);
            break;
        }
        case E_DMS_CMD_RELEASECAPTUREBUFFER:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DMS_RELEASECAPTUREBUF, spt_DMS_RELEASECAPTUREBUF, NULL,0);
            break;
        }
        default:
        {
            break;
        }  
    }
     return u32Ret;
}

