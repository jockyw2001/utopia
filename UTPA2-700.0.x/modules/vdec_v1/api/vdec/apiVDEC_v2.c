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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiVDEC_v2.c
/// @brief  VDEC API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////



//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"
#include "vdec_private.h"
#include "apiVDEC_v2.h"
#include "apiVDEC.h"
//-------------------------------------------------------------------------------------------------
//  Global Variable
//-------------------------------------------------------------------------------------------------
void* pModuleVDEC_V2 = NULL;
char  ResourceGroup[1][20] = {{"VDEC_R0"},};

//-------------------------------------------------------------------------------------------------
//  API Function
//-------------------------------------------------------------------------------------------------
void VDEC_V2_RegisterToUtopia(void)
{
    MS_U32 j = 0;
    void* psResource = NULL;

    // 1. deal with module
    UtopiaModuleCreate(MODULE_VDEC, 0, &pModuleVDEC_V2);
    UtopiaModuleRegister(pModuleVDEC_V2);
    UtopiaModuleSetupFunctionPtr(pModuleVDEC_V2,
                                (FUtopiaOpen)VDEC_V2_Open,
                                (FUtopiaClose)VDEC_V2_Close,
                                (FUtopiaIOctl)VDEC_V2_Ioctl);

    // 2. deal with resource
    //for(i=E_VDEC_EX_POOL_ID_INTERNAL;i<E_VDEC_EX_POOL_ID_MAX;i++) // for each resource group in different memory pool
    {
        UtopiaModuleAddResourceStart(pModuleVDEC_V2,0);
        for(j=0;j<1;j++) // for each resource group in same memory pool
        {
            UtopiaResourceCreate(ResourceGroup[0],sizeof(VDEC_RESOURCE_PRIVATE),&psResource);
            UtopiaResourceRegister(pModuleVDEC_V2,psResource,0);
        }
        UtopiaModuleAddResourceEnd(pModuleVDEC_V2,0);
    }
}

MS_U32 VDEC_V2_Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    void* pResource = NULL;
    VDEC_RESOURCE_PRIVATE* pVDEC_ResourcePrivate = NULL;
    VDEC_INSTANCE_PRIVATE* psVDEC_InstPri = NULL;
    //MS_U8 i = 0;

    // Check there is available decoder to be use
    if(UtopiaResourceObtain(pModuleVDEC_V2, /*E_VDEC_EX_POOL_ID_INTERNAL*/ 0, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(pResource,(void*)(&pVDEC_ResourcePrivate));


    if(pVDEC_ResourcePrivate->u8DecoderInUse == 0)
    {
        pVDEC_ResourcePrivate->u8DecoderInUse = 1;

    }
    else
    {
        printf("No decoder can be used,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceRelease(pResource);

    //Create instance
    UtopiaInstanceCreate(sizeof(VDEC_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psVDEC_InstPri);

    //Recode de coder index
    //psVDEC_InstPri->Index_of_decoder = 0;

    //Link Function pointer
    //psVDEC_InstPri->fpVDEC_EX_GetFreeStream  = MApi_VDEC_EX_V2_GetFreeStream;
    psVDEC_InstPri->fpVDEC_Init                         = MApi_VDEC_V2_Init;
    psVDEC_InstPri->fpVDEC_Init_EX                      = MApi_VDEC_V2_Init_EX;
    psVDEC_InstPri->fpVDEC_Rst                          = MApi_VDEC_V2_Rst;
    psVDEC_InstPri->fpVDEC_Exit                         = MApi_VDEC_V2_Exit;
    psVDEC_InstPri->fpVDEC_CheckDispInfoRdy             = MApi_VDEC_V2_CheckDispInfoRdy;
    psVDEC_InstPri->fpVDEC_SetFrcMode                   = MApi_VDEC_V2_SetFrcMode;
    psVDEC_InstPri->fpVDEC_SetDynScalingParams          = MApi_VDEC_V2_SetDynScalingParams;
    psVDEC_InstPri->fpVDEC_Play                         = MApi_VDEC_V2_Play;
    psVDEC_InstPri->fpVDEC_Pause                        = MApi_VDEC_V2_Pause;
    psVDEC_InstPri->fpVDEC_Resume                       = MApi_VDEC_V2_Resume;
    psVDEC_InstPri->fpVDEC_StepDisp                     = MApi_VDEC_V2_StepDisp;
    psVDEC_InstPri->fpVDEC_IsStepDispDone               = MApi_VDEC_V2_IsStepDispDone;
    psVDEC_InstPri->fpVDEC_SeekToPTS                    = MApi_VDEC_V2_SeekToPTS;
    psVDEC_InstPri->fpVDEC_SkipToPTS                    = MApi_VDEC_V2_SkipToPTS;
    psVDEC_InstPri->fpVDEC_StepDecode                   = MApi_VDEC_V2_StepDecode;
    psVDEC_InstPri->fpVDEC_IsStepDecodeDone             = MApi_VDEC_V2_IsStepDecodeDone;
    psVDEC_InstPri->fpVDEC_SetTrickMode                 = MApi_VDEC_V2_SetTrickMode;
    psVDEC_InstPri->fpVDEC_PushDecQ                     = MApi_VDEC_V2_PushDecQ;
    psVDEC_InstPri->fpVDEC_FireDecCmd                   = MApi_VDEC_V2_FireDecCmd;
    psVDEC_InstPri->fpVDEC_GetDecQVacancy               = MApi_VDEC_V2_GetDecQVacancy;
    psVDEC_InstPri->fpVDEC_IsCCAvailable                = MApi_VDEC_V2_IsCCAvailable;
    psVDEC_InstPri->fpVDEC_GetCCInfo                    = MApi_VDEC_V2_GetCCInfo;
    psVDEC_InstPri->fpVDEC_Flush                        = MApi_VDEC_V2_Flush;
    psVDEC_InstPri->fpVDEC_GetESWritePtr                = MApi_VDEC_V2_GetESWritePtr;
    psVDEC_InstPri->fpVDEC_GetESReadPtr                 = MApi_VDEC_V2_GetESReadPtr;
    psVDEC_InstPri->fpVDEC_EnableLastFrameShow          = MApi_VDEC_V2_EnableLastFrameShow;
    psVDEC_InstPri->fpVDEC_IsDispFinish                 = MApi_VDEC_V2_IsDispFinish;
    psVDEC_InstPri->fpVDEC_SetSpeed                     = MApi_VDEC_V2_SetSpeed;
    psVDEC_InstPri->fpVDEC_IsFrameRdy                   = MApi_VDEC_V2_IsFrameRdy;
    psVDEC_InstPri->fpVDEC_SetFreezeDisp                = MApi_VDEC_V2_SetFreezeDisp;
    psVDEC_InstPri->fpVDEC_SetBlueScreen                = MApi_VDEC_V2_SetBlueScreen;
    psVDEC_InstPri->fpVDEC_ResetPTS                     = MApi_VDEC_V2_ResetPTS;
    psVDEC_InstPri->fpVDEC_AVSyncOn                     = MApi_VDEC_V2_AVSyncOn;
    psVDEC_InstPri->fpVDEC_SetAVSyncFreerunThreshold    = MApi_VDEC_V2_SetAVSyncFreerunThreshold;
    psVDEC_InstPri->fpVDEC_IsAVSyncOn                   = MApi_VDEC_V2_IsAVSyncOn;
    psVDEC_InstPri->fpVDEC_GetPTS                       = MApi_VDEC_V2_GetPTS;
    psVDEC_InstPri->fpVDEC_GetNextPTS                   = MApi_VDEC_V2_GetNextPTS;
    psVDEC_InstPri->fpVDEC_IsStartSync                  = MApi_VDEC_V2_IsStartSync;
    psVDEC_InstPri->fpVDEC_IsReachSync                  = MApi_VDEC_V2_IsReachSync;
    psVDEC_InstPri->fpVDEC_IsFreerun                    = MApi_VDEC_V2_IsFreerun;
    psVDEC_InstPri->fpVDEC_MhegDecodeIFrame             = MApi_VDEC_V2_MHEG_DecodeIFrame;
    psVDEC_InstPri->fpVDEC_MhegIsIFrameDecoding         = MApi_VDEC_V2_MHEG_IsIFrameDecoding;
    psVDEC_InstPri->fpVDEC_MhegRstIFrameDec             = MApi_VDEC_V2_MHEG_RstIFrameDec;
    psVDEC_InstPri->fpVDEC_GetErrCode                   = MApi_VDEC_V2_GetErrCode;
    psVDEC_InstPri->fpVDEC_GetErrCnt                    = MApi_VDEC_V2_GetErrCnt;
    psVDEC_InstPri->fpVDEC_GetActiveCodecType           = MApi_VDEC_V2_GetActiveCodecType;
    psVDEC_InstPri->fpVDEC_GetBitsRate                  = MApi_VDEC_V2_GetBitsRate;
    psVDEC_InstPri->fpVDEC_Is32PullDown                 = MApi_VDEC_V2_Is32PullDown;
    psVDEC_InstPri->fpVDEC_IsAlive                      = MApi_VDEC_V2_IsAlive;
    psVDEC_InstPri->fpVDEC_GetVideoPtsStcDelta          = MApi_VDEC_V2_GetVideoPtsStcDelta;
    psVDEC_InstPri->fpVDEC_IsWithValidStream            = MApi_VDEC_V2_IsWithValidStream;
    psVDEC_InstPri->fpVDEC_IsIFrameFound                = MApi_VDEC_V2_IsIFrameFound;
    psVDEC_InstPri->fpVDEC_IsWithLowDelay               = MApi_VDEC_V2_IsWithLowDelay;
    psVDEC_InstPri->fpVDEC_IsAllBufferEmpty             = MApi_VDEC_V2_IsAllBufferEmpty;
    psVDEC_InstPri->fpVDEC_GetGOPCnt                    = MApi_VDEC_V2_GetGOPCnt;
    psVDEC_InstPri->fpVDEC_GetFrameCnt                  = MApi_VDEC_V2_GetFrameCnt;
    psVDEC_InstPri->fpVDEC_GetSkipCnt                   = MApi_VDEC_V2_GetSkipCnt;
    psVDEC_InstPri->fpVDEC_GetDropCnt                   = MApi_VDEC_V2_GetDropCnt;
    psVDEC_InstPri->fpVDEC_GetDispInfo                  = MApi_VDEC_V2_GetDispInfo;
    psVDEC_InstPri->fpVDEC_GetTrickMode                 = MApi_VDEC_V2_GetTrickMode;
    psVDEC_InstPri->fpVDEC_GetActiveFormat              = MApi_VDEC_V2_GetActiveFormat;
    psVDEC_InstPri->fpVDEC_GetColourPrimaries           = MApi_VDEC_V2_GetColourPrimaries;
    psVDEC_InstPri->fpVDEC_GetHWKey                     = MApi_VDEC_V2_GetHWKey;
    psVDEC_InstPri->fpVDEC_IsSeqChg                     = MApi_VDEC_V2_IsSeqChg;
    psVDEC_InstPri->fpVDEC_SetDbgLevel                  = MApi_VDEC_V2_SetDbgLevel;
    psVDEC_InstPri->fpVDEC_GetDecFrameInfo              = MApi_VDEC_V2_GetDecFrameInfo;
    psVDEC_InstPri->fpVDEC_GetDispFrameInfo             = MApi_VDEC_V2_GetDispFrameInfo;
    psVDEC_InstPri->fpVDEC_GetExtDispInfo               = MApi_VDEC_V2_GetExtDispInfo;
    psVDEC_InstPri->fpVDEC_GetDecTimeCode               = MApi_VDEC_V2_GetDecTimeCode;
    psVDEC_InstPri->fpVDEC_GetDispTimeCode              = MApi_VDEC_V2_GetDispTimeCode;


    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 VDEC_V2_Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    VDEC_INSTANCE_PRIVATE* psVDEC_InstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psVDEC_InstPri);
    E_VDEC_V2_IOCTL_CMD IOcmd = (E_VDEC_V2_IOCTL_CMD)u32Cmd;
    MS_U32 ret = UTOPIA_STATUS_FAIL;
    VDEC_Result *pRet = (((VDEC_V2_IO_Param *)pArgs)->pRet);
    *pRet = E_VDEC_FAIL;

    switch(IOcmd)
    {
        case E_VDEC_V2_CMD_GETLIBVER:
                *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_GetLibVer((const MSIF_Version **)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
                ret = UTOPIA_STATUS_SUCCESS;
                break;

        case E_VDEC_V2_CMD_GETINFO:
                *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_GetInfo((const VDEC_Info **)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
                ret = UTOPIA_STATUS_SUCCESS;
                break; 

        case E_VDEC_V2_CMD_GETSTATUS:
                *((MS_BOOL *)pRet) = psVDEC_InstPri->fpVDEC_GetStatus((VDEC_Status*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
                ret = UTOPIA_STATUS_SUCCESS;
                break;    
                
        case E_VDEC_V2_CMD_CHECKCAPS:
                *((MS_BOOL *)pRet) = psVDEC_InstPri->fpVDEC_CheckCaps(*((VDEC_CodecType*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
                ret = UTOPIA_STATUS_SUCCESS;
                break;    

        case E_VDEC_V2_CMD_ENABLETUEBOMODE:
                *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_EnableTurboMode(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
                ret = UTOPIA_STATUS_SUCCESS;
                break;    

        case E_VDEC_V2_CMD_INIT:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_Init((VDEC_InitParam*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_INIT_EX:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_Init_EX((void*)(((VDEC_V2_IO_Param *)pArgs)->param[0]),
                                                                    *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_RST:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_Rst();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_EXIT:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_Exit(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CHECK_DISP_INFO_RDY:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CheckDispInfoRdy();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_FRC_MODE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetFrcMode(*((VDEC_FrcMode*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_DYNSCALING_PARAMS:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetDynScalingParams(*((MS_PHYADDR*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                          *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_PLAY:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_Play();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_PAUSE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_Pause();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_RESUME:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_Resume();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_STEP_DISP:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_StepDisp();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_STEP_DISP_DONE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsStepDispDone();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SEEK_TO_PTS:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SeekToPTS(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SKIP_TO_PTS:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SkipToPTS(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_STEP_DECODE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_StepDecode();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_STEP_DECODE_DONE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsStepDecodeDone();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_TRICK_MODE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetTrickMode(*((VDEC_TrickDec*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_PUSH_DECQ:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_PushDecQ(((VDEC_DecCmd*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_FIRE_DEC_CMD:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_FireDecCmd();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DECQ_VACANCY:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetDecQVacancy();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_CC_AVAILABLE:

            *((MS_BOOL *)pRet)  = psVDEC_InstPri->fpVDEC_IsCCAvailable();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_CC_INFO:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_GetCCInfo(((void *)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                 (*((MS_U32 *)(((VDEC_V2_IO_Param *)pArgs)->param[1]))));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_FLUSH:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_Flush(*((VDEC_FreezePicSelect *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_ES_WRITE_PTR:

            *((MS_U32 *)pRet) = psVDEC_InstPri->fpVDEC_GetESWritePtr();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_ES_READ_PTR:

            *((MS_U32 *)pRet) = psVDEC_InstPri->fpVDEC_GetESReadPtr();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_ENABLE_LAST_FRAME_SHOW:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_EnableLastFrameShow(*((MS_BOOL *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_DISP_FINISH:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_IsDispFinish();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_SPEED:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_SetSpeed(*((VDEC_SpeedType *)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                    *((VDEC_DispSpeed *)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_FRAME_RDY:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_IsFrameRdy();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_FREEZE_DISP:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_SetFreezeDisp(*((MS_BOOL *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_BLUE_SCREEN:

            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_SetBlueScreen(*((MS_BOOL *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_RESET_PTS:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_ResetPTS(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_AVSYNC_ON:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_AVSyncOn(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                    *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                    *((MS_U16*)(((VDEC_V2_IO_Param *)pArgs)->param[2])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_AVSYNC_FREERUN_THRESHOLD:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetAVSyncFreerunThreshold(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_AVSYNC_ON:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsAVSyncOn();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_PTS:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetPTS();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_NEXT_PTS:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetNextPTS();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_START_SYNC:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsStartSync();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_REACH_SYNC:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsReachSync();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_FREERUN:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsFreerun();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_MHEG_DECODE_I_FRAME:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_MhegDecodeIFrame(*((MS_PHYADDR*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                    *((MS_PHYADDR*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                    *((MS_PHYADDR*)(((VDEC_V2_IO_Param *)pArgs)->param[2])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_MHEG_IS_I_FRAME_DECODING:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_MhegIsIFrameDecoding();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_MHEG_RST_I_FRAME_DEC:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_MhegRstIFrameDec();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_ERR_CODE:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetErrCode();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_ERR_CNT:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetErrCnt();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_ACTIVE_CODEC_TYPE:

            *((VDEC_CodecType *)pRet)  = psVDEC_InstPri->fpVDEC_GetActiveCodecType();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_BITS_RATE:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetBitsRate();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_32PULLDOWN:

            *((MS_BOOL *)pRet)  = psVDEC_InstPri->fpVDEC_Is32PullDown();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_ALIVE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsAlive();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_VIDEO_PTS_STC_DELTA:

            *((MS_S64 *)pRet)  = psVDEC_InstPri->fpVDEC_GetVideoPtsStcDelta();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_WITH_VALID_STREAM:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsWithValidStream();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_I_FRAME_FOUND:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsIFrameFound();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_WITH_LOW_DELAY:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsWithLowDelay();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_ALL_BUFFER_EMPTY:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsAllBufferEmpty();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_GOP_CNT:

            *((MS_U8 *)pRet)  = psVDEC_InstPri->fpVDEC_GetGOPCnt();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_FRAME_CNT:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetFrameCnt();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_SKIP_CNT:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetSkipCnt();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DROP_CNT:

            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetDropCnt();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DISP_INFO:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetDispInfo((VDEC_DispInfo *)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_TRICK_MODE:

            *((VDEC_TrickDec *)pRet)  = psVDEC_InstPri->fpVDEC_GetTrickMode();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_ACTIVE_FORMAT:

            *((MS_U8 *)pRet)  = psVDEC_InstPri->fpVDEC_GetActiveFormat();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_COLOUR_PRIMARIES:

            *((MS_U8 *)pRet)  = psVDEC_InstPri->fpVDEC_GetColourPrimaries();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_HW_KEY:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetHWKey(((MS_U8*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_IS_SEQ_CHG:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_IsSeqChg();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SET_DBG_LEVEL:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetDbgLevel(*((VDEC_DbgLevel *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DEC_FRAME_INFO:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetDecFrameInfo(((VDEC_FrameInfo *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DISP_FRAME_INFO:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetDispFrameInfo(((VDEC_FrameInfo *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_EXT_DISP_INFO:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetExtDispInfo(((VDEC_ExtDispInfo *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DEC_TIME_CODE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetDecTimeCode(((VDEC_TimeCode *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GET_DISP_TIME_CODE:

            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetDispTimeCode(((VDEC_TimeCode *)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_STARTPARSING:
            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_CC_StartParsing((VDEC_CCCfg*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_STOPPARSING:
            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_CC_StopParsing((VDEC_CCCfg*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETWRITEPTR:
            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_CC_GetWritePtr((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETREADPTR:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetReadPtr((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_UPDATEREADPTR:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_UpdateReadPtr(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETISOVERFLOW:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetIsOverflow((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SETEVENT:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetEvent(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                      *((VDEC_EventCb*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                      (void*)(((VDEC_V2_IO_Param *)pArgs)->param[2]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_UNSETEVENT:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_UnsetEvent(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SETENVENT_EX:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetEvent_EX(*((VDEC_CB_TYPE*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                      *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                      *((VDEC_EventCb*)(((VDEC_V2_IO_Param *)pArgs)->param[2])),
                                                                      (void*)(((VDEC_V2_IO_Param *)pArgs)->param[3]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_UNSETEVENT_EX:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_UnsetEvent_EX(*((VDEC_CB_TYPE*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                      *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETEVENTINFO:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetEventInfo((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GENPATTERN:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GenPattern(*((VDEC_PatternType*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                        *((MS_PHYADDR*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                        (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[2]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GENPATTERNLEASTLENGTH:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetPatternLeastLength(*((VDEC_PatternType*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_DISABLEDEBLOCKING:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_DisableDeblocking(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_DISABLEQUARTERPIXEL:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_DisableQuarterPixel(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SETAUTORMLSTZEROBYTE:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetAutoRmLstZeroByte(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SETBALANCEBW:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetBalanceBW(*((MS_U8*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                          *((MS_U8*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                          *((MS_U8*)(((VDEC_V2_IO_Param *)pArgs)->param[2])),
                                                                          *((MS_U8*)(((VDEC_V2_IO_Param *)pArgs)->param[3])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_DBGDUMPSTATUS:
            *((VDEC_Result *)pRet) = psVDEC_InstPri->fpVDEC_DbgDumpStatus();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SETCONTROL:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SetControl(*((VDEC_User_Cmd*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                        *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETCONTROL:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetControl(*((VDEC_User_Cmd*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                        *((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETDISPCNT:
            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetDispCnt();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETFWVERSION:
            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetFwVersion(*((VDEC_FwType*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));;
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SETBLOCKDISPLAY:
            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_SetBlockDisplay(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_ENABLEESBUFFMALLOC:
            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_EnableESBuffMalloc(*((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETESBUFFVACANCY:
            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_GetESBuffVacancy((void*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETESBUFF:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetESBuff(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                   (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1]),
                                                                   (MS_PHYADDR*)(((VDEC_V2_IO_Param *)pArgs)->param[2]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_GETNEXTDISPFRAME:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_GetNextDispFrame((VDEC_DispFrame**)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_DISPLAYFRAME:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_DisplayFrame((VDEC_DispFrame*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_RELEASEFRAME:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_ReleaseFrame((VDEC_DispFrame*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_FLUSHPTSBUF:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_FlushPTSBuf();
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CAPTUREFRAME:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CaptureFrame(*((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                          *((MS_BOOL*)(((VDEC_V2_IO_Param *)pArgs)->param[1])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_INIT:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_Init((MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[0]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_SETCFG:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_SetCfg(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                       *((VDEC_CCType*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                        (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[2]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_SETBUFFSTARTADD:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_SetBuffStartAdd(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                            (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_UPDATEWRITEADD:
            *((MS_U32 *)pRet)  = psVDEC_InstPri->fpVDEC_CC_UpdateWriteAdd(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                           (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_UPDATEREADADD:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_UpdateReadAdd(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                          (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[1]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_DISABLEPARSING:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_DisableParsing(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETINFO:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetInfo(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                        *((VDEC_CCInfoCmd*)(((VDEC_V2_IO_Param *)pArgs)->param[1])),
                                                                         (MS_U32*)(((VDEC_V2_IO_Param *)pArgs)->param[2]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETISRSTDONE:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetIsRstDone(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETISBUFFOVERFLOW:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetIsBuffOverflow(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETWRITEADD:
            *((MS_PHYADDR *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetWriteAdd(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_CC_GETREADADD:
            *((MS_PHYADDR *)pRet)  = psVDEC_InstPri->fpVDEC_CC_GetReadAdd(*((VDEC_CCFormat*)(((VDEC_V2_IO_Param *)pArgs)->param[0])));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_SYSTEMPRESETCONTROL:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_SystemPreSetControl(*((VDEC_User_Cmd*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                                  (void*)(((VDEC_V2_IO_Param *)pArgs)->param[1]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        case E_VDEC_V2_CMD_PRESETCONTROL:
            *((VDEC_Result *)pRet)  = psVDEC_InstPri->fpVDEC_PreSetControl(*((VDEC_User_Cmd*)(((VDEC_V2_IO_Param *)pArgs)->param[0])),
                                                                           (MS_U32 **)(((VDEC_V2_IO_Param *)pArgs)->param[1]));
            ret = UTOPIA_STATUS_SUCCESS;
            break;

        default:
            printf("Error Command = %ld\n",u32Cmd);
            ret = UTOPIA_STATUS_NOT_SUPPORTED;
            break;
    }
    if(*pRet != E_VDEC_OK)
    {
        ret =  UTOPIA_STATUS_FAIL;
    }
    return ret;
}

MS_U32 VDEC_V2_Close(void* pInstance)
{
    void* pResource = NULL;
    VDEC_RESOURCE_PRIVATE* pVDEC_ResourcePrivate = NULL;
    VDEC_INSTANCE_PRIVATE* psVDEC_InstPri = NULL;

    // Get instance private
    UtopiaInstanceGetPrivate(pInstance, (void**)&psVDEC_InstPri);

    // Get resource private
    if(UtopiaResourceObtain(pModuleVDEC_V2, 0, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(pResource,(void*)(&pVDEC_ResourcePrivate));

    // Set using decoder index to 0
    pVDEC_ResourcePrivate->u8DecoderInUse = 0;

    UtopiaResourceRelease(pResource);

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}




