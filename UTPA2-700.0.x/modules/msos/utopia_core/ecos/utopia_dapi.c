#include "MsTypes.h"
#include "utopia_private.h"
#include <stdlib.h>
#include "MsOS.h"
#include <stdio.h>
#include "utopia.h"
#include <string.h>
#include "utopia_dapi.h"

#define PRINT_DEBUG_INFO    0
#if PRINT_DEBUG_INFO
#define PRINT_INFO(fmt, args...)  printf("[Utopia Info]" fmt, ## args)
#else
#define PRINT_INFO(fmt, args...)
#endif

// length should be the same to definition
extern char moduleNames[][40];
extern char rpoolNames[][40];
extern char ResourceNames[][40];

struct shm_info {
	void* va;
	char name[40]; // FIXME: array length should be unify
};

/*
 * could be equal to MAX_CLIENT_NUM=32 defined in MsOS_eCos.c
 * cause we ues shm id as table index, there may be holes in the table
 */
#define SHM_INFO_TABLE_LENGTH 300
struct shm_info shm_info_table[SHM_INFO_TABLE_LENGTH] = {{NULL, ""}};

extern UTOPIA_PRIVATE* psUtopiaPrivate;

//############################
//## Static Functions Start ##
//############################
enum eObjectType
{
	E_TYPE_INSTANCE,
	E_TYPE_MODULE,
	E_TYPE_MODULE_SHM,
	E_TYPE_RESOURCE_POOL,
	E_TYPE_RESOURCE,
};

static void obtain_sem(MS_U32 u32SemID)
{
    /* Perform down operation */
	MsOS_ObtainSemaphore(u32SemID, MSOS_WAIT_FOREVER);
}

static void release_sem(MS_U32 u32SemID)
{
    /* Perform down operation */
	MsOS_ReleaseSemaphore(u32SemID);
}


static inline void* shmid2va(MS_U32 u32ShmID)
{
	if (u32ShmID == 0xFFFFFFFF)
		return NULL;
	return shm_info_table[u32ShmID].va;
}

static inline int isFirstCreation(void* pUnknown, enum eObjectType eObjectType)
{
	switch(eObjectType)
	{
		case E_TYPE_MODULE_SHM:
			return TO_MODULE_SHM_PTR(pUnknown)->shmid_self.ID == 0;
		case E_TYPE_RESOURCE:
			return TO_RESOURCE_PTR(pUnknown)->shmid_self.ID == 0;
		case E_TYPE_RESOURCE_POOL:
			return TO_RPOOL_PTR(pUnknown)->shmid_self.ID == 0;
		default:
			printf("[utopia error] type is not supported\n");
			RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
}

/*
 * u32ShmSize: user specified shared memory size
 * shmName: user specified shared memory name
 * p32RetShmID: for returning shared memory id
 * return: shared memory address in virtual
 */
static void* shm_malloc(MS_U32 u32ShmSize, char* shmName, MS_U32* pu32RetShmID)
{
    MS_U32 u32RetShmID=0, u32RetAddr=0, u32RetShmSize=0;
	MS_U32 u32Ret;

	/* check param. */
	if (strcmp(shmName, "") == 0)
	{
		printf("shmem name string should not be empty\n");
		while(1);
	}

	/*
	 * u32Ret: 0 for not found
	 *         1 for existence
	 * u32RetShmID: 0 for not found(original value)
	 *              n for created share mem id, start from 1
	 */
	u32Ret = MsOS_SHM_GetId((MS_U8*)shmName, u32ShmSize, &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_QUERY);

	/*
	 * MsOS_SHM_GetId need to be called for each process
	 * to set up per-process shmid2va table
	 * it's ok for duplicated calls with same shmName
	 */
	MsOS_SHM_GetId((MS_U8*)shmName, u32ShmSize, &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_CREATE);

	/*
	 * check whether table limit is reached?
	 */
    // shm_info_table is for the shm_id and shm_addr mapping
	if(u32RetShmID >= SHM_INFO_TABLE_LENGTH)
	{
		printf("[%s error] returned shm id %d exceeds shm-info table length %d\n"
				, __FUNCTION__, (int)u32RetShmID, SHM_INFO_TABLE_LENGTH);
		while(1);
	}
	if(shm_info_table[u32RetShmID].va != NULL)
	{
		printf("error: register duplicated shared memory ID %d: %s@%p\n"
		, (int)u32RetShmID, shm_info_table[u32RetShmID].name, shm_info_table[u32RetShmID].va);
		while(1);
	}
    else
    {
		shm_info_table[u32RetShmID].va = (void*)u32RetAddr;
		strncpy(shm_info_table[u32RetShmID].name, shmName, sizeof(shm_info_table[u32RetShmID].name)); // FIXME: potential bug
        if(!(0 == strcmp(shm_info_table[u32RetShmID].name, shmName)))
        {
            printf("\033[35mFunction = %s, Line = %d, error, strncpy failed!!\033[m\n", __FUNCTION__, __LINE__);
            while(1);
        }
		PRINT_INFO("shm_info_table[%d] = %s@%p\n", (int)u32RetShmID, shmName, (void*)u32RetAddr);
	}

	// u32Ret: TRUE for success, FALSE for failure
	// if failure --> first time allocation, memset to 0
	if(u32Ret != 1)
		memset((void*)u32RetAddr, 0, u32RetShmSize);

	*pu32RetShmID = u32RetShmID;
	return (void*)u32RetAddr;
}

static UTOPIA_RESOURCE* next_resource(void* pUnkown, enum eObjectType eObjectType)
{
	switch(eObjectType)
	{
		case E_TYPE_RESOURCE_POOL:
			return (UTOPIA_RESOURCE*)shmid2va(((UTOPIA_RESOURCE_POOL*)pUnkown)->shmid_resource_head.ID);
		case E_TYPE_RESOURCE:
			return (UTOPIA_RESOURCE*)shmid2va(((UTOPIA_RESOURCE*)pUnkown)->shmid_next_resource.ID);
		default:
			printf("%s, %d, %s, [utopia error] type is not supported\n", __FUNCTION__, __LINE__, __FILE__);
			RET_OR_BLOCK(NULL);
	}
}

/* return located module address or NULL if not found */
static void* locate_module(MS_U32 u32ModuleID)
{
	UTOPIA_MODULE* pLocatedModule = psUtopiaPrivate->psModuleHead;

	while (pLocatedModule != NULL && pLocatedModule->u32ModuleID != u32ModuleID)
		pLocatedModule = pLocatedModule->psNext;

	return pLocatedModule;
}

// case 1: there's no resource pool --> *ppRPool = NULL & *ppRPoolPrev = NULL
// case 2: at least one resource pool exists, but not the located one --> *ppRPool = NULL & *ppRPoolPrev != NULL
// case 3: at least one resource pool exists, and so is the located one --> *ppRPool != NULL & ppRPoolPrev doesn't matter
static int locate_resource_pool(UTOPIA_MODULE* pModule, MS_U32 u32LocatedRPoolID
		, UTOPIA_RESOURCE_POOL** ppRPool, UTOPIA_RESOURCE_POOL** ppRPoolPrev)
{
	UTOPIA_RESOURCE_POOL* pRPool = TO_RPOOL_PTR(shmid2va(pModule->psModuleShm->shmid_rpool_head.ID));
	UTOPIA_RESOURCE_POOL* pRPoolPrev = NULL;

	while (pRPool != NULL && pRPool->u32PoolID != u32LocatedRPoolID)
	{
		pRPoolPrev = pRPool;
		pRPool = TO_RPOOL_PTR(shmid2va(pRPool->shmid_next_rpool.ID));
	}

	*ppRPool = pRPool;
	*ppRPoolPrev = pRPoolPrev;
	return 0;
}

static int locate_resource(UTOPIA_RESOURCE_POOL* pRPool, MS_U32 u32LocatedResourceID
		, UTOPIA_RESOURCE** ppResource, UTOPIA_RESOURCE** ppResourcePrev)
{
	UTOPIA_RESOURCE* pResource = TO_RESOURCE_PTR(shmid2va(pRPool->shmid_resource_head.ID));
	UTOPIA_RESOURCE* pResourcePrev = NULL;

	while (pResource != NULL && (pResource->shmid_self.ID != u32LocatedResourceID))
	{
		pResourcePrev = pResource;
		pResource = TO_RESOURCE_PTR(shmid2va(pResource->shmid_next_resource.ID));
	}

	*ppResource = pResource;
	*ppResourcePrev = pResourcePrev;
	return 0;
}

int check_rpool_semaphore(MS_U32 u32ModuleID)
{
	/* locate target module in stupid way */
	UTOPIA_MODULE* pModule = NULL;
	pModule = psUtopiaPrivate->psModuleHead;
	while (pModule != NULL && pModule->u32ModuleID != u32ModuleID)
	{
		pModule = pModule->psNext;
	}

	/* locate corresponding rpool in stupid way */
	UTOPIA_RESOURCE_POOL* pRPool = NULL;
	if (pModule != NULL) /* found */
	{
		pRPool = TO_RPOOL_PTR(shmid2va(pModule->psModuleShm->shmid_rpool_head.ID));
	}
    else
    {
		printf("[utopia error] module of u32ModuleID isn't found\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	/* check rpool semaphore one by one */
	while (pRPool != NULL)
	{
		if (!(pRPool->u32ResourcesSemaphore))
		{
			printf("[utopia error] resource pool %s has no semaphore\n", pRPool->shmid_self.name);
			RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
		}
		pRPool = TO_RPOOL_PTR(shmid2va(pRPool->shmid_next_rpool.ID));
	}

	return 0;
}
//##########################
//## Static Functions End ##
//##########################

//#############################
//## Instance Functions Start ##
//#############################
MS_U32 UtopiaInstanceCreate(MS_U32 u32PrivateSize, void** ppInstance)
{
    UTOPIA_INSTANCE* pInstance = NULL;

	/* check param. */
	if (ppInstance == NULL)
	{
		printf("[utopia error] instance ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

    pInstance = malloc(sizeof(UTOPIA_INSTANCE));
    memset(pInstance, 0, sizeof(UTOPIA_INSTANCE));
    pInstance->pPrivate = malloc(u32PrivateSize);
    memset(pInstance->pPrivate, 0, u32PrivateSize);
	PRINT_INFO("create instance at %p with private size %d bytes at %p\n" \
			, pInstance, (int)u32PrivateSize, pInstance->pPrivate);

	*ppInstance = pInstance;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceDelete(void* pInstance)
{
	free(TO_INSTANCE_PTR(pInstance)->pPrivate);
	free(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceGetPrivate(void* pInstance, void** ppPrivate)
{
	/* check param. */
	if (pInstance == NULL)
	{
		printf("[utopia error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
	if (ppPrivate == NULL)
	{
		printf("[utopia error] private ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	*ppPrivate = TO_INSTANCE_PTR(pInstance)->pPrivate;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceGetModule(void* pInstance, void** ppModule)
{
	/* check param. */
	if (pInstance == NULL)
	{
		printf("[uopia error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
	if (ppModule == NULL)
	{
		printf("[utopia error] module ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	*ppModule = TO_INSTANCE_PTR(pInstance)->psModule;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceGetModuleID(void* pInstance, MS_U32* pu32ModuleID)
{
	*pu32ModuleID = TO_INSTANCE_PTR(pInstance)->psModule->u32ModuleID;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceGetModuleVersion(void* pInstance, MS_U32* pu32Version)
{
	*pu32Version = TO_INSTANCE_PTR(pInstance)->psModule->u32Version;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceGetAppRequiredModuleVersion(void* pInstance, MS_U32* pu32ModuleVersion)
{
	*pu32ModuleVersion = TO_INSTANCE_PTR(pInstance)->u32AppRequireModuleVersion;
	return UTOPIA_STATUS_SUCCESS;
}
//###########################
//## Instant Functions End ##
//###########################

//############################
//## Module Functions Start ##
//############################
// assume one module for each driver, otherwise they have to pass module name as parameter
MS_U32 UtopiaModuleCreate(MS_U32 u32ModuleID, MS_U32 u32PrivateSize, void** ppModule)
{
	MS_U32 u32ShmID;
	UTOPIA_MODULE_SHM* pModuleShm = NULL;
	UTOPIA_MODULE* pModule = NULL;
	void* pPrivate = NULL;
	char privateName[50]; // FIXME: potential bug

	/* 1. create module@shm */
	pModuleShm = shm_malloc(sizeof(UTOPIA_MODULE_SHM), moduleNames[u32ModuleID], &u32ShmID);
	if (isFirstCreation(pModuleShm, E_TYPE_MODULE_SHM))
	{
		pModuleShm->shmid_self.ID = u32ShmID;
		strcpy(pModuleShm->shmid_self.name, moduleNames[u32ModuleID]);
		pModuleShm->shmid_rpool_head.ID = 0xFFFFFFFF;
	}
	PRINT_INFO("create module@shm %s at %p\n", moduleNames[u32ModuleID], pModuleShm);

	/* 2. create module */
	pModule = malloc(sizeof(UTOPIA_MODULE));
    memset(pModule, 0, sizeof(UTOPIA_MODULE));
	pModule->u32ModuleID = u32ModuleID;
	pModule->psModuleShm = pModuleShm;
	PRINT_INFO("create module %s at %p\n", moduleNames[u32ModuleID], pModule);

	/* 3. create private of module */
	sprintf(privateName, "%s_PRI", moduleNames[u32ModuleID]); // FIXME: potential bug
	pPrivate = shm_malloc(u32PrivateSize, privateName, &(pModule->shmid_private.ID));
	PRINT_INFO("create private of module %s at %p\n", privateName, pPrivate);

	*ppModule = pModule;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleGetPrivate(void* pModule, void** ppPrivate)
{
	*ppPrivate = shmid2va(TO_MODULE_PTR(pModule)->shmid_private.ID);
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32  UtopiaModuleRegister(void* pModuleTmp)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_MODULE* pLocatedModule = NULL;

	/* check para. */
	if (pModule == NULL)
	{
		printf("module pointer can't be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	pLocatedModule = TO_MODULE_PTR(locate_module(pModule->u32ModuleID));
	if (pLocatedModule == NULL) /* module is not yet registered */
	{
		/* insert module into list head */
		PRINT_INFO("register module: %s\n", moduleNames[pModule->u32ModuleID]);
		pModule->psNext = psUtopiaPrivate->psModuleHead;
		psUtopiaPrivate->psModuleHead = pModule;
		return UTOPIA_STATUS_SUCCESS;
	}
	else  /* module is already registered */
    {
        printf("error: register duplicated module: %s\n", moduleNames[pModule->u32ModuleID]);
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
}

MS_U32 UtopiaModuleSetupFunctionPtr(void* pModuleTmp, FUtopiaOpen fpOpen, FUtopiaClose fpClose, FUtopiaIOctl fpIoctl)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

	PRINT_INFO("setup function pointers of module %s\n", moduleNames[pModule->u32ModuleID]);
	pModule->fpOpen = fpOpen;
	pModule->fpClose = fpClose;
	pModule->fpIoctl = fpIoctl;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleSetVersion(void* pModule, MS_U32 u32Version)
{
	TO_MODULE_PTR(pModule)->u32Version = u32Version;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleGetDebugLevel(void* pInstance, MS_U32* pu32DebugLevel)
{
	*pu32DebugLevel = TO_INSTANCE_PTR(pInstance)->psModule->u32DeviceDebugLevel;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleGetPtr(MS_U32 u32ModuleID, void** ppModule)
{
	*ppModule = locate_module(u32ModuleID);

	return UTOPIA_STATUS_SUCCESS;
}
//##########################
//## Module Functions End ##
//##########################

//##############################
//## Resource Functions Start ##
//##############################
MS_U32 UtopiaResourceCreate(char* resourceName, MS_U32 u32PrivateSize, void** ppResource)
{
	MS_U32 u32RetShmID;
	UTOPIA_RESOURCE* pResource = NULL;
	char privateName[50]; // FIXME: potential bug

	/* 1. create resource */
	PRINT_INFO("create resource %s\n", resourceName);
	pResource = shm_malloc(sizeof(UTOPIA_RESOURCE), resourceName, &(u32RetShmID));
	if (isFirstCreation(pResource, E_TYPE_RESOURCE))
	{
		pResource->shmid_self.ID = u32RetShmID;
		pResource->shmid_next_resource.ID = 0xFFFFFFFF;
	}
	*ppResource = pResource;

	/* 2. create resource private */
	sprintf(privateName, "%s_PRI", resourceName); // FIXME: potential bug
	PRINT_INFO("create private %s of resource %s\n", privateName, resourceName);
	shm_malloc(u32PrivateSize, privateName, &(pResource->shmid_private.ID));

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaResourceGetPrivate(void* pResource, void** ppPrivate)
{
	*ppPrivate = shmid2va(TO_RESOURCE_PTR(pResource)->shmid_private.ID);
	return 0;
}

// case 1: no resource pool -> create resource pool & attach resource to it. combine with case 2
// case 2: resource pool exists, but no mathcing resource pool -> create resource pool & attach resource to it. combine with case 1
// case 3: resource pool exists, and there's matching resource pool -> attach resource to it
MS_U32 UtopiaResourceRegister(void* pModuleTmp, void* pResourceTmp, MS_U32 u32RPoolID)
{
    //lock(u32ResourcePoolsMutex)
    //insert psResouce into psModuleNode->psResourcePoolHeads->psResourceHeads->psResource

	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_RESOURCE* pResource = TO_RESOURCE_PTR(pResourceTmp);
	char rpoolName[50];

	/* 1. deal with resource pool */
	UTOPIA_RESOURCE_POOL* pRPool = NULL;
	UTOPIA_RESOURCE_POOL* pRPoolPrev = NULL;
	locate_resource_pool(pModule, u32RPoolID, &pRPool, &pRPoolPrev);

	/* rpool semaphore has been created */
	if (pRPool != NULL && pRPool->u32ResourcesSemaphore != 0) // if rpool semaphore has been created
	{
		printf("%s has already created semaphroe for %s\n", moduleNames[pModule->u32ModuleID], pRPool->shmid_self.name);
		return UTOPIA_STATUS_FAIL;
	}

	// case 1: there's no resource pool --> *ppRPool = NULL & *ppRPoolPrev = NULL
	// case 2: at least one resource pool exists, but not the located one --> *ppRPool = NULL & *ppRPoolPrev != NULL
	// case 3: at least one resource pool exists, and so is the located one --> *ppRPool != NULL & ppRPoolPrev doesn't matter
	if (pRPool == NULL) { // case 1 or 2
		MS_U32 u32RetRPoolShmID;
		MS_U32 u32ModuleID = pModule->u32ModuleID;

		sprintf(rpoolName, "%s_%d", rpoolNames[u32ModuleID], (int)u32RPoolID);
		pRPool = shm_malloc(sizeof(UTOPIA_RESOURCE_POOL), rpoolName, &u32RetRPoolShmID);
		if (isFirstCreation(pRPool, E_TYPE_RESOURCE_POOL))
		{
			pRPool->shmid_self.ID = u32RetRPoolShmID;
			strncpy(pRPool->shmid_self.name, rpoolName, sizeof(pRPool->shmid_self.name)); // FIXME: potential bug
            if(!(0 == strcmp(pRPool->shmid_self.name, rpoolName)))
            {
                printf("\033[35mFunction = %s, Line = %d, error, strncpy failed!!\033[m\n", __FUNCTION__, __LINE__);
                while(1);
            }
			pRPool->u32PoolID = u32RPoolID;
			pRPool->shmid_next_rpool.ID = 0xFFFFFFFF;
			pRPool->shmid_resource_head.ID = 0xFFFFFFFF;
			pRPool->u32MutexID = MsOS_CreateMutex(E_MSOS_FIFO, rpoolName, MSOS_PROCESS_SHARED);
		}
		PRINT_INFO("create resource pool %s at %p\n", rpoolName, pRPool);
		/* check param. */
		if (pResource->shmid_rpool.ID != 0)
		{
			return UTOPIA_STATUS_SUCCESS;
		}

		/* set up connection */
		if (pRPoolPrev == NULL) // case 1
		{
			pModule->psModuleShm->shmid_rpool_head.ID = u32RetRPoolShmID;
		}
		else /* case 2 */
		{
			pRPoolPrev->shmid_next_rpool.ID = u32RetRPoolShmID;
		}

		pModule->psModuleShm->u32ResourcePoolTotal++;
	}

	/* 2. deal with resource */
	pResource->shmid_rpool = pRPool->shmid_self;

	UTOPIA_RESOURCE* pLocatedResource = NULL;
	UTOPIA_RESOURCE* pLocatedResourcePrev = NULL;
	locate_resource(pRPool, pResource->shmid_self.ID, &pLocatedResource, &pLocatedResourcePrev);
	// case 1: there's no resource in pool --> pLocatedResource = NULL, pLocatedResourcePrev = NULL
	// case 2: at least one resource exists, but not the located one --> pLocatedResource = NULL, pLocatedResourcePrev != NULL
	// case 3: at least one resource exists, and so is the located one --> pLocatedResource != NULL, pLocatedResourcePrev doesn't matter
	if (pLocatedResource == NULL) // case 1 or 2
	{
		PRINT_INFO("register resource into pool %s\n", rpoolName);
		if (pLocatedResourcePrev == NULL) // case 1
		{
			pRPool->shmid_resource_head.ID = pResource->shmid_self.ID;
		}
		else /* case 2 */
		{
			pLocatedResourcePrev->shmid_next_resource.ID = pResource->shmid_self.ID;
		}
		pRPool->u32ResourceTotal++;

		return UTOPIA_STATUS_SUCCESS;
	}
	else /* case 3 */
	{
		/* duplicated registration: it may be registered by other process */
		return UTOPIA_STATUS_FAIL;
	}
}

MS_U32 UtopiaResourceTryObtain(void* pModuleTmp, MS_U32 u32RPoolID, void** ppResource)
{
    //Search and get one resource from psModuleNode->psResourcePoolHeads
    //set flag bit0 to 1 for using

	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	UTOPIA_RESOURCE* pAvailResource = NULL;
	locate_resource_pool(pModule, u32RPoolID, &pLocatedRPool, &pLocatedRPoolPrev);

	/* to avoid obtaining resource before UtopiaModuleAddResourceEnd() */
	//check_rpool_semaphore(pModule->u32ModuleID);
	if (!(pLocatedRPool->u32ResourceAvail))
		return UTOPIA_STATUS_FAIL;
	obtain_sem(pLocatedRPool->u32ResourcesSemaphore);

	pAvailResource = next_resource(pLocatedRPool, E_TYPE_RESOURCE_POOL);
	MsOS_ObtainMutex(pLocatedRPool->u32MutexID, MSOS_WAIT_FOREVER);
	while (pAvailResource != NULL)
	{
		if (!(pAvailResource->bInUse))
		{
			pAvailResource->bInUse = true;
			*ppResource	= pAvailResource;
			pLocatedRPool->u32ResourceAvail--;
			MsOS_ReleaseMutex(pLocatedRPool->u32MutexID);
			return UTOPIA_STATUS_SUCCESS;
		}
		else
		{
			pAvailResource = next_resource(pAvailResource, E_TYPE_RESOURCE);
		}
	}
	MsOS_ReleaseMutex(pLocatedRPool->u32MutexID);

	return UTOPIA_STATUS_FAIL;
}

MS_U32 UtopiaResourceObtain(void* pModTmp, MS_U32 u32RPoolID, void** ppResource)
{
    //Search and get one resource from psModuleNode->psResourcePoolHeads
    //set flag bit0 to 1 for using

	/* check param. */
	if (pModTmp == NULL)
	{
		printf("module pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
	if (ppResource == NULL)
	{
		printf("resource ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModTmp);
	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	UTOPIA_RESOURCE* pAvailResource = NULL;
	locate_resource_pool(pModule, u32RPoolID
			, &pLocatedRPool, &pLocatedRPoolPrev);

	// to avoid obtaining resource before UtopiaModuleAddResourceEnd()
	PRINT_INFO("%s try to obtain %s semaphore\n", moduleNames[pModule->u32ModuleID], pLocatedRPool->shmid_self.name);
	//check_rpool_semaphore(pModule->u32ModuleID);
	obtain_sem(pLocatedRPool->u32ResourcesSemaphore);

	pAvailResource = next_resource(pLocatedRPool, E_TYPE_RESOURCE_POOL);
	MsOS_ObtainMutex(pLocatedRPool->u32MutexID, MSOS_WAIT_FOREVER);
	while (pAvailResource != NULL)
	{
		if (!(pAvailResource->bInUse))
		{
			pAvailResource->bInUse = true;
			*ppResource	= pAvailResource;
			pLocatedRPool->u32ResourceAvail--;
			MsOS_ReleaseMutex(pLocatedRPool->u32MutexID);
			return UTOPIA_STATUS_SUCCESS;
		}
		else
		{
			pAvailResource = next_resource(pAvailResource, E_TYPE_RESOURCE);
		}
	}
	MsOS_ReleaseMutex(pLocatedRPool->u32MutexID);

	return UTOPIA_STATUS_FAIL;
}

MS_U32 UtopiaResourceRelease(void* pResource)
{
	UTOPIA_RESOURCE_POOL* pRPool = RESOURCE_PTR_TO_RPOOL_PTR(TO_RESOURCE_PTR(pResource));

	// modify resource before releasing it, to avoid race condition
	MsOS_ObtainMutex(pRPool->u32MutexID, MSOS_WAIT_FOREVER);
	TO_RESOURCE_PTR(pResource)->bInUse = false;
	pRPool->u32ResourceAvail++;
	release_sem(pRPool->u32ResourcesSemaphore);
	MsOS_ReleaseMutex(pRPool->u32MutexID);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleAddResourceStart(void* psModuleTmp, MS_U32 u32PoolID)
{
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleAddResourceEnd(void* pModuleTmp, MS_U32 u32RPoolID)
{
	int u32SemaphoreID;
    UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

    UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
    UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
    locate_resource_pool(pModule, u32RPoolID, &pLocatedRPool, &pLocatedRPoolPrev);

    /* Check RPool Semaphore ID*/
    if (pLocatedRPool->u32ResourcesSemaphore != 0 && pLocatedRPool->u32ResourcesSemaphore != 0xFFFFFFFF)
    {
        printf("semaphore id %d already exists\n", (int)pLocatedRPool->u32ResourcesSemaphore);
        return UTOPIA_STATUS_FAIL;
    }

    /* Create RPOOL Semaphore */
    u32SemaphoreID = MsOS_CreateSemaphore(pLocatedRPool->u32ResourceTotal, E_MSOS_FIFO , "dont care");
    if (u32SemaphoreID < 0)
    {
        printf("[utopia error] create semaphore fail\n");
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
	pLocatedRPool->u32ResourceAvail = pLocatedRPool->u32ResourceTotal;

	pLocatedRPool->u32ResourcesSemaphore = u32SemaphoreID;
	return UTOPIA_STATUS_SUCCESS;
}
//############################
//## Resource Functions End ##
//############################

#if 0

 UTOPIA_HIBERNATE_HEADER* pHibernateHeaderStartVA=NULL;
 UTOPIA_HIBERNATE_BLOCK* pHibernateBlockStartVA=NULL;
 MS_U32 u32HibernateMaxBlock=0;
 MS_U32 u32HibernateHeaderSizeTotal = 0;
 MS_U32 u32HibernateBlockSizeTotal = 0;
 MS_U32 u32HibernateCurrentBlkNum = 0;
 MS_U32 u32HibernateCurrentFileNum = 0;
 MS_BOOL bHibernateInit=false;


MS_U32 _serch_empyblock(void)
{
    UTOPIA_HIBERNATE_BLOCK* pBlockVA = pHibernateBlockStartVA;
	MS_U32 ret = 0;
	MS_U32 u32Count=0;
	while(pBlockVA->u32NextBlock != 0x0 && u32Count <= u32HibernateMaxBlock )
	{
	   u32Count+=1;
	   pBlockVA = pHibernateBlockStartVA + sizeof(UTOPIA_HIBERNATE_BLOCK)*u32Count;

	}

	if(u32Count == u32HibernateMaxBlock)
		ret = u32HibernateMaxBlock;

    return ret;
}


void _EndOfFile(MS_U32 u32BlockNum)
{
    UTOPIA_HIBERNATE_BLOCK* pBlockVA = pHibernateBlockStartVA+sizeof(UTOPIA_HIBERNATE_BLOCK)*u32BlockNum;
	pBlockVA->u32NextBlock = END_OF_BLOCK_LINKLIST;
}



MS_U32 UtopiaHibernateInit(MS_U8 u8MiuSel,MS_U32 pBAddrMem,MS_U32 u32size,MS_BOOL bFormat) //return: Sucessed
{



    if(MsOS_MPool_Mapping(u8MiuSel, pBAddrMem, u32size, 0) == false)
   	{
   	printf("[%s] MsOS_MPool_Mapping fail\n",__FUNCTION__);
   	return false;
   	}

	u32HibernateHeaderSizeTotal = sizeof(UTOPIA_HIBERNATE_HEADER)*MAX_FILE_NUM;

	if(u32HibernateHeaderSizeTotal>u32size)
	{
		    return UTOPIA_STATUS_FAIL;
	}



    u32HibernateBlockSizeTotal = u32size - u32HibernateHeaderSizeTotal;
	u32HibernateMaxBlock = u32HibernateBlockSizeTotal/BLOCK_SIZE;

    pHibernateHeaderStartVA = (void*)MsOS_MPool_PA2KSEG1(pBAddrMem);
    pHibernateBlockStartVA = (void*)MsOS_MPool_PA2KSEG1(pBAddrMem+u32HibernateHeaderSizeTotal);

	if(bFormat)
    {
       memset(pHibernateHeaderStartVA,u32size,0);
    }

	bHibernateInit=UTOPIA_STATUS_SUCCESS;
	return UTOPIA_STATUS_SUCCESS;


}



MS_U32 UtopiaHibernateCreate(char* hibernateName)  //return: Sucessed
{
     UTOPIA_HIBERNATE_HEADER* TEMPaa=pHibernateHeaderStartVA;
	 MS_U32 ret = UTOPIA_STATUS_FAIL;
	 MS_U32 u32CountTmp=0;

	 if(pHibernateHeaderStartVA == NULL)
	 	return ret;

	 for(u32CountTmp =0;u32CountTmp<MAX_FILE_NUM;u32CountTmp++)
	 {
	    TEMPaa = TEMPaa + sizeof(UTOPIA_HIBERNATE_HEADER)*u32CountTmp;
	    if(TEMPaa->u32HibernateFileExist != 0x0)
	    {
	       if(strcmp(TEMPaa->HibernateName,hibernateName)==0)
		   	ret =  UTOPIA_STATUS_SHM_EXIST ;
		    break;
	    }else
	    {
	       TEMPaa->u32HibernateFileExist=0x1;
		   strcpy(TEMPaa->HibernateName,hibernateName);
		   //TEMPaa->u32FirstBlock= _serch_empyblock();
		   u32HibernateCurrentFileNum = u32HibernateCurrentFileNum +1;
		   ret = UTOPIA_STATUS_SUCCESS ;
		   break;
	    }
	 }

	 return ret;


}


MS_U32 UtopiaHibernateOpen(char* hibernateName)//return: ID
{  MS_U32 ret = -1;
   UTOPIA_HIBERNATE_HEADER* TEMPaa=pHibernateHeaderStartVA;
   MS_U32 u32CountTmp=0;

   if(pHibernateHeaderStartVA == NULL )
	 	return ret;

   for(u32CountTmp =0;u32CountTmp<MAX_FILE_NUM;u32CountTmp++)
	 {
	    TEMPaa = TEMPaa + sizeof(UTOPIA_HIBERNATE_HEADER)*u32CountTmp;
	    if(TEMPaa->u32HibernateFileExist != 0x0)
	    {
	       if(strcmp(TEMPaa->HibernateName,hibernateName)==0)
	       	{
	       	    TEMPaa->bUSE=1;
		   	    ret = u32CountTmp;
	       	}
	    }else
	       break;
	 }

   return ret;


}


MS_U32 UtopiaHibernateWrite(MS_U32 FILEid, void* buff, MS_U32 size) //return: size
{
    MS_U32 ret=0;
	UTOPIA_HIBERNATE_HEADER* TEMPaa = NULL;
	UTOPIA_HIBERNATE_BLOCK* TEMPBLKaa = NULL ;
	MS_U32 u32BlockNum=0;
	void* pTemBuff = buff;
	//MS_U32 u32NextBlockNum=0;
	//MS_U32 u32writeCount=size;
	//MS_U8 u8ArrayBlockContact[BLOCK_CONTACT_SIZE] = {0};

	MS_U32 u32BlockNumOfFile = size/BLOCK_CONTACT_SIZE;
	MS_U32 u32RemanderOfFile = size%BLOCK_CONTACT_SIZE;

	TEMPaa = pHibernateHeaderStartVA + sizeof(UTOPIA_HIBERNATE_HEADER)*FILEid;;

	if(TEMPaa->bUSE ==0 || bHibernateInit == false || size==0)
		return ret;

	if((u32HibernateMaxBlock-u32HibernateCurrentFileNum) < u32BlockNumOfFile)
		return ret;


    TEMPaa->u32FirstBlock=_serch_empyblock();
	u32BlockNum = TEMPaa->u32FirstBlock;

	TEMPBLKaa = pHibernateBlockStartVA+ sizeof(UTOPIA_HIBERNATE_BLOCK)*u32BlockNum;

  //  u32BlockNum
    while(u32BlockNumOfFile>0)
    {
       // u32BlockNum = _serch_empyblock();
		if(u32BlockNum>=u32HibernateMaxBlock)
			break;
		pTemBuff = (void*)((MS_U32)pTemBuff +ret);
        memcpy((void*)TEMPBLKaa->u8File,pTemBuff,BLOCK_CONTACT_SIZE);
		_EndOfFile(u32BlockNum);
        ret = ret+BLOCK_CONTACT_SIZE;
		u32BlockNumOfFile=u32BlockNumOfFile-1;

		if(u32BlockNumOfFile==0)
			break;

	    u32BlockNum = _serch_empyblock();
	    TEMPBLKaa->u32NextBlock = u32BlockNum;
	    TEMPBLKaa = pHibernateBlockStartVA+ sizeof(UTOPIA_HIBERNATE_BLOCK)*u32BlockNum;

    }





	if(u32RemanderOfFile > 0)
	{
       u32BlockNum = _serch_empyblock();
	   TEMPBLKaa->u32NextBlock = u32BlockNum;
	   TEMPBLKaa = pHibernateBlockStartVA+ sizeof(UTOPIA_HIBERNATE_BLOCK)*u32BlockNum;

	   pTemBuff = (void*)((MS_U32)pTemBuff +ret);
	   memcpy((void*)TEMPBLKaa->u8File,pTemBuff,u32RemanderOfFile);
	   _EndOfFile(u32BlockNum);
	   ret+=u32RemanderOfFile;
	}

    return ret;

}


MS_U32 UtopiaHibernateRead(MS_U32 FILEid,void* buff , MS_U32 size) ////return: size
{
   MS_U32 ret=0;
   UTOPIA_HIBERNATE_HEADER* TEMPaa = pHibernateHeaderStartVA + sizeof(UTOPIA_HIBERNATE_HEADER)*FILEid;
   UTOPIA_HIBERNATE_BLOCK* TEMPBLKaa = NULL ;
   //MS_U32 u32writeCount=size;
   MS_U32 u32BlockNum=0;

   MS_U32 u32BlockNumOfFile = size/BLOCK_CONTACT_SIZE;
   MS_U32 u32RemanderOfFile = size%BLOCK_CONTACT_SIZE;

   void* pTemBuff = buff;


   if(TEMPaa->bUSE ==0)
		return ret;

   if(u32RemanderOfFile>0)
      u32BlockNumOfFile = u32BlockNumOfFile + 1;


   u32BlockNum = TEMPaa->u32FirstBlock;
   TEMPBLKaa = pHibernateBlockStartVA+sizeof(UTOPIA_HIBERNATE_BLOCK)*u32BlockNum;

   while((TEMPBLKaa->u32NextBlock != 0) && (u32BlockNumOfFile > 0))
   {
      if(TEMPBLKaa->u32NextBlock ==END_OF_BLOCK_LINKLIST )
      {
          pTemBuff = (void*)((MS_U32)pTemBuff +ret);
		  if(u32RemanderOfFile == 0)
		  {
              memcpy(pTemBuff,(void*)TEMPBLKaa->u8File,BLOCK_CONTACT_SIZE);
			  ret = ret + BLOCK_CONTACT_SIZE;
		  }
		  else
		  {
		  	  memcpy(pTemBuff,(void*)TEMPBLKaa->u8File,u32RemanderOfFile);
			  ret = ret + u32RemanderOfFile;
		  }

		  u32BlockNumOfFile = u32BlockNumOfFile -1;



      }else
      {
          pTemBuff = (void*)((MS_U32)pTemBuff +ret);
		  memcpy(pTemBuff,(void*)TEMPBLKaa->u8File,BLOCK_CONTACT_SIZE);
	      ret = ret + BLOCK_CONTACT_SIZE;
          u32BlockNumOfFile = u32BlockNumOfFile -1;
          u32BlockNum = TEMPBLKaa->u32NextBlock;
		  TEMPBLKaa = pHibernateBlockStartVA+sizeof(UTOPIA_HIBERNATE_BLOCK)*u32BlockNum;

      	}
   }



   return ret;


}

#endif
