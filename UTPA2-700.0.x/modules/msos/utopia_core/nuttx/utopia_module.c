#include "MsTypes.h"
#include "utopia_module.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "utopia_private.h"
#include <stdio.h>

int lib_lowprintf(const char *fmt, ...);
#define printf lib_lowprintf
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

	//MsOS_Init(); // contain MsOS_SHM_Init()
	//MsOS_SHM_Init();

	// return 0 for first initiaization, 1 for repeated initialization
	MS_U32 bInitialized = MsOS_SHM_GetId((MS_U8*)"MS_MODULE_UTOPIA", sizeof(UTOPIA_MODULE_SHM), &u32ShmId, &u32Addr, &u32BufSize, MSOS_SHM_QUERY);

	void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_UTOPIA, 0, &pUtopiaModule);
	UtopiaModuleRegister(pUtopiaModule);
	//TOPIA_RESOURCE* psResource = (UTOPIA_RESOURCE*)UtopiaResourceCreate("SemStartKey", sizeof(int));
	UTOPIA_RESOURCE* psResource = NULL;
	UtopiaResourceCreate("SemStartKey", sizeof(int), (void**)&psResource);

	if (!bInitialized) 
	{
		//MS_U32* pu32SemKey = (MS_U32*)shmid2va(psResource->shmid_private.ID);
		MS_U32* pu32SemKey = NULL;
		printf("\n%s, %d\n", __FUNCTION__, __LINE__); // bob.fu
		UtopiaResourceGetPrivate(psResource, (void**)&pu32SemKey);
		printf("\n%s, %d, %p\n", __FUNCTION__, __LINE__, pu32SemKey); // bob.fu
		*pu32SemKey = UTOPIA_SEM_START;
	}
	UtopiaResourceRegister(pUtopiaModule, psResource, UTOPIA_POOL_ID_DATA);
	return 1;
}
