
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include <stdio.h>
#include <string.h>
#else
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#endif

#include "utopia_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "drvMMIO.h"
#include "utopia_driver_id.h"

UTOPIA_PRIVATE* psUtopiaPrivate;

char moduleNames[][40] = {
#define PREFIX(MODULE) "MODULE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char rpoolNames[][40] = {
#define PREFIX(MODULE) "RPOOL_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

char ResourceNames[][40] = {
#define PREFIX(MODULE) "RESOURCE_"#MODULE,
	INCLUDED_MODULE
#undef PREFIX
};

#define PREFIX(MODULE) MS_U32 MODULE_##MODULE##_OPEN = MODULE_##MODULE;
INCLUDED_MODULE
#undef PREFIX

#define PREFIX(MODULE) \
	extern __attribute__((weak)) MS_U32 MODULE##RegisterToUtopia(FUtopiaOpen ModuleType);
INCLUDED_MODULE
#undef PREFIX

#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT) || defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_32BIT)

typedef MS_U32 (*FUResgisterModuleList)(FUtopiaOpen ModuleType);

MS_U32 emptyRegisterToUtopia(FUtopiaOpen ModuleType)
{
    //printu("[utopia info] emptyRegisterToUtopia\n" );
	return 0;
}

//FUResgisterModuleList AAResgisterModule = emptyRegisterToUtopia;


#define PREFIX(MODULE) \
    FUResgisterModuleList MODULE##ResgisterModule = emptyRegisterToUtopia; 
    INCLUDED_MODULE
#undef PREFIX




MS_U32 autoRegisterModule(void)
{
	
#define PREFIX(MODULE) \
	   if (MODULE##RegisterToUtopia != NULL && MODULE_TYPE_##MODULE##_FULL) \
	   { \
		   MODULE##ResgisterModule = MODULE##RegisterToUtopia; \
	   }
	   INCLUDED_MODULE
#undef PREFIX



   return 0;
}


#endif


MS_U32 UtopiaInit()
{
	MS_U32 u32Ret = 0;

	printu("[utopia info] utopia init\n" );
    psUtopiaPrivate = (UTOPIA_PRIVATE*)malloc(sizeof(UTOPIA_PRIVATE));
	memset(psUtopiaPrivate, 0, sizeof(UTOPIA_PRIVATE));
	psUtopiaPrivate->u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO
			, "UtopiaPrivateMutex", MSOS_PROCESS_SHARED);

	MsOS_ObtainMutex(psUtopiaPrivate->u32MutexID, MSOS_WAIT_FOREVER);
#if defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT) || defined(CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_32BIT)
    u32Ret=autoRegisterModule();

#define PREFIX(MODULE) \
	u32Ret |=(*(MODULE##ResgisterModule))((FUtopiaOpen)MODULE_TYPE_##MODULE##_FULL);
	INCLUDED_MODULE
#undef PREFIX



#else
	
#define PREFIX(MODULE) \
	if (MODULE##RegisterToUtopia != NULL && MODULE_TYPE_##MODULE##_FULL) \
	{ \
		u32Ret |= MODULE##RegisterToUtopia((FUtopiaOpen)MODULE_TYPE_##MODULE##_FULL); \
	}
	INCLUDED_MODULE
#undef PREFIX

#endif //CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaCreateMdbNode();
#endif
	MsOS_ReleaseMutex(psUtopiaPrivate->u32MutexID);

	return u32Ret;
}

MS_U32 UtopiaOpen(MS_U32 u32ModuleID, void** ppInstanceTmp
		, MS_U32 u32ModuleVersion, const void* const pAttribute)
{
    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    UTOPIA_INSTANCE** ppInstance = (UTOPIA_INSTANCE**)ppInstanceTmp;


    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == (u32ModuleID & (~KERNEL_MODE)))
        {
            int ret = psUtopiaModule->fpOpen((void**)ppInstance, pAttribute);

            if(MSOS_BRANCH_PREDICTION_UNLIKELY(ret))
            {
                printu("[utopia open error] fail to create instance\n");
				RET_OR_BLOCK(ret);
            }

            (*ppInstance)->psModule = psUtopiaModule;
			(*ppInstance)->u32AppRequireModuleVersion = u32ModuleVersion;
            return ret; /* depend on fpOpen, may not be UTOPIA_STATUS_SUCCESS */
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }
	return UTOPIA_STATUS_FAIL;
}
EXPORT_SYMBOL(UtopiaOpen);

MS_U32 UtopiaIoctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
	UTOPIA_INSTANCE* pInstance = (UTOPIA_INSTANCE*)pInstanceTmp;
	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pInstance == NULL))
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	return TO_INSTANCE_PTR(pInstance)->psModule->fpIoctl(pInstance,
			u32Cmd, pArgs);
}
EXPORT_SYMBOL(UtopiaIoctl);

MS_U32 UtopiaClose(void* pInstantTmp)
{
    UTOPIA_INSTANCE* pInstant = (UTOPIA_INSTANCE*)pInstantTmp;

    UTOPIA_MODULE* psUtopiaModule = psUtopiaPrivate->psModuleHead;
    while(psUtopiaModule != NULL)
    {
        if(psUtopiaModule->u32ModuleID == pInstant->psModule->u32ModuleID)
        {
            return psUtopiaModule->fpClose(pInstant);
        }
        psUtopiaModule = psUtopiaModule->psNext;
    }
	return 0;
}
EXPORT_SYMBOL(UtopiaClose);
