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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvAVD.c
/// @brief  AVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
/*#include "MsVersion.h"
#include "drvAVD.h"
#include "MsIRQ.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
//#if !defined (MSOS_TYPE_NOS)
    #include "MsOS.h"
//#endif

// Internal Definition
#include "regCHIP.h"
#include "regAVD.h"
#include "halAVD.h"
#include "drvBDMA.h"
#include "drvSERFLASH.h"*/

#include "drvAVD_v2.h"
#include "drvAVD.h"
#include "../../utopia_core/utopia.h"

#include "UFO.h"
#ifndef DONT_USE_CMA
#include "drvCMAPool.h"
#include "halCHIP.h"
#endif
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  definition
//-------------------------------------------------------------------------------------------------
//#define MODULE_AVD 10


//-------------------------------------------------------------------------------------------------
//  Global variables
//-------------------------------------------------------------------------------------------------

    void* ppAVDInstant=NULL;
    static MS_U32 u32AVDopen=0;
    static MS_U8 u8AVDUtopiaOpen = 0;   //for SetStillImagePara is earlier called than Init
    static AVD_Result bAVD_Open_result=E_AVD_FAIL;
#ifndef DONT_USE_CMA
#ifdef UFO_AVD_CMA
MS_BOOL bAVDGetMA = 0;    //0: not GetMA yet   1: already Get MA
MS_BOOL bAVDCMAInit = 0;
MS_PHY u32COMB_3D_CMA_addr;
MS_U32 u32COMB_3D_CMA_LEN;
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Global VD functions
//-------------------------------------------------------------------------------------------------
#ifndef DONT_USE_CMA
#ifdef UFO_AVD_CMA
struct CMA_Pool_Init_Param AVD_CMA_Pool_Init_PARAM;   // for MApi_CMA_Pool_Init
struct CMA_Pool_Alloc_Param AVD_CMA_Alloc_PARAM;    // for MApi_CMA_Pool_GetMem
struct CMA_Pool_Free_Param AVD_CMA_Free_PARAM;      // for MApi_CMA_Pool_PutMem
#endif
#endif
AVD_Result MDrv_AVD_Init(VD_INITDATA *pVD_InitData, MS_U32 u32InitDataLen)
{
    //AVD_Result _eResult;
    AVD_INIT _eAVD_Init;
    _eAVD_Init.pVD_InitData = *pVD_InitData;
    _eAVD_Init.u32InitDataLen = u32InitDataLen;
    _eAVD_Init.pVD_Result = E_AVD_FAIL;
    void* pAttribte = NULL;
    if(u8AVDUtopiaOpen==0)
    {
        if(UtopiaOpen(MODULE_AVD, &ppAVDInstant, 0, pAttribte)== UTOPIA_STATUS_SUCCESS)
        {
            u32AVDopen = 1;
        }
        u8AVDUtopiaOpen = 1;
    }
    #ifndef DONT_USE_CMA
    #ifdef UFO_AVD_CMA
    /* do CMA Init*/
    AVD_CMA_Pool_Init_PARAM.heap_id = 26;    // need to refine
    AVD_CMA_Pool_Init_PARAM.flags = CMA_FLAG_MAP_VMA;
    if(bAVDCMAInit==0)
    {
        if(MApi_CMA_Pool_Init(&AVD_CMA_Pool_Init_PARAM) == FALSE)
        {
            bAVDCMAInit = 0;
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, CMA_POOL_INIT ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
            return FALSE;
        }
        else
        {
            bAVDCMAInit = 1;
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, get pool_handle_id is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Pool_Init_PARAM.pool_handle_id);
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, get miu is %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Pool_Init_PARAM.miu);
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, get heap_miu_start_offset is 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Pool_Init_PARAM.heap_miu_start_offset);
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, get heap_length is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Pool_Init_PARAM.heap_length);
        }
    }
    /*do Get Mem*/
    if((bAVDCMAInit==1)&&(bAVDGetMA==0))   //CMA already Init, not get mem yet
    {
        AVD_CMA_Alloc_PARAM.pool_handle_id = AVD_CMA_Pool_Init_PARAM.pool_handle_id;
        AVD_CMA_Alloc_PARAM.offset_in_pool = _eAVD_Init.pVD_InitData->u32COMB_3D_ADR - AVD_CMA_Pool_Init_PARAM.heap_miu_start_offset;
        AVD_CMA_Alloc_PARAM.length = _eAVD_Init.pVD_InitData->u32COMB_3D_LEN;
        AVD_CMA_Alloc_PARAM.flags = CMA_FLAG_VIRT_ADDR;
    }
    #endif
    #endif

    //_eResult = (AVD_Result)UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Init, &_eAVD_Init);
    UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Init, &_eAVD_Init);
    bAVD_Open_result=_eAVD_Init.pVD_Result;
    return _eAVD_Init.pVD_Result;
}

void MDrv_AVD_Exit(void)
{
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Exit, NULL);
#ifndef DONT_USE_CMA
#ifdef UFO_AVD_CMA
    /* do CMA Put Mem*/
    if((bAVDCMAInit==1)&&(bAVDGetMA==1))
    {
        if(MApi_CMA_Pool_PutMem(&AVD_CMA_Free_PARAM)==FALSE)
        {
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, CMA_Pool_PutMem ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
            bAVDGetMA = 1;
        }
        else
        {
            bAVDGetMA = 0;
        }
    }
#endif
#endif
    }
}

MS_BOOL MDrv_AVD_McuReset(void)
{
   AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
   {
       UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_ResetMCU, &pArgs);
       return pArgs.bEnable;
   }
    else
        return FALSE;
}

void MDrv_AVD_MCUFreeze(MS_BOOL bEnable)
{
    MS_BOOL *pArgs = &bEnable;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_FreezeMCU, pArgs);
}

MS_U16 MDrv_AVD_Scan_HsyncCheck(MS_U8 u8HtotalTolerance)
{
    AVD_SCANHSYNCCHECK pArgs = {0};
    pArgs.u8HtotalTolerance = u8HtotalTolerance;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Scan_HsyncCheck, &pArgs);
        return pArgs.u16ScanHsyncCheck;
    }
    else
        return 0;
}

void MDrv_AVD_StartAutoStandardDetection(void)
{
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_StartAutoStandardDetection, NULL);
}

MS_BOOL MDrv_AVD_ForceVideoStandard(AVD_VideoStandardType eVideoStandardType)
{
    AVD_FORCEVIDEOSTANDARD pArgs ={0};
    pArgs.eVideoStandardType = eVideoStandardType;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_ForceVideoStandard, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE;
}

void MDrv_AVD_3DCombSpeedup(void)
{
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_3DCombSpeedup, NULL);
}

void MDrv_AVD_LoadDSP(MS_U8 *pu8VD_DSP, MS_U32 len)
{
    AVD_LOADDSP _eAVD_LOADDSP;
    _eAVD_LOADDSP.pu8VD_DSP = pu8VD_DSP;
    _eAVD_LOADDSP.len = len;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_LoadDSP, &_eAVD_LOADDSP);
}

void MDrv_AVD_BackPorchWindowPositon(MS_BOOL bEnable , MS_U8 u8Value)
{
    AVD_BACKPORCHWINPOS _eAVD_BACKPORCHWINPOS;
    _eAVD_BACKPORCHWINPOS.bEnable= bEnable;
    _eAVD_BACKPORCHWINPOS.u8Value = u8Value;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_BackPorchWindowPositon, &_eAVD_BACKPORCHWINPOS);
}

MS_U8 MDrv_AVD_MBX_ReadByteByVDMbox(MS_U8 u8Addr)
{
    AVD_MBXREADBYTEBYVDMBOX pArgs ={0};
    pArgs.u8Addr = u8Addr;

    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_MBX_ReadByteByVDMbox, &pArgs);
        return pArgs.u8Value;
    }
    else
        return FALSE;
}

void MDrv_AVD_SetFlag(MS_U32  u32VDPatchFlag)
{
    MS_U32 *pArgs = &u32VDPatchFlag;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetFlag, pArgs);
}

void MDrv_AVD_SetRegValue(MS_U16 u16Addr,MS_U8 u8Value)
{
    AVD_SETREGVALUE _eAVD_SETREGVALUE;
    _eAVD_SETREGVALUE.u16Addr= u16Addr;
    _eAVD_SETREGVALUE.u8Value = u8Value;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetRegValue, &_eAVD_SETREGVALUE);
}

void MDrv_AVD_SetRegFromDSP(void)
{
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetRegFromDSP, NULL);
}

MS_BOOL MDrv_AVD_SetInput(AVD_InputSourceType eSource, MS_U8 u8ScartFB)
{
    AVD_SETINPUT _eAVD_SETINPUT = {0};
    _eAVD_SETINPUT.eSource= eSource;
    _eAVD_SETINPUT.u8ScartFB = u8ScartFB;
#ifndef DONT_USE_CMA
#ifdef UFO_AVD_CMA
    /*do Get Mem*/
    if((bAVDCMAInit==1)&&(bAVDGetMA==0))   //CMA already Init, not get mem yet
    {
        if(MApi_CMA_Pool_GetMem(&AVD_CMA_Alloc_PARAM) == FALSE)
        {
            bAVDGetMA = 0;
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, CMA_Pool_GetMem ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        }
        else
        {
            bAVDGetMA = 1;
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, get from heap_id %d\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Alloc_PARAM.pool_handle_id);
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, ask offset 0x%llX\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Alloc_PARAM.offset_in_pool);
            ULOGE("AVD","\033[35mFunction = %s, Line = %d, ask length 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Alloc_PARAM.length);
            ULOGE("AVD","\033[37mFunction = %s, Line = %d, return va is 0x%X\033[m\n", __PRETTY_FUNCTION__, __LINE__, AVD_CMA_Alloc_PARAM.virt_addr);

            AVD_CMA_Free_PARAM.pool_handle_id = AVD_CMA_Pool_Init_PARAM.pool_handle_id;
            AVD_CMA_Free_PARAM.offset_in_pool = AVD_CMA_Alloc_PARAM.offset_in_pool;
            AVD_CMA_Free_PARAM.length = AVD_CMA_Alloc_PARAM.length;
        }
    }
#endif
#endif
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetInput, &_eAVD_SETINPUT);
        return _eAVD_SETINPUT.bEnable;
    }
    else
        return FALSE;
}

MS_BOOL MDrv_AVD_SetVideoStandard(AVD_VideoStandardType eStandard, MS_BOOL bIsInAutoTuning)
{
    AVD_SETVIDEOSTANDARD _eAVD_SETVIDEOSTANDARD ={0};
    _eAVD_SETVIDEOSTANDARD.eStandard= eStandard;
    _eAVD_SETVIDEOSTANDARD.bIsInAutoTuning = bIsInAutoTuning;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetVideoStandard, &_eAVD_SETVIDEOSTANDARD);
        return _eAVD_SETVIDEOSTANDARD.bEnable;
    }
    else
        return FALSE;
}

void MDrv_AVD_SetChannelChange(void)
{
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetChannelChange, NULL);
}

void MDrv_AVD_SetHsyncDetectionForTuning(MS_BOOL bEnable)
{
    MS_BOOL *pArgs = &bEnable;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetHsyncDetectionForTuning, pArgs);
}

void MDrv_AVD_Set3dComb(MS_BOOL bEnable)
{
    MS_BOOL *pArgs = &bEnable;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Set3dComb, pArgs);
}

void MDrv_AVD_SetShiftClk(MS_BOOL bEnable,AVD_ATV_CLK_TYPE eShiftMode)
{
    AVD_SETSHIFTCLK _eAVD_SETSHIFTCLK;
    _eAVD_SETSHIFTCLK.bEnable= bEnable;
    _eAVD_SETSHIFTCLK.eShiftMode = eShiftMode;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetShiftClk, &_eAVD_SETSHIFTCLK);
}

void MDrv_AVD_SetFreerunPLL(AVD_VideoFreq eVideoFreq)
{
    AVD_VideoFreq *pArgs = &eVideoFreq;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetFreerunPLL, pArgs);
}

void MDrv_AVD_SetFreerunFreq(AVD_FreeRunFreq eFreerunfreq)
{
    AVD_FreeRunFreq *pArgs = &eFreerunfreq;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetFreerunFreq, pArgs);
}

void MDrv_AVD_SetFactoryPara(AVD_Factory_Para FactoryPara,MS_U8 u8Value)
{
    AVD_SETFACTORYPARA _eAVD_SETFACTORYPARA;
    _eAVD_SETFACTORYPARA.FactoryPara= FactoryPara;
    _eAVD_SETFACTORYPARA.u8Value = u8Value;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetFactoryPara, &_eAVD_SETFACTORYPARA);
}

void MDrv_AVD_Set_Htt_UserMD(MS_U16 u16Htt)
{
    MS_U16 *pArgs = &u16Htt;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Set_Htt_UserMD, pArgs);
}

MS_BOOL MDrv_AVD_SetDbgLevel(AVD_DbgLv u8DbgLevel)
{
    AVD_SETDBGLEVEL pArgs = {0};
    pArgs.u8DbgLevel = u8DbgLevel;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetDbgLevel, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE;
}

void MDrv_AVD_SetPQFineTune(void)
{
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetPQFineTune, NULL);
}

void MDrv_AVD_Set3dCombSpeed(MS_U8 u8COMB57, MS_U8 u8COMB58)
{
    AVD_SET3DCOMBSPEED _eAVD_SET3DCOMBSPEED;
    _eAVD_SET3DCOMBSPEED.u8COMB57= u8COMB57;
    _eAVD_SET3DCOMBSPEED.u8COMB58 = u8COMB58;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Set3dCombSpeed, &_eAVD_SET3DCOMBSPEED);
}

void MDrv_AVD_SetStillImageParam(AVD_Still_Image_Param param)
{

    AVD_Still_Image_Param *pArgs = &param;

    void* pAttribte = NULL;

    if(u8AVDUtopiaOpen==0)
    {
        if(UtopiaOpen(MODULE_AVD, &ppAVDInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
        {
            u32AVDopen =1;
        }
        u8AVDUtopiaOpen = 1;
    }

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetStillImageParam, pArgs);
}

void MDrv_AVD_SetAFECD4Factory(MS_U8 u8Value)
{
    MS_U8 *pArgs = &u8Value;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetAFECD4Factory, pArgs);
}

void MDrv_AVD_Set2D3DPatchOnOff(MS_BOOL bEnable)
{
    MS_BOOL *pArgs = &bEnable;

    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_Set2D3DPatchOnOff, pArgs);
}

MS_U8 MDrv_AVD_SetAutoFineGainToFixed(void)
{
    if(u32AVDopen==1)
    {
#if 0                // Keep the function but do nothing, for compatibility.
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetAutoFineGainToFixed, &pArgs);
        return pArgs->u8Value;
#endif
        return 0;
    }
    else
        return FALSE; // need to check
}

MS_U32 MDrv_AVD_GetFlag(void)
{
    AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetFlag, &pArgs);
        return pArgs.u32Value;
    }
    else
        return FALSE; // need to check
}

MS_U8 MDrv_AVD_GetRegValue(MS_U16 u16Addr)
{
    AVD_GETREGVALUE pArgs ={0};
    pArgs.u16Addr = u16Addr;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetRegValue, &pArgs);
        return pArgs.u8Value;
    }
    else
        return FALSE; // need to check
}

MS_U16 MDrv_AVD_GetStatus(void)
{
    AVD_COPYTOUSER pArgs ={0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetStatus, &pArgs);
        return pArgs.u16Value;
    }
    else
        return FALSE; // need to check
}

MS_U8 MDrv_AVD_GetNoiseMag(void)
{
    AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetNoiseMag, &pArgs);
        return pArgs.u8Value;
    }
    else
        return FALSE; // need to check
}

MS_U16 MDrv_AVD_GetVTotal(void)
{
    AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetVTotal, &pArgs);
        return pArgs.u16Value;
    }
    else
        return FALSE; // need to check
}

AVD_VideoStandardType MDrv_AVD_GetStandardDetection(MS_U16 *u16LatchStatus)
{
    AVD_GETSTANDARDDETECTION pArgs ={0};
    if(u32AVDopen==1)
    {
         UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetStandardDetection, &pArgs);
         *u16LatchStatus = MDrv_AVD_GetStatus();
         return pArgs.VideoStandardType;
    }
    else
        return FALSE; // need to check
}

AVD_VideoStandardType MDrv_AVD_GetLastDetectedStandard(void)
{
    AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
    {
         UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetLastDetectedStandard, &pArgs);
         return pArgs.eVideoStandardType;
    }
    else
        return FALSE; // need to check
}

void MDrv_AVD_GetCaptureWindow(void *stCapWin, AVD_VideoStandardType eVideoStandardType)
{
    AVD_GETCAPTUREWINDOW _eAVD_GETCAPTUREWINDOW;
    _eAVD_GETCAPTUREWINDOW.stCapWin = stCapWin;
    _eAVD_GETCAPTUREWINDOW.eVideoStandardType = eVideoStandardType;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetCaptureWindow, &_eAVD_GETCAPTUREWINDOW);
}

AVD_VideoFreq MDrv_AVD_GetVerticalFreq(void)
{
    AVD_COPYTOUSER pArgs ={0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetVerticalFreq, &pArgs);
        return pArgs.eVideoFreq;
    }
    else
        return FALSE; // need to check
}

MS_U8 MDrv_AVD_GetHsyncEdge(void)
{
    AVD_COPYTOUSER pArgs;
    pArgs.u8Value = 0;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetHsyncEdge, &pArgs);
        return pArgs.u8Value;
    }
    else
        return FALSE; // need to check
}

MS_U8 MDrv_AVD_GetDSPFineGain(void)
{
    AVD_COPYTOUSER pArgs;
    pArgs.u8Value = 0;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetDSPFineGain, &pArgs);
        return pArgs.u8Value;
    }
    else
        return FALSE; // need to check
}

MS_U16 MDrv_AVD_GetDSPVersion(void)
{
    AVD_COPYTOUSER pArgs;
    pArgs.u16Value = 0;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetDSPVersion, &pArgs);
        return pArgs.u16Value;
    }
    else
        return FALSE; // need to check
}

AVD_Result MDrv_AVD_GetLibVer(const MSIF_Version **ppVersion)
{
    AVD_GETLIBVER pArgs = {0};
    pArgs.ppVersion = ppVersion;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetLibVer, &pArgs);
        return pArgs.eAVDResult;
    }
    else
        return FALSE; // need to check
}

const AVD_Info* MDrv_AVD_GetInfo(void)    //MDrv_CMD_AVD_GetInfo,
{
    static AVD_COPYTOUSER spArgs;
    //static AVD_Info sAVD_Info;
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetInfo, &spArgs);
        return &(spArgs.eAVDInfo);
    //return & sAVD_Info;
}

MS_BOOL MDrv_AVD_IsSyncLocked(void)
{
    AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
    {
           UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_IsSyncLocked, &pArgs);
           return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}

MS_BOOL MDrv_AVD_IsSignalInterlaced(void)
{
    AVD_COPYTOUSER pArgs ={0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_IsSignalInterlaced, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}

MS_BOOL MDrv_AVD_IsColorOn(void)
{
    AVD_COPYTOUSER pArgs ={0};
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_IsColorOn, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}

MS_U32 MDrv_AVD_SetPowerState(EN_POWER_MODE u16PowerState)
{
    AVD_SETPOWERSTATE pArgs;
    pArgs.u32Value = 0;
    pArgs.u16PowerState = u16PowerState;

    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetPowerState, &pArgs);
        return pArgs.u32Value;
    }
    else
        return FALSE; // need to check
}

MS_BOOL MDrv_AVD_GetMacroVisionDetect(void)
{
    AVD_COPYTOUSER pArgs;
    pArgs.bEnable = FALSE;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetMacroVisionDetect, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}

MS_BOOL MDrv_AVD_GetCGMSDetect(void)
{
    AVD_COPYTOUSER pArgs;
    pArgs.bEnable = FALSE;
    if(u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_GetCGMSDetect, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}

void SYMBOL_WEAK MDrv_AVD_SetBurstWinStart(MS_U8 u8BusrtStartPosition)
{
    MS_U8*pArgs = &u8BusrtStartPosition;
    if(u32AVDopen==1)
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_SetBurstWinStart, pArgs);
}

MS_BOOL SYMBOL_WEAK MDrv_AVD_AliveCheck(void)
{
    AVD_COPYTOUSER pArgs;
    pArgs.bEnable = FALSE;
    if(bAVD_Open_result==1 && u32AVDopen==1)
    {
        UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_AliveCheck, &pArgs);
        return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}

MS_BOOL SYMBOL_WEAK MDrv_AVD_IsLockAudioCarrier(void)
{
    AVD_COPYTOUSER pArgs = {0};
    if(u32AVDopen==1)
    {
           UtopiaIoctl(ppAVDInstant, MDrv_CMD_AVD_IsLockAudioCarrier, &pArgs);
           return pArgs.bEnable;
    }
    else
        return FALSE; // need to check
}