#include "MsTypes.h"
#include "utopia_module.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "utopia_private.h"
#include <stdio.h>


MS_U32 _UtopiaModule_Ioctl(void** pInstant, MS_U32 u32Cmd, MS_U32* pu32Args)
{
    switch(u32Cmd)
    {
        case CHANGE_DEBUG_LEVEL_BY_MODULE:
        case HOOK_MODULE_IOCTL_BY_MODULE:
        case HOOK_MODULE_CMD_BY_MODULE:
        case DUMP_MODULE_HW_REGISTER:
        case DUMP_MODULE_STRUCTURE:
        case DUMP_INSTANT_STRUCTURE:
        case DUMP2MEMORY_MODULE_HW_REGISTER:
        case DUMP2MEMORY_MODULE_STRUCTURE:
        case DUMP2MEMORY_INSTANT_STRUCTURE:
        case SHOW_RESOURCE_STATUS_BY_MODULE:
			;
    }
	return 0;
}

extern char moduleNames[][40];
#define UTOPIA_SEM_START 0x4427 // joe's phone number
MS_U32 UTOPIARegisterToUtopia(FUtopiaOpen ModuleType)
{
	MS_U32 u32ShmId, u32Addr, u32BufSize;

	// return 0 for first initiaization, 1 for repeated initialization
	MS_U32 bInitialized = MsOS_SHM_GetId((MS_U8*)"sem_key_start", sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY);

	void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_UTOPIA, 0, &pUtopiaModule);
	UtopiaModuleRegister(pUtopiaModule);

	if (!bInitialized)
	{
		MsOS_SHM_GetId((MS_U8*)"sem_key_start", sizeof(MS_U32), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_CREATE);
		*(MS_U32*)u32Addr = UTOPIA_SEM_START;

        //printf("shm_info_table[%d] = sem_key_start@%p\n", (int)u32ShmId, (void*)u32Addr);
	}
	return 0;
}
