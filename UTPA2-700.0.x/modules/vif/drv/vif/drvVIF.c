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
#ifndef _DRVVIF_C_
#define _DRVVIF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
//#include "MsVersion.h"
#include "MsCommon.h"
//#include "halVIF.h"
//#include "halVIF_Customer.h"
//#include "regVIF.h"
#include "drvVIF.h"
#include "drvVIF_v2.h"
#include "../../utopia_core/utopia.h"

//-------------------------------------------------------------------------------------------------
//  Global variables
//-------------------------------------------------------------------------------------------------

void* ppVIFInstant = NULL;
static MS_U32 u32VIFopen = 0;  
    
//-------------------------------------------------------------------------------------------------
//  Global VIF functions
//-------------------------------------------------------------------------------------------------

void DRV_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    VIF_SETREGVALUE SetRegInfo = {0};

    SetRegInfo.u32Addr = u32Reg;
    SetRegInfo.u8Value = u8Val;
    
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_WriteByte, &SetRegInfo);
    else
        printf("\r\n ======== DRV_VIF_WriteByte/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

U8 DRV_VIF_ReadByte(U32 u32Reg )
{
    VIF_SETREGVALUE SetRegInfo = {0};

    SetRegInfo.u32Addr = u32Reg;
    SetRegInfo.u8Value = 0;

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_ReadByte, &SetRegInfo);
        return SetRegInfo.u8Value;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_ReadByte/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return 0;
    }
}

void DRV_VIF_Version(void)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Version, NULL);
    else
        printf("\r\n ======== DRV_VIF_Version/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

U8 DRV_VIF_Read_CR_FOE(void)
{
    VIF_COPYTOUSER CopyToUser = {0};

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Read_CR_FOE, &CopyToUser);
        return CopyToUser.u8Value;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_Read_CR_FOE/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return 0;
    }
}

U8 DRV_VIF_Read_CR_LOCK_STATUS(void)
{
    VIF_COPYTOUSER CopyToUser = {0};

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Read_CR_LOCK_STATUS, &CopyToUser);
        return CopyToUser.u8Value;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_Read_CR_LOCK_STATUS/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return 0;
    }
}

void DRV_VIF_SetClock(BOOL bEnable)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetClock, &bEnable);
    else
        printf("\r\n ======== DRV_VIF_SetClock/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen)
{
    eVIF_INIT _eVIF_Init = {0};
    
    _eVIF_Init.pVIF_InitData = pVIF_InitData;
    _eVIF_Init.u32InitDataLen = u32InitDataLen;

    //printf("\r\n ========= DRV_VIF_Init =========== \n");

    void* pAttribte = NULL;
    if(u32VIFopen == 0)  // First time open
    {
        //if(UtopiaOpen(MODULE_VIF |KERNEL_MODE, &ppVIFInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
        if(UtopiaOpen(MODULE_VIF, &ppVIFInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
        {
            u32VIFopen = 1;

            UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Init, &_eVIF_Init);

            //printf("\r\n ======== DRV_VIF_Init/VIF Open Successful %x =========", (WORD)u32VIFopen);
        }
        else
            printf("\r\n ======== DRV_VIF_Init/VIF Open Fail %x =========", (WORD)u32VIFopen);
    }
    else
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Reset, NULL);
}

void DRV_VIF_Exit(void)
{
    //printf("\r\n ========== DRV_VIF_Exit ========== \n");

    if(u32VIFopen == 1)
    {
        u32VIFopen = 0;
    
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Exit, NULL);
    }
    else
        printf("\r\n ======== DRV_VIF_Exit/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_Reset(void)
{
    //printf("\r\n ========== DRV_VIF_Reset ========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Reset, NULL);
    else
        printf("\r\n ======== DRV_VIF_Reset/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_Handler(BOOL bAutoScan)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Handler, &bAutoScan);
    else
        printf("\r\n ======== DRV_VIF_Handler/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    //printf("\r\n ========= DRV_VIF_SetSoundSystem ========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetSoundSystem, &ucSoundSystem);
    else
        printf("\r\n ======== DRV_VIF_SetSoundSystem/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_SetIfFreq(IfFrequencyType u16IfFreq)
{
    //printf("\r\n ========= DRV_VIF_SetIfFreq =========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetIfFreq, &u16IfFreq);
    else
        printf("\r\n ======== DRV_VIF_SetIfFreq/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_BypassDBBAudioFilter, &bEnable);
    else
        printf("\r\n ======== DRV_VIF_BypassDBBAudioFilter/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_SetFreqBand(FrequencyBand u8FreqBand)
{
    //printf("\r\n ========= DRV_VIF_SetFreqBand =========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetFreqBand, &u8FreqBand);
    else
        printf("\r\n ======== DRV_VIF_SetFreqBand/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

BOOL DRV_VIF_GetInputLevelIndicator(void)
{
    VIF_COPYTOUSER CopyToUser = {0};

    //printf("\r\n ========= DRV_VIF_GetInputLevelIndicator =========== \n");

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_GetInputLevelIndicator, &CopyToUser);
        return CopyToUser.bEnable;
    }
    else 
    {
        printf("\r\n ======== DRV_VIF_GetInputLevelIndicator/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return FALSE;
    }
}

BOOL DRV_VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen)
{
    //printf("\r\n =========== DRV_VIF_SetParameter =========== \n");

    VIF_SETPARA SetParaInfo = {0};

    SetParaInfo.ParaGroup = paraGroup;
    SetParaInfo.pVIF_Para = pVIF_Para;
    SetParaInfo.u32DataLen = u32DataLen;
    SetParaInfo.bRet = 0;

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetParameter, &SetParaInfo);
        return SetParaInfo.bRet;
    }
    else 
    {
        printf("\r\n ======== DRV_VIF_SetParameter/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return FALSE;
    }
}

void DRV_VIF_ShiftClk(BYTE VifShiftClk)
{
     //printf("\r\n =========== DRV_VIF_ShiftClk =========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_ShiftClk, &VifShiftClk);
    else
        printf("\r\n ======== DRV_VIF_ShiftClk/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

MS_U32 MDrv_VIF_SetPowerState(EN_POWER_MODE u16PowerState)
{
    VIF_COPYTOUSER CopyToUser = {0};
    MS_U32 u32Ret = 0;

    CopyToUser.u16PowerState = u16PowerState;
    CopyToUser.u32Value = 0;

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetPowerState, &CopyToUser);
        
        u32Ret = CopyToUser.u32Value;
        
        if(u32Ret == 0)
            return UTOPIA_STATUS_SUCCESS;
        else
            return UTOPIA_STATUS_FAIL;
    }
    else 
    {
        printf("\r\n ======== MDrv_VIF_SetPowerState/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return UTOPIA_STATUS_FAIL;
    }
}

#endif //_DRVVIF_C_

