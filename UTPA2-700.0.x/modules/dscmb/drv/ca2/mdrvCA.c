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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file   mdrvDSCMB.c
/// @brief   API Interface
/// @author MStar Semiconductor,Inc.
/// @attention
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#include "MsTypes.h"

#include "drvCA_v2.h"
#include "drvCA_private.h"

#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
void* pModuleCA = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void CARegisterToUtopia(void)
{
    CA_RESOURCE_PRIVATE * pResPri = NULL;
    void* psResource = NULL;

    // 1. deal with module

    // create a CA module
    // a. size = 8
    // b. Dst: pModuleDscmb
    UtopiaModuleCreate(MODULE_CA, 8, &pModuleCA);
    // register a dscmb module
    UtopiaModuleRegister(pModuleCA);
    //setup module functions
    UtopiaModuleSetupFunctionPtr(pModuleCA, (FUtopiaOpen)CAOpen, (FUtopiaClose)CAClose, (FUtopiaIOctl)CAIoctl);

    // 2. deal with resource
    // create and allocate resource private data(drv Global shared data)
    UtopiaResourceCreate("CA", sizeof(CA_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleCA, psResource, E_CA_RESOURCE);
    UtopiaModuleAddResourceEnd(pModuleCA, E_CA_RESOURCE);

    if(UtopiaResourceObtain(pModuleCA, E_CA_RESOURCE, &psResource) != UTOPIA_STATUS_SUCCESS)
    {
         MS_DEBUG_MSG(printf("[%s][%d] UtopiaResourceObtain fail\n", __FUNCTION__, __LINE__));
         return ;
    }
    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    UtopiaResourceRelease(psResource);
}

MS_U32 CAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MS_DEBUG_MSG(printf("[%s][%d] DSCMB open \n", __FUNCTION__, __LINE__));

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

    CA_INSTANT_PRIVATE*    psCAInstPri = NULL;

    //create instance
    //1.  Allocate instant private data, prepare argument variable.
    //2.  Dst: ppInstance
    UtopiaInstanceCreate(sizeof(CA_INSTANT_PRIVATE), ppInstance);

    //get instant private data pointer
    // 1. Dst: psDscmbInstPri
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psCAInstPri);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 CAIoctl(void* pInstance, MS_U32 u32Cmd, void* pu32Args)
{
    void* pstRes = NULL;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    CA_RESOURCE_PRIVATE* pstResPri  = NULL;

    if(UtopiaResourceObtain(pModuleCA, E_CA_RESOURCE, &pstRes) != UTOPIA_STATUS_SUCCESS)
    {
         MS_DEBUG_MSG(printf("[%s][%d] UtopiaResourceObtain fail\n", __FUNCTION__, __LINE__));
         return UTOPIA_STATUS_ERR_RESOURCE;
    }
    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    switch(u32Cmd)
    {
        case E_MDRV_CMD_CA_Init:
        {
            if(TRUE == _MDrv_CA_Init())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_Init fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_Init\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_EnableSecureBoot:
        {

            if(TRUE == _MDrv_CA_OTP_EnableSecureBoot())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_EnableSecureBoot fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_EnableSecureBoot\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_IsSecureBootEnabled:
        {

            if(TRUE == _MDrv_CA_OTP_IsSecureBootEnabled())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_IsSecureBootEnabled fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_IsSecureBootEnabled\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_SetBlockLock:
        {
            CA_OTP_SETBLOCKLOCK* pSetBlockLock = (CA_OTP_SETBLOCKLOCK*)pu32Args;

            if(TRUE == _MDrv_CA_OTP_SetBlockLock(pSetBlockLock->u32Start, pSetBlockLock->u32End, pSetBlockLock->eLockType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetBlockLock fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetBlockLock\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_IsBlank:
        {
            CA_OTP_ISBLANK* pIsBlank = (CA_OTP_ISBLANK*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_IsBlank(pIsBlank->u32addr))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_IsBlank fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_IsBlank\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_Read:
        {
            CA_OTP_READ* pRead = (CA_OTP_READ*)pu32Args;

            pRead->u32OTPValue = _MDrv_CA_OTP_Read(pRead->u32Addr);

            u32Ret = UTOPIA_STATUS_SUCCESS;

            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_Read\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_Write:
        {
            CA_OTP_WRITE* pWrite = (CA_OTP_WRITE*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_Write(pWrite->u32Addr, pWrite->u32value))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_Write fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_Write\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_Locked:
        {
            CA_OTP_LOCK20* pLock = (CA_OTP_LOCK20*)pu32Args;
            if(TRUE == _MDrv_CA_Locked(pLock->eLock))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_Locked fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_Locked\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_Lock:
        {
            CA_OTP_LOCK20* pLock = (CA_OTP_LOCK20*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_Lock(pLock->eLock))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_Lock fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_Lock\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_SetRSAextID:
        {
            CA_OTP_SETRSAEXTID* pSetRsaextID = (CA_OTP_SETRSAEXTID*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_SetRSAextID(pSetRsaextID->u32value))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetRSAextID fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetRSAextID\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_GetRSAextID:
        {
            CA_OTP_GETRSAEXTID* pGetRsaextID = (CA_OTP_GETRSAEXTID*)pu32Args;
            pGetRsaextID->u32value = _MDrv_CA_OTP_GetRSAextID();

            u32Ret = UTOPIA_STATUS_SUCCESS;

            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetRSAextID\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER:
        {
            CA_OTP_SETHASH1REFVER* pSetHash1RefVer = (CA_OTP_SETHASH1REFVER*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_SetHASH1_REF_VER(pSetHash1RefVer->u32HASH1_REF_VER))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER:
        {
            CA_OTP_GETHASH1REFVER* pGetHash1RefVer = (CA_OTP_GETHASH1REFVER*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_GetHASH1_REF_VER(pGetHash1RefVer->pu32HASH1_REF_VER))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_SetHASH_REF_VER:
        {
            CA_OTP_SETHASHREFVER* pSetHashRefVer = (CA_OTP_SETHASHREFVER*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_SetHASH_REF_VER(pSetHashRefVer->u32HASH0_REF_VER, pSetHashRefVer->u32HASH2_REF_VER))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetHASH_REF_VER fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetHASH_REF_VER\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_GetHASH_REF_VER:
        {
            CA_OTP_GETHASHREFVER* pGetHashRefVer = (CA_OTP_GETHASHREFVER*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_GetHASH_REF_VER(pGetHashRefVer->pu32HASH0_REF_VER, pGetHashRefVer->pu32HASH2_REF_VER))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetHASH_REF_VER fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetHASH_REF_VER\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_SetDebugPortMode:
        {
            CA_OTP_SETDEBUGPORTMODE* pSetDebugPortMode = (CA_OTP_SETDEBUGPORTMODE*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_SetDebugPortMode(pSetDebugPortMode->eDebugPort, pSetDebugPortMode->eMode))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetDebugPortMode fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetDebugPortMode\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_GetDebugPortMode:
        {
            CA_OTP_GETDEBUGPORTMODE* pGetDebugPortMode = (CA_OTP_GETDEBUGPORTMODE*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_GetDebugPortMode(pGetDebugPortMode->eDebugPort, pGetDebugPortMode->eMode))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetDebugPortMode fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetDebugPortMode\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_SetDeviceId:
        {
            CA_OTP_SETDEVICEID* pDeviceId = (CA_OTP_SETDEVICEID*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_SetDeviceId(pDeviceId->pu8Did, pDeviceId->u32Size))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetDeviceId fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_SetDeviceId\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_GetDeviceId:
        {
            CA_OTP_GETDEVICEID* pDeviceId = (CA_OTP_GETDEVICEID*)pu32Args;
            if(TRUE == _MDrv_CA_OTP_GetDeviceId(pDeviceId->pu8Did, pDeviceId->u32Size))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetDeviceId fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_GetDeviceId\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_MaxDeviceIdSize:
        {
            CA_MAXDEVICEIDSIZE* pMaxDeviceIdSize = (CA_MAXDEVICEIDSIZE*)pu32Args;

            pMaxDeviceIdSize->u32MaxDeviceIdSize = _MDrv_CA_MaxDeviceIdSize();
            if(FALSE != pMaxDeviceIdSize->u32MaxDeviceIdSize)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_MaxDeviceIdSize fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_MaxDeviceIdSize\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_EnableSecureCWMode:
        {
            if(FALSE != _MDrv_CA_OTP_EnableSecureCWMode())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_EnableSecureCWMode fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_EnableSecureCWMode\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_IsSecureCWMode:
        {
            if(FALSE != _MDrv_CA_OTP_IsSecureCWMode())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_IsSecureCWMode fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_IsSecureCWMode\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_Random:
        {
            CA_RANDOM* pRandom = (CA_RANDOM*)pu32Args;

            pRandom->u32Random = _MDrv_CA_Random();
            if(FALSE != pRandom->u32Random)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_Random fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_Random\n", __FUNCTION__, __LINE__));
        }
        break;

        case E_MDRV_CMD_CA_OTP_ReadCAVendor:
        {
            CA_RANDOM* pRandom = (CA_RANDOM*)pu32Args;

            pRandom->u32Random = _MDrv_CA_OTP_ReadCAVendor();
            if(FALSE != pRandom->u32Random)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                u32Ret = UTOPIA_STATUS_FAIL;
                MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_ReadCAVendor fail\n", __FUNCTION__, __LINE__));
            }
            MS_DEBUG_MSG(printf("[%s][%d] E_MDRV_CMD_CA_OTP_ReadCAVendor\n", __FUNCTION__, __LINE__));
        }
        break;

        default:
            MS_DEBUG_MSG(printf("[%s][%d] DSCMBIoctl - Unknown commend 0x%x!!!\n", __FUNCTION__, __LINE__));
            return UTOPIA_STATUS_FAIL;
    }

    UtopiaResourceRelease(pstRes);

    return u32Ret;
}

MS_U32 CAClose(void* pInstance)
{

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}
