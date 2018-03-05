

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#else
#include <string.h>
#include <stdio.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "gpd.h"
#include "apiGPD.h"
#include "MsOS.h"

#include "utopia.h"
#include "apiGPD_priv.h"
#include "apiGPD_v2.h"



enum
{
    GPD_POOL_ID_GPD0=0,
    GPD_POOL_ID_GPD_MAX
} eGPDPoolID;

#if (defined CONFIG_MLOG)
#include "ULog.h"

#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ULOGE("API_GPD", format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ULOGI("API_GPD", format, ##args);}while(0)
#else

#ifdef ANDROID
#ifndef LOGD
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) ALOGE(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) ALOGI(format, ##args);}while(0)
#else
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) LOGE(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) LOGI(format, ##args);}while(0)
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printk(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printk(format, ##args);}while(0)
#else
#define GPD_API_ERROR(format, args...)      do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_ERROR) printf(format, ##args);}while(0)
#define GPD_API_INFO(format, args...)       do{if(eEnableDbgMsg & E_GPD_DEBUG_LEVEL_INFO) printf(format, ##args);}while(0)
#endif
#endif

#endif

#define GPD_SUSPEND_TIMEOUT        1000

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
#include "utopia_dapi.h"

MS_U32 GPDMdbIoctl(MS_U32 u32Cmd, const void* const pArgs)
{
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;

    switch(u32Cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            break;
        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            _MApi_GPD_GetMdbInfo(paraGetInfo->u64ReqHdl);
            break;
        default:
            break;
    }
    return 0;
}
#endif

// this func will be call to init by utopia20 framework
void GPDRegisterToUtopia(FUtopiaOpen ModuleType)
{
    GPD_API_INFO("%s(): start\n", __FUNCTION__);
    // 1. deal with module
    void* pModuleGPD_V2 = NULL;
    UtopiaModuleCreate(MODULE_GPD, 0, &pModuleGPD_V2);
    UtopiaModuleRegister(pModuleGPD_V2);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleGPD_V2,
                                    (FUtopiaOpen)GPDOpen,
                                    (FUtopiaClose)GPDClose,
                                    (FUtopiaIOctl)GPDIoctl);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("gpd", (FUtopiaMdbIoctl)GPDMdbIoctl);
#endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModuleGPD_V2,(FUtopiaSTR)GPDStr);
    UtopiaModuleSetSTRPrivate(pModuleGPD_V2, sizeof(GPD_INSTANCE_PRIVATE));
#endif

    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pModuleGPD_V2, GPD_POOL_ID_GPD0);
    UtopiaResourceCreate("GPD", sizeof(GPD_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleGPD_V2, psResource, GPD_POOL_ID_GPD0);
    UtopiaModuleAddResourceEnd(pModuleGPD_V2, GPD_POOL_ID_GPD0);
    GPD_API_INFO("%s(): end\n", __FUNCTION__);

}

MS_U32 GPDOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    GPD_INSTANCE_PRIVATE* psGPD_InstPri = NULL;
    MS_U8 i = 0;
    GPD_API_INFO("%s(): start...\n", __FUNCTION__);


    //Create instance
    UtopiaInstanceCreate(sizeof(GPD_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psGPD_InstPri);

    //Recode decoder index
    psGPD_InstPri->Index_of_decoder = i;

#ifdef UTOPIA_20
    psGPD_InstPri->fpGPDInit = (IOCTL_GPD_INIT)_MApi_GPD_Init;
    psGPD_InstPri->fpGPDInputSource = (IOCTL_GPD_INPUT_SOURCE)_MApi_GPD_InputSource;
    psGPD_InstPri->fpGPDOutputDecode = (IOCTL_GPD_OUTPUT_DECODE)_MApi_GPD_OutputDecode;
    psGPD_InstPri->fpGPDOutputDecodeROI = (IOCTL_GPD_OUTPUT_DECODE_ROI)_MApi_GPD_OutputDecodeROI;
    psGPD_InstPri->fpGPDOutputDecodeMGIF = (IOCTL_GPD_OUTPUT_DECODE_MGIF)_MApi_GPD_OutputDecodeMGIF;
    psGPD_InstPri->fpGPDGetDuration = (IOCTL_GPD_GET_DURATION)_MApi_GPD_GetDuration;
    psGPD_InstPri->fpGPDSetGIFMode = (IOCTL_GPD_SET_GIF_MODE)_MApi_GPD_SetGIFMode;
    psGPD_InstPri->fpGPDScalingEnable = (IOCTL_GPD_SCALING_ENABLE)_MApi_GPD_ScalingEnable;
    psGPD_InstPri->fpGPDScalingDisable = (IOCTL_GPD_SCALING_DISABLE)_MApi_GPD_ScalingDisable;
    psGPD_InstPri->fpGPDSetControl = (IOCTL_GPD_SET_CONTROL)_MApi_GPD_SetControl;
    psGPD_InstPri->fpGPDGetControl = (IOCTL_GPD_GET_CONTROL)_MApi_GPD_GetControl;
    psGPD_InstPri->fpGPDGetCRCResult = (IOCTL_GPD_GET_CRC_RESULT)_MApi_GPD_GetCRCResult;
#endif
    void* pModuleGPD_V2;
    UtopiaModuleGetPtr(MODULE_GPD,&pModuleGPD_V2);

    // Check there is available decoder to be use
    if(UtopiaResourceObtain(pModuleGPD_V2, GPD_POOL_ID_GPD0, &psGPD_InstPri->pResource) != UTOPIA_STATUS_SUCCESS)
    {
        GPD_API_ERROR("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    UtopiaResourceRelease(psGPD_InstPri->pResource);
    GPD_API_INFO("%s(): end\n", __FUNCTION__);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    GPD_INSTANCE_PRIVATE* psGPD_STR_InstPri = NULL;
    UtopiaModuleGetSTRPrivate(pModuleGPD_V2, (void**)&psGPD_STR_InstPri);
    psGPD_STR_InstPri->bGPDInUse = TRUE;
    psGPD_STR_InstPri->bGPDSuspend = FALSE;
#endif

    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 GPDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
#ifdef UTOPIA_20
    GPD_INSTANCE_PRIVATE* psGPD_InstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psGPD_InstPri);
    E_GPD_V2_IOCTL_CMD IOcmd = (E_GPD_V2_IOCTL_CMD)u32Cmd;
    MS_U32 ret = UTOPIA_STATUS_SUCCESS;

    PGPD_INIT_PARAM pInit = NULL;
    PGPD_INPUTSOURCE_PARAM pInputSource = NULL;
    PGPD_OUTPUTDECODE_PARAM pOutputDecode = NULL;
    PGPD_OUTPUTDECODEROI_PARAM pOutputDecodeROI = NULL;
    PGPD_GETDURATION_PARAM pGetDuration = NULL;
    PGPD_SETGIFMODE_PARAM pSetGIFMode = NULL;
    PGPD_SCALINGENABLE_PARAM pScalingEnable = NULL;
    PGPD_CONTROL_PARAM pControl = NULL;
    PGPD_GET_CONTROL_PARAM pGetControl = NULL;
    PGPD_GETCRCRESULT_PARAM pGetCRCResult = NULL;

    switch(IOcmd)
    {
        case MApi_CMD_GPD_Init:
            pInit = (PGPD_INIT_PARAM)pArgs;
            pInit->retVal = psGPD_InstPri->fpGPDInit(pInit->SrcBuf);
            break;
        case MApi_CMD_GPD_InputSource:
            pInputSource = (PGPD_INPUTSOURCE_PARAM)pArgs;
            pInputSource->retVal = psGPD_InstPri->fpGPDInputSource(pInputSource->param, pInputSource->InputBuf, pInputSource->Size);
            break;
        case MApi_CMD_GPD_OutputDecode:
            pOutputDecode = (PGPD_OUTPUTDECODE_PARAM)pArgs;
            pOutputDecode->retVal = psGPD_InstPri->fpGPDOutputDecode(pOutputDecode->DecodeBuf, pOutputDecode->ocolor, pOutputDecode->Size);
            break;
        case MApi_CMD_GPD_OutputDecodeROI:
            pOutputDecodeROI = (PGPD_OUTPUTDECODEROI_PARAM)pArgs;
            pOutputDecodeROI->retVal = psGPD_InstPri->fpGPDOutputDecodeROI(pOutputDecodeROI->DecodeBuf, pOutputDecodeROI->ocolor, pOutputDecodeROI->Size, pOutputDecodeROI->roi);
            break;
        case MApi_CMD_GPD_OutputDecodeMGIF:
            pOutputDecode= (PGPD_OUTPUTDECODE_PARAM)pArgs;
            pOutputDecode->retVal = psGPD_InstPri->fpGPDOutputDecodeMGIF(pOutputDecode->DecodeBuf, pOutputDecode->ocolor, pOutputDecode->Size);
            break;
        case MApi_CMD_GPD_GetDuration:
            pGetDuration = (PGPD_GETDURATION_PARAM)pArgs;
            pGetDuration->retVal = psGPD_InstPri->fpGPDGetDuration();
            break;
        case MApi_CMD_GPD_SetGIFMode:
            pSetGIFMode = (PGPD_SETGIFMODE_PARAM)pArgs;
            psGPD_InstPri->fpGPDSetGIFMode(pSetGIFMode->mode);
            break;
        case MApi_CMD_GPD_ScalingEnable:
            pScalingEnable = (PGPD_SCALINGENABLE_PARAM)pArgs;
            psGPD_InstPri->fpGPDScalingEnable(pScalingEnable->mode);
            break;
        case MApi_CMD_GPD_ScalingDisable:
            psGPD_InstPri->fpGPDScalingDisable();
            break;
        case MApi_CMD_GPD_SetControl:
            pControl = (PGPD_CONTROL_PARAM)pArgs;
            pControl->retVal = psGPD_InstPri->fpGPDSetControl(pControl->cmd_id, pControl->param);
            break;
        case MApi_CMD_GPD_GetControl:
            pGetControl = (PGPD_GET_CONTROL_PARAM)pArgs;
            pGetControl->retVal = psGPD_InstPri->fpGPDGetControl(pGetControl->cmd_id, pGetControl->param);
            break;
        case MApi_CMD_GPD_GetCRCResult:
            pGetCRCResult = (PGPD_GETCRCRESULT_PARAM)pArgs;
            pGetCRCResult->retVal = psGPD_InstPri->fpGPDGetCRCResult(pGetCRCResult->pu8Data, pGetCRCResult->u32Size);
            break;

        default:
            ret = UTOPIA_STATUS_NOT_SUPPORTED;
            GPD_API_ERROR("Error Command = %td\n",(ptrdiff_t)u32Cmd);

            break;

    }
    return ret;
#else
    return UTOPIA_STATUS_SUCCESS;
#endif
}
MS_U32 GPDClose(void* pInstance)
{
    GPD_RESOURCE_PRIVATE* pGPD_ResourcePrivate = NULL;
    GPD_INSTANCE_PRIVATE* psGPD_InstPri = NULL;
    GPD_API_INFO("%s(): start\n", __FUNCTION__);
    void* pModuleGPD_V2 = NULL;
    UtopiaModuleGetPtr(MODULE_GPD, &pModuleGPD_V2);

    // Get instance private
    UtopiaInstanceGetPrivate(pInstance, (void**)&psGPD_InstPri);


    UtopiaResourceGetPrivate(psGPD_InstPri->pResource,(void*)(&pGPD_ResourcePrivate));

    // Set using decoder index to 0
    pGPD_ResourcePrivate->u8DecoderInUse[psGPD_InstPri->Index_of_decoder] = 0;

    UtopiaResourceRelease(psGPD_InstPri->pResource);

    UtopiaInstanceDelete(pInstance);
    GPD_API_INFO("%s(): end\n", __FUNCTION__);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    GPD_INSTANCE_PRIVATE* psGPD_STR_InstPri = NULL;
    UtopiaModuleGetSTRPrivate(pModuleGPD_V2, (void**)&psGPD_STR_InstPri);
    psGPD_STR_InstPri->bGPDInUse = FALSE;
    psGPD_STR_InstPri->bGPDSuspend = FALSE;
#endif

    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 GPDStr(MS_U32 u32PowerState, void* pModule)
{
#ifdef UTOPIA_20
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

#if defined(MSOS_TYPE_LINUX_KERNEL)
    GPD_INSTANCE_PRIVATE* psGPD_STR_InstPri = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&psGPD_STR_InstPri);

    if (u32PowerState == E_POWER_SUSPEND)
    {
        MS_U32 u32Time=MsOS_GetSystemTime();
        while(1)
        {
            if(psGPD_STR_InstPri->bGPDInUse==FALSE)
            {
                psGPD_STR_InstPri->bGPDSuspend = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }
            if((MsOS_GetSystemTime() - u32Time) >= GPD_SUSPEND_TIMEOUT)
            {
                GPD_API_INFO("Suspend request Timeout!!!, force suspend()\n");
                psGPD_STR_InstPri->bGPDSuspend = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }
            else if(u32Time > MsOS_GetSystemTime())  // For the overflow case
            {
                u32Time = 0;
            }
            MsOS_DelayTask(1);
        }
        /* Please Implement Module Suspend Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        if(psGPD_STR_InstPri->bGPDSuspend == FALSE)
        {
            GPD_API_INFO("Resume fail!!! Not in suspend state!!!\n");
            u32Return = UTOPIA_STATUS_FAIL;
        }
        psGPD_STR_InstPri->bGPDSuspend = FALSE;
        /* Please Implement Module Resume Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
    }
#endif
    return u32Return;// for success
#else
    return UTOPIA_STATUS_SUCCESS;
#endif
}
