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
// ********************************************************
//

///////////////////////////////////////////////////////////////////////////////
///
/// file    pcmcia.c
/// @brief  PCMCIA Driver Utopia2.0 Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
/*                       Header Files                                        */
/*****************************************************************************/
// Common Definition
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include <stdio.h>
#include <string.h>
#include "pcmcia.h"
#include "pcmcia_private.h"

#include "drvPCMCIA.h"
#include "regPCMCIA.h"
#include "halPCMCIA.h"
#include "MsOS.h"
/*****************************************************************************/
/*                       Define                                              */
/*****************************************************************************/
#define CI_DBG  0
/*****************************************************************************/
/*                       Global Variables                                    */
/*****************************************************************************/
void* pModulePcm = NULL;
void* psResource = NULL;
PCMCIA_RESOURCE_PRIVATE* pPcmEnv = NULL;

#if CI_DBG
static const char *aCmdStr[E_MDRV_CMD_PCM_MAX] = {"E_MDRV_CMD_PCM_Init",
                                                  "E_MDRV_CMD_PCM_Detect",
                                                  "E_MDRV_CMD_PCM_Enable_Interrupt",
                                                  "E_MDRV_CMD_PCM_Exit",
                                                  "E_MDRV_CMD_PCM_Get_CD_Interval",
                                                  "E_MDRV_CMD_PCM_Get_InterruptStatus",
                                                  "E_MDRV_CMD_PCM_GetLibVer",
                                                  "E_MDRV_CMD_PCM_InstallIsrCallback",
                                                  "E_MDRV_CMD_PCM_IsDataAvailable",
                                                  "E_MDRV_CMD_PCM_IsModuleStillPlugged",
                                                  "E_MDRV_CMD_PCM_NegotiateBufferSize",
                                                  "E_MDRV_CMD_PCM_ParseAttribMem",
                                                  "E_MDRV_CMD_PCM_Polling",
                                                  "E_MDRV_CMD_PCM_ReadAttribMem", 
                                                  "E_MDRV_CMD_PCM_ReadData",
                                                  "E_MDRV_CMD_PCM_ReadIOMem",
                                                  "E_MDRV_CMD_PCM_ResetHW",
                                                  "E_MDRV_CMD_PCM_ResetInterface",
                                                  "E_MDRV_CMD_PCM_Set_Detect_Enable",
                                                  "E_MDRV_CMD_PCM_Set_Detect_Trigger",
                                                  "E_MDRV_CMD_PCM_Set_HW_ResetDuration",
                                                  "E_MDRV_CMD_PCM_Set_InterruptStatus",
                                                  "E_MDRV_CMD_PCM_SetCommandBit",
                                                  "E_MDRV_CMD_PCM_SwitchToIOmode",
                                                  "E_MDRV_CMD_PCM_WaitForStatusBit",
                                                  "E_MDRV_CMD_PCM_WriteAttribMem",
                                                  "E_MDRV_CMD_PCM_WriteBufferSize",
                                                  "E_MDRV_CMD_PCM_WriteData",
                                                  "E_MDRV_CMD_PCM_WriteIOMem",
                                                  "E_MDRV_CMD_PCM_WriteIOMemLong",
                                                  "E_MDRV_CMD_PCM_SetPowerState"};
#endif
/*****************************************************************************/
/*                       Macro                                               */
/*****************************************************************************/
#define PCM_ASSERT_RETURN(x, y) if(!(x)) {ULOGE("PCMCIA", "Assert Fail : %s, %d\n", __FUNCTION__, __LINE__); return y;}

#if CI_DBG
    #define PCM_PRINT(x, a...) ULOGD("PCMCIA", x, ##a)
#else    
    #define PCM_PRINT(x, a...)
#endif    

/*****************************************************************************/
/*                       Local Variables                                     */
/*****************************************************************************/

/*****************************************************************************/
/*                       Local Functions                                     */
/*****************************************************************************/

void CIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    PCMCIA_RESOURCE_PRIVATE * pResPri = NULL;
    
    // 1. deal with module
    if(UtopiaModuleCreate(MODULE_CI, 8, &pModulePcm) == UTOPIA_STATUS_FAIL)
    {
        ULOGE("PCMCIA", "[%s] create module CI failed\n", __FUNCTION__);
        return;
    }
    
    UtopiaModuleRegister(pModulePcm);
    UtopiaModuleSetupFunctionPtr(pModulePcm, (FUtopiaOpen)PCMCIAOpen, (FUtopiaClose)PCMCIAClose, (FUtopiaIOctl)PCMCIAIoctl);

    // 2. deal with resource
    psResource = NULL;

    if(UtopiaResourceCreate("PCMCIA", sizeof(PCMCIA_RESOURCE_PRIVATE), &psResource) == UTOPIA_STATUS_FAIL)
    {
        ULOGE("PCMCIA", "[%s] create resource CI failed\n", __FUNCTION__);
    }
    UtopiaResourceRegister(pModulePcm, psResource, E_PCMCIA_RESOURCE);
    
    UtopiaModuleAddResourceEnd(pModulePcm, E_PCMCIA_RESOURCE);
    
    if(UtopiaResourceObtain(pModulePcm, E_PCMCIA_RESOURCE, &psResource) == UTOPIA_STATUS_FAIL)
    {
        ULOGE("PCMCIA", "[%s][%d]UtopiaResourceObtain PCMCIA fail\n", __FUNCTION__, __LINE__);
        return;
    }
    
    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);
    pPcmEnv = pResPri;
    if(pResPri->u32Magic != MAGIC_NUM)
    {
        // not regiser yet
        pResPri->u32Magic = MAGIC_NUM;
        pResPri->bInited_Drv = FALSE;   
        
        pResPri->u8PCMCIACurModule = PCMCIA_DEFAULT_MODULE;
    }    
    
    UtopiaResourceRelease(psResource);
}


MS_U32 PCMCIAOpen(void** ppInstantTmp, const void* const pAttribute)
{
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,ULOGD("PCMCIA", "enter %s %d\n",__FUNCTION__,__LINE__));
	void** ppInstant = ppInstantTmp;
	UtopiaInstanceCreate(sizeof(PCMCIA_INSTANT_PRIVATE), ppInstant);
    
    //PCMCIA_INSTANT_PRIVATE* psPcmInstPri = (PCMCIA_INSTANT_PRIVATE*)UtopiaInstantGetPrivate(*pInstant);
    
    
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,ULOGD("PCMCIA", "leave %s %d\n",__FUNCTION__,__LINE__));
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 PCMCIAIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* const pu32Args)
{
    PCMCIA_MODULE eMod = PCMCIA_DEFAULT_MODULE;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    //psResource = NULL;
    PCMCIA_RESOURCE_PRIVATE * pResPri = NULL;
    
    PCM_ASSERT_RETURN(u32Cmd < E_MDRV_CMD_PCM_MAX, UTOPIA_STATUS_FAIL);
#if CI_DBG    
    PCM_PRINT("%s cmd : %s\n", __FUNCTION__, aCmdStr[u32Cmd]);
#endif    
   
    //UtopiaResourceObtain(pModulePcm, E_PCMCIA_RESOURCE, &psResource);
    PCM_ASSERT_RETURN((psResource != NULL), UTOPIA_STATUS_FAIL);

    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);
        
    switch(u32Cmd)
    {
        case E_MDRV_CMD_PCM_Init:
            {
                if(pu32Args == NULL)
                    break;
                    
                MS_BOOL bActiveLevel = *(MS_BOOL*)pu32Args;
                _MDrv_PCMCIA_InitSW(bActiveLevel);
                pResPri->bCD_Reverse = bActiveLevel;
                if(pResPri->bInited_Drv == FALSE)
                {
                    _MDrv_PCMCIA_InitHW(FALSE); 
                    pResPri->bInited_Drv = TRUE;
                }
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;

        case E_MDRV_CMD_PCM_Detect:
            {
                if(pu32Args == NULL)
                    break;

                PCM_DETECT* pDetect = (PCM_DETECT*)pu32Args;
                pDetect->bDetected = (MS_BOOL)_MDrv_PCMCIA_DetectV2(pDetect->eModule);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
            
        case E_MDRV_CMD_PCM_Enable_Interrupt:
            {
                if(pu32Args == NULL)
                    break;

                PCM_ENABLE_INT* pstEnableInt = (PCM_ENABLE_INT*)pu32Args;
                _MDrv_PCMCIA_Enable_InterruptV2(pstEnableInt->eModule, 
                                                pstEnableInt->bEnable);
                u32Ret = UTOPIA_STATUS_SUCCESS;             
            }
            break;
        case E_MDRV_CMD_PCM_Exit:
            _MDrv_PCMCIA_Exit(FALSE);
            pResPri->bInited_Drv = FALSE;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case E_MDRV_CMD_PCM_Get_CD_Interval:
            {
                if(pu32Args == NULL)
                    break;

                PCM_GET_CD_INTERVAL* pstCDInterval = (PCM_GET_CD_INTERVAL*)pu32Args;
                pstCDInterval->u32Interval = _MDrv_PCMCIA_Get_CD_IntervalV2(pstCDInterval->eModule);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_Get_InterruptStatus:
            {
                if(pu32Args == NULL)
                    break;

                PCM_GET_INT_STATUS* pstIntStatus = (PCM_GET_INT_STATUS*)pu32Args;
                pstIntStatus->bIntStatus = _MDrv_PCMCIA_Get_InterruptStatusV2(pstIntStatus->eModule);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_GetLibVer:
            {
                if(pu32Args == NULL)
                    break;

                const MSIF_Version** ppVersion = (const MSIF_Version**)pu32Args;
                if(_MDrv_PCMCIA_GetLibVer(ppVersion) == TRUE)
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
                else
                {
                    u32Ret = UTOPIA_STATUS_FAIL;
                }
            }
            break;
            
        case E_MDRV_CMD_PCM_InstallIsrCallback:
            {
                if(pu32Args == NULL)
                    break;

                PCM_INSTCB *pstInstCB = (PCM_INSTCB*)pu32Args;
                _MDrv_PCMCIA_InstarllIsrCallbackV2(pstInstCB->eModule, 
                                                   pstInstCB->pfcb);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
            
        case E_MDRV_CMD_PCM_IsDataAvailable:
            {
                if(pu32Args == NULL)
                    break;

                PCM_DATA_AVAILABLE* pstAvailable = (PCM_DATA_AVAILABLE*)pu32Args;
                pstAvailable->bDA = _MDrv_PCMCIA_IsDataAvailableV2(pstAvailable->eModule);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_IsModuleStillPlugged:
            {
                if(pu32Args == NULL)
                    break;

                PCM_MODULE_PLUGIN* pstPlugIn = (PCM_MODULE_PLUGIN*)pu32Args;
                pstPlugIn->bPlugIn = (MS_U32)_MDrv_PCMCIA_IsModuleStillPluggedV2(pstPlugIn->eModule);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_NegotiateBufferSize:
            {
                if(pu32Args == NULL)
                    break;

                PCM_NETOTIAGE_BUFSIZE* pstNegoBufSize = (PCM_NETOTIAGE_BUFSIZE*)pu32Args;
                pstNegoBufSize->u16BufSize = _MDrv_PCMCIA_NegotiateBufferSizeV2(pstNegoBufSize->eModule,
                                                                                pstNegoBufSize->pInfo);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_ParseAttribMem:
            {
                if(pu32Args == NULL)
                    break;

                PCM_PARSE_ATTRIBUTE* pstParseAttrib = (PCM_PARSE_ATTRIBUTE*)pu32Args;
                _MDrv_PCMCIA_ParseAttribMem(pstParseAttrib->pu8AttribMem, 
                                           pstParseAttrib->u16DwLen,
                                           pstParseAttrib->pInfo);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_Polling:
            {
                if(pu32Args == NULL)
                    break;

                PCM_POLL* pstPoll = (PCM_POLL*)pu32Args;
                pstPoll->bChanged = _MDrv_PCMCIA_PollingV2(pstPoll->eModule);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        
        case E_MDRV_CMD_PCM_ReadAttribMem:
            {
                if(pu32Args == NULL)
                    break;

                PCM_READ_ATTRIB_MEM* pstRead = (PCM_READ_ATTRIB_MEM*)pu32Args;
                _MDrv_PCMCIA_ReadAttribMemV2(pstRead->eModule, pstRead->u16Addr, &pstRead->u8Data);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
            
        case E_MDRV_CMD_PCM_ReadData:
            {
                if(pu32Args == NULL)
                    break;

                PCM_READ_DATA* pstRead = (PCM_READ_DATA*)pu32Args;
                pstRead->u16SizeRead = (MS_U32)_MDrv_PCMCIA_ReadDataV2(pstRead->eModule, 
                                                                       pstRead->pu8ReadBuffer,
                                                                       pstRead->u16ReadBufferSize);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_ReadIOMem:
            {
                if(pu32Args == NULL)
                    break;

                PCM_READ_IO_MEM* pstReadIO = (PCM_READ_IO_MEM*)pu32Args;
                pstReadIO->u8Data = (MS_U8)_MDrv_PCMCIA_ReadIOMemV2(pstReadIO->eModule, 
                                                                     pstReadIO->u16Addr);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_ResetHW:
            if(pu32Args == NULL)
                break;

            eMod = *(PCMCIA_MODULE*)pu32Args;
            _MDrv_PCMCIA_ResetHW_V2(eMod);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
            
        case E_MDRV_CMD_PCM_ResetInterface:
            {
                if(pu32Args == NULL)
                    break;

                PCM_RESET_INTERFACE* pstReset = (PCM_RESET_INTERFACE*)pu32Args;
                
                if(TRUE == _MDrv_PCMCIA_ResetInterfaceV2(pstReset->eModule))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;
        case E_MDRV_CMD_PCM_Set_Detect_Enable:
            {
                if(pu32Args == NULL)
                    break;

                MS_BOOL bEnable = *(MS_BOOL*)pu32Args;
                _MDrv_PCMCIA_Set_Detect_Enable(bEnable);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_Set_Detect_Trigger:
            {
                if(pu32Args == NULL)
                    break;

                MS_BOOL bHighActive = *(MS_BOOL*)pu32Args;
                _MDrv_PCMCIA_Set_Detect_Trigger(bHighActive);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_Set_HW_ResetDuration:
            {
                if(pu32Args == NULL)
                    break;

                MS_U8 u8Duration = *(MS_U8*)pu32Args;
                _MDrv_PCMCIA_Set_HW_ResetDuration(u8Duration);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_Set_InterruptStatus:
            {
                if(pu32Args == NULL)
                    break;

                PCM_SET_INTR_STATUS* pstStatus = (PCM_SET_INTR_STATUS*)pu32Args;
                _MDrv_PCMCIA_Set_InterruptStatusV2(pstStatus->eModule, 
                                                  pstStatus->bStatus);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_SetCommandBit:
            {
                if(pu32Args == NULL)
                    break;

                PCM_SETCOMMANDBIT* pstSetCmdBit = (PCM_SETCOMMANDBIT*)pu32Args;
                _MDrv_PCMCIA_SetCommandBitV2(pstSetCmdBit->eModule, 
                                            pstSetCmdBit->u8CmdBit,
                                            pstSetCmdBit->bValue);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_SwitchToIOmode:
            {
                if(pu32Args == NULL)
                    break;

                PCM_SWITCH_TO_IOMODE* pstSwitchIOMode = (PCM_SWITCH_TO_IOMODE*)pu32Args;
                if(TRUE == _MDrv_PCMCIA_SwitchToIOmodeV2(pstSwitchIOMode->eModule, 
                                                         pstSwitchIOMode->pInfo))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;
        case E_MDRV_CMD_PCM_WaitForStatusBit:
            {
                if(pu32Args == NULL)
                    break;

                PCM_WAIT_STATUS* pstWaitStatus = (PCM_WAIT_STATUS*)pu32Args;
                if(TRUE == _MDrv_PCMCIA_WaitForStatusBitV2(pstWaitStatus->eModule, 
                                                           pstWaitStatus->u8StatusBit))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;                                  
                }
            }
            break;
        case E_MDRV_CMD_PCM_WriteAttribMem:
            {
                if(pu32Args == NULL)
                    break;

                PCM_WRITE_ATTRIB_MEM* pstWrite = (PCM_WRITE_ATTRIB_MEM*)pu32Args;
                _MDrv_PCMCIA_WriteAttribMemV2(pstWrite->eModule, 
                                             pstWrite->u16Addr, 
                                             pstWrite->u8Value);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_WriteBufferSize:
            {
                if(pu32Args == NULL)
                    break;

                PCM_WRITE_BUF_SIZE* pstBufSize = (PCM_WRITE_BUF_SIZE*)pu32Args;
                _MDrv_PCMCIA_WriteBufferSizeV2(pstBufSize->eModule, 
                                              pstBufSize->u16BufferSize);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_WriteData:
            {
                if(pu32Args == NULL)
                    break;

                PCM_WRITE_DATA* pstWriteData = (PCM_WRITE_DATA*)pu32Args;
                if(TRUE == _MDrv_PCMCIA_WriteDataV2(pstWriteData->eModule, 
                                                    pstWriteData->pu8WriteBuffer,
                                                    pstWriteData->u16DataLen))
                {
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }
            }
            break;
        case E_MDRV_CMD_PCM_WriteIOMem:
            {
                if(pu32Args == NULL)
                    break;

                PCM_WRITE_IO_MEM* pstWriteIO = (PCM_WRITE_IO_MEM*)pu32Args;
                _MDrv_PCMCIA_WriteIOMemV2(pstWriteIO->eModule, 
                                         pstWriteIO->u16Addr, 
                                         pstWriteIO->u8Value);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_WriteIOMemLong:
            {
                if(pu32Args == NULL)
                    break;

                PCM_WRITE_IO_MEM_LONG* pstWriteIOLong = (PCM_WRITE_IO_MEM_LONG*)pu32Args;
                _MDrv_PCMCIA_WriteIOMemLongV2(pstWriteIOLong->eModule, 
                                             pstWriteIOLong->u16Addr, 
                                             pstWriteIOLong->u8Value,
                                             pstWriteIOLong->u16DataLen, 
                                             pstWriteIOLong->pu8WriteBuffer);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_SetPowerState:
            {
                if(pu32Args == NULL)
                    break;

                _MDrv_PCMCIA_SetPowerState(*(EN_POWER_MODE*)pu32Args);
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            break;
        case E_MDRV_CMD_PCM_GetConfig:
            {
                if(pu32Args == NULL)
                    break;

                if(pResPri->bInited_Drv == TRUE)
                {
                    *(MS_BOOL*)pu32Args = pResPri->bCD_Reverse;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                }       
            }
            break;
        default :
            ULOGE("PCMCIA", "[PCMCIA] unknown IOCTL 0x%x\n", (int)u32Cmd);
            break;
    }
        
    //UtopiaResourceRelease(psResource);
    return u32Ret;
}

MS_U32 PCMCIAClose(void* pInstant)
{    
    if(pInstant == NULL)
    {
        return UTOPIA_STATUS_FAIL;
    }
    else
    {
        UtopiaInstanceDelete(pInstant);    
        return UTOPIA_STATUS_SUCCESS;
    }
}

