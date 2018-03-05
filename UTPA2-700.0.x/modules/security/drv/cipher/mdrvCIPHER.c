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
/// file   mdrvCIPHER.c
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

#include "drvCIPHER_v2.h"
#include "drvCIPHER_private.h"

#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
void* pModuleCipher = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define CIPHER_MSG(_f, _a...)                             //printf(_f, ##_a)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U16 _u16ThreadID = 1;
MS_U32 _u32CallbackID = 1;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void CIPHERRegisterToUtopia(void)
{
    CIPHER_RESOURCE_PRIVATE * pResPri = NULL;
    void* psResource = NULL;

    // 1. deal with module

    // create a DSCMB module
    // a. size = 8
    // b. Dst: pModuleDscmb
    UtopiaModuleCreate(MODULE_CIPHER, 8, &pModuleCipher);
    // register a dscmb module
    UtopiaModuleRegister(pModuleCipher);
    //setup module functions
    UtopiaModuleSetupFunctionPtr(pModuleCipher, (FUtopiaOpen)CIPHEROpen, (FUtopiaClose)CIPHERClose, (FUtopiaIOctl)CIPHERIoctl);

    // 2. deal with resource
    // create and allocate resource private data(drv Global shared data)
    UtopiaResourceCreate("CIPHER", sizeof(CIPHER_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleCipher, psResource, E_CIPHER_RESOURCE);
    UtopiaModuleAddResourceEnd(pModuleCipher, E_CIPHER_RESOURCE);

    if(UtopiaResourceObtain(pModuleCipher, E_CIPHER_RESOURCE, &psResource) != UTOPIA_STATUS_SUCCESS)
    {
         CIPHER_MSG("UtopiaResourceObtain fail\n");
         return ;
    }
    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    UtopiaResourceRelease(psResource);
}

MS_U32 CIPHEROpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    CIPHER_MSG("CIPHER open \n");

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

    CIPHER_INSTANT_PRIVATE*    psCipherInstPri = NULL;

    //create instance
    //1.  Allocate instant private data, prepare argument variable.
    //2.  Dst: ppInstance
    UtopiaInstanceCreate(sizeof(CIPHER_INSTANT_PRIVATE), ppInstance);

    //get instant private data pointer
    // 1. Dst: psCipherInstPri
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psCipherInstPri);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 CIPHERIoctl(void* pInstance, MS_U32 u32Cmd, void* pu32Args)
{
    void* pstRes = NULL;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    CIPHER_RESOURCE_PRIVATE* pstResPri  = NULL;
    //CIPHER_INSTANT_PRIVATE*  pstInstPri = NULL;

    if(UtopiaResourceObtain(pModuleCipher, E_CIPHER_RESOURCE, &pstRes) != UTOPIA_STATUS_SUCCESS)
    {
         CIPHER_MSG("UtopiaResourceObtain fail\n");
         return UTOPIA_STATUS_ERR_RESOURCE;
    }
    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    if((FALSE == pstResPri->bInited_Drv)  &&
        (u32Cmd != E_MDRV_CMD_CIPHER_Init) /*&&          // which command don't need do it after init?
        (u32Cmd != E_MDRV_CMD_DSCMB_QueryCap) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_SetDBGLevel) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_GetLibVer) &&
        (u32Cmd != E_MDRV_CMD_DSCMB_Exit)*/)
    {
        UtopiaResourceRelease(pstRes);
        return UTOPIA_STATUS_FAIL;
    }

    //set value from resource private to local file variable
    //the local file pointer variable will extern to drvCIPHER.c
    if(_u16ThreadID < pstResPri->u16ThreadID)
        _u16ThreadID = pstResPri->u16ThreadID;
    else
        pstResPri->u16ThreadID = _u16ThreadID;

    if(_u32CallbackID < pstResPri->u32CallbackID)
        _u32CallbackID = pstResPri->u32CallbackID;
    else
        pstResPri->u32CallbackID = _u32CallbackID;

    switch(u32Cmd)
    {
        // General API
        case E_MDRV_CMD_CIPHER_Init:
        {
            if(pstResPri->bInited_Drv == FALSE)
            {
                if(DRV_CIPHER_OK == _MDrv_CIPHER_Init())
                {
                     pstResPri->bInited_Drv = TRUE;
                     u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    CIPHER_MSG("E_MDRV_CMD_CIPHER_Init fail\n");
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            else
            {
                if(DRV_CIPHER_OK == _MDrv_CIPHER_InitBySWFlag(TRUE))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    CIPHER_MSG("E_MDRV_CMD_CIPHER_Init fail\n");
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_Init\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_Reset:
        {
            if(DRV_CIPHER_OK == _MDrv_CIPHER_Reset())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_Reset fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_Reset\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_ResetKey:
        {
            CIPHER_RESETKEY* pstRestKey = (CIPHER_RESETKEY*)pu32Args;

            pstRestKey->ret = _MDrv_CIPHER_ResetKey(pstRestKey->u32CAVid, pstRestKey->u32KeyIdx);
            if(DRV_CIPHER_OK == pstRestKey->ret)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_ResetKey fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_ResetKey\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_DbgLevel:
        {
            CIPHER_DBGLEVEL* pstDbgLevel = (CIPHER_DBGLEVEL*)pu32Args;

            if(DRV_CIPHER_OK == _MDrv_CIPHER_SetDbgLevel(pstDbgLevel->eDBGMsgLevel))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_DbgLevel fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_DbgLevel\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_DMACalc:
        {
            CIPHER_DMACALC* pstDMACalc = (CIPHER_DMACALC*)pu32Args;

            pstDMACalc->ret = _MDrv_CIPHER_DMACalc(pstDMACalc->stCfg, pstDMACalc->pu32CmdId);
            if(DRV_CIPHER_OK == pstDMACalc->ret)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_DMACalc fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_DMACalc\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_HASH:
        {
            CIPHER_HASHCALC* pstHASHCalc = (CIPHER_HASHCALC*)pu32Args;

            pstHASHCalc->ret = _MDrv_CIPHER_HASH(pstHASHCalc->stCfg, pstHASHCalc->pu32CmdId);
            if(DRV_CIPHER_OK == pstHASHCalc->ret)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_HASH fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_HASH\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_IsDMADone:
        {
            CIPHER_ISDMADONE* pstIsDMADone = (CIPHER_ISDMADONE*)pu32Args;

            if(TRUE == _MDrv_CIPHER_IsDMADone(pstIsDMADone->u32CmdId, pstIsDMADone->pu32Exception))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_IsDMADone fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_IsDMADone\n");
        }
        break;

        case E_MDRV_CMD_CIPHER_IsHASHDone:
        {
            CIPHER_ISHASHDONE* pstIsHASHDone = (CIPHER_ISHASHDONE*)pu32Args;

            if(TRUE == _MDrv_CIPHER_IsHASHDone(pstIsHASHDone->u32CmdId, pstIsHASHDone->pu32Exception))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            else
            {
                CIPHER_MSG("E_MDRV_CMD_CIPHER_IsHASHDone fail\n");
                u32Ret = UTOPIA_STATUS_FAIL;
            }

            CIPHER_MSG("E_MDRV_CMD_CIPHER_IsHASHDone\n");
        }
        break;

        default:
            CIPHER_MSG("CIPHERIoctl - Unknown commend 0x%x!!!\n", (int)u32Cmd);
            return UTOPIA_STATUS_FAIL;
    }

    pstResPri->u16ThreadID = _u16ThreadID;
    pstResPri->u32CallbackID = _u32CallbackID;

    UtopiaResourceRelease(pstRes);

    return u32Ret;
}

MS_U32 CIPHERClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}
