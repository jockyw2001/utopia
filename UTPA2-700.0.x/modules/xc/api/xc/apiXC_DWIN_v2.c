#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsCommon.h"
#include "mhal_xc_chip_config.h"
#include "apiXC_DWIN_v2.h"
#include "XC_DWIN_private.h"
#include "mvideo_context.h"
#include "drv_sc_DIP_scaling.h"
#include "xc_hwreg_utility2.h"

#if (defined ANDROID) && (defined TV_OS)
#include <cutils/log.h>
#define printf LOGD
#ifndef LOGD // android 4.1 rename LOGx to ALOGx
#define LOGD ALOGD
#endif
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 DIPMdbIoctl(MS_U32 u32Cmd, const void* const pArgs)
{
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;
    switch(u32Cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            MdbPrint(paraCmdLine->u64ReqHdl,"=====Echo DIP=====\n");
            MDrv_XC_DIP_Mdb_Cmdline(paraCmdLine->u64ReqHdl,paraCmdLine->u32CmdSize,paraCmdLine->pcCmdLine);
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;
            break;
        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            MdbPrint(paraGetInfo->u64ReqHdl,"=====Read all DIP Information Start=====\n");
            MDrv_XC_DIP_Mdb_GetInfo(paraGetInfo->u64ReqHdl);
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;
            MdbPrint(paraGetInfo->u64ReqHdl,"=====Read all DIP Information End=====\n");
            break;
        default:
            break;
    }
    return 0;
}
#endif

void DIPRegisterToUtopia(FUtopiaOpen ModuleType)
{
    SCALER_DIP_WIN i;
    char name[8] = "\0";
    void* psResource = NULL;
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_DIP, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DIPOpen, (FUtopiaClose)DIPClose, (FUtopiaIOctl)DIPIoctl);
    #ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("dip", (FUtopiaMdbIoctl)DIPMdbIoctl);
    #endif

    // 2. deal with resource
    for(i=DIP_WINDOW ;i<MAX_DIP_WINDOW;i++)
    {
        snprintf(name,sizeof(name),"DIP%d",i);
        UtopiaModuleAddResourceStart(pUtopiaModule,i);
        UtopiaResourceCreate(name, sizeof(DIP_RESOURCE_PRIVATE), &psResource);
        UtopiaResourceRegister(pUtopiaModule, psResource, i);
        UtopiaModuleAddResourceEnd(pUtopiaModule, i);
    }
    UtopiaModuleAddResourceStart(pUtopiaModule,MAX_DIP_WINDOW);
    UtopiaResourceCreate("DIPR", sizeof(DIP_SHARE_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, MAX_DIP_WINDOW);
    UtopiaModuleAddResourceEnd(pUtopiaModule,MAX_DIP_WINDOW);
}

MS_U32 DIPOpen(void** ppInstance, const void* const pAttribute)
{
    XC_INFO("\n[DIP INFO] dip open");
    DIP_INSTANCE_PRIVATE *pDipPri = NULL;
    UtopiaInstanceCreate(sizeof(DIP_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pDipPri);

    pDipPri->bInit = FALSE;
    pDipPri->u8DbgLevel = E_XC_DIP_Debug_Level_LOW;
    pDipPri->eWindow = MAX_DIP_WINDOW;
    pDipPri->pResource = NULL;
    pDipPri->pShareResource = NULL;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DIPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //printf("\n[DIP INFO] dip ioctl cmd = %ld\n",u32Cmd);
    DIP_RESOURCE* pDipResource = NULL;
    DIP_INPUT_SOURCE* pInputSource = NULL;
    DIP_SET_WINDOW* pSetWindow = NULL;
    DIP_SET_WIN_PROPERTY* pSetWinProperty = NULL;
    DIP_SET_PROPERTY* pSetProperty = NULL;
    DIP_BUF_INFO* pBufInfo = NULL;
    DIP_INTER_STUS* pIntrStus = NULL;
    DIP_INTER_CB* pIntrCb = NULL;
    DIP_TRIGGER_MODE* pTrigMode = NULL;
    DIP_SET_DIPR_PROPERTY_EX* pSetDIPRProperty_ex = NULL;
    DIP_CMDQ_SET_ACTION* pDipCmdq = NULL;
    DIP_CMDQ_SET_OPERATIONS* pDipCmdqOps = NULL;
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    MS_U32 u32Ret = UTOPIA_STATUS_SUCCESS;
    DIP_CONFIG_PIPE* pDIP_Config_Pipe = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    //Keep share memory point
    if(pDipPri->pShareResource == NULL)
    {
        if(UtopiaResourceObtain(pModule,MAX_DIP_WINDOW, &(pDipPri->pShareResource)) != 0)
        {
            XC_ERR("UtopiaResourceObtainToInstant fail\n");
            return UTOPIA_STATUS_FAIL;
        }
        UtopiaResourceRelease(pDipPri->pShareResource);
    }
    switch(u32Cmd)
    {
        case MDrv_CMD_DIP_INIT:
            pDipResource = (DIP_RESOURCE*)pArgs;
            MDrv_XC_DIP_Init(pInstance,pDipResource->eWindow);
            break;
        case MDrv_CMD_DIP_GET_RESOURCE:
            pDipResource = (DIP_RESOURCE*)pArgs;
            //XC_INFO("MDrv_CMD_DIP_GET_RESOURCE[%d]  start\n",pDipResource->eWindow);

            //take to use mutex lock
            if(UtopiaResourceObtain(pModule,MAX_DIP_WINDOW, &(pDipPri->pShareResource)) != 0)
            {
                XC_ERR("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));

            //obtain check
            if(pDipPri->pResource != NULL)
            {
                XC_ERR("MDrv_CMD_DIP_GET_RESOURCE[%d]   have already obtained\n",pDipResource->eWindow);
                //take to use mutex unlock
                UtopiaResourceRelease(pDipPri->pShareResource);
                return UTOPIA_STATUS_FAIL;
            }
            if(UtopiaResourceObtain(pModule, pDipResource->eWindow, &(pDipPri->pResource)) != 0)
            {
                XC_ERR("UtopiaResourceObtainToInstant fail\n");
                //take to use mutex unlock
                UtopiaResourceRelease(pDipPri->pShareResource);
                return UTOPIA_STATUS_FAIL;
            }
            pDipResPri->bLock[pDipResource->eWindow] = 1;

            //take to use mutex unlock
            UtopiaResourceRelease(pDipPri->pShareResource);
            //XC_INFO("MDrv_CMD_DIP_GET_RESOURCE[%d]   end\n",pDipResource->eWindow);
            break;
        case MDrv_CMD_DIP_RELEASE_RESOURCE:
            pDipResource = (DIP_RESOURCE*)pArgs;
            //XC_INFO("MDrv_CMD_DIP_RELEASE_RESOURCE[%d]   start\n",pDipResource->eWindow);

            //take to use mutex lock
            if(UtopiaResourceObtain(pModule,MAX_DIP_WINDOW, &(pDipPri->pShareResource)) != 0)
            {
                XC_ERR("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));

            //release check
            if(pDipPri->pResource == NULL)
            {
                XC_ERR("MDrv_CMD_DIP_RELEASE_RESOURCE[%d]   have already released\n",pDipResource->eWindow);
                //take to use mutex unlock
                UtopiaResourceRelease(pDipPri->pShareResource);
                return UTOPIA_STATUS_FAIL;
            }
            UtopiaResourceRelease(pDipPri->pResource);
            pDipPri->pResource = NULL;
            pDipResPri->bLock[pDipResource->eWindow] = 0;

            //take to use mutex unlock
            UtopiaResourceRelease(pDipPri->pShareResource);
            //XC_INFO("MDrv_CMD_DIP_RELEASE_RESOURCE[%d]   end\n",pDipResource->eWindow);
            break;
        case MDrv_CMD_DIP_QUERY_RESOURCE:
            pDipResource = (DIP_RESOURCE*)pArgs;

            //take to use mutex lock
            if(UtopiaResourceObtain(pModule,MAX_DIP_WINDOW, &(pDipPri->pShareResource)) != 0)
            {
                XC_ERR("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_FAIL;
            }
            UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));

            if(pDipResPri->bLock[pDipResource->eWindow] == 1)
            {
                u32Ret = UTOPIA_STATUS_NO_RESOURCE;
            }

            //take to use mutex unlock
            UtopiaResourceRelease(pDipPri->pShareResource);
            break;

        case MDrv_CMD_DIP_CMDQ_SETACTION:
            pDipCmdq = (DIP_CMDQ_SET_ACTION*)pArgs;
            MDrv_XC_DIP_CMDQ_SetAction(pInstance, pDipCmdq->eAction, pDipCmdq->eWindow);
            break;
        case MDrv_CMD_DIP_CMDQ_SETOPERATIONS:
            pDipCmdqOps = (DIP_CMDQ_SET_OPERATIONS*)pArgs;
            MDrv_XC_DIP_CMDQ_SetOperations(pInstance, pDipCmdqOps->pOps, pDipCmdqOps->bEnable, pDipCmdqOps->eWindow);
            break;

        case MDrv_CMD_DIP_SET_INPUTSOURCE:
            pInputSource = (DIP_INPUT_SOURCE*)pArgs;

            break;
        case MDrv_CMD_DIP_SET_WINDOW:
            pSetWindow = (DIP_SET_WINDOW*)pArgs;
            //_XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
            MDrv_XC_DIP_SetWindow(pInstance,&(pSetWindow->WinInfo), pSetWindow->u32DataLen, pSetWindow->eWindow);
            //_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            break;
        case MDrv_CMD_DIP_SET_WINPROPERTY:
            pSetWinProperty = (DIP_SET_WIN_PROPERTY*)pArgs;
            MDrv_XC_DIP_SetWinProperty(pInstance,&(pSetWinProperty->WinProperty),pSetWinProperty->eWindow);
            break;
        case MDrv_CMD_DIP_GET_BUFINFO:
            pBufInfo = (DIP_BUF_INFO*)pArgs;
            pBufInfo->BufInfo = MDrv_XC_DIP_GetBufferInfo(pInstance,pBufInfo->eWindow);
            break;
        case MDrv_CMD_DIP_INTERRUPT:
            pIntrStus = (DIP_INTER_STUS*)pArgs;
            if(pIntrStus->eFlag == DIP_INTR_ENABLE)
                MDrv_XC_DIP_EnableIntr(pInstance,pIntrStus->IntrStautus,TRUE,pIntrStus->eWindow);
            else if(pIntrStus->eFlag == DIP_INTR_GET)
                pIntrStus->IntrStautus = MDrv_XC_DIP_GetIntrStatus(pInstance,pIntrStus->eWindow);
            else if(pIntrStus->eFlag == DIP_INTR_CLEAR)
                MDrv_XC_DIP_ClearIntr(pInstance,pIntrStus->IntrStautus,pIntrStus->eWindow);
            else
                MDrv_XC_DIP_EnableIntr(pInstance,pIntrStus->IntrStautus,FALSE,pIntrStus->eWindow);
            break;
        case MDrv_CMD_DIP_INTERRUPT_CB:
            pIntrCb = (DIP_INTER_CB*)pArgs;
            if(pIntrCb->eFlag == DIP_INTR_ATTACH)
                MDrv_XC_DIP_InterruptAttach(pInstance,pIntrCb->pIntCb,pIntrCb->eWindow);
            else if(pIntrCb->eFlag == DIP_INTR_DETACH)
                MDrv_XC_DIP_InterruptDetach(pInstance,pIntrCb->eWindow);
            break;
        case MDrv_CMD_DIP_TRIGGER:
            pTrigMode = (DIP_TRIGGER_MODE*)pArgs;
            if(pTrigMode->eTrigMode == DIP_TRIG_ON)
            {
                //_XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
                MDrv_XC_DIP_EnableCaptureStream(pInstance,TRUE,pTrigMode->eWindow);
                //_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            }
            else if(pTrigMode->eTrigMode == DIP_TRIG_ONESHOT)
                MDrv_XC_DIP_CaptureOneFrame(pInstance,pTrigMode->eWindow);
            else if(pTrigMode->eTrigMode == DIP_TRIG_ONESHOTFAST)
                MDrv_XC_DIP_CaptureOneFrame2(pInstance,pTrigMode->eWindow);
            else if(pTrigMode->eTrigMode == DIP_TRIG_RESET)
                MDrv_XC_DIP_SWReset(pInstance,pTrigMode->eWindow);
            else
            {
                //_XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
                MDrv_XC_DIP_EnableCaptureStream(pInstance,FALSE,pTrigMode->eWindow);
                //_XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            }
            break;
        case MDrv_CMD_DIP_SET_DIPR_PROPERTY:
            pSetDIPRProperty_ex = (DIP_SET_DIPR_PROPERTY_EX*)pArgs;
            if(pSetDIPRProperty_ex->enDIPRFlag==E_DIP_SET_DIPR_NORMAL)
            {
                MDrv_XC_DIP_SetDIPRProperty(pInstance,&(pSetDIPRProperty_ex->stDIPRProperty_ex.stDIPRProperty),pSetDIPRProperty_ex->eWindow);
            }
            else if(pSetDIPRProperty_ex->enDIPRFlag==E_DIP_SET_DIPR_EX)
            {
                MDrv_XC_DIP_SetDIPRProperty_EX(pInstance,&(pSetDIPRProperty_ex->stDIPRProperty_ex),pSetDIPRProperty_ex->eWindow);
            }
            break;
        case MDrv_CMD_DIP_SET_PROPERTY:
            pSetProperty = (DIP_SET_PROPERTY*)pArgs;

            if (pSetProperty->eFlag& DIP_PRO_SCANMODE)
                MDrv_XC_DIP_SelectSourceScanType(pInstance,(EN_XC_DWIN_SCAN_TYPE)pSetProperty->Property.eScanMode,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_DATAFMT)
                MDrv_XC_DIP_SetDataFmt(pInstance,(EN_DRV_XC_DWIN_DATA_FMT)pSetProperty->Property.eDataFmt,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_R2Y)
                MDrv_XC_DWIN_EnableR2YCSC(pInstance,pSetProperty->Property.bR2Y,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_Y2R)
                MDrv_XC_DIP_SetY2R(pInstance,pSetProperty->Property.bY2R,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_SWAPUV)
                MDrv_XC_DIP_SetUVSwap(pInstance,pSetProperty->Property.bSwapUV,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_SWAPYC)
                MDrv_XC_DIP_SetYCSwap(pInstance,pSetProperty->Property.bSwapYC,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_SWAPRGB)
                MDrv_XC_DIP_SetRGBSwap(pInstance,pSetProperty->Property.stSwapRGB.bSwap,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_INTERLACE_W)
                MDrv_XC_DIP_SetInterlaceWrite(pInstance,pSetProperty->Property.bInterlaceW,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_ALPHA)
                MDrv_XC_DIP_SetAlphaValue(pInstance,pSetProperty->Property.u8Alpha,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_FRC)
                MDrv_XC_DIP_SetFRC(pInstance,pSetProperty->Property.stFrameRateControl.bFrameRateCtrl,pSetProperty->Property.stFrameRateControl.u16In,pSetProperty->Property.stFrameRateControl.u16Out,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_OP_CAPTURE)
                MDrv_XC_DIP_SetOutputCapture(pInstance,pSetProperty->Property.stOpCapture.bOpCapture,pSetProperty->Property.stOpCapture.eOpCapture,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_MIRROR)
                MDrv_XC_DIP_SetMirror(pInstance,pSetProperty->Property.stMirror.bHMirror,pSetProperty->Property.stMirror.bVMirror,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_PINPON)
                MDrv_XC_DIP_SetPinpon(pInstance,pSetProperty->Property.stPinpon.bPinpon,pSetProperty->Property.stPinpon.u32PinponAddr,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_ROTATION)
                MDrv_XC_DIP_SetRotation(pInstance,pSetProperty->Property.stRotation.bRotation,pSetProperty->Property.stRotation.eRoDirection,pSetProperty->eWindow);
            if (pSetProperty->eFlag & DIP_PRO_HVSP)
            {
                if( MDrv_XC_DIP_SetHVSP(pInstance,pSetProperty->Property.bHVSP,pSetProperty->eWindow) == FALSE)
                    return UTOPIA_STATUS_NO_RESOURCE;
            }
            if (pSetProperty->eFlag & DIP_PRO_TILEBLOCK)
            {
                if( MDrv_XC_DIP_Set420TileBlock(pInstance,pSetProperty->Property.eTileBlock,pSetProperty->eWindow) == FALSE)
                    return UTOPIA_STATUS_NOT_SUPPORTED;
            }
            break;
        case MDrv_CMD_DIP_SET_DBG_LEVEL:
                pDipPri->u8DbgLevel = *(MS_U8 *)(pArgs);
                MDrv_XC_DIP_SetBdgLevel(*(MS_U8 *)(pArgs));
            break;

        case MDrv_CMD_DIP_CONFIG_PIPE:
                pDIP_Config_Pipe = (DIP_CONFIG_PIPE*)pArgs;
                MDrv_XC_DIP_ConfigPipe_U2(pInstance, pDIP_Config_Pipe->u32PipeID, pDIP_Config_Pipe->u32SecureDMA, pDIP_Config_Pipe->u32OperationMode);
                break;
    }
    return u32Ret; // FIXME: error code
}

MS_U32 DIPClose(void* pInstance)
{
    XC_INFO("\n[DIP INFO] dip close");
    UtopiaInstanceDelete(pInstance);
    return UTOPIA_STATUS_SUCCESS;
}
