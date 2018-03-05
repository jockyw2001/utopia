#include "MsTypes.h"
#include <stdio.h>
#include "utopia_dapi.h"
#include "drvGOPSCD.h"
#include "apiGOPSC_Ex.h"
#include "apiGOPSCD_v2.h"
#include "apiGOPSCD_private.h"
#include "MsOS.h"
#include "utopia.h"


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX_KERNEL

#include <stdio.h>
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------


//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// this func will be call to init by utopia20 framework
void GOPSCDRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_GOPSCD, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)GOPSCDOpen, (FUtopiaClose)GOPSCDClose, (FUtopiaIOctl)GOPSCDIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, GOPSCD_POOL);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("gopscd", sizeof(GOPSCD_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, GOPSCD_POOL);

	UtopiaModuleAddResourceEnd(pUtopiaModule, GOPSCD_POOL);


}

MS_U32 GOPSCDOpen(void** ppInstance, const void* const pAttribute)
{
    GOPSCD_INSTANT_PRIVATE *pGopSCDPri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(GOPSCD_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pGopSCDPri);

    pGopSCDPri->fpGOPSCDInit            = (IOCTL_GOPSCD_INIT)MApi_GOPSCD_Init_U2;
    pGopSCDPri->fpGOPSCDScaling     = (IOCTL_GOPSCD_SCALING)MApi_GOPSCD_Scaling_U2;
    pGopSCDPri->fpGOPSCDSetInfo     = (IOCTL_GOPSCD_SET_INFO)MApi_GOPSC_SetGOPSCInfo_U2;
    pGopSCDPri->fpGOPSCDGetInfo     = (IOCTL_GOPSCD_GET_INFO)MApi_GOPSC_GetGOPSCInfo_U2;
    pGopSCDPri->fpGOPSCDSetHVSP          = (IOCTL_GOPSCD_SET_HVSPSIZE)MApi_GOPSC_SetHVSPSize_U2;
    pGopSCDPri->fpGOPSCDSetSkipPixel          = (IOCTL_GOPSCD_SET_SKIPPIXEL)MApi_GOPSC_SetSkipPixel_U2;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GOPSCDIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    MS_U32 u32Ret = En_GOPSC_OK;
    GOPSCD_INSTANT_PRIVATE* psGOPSCDInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psGOPSCDInstPri);

    if(UtopiaResourceObtain(pModule, GOPSCD_POOL, &g_pGOPSCDResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    //printf("[%s] cmd:%lx\n",__FUNCTION__,u32Cmd);
    switch(u32Cmd)
    {
        case MAPI_CMD_GOPSCD_INIT:
        {
            PGOPSCD_INIT_INFO ptr = (PGOPSCD_INIT_INFO)pArgs;
            ptr->bReturnValue = psGOPSCDInstPri->fpGOPSCDInit(pInstance);
            break;
        }
        case MAPI_CMD_GOPSCD_SCALING:
        {
            PGOPSCD_SCALING ptr = (PGOPSCD_SCALING)pArgs;
            ptr->bReturnValue = psGOPSCDInstPri->fpGOPSCDScaling(pInstance,ptr->gopscCmd,ptr->u8FrameCount);
            break;

        }
        case MAPI_CMD_GOPSCD_SET_INFO:
        {
            PGOPSCD_SET_INFO ptr = (PGOPSCD_SET_INFO)pArgs;
            ptr->bReturnValue = psGOPSCDInstPri->fpGOPSCDSetInfo(pInstance,ptr->gopscInfo);

            break;

        }
        case MAPI_CMD_GOPSCD_GET_INFO:
        {
            PGOPSCD_GET_INFO ptr = (PGOPSCD_GET_INFO)pArgs;
            ptr->bReturnValue = psGOPSCDInstPri->fpGOPSCDGetInfo(pInstance,ptr->gopscInfo);

            break;
        }
        case MAPI_CMD_GOPSCD_SET_HVSPSIZE:
        {
            PGOPSCD_SET_HVSPSZIE ptr = (PGOPSCD_SET_HVSPSZIE)pArgs;
            ptr->bReturnValue = psGOPSCDInstPri->fpGOPSCDSetHVSP(pInstance,ptr->sizeInfo,ptr->flag);

            break;
        }
        case MAPI_CMD_GOPSCD_SET_SKIPPIXEL:
        {
            PGOPSCD_SET_SKIPPIXEL ptr = (PGOPSCD_SET_SKIPPIXEL)pArgs;
            ptr->bReturnValue = psGOPSCDInstPri->fpGOPSCDSetSkipPixel(pInstance,ptr->skippixelInfo,ptr->flag);

            break;
        }
        default:
            break;
    };

    UtopiaResourceRelease(g_pGOPSCDResource);

    if(u32Ret != En_GOPSC_OK)
    {
        printf("[%s] ERROR on cmd:%d\n",__FUNCTION__,u32Cmd);
    }

    //printf("(%s) Done\n\n",__FUNCTION__);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 GOPSCDClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


