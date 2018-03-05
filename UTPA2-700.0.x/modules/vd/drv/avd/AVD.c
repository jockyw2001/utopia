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
/// @file   AVD.c
/// @brief  AVD Driver
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
//#include <string.h>
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
//#if !defined (MSOS_TYPE_NOS)
    #include "MsOS.h"
//#endif

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#endif
#include "drvAVD_priv.h"
#include "drvAVD_v2.h"
#include "../../utopia_core/utopia_dapi.h"
#include "../../utopia_core/utopia.h"
#include "../../utopia_core/utopia_driver_id.h"

// Internal Definition
#include "regCHIP.h"
#include "regAVD.h"
#include "halAVD.h"
#include "drvBDMA.h"
//#include "drvSERFLASH.h"
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define TEST_VD_DSP                     0               // 1: for TEST VD DSP, 0: for MSTAR VD DSP
#define ADJUST_CHANNEL_CHANGE_HSYNC 1
#define ADJUST_NTSC_BURST_WINDOW 1
#define AVD_2K_STR 1
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//avd mutex wait time
#define AVD_MUTEX_WAIT_TIME    1000

#if (TEST_VD_DSP)
#undef  VD_STATUS_RDY
#define VD_STATUS_RDY                   (BIT(8))
#define VD_STANDARD_VALID               BIT(0)
#endif

//#define MODULE_AVD 10  //weicheng 201303 multi process safe

#define AVD_debug(x) //x
#define AVD_debug_Monitor(x) //x

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define VDDBG(x)          (x)
//#define AVD_DRV_DEBUG 1
#define AVD_LOCK(_s32AVD_Mutex,_u8AVDDbgLevel)      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_u8AVDDbgLevel == AVD_DBGLV_DEBUG) ULOGE("AVD","%s lock\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32AVD_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)
#define AVD_UNLOCK(_s32AVD_Mutex,_u8AVDDbgLevel)      \
    do{                         \
        MsOS_ReleaseMutex(_s32AVD_Mutex);\
        if (_u8AVDDbgLevel == AVD_DBGLV_DEBUG) ULOGE("AVD","%s unlock\n", __FUNCTION__); \
        }while(0)
#else
#define VDDBG(x)          //(x)
//#define AVD_DRV_DEBUG 0
#define AVD_LOCK(_s32AVD_Mutex,_u8AVDDbgLevel)      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        MsOS_ObtainMutex(_s32AVD_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)
#define AVD_UNLOCK(_s32AVD_Mutex,_u8AVDDbgLevel)      \
    do{                         \
        MsOS_ReleaseMutex(_s32AVD_Mutex);\
        }while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Global Constant
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_avd_version = {
    .MW = { AVD_VER, },
};
static MS_PHY u32COMB_3D_Addr_Str = 0;
static MS_U32 u32COMB_3D_Len_Str = 0;
static AVD_InputSourceType eSource_Str = E_INPUT_SOURCE_INVALID;
static MS_U8 u8ScartFB_Str = 1;
static MS_U32 u32XTAL_Clock_Str = 12000000UL;
static AVD_DemodType eDemodType_Str = DEMODE_MSTAR_VIF;
static MS_U8 *pu8VD_DSP_Code_Address_Str;
static MS_U32 u32VD_DSP_Code_Len_Str;
static MS_BOOL bInitSTR=0;

#if defined(MSOS_TYPE_LINUX_KERNEL)
static AVD_VideoStandardType eVideoStandard_Str=E_VIDEOSTANDARD_NOTSTANDARD;
#endif

MS_U16 _u16HtotalNTSC[]={(1135L*3/2), (1135L), (910L), (1135L)};
MS_U16 _u16HtotalPAL[]={(1135L*3/2), (1135L), (1135L), (1135L)};
MS_U16 _u16HtotalSECAM[]={(1135L*3/2), (1135L), (1097L), (1135L)};
MS_U16 _u16HtotalNTSC_443[]={(1135L*3/2), (1135L), (1127L), (1135L)};
MS_U16 _u16HtotalPAL_M[]={(1135L*3/2), (1135L), (909L), (1135L)};
MS_U16 _u16HtotalPAL_NC[]={(1135L*3/2), (1135L), (917L), (1135L)};
MS_U16 _u16HtotalPAL_60[]={(1135L*3/2), (1135L), (1127L), (1135L)};

//-------------------------------------------------------------------------------------------------
//  Enum
//-------------------------------------------------------------------------------------------------

enum
{
    AVD_POOL_ID_AVD = 0,
} eAVDPoolID;

//-------------------------------------------------------------------------------------------------
//  Interface function
//-------------------------------------------------------------------------------------------------
static void vd_str_store_reg(void);
static void vd_str_store_comb_sub_reg(void);
static void vd_str_init(void);
static void vdmcu_str_init(void);
static void vd_str_setinput(void);
static void vd_str_load_reg(void);
static void vd_str_load_comb_sub_reg(void);

#if defined(MSOS_TYPE_LINUX_KERNEL)
static AVD_VideoStandardType vd_str_timingchangedetection(void);
#endif

void AVDRegisterToUtopia(void)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_AVD, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)AVDOpen, (FUtopiaClose)AVDClose, (FUtopiaIOctl)AVDIoctl);

   // Utopia2K STR
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)AVDStr);
    //UtopiaModuleSetSTRPrivate(pUtopiaModule, STRPrivateDataSize);
#endif

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, AVD_POOL_ID_AVD);
    UtopiaResourceCreate("AVD", sizeof(AVD_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, AVD_POOL_ID_AVD);
    UtopiaModuleAddResourceEnd(pUtopiaModule, AVD_POOL_ID_AVD);

}

MS_U32 AVDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    ULOGE("AVD","\n[AVD INFO]AVD open \n");

    AVD_INSTANT_PRIVATE *pAvdPri = NULL;
    UtopiaInstanceCreate(sizeof(AVD_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void*)&pAvdPri);

    pAvdPri->fpAVDInit = Drv_AVD_Init;
    pAvdPri->fpAVDExit = Drv_AVD_Exit;
    pAvdPri->fpAVDResetMCU = Drv_AVD_ResetMCU;
    pAvdPri->fpAVDFreezeMCU = Drv_AVD_FreezeMCU;
    pAvdPri->fpAVDScanHsyncCheck = Drv_AVD_Scan_HsyncCheck;
    pAvdPri->fpAVDStartAutoStandardDetection = Drv_AVD_StartAutoStandardDetection;
    pAvdPri->fpAVDForceVideoStandard = Drv_AVD_ForceVideoStandard;
    pAvdPri->fpAVD3DCombSpeedup = Drv_AVD_3DCombSpeedup;
    pAvdPri->fpAVDLoadDSP = Drv_AVD_LoadDSP;
    pAvdPri->fpAVDBackPorchWindowPositon = Drv_AVD_BackPorchWindowPositon;
    pAvdPri->fpAVDMBX_ReadByteByVDMbox = Drv_AVD_MBX_ReadByteByVDMbox;

    pAvdPri->fpAVDSetFlag = Drv_AVD_SetFlag;
    pAvdPri->fpAVDSetRegValue = Drv_AVD_SetRegValue;
    pAvdPri->fpAVDSetRegFromDSP = Drv_AVD_SetRegFromDSP;
    pAvdPri->fpAVDSetInput = Drv_AVD_SetInput;
    pAvdPri->fpAVDSetVideoStandard = Drv_AVD_SetVideoStandard;
    pAvdPri->fpAVDSetChannelChange = Drv_AVD_SetChannelChange;
    pAvdPri->fpAVDSetHsyncDetectionForTuning = Drv_AVD_SetHsyncDetectionForTuning;
    pAvdPri->fpAVDSet3dComb = Drv_AVD_Set3dComb;
    pAvdPri->fpAVDSetShiftClk = Drv_AVD_SetShiftClk;
    pAvdPri->fpAVDSetFreerunPLL = Drv_AVD_SetFreerunPLL;
    pAvdPri->fpAVDSetFreerunFreq = Drv_AVD_SetFreerunFreq;
    pAvdPri->fpAVDSetFactoryPara = Drv_AVD_SetFactoryPara;
    pAvdPri->fpAVDSetDbgLevel = Drv_AVD_SetDbgLevel;
    pAvdPri->fpAVDSetPQFineTune = Drv_AVD_SetPQFineTune;
    pAvdPri->fpAVDSet3dCombSpeed = Drv_AVD_Set3dCombSpeed;
    pAvdPri->fpAVDSetStillImageParam = Drv_AVD_SetStillImageParam;
    pAvdPri->fpAVDSet2D3DPatchOnOff = Drv_AVD_Set2D3DPatchOnOff;
    pAvdPri->fpAVDSetAutoFineGainToFixed = Drv_AVD_SetAutoFineGainToFixed;

    pAvdPri->fpAVDGetRegValue = Drv_AVD_GetRegValue;
    pAvdPri->fpAVDGetStatus = Drv_AVD_GetStatus;
    pAvdPri->fpAVDGetNoiseMag = Drv_AVD_GetNoiseMag;
    pAvdPri->fpAVDGetVTotal = Drv_AVD_GetVTotal;
    pAvdPri->fpAVDGetStandardDetection = Drv_AVD_GetStandardDetection;
    pAvdPri->fpAVDGetCaptureWindow = Drv_AVD_GetCaptureWindow;
    pAvdPri->fpAVDGetVerticalFreq = Drv_AVD_GetVerticalFreq;
    pAvdPri->fpAVDGetHsyncEdge = Drv_AVD_GetHsyncEdge;
    pAvdPri->fpAVDGetDSPFineGain = Drv_AVD_GetDSPFineGain;
    pAvdPri->fpAVDGetDSPVersion = Drv_AVD_GetDSPVersion;
    pAvdPri->fpAVDGetLibVer = Drv_AVD_GetLibVer;
    pAvdPri->fpAVDGetInfo = Drv_AVD_GetInfo;
    pAvdPri->fpAVDIsSyncLocked = Drv_AVD_IsSyncLocked;
    pAvdPri->fpAVDIsSignalInterlaced = Drv_AVD_IsSignalInterlaced;
    pAvdPri->fpAVDIsColorOn = Drv_AVD_IsColorOn;
    pAvdPri->fpAVDSetPowerState = Drv_AVD_SetPowerState;
    pAvdPri->fpAVDGetMacroVisionDetect = Drv_AVD_GetMacroVisionDetect;
    pAvdPri->fpAVDGetCGMSDetect = Drv_AVD_GetCGMSDetect;
    pAvdPri->fpAVDSetBurstWinStart =Drv_AVD_SetBurstWinStart;
    pAvdPri->fpAVDIsAVDAlive = Drv_AVD_IsAVDAlive;
    pAvdPri->fpAVDIsLockAudioCarrier =Drv_AVD_IsLockAudioCarrier;
    //return TRUE;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 AVDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    MS_BOOL bEnable = 0;
    //MS_U8 u8Value;
    //MS_U16 u16Value;
    MS_U32 u32Value;
    MS_U32 u32Ret = 0;
    //MS_U16 u16PowerState=0;

    PAVD_INIT pINITParam = NULL;
    PAVD_LOADDSP pLoadDSPParam = NULL;
    PAVD_BACKPORCHWINPOS pBackPorchWinPos = NULL;
    PAVD_SETREGVALUE pSetRegValue = NULL;
    PAVD_SETINPUT pSetInput = NULL;
    PAVD_SETVIDEOSTANDARD pSetVideoStandard = NULL;
    PAVD_SETSHIFTCLK pSetShiftCLK = NULL;
    PAVD_SETFACTORYPARA pSetFactorypara = NULL;
    PAVD_SET3DCOMBSPEED pSet3DCOMBSpeed = NULL;
    PAVD_GETCAPTUREWINDOW pGetCaptureWindow = NULL;
    PAVD_GETSTANDARDDETECTION pStandardDetection = NULL;
    PAVD_SCANHSYNCCHECK pScanHsyncCheck = NULL;
    PAVD_FORCEVIDEOSTANDARD pForceVideoStandard = NULL;
    PAVD_MBXREADBYTEBYVDMBOX pMbxReadByteByVDMbox = NULL;
    PAVD_SETDBGLEVEL pSetDbgLevel = NULL;
    PAVD_COPYTOUSER pCopyToUser = NULL;
    PAVD_GETREGVALUE pGetRegValue = NULL;
    PAVD_SETPOWERSTATE pSetPowerState = NULL;
    PAVD_GETLIBVER pGetLibVer = NULL;

    void* pModule = NULL;
    AVD_INSTANT_PRIVATE* psAvdInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psAvdInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);

    void* pResource = NULL;
    AVD_RESOURCE_PRIVATE* psAvdResPri = NULL;
    if(UtopiaResourceObtain(pModule, AVD_POOL_ID_AVD , &pResource) != 0)
    {
        ULOGE("AVD","UtopiaResourceObtainToInstant fail\n");
        return 0xFFFFFFFF;
    }

    if(UtopiaResourceGetPrivate(pResource,(void*)&psAvdResPri) != 0)
    {
        ULOGE("AVD","UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    switch(u32Cmd)
    {
        case MDrv_CMD_AVD_Init:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Init\n"));
            //---------------------------- init global variables-----------------------------------------
            psAvdResPri->_bShiftClkFlag = 0;
            psAvdResPri->_b2d3dautoflag = 1;
            psAvdResPri->_bSTRFlag = 0;

            psAvdResPri->_u8HtotalDebounce = 0;
            psAvdResPri->_u8AutoDetMode = FSC_AUTO_DET_ENABLE;
            psAvdResPri->_u8AfecD4Factory = 0;
            psAvdResPri->_u8Comb10Bit3Flag = 1;
            psAvdResPri->_u8Comb57 = 0x04;
            psAvdResPri->_u8Comb58 = 0x01;
            psAvdResPri->_u8Comb5F = 0x08;
            psAvdResPri->_u8SCARTSwitch = 0;  // 0: CVBS, 1:Svideo;
            psAvdResPri->_u8SCARTPrestandard = 0;
            psAvdResPri->_u8AutoTuningIsProgress = 0;

            psAvdResPri->_u16CurVDStatus = 0;
            psAvdResPri->_u16DataH[0]=0;
            psAvdResPri->_u16DataH[1]=0;
            psAvdResPri->_u16DataH[2]=0;
            psAvdResPri->_u16LatchH = 1135L;
            psAvdResPri->_u16Htt_UserMD = 1135;
            psAvdResPri->_u16DPL_LSB = 0;
            psAvdResPri->_u16DPL_MSB = 0;

            psAvdResPri->u32VDPatchFlagStatus = 0;
            psAvdResPri->_u32VideoSystemTimer = 0;
            psAvdResPri->_u32SCARTWaitTime = 0;

            psAvdResPri->_eVideoSystem = E_VIDEOSTANDARD_PAL_BGHI;
            psAvdResPri->_eForceVideoStandard = E_VIDEOSTANDARD_AUTO;
            psAvdResPri->_eLastStandard = E_VIDEOSTANDARD_NOTSTANDARD;
            psAvdResPri->_eVDInputSource = E_INPUT_SOURCE_ATV;
            psAvdResPri->gShiftMode = E_ATV_CLK_ORIGIN_43P2MHZ;
            //------------------------------------ VD init function------------------------------------------
            pINITParam = (PAVD_INIT)pArgs;
            u32Ret = (psAvdInstPri->fpAVDInit)(&(pINITParam->pVD_InitData), pINITParam->u32InitDataLen, psAvdResPri);
            pINITParam->pVD_Result = (AVD_Result)u32Ret;
            break;
        case MDrv_CMD_AVD_Exit:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Init\n"));
            (psAvdInstPri->fpAVDExit)(psAvdResPri);
            break;
        case MDrv_CMD_AVD_ResetMCU:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_ResetMCU\n"));
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDResetMCU)(psAvdResPri);
            pCopyToUser->bEnable = (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_FreezeMCU:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_FreezeMCU: %d\n", *(MS_BOOL *)pArgs));
            bEnable = *(MS_BOOL *)pArgs;
            (psAvdInstPri->fpAVDFreezeMCU)(bEnable, psAvdResPri);
            break;
        case MDrv_CMD_AVD_Scan_HsyncCheck:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Scan_HsyncCheck\n"));
            pScanHsyncCheck = (PAVD_SCANHSYNCCHECK)pArgs;
            u32Ret = (psAvdInstPri->fpAVDScanHsyncCheck)(pScanHsyncCheck->u8HtotalTolerance, psAvdResPri);
            pScanHsyncCheck->u16ScanHsyncCheck = (MS_U16)u32Ret;
            break;
        case MDrv_CMD_AVD_StartAutoStandardDetection:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_StartAutoStandardDetection\n"));
            (psAvdInstPri->fpAVDStartAutoStandardDetection)(psAvdResPri);
            break;
        case MDrv_CMD_AVD_ForceVideoStandard:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_ForceVideoStandard: Force standard=%d\n",*(AVD_VideoStandardType *)pArgs));
            pForceVideoStandard = (PAVD_FORCEVIDEOSTANDARD)pArgs;
            u32Ret =(psAvdInstPri->fpAVDForceVideoStandard)(pForceVideoStandard->eVideoStandardType, psAvdResPri);
            pForceVideoStandard->bEnable = (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_3DCombSpeedup:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_3DCombSpeedup\n"));
            (psAvdInstPri->fpAVD3DCombSpeedup)(psAvdResPri);
            break;
        case MDrv_CMD_AVD_LoadDSP:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_LoadDSP \n"));
            pLoadDSPParam = (PAVD_LOADDSP)pArgs;
            (psAvdInstPri->fpAVDLoadDSP)(pLoadDSPParam->pu8VD_DSP, pLoadDSPParam->len, psAvdResPri);
            break;
        case MDrv_CMD_AVD_BackPorchWindowPositon:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_BackPorchWindowPositon\n"));
            pBackPorchWinPos= (PAVD_BACKPORCHWINPOS)pArgs;
            (psAvdInstPri->fpAVDBackPorchWindowPositon)(pBackPorchWinPos->bEnable,pBackPorchWinPos->u8Value);
            break;
        case MDrv_CMD_AVD_MBX_ReadByteByVDMbox:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_MBX_ReadByteByVDMbox\n"));
            pMbxReadByteByVDMbox = (PAVD_MBXREADBYTEBYVDMBOX)pArgs;
            u32Ret = (psAvdInstPri->fpAVDMBX_ReadByteByVDMbox)(pMbxReadByteByVDMbox->u8Addr, psAvdResPri);
            pMbxReadByteByVDMbox->u8Value = (MS_U8)u32Ret;
            break;
        case MDrv_CMD_AVD_SetFlag:
            u32Value = *(MS_U32*)pArgs;
            (psAvdInstPri->fpAVDSetFlag)(u32Value, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetRegValue:
            pSetRegValue = (PAVD_SETREGVALUE)pArgs;
            (psAvdInstPri->fpAVDSetRegValue)(pSetRegValue->u16Addr, pSetRegValue->u8Value);
            break;
        case MDrv_CMD_AVD_SetRegFromDSP:
            AVD_debug_Monitor(ULOGE("AVD","MDrv_CMD_AVD_SetRegFromDSP\n"));
            (psAvdInstPri->fpAVDSetRegFromDSP)(psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetInput:
            pSetInput = (PAVD_SETINPUT)pArgs;
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetInput: Source= %d, ScartFB= %d\n",pSetInput->eSource, pSetInput->u8ScartFB));
            u32Ret = (psAvdInstPri->fpAVDSetInput)(pSetInput->eSource, pSetInput->u8ScartFB, psAvdResPri);
            pSetInput->bEnable = (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_SetVideoStandard:
            pSetVideoStandard = (PAVD_SETVIDEOSTANDARD)pArgs;
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetVideoStandard: Standard=%d, IsInAutoTuning= %d\n",pSetVideoStandard->eStandard,pSetVideoStandard->bIsInAutoTuning));
            u32Ret = (psAvdInstPri->fpAVDSetVideoStandard)(pSetVideoStandard->eStandard, pSetVideoStandard->bIsInAutoTuning, psAvdResPri);
            pSetVideoStandard->bEnable = (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_SetChannelChange:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetChannelChange\n"));
            (psAvdInstPri->fpAVDSetChannelChange)(psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetHsyncDetectionForTuning:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetHsyncDetectionForTuning: Enanle=%d\n", *(MS_BOOL *)pArgs));
            (psAvdInstPri->fpAVDSetHsyncDetectionForTuning)(*(MS_BOOL *)pArgs, psAvdResPri);
            break;
        case MDrv_CMD_AVD_Set3dComb:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Set3dComb: Enable=%d\n", *(MS_BOOL *)pArgs));
            (psAvdInstPri->fpAVDSet3dComb)(*(MS_BOOL *)pArgs, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetShiftClk:
            pSetShiftCLK = (PAVD_SETSHIFTCLK)pArgs;
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetShiftClk: Enable=%d, ShiftMode= %d\n",pSetShiftCLK->bEnable,pSetShiftCLK->eShiftMode));
            (psAvdInstPri->fpAVDSetShiftClk)(pSetShiftCLK->bEnable, pSetShiftCLK->eShiftMode, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetFreerunPLL:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetFreerunPLL:%d\n", *(AVD_VideoFreq *)pArgs));
            (psAvdInstPri->fpAVDSetFreerunPLL)(*(AVD_VideoFreq *)pArgs, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetFreerunFreq:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetFreerunFreq: %d\n",*(AVD_FreeRunFreq *)pArgs));
            (psAvdInstPri->fpAVDSetFreerunFreq)(*(AVD_FreeRunFreq *)pArgs, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetFactoryPara:
            pSetFactorypara = (PAVD_SETFACTORYPARA)pArgs;
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetFactoryPara: FactoryPara= %d , value= 0x%x\n", pSetFactorypara->FactoryPara,pSetFactorypara->u8Value));
            (psAvdInstPri->fpAVDSetFactoryPara)(pSetFactorypara->FactoryPara, pSetFactorypara->u8Value,psAvdResPri);
            break;
        case MDrv_CMD_AVD_Set_Htt_UserMD:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Set_Htt_UserMD: 0x%x\n", *(MS_U16*)pArgs));
            psAvdResPri->_u16Htt_UserMD = *(MS_U16*)pArgs;
            break;
        case MDrv_CMD_AVD_SetDbgLevel:
            pSetDbgLevel = (PAVD_SETDBGLEVEL)pArgs;
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetDbgLevel: %d\n",*(AVD_DbgLv*)pArgs));
            u32Ret = (psAvdInstPri->fpAVDSetDbgLevel)(pSetDbgLevel->u8DbgLevel, psAvdResPri);
            pSetDbgLevel->bEnable = (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_SetPQFineTune:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetPQFineTune\n"));
            (psAvdInstPri->fpAVDSetPQFineTune)(psAvdResPri);
            break;
        case MDrv_CMD_AVD_Set3dCombSpeed:
            pSet3DCOMBSpeed = (PAVD_SET3DCOMBSPEED)pArgs;
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Set3dCombSpeed : COMB_57=0x%x , COMB_58=0x%x \n",pSet3DCOMBSpeed->u8COMB57,pSet3DCOMBSpeed->u8COMB58));
            (psAvdInstPri->fpAVDSet3dCombSpeed)(pSet3DCOMBSpeed->u8COMB57, pSet3DCOMBSpeed->u8COMB58, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetStillImageParam:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetStillImageParam\n"));
            (psAvdInstPri->fpAVDSetStillImageParam)(*(AVD_Still_Image_Param*)pArgs, psAvdResPri);
            break;
        case MDrv_CMD_AVD_SetAFECD4Factory:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetAFECD4Factory: 0x%x\n",*(MS_U8*)pArgs));
            psAvdResPri->_u8AfecD4Factory = *(MS_U8*)pArgs;
            break;
        case MDrv_CMD_AVD_Set2D3DPatchOnOff:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_Set2D3DPatchOnOff: %d\n",*(MS_BOOL*)pArgs));
            (psAvdInstPri->fpAVDSet2D3DPatchOnOff)(*(MS_BOOL*)pArgs);
            break;
        case MDrv_CMD_AVD_SetAutoFineGainToFixed:
            AVD_debug(ULOGE("AVD","MDrv_CMD_AVD_SetAutoFineGainToFixed\n"));
            u32Ret = (psAvdInstPri->fpAVDSetAutoFineGainToFixed)();
            break;
        case MDrv_CMD_AVD_GetFlag:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret= psAvdResPri->g_VD_InitData.u32VDPatchFlag;
            pCopyToUser->u32Value = u32Ret;
            break;
        case MDrv_CMD_AVD_GetRegValue:
            pGetRegValue = (PAVD_GETREGVALUE)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetRegValue)(pGetRegValue->u16Addr);
            pGetRegValue->u8Value = (MS_U8)u32Ret;
            break;
        case MDrv_CMD_AVD_GetStatus:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetStatus)();
            pCopyToUser->u16Value = (MS_U16)u32Ret;
            break;
        case MDrv_CMD_AVD_GetNoiseMag:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetNoiseMag)();
            pCopyToUser->u8Value = (MS_U8)u32Ret;
            break;
        case MDrv_CMD_AVD_GetVTotal:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetVTotal)();
            pCopyToUser->u16Value = (MS_U16)u32Ret;
            break;
        case MDrv_CMD_AVD_GetStandardDetection:
            pStandardDetection = (PAVD_GETSTANDARDDETECTION)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetStandardDetection)(&(pStandardDetection->vdLatchStatus),psAvdResPri);
            pStandardDetection->VideoStandardType = (AVD_VideoStandardType)u32Ret;
            break;
        case MDrv_CMD_AVD_GetLastDetectedStandard:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (MS_U32)psAvdResPri->_eLastStandard;
            pCopyToUser->eVideoStandardType = (AVD_VideoStandardType)u32Ret;
            break;
        case MDrv_CMD_AVD_GetCaptureWindow:
            pGetCaptureWindow = (PAVD_GETCAPTUREWINDOW)pArgs;
            (psAvdInstPri->fpAVDGetCaptureWindow)(pGetCaptureWindow->stCapWin, pGetCaptureWindow->eVideoStandardType, psAvdResPri);
            break;
        case MDrv_CMD_AVD_GetVerticalFreq:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetVerticalFreq)();
            pCopyToUser->eVideoFreq = (AVD_VideoFreq)u32Ret;
            break;
        case MDrv_CMD_AVD_GetHsyncEdge:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetHsyncEdge)();
            pCopyToUser->u8Value = (MS_U8)u32Ret;
            break;
        case MDrv_CMD_AVD_GetDSPFineGain:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetDSPFineGain)(psAvdResPri);
            pCopyToUser->u8Value = (MS_U8)u32Ret;
            break;
        case MDrv_CMD_AVD_GetDSPVersion:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetDSPVersion)();
            pCopyToUser->u16Value = (MS_U16)u32Ret;
            break;
        case MDrv_CMD_AVD_GetLibVer:
            pGetLibVer = (PAVD_GETLIBVER)pArgs;
            //u32Ret = (psAvdInstPri->fpAVDGetLibVer)((const MSIF_Version**)pArgs);
            u32Ret = (psAvdInstPri->fpAVDGetLibVer)(pGetLibVer->ppVersion);
            pGetLibVer->eAVDResult = (AVD_Result)u32Ret;
            break;
        case MDrv_CMD_AVD_GetInfo:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            //(psAvdInstPri->fpAVDGetInfo)((AVD_Info*)pArgs, psAvdResPri);
            pCopyToUser->eAVDInfo.eVDInputSource = psAvdResPri->_eVDInputSource;
            pCopyToUser->eAVDInfo.eVideoSystem = psAvdResPri->_eVideoSystem;
            pCopyToUser->eAVDInfo.eLastStandard = psAvdResPri->_eLastStandard;
            pCopyToUser->eAVDInfo.u8AutoDetMode=psAvdResPri->_u8AutoDetMode;
            pCopyToUser->eAVDInfo.u16CurVDStatus=psAvdResPri->_u16CurVDStatus;
            pCopyToUser->eAVDInfo.u8AutoTuningIsProgress=psAvdResPri->_u8AutoTuningIsProgress;
            break;
        case MDrv_CMD_AVD_IsSyncLocked:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDIsSyncLocked)();
            pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_IsSignalInterlaced:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDIsSignalInterlaced)();
            pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_IsColorOn:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDIsColorOn)();
            pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_SetPowerState:
            pSetPowerState = (PAVD_SETPOWERSTATE)pArgs;
            u32Ret = (psAvdInstPri->fpAVDSetPowerState)(pSetPowerState->u16PowerState, psAvdResPri);
            pSetPowerState->u32Value = (MS_U32)u32Ret;
            break;
        case MDrv_CMD_AVD_GetMacroVisionDetect:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetMacroVisionDetect)();
            pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_GetCGMSDetect:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDGetCGMSDetect)();
            pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_SetBurstWinStart:
            (psAvdInstPri->fpAVDSetBurstWinStart)(*(MS_U8*)pArgs);
            break;
         case MDrv_CMD_AVD_AliveCheck:
             pCopyToUser = (PAVD_COPYTOUSER)pArgs;
             u32Ret = (psAvdInstPri->fpAVDIsAVDAlive)();
             pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        case MDrv_CMD_AVD_IsLockAudioCarrier:
            pCopyToUser = (PAVD_COPYTOUSER)pArgs;
            u32Ret = (psAvdInstPri->fpAVDIsLockAudioCarrier)();
            pCopyToUser->bEnable= (MS_BOOL)u32Ret;
            break;
        default:
            break;
    }
    UtopiaResourceRelease(pResource);

    return u32Ret;
}

MS_U32 AVDClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}

extern void SerPrintf(char *fmt,...);

MS_U32 AVDStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        if(bInitSTR==1)
        {
#if defined(MSOS_TYPE_LINUX_KERNEL)
            eVideoStandard_Str = vd_str_timingchangedetection();
#endif
            vd_str_store_reg();
            vd_str_store_comb_sub_reg();
            HAL_AVD_COMB_SetMemoryRequest(DISABLE);
        }
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
#if defined(MSOS_TYPE_LINUX_KERNEL)
        UtopiaStrWaitCondition("avd", u32PowerState, 0);
#endif

        if(bInitSTR==1)
        {
            vd_str_init();
            #if ((T3_LOAD_CODE==0)&&(TWO_VD_DSP_CODE==0))
            vdmcu_str_init();
            #endif

            vd_str_setinput();
            vd_str_load_reg();
            vd_str_load_comb_sub_reg();
            if((eSource_Str!=E_INPUT_SOURCE_INVALID))
            {
                vd_str_stablestatus();
            }
#if defined(MSOS_TYPE_LINUX_KERNEL)
            if((eSource_Str==E_INPUT_SOURCE_CVBS1) ||
               (eSource_Str==E_INPUT_SOURCE_CVBS2) ||
               (eSource_Str==E_INPUT_SOURCE_CVBS3) ||
               (eSource_Str==E_INPUT_SOURCE_SCART1) ||
               (eSource_Str==E_INPUT_SOURCE_SCART2) ||
               (eSource_Str==E_INPUT_SOURCE_SVIDEO1) ||
               (eSource_Str==E_INPUT_SOURCE_SVIDEO2))
            {
                AVD_VideoStandardType etemp_VideoStandard = vd_str_timingchangedetection();

                if(eVideoStandard_Str!=etemp_VideoStandard)
                {
                    UtopiaStrSetData("timing change","yes");
                }
                else
                {
                    UtopiaStrSetData("timing change","no");
                }
            }
#endif
        }
#if defined(MSOS_TYPE_LINUX_KERNEL)
        UtopiaStrSendCondition("avd",u32PowerState ,0);
#endif

        u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
    }
    else
    {
        //SerPrintf("[AVD] STR unknown State!!!\n");
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}


#define BANK_SIZE                      0x100
//======Bank============
static MS_U8 s_au16Bank_AFEC[BANK_SIZE] = {0} ;
static MS_U8 s_au16Bank_COMB[BANK_SIZE] = {0} ;
static MS_U8 s_au16Bank_SECAM[BANK_SIZE] = {0} ;
//static MS_U16 bank_clkgen0[BANK_SIZE] = {0} ;

//======Sub-bank==========
static MS_U8 s_au16SubBank_COMB01[0x70] = {0};
static MS_U8 s_au16SubBank_COMB02[0x10] = {0};
static MS_U8 s_au16SubBank_COMB03[0x10] = {0};
static MS_U8 s_au16SubBank_COMB04[0x10] = {0};
static MS_U8 s_au16SubBank_COMB05[0x10] = {0};

static void vd_str_store_reg(void)
{
    MS_U32 i;

    /*store afec bank*/
    for(i = 0; i < BANK_SIZE ; i++)
    {
        s_au16Bank_AFEC[i] = HAL_AVD_GetReg(AFEC_REG_BASE+i);
     }

    /*store comb bank*/
    for(i = 0; i < BANK_SIZE ; i++)
    {
        s_au16Bank_COMB[i] = HAL_AVD_GetReg(COMB_REG_BASE+i);
    }

    /*store secam bank*/
    for(i = 0; i < BANK_SIZE ; i++)
    {
        s_au16Bank_SECAM[i] = HAL_AVD_GetReg(SCM_REG_BASE+i);
    }
}

static void vd_str_store_comb_sub_reg(void)
{
    MS_U32 i;
    // store comb sub-bank 01
    HAL_AVD_SetReg(COMB_REG_BASE,0x01);
    for(i = 0; i < 0x70 ; i++)
    {
        s_au16SubBank_COMB01[i] = HAL_AVD_GetReg(COMB_REG_BASE+i);
    }

    // store comb sub-bank 02
    HAL_AVD_SetReg(COMB_REG_BASE,0x02);
    for(i = 0; i < 0x10 ; i++)
    {
        s_au16SubBank_COMB02[i] = HAL_AVD_GetReg(COMB_REG_BASE+i);
    }

    // store comb sub-bank 03
    HAL_AVD_SetReg(COMB_REG_BASE,0x03);
    for(i = 0; i < 0x10 ; i++)
    {
        s_au16SubBank_COMB03[i] = HAL_AVD_GetReg(COMB_REG_BASE+i);
    }

    // store comb sub-bank 04
    HAL_AVD_SetReg(COMB_REG_BASE,0x04);
    for(i = 0; i < 0x10 ; i++)
    {
        s_au16SubBank_COMB04[i] = HAL_AVD_GetReg(COMB_REG_BASE+i);
    }

    // store comb sub-bank 05
    HAL_AVD_SetReg(COMB_REG_BASE,0x05);
    for(i = 0; i < 0x10 ; i++)
    {
        s_au16SubBank_COMB05[i] = HAL_AVD_GetReg(COMB_REG_BASE+i);
    }

    // return to comb bank
    HAL_AVD_SetReg(COMB_REG_BASE,0x00);

}

static void vd_str_init(void)
{
    HAL_AVD_RegInit();

    HAL_AVD_COMB_SetMemoryProtect(u32COMB_3D_Addr_Str,u32COMB_3D_Len_Str);
}

static void vdmcu_str_init(void)
{
    #if T3_LOAD_CODE
        HAL_AVD_AFEC_SetClockSource(TRUE);
    #endif

    HAL_AVD_VDMCU_LoadDSP(pu8VD_DSP_Code_Address_Str,u32VD_DSP_Code_Len_Str);

    #if T3_LOAD_CODE
    HAL_AVD_VDMCU_SetFreeze(ENABLE);
    HAL_AVD_AFEC_SetClockSource(FALSE);
    HAL_AVD_VDMCU_SetFreeze(DISABLE);
    #endif
    HAL_AVD_VDMCU_SetClock(AVD_VDMCU_CLOCK_108Mhz, AVD_VDMCU_CLOCK_NORMAL);
}

static void vd_str_setinput(void)
{
    HAL_AVD_AFEC_SetInput(eSource_Str,u8ScartFB_Str,eDemodType_Str,u32XTAL_Clock_Str);

    #if(TWO_VD_DSP_CODE)
    vdmcu_str_init();
    #endif

    #if T3_LOAD_CODE
    _Drv_AVD_VDMCU_Init();
    #else
    HAL_AVD_AFEC_McuReset();
    #endif

    HAL_AVD_COMB_SetMemoryRequest(ENABLE);
}

static void vd_str_load_reg(void)
{
    MS_U32 i;

    /*load afec bank*/
    for(i = 0; i < BANK_SIZE ; i++)
    {
        HAL_AVD_SetReg(AFEC_REG_BASE+i, s_au16Bank_AFEC[i]);
     }

    /*load comb bank*/
    for(i = 0; i < BANK_SIZE ; i++)
    {
        HAL_AVD_SetReg(COMB_REG_BASE+i, s_au16Bank_COMB[i]);
    }

    /*load secam bank*/
    for(i = 0; i < BANK_SIZE ; i++)
    {
        HAL_AVD_SetReg(SCM_REG_BASE+i, s_au16Bank_SECAM[i]);
    }

}

static void vd_str_load_comb_sub_reg(void)
{
    MS_U32 i;
    // load comb sub-bank 01
    HAL_AVD_SetReg(COMB_REG_BASE,0x01);   //set to comb sub bank01
    for(i = 1; i < 0x70 ; i++)
    {
        HAL_AVD_SetReg(COMB_REG_BASE+i, s_au16SubBank_COMB01[i]);
    }

    // store comb sub-bank 02
    HAL_AVD_SetReg(COMB_REG_BASE,0x02);
    for(i = 1; i < 0x10 ; i++)
    {
        HAL_AVD_SetReg(COMB_REG_BASE+i, s_au16SubBank_COMB02[i]);
    }

    // store comb sub-bank 03
    HAL_AVD_SetReg(COMB_REG_BASE,0x03);
    for(i = 1; i < 0x10 ; i++)
    {
        HAL_AVD_SetReg(COMB_REG_BASE+i, s_au16SubBank_COMB03[i]);
    }

    // store comb sub-bank 04
    HAL_AVD_SetReg(COMB_REG_BASE,0x04);
    for(i = 1; i < 0x10 ; i++)
    {
        HAL_AVD_SetReg(COMB_REG_BASE+i, s_au16SubBank_COMB04[i]);
    }

    // store comb sub-bank 05
    HAL_AVD_SetReg(COMB_REG_BASE,0x05);
    for(i = 1; i < 0x10 ; i++)
    {
        HAL_AVD_SetReg(COMB_REG_BASE+i, s_au16SubBank_COMB05[i]);
    }

    // return to comb bank
    HAL_AVD_SetReg(COMB_REG_BASE,0x00);

}


//-------------------------------------------------------------------------------------------------
//  Local VD functions
//-------------------------------------------------------------------------------------------------

static void _Drv_AVD_VDMCU_Init(AVD_RESOURCE_PRIVATE *pResource)
{
    #if NEW_VD_MCU
    MS_U8 u8Temp;
    #endif

    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","_MDrv_AVD_VDMCU_Init\n");
    }
    #endif

    #if T3_LOAD_CODE // weicheng 20121003
    HAL_AVD_AFEC_SetClockSource(TRUE);
    #endif

#if 0
    if(!(g_VD_InitData.u32VDPatchFlag & AVD_PATCH_DISABLE_PWS))
    {
        HAL_PWS_Stop_VDMCU();
    }
#endif

    // store DSP code information for STR function
    pu8VD_DSP_Code_Address_Str = pResource->g_VD_InitData.pu8VD_DSP_Code_Address;
    u32VD_DSP_Code_Len_Str = pResource->g_VD_InitData.u32VD_DSP_Code_Len;

    //--------------------------------------------------------
    // Load code
    //--------------------------------------------------------
    #if (LOAD_CODE_BYTE_WRITE_ONLY)//weicheng 20121003
    if (1)
    #else
    if (pResource->g_VD_InitData.eLoadCodeType == AVD_LOAD_CODE_BYTE_WRITE) //BY 20090402 (_bVD_FWStatus)
    #endif
    {
        // TODO add different loading function for T3
        #if AVD_DRV_DEBUG
        if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
        {
            ULOGE("AVD","[AVD_LOAD_CODE_BYTE_WRITE]F/W ADDR = 0x%X\n", (unsigned int)(pResource->g_VD_InitData.pu8VD_DSP_Code_Address));
        }
        #endif
        HAL_AVD_VDMCU_LoadDSP(pResource->g_VD_InitData.pu8VD_DSP_Code_Address, pResource->g_VD_InitData.u32VD_DSP_Code_Len);
        #if T3_LOAD_CODE  //weicheng 20121003
        HAL_AVD_VDMCU_SetFreeze(ENABLE);
        HAL_AVD_AFEC_SetClockSource(FALSE);
        HAL_AVD_VDMCU_SetFreeze(DISABLE);
        #endif
    }
    else if (pResource->g_VD_InitData.eLoadCodeType == AVD_LOAD_CODE_BDMA_FROM_SPI)
    {
        // TODO implement DMA function
        // copy code from flash to SDRAM
        // BY 20090402 MDrv_DMA_XCopy(MCPY_LOADVDMCUFW, BinInfo.B_FAddr, 0x00000l, BinInfo.B_Len); // TODO add DMA load function
        //MDrv_BDMA_CopyHnd(g_VD_InitData.pu8VD_DSP_Code_Address, 0 , g_VD_InitData.u32VD_DSP_Code_Len, BDMA_CPYTYPE_COL, BDMA_OPCFG_DEF);
        #if AVD_DRV_DEBUG
        if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
        {
            ULOGE("AVD","[AVD_LOAD_CODE_BDMA_FROM_SPI]F/W ADDR = 0x%X 0x%X\n", (unsigned int)pResource->g_VD_InitData.u32VD_DSP_Code_Address, (unsigned int)pResource->g_VD_InitData.u32VD_DSP_Code_Len);
        }
        #endif
        //MDrv_BDMA_SetDbgLevel(E_BDMA_DBGLV_ALL);
        HAL_AVD_VDMCU_SetFreeze(ENABLE);
        #if NEW_VD_MCU
        u8Temp = Drv_AVD_GetRegValue(VD_MCU_SRAM_EN);
        Drv_AVD_SetRegValue(VD_MCU_SRAM_EN,u8Temp&(~ BIT(0)));
        #endif
        //MDrv_SERFLASH_CopyHnd(pResource->g_VD_InitData.u32VD_DSP_Code_Address, 0 , pResource->g_VD_InitData.u32VD_DSP_Code_Len, E_SPIDMA_DEV_VDMCU, SPIDMA_OPCFG_DEF);
        #if NEW_VD_MCU
        Drv_AVD_SetRegValue(VD_MCU_SRAM_EN,u8Temp|BIT(0));
        #endif
        HAL_AVD_VDMCU_SetFreeze(DISABLE);
        #if T3_LOAD_CODE // weicheng 20121003
        HAL_AVD_VDMCU_SetFreeze(ENABLE);
        HAL_AVD_AFEC_SetClockSource(FALSE);
        HAL_AVD_VDMCU_SetFreeze(DISABLE);
        #endif
    }
    #if T3_LOAD_CODE // weicheng 20121003
    else if (pResource->g_VD_InitData.eLoadCodeType == AVD_LOAD_CODE_BDMA_FROM_DRAM) // TODO add MIU0/1 check
    {
        // TODO implement DMA function
        // copy code from flash to SDRAM
        // BY 20090402 MDrv_DMA_XCopy(MCPY_LOADVDMCUFW, BinInfo.B_FAddr, 0x00000l, BinInfo.B_Len); // TODO add DMA load function
        //MDrv_BDMA_CopyHnd(g_VD_InitData.pu8VD_DSP_Code_Address, 0 , g_VD_InitData.u32VD_DSP_Code_Len, BDMA_CPYTYPE_COL, BDMA_OPCFG_DEF);
        #if AVD_DRV_DEBUG
        if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
        {
            ULOGE("AVD","[AVD_LOAD_CODE_BDMA_FROM_DRAM]F/W ADDR = 0x%X\n", (unsigned int)(pResource->g_VD_InitData.pu8VD_DSP_Code_Address));
        }
        #endif
        HAL_AVD_VDMCU_SetFreeze(ENABLE);
        MDrv_BDMA_CopyHnd(pResource->g_VD_InitData.u32VD_DSP_Code_Address, 0 , pResource->g_VD_InitData.u32VD_DSP_Code_Len, E_BDMA_SDRAM2VDMCU, BDMA_OPCFG_DEF);
        HAL_AVD_VDMCU_SetFreeze(DISABLE);

        HAL_AVD_VDMCU_SetFreeze(ENABLE);
        HAL_AVD_AFEC_SetClockSource(FALSE);
        HAL_AVD_VDMCU_SetFreeze(DISABLE);
    }
    #endif
    else
    {
        #if AVD_DRV_DEBUG
        if (pResource->_u8AVDDbgLevel >= AVD_DBGLV_ERR)
        {
            ULOGE("AVD","_MDrv_AVD_VDMCU_Init eLoadCodeType Invalid\r\n");
        }
        #endif
    }

    HAL_AVD_VDMCU_SetClock(AVD_VDMCU_CLOCK_108Mhz, AVD_VDMCU_CLOCK_NORMAL);

    HAL_AVD_AFEC_SetPatchFlag(pResource->g_VD_InitData.u32VDPatchFlag);

    //HAL_AVD_RegInitExt(pResource->g_VD_InitData.u8VdDecInitializeExt); // TODO use system variable type

    pResource->_u8AfecD4Factory=HAL_AVD_GetReg(BK_AFEC_D4);
}

static void _Drv_AVD_3DCombSpeedup(AVD_RESOURCE_PRIVATE *pResource)
{
    HAL_AVD_COMB_Set3dCombSpeed(pResource->_u8Comb57, pResource->_u8Comb58, pResource->_u8Comb5F); //need to review, sonic 20091218
    #if ADJUST_CHANNEL_CHANGE_HSYNC
    HAL_AVD_AFEC_SetSwingLimit(0);
    HAL_AVD_AFEC_EnableBottomAverage(DISABLE); // more sensitivity
    #endif
    if (!(pResource->_u8AfecD4Factory & BIT(4)))
    {
        HAL_AVD_AFEC_EnableVBIDPLSpeedup(ENABLE);
    }
    HAL_AVD_SetReg(BK_AFEC_A1, 0x6A);
    HAL_AVD_SetReg(BK_AFEC_A0, 0xBC);
    pResource->_u8Comb10Bit3Flag=BIT(2)|BIT(1)|BIT(0);
    pResource->_u32VideoSystemTimer=MsOS_GetSystemTime();
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","_MDrv_AVD_3DCombSpeedup Enable %d\n",(int)pResource->_u32VideoSystemTimer);
    }
    #endif
}

//20120719 Brian Monitor the Dsp frequency and change the CLK setting for the shift CLK
static void _Drv_AVD_ShifClk_Monitor(AVD_RESOURCE_PRIVATE *pResource)
{
       MS_U16 u16Status;
       AVD_VideoStandardType eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
       u16Status = HAL_AVD_AFEC_GetStatus();
       switch (u16Status & VD_FSC_TYPE)
            {
                case VD_FSC_4433:
                    eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
                    break;

                case VD_FSC_3579:
                    eVideoStandard = E_VIDEOSTANDARD_NTSC_M;
                    break;

                case VD_FSC_3575:
                    eVideoStandard = E_VIDEOSTANDARD_PAL_M;
                    break;

                case VD_FSC_3582:
                    eVideoStandard = E_VIDEOSTANDARD_PAL_N;
                    break;
                 case VD_FSC_4285:
                    eVideoStandard = E_VIDEOSTANDARD_SECAM;
                    break;

                 default :
                    break;

    }
       HAL_AVD_ShiftClk(pResource->gShiftMode, eVideoStandard, pResource->g_VD_InitData.u32XTAL_Clock);
}

static void _Drv_AVD_SCART_Monitor(AVD_RESOURCE_PRIVATE *pResource)
{
    pResource->_u32SCARTWaitTime=MsOS_GetSystemTime();
    if(pResource->_u8SCARTSwitch==1) //Svideo
    {
        HAL_AVD_SetReg(BK_AFEC_1A,(HAL_AVD_GetReg(BK_AFEC_1A)|0xC0));
        HAL_AVD_SetReg(BK_COMB_10,(HAL_AVD_GetReg(BK_COMB_10)|0x80));
        HAL_AVD_SetReg(BK_AFEC_1F,(HAL_AVD_GetReg(BK_AFEC_1F)|0x80));
        HAL_AVD_SetReg(BK_AFEC_A2,(HAL_AVD_GetReg(BK_AFEC_A2)&0xCF)|0x20);

        if (E_VIDEOSTANDARD_NTSC_M == pResource->_eVideoSystem)
        {
            HAL_AVD_COMB_SetYCPipe(0x20);
        }
        else // SIG_NTSC_443, SIG_PAL, SIG_PAL_M, SIG_PAL_NC, SIG_SECAM
        {
            HAL_AVD_COMB_SetYCPipe(0x30);
        }

        switch (pResource->_eVideoSystem)
        {
        case E_VIDEOSTANDARD_PAL_M:
        case E_VIDEOSTANDARD_PAL_N:
            HAL_AVD_COMB_SetCbCrInverse(0x04);
            break;

        case E_VIDEOSTANDARD_NTSC_44:
            HAL_AVD_COMB_SetCbCrInverse(0x0C);
            break;

        case E_VIDEOSTANDARD_PAL_60:
            HAL_AVD_COMB_SetCbCrInverse(0x08);
            break;

        case E_VIDEOSTANDARD_NTSC_M:
        case E_VIDEOSTANDARD_SECAM:
            HAL_AVD_COMB_SetCbCrInverse(0x00);
            break;

        case E_VIDEOSTANDARD_PAL_BGHI:
        default:
            HAL_AVD_COMB_SetCbCrInverse(0x08);
            break;
        }

    }
    else  //CVBS
    {
        HAL_AVD_SetReg(BK_AFEC_1A,(HAL_AVD_GetReg(BK_AFEC_1A)&0x3F));
        HAL_AVD_SetReg(BK_COMB_10,(HAL_AVD_GetReg(BK_COMB_10)&0x7F));
        HAL_AVD_SetReg(BK_AFEC_1F,(HAL_AVD_GetReg(BK_AFEC_1F)&0x7F));
        HAL_AVD_SetReg(BK_AFEC_A2,(HAL_AVD_GetReg(BK_AFEC_A2)&0xCF));

        HAL_AVD_COMB_SetYCPipe(0x20);
        HAL_AVD_COMB_SetCbCrInverse(0x00);
    }
    pResource->_u8SCARTPrestandard=pResource->_eVideoSystem;

}

static void _Drv_AVD_MBX_WaitReady(void)
{
    MS_U32 u32StartTime=OS_SYSTEM_TIME();
    while (OS_SYSTEM_TIME()-u32StartTime < 10)
    {
        if (!HAL_AVD_GetReg(BK_AFEC_C0)) break;
    }
}

#if 0
static void _Drv_AVD_SyncRangeHandler(AVD_RESOURCE_PRIVATE *pResource)
{
#ifndef MSOS_TYPE_LINUX
    XC_ApiStatus sXC_Status;
    XC_SETWIN_INFO sXC_SetWinInfo;
    MS_WINDOW_TYPE SrcWin;
    static MS_BOOL bXC_BlackScreen_Enabled = TRUE;
    static MS_U16   u16XC_VCap;

    MS_U16 wVtotal = HAL_AVD_AFEC_GetVTotal();
    MS_U16 wVsize_bk, g_cVSizeShift;
    static MS_U8 u8SyncStableCounter = 6;

    if( (!Drv_AVD_IsSyncLocked()) ||
        MApi_XC_IsFreezeImg(MAIN_WINDOW) ||
        (pResource->u16PreVtotal == 0))
    {
        bXC_BlackScreen_Enabled = TRUE;
        return;
    }

/*
    if ((wHPeriod > (u16PreHPeriod + 3)) || (wHPeriod < (u16PreHPeriod - 3)))
  {
    printf("P3 \r\n");
    return;
      }
*/

    // Get XC capture height from the begining
    if(MApi_XC_IsBlackVideoEnable(MAIN_WINDOW))
    {
        // XC still blocked the video
        bXC_BlackScreen_Enabled = TRUE;
        return;
    }
    else
    {
        if(bXC_BlackScreen_Enabled)
        {
            // update new capture height
            bXC_BlackScreen_Enabled = FALSE;
            MApi_XC_GetCaptureWindow( &SrcWin, MAIN_WINDOW );
            u16XC_VCap = SrcWin.height;

#if 0
            if ((wVtotal < 675 ) && (wVtotal > 570))
                u16PreVtotal = 625;
            else
                u16PreVtotal = 525;
#endif
            printf("******** VCap = %d, PreVtt = %d\n", u16XC_VCap, pResource->u16PreVtotal);
        }
    }

    //if (((wVtotal < 620) && (wVtotal > 570)) || ((wVtotal > 630) && (wVtotal < 670)))
    if ((wVtotal < 620) && (wVtotal > 570))
    {
        if (pResource->u16PreVtotal != wVtotal)
        {
            pResource->u16PreVtotal  = wVtotal;
            u8SyncStableCounter = 0;
        }
        else if (u8SyncStableCounter < 5)
            u8SyncStableCounter ++;
    }
    //else if (((wVtotal < 520) && (wVtotal > 470)) || ((wVtotal > 530) && (wVtotal < 570)))
    else if ((wVtotal < 520) && (wVtotal > 470))
    {

        if (pResource->u16PreVtotal != wVtotal)
        {
            pResource->u16PreVtotal  = wVtotal;
            u8SyncStableCounter = 0;
        }
        else if (u8SyncStableCounter < 5)
            u8SyncStableCounter ++;
    }
    else if(((wVtotal <= 630) && (wVtotal >= 620)) || ((wVtotal <= 530) && (wVtotal >= 520)))
    {
        if((pResource->u16PreVtotal > (wVtotal + 5)) || (pResource->u16PreVtotal < (wVtotal - 5)))
        {
            pResource->u16PreVtotal  = wVtotal;
            u8SyncStableCounter = 0;
        }
        else if (u8SyncStableCounter < 5)
            u8SyncStableCounter ++;
    }

    if(u8SyncStableCounter == 5)
    {
        u8SyncStableCounter = 6;
        wVsize_bk = u16XC_VCap;
        if (((wVtotal < 620) && (wVtotal > 570)) || ((wVtotal > 630) && (wVtotal < 670)))
        {
            if (wVtotal > 625)
            {
                g_cVSizeShift = wVtotal - 625;
               u16XC_VCap = u16XC_VCap + g_cVSizeShift;
            }
            else
            {
                g_cVSizeShift = 625 - wVtotal;
               u16XC_VCap = u16XC_VCap - g_cVSizeShift;
            }
        }
        else if (((wVtotal < 520) && (wVtotal > 470)) || ((wVtotal > 530) && (wVtotal < 570)))
        {
            if (wVtotal > 525)
            {
                g_cVSizeShift = wVtotal - 525;
                u16XC_VCap = u16XC_VCap + g_cVSizeShift;
            }
            else
            {
                g_cVSizeShift = 525 - wVtotal;
                u16XC_VCap = u16XC_VCap - g_cVSizeShift;
            }
        }

        memset(&sXC_Status, 0, sizeof(XC_ApiStatus));
        memset(&sXC_SetWinInfo, 0, sizeof(XC_SETWIN_INFO));
        MApi_XC_GetStatus(&sXC_Status, MAIN_WINDOW);
        memcpy(&sXC_SetWinInfo, &sXC_Status, sizeof(XC_SETWIN_INFO));

        // reset changed part
        sXC_SetWinInfo.bPreHCusScaling = FALSE;
        sXC_SetWinInfo.bPreVCusScaling = FALSE;
        sXC_SetWinInfo.stCapWin.height = u16XC_VCap;

        MApi_XC_SetWindow(&sXC_SetWinInfo, sizeof(XC_SETWIN_INFO), MAIN_WINDOW);

        u16XC_VCap = wVsize_bk;
   }
#endif
}
#endif
void _Drv_AVD_COMB_StillImage(AVD_RESOURCE_PRIVATE *pResource)
{
    MS_U8  u8Ctl;
    MS_U16 u16Value;
    static MS_U8 check_counter = 0;
    static MS_U16 Total_Nosiemag = 0;
    static MS_U8 status = 2;
    static MS_U16 _u16PreNoiseMag = 0;

    if(pResource->_eVDInputSource == E_INPUT_SOURCE_ATV)
        {
            u8Ctl = HAL_AVD_AFEC_GetNoiseMag();// get VD noise magnitude
            if(pResource->g_stillImageParam.bMessageOn)
            {
                //printf(" ====================>  Noise mag =%d \n",(int)u8Ctl);
            }

            if (check_counter<10)
            {
                Total_Nosiemag+= u8Ctl;
                check_counter++;
            }
            else
            {
                u16Value =Total_Nosiemag;
                if(pResource->g_stillImageParam.bMessageOn)
                {
                    //printf(" ========================>  AVG noise mag =%d \n",(int) u16Value);
                }
                u16Value = ( 8*_u16PreNoiseMag + 8*u16Value )/16;
                /////////////////////////////////////////////////////////////////////////////////////

                if(u16Value <= pResource->g_stillImageParam.u8Threshold1)
                {
                    _u16PreNoiseMag = u16Value;
                    check_counter = 0;
                    Total_Nosiemag = 0;
                    u8Ctl = HAL_AVD_GetReg(BK_COMB_38);
                    if(pResource->g_stillImageParam.bMessageOn)
                    {
                        //printf("=====>Thread =%d \n\n",u16Value);
                        ULOGE("AVD","=====>1Thread1: u16Value =0x%x \n\n",u16Value);
                    }
                    HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str1_COMB37);
                    HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str1_COMB38));
                    HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str1_COMB7C);
                    HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str1_COMBED);
                    //MDrv_AVD_SetRegValue(BK_COMB_C0, 0x6C);
                    status = 1;
                }

                else if((u16Value<pResource->g_stillImageParam.u8Threshold2) && (u16Value>pResource->g_stillImageParam.u8Threshold1))
                {
                    _u16PreNoiseMag = u16Value;
                    check_counter = 0;
                    Total_Nosiemag = 0;
                    u8Ctl = HAL_AVD_GetReg(BK_COMB_38);
                    if(pResource->g_stillImageParam.bMessageOn)
                    {
                        //printf("=====>Thread =%d \n\n",u16Value);
                    }
                    if(status==1)
                    {
                       if(pResource->g_stillImageParam.bMessageOn)
                       {
                            ULOGE("AVD","=====>4Thread1: u16Value =0x%x \n\n",u16Value);
                       }
                       HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str1_COMB37);
                       HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str1_COMB38));
                       HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str1_COMB7C);
                       HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str1_COMBED);
                       //MDrv_AVD_SetRegValue(BK_COMB_C0, 0x6C);
                       status = 1;
                    }
                    else if(status==2)
                    {
                       if(pResource->g_stillImageParam.bMessageOn)
                       {
                            ULOGE("AVD","=====>5Thread2: u16Value =0x%x \n\n",u16Value);
                       }
                       HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str2_COMB37);
                       HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str2_COMB38));
                       HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str2_COMB7C);
                       HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str2_COMBED);
                       //MDrv_AVD_SetRegValue(BK_COMB_C0, 0x6C);
                       status = 2;
                   }
                   else if(status==3)
                   {
                       if(pResource->g_stillImageParam.bMessageOn)
                       {
                            ULOGE("AVD","=====>6Thread3: u16Value =0x%x \n\n",u16Value);
                       }
                       status = 2;
                   }
                }

                else if((u16Value<=pResource->g_stillImageParam.u8Threshold3) && (u16Value>=pResource->g_stillImageParam.u8Threshold2))
                {
                    _u16PreNoiseMag = u16Value;
                    check_counter = 0;
                    Total_Nosiemag = 0;
                    u8Ctl = HAL_AVD_GetReg(BK_COMB_38);
                    if(pResource->g_stillImageParam.bMessageOn)
                    {
                        //printf("=====>Thread =%d \n\n",u16Value);
                        ULOGE("AVD","=====>2Thread2: u16Value =0x%x \n\n",u16Value);
                    }
                    HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str2_COMB37);
                    HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str2_COMB38));
                    HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str2_COMB7C);
                    HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str2_COMBED);
                    //MDrv_AVD_SetRegValue(BK_COMB_C0, 0x6C);
                    status = 2;
                }

                else if((u16Value < pResource->g_stillImageParam.u8Threshold4) &&( u16Value > pResource->g_stillImageParam.u8Threshold3))
                {
                    _u16PreNoiseMag = u16Value;
                    check_counter = 0;
                    Total_Nosiemag = 0;
                    u8Ctl = HAL_AVD_GetReg(BK_COMB_38);
                    if(pResource->g_stillImageParam.bMessageOn)
                    {
                        //printf("=====>Thread =%d \n\n",u16Value);
                    }
                    if(status==1)
                    {
                        if(pResource->g_stillImageParam.bMessageOn)
                        {
                            ULOGE("AVD","=====>7Thread1: u16Value =0x%x \n\n",u16Value);
                        }
                        status = 2;
                    }
                    else if(status==2)
                    {
                       if(pResource->g_stillImageParam.bMessageOn)
                       {
                            ULOGE("AVD","=====>8Thread2: u16Value =0x%x \n\n",u16Value);
                       }
                       HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str2_COMB37);
                       HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str2_COMB38));
                       HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str2_COMB7C);
                       HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str2_COMBED);
                       //MDrv_AVD_SetRegValue(BK_COMB_C0, 0x6C);
                       status = 2;
                    }
                    else if(status==3)
                    {
                       if(pResource->g_stillImageParam.bMessageOn)
                       {
                            ULOGE("AVD","=====>9Thread3: u16Value =0x%x \n\n",u16Value);
                       }
                       HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str3_COMB37);
                       HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str3_COMB38));
                       HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str3_COMB7C);
                       HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str3_COMBED);
                       //MDrv_AVD_SetRegValue(BK_COMB_C0, 0x6C);
                       status = 3;
                   }
                }

                else if(u16Value >= pResource->g_stillImageParam.u8Threshold4)
                {
                    _u16PreNoiseMag = u16Value;
                    check_counter = 0;
                    Total_Nosiemag = 0;
                    u8Ctl = HAL_AVD_GetReg(BK_COMB_38);
                    if(pResource->g_stillImageParam.bMessageOn)
                    {
                        //printf("=====>Thread =%d \n\n",u16Value);
                        ULOGE("AVD","=====>3Thread3: u16Value =0x%x \n\n",u16Value);
                    }
                    HAL_AVD_SetReg(BK_COMB_37, pResource->g_stillImageParam.u8Str3_COMB37);
                    HAL_AVD_SetReg(BK_COMB_38, (u8Ctl & pResource->g_stillImageParam.u8Str3_COMB38));
                    HAL_AVD_SetReg(BK_COMB_7C, pResource->g_stillImageParam.u8Str3_COMB7C);
                    HAL_AVD_SetReg(BK_COMB_ED, pResource->g_stillImageParam.u8Str3_COMBED);
                    //MDrv_AVD_SetRegValue(BK_COMB_C0/., 0x6C);
                    status = 3;
                }


            /////////////////////////////////////////////////////////////////////////////////////

            }
        }
}

static MS_U8 _Drv_AVD_DSPReadByte(MS_U8 u8Cmd, MS_U16 u16Addr)
{
    MS_U8 u8Dummy = 200;
    MS_U8 u8Data;

    HAL_AVD_SetReg(BK_AFEC_C1, u16Addr);
    HAL_AVD_SetReg(BK_AFEC_C3, u16Addr>>8);
    HAL_AVD_SetReg(BK_AFEC_C0, u8Cmd);

    while(u8Dummy--)
    {
        if (HAL_AVD_GetReg(BK_AFEC_C0) == 0x00)
            break;
        MsOS_DelayTask(1);
    }
    u8Data =  HAL_AVD_GetReg(BK_AFEC_C2);

    return u8Data;
}

//-------------------------------------------------------------------------------------------------
//  Global VD functions
//-------------------------------------------------------------------------------------------------

AVD_Result Drv_AVD_Init(VD_INITDATA *pVD_InitData, MS_U32 u32InitDataLen, AVD_RESOURCE_PRIVATE *pResource)
{
    //BY 20090410 MS_U16 u16TimeOut;
    //BY 20090410 MS_U32 u32dst;
    //BY 20090410 MS_BOOL bResult;
   char pAVD_Mutex_String[10];
   if (NULL == strncpy(pAVD_Mutex_String,"Mutex VD",10))
    {
         #if AVD_DRV_DEBUG
         if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
         {
             ULOGE("AVD","MDrv_AVD_Init strcpy Fail\n");
          }
         #endif
        return E_AVD_FAIL;
    }
    pResource->_s32AVD_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pAVD_Mutex_String, MSOS_PROCESS_SHARED);
    MS_ASSERT(pResource->_s32AVD_Mutex >= 0);

    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_Init\n");
    }
    #endif
    if ( sizeof(pResource->g_VD_InitData) == u32InitDataLen)
    {
        memcpy(&pResource->g_VD_InitData, pVD_InitData, u32InitDataLen);
    }
    else
    {
        MS_ASSERT(FALSE);
        return E_AVD_FAIL;
    }

    pResource->_u16DataH[0] = pResource->_u16DataH[1] = pResource->_u16DataH[2] = 1135L;
    pResource->_u8HtotalDebounce = 0;
    pResource->_eVideoSystem = E_VIDEOSTANDARD_NOTSTANDARD;
    pResource->_u8AutoDetMode = FSC_AUTO_DET_ENABLE;
    pResource->_u16CurVDStatus = 0;
    #if AVD_DRV_DEBUG
    pResource->_u8AVDDbgLevel = AVD_DBGLV_NONE;
    pResource->u32VDPatchFlagStatus=pResource->u32PreVDPatchFlagStatus=0;
    #endif
    // BY 20090402 remove un-necessary code HAL_AVD_VDMCU_ResetAddress();

    HAL_AVD_RegInit(); // !! any register access should be after this function
    HAL_AVD_COMB_SetMemoryProtect(pResource->g_VD_InitData.u32COMB_3D_ADR, pResource->g_VD_InitData.u32COMB_3D_LEN);
    bInitSTR = 1;   //Init flag for STR load and write DRAM
    //store COMB address for STR function
    u32COMB_3D_Addr_Str = pResource->g_VD_InitData.u32COMB_3D_ADR;
    u32COMB_3D_Len_Str = pResource->g_VD_InitData.u32COMB_3D_LEN;
    #if 0 // BY 20090331, for FPGA only, take on effects in real chip, by book.weng
    RIU_WriteByte(BK_AFEC_0A, 0x10);
    RIU_WriteByte(BK_AFEC_0F, 0x48);
    #endif


    #if 0 // should call set input directly to set input source
    if(g_VD_InitData.eDemodType == DEMODE_MSTAR_VIF)
    {
        RIU_WriteByte(BK_AFEC_CF, 0x80);
        RIU_WriteByte(BK_AFEC_21, 0x1D); // TODO check setting correct or not?
        RIU_WriteByte(BK_AFEC_40, 0x91);
    }
    #endif

    #if 0 // BY 20090402
    //========MDrv_VD_LoadDSPCode=======
    BinInfo.B_ID = g_VD_InitData.u16VDDSPBinID;
    MDrv_DMA_Get_BinInfo(&BinInfo, &bResult);
    if( bResult != PASS )
    {
        return;
    }
    #endif
    HAL_AVD_SetDSPCodeType(pResource->g_VD_InitData.u32VDPatchFlag);
    #if ((T3_LOAD_CODE==0)&&(TWO_VD_DSP_CODE==0)) //20121003 weicheng
    _Drv_AVD_VDMCU_Init(pResource); // should keep this for T3 because VIF initial will need mailbox
    #endif

    return E_AVD_OK;
}

void Drv_AVD_Exit(AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_Exit\n");
    }
    #endif
    #if (NEW_VD_MCU)
    //HAL_AVD_VDMCU_SoftStop();
    #endif
    eSource_Str=E_INPUT_SOURCE_INVALID;
    HAL_AVD_AFEC_SetClock(DISABLE);
    HAL_AVD_COMB_SetMemoryRequest(DISABLE);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

MS_BOOL Drv_AVD_ResetMCU(AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_ResetMCU\n");
    }
    #endif
    HAL_AVD_AFEC_McuReset(); // assume clock source is ready, it's required to switch source before reset after T4
    HAL_AVD_AFEC_SetPatchFlag(pResource->g_VD_InitData.u32VDPatchFlag);

    // Forced to PAL mode
    pResource->_u16CurVDStatus = 0;

    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    return TRUE;
}

void Drv_AVD_FreezeMCU(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource) // should be static, no mutex
{
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_FreezeMCU %d\n",bEnable);
    }
    #endif
    HAL_AVD_VDMCU_SetFreeze(bEnable);
}

MS_U16 Drv_AVD_Scan_HsyncCheck(MS_U8 u8HtotalTolerance, AVD_RESOURCE_PRIVATE *pResource) //TODO should add delay between register read
{
    MS_U16 u16CurrentStatus = VD_RESET_ON;
    MS_U16 u16Dummy, u16Temp, u16SPLHTotal;
    MS_U8 u8Value, u8VdHsyncLockedCount;
    static const MS_U16 u16SPL_NSPL[5] =
    {
        1135,   // PAL
        1097,   // SECAM
        910,    // NTSC, PAL M
        917,    // PAL Nc
        1127,   // NTSC 4.43
    };

    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    u16Dummy = 20; // just protect program dead lock
    u8VdHsyncLockedCount = 0;
    while( u16Dummy-- )
    {
        if( HAL_AVD_AFEC_GetHWHsync() )
        {
            u16CurrentStatus = HAL_AVD_AFEC_GetStatus();
            u8VdHsyncLockedCount++;
        }
    }
    //printf("vdLoop1=%bd\n", u8VdHsyncLockedCount);

    if( u8VdHsyncLockedCount>14 )
    {
        //printf("Second Check\n");
        u8VdHsyncLockedCount = 0;
        u16Dummy = 10; // just protect program dead lock
        u16SPLHTotal = HAL_AVD_AFEC_GetHTotal(); // SPL_NSPL, H total
        //printf("Ht=%d\n", u16SPLHTotal);
        while( u16Dummy-- )
        {
           u16Temp = HAL_AVD_AFEC_GetHTotal(); // SPL_NSPL, H total
            u16Temp = (u16Temp+u16SPLHTotal)>>1;
            u16SPLHTotal = u16Temp;

            //printf("Ht=%d\n", u16SPLHTotal);
            for(u8Value=0; u8Value<=4;u8Value++)
            {
                if( abs(u16SPLHTotal-u16SPL_NSPL[u8Value]) < u8HtotalTolerance )
                {
                    //printf("+");
                    u8VdHsyncLockedCount++;
                }
                else
                {
                    //printf("-");
                }
            }
            //printf("\n");
        }

        //printf("vdLoop2=%bd\n", u8VdHsyncLockedCount);
        if( u8VdHsyncLockedCount < 8 )
        {
            u16CurrentStatus = VD_RESET_ON;
        }
    }
    else
    {
        u16CurrentStatus = VD_RESET_ON;
    }

    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    return u16CurrentStatus;
}

void Drv_AVD_StartAutoStandardDetection(AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    pResource->_u8AutoDetMode=FSC_AUTO_DET_ENABLE;
    HAL_AVD_AFEC_EnableForceMode(DISABLE);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

MS_BOOL Drv_AVD_ForceVideoStandard(AVD_VideoStandardType eVideoStandardType, AVD_RESOURCE_PRIVATE *pResource)
{
   //Check input
   AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_ForceVideoStandard %d\n",eVideoStandardType);
    }
    #endif

    if (eVideoStandardType >= E_VIDEOSTANDARD_MAX)
        MS_ASSERT(FALSE);

    pResource->_u8AutoDetMode= FSC_AUTO_DET_DISABLE;
    pResource->_eForceVideoStandard=eVideoStandardType;
    HAL_AVD_AFEC_EnableForceMode(ENABLE);
    switch(eVideoStandardType)
    {
        case E_VIDEOSTANDARD_NTSC_M:
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_NTSC);
            break;
        case E_VIDEOSTANDARD_SECAM:
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_SECAM);
            break;
        case E_VIDEOSTANDARD_NTSC_44:
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_NTSC_443);
            break;
        case E_VIDEOSTANDARD_PAL_M:
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL_M);
            break;
        case E_VIDEOSTANDARD_PAL_N:
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL_N);
            break;
        case E_VIDEOSTANDARD_PAL_60:
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL_60);
            break;
        default:    //E_VIDEOSTANDARD_PAL_BGHI
            HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL);
            break;
    }

    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    return TRUE;
}

void Drv_AVD_3DCombSpeedup(AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    _Drv_AVD_3DCombSpeedup(pResource);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_LoadDSP(MS_U8 *pu8VD_DSP, MS_U32 len, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    HAL_AVD_VDMCU_LoadDSP(pu8VD_DSP,len);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_BackPorchWindowPositon(MS_BOOL bEnable , MS_U8 u8Value)
{
    HAL_AVD_AFEC_BackPorchWindowPosition(bEnable , u8Value);
}

MS_U8 Drv_AVD_MBX_ReadByteByVDMbox(MS_U8 u8Addr, AVD_RESOURCE_PRIVATE *pResource)
{
    MS_U8 u8Value;
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    _Drv_AVD_MBX_WaitReady();

    HAL_AVD_SetReg(BK_AFEC_C1, u8Addr);
    HAL_AVD_SetReg(BK_AFEC_C0, 0x02);

    _Drv_AVD_MBX_WaitReady();
    u8Value=Drv_AVD_GetRegValue(BK_AFEC_C2);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    return u8Value;
}

void Drv_AVD_SetFlag(MS_U32  u32VDPatchFlag, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    pResource->g_VD_InitData.u32VDPatchFlag=u32VDPatchFlag;
    HAL_AVD_AFEC_SetPatchFlag(pResource->g_VD_InitData.u32VDPatchFlag);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_SetRegValue(MS_U16 u16Addr,MS_U8 u8Value)
{
    // avoid writing other block
    if (((u16Addr & 0xFF00 ) == AFEC_REG_BASE) || \
        ((u16Addr & 0xFF00 ) == COMB_REG_BASE) || \
        ((u16Addr & 0xFF00 ) == VBI_REG_BASE) || \
        ((u16Addr & 0xFF00 ) == MIIC_REG_BASE) ||\
        ((u16Addr & 0xFF00 ) == SCM_REG_BASE))
    {
        HAL_AVD_SetReg( u16Addr ,u8Value);
    }
}

void Drv_AVD_SetRegFromDSP(AVD_RESOURCE_PRIVATE *pResource)
{
    MS_U8  u8Ctl, u8Value;
    MS_U16 u16Htotal, u16CurrentHStart;
    MS_U8  u8update;
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    if(((pResource->_eVDInputSource==E_INPUT_SOURCE_SCART1)||(pResource->_eVDInputSource==E_INPUT_SOURCE_SCART2))&&(pResource->g_VD_InitData.u32VDPatchFlag &AVD_PATCH_SCART_SVIDEO))     //20121211
    {
        if(MsOS_GetSystemTime() -pResource->_u32SCARTWaitTime >2000)
        {
            if(!(HAL_AVD_GetReg(BK_AFEC_CC)&0x02)&&(pResource->_eVideoSystem!=E_VIDEOSTANDARD_SECAM))
            {
                if(pResource->_u8SCARTSwitch==0)
                    pResource->_u8SCARTSwitch=1;
                else
                    pResource->_u8SCARTSwitch=0;

                _Drv_AVD_SCART_Monitor(pResource);
            }
            else if(pResource->_u8SCARTPrestandard!=pResource->_eVideoSystem)
            {
                 _Drv_AVD_SCART_Monitor(pResource);
            }
        }
    }

    //20120719 Brian Shift CLK patch for 43.2M water noise
    if( pResource->_bShiftClkFlag == TRUE)
    {
        _Drv_AVD_ShifClk_Monitor(pResource);
    }
    HAL_AVD_AFEC_SetRegFromDSP();

    {
        u16Htotal = HAL_AVD_AFEC_GetHTotal(); // SPL_NSPL, H total

        switch( pResource->_eVideoSystem ) // 2006.06.17 Michael, need to check SRC1 if we use MST6xxx
        {
            case E_VIDEOSTANDARD_NTSC_M:     // SIG_NTSC: 910
                 u16CurrentHStart = 910;
                 break;

            case E_VIDEOSTANDARD_PAL_BGHI:   // SIG_PAL: 1135
                 u16CurrentHStart = 1135;
                 break;

            case E_VIDEOSTANDARD_NTSC_44:    // SIG_NTSC_443: 1127
            case E_VIDEOSTANDARD_PAL_60:
                 u16CurrentHStart = 1127;
                 break;

            case E_VIDEOSTANDARD_PAL_M:      // SIG_PAL_M: 909
                 u16CurrentHStart = 909;
                 break;

            case E_VIDEOSTANDARD_PAL_N:      // SIG_PAL_NC: 917
                 u16CurrentHStart = 917;
                 break;

            case E_VIDEOSTANDARD_SECAM:      // SIG_SECAM: 1097
                 u16CurrentHStart = 1097;
                 break;

            default:
                 // ASSERT
                 u16CurrentHStart = 1135;
                 break;
        }

        pResource->_u16DataH[2] = pResource->_u16DataH[1];
        pResource->_u16DataH[1] = pResource->_u16DataH[0];
        pResource->_u16DataH[0] = u16Htotal;

        if( (pResource->_u16DataH[2] == pResource->_u16DataH[1]) && (pResource->_u16DataH[1] == pResource->_u16DataH[0]) )
        {
            if( pResource->_u8HtotalDebounce > 3 )
            {
                pResource->_u16LatchH = pResource->_u16DataH[0];
            }
            else
            {
                pResource->_u8HtotalDebounce++;
            }
        }
        else
        {
            pResource->_u8HtotalDebounce = 0;
        }

        u16Htotal = pResource->_u16LatchH;

        switch( pResource->_eVideoSystem )
        {
            case E_VIDEOSTANDARD_PAL_BGHI:  // SIG_PAL
            case E_VIDEOSTANDARD_NTSC_44:   // SIG_NTSC_443
                    u8Value = 3;
                    break;

            case E_VIDEOSTANDARD_PAL_M:     // SIG_PAL_M
            case E_VIDEOSTANDARD_PAL_N:     // SIG_PAL_NC
                    u8Value = 1;
                    break;

            default:                        // NTSC
                    u8Value = 2;
                    break;
        }

        if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NTSC_50)
        {
            if ( ((HAL_AVD_AFEC_GetStatus() & (VD_FSC_TYPE|VD_VSYNC_50HZ|VD_COLOR_LOCKED))
                == (VD_FSC_3579|VD_VSYNC_50HZ|VD_COLOR_LOCKED)) ) // not NTSC-50
                    u8Value = 2;
        }

        u8Ctl = ( u16Htotal - u8Value ) % 4;
        u8update =  1;
        if( u8Ctl == 3 )
        {
            u16Htotal = u16Htotal + 1;
        }
        else if( u8Ctl == 2 )
        {
            if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NTSC_50)
            {
                if ( ((HAL_AVD_AFEC_GetStatus() & (VD_FSC_TYPE|VD_VSYNC_50HZ|VD_COLOR_LOCKED))
                    == (VD_FSC_3579|VD_VSYNC_50HZ|VD_COLOR_LOCKED)) ) // not NTSC-50
                    u16Htotal = u16Htotal + 2;
            }
            else
            {
                u8update = 0;
            }
        }
        else
        {
            u16Htotal = u16Htotal - u8Ctl;
        }

        if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NTSC_50)
        {
            if ( ((HAL_AVD_AFEC_GetStatus() & (VD_FSC_TYPE|VD_VSYNC_50HZ|VD_COLOR_LOCKED))
                == (VD_FSC_3579|VD_VSYNC_50HZ|VD_COLOR_LOCKED)) ) // not NTSC-50
                u16CurrentHStart = 918;
        }

        if( u8update )
        {
            // HAL_AVD_COMB_SetHtotal( u16Htotal );
            HAL_AVD_COMB_SetHtotal(u16CurrentHStart);
        }

        if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_FINE_TUNE_FH_DOT)
        {
            #define FINE_TUNE_FH_DOT_MAX 20
            static MS_U8 u8FhDotDebouncer=0;
            u8Ctl = HAL_AVD_AFEC_GetNoiseMag(); // get VD noise magnitude

            if((((pResource->_eVDInputSource == E_INPUT_SOURCE_ATV) ||
                 (pResource->_eVDInputSource == E_INPUT_SOURCE_CVBS1) ||
                 (pResource->_eVDInputSource == E_INPUT_SOURCE_CVBS2) ||
                 ( pResource->_eVDInputSource == E_INPUT_SOURCE_CVBS3)) ||
                ((pResource->_eVDInputSource == E_INPUT_SOURCE_SCART1) ||
                 ( pResource->_eVDInputSource == E_INPUT_SOURCE_SCART2))) &&
               ((E_VIDEOSTANDARD_NTSC_M ==pResource->_eVideoSystem)||(E_VIDEOSTANDARD_PAL_BGHI==pResource->_eVideoSystem)))
            {
                if((abs(pResource->_u16LatchH-u16CurrentHStart)>=2)&&(u8Ctl<=((pResource->_eVDInputSource == E_INPUT_SOURCE_ATV) ? 2: 1)))
                {
                    if (u8FhDotDebouncer<FINE_TUNE_FH_DOT_MAX)
                    {
                        u8FhDotDebouncer++;
                    }
                }
                else if (u8FhDotDebouncer)
                {
                    u8FhDotDebouncer--;
                }
            }
            else
            {
                u8FhDotDebouncer=0;
            }

            if (u8FhDotDebouncer>=FINE_TUNE_FH_DOT_MAX)
            {
                pResource->u32VDPatchFlagStatus |= AVD_PATCH_FINE_TUNE_FH_DOT;
                HAL_AVD_COMB_SetNonStandardHtotal(TRUE);
            }
            else if (!u8FhDotDebouncer)
            {
                pResource->u32VDPatchFlagStatus &= ~AVD_PATCH_FINE_TUNE_FH_DOT;
                HAL_AVD_COMB_SetNonStandardHtotal(FALSE);
            }
            #if AVD_DRV_DEBUG
            if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
            {
                if (pResource->u32VDPatchFlagStatus & AVD_PATCH_FINE_TUNE_FH_DOT)
                    ULOGE("AVD","_u16LatchH,u16CurrentHStart,u8FhDotDebouncer= %d %d % d\n",pResource->_u16LatchH,u16CurrentHStart,u8FhDotDebouncer);
            }
            #endif
        }
    }


    // for Color Hysteresis
    if (pResource->_eVDInputSource == E_INPUT_SOURCE_ATV)
    {
        if ( (HAL_AVD_AFEC_GetStatus() & (VD_HSYNC_LOCKED | VD_STATUS_RDY | VD_COLOR_LOCKED))
            == (VD_HSYNC_LOCKED | VD_STATUS_RDY) )
        {
            HAL_AVD_AFEC_SetColorKillLevel(pResource->g_VD_InitData.u8ColorKillHighBound);
        }
        else
        {
            HAL_AVD_AFEC_SetColorKillLevel(pResource->g_VD_InitData.u8ColorKillLowBound);
        }
    }
    else
    {
        HAL_AVD_AFEC_SetColorKillLevel(pResource->g_VD_InitData.u8ColorKillLowBound);
    }

    // co-channel
    {
        if (HAL_AVD_AFEC_GetCoChannelOn())
        {
            HAL_AVD_VBI_SetTTSigDetSel(TRUE);
        }
        else
        {
            HAL_AVD_VBI_SetTTSigDetSel(FALSE);
        }
    }

    // For maintain the Hsync reliability and seneitivity
    {
        static MS_U8 u8DebounceCnt=0;

        if (!(HAL_AVD_AFEC_GetStatus() & (VD_HSYNC_LOCKED|VD_SYNC_LOCKED)))
        {
            if (u8DebounceCnt < 15)
            {
                u8DebounceCnt++;
            }
        }
        else
        {
            if (u8DebounceCnt > 0)
            {
                u8DebounceCnt--;
            }
        }

        if ((u8DebounceCnt >= 10) || pResource->_u8AutoTuningIsProgress)
        {
            HAL_AVD_AFEC_EnableBottomAverage(FALSE); // more sensitivity
        }
        else if (u8DebounceCnt <= 5)
        {
            HAL_AVD_AFEC_EnableBottomAverage(TRUE); // more reliability
        }
    }

    #if 0 // BY 20090403 Metis,Martina,T3,U3 fixed by hardware, removed
    {
        MS_U16 u16Status;
        u16Status=HAL_AVD_AFEC_GetStatus();
        if ( IS_BITS_SET(u16Status, VD_HSYNC_LOCKED|VD_SYNC_LOCKED|VD_STATUS_RDY) )
        {
            if ((u16Status&VD_FSC_TYPE)==VD_FSC_4285) //SECAM
            {
                RIU_WriteByteMask( BK_COMB_4A, 0x00, 0x01 ); // COMB_4A=0x56, James.Lu, 20080730
            }
        }
    }
    #endif

    {
        MS_U16 u16StandardVtotal;

        if (HAL_AVD_AFEC_GetStatus() & VD_VSYNC_50HZ)
        {
            u16StandardVtotal = 625;
        }
        else
        {
            u16StandardVtotal = 525;
        }

        // Patch for non-standard V freq issue (3D COMB) should use 2D comb
        if (abs(u16StandardVtotal - HAL_AVD_AFEC_GetVTotal()) >= 2)
        {
            HAL_AVD_COMB_Set3dComb(FALSE);
        }
        else
        {
            if(pResource->_b2d3dautoflag==1)
            {
               HAL_AVD_COMB_Set3dComb(TRUE);
            }
            else
            {
               HAL_AVD_COMB_Set3dComb(FALSE);
            }

            #ifdef AVD_COMB_3D_MID // BY 20090717 enable MID mode after enable 3D comb, if the sequence is wrong, there will be garbage
            if ((pResource->_eVDInputSource != E_INPUT_SOURCE_ATV) && (pResource->_eVDInputSource != E_INPUT_SOURCE_SVIDEO1) && (pResource->_eVDInputSource != E_INPUT_SOURCE_SVIDEO2))
            {
                if (( pResource->_eVideoSystem == E_VIDEOSTANDARD_PAL_BGHI ) || ( pResource->_eVideoSystem == E_VIDEOSTANDARD_NTSC_M ))
                {
                    if (!(pResource->u32VDPatchFlagStatus & AVD_PATCH_FINE_TUNE_FH_DOT))
                    {
                        HAL_AVD_COMB_Set3dCombMid(ENABLE);
                    }
                }
            }
            #endif
        }

        // patch for color stripe function abnormal at non-standard v, Vestel TG45 issue
        if (abs(u16StandardVtotal - HAL_AVD_AFEC_GetVTotal()) >= 9)
        {
            HAL_AVD_AFEC_SetColorStripe(0x31);
        }
        else
        {
            HAL_AVD_AFEC_SetColorStripe(0x40);
        }
    }

    // Fix Comb bug
    {
        if (HAL_AVD_AFEC_GetBurstOn())
        {
            HAL_AVD_COMB_SetHsyncTolerance(0x00); // Burst On
        }
        else
        {
            HAL_AVD_COMB_SetHsyncTolerance(0x20); // No Burst (for hsync tolerance)
        }
    }

#if 0
    if (g_VD_InitData.u32VDPatchFlag & AVD_PATCH_FINE_TUNE_FSC_SHIFT)
    {
        #define FINE_TUNE_FSC_SHIFT_CNT_STEP 3
        #define FINE_TUNE_FSC_SHIFT_CNT_MAX FINE_TUNE_FSC_SHIFT_CNT_STEP*7
        #define FINE_TUNE_FSC_SHIFT_CNT_THR FINE_TUNE_FSC_SHIFT_CNT_STEP*3
        static MS_U8 u8FscShiftDebounceCnt=0;

        if ((E_VIDEOSTANDARD_NTSC_M==_eVideoSystem)||(E_VIDEOSTANDARD_PAL_BGHI==_eVideoSystem) ||
            (E_VIDEOSTANDARD_PAL_M==_eVideoSystem) || (E_VIDEOSTANDARD_PAL_N==_eVideoSystem))
        {
            if ( !HAL_AVD_COMB_Get3dCombTimingCheck()) // got comb 3D unlocked
            {
                if (u8FscShiftDebounceCnt<FINE_TUNE_FSC_SHIFT_CNT_MAX)
                {
                    u8FscShiftDebounceCnt+=FINE_TUNE_FSC_SHIFT_CNT_STEP;
                }
            }
            else
            {
                if (u8FscShiftDebounceCnt)
                {
                    u8FscShiftDebounceCnt--;
                }
            }
        }
        else
        {
            u8FscShiftDebounceCnt = 0;
        }
        if (u8FscShiftDebounceCnt>=FINE_TUNE_FSC_SHIFT_CNT_THR)
        {
            u32VDPatchFlagStatus |= AVD_PATCH_FINE_TUNE_FSC_SHIFT;
            HAL_AVD_COMB_SetNonStandardFSC(TRUE,TRUE);
        }
        else if (!u8FscShiftDebounceCnt)
        {
            u32VDPatchFlagStatus &= ~AVD_PATCH_FINE_TUNE_FSC_SHIFT;
            HAL_AVD_COMB_SetNonStandardFSC((E_VIDEOSTANDARD_PAL_BGHI==_eVideoSystem), FALSE);
        }
    }
#endif

    {  // burst
        MS_BOOL bBurstOn;
        static MS_BOOL bPrevBurstOn=FALSE;

        bBurstOn = HAL_AVD_AFEC_GetBurstOn();
        if ( bBurstOn  )
        {
            if ( !bPrevBurstOn )
            {
                _Drv_AVD_3DCombSpeedup(pResource);
            }
        }
        #ifdef LOCK3DSPEEDUP_PERFORMANCE_1
        else
        {
            if ( !pResource->_u8Comb10Bit3Flag )
            {
                HAL_AVD_COMB_Set3dCombSpeed(0x14, 0x10, 0x88);
            }
        }
        #endif
        bPrevBurstOn=bBurstOn;

        if ( pResource->_u8Comb10Bit3Flag )
        {
            if ((pResource->_u8Comb10Bit3Flag&(BIT(0))) && (MsOS_GetSystemTime() -pResource->_u32VideoSystemTimer >3000))
            {
                //MDrv_WriteByteMask( BK_COMB_10, 0, BIT3 );
                HAL_AVD_COMB_Set3dCombSpeed(0x50, 0x20, 0x88);
                #if ADJUST_CHANNEL_CHANGE_HSYNC
                HAL_AVD_AFEC_SetSwingLimit(pResource->g_VD_InitData.u8SwingThreshold);
                HAL_AVD_AFEC_EnableBottomAverage(ENABLE); // more reliability
                #endif
                pResource->_u8Comb10Bit3Flag&=(~(BIT(0)));
                #if AVD_DRV_DEBUG
                if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
                {
                    ULOGE("AVD","Drv_AVD_3DCombSpeedup Disable\n");
                }
                #endif
            }
            if ((pResource->_u8Comb10Bit3Flag&(BIT(1))) && (MsOS_GetSystemTime() -pResource->_u32VideoSystemTimer >2000))
            {
                if (!(pResource->_u8AfecD4Factory & BIT(4)))
                {
                    HAL_AVD_AFEC_EnableVBIDPLSpeedup(DISABLE);
                }
                pResource->_u8Comb10Bit3Flag&=(~(BIT(1)));
            }
            if ((pResource->_u8Comb10Bit3Flag&(BIT(2))) && (MsOS_GetSystemTime() -pResource->_u32VideoSystemTimer >400))
            {
                HAL_AVD_SetReg(BK_AFEC_A0, HAL_AVD_GetReg(BK_AFEC_A0)&(~BIT(7)));
                pResource->_u8Comb10Bit3Flag&=(~(BIT(2)));
            }
        }
    }

    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        if (pResource->u32PreVDPatchFlagStatus != pResource->u32VDPatchFlagStatus)
        {
            ULOGE("AVD","u32VDPatchFlagStatus %x\n",(unsigned int)pResource->u32VDPatchFlagStatus);
            pResource->u32PreVDPatchFlagStatus=pResource->u32VDPatchFlagStatus;
        }
    }
    #endif
    #if 0
    if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NON_STANDARD_VTOTAL)
    {
        _Drv_AVD_SyncRangeHandler(pResource);
    }
    #endif
    if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_FINE_TUNE_STILL_IMAGE)
    {
        _Drv_AVD_COMB_StillImage(pResource);
    }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

MS_BOOL Drv_AVD_SetInput(AVD_InputSourceType eSource, MS_U8 u8ScartFB, AVD_RESOURCE_PRIVATE *pResource) //SCART_FB eFB)
{
    VD_HSYNC_SENSITIVITY eVDHsyncSensitivityforAV;
    //Basic Input Checking
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_SetInput %d\n",eSource);
    }
    #endif
    if (eSource >= E_INPUT_SOURCE_MAX)
        MS_ASSERT(FALSE);

    eSource= (AVD_InputSourceType)(((MS_U8)eSource) & 0x0F);
    pResource->_eVDInputSource=eSource;

    // store source type information for STR function
    eSource_Str = eSource;
    u8ScartFB_Str = u8ScartFB;
    eDemodType_Str = pResource->g_VD_InitData.eDemodType;
    u32XTAL_Clock_Str = pResource->g_VD_InitData.u32XTAL_Clock;

    HAL_AVD_AFEC_SetInput(eSource,u8ScartFB,pResource->g_VD_InitData.eDemodType, pResource->g_VD_InitData.u32XTAL_Clock);
    //HAL_AVD_COMB_SetMemoryRequest(ENABLE);

  ///Brian A1 VIF and ADC different AGC mapping
     #if(TWO_VD_DSP_CODE)// weicheng 20121003
    /*
            if(eSource==E_INPUT_SOURCE_ATV)
              {
                 HAL_AVD_SetDSPCodeType(AVD_DSP_CODE_TYPE_VIF);
              }
          else
            {
            HAL_AVD_SetDSPCodeType(AVD_DSP_CODE_TYPE_ADC);
            }
    */
           _Drv_AVD_VDMCU_Init(pResource);
     #endif

    if(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_CVBS_NEGATIVESIG)   //20130115 dia.chiu for TCL CVBS negative signal unlock
    {
        if((eSource == E_INPUT_SOURCE_CVBS1)||(eSource == E_INPUT_SOURCE_CVBS2)||(eSource == E_INPUT_SOURCE_CVBS3))
        {
            eVDHsyncSensitivityforAV.u8DetectWinAfterLock= 0x01;
            eVDHsyncSensitivityforAV.u8DetectWinBeforeLock= 0x01;
            eVDHsyncSensitivityforAV.u8CNTRFailBeforeLock = 0x01;
            eVDHsyncSensitivityforAV.u8CNTRSyncBeforeLock = 0x3E;
            eVDHsyncSensitivityforAV.u8CNTRSyncAfterLock = 0x00;
            HAL_AVD_AFEC_SetHsyncSensitivity(eVDHsyncSensitivityforAV);
        }
        else
        {
             HAL_AVD_AFEC_SetHsyncSensitivity(pResource->g_VD_InitData.eVDHsyncSensitivityNormal);
        }
    }

    // set gain
    if (eSource == E_INPUT_SOURCE_ATV)
    {
        if (pResource->g_VD_InitData.bRFGainSel==VD_USE_FIX_GAIN)
        {
            HAL_AVD_AFEC_AGCSetMode(AVD_AGC_DISABLE);

            if(pResource->g_VD_InitData.eDemodType == DEMODE_MSTAR_VIF)
            {
                HAL_AVD_AFEC_AGCSetCoarseGain(VD_AGC_COARSE_GAIN);
                HAL_AVD_AFEC_AGCSetFineGain(pResource->g_VD_InitData.u8RFGain);
            }
            else
            {
                HAL_AVD_AFEC_AGCSetCoarseGain(VD_AGC_COARSE_GAIN);
                HAL_AVD_AFEC_AGCSetFineGain(pResource->g_VD_InitData.u8RFGain);
            }
        }
        else
        {
            HAL_AVD_AFEC_AGCSetMode(AVD_AGC_ENABLE);
        }
    }
    else
    {
        if (pResource->g_VD_InitData.bAVGainSel==VD_USE_FIX_GAIN)
        {
            HAL_AVD_AFEC_AGCSetMode(AVD_AGC_DISABLE);
            HAL_AVD_AFEC_AGCSetCoarseGain(VD_AGC_COARSE_GAIN);
            HAL_AVD_AFEC_AGCSetFineGain(pResource->g_VD_InitData.u8AVGain);
        }
        else
        {
            HAL_AVD_AFEC_AGCSetMode(AVD_AGC_ENABLE);
        }
        //20120719 Brian Leave ATV source, disable the shift clk monitor
        pResource->_bShiftClkFlag = FALSE;
    }

    // VD MCU Reset
    // BY 20090410, move function body here  MDrv_AVD_McuReset();
    #if T3_LOAD_CODE //weicheng 20121003
    _Drv_AVD_VDMCU_Init(); // must reload code for T3
    #else
    HAL_AVD_AFEC_McuReset();
    #endif
    HAL_AVD_AFEC_SetPatchFlag(pResource->g_VD_InitData.u32VDPatchFlag);
    // Forced to PAL mode
    pResource->_eLastStandard = E_VIDEOSTANDARD_NOTSTANDARD;
    pResource->_eVideoSystem=E_VIDEOSTANDARD_PAL_BGHI;
    pResource->_u16CurVDStatus = 0;
    HAL_AVD_COMB_SetMemoryRequest(ENABLE);
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    return TRUE;
}

MS_BOOL Drv_AVD_SetVideoStandard(AVD_VideoStandardType eStandard, MS_BOOL bIsInAutoTuning, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_SetVideoStandard %d\n",eStandard);
    }
    #endif

    pResource->_eVideoSystem = eStandard;

   //Check input
   if (eStandard>=E_VIDEOSTANDARD_MAX)
        MS_ASSERT(FALSE);

    switch (eStandard)
    {
    case E_VIDEOSTANDARD_NTSC_44:
        if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
        {
            HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
        }
        else
        {
            HAL_AVD_AFEC_SetHTotal(_u16HtotalNTSC_443[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8]* (VD_USE_FB + 1));
        }

        HAL_AVD_AFEC_SetBT656Width(0xB4);
        pResource->_u16LatchH = 1127L;
        pResource->_u16CurVDStatus= VD_FSC_4433 | VD_STATUS_RDY;
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_NTSC_443);
        break;
    case E_VIDEOSTANDARD_PAL_60:    // 1127
        if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
        {
            HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
        }
        else
        {
            HAL_AVD_AFEC_SetHTotal(_u16HtotalPAL_60[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
        }
        HAL_AVD_AFEC_SetBT656Width(0xB4);
        pResource->_u16LatchH = 1127L;
        pResource->_u16CurVDStatus= VD_PAL_SWITCH | VD_FSC_4433 | VD_STATUS_RDY;
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL_60);
        break;
    case E_VIDEOSTANDARD_SECAM:
        if ( bIsInAutoTuning )
        {
            if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
            {
                HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
            }
            else
            {
                HAL_AVD_AFEC_SetHTotal(_u16HtotalPAL[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
            }
        }
        else
        {
            if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
            {
                HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
            }
            else
            {
                HAL_AVD_AFEC_SetHTotal(_u16HtotalSECAM[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
            }
        }

        HAL_AVD_AFEC_SetBT656Width(0xB2);
        pResource->_u16LatchH = 1097L;
        pResource->_u16CurVDStatus = (VD_VSYNC_50HZ | VD_FSC_4285 | VD_STATUS_RDY);
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_SECAM);
        break;

    case E_VIDEOSTANDARD_PAL_M: // 909
        if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
        {
            HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
        }
        else
        {
            HAL_AVD_AFEC_SetHTotal(_u16HtotalPAL_M[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
        }
        HAL_AVD_AFEC_SetBT656Width(0x8E);
        pResource->_u16LatchH = 909L;
        pResource->_u16CurVDStatus = (VD_PAL_SWITCH | VD_FSC_3575 | VD_STATUS_RDY);
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL_M);
        break;

    case E_VIDEOSTANDARD_PAL_N: // 917
        if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
        {
            HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
        }
        else
        {
            HAL_AVD_AFEC_SetHTotal(_u16HtotalPAL_NC[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
        }
        HAL_AVD_AFEC_SetBT656Width(0x93);
        pResource->_u16LatchH = 917L;
        pResource->_u16CurVDStatus = (VD_VSYNC_50HZ | VD_PAL_SWITCH | VD_FSC_3582 | VD_STATUS_RDY);
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL_N);
        break;

    case E_VIDEOSTANDARD_NTSC_M: // 910
        if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
        {
            HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
        }
        else
        {
            HAL_AVD_AFEC_SetHTotal(_u16HtotalNTSC[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
        }
        HAL_AVD_AFEC_SetBT656Width(0x6D);
        pResource->_u16LatchH = 910L;
        pResource->_u16CurVDStatus = (VD_FSC_3579 | VD_STATUS_RDY);
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_NTSC);
        break;
    default: // case E_VIDEOSTANDARD_PAL_BGHI:  // 1135
        if((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK) == AVD_PATCH_HTOTAL_USER)
        {
            HAL_AVD_AFEC_SetHTotal(pResource->_u16Htt_UserMD);
        }
        else
        {
            HAL_AVD_AFEC_SetHTotal(_u16HtotalPAL[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8] * (VD_USE_FB + 1));
        }
        HAL_AVD_AFEC_SetBT656Width(0xB6);
        pResource->_u16LatchH = 1135L;
        pResource->_u16CurVDStatus = (VD_VSYNC_50HZ | VD_PAL_SWITCH | VD_FSC_4433 | VD_STATUS_RDY);
        HAL_AVD_AFEC_SetFSCMode(FSC_MODE_PAL);
        break;
    }

    if ((pResource->_eVDInputSource == E_INPUT_SOURCE_SVIDEO1) || (pResource->_eVDInputSource == E_INPUT_SOURCE_SVIDEO2))
    {
        if (E_VIDEOSTANDARD_NTSC_M == eStandard)
        {
            HAL_AVD_COMB_SetYCPipe(0x20);
        }
        else // SIG_NTSC_443, SIG_PAL, SIG_PAL_M, SIG_PAL_NC, SIG_SECAM
        {
            HAL_AVD_COMB_SetYCPipe(0x30);
        }

        switch (eStandard)
        {
        case E_VIDEOSTANDARD_PAL_M:
        case E_VIDEOSTANDARD_PAL_N:
            HAL_AVD_COMB_SetCbCrInverse(0x04);
            break;

        case E_VIDEOSTANDARD_NTSC_44:
            HAL_AVD_COMB_SetCbCrInverse(0x0C);
            break;

        case E_VIDEOSTANDARD_PAL_60:
            HAL_AVD_COMB_SetCbCrInverse(0x08);
            break;

        case E_VIDEOSTANDARD_NTSC_M:
        case E_VIDEOSTANDARD_SECAM:
            HAL_AVD_COMB_SetCbCrInverse(0x00);
            break;

        case E_VIDEOSTANDARD_PAL_BGHI:
        default:
            HAL_AVD_COMB_SetCbCrInverse(0x08);
            break;
        }
    }
    else
    {
        if(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_SCART_SVIDEO)
        {
            if((pResource->_eVDInputSource!=E_INPUT_SOURCE_SCART1)&&(pResource->_eVDInputSource!=E_INPUT_SOURCE_SCART2))
            {
                HAL_AVD_COMB_SetYCPipe(0x20);
                HAL_AVD_COMB_SetCbCrInverse(0x00);
            }
        }
        else
        {
            HAL_AVD_COMB_SetYCPipe(0x20);
            HAL_AVD_COMB_SetCbCrInverse(0x00);
        }
    }

    if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NTSC_50)
    {
        if ((HAL_AVD_AFEC_GetStatus() & (VD_FSC_TYPE|VD_VSYNC_50HZ|VD_COLOR_LOCKED))
              == (VD_FSC_3579|VD_VSYNC_50HZ|VD_COLOR_LOCKED)) // NTSC-50
        {
            HAL_AVD_COMB_SetVerticalTimingDetectMode(0x02);
        }
        else
        {
            HAL_AVD_COMB_SetVerticalTimingDetectMode(0x00);
        }
    }

    if (eStandard == E_VIDEOSTANDARD_NTSC_44)
    {
        HAL_AVD_COMB_SetLineBufferMode(0x06);
        HAL_AVD_COMB_SetHtotal(0x467);
    }
    else if ((pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NTSC_50) && ((HAL_AVD_AFEC_GetStatus() & (VD_FSC_TYPE|VD_VSYNC_50HZ|VD_COLOR_LOCKED))
               == (VD_FSC_3579|VD_VSYNC_50HZ|VD_COLOR_LOCKED))) // NTSC-50
    {
        HAL_AVD_COMB_SetLineBufferMode(0x06);
        HAL_AVD_COMB_SetHtotal(0x396);
    }
    else
    {
        HAL_AVD_COMB_SetLineBufferMode(0x07);
        HAL_AVD_COMB_SetHtotal(0x38D);
    }

    if (eStandard == E_VIDEOSTANDARD_SECAM)
    {
        HAL_AVD_VBI_SetVPSPhaseAcc(0x9A6D);
    }
    else
    {
        HAL_AVD_VBI_SetVPSPhaseAcc(0x018C);
    }

    {
        if (HAL_AVD_AFEC_GetBurstOn())
        {
            _Drv_AVD_3DCombSpeedup(pResource);
        }
    }

#if 0
    if (g_VD_InitData.u32VDPatchFlag & AVD_PATCH_FINE_TUNE_COMB_F2) //bug's life, for Eris ATT, James.Lu, 20080327
    {

        if(((_eVDInputSource == E_INPUT_SOURCE_CVBS1) ||
            (_eVDInputSource == E_INPUT_SOURCE_CVBS2) ||
            ( _eVDInputSource == E_INPUT_SOURCE_CVBS3)) && (eStandard == E_VIDEOSTANDARD_NTSC_M))
        {
            HAL_AVD_COMB_SetF2(0x57);
        }
        else
        {
            HAL_AVD_COMB_SetF2(0x47);
        }
    }


    if (g_VD_InitData.u32VDPatchFlag & AVD_PATCH_FINE_TUNE_3D_COMB)
    {
        HAL_AVD_COMB_Set3dFineTune((eStandard==E_VIDEOSTANDARD_PAL_BGHI));
    }


    {
        switch (eStandard)
        {
            case E_VIDEOSTANDARD_PAL_BGHI:
                HAL_AVD_COMB_Set3dDetectionTolerance(0x06);
                break;
            case E_VIDEOSTANDARD_PAL_M:
                HAL_AVD_COMB_Set3dDetectionTolerance(0x04);
                break;
            default:
                HAL_AVD_COMB_Set3dDetectionTolerance(0x05);
                break;
        }
    }
#endif

    if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NON_STANDARD_VTOTAL) //bug's life, for Eris ATT, James.Lu, 20080327
    {
#ifndef MSOS_TYPE_LINUX
        if(
            ((pResource->_eVDInputSource == E_INPUT_SOURCE_CVBS1) ||
            (pResource->_eVDInputSource == E_INPUT_SOURCE_CVBS2) ||
            (pResource->_eVDInputSource == E_INPUT_SOURCE_CVBS3) ||
            ( pResource->_eVDInputSource == E_INPUT_SOURCE_ATV)) &&
            (eStandard!=E_VIDEOSTANDARD_NOTSTANDARD)
            )
        {
            MS_U16 u16Vtotal=HAL_AVD_AFEC_GetVTotal();
            if ((u16Vtotal < 675 ) && (u16Vtotal > 570))
                pResource->u16PreVtotal = 625;
            else
                pResource->u16PreVtotal = 525;
        }
        else
        {
            pResource->u16PreVtotal = 0;
        }
#endif
    }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    return TRUE;
}

void Drv_AVD_SetChannelChange(AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","Drv_AVD_SetChannelChange\n");
    }
    #endif
    HAL_AVD_AFEC_SetChannelChange();
    //MDrv_AVD_3DCombSpeedup();
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_SetHsyncDetectionForTuning(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_SetHsyncDetectionForTuning %d\n",bEnable);
    }
    #endif

    if (bEnable)    // for Auto Scan Mode
    {
        HAL_AVD_AFEC_SetHsyncSensitivity(pResource->g_VD_InitData.eVDHsyncSensitivityTuning);
        HAL_AVD_AFEC_SetSwingLimit(0);
        HAL_AVD_AFEC_EnableBottomAverage(DISABLE); // more sensitivity
        pResource->_u8AutoTuningIsProgress = TRUE;
    }
    else            // for Normal Mode
    {
        HAL_AVD_AFEC_SetHsyncSensitivity(pResource->g_VD_InitData.eVDHsyncSensitivityNormal);
        HAL_AVD_AFEC_SetSwingLimit(pResource->g_VD_InitData.u8SwingThreshold);
        HAL_AVD_AFEC_EnableBottomAverage(ENABLE); // more reliability
        pResource->_u8AutoTuningIsProgress = FALSE;
    }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_Set3dComb(MS_BOOL bEnable, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    // it's ok to enable/disable 3D comb for svideo input
    if(bEnable)
    {
        pResource->_b2d3dautoflag=1;
    }
    else
    {
        pResource->_b2d3dautoflag=0;
    }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_SetShiftClk(MS_BOOL bEnable,AVD_ATV_CLK_TYPE eShiftMode, AVD_RESOURCE_PRIVATE *pResource)
{
    if( (pResource->_eVDInputSource == E_INPUT_SOURCE_ATV) && (pResource->g_VD_InitData.eDemodType == DEMODE_MSTAR_VIF))
    {
        if(pResource->_u8AutoTuningIsProgress == TRUE)
        {
            eShiftMode=E_ATV_CLK_ORIGIN_43P2MHZ;
        }
        if((bEnable == FALSE)  ||  (eShiftMode == E_ATV_CLK_ORIGIN_43P2MHZ))
        {
            pResource->_bShiftClkFlag = FALSE;
            HAL_AVD_ShiftClk(eShiftMode, E_VIDEOSTANDARD_AUTO,pResource->g_VD_InitData.u32XTAL_Clock);
        }
        else
        {
            pResource->_bShiftClkFlag = bEnable;
        }
        pResource->gShiftMode = eShiftMode;
    }
    else
    {
        pResource->_bShiftClkFlag = FALSE;
    }
}

void Drv_AVD_SetFreerunPLL(AVD_VideoFreq eVideoFreq, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    if( eVideoFreq == E_VIDEO_FQ_60Hz )
    {
        HAL_AVD_COMB_SetHtotal((MS_U16)0x38E);
    }
    else if( eVideoFreq == E_VIDEO_FQ_50Hz )
    {
        HAL_AVD_COMB_SetHtotal((MS_U16)0x46F);
    }
    else if( eVideoFreq == E_VIDEO_FQ_NOSIGNAL )
    {
        //Do nothing
    }else
    {
        MS_ASSERT(FALSE);
     }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_SetFreerunFreq(AVD_FreeRunFreq eFreerunfreq, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    if (eFreerunfreq >= E_FREERUN_FQ_MAX)
        MS_ASSERT(FALSE);

    switch(eFreerunfreq)
    {
        case E_FREERUN_FQ_AUTO:
            HAL_AVD_AFEC_SetVtotal(0x00);
            break;

        case E_FREERUN_FQ_50Hz:
            HAL_AVD_AFEC_SetVtotal(0x01);
            break;

        case E_FREERUN_FQ_60Hz:
            HAL_AVD_AFEC_SetVtotal(0x03);
            break;

        default:
            break;
    }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_SetFactoryPara(AVD_Factory_Para FactoryPara,MS_U8 u8Value, AVD_RESOURCE_PRIVATE *pResource)
{
    MS_U8 u8tmp;

    switch(FactoryPara)
    {
       case E_FACTORY_PARA_AFEC_D4:
           HAL_AVD_SetReg(BK_AFEC_D4,u8Value);
           break;
       case E_FACTORY_PARA_AFEC_D8:
           HAL_AVD_SetReg(BK_AFEC_D8,u8Value);
           break;
       case E_FACTORY_PARA_AFEC_D5_BIT2:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_D5);
           u8tmp &= (~BIT(2));
           HAL_AVD_SetReg(BK_AFEC_D5,u8tmp |(u8Value & BIT(2)));
           break;
       case E_FACTORY_PARA_AFEC_D9_BIT0:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_D9);
           u8tmp &= (~BIT(0));
           HAL_AVD_SetReg(BK_AFEC_D9,u8tmp |(u8Value & BIT(0)));
           break;
       case E_FACTORY_PARA_AFEC_A0:
           HAL_AVD_SetReg(BK_AFEC_A0,u8Value);
           break;
       case E_FACTORY_PARA_AFEC_A1:
           HAL_AVD_SetReg(BK_AFEC_A1,u8Value);
           break;
       case E_FACTORY_PARA_AFEC_66_BIT67:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_66);
           u8tmp &= (~(BIT(6)|BIT(7)));
           HAL_AVD_SetReg(BK_AFEC_66,u8tmp |(u8Value & (BIT(6)|BIT(7))));
           break;
       case E_FACTORY_PARA_AFEC_6E_BIT7654:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_6E);
           u8tmp &= 0x0F;
           HAL_AVD_SetReg(BK_AFEC_6E,u8tmp |(u8Value & 0xF0));
           break;
       case E_FACTORY_PARA_AFEC_6E_BIT3210:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_6E);
           u8tmp &= 0XF0;
           HAL_AVD_SetReg(BK_AFEC_6E,u8tmp |(u8Value & 0x0F));
           break;
       case E_FACTORY_PARA_AFEC_43:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_43);
           u8tmp &= (~(BIT(5)|BIT(6)));
           HAL_AVD_SetReg(BK_AFEC_43,u8tmp |(u8Value & (BIT(5)|BIT(6))));
           u8tmp = HAL_AVD_GetReg(BK_AFEC_55);
           HAL_AVD_SetReg(BK_AFEC_55,u8tmp|BIT(2));
           MsOS_DelayTask(2);
           HAL_AVD_SetReg(BK_AFEC_55,u8tmp &(~BIT(2)));
           break;
       case E_FACTORY_PARA_AFEC_44:
           HAL_AVD_SetReg(BK_AFEC_44,u8Value);
           HAL_AVD_AFEC_AGCSetMode(AVD_AGC_DISABLE);
           break;
       case E_FACTORY_PARA_AFEC_CB:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_CB);
           u8tmp &= BIT(7);
           HAL_AVD_SetReg(BK_AFEC_CB,u8tmp|(u8Value & 0x7F)); //except BIT7
           break;
       case E_FACTORY_PARA_AFEC_CF_BIT2:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_CF);
           u8tmp &= (~BIT(2));
           HAL_AVD_SetReg(BK_AFEC_CF,u8tmp |(u8Value & BIT(2)));
           break;
       case E_FACTORY_PARA_AFEC_D5_BIT3:
           u8tmp = HAL_AVD_GetReg(BK_AFEC_D5);
           u8tmp &= (~BIT(3));
           HAL_AVD_SetReg(BK_AFEC_D5,u8tmp |(u8Value & BIT(3)));
           break;
       case E_FACTORY_PARA_AFEC_D7_HIGH:
           pResource->g_VD_InitData.u8ColorKillHighBound = u8Value;
           break;
       case E_FACTORY_PARA_AFEC_D7_LOW:
           pResource->g_VD_InitData.u8ColorKillLowBound = u8Value;
           break;
       default:
           ULOGE("AVD","ERROR!!! Invalid Factory Parameter!\n");
           break;
    }
}

MS_BOOL Drv_AVD_SetDbgLevel(AVD_DbgLv u8DbgLevel, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    pResource->_u8AVDDbgLevel = u8DbgLevel;
    #endif
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    return TRUE;
}

void Drv_AVD_SetPQFineTune(AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    HAL_AVD_SetPQFineTune();
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_Set3dCombSpeed(MS_U8 u8COMB57, MS_U8 u8COMB58, AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    pResource->_u8Comb57 = u8COMB57;
    pResource->_u8Comb58 = u8COMB58;
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

void Drv_AVD_SetStillImageParam(AVD_Still_Image_Param param, AVD_RESOURCE_PRIVATE *pResource)
{
    pResource->g_stillImageParam.bMessageOn   = param.bMessageOn;

    pResource->g_stillImageParam.u8Str1_COMB37 = param.u8Str1_COMB37;
    pResource->g_stillImageParam.u8Str1_COMB38 = param.u8Str1_COMB38;
    pResource->g_stillImageParam.u8Str1_COMB7C = param.u8Str1_COMB7C;
    pResource->g_stillImageParam.u8Str1_COMBED = param.u8Str1_COMBED;

    pResource->g_stillImageParam.u8Str2_COMB37 = param.u8Str2_COMB37;
    pResource->g_stillImageParam.u8Str2_COMB38 = param.u8Str2_COMB38;
    pResource->g_stillImageParam.u8Str2_COMB7C = param.u8Str2_COMB7C;
    pResource->g_stillImageParam.u8Str2_COMBED = param.u8Str2_COMBED;

    pResource->g_stillImageParam.u8Str3_COMB37 = param.u8Str3_COMB37;
    pResource->g_stillImageParam.u8Str3_COMB38 = param.u8Str3_COMB38;
    pResource->g_stillImageParam.u8Str3_COMB7C = param.u8Str3_COMB7C;
    pResource->g_stillImageParam.u8Str3_COMBED = param.u8Str3_COMBED;

    pResource->g_stillImageParam.u8Threshold1 = param.u8Threshold1;
    pResource->g_stillImageParam.u8Threshold2 = param.u8Threshold2;
    pResource->g_stillImageParam.u8Threshold3 = param.u8Threshold3;
    pResource->g_stillImageParam.u8Threshold4 = param.u8Threshold4;
}

void Drv_AVD_Set2D3DPatchOnOff(MS_BOOL bEnable)
{
    HAL_AVD_Set2D3DPatchOnOff(bEnable);
    ULOGE("AVD","Patch %s \r\n",(bEnable)?"On" : "Off" );
}

MS_U8 Drv_AVD_SetAutoFineGainToFixed(void)
{
#if 1
    // Keep the function but do nothing, for compatibility.
    return 0;
#else
    MS_U8 u8CoaseGain,u8LPFGain, u8temp;
    MS_U16 u16FineGain;

    AVD_LOCK();

    HAL_AVD_AFEC_AGCSetMode(AVD_AGC_ENABLE);//auto gain
    MsOS_DelayTask(400);

    //printf("0x353F:%x\r\n",HAL_AVD_GetReg(0x353F));
    //printf("0x3544:%x\r\n",HAL_AVD_GetReg(0x3544));
    //printf("0x355F:%x\r\n",HAL_AVD_GetReg(0x355F));
    //printf("0x3540:%x\r\n",HAL_AVD_GetReg(0x3540));

    u8LPFGain = ((MDrv_AVD_DSPReadByte(2, 0x81)&BIT(4))>>4); //DSP_81[4], LPF gain
    u8CoaseGain = MDrv_AVD_DSPReadByte(2, 0x80)&0x0F;        //DSP_80[3:0], coarse gain
    //Read AGC fine-gain data DSP_80[11:4]
    u16FineGain = ((MDrv_AVD_DSPReadByte(2, 0x80)&0xF0)>>4)+((MDrv_AVD_DSPReadByte(2, 0x81)&0x0F)<<4);

    //printf("u8LPFGain:%x\r\n",u8LPFGain);
    //printf("u8CoaseGain:%x\r\n",u8CoaseGain);
    //printf("u16FineGain:%x\r\n",u16FineGain);

    u8temp = HAL_AVD_GetReg(BK_AFEC_3F)& 0xF0;
    HAL_AVD_SetReg(BK_AFEC_3F, u8CoaseGain | u8temp);  //AFEC_3F[3:0], coarse gain

    HAL_AVD_SetReg(BK_AFEC_44, (u16FineGain)& 0xFF);//AFEC_44[7:0], fine gain

    u8temp = HAL_AVD_GetReg(BK_AFEC_40);
    u8temp &= (~ BIT(4));
    HAL_AVD_SetReg(BK_AFEC_40,u8temp |(u8LPFGain & BIT(2))); //AFEC_40[4]LPF gain

    //printf("0x353F:%x\r\n",HAL_AVD_GetReg(0x353F));
    //printf("0x3544:%x\r\n",HAL_AVD_GetReg(0x3544));
    //printf("0x355F:%x\r\n",HAL_AVD_GetReg(0x355F));
    //printf("0x3540:%x\r\n",HAL_AVD_GetReg(0x3540));

    MsOS_DelayTask(10);

    HAL_AVD_AFEC_AGCSetMode(AVD_AGC_DISABLE);//fixed gain

    AVD_UNLOCK();

    return (u16FineGain>>6);
#endif
}

MS_U8 Drv_AVD_GetRegValue(MS_U16 u16Addr)
{
    return HAL_AVD_GetReg( u16Addr );
}

MS_U16 Drv_AVD_GetStatus(void)
{
    return HAL_AVD_AFEC_GetStatus();
}

MS_U8 Drv_AVD_GetNoiseMag(void)
{
    return HAL_AVD_AFEC_GetNoiseMag();
}

MS_U16 Drv_AVD_GetVTotal(void)
{
    return HAL_AVD_AFEC_GetVTotal();
}

AVD_VideoStandardType Drv_AVD_GetStandardDetection(MS_U16 *u16LatchStatus, AVD_RESOURCE_PRIVATE *pResource)
{
    MS_U16 u16Status;
    AVD_VideoStandardType eVideoStandard;

    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    *u16LatchStatus = u16Status = HAL_AVD_AFEC_GetStatus();


    if ( !IS_BITS_SET(u16Status, VD_HSYNC_LOCKED|VD_SYNC_LOCKED|VD_STATUS_RDY) )
    {
        pResource->_eLastStandard=E_VIDEOSTANDARD_NOTSTANDARD;
        AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
        return E_VIDEOSTANDARD_NOTSTANDARD;
    }


    if (pResource->_u8AutoDetMode==FSC_AUTO_DET_DISABLE)
    {
        eVideoStandard = pResource->_eForceVideoStandard;
    }
    else
    {
        if (u16Status & VD_BURST_ON)
        {
            switch (u16Status & VD_FSC_TYPE)
            {
                case VD_FSC_4433: // (FSC_MODE_PAL << 5):
                    HAL_AVD_ADC_SetGMC(0x06);
                    #if (!TEST_VD_DSP)
                    if (u16Status & VD_VSYNC_50HZ) // 50 Hz
                    #endif
                    {
                        eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
                    }
                    #if (!TEST_VD_DSP)
                    else // 60MHz
                    {
                        if (u16Status & VD_PAL_SWITCH)
                        {
                            eVideoStandard = E_VIDEOSTANDARD_PAL_60; // or vsdNTSC_44
                        }
                        else
                        {
                            eVideoStandard = E_VIDEOSTANDARD_NTSC_44;
                        }
                    }
                    #endif
                    break;

                case VD_FSC_3579:
                    if (pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_NTSC_50)
                    {
                         if ( u16Status & VD_VSYNC_50HZ ) // 50Hz
                         {
                             // 3.579545MHz, NTSC-50
                             // glock RIU_WriteByteMask(L_BK_ADC_ATOP(0x37), 0x07, 0x07);
                             eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
                         }
                         else
                        {
                             // 3.579545MHz, NTSC
                             HAL_AVD_ADC_SetGMC(0x07);
                             eVideoStandard = E_VIDEOSTANDARD_NTSC_M;
                        }
                    }
                    else
                    {
                        HAL_AVD_ADC_SetGMC(0x07);
                        eVideoStandard = E_VIDEOSTANDARD_NTSC_M;
                    }
                    break;

                case VD_FSC_3575: // (FSC_MODE_PAL_M << 5):
                    HAL_AVD_ADC_SetGMC(0x07);
                    eVideoStandard = E_VIDEOSTANDARD_PAL_M;
                    break;

                case VD_FSC_3582: // (FSC_MODE_PAL_N << 5):
                    HAL_AVD_ADC_SetGMC(0x07);
                    eVideoStandard = E_VIDEOSTANDARD_PAL_N;
                    break;

                #if (TEST_VD_DSP)
                case (FSC_MODE_NTSC_443 << 5):
                    eVideoStandard = E_VIDEOSTANDARD_NTSC_44;
                    break;

                case (FSC_MODE_PAL_60 << 5):
                    eVideoStandard = E_VIDEOSTANDARD_PAL_60;
                    break;
                #endif

                default:
                    eVideoStandard = E_VIDEOSTANDARD_NOTSTANDARD;
                    break;
            }
        }
        else
        {
            if (u16Status & VD_VSYNC_50HZ)
            {
                if ((u16Status & VD_FSC_TYPE) ==  VD_FSC_4285)
                {
                    HAL_AVD_ADC_SetGMC(0x06);
                    // if (u16Status & VD_VSYNC_50HZ) // SECAM must 50 Hz
                    {
                        eVideoStandard = E_VIDEOSTANDARD_SECAM;
                    }
                }
                else
                {
                    HAL_AVD_ADC_SetGMC(0x06);
                    eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
                }
            }
            else
            {
                HAL_AVD_ADC_SetGMC(0x07);
                eVideoStandard = E_VIDEOSTANDARD_NTSC_M;
            }
        }
    }

    pResource->_eLastStandard=eVideoStandard;

    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    return eVideoStandard;
}

void Drv_AVD_GetCaptureWindow(void *stCapWin, AVD_VideoStandardType eVideoStandardType , AVD_RESOURCE_PRIVATE *pResource)
{
    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
    #if AVD_DRV_DEBUG
    if (pResource->_u8AVDDbgLevel == AVD_DBGLV_DEBUG)
    {
        ULOGE("AVD","MDrv_AVD_GetCaptureWindow %ld %d\n",(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8, _u16HtotalPAL[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8]);
    }
    #endif
    switch (eVideoStandardType)
    {
        case E_VIDEOSTANDARD_PAL_BGHI:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalPAL[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+432)/864);
            break;
        case E_VIDEOSTANDARD_NTSC_M:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalNTSC[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+429)/858);
            break;
        case E_VIDEOSTANDARD_SECAM:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalSECAM[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+432)/864);
            break;
        case E_VIDEOSTANDARD_NTSC_44:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalNTSC_443[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+432)/864);
            break;
        case E_VIDEOSTANDARD_PAL_M:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalPAL_M[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+429)/858);
            break;
        case E_VIDEOSTANDARD_PAL_N:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalPAL_NC[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+429)/858);
            break;
        case E_VIDEOSTANDARD_PAL_60:
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalPAL_60[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+432)/864);
            break;
        default :
                ((MS_WINDOW_TYPE *)stCapWin)->width=(((MS_U32)720*(_u16HtotalPAL[(pResource->g_VD_InitData.u32VDPatchFlag & AVD_PATCH_HTOTAL_MASK)>>8])+432)/864);
            break;
    }
    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);
}

AVD_VideoFreq Drv_AVD_GetVerticalFreq(void)
{
    MS_U16 u16Status;

    u16Status = HAL_AVD_AFEC_GetStatus();
    if( IS_BITS_SET(u16Status, VD_HSYNC_LOCKED | VD_STATUS_RDY) )
    {
        if( VD_VSYNC_50HZ & u16Status )
        {
            return E_VIDEO_FQ_50Hz;
        }
        else
        {
            return E_VIDEO_FQ_60Hz;
        }
    }
    return E_VIDEO_FQ_NOSIGNAL;
}

MS_U8 Drv_AVD_GetHsyncEdge(void)
{
    return HAL_AVD_GetHsyncEdge();
}

MS_U8 Drv_AVD_GetDSPFineGain(AVD_RESOURCE_PRIVATE *pResource)
{
    MS_U8 u8CoaseGain,u8LPFGain, u8temp;
    MS_U16 u16FineGain;

    AVD_LOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    u8LPFGain = ((_Drv_AVD_DSPReadByte(2, 0x81)&BIT(4))>>4); //DSP_81[4], LPF gain
    u8CoaseGain = _Drv_AVD_DSPReadByte(2, 0x80)&0x0F;        //DSP_80[3:0], coarse gain
    //Read AGC fine-gain data DSP_80[11:4]
    u16FineGain = ((_Drv_AVD_DSPReadByte(2, 0x80)&0xF0)>>4)+((_Drv_AVD_DSPReadByte(2, 0x81)&0x0F)<<4);

    u8temp = HAL_AVD_GetReg(BK_AFEC_3F)& 0xF0;
    HAL_AVD_SetReg(BK_AFEC_3F, u8CoaseGain | u8temp);  //AFEC_3F[3:0], coarse gain

    HAL_AVD_SetReg(BK_AFEC_44, (u16FineGain)& 0xFF);//AFEC_44[7:0], fine gain

    u8temp = HAL_AVD_GetReg(BK_AFEC_40);
    u8temp &= (~ BIT(4));
    HAL_AVD_SetReg(BK_AFEC_40,u8temp |(u8LPFGain & BIT(2))); //AFEC_40[4]LPF gain

    AVD_UNLOCK(pResource->_s32AVD_Mutex,pResource->_u8AVDDbgLevel);

    return (u16FineGain>>6);
}

MS_U16 Drv_AVD_GetDSPVersion(void)
{
    return (HAL_AVD_GetReg(BK_AFEC_D0)+DSP_VER_OFFSET);
}

AVD_Result Drv_AVD_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return E_AVD_FAIL;
    }

    *ppVersion = &_drv_avd_version;

    return E_AVD_OK;
}

void Drv_AVD_GetInfo(AVD_Info *pAVD_Info,AVD_RESOURCE_PRIVATE *pResource)
{
    pAVD_Info->eVDInputSource=pResource->_eVDInputSource;
    pAVD_Info->eVideoSystem=pResource->_eVideoSystem;
    pAVD_Info->eLastStandard=pResource->_eLastStandard;
    pAVD_Info->u8AutoDetMode=pResource->_u8AutoDetMode;
    pAVD_Info->u16CurVDStatus=pResource->_u16CurVDStatus;
    pAVD_Info->u8AutoTuningIsProgress=pResource->_u8AutoTuningIsProgress;
}

MS_BOOL Drv_AVD_IsSyncLocked(void)
{
    if( IS_BITS_SET(HAL_AVD_AFEC_GetStatus(), VD_HSYNC_LOCKED) ) // should not check vsync lock during channel scan
    {
        return TRUE;
    }
    return FALSE;
}

MS_BOOL Drv_AVD_IsSignalInterlaced(void)
{
    if ( IS_BITS_SET(HAL_AVD_AFEC_GetStatus(), VD_INTERLACED | VD_HSYNC_LOCKED | VD_STATUS_RDY) )
    {
        return TRUE;
    }
    return FALSE;
}

MS_BOOL Drv_AVD_IsColorOn(void)
{
    if ( IS_BITS_SET(HAL_AVD_AFEC_GetStatus(), VD_COLOR_LOCKED | VD_HSYNC_LOCKED | VD_STATUS_RDY) )
    {
        return TRUE;
    }
    return FALSE;
}

MS_U32 Drv_AVD_SetPowerState(EN_POWER_MODE u16PowerState, AVD_RESOURCE_PRIVATE *pResource)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = 1;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        u32Return = 0;//SUSPEND_OK;
        pResource->_bSTRFlag = 1;
        pResource->_u16DPL_MSB = HAL_AVD_GetReg(BK_AFEC_9D);    //for STR store DPL info for DC on/off
        pResource->_u16DPL_LSB = HAL_AVD_GetReg(BK_AFEC_9E);
    }
    else if (u16PowerState == E_POWER_RESUME)
    {

        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            Drv_AVD_Init(&pResource->g_VD_InitData, sizeof(pResource->g_VD_InitData) , pResource);
            pResource->_bSTRFlag = 0;
            HAL_AVD_SetReg(BK_AFEC_9D, pResource->_u16DPL_MSB);   //for STR load DPL info for DC on/off
            HAL_AVD_SetReg(BK_AFEC_9E, pResource->_u16DPL_LSB);

            // for STR resume, PAL-I error detect to SECAM. Need to re-init SECAM register.
            HAL_AVD_SetReg(BK_SECAM_0A,0x20);
            HAL_AVD_SetReg(BK_SECAM_0B,0x42);
            _prev_u16PowerState = u16PowerState;
            u32Return = 0;//RESUME_OK;
        }
        else
        {
            u32Return = 1;//SUSPEND_FAILED;
        }
    }
    else
    {
        u32Return = 1;
    }

    return u32Return;// for success
}

MS_BOOL Drv_AVD_GetMacroVisionDetect(void)
{
    return HAL_AVD_AFEC_GetMacroVisionDetect();
}

MS_BOOL Drv_AVD_GetCGMSDetect(void)
{
    return HAL_AVD_VBI_GetCGMSDetect();
}

void vd_str_stablestatus(void)
{
    MS_U16 u16Status=0;
    MS_U8 u8StableCounter=0;

    MS_U32 u32TimeoutATV = MsOS_GetSystemTime() + 600;
    MS_U32 u32TimeoutAV   = MsOS_GetSystemTime() + 1000;

    if(eSource_Str==E_INPUT_SOURCE_ATV)
    {
        while(MsOS_GetSystemTime() < u32TimeoutATV)
        {
            if((u16Status  &  VD_STATUS_RDY)  != VD_STATUS_RDY)  //VD status Not ready
            {
                u8StableCounter=0;
            }
            else  //VD status ready
            {
                if((u16Status & VD_HSYNC_LOCKED)) //VD Hsync Luck
                {
                    u8StableCounter++;
                    if(u8StableCounter>10)
                    {
                        break;
                    }
                }
                else  //VD Hsync unLuck
                {
                    u8StableCounter=0;
                }
            }
            MsOS_DelayTask(10);
            u16Status=HAL_AVD_AFEC_GetStatus();
        }
    }
    else
    {
        while(MsOS_GetSystemTime() < u32TimeoutAV)
        {
            if((u16Status  &  VD_STATUS_RDY)  != VD_STATUS_RDY)  //VD status Not ready
            {
                u8StableCounter=0;
            }
            else  //VD status ready
            {
                if((u16Status & VD_HSYNC_LOCKED)) //VD Hsync Luck
                {
                    u8StableCounter++;
                    if(u8StableCounter>10)
                    {
                        break;
                    }
                }
                else  //VD Hsync unLuck
                {
                    u8StableCounter=0;
                }
            }
            MsOS_DelayTask(10);
            u16Status=HAL_AVD_AFEC_GetStatus();
        }
    }
}

void Drv_AVD_SetBurstWinStart(MS_U8 u8BusrtStartPosition)
{
    HAL_AVD_SetBurstWinStart(u8BusrtStartPosition);
}

MS_BOOL Drv_AVD_IsAVDAlive(void)
{
    MS_U16 u16CurHeartclock;
    MS_U16 u16LastHeartclock;
    MS_U8 u8DeadCnt=0;
    MS_U8 i=0;

    u16LastHeartclock=HAL_AVD_GetReg(BK_AFEC_D6);
    MsOS_DelayTask(1);
    for(i=0;i<5;i++)
    {
        u16CurHeartclock=HAL_AVD_GetReg(BK_AFEC_D6);
        if(u16CurHeartclock == u16LastHeartclock)
        {
            u8DeadCnt++;
        }
        u16LastHeartclock=u16CurHeartclock;

        MsOS_DelayTask(2);
    }

    if(u8DeadCnt==0)
    {
        ULOGE("AVD","MDrv_AVD_IsAVDAlive Status : Alive\n");
        return TRUE;
    }
    else
    {
       ULOGE("AVD","MDrv_AVD_IsAVDAlive Status : Dead\n");
       return FALSE;
    }
}

#if defined(MSOS_TYPE_LINUX_KERNEL)
AVD_VideoStandardType vd_str_timingchangedetection(void)
{
    MS_U16 u16Status=0;
    AVD_VideoStandardType eVideoStandard = E_VIDEOSTANDARD_NOTSTANDARD;
    u16Status= HAL_AVD_AFEC_GetStatus();

    if (!IS_BITS_SET(u16Status, VD_HSYNC_LOCKED|VD_SYNC_LOCKED|VD_STATUS_RDY))
    {
        return E_VIDEOSTANDARD_NOTSTANDARD;
    }
    else
    {
        if(u16Status & VD_BURST_ON)
        {
            switch (u16Status & VD_FSC_TYPE)
            {
                case VD_FSC_4433: // (FSC_MODE_PAL << 5):
                    if (u16Status & VD_VSYNC_50HZ) // 50 Hz
                    {
                        eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
                    }
                    else // 60MHz
                    {
                        if (u16Status & VD_PAL_SWITCH)
                        {
                            eVideoStandard = E_VIDEOSTANDARD_PAL_60; // or vsdNTSC_44
                        }
                        else
                        {
                            eVideoStandard = E_VIDEOSTANDARD_NTSC_44;
                        }
                    }
                    break;

                case VD_FSC_3579:
                        eVideoStandard = E_VIDEOSTANDARD_NTSC_M;
                        break;

                case VD_FSC_3575: // (FSC_MODE_PAL_M << 5):
                        eVideoStandard = E_VIDEOSTANDARD_PAL_M;
                        break;

                case VD_FSC_3582: // (FSC_MODE_PAL_N << 5):
                        eVideoStandard = E_VIDEOSTANDARD_PAL_N;
                        break;

                default:
                        eVideoStandard = E_VIDEOSTANDARD_NOTSTANDARD;
                        break;
            }
        }
        else
        {
            if (u16Status & VD_VSYNC_50HZ)
            {
                if ((u16Status & VD_FSC_TYPE) ==  VD_FSC_4285)
                {
                    {
                        eVideoStandard = E_VIDEOSTANDARD_SECAM;
                    }
                }
                else
                {
                    eVideoStandard = E_VIDEOSTANDARD_PAL_BGHI;
                }
            }
            else
            {
                eVideoStandard = E_VIDEOSTANDARD_NTSC_M;
            }
        }
    }

    return eVideoStandard;
}
#endif

MS_BOOL Drv_AVD_IsLockAudioCarrier(void)
{
    MS_U16 u16tmp,u16tmp1=0;
    MS_U8 u8tmp;
    MS_U16 u16outedge,u16outedgesum=0;
    MS_U16 u16noise,u16noisesum=0;

    // in of window H sync edge;
    u16tmp1 = HAL_AVD_GetHsyncEdge() & 0x3F;
    if(u16tmp1 == 0)
        u16tmp1 = 1 ;// to protect u16tmp1 not equal 0

    // Output of window H sync edge
    HAL_AVD_SetReg(BK_AFEC_04, 0x0B);
    u16outedgesum = HAL_AVD_GetReg(BK_AFEC_03);
    if(u16outedgesum == 0)
        u16outedgesum = 1 ;// to protect u16outedgesum not equal 0

    // noise mag
    HAL_AVD_SetReg(BK_AFEC_04, 0x04);
    u16noisesum = HAL_AVD_GetReg(BK_AFEC_02);
    if(u16noisesum == 0)
        u16noisesum = 1 ;// to protect u16noisesum not equal 0

    for(u8tmp=0;u8tmp<10;u8tmp++)
    {
        // in of window H sync edge
        u16tmp = HAL_AVD_GetHsyncEdge() & 0x3F;
        u16tmp1 = ((9*u16tmp1) + (1*u16tmp))/10;

        // Output of window H sync edge
        HAL_AVD_SetReg(BK_AFEC_04, 0x0B);
        u16outedge = HAL_AVD_GetReg(BK_AFEC_03);
        u16outedgesum = ((9*u16outedgesum) + (1*u16outedge))/10;

        // noise mag
        HAL_AVD_SetReg(BK_AFEC_04, 0x04);
        u16noise = HAL_AVD_GetReg(BK_AFEC_02);
        u16noisesum = ((9*u16noisesum) + (1*u16noise))/10;
        MsOS_DelayTask(1);
    }

    if((u16tmp1>=0x35)||((u16outedgesum >3)&&(u16noise > 0x50)))
        return TRUE;
    else
        return FALSE;
}