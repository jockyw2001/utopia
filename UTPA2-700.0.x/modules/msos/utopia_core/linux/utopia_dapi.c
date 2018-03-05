#include "MsTypes.h"
#include "utopia_private.h"
#include "MsOS.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/syscalls.h> // for syscall
#else
#include <stdio.h>
#include <unistd.h> /* for usleep */
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#endif
#include "drvIPAUTH.h"

#include "utopia.h"
#include "utopia_dapi.h"
#include <errno.h>
#include "mdrv_semutex_io.h"

#if defined(MSOS_TYPE_LINUX_KERNEL) || defined(ANDROID) || defined(TV_OS)
#include <linux/sem.h>
#else
#include <sys/sem.h>
#endif

#define DEADLOCK_DEBUG 0

#if DEADLOCK_DEBUG == 1
#include <execinfo.h>
#endif

#if defined(CONFIG_UTOPIA_FRAMEWORK_DISABLE_SYSTEM_V_IPCS) && defined(TV_OS)
#define DISABLE_SYSCALL 1
#endif

MS_U32 bt_threshold = 30000; /* 30 sec */
MS_U32 bt_period = 3000; /* 3 sec */

// length should be the same to definition
extern char moduleNames[][40];
extern char rpoolNames[][40];
extern char ResourceNames[][40];
extern unsigned int moduleMode[];


#if defined(MSOS_TYPE_LINUX_KERNEL) || defined(ANDROID) || defined(TV_OS)
# if defined(CONFIG_ARM_CA7) || defined(CONFIG_ARM_CA9) || \
	defined(CONFIG_ARM_CA12) || defined(MCU_ARM_9) || defined(MCU_ARM_CA12) || defined(CONFIG_ARM_CA53)



#if defined (__aarch64__)
#define SYS_semop  193
#define SYS_semget 190
#define SYS_semctl 191
#else
#define SYS_semop  298
#define SYS_semget 299
#define SYS_semctl 300
#endif

long int syscall (long int __sysno, ...);

int semop (int __semid, struct sembuf *__sops, size_t __nsops)
{
	return syscall(SYS_semop, __semid, __sops, __nsops);
}

int semget (key_t __key, int __nsems, int __semflg)
{
	return syscall(SYS_semget, __key, __nsems, __semflg);
}

int semctl (int __semid, int __semnum, int __cmd, int __counter)
{
	return syscall(SYS_semctl, __semid, __semnum, __cmd, __counter);
}
# else
int semop (int __semid, struct sembuf *__sops, size_t __nsops);
int semget (key_t __key, int __nsems, int __semflg);
int semctl (int __semid, int __semnum, int __cmd, int __counter);
# endif
#endif

UTOPIA_SHM_ID* utopiaShmIdArray[50]; // FIXME: enum
MS_U32 utopiaShmIdIndex = 0;

struct shm_info {
	void* va;
	char name[40]; // FIXME: array length should be unify
};

/*
 * could be equal to MAX_SHM_CLIENT_NUM=320 defined in MsOS_linux.c
 * cause we ues shm id as table index, there may be holes in the table
 */
#define SHM_INFO_TABLE_LENGTH 300
struct shm_info shm_info_table[SHM_INFO_TABLE_LENGTH] = {{NULL, ""}};

extern UTOPIA_PRIVATE* psUtopiaPrivate;

enum eObjectType
{
	E_TYPE_INSTANCE,
	E_TYPE_MODULE,
	E_TYPE_MODULE_SHM,
	E_TYPE_RESOURCE_POOL,
	E_TYPE_RESOURCE,
};

#if !defined(DISABLE_SYSCALL)
/* 1st field: the n-th semaphore in set */
/* 2nd field: up/down operation */
/* 3rd field: ??? */
struct sembuf op_down[1] = {{0, -1, 0}};
static inline MS_U32 obtain_sem(MS_U32 u32SemID)
{
    return semop(u32SemID, &op_down[0], 1);
}

struct sembuf op_up[1] = {{0, 1, 0}};
static inline MS_U32 release_sem(MS_U32 u32SemID)
{
    return semop(u32SemID, &op_up[0], 1);
}
#endif

static inline void* shmid2va(MS_U32 u32ShmID)
{
	if (u32ShmID == 0xFFFFFFFF)
		return NULL;
	return shm_info_table[u32ShmID].va;
}

#if 0 //coverity (PW.DECLARED_BUT_NOT_REFERENCED) cannot code suppression
static inline MS_U32 isFirstCreation(void* pUnknown, enum eObjectType eObjectType)
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
			printu("[utopia param error] type is not supported in %s\n",
					__FUNCTION__);
			RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
}
#endif

/*
 * u32ShmSize: user specified shared memory size
 * shmName: user specified shared memory name
 * p32RetShmID: for returning shared memory id
 * return: shared memory address in virtual
 */
static MS_U32 shm_malloc(MS_U32 u32ShmSize, char* shmName,
		void** ppRetAddr, MS_U32* pu32RetShmID)
{
	MS_U32 u32RetShmID = 0, u32RetShmSize= 0;
	MS_U32 u32QueryRet, u32CreateRet, u32StatusRet;
	MS_VIRT vaRetAddr = 0;

	/* FIXME: if shm exist, maybe query is enough to fill shm_info_table */
	/* 		--> no need to create again? */

	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(strcmp(shmName, "") == 0)) {
		printu("[utopia param error] shm name string should not be empty\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	/*
	 * u32QueryRet: 0 for not found
	 *         1 for existence
	 * u32RetShmID: 0 for not found
	 *              n for created share mem id, start from 1
	 */
	u32QueryRet = MsOS_SHM_GetId((MS_U8*)shmName, u32ShmSize, &u32RetShmID
			, &vaRetAddr, &u32RetShmSize, MSOS_SHM_QUERY);

	/*
	 * MsOS_SHM_GetId need to be called for each process
	 * to set up per-process shmid2va table
	 * it's ok for duplicated calls with same shmName
	 */

	/*
	 * u32CreateRet: 0 for failed creation
	 *            1 for successful creation
	 * u32RetShmID: 0 for not found
	 *              n for created share mem id, start from 1
	 */
	u32CreateRet = MsOS_SHM_GetId((MS_U8*)shmName, u32ShmSize, &u32RetShmID
			, &vaRetAddr, &u32RetShmSize, MSOS_SHM_CREATE);
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(u32CreateRet == 0)) {
		printu("[utopia shm error] something wrong in MsOS_SHM_GetId\n");
		printu("is SHM_SIZE reached?\n");
		printu("is MAX_SHM_CLIENT_NUM reached?\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	/* check whether table limit is reached? */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(u32RetShmID >= SHM_INFO_TABLE_LENGTH)) {
		printu("[utopia shm error] shm id %d exceeds shm-info table length %d\n"
				, (int)u32RetShmID, SHM_INFO_TABLE_LENGTH);
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	/* it has already been registered */
	if (shm_info_table[u32RetShmID].va != NULL) {
		printu("[utopia shm warning] \
				register duplicated shared memory ID %d: %s@%p\n"
				, (int)u32RetShmID, shm_info_table[u32RetShmID].name
				, shm_info_table[u32RetShmID].va);
	} else { /* fill the table */
		shm_info_table[u32RetShmID].va = (void*)vaRetAddr;
		strncpy(shm_info_table[u32RetShmID].name
				, shmName, sizeof(shm_info_table[0].name));
	}

	/*
	 * u32QueryRet: TRUE for success, FALSE for failure
	 * if failure --> first time allocation, memset to 0
	 */
	if (u32QueryRet != 1) {
		memset((void*)vaRetAddr, 0, u32RetShmSize);
		u32StatusRet = UTOPIA_STATUS_SUCCESS;
	} else
		u32StatusRet = UTOPIA_STATUS_SHM_EXIST;

	*pu32RetShmID = u32RetShmID;
	*ppRetAddr = (void*)vaRetAddr;
	return u32StatusRet;
}

static UTOPIA_RESOURCE* next_resource(void* pUnkown
		, enum eObjectType eObjectType)
{
	switch(eObjectType)
	{
		case E_TYPE_RESOURCE_POOL:
			return (UTOPIA_RESOURCE*)shmid2va(
					((UTOPIA_RESOURCE_POOL*)pUnkown)->shmid_resource_head.ID);
		case E_TYPE_RESOURCE:
			return (UTOPIA_RESOURCE*)shmid2va(
					((UTOPIA_RESOURCE*)pUnkown)->shmid_next_resource.ID);
		default:
			printu("[utopia error] type is not supported in %s\n", __FUNCTION__);
			RET_OR_BLOCK(NULL);
	}
}

#if DEADLOCK_DEBUG == 1
void print_trace(void)
{
	void *array[10];
	size_t size;
	char **strings;
	size_t i;

	size = backtrace (array, 10);
	strings = backtrace_symbols (array, size);

	printu ("Obtained %zd stack frames.\n", size);

	for (i = 0; i < size; i++)
		printu ("%s\n", strings[i]);

	free (strings);
}
#endif

/* return located module address or NULL if not found */
static void* locate_module(MS_U32 u32ModuleID)
{
	UTOPIA_MODULE* pLocatedModule = psUtopiaPrivate->psModuleHead;

	while (pLocatedModule != NULL && pLocatedModule->u32ModuleID != u32ModuleID)
		pLocatedModule = pLocatedModule->psNext;

	return pLocatedModule;
}

/*
 * case 1: there's no resource pool --> *ppRPool = NULL & *ppRPoolPrev = NULL
 * case 2: at least one resource pool exists, but not the located one
 * 		--> *ppRPool = NULL & *ppRPoolPrev != NULL
 * case 3: at least one resource pool exists, and so is the located one
 * 		--> *ppRPool != NULL & ppRPoolPrev doesn't matter
 */
static MS_U32 locate_resource_pool(UTOPIA_MODULE* pModule, MS_U32 u32LocatedRPoolID
		, UTOPIA_RESOURCE_POOL** ppRPool, UTOPIA_RESOURCE_POOL** ppRPoolPrev)
{
	UTOPIA_RESOURCE_POOL* pRPool
		= TO_RPOOL_PTR(shmid2va(pModule->psModuleShm->shmid_rpool_head.ID));
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

static MS_U32 locate_resource(UTOPIA_RESOURCE_POOL* pRPool
		, MS_U32 u32LocatedResourceID, UTOPIA_RESOURCE** ppRes
		, UTOPIA_RESOURCE** ppResPrev)
{
	UTOPIA_RESOURCE* pRes
		= TO_RESOURCE_PTR(shmid2va(pRPool->shmid_resource_head.ID));
	UTOPIA_RESOURCE* pResPrev = NULL;

	while (pRes != NULL
			&& (pRes->shmid_self.ID != u32LocatedResourceID))
	{
		pResPrev = pRes;
		pRes = TO_RESOURCE_PTR(shmid2va(pRes->shmid_next_resource.ID));
	}

	*ppRes = pRes;
	*ppResPrev = pResPrev;
	return 0;
}

MS_U32 UtopiaInstanceCreate(MS_U32 u32PrivateSize, void** ppInstance)
{
    UTOPIA_INSTANCE* pInstance = NULL;

	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(ppInstance == NULL))
	{
		printu("[utopia param error] instance ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

    pInstance = malloc(sizeof(UTOPIA_INSTANCE));
    if(MSOS_BRANCH_PREDICTION_LIKELY(pInstance != NULL)) //Coverity
    {
        memset(pInstance, 0, sizeof(UTOPIA_INSTANCE));
        pInstance->pPrivate = malloc(u32PrivateSize);
        if(pInstance->pPrivate != NULL)
            memset(pInstance->pPrivate, 0, u32PrivateSize);
        else
            printu("[utopia error] Line: %d pInstance->pPrivate == NULL\n",__LINE__);
	*ppInstance = pInstance;
    }
    else
    {
        printu("[utopia malloc error] Line: %d pInstance == NULL\n",__LINE__);
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
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
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pInstance == NULL))
	{
		printu("[utopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
	else if (MSOS_BRANCH_PREDICTION_UNLIKELY(ppPrivate == NULL))
	{
		printu("[utopia param error] private ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
    else
    {
	*ppPrivate = TO_INSTANCE_PTR(pInstance)->pPrivate;
    }
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaInstanceGetModule(void* pInstance, void** ppModule)
{
	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pInstance == NULL))
	{
		printu("[uopia param error] instance pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
	else if (MSOS_BRANCH_PREDICTION_UNLIKELY(ppModule == NULL))
	{
		printu("[utopia param error] module ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
    else
    {
	*ppModule = TO_INSTANCE_PTR(pInstance)->psModule;
    }

	/* check module pointer */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(*ppModule == NULL))
	{
		printu("[utopia param error] module pointer should not be null\n");
		printu("forgot to call UtopiaOpen first?\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

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

MS_U32 UtopiaInstanceGetAppRequiredModuleVersion(void* pInstance
		, MS_U32* pu32ModuleVersion)
{
	*pu32ModuleVersion = TO_INSTANCE_PTR(pInstance)->u32AppRequireModuleVersion;
	return UTOPIA_STATUS_SUCCESS;
}

/*
 * assume one module for each driver
 * otherwise they have to pass module name as parameter
 */
MS_U32 UtopiaModuleCreate(MS_U32 u32ModuleID
		, MS_U32 u32PrivateSize, void** ppModule)
{
	MS_U32 u32ShmID, u32Status;
	UTOPIA_MODULE_SHM* pModuleShm = NULL;
	UTOPIA_MODULE* pModule = NULL;
	void* pPrivate = NULL;
	char privateName[50]; // FIXME: potential bug

	/* 1. create module@shm */
	u32Status = shm_malloc(sizeof(UTOPIA_MODULE_SHM),
			moduleNames[u32ModuleID], (void**)&pModuleShm, &u32ShmID);
	if (u32Status == UTOPIA_STATUS_SUCCESS)
	{
		pModuleShm->shmid_self.ID = u32ShmID;
		strncpy(pModuleShm->shmid_self.name, moduleNames[u32ModuleID]
				, sizeof(pModuleShm->shmid_self.name));
		pModuleShm->shmid_rpool_head.ID = 0xFFFFFFFF;
	}

	/* 2. create module */
	pModule = malloc(sizeof(UTOPIA_MODULE));
        if(pModule != NULL) //Coverity
        {
            memset(pModule, 0, sizeof(UTOPIA_MODULE));
            pModule->u32ModuleID = u32ModuleID;
            pModule->psModuleShm = pModuleShm;

            /* 3. create private of module */
            snprintf(privateName, sizeof(privateName)
			, "%s_PRI", moduleNames[u32ModuleID]);
            u32Status = shm_malloc(u32PrivateSize, privateName,
			&pPrivate, &(pModule->shmid_private.ID));
            if((u32Status == UTOPIA_STATUS_SUCCESS)||(u32Status == UTOPIA_STATUS_SHM_EXIST))  //Coverity
            {
                /* 4. initial str private */
                pModule->shmid_str.ID=0xFFFFFFFF;
                *ppModule = pModule;
            }
            else
            {
                printu("[utopia malloc error] Line: %d shm_malloc fail\n",__LINE__);
                u32Status = UTOPIA_STATUS_FAIL;
            }
        }
        else
        {
            printu("[utopia malloc error] Line: %d pModule == NULL\n",__LINE__);
            u32Status = UTOPIA_STATUS_FAIL;
        }
	return u32Status;
}

MS_U32 UtopiaModuleGetPrivate(void* pModule, void** ppPrivate)
{
	*ppPrivate = shmid2va(TO_MODULE_PTR(pModule)->shmid_private.ID);
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleSetSTRPrivate(void* pModule, MS_U32 u32STRPrivateSize)
{
    void* pPrivate = NULL;
    char privateName[50]; // FIXME: potential bug
    MS_U32 shmid_str=0xFFFFFFFF;
    MS_U32 u32Status = 0;

    /* check str private exist? */
    if (MSOS_BRANCH_PREDICTION_UNLIKELY(!pModule))
    {
		printu("[utopia param error] module pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
    if (MSOS_BRANCH_PREDICTION_UNLIKELY(0xFFFFFFFF != TO_MODULE_PTR(pModule)->shmid_str.ID))
    {
		printu("[utopia module error] set duplicated STR private of module: %s\n"
				, moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }

    /* create str private of module */
	snprintf(privateName, sizeof(privateName)
			, "%s_STR", moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
    u32Status = shm_malloc(u32STRPrivateSize, privateName,
			&pPrivate, &shmid_str);

    if((u32Status != UTOPIA_STATUS_SUCCESS)&&(u32Status != UTOPIA_STATUS_SHM_EXIST))  //Coverity
    {
        printu("[utopia malloc error] Line: %d shm_malloc fail\n",__LINE__);
    }
		
    TO_MODULE_PTR(pModule)->shmid_str.ID = shmid_str;

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleGetSTRPrivate(void* pModule, void** ppPrivate)
{
    /* check str private exist? */
    if (MSOS_BRANCH_PREDICTION_UNLIKELY(!pModule))
    {
		printu("[utopia param error] module pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }
    if (MSOS_BRANCH_PREDICTION_UNLIKELY(0xFFFFFFFF == TO_MODULE_PTR(pModule)->shmid_str.ID))
    {
		printu("[utopia module error] NULL STR private of module: %s\n"
				, moduleNames[TO_MODULE_PTR(pModule)->u32ModuleID]);
        RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }

    *ppPrivate = shmid2va(TO_MODULE_PTR(pModule)->shmid_str.ID);
    return UTOPIA_STATUS_SUCCESS;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 UtopiaModuleRegisterMdbNode(char sMdbNodeName[MDB_NODE_NAME_MAX], FUtopiaMdbIoctl fpMdbIoctl)
{
    printu("UtopiaModuleRegisterMdbNode: %s\n", sMdbNodeName);
    //todo
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaCreateMdbNode()
{
    //todo
    return UTOPIA_STATUS_SUCCESS;
}

void* UtopiaModuleGetLocalInstantList(MS_U32 u32ModuleID, void* pLastInstance)
{
    //todo
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 MdbPrint(MS_U64* u64ReqHdl, const char* fmt, ...)
{
    //todo
    return UTOPIA_STATUS_SUCCESS;
}
#endif

MS_U32  UtopiaModuleRegister(void* pModuleTmp)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_MODULE* pLocatedModule = NULL;

	/* check para. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pModule == NULL))
	{
		printu("[utopia param error] module pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	pLocatedModule = TO_MODULE_PTR(locate_module(pModule->u32ModuleID));
	if (pLocatedModule == NULL) /* module is not yet registered */
	{
		/* insert module into list head */
		printu("[utopia info] register module: %s\n"
				, moduleNames[pModule->u32ModuleID]);
		pModule->psNext = psUtopiaPrivate->psModuleHead;
		psUtopiaPrivate->psModuleHead = pModule;
		return UTOPIA_STATUS_SUCCESS;
	}
	else  /* module is already registered */
		printu("[utopia module warning] register duplicated module: %s\n"
				, moduleNames[pModule->u32ModuleID]);
		return UTOPIA_STATUS_SHM_EXIST;
}

MS_U32 UtopiaModuleSetupFunctionPtr(void* pModuleTmp
		, FUtopiaOpen fpOpen, FUtopiaClose fpClose, FUtopiaIOctl fpIoctl)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

	pModule->fpOpen = fpOpen;
	pModule->fpClose = fpClose;
	pModule->fpIoctl = fpIoctl;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleSetupSTRFunctionPtr(void* pModuleTmp, FUtopiaSTR fpSTR)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

    if (MSOS_BRANCH_PREDICTION_UNLIKELY(!pModuleTmp))
    {
		printu("[utopia param error] module pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
    }

    pModule->fpSTR = fpSTR;

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

MS_U32 UtopiaResourceCreate(char* resourceName
		, MS_U32 u32PrivateSize, void** ppResource)
{
	MS_U32 u32RetShmID, u32Status, u32Ret;
	UTOPIA_RESOURCE* pResource = NULL;
	char privateName[50]; // FIXME: potential bug
	void* pPrivate = NULL;

	/* 1. create resource */
	u32Status = shm_malloc(sizeof(UTOPIA_RESOURCE), resourceName,
			(void**)&pResource, &(u32RetShmID));
	if (u32Status == UTOPIA_STATUS_SUCCESS)
	{
		pResource->shmid_self.ID = u32RetShmID;
		pResource->shmid_next_resource.ID = 0xFFFFFFFF;
	}
	*ppResource = pResource;

	/* 2. create resource private */
	snprintf(privateName, sizeof(privateName), "%s_PRI", resourceName);
	u32Ret = shm_malloc(u32PrivateSize, privateName,
			&pPrivate, &(pResource->shmid_private.ID));

        if((u32Ret != UTOPIA_STATUS_SUCCESS)&&(u32Status != UTOPIA_STATUS_SHM_EXIST))  //Coverity
        {
            printu("[utopia malloc error] Line: %d shm_malloc fail\n",__LINE__);
        }

	return u32Status;
}

MS_U32 UtopiaResourceGetPrivate(void* pResource, void** ppPrivate)
{
	*ppPrivate = shmid2va(TO_RESOURCE_PTR(pResource)->shmid_private.ID);
	return 0;
}

/*
 * case 1: no resource pool -
 * 		--> create resource pool & attach resource to it. combine with case 2
 * case 2: resource pool exists, but no mathcing resource pool
 * 		--> create resource pool & attach resource to it. combine with case 1
 * case 3: resource pool exists, and there's matching resource pool
 *		--> attach resource to it
 */
MS_U32 UtopiaResourceRegister(void* pModuleTmp
		, void* pResourceTmp, MS_U32 u32RPoolID)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_RESOURCE* pResource = TO_RESOURCE_PTR(pResourceTmp);
	char rpoolName[50];
	MS_U32 u32Status;

	/* 1. deal with resource pool */
	UTOPIA_RESOURCE_POOL* pRPool = NULL;
	UTOPIA_RESOURCE_POOL* pRPoolPrev = NULL;
	locate_resource_pool(pModule, u32RPoolID, &pRPool, &pRPoolPrev);

	/* rpool semaphore has been created in this process */
	if (pRPool != NULL && pRPool->u32ResourcesSemaphore != 0)
	{
		printu("[utopia rpool warning] %s has already been established in this process%s\n"
				, moduleNames[pModule->u32ModuleID], pRPool->shmid_self.name);
		return UTOPIA_STATUS_SHM_EXIST ;
	}

	/*
	 * case 1: there's no resource pool
	 *		--> *ppRPool = NULL & *ppRPoolPrev = NULL
	 * case 2: at least one resource pool exists, but not the located one
	 * 		--> *ppRPool = NULL & *ppRPoolPrev != NULL
	 * case 3: at least one resource pool exists, and so is the located one
	 *		--> *ppRPool != NULL & ppRPoolPrev doesn't matter
	 */
	if (pRPool == NULL) { /* case 1 or 2 */
		MS_U32 u32RetRPoolShmID = 0;
		MS_U32 u32ModuleID = pModule->u32ModuleID;

		snprintf(rpoolName, sizeof(rpoolName), "%s_%d"
				, rpoolNames[u32ModuleID], (int)u32RPoolID);
		u32Status = shm_malloc(sizeof(UTOPIA_RESOURCE_POOL), rpoolName,
				(void**)&pRPool, &u32RetRPoolShmID);
		if (u32Status == UTOPIA_STATUS_SUCCESS)
		{
			pRPool->shmid_self.ID = u32RetRPoolShmID;
			strncpy(pRPool->shmid_self.name, rpoolName
					, sizeof(pRPool->shmid_self.name));
			pRPool->u32PoolID = u32RPoolID;
			pRPool->shmid_next_rpool.ID = 0xFFFFFFFF;
			pRPool->shmid_resource_head.ID = 0xFFFFFFFF;
			pRPool->u32MutexID = MsOS_CreateNamedMutex((MS_S8*)rpoolName);
			pRPool->u32ResourcesSemaphore = 0;
		}
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

	UTOPIA_RESOURCE* pTargetRes = NULL;
	UTOPIA_RESOURCE* pTargetResPrev = NULL;
	locate_resource(pRPool, pResource->shmid_self.ID
			, &pTargetRes, &pTargetResPrev);
	/* case 1: there's no resource in pool */
	/*		--> pTargetRes = NULL, pTargetResPrev = NULL */
	/* case 2: at least one resource exists, but not the located one */
	/*		--> pTargetRes = NULL, pTargetResPrev != NULL */
	/* case 3: at least one resource exists, and so is the located one */
	/*		--> pTargetRes != NULL, pTargetResPrev doesn't matter */
	if (pTargetRes == NULL) /* case 1 or 2 */
	{
		if (pTargetResPrev == NULL) /* case 1 */
		{
			pRPool->shmid_resource_head.ID = pResource->shmid_self.ID;
		}
		else /* case 2 */
		{
			pTargetResPrev->shmid_next_resource.ID = pResource->shmid_self.ID;
		}
		pRPool->u32ResourceTotal++;

		return UTOPIA_STATUS_SUCCESS;
	}
	else /* case 3 */
	{
		/* duplicated registration: it may be registered by other process */
		return UTOPIA_STATUS_SHM_EXIST ;
	}
}

MS_U32 UtopiaResourceTryObtain(void* pModuleTmp
		, MS_U32 u32RPoolID, void** ppResource)
{
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);
	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	UTOPIA_RESOURCE* pAvailResource = NULL;
	locate_resource_pool(pModule, u32RPoolID
			, &pLocatedRPool, &pLocatedRPoolPrev);

	if (!(pLocatedRPool->u32ResourceAvail))
		return UTOPIA_STATUS_NO_RESOURCE;
#if defined(DISABLE_SYSCALL)
    //flag =-1 : try lock. flag =0 : lock. others : not implement yet.
    MsOS_ObtainNamedSemaphore(pLocatedRPool->u32ResourcesSemaphore, -1);
#else
	obtain_sem(pLocatedRPool->u32ResourcesSemaphore);
#endif

	pAvailResource = next_resource(pLocatedRPool, E_TYPE_RESOURCE_POOL);
	MsOS_LockMutex(pLocatedRPool->u32MutexID, 0);
	while (pAvailResource != NULL)
	{
		if (!(pAvailResource->bInUse))
		{
			pAvailResource->bInUse = true;
			*ppResource	= pAvailResource;
			pLocatedRPool->u32ResourceAvail--;
			MsOS_UnlockMutex(pLocatedRPool->u32MutexID, 0);
			return UTOPIA_STATUS_SUCCESS;
		}
		else
		{
			pAvailResource = next_resource(pAvailResource, E_TYPE_RESOURCE);
		}
	}
	MsOS_UnlockMutex(pLocatedRPool->u32MutexID, 0);

	printu("[utopia resource error] code flow should not reach here\n");
	RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
}

MS_U32 UtopiaResourceObtain(void* pModTmp, MS_U32 u32RPoolID, void** ppResource)
{
//	MS_S32 ret; /* check semop return value */ //coverity ret unused

#if DEADLOCK_DEBUG == 1
	MS_U32 count = 0;
	while(UTOPIA_STATUS_NO_RESOURCE ==
			UtopiaResourceTryObtain(pModTmp, u32RPoolID, ppResource))
	{
		usleep(1000);
		count++;
		if ((count >= bt_threshold) && ((count % bt_period) == 0))
		{
			printu("%s deadlock!!!???\n",
					moduleNames[TO_MODULE_PTR(pModTmp)->u32ModuleID]);
			print_trace ();
		}
	}
	return 0;
#endif

	/* check param. */
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(pModTmp == NULL))
	{
		printu("[utopia param error] module pointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(ppResource == NULL))
	{
		printu("[utopia param error] resource ppointer should not be null\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModTmp);
	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	UTOPIA_RESOURCE* pAvailResource = NULL;
	locate_resource_pool(pModule, u32RPoolID
			, &pLocatedRPool, &pLocatedRPoolPrev);
	if (MSOS_BRANCH_PREDICTION_UNLIKELY(!pLocatedRPool))
	{
		printu("[utopia param error] non-registered rpool id?\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

#if defined(DISABLE_SYSCALL)
    //flag =-1 : try lock. flag =0 : lock. others : not implement yet.
        if (MSOS_BRANCH_PREDICTION_UNLIKELY(FALSE == MsOS_ObtainNamedSemaphore(pLocatedRPool->u32ResourcesSemaphore, 0)))//coverity
	{
        printu("[utopia] obtain sem error :%d.\n", errno);
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}
#else
retry:
//	ret = obtain_sem(pLocatedRPool->u32ResourcesSemaphore);
//	if (ret < 0) /* -1 for error */
        if (0 > obtain_sem(pLocatedRPool->u32ResourcesSemaphore))//coverity
	{
		switch(errno)
		{
			case EINTR:
				goto retry;
			default:
				printu("[utopia error] unexpected errno: %d\n", errno);
				RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
		}
	}
#endif

	pAvailResource = next_resource(pLocatedRPool, E_TYPE_RESOURCE_POOL);
	MsOS_LockMutex(pLocatedRPool->u32MutexID, 0);
	while (pAvailResource != NULL)
	{
		if (!(pAvailResource->bInUse))
		{
			pAvailResource->bInUse = true;
			*ppResource	= pAvailResource;
			pLocatedRPool->u32ResourceAvail--;
			MsOS_UnlockMutex(pLocatedRPool->u32MutexID, 0);
			return UTOPIA_STATUS_SUCCESS;
		}
		else
		{
			pAvailResource = next_resource(pAvailResource, E_TYPE_RESOURCE);
		}
	}
	MsOS_UnlockMutex(pLocatedRPool->u32MutexID, 0);

	printu("[utopia error] code flow should not reach here\n");
	RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
}

MS_U32 UtopiaResourceRelease(void* pResource)
{
	UTOPIA_RESOURCE_POOL* pRPool
		= RESOURCE_PTR_TO_RPOOL_PTR(TO_RESOURCE_PTR(pResource));
        if(MSOS_BRANCH_PREDICTION_UNLIKELY(pRPool == NULL)) //Coverity
        {
            printu("[utopia error] Line: %d pRPool == NULL\n",__LINE__);
            RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
        }
	/* modify resource before releasing it, to avoid race condition */
	MsOS_LockMutex(pRPool->u32MutexID, 0);
	TO_RESOURCE_PTR(pResource)->bInUse = false;
	pRPool->u32ResourceAvail++;
#if defined(DISABLE_SYSCALL)
    MsOS_ReleaseNamedSemaphore(pRPool->u32ResourcesSemaphore, 0);
#else
	release_sem(pRPool->u32ResourcesSemaphore);
#endif
	MsOS_UnlockMutex(pRPool->u32MutexID, 0);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleAddResourceStart(void* psModuleTmp, MS_U32 u32PoolID)
{
	return UTOPIA_STATUS_SUCCESS;
}

#define PERMS 0666
#define SEM_NUM 1
/* semaphore num isn't equal to resource count */
static MS_U32 set_rpool_sem(void* pModuleTmp, MS_U32 u32RPoolID, MS_U32 reset)
{
#if defined(DISABLE_SYSCALL)
	MS_S32 s32SemaphoreID;
#else
	MS_U32 u32SemaphoreID;
#endif
	UTOPIA_MODULE* pModule = TO_MODULE_PTR(pModuleTmp);

	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	locate_resource_pool(pModule, u32RPoolID
			, &pLocatedRPool, &pLocatedRPoolPrev);

	if (!reset && /* no need to check for reset case */
			pLocatedRPool->u32ResourcesSemaphore != 0 &&
			pLocatedRPool->u32ResourcesSemaphore != 0xFFFFFFFF)
		return UTOPIA_STATUS_RPOOL_ESTABLISHED;

#if defined(DISABLE_SYSCALL)
    if (reset)
    {
        if (MSOS_BRANCH_PREDICTION_LIKELY(TRUE == MsOS_ResetNamedSemaphore(pLocatedRPool->u32ResourcesSemaphore)))
            pLocatedRPool->u32ResourceAvail = pLocatedRPool->u32ResourceTotal;
        else
        {
            printu("[utopia semaphore error] reset semaphore fail.\n");
            RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
        }
    }
    else
    {
        s32SemaphoreID = MsOS_CreateNamedSemaphore(pLocatedRPool->shmid_self.name, pLocatedRPool->u32ResourceTotal);
        if (MSOS_BRANCH_PREDICTION_UNLIKELY(s32SemaphoreID < 0))
        {
            printu("[utopia semaphore error] create semaphore fail.\n");
            RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
        }
    	pLocatedRPool->u32ResourceAvail = pLocatedRPool->u32ResourceTotal;
    	pLocatedRPool->u32ResourcesSemaphore = s32SemaphoreID;
    }
#else
	MS_U32 u32ShmId, u32BufSize, u32Ret;
	MS_VIRT vaShm;
	u32Ret = MsOS_SHM_GetId((MS_U8*)"sem_key_start", sizeof(MS_U32)
			, &u32ShmId, &vaShm, &u32BufSize, MSOS_SHM_QUERY);
	MS_U32* pu32SemKey = NULL;
	if (MSOS_BRANCH_PREDICTION_LIKELY(u32Ret == TRUE))
		pu32SemKey = (MS_U32*)vaShm;
	else {
		printu("[utopia semaphore error] sem_key_start@shm can't be found\n");
		RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
	}

	/*
	 * use sem key to create sem id
	 * IPC_CREATE: if sem id exists, return it
	 *             if sem id doesn't exist, create one & return it
	 * return 0 or 0xFFFFFFFF means fail, but i don't know why~
	 */
	u32SemaphoreID = semget(*pu32SemKey, 0, IPC_CREAT | PERMS); // why 0 here???
	if(u32SemaphoreID == 0 || (unsigned int)u32SemaphoreID == 0xFFFFFFFF)
    {

        u32SemaphoreID = semget(*pu32SemKey, SEM_NUM, IPC_CREAT | PERMS);
		/* create success, increase *pu32SemKey for next sem create */
        if ((u32SemaphoreID != 0 && (unsigned int)u32SemaphoreID != 0xFFFFFFFF))
        {
            (*pu32SemKey)++;
        }

        while(u32SemaphoreID == 0 || (unsigned int)u32SemaphoreID == 0xFFFFFFFF)
        {
			u32SemaphoreID = semget(*pu32SemKey, SEM_NUM, IPC_CREAT | PERMS);
            (*pu32SemKey)++;
        }
        if (MSOS_BRANCH_PREDICTION_UNLIKELY(u32SemaphoreID < 0))
        {
            printu("[utopia semaphore error] create semaphore fail\n");
			RET_OR_BLOCK(UTOPIA_STATUS_FAIL);
        }

		/* "0" is the semaphore id in sem. set which we got by semget */
        semctl(u32SemaphoreID, 0, SETVAL, pLocatedRPool->u32ResourceTotal);
    } else {
		(*pu32SemKey)++;
	}
	pLocatedRPool->u32ResourceAvail = pLocatedRPool->u32ResourceTotal;
	pLocatedRPool->u32ResourcesSemaphore = u32SemaphoreID;
#endif

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 UtopiaModuleAddResourceEnd(void* pModuleTmp, MS_U32 u32RPoolID)
{
	return (MS_U32)set_rpool_sem(pModuleTmp, u32RPoolID, 0);
}

MS_U32 UtopiaModuleResetPool(void* pModuleTmp, MS_U32 u32RPoolID)
{
	UTOPIA_RESOURCE_POOL* pLocatedRPool = NULL;
	UTOPIA_RESOURCE_POOL* pLocatedRPoolPrev = NULL;
	UTOPIA_RESOURCE* pAvailResource = NULL;

	locate_resource_pool(pModuleTmp, u32RPoolID
			, &pLocatedRPool, &pLocatedRPoolPrev);
	pAvailResource = next_resource(pLocatedRPool, E_TYPE_RESOURCE_POOL);
	MsOS_LockMutex(pLocatedRPool->u32MutexID, 0);
	while (pAvailResource != NULL)
	{
		pAvailResource->bInUse = false;
		pAvailResource = next_resource(pAvailResource, E_TYPE_RESOURCE);
	}
	MsOS_UnlockMutex(pLocatedRPool->u32MutexID, 0);

	return set_rpool_sem(pModuleTmp, u32RPoolID, 1);
}

MS_U32 UtopiaSetIPAUTH(ST_IPAUTH_SHARED_VARS *IpControl, MS_U8 *gCusID, MS_U8 *gCusHash)
{
    MS_S32 s32UtopiaFd;
    UTOPIA_AUTH_IPCHECK_ARG* sIPCheckArg;
    UTOPIA_AUTH_HASHINFO_ARG* sHashInfoArg;

    s32UtopiaFd = open("/proc/utopia", O_RDWR);
    if(0 > s32UtopiaFd)
    {
        printf("open /proc/utopia fail\n");
        close(s32UtopiaFd);
        return UTOPIA_STATUS_FAIL;
    }
    sIPCheckArg=malloc(sizeof(UTOPIA_AUTH_IPCHECK_ARG));
    if(sIPCheckArg == NULL)//coverity
    {
        printf("sIPCheckArg=malloc(sizeof(UTOPIA_AUTH_IPCHECK_ARG)) fail\n");
        close(s32UtopiaFd);
        return UTOPIA_STATUS_FAIL;
    }
    memcpy(sIPCheckArg,IpControl,sizeof(UTOPIA_AUTH_IPCHECK_ARG));
    if(ioctl(s32UtopiaFd,UTOPIA_IOCTL_AUTH_SetIPCheck,sIPCheckArg))//coverity
    {
        printf("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
        free(sIPCheckArg);
        close(s32UtopiaFd);
        return UTOPIA_STATUS_FAIL;
    }

    sHashInfoArg=malloc(sizeof(UTOPIA_AUTH_HASHINFO_ARG));
    if(sHashInfoArg == NULL)//coverity
    {
        printf("sHashInfoArg=malloc(sizeof(UTOPIA_AUTH_HASHINFO_ARG)) fail\n");
        free(sIPCheckArg);
        close(s32UtopiaFd);
        return UTOPIA_STATUS_FAIL;
    }
    memcpy(sHashInfoArg->u8CusID,gCusID,sizeof(sHashInfoArg->u8CusID));
    memcpy(sHashInfoArg->u8CusHash,gCusHash,sizeof(sHashInfoArg->u8CusHash));
    if(ioctl(s32UtopiaFd,UTOPIA_IOCTL_AUTH_SetHashInfo,sHashInfoArg))//coverity
    {
        printf("[%s][%d] Fail!!!!\n",__FUNCTION__,__LINE__);
        free(sIPCheckArg);
        free(sHashInfoArg);
        close(s32UtopiaFd);
        return UTOPIA_STATUS_FAIL;
    }

    close(s32UtopiaFd);
    free(sIPCheckArg);
    free(sHashInfoArg);
    return 0;
}


MS_U32 UtopiaModuleQueryMode(MS_U32 u32ModuleID)
{
    if(moduleMode[u32ModuleID]&KERNEL_MODE)
        return KERNEL_MODE;
    else
        return 0;
}
