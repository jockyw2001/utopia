#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "utopia_driver_id.h"

#include "MsDevice.h"
//#include "drvTVEncoder.h"

#include "MsOS.h"
#include "tvencoder_private.h"
#include "tvencoder.h"
#include "utopia_api_database.h"
#include "ve_hwreg_utility2.h"

#define IOCTL_SUCCESS   0
#define atoi(str) simple_strtoul(((str != NULL) ? str : ""), NULL, 0)
// ========== Global definition ==========
void* pUtopiaTVEncoder=NULL;
#ifdef MSOS_TYPE_LINUX_KERNEL

MS_U32 VEStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return=UTOPIA_STATUS_FAIL;

    switch(u32PowerState)
    {
        case E_POWER_SUSPEND:
            _MDrv_TVE_SetPowerState(u32PowerState);
            u32Return=UTOPIA_STATUS_SUCCESS;
            break;
        case E_POWER_RESUME:
            _MDrv_TVE_SetPowerState(u32PowerState);
            u32Return=UTOPIA_STATUS_SUCCESS;
            break;
        default:
            printf("[%s][%d] PowerState:%lx not Implement now!!!\n",__FUNCTION__,__LINE__,u32PowerState);
            break;
    }
    return u32Return;
}
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT

void TVENCODERMdb_Cat(MDBCMD_CMDLINE_PARAMETER *paraCmdInfo)
{
    VE_DrvStatus* pDrvStatus;
    if(MDrv_VE_GetStatus(pDrvStatus) == E_VE_OK)
    {
        MdbPrint(paraCmdInfo->u64ReqHdl,"----------------  Mstar VE Info -----------------\n");
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE H capture start         : %d\n", pDrvStatus->u16H_CapStart);
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE H capture end           : %d\n", pDrvStatus->u16H_CapStart + pDrvStatus->u16H_CapSize);
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE H input size            : %d\n", pDrvStatus->u16H_CapSize);
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE H output size           : %d\n", pDrvStatus->u16H_CapSize* R2BYTE(0x103B0CUL));
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE H scaling ratio         : 0x%x\n", R2BYTE(0x103B0CUL));
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE V capture start         : %d\n", pDrvStatus->u16V_CapStart);
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE V capture end           : %d\n", pDrvStatus->u16V_CapStart + pDrvStatus->u16V_CapSize);
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE V output size           : %d\n", pDrvStatus->u16H_CapSize* R2BYTE(0x103B0EUL));
        MdbPrint(paraCmdInfo->u64ReqHdl,"VE V scaling ratio         : 0x%x\n", R2BYTE(0x103B0EUL));
        if(R2BYTEMSK(0x103B0EUL, 0x80) == 0x80)
        {
            MdbPrint(paraCmdInfo->u64ReqHdl,"VE mode                    : DE mode\n");
        }
        else
        {
            MdbPrint(paraCmdInfo->u64ReqHdl,"VE mode                    : HV mode\n");
        }
    }
}

void TVENCODERMdb_TestPattern(MS_U8 u8pattern, MS_BOOL bEnable)
{
    if(u8pattern == 1)
    {
        if(bEnable == TRUE)
        {
            W2BYTEMSK(0x103E06UL,BIT(4),BIT(4));
        }
        else if(bEnable == FALSE)
        {
            W2BYTEMSK(0x103E06UL,0,BIT(4));
        }
    }
    else if(u8pattern == 0)
    {
        static MS_BOOL bLastStatus = FALSE;
        static MS_U16 u16Hscaling, u16Vscaling, u16HscalingRatio, u16VscalingRatio;
        if(bEnable == TRUE)
        {
            if(bLastStatus == FALSE)
            {
                u16Hscaling = R2BYTEMSK(0x103B00UL,BIT(8));
                u16Vscaling = R2BYTEMSK(0x103B00UL,BIT(9));
                u16HscalingRatio = R2BYTE(0x103B2CUL);
                u16VscalingRatio = R2BYTE(0x103B2EUL);
            }
            W2BYTEMSK(0x103B00UL,BIT(10),BIT(10));
            W2BYTEMSK(0x103B00UL,BIT(11),BIT(11));
            W2BYTEMSK(0x103B00UL,BIT(8),BIT(8));
            W2BYTEMSK(0x103B00UL,BIT(9),BIT(9));
            W2BYTE(0x103B2CUL,0x0400);
            W2BYTE(0x103B2EUL,0x0400);
            bLastStatus = TRUE;
        }
        else if(bEnable == FALSE)
        {
            W2BYTEMSK(0x103B00UL,0,BIT(10));
            W2BYTEMSK(0x103B00UL,0,BIT(11));
            W2BYTEMSK(0x103B00UL,u16Hscaling,BIT(8));
            W2BYTEMSK(0x103B00UL,u16Vscaling,BIT(9));
            W2BYTE(0x103B2CUL,u16HscalingRatio);
            W2BYTE(0x103B2EUL,u16VscalingRatio);
            bLastStatus = FALSE;
        }
    }
}

void TVENCODERMdb_EchoCmd(MS_U64* u64ReqHdl, char* pcCmdLine)
{
    char pch[] = " =,";
    char* psep;
    psep = strsep(&pcCmdLine,pch);
    MS_U8 u8pattern;
    MS_BOOL bEnable = FALSE;

    if(strcmp(psep, "pattern") == 0)
    {
        psep = strsep(&pcCmdLine,pch); //for num
        psep = strsep(&pcCmdLine,pch);
        if(psep != NULL)
        {
            u8pattern = (MS_U8)atoi(psep);
        }
        psep = strsep(&pcCmdLine,pch);
        if(psep != NULL)
        {
            bEnable = (MS_BOOL)atoi(psep);
        }
        TVENCODERMdb_TestPattern(u8pattern, bEnable);
    }
}

MS_U32 TVENCODERMdbIoctl(MS_U32 cmd, const void* const pArgs)
{
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;
    switch(cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            /*MdbPrint(paraCmdLine->u64ReqHdl,"LINE:%d, MDBCMD_CMDLINE\n", __LINE__);
            MdbPrint(paraCmdLine->u64ReqHdl,"u32CmdSize: %d\n", paraCmdLine->u32CmdSize);
            MdbPrint(paraCmdLine->u64ReqHdl,"pcCmdLine: %s\n", paraCmdLine->pcCmdLine);*/
            TVENCODERMdb_EchoCmd(paraCmdLine->u64ReqHdl, paraCmdLine->pcCmdLine);
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;
            break;

        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            //MdbPrint(paraGetInfo->u64ReqHdl,"LINE:%d, MDBCMD_GETINFO\n", __LINE__);
            TVENCODERMdb_Cat(paraGetInfo);
            paraGetInfo->result = MDBRESULT_SUCCESS_FIN;
            break;
        default:
            MdbPrint(paraGetInfo->u64ReqHdl,"unknown cmd\n", __LINE__);
            break;
     }
     return 0;
}
#endif

// this func will be call to init by utopia20 framework
void TVENCODERRegisterToUtopia(FUtopiaOpen ModuleType)
{
    TVENCODER_RESOURCE_PRIVATE * pResPri = NULL;

    // 1. deal with module
    //void* pUtopiaTVEncoder=NULL;
    UtopiaModuleCreate(MODULE_TVENCODER, 8, &pUtopiaTVEncoder);
    UtopiaModuleRegister(pUtopiaTVEncoder);
    // register func for module, after register here,then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaTVEncoder, (FUtopiaOpen)TVENCODEROpen, (FUtopiaClose)TVENCODERClose, (FUtopiaIOctl)TVENCODERIoctl);

    #ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("ve", (FUtopiaMdbIoctl)TVENCODERMdbIoctl);
    #endif

    // 2. deal with resource
    void* psResource = NULL;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaTVEncoder, (FUtopiaSTR)VEStr);
    UtopiaModuleSetSTRPrivate(pUtopiaTVEncoder, sizeof(VE_REGS_SAVE_AREA));
#endif
    // start func to add res, call once will create 2 access in resource. Also can declare VE for another
    UtopiaModuleAddResourceStart(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0);
    // resource can alloc private for internal use, ex, VE_RESOURCE_PROVATE
    UtopiaResourceCreate("tvencoder0",sizeof(TVENCODER_RESOURCE_PRIVATE),&psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaTVEncoder, psResource, E_TVENCODER_POOL_ID_VE0);
    // end function to add res
    UtopiaModuleAddResourceEnd(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0);

    if(UtopiaResourceObtain(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0, &psResource) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return;
    }

    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    if(pResPri->u32Magic != MAGIC_NUM)
    {
        // not register
        pResPri->u32Magic = MAGIC_NUM;
        pResPri->bInited_Drv = FALSE;
        pResPri->u32MIUAddress = 0x00;
    }

    UtopiaResourceRelease(psResource);

}

MS_U32 TVENCODEROpen(void** ppInstance, const void* const pAttribute)
{
    //TVENCODER_INSTANT_PRIVATE *pVEpir = NULL;
    TVENCODER_INSTANT_PRIVATE *pstInstPri = NULL;
    printf("[VE info] VE open\n");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d",__FUNCTION__,__LINE__);
    // instance is allocated here, also can allocate private for internal use, ex
    UtopiaInstanceCreate(sizeof(TVENCODER_INSTANT_PRIVATE),ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private

    //UtopiaInstanceGetPrivate(*ppInstance,(void**)&pVEpir);
    UtopiaInstanceGetPrivate(*ppInstance,(void**)&pstInstPri);
    //pVEpir->fpTVESetInputSRC = (IOCTL_TVENCODER_SETINPUTSRC)MDrv_VE_SetInputSource;

#ifdef CONFIG_UTOPIA_SIZE_CUSTOMER
    #define URELATION(a,b,c)   pstInstPri->_URELATION_ASSIGN(a,b,c)
            _MODULE_VE_U1_U2_RELATION
    #undef URELEATION
#else
    pstInstPri->fpVE_GetInfo                                  =_MDrv_VE_GetInfo;
    pstInstPri->fpVE_GetStatus                              =_MDrv_VE_GetStatus;
    pstInstPri->fpVE_SetDbgLevel                           =_MDrv_VE_SetDbgLevel;
    pstInstPri->fpVE_SwitchInputSource                  =_MDrv_VE_SwitchInputSource;
    pstInstPri->fpVE_SetInputSource                      =_MDrv_VE_SetInputSource;
    pstInstPri->fpVE_SwitchOuputDest                    =_MDrv_VE_SwitchOuputDest;
    pstInstPri->fpVE_SetOutputCtrl                         =_MDrv_VE_SetOutputCtrl;
    pstInstPri->fpVE_SetOutputVideoStd                  =_MDrv_VE_SetOutputVideoStd;
    pstInstPri->fpVE_SetMemoryBaseAddr                 =_MDrv_VE_SetMemoryBaseAddr;
    pstInstPri->fpVE_Init                                       = _MDrv_VE_Init;
    pstInstPri->fpVE_Exit                                       =_MDrv_VE_Exit;
    pstInstPri->fpVE_SetWSSData                           =_MDrv_VE_SetWSSData;
    pstInstPri->fpVE_GetWSSData                           =_MDrv_VE_GetWSSData;
    pstInstPri->fpVE_SetMode                                =_MDrv_VE_SetMode;
    pstInstPri->fpVE_SetBlackScreen                       =_MDrv_VE_SetBlackScreen;
    pstInstPri->fpVE_IsBlackScreenEnabled               =_MDrv_VE_IsBlackScreenEnabled;
    pstInstPri->fpVE_SetTtxBuffer                           =_MDrv_VE_SetTtxBuffer;
    pstInstPri->fpVE_GetTtxReadDoneStatus             =_MDrv_VE_GetTtxReadDoneStatus;
    pstInstPri->fpVE_EnableCcSw                            =_MDrv_VE_EnableCcSw;
    pstInstPri->fpVE_SetCcRange                            =_MDrv_VE_SetCcRange;
    pstInstPri->fpVE_SendCcData                            =_MDrv_VE_SendCcData;
    pstInstPri->fpVE_set_display_window                  =_MDrv_VE_set_display_window;
    pstInstPri->fpVE_SetOSD                                  =_MDrv_VE_SetOSD;
    pstInstPri->fpVE_SetRGBIn                                =_MDrv_VE_SetRGBIn;
    pstInstPri->fpVE_Get_Output_Video_Std              =_MDrv_VE_Get_Output_Video_Std;
    pstInstPri->fpVE_SetCaptureMode                      =_MDrv_VE_SetCaptureMode;
    pstInstPri->fpVE_GetDstInfo                              = _MApi_VE_GetDstInfo;
    pstInstPri->fpVE_Set_TestPattern                      =_MDrv_VE_Set_TestPattern;
    pstInstPri->fpVE_DisableRegWrite                       =_MDrv_VE_DisableRegWrite;
    pstInstPri->fpVE_AdjustPositionBase                   =_MDrv_VE_AdjustPositionBase;
    pstInstPri->fpVE_Set_Customer_Scaling              =_MDrv_VE_Set_Customer_Scaling;
    pstInstPri->fpVE_InitVECapture                          = _MDrv_VE_InitVECapture;
    pstInstPri->fpVE_EnaVECapture                         = _MDrv_VE_EnaVECapture;
    pstInstPri->fpVE_Adjust_FrameStart                   =_MDrv_VE_Adjust_FrameStart;
    pstInstPri->fpTVE_SetPowerState                      =_MDrv_TVE_SetPowerState;
    pstInstPri->fpVE_SetCusTable                           =_MDrv_VE_SetCusTable;
    pstInstPri->fpVE_EnableCusTable                       =_MDrv_VE_EnableCusTable;
    pstInstPri->fpVE_SetFrameColor                        =_MDrv_VE_SetFrameColor;
    pstInstPri->fpVE_Set_OSDLayer                         =_MDrv_VE_Set_OSDLayer;
    pstInstPri->fpVE_Get_OSDLayer                        =_MDrv_VE_Get_OSDLayer;
    pstInstPri->fpVE_Set_VideoAlpha                       =_MDrv_VE_Set_VideoAlpha;
    pstInstPri->fpVE_Get_VideoAlpha                       =_MDrv_VE_Get_VideoAlpha;
    pstInstPri->fpVE_DCS_SetType                         =_MDrv_VE_DCS_SetType;
    pstInstPri->fpVE_DCS_SetActivationKey             =_MDrv_VE_DCS_SetActivationKey;
    pstInstPri->fpVE_ReloadSetting                         =_MDrv_VE_ReloadSetting;
    pstInstPri->fpVE_GetConfig                              =_MDrv_VE_GetConfig;
#endif

    return UTOPIA_STATUS_SUCCESS;



}

MS_U32 TVENCODERIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //UTOPIA_INSTANT* pInstant = (UTOPIA_INSTANT*)*pInstantTmp;
    //UTOPIA_RESOURCE* pstRes = NULL;

    //void* pModule = NULL;
    //UtopiaInstanceGetModule(pInstance, &pModule);
    //void* pResource = NULL;

    void* pstRes = NULL;
    TVENCODER_RESOURCE_PRIVATE * pstResPri = NULL;
    MS_U32 u32Ret =UTOPIA_STATUS_FAIL;
    ///TVENCODER_RESOURCE_PRIVATE *pstResPri = NULL;
    TVENCODER_INSTANT_PRIVATE *pstInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pstInstPri);

    // Assert

    if(UtopiaResourceObtain(pUtopiaTVEncoder, E_TVENCODER_POOL_ID_VE0, &pstRes) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    // E_MDRV_CMD_TVE_DisableRegWrite and E_MDRV_CMD_TVE_SetDbgLevel
    // might be invoked before VE Init
    if ((u32Cmd != E_MDRV_CMD_TVE_DisableRegWrite)
     && (u32Cmd != E_MDRV_CMD_TVE_Adjust_FrameStart)
     && (u32Cmd != E_MDRV_CMD_TVE_SetDbgLevel)
     && (u32Cmd != E_MDRV_CMD_TVE_AdjustPositionBase))
    {
        // to check init first
        if((FALSE == pstResPri->bInited_Drv)  &&
            (u32Cmd != E_MDRV_CMD_TVE_Init))
        {
            UtopiaResourceRelease(pstRes);
            return UTOPIA_STATUS_FAIL;
        }
    }
    //TVENCODER_SetInputSRC pSetinputSRCParam = NULL;

    switch(u32Cmd)
    {
        case E_MDRV_CMD_TVE_GetInfo:
        {
            PTVE_GETINFO pGetInfo = (PTVE_GETINFO)pArgs;
            pGetInfo->eRet = E_TVENCODER_OK;
            memcpy(pGetInfo->pVE_DrvInfo,pstInstPri->fpVE_GetInfo(),sizeof(VE_DrvInfo));
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
            //pstInstPri->fpVE_GetInfo(pInstance,pGetInfo->pVE_DrvInfo);
        }

        case E_MDRV_CMD_TVE_GetStatues:
        {
            PTVE_GETSTATUS pGetStatus = (PTVE_GETSTATUS)pArgs;
            if(E_VE_OK == pstInstPri->fpVE_GetStatus(pGetStatus->pStaus))
            {
                pGetStatus->eRet = E_TVENCODER_OK;
            }
            else
            {
                pGetStatus->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetDbgLevel:
        {
            PTVE_SETDBGLEVEL pDbgLevel = (PTVE_SETDBGLEVEL)pArgs;
            pstInstPri->fpVE_SetDbgLevel(pDbgLevel->u16Level);
            pDbgLevel->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SwitchInputSRC:
        {
            PTVE_SWITCH_SRC_INFO pSwitchSrc_info = (PTVE_SWITCH_SRC_INFO)pArgs;
            pstInstPri->fpVE_SwitchInputSource(pSwitchSrc_info->pSwithcSrc_info);
            pSwitchSrc_info->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetInputSRC:
        {
            PTVE_SETINPUTSRC pSetInputSrc = (PTVE_SETINPUTSRC)pArgs;
            if(MS_VE_SWITCH_SRC_SUCCESS == pstInstPri->fpVE_SetInputSource(pSetInputSrc->pInputSrcInfo))
            {
                pSetInputSrc->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetInputSrc->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SwitchOutputDest:
        {
            PTVE_SWITCHOUTPUTDEST pSwitchOutputDest = (PTVE_SWITCHOUTPUTDEST)pArgs;
            pstInstPri->fpVE_SwitchOuputDest(pSwitchOutputDest->pSwitchInfo);
            pSwitchOutputDest->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetOutputCtrl:
        {
            PTVE_SETOUTPUTCTRL pSetOutputCtrl = (PTVE_SETOUTPUTCTRL)pArgs;
            pstInstPri->fpVE_SetOutputCtrl(pSetOutputCtrl->pOutputCtrl);
            pSetOutputCtrl->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetOutputVideoStd:
        {
            PTVE_SETOUTPUTVIDEOSTD pSetOutputVideoStd = (PTVE_SETOUTPUTVIDEOSTD)pArgs;
            if(TRUE == pstInstPri->fpVE_SetOutputVideoStd(pSetOutputVideoStd->VideoSystem))
            {
                pSetOutputVideoStd->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetOutputVideoStd->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetMemoryBaseAddr:
        {
            PTVE_SETMEMORYBASEADDR pSetMemoryBaseAddr = (PTVE_SETMEMORYBASEADDR)pArgs;
            pstInstPri->fpVE_SetMemoryBaseAddr(pSetMemoryBaseAddr->phyMIUAddress,pSetMemoryBaseAddr->u32MIUSize);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Init:
        {
            if(pstResPri->bInited_Drv == FALSE)
            {
                PTVE_INIT pInit = (PTVE_INIT)pArgs;
                if(pInit->u32MIUAddress == NULL)
                {
                    // coverity[loop_top]
                    while(!pstResPri->bInited_Drv); // wait initialize in other process
                    pInit->eRet = E_TVENCODER_OK;
                    u32Ret = UTOPIA_STATUS_SUCCESS;

                }
                else
                {
                    pstInstPri->fpVE_Init(pInit->u32MIUAddress);
                    pInit->eRet = E_TVENCODER_OK;
                    u32Ret = UTOPIA_STATUS_SUCCESS;
                    pstResPri->bInited_Drv = TRUE;
                    pstResPri->u32MIUAddress = pInit->u32MIUAddress;
                }
            }
            break;
        }

        case E_MDRV_CMD_TVE_Exit:
        {
            PTVE_EXIT pExit = (PTVE_EXIT)pArgs;
            if(TRUE == pstInstPri->fpVE_Exit())
            {
                pExit->eRet = E_TVENCODER_OK;
            }
            else
            {
                pExit->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetWSSData:
        {
            PTVE_SETWSSDATA pSetWSSdata = (PTVE_SETWSSDATA) pArgs;
            pstInstPri->fpVE_SetWSSData(pSetWSSdata->bEn,pSetWSSdata->u16WSSData);
            pSetWSSdata->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_GetWSSData:
        {
            PTVE_GETWSSDATA pGetWSSData = (PTVE_GETWSSDATA) pArgs;
            pGetWSSData->u16WSSData = pstInstPri->fpVE_GetWSSData();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetMode:
        {
            PTVE_SETMODE pSetMode = (PTVE_SETMODE)pArgs;
            pstInstPri->fpVE_SetMode(pSetMode-> pVESetMode);
            pSetMode->eRet = E_TVENCODER_OK;
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetBlackSceen:
        {
            PTVE_SETBLACKSCREEN pSetBlackScreen = (PTVE_SETBLACKSCREEN)pArgs;
            pstInstPri->fpVE_SetBlackScreen(pSetBlackScreen->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_IsBlackScreenEnabled:
        {
            PTVE_ISBLACKSCREENENABLE pBlackScreenEnable = (PTVE_ISBLACKSCREENENABLE)pArgs;
            pBlackScreenEnable->bEnable = pstInstPri->fpVE_IsBlackScreenEnabled();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetTtxBuffer:
        {
            PTVE_SETTTXBUFFER pSetTtxBuffer = (PTVE_SETTTXBUFFER)pArgs;
            pstInstPri->fpVE_SetTtxBuffer(pSetTtxBuffer->u32StartAddr,pSetTtxBuffer->u32Size);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_VE_GetTtxReadDoneStatus:
        {
            PTVE_GETTTXREADDONE pGetTtxReadDoneStatus = (PTVE_GETTTXREADDONE)pArgs;
            pGetTtxReadDoneStatus->bDone = pstInstPri->fpVE_GetTtxReadDoneStatus();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_EnableCcSw:
        {
            PTVE_ENABLECCSW pEnableCcSw = (PTVE_ENABLECCSW)pArgs;
            pstInstPri->fpVE_EnableCcSw(pEnableCcSw->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetCcRange:
        {
            PTVE_SETCCRANGE pSetCcRange = (PTVE_SETCCRANGE)pArgs;
            pstInstPri->fpVE_SetCcRange(pSetCcRange->u16odd_start,pSetCcRange->u16odd_end,
                               pSetCcRange->u16even_start,pSetCcRange->u16even_end);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SendCcData:
        {
            PTVE_SENDCCDATA pSendCcData = (PTVE_SENDCCDATA) pArgs;
            pstInstPri->fpVE_SendCcData(pSendCcData->bIsOdd,pSendCcData->u16data);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_set_display_window:
        {
            PTVE_SETDISPLAYWIN pSetDisplayWin = (PTVE_SETDISPLAYWIN)pArgs;
            pstInstPri->fpVE_set_display_window(pSetDisplayWin->stDispWin);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetOSD:
        {
            PTVE_SETOSD pSetOsd = (PTVE_SETOSD)pArgs;
            pstInstPri->fpVE_SetOSD(pSetOsd->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetRGBIn:
        {
            PTVE_SETRGBIN pSetRGBIn = (PTVE_SETRGBIN)pArgs;
            pstInstPri->fpVE_SetRGBIn(pSetRGBIn->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Get_Output_Video_Std:
        {
            PTVE_GETOUTPUTVIDEOSTD pOutputVideoSTD = (PTVE_GETOUTPUTVIDEOSTD)pArgs;
            pOutputVideoSTD->stVideoSys = pstInstPri->fpVE_Get_Output_Video_Std();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetCaptureMode:
        {
            PTVE_SETCAPMODE pSetCaptureMode = (PTVE_SETCAPMODE)pArgs;
            pstInstPri->fpVE_SetCaptureMode(pSetCaptureMode->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_GetDstInfo:
        {
            PTVE_GETDSTINFO pGetDstInfo = (PTVE_GETDSTINFO)pArgs;
            if(TRUE == pstInstPri->fpVE_GetDstInfo(pGetDstInfo->pDstInfo,pGetDstInfo->u32SizeofDstInfo))
            {
                pGetDstInfo->eRet = E_TVENCODER_OK;
            }
            else
            {
                pGetDstInfo->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_TestPattern:
        {
            PTVE_SETTESTPATTERN pSetTestPattern = (PTVE_SETTESTPATTERN)pArgs;
            pstInstPri->fpVE_Set_TestPattern(pSetTestPattern->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_DisableRegWrite:
        {
            PTVE_DISABLEREGWRITE pDiableRegWrite = (PTVE_DISABLEREGWRITE)pArgs;
            if(E_VE_OK == pstInstPri->fpVE_DisableRegWrite(pDiableRegWrite->bFlag))
            {
                pDiableRegWrite->eRet = E_TVENCODER_OK;
            }
            else
            {
                pDiableRegWrite->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_AdjustPositionBase:
        {
            PTVE_ADJPOSITIONBASE pAdjPositionBase = (PTVE_ADJPOSITIONBASE)pArgs;
            if(E_VE_OK == pstInstPri->fpVE_AdjustPositionBase(pAdjPositionBase->s32WAddrAdjustment,
                                                     pAdjPositionBase->s32RAddrAdjustment))
            {
                pAdjPositionBase->eRet = E_TVENCODER_OK;
            }
            else
            {
                pAdjPositionBase->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_Customer_Scaling:
        {
            PTVE_SETCUSSCALING pSetCusScaling = (PTVE_SETCUSSCALING)pArgs;
            if(E_VE_OK == pstInstPri->fpVE_Set_Customer_Scaling(pSetCusScaling->pstVECusScalingInfo))
            {
                pSetCusScaling->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetCusScaling->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_InitVECapture:
        {
            P_TVE_INITCAPTURE pInitCapture = (P_TVE_INITCAPTURE)pArgs;
            pstInstPri->fpVE_InitVECapture(pInitCapture->pVECapture);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_EnaVECapture:
        {
            PTVE_ENACAPTURE pEnableCapture =(PTVE_ENACAPTURE)pArgs;
            if(TRUE == pstInstPri->fpVE_EnaVECapture(pEnableCapture->pstVECapState))
            {
                pEnableCapture->eRet = E_TVENCODER_OK;
            }
            else
            {
                pEnableCapture->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Adjust_FrameStart:
        {
            PTVE_ADJFRAMESTART pAdjustFrameStart = (PTVE_ADJFRAMESTART)pArgs;
            if(E_VE_OK == pstInstPri->fpVE_Adjust_FrameStart(pAdjustFrameStart->stSrcWin,
                pAdjustFrameStart->pixel_offset))
            {
                pAdjustFrameStart->eRet = E_TVENCODER_OK;
            }
            else
            {
                pAdjustFrameStart->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_SetPowerState:
        {
            PTVE_SETPOWERSTATE pSetPowerState = (PTVE_SETPOWERSTATE)pArgs;

            if(TRUE == pstInstPri->fpTVE_SetPowerState(pSetPowerState->ePowerState))
            {
                pSetPowerState->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetPowerState->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_CusTable:
        {
            PTVE_SETCUSTABLE pSetCusTable = (PTVE_SETCUSTABLE)pArgs;

            if(TRUE == pstInstPri->fpVE_SetCusTable(pSetCusTable->VideoSystem,pSetCusTable->pTbl))
            {
                pSetCusTable->eRet = E_TVENCODER_OK;
            }
            else
            {
                pSetCusTable->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_EnableCusTable:
        {
            PTVE_ENABLECUSTABLE pSetEnableTable = (PTVE_ENABLECUSTABLE)pArgs;
            pstInstPri->fpVE_EnableCusTable(pSetEnableTable->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_FrameColor:
        {
            PTVE_SETFRAMECOLOR pSetFrameColor = (PTVE_SETFRAMECOLOR)pArgs;
            pSetFrameColor->eRet = pstInstPri->fpVE_SetFrameColor(pSetFrameColor->u32aRGB);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_OSDLayer:
        {
            PTVE_SETOSDLAYER pSetOSDLayer = (PTVE_SETOSDLAYER)pArgs;
            pSetOSDLayer->eRet = pstInstPri->fpVE_Set_OSDLayer(pSetOSDLayer->eVideoOSDLayer);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Get_OSDLayer:
        {
            PTVE_GETOSDLAYER pGetOSDLayer = (PTVE_GETOSDLAYER)pArgs;
            pGetOSDLayer->eRet = pstInstPri->fpVE_Get_OSDLayer();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Set_VideoAlpha:
        {
            PTVE_SETVIDEOALPHA pSetVideoAlpha = (PTVE_SETVIDEOALPHA)pArgs;
            pSetVideoAlpha->eRet = pstInstPri->fpVE_Set_VideoAlpha(pSetVideoAlpha->u8Val);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_Get_VideoAlpha:
        {
            PTVE_GETVIDEOALPHA pGetVideoAlpha = (PTVE_GETVIDEOALPHA)pArgs;
            pGetVideoAlpha->eRet = pstInstPri->fpVE_Get_VideoAlpha(pGetVideoAlpha->pu8Val);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }

        case E_MDRV_CMD_TVE_DCS_SetType:
        {
            PTVE_DCSSETTYPE pDCSSetType= (PTVE_DCSSETTYPE)pArgs;

            if(TRUE == pstInstPri->fpVE_DCS_SetType(pDCSSetType->bEnable,
                                            pDCSSetType->eType))
            {
                pDCSSetType->eRet = E_TVENCODER_OK;
            }
            else
            {
                pDCSSetType->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_MDRV_CMD_TVE_DCS_SetActivationKey:
        {
            PTVE_DCSSETACTIVATIONKEY pDCSSetActivationKey= (PTVE_DCSSETACTIVATIONKEY)pArgs;

            if(TRUE == pstInstPri->fpVE_DCS_SetActivationKey(pDCSSetActivationKey->pu8ActivationKeyTbl,
                                            pDCSSetActivationKey->u8ActivationKeyTblSize))
            {
                pDCSSetActivationKey->eRet = E_TVENCODER_OK;
            }
            else
            {
                pDCSSetActivationKey->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_MDRV_CMD_TVE_ReloadSetting:
        {
            PTVE_RELOADSETTING pReloadSetting = (PTVE_RELOADSETTING)pArgs;
            if(TRUE == pstInstPri->fpVE_ReloadSetting())
            {
                pReloadSetting->eRet = E_TVENCODER_OK;
            }
            else
            {
                pReloadSetting->eRet = E_TVENCODER_FAIL;
            }
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_MDRV_CMD_TVE_GetConfig:
        {
            PTVE_GETCONFIG pGetConfig = (PTVE_GETCONFIG)pArgs;

            UtopiaResourceRelease(pstRes);

            if(TRUE == pstInstPri->fpVE_GetConfig(pInstance, pGetConfig->pMIUAddress))
            {
                pGetConfig->eRet = E_TVENCODER_OK;
            }
            else
            {
                pGetConfig->eRet = E_TVENCODER_FAIL;
            }

            u32Ret = UTOPIA_STATUS_SUCCESS;
            return u32Ret;
            break;
        }

    }
    UtopiaResourceRelease(pstRes);
    return u32Ret; // FIXME: error code
}

MS_U32 TVENCODERClose(void* pInstance)
{
    //UTOPIA_INSTANT* pInstant = *pInstantTmp;
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    UtopiaInstanceDelete(pInstance);
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("leave %s %d\n",__FUNCTION__,__LINE__));
    //utopia_free(pInstant);
    return UTOPIA_STATUS_SUCCESS;
}
