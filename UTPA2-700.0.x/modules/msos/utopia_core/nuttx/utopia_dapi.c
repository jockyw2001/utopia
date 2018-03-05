#include "MsTypes.h"
#include "utopia_private.h"
#include <stdlib.h>
#include "MsOS.h"
#include <stdio.h>
#include "utopia.h"
#include <string.h>
#include "utopia_dapi.h"

// length should be the same to definition
extern char moduleNames[][40]; 
extern char rpoolNames[][40];
extern char ResourceNames[][40];

UTOPIA_SHM_ID* utopiaShmIdArray[50]; // FIXME: enum
MS_U32 utopiaShmIdIndex = 0;

UTOPIA_MODULE_STR_LIST* UTOPIA_MODULE_STR_LIST_HEAD = NULL;
UTOPIA_MODULE_STR_LIST* UTOPIA_MODULE_STR_LIST_CURRENT = NULL;
UTOPIA_MODULE_STR_LIST* UTOPIA_MODULE_STR_LIST_PREV = NULL;

// should directly refer to SHM_ARRAY_LENGTH in nuttx/MsOS_nuttx.c
// FIXME: for now, we do it with hardcode
#define SHM_ARRAY_LENGTH 200
int lib_lowprintf(const char *fmt, ...);
#define printf lib_lowprintf
void* shmid2va_table[SHM_ARRAY_LENGTH] = {NULL};

extern UTOPIA_PRIVATE* psUtopiaPrivate;

//struct sembuf op_down[1] = {{0, -1, 0}}; // "0" means 0th semaphore in sem. set
                                        // second "0" is flag, but I don't what it means
//struct sembuf op_up[1] = {{0, 1, 0}}; // "0" means 0th semaphore in sem. set
                                        // second "0" is flag, but I don't what it means
//############################
//## Static Functions Start ##
//############################
static void obtain_sem(MS_U32 u32SemID)
{
    /* Perform down operation */
    //semop(u32SemID, &op_down[0], 1);
	MsOS_ObtainSemaphore(u32SemID, MSOS_WAIT_FOREVER);
}

static void release_sem(MS_U32 u32SemID)
{
    /* Perform down operation */
    //semop(u32SemID, &op_up[0], 1);
	MsOS_ReleaseSemaphore(u32SemID);
}


static inline void* shmid2va(MS_U32 u32ShmID)
{
	if (u32ShmID == 0xFFFFFFFF)
		return NULL;
	return shmid2va_table[u32ShmID];
}

// u32ShmID: shmid2va table index
// u32ShmAddr: shmid2va table value
static MS_U32 fill_shmid2va_table(MS_U32 u32ShmID, void* u32ShmAddr)
{
	if (shmid2va_table[u32ShmID] != NULL)
	{
		printf("error: register duplicated shared memory ID %d\n", (int)u32ShmID);
		//while(1);
		return -1;
	}
	else
	{
		shmid2va_table[u32ShmID] = u32ShmAddr;
		return 0;
	}
}

// u32ShmSize: user specified shared memory size
// shmName: user specified shared memory name
// p32RetShmID: for returning shared memory id
// return: shared memory address in virtual
static void* shm_malloc(MS_U32 u32ShmSize, char* shmName, MS_U32* pu32RetShmID)
{
	MS_U32 u32RetShmID, u32RetAddr, u32RetShmSize;
	MS_U32 u32Ret;

	// u32Ret: TRUE for success, FALSE for failure
	u32Ret = MsOS_SHM_GetId((MS_U8*)shmName, u32ShmSize, &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_QUERY);

	// MsOS_SHM_GetId need to be called for each process to set up per-process shmid2va table
	// it's ok for duplicated calls with same shmName
	MsOS_SHM_GetId((MS_U8*)shmName, u32ShmSize, &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_CREATE);
	fill_shmid2va_table(u32RetShmID, (void*)u32RetAddr);

	// u32Ret: TRUE for success, FALSE for failure
	// if failure --> first time allocation, memset to 0
	if (u32Ret != 1)
		memset((void*)u32RetAddr, 0, u32RetShmSize);

	*pu32RetShmID = u32RetShmID;
	return (void*)u32RetAddr;
}

enum eObjectType
{
	E_TYPE_INSTANCE,
	E_TYPE_MODULE,
	E_TYPE_MODULE_SHM,
	E_TYPE_RESOURCE_POOL,
	E_TYPE_RESOURCE,
};

static UTOPIA_RESOURCE* next_resource(void* pUnkown, enum eObjectType eObjectType)
{
	switch(eObjectType)
	{
		case E_TYPE_RESOURCE_POOL:
			return (UTOPIA_RESOURCE*)shmid2va(((UTOPIA_RESOURCE_POOL*)pUnkown)->shmid_resource_head.ID);
		case E_TYPE_RESOURCE:
			return (UTOPIA_RESOURCE*)shmid2va(((UTOPIA_RESOURCE*)pUnkown)->shmid_next_resource.ID);
		default:
			printf("[utopia error] type is not supported\n");
			while(1);
	}
}

/*static MS_U32 isResourceInPool(UTOPIA_RESOURCE_POOL* psResourcePool, UTOPIA_RESOURCE* pResource)
{
	MS_U32 i;
	UTOPIA_RESOURCE* pResourceFound = NULL;

	pResourceFound = nextResource(psResourcePool, E_TYPE_RESOURCE_POOL);
	for (i = 0; i < psResourcePool->u32ResourceTotal; i++)
	{
		if (pResource == pResourceFound)
			return 1;
		pResourceFound = nextResource(pResourceFound, E_TYPE_RESOURCE);
	}
	return 0;
}*/

/*static UTOPIA_RESOURCE_POOL* nextResourcePool(void* pUnkown, enum eUtopiaType eUtopiaType)
{
	switch(eUtopiaType)
	{
		case E_TYPE_INSTANT:
			return (UTOPIA_RESOURCE_POOL*)shmid2va(((UTOPIA_INSTANCE*)pUnkown)->psModuleNode->psModuleNodeShm->shmid_UTOPIA_RESOURCE_POOL__Head.ID);
		case E_TYPE_MODULE_SHM:
			return (UTOPIA_RESOURCE_POOL*)shmid2va(((UTOPIA_MODULE_NODE_SHM*)pUnkown)->shmid_UTOPIA_RESOURCE_POOL__Head.ID);
		case E_TYPE_RESOURCE_POOL:
			return (UTOPIA_RESOURCE_POOL*)shmid2va(((UTOPIA_RESOURCE_POOL*)pUnkown)->shmid_UTOPIA_RESOURCE_POOL__Next.ID);
		default:
			printf("\n%s, %d, %s, what the fuck!\n", __FUNCTION__, __LINE__, __FILE__);
			while(1);
	}
}*/

/*static UTOPIA_RESOURCE_POOL* locate_pool_by_instant_resource(UTOPIA_INSTANCE* psInstant, UTOPIA_RESOURCE* pResource)
{
	//MS_U32 u32ShmID = psInstant->psModuleNode->psModuleNodeShm->shmid_UTOPIA_RESOURCE_POOL__Head.ID;
	//UTOPIA_RESOURCE_POOL* psResourcePool = (UTOPIA_RESOURCE_POOL*)shmid2va(u32ShmID);
	UTOPIA_RESOURCE_POOL* psResourcePool = nextResourcePool(psInstant, E_TYPE_INSTANT);
	while (psResourcePool != NULL && !isResourceInPool(psResourcePool, pResource))
	{
		psResourcePool = nextResourcePool(psResourcePool, E_TYPE_RESOURCE_POOL);
	}

	return psResourcePool;
}*/

// return located module address or NULL if not found
void* locate_module(MS_U32 u32ModuleID)
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

/*static void UtopiaResourcePoolLocate(MS_U32 u32PoolID, void** ppsResourcePoolTmp, void** ppsResourcePoolPrevTmp)
{
	UTOPIA_RESOURCE_POOL** ppsResourcePool = (UTOPIA_RESOURCE_POOL**)ppsResourcePoolTmp;
	UTOPIA_RESOURCE_POOL** ppsResourcePoolPrev = (UTOPIA_RESOURCE_POOL**)ppsResourcePoolPrevTmp;
	UTOPIA_RESOURCE_POOL* psResourcePool = *ppsResourcePool;
	UTOPIA_RESOURCE_POOL* psResourcePoolPrev = *ppsResourcePoolPrev = NULL;

	while (psResourcePool != NULL && psResourcePool->u32PoolID != u32PoolID)
	{
		psResourcePoolPrev = psResourcePool;
		psResourcePool = (UTOPIA_RESOURCE_POOL*)shmid2va(psResourcePool->shmid_next_rpool.ID);
	}
	*ppsResourcePool = psResourcePool;
	*ppsResourcePoolPrev = psResourcePoolPrev;
}*/

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

//##########################
//## Static Functions End ##
//##########################

//#############################
//## Instance Functions Start ##
//#############################
MS_U32 UtopiaInstanceCreate(MS_U32 u32PrivateSize, void** ppInstance)
{
    UTOPIA_INSTANCE* pInstance = NULL;

	// check parameters
	if (ppInstance == NULL)
		printf("ppInstance should not be NULL\n");

    pInstance = malloc(sizeof(UTOPIA_INSTANCE));
    memset((void*)pInstance, 0, sizeof(UTOPIA_INSTANCE));
    if (u32PrivateSize != 0)
    {
        pInstance->pPrivate = malloc(u32PrivateSize);
        memset(pInstance->pPrivate, 0, u32PrivateSize);
    }
    //pInstant->u32DeviceDebugLevel   = 0; // FIXME: u32DeviceDebugLevel is in UTOPIA_MODULE_NODE_SHM

	*ppInstance = pInstance;
    return 0; // FIXME: error code
}

MS_U32 UtopiaInstanceDelete(void* pInstance)
{
	free(TO_INSTANCE_PTR(pInstance)->pPrivate);
	free(pInstance);

	return 0; // FIXME: error code
}

MS_U32 UtopiaInstanceGetPrivate(void* pInstance, void** ppPrivate)
{
	*ppPrivate = TO_INSTANCE_PTR(pInstance)->pPrivate;
	return 0; // FIXME: error code
}

MS_U32 UtopiaInstanceGetModule(void* pInstance, void** ppModule)
{
	*ppModule = TO_INSTANCE_PTR(pInstance)->psModule;
	return 0; // FIXME: error code
}

MS_U32 UtopiaInstanceGetModuleID(void* pInstance, MS_U32* pu32ModuleID)
{
	*pu32ModuleID = TO_INSTANCE_PTR(pInstance)->psModule->u32ModuleID;
	return 0;
}

MS_U32 UtopiaInstanceGetModuleVersion(void* pInstance, MS_U32* pu32Version)
{
	*pu32Version = TO_INSTANCE_PTR(pInstance)->psModule->u32Version;
	return 0;
}

MS_U32 UtopiaInstanceGetAppRequiredModuleVersion(void* pInstance, MS_U32* pu32ModuleVersion)
{
	*pu32ModuleVersion = TO_INSTANCE_PTR(pInstance)->u32AppRequireModuleVersion;	
	return 0;
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

	// 1. create module@shm
	pModuleShm = shm_malloc(sizeof(UTOPIA_MODULE_SHM), moduleNames[u32ModuleID], &u32ShmID);
	pModuleShm->shmid_rpool_head.ID = 0xFFFFFFFF;

	// 2. create module
	pModule = malloc(sizeof(UTOPIA_MODULE));
    memset(pModule, 0, sizeof(UTOPIA_MODULE));
	pModule->u32ModuleID = u32ModuleID;
	pModule->psModuleShm = pModuleShm;

	// 3. create private of module
	sprintf(privateName, "%s_PRI", moduleNames[u32ModuleID]); // FIXME: potential bug
	pPrivate = shm_malloc(u32PrivateSize, privateName, &(pModule->shmid_private.ID));

	// 4. initial str private
	pModule->shmid_str.ID=0xFFFFFFFF;

	*ppModule = pModule;
	return 0; //FIXME: error code
}

MS_U32 UtopiaModuleGetPrivate(void* pModule, void** ppPrivate)
{
	*ppPrivate = shmid2va(TO_MODULE_PTR(pModule)->shmid_private.ID);
	return 0;
}

MS_U32 UtopiaModuleSetSTRPrivate(void* pModule, MS_U32 u32STRPrivateSize)
{
    char privateName[50]; // FIXME: potential bug
    void* pPrivate = NULL;

    /* check str private exist? */
    if (!pModule)
    {
        printf("[utopia param error] module pointer should not be null\n");
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
    if (0xFFFFFFFF != TO_MODULE_PTR(pModule)->shmid_str.ID)
    {
        printf("[utopia module error] set duplicated STR private of module: %s\n"
                , moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }

    /* create str private of module */
    snprintf(privateName, sizeof(privateName)
            , "%s_STR", moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
    pPrivate = shm_malloc(u32STRPrivateSize, privateName,
            &(TO_MODULE_PTR(pModule)->shmid_str.ID));

    if(pPrivate == NULL)  //Coverity
    {
        printu("[utopia malloc error] Line: %d shm_malloc fail\n",__LINE__);
    }

    return 0;
}

MS_U32 UtopiaModuleGetSTRPrivate(void* pModule, void** ppPrivate)
{
    /* check str private exist? */
    if (!pModule)
    {
        printu("[utopia param error] module pointer should not be null\n");
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
    if (0xFFFFFFFF == TO_MODULE_PTR(pModule)->shmid_str.ID)
    {
        printf("[utopia module error] NULL STR private of module: %s\n"
                , moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }

    *ppPrivate = shmid2va(TO_MODULE_PTR(pModule)->shmid_str.ID);
    return 0;
}

MS_U32  UtopiaModuleRegister(void* pModuleTmp)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_MODULE* pLocatedModule = NULL;

	// check parameter
	if (pModule == NULL)
	{
		printf("module pointer can't be null\n"); 
		return 1; // FIXME: error code
	}

	pLocatedModule = TO_MODULE_PTR(locate_module(pModule->u32ModuleID));
	if (pLocatedModule == NULL) // module is not yet registered
	{
		// insert module into list head
		printf("register module: %s\n", moduleNames[pModule->u32ModuleID]);
		pModule->psNext = psUtopiaPrivate->psModuleHead;
		psUtopiaPrivate->psModuleHead = pModule;
		return 0; // FIXME: error code
	}
	else  // module is already registered
    {
        printf("error: register duplicated module: %s\n", moduleNames[pModule->u32ModuleID]);
        return 1; // FIXME: error code
    }
}

MS_U32 UtopiaModuleSetupFunctionPtr(void* pModuleTmp, FUtopiaOpen fpOpen, FUtopiaClose fpClose, FUtopiaIOctl fpIoctl)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

	pModule->fpOpen = fpOpen;
	pModule->fpClose = fpClose;
	pModule->fpIoctl = fpIoctl;

	return 0; //FIXME: error code
}

MS_U32 UtopiaModuleSetupSTRFunctionPtr(void* pModuleTmp, FUtopiaSTR fpSTR)
{
    UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

    if (!pModuleTmp)
    {
        printf("[utopia param error] module pointer should not be null\n");
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
    pModule->fpSTR = fpSTR;
    /* insert linked list */
    UTOPIA_MODULE_STR_LIST_CURRENT = (UTOPIA_MODULE_STR_LIST*)malloc( sizeof(UTOPIA_MODULE_STR_LIST) );
    UTOPIA_MODULE_STR_LIST_CURRENT->fpSTR = fpSTR;
    UTOPIA_MODULE_STR_LIST_CURRENT->pModule = pModuleTmp;
    UTOPIA_MODULE_STR_LIST_CURRENT->next = NULL;
    if(UTOPIA_MODULE_STR_LIST_HEAD == NULL)
    {
        UTOPIA_MODULE_STR_LIST_HEAD = UTOPIA_MODULE_STR_LIST_CURRENT;
    }
    else
    {
        UTOPIA_MODULE_STR_LIST_PREV->next = UTOPIA_MODULE_STR_LIST_CURRENT;
    }
    UTOPIA_MODULE_STR_LIST_PREV = UTOPIA_MODULE_STR_LIST_CURRENT;
    return 0;
}

MS_U32 UtopiaModuleSuspend(void)
{
    UTOPIA_MODULE_STR_LIST_CURRENT = UTOPIA_MODULE_STR_LIST_HEAD;
    while(1)
    {
        if(UTOPIA_MODULE_STR_LIST_CURRENT == NULL)
        {
            break;
        }
        UTOPIA_MODULE_STR_LIST_CURRENT->fpSTR(E_POWER_SUSPEND, UTOPIA_MODULE_STR_LIST_CURRENT->pModule);
        if(UTOPIA_MODULE_STR_LIST_CURRENT->next == NULL)
        {
            break;
        }
        UTOPIA_MODULE_STR_LIST_CURRENT = UTOPIA_MODULE_STR_LIST_CURRENT->next;
    }
    return 0;
}

MS_U32 UtopiaModuleResume(void)
{
    FUtopiaSTR fpSealSTR = NULL;
    UTOPIA_MODULE_STR_LIST_CURRENT = UTOPIA_MODULE_STR_LIST_HEAD;
    while(1)
    {
        if(UTOPIA_MODULE_STR_LIST_CURRENT == NULL)
        {
            break;
        }
        if(MODULE_SEAL == TO_MODULE_PTR(UTOPIA_MODULE_STR_LIST_CURRENT->pModule)->u32ModuleID)
        {
            fpSealSTR = UTOPIA_MODULE_STR_LIST_CURRENT->fpSTR;
        }
        else
        {
            UTOPIA_MODULE_STR_LIST_CURRENT->fpSTR(E_POWER_RESUME, UTOPIA_MODULE_STR_LIST_CURRENT->pModule);
        }
        if(UTOPIA_MODULE_STR_LIST_CURRENT->next == NULL)
        {
            break;
        }
        UTOPIA_MODULE_STR_LIST_CURRENT = UTOPIA_MODULE_STR_LIST_CURRENT->next;
    }
    if(fpSealSTR != NULL)
    {
        fpSealSTR(E_POWER_RESUME, UTOPIA_MODULE_STR_LIST_CURRENT->pModule);
    }
    return 0;
}

MS_U32 UtopiaModuleSetVersion(void* pModule, MS_U32 u32Version)
{
	TO_MODULE_PTR(pModule)->u32Version = u32Version;

	return 0;
}

MS_U32 UtopiaModuleGetDebugLevel(void* pInstance, MS_U32* pu32DebugLevel)
{
	*pu32DebugLevel = TO_INSTANCE_PTR(pInstance)->psModule->u32DeviceDebugLevel;
	return 0;
}

MS_U32 UtopiaModuleGetPtr(MS_U32 u32ModuleID, void** ppModule)
{
	*ppModule = locate_module(u32ModuleID);

	return 0;
}
//##########################
//## Module Functions End ##
//##########################

//##############################
//## Resource Functions Start ##
//##############################
MS_U32 UtopiaResourceCreate(char* resourceName, MS_U32 u32PrivateSize, void** ppResource)
{
	MS_U32 u32RetResourceShmID;
	UTOPIA_RESOURCE* pResource = NULL;
	char privateName[50]; // FIXME: potential bug

	// 1. create resource
	pResource = shm_malloc(sizeof(UTOPIA_RESOURCE), resourceName, &(u32RetResourceShmID));
	pResource->shmid_self.ID = u32RetResourceShmID;
	pResource->shmid_next_resource.ID = 0xFFFFFFFF;
	*ppResource = pResource;

	// 2. create resource private
	sprintf(privateName, "%s_PRI", resourceName); // FIXME: potential bug
	shm_malloc(u32PrivateSize, privateName, &(pResource->shmid_private.ID));

	return 0; // FIXME: error code
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

	// 1. deal with resource pool
	UTOPIA_RESOURCE_POOL* pRPool = NULL;
	UTOPIA_RESOURCE_POOL* pRPoolPrev = NULL;
	locate_resource_pool(pModule, u32RPoolID, &pRPool, &pRPoolPrev);
	// case 1: there's no resource pool --> *ppRPool = NULL & *ppRPoolPrev = NULL
	// case 2: at least one resource pool exists, but not the located one --> *ppRPool = NULL & *ppRPoolPrev != NULL
	// case 3: at least one resource pool exists, and so is the located one --> *ppRPool != NULL & ppRPoolPrev doesn't matter
	if (pRPool == NULL) { // case 1 or 2
		MS_U32 u32RetRPoolShmID;
		MS_U32 u32ModuleID = pModule->u32ModuleID;

		sprintf(rpoolName, "%s_%d", rpoolNames[u32ModuleID], (int)u32RPoolID);
		pRPool = shm_malloc(sizeof(UTOPIA_RESOURCE_POOL), rpoolName, &u32RetRPoolShmID);
		pRPool->shmid_self.ID = u32RetRPoolShmID;
		pRPool->u32PoolID = u32RPoolID;
		pRPool->shmid_next_rpool.ID = 0xFFFFFFFF;
		pRPool->shmid_resource_head.ID = 0xFFFFFFFF;

		// set up connection
		if (pRPoolPrev == NULL) // case 1
		{
			pModule->psModuleShm->shmid_rpool_head.ID = u32RetRPoolShmID;
		}
		else // case 2
		{
			pRPoolPrev->shmid_next_rpool.ID = u32RetRPoolShmID;
		}

		pModule->psModuleShm->u32ResourcePoolTotal++;
	}

	// 2. deal with resource
	pResource->shmid_rpool = pRPool->shmid_self;

	UTOPIA_RESOURCE* pLocatedResource = NULL;
	UTOPIA_RESOURCE* pLocatedResourcePrev = NULL;
	locate_resource(pRPool, pResource->shmid_self.ID, &pLocatedResource, &pLocatedResourcePrev);
	// case 1: there's no resource in pool --> pLocatedResource = NULL, pLocatedResourcePrev = NULL
	// case 2: at least one resource exists, but not the located one --> pLocatedResource = NULL, pLocatedResourcePrev != NULL
	// case 3: at least one resource exists, and so is the located one --> pLocatedResource != NULL, pLocatedResourcePrev doesn't matter
	if (pLocatedResource == NULL) // case 1 or 2
	{
		if (pLocatedResourcePrev == NULL) // case 1
		{
			pRPool->shmid_resource_head.ID = pResource->shmid_self.ID;	
		}
		else // case 2
		{
			pLocatedResourcePrev->shmid_next_resource.ID = pResource->shmid_self.ID;	
		}
		pRPool->u32ResourceTotal++;

		return 0;
	}
	else // case 3
	{
		// duplicated registration: it may be registered by other process
		return 1;
	}

    //unlock(u32ResourcePoolsMutex)
}


MS_U32 UtopiaResourceObtain(void* pModule, MS_U32 u32RPoolID, void** ppResource)
{
    //Search and get one resource from psModuleNode->psResourcePoolHeads
    //set flag bit0 to 1 for using

	//UTOPIA_RESOURCE_POOL* psResourcePool = NULL;
	//psResourcePool = MODULE_PTR_TO_RPOOL_PTR(TO_MODULE_PTR(pModule));
	//UTOPIA_RESOURCE_POOL* psResourcePoolPrev = NULL;
	//UTOPIA_RESOURCE* psResource = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	UTOPIA_RESOURCE* pAvailResource = NULL;
	locate_resource_pool(TO_MODULE_PTR(pModule), u32RPoolID, &pLocatedRPool, &pLocatedRPoolPrev);


    //semephone_lock(u32ResourcesSemaphore)
	obtain_sem(pLocatedRPool->u32ResourcesSemaphore); 

	pAvailResource = next_resource(pLocatedRPool, E_TYPE_RESOURCE_POOL);
	while (pAvailResource != NULL)
	{
		if (!(pAvailResource->bInUse))
		{
			pAvailResource->bInUse = true;
			*ppResource	= pAvailResource;
			return 0;
		}
		else
		{
			pAvailResource = next_resource(pAvailResource, E_TYPE_MODULE);
		}
	}

	return 1; // what kind of situation? FIXME: error code

    //lock(u32ResourcesSemaphore)
    //semephone_unlock(u32ResourcesSemaphore)
}

MS_U32 UtopiaResourceRelease(void* pResource)
{
	UTOPIA_RESOURCE_POOL* pRPool = RESOURCE_PTR_TO_RPOOL_PTR(TO_RESOURCE_PTR(pResource));

	// modify resource before releasing it, to avoid race condition
	TO_RESOURCE_PTR(pResource)->bInUse = false;
	release_sem(pRPool->u32ResourcesSemaphore); 

	return 0;
}

MS_U32 UtopiaModuleAddResourceStart(void* psModuleTmp, MS_U32 u32PoolID)
{
	return 0;
}

#define PERMS 0666
// semaphore num isn't equal to resource count
#define SEMAPHORE_NUM 1
//MS_U32 u32LastUtopiaSemID = UTOPIA_SEM_START;
MS_U32 UtopiaModuleAddResourceEnd(void* pModuleTmp, MS_U32 u32RPoolID)
{
	int u32SemaphoreID;
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	//psResourcePool = (UTOPIA_RESOURCE_POOL*)shmid2va(pModule->psModuleShm->shmid_rpool_head.ID);
	locate_resource_pool(pModule, u32RPoolID, &pLocatedRPool, &pLocatedRPoolPrev);

	if (pLocatedRPool->u32ResourcesSemaphore != 0 && pLocatedRPool->u32ResourcesSemaphore != 0xFFFFFFFF)
	{
		printf("semaphore id %d already exists\n", (int)pLocatedRPool->u32ResourcesSemaphore);
		return 1; // FIXME: error code
	}

	//MS_U32* pu32SemKey = (MS_U32*)shmid2va(4);  // 4 for semaphore private
	MS_U32 u32PoolShmID = pModule->psModuleShm->shmid_rpool_head.ID;
	UTOPIA_RESOURCE_POOL* pResourcePool = shmid2va(u32PoolShmID);
	// FIXME: misuse? should I use utopia module resource instead?
	MS_U32* pu32SemKey = (MS_U32*)shmid2va(pResourcePool->shmid_resource_head.ID);

	//u32SemID = semget(*pu32SemKey, 0, IPC_CREAT | PERMS);
	u32SemaphoreID = MsOS_InfoSemaphore(*pu32SemKey, &(pLocatedRPool->u32ResourceTotal), NULL, NULL);

	//if(u32SemID == 0 || u32SemID == 0xFFFFFFFF)
	if(u32SemaphoreID == FALSE)
    {

        //u32SemID = semget(*pu32SemKey, 1, IPC_CREAT | PERMS); // "1" means semaphore num, not its initial value
		u32SemaphoreID = MsOS_CreateSemaphore(pLocatedRPool->u32ResourceTotal,E_MSOS_FIFO ,"dont care");
        //if ((u32SemID != 0 && u32SemID != 0xFFFFFFFF)) // create success, increase *pu32SemKey for next sem create
        //{
            (*pu32SemKey)++;
        //}

        //while(u32SemID == 0 || u32SemID == 0xFFFFFFFF)
        //{
            //u32SemID = semget(*pu32SemKey, 1, IPC_CREAT | PERMS); // "1" means semaphore num, not its initial value
			//u32SemID = MsOS_CreateSemaphore(0,E_MSOS_FIFO ,"dont care");
            //(*pu32SemKey)++;
        //}
        if (u32SemaphoreID < 0)
        {
            printf("Couldn't create semaphore!\n");
            exit(-1);
        }

        //semctl(u32SemID, 0, SETVAL, psResourcePool->u32ResourceTotal); // "0" is the semaphore id in sem. set which we got by semget
                                            //SEVAL, "1" set its initial value
        //semctl(u32SemID, 0, SETVAL, 3); // "0" is the semaphore id in sem. set which we got by semget
    }
	pLocatedRPool->u32ResourcesSemaphore = u32SemaphoreID;

	return 0;
}
