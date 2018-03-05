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
//
/// This file contains MStar ACE Application Interface.
/// brief API for Advanced Color Engine Control
/// author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef _API_XC_ACE_EX_C_
#define _API_XC_ACE_EX_C_

/******************************************************************************/
/*                    Header Files                                            */
/* ****************************************************************************/

// Need to remove
//#include "msAPI_Timer.h"

#include "apiXC_Ace_EX.h"
#include "MsIRQ.h"
#include "MsOS.h"
// Must have
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

// Internal Definition
#include "drvMMIO.h"
#include "apiXC_Ace.h"
#include "drvACE.h"
#include "apiXC_Ace_v2.h"
#include "apiXC.h"
#include "mvideo_context.h"
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
/*                      Debug information                                      */
/******************************************************************************/
//#define XC_ACE_DBG  0
#define XC_ACE_MUTEX_DBG 0

// OS related
#define XC_ACE_WAIT_MUTEX              (TICK_PER_ONE_MS * 50)              // 50 ms

// Mutex & Lock
#if 0
static MS_S32 _s32XC_ACEMutex = -1;

#if XC_ACE_MUTEX_DBG
#define XC_ACE_ENTRY()      if(!MsOS_ObtainMutex(_s32XC_ACEMutex, MSOS_WAIT_FOREVER))                     \
                            {                                                                        \
                                printf("==========================\n");                              \
                                printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
                            }
#else
#define XC_ACE_ENTRY()      if(!MsOS_ObtainMutex(_s32XC_ACEMutex, MSOS_WAIT_FOREVER))                     \
                            {                                                                        \
                            }
#endif

#define XC_ACE_RETURN()     MsOS_ReleaseMutex(_s32XC_ACEMutex);
#endif

static MSIF_Version _api_xc_ace_version = {
    .DDI = { XC_ACE_EX_API_VERSION },
};

extern void* pu32ACEInst;
extern void* pu32ACE_XCInst;
void* pu32ACEInst_1 = NULL;
void* pu32ACE_XCInst_1 = NULL;

//******************************************************************************
///-initialize ACE
//*************************************************************************

// for Utopia 2.0 to Utopia 1.0 compatibility.
#include "ACE_private.h"
#include "utopia_dapi.h"
#include "utopia.h"

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


#if 0
static E_XC_ACE_EX_RESULT _ACE_Map_Result(E_XC_ACE_RESULT eRet)
{
    E_XC_ACE_EX_RESULT eExRet = E_XC_ACE_EX_FAIL;
    const E_XC_ACE_EX_RESULT eEXRetMapTable[E_XC_ACE_EX_OBTAIN_MUTEX_FAIL+1] =
    {
        E_XC_ACE_EX_FAIL,
        E_XC_ACE_EX_OK,
        E_XC_ACE_EX_GET_BASEADDR_FAIL,
        E_XC_ACE_EX_OBTAIN_MUTEX_FAIL,
    };

    if (eRet <= E_XC_ACE_OBTAIN_RESOURCE_FAIL)
    {
        eExRet = eEXRetMapTable[eRet];
    }
    return eExRet;
}

static XC_ACE_IHC_COLOR_TYPE _ACE_Map_EX_IHC(XC_ACE_EX_IHC_COLOR_TYPE eEXIHC)
{
    XC_ACE_IHC_COLOR_TYPE eIHC = E_ACE_IHC_COLOR_MAX;
    const XC_ACE_IHC_COLOR_TYPE eIHCMapTable[E_ACE_IHC_COLOR_MAX] =
    {
        E_ACE_IHC_COLOR_R,
        E_ACE_IHC_COLOR_G,
        E_ACE_IHC_COLOR_B,
        E_ACE_IHC_COLOR_C,
        E_ACE_IHC_COLOR_M,
        E_ACE_IHC_COLOR_Y,
        E_ACE_IHC_COLOR_F,
    };

    if (eEXIHC < E_ACE_EX_IHC_COLOR_MAX)
    {
        eIHC = eIHCMapTable[eEXIHC];
    }
    return eIHC;
}
#endif //temp remove

static E_XC_ACE_INFOTYPE _ACE_Map_EX_InfoType(E_XC_ACE_EX_INFOTYPE eExInfoType)
{
    E_XC_ACE_INFOTYPE eInfoType = E_XC_ACE_INFO_TYPE_DUMMY;
    const E_XC_ACE_INFOTYPE eInfoTypeMapTable[E_XC_ACE_INFO_TYPE_DUMMY] =
    {
        E_XC_ACE_INFO_TYPE_VERSION,
        E_XC_ACE_INFO_TYPE_CONTRAST,
        E_XC_ACE_INFO_TYPE_R_GAIN,
        E_XC_ACE_INFO_TYPE_G_GAIN,
        E_XC_ACE_INFO_TYPE_B_GAIN,
        E_XC_ACE_INFO_TYPE_SATURATION,
        E_XC_ACE_INFO_TYPE_HUE,
        E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_XY_R,
        E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_XY_G,
        E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_XY_B,
        E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R,
        E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G,
        E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B,
        //E_XC_ACE_INFO_TYPE_BRIGHTNESS_R,
        //E_XC_ACE_INFO_TYPE_BRIGHTNESS_G,
        //E_XC_ACE_INFO_TYPE_BRIGHTNESS_B,
    };

    if (eExInfoType < E_XC_ACE_EX_INFO_TYPE_DUMMY)
    {
        eInfoType = eInfoTypeMapTable[eExInfoType];
    }
    return eInfoType;
}

#if 0  //Fucntion 'MApi_XC_ACE_MWESetMirrorMode' is not in header file
static E_XC_ACE_EX_MWE_MIRROR_TYPE _ACE_Map_EX_MirrorType(E_XC_ACE_MWE_MIRROR_TYPE eMirrorMode)
{
    E_XC_ACE_EX_MWE_MIRROR_TYPE eEXMirrorMode = E_XC_ACE_EX_MWE_MIRROR_MAX;
    const E_XC_ACE_EX_MWE_MIRROR_TYPE eEXMirrorModeMapTable[E_XC_ACE_EX_MWE_MIRROR_MAX] =
    {
        E_XC_ACE_EX_MWE_MIRROR_NORMAL,
        E_XC_ACE_EX_MWE_MIRROR_H_ONLY,
        E_XC_ACE_EX_MWE_MIRROR_V_ONLY,
        E_XC_ACE_EX_MWE_MIRROR_HV,
    };

    if (eMirrorMode < E_XC_ACE_MWE_MIRROR_MAX)
    {
        eEXMirrorMode = eEXMirrorModeMapTable[eMirrorMode];
    }
    return eEXMirrorMode;
}
#endif

static XC_ACE_WEAVETYPE _ACE_Map_EX_WeaveType(XC_ACE_EX_WEAVETYPE eEXWeaveType)
{
    XC_ACE_WEAVETYPE eWeaveType = E_ACE_WEAVETYPE_NUM;
    const XC_ACE_WEAVETYPE eWeaveTypeMapTable[E_ACE_WEAVETYPE_NUM] =
    {
        E_ACE_WEAVETYPE_NONE,
        E_ACE_WEAVETYPE_H,
        E_ACE_WEAVETYPE_V,
        E_ACE_WEAVETYPE_DUALVIEW,
    };

    if (eEXWeaveType < E_ACE_EX_WEAVETYPE_NUM)
    {
        eWeaveType = eWeaveTypeMapTable[eEXWeaveType];
    }
    return eWeaveType;
}

static E_XC_ACE_MWE_FUNC _ACE_Map_EX_MWEFunc(E_XC_ACE_EX_MWE_FUNC eEXMWEFunc)
{
    E_XC_ACE_MWE_FUNC eMWEFunc = E_XC_ACE_MWE_MODE_NUMS;
    const E_XC_ACE_MWE_FUNC eMWEFuncMapTable[E_XC_ACE_MWE_MODE_NUMS] =
    {
        E_XC_ACE_MWE_MODE_OFF,
        E_XC_ACE_MWE_MODE_H_SPLIT,
        E_XC_ACE_MWE_MODE_MOVE,
        E_XC_ACE_MWE_MODE_ZOOM,
        E_XC_ACE_MWE_MODE_H_SCAN,
        E_XC_ACE_MWE_MODE_H_SPLIT_LEFT,
    };

    if (eEXMWEFunc < E_XC_ACE_EX_MWE_MODE_NUMS)
    {
        eMWEFunc = eMWEFuncMapTable[eEXMWEFunc];
    }
    return eMWEFunc;
}

static XC_ACE_DNR_Param _ACE_Map_EX_DNRParam(XC_ACE_EX_DNR_Param eEXDNRParam)
{
    XC_ACE_DNR_Param eDNRParam = E_ACE_DNR_NR_ONOFF;
    const XC_ACE_DNR_Param eDNRParamMapTable[E_ACE_DNR_SNR_NM+1] =
    {
        E_ACE_DNR_NR_ONOFF,
        E_ACE_DNR_SNR,
        E_ACE_DNR_SPIKE_NR_0,
        E_ACE_DNR_SPIKE_NR_1,
        E_ACE_DNR_GRAY_GROUND_GAIN,
        E_ACE_DNR_GRAY_GROUND_EN,
        E_ACE_DNR_PEAKING_BANK_CORING,
        E_ACE_DNR_GUASSIN_SNR_THRESHOLD,
        E_ACE_DNR_MOTION,
        E_ACE_DNR_PEAKING_CORING_THRESHOLD,
        E_ACE_DNR_SHARPNESS_ADJUST,
        E_ACE_DNR_NM_V,
        E_ACE_DNR_GNR_0,
        E_ACE_DNR_GNR_1,
        E_ACE_DNR_CP,
        E_ACE_DNR_DP,
        E_ACE_DNR_NM_H_0,
        E_ACE_DNR_NM_H_1,
        E_ACE_DNR_SC_CORING,
        E_ACE_DNR_SNR_NM,
    };

    if (eEXDNRParam <= E_ACE_EX_DNR_SNR_NM)
    {
        eDNRParam = eDNRParamMapTable[eEXDNRParam];
    }
    return eDNRParam;
}

static MS_BOOL _ACE_Map_EX_InitData(XC_ACE_InitData *pstInitData, XC_ACE_EX_InitData* pstExInitData)
{
    pstInitData->bMWE_Enable = pstExInitData->bMWE_Enable;
    pstInitData->eWindow = pstExInitData->eWindow;
    //below pointer is null pointer, can not do memcpy
    //memcpy(pstInitData->S16ColorCorrectionMatrix,pstExInitData->S16ColorCorrectionMatrix,32);
    //memcpy(pstInitData->S16RGB,pstExInitData->S16RGB,9);
    pstInitData->S16ColorCorrectionMatrix = pstExInitData->S16ColorCorrectionMatrix;
    pstInitData->S16RGB = pstExInitData->S16RGB;
    pstInitData->u16MWEHeight = pstExInitData->u16MWEHeight;
    pstInitData->u16MWEHstart = pstExInitData->u16MWEHstart;
    pstInitData->u16MWEVstart = pstExInitData->u16MWEVstart;
    pstInitData->u16MWEWidth = pstExInitData->u16MWEWidth;
    pstInitData->u16MWE_Disp_Height = pstExInitData->u16MWE_Disp_Height;
    pstInitData->u16MWE_Disp_Hstart = pstExInitData->u16MWE_Disp_Hstart;
    pstInitData->u16MWE_Disp_Vstart = pstExInitData->u16MWE_Disp_Vstart;
    pstInitData->u16MWE_Disp_Width = pstExInitData->u16MWE_Disp_Width;
    return TRUE;
}

static void _ACE_Map_EX_InfoEX(XC_ACE_InfoEx *pInfoEx, XC_ACE_EX_InfoEx *pEXInfoEx)
{
    pEXInfoEx->u32Mode = pInfoEx->u32Mode;
    pEXInfoEx->u32Reserved = pInfoEx->u32Reserved;
}

static void _ACE_Map_EX_ColorTemp(XC_ACE_color_temp *pColorTemp, XC_ACE_EX_color_temp *pEXColorTemp)
{
    pColorTemp->cBlueColor = pEXColorTemp->cBlueColor;
    pColorTemp->cBlueOffset = pEXColorTemp->cBlueOffset;
    pColorTemp->cBlueScaleValue = pEXColorTemp->cBlueScaleValue;
    pColorTemp->cGreenColor = pEXColorTemp->cGreenColor;
    pColorTemp->cGreenOffset = pEXColorTemp->cGreenOffset;
    pColorTemp->cGreenScaleValue = pEXColorTemp->cGreenScaleValue;
    pColorTemp->cRedColor = pEXColorTemp->cRedColor;
    pColorTemp->cRedOffset = pEXColorTemp->cRedOffset;
    pColorTemp->cRedScaleValue = pEXColorTemp->cRedScaleValue;
}

static void _ACE_Map_EX_ColorTempEX(XC_ACE_color_temp_ex *pColorTemp, XC_ACE_EX_color_temp_ex *pEXColorTemp)
{
    pColorTemp->cBlueColor = pEXColorTemp->cBlueColor;
    pColorTemp->cBlueOffset = pEXColorTemp->cBlueOffset;
    pColorTemp->cBlueScaleValue = pEXColorTemp->cBlueScaleValue;
    pColorTemp->cGreenColor = pEXColorTemp->cGreenColor;
    pColorTemp->cGreenOffset = pEXColorTemp->cGreenOffset;
    pColorTemp->cGreenScaleValue = pEXColorTemp->cGreenScaleValue;
    pColorTemp->cRedColor = pEXColorTemp->cRedColor;
    pColorTemp->cRedOffset = pEXColorTemp->cRedOffset;
    pColorTemp->cRedScaleValue = pEXColorTemp->cRedScaleValue;
}

static void _ACE_Map_EX_ColorTempEX2(XC_ACE_color_temp_ex2 *pColorTemp, XC_ACE_EX_color_temp_ex2 *pEXColorTemp)
{
    pColorTemp->cBlueColor = pEXColorTemp->cBlueColor;
    pColorTemp->cBlueOffset = pEXColorTemp->cBlueOffset;
    pColorTemp->cBlueScaleValue = pEXColorTemp->cBlueScaleValue;
    pColorTemp->cGreenColor = pEXColorTemp->cGreenColor;
    pColorTemp->cGreenOffset = pEXColorTemp->cGreenOffset;
    pColorTemp->cGreenScaleValue = pEXColorTemp->cGreenScaleValue;
    pColorTemp->cRedColor = pEXColorTemp->cRedColor;
    pColorTemp->cRedOffset = pEXColorTemp->cRedOffset;
    pColorTemp->cRedScaleValue = pEXColorTemp->cRedScaleValue;
}

static void _ACE_Map_EX_ApiStaus(XC_ACE_ApiStatus *pApiStatus, XC_ACE_EX_ApiStatus *pEXApiStatus)
{
    pEXApiStatus->ACEinit.bMWE_Enable = pApiStatus->ACEinit.bMWE_Enable;
    pEXApiStatus->ACEinit.eWindow = pApiStatus->ACEinit.eWindow;
    memcpy(pEXApiStatus->ACEinit.S16ColorCorrectionMatrix,pApiStatus->ACEinit.S16ColorCorrectionMatrix,32);
    memcpy(pEXApiStatus->ACEinit.S16RGB,pApiStatus->ACEinit.S16RGB,9);
    pEXApiStatus->ACEinit.u16MWEHeight = pApiStatus->ACEinit.u16MWEHeight;
    pEXApiStatus->ACEinit.u16MWEHstart = pApiStatus->ACEinit.u16MWEHstart;
    pEXApiStatus->ACEinit.u16MWEVstart = pApiStatus->ACEinit.u16MWEVstart;
    pEXApiStatus->ACEinit.u16MWEWidth = pApiStatus->ACEinit.u16MWEWidth;
    pEXApiStatus->ACEinit.u16MWE_Disp_Height = pApiStatus->ACEinit.u16MWE_Disp_Height;
    pEXApiStatus->ACEinit.u16MWE_Disp_Hstart = pApiStatus->ACEinit.u16MWE_Disp_Hstart;
    pEXApiStatus->ACEinit.u16MWE_Disp_Vstart = pApiStatus->ACEinit.u16MWE_Disp_Vstart;
    pEXApiStatus->ACEinit.u16MWE_Disp_Width = pApiStatus->ACEinit.u16MWE_Disp_Width;
}

static void _ACE_Map_EX_TABInfo(XC_ACE_TAB_Info *pMWETable, XC_ACE_EX_TAB_Info *pEXMWETable)
{
    pMWETable->pTable = pEXMWETable->pTable;
    pMWETable->u32ACE_TabInfo_version = pEXMWETable->u32ACE_TabInfo_version;
    pMWETable->u8TabCols = pEXMWETable->u8TabCols;
    pMWETable->u8TabIdx = pEXMWETable->u8TabIdx;
    pMWETable->u8TabRows = pEXMWETable->u8TabRows;
}

#if 0
void* _ACE_SELECT_INSTANCE(MS_U32 u32Id)
{
    switch(u32Id)
    {
        case 1:
            return pu32ACEInst_1;
        case 0:
        default:
            return pu32ACEInst;
    }
}
#else
#define _ACE_SELECT_INSTANCE(u32Id) ((u32Id == 0)? pu32ACEInst : pu32ACEInst_1)
#endif

MS_BOOL MApi_XC_ACE_EX_Init(XC_ACE_DeviceId *pDeviceId, XC_ACE_EX_InitData *pstXC_ACE_InitData, MS_U32 u32InitDataLen)
{
    XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
    memset(&stXCInstantAttribute, 0, sizeof(stXCInstantAttribute));
    stXCInstantAttribute.u32DeviceID = 0;
    stXCInstantAttribute.eXCType     = E_XC_INSTANT_TYPE_ACE;

    if (pDeviceId->u32Id == 0 && pu32ACEInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACE failed\n");
            return E_XC_ACE_FAIL;
        }

    }
    else if(pDeviceId->u32Id == 1 && pu32ACEInst_1 == NULL)
    {
        MS_U32 u32ID = pDeviceId->u32Id;
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst_1, 0, &u32ID) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACE failed\n");
            return E_XC_ACE_FAIL;
        }

    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        printf("Device %td has initialized\n",(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_ACE_FAIL;
    }


    if (pDeviceId->u32Id == 0 && pu32ACE_XCInst == NULL)
    {

        if(UtopiaOpen(MODULE_XC, &pu32ACE_XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen pu32ACE_XCInst failed\n");
            return E_XC_ACE_FAIL;
        }

    }
    else if(pDeviceId->u32Id == 1 && pu32ACE_XCInst_1 == NULL)
    {
        stXCInstantAttribute.u32DeviceID = 1;
        if(UtopiaOpen(MODULE_XC, &pu32ACE_XCInst_1, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen pu32ACE_XCInst failed\n");
            return E_XC_ACE_FAIL;
        }
    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        printf("Device %td has initialized\n",(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_ACE_FAIL;
    }


    stACE_Init ACEArgs;
    XC_ACE_InitData stXC_ACE_InitData;
    memset(&stXC_ACE_InitData,0,sizeof(stXC_ACE_InitData));
    ACEArgs.pstXC_ACE_InitData = &stXC_ACE_InitData;
    _ACE_Map_EX_InitData(ACEArgs.pstXC_ACE_InitData,pstXC_ACE_InitData);
    ACEArgs.u32InitDataLen = u32InitDataLen;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_INIT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_EX_Exit(XC_ACE_DeviceId *pDeviceId)
{
    if (pDeviceId->u32Id == 0 && pu32ACEInst == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }
    if (pDeviceId->u32Id == 1 && pu32ACEInst_1 == NULL)
    {
        printf("ACE not initialized!\n");
        return FALSE;
    }

    stACE_Exit ACEArgs;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl((pDeviceId->u32Id == 0)? pu32ACEInst : pu32ACEInst_1, E_ACE_CMD_EXIT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        ACEArgs.bReturn = FALSE;
    }

    if(UtopiaClose((pDeviceId->u32Id == 0)? pu32ACEInst : pu32ACEInst_1) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaClose ACE failed\n");
        return FALSE;
    }
    if(pDeviceId->u32Id == 0)
    {
        pu32ACEInst = NULL;
    }
    else
    {
        pu32ACEInst_1 = NULL;
    }

    if(UtopiaClose((pDeviceId->u32Id == 0)? pu32ACE_XCInst : pu32ACE_XCInst_1) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaClose ACE failed\n");
        return FALSE;
    }
    if(pDeviceId->u32Id == 0)
    {
        pu32ACE_XCInst = NULL;
    }
    else
    {
        pu32ACE_XCInst_1 = NULL;
    }


    return ACEArgs.bReturn;

}

void MApi_XC_ACE_EX_DMS(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bisATV)
{
    stACE_DMS ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bisATV = bisATV;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DMS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;

}

void MApi_XC_ACE_EX_SetPCYUV2RGB(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bEnable)
{
    stACE_SetPCYUV2RGB ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bEnable = bEnable;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETPCYUV2RGB, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SelectYUVtoRGBMatrix(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix)
{
    stACE_SelectYUVtoRGBMatrix ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.ucMatrix = ucMatrix;
    ACEArgs.psUserYUVtoRGBMatrix = psUserYUVtoRGBMatrix;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SELECTYUVTORGBMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetColorCorrectionTable( XC_ACE_DeviceId *pDeviceId, MS_BOOL bScalerWin)
{
    stACE_SetColorCorrectionTable ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETCOLORCORRECTIONTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetPCsRGBTable( XC_ACE_DeviceId *pDeviceId, MS_BOOL bScalerWin)
{
    stACE_SetPCsRGBTable ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETPCSRGBTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_GetColorMatrix( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U16* pu16Matrix)
{
    stACE_ColorMatrix ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pu16Matrix = pu16Matrix;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_GETCOLORMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetColorMatrix( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U16* pu16Matrix)
{
    stACE_ColorMatrix ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.pu16Matrix = pu16Matrix;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETCOLORMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

E_XC_ACE_EX_RESULT MApi_XC_ACE_EX_SetBypassColorMatrix( XC_ACE_DeviceId *pDeviceId, MS_BOOL bEnable )
{
    if (pDeviceId->u32Id == 0 && pu32ACEInst == NULL)
    {
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACE failed\n");
            return E_XC_ACE_FAIL;
        }
        ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
        UtopiaInstanceGetPrivate(pu32ACEInst, (void**)&psACEInstPri);
        psACEInstPri->u32DeviceID = 0;
    }
    else if(pDeviceId->u32Id == 1 && pu32ACEInst_1 == NULL)
    {
        if(UtopiaOpen(MODULE_ACE, &pu32ACEInst_1, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen ACE failed\n");
            return E_XC_ACE_FAIL;
        }
        ACE_INSTANCE_PRIVATE *psACEInstPri = NULL;
        UtopiaInstanceGetPrivate(pu32ACEInst_1, (void**)&psACEInstPri);
        psACEInstPri->u32DeviceID = 1;
    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        printf("Device %td has initialized\n",(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_ACE_FAIL;
    }


    stACE_SetBypassColorMatrix ACEArgs;
    ACEArgs.bEnable = bEnable;
    ACEArgs.eResult = E_XC_ACE_FAIL;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETBYPASSCOLORMATRIX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        return ACEArgs.eResult;
    }
}

MS_BOOL MApi_XC_ACE_EX_SetIHC(XC_ACE_DeviceId *pDeviceId, MS_BOOL bScalerWin, XC_ACE_EX_IHC_COLOR_TYPE eIHC, MS_U8 u8Val)
{
    stACE_SetIHC ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.eIHC = eIHC;
    ACEArgs.u8Val = u8Val;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETIHC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

void MApi_XC_ACE_EX_PatchDTGColorChecker( XC_ACE_DeviceId *pDeviceId, MS_U8 u8Mode)
{
    stACE_PatchDTGColorChecker ACEArgs;
    ACEArgs.u8Mode = u8Mode;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PATCHDTGCOLORCHECKER, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_U16 MApi_XC_ACE_EX_GetACEInfo( XC_ACE_DeviceId *pDeviceId, MS_BOOL bWindow, E_XC_ACE_EX_INFOTYPE eXCACEInfoType )
{
    stACE_GetACEInfo ACEArgs;
    ACEArgs.bWindow = bWindow;
    ACEArgs.eXCACEInfoType = _ACE_Map_EX_InfoType(eXCACEInfoType);
    ACEArgs.u16Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_GETACEINFO, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return ACEArgs.u16Return;
    }
}

void MApi_XC_ACE_EX_GetACEInfoEx( XC_ACE_DeviceId *pDeviceId, MS_BOOL bWindow, XC_ACE_EX_InfoEx *pInfoEx)
{
    XC_ACE_InfoEx pRawInfoEx;
    stACE_GetACEInfoEx ACEArgs;
    ACEArgs.bWindow = bWindow;
    ACEArgs.pInfoEx = &pRawInfoEx;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_GETACEINFOEX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    _ACE_Map_EX_InfoEX(ACEArgs.pInfoEx,pInfoEx);
    return;
}

/********************************************************************************/
/*                   msAPI_ACE_Ext.c                   */
/********************************************************************************/

/********************************************************************************/
/*                   Functions                      */
/********************************************************************************/


/******************************************************************************/
///-This function will set up brightness for YPbPr and digital port
///@param eWindow        \b IN: Window type
///@param u8Brightness_R \b IN: brightness R value
///@param u8Brightness_G \b IN: brightness G value
///@param u8Brightness_B \b IN: brightness B value
/******************************************************************************/
void MApi_XC_ACE_EX_PicSetBrightness( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U8 u8Brightness_R, MS_U8 u8Brightness_G, MS_U8 u8Brightness_B)
{
    stACE_PicSetBrightness ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u8Brightness_R = u8Brightness_R;
    ACEArgs.u8Brightness_G = u8Brightness_G;
    ACEArgs.u8Brightness_B = u8Brightness_B;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETBRIGHTNESS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
///-This function will set up Precise brightness for YPbPr and digital port
///@param eWindow        \b IN: Window type
///@param u16Brightness_R \b IN: brightness R value
///@param u16Brightness_G \b IN: brightness G value
///@param u16Brightness_B \b IN: brightness B value
/******************************************************************************/
void MApi_XC_ACE_EX_PicSetBrightnessPrecise(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U16 u16Brightness_R, MS_U16 u16Brightness_G, MS_U16 u16Brightness_B)
{
    stACE_PicSetBrightnessPrecise ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u16Brightness_R = u16Brightness_R;
    ACEArgs.u16Brightness_G = u16Brightness_G;
    ACEArgs.u16Brightness_B = u16Brightness_B;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETBRIGHTNESSPRECISE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
///-This function will set Pre Y Offset
///@param eWindow        \b IN: Window type
///@param u8PreYOffset \b IN: Pre Y Offset value
/******************************************************************************/
E_XC_ACE_EX_RESULT MApi_XC_ACE_EX_PicSetPreYOffset(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U8 u8PreYOffset)
{
    stACE_PreYOffset stACEArgs;
    stACEArgs.eWindow = eWindow;
    stACEArgs.u8PreYOffset = u8PreYOffset;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSET_PRE_Y_OFFSET, (void*)&stACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_OBTAIN_RESOURCE_FAIL;
    }

    return E_XC_ACE_OK;
}

/******************************************************************************/
///-This function will get Pre Y Offset
///@param eWindow        \b IN: Window type
/******************************************************************************/
MS_U8 MApi_XC_ACE_EX_PicGetPreYOffset(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow)
{
    stACE_PreYOffset stACEArgs;
    stACEArgs.eWindow = eWindow;
    stACEArgs.u8PreYOffset = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICGET_PRE_Y_OFFSET, (void*)&stACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_OBTAIN_RESOURCE_FAIL;
    }

    return stACEArgs.u8PreYOffset;
}

/******************************************************************************/
//-Adjust sharpness
///@param eWindow        \b IN: Window type
///@param u8Sharpness    \b IN: sharpness value
//- 0 -0x3f
/******************************************************************************/
void MApi_XC_ACE_EX_PicSetSharpness( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U8 u8Sharpness )
{
    stACE_PicSetSharpness ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u8Sharpness = u8Sharpness;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETSHARPNESS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
//-Adjust contrast for YPbPr and digital port
//@param u8Contrast \b IN:
//- video color Contrast setting 0 -0xff
//@param u8SubContrast \b IN:
//- video color SubContrast setting 0 -0xff
/******************************************************************************/

void MApi_XC_ACE_EX_PicSetContrast( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast)
{
    stACE_PicSetContrast ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.u8Contrast = u8Contrast;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETCONTRAST, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
///-Adjust Hue for YPbPr and digital port
///@param eWindow \b IN: Enable Hue
///@param bUseYUVSpace \b IN: Is Yuv space
///@param u8Hue \b IN: Hue value
/******************************************************************************/
void MApi_XC_ACE_EX_PicSetHue( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Hue )
{
    stACE_PicSetHue ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.u8Hue = u8Hue;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETHUE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//-------------------------------------------------------------------------------------------------
// Skip Wait Vsync
/// @param eWindow               \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
E_XC_ACE_EX_RESULT MApi_XC_ACE_EX_SkipWaitVsync( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{
    stACE_SkipWaitVsync ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bIsSkipWaitVsyn = bIsSkipWaitVsyn;
    ACEArgs.eResult = E_XC_ACE_FAIL;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SKIPWAITVSYNC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return E_XC_ACE_FAIL;
    }
    else
    {
        return  ACEArgs.eResult;
    }
}

/******************************************************************************/
//-Adjust Saturation for YPbPr and digital port
//@param u8Saturation \b IN:
//- video color setting 0 -100
/******************************************************************************/
void MApi_XC_ACE_EX_PicSetSaturation(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Saturation )
{
    stACE_PicSetSaturation ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    ACEArgs.u8Saturation = u8Saturation;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETSATURATION, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetFleshTone(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bEn, MS_U8 u8FleshTone)
{
    stACE_SetFleshTone ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bEn = bEn;
    ACEArgs.u8FleshTone = u8FleshTone;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETFLESHTONE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetBlackAdjust(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_U8 u8BlackAdjust)
{
    stACE_SetBlackAdjust ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.u8BlackAdjust = u8BlackAdjust;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETBLACKADJUST, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_EX_PicSetColorTemp( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_EX_color_temp* pstColorTemp)
{
    stACE_PicSetColorTemp ACEArgs;
    XC_ACE_color_temp  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    _ACE_Map_EX_ColorTemp(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETCOLORTEMP, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color ex
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_EX_PicSetColorTempEx( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_EX_color_temp_ex* pstColorTemp)
{
    stACE_PicSetColorTempEx ACEArgs;
    XC_ACE_color_temp_ex  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    ACEArgs.bUseYUVSpace = bUseYUVSpace;
    _ACE_Map_EX_ColorTempEX(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETCOLORTEMPEX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///- set post color temp Version 2
///@param eWindow \b IN: Enable Color temp
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_EX_PicSetPostColorTemp_V02(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, XC_ACE_EX_color_temp_ex2* pstColorTemp)
{
    stACE_PicSetPostColorTemp_V02 ACEArgs;
    XC_ACE_color_temp_ex2  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    _ACE_Map_EX_ColorTempEX2(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETPOSTCOLORTEMP_V02, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-obosolte!! use MApi_XC_ACE_EX_PicSetPostColorTemp_V02 instead
//*************************************************************************
void MApi_XC_ACE_EX_PicSetPostColorTemp(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, XC_ACE_EX_color_temp* pstColorTemp)
{
    stACE_PicSetPostColorTemp ACEArgs;
    XC_ACE_color_temp  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    _ACE_Map_EX_ColorTemp(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETPOSTCOLORTEMP, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-obosolte!! use MApi_XC_ACE_EX_PicSetPostColorTemp_V02 instead
//*************************************************************************
void MApi_XC_ACE_EX_PicSetPostColorTempEx(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, XC_ACE_EX_color_temp_ex* pstColorTemp)
{
    stACE_PicSetPostColorTempEx ACEArgs;
    XC_ACE_color_temp_ex  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    _ACE_Map_EX_ColorTempEX(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETPOSTCOLORTEMPEX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;

}

//*************************************************************************
///-set video color
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_EX_PicSetPostColorTemp2(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_EX_color_temp* pstColorTemp)
{
    stACE_PicSetPostColorTemp ACEArgs;
    XC_ACE_color_temp  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    _ACE_Map_EX_ColorTemp(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETPOSTCOLORTEMP2, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

//*************************************************************************
///-set video color ex
///@param eWindow \b IN: Enable Color temp
///@param bUseYUVSpace \b IN: Is in YUV space
///@param pstColorTemp \b IN: Color temp need to be set
//*************************************************************************
void MApi_XC_ACE_EX_PicSetPostColorTemp2Ex(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_EX_color_temp_ex* pstColorTemp)
{
    stACE_PicSetPostColorTempEx ACEArgs;
    XC_ACE_color_temp_ex  stColorTemp;
    memset(&stColorTemp,0,sizeof(stColorTemp));
    ACEArgs.pstColorTemp= &stColorTemp;
    ACEArgs.eWindow = eWindow;
    _ACE_Map_EX_ColorTempEX(ACEArgs.pstColorTemp,pstColorTemp);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_PICSETPOSTCOLORTEMP2EX, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

/******************************************************************************/
/*                     Local                                                  */
/* ****************************************************************************/

E_XC_ACE_EX_RESULT MApi_XC_ACE_EX_GetLibVer(const MSIF_Version **ppVersion)              // Get version
{
    if (!ppVersion)
    {
        return E_XC_ACE_EX_FAIL;
    }

    *ppVersion = &_api_xc_ace_version;
    return E_XC_ACE_EX_OK;
}

const XC_ACE_EX_ApiInfo * MApi_XC_ACE_EX_GetInfo(XC_ACE_DeviceId *pDeviceId)          // Get info from driver
{
    stACE_GetInfo ACEArgs;
    ACEArgs.pstApiInfo = NULL;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_GETINFO, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return NULL;
    }
    else
    {
        return (XC_ACE_EX_ApiInfo*)ACEArgs.pstApiInfo;
    }
}

MS_BOOL MApi_XC_ACE_EX_GetStatus(XC_ACE_DeviceId *pDeviceId, XC_ACE_EX_ApiStatus *pDrvStatus, MS_BOOL bWindow)    // Get DLC current status
{
    XC_ACE_ApiStatus stDrvStatus;
    stACE_GetStatus ACEArgs;
    ACEArgs.pDrvStatus = &stDrvStatus;
    ACEArgs.bWindow = bWindow;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_GETSTATUS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        _ACE_Map_EX_ApiStaus(&stDrvStatus, pDrvStatus);
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_EX_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    stACE_SetDbgLevel ACEArgs;
    ACEArgs.u16DbgSwitch = u16DbgSwitch;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(pu32ACEInst, E_ACE_CMD_SETDBGLEVEL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

/******************************************************************************/
/*                   Functions                                                */
/******************************************************************************/

void MApi_XC_ACE_EX_EnableMWE(XC_ACE_DeviceId *pDeviceId, MS_BOOL bEnable)
{
    stACE_EnableMWE ACEArgs;
    ACEArgs.bEnable = bEnable;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_ENABLEMWE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

#if 0
void MApi_XC_ACE_EX_MWESetMirrorMode( XC_ACE_DeviceId *pDeviceId, E_XC_ACE_EX_MWE_MIRROR_TYPE eMirrorMode )
{
    _ACE_EX_SET_PRIVATE_DEVICE_ID(pDeviceId->u32Id);
    MApi_XC_ACE_MWESetMirrorMode(eMirrorMode);
}
#endif

void MApi_XC_ACE_EX_MWECloneVisualEffect(XC_ACE_DeviceId *pDeviceId)
{
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_MWECLONEVISUALEFFECT, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;

}

void MApi_XC_ACE_EX_3DClonePQMap(XC_ACE_DeviceId *pDeviceId, XC_ACE_EX_WEAVETYPE enWeaveType)
{
    stACE_3DClonePQMap ACEArgs;
    ACEArgs.enWeaveType = _ACE_Map_EX_WeaveType(enWeaveType);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_3DCLONEPQMAP, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_BOOL MApi_XC_ACE_EX_MWESetRegTable(XC_ACE_DeviceId *pDeviceId, XC_ACE_EX_TAB_Info *pMWETable)
{
    stACE_MWESetRegTable ACEArgs;
    XC_ACE_TAB_Info pstMWETable;
    memset(&pstMWETable,0,sizeof(XC_ACE_TAB_Info));
    ACEArgs.pMWETable = &pstMWETable;
    _ACE_Map_EX_TABInfo(ACEArgs.pMWETable,pMWETable);
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_MWESETREGTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_EX_SetMWELoadFromPQ(XC_ACE_DeviceId *pDeviceId, MS_BOOL bEnable)
{
    stACE_SetMWELoadFromPQ ACEArgs;
    ACEArgs.bEnable = bEnable;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETMWELOADFROMPQ, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

void MApi_XC_ACE_EX_MWESetDispWin(XC_ACE_DeviceId *pDeviceId, MS_U16 u16MWE_Disp_Hstart, MS_U16 u16MWE_Disp_Vstart, MS_U16 u16MWE_Disp_Width, MS_U16 u16MWE_Disp_Height)
{
    stACE_MWESetDispWin ACEArgs;
    ACEArgs.u16MWE_Disp_Hstart = u16MWE_Disp_Hstart;
    ACEArgs.u16MWE_Disp_Vstart = u16MWE_Disp_Vstart;
    ACEArgs.u16MWE_Disp_Width = u16MWE_Disp_Width;
    ACEArgs.u16MWE_Disp_Height = u16MWE_Disp_Height;

    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_MWESETDISPWIN, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_BOOL MApi_XC_ACE_EX_MWEStatus(XC_ACE_DeviceId *pDeviceId)
{
    stACE_MWEStatus ACEArgs;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_MWESTATUS, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

void MApi_XC_ACE_EX_MWEFuncSel( XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, E_XC_ACE_EX_MWE_FUNC  mwe_func)
{
    stACE_MWEFuncSel ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.mwe_func = _ACE_Map_EX_MWEFunc(mwe_func);
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_MWEFUNCSEL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_MWEHandle( XC_ACE_DeviceId *pDeviceId )
{
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_MWEHANDLE, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_BOOL MApi_XC_ACE_EX_Set_IHC_SRAM(XC_ACE_DeviceId *pDeviceId, MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    stACE_Set_IHC_SRAM ACEArgs;
    ACEArgs.pBuf = pBuf;
    ACEArgs.u16ByteSize = u16ByteSize;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SET_IHC_SRAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

MS_BOOL MApi_XC_ACE_EX_Set_ICC_SRAM(XC_ACE_DeviceId *pDeviceId, MS_U16 *pBuf, MS_U16 u16ByteSize)
{
    stACE_Set_ICC_SRAM ACEArgs;
    ACEArgs.pBuf = pBuf;
    ACEArgs.u16ByteSize = u16ByteSize;
    ACEArgs.bReturn = FALSE;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SET_ICC_SRAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return FALSE;
    }
    else
    {
        return ACEArgs.bReturn;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//  DynamicNR start
//
////////////////////////////////////////////////////////////////////////////////
#define NR_DBG(x)   //x

#if 0
#define DYNAMIC_NR_TBL_REG_NUM                  8
#define DYNAMICNR_DEFLICKER_PRECISION_SHFIT     64UL
#define DYNAMICNR_DEFLICK_TOTAL                 32UL
#define DYNAMICNR_DEFLICK_STABLE_CNT            0x10


MS_U8  g_u8MotionStableCnt[XC_ACE_EX_MAX_DEVICE_NUM];
MS_U8  g_u8LumaStableCnt[XC_ACE_EX_MAX_DEVICE_NUM];
MS_U8  g_u8FlickStableCnt[XC_ACE_EX_MAX_DEVICE_NUM];
#endif

MS_U16 MApi_XC_ACE_EX_DNR_Get_PrecisionShift(XC_ACE_DeviceId *pDeviceId)
{
    stACE_DNR_Get_PrecisionShift ACEArgs;
    ACEArgs.u16Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_GET_PRECISIONSHIFT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return ACEArgs.u16Return;
    }
}

void MApi_XC_ACE_EX_DNR_Blending_NRTbl(
    XC_ACE_DeviceId *pDeviceId,
    MS_U8 *pu8NewNR,
    MS_U8 u8Weight,
    MS_U8 u8Step,
    MS_U16 *pu16PreY0,
    MS_U16 *pu16PreY1,
    MS_U8 *pu8NRTBL)
{
    stACE_DNR_Blending_NRTbl ACEArgs;
    ACEArgs.pu8NewNR = pu8NewNR;
    ACEArgs.u8Weight = u8Weight;
    ACEArgs.pu16PreY0 = pu16PreY0;
    ACEArgs.pu16PreY1 = pu16PreY1;
    ACEArgs.pu8NRTBL = pu8NRTBL;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_BLENDING_NRTBL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_U8 MApi_XC_ACE_EX_DNR_Blending(XC_ACE_DeviceId *pDeviceId, MS_U8 u8NewItem, MS_U16 u16PreItem, MS_U8 u8Weight)
{
    stACE_DNR_Blending ACEArgs;
    ACEArgs.u8NewItem = u8NewItem;
    ACEArgs.u16PreItem = u16PreItem;
    ACEArgs.u8Weight = u8Weight;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_BLENDING, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U16 MApi_XC_ACE_EX_DNR_Blending_MISC(
    XC_ACE_DeviceId *pDeviceId,
    MS_U8 u8NewItem,
    MS_U16 u16PreItem,
    MS_U8 u8Weight,
    MS_U8 u8Step)
{
    stACE_DNR_Blending_MISC ACEArgs;
    ACEArgs.u8NewItem = u8NewItem;
    ACEArgs.u16PreItem = u16PreItem;
    ACEArgs.u8Weight = u8Weight;
    ACEArgs.u8Step = u8Step;
    ACEArgs.u16Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_BLENDING_MISC, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u16Return;
    }
}


MS_U8 MApi_XC_ACE_EX_DNR_GetMotion(XC_ACE_DeviceId *pDeviceId)
{
    stACE_DNR_GetMotion ACEArgs;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_GETMOTION, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}


MS_U8 MApi_XC_ACE_EX_DNR_GetMotion_Weight(
    XC_ACE_DeviceId *pDeviceId,
    MS_U8 u8CurMotion,
    MS_U8 u8PreMotion,
    MS_U8 u8CurMotionLvl,
    MS_U8 u8PreMotionLvl,
    MS_U8 u8DeFlick_Thre,
    MS_U8 u8DeFilick_Step)
{
    stACE_DNR_GetMotion_Weight ACEArgs;
    ACEArgs.u8CurMotion = u8CurMotion;
    ACEArgs.u8PreMotion = u8PreMotion;
    ACEArgs.u8CurMotionLvl = u8CurMotionLvl;
    ACEArgs.u8PreMotionLvl = u8CurMotionLvl;
    ACEArgs.u8DeFlick_Thre = u8DeFlick_Thre;
    ACEArgs.u8DeFilick_Step = u8DeFilick_Step;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_GETMOTION_WEIGHT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U8  MApi_XC_ACE_EX_DNR_GetLuma_Weight(
    XC_ACE_DeviceId *pDeviceId,
    MS_U8 u8CurAvgLuma,
    MS_U8 u8PreAvgLuam,
    MS_U8 u8CurLumaLvl,
    MS_U8 u8PreLumaLvl,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step)
{
    stACE_DNR_GetLuma_Weight ACEArgs;
    ACEArgs.u8CurAvgLuma = u8CurAvgLuma;
    ACEArgs.u8PreAvgLuam = u8PreAvgLuam;
    ACEArgs.u8CurLumaLvl = u8CurLumaLvl;
    ACEArgs.u8PreLumaLvl = u8PreLumaLvl;
    ACEArgs.u8DeFlick_Th = u8DeFlick_Th;
    ACEArgs.u8DeFlick_Step = u8DeFlick_Step;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_GETLUMA_WEIGHT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

MS_U8 MApi_XC_ACE_EX_DNR_GetNoise_Weight(
    XC_ACE_DeviceId *pDeviceId,
    MS_U8 u8CurNoise,
    MS_U8 u8PreNoise,
    MS_U8 u8Range,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step)
{
    stACE_DNR_GetNoise_Weight ACEArgs;
    ACEArgs.u8CurNoise = u8CurNoise;
    ACEArgs.u8PreNoise = u8PreNoise;
    ACEArgs.u8Range = u8Range;
    ACEArgs.u8Range = u8Range;
    ACEArgs.u8DeFlick_Th = u8DeFlick_Th;
    ACEArgs.u8DeFlick_Step = u8DeFlick_Step;
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_GETNOISE_WEIGHT, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

void MApi_XC_ACE_EX_DNR_Init_Motion(XC_ACE_DeviceId *pDeviceId)
{
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_INIT_MOTION, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_DNR_Init_Luma(XC_ACE_DeviceId *pDeviceId)
{
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_INIT_LUMA, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_DNR_Init_Noise(XC_ACE_DeviceId *pDeviceId)
{
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_INIT_NOISE, (void*)NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

MS_U8 MApi_XC_ACE_EX_DNR_GetParam(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, XC_ACE_EX_DNR_Param eParam)
{
    stACE_DNR_GetParam ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.eParam = _ACE_Map_EX_DNRParam(eParam);
    ACEArgs.u8Return = 0;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_GETPARAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
        return 0;
    }
    else
    {
        return  ACEArgs.u8Return;
    }
}

void MApi_XC_ACE_EX_DNR_SetParam(XC_ACE_DeviceId *pDeviceId, MS_BOOL eWindow, XC_ACE_EX_DNR_Param eParam, MS_U16 u16val)
{
    stACE_DNR_SetParam ACEArgs;
    ACEArgs.eWindow = eWindow;
    ACEArgs.eParam = _ACE_Map_EX_DNRParam(eParam);
    ACEArgs.u16val = u16val;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_SETPARAM, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_DNR_SetNRTbl(XC_ACE_DeviceId *pDeviceId, MS_U8 *pu8Tbl)
{
    stACE_DNR_SetNRTbl ACEArgs;
    ACEArgs.pu8Tbl = pu8Tbl;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_DNR_SETNRTBL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_ColorCorrectionTable( XC_ACE_DeviceId *pDeviceId, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable )
{
    stACE_ColorCorrectionTable ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.psColorCorrectionTable = psColorCorrectionTable;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_COLORCORRECTIONTABLE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetColorMatrixControl( XC_ACE_DeviceId *pDeviceId, MS_BOOL bScalerWin, MS_BOOL bEnable )
{
    stACE_SetColorMatrixControl ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.bEnable = bEnable;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETCOLORMATRIXCONTROL, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

void MApi_XC_ACE_EX_SetRBChannelRange( XC_ACE_DeviceId *pDeviceId, MS_BOOL bScalerWin, MS_BOOL bRange )
{
    stACE_SetRBChannelRange ACEArgs;
    ACEArgs.bScalerWin = bScalerWin;
    ACEArgs.bRange = bRange;
    if(UtopiaIoctl(_ACE_SELECT_INSTANCE(pDeviceId->u32Id), E_ACE_CMD_SETRBCHANNELRANGE, (void*)&ACEArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain ACE engine fail\n");
    }
    return;
}

#undef _API_XC_ACE_EX_C_
#endif  // _API_XC_ACE_EX_C_

