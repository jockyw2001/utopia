#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "MsTypes.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvSAR.h"
#include "drvSAR_v2.h"
#include "drvSAR_private.h"
#include "MsOS.h"

enum
{
    SAR_POOL_ID=0
} eSAR_PoolID;

#ifdef CONFIG_MSTAR_UTPA2K_SAR_ENABLE
void* psResource = NULL;
void* pUtopiaModule = NULL;
void SARRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    SAR_RESOURCE_PRIVATE * pResPri = NULL;
    UtopiaModuleCreate(MODULE_SAR, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)SAROpen, (FUtopiaClose)SARClose, (FUtopiaIOctl)SARIoctl);

    // 2. deal with resource
    UtopiaModuleAddResourceStart(pUtopiaModule, SAR_POOL_ID);
    UtopiaResourceCreate("SAR", sizeof(SAR_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, SAR_POOL_ID);
    UtopiaModuleAddResourceEnd(pUtopiaModule, SAR_POOL_ID);
    if(UtopiaResourceObtain(pUtopiaModule, SAR_POOL_ID, &psResource) != 0)
    {
    	 printf("UtopiaResourceObtainToInstant fail\n");
    }

    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    UtopiaResourceRelease(psResource);
}

MS_U32 SAROpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    SAR_INSTANT_PRIVATE *pSarPri = NULL;
    void *pSarPriVoid = NULL;
    MS_DEBUG_MSG(printf("\n[SAR INFO]SAR open \n"));

    UtopiaInstanceCreate(sizeof(SAR_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pSarPriVoid);

    pSarPri = (SAR_INSTANT_PRIVATE *)pSarPriVoid;
    pSarPri->fpSARKpdGetKeyCode =MDrv_SAR_Kpd_GetKeyCode_U2K;
    pSarPri->fpSARKpdConfig =MDrv_SAR_Kpd_SetChInfo_U2K;
    MDrv_SAR_Kpd_Init();
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 SARIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    SAR_RESOURCE_PRIVATE * pResPri = NULL;
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    MS_U32 u32Ret = 0;


    SAR_PRIVATE_PARAM *param = NULL;

    SAR_INSTANT_PRIVATE* psSARInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psSARInstPri);
    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    if (pArgs != NULL)
        param= (SAR_PRIVATE_PARAM *) pArgs;

    switch(u32Cmd)
    {
    	case MDrv_CMD_Kpd_GetKeyCode:
            u32Ret=psSARInstPri->fpSARKpdGetKeyCode(&param->privateKpd_GetKeyCode.u8Key,
                                                    &param->privateKpd_GetKeyCode.u8Repeat);
						param->privateKpd_GetKeyCode.u32Retn=u32Ret;
            break;
      case MDrv_CMD_SAR_Config:
           pResPri->privateKpd_Config.pKeyRegCfg = param->privateKpd_Config.pKeyRegCfg;
      	   u32Ret = psSARInstPri->fpSARKpdConfig(param->privateKpd_Config.pKeyRegCfg);
      break;
      case MDrv_CMD_SAR_GetConfig:
      	   param->privateKpd_Config.pKeyRegCfg = pResPri->privateKpd_Config.pKeyRegCfg;
      	   u32Ret = UTOPIA_STATUS_SUCCESS;
      break;
      default:
      break;
    };
	return u32Ret;
}

MS_U32 SARClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}
#endif
