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
#ifndef _VIF_C_
#define _VIF_C_

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
#include "../../utopia_core/utopia_dapi.h"
#include "../../utopia_core/utopia.h"
#include "../../utopia_core/utopia_driver_id.h"

// Internal Definition
#include "halVIF.h"
#include "halVIF_Customer.h"
#include "regVIF.h"
#include "VIF.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//****************************************************************************
// VIF lib Changelist
//****************************************************************************
#define MSIF_VIF_LIB_CODE     {'V','I','F','_'}
#define MSIF_VIF_LIBVER       {'1','8'}
#define MSIF_VIF_BUILDNUM     {'4','8'}
#define MSIF_VIF_CHANGELIST   {'0','1','4','9','7','8','2','3'}
#define MSIF_VIF_DASH          {'_'}
#define MSIF_VIF_DATE          {'1','6','0','9','1','9'}    // yymmdd

#define VIF_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_VIF_LIB_CODE,                  /* IP__                                             */  \
    MSIF_VIF_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_VIF_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_VIF_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

const U8 VIF_Version1[] = {'V','I','F', '_', '_'};
const U8 VIF_Version2[] = {CHIP_TYPE};
const U8 VIF_Version3[] = MSIF_VIF_DASH;
const U8 VIF_Version4[] = MSIF_VIF_LIBVER;
const U8 VIF_Version5[] = MSIF_VIF_DASH;
const U8 VIF_Version6[] = MSIF_VIF_BUILDNUM;
const U8 VIF_Version7[] = MSIF_VIF_DASH;
const U8 VIF_Version8[] = MSIF_VIF_DATE;
const U8 VIF_Version9[] = MSIF_VIF_DASH;
const U8 VIF_VersionA[] = MSIF_VIF_CHANGELIST;

#define DRVVIFDBG(x)          //x

//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------
#define DRV_VIF_GetSystemTime()                     MsOS_GetSystemTime()

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_vif_version = {
    .DDI = { VIF_DRV_VERSION },
};

VIFInitialIn VIFInitialIn_inst;
VIFUserFilter sVIFUserFilter;
//VIFNotchA1A2 sVIFNotchA1A2;
//VIFSOS1112 sVIFSOS1112;
//VIFSOS33 sVIFSOS33;
//DWORD dwVIFPara;
VIFSoundSystem gVIFSoundSystem;
VIFSTRParameter sVIFSTRInfo;

BOOL bEnableUsrSteadyAgcK;
U8 u8UsrSteadyAgcK;
BOOL bEnableUsrNonSteadyAgcK;
U8 u8UsrNonSteadyAgcK;
BOOL bIsATV = FALSE;

#define VIF_debug(x) //x

//-------------------------------------------------------------------------------------------------
//  Enum
//-------------------------------------------------------------------------------------------------

typedef enum
{
    VIF_POOL_ID_VIF = 0,
} eVIFPoolID;


void VIFRegisterToUtopia(void)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_VIF, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)VIFOpen, (FUtopiaClose)VIFClose, (FUtopiaIOctl)VIFIoctl);
 
   // Utopia2K STR
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)VIFStr);
    //UtopiaModuleSetSTRPrivate(pUtopiaModule, STRPrivateDataSize);
#endif

}

MS_U32 VIFOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    printf("\n[VIF INFO] VIF open \n");

    VIF_INSTANT_PRIVATE *pVifPri = NULL;
    UtopiaInstanceCreate(sizeof(VIF_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void*)&pVifPri);

    pVifPri->fpVIFVersion = VIF_Version;
    pVifPri->fpVIFSetClock = VIF_SetClock;
    pVifPri->fpVIFInit = VIF_Init;
    pVifPri->fpVIFExit = VIF_Exit;
    pVifPri->fpVIFReset = VIF_Reset;
    pVifPri->fpVIFHandler = VIF_Handler;
    pVifPri->fpVIFSetSoundSystem = VIF_SetSoundSystem;
    pVifPri->fpVIFSetIfFreq = VIF_SetIfFreq;
    pVifPri->fpVIFCRFOE = VIF_Read_CR_FOE;
    pVifPri->fpVIFLOCKSTATUS = VIF_Read_CR_LOCK_STATUS;
    pVifPri->fpVIFBypassDBBAudioFilter = VIF_BypassDBBAudioFilter;
    pVifPri->fpVIFSetFreqBand = VIF_SetFreqBand;

    pVifPri->fpVIFGetInputLevelIndicator = VIF_GetInputLevelIndicator;
    pVifPri->fpVIFSetParameter = VIF_SetParameter;
    pVifPri->fpVIFShiftClk = VIF_ShiftClk;
    pVifPri->fpVIFSetPowerState = VIF_SetPowerState;
    pVifPri->fpVIFWriteByte = VIF_WriteByte;
    pVifPri->fpVIFReadBtye = VIF_ReadByte;

    //return TRUE;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 VIFIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    BOOL bEnable = 0;
    BYTE u8Value;
    //WORD u16Value;
    //DWORD u32Value;
    DWORD u32Ret = 0;
    //WORD u16PowerState = 0;
    VIFSoundSystem SoundSystem;
    IfFrequencyType IfFreqType;
    FrequencyBand FreqBand;
    PVIF_COPYTOUSER pCopyToUser;

    PVIF_INIT pINITParam = NULL;
    PVIF_SETREGVALUE pSetRegValue = NULL;
    PVIF_SETPARA pSetGroupParam = NULL;

    void* pModule = NULL;
    VIF_INSTANT_PRIVATE* psVIFInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psVIFInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);

    switch(u32Cmd)
    {
        case Drv_CMD_VIF_Version:
            VIF_debug(printf("Drv_CMD_VIF_Version\n"));
            (psVIFInstPri->fpVIFVersion)();
            break;
        case Drv_CMD_VIF_SetClock:
            VIF_debug(printf("Drv_CMD_VIF_SetClock s\n"));
            bEnable = *(BOOL *)pArgs;
            (psVIFInstPri->fpVIFSetClock)(bEnable);
            break;
        case Drv_CMD_VIF_Init:
            VIF_debug(printf("Drv_CMD_VIF_Init \n"));
            pINITParam = (PVIF_INIT)pArgs;
            (psVIFInstPri->fpVIFInit)(pINITParam->pVIF_InitData, pINITParam->u32InitDataLen);
            break;
        case Drv_CMD_VIF_Exit:
            VIF_debug(printf("Drv_CMD_VIF_Exit \n"));
            (psVIFInstPri->fpVIFExit)();
            break;
        case Drv_CMD_VIF_Reset:
            VIF_debug(printf("Drv_CMD_VIF_Reset \n"));
            (psVIFInstPri->fpVIFReset)();
            break;
        case Drv_CMD_VIF_Handler:
            VIF_debug(printf("Drv_CMD_VIF_Handler \n"));
            bEnable = *(BOOL *)pArgs;
            (psVIFInstPri->fpVIFHandler)(bEnable);
            break;
        case Drv_CMD_VIF_SetSoundSystem:
            VIF_debug(printf("Drv_CMD_VIF_SetSoundSystem \n"));
            SoundSystem = *(VIFSoundSystem *)pArgs;
            (psVIFInstPri->fpVIFSetSoundSystem)(SoundSystem);
            break;
        case Drv_CMD_VIF_SetIfFreq:
            VIF_debug(printf("Drv_CMD_VIF_SetIfFreq \n"));
            IfFreqType = *(IfFrequencyType *)pArgs;
            (psVIFInstPri->fpVIFSetIfFreq)(IfFreqType);
            break;
        case Drv_CMD_VIF_Read_CR_FOE:
            VIF_debug(printf("Drv_CMD_VIF_Read_CR_FOE \n"));
            pCopyToUser = (PVIF_COPYTOUSER)pArgs;
            u32Ret = (psVIFInstPri->fpVIFCRFOE)();
            pCopyToUser->u8Value = (BYTE)u32Ret;
            break;
        case Drv_CMD_VIF_Read_CR_LOCK_STATUS:
            VIF_debug(printf("Drv_CMD_VIF_Read_CR_LOCK_STATUS \n"));
            pCopyToUser = (PVIF_COPYTOUSER)pArgs;
            u32Ret = (psVIFInstPri->fpVIFLOCKSTATUS)();
            pCopyToUser->u8Value = (BYTE)u32Ret;
            break;
        case Drv_CMD_VIF_BypassDBBAudioFilter:
            VIF_debug(printf("Drv_CMD_VIF_BypassDBBAudioFilter \n"));
            bEnable = *(BOOL *)pArgs;
            (psVIFInstPri->fpVIFBypassDBBAudioFilter)(bEnable);
            break;
        case Drv_CMD_VIF_SetFreqBand:
            VIF_debug(printf("Drv_CMD_VIF_SetFreqBand \n"));
            FreqBand = *(FrequencyBand *)pArgs;
            (psVIFInstPri->fpVIFSetFreqBand)(FreqBand);
            break;
        case Drv_CMD_VIF_GetInputLevelIndicator:
            VIF_debug(printf("Drv_CMD_VIF_GetInputLevelIndicator \n"));
            pCopyToUser = (PVIF_COPYTOUSER)pArgs;
            u32Ret = (psVIFInstPri->fpVIFGetInputLevelIndicator)();
            pCopyToUser->bEnable = (BOOL)u32Ret;
            break;
        case Drv_CMD_VIF_SetParameter:
            VIF_debug(printf("Drv_CMD_VIF_SetParameter \n"));
            pSetGroupParam = (PVIF_SETPARA)pArgs;
            u32Ret = (psVIFInstPri->fpVIFSetParameter)(pSetGroupParam->ParaGroup, pSetGroupParam->pVIF_Para, pSetGroupParam->u32DataLen);
            pSetGroupParam->bRet = (BOOL)u32Ret;
            break;
        case Drv_CMD_VIF_ShiftClk:
            VIF_debug(printf("Drv_CMD_VIF_ShiftClk \n"));
            u8Value = *(BYTE *)pArgs;
            (psVIFInstPri->fpVIFShiftClk)(u8Value);
            break;
        case Drv_CMD_VIF_SetPowerState:
            VIF_debug(printf("Drv_CMD_VIF_SetPowerState \n"));
            pCopyToUser = (PVIF_COPYTOUSER)pArgs;
            u32Ret = (psVIFInstPri->fpVIFSetPowerState)(pCopyToUser->u16PowerState);
            pCopyToUser->u32Value = (MS_U32)u32Ret;
            break;
        case Drv_CMD_VIF_WriteByte:
            VIF_debug(printf("Drv_CMD_VIF_WriteByte \n"));
            pSetRegValue = (PVIF_SETREGVALUE)pArgs;
            (psVIFInstPri->fpVIFWriteByte)(pSetRegValue->u32Addr, pSetRegValue->u8Value);
            break;
        case Drv_CMD_VIF_ReadByte:
            VIF_debug(printf("Drv_CMD_VIF_ReadByte \n"));
            pSetRegValue = (PVIF_SETREGVALUE)pArgs;
            u32Ret = (psVIFInstPri->fpVIFReadBtye)(pSetRegValue->u32Addr);
            pSetRegValue->u8Value = (U8)u32Ret;
            break;
        default:
            break;
    }

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 VIFClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}

MS_U32 VIFStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U32 u32Ret = 0;

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Ret = VIF_SetPowerState(E_POWER_SUSPEND);
        
        if(u32Ret == 0)
          u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Ret = VIF_SetPowerState(E_POWER_RESUME);

        if(u32Ret == 0)        
          u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
#if defined(MSOS_TYPE_LINUX_KERNEL)
        UtopiaStrSendCondition("vif", u32PowerState, 0);
#endif
    }
    else
    {
        printf("\r\n ====== [VIFStr] Unknown Power State ====== \n");
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;
}

//-------------------------------------------------------------------------------------------------
//  Local VIF functions
//-------------------------------------------------------------------------------------------------

void VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    HAL_VIF_WriteByteMask(u32Reg, u8Val, u8Mask);
}

void VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    HAL_VIF_WriteBit(u32Reg, bEnable, u8Mask);
}

void VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    HAL_VIF_WriteByte(u32Reg, u8Val);
}

U8 VIF_ReadByte(U32 u32Reg )
{
    return HAL_VIF_ReadByte(u32Reg);
}

/***********************************************************************************
  Subject:    printf version number
  Function:   DRV_VIF_Version
  Parmeter:
  Return:
  Remark:
************************************************************************************/
void VIF_MSIFVersion(MSIF_Version** mVersion)
{
    *mVersion = &_drv_vif_version;
}

void VIF_Version(void)
{
    U8 sVersion[30]={0x00};
    // VIF version
#ifdef _2_DIGIT_CHIP_TYPE_
    memcpy(sVersion, VIF_Version1, 5);
    memcpy(sVersion+5, VIF_Version2, 2);
    memcpy(sVersion+7, VIF_Version3, 1);
    memcpy(sVersion+8, VIF_Version4, 2);
    memcpy(sVersion+10, VIF_Version5, 1);
    memcpy(sVersion+11, VIF_Version6, 2);
    memcpy(sVersion+13, VIF_Version7, 1);
    memcpy(sVersion+14, VIF_Version8, 6);
    memcpy(sVersion+20, VIF_Version9, 1);
    memcpy(sVersion+21, VIF_VersionA, 8);
#else
    memcpy(sVersion, VIF_Version1, 5);
    memcpy(sVersion+5, VIF_Version2, 1);
    memcpy(sVersion+6, VIF_Version3, 1);
    memcpy(sVersion+7, VIF_Version4, 2);
    memcpy(sVersion+9, VIF_Version5, 1);
    memcpy(sVersion+10, VIF_Version6, 2);
    memcpy(sVersion+12, VIF_Version7, 1);
    memcpy(sVersion+13, VIF_Version8, 6);
    memcpy(sVersion+19, VIF_Version9, 1);
    memcpy(sVersion+20, VIF_VersionA, 8);
#endif

    printf("\r\n[VIF Version]: ");
    printf("%s\r\n",sVersion);
}

/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_Read_CR_FOE
  Parmeter:
  Return:
  Remark:
************************************************************************************/
U8 VIF_Read_CR_FOE(void)
{
    //DRVVIFDBG(printf("\r\nDRV_VIF_Read_CR_FOE()"));
    return msVifReadCRFOE();
}
/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_Read_CR_LOCK_STATUS
  Parmeter:
  Return:
  Remark:
************************************************************************************/
U8 VIF_Read_CR_LOCK_STATUS(void)
{
    //DRVVIFDBG(printf("\r\nDRV_VIF_Read_CR_LOCK_STATUS()"));
    return msVifReadLockStatus();
}

void VIF_SetClock(BOOL bEnable)
{
    DRVVIFDBG(printf("\r\nDRV_VIF_SetClock %d",bEnable));
    HAL_VIF_SetClock(bEnable);
}

void VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen)
{
    U32 u32StartTime = DRV_VIF_GetSystemTime();
    U32 u32TopTime = sizeof(U32) -1;
    U32 u32DiffTime = 0;

    DRVVIFDBG(printf("\r\nDRV_VIF_Init"));

    if(DRV_VIF_GetSystemTime() >= u32StartTime)
    {
        while (DRV_VIF_GetSystemTime() -u32StartTime < 3)
        {
             // do nothing;
        }
    }
    else
    {
        u32DiffTime = u32TopTime - u32StartTime;
        while((DRV_VIF_GetSystemTime() + u32DiffTime) < 3)
        {
            // do nothing;
        }
    }

    HAL_VIF_RegInit(); // !! any register access should be after this function

    if ( sizeof(VIFInitialIn_inst) == u32InitDataLen)
    {
        memcpy(&VIFInitialIn_inst, pVIF_InitData, u32InitDataLen);
        
        VIFInitialIn_inst.VifFreqBand = (BYTE)(VIF_ReadByte(VIF_RF_RESERVED_1+2))/*g_FreqBand*/;

        bEnableUsrSteadyAgcK = FALSE;
        u8UsrSteadyAgcK = 0;
        bEnableUsrNonSteadyAgcK = FALSE;
        u8UsrNonSteadyAgcK = 0;
        bIsATV = TRUE;
    }
    else
    {
        printf("\r\nERROR  DRV_VIF init data size FAILE!!");
        return;
    }

    if (VIFInitialIn_inst.VifReserve & _BIT2)
    {
        printf("\r\n------------Start of VIF lib para-----------");
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifTop);// = VIF_TOP;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifIfBaseFreq);// = VIF_IF_BASE_FREQ;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifTunerStepSize);// = VIF_TUNER_STEP_SIZE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSawArch);//=VIF_SAW_ARCH;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifVgaMaximum);// = VIF_VGA_MAXIMUM;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifVgaMinimum);// = VIF_VGA_MINIMUM;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.GainDistributionThr);// = GAIN_DISTRIBUTION_THR;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcVgaBase);//=VIF_AGC_VGA_BASE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcVgaOffs);//=VIF_AGC_VGA_OFFS;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcRefNegative);// = VIF_AGC_REF_NEGATIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAgcRefPositive);// = VIF_AGC_REF_POSITIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc1Ref);// = VIF_DAGC1_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc2Ref);// = VIF_DAGC2_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc1GainOv);//=VIF_DAGC1_GAIN_OV;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDagc2GainOv);//=VIF_DAGC2_GAIN_OV;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKf1);//=VIF_CR_KF1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp1);//=VIF_CR_KP1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi1);//=VIF_CR_KI1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp2);//=VIF_CR_KP2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi2);//=VIF_CR_KI2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKp);// = VIF_CR_KP;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKi);// = VIF_CR_KI;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLockThr);// = VIF_CR_LOCK_THR;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrThr);//= VIF_CR_THR;
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrLockNum>>16)&0x0000FFFF));//=VIF_CR_LOCK_NUM;
        printf("%x",(WORD) (VIFInitialIn_inst.VifCrLockNum&0x0000FFFF));//=VIF_CR_LOCK_NUM;
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrUnlockNum>>16)&0x0000FFFF));//=VIF_CR_UNLOCK_NUM;
        printf("%x",(WORD) (VIFInitialIn_inst.VifCrUnlockNum&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdErrMax);//=VIF_CR_PD_ERR_MAX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLockLeakySel);//=VIF_CR_LOCK_LEAKY_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdX2);//=VIF_CR_PD_X2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrLpfSel);//=VIF_CR_LPF_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrPdModeSel);//=VIF_CR_PD_MODE_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjust);//=VIF_CR_KP_KI_ADJUST;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustGear);//=VIF_CR_KP_KI_ADJUST_GEAR;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr1);//=VIF_CR_KP_KI_ADJUST_THR1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr2);//=VIF_CR_KP_KI_ADJUST_THR2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrKpKiAdjustThr3);//=VIF_CR_KP_KI_ADJUST_THR3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDynamicTopAdjust);//=VIF_DYNAMIC_TOP_ADJUST;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDynamicTopMin);//=VIF_DYNAMIC_TOP_MIN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifAmHumDetection);//=VIF_AM_HUM_DETECTION;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampSel);//=VIF_CLAMPGAIN_CLAMP_SEL;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainSyncbottRef);//=VIF_CLAMPGAIN_SYNCBOTT_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainSyncheightRef);//=VIF_CLAMPGAIN_SYNCHEIGHT_REF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainKc);//=VIF_CLAMPGAIN_KC;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainKg);//=VIF_CLAMPGAIN_KG;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOren);//=VIF_CLAMPGAIN_CLAMP_OREN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOren);//=VIF_CLAMPGAIN_GAIN_OREN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOvNegative);//=VIF_CLAMPGAIN_CLAMP_OV_NEGATIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOvNegative);//=VIF_CLAMPGAIN_GAIN_OV_NEGATIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampOvPositive);//=VIF_CLAMPGAIN_CLAMP_OV_POSITIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainOvPositive);//=VIF_CLAMPGAIN_GAIN_OV_POSITIVE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampMin);//=VIF_CLAMPGAIN_CLAMP_MIN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainClampMax);//=VIF_CLAMPGAIN_CLAMP_MAX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainMin);//=VIF_CLAMPGAIN_GAIN_MIN;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainGainMax);//=VIF_CLAMPGAIN_GAIN_MAX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifClampgainPorchCnt);//=VIF_CLAMPGAIN_PORCH_CNT;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_VHF_L);//=VIF_PEAKING_FILTER_B_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_VHF_L);//=VIF_YC_DELAY_FILTER_B_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L);//=VIF_GROUP_DELAY_FILTER_B_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_VHF_L);//=VIF_PEAKING_FILTER_GH_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L);//=VIF_YC_DELAY_FILTER_GH_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L);//=VIF_GROUP_DELAY_FILTER_GH_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_VHF_L);//=VIF_PEAKING_FILTER_DK_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L);//=VIF_YC_DELAY_FILTER_DK_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L);//=VIF_GROUP_DELAY_FILTER_DK_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_VHF_L);//=VIF_PEAKING_FILTER_I_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_VHF_L);//=VIF_YC_DELAY_FILTER_I_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L);//=VIF_GROUP_DELAY_FILTER_I_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_VHF_L);//=VIF_PEAKING_FILTER_L_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_VHF_L);//=VIF_YC_DELAY_FILTER_L_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L);//=VIF_GROUP_DELAY_FILTER_L_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_VHF_L);//=VIF_PEAKING_FILTER_LL_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L);//=VIF_YC_DELAY_FILTER_LL_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L);//=VIF_GROUP_DELAY_FILTER_LL_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_VHF_L);//=VIF_PEAKING_FILTER_MN_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L);//=VIF_YC_DELAY_FILTER_MN_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L);//=VIF_GROUP_DELAY_FILTER_MN_VHF_L;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_VHF_H);//=VIF_PEAKING_FILTER_B_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_VHF_H);//=VIF_YC_DELAY_FILTER_B_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H);//=VIF_GROUP_DELAY_FILTER_B_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_VHF_H);//=VIF_PEAKING_FILTER_GH_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H);//=VIF_YC_DELAY_FILTER_GH_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H);//=VIF_GROUP_DELAY_FILTER_GH_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_VHF_H);//=VIF_PEAKING_FILTER_DK_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H);//=VIF_YC_DELAY_FILTER_DK_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H);//=VIF_GROUP_DELAY_FILTER_DK_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_VHF_H);//=VIF_PEAKING_FILTER_I_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_VHF_H);//=VIF_YC_DELAY_FILTER_I_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H);//=VIF_GROUP_DELAY_FILTER_I_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_VHF_H);//=VIF_PEAKING_FILTER_L_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_VHF_H);//=VIF_YC_DELAY_FILTER_L_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H);//=VIF_GROUP_DELAY_FILTER_L_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_VHF_H);//=VIF_PEAKING_FILTER_LL_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H);//=VIF_YC_DELAY_FILTER_LL_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H);//=VIF_GROUP_DELAY_FILTER_LL_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_VHF_H);//=VIF_PEAKING_FILTER_MN_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H);//=VIF_YC_DELAY_FILTER_MN_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H);//=VIF_GROUP_DELAY_FILTER_MN_VHF_H;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterB_UHF);//=VIF_PEAKING_FILTER_B_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterB_UHF);//=VIF_YC_DELAY_FILTER_B_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterB_UHF);//=VIF_GROUP_DELAY_FILTER_B_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterGH_UHF);//=VIF_PEAKING_FILTER_GH_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterGH_UHF);//=VIF_YC_DELAY_FILTER_GH_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterGH_UHF);//=VIF_GROUP_DELAY_FILTER_GH_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterDK_UHF);//=VIF_PEAKING_FILTER_DK_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterDK_UHF);//=VIF_YC_DELAY_FILTER_DK_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterDK_UHF);//=VIF_GROUP_DELAY_FILTER_DK_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterI_UHF);//=VIF_PEAKING_FILTER_I_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterI_UHF);//=VIF_YC_DELAY_FILTER_I_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterI_UHF);//=VIF_GROUP_DELAY_FILTER_I_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterL_UHF);//=VIF_PEAKING_FILTER_L_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterL_UHF);//=VIF_YC_DELAY_FILTER_L_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterL_UHF);//=VIF_GROUP_DELAY_FILTER_L_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterLL_UHF);//=VIF_PEAKING_FILTER_LL_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterLL_UHF);//=VIF_YC_DELAY_FILTER_LL_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterLL_UHF);//=VIF_GROUP_DELAY_FILTER_LL_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifPeakingFilterMN_UHF);//=VIF_PEAKING_FILTER_MN_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifYcDelayFilterMN_UHF);//=VIF_YC_DELAY_FILTER_MN_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifGroupDelayFilterMN_UHF);//=VIF_GROUP_DELAY_FILTER_MN_UHF;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.ChinaDescramblerBox);// = CHINA_DESCRAMBLER_BOX;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifDelayReduce);//= VIF_DELAY_REDUCE;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifOverModulation);//=VIF_OVER_MODULATION;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifOverModulationDetect);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetect);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIAGCREF);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifADCOverflowAGCREF);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifChanelScanAGCREF);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR1);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR2);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR3);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifACIDetTHR4);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifFreqBand);// = FREQ_VHF_L; // should be changed when setting tuner freq
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC0);//= VIF_SOS21_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC1);//= VIF_SOS21_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC2);//= VIF_SOS21_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC3);//= VIF_SOS21_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos21FilterC4);//= VIF_SOS21_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC0);//= VIF_SOS22_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC1);//= VIF_SOS22_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC2);//= VIF_SOS22_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC3);//= VIF_SOS22_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos22FilterC4);//= VIF_SOS22_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC0);//= VIF_SOS31_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC1);//= VIF_SOS31_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC2);//= VIF_SOS31_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC3);//= VIF_SOS31_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos31FilterC4);//= VIF_SOS31_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC0);//= VIF_SOS32_FILTER_C0;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC1);//= VIF_SOS32_FILTER_C1;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC2);//= VIF_SOS32_FILTER_C2;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC3);//= VIF_SOS32_FILTER_C3;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifSos32FilterC4);//= VIF_SOS32_FILTER_C4;
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifTunerType);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_B>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_B&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_B);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_GH>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_GH&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_GH);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_DK>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_DK&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_DK);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_I>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_I&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_I);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_L>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_L&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_L);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_LL>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_LL&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_LL);
        printf("\r\n %x",(WORD)((VIFInitialIn_inst.VifCrRate_MN>>16)&0x0000FFFF));
        printf("%x",(WORD)(VIFInitialIn_inst.VifCrRate_MN&0x0000FFFF));
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifCrInvert_MN);
        printf("\r\n %x",(WORD)VIFInitialIn_inst.VifReserve);
        printf("\r\n------------End of VIF lib para-----------");
    }

    msVifAdcInitial();

    if (VIFInitialIn_inst.VifIfBaseFreq == IF_FREQ_45_75)
        msVifSetIfFreq(IF_FREQ_4575);
    else if (VIFInitialIn_inst.VifIfBaseFreq == IF_FREQ_38_00)
        msVifSetIfFreq(IF_FREQ_3800);
    else
        msVifSetIfFreq(IF_FREQ_3890);

    msVifSetSoundSystem(VIF_SOUND_DK2);//initial
    msVifTopAdjust();
    msVifInitial();
}

void VIF_Exit(void)
{    
    msVifExit();
    bIsATV = FALSE;
}

void VIF_Reset(void)
{
    msVifInitial();
}

void VIF_Handler(BOOL bAutoScan)
{
    msVifHandler(bAutoScan);
}

void VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    DRVVIFDBG(printf("DRV_VIF_SetSoundSystem ucSoundSystem=%d\n",ucSoundSystem));
    gVIFSoundSystem = ucSoundSystem;
    msVifSetSoundSystem(ucSoundSystem);
}

void VIF_SetIfFreq(IfFrequencyType u16IfFreq)
{
    DRVVIFDBG(printf("DRV_VIF_SetIfFreq() u16IfFreq=%d\n",u16IfFreq));
    msVifSetIfFreq(u16IfFreq);
}

void VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    DRVVIFDBG(printf("DRV_VIF_BypassDBBAudioFilter() bEnableq=%d\n",bEnable));
    HAL_VIF_BypassDBBAudioFilter(bEnable);
}

void VIF_SetFreqBand(FrequencyBand u8FreqBand)
{
    DRVVIFDBG(printf("DRV_VIF_SetFreqBand() u8FreqBand=%d\n",u8FreqBand));
    VIFInitialIn_inst.VifFreqBand = u8FreqBand;
    
    //g_FreqBand = u8FreqBand; // 0x1121_D4
    VIF_WriteByte(VIF_RF_RESERVED_1+2, u8FreqBand);
}

BOOL VIF_GetInputLevelIndicator(void)
{
    DRVVIFDBG(printf("DRV_VIF_GetInputLevelIndicator() \n"));
    return HAL_VIF_GetInputLevelIndicator();
}

void VIF_CopyFilter(BYTE i)
{
    if (i==1)       // set peaking
    {
        VIFInitialIn_inst.VifSos21FilterC0=sVIFUserFilter.VifSos21FilterC0;
        VIFInitialIn_inst.VifSos21FilterC1=sVIFUserFilter.VifSos21FilterC1;
        VIFInitialIn_inst.VifSos21FilterC2=sVIFUserFilter.VifSos21FilterC2;
        VIFInitialIn_inst.VifSos21FilterC3=sVIFUserFilter.VifSos21FilterC3;
        VIFInitialIn_inst.VifSos21FilterC4=sVIFUserFilter.VifSos21FilterC4;
    }
    else if (i==2)    // set YC
    {
        VIFInitialIn_inst.VifSos22FilterC0=sVIFUserFilter.VifSos22FilterC0;
        VIFInitialIn_inst.VifSos22FilterC1=sVIFUserFilter.VifSos22FilterC1;
        VIFInitialIn_inst.VifSos22FilterC2=sVIFUserFilter.VifSos22FilterC2;
        VIFInitialIn_inst.VifSos22FilterC3=sVIFUserFilter.VifSos22FilterC3;
        VIFInitialIn_inst.VifSos22FilterC4=sVIFUserFilter.VifSos22FilterC4;
    }
    else if (i==3)    // set group delay
    {
        VIFInitialIn_inst.VifSos31FilterC0=sVIFUserFilter.VifSos31FilterC0;
        VIFInitialIn_inst.VifSos31FilterC1=sVIFUserFilter.VifSos31FilterC1;
        VIFInitialIn_inst.VifSos31FilterC2=sVIFUserFilter.VifSos31FilterC2;
        VIFInitialIn_inst.VifSos31FilterC3=sVIFUserFilter.VifSos31FilterC3;
        VIFInitialIn_inst.VifSos31FilterC4=sVIFUserFilter.VifSos31FilterC4;
        VIFInitialIn_inst.VifSos32FilterC0=sVIFUserFilter.VifSos32FilterC0;
        VIFInitialIn_inst.VifSos32FilterC1=sVIFUserFilter.VifSos32FilterC1;
        VIFInitialIn_inst.VifSos32FilterC2=sVIFUserFilter.VifSos32FilterC2;
        VIFInitialIn_inst.VifSos32FilterC3=sVIFUserFilter.VifSos32FilterC3;
        VIFInitialIn_inst.VifSos32FilterC4=sVIFUserFilter.VifSos32FilterC4;
    /*    
        sVIFSOS33.Vif_SOS_33_C0=sVIFUserFilter.VifSos33FilterC0;
        sVIFSOS33.Vif_SOS_33_C1=sVIFUserFilter.VifSos33FilterC1;
        sVIFSOS33.Vif_SOS_33_C2=sVIFUserFilter.VifSos33FilterC2;
        sVIFSOS33.Vif_SOS_33_C3=sVIFUserFilter.VifSos33FilterC3;
        sVIFSOS33.Vif_SOS_33_C4=sVIFUserFilter.VifSos33FilterC4;
    */
    }
}

BOOL VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen)
{
    static VIFNotchA1A2 sVIFNotchA1A2 = {0};
    static VIFSOS1112 sVIFSOS1112 = {0};
    static DWORD dwVIFPara = 0;

    DRVVIFDBG(printf("DRV_VIF_SetParameter() \n"));

    switch (paraGroup)
    {
        case VIF_PARA_USER_FILTER:
            if ( sizeof(VIFUserFilter) == u32DataLen)
            {
                memcpy(&sVIFUserFilter, (VIFUserFilter *) pVIF_Para, u32DataLen);

                // check if need to update user define peaking filter
                if ((sVIFUserFilter.VifUserPeakingFilterSelect>=PK_START) &&
                    (sVIFUserFilter.VifUserPeakingFilterSelect<=PK_END))
                {
                    VIF_CopyFilter(1);

                    if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_B_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterB_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_GH_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterGH_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_DK_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterDK_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_I_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterI_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_L_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_LL_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterLL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_MN_VHF_L)
                        VIFInitialIn_inst.VifPeakingFilterMN_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_B_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterB_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_GH_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterGH_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_DK_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterDK_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_I_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterI_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_L_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_LL_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterLL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_MN_VHF_H)
                        VIFInitialIn_inst.VifPeakingFilterMN_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_B_UHF)
                        VIFInitialIn_inst.VifPeakingFilterB_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_GH_UHF)
                        VIFInitialIn_inst.VifPeakingFilterGH_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_DK_UHF)
                        VIFInitialIn_inst.VifPeakingFilterDK_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_I_UHF)
                        VIFInitialIn_inst.VifPeakingFilterI_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_L_UHF)
                        VIFInitialIn_inst.VifPeakingFilterL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_LL_UHF)
                        VIFInitialIn_inst.VifPeakingFilterLL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserPeakingFilterSelect==PK_MN_UHF)
                        VIFInitialIn_inst.VifPeakingFilterMN_UHF = 0x80;
                }

                // check if need to update user define YC filter
                if ((sVIFUserFilter.VifUserYcDelayFilterSelect>=YC_START) &&
                    (sVIFUserFilter.VifUserYcDelayFilterSelect<=YC_END))
                {
                    VIF_CopyFilter(2);

                    if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_B_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterB_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_GH_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterGH_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_DK_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterDK_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_I_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterI_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_L_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_LL_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterLL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_MN_VHF_L)
                        VIFInitialIn_inst.VifYcDelayFilterMN_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_B_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterB_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_GH_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterGH_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_DK_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterDK_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_I_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterI_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_L_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_LL_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterLL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_MN_VHF_H)
                        VIFInitialIn_inst.VifYcDelayFilterMN_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_B_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterB_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_GH_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterGH_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_DK_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterDK_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_I_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterI_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_L_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_LL_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterLL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserYcDelayFilterSelect==YC_MN_UHF)
                        VIFInitialIn_inst.VifYcDelayFilterMN_UHF = 0x80;
                }

                // check if need to update user define Group delay filter
                if ((sVIFUserFilter.VifUserGroupDelayFilterSelect>=GP_START) &&
                    (sVIFUserFilter.VifUserGroupDelayFilterSelect<=GP_END))
                {
                    VIF_CopyFilter(3);

                    if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_B_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterB_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_GH_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_DK_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_I_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterI_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_L_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_LL_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_MN_VHF_L)
                        VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_L = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_B_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterB_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_GH_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterGH_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_DK_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterDK_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_I_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterI_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_L_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_LL_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterLL_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_MN_VHF_H)
                        VIFInitialIn_inst.VifGroupDelayFilterMN_VHF_H = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_B_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterB_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_GH_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterGH_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_DK_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterDK_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_I_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterI_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_L_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_LL_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterLL_UHF = 0x80;
                    else if (sVIFUserFilter.VifUserGroupDelayFilterSelect==GP_MN_UHF)
                        VIFInitialIn_inst.VifGroupDelayFilterMN_UHF = 0x80;
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_USER_FILTER");
            break;

        case VIF_PARA_TOP:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIFInitialIn_inst.VifTop = (U8) dwVIFPara;
                msVifTopAdjust();
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_TOP");
            break;

        case VIF_PARA_LEAKY_SEL:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIFInitialIn_inst.VifCrLockLeakySel = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                VIF_WriteBit(LOCK_LEAKY_FF_SEL, VIFInitialIn_inst.VifCrLockLeakySel, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_LEAKY_SEL");
            break;

        case VIF_PARA_K_SEL:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL K_select;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                K_select = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                VIF_WriteBit(CR_K_SEL, K_select, _BIT6);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_K_SEL");
            break;

        case VIF_PARA_GET_VERSION:
            if ( 30 == u32DataLen)
            {
                U8 sVersion[30]={0x00};
                // VIF version
#ifdef _2_DIGIT_CHIP_TYPE_
                memcpy(sVersion, VIF_Version1, 5);
                memcpy(sVersion+5, VIF_Version2, 2);
                memcpy(sVersion+7, VIF_Version3, 1);
                memcpy(sVersion+8, VIF_Version4, 2);
                memcpy(sVersion+10, VIF_Version5, 1);
                memcpy(sVersion+11, VIF_Version6, 2);
                memcpy(sVersion+13, VIF_Version7, 1);
                memcpy(sVersion+14, VIF_Version8, 6);
                memcpy(sVersion+20, VIF_Version9, 1);
                memcpy(sVersion+21, VIF_VersionA, 8);
#else
                memcpy(sVersion, VIF_Version1, 5);
                memcpy(sVersion+5, VIF_Version2, 1);
                memcpy(sVersion+6, VIF_Version3, 1);
                memcpy(sVersion+7, VIF_Version4, 2);
                memcpy(sVersion+9, VIF_Version5, 1);
                memcpy(sVersion+10, VIF_Version6, 2);
                memcpy(sVersion+12, VIF_Version7, 1);
                memcpy(sVersion+13, VIF_Version8, 6);
                memcpy(sVersion+19, VIF_Version9, 1);
                memcpy(sVersion+20, VIF_VersionA, 8);
#endif
                memcpy(pVIF_Para ,sVersion,30);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_VERSION");
            break;

        case VIF_PARA_SET_BYPASS_PEAKING_SOS21:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bBypassSOS21;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bBypassSOS21 = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                VIF_WriteBit(BYPASS_SOS21, bBypassSOS21, _BIT2);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_BYPASS_FILTER");
            break;

        case VIF_PARA_GET_PGA2_LEVEL:
            if ( sizeof(BYTE) == u32DataLen)
            {
                BYTE u8Pga2;
                u8Pga2= VIF_ReadByte(AGC_PGA2C) & 0x1F;
                memcpy(pVIF_Para, &u8Pga2, u32DataLen);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_PGA2_LEVEL");
            break;

	case VIF_PARA_GET_IF:
            if ( sizeof(WORD) == u32DataLen)
	     {
	 	   U16 u16FreqType;// = g_FreqType;

                 u16FreqType = (U16)(VIF_ReadByte(VIF_RF_RESERVED_1));
		   memcpy(pVIF_Para, &u16FreqType, u32DataLen);
	     }
	     else
                 printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_IF");
	     break;

	 case VIF_PARA_GET_K_SEL:
             if ( sizeof(BYTE) == u32DataLen)
             {
                 U8 K_select;

                 if ((VIF_ReadByte(CR_K_SEL) & _BIT6)!=0)
		       K_select=1;
                 else
		       K_select=0;

                 memcpy(pVIF_Para, &K_select,1);
             }
             else
                 printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_K_SEL");
             break;

        case VIF_PARA_SET_HW_KPKI:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                VIF_WriteByte(CR_KP1_HW, (U8) dwVIFPara);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_HW_KPKI");
            break;

        case VIF_PARA_SET_TAGC_ODMODE:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                if ((U8) dwVIFPara==0)
                {
                    VIF_WriteBit(TAGC_ODMODE,  FALSE,  _BIT1);
                    VIF_WriteBit(TAGC_ODMODE,  FALSE,  _BIT5);
                }
                else if ((U8) dwVIFPara==1) // RF AGC OD MODE
                {
                    VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT1);
                }
                else if ((U8) dwVIFPara==2) // IF AGC OD MODE
                {
                    VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT5);
                }
                else if ((U8) dwVIFPara==3)
                {
                    VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT1);
                    VIF_WriteBit(TAGC_ODMODE,  TRUE,  _BIT5);
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_TAGC_ODMODE");
            break;

        case VIF_PARA_SET_NOTCH_A1A2:
            if ( sizeof(VIFNotchA1A2) == u32DataLen)
            {
                memcpy(&sVIFNotchA1A2, (VIFNotchA1A2 *) pVIF_Para, u32DataLen);
                
                if(strcmp(CHIP_TYPE, EDEN) < 0) 
                {
                   VIF_WriteByte(N_A1_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C0 & 0x00FF));
                   VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C0>>8) & 0x00FF));
                   VIF_WriteByte(N_A1_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C1 & 0x00FF));
                   VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C1>>8) & 0x00FF));
                   VIF_WriteByte(N_A1_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C2 & 0x00FF));
                   VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C2>>8) & 0x00FF));
                   VIF_WriteByte(N_A2_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C0 & 0x00FF));
                   VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C0>>8) & 0x00FF));
                   VIF_WriteByte(N_A2_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C1 & 0x00FF));
                   VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C1>>8) & 0x00FF));
                   VIF_WriteByte(N_A2_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C2 & 0x00FF));
                   VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C2>>8) & 0x00FF));
                }
                else
                {
                   if((VIFInitialIn_inst.ChinaDescramblerBox >= 2)&&(VIFInitialIn_inst.ChinaDescramblerBox != 7))
                   {
                      VIF_WriteByte(N_A1_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C0 & 0x00FF));
                      VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C0>>8) & 0x00FF));
                      VIF_WriteByte(N_A1_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C1 & 0x00FF));
                      VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C1>>8) & 0x00FF));
                      VIF_WriteByte(N_A1_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A1_C2 & 0x00FF));
                      VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A1_C2>>8) & 0x00FF));
                      VIF_WriteByte(N_A2_C0_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C0 & 0x00FF));
                      VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C0>>8) & 0x00FF));
                      VIF_WriteByte(N_A2_C1_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C1 & 0x00FF));
                      VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C1>>8) & 0x00FF));
                      VIF_WriteByte(N_A2_C2_L, (U8) (sVIFNotchA1A2.Vif_N_A2_C2 & 0x00FF));
                      VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFNotchA1A2.Vif_N_A2_C2>>8) & 0x00FF));
                   }
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_NOTCH_A1A2");
	     break;

	   case VIF_PARA_SET_SOS_1112:
	 	if( sizeof(VIFSOS1112) == u32DataLen)
	 	{
	 	   memcpy(&sVIFSOS1112, (VIFSOS1112 *) pVIF_Para, u32DataLen);

		   VIF_WriteByte(SOS11_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
		   VIF_WriteByte(SOS11_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0>>8) & 0x00FF));
		   VIF_WriteByte(SOS11_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
		   VIF_WriteByte(SOS11_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1>>8) & 0x00FF));
		   VIF_WriteByte(SOS11_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C2 & 0x00FF));
		   VIF_WriteByte(SOS11_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C2>>8) & 0x00FF));
		   VIF_WriteByte(SOS11_C3_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
		   VIF_WriteByte(SOS11_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3>>8) & 0x00FF));
		   VIF_WriteByte(SOS11_C4_L, (U8) (sVIFSOS1112.Vif_SOS_11_C4 & 0x00FF));
		   VIF_WriteByte(SOS11_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C4>>8) & 0x00FF));
                     
                 if(strcmp(CHIP_TYPE, EDEN) < 0) 
                 {
		      VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_12_C0 & 0x00FF));
		      VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C0>>8) & 0x00FF));
		      VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_12_C1 & 0x00FF));
		      VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C1>>8) & 0x00FF));
		      VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_12_C2 & 0x00FF));
		      VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C2>>8) & 0x00FF));
		      VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_12_C3 & 0x00FF));
		      VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C3>>8) & 0x00FF));
		      VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_12_C4 & 0x00FF));
		      VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C4>>8) & 0x00FF));
                 }
                 else
                 {
                    if((VIFInitialIn_inst.ChinaDescramblerBox >= 2)&&(VIFInitialIn_inst.ChinaDescramblerBox != 7))
                    {
                        VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_12_C0 & 0x00FF));
		          VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C0>>8) & 0x00FF));
		          VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_12_C1 & 0x00FF));
		          VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C1>>8) & 0x00FF));
		          VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_12_C2 & 0x00FF));
		          VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C2>>8) & 0x00FF));
		          VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_12_C3 & 0x00FF));
		          VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C3>>8) & 0x00FF));
		          VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_12_C4 & 0x00FF));
		          VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_12_C4>>8) & 0x00FF));
                    }	
                    else if(VIFInitialIn_inst.ChinaDescramblerBox == 0)
                    {
                        if(strcmp(CHIP_TYPE, MADISON) >= 0)
                        {
                             VIF_WriteByte(N_A1_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
                             VIF_WriteByte(N_A1_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0 >>8) & 0x00FF));
                             VIF_WriteByte(N_A1_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
                             VIF_WriteByte(N_A1_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1 >>8) & 0x00FF));
                             VIF_WriteByte(N_A1_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
                             VIF_WriteByte(N_A1_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3 >>8) & 0x00FF));
                        
                             VIF_WriteByte(N_A2_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
                             VIF_WriteByte(N_A2_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0 >>8) & 0x00FF));
                             VIF_WriteByte(N_A2_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
                             VIF_WriteByte(N_A2_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1 >>8) & 0x00FF));
                             VIF_WriteByte(N_A2_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
                             VIF_WriteByte(N_A2_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3 >>8) & 0x00FF));
                    
                             VIF_WriteByte(SOS12_C0_L, (U8) (sVIFSOS1112.Vif_SOS_11_C0 & 0x00FF));
                             VIF_WriteByte(SOS12_C0_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C0>>8) & 0x00FF));
                             VIF_WriteByte(SOS12_C1_L, (U8) (sVIFSOS1112.Vif_SOS_11_C1 & 0x00FF));
                             VIF_WriteByte(SOS12_C1_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C1>>8) & 0x00FF));
                             VIF_WriteByte(SOS12_C2_L, (U8) (sVIFSOS1112.Vif_SOS_11_C2 & 0x00FF));
                             VIF_WriteByte(SOS12_C2_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C2>>8) & 0x00FF));
                             VIF_WriteByte(SOS12_C3_L, (U8) (sVIFSOS1112.Vif_SOS_11_C3 & 0x00FF));
                             VIF_WriteByte(SOS12_C3_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C3>>8) & 0x00FF));
                             VIF_WriteByte(SOS12_C4_L, (U8) (sVIFSOS1112.Vif_SOS_11_C4 & 0x00FF));
                             VIF_WriteByte(SOS12_C4_H, (U8) ((sVIFSOS1112.Vif_SOS_11_C4>>8) & 0x00FF));
                        }
                    }
                 }
	 	}
		else
		   printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_SOS_1112");
		break;

        case VIF_PARA_GET_PD_INV:
             if ( sizeof(BYTE) == u32DataLen)
            {
                 U8 pd_inv;

                 pd_inv=HAL_VIF_GetCrPDInverse();

                 memcpy(pVIF_Para, &pd_inv,1);
             }
             else
                 printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_PD_INV");
             break;

        case VIF_PARA_SET_PD_INV:
            if ( sizeof(DWORD) == u32DataLen)
            {
                 // if in overmodulation, do NOT diable inverse Qpart
                 if ((VIF_ReadByte(ZERO_DETECT) & _BIT4)==0)
                {
                    memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                    if ((U8) dwVIFPara==1)
                    {
                        HAL_VIF_SetCrPDInverse(TRUE);
                    }
                    else
                    {
                        HAL_VIF_SetCrPDInverse(FALSE);
                    }
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_PD_INV");
            break;

        case VIF_PARA_GET_VGA:
            if ( sizeof(WORD) == u32DataLen)
            {
                U16 u16Vga;
                u16Vga= (U16)(VIF_ReadByte(AGC_VGA))+ (U16)(VIF_ReadByte(AGC_VGA+1)<<8) ;
                memcpy(pVIF_Para, &u16Vga, u32DataLen);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_GET_VGA");
            break;

        case VIF_PARA_SET_CLAMPGAIN_GAIN_OV_POS:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIFInitialIn_inst.VifClampgainGainOvPositive = (U16)dwVIFPara;
                VIF_WriteByte(CLAMPGAIN_GAIN_OVERWRITE, dwVIFPara&0x000000FF);    // clampgain gain overwrite value
                VIF_WriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, dwVIFPara>>8, 0x07);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_CLAMPGAIN_GAIN_OV_POS");
            break;

        case VIF_PARA_SET_CLAMPGAIN_GAIN_OV_NEG:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIFInitialIn_inst.VifClampgainGainOvNegative = (U16)dwVIFPara;
                VIF_WriteByte(CLAMPGAIN_GAIN_OVERWRITE, dwVIFPara&0x000000FF);    // clampgain gain overwrite value
                VIF_WriteByteMask(CLAMPGAIN_GAIN_OVERWRITE+1, dwVIFPara>>8, 0x07);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_CLAMPGAIN_GAIN_OV_NEG");
            break;

        case VIF_PARA_SET_AGC_K:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                if (((dwVIFPara>>24)&0x00000001)==1)
                {
                    bEnableUsrSteadyAgcK = TRUE;
                    u8UsrSteadyAgcK = (dwVIFPara>>16)&0x00000007;
                }
                else
                {
                    bEnableUsrSteadyAgcK = FALSE;
                    u8UsrSteadyAgcK = 0;
                }

                if (((dwVIFPara>>8)&0x00000001)==1)
                {
                    bEnableUsrNonSteadyAgcK = TRUE;
                    u8UsrNonSteadyAgcK = dwVIFPara&0x00000007;
                }
                else
                {
                    bEnableUsrNonSteadyAgcK = FALSE;
                    u8UsrNonSteadyAgcK = 0;
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_AGC_K");
            break;
        case VIF_PARA_SET_AGC_REF:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIFInitialIn_inst.VifAgcRefNegative = (U16)dwVIFPara;
                VIFInitialIn_inst.VifAgcRefPositive = (U16)dwVIFPara;
                VIF_WriteByte(AGC_REF, dwVIFPara&0x000000FF);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_AGC_REF");
            break;            

        case VIF_PARA_SET_IF_RATE_FM:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIF_WriteByte(IF_RATE, (BYTE)(dwVIFPara & 0x000000FF));
                VIF_WriteByte(IF_RATE+1 , (BYTE)((dwVIFPara >> 8) & 0x000000FF));
                VIF_WriteByteMask(IF_RATE+2, (BYTE)((dwVIFPara >> 16) & 0x000000FF), 0x3F);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_IF_RATE_FM");
            break;            

        case VIF_PARA_SET_CR_RATE_FM:
            if ( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                VIF_WriteByte(CR_RATE, (BYTE)(dwVIFPara & 0x000000FF));
                VIF_WriteByte(CR_RATE+1 , (BYTE)((dwVIFPara >> 8) & 0x000000FF));
                VIF_WriteByteMask(CR_RATE+2, (BYTE)((dwVIFPara >> 16) & 0x000000FF), 0x1F);
                VIF_WriteBit(CR_RATE_INV, 0, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_CR_RATE_FM");
            break;   

       case VIF_PARA_SET_DESCRAMBLERBOX_DELAY:
            if( sizeof(DWORD) == u32DataLen)
            {
                 DWORD VifDelayNumTemp;
                 
                 memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);

                 U8 u8IsADC48MHz = VIF_ReadByte(0x122F6)&0x10;

                 // VifShiftClk : 0x1121_D3
                 BYTE u8VifShiftClk = VIF_ReadByte(VIF_RF_RESERVED_1+1);

                 if((strcmp(CHIP_TYPE, EULER) == 0) ||(strcmp(CHIP_TYPE, NIKE) == 0) ||(strcmp(CHIP_TYPE, EIFFEL) == 0))
                 {
                    if(VIFInitialIn_inst.ChinaDescramblerBox == 7)
                    {
                         if(u8VifShiftClk/*g_VifShiftClk*/ == 1)
                         {
                             VifDelayNumTemp = dwVIFPara;
                             VifDelayNumTemp = (VifDelayNumTemp * 1992) >> 11;
                             VIF_WriteByte(0x123F4, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAGC1_DLY_NUM
                             VIF_WriteByteMask(0x123F5, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x1F);
                         }
                         else if(u8VifShiftClk/*g_VifShiftClk*/ == 2)
                         {
                             VifDelayNumTemp = dwVIFPara;
                             VifDelayNumTemp = (VifDelayNumTemp * 2105) >> 11;
                             VIF_WriteByte(0x123F4, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAGC1_DLY_NUM
                             VIF_WriteByteMask(0x123F5, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x1F);
                         }
                         else
                         {
                             VIF_WriteByte(0x123F4, (BYTE)(dwVIFPara & 0x000000FF)); // DAGC1_DLY_NUM
                             VIF_WriteByteMask(0x123F5, (BYTE)((dwVIFPara >> 8) & 0x000000FF), 0x1F);
                         }
                        
                         VIF_WriteBit(0x123F5, 1, _BIT7);                                 // DAGC1_DLY_EN    
                         VIF_WriteByteMask(VIFDAC_OUT_SEL, 0x04, 0x07);    // 0: cvbs output; 4: debug bus
                         VIF_WriteBit(DEBUG2_EN, 1, _BIT7);                           // select debug2 data
                         VIF_WriteByteMask(DEBUG_MODULE, 0x00, 0x0F);      // select filter debug bus
                         VIF_WriteByte(DEBUG_PORT, 0x8D);                           // selsect CVBS output after DAGC1 filter

                         if(strcmp(CHIP_TYPE, NIKE) == 0)
                              VIF_WriteBit(DAGC2_DL_BYPASS, 1, _BIT3);
                    }
                 }
                 else if(strcmp(CHIP_TYPE, EINSTEIN) >= 0)
                 {
                    if(VIFInitialIn_inst.ChinaDescramblerBox == 0)
                    {
                         if((u8VifShiftClk/*g_VifShiftClk*/ == 1)&&(u8IsADC48MHz == 0))
                         {
                             VifDelayNumTemp = dwVIFPara;
                             VifDelayNumTemp = (VifDelayNumTemp * 1992) >> 11;
                             
                             if(strcmp(CHIP_TYPE, MUJI) == 0)                                
                                 VifDelayNumTemp = VifDelayNumTemp + 0x00000409; // offset = 24.6us
                             else
                                 VifDelayNumTemp = VifDelayNumTemp + 0x00000751; // offset = 44.6us
                                 
                             VIF_WriteByte(0x12124, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAC_DLY_NUM
                             VIF_WriteByteMask(0x12125, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x0F);
                         }
                         else if((u8VifShiftClk/*g_VifShiftClk*/ == 2)&&(u8IsADC48MHz == 0))
                         {
                             VifDelayNumTemp = dwVIFPara; 
                             VifDelayNumTemp = (VifDelayNumTemp * 2105) >> 11;

                             if(strcmp(CHIP_TYPE, MUJI) == 0) 
                                 VifDelayNumTemp = VifDelayNumTemp + 0x0000068A; // offset = 37.7us
                             else
                                 VifDelayNumTemp = VifDelayNumTemp + 0x00000330; // offset = 18.4us
                                 
                             VIF_WriteByte(0x12124, (BYTE)(VifDelayNumTemp & 0x000000FF)); // DAC_DLY_NUM
                             VIF_WriteByteMask(0x12125, (BYTE)((VifDelayNumTemp >> 8) & 0x000000FF), 0x0F);
                         }
                         else
                         {
                             VIF_WriteByte(0x12124, (BYTE)(dwVIFPara & 0x000000FF)); // DAC_DLY_NUM
                             VIF_WriteByteMask(0x12125, (BYTE)((dwVIFPara >> 8) & 0x000000FF), 0x0F);
                         }
                         
                         VIF_WriteBit(0x12125, 0, _BIT7);                                  //DAC_DLY not bypass
                         VIF_WriteByteMask(VIFDAC_OUT_SEL, 0x00, 0x07);    // 0: cvbs output; 4: debug bus
                         VIF_WriteBit(DEBUG2_EN, 0, _BIT7);                           // not select debug2 data
                    }
                }
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_DESCRAMBLERBOX_DELAY");
            break;

        case VIF_PARA_SET_SSC_ENABLE:
            if( sizeof(DWORD) == u32DataLen)
            {
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                
                if((dwVIFPara&0x00000001) != 0)
                   VIF_WriteBit(0x120A1, 1, _BIT0);
                else
                   VIF_WriteBit(0x120A1, 0, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_SSC_ENABLE");
            break;
            
        case VIF_PARA_SET_DISABLE_ADAGC:
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bDisableAdagc;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bDisableAdagc = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                VIF_WriteBit(ADAGC_BYPASS, bDisableAdagc, _BIT1);
                VIF_WriteBit(ADAGC_ENABLE, !bDisableAdagc, _BIT0);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_DISABLE_ADAGC");
            break;
           
        case VIF_PARA_SET_ENABLE_SOS33:  
            if ( sizeof(DWORD) == u32DataLen)
            {
                BOOL bEnableSOS33;
                memcpy(&dwVIFPara, (DWORD *) pVIF_Para, u32DataLen);
                bEnableSOS33 = ((U8) dwVIFPara==0) ? FALSE:TRUE;
                VIF_WriteBit(0x1220A, !bEnableSOS33, _BIT6);
            }
            else
                printf("\r\nERROR!! vif parameter copy failed with wrong size: VIF_PARA_SET_ENABLE_SOS33");
            break;
     
        default:
            break;
    }
    return TRUE;
}

void VIF_ShiftClk(BYTE VifShiftClk)
{
    DRVVIFDBG(printf("DRV_VIF_ShiftClk() VifShiftClk=%d\n",VifShiftClk));
    #ifdef _2_DIGIT_CHIP_TYPE_
    msVifShiftClk(VifShiftClk);
    #endif
}

void VIF_GetSTRParameter(void)
{
    //printf("\r\n ======== VIF_GetSTRParameter ========= \n");

    sVIFSTRInfo.Vif_Clamp_L = VIF_ReadByte(CLAMPGAIN_CLAMP_OVERWRITE);
    sVIFSTRInfo.Vif_Clamp_H = VIF_ReadByte(CLAMPGAIN_CLAMP_OVERWRITE+1);
    sVIFSTRInfo.Vif_Clamp_Gain_L = VIF_ReadByte(CLAMPGAIN_GAIN_OVERWRITE);
    sVIFSTRInfo.Vif_Clamp_Gain_H = VIF_ReadByte(CLAMPGAIN_GAIN_OVERWRITE+1);

    sVIFSTRInfo.Vif_Cr_Rate_1 = VIF_ReadByte(CR_RATE);
    sVIFSTRInfo.Vif_Cr_Rate_2 = VIF_ReadByte(CR_RATE+1);
    sVIFSTRInfo.Vif_Cr_Rate_3 = VIF_ReadByte(CR_RATE+2);
    sVIFSTRInfo.Vif_Cr_Rate_Inv = VIF_ReadByte(CR_RATE_INV);

    sVIFSTRInfo.Vif_N_A1_C0_L = VIF_ReadByte(N_A1_C0_L);
    sVIFSTRInfo.Vif_N_A1_C0_H = VIF_ReadByte(N_A1_C0_H);
    sVIFSTRInfo.Vif_N_A1_C1_L = VIF_ReadByte(N_A1_C1_L);
    sVIFSTRInfo.Vif_N_A1_C1_H = VIF_ReadByte(N_A1_C1_H);
    sVIFSTRInfo.Vif_N_A1_C2_L = VIF_ReadByte(N_A1_C2_L);
    sVIFSTRInfo.Vif_N_A1_C2_H = VIF_ReadByte(N_A1_C2_H);

    sVIFSTRInfo.Vif_N_A2_C0_L = VIF_ReadByte(N_A2_C0_L);
    sVIFSTRInfo.Vif_N_A2_C0_H = VIF_ReadByte(N_A2_C0_H);
    sVIFSTRInfo.Vif_N_A2_C1_L = VIF_ReadByte(N_A2_C1_L);
    sVIFSTRInfo.Vif_N_A2_C1_H = VIF_ReadByte(N_A2_C1_H);
    sVIFSTRInfo.Vif_N_A2_C2_L = VIF_ReadByte(N_A2_C2_L);
    sVIFSTRInfo.Vif_N_A2_C2_H = VIF_ReadByte(N_A2_C2_H);

    sVIFSTRInfo.Vif_SOS_11_C0_L = VIF_ReadByte(SOS11_C0_L);
    sVIFSTRInfo.Vif_SOS_11_C0_H = VIF_ReadByte(SOS11_C0_H);
    sVIFSTRInfo.Vif_SOS_11_C1_L = VIF_ReadByte(SOS11_C1_L);
    sVIFSTRInfo.Vif_SOS_11_C1_H = VIF_ReadByte(SOS11_C1_H);
    sVIFSTRInfo.Vif_SOS_11_C2_L = VIF_ReadByte(SOS11_C2_L);
    sVIFSTRInfo.Vif_SOS_11_C2_H = VIF_ReadByte(SOS11_C2_H);
    sVIFSTRInfo.Vif_SOS_11_C3_L = VIF_ReadByte(SOS11_C3_L);
    sVIFSTRInfo.Vif_SOS_11_C3_H = VIF_ReadByte(SOS11_C3_H);
    sVIFSTRInfo.Vif_SOS_11_C4_L = VIF_ReadByte(SOS11_C4_L);
    sVIFSTRInfo.Vif_SOS_11_C4_H = VIF_ReadByte(SOS11_C4_H);

    sVIFSTRInfo.Vif_SOS_12_C0_L = VIF_ReadByte(SOS12_C0_L);
    sVIFSTRInfo.Vif_SOS_12_C0_H = VIF_ReadByte(SOS12_C0_H);
    sVIFSTRInfo.Vif_SOS_12_C1_L = VIF_ReadByte(SOS12_C1_L);
    sVIFSTRInfo.Vif_SOS_12_C1_H = VIF_ReadByte(SOS12_C1_H);
    sVIFSTRInfo.Vif_SOS_12_C2_L = VIF_ReadByte(SOS12_C2_L);
    sVIFSTRInfo.Vif_SOS_12_C2_H = VIF_ReadByte(SOS12_C2_H);
    sVIFSTRInfo.Vif_SOS_12_C3_L = VIF_ReadByte(SOS12_C3_L);
    sVIFSTRInfo.Vif_SOS_12_C3_H = VIF_ReadByte(SOS12_C3_H);
    sVIFSTRInfo.Vif_SOS_12_C4_L = VIF_ReadByte(SOS12_C4_L);
    sVIFSTRInfo.Vif_SOS_12_C4_H = VIF_ReadByte(SOS12_C4_H);

    sVIFSTRInfo.VifSos21FilterC0_L = VIF_ReadByte(SOS21_C0_L);
    sVIFSTRInfo.VifSos21FilterC0_H = VIF_ReadByte(SOS21_C0_H);
    sVIFSTRInfo.VifSos21FilterC1_L = VIF_ReadByte(SOS21_C1_L);
    sVIFSTRInfo.VifSos21FilterC1_H = VIF_ReadByte(SOS21_C1_H);
    sVIFSTRInfo.VifSos21FilterC2_L = VIF_ReadByte(SOS21_C2_L);
    sVIFSTRInfo.VifSos21FilterC2_H = VIF_ReadByte(SOS21_C2_H);
    sVIFSTRInfo.VifSos21FilterC3_L = VIF_ReadByte(SOS21_C3_L);
    sVIFSTRInfo.VifSos21FilterC3_H = VIF_ReadByte(SOS21_C3_H);
    sVIFSTRInfo.VifSos21FilterC4_L = VIF_ReadByte(SOS21_C4_L);
    sVIFSTRInfo.VifSos21FilterC4_H = VIF_ReadByte(SOS21_C4_H);

    sVIFSTRInfo.VifSos22FilterC0_L = VIF_ReadByte(SOS22_C0_L);
    sVIFSTRInfo.VifSos22FilterC0_H = VIF_ReadByte(SOS22_C0_H);
    sVIFSTRInfo.VifSos22FilterC1_L= VIF_ReadByte(SOS22_C1_L);
    sVIFSTRInfo.VifSos22FilterC1_H = VIF_ReadByte(SOS22_C1_H);
    sVIFSTRInfo.VifSos22FilterC2_L = VIF_ReadByte(SOS22_C2_L);
    sVIFSTRInfo.VifSos22FilterC2_H = VIF_ReadByte(SOS22_C2_H);
    sVIFSTRInfo.VifSos22FilterC3_L = VIF_ReadByte(SOS22_C3_L);
    sVIFSTRInfo.VifSos22FilterC3_H = VIF_ReadByte(SOS22_C3_H);
    sVIFSTRInfo.VifSos22FilterC4_L = VIF_ReadByte(SOS22_C4_L);
    sVIFSTRInfo.VifSos22FilterC4_H = VIF_ReadByte(SOS22_C4_H);

    sVIFSTRInfo.VifSos31FilterC0_L = VIF_ReadByte(SOS31_C0_L);
    sVIFSTRInfo.VifSos31FilterC0_H = VIF_ReadByte(SOS31_C0_H);
    sVIFSTRInfo.VifSos31FilterC1_L = VIF_ReadByte(SOS31_C1_L);
    sVIFSTRInfo.VifSos31FilterC1_H = VIF_ReadByte(SOS31_C1_H);
    sVIFSTRInfo.VifSos31FilterC2_L = VIF_ReadByte(SOS31_C2_L);
    sVIFSTRInfo.VifSos31FilterC2_H = VIF_ReadByte(SOS31_C2_H);
    sVIFSTRInfo.VifSos31FilterC3_L = VIF_ReadByte(SOS31_C3_L);
    sVIFSTRInfo.VifSos31FilterC3_H = VIF_ReadByte(SOS31_C3_H);
    sVIFSTRInfo.VifSos31FilterC4_L = VIF_ReadByte(SOS31_C4_L);
    sVIFSTRInfo.VifSos31FilterC4_H = VIF_ReadByte(SOS31_C4_H);

    sVIFSTRInfo.VifSos32FilterC0_L = VIF_ReadByte(SOS32_C0_L);
    sVIFSTRInfo.VifSos32FilterC0_H = VIF_ReadByte(SOS32_C0_H);
    sVIFSTRInfo.VifSos32FilterC1_L = VIF_ReadByte(SOS32_C1_L);
    sVIFSTRInfo.VifSos32FilterC1_H = VIF_ReadByte(SOS32_C1_H);
    sVIFSTRInfo.VifSos32FilterC2_L = VIF_ReadByte(SOS32_C2_L);
    sVIFSTRInfo.VifSos32FilterC2_H = VIF_ReadByte(SOS32_C2_H);
    sVIFSTRInfo.VifSos32FilterC3_L = VIF_ReadByte(SOS32_C3_L);
    sVIFSTRInfo.VifSos32FilterC3_H = VIF_ReadByte(SOS32_C3_H);
    sVIFSTRInfo.VifSos32FilterC4_L = VIF_ReadByte(SOS32_C4_L);
    sVIFSTRInfo.VifSos32FilterC4_H = VIF_ReadByte(SOS32_C4_H);

    sVIFSTRInfo.Vif_Agc_Ref = VIF_ReadByte(AGC_REF);
    sVIFSTRInfo.Vif_CR_K_Sel = VIF_ReadByte(CR_K_SEL);
    sVIFSTRInfo.Vif_CR_KpKi = VIF_ReadByte(CR_KP_SW);
}

MS_U32 VIF_SetPowerState(EN_POWER_MODE u16PowerState)
{    
    MS_U32 u32Return = 1;

    if(bIsATV == TRUE)
    {
         if(u16PowerState == E_POWER_RESUME)   // RESUME
         { 
             VIF_Init(&VIFInitialIn_inst, sizeof(VIFInitialIn_inst));
             
             if((gVIFSoundSystem == VIF_SOUND_L)||(gVIFSoundSystem == VIF_SOUND_LL))
             {
                 VIF_SetSoundSystem(gVIFSoundSystem);
                 VIF_Reset();
             }
             
             VIF_SetSoundSystem(gVIFSoundSystem);
             
             VIF_WriteByte(CLAMPGAIN_CLAMP_OVERWRITE, sVIFSTRInfo.Vif_Clamp_L);
             VIF_WriteByte(CLAMPGAIN_CLAMP_OVERWRITE+1, sVIFSTRInfo.Vif_Clamp_H);
             VIF_WriteByte(CLAMPGAIN_GAIN_OVERWRITE, sVIFSTRInfo.Vif_Clamp_Gain_L);
             VIF_WriteByte(CLAMPGAIN_GAIN_OVERWRITE+1, sVIFSTRInfo.Vif_Clamp_Gain_H);
             
             VIF_WriteByte(CR_RATE, sVIFSTRInfo.Vif_Cr_Rate_1);
             VIF_WriteByte(CR_RATE+1, sVIFSTRInfo.Vif_Cr_Rate_2);
             VIF_WriteByte(CR_RATE+2, sVIFSTRInfo.Vif_Cr_Rate_3);
             VIF_WriteByte(CR_RATE_INV, sVIFSTRInfo.Vif_Cr_Rate_Inv);
             
             VIF_WriteByte(N_A1_C0_L, sVIFSTRInfo.Vif_N_A1_C0_L);
             VIF_WriteByte(N_A1_C0_H, sVIFSTRInfo.Vif_N_A1_C0_H);
             VIF_WriteByte(N_A1_C1_L, sVIFSTRInfo.Vif_N_A1_C1_L);
             VIF_WriteByte(N_A1_C1_H, sVIFSTRInfo.Vif_N_A1_C1_H);
             VIF_WriteByte(N_A1_C2_L, sVIFSTRInfo.Vif_N_A1_C2_L);
             VIF_WriteByte(N_A1_C2_H, sVIFSTRInfo.Vif_N_A1_C2_H);
             
             VIF_WriteByte(N_A2_C0_L, sVIFSTRInfo.Vif_N_A2_C0_L);
             VIF_WriteByte(N_A2_C0_H, sVIFSTRInfo.Vif_N_A2_C0_H);
             VIF_WriteByte(N_A2_C1_L, sVIFSTRInfo.Vif_N_A2_C1_L);
             VIF_WriteByte(N_A2_C1_H, sVIFSTRInfo.Vif_N_A2_C1_H);
             VIF_WriteByte(N_A2_C2_L, sVIFSTRInfo.Vif_N_A2_C2_L);
             VIF_WriteByte(N_A2_C2_H, sVIFSTRInfo.Vif_N_A2_C2_H);
             
             VIF_WriteByte(SOS11_C0_L, sVIFSTRInfo.Vif_SOS_11_C0_L);
             VIF_WriteByte(SOS11_C0_H, sVIFSTRInfo.Vif_SOS_11_C0_H);
             VIF_WriteByte(SOS11_C1_L, sVIFSTRInfo.Vif_SOS_11_C1_L);
             VIF_WriteByte(SOS11_C1_H, sVIFSTRInfo.Vif_SOS_11_C1_H);
             VIF_WriteByte(SOS11_C2_L, sVIFSTRInfo.Vif_SOS_11_C2_L);
             VIF_WriteByte(SOS11_C2_H, sVIFSTRInfo.Vif_SOS_11_C2_H);
             VIF_WriteByte(SOS11_C3_L, sVIFSTRInfo.Vif_SOS_11_C3_L);
             VIF_WriteByte(SOS11_C3_H, sVIFSTRInfo.Vif_SOS_11_C3_H);
             VIF_WriteByte(SOS11_C4_L, sVIFSTRInfo.Vif_SOS_11_C4_L);
             VIF_WriteByte(SOS11_C4_H, sVIFSTRInfo.Vif_SOS_11_C4_H);
             
             VIF_WriteByte(SOS12_C0_L, sVIFSTRInfo.Vif_SOS_12_C0_L);
             VIF_WriteByte(SOS12_C0_H, sVIFSTRInfo.Vif_SOS_12_C0_H);
             VIF_WriteByte(SOS12_C1_L, sVIFSTRInfo.Vif_SOS_12_C1_L);
             VIF_WriteByte(SOS12_C1_H, sVIFSTRInfo.Vif_SOS_12_C1_H);
             VIF_WriteByte(SOS12_C2_L, sVIFSTRInfo.Vif_SOS_12_C2_L);
             VIF_WriteByte(SOS12_C2_H, sVIFSTRInfo.Vif_SOS_12_C2_H);
             VIF_WriteByte(SOS12_C3_L, sVIFSTRInfo.Vif_SOS_12_C3_L);
             VIF_WriteByte(SOS12_C3_H, sVIFSTRInfo.Vif_SOS_12_C3_H);
             VIF_WriteByte(SOS12_C4_L, sVIFSTRInfo.Vif_SOS_12_C4_L);
             VIF_WriteByte(SOS12_C4_H, sVIFSTRInfo.Vif_SOS_12_C4_H);
             
             VIF_WriteByte(SOS21_C0_L, sVIFSTRInfo.VifSos21FilterC0_L);
             VIF_WriteByte(SOS21_C0_H, sVIFSTRInfo.VifSos21FilterC0_H);
             VIF_WriteByte(SOS21_C1_L, sVIFSTRInfo.VifSos21FilterC1_L);
             VIF_WriteByte(SOS21_C1_H, sVIFSTRInfo.VifSos21FilterC1_H);
             VIF_WriteByte(SOS21_C2_L, sVIFSTRInfo.VifSos21FilterC2_L);
             VIF_WriteByte(SOS21_C2_H, sVIFSTRInfo.VifSos21FilterC2_H);
             VIF_WriteByte(SOS21_C3_L, sVIFSTRInfo.VifSos21FilterC3_L);
             VIF_WriteByte(SOS21_C3_H, sVIFSTRInfo.VifSos21FilterC3_H);
             VIF_WriteByte(SOS21_C4_L, sVIFSTRInfo.VifSos21FilterC4_L);
             VIF_WriteByte(SOS21_C4_H, sVIFSTRInfo.VifSos21FilterC4_H);
             
             VIF_WriteByte(SOS22_C0_L, sVIFSTRInfo.VifSos22FilterC0_L);
             VIF_WriteByte(SOS22_C0_H, sVIFSTRInfo.VifSos22FilterC0_H);
             VIF_WriteByte(SOS22_C1_L, sVIFSTRInfo.VifSos22FilterC1_L);
             VIF_WriteByte(SOS22_C1_H, sVIFSTRInfo.VifSos22FilterC1_H);
             VIF_WriteByte(SOS22_C2_L, sVIFSTRInfo.VifSos22FilterC2_L);
             VIF_WriteByte(SOS22_C2_H, sVIFSTRInfo.VifSos22FilterC2_H);
             VIF_WriteByte(SOS22_C3_L, sVIFSTRInfo.VifSos22FilterC3_L);
             VIF_WriteByte(SOS22_C3_H, sVIFSTRInfo.VifSos22FilterC3_H);
             VIF_WriteByte(SOS22_C4_L, sVIFSTRInfo.VifSos22FilterC4_L);
             VIF_WriteByte(SOS22_C4_H, sVIFSTRInfo.VifSos22FilterC4_H);
             
             VIF_WriteByte(SOS31_C0_L, sVIFSTRInfo.VifSos31FilterC0_L);
             VIF_WriteByte(SOS31_C0_H, sVIFSTRInfo.VifSos31FilterC0_H);
             VIF_WriteByte(SOS31_C1_L, sVIFSTRInfo.VifSos31FilterC1_L);
             VIF_WriteByte(SOS31_C1_H, sVIFSTRInfo.VifSos31FilterC1_H);
             VIF_WriteByte(SOS31_C2_L, sVIFSTRInfo.VifSos31FilterC2_L);
             VIF_WriteByte(SOS31_C2_H, sVIFSTRInfo.VifSos31FilterC2_H);
             VIF_WriteByte(SOS31_C3_L, sVIFSTRInfo.VifSos31FilterC3_L);
             VIF_WriteByte(SOS31_C3_H, sVIFSTRInfo.VifSos31FilterC3_H);
             VIF_WriteByte(SOS31_C4_L, sVIFSTRInfo.VifSos31FilterC4_L);
             VIF_WriteByte(SOS31_C4_H, sVIFSTRInfo.VifSos31FilterC4_H);
             
             VIF_WriteByte(SOS32_C0_L, sVIFSTRInfo.VifSos32FilterC0_L);
             VIF_WriteByte(SOS32_C0_H, sVIFSTRInfo.VifSos32FilterC0_H);
             VIF_WriteByte(SOS32_C1_L, sVIFSTRInfo.VifSos32FilterC1_L);
             VIF_WriteByte(SOS32_C1_H, sVIFSTRInfo.VifSos32FilterC1_H);
             VIF_WriteByte(SOS32_C2_L, sVIFSTRInfo.VifSos32FilterC2_L);
             VIF_WriteByte(SOS32_C2_H, sVIFSTRInfo.VifSos32FilterC2_H);
             VIF_WriteByte(SOS32_C3_L, sVIFSTRInfo.VifSos32FilterC3_L);
             VIF_WriteByte(SOS32_C3_H, sVIFSTRInfo.VifSos32FilterC3_H);
             VIF_WriteByte(SOS32_C4_L, sVIFSTRInfo.VifSos32FilterC4_L);
             VIF_WriteByte(SOS32_C4_H, sVIFSTRInfo.VifSos32FilterC4_H);
             
             VIF_WriteByte(AGC_REF, sVIFSTRInfo.Vif_Agc_Ref);
             VIF_WriteByte(CR_K_SEL, sVIFSTRInfo.Vif_CR_K_Sel);
             VIF_WriteByte(CR_KP_SW, sVIFSTRInfo.Vif_CR_KpKi);
             
             //printf("\r\n ======== VIF_SetPowerState_Resume ========= \n");
             
             u32Return = 0; //RESUME_OK;
         }
         else if(u16PowerState == E_POWER_SUSPEND)
         {    
             VIF_GetSTRParameter();
         
             //printf("\r\n ======== VIF_SetPowerState_Suspend ========= \n");
         
             u32Return = 0; //SUSPEND_OK;
         }
         else
         {
            printf("\r\n ====== [VIF_SetPowerState] Uknown Power State ====== \n");
            u32Return = 1;
         }
    }
    else
    {
         printf("\r\n ====== VIF don't need to Suspend/Resume at Non-ATV mode ====== \r\n");
         u32Return = 0; 
    }

    return u32Return;
}

#endif //_VIF_C_

