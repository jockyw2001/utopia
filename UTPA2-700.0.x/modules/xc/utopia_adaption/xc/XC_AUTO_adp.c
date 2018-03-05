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
/// file    XC_AUTO_adp.c
/// @brief  Main API in XC library.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_AUTO_ADP_C_
#define _XC_AUTO_ADP_C_

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
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_Auto_v2.h"
#include "apiXC_v2.h"
#include "XC_AUTO_adp.h"
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
//Function parameter
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_GEOMETRY[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_GEOMETRY_EX[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_STOPAUTOGEOMETRY[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_GAINOFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_GETHWFIXEDGAINOFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_SETVALIDDATA[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_AUTOOFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_DETECTWIDTH[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_SETCALIBRATIONMODE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTO_GETCALIBRATIONMODE[5];

//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_Auto_Signal_Info[5];
UADP_STRUCT_POINTER_TABLE spt_XC_Auto_Signal_Info_Ex[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AUTOGainOffsetSetting[5];
//UADP_STRUCT_POINTER_TABLE spt_XC_Auto_Signal_Info_Ex[5];
UADP_STRUCT_POINTER_TABLE spt_XC_Auto_CalibrationMode[5];

//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 XC_AUTO_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    pstXC_AUTO_GEOMETRY              pXC_AUTO_GEOMETRY             = (pstXC_AUTO_GEOMETRY)temp;
	pstXC_AUTO_GEOMETRY_EX           pXC_AUTO_GEOMETRY_EX          = (pstXC_AUTO_GEOMETRY_EX)temp;
	//pstXC_AUTO_STOPAUTOGEOMETRY      pXC_AUTO_STOPAUTOGEOMETRY     = (pstXC_AUTO_STOPAUTOGEOMETRY)temp;
	pstXC_AUTO_GAINOFFSET            pXC_AUTO_GAINOFFSET           = (pstXC_AUTO_GAINOFFSET)temp;
	pstXC_AUTO_GETHWFIXEDGAINOFFSET  pXC_AUTO_GETHWFIXEDGAINOFFSET = (pstXC_AUTO_GETHWFIXEDGAINOFFSET)temp;
	//pstXC_AUTO_SETVALIDDATA          pXC_AUTO_SETVALIDDATA         = (pstXC_AUTO_SETVALIDDATA)temp;
	//pstXC_AUTO_AUTOOFFSET            pXC_AUTO_AUTOOFFSET           = (pstXC_AUTO_AUTOOFFSET)temp;
	pstXC_AUTO_DETECTWIDTH           pXC_AUTO_DETECTWIDTH          = (pstXC_AUTO_DETECTWIDTH)temp;
	//pstXC_AUTO_SETCALIBRATIONMODE    pXC_AUTO_SETCALIBRATIONMODE   = (pstXC_AUTO_SETCALIBRATIONMODE)temp;
	pstXC_AUTO_GETCALIBRATIONMODE    pXC_AUTO_GETCALIBRATIONMODE   = (pstXC_AUTO_GETCALIBRATIONMODE)temp;


	//Function parameter's pointer
    //XC_Auto_Signal_Info*        pXC_Auto_Signal_Info     = (XC_Auto_Signal_Info*)temp;
    //XC_Auto_Signal_Info_Ex*     pXC_Auto_Signal_Info_Ex  = (XC_Auto_Signal_Info_Ex*)temp;
    //APIXC_AdcGainOffsetSetting* pXC_AdcGainOffsetSetting = (APIXC_AdcGainOffsetSetting*)temp;
    //XC_Auto_CalibrationMode*    pXC_Auto_CalibrationMode = (XC_Auto_CalibrationMode*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_Auto_Signal_Info[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_Auto_Signal_Info));
    UADPBypassSetSPT(&spt_XC_Auto_Signal_Info[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_Auto_Signal_Info_Ex[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_Auto_Signal_Info_Ex));
    UADPBypassSetSPT(&spt_XC_Auto_Signal_Info_Ex[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTOGainOffsetSetting[0],UADP_SPT_SELF_SIZE,0, sizeof(APIXC_AdcGainOffsetSetting));
    UADPBypassSetSPT(&spt_XC_AUTOGainOffsetSetting[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_Auto_CalibrationMode[0],UADP_SPT_SELF_SIZE,0, sizeof(XC_Auto_CalibrationMode));
    UADPBypassSetSPT(&spt_XC_Auto_CalibrationMode[1],UADP_SPT_END , 0, 0);

    //Function parameter
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_GEOMETRY));
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GEOMETRY->ActiveInfo)) - (unsigned long)pXC_AUTO_GEOMETRY), (unsigned long)spt_XC_Auto_Signal_Info);
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GEOMETRY->StandardInfo)) - (unsigned long)pXC_AUTO_GEOMETRY), (unsigned long)spt_XC_Auto_Signal_Info);
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY[3],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY_EX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_GEOMETRY_EX));
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY_EX[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GEOMETRY_EX->ActiveInfo)) - (unsigned long)pXC_AUTO_GEOMETRY_EX), (unsigned long)spt_XC_Auto_Signal_Info_Ex);
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY_EX[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GEOMETRY_EX->StandardInfo)) - (unsigned long)pXC_AUTO_GEOMETRY_EX), (unsigned long)spt_XC_Auto_Signal_Info_Ex);
    UADPBypassSetSPT(&spt_XC_AUTO_GEOMETRY_EX[3],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_STOPAUTOGEOMETRY[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_STOPAUTOGEOMETRY));
    UADPBypassSetSPT(&spt_XC_AUTO_STOPAUTOGEOMETRY[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_GAINOFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_GAINOFFSET));
    UADPBypassSetSPT(&spt_XC_AUTO_GAINOFFSET[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GAINOFFSET->pstADCSetting)) - (unsigned long)pXC_AUTO_GAINOFFSET), (unsigned long)spt_XC_AUTOGainOffsetSetting);
    UADPBypassSetSPT(&spt_XC_AUTO_GAINOFFSET[2],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_AUTO_GETHWFIXEDGAINOFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_GETHWFIXEDGAINOFFSET));
	UADPBypassSetSPT(&spt_XC_AUTO_GETHWFIXEDGAINOFFSET[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GETHWFIXEDGAINOFFSET->pstADCSetting)) - (unsigned long)pXC_AUTO_GETHWFIXEDGAINOFFSET), (unsigned long)spt_XC_AUTOGainOffsetSetting);
    UADPBypassSetSPT(&spt_XC_AUTO_GETHWFIXEDGAINOFFSET[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_SETVALIDDATA[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_SETVALIDDATA));
    UADPBypassSetSPT(&spt_XC_AUTO_SETVALIDDATA[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_AUTOOFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_AUTOOFFSET));
    UADPBypassSetSPT(&spt_XC_AUTO_AUTOOFFSET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_DETECTWIDTH[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_DETECTWIDTH));
    UADPBypassSetSPT(&spt_XC_AUTO_DETECTWIDTH[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_DETECTWIDTH->pSignalInfo)) - (unsigned long)pXC_AUTO_DETECTWIDTH), (unsigned long)spt_XC_Auto_Signal_Info_Ex);
    UADPBypassSetSPT(&spt_XC_AUTO_DETECTWIDTH[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_SETCALIBRATIONMODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_SETCALIBRATIONMODE));
    UADPBypassSetSPT(&spt_XC_AUTO_SETCALIBRATIONMODE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AUTO_GETCALIBRATIONMODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_AUTO_GETCALIBRATIONMODE));
    UADPBypassSetSPT(&spt_XC_AUTO_GETCALIBRATIONMODE[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_AUTO_GETCALIBRATIONMODE->eMode)) - (unsigned long)pXC_AUTO_GETCALIBRATIONMODE), (unsigned long)spt_XC_Auto_CalibrationMode);
    UADPBypassSetSPT(&spt_XC_AUTO_GETCALIBRATIONMODE[2],UADP_SPT_END , 0, 0);



    return 0;

}

MS_U32 XC_AUTO_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
	char buffer_arg[2];

    switch(u32Cmd)
    {

        case E_XC_AUTO_CMD_GEOMETRY :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_GEOMETRY, spt_XC_AUTO_GEOMETRY,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_AUTO_CMD_GEOMETRY_EX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_GEOMETRY_EX, spt_XC_AUTO_GEOMETRY_EX,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_AUTO_CMD_STOPAUTOGEOMETRY:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_XC_AUTO_STOPAUTOGEOMETRY,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_AUTO_CMD_GAINOFFSET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_GAINOFFSET, spt_XC_AUTO_GAINOFFSET,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_AUTO_CMD_GETHWFIXEDGAINOFFSET:
        {
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_GETHWFIXEDGAINOFFSET, spt_XC_AUTO_GETHWFIXEDGAINOFFSET,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_AUTO_CMD_SETVALIDDATA:
        {
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_SETVALIDDATA,spt_XC_AUTO_SETVALIDDATA,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_AUTO_CMD_AUTOOFFSET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_AUTOOFFSET,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_AUTO_CMD_DETECTWIDTH:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_DETECTWIDTH,spt_XC_AUTO_DETECTWIDTH,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_AUTO_CMD_SETCALIBRATIONMODE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_AUTO_SETCALIBRATIONMODE,spt_XC_AUTO_SETCALIBRATIONMODE,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_AUTO_CMD_GETCALIBRATIONMODE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_AUTO_GETCALIBRATIONMODE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_AUTO_CMD_NONE:
        case E_XC_AUTO_CMD_MAX:
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


