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
/// file    XC_PCMONITOR_adp.c
/// @brief  Scaler API layer Interface.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_PCMONITOR_ADP_C_
#define _XC_PCMONITOR_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_PCMonitor_v2.h"
#include "apiXC_v2.h"
#include "XC_PCMonitor_adp.h"
#include "XC_adp.h"

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

//Function parameter
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_INIT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_RESTART[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_SETTIMINGCOUNT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GETCURRENTSTATE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GETSYNCSTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_HFREQX10[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_HFREQX1K[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_VFREQX10[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_VFREQX1K[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_VTOTAL[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_SYNCLOSS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_INVALIDTIMINGDETECT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_SETTIMINGCOUNTEX[5];

//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_MS_WINDOW_TYPE[5];

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 XC_PCMONITOR_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    //pstXC_PCMONITOR_INIT                  pXC_PCMONITOR_INIT                 = (pstXC_PCMONITOR_INIT)temp;
    //pstXC_PCMONITOR_RESTART               pXC_PCMONITOR_RESTART              = (pstXC_PCMONITOR_RESTART)temp;
    //pstXC_PCMONITOR_SETTIMINGCOUNT        pXC_PCMONITOR_SETTIMINGCOUNT       = (pstXC_PCMONITOR_SETTIMINGCOUNT)temp;
    //pstXC_PCMONITOR                       pXC_PCMONITOR                      = (pstXC_PCMONITOR)temp;
    //pstXC_PCMONITOR_GETCURRENTSTATE       pXC_PCMONITOR_GETCURRENTSTATE      = (pstXC_PCMONITOR_GETCURRENTSTATE)temp;
    //pstXC_PCMONITOR_GETSYNCSTATUS         pXC_PCMONITOR_GETSYNCSTATUS        = (pstXC_PCMONITOR_GETSYNCSTATUS)temp;
    //pstXC_PCMONITOR_GET_HFREQX10          pXC_PCMONITOR_GET_HFREQX10         = (pstXC_PCMONITOR_GET_HFREQX10)temp;
    //pstXC_PCMONITOR_GET_HFREQX1K          pXC_PCMONITOR_GET_HFREQX1K         = (pstXC_PCMONITOR_GET_HFREQX1K)temp;
    //pstXC_PCMONITOR_GET_VFREQX10          pXC_PCMONITOR_GET_VFREQX10         = (pstXC_PCMONITOR_GET_VFREQX10)temp;
    //pstXC_PCMONITOR_GET_VFREQX1K          pXC_PCMONITOR_GET_VFREQX1K         = (pstXC_PCMONITOR_GET_VFREQX1K)temp;
    //pstXC_PCMONITOR_GET_VTOTAL            pXC_PCMONITOR_GET_VTOTAL           = (pstXC_PCMONITOR_GET_VTOTAL)temp;
    pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO  pXC_PCMONITOR_GET_DVI_HDMI_DE_INFO = (pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO)temp;
    //pstXC_PCMONITOR_SYNCLOSS              pXC_PCMONITOR_SYNCLOSS             = (pstXC_PCMONITOR_SYNCLOSS)temp;
    //pstXC_PCMONITOR_INVALIDTIMINGDETECT   pXC_PCMONITOR_INVALIDTIMINGDETECT  = (pstXC_PCMONITOR_INVALIDTIMINGDETECT)temp;
    //pstXC_PCMONITOR_SETTIMINGCOUNTEX      pXC_PCMONITOR_SETTIMINGCOUNTEX     = (pstXC_PCMONITOR_SETTIMINGCOUNTEX)temp;


	//Function parameter's pointer
    //MS_WINDOW_TYPE*        pXC_PCMOTITOR_msWin     = (MS_WINDOW_TYPE*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_PCMONITOR_MS_WINDOW_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_WINDOW_TYPE));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_MS_WINDOW_TYPE[1],UADP_SPT_END , 0, 0);

    //Function parameter
    UADPBypassSetSPT(&spt_XC_PCMONITOR_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_INIT));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_RESTART[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_RESTART));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_RESTART[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_SETTIMINGCOUNT));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR));
    UADPBypassSetSPT(&spt_XC_PCMONITOR[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_PCMONITOR_GETCURRENTSTATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GETCURRENTSTATE));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GETCURRENTSTATE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GETSYNCSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GETSYNCSTATUS));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GETSYNCSTATUS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX10[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_HFREQX10));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX10[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX1K[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_HFREQX1K));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX1K[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX10[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_VFREQX10));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX10[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX1K[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_VFREQX1K));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX1K[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VTOTAL[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_VTOTAL));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VTOTAL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_DVI_HDMI_DE_INFO));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_PCMONITOR_GET_DVI_HDMI_DE_INFO->msWin)) - (unsigned long)pXC_PCMONITOR_GET_DVI_HDMI_DE_INFO), (unsigned long)spt_XC_PCMONITOR_MS_WINDOW_TYPE);
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_SYNCLOSS[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_SYNCLOSS));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_SYNCLOSS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_INVALIDTIMINGDETECT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_INVALIDTIMINGDETECT));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_INVALIDTIMINGDETECT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNTEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_SETTIMINGCOUNTEX));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNTEX[1],UADP_SPT_END , 0, 0);


    return 0;

}

MS_U32 XC_PCMONITOR_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2];

    switch(u32Cmd)
    {

        case E_XC_PCMONITOR_CMD_INIT :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_INIT, spt_XC_PCMONITOR_INIT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_RESTART:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_RESTART, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_SETTIMINGCOUNT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_SETTIMINGCOUNT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR, spt_XC_PCMONITOR,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GETCURRENTSTATE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GETCURRENTSTATE, spt_XC_PCMONITOR_GETCURRENTSTATE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GETSYNCSTATUS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GETSYNCSTATUS,spt_XC_PCMONITOR_GETSYNCSTATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_HFREQX10:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_HFREQX10,spt_XC_PCMONITOR_GET_HFREQX10,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_HFREQX1K:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_HFREQX1K,spt_XC_PCMONITOR_GET_HFREQX1K,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_VFREQX10:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_VFREQX10,spt_XC_PCMONITOR_GET_VFREQX10,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_VFREQX1K:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_VFREQX1K,spt_XC_PCMONITOR_GET_VFREQX1K,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_VTOTAL:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_VTOTAL,spt_XC_PCMONITOR_GET_VTOTAL,buffer_arg,sizeof(buffer_arg));
            break;
        }

		case E_XC_PCMONITOR_CMD_GET_DVI_HDMI_DE_INFO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO,spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_SYNCLOSS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_SYNCLOSS,spt_XC_PCMONITOR_SYNCLOSS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_INVALIDTIMINGDETECT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_INVALIDTIMINGDETECT,spt_XC_PCMONITOR_INVALIDTIMINGDETECT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_SETTIMINGCOUNTEX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_SETTIMINGCOUNTEX,spt_XC_PCMONITOR_SETTIMINGCOUNTEX,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_NONE:
        case E_XC_PCMONITOR_CMD_MAX:
        default:
        {
            printf("Command %ld is not existed\n",u32Cmd);
            u32Ret = UTOPIA_STATUS_ERR_INVALID_HANDLE;
            break;
        }

    }

	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif


