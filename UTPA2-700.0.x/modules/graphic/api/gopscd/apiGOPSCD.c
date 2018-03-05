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
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  [Doxygen]
/// file apiGOP_SC.c
/// This file includes MStar application interface for Graphic output path
/// @brief API for Graphics output path (GOP)
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"

#include "MsTypes.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "apiGOPSC_Ex.h"
#include "apiGOPSCD_v2.h"
#include "apiGOPSCD_private.h"
#include "drvGOPSCD.h"
#include "drvMMIO.h"

//GOPSC_Info gGOPSCInfoData =
//{EN_Cubic,EN_Source_GOP,0x40,FALSE,FALSE,FALSE,TRUE};

#define HW_LIMIT_WIDTH 720

#define _GOPSCD_ENTRY_CHECKER(ARGS)															  \
		ARGS.bReturnValue = En_GOPSC_NOT_INITIALIZE; \
		if (pu32GOPSCDInst == NULL) 							  \
		{																						  \
			printf("[%s,%5d] No instance existed, please get an instance by calling MApi_GOPSC_Init() first\n",__FUNCTION__,__LINE__); 				  \
			return ARGS.bReturnValue; \
		}																						   \

#define _GOPSCD_SET_INSTPRI_ID_FROM_INFO(pINSTPRI, pDEV)                                    \
		if(UtopiaInstanceGetPrivate(pu32GOPSCDInst, (void*)&pINSTPRI)!=UTOPIA_STATUS_SUCCESS) \
		{ \
			printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__); \
			return En_GOPSC_FAIL;                                                  \
		} \
		pINSTPRI->u32DeviceID = _GetDeviceIdFromInfo(pDEV);


static MS_U32 _GetDeviceIdFromInfo(GOPSC_DeviceInfo* pDevInfo)
{
    if(NULL == pDevInfo)
    {
        return 0;
    }
    else
    {
        return pDevInfo->u32Id;
    }
}

static MS_U32 _GetDeviceIDFromInstance(void* pInstance)
{
	if(NULL == pInstance)
	{
		printf("[%s] NULL instance\n",__FUNCTION__);
		return 0;
	}
	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	if(UtopiaInstanceGetPrivate(pInstance, (void*)&psGOPSCDInstPri)!=UTOPIA_STATUS_SUCCESS)
	{
		printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
		return 0;
	}
	return psGOPSCDInstPri->u32DeviceID;
}

GOPSC_Result MApi_GOPSCD_Init_U2(void* pInstance)
{
    MS_VIRT virtGOPSCRiuBaseAddr;
    MS_U32 u32NonPMBankSize;

	GOPSCD_RESOURCE_PRIVATE* psGOPSCDResPri = NULL;
	if(UtopiaResourceGetPrivate(g_pGOPSCDResource, (void*)&psGOPSCDResPri)!=UTOPIA_STATUS_SUCCESS)
	{
		printf("[%s,%5d] Resource Get Private failed\n",__FUNCTION__,__LINE__);
		return En_GOPSC_FAIL;
	}

	//Initialize globle
	psGOPSCDResPri->GOPSCDInfoData.en_Source = EN_Source_GOP;
	psGOPSCDResPri->GOPSCDInfoData.en_VScalingMode = EN_Cubic;
	psGOPSCDResPri->GOPSCDInfoData.dlyLineNumber = 0x40;
	psGOPSCDResPri->GOPSCDInfoData.P2IEnable = FALSE;
	psGOPSCDResPri->GOPSCDInfoData.HalfModeEnable = FALSE;
	psGOPSCDResPri->GOPSCDInfoData.OutputRateControlEnable = FALSE;
	psGOPSCDResPri->GOPSCDInfoData.UseP2IFormula = TRUE;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &virtGOPSCRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_GE ) != TRUE)
    {
        printf("GOP_SC GetBase failed\n");
        return En_GOPSC_FAIL;
    }
    MDrv_GOPSC_Init_riu_base(_GetDeviceIDFromInstance(pInstance),virtGOPSCRiuBaseAddr);
    MDrv_GOPSC_Init(_GetDeviceIDFromInstance(pInstance));
    MDrv_GOPSC_SetSkipPixel(_GetDeviceIDFromInstance(pInstance),TRUE,FALSE,0,HW_LIMIT_WIDTH,0,0); //for HW limit, XSkip must 720.
    MApi_GOPSC_SetGOPSCInfo_U2(pInstance,NULL);

    return En_GOPSC_OK;
}

GOPSC_Result MApi_GOPSC_Init(GOPSC_DeviceInfo* DevInfo)
{
    if (pu32GOPSCDInst == NULL)
    {
        if(UtopiaOpen(MODULE_GOPSCD, &pu32GOPSCDInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen pu32GOPSCDInst failed\n");
    		return En_GOPSC_FAIL;
        }
    }

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	if(UtopiaInstanceGetPrivate(pu32GOPSCDInst, (void*)&psGOPSCDInstPri)!=UTOPIA_STATUS_SUCCESS)
	{
		printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
	}
	psGOPSCDInstPri->u32DeviceID = _GetDeviceIdFromInfo(DevInfo);

	GOPSCD_INIT_INFO GOPSCDArgs;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_INIT, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL;
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}
}

GOPSC_Result MApi_GOPSCD_Scaling_U2(void* pInstance, E_GOPSCD_SCALING_CMD cmd, MS_U8 FrameNum)
{
	if(E_GOPSCD_CMD_ENABLE == cmd)
	{
		MDrv_GOPSC_Enable(_GetDeviceIDFromInstance(pInstance),TRUE);
	}
	else if(E_GOPSCD_CMD_DISABLE == cmd)
	{
		MDrv_GOPSC_Enable(_GetDeviceIDFromInstance(pInstance),FALSE);
	}
	else if(E_GOPSCD_CMD_SCALING_ONCE == cmd)
	{
	    if(0 == FrameNum)
	    {
	        printf("[%s] FrameNum is 0.\n",__FUNCTION__);
	        return En_GOPSC_INVALID_PARAM;
	    }
	    MDrv_GOPSC_Enable(_GetDeviceIDFromInstance(pInstance),FALSE);
	    MDrv_GOPSC_TriggerOnce(_GetDeviceIDFromInstance(pInstance),FrameNum);
	}
	else
	{
		printf("[%s,%5d] Unkown command\n",__FUNCTION__,__LINE__);
		return En_GOPSC_FAIL;
	}
    return En_GOPSC_OK;
}

GOPSC_Result MApi_GOPSC_Enable(GOPSC_DeviceInfo* DevInfo, MS_BOOL enable)
{
	GOPSCD_SCALING GOPSCDArgs;
	_GOPSCD_ENTRY_CHECKER(GOPSCDArgs);

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	_GOPSCD_SET_INSTPRI_ID_FROM_INFO(psGOPSCDInstPri,DevInfo);

	GOPSCDArgs.gopscCmd = E_GOPSCD_CMD_ENABLE;
	GOPSCDArgs.u8FrameCount = 0;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_SCALING, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL; // return meaningless value because get instance failed.
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}
}

GOPSC_Result MApi_GOPSC_GetGOPSCInfo_U2(void* pInstance, GOPSC_Info* gopscInfo)
{
    if(gopscInfo == NULL)
    {
        printf("[%s] gopscInfo is NULL\n",__FUNCTION__);
        return En_GOPSC_INVALID_PARAM;
    }

	GOPSCD_RESOURCE_PRIVATE* psGOPSCDResPri = NULL;
	if(UtopiaResourceGetPrivate(g_pGOPSCDResource, (void*)&psGOPSCDResPri)!=UTOPIA_STATUS_SUCCESS)
	{
		printf("[%s,%5d] Resource Get Private failed\n",__FUNCTION__,__LINE__);
		return En_GOPSC_FAIL;
	}

    gopscInfo->en_Source = psGOPSCDResPri->GOPSCDInfoData.en_Source;
    gopscInfo->en_VScalingMode = psGOPSCDResPri->GOPSCDInfoData.en_VScalingMode;
    gopscInfo->dlyLineNumber = psGOPSCDResPri->GOPSCDInfoData.dlyLineNumber;
    gopscInfo->P2IEnable = psGOPSCDResPri->GOPSCDInfoData.P2IEnable;
    gopscInfo->HalfModeEnable = psGOPSCDResPri->GOPSCDInfoData.HalfModeEnable;
    gopscInfo->OutputRateControlEnable = psGOPSCDResPri->GOPSCDInfoData.OutputRateControlEnable;
	gopscInfo->UseP2IFormula = psGOPSCDResPri->GOPSCDInfoData.UseP2IFormula;

    return En_GOPSC_OK;
}

GOPSC_Result MApi_GOPSC_GetGOPSCInfo(GOPSC_DeviceInfo* DevInfo, GOPSC_Info* gopscInfo)
{
	GOPSCD_GET_INFO GOPSCDArgs;
	_GOPSCD_ENTRY_CHECKER(GOPSCDArgs);
	//If gopscInfo == NULL, it will set value for globle variable directly

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	_GOPSCD_SET_INSTPRI_ID_FROM_INFO(psGOPSCDInstPri,DevInfo);

    GOPSCDArgs.gopscInfo = gopscInfo;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_GET_INFO, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL; // return meaningless value because get instance failed.
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}
}

GOPSC_Result MApi_GOPSC_SetGOPSCInfo_U2(void* pInstance, GOPSC_Info* gopscInfo)
{
	GOPSCD_RESOURCE_PRIVATE* psGOPSCDResPri = NULL;
	if(UtopiaResourceGetPrivate(g_pGOPSCDResource, (void*)&psGOPSCDResPri)!=UTOPIA_STATUS_SUCCESS)
	{
		printf("[%s,%5d] Resource Get Private failed\n",__FUNCTION__,__LINE__);
		return En_GOPSC_FAIL;
	}

    if(gopscInfo != NULL)
    {
		psGOPSCDResPri->GOPSCDInfoData.en_Source = gopscInfo->en_Source;
		psGOPSCDResPri->GOPSCDInfoData.en_VScalingMode = gopscInfo->en_VScalingMode;
		psGOPSCDResPri->GOPSCDInfoData.dlyLineNumber = gopscInfo->dlyLineNumber;
		psGOPSCDResPri->GOPSCDInfoData.P2IEnable = gopscInfo->P2IEnable;
		psGOPSCDResPri->GOPSCDInfoData.HalfModeEnable = gopscInfo->HalfModeEnable;
		psGOPSCDResPri->GOPSCDInfoData.OutputRateControlEnable = gopscInfo->OutputRateControlEnable;
		psGOPSCDResPri->GOPSCDInfoData.UseP2IFormula = gopscInfo->UseP2IFormula;
    }

    MDrv_GOPSC_SetVScalingMode(_GetDeviceIDFromInstance(pInstance),psGOPSCDResPri->GOPSCDInfoData.en_VScalingMode);
    MDrv_GOPSC_SetSource(_GetDeviceIDFromInstance(pInstance),psGOPSCDResPri->GOPSCDInfoData.en_Source);
    MDrv_GOPSC_SetDelayLineNumber(_GetDeviceIDFromInstance(pInstance),psGOPSCDResPri->GOPSCDInfoData.dlyLineNumber);
    MDrv_GOPSC_P2IEnable(_GetDeviceIDFromInstance(pInstance),psGOPSCDResPri->GOPSCDInfoData.P2IEnable);
    MDrv_GOPSC_HalfMode(_GetDeviceIDFromInstance(pInstance),psGOPSCDResPri->GOPSCDInfoData.HalfModeEnable);

    return En_GOPSC_OK;
}

GOPSC_Result MApi_GOPSC_SetGOPSCInfo(GOPSC_DeviceInfo* DevInfo, GOPSC_Info* gopscInfo)
{
	GOPSCD_SET_INFO GOPSCDArgs;
	_GOPSCD_ENTRY_CHECKER(GOPSCDArgs);
	//If gopscInfo == NULL, it will set value for globle variable directly

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	_GOPSCD_SET_INSTPRI_ID_FROM_INFO(psGOPSCDInstPri,DevInfo);

    GOPSCDArgs.gopscInfo = gopscInfo;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_SET_INFO, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL; // return meaningless value because get instance failed.
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}
}

GOPSC_Result MApi_GOPSC_SetHVSPSize_U2(void* pInstance, const GOPSC_SizeInfo* sizeInfo, MS_U32 flag)
{
    if(NULL == sizeInfo)
    {
        printf("[%s] sizeInfo is NULL\n",__FUNCTION__);
        return En_GOPSC_INVALID_PARAM;
    }
    //No support destination > source (this case is not function of GOPSC)
    if((sizeInfo->destHeight > sizeInfo->srcHeight) || (sizeInfo->destWidth > sizeInfo->srcWidth))
    {
        printf("[%s] sizeInfo data is error\n",__FUNCTION__);
        return En_GOPSC_FAIL;
    }

	GOPSCD_RESOURCE_PRIVATE* psGOPSCDResPri = NULL;
	if(UtopiaResourceGetPrivate(g_pGOPSCDResource, (void*)&psGOPSCDResPri)!=UTOPIA_STATUS_SUCCESS)
	{
		printf("[%s,%5d] Resource Get Private failed\n",__FUNCTION__,__LINE__);
		return En_GOPSC_FAIL;
	}

    if(psGOPSCDResPri->GOPSCDInfoData.OutputRateControlEnable)
    {
        MDrv_GOPSC_SetOutputControlRate(_GetDeviceIDFromInstance(pInstance),TRUE,
            sizeInfo->srcWidth,sizeInfo->srcHeight,sizeInfo->destWidth,sizeInfo->destHeight,
            psGOPSCDResPri->GOPSCDInfoData.P2IEnable);
    }
    else
    {
        MDrv_GOPSC_SetOutputControlRate(_GetDeviceIDFromInstance(pInstance),
            FALSE,0,0,0,0,FALSE);
    }
	MDrv_GOPSC_SetScalingFormula(_GetDeviceIDFromInstance(pInstance),psGOPSCDResPri->GOPSCDInfoData.UseP2IFormula);

    return MDrv_GOPSC_SetHVSP(_GetDeviceIDFromInstance(pInstance),sizeInfo->bEnableScaleX,sizeInfo->bEnableScaleY,
        sizeInfo->srcWidth,sizeInfo->srcHeight,sizeInfo->destWidth,sizeInfo->destHeight);
}

// flag is reserve for enable dither, V-y Scaling, V-c Scaling...options
GOPSC_Result MApi_GOPSC_SetHVSPSize(GOPSC_DeviceInfo* DevInfo, GOPSC_SizeInfo* sizeInfo, MS_U32 flag)
{
	GOPSCD_SET_HVSPSZIE GOPSCDArgs;
	_GOPSCD_ENTRY_CHECKER(GOPSCDArgs);

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	_GOPSCD_SET_INSTPRI_ID_FROM_INFO(psGOPSCDInstPri,DevInfo);

	GOPSCDArgs.sizeInfo = sizeInfo;
	GOPSCDArgs.flag = flag;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_SET_HVSPSIZE, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL; // return meaningless value because get instance failed.
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}
}

GOPSC_Result MApi_GOPSC_SetSkipPixel_U2(void* pInstance, const GOPSC_SkipPixelInfo* skippixelInfo, MS_U32 flag)
{
    if(NULL == skippixelInfo)
    {
        printf("[%s] skippixelInfo is NULL\n",__FUNCTION__);
        return En_GOPSC_INVALID_PARAM;
    }
    if(skippixelInfo->xEnd <= skippixelInfo->xStart || skippixelInfo->yEnd <= skippixelInfo->yStart)
    {
        printf("[%s] skippixelInfo data is error\n",__FUNCTION__);
        return En_GOPSC_FAIL;
    }
    if(skippixelInfo->xEnd > HW_LIMIT_WIDTH)
    {
        printf("[%s] skippixelInfo xEnd must be smaller than HW_LIMIT_WIDTH\n",__FUNCTION__);
        return En_GOPSC_FAIL;
    }
    return MDrv_GOPSC_SetSkipPixel(_GetDeviceIDFromInstance(pInstance),skippixelInfo->bEnableSkipX,skippixelInfo->bEnableSkipY,
        skippixelInfo->xStart,skippixelInfo->xEnd,skippixelInfo->yStart,skippixelInfo->yEnd);
}

GOPSC_Result MApi_GOPSC_SetSkipPixel(GOPSC_DeviceInfo* DevInfo, GOPSC_SkipPixelInfo* skippixelInfo, MS_U32 flag)
{
	GOPSCD_SET_SKIPPIXEL GOPSCDArgs;
	_GOPSCD_ENTRY_CHECKER(GOPSCDArgs);

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	_GOPSCD_SET_INSTPRI_ID_FROM_INFO(psGOPSCDInstPri,DevInfo);

	GOPSCDArgs.skippixelInfo = skippixelInfo;
	GOPSCDArgs.flag = flag;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_SET_SKIPPIXEL, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL; // return meaningless value because get instance failed.
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}

}

#if 0 //take out this because of no use now.
GOPSC_Result MApi_GOPSC_SetFieldStartPosition(GOPSC_DeviceInfo* DevInfo, MS_BOOL enable, MS_U16 topFieldOffset, MS_U16 bottomFieldOffset)
{
    return MDrv_GOPSC_SetFieldStartPosition(_GetDeviceIdFromInfo(DevInfo),
        enable,topFieldOffset,bottomFieldOffset);
}
#endif

GOPSC_Result MApi_GOPSC_ScalingDownOnce(GOPSC_DeviceInfo* DevInfo, MS_U8 FrameNum)
{
	GOPSCD_SCALING GOPSCDArgs;
	_GOPSCD_ENTRY_CHECKER(GOPSCDArgs);

	GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	_GOPSCD_SET_INSTPRI_ID_FROM_INFO(psGOPSCDInstPri,DevInfo);

	GOPSCDArgs.gopscCmd = E_GOPSCD_CMD_SCALING_ONCE;
	GOPSCDArgs.u8FrameCount = FrameNum;
	if(UtopiaIoctl(pu32GOPSCDInst, MAPI_CMD_GOPSCD_SCALING, (void*)&GOPSCDArgs) != UTOPIA_STATUS_SUCCESS)
	{
		printf("Obtain GOPSCD engine fail\n");
		return En_GOPSC_FAIL; // return meaningless value because get instance failed.
	}
	else
	{
		return GOPSCDArgs.bReturnValue;
	}
}

