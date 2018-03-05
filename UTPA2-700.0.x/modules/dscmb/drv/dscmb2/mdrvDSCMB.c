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

#include "drvDSCMB_v2.h"
#include "drvDSCMB_private.h"

#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
void* pModuleDscmb = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DSCMB_MSG(_f, _a...)                             //printf(_f, ##_a)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL* _bFreeSlot;
stDscmbTspMap (*DscmbTspMap)[HAL_DSCMB_KTE_MAX];
stTspFltInfo* _u32PidFlt2Dscmb;
MS_BOOL* _bFreeRIVSlot;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void DSCMBRegisterToUtopia(void)
{
    DSCMB_RESOURCE_PRIVATE * pResPri = NULL;
    void* psResource = NULL;

    // 1. deal with module

    // create a DSCMB module
    // a. size = 8
    // b. Dst: pModuleDscmb
    UtopiaModuleCreate(MODULE_DSCMB, 8, &pModuleDscmb);
    // register a dscmb module
    UtopiaModuleRegister(pModuleDscmb);
    //setup module functions
    UtopiaModuleSetupFunctionPtr(pModuleDscmb, (FUtopiaOpen)DSCMBOpen, (FUtopiaClose)DSCMBClose, (FUtopiaIOctl)DSCMBIoctl);

    // 2. deal with resource
    // create and allocate resource private data(drv Global shared data)
    UtopiaResourceCreate("DSCMB", sizeof(DSCMB_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleDscmb, psResource, E_DSCMB_RESOURCE);
    UtopiaModuleAddResourceEnd(pModuleDscmb, E_DSCMB_RESOURCE);

    if(UtopiaResourceObtain(pModuleDscmb, E_DSCMB_RESOURCE, &psResource) != UTOPIA_STATUS_SUCCESS)
    {
         DSCMB_MSG("UtopiaResourceObtain fail\n");
         return ;
    }
    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    if(pResPri->u32Magic != MAGIC_NUM)
    {
        // not regiser yet
        pResPri->u32Magic = MAGIC_NUM;
        pResPri->bInited_Drv = FALSE;
        pResPri->u32Cavid = DEFAULT_CAVID;
    }

    UtopiaResourceRelease(psResource);
}

MS_U32 DSCMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    DSCMB_MSG("DSCMB open \n");

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

    DSCMB_INSTANT_PRIVATE*    psDscmbInstPri = NULL;

    //create instance
    //1.  Allocate instant private data, prepare argument variable.
    //2.  Dst: ppInstance
    UtopiaInstanceCreate(sizeof(DSCMB_INSTANT_PRIVATE), ppInstance);

    //get instant private data pointer
    // 1. Dst: psDscmbInstPri
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psDscmbInstPri);

    //reset argument variables.
//    memset(&psDscmbInstPri->pInitVector, 0x0, DSCMB_IV_LENGTH);
//    memset(&psDscmbInstPri->pCipherKey, 0x0, DSCMB_KEY_LENGTH);
//    memset(&psDscmbInstPri, 0, sizeof(DSCMB_INSTANT_PRIVATE));

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DSCMBIoctl(void* pInstance, MS_U32 u32Cmd, void* pu32Args)
{
    void* pstRes = NULL;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    DSCMB_RESOURCE_PRIVATE* pstResPri  = NULL;
    DSCMB_INSTANT_PRIVATE*  pstInstPri = NULL;

    if(UtopiaResourceObtain(pModuleDscmb, E_DSCMB_RESOURCE, &pstRes) != UTOPIA_STATUS_SUCCESS)
    {
         DSCMB_MSG("UtopiaResourceObtain fail\n");
         return UTOPIA_STATUS_ERR_RESOURCE;
    }
    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    if((FALSE == pstResPri->bInited_Drv)  &&
        (u32Cmd != E_MDRV_CMD_DSCMB_Init) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_QueryCap) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_SetDBGLevel) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_GetLibVer) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_Exit))
    {
        UtopiaResourceRelease(pstRes);
        return UTOPIA_STATUS_FAIL;
    }

    //set pointer from resource private to local file pointer variable
    //the local file pointer variable will extern to drvDSCMB.c
    _bFreeSlot       = pstResPri->bFreeSlot;
    _bFreeRIVSlot    = pstResPri->bFreeRIVSlot;
    _u32PidFlt2Dscmb = pstResPri->u32PidFlt2Dscmb;
    DscmbTspMap      = pstResPri->DscmbTspMap;

    switch(u32Cmd)
    {
        // General API
        case E_MDRV_CMD_DSCMB_Init:
        {
            if(pstResPri->bInited_Drv == FALSE)
            {
                if(TRUE == _MDrv_DSCMB2_Init())
                {
                     pstResPri->bInited_Drv = TRUE;
                }
                else
                {
                    DSCMB_MSG("E_MDRV_CMD_DSCMB_Init fail\n");
                }
            }
            else
            {
                // SW init only
                if(FALSE == _MDrv_DSCMB2_InitBySWFlag(TRUE))
                {
                    DSCMB_MSG("E_MDRV_CMD_DSCMB_Init fail\n");
                }

            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_Init\n");

            u32Ret = UTOPIA_STATUS_SUCCESS;
        }
        break;

        case E_MDRV_CMD_DSCMB_Exit:
        {
            if(pstResPri->bInited_Drv == TRUE)
            {
                if(TRUE == _MDrv_DSCMB2_Exit())
                {
                    pstResPri->bInited_Drv = FALSE;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            else
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_Exit\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_SetPowerState:
        {
            EN_POWER_MODE * pPowereMode = (EN_POWER_MODE *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_SetPowerState(*pPowereMode))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_SetPowerState,\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltAlloc:
        {
            DSCMB_FLTALLOC* pFltAlloc = (DSCMB_FLTALLOC *)pu32Args;

            pFltAlloc->u32DscmbId = _MDrv_DSCMB2_FltAlloc_Ex(pFltAlloc->u32EngId,
                                                             pFltAlloc->eFltType);
            if(pFltAlloc->u32DscmbId != DRV_DSCMB_FLT_NULL)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltAlloc\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltFree:
        {
            DSCMB_FLTFREE * pFltFree = (DSCMB_FLTFREE *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltFree(pFltFree->u32EngId, pFltFree->u32DscmbId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltFree\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltConnectFltId:
        {
            DSCMB_CONNECTFLT * pConnectFlt = (DSCMB_CONNECTFLT *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltConnectFltId(pConnectFlt->u32EngId,
                                                    pConnectFlt->u32DscmbId,
                                                    pConnectFlt->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltConnectFltId\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_FltDisconnectFltId:
        {
            DSCMB_DISCONNECTFLT * pDisconnectFlt = (DSCMB_DISCONNECTFLT *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltDisconnectFltId(pDisconnectFlt->u32EngId,
                                                       pDisconnectFlt->u32DscmbId,
                                                       pDisconnectFlt->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltDisconnectFltId\n");
        }
        break;

#if !(defined(MSOS_TYPE_OPTEE) || defined(MSOS_TYPE_NUTTX))
        case E_MDRV_CMD_DSCMB_FltConnectPid:
        {
            DSCMB_CONNECTPID * pstConnectPid = (DSCMB_CONNECTPID *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltConnectPid(
                        pstConnectPid->u32EngId,
                        pstConnectPid->u32DscmbId,
                        pstConnectPid->u32Pid))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltConnectPid\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltDisconnectPid:
        {
            DSCMB_DISCONNECTPID * pstDisconnectPid = (DSCMB_DISCONNECTPID *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltDisconnectPid_Ex(pstDisconnectPid->u32EngId,
                    pstDisconnectPid->u32DscmbId,
                    pstDisconnectPid->u32Pid))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltDisconnectPid\n");
        }
        break;
#endif
        case E_MDRV_CMD_DSCMB_FltDscmb:
        {
            DSCMB_FLTDSCMB * pstFltDscmb = (DSCMB_FLTDSCMB *)pu32Args;

            if(TRUE == _MDrv_DSCMB2_FltDscmb(pstFltDscmb->u32EngId,
                                             pstFltDscmb->u32DscmbId,
                                             pstFltDscmb->bDscmb))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltDscmb\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltTypeSet:
        {
            DSCMB_TYPESET *pstTypeSet = (DSCMB_TYPESET *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltTypeSet(pstTypeSet->u32EngId,
                                               pstTypeSet->u32DscmbId,
                                               pstTypeSet->eType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltTypeSet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltKeySet:
        {
            DSCMB_FLTKEYSET * pstFltKeySet = (DSCMB_FLTKEYSET *)pu32Args;

            if(TRUE == _MDrv_DSCMB2_FltKeySet(pstFltKeySet->u32EngId,
                                              pstFltKeySet->u32DscmbId,
                                              pstFltKeySet->eKeyType,
                                              pstFltKeySet->pu8Key))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltKeySet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltKeyReset:
        {
            DSCMB_FLTKEYRESET * pstFltKeyReset = (DSCMB_FLTKEYRESET *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltKeyReset(pstFltKeyReset->u32EngId,
                                                pstFltKeyReset->u32DscmbId,
                                                pstFltKeyReset->eKeyType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltKeyReset\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltIVSet:
        {
            UtopiaInstanceGetPrivate(pInstance, (void**)&pstInstPri);
            DSCMB_FLTIVSET * pstFltIVSet = (DSCMB_FLTIVSET *)pu32Args;

            if(TRUE == _MDrv_DSCMB2_FltIVSet(pstFltIVSet->u32EngId,
                                             pstFltIVSet->u32DscmbId,
                                             pstFltIVSet->eKeyType,
                                             pstFltIVSet->pu8IV))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltIVSet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_HDCP2_SetRiv:
        {
            DSCMB_HDCP2_SETRIV * pstSetRiv = (DSCMB_HDCP2_SETRIV *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_HDCP2_SetRIV(pstSetRiv->u32EngId,
                                                 pstSetRiv->pu8Riv))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_HDCP2_SetRiv\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_HDCP2_SetRiv_Ex:
        {
            DSCMB_HDCP2_SETRIV_EX * pstSetRiv = (DSCMB_HDCP2_SETRIV_EX *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_HDCP2_SetRIV_Ex(pstSetRiv->u32EngId,
                                                    pstSetRiv->u32DscmbId,
                                                    pstSetRiv->pu8Riv))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_HDCP2_SetRiv_Ex\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_Multi2_SetRound:
        {
            DSCMB_MULTI2_SETROUND * pstSetRnd = (DSCMB_MULTI2_SETROUND *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_Multi2_SetRound(pstSetRnd->u32EngId,
                                                    pstSetRnd->u32Round))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
        }
        break;

        case E_MDRV_CMD_DSCMB_Multi2_SetSysKey:
        {
            DSCMB_MULTI2_SETSYSKEY * psSetSysKey = (DSCMB_MULTI2_SETSYSKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_Multi2_SetSystemKey(psSetSysKey->u32EngId,
                    (MS_U8*)psSetSysKey->pu8SysKey))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
        }
        break;

        case E_MDRV_CMD_DSCMB_ES_SetStaticKey:
        {
            DSCMB_ES_SETSTATICKEY * psSetStaticKey = (DSCMB_ES_SETSTATICKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_ES_SetStaticKey(psSetStaticKey->u32EngId,
                    (MS_U8*)psSetStaticKey->pu8StaticKey))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
        }
        break;

        case E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus:
        {
            DSCMB_DSCMBSTATUS * pDscmbStatus = (DSCMB_DSCMBSTATUS *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_PidFlt_ScmbStatus(pDscmbStatus->u32EngId,
                    pDscmbStatus->u32DmxFltId,
                    pDscmbStatus->pScmbLevel))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_PVR_RecCtrl:
        {
            DSCMB_PVR_RECCTRL * pPvrRecCtrl = (DSCMB_PVR_RECCTRL *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_PVR_RecCtrl(pPvrRecCtrl->u32EngId,
                                                      pPvrRecCtrl->bEnable))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_PVR_RecCtrl\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_SetDefaultCAVid:
        {
            DSCMB_DEFAULT_CAVID * pu32DefaultVid = (DSCMB_DEFAULT_CAVID *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_SetDefaultCAVid(pu32DefaultVid->u32EngId, pu32DefaultVid->u32CAVid))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_SetDefaultCAVid\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetAlgo:
        {
            DSCMB_ENGALGO * pAlgo = (DSCMB_ENGALGO *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetAlgo(pAlgo->u32EngId, pAlgo->u32DscmbId, pAlgo->eEngType, pAlgo->stConfig))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetAlgo\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetKey:
        {
            DSCMB_ENGKEY * pKey = (DSCMB_ENGKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetKey(pKey->u32EngId, pKey->u32DscmbId, pKey->eEngType, pKey->eKeyType, pKey->pu8Key))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetKey\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetKeyFSCB:
        {
            DSCMB_ENGKEYFSCB * pKeyFSCB = (DSCMB_ENGKEYFSCB *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetKeyFSCB(pKeyFSCB->u32EngId, pKeyFSCB->u32DscmbId, pKeyFSCB->eEngType, pKeyFSCB->eKeyType, pKeyFSCB->eForceSCB))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetKeyFSCB\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngResetKey:
        {
            DSCMB_ENGRESETKEY * pKey = (DSCMB_ENGRESETKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngResetKey(pKey->u32EngId, pKey->u32DscmbId, pKey->eEngType, pKey->eKeyType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngResetKey\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetIV:
        {
            DSCMB_ENGIV * pIV = (DSCMB_ENGIV *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetIV(pIV->u32EngId, pIV->u32DscmbId, pIV->eKeyType, pIV->pu8IV))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetIV\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetSwitch:
        {
            DSCMB_ENGSWITCH * pSwitch = (DSCMB_ENGSWITCH *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetSwitch(pSwitch->u32EngId, pSwitch->u32DscmbId, pSwitch->eUppSwitch, pSwitch->eLowSwitch))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetSwitch\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetFSCB:
        {
            DSCMB_ENGFSCB * pFSCB = (DSCMB_ENGFSCB *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetFSCB(pFSCB->u32EngId, pFSCB->u32DscmbId, pFSCB->eForceSCB))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetFSCB\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_KLadder_AtomicExec:
        {
            DSCMB_KL_ATOMICEXEC * pKLArgs = (DSCMB_KL_ATOMICEXEC *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_KLadder_AtomicExec(pKLArgs->KLCfg,
                                                       pKLArgs->ACPU_Out,
                                                       pKLArgs->u32Status))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_KLadder_AtomicExec\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_KLadder_ETSI:
        {
            DSCMB_KL_ETSI * pKLArgs = (DSCMB_KL_ETSI *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_KLadder_ETSI(pKLArgs->KLCfg,
                                                 pKLArgs->ACPU_Out,
                                                 pKLArgs->pu8Nonce,
                                                 pKLArgs->pu8Response,
                                                 pKLArgs->u32Status))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_KLadder_ETSI\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_QueryCap:
        {
            DSCMB_QUERYCAP* pGetCap = (DSCMB_QUERYCAP*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetCap(pGetCap->u32EngId,
                                           pGetCap->eQueryType,
                                           pGetCap->pInput,
                                           pGetCap->pOutput))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_GetCap\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_SetDBGLevel:
        {
            MS_U32* pDBGArgs = (MS_U32*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_SetDBGLevel(*pDBGArgs))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_SetDBGLevel\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_GetLibVer:
        {
            const MSIF_Version** pLibVerArgs = (const MSIF_Version**)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetLibVer(pLibVerArgs))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_GetLibVer\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_GetConnectStatus:
        {
            DSCMB_CONNECTSTATUS* pStatusArgs = (DSCMB_CONNECTSTATUS*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetConnectStatus (pStatusArgs->u32EngId,
                                                      pStatusArgs->u32DscmbId,
                                                      pStatusArgs->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_GetConnectStatus\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_CAPVR_FlowSet:
        {
            DSCMB_CAPVR_FLOWSET* pStatusArgs = (DSCMB_CAPVR_FLOWSET*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_CAPVR_FlowSet (pStatusArgs->u32EngId,
                                                      pStatusArgs->eCaMode,
                                                      pStatusArgs->ePvrSrcTsif))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_CAPVR_FlowSet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_DualPath_Enable:
        {
            DSCMB_DUALPATH* pStatusArgs = (DSCMB_DUALPATH*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_DualPath_Enable (pStatusArgs->u32EngId,
                                                      pStatusArgs->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_DualPath_Enable\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_DualPath_Disable:
        {
            DSCMB_DUALPATH* pStatusArgs = (DSCMB_DUALPATH*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_DualPath_Disable (pStatusArgs->u32EngId,
                                                      pStatusArgs->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_DualPath_Disable\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetIV_Ex:
        {
            DSCMB_ENGIV_EX* pStatusArgs = (DSCMB_ENGIV_EX*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetIV_Ex (pStatusArgs->u32EngId,
                                                 pStatusArgs->u32DscmbId,
                                                 pStatusArgs->eEngType,
                                                 pStatusArgs->eKeyType,
                                                 pStatusArgs->pu8IV))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetIV_Ex\n");
        }
        break;

        default:
            DSCMB_MSG("DSCMBIoctl - Unknown commend 0x%x!!!\n", (int)u32Cmd);
            return UTOPIA_STATUS_FAIL;
    }


    UtopiaResourceRelease(pstRes);

    return u32Ret;
}

MS_U32 DSCMBClose(void* pInstance)
{

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

