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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTEMP.c
/// @brief  TEMP Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvCH34.h"
#include "drvCH34_v2.h"
#include "drvCH34_private.h"
#include "drvMMIO.h"

//#include "drvCH34_Tbl.h"

// Internal Definition
#include "MsOS.h"
#include "regCHIP.h"
#include "regCH34.h"
#include "halCH34.h"
#include "utopia.h"

//#include "halCH34_Tbl.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
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


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#define CH34_TABLE_OFFSET                  4UL
#define CH34_TABLE_MAX_DEPTH               2000UL

static MS_BOOL _bInit;
void* pu32CH34Inst = NULL;

CH34_Drv_Result MDrv_Ch34_Init_U2(void)
{
    MS_VIRT virtCH34RiuBaseAddr;
    MS_U32 u32NonPMBankSize;
	// get MMIO base
	if(MDrv_MMIO_GetBASE( &virtCH34RiuBaseAddr, &u32NonPMBankSize, MS_MODULE_GE ) != TRUE)
	{
		printf("GetBase failed\n");
		return En_Drv_CH34_FAIL;
	}

    if(!_bInit)
    {
        // Driver Initialization
        HAL_CH34_Init(virtCH34RiuBaseAddr);
        _bInit = TRUE;
    }
    return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_Init(void)
{
    if (pu32CH34Inst == NULL)
    {
        if(UtopiaOpen(MODULE_CH34, &pu32CH34Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen pu32CH34Inst failed\n");
    		return En_Drv_CH34_FAIL;
        }
    }
	else
	{
        printf("CH34 has been initialized\n");
        return En_Drv_CH34_OK;
	}

	CH34_INIT_INFO CH34Args;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_INIT, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}
}

CH34_Drv_Result MDrv_Ch34_TurnOnOff_U2(MS_BOOL bOn)
{
	HAL_CH34_EnableDAC(bOn);
    return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_TurnOnOff(MS_BOOL bOn)
{
    if (pu32CH34Inst == NULL)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;

    }

	CH34_TURN_ONOFF_INFO CH34Args;
	CH34Args.bOn = bOn;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_TURNONOFF, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}
}

static void _MappingEnum(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys,
			E_HAL_CH34_CHANNEL *halchannel, E_HAL_CH34_VIDEO_SYS *halvideosys, E_HAL_CH34_AUDIO_SYS *halaudiosys)
{
	switch(channel)
	{
		case E_CH34_CH3:
			*halchannel = E_HAL_CH34_CH3;
			break;
		case E_CH34_CH4:
		default:
			*halchannel = E_HAL_CH34_CH4;
			break;
	}
	switch(videosys)
	{
		case E_CH34_VIDEO_SYS_NTSC:
			*halvideosys = E_HAL_CH34_VIDEO_SYS_NTSC;
			break;
		case E_CH34_VIDEO_SYS_PAL:
		default:
			*halvideosys = E_HAL_CH34_VIDEO_SYS_PAL;
			break;
	}
	switch(audiosys)
	{
		case E_CH34_AUDIO_SYS_MONO:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_MONO;
			break;
		case E_CH34_AUDIO_SYS_BTSC:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_BTSC;
			break;
                  case E_CH34_AUDIO_SYS_BTSC_SAP:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_BTSC_SAP;
			break;
		case E_CH34_AUDIO_SYS_A2:
		default:
			*halaudiosys = E_HAL_CH34_AUDIO_SYS_A2;
			break;
	}
}

CH34_Drv_Result MDrv_Ch34_SetMode_U2(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys)
{
	MS_U8 i = 0;
	E_HAL_CH34_CHANNEL eChannel;
	E_HAL_CH34_VIDEO_SYS eVideoSys;
	E_HAL_CH34_AUDIO_SYS eAudiosys;
	_MappingEnum(channel,videosys,audiosys,&eChannel,&eVideoSys,&eAudiosys);
	for(i = 0 ; i < E_HAL_CH34_TBL_MAX ; i++)
	{
		MS_U8* pTable = NULL;
		if(HAL_CH34_GetTable(eChannel,eVideoSys,eAudiosys,(E_HAL_CH34_TABLETYPE)i,&pTable) == FALSE)
		{
			printf("[%s] Get table fail\n",__FUNCTION__);
			return En_Drv_CH34_INVALID_PARAM;
		}
		if(NULL != pTable)
		{
	    	MDrv_Ch34_WriteCH34Table_U2(pTable);
		}
	}
	return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_SetMode(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys)
{
    if (pu32CH34Inst == NULL)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;

    }

	CH34_SET_MODE_INFO CH34Args;
	CH34Args.channel = channel;
	CH34Args.videosys = videosys;
	CH34Args.audiosys = audiosys;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_SETMODE, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}
}

CH34_Drv_Result MDrv_Ch34_WriteCH34Table_U2(MS_U8* pTable)
{
    MS_U32 u32Index; // register addr+offset
    MS_U16 u16Dummy;
    if(!_bInit)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;
    }

    u16Dummy = CH34_TABLE_MAX_DEPTH;
    do
    {
        u32Index = ((pTable[0] << 16) + (pTable[1] << 8) + pTable[2]);
		u32Index &= 0xFFFFFFUL;
        if (u32Index == 0xFFFFFFUL) // check end of table
            break;

		CH34_WriteReg(u32Index,pTable[3]);

        pTable += CH34_TABLE_OFFSET; //next row of table

    }while (--u16Dummy > 0);

    return En_Drv_CH34_OK;
}

CH34_Drv_Result MDrv_Ch34_WriteCH34Table(MS_U8* pTable)
{
    if (pu32CH34Inst == NULL)
    {
		printf("CH34 not initialize!\n");
        return En_Drv_CH34_FAIL;

    }

	CH34_WRITE_TABLE_INFO CH34Args;
	CH34Args.pTable = pTable;
	if(UtopiaIoctl(pu32CH34Inst, MAPI_CMD_CH34_WRITETABLE, (void*)&CH34Args) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain CH34 engine fail\n");
		return En_Drv_CH34_FAIL;
	}
	else
	{
		return CH34Args.bReturnValue;
	}

}

