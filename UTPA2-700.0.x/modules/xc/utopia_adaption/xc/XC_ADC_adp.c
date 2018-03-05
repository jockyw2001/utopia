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
/// file    XC_adp.c
/// @brief  Main API in XC library.
/// @author MStar Semiconductor Inc.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_ADP_C_
#define _XC_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Adc_v2.h"
#include "apiXC_v2.h"
#include "XC_ADC_adp.h"
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
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETCVBSOUT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_ISCVBSOUTENABLED[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETPCCLOCK[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETPHASE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETPHASEEX[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETPHASERANGE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETPHASE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETPHASEEX[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_ISSCARTRGB[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETPCCLOCK[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETSOGLEVELRANGE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETSOGLEVEL[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETDEFAULTGAINOFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETMAXIMALOFFSETVALUE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETMAXIMALGAINVALUE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETCENTERGAIN[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_GETCENTEROFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETGAIN[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETOFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_ADJUSTGAINOFFSET[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SOURCE_CALIBRATE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETRGB_PIPE_DELAY[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SCARTRGB_SOG_CLAMPDELAY[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SET_YPBPRLOOSELPF[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SET_SOGBW[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_SETCLAMPDURATION[5];
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_ENABLEHWCALIBRATION[5];


//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_ADC_TYPE_U32[5];
UADP_STRUCT_POINTER_TABLE spt_XC_AdcGainOffsetSetting[5];

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
MS_U32 XC_ADC_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    //pstXC_ADC_SETCVBSOUT          pXC_ADC_SETCVBSOUT       = (pstXC_ADC_SETCVBSOUT)temp;
	//pstXC_ADC_ISCVBSOUTENABLED    pXC_ADC_ISCVBSOUTENABLED = (pstXC_ADC_ISCVBSOUTENABLED)temp;
	//pstXC_ADC_SETPCCLOCK          pXC_ADC_SETPCCLOCK       = (pstXC_ADC_SETPCCLOCK)temp;
	//pstXC_ADC_SETPHASE            pXC_ADC_SETPHASE         = (pstXC_ADC_SETPHASE)temp;
	//pstXC_ADC_SETPHASEEX          pXC_ADC_SETPHASEEX       = (pstXC_ADC_SETPHASEEX)temp;
	//pstXC_ADC_GETPHASERANGE       pXC_ADC_GETPHASERANGE    = (pstXC_ADC_GETPHASERANGE)temp;
	//pstXC_ADC_GETPHASE            pXC_ADC_GETPHASE         = (pstXC_ADC_GETPHASE)temp;
	//pstXC_ADC_GETPHASEEX          pXC_ADC_GETPHASEEX       = (pstXC_ADC_GETPHASEEX)temp;
	//pstXC_ADC_ISSCARTRGB          pXC_ADC_ISSCARTRGB       = (pstXC_ADC_ISSCARTRGB)temp;
	//pstXC_ADC_GETPCCLOCK          pXC_ADC_GETPCCLOCK       = (pstXC_ADC_GETPCCLOCK)temp;
	pstXC_ADC_GETSOGLEVELRANGE    pXC_ADC_GETSOGLEVELRANGE = (pstXC_ADC_GETSOGLEVELRANGE)temp;
	//pstXC_ADC_SETSOGLEVEL         pXC_ADC_SETSOGLEVEL      = (pstXC_ADC_SETSOGLEVEL)temp;
	pstXC_ADC_GETDEFAULTGAINOFFSET  pXC_ADC_GETDEFAULTGAINOFFSET  = (pstXC_ADC_GETDEFAULTGAINOFFSET)temp;
	//pstXC_ADC_GETMAXIMALOFFSETVALUE pXC_ADC_GETMAXIMALOFFSETVALUE = (pstXC_ADC_GETMAXIMALOFFSETVALUE)temp;
	//pstXC_ADC_GETMAXIMALGAINVALUE   pXC_ADC_GETMAXIMALGAINVALUE   = (pstXC_ADC_GETMAXIMALGAINVALUE)temp;
	//pstXC_ADC_GETCENTERGAIN         pXC_ADC_GETCENTERGAIN         = (pstXC_ADC_GETCENTERGAIN)temp;
	//pstXC_ADC_GETCENTEROFFSET       pXC_ADC_GETCENTEROFFSET       = (pstXC_ADC_GETCENTEROFFSET)temp;
	//pstXC_ADC_SETGAIN               pXC_ADC_SETGAIN               = (pstXC_ADC_SETGAIN)temp;
	//pstXC_ADC_SETOFFSET             pXC_ADC_SETOFFSET             = (pstXC_ADC_SETOFFSET)temp;
	pstXC_ADC_ADJUSTGAINOFFSET      pXC_ADC_ADJUSTGAINOFFSET      = (pstXC_ADC_ADJUSTGAINOFFSET)temp;
	//pstXC_ADC_SOURCE_CALIBRATE      pXC_ADC_SOURCE_CALIBRATE      = (pstXC_ADC_SOURCE_CALIBRATE)temp;
	//pstXC_ADC_SETRGB_PIPE_DELAY     pXC_ADC_SETRGB_PIPE_DELAY     = (pstXC_ADC_SETRGB_PIPE_DELAY)temp;
	//pstXC_ADC_SCARTRGB_SOG_CLAMPDELAY pXC_ADC_SCARTRGB_SOG_CLAMPDELAY = (pstXC_ADC_SCARTRGB_SOG_CLAMPDELAY)temp;
	//pstXC_ADC_SET_YPBPRLOOSELPF       pXC_ADC_SET_YPBPRLOOSELPF       = (pstXC_ADC_SET_YPBPRLOOSELPF)temp;
	//pstXC_ADC_SET_SOGBW               pXC_ADC_SET_SOGBW               = (pstXC_ADC_SET_SOGBW)temp;
	//pstXC_ADC_SETCLAMPDURATION        pXC_ADC_SETCLAMPDURATION        = (pstXC_ADC_SETCLAMPDURATION)temp;
	//pstXC_ADC_ENABLEHWCALIBRATION     pXC_ADC_ENABLEHWCALIBRATION     = (pstXC_ADC_ENABLEHWCALIBRATION)temp;

	//Function parameter's pointer
	//MS_U32* pstXC_ADC_TYPE_U32 = (MS_U32*)temp;
    //APIXC_AdcGainOffsetSetting* pstADCSetting = (APIXC_AdcGainOffsetSetting*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_ADC_TYPE_U32[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U32));
    UADPBypassSetSPT(&spt_XC_ADC_TYPE_U32[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_AdcGainOffsetSetting[0],UADP_SPT_SELF_SIZE,0, sizeof(APIXC_AdcGainOffsetSetting));
    UADPBypassSetSPT(&spt_XC_AdcGainOffsetSetting[1],UADP_SPT_END , 0, 0);

    //Function parameter
    UADPBypassSetSPT(&spt_XC_ADC_SETCVBSOUT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETCVBSOUT));
    UADPBypassSetSPT(&spt_XC_ADC_SETCVBSOUT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_ISCVBSOUTENABLED[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_ISCVBSOUTENABLED));
    UADPBypassSetSPT(&spt_XC_ADC_ISCVBSOUTENABLED[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETPCCLOCK[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETPCCLOCK));
    UADPBypassSetSPT(&spt_XC_ADC_SETPCCLOCK[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_ADC_SETPHASE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETPHASE));
    UADPBypassSetSPT(&spt_XC_ADC_SETPHASE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETPHASEEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETPHASEEX));
    UADPBypassSetSPT(&spt_XC_ADC_SETPHASEEX[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETPHASERANGE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETPHASERANGE));
    UADPBypassSetSPT(&spt_XC_ADC_GETPHASERANGE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_ADC_GETPHASE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETPHASE));
    UADPBypassSetSPT(&spt_XC_ADC_GETPHASE[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_ADC_GETPHASEEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETPHASEEX));
	UADPBypassSetSPT(&spt_XC_ADC_GETPHASEEX[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_ADC_ISSCARTRGB[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_ISSCARTRGB));
    UADPBypassSetSPT(&spt_XC_ADC_ISSCARTRGB[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETPCCLOCK[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETPCCLOCK));
	UADPBypassSetSPT(&spt_XC_ADC_GETPCCLOCK[1],UADP_SPT_END , 0, 0);


    UADPBypassSetSPT(&spt_XC_ADC_GETSOGLEVELRANGE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETSOGLEVELRANGE));
    UADPBypassSetSPT(&spt_XC_ADC_GETSOGLEVELRANGE[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_ADC_GETSOGLEVELRANGE->u32min)) - (unsigned long)pXC_ADC_GETSOGLEVELRANGE), (unsigned long)spt_XC_ADC_TYPE_U32);
    UADPBypassSetSPT(&spt_XC_ADC_GETSOGLEVELRANGE[2],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_ADC_GETSOGLEVELRANGE->u32max)) - (unsigned long)pXC_ADC_GETSOGLEVELRANGE), (unsigned long)spt_XC_ADC_TYPE_U32);
    UADPBypassSetSPT(&spt_XC_ADC_GETSOGLEVELRANGE[3],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_ADC_GETSOGLEVELRANGE->u32Recommend_value)) - (unsigned long)pXC_ADC_GETSOGLEVELRANGE), (unsigned long)spt_XC_ADC_TYPE_U32);
	UADPBypassSetSPT(&spt_XC_ADC_GETSOGLEVELRANGE[4],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETSOGLEVEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETSOGLEVEL));
	UADPBypassSetSPT(&spt_XC_ADC_SETSOGLEVEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETDEFAULTGAINOFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETDEFAULTGAINOFFSET));
    UADPBypassSetSPT(&spt_XC_ADC_GETDEFAULTGAINOFFSET[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_ADC_GETDEFAULTGAINOFFSET->pstADCSetting)) - (unsigned long)pXC_ADC_GETDEFAULTGAINOFFSET), (unsigned long)spt_XC_AdcGainOffsetSetting);
	UADPBypassSetSPT(&spt_XC_ADC_GETDEFAULTGAINOFFSET[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETMAXIMALOFFSETVALUE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETMAXIMALOFFSETVALUE));
	UADPBypassSetSPT(&spt_XC_ADC_GETMAXIMALOFFSETVALUE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETMAXIMALGAINVALUE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETMAXIMALGAINVALUE));
	UADPBypassSetSPT(&spt_XC_ADC_GETMAXIMALGAINVALUE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETCENTERGAIN[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETCENTERGAIN));
	UADPBypassSetSPT(&spt_XC_ADC_GETCENTERGAIN[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_GETCENTEROFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_GETCENTEROFFSET));
	UADPBypassSetSPT(&spt_XC_ADC_GETCENTEROFFSET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETGAIN[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETGAIN));
	UADPBypassSetSPT(&spt_XC_ADC_SETGAIN[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETOFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETOFFSET));
	UADPBypassSetSPT(&spt_XC_ADC_SETOFFSET[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_ADJUSTGAINOFFSET[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_ADJUSTGAINOFFSET));
    UADPBypassSetSPT(&spt_XC_ADC_ADJUSTGAINOFFSET[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_ADC_ADJUSTGAINOFFSET->pstADCSetting)) - (unsigned long)pXC_ADC_ADJUSTGAINOFFSET), (unsigned long)spt_XC_AdcGainOffsetSetting);
	UADPBypassSetSPT(&spt_XC_ADC_ADJUSTGAINOFFSET[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SOURCE_CALIBRATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SOURCE_CALIBRATE));
	UADPBypassSetSPT(&spt_XC_ADC_SOURCE_CALIBRATE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETRGB_PIPE_DELAY[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETRGB_PIPE_DELAY));
	UADPBypassSetSPT(&spt_XC_ADC_SETRGB_PIPE_DELAY[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SCARTRGB_SOG_CLAMPDELAY[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SCARTRGB_SOG_CLAMPDELAY));
	UADPBypassSetSPT(&spt_XC_ADC_SCARTRGB_SOG_CLAMPDELAY[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SET_YPBPRLOOSELPF[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SET_YPBPRLOOSELPF));
	UADPBypassSetSPT(&spt_XC_ADC_SET_YPBPRLOOSELPF[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SET_SOGBW[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SET_SOGBW));
	UADPBypassSetSPT(&spt_XC_ADC_SET_SOGBW[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_SETCLAMPDURATION[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_SETCLAMPDURATION));
    UADPBypassSetSPT(&spt_XC_ADC_SETCLAMPDURATION[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_ADC_ENABLEHWCALIBRATION[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_ADC_ENABLEHWCALIBRATION));
	UADPBypassSetSPT(&spt_XC_ADC_ENABLEHWCALIBRATION[1],UADP_SPT_END , 0, 0);



    return 0;

}

MS_U32 XC_ADC_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2];

    switch(u32Cmd)
    {

        case E_XC_ADC_CMD_SETCVBSOUT :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETCVBSOUT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_ISCVBSOUTENABLED:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_ISCVBSOUTENABLED, spt_XC_ADC_ISCVBSOUTENABLED,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETPCCLOCK:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETPCCLOCK, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETPHASE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETPHASE, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETPHASEEX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETPHASEEX, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETPHASERANGE:
        {
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETPHASERANGE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETPHASE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETPHASE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETPHASEEX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETPHASEEX,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_ISSCARTRGB:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_ISSCARTRGB,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETPCCLOCK:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETPCCLOCK,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETSOGLEVELRANGE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETSOGLEVELRANGE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETSOGLEVEL:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETSOGLEVEL,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_POWEROFF:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }

        case E_XC_ADC_CMD_GETDEFAULTGAINOFFSET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_GETDEFAULTGAINOFFSET,spt_XC_ADC_GETDEFAULTGAINOFFSET,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETMAXIMALOFFSETVALUE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETMAXIMALOFFSETVALUE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETMAXIMALGAINVALUE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETMAXIMALGAINVALUE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETCENTERGAIN:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETCENTERGAIN,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_GETCENTEROFFSET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,spt_XC_ADC_GETCENTEROFFSET,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETGAIN:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETGAIN,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETOFFSET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETOFFSET,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_ADJUSTGAINOFFSET:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_ADJUSTGAINOFFSET,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SOURCE_CALIBRATE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SOURCE_CALIBRATE,NULL,buffer_arg,sizeof(buffer_arg));
		    break;
        }

        case E_XC_ADC_CMD_SETSOGCAL:
        {
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        }

        case E_XC_ADC_CMD_SETRGB_PIPE_DELAY:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETRGB_PIPE_DELAY,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SCARTRGB_SOG_CLAMPDELAY:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SCARTRGB_SOG_CLAMPDELAY,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SET_YPBPRLOOSELPF:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SET_YPBPRLOOSELPF,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SET_SOGBW:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SET_SOGBW,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_SETCLAMPDURATION:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_SETCLAMPDURATION,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_ENABLEHWCALIBRATION:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_ADC_ENABLEHWCALIBRATION,spt_XC_ADC_ENABLEHWCALIBRATION,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_ADC_CMD_NONE:
        case E_XC_ADC_CMD_MAX:
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


