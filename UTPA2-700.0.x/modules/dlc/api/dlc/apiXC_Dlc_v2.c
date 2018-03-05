#include "MsTypes.h"
#include "MsCommon.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif

#include "utopia_dapi.h"
#include "apiXC_Dlc.h"
#include "apiXC_DBC.h"
#include "apiXC_DLC_private.h"
#include "apiXC_Dlc_v2.h"
#include "utopia.h"



// ========== Global definition ==========
void* pModuleDLC = NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL
void* pInstanceDLCStr = NULL;
MS_U32 DLCStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;
    stDLC_SetPowerState DBCArgs;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            if (pInstanceDLCStr == NULL)
            {
                if(UtopiaOpen(MODULE_DLC, &pInstanceDLCStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen DBC failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            DBCArgs.enPowerState = u32PowerState;
            DBCArgs.u32Return = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstanceDLCStr, E_DLC_CMD_SET_POWERSTATE, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain DBC engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=DBCArgs.u32Return;
            break;
        case E_POWER_RESUME:
            if (pInstanceDLCStr == NULL)
            {
                if(UtopiaOpen(MODULE_DLC, &pInstanceDLCStr, 0, NULL) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaOpen DBC failed\n");
                    return UTOPIA_STATUS_FAIL;
                }
            }
            DBCArgs.enPowerState = u32PowerState;
            DBCArgs.u32Return = UTOPIA_STATUS_FAIL;
            if(UtopiaIoctl(pInstanceDLCStr, E_DLC_CMD_SET_POWERSTATE, (void*)&DBCArgs) != UTOPIA_STATUS_SUCCESS)
            {
                printf("Obtain DBC engine fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            u32Return=DBCArgs.u32Return;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif
void DLCRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    UtopiaModuleCreate(MODULE_DLC, 0, &pModuleDLC);
    UtopiaModuleRegister(pModuleDLC);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleDLC, (FUtopiaOpen)DLCOpen, (FUtopiaClose)DLCClose, (FUtopiaIOctl)DLCIoctl);

    // 2. deal with resource  , XCݥiݭnresourceA䤤@ӬOreadonlyAt~@ӬOXCӥ]resource
    void* psResource = NULL;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModuleDLC, (FUtopiaSTR)DLCStr);
    UtopiaModuleSetSTRPrivate(pModuleDLC, sizeof(DLC_REGS_SAVE_AREA));
#endif
    // 3. resource can allocate private for internal use
    UtopiaModuleAddResourceStart(pModuleDLC, E_DLC_POOL_ID_INTERNAL);
    UtopiaResourceCreate("DLC", sizeof(DLC_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleDLC, psResource, E_DLC_POOL_ID_INTERNAL);
    UtopiaModuleAddResourceEnd(pModuleDLC, E_DLC_POOL_ID_INTERNAL);

    // shall we init first?
}


MS_U32 DLCOpen(void** ppInstance, const void* const pAttribute)
{
    DLC_INSTANCE_PRIVATE *psDLCInstPri = NULL;
    UtopiaInstanceCreate(sizeof(DLC_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psDLCInstPri);

    psDLCInstPri->fpDLC_Init_Ex = MApi_XC_DLC_Init_Ex_U2;
    psDLCInstPri->fpDLC_SetSetting_Ex = MApi_XC_DLC_SetSetting_Ex_U2;
    psDLCInstPri->fpDLC_SetCurve = MApi_XC_DLC_SetCurve_U2;
    psDLCInstPri->fpDLC_SetBleSlopPoint = MApi_XC_DLC_SetBleSlopPoint_U2;
    psDLCInstPri->fpDLC_SetHDRInit = MApi_XC_DLC_SetHDRInit_U2;
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
//MApi_XC_DLC_SetTMOInfo_U2 is defined by (UFO_XC_HDR_VERSION == 2)
    psDLCInstPri->fpDLC_SetTMOInfo = MApi_XC_DLC_SetTMOInfo_U2;
#endif
#endif
    psDLCInstPri->fpDLC_Exit = MApi_XC_DLC_Exit_U2;
    psDLCInstPri->fpDLC_SetOnOff = MApi_XC_DLC_SetOnOff_U2;
    psDLCInstPri->fpDLC_SetDlcHandlerOnOff = MApi_XC_DLC_SetDlcHandlerOnOff_U2;
    psDLCInstPri->fpDLC_SetBleOnOff = MApi_XC_DLC_SetBleOnOff_U2;
    psDLCInstPri->fpDLC_EnableMainSubCurveSynchronization = MApi_XC_DLC_EnableMainSubCurveSynchronization_U2;
    psDLCInstPri->fpDLC_Handler = MApi_XC_DLC_Handler_U2;
    psDLCInstPri->fpDLC_GetHistogramHandler = MApi_XC_DLC_GetHistogramHandler_U2;
    psDLCInstPri->fpDLC_GetAverageValue = MApi_XC_DLC_GetAverageValue_U2;
    psDLCInstPri->fpDLC_GetAverageValue_Ex = MApi_XC_DLC_GetAverageValue_Ex_U2;
    psDLCInstPri->fpDLC_InitCurve = MApi_XC_DLC_InitCurve_U2;
    psDLCInstPri->fpDLC_SpeedupTrigger = MApi_XC_DLC_SpeedupTrigger_U2;
    psDLCInstPri->fpDLC_GetLumaCurveStatus = MApi_XC_DLC_GetLumaCurveStatus_U2;
    psDLCInstPri->fpDLC_CGC_ResetCGain = MApi_XC_DLC_CGC_ResetCGain_U2;
    psDLCInstPri->fpDLC_CGC_CheckCGainInPQCom = MApi_XC_DLC_CGC_CheckCGainInPQCom_U2;
    psDLCInstPri->fpDLC_CGC_ResetYGain = MApi_XC_DLC_CGC_ResetYGain_U2;
    psDLCInstPri->fpDLC_CGC_CheckYGainInPQCom = MApi_XC_DLC_CGC_CheckYGainInPQCom_U2;
    psDLCInstPri->fpDLC_CGC_Reset = MApi_XC_DLC_CGC_Reset_U2;
    psDLCInstPri->fpDLC_CGC_Init = MApi_XC_DLC_CGC_Init_U2;
    psDLCInstPri->fpDLC_CGC_ReInit = MApi_XC_DLC_CGC_ReInit_U2;
    psDLCInstPri->fpDLC_CGC_Handler = MApi_XC_DLC_CGC_Handler_U2;
    psDLCInstPri->fpDLC_GetLibVer = MApi_XC_DLC_GetLibVer_U2;
    psDLCInstPri->fpDLC_GetInfo = MApi_XC_DLC_GetInfo_U2;
    psDLCInstPri->fpDLC_GetStatus_Ex = MApi_XC_DLC_GetStatus_Ex_U2;
    psDLCInstPri->fpDLC_SetDbgLevel = MApi_XC_DLC_SetDbgLevel_U2;
    psDLCInstPri->fpDLC_WriteCurve = MApi_XC_DLC_WriteCurve_U2;
    psDLCInstPri->fpDLC_WriteCurve_Sub = MApi_XC_DLC_WriteCurve_Sub_U2;
    psDLCInstPri->fpDLC_GetHistogram = MApi_XC_DLC_GetHistogram_U2;
    psDLCInstPri->fpDLC_DecodeExtCmd = MApi_XC_DLC_DecodeExtCmd_U2;
    psDLCInstPri->fpDLC_SetCaptureRange = MApi_XC_DLC_SetCaptureRange_U2;
    psDLCInstPri->fpDLC_SetSetting = MApi_XC_DLC_SetSetting_U2;
    psDLCInstPri->fpDLC_Init = MApi_XC_DLC_Init_U2;
    psDLCInstPri->fpDLC_GetStatus = MApi_XC_DLC_GetStatus_U2;
    psDLCInstPri->fpDLC_SetPowerState = MApi_XC_DLC_SetPowerState_U2;
    psDLCInstPri->fpDBC_DLC_Init =MApi_XC_DLC_DBC_Init_U2 ;
    psDLCInstPri->fpDBC_Init =MApi_XC_DBC_Init_U2 ;
    psDLCInstPri->fpDBC_Setstatus= MApi_XC_DLC_DBC_Setstatus_U2;
    psDLCInstPri->fpDBC_Getstatus =MApi_XC_DLC_DBC_Getstatus_U2 ;
    psDLCInstPri->fpDBC_SetReady = MApi_XC_DLC_DBC_SetReady_U2;
    psDLCInstPri->fpDBC_SetDebugMode =MApi_XC_DLC_DBC_SetDebugMode_U2 ;
    psDLCInstPri->fpDBC_GetDebugMode = MApi_XC_DLC_DBC_GetDebugMode_U2;
    psDLCInstPri->fpDBC_UpdatePWM =MApi_XC_DLC_DBC_UpdatePWM_U2 ;
    psDLCInstPri->fpDBC_Handler =MApi_XC_DLC_DBC_Handler_U2 ;
    psDLCInstPri->fpDBC_AdjustYCGain = MApi_XC_DLC_DBC_AdjustYCGain_U2;
    psDLCInstPri->fpDBC_YCGainInit = MApi_XC_DLC_DBC_YCGainInit_U2;
    psDLCInstPri->fpDBC_Reset = MApi_XC_DLC_DBC_Reset_U2;
    psDLCInstPri->fpDBC_DecodeExtCmd = MApi_XC_DBC_DecodeExtCmd_U2;
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 DLCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    void* pResource = NULL;
    DLC_INSTANCE_PRIVATE* psDLCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psDLCInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);


    switch(u32Cmd)
    {
        case E_DLC_CMD_INIT_EX:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_Init_Ex pDLCArgs = (pstDLC_Init_Ex)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_Init_Ex(pInstance, pDLCArgs->pstXC_DLC_InitData ,pDLCArgs->u32InitDataLen);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_SETTING_EX:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetSetting_Ex pDLCArgs = (pstDLC_SetSetting_Ex)pArgs;
            psDLCInstPri->fpDLC_SetSetting_Ex(pInstance, pDLCArgs->DLC_MFinit_Ex);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_CURVE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetCurve pDLCArgs = (pstDLC_SetCurve)pArgs;
            psDLCInstPri->fpDLC_SetCurve(pInstance, pDLCArgs->pNormal ,pDLCArgs->pLight ,pDLCArgs->pDark);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_BLE_SLOP_POINT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetBleSlopPoint pDLCArgs = (pstDLC_SetBleSlopPoint)pArgs;
            psDLCInstPri->fpDLC_SetBleSlopPoint(pInstance, pDLCArgs->pBLESlopPoint );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_HDR_INFO:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_HDRInitData pDLCArgs = (pstDLC_HDRInitData)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_SetHDRInit(pInstance, pDLCArgs->pstDLC_HDRInitData );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_DLC_CMD_SET_TMOINFO:
        {
            #if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
                if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
                {
                    printf("UtopiaDlcResourceObtain fail\n");
                    return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstDLC_SetTMOInfo pDLCArgs = (pstDLC_SetTMOInfo)pArgs;
                pDLCArgs->bReturn = psDLCInstPri->fpDLC_SetTMOInfo(pInstance, pDLCArgs->pstDLC_TMOInfo );

                UtopiaResourceRelease(pResource);
            #endif
            return UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_DLC_CMD_EXIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_Exit pDLCArgs = (pstDLC_Exit)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_Exit(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_ON_OFF:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetOnOff pDLCArgs = (pstDLC_SetOnOff)pArgs;
            psDLCInstPri->fpDLC_SetOnOff(pInstance, pDLCArgs->bSwitch ,pDLCArgs->bWindow);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_HANDLER_ON_OFF:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetDlcHandlerOnOff pDLCArgs = (pstDLC_SetDlcHandlerOnOff)pArgs;
            psDLCInstPri->fpDLC_SetDlcHandlerOnOff(pInstance, pDLCArgs->bDlcEnable );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_BLE_ON_OFF:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetBleOnOff pDLCArgs = (pstDLC_SetBleOnOff)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_SetBleOnOff(pInstance, pDLCArgs->bSwitch );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_ENABLE_MAIN_SUB_CURVE_SYNCHRONIZATION:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

             pstDLC_EnableMainSubCurveSynchronization pDLCArgs = (pstDLC_EnableMainSubCurveSynchronization)pArgs;
             psDLCInstPri->fpDLC_EnableMainSubCurveSynchronization(pInstance, pDLCArgs->bEnable );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_HANDLER:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_Handler pDLCArgs = (pstDLC_Handler)pArgs;
            psDLCInstPri->fpDLC_Handler(pInstance, pDLCArgs->bWindow);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_HISTOGRAM_HANDLER:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetHistogramHandler pDLCArgs = (pstDLC_GetHistogramHandler)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_GetHistogramHandler(pInstance, pDLCArgs->bWindow );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_AVERAGE_VALUE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetAverageValue pDLCArgs = (pstDLC_GetAverageValue)pArgs;
            pDLCArgs->u8Return = psDLCInstPri->fpDLC_GetAverageValue(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_AVERAGE_VALUE_EX:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetAverageValue_Ex pDLCArgs = (pstDLC_GetAverageValue_Ex)pArgs;
            pDLCArgs->u16Return = psDLCInstPri->fpDLC_GetAverageValue_Ex(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_INIT_CURVE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }


             pstDLC_InitCurve pDLCArgs = (pstDLC_InitCurve)pArgs;
             psDLCInstPri->fpDLC_InitCurve(pInstance, pDLCArgs->u16HStart , pDLCArgs->u16HEnd , pDLCArgs->u16VStart , pDLCArgs->u16VEnd );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SPEEDUP_TRIGGER:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SpeedupTrigger pDLCArgs = (pstDLC_SpeedupTrigger)pArgs;
            psDLCInstPri->fpDLC_SpeedupTrigger(pInstance, pDLCArgs->u8Loop );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_LUMA_CURVE_STATUS:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetLumaCurveStatus pDLCArgs = (pstDLC_GetLumaCurveStatus)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_GetLumaCurveStatus(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_RESET_C_GAIN:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_ResetCGain(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_CHECK_C_GAIN_IN_PQ_COM:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_CheckCGainInPQCom(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_RESET_Y_GAIN:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_ResetYGain(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_CHECK_Y_GAIN_IN_PQ_COM:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_CheckYGainInPQCom(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_RESET:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_Reset(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_INIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_Init(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_REINIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_ReInit(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_CGC_HANDLER:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDLC_CGC_Handler(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_LIB_VER:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetLibVer pDLCArgs = (pstDLC_GetLibVer)pArgs;
            pDLCArgs->eResult = psDLCInstPri->fpDLC_GetLibVer(pInstance, pDLCArgs->ppVersion);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_INFO:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetInfo pDLCArgs = (pstDLC_GetInfo)pArgs;
            pDLCArgs->eResult = psDLCInstPri->fpDLC_GetInfo(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_STATUS_EX:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetStatus_Ex pDLCArgs = (pstDLC_GetStatus_Ex)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_GetStatus_Ex(pInstance, pDLCArgs->pDrvStatus ,pDLCArgs->bWindow);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_DBG_LEVEL:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetDbgLevel pDLCArgs = (pstDLC_SetDbgLevel)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_SetDbgLevel(pInstance, pDLCArgs->u16DbgSwitch);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_WRITE_CURVE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_WriteCurve pDLCArgs = (pstDLC_WriteCurve)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_WriteCurve(pInstance, pDLCArgs->pu8Table );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_WRITE_CURVE_SUB:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_WriteCurve_Sub pDLCArgs = (pstDLC_WriteCurve_Sub)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_WriteCurve_Sub(pInstance, pDLCArgs->pu8Table );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_HISTOGRAM:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetHistogram pDLCArgs = (pstDLC_GetHistogram)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_GetHistogram(pInstance, pDLCArgs->pu16Histogram ,pDLCArgs->enHistogramType);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_DECODE_EXT_CMD:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_DecodeExtCmd pDLCArgs = (pstDLC_DecodeExtCmd)pArgs;
            psDLCInstPri->fpDLC_DecodeExtCmd(pInstance, pDLCArgs->params );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_CAPTURE_RANGE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetCaptureRange pDLCArgs = (pstDLC_SetCaptureRange)pArgs;
            psDLCInstPri->fpDLC_SetCaptureRange(pInstance, pDLCArgs->pu16_Range );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_SET_SETTING:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetSetting pDLCArgs = (pstDLC_SetSetting)pArgs;
            psDLCInstPri->fpDLC_SetSetting(pInstance, pDLCArgs->DLC_MFinit);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_INIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_Init pDLCArgs = (pstDLC_Init)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_Init(pInstance, pDLCArgs->pstXC_DLC_InitData ,pDLCArgs->u32InitDataLen);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DLC_CMD_GET_STATUS:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_GetStatus pDLCArgs = (pstDLC_GetStatus)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDLC_GetStatus(pInstance, pDLCArgs->pDrvStatus ,pDLCArgs->bWindow);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_DLC_CMD_SET_POWERSTATE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDLC_SetPowerState pDLCArgs = (pstDLC_SetPowerState)pArgs;
            pDLCArgs->u32Return = psDLCInstPri->fpDLC_SetPowerState(pInstance, pDLCArgs->enPowerState);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_DBC_CMD_DLC_INIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_DLC_Init pDLCArgs = (pstDBC_DLC_Init)pArgs;
            psDLCInstPri->fpDBC_DLC_Init(pInstance, pDLCArgs->DLC_DBC_MFinit );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_INIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDBC_Init(pInstance );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_SET_STATUS:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_Setstatus pDLCArgs = (pstDBC_Setstatus)pArgs;
            psDLCInstPri->fpDBC_Setstatus(pInstance,  pDLCArgs->bDBCEnable );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_GET_STATUS:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_Getstatus pDLCArgs = (pstDBC_Getstatus)pArgs;
            pDLCArgs->bReturn = psDLCInstPri->fpDBC_Getstatus(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_SET_READY:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_SetReady pDLCArgs = (pstDBC_SetReady)pArgs;
            psDLCInstPri->fpDBC_SetReady(pInstance,  pDLCArgs->bDBCReady);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_SET_DEBUG_MODE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_SetDebugMode pDLCArgs = (pstDBC_SetDebugMode)pArgs;
            psDLCInstPri->fpDBC_SetDebugMode(pInstance,  pDLCArgs->ucDBC_DebugMode);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_GET_DEBUG_MODE:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_GetDebugMode pDLCArgs = (pstDBC_GetDebugMode)pArgs;
            pDLCArgs->u8Return = psDLCInstPri->fpDBC_GetDebugMode(pInstance);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_UPDATE_PWM:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_UpdatePWM pDLCArgs = (pstDBC_UpdatePWM)pArgs;
            psDLCInstPri->fpDBC_UpdatePWM(pInstance ,pDLCArgs->u8PWMvalue);

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_HANDLER:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_Handler pDLCArgs = (pstDBC_Handler)pArgs;
            pDLCArgs->u16Return = psDLCInstPri->fpDBC_Handler(pInstance );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_ADJUST_YC_GAIN:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDBC_AdjustYCGain(pInstance );


            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_YC_GAIN_INIT:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_YCGainInit pDLCArgs = (pstDBC_YCGainInit)pArgs;
            psDLCInstPri->fpDBC_YCGainInit(pInstance ,pDLCArgs->u8YGain_M ,pDLCArgs->u8YGain_L ,pDLCArgs->u8CGain_M ,pDLCArgs->u8CGain_L );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_RESET:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            psDLCInstPri->fpDBC_Reset(pInstance );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DBC_CMD_DECODE_EXT_CMD:
        {
            if (UtopiaResourceObtain(pModule, E_DLC_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaDlcResourceObtain fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }

            pstDBC_DecodeExtCmd pDLCArgs = (pstDBC_DecodeExtCmd)pArgs;
            psDLCInstPri->fpDBC_DecodeExtCmd(pInstance ,pDLCArgs->params );

            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
            break;
        }

        default:
            printf("Error DLC Command = %td\n",(ptrdiff_t)u32Cmd);
            break;
    }
	return UTOPIA_STATUS_SUCCESS;
}


MS_U32 DLCClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


