#include "string.h"
#include "MsTypes.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "drvCPU.h"
#include "drvCPU_private.h"
#include "MsOS.h"
#if (CPU_UTOPIA20)
#include "utopia.h"
#endif

#include "ULog.h"

#define TAG_CPU "CPU"

enum
{
    CPU_POOL_ID_CPU0=0
} eCPUPoolID;

// this func will be called to init by utopia20 framework
void CPURegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_CPU, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)CPUOpen, (FUtopiaClose)CPUClose, (FUtopiaIOctl)CPUIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare CPU_POOL_ID_CPU1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, CPU_POOL_ID_CPU0);
    // resource can alloc private for internal use, ex, CPU_RESOURCE_PRIVATE
    UtopiaResourceCreate("cpu0", sizeof(CPU_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, CPU_POOL_ID_CPU0);
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, CPU_POOL_ID_CPU0);
}

MS_U32 CPUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    CPU_INSTANT_PRIVATE *psCpuInstPri = NULL;
    void *pCpuInstPriVoid = NULL;

	ULOGD(TAG_CPU, "\n[CPU INFO] cpu open");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, CPU_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(CPU_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pCpuInstPriVoid);
    psCpuInstPri = (CPU_INSTANT_PRIVATE*)pCpuInstPriVoid;
    psCpuInstPri->fpCPUInitFront = (IOCTL_CPU_INITFRONT)_MDrv_CPU_InitFront;
    psCpuInstPri->fpCPUInitEnd = (IOCTL_CPU_INITEND)_MDrv_CPU_InitEnd;
    psCpuInstPri->fpCPUDisable = (IOCTL_CPU_DISABLE)_MDrv_CPU_Disable;
    psCpuInstPri->fpCPUEnable = (IOCTL_CPU_ENABLE)_MDrv_CPU_Enable;
    psCpuInstPri->fpCPUSetHost = (IOCTL_CPU_SETHOST)_MDrv_CPU_SetHost;
    psCpuInstPri->fpCPUGetInfo = (IOCTL_CPU_GETINFO)_MDrv_CPU_GetInfo;
    psCpuInstPri->fpCPUGetLibVer = (IOCTL_CPU_GETLIBVER)_MDrv_CPU_GetLibVer;
    psCpuInstPri->fpCPUGetStatus = (IOCTL_CPU_GETSTATUS)_MDrv_CPU_GetStatus;
    psCpuInstPri->fpCPUSetDbgLevel = (IOCTL_CPU_SETDBGLEVEL)_MDrv_CPU_SetDbgLevel;
    psCpuInstPri->fpCPUGetBase = (IOCTL_CPU_GETBASE)_MDrv_CPU_GetBase;
    psCpuInstPri->fpCPUQueryClock = (IOCTL_CPU_QUERYCLOCK)_MDrv_CPU_QueryClock;

	_MDrv_CPU_GetBase();
	
	return UTOPIA_STATUS_SUCCESS;
}

// FIXME: why static?
MS_U32 CPUIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	void* pResource = NULL;
    CPU_INSTANT_PRIVATE* psCpuInstPri = NULL;
    void* pCpuInstPriVoid = NULL;

    PCPU_ENABLE_PARAM pEnableParam = NULL;
    PCPU_SETHOST_PARAM pSetHostParam = NULL;
    PCPU_INITEND_PARAM pInitEndParam = NULL;
    PCPU_GETLIBVER_PARAM pGetLibVerParam = NULL;
    PCPU_GETSTATUS_PARAM pGetStatusParam = NULL;
    PCPU_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;
    PCPU_GETINFO_PARAM pGetInfoParam = NULL;
    COPRO_DrvInfo* pCPUInfo = NULL;
    COPRO_Result eCPURet=E_COPRO_FAIL;

	UtopiaInstanceGetModule(pInstance, &pModule);
    if(pModule==(void*)NULL)
    {
        ULOGE(TAG_CPU, "UtopiaInstanceGetModule fail\n");
        return UTOPIA_STATUS_FAIL;
    }

	UtopiaInstanceGetPrivate(pInstance, &pCpuInstPriVoid);
    psCpuInstPri = (CPU_INSTANT_PRIVATE*)pCpuInstPriVoid;
    if(psCpuInstPri==(CPU_INSTANT_PRIVATE*)NULL)
    {
        ULOGE(TAG_CPU,"UtopiaInstanceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    
    // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
    switch(u32Cmd)
    {
        case MDrv_CMD_CPU_InitFront:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_InitFront\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psCpuInstPri->fpCPUInitFront();
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_InitEnd:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_InitEnd\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            pInitEndParam = (PCPU_INITEND_PARAM)pArgs;
            if(pInitEndParam==(PCPU_INITEND_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            psCpuInstPri->fpCPUInitEnd(pInitEndParam->u32_ADR);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_Disable:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_Disable\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
	            return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psCpuInstPri->fpCPUDisable();
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_Enable:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_Enable\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            pEnableParam = (PCPU_ENABLE_PARAM)pArgs;
            if(pEnableParam==(PCPU_ENABLE_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            psCpuInstPri->fpCPUEnable(pEnableParam->u32_ADR);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_SetHost:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_SetHost\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            pSetHostParam = (PCPU_SETHOST_PARAM)pArgs;
            if(pSetHostParam==(PCPU_SETHOST_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            psCpuInstPri->fpCPUSetHost(pSetHostParam->bHost);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_GetInfo:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_GetInfo\n");
            pGetInfoParam = (PCPU_GETINFO_PARAM)pArgs;
            if(pGetInfoParam==(PCPU_GETINFO_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            pCPUInfo = (COPRO_DrvInfo*)psCpuInstPri->fpCPUGetInfo();
            if(pCPUInfo==(COPRO_DrvInfo*)NULL)
            {
                return UTOPIA_STATUS_FAIL;
            }
            memcpy((void*)pGetInfoParam,(void*)pCPUInfo,sizeof(COPRO_DrvInfo));
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_GetLibVer:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_GetLibVer\n");
            // this function won't occupy h/w, so don't need to call UtopiaResourceObtainToInstant
            pGetLibVerParam = (PCPU_GETLIBVER_PARAM)pArgs;
            if(pGetLibVerParam==(PCPU_GETLIBVER_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            eCPURet = psCpuInstPri->fpCPUGetLibVer(pGetLibVerParam->ppVersion);
            if (eCPURet == E_COPRO_FAIL)
            {
                return UTOPIA_STATUS_FAIL;
            }
            return UTOPIA_STATUS_SUCCESS;
            
        case MDrv_CMD_CPU_GetStatus:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_GetStatus\n");
            // this function won't occupy h/w, so don't need to call UtopiaResourceObtainToInstant
            pGetStatusParam = (PCPU_GETSTATUS_PARAM)pArgs;
            if(pGetStatusParam==(PCPU_GETSTATUS_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            eCPURet = psCpuInstPri->fpCPUGetStatus(pGetStatusParam->pStatus);
            if (eCPURet == E_COPRO_FAIL)
            {
                return UTOPIA_STATUS_FAIL;
            }
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_SetDbgLevel:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_SetDbgLevel\n");
            // this function won't occupy h/w, so don't need to call UtopiaResourceObtainToInstant
            pSetDbgLevelParam = (PCPU_SETDBGLEVEL_PARAM)pArgs;
            if(pSetDbgLevelParam==(PCPU_SETDBGLEVEL_PARAM)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            psCpuInstPri->fpCPUSetDbgLevel(pSetDbgLevelParam->u8Level);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_GetBase:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_GetBase\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            if(pArgs==(void*)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            *((MS_U32*)pArgs) = psCpuInstPri->fpCPUGetBase();
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_CPU_QueryClock:
            ULOGD(TAG_CPU, "CPUIoctl - MDrv_CMD_CPU_QueryClock\n");
            if(UtopiaResourceObtain(pModule, CPU_POOL_ID_CPU0, &pResource) != 0)
            {
                ULOGE(TAG_CPU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            if(pArgs==(void*)NULL)
            {
                return UTOPIA_STATUS_PARAMETER_ERROR;
            }
            *((MS_U32*)pArgs) = psCpuInstPri->fpCPUQueryClock();
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        default:
            return UTOPIA_STATUS_ERR_UNIMPLEM;
    };

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 CPUClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}



