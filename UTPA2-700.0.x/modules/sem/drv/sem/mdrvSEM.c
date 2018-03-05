#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include <stdio.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "drvSEM.h"
#include "drvSEM_priv.h"
#include "MsOS.h"
#include "utopia.h"


enum
{
    SEM_POOL_ID_SEM0 = 0
} eSEM_PoolID;

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register module
//--------------------------------------------------------------------------------------------------
void SEMRegisterToUtopia(void)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_SEM, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)SEMOpen, (FUtopiaClose)SEMClose, (FUtopiaIOctl)SEMIoctl);

    // 2. Resource register
    void* psResource = NULL;
    
    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, SEM_POOL_ID_SEM0);

    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("sem0", sizeof(SEM_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, SEM_POOL_ID_SEM0);

    //UtopiaResourceCreate("sem1", sizeof(SEM_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, SEM_POOL_ID_SEM0);
    
    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, SEM_POOL_ID_SEM0);

    // Duplicated Init, check this(FIX LATER)
    //ULOGD(TAG_SEM, "\033[35mFunction = %s, Line = %d, SEM_Init is opened\033[m\n", __PRETTY_FUNCTION__, __LINE__);
    MDrv_SEM_Init();
   // ULOGE(TAG_SEM, "\033[35mFunction = %s, Line = %d, SEM Register finished\033[m\n", __PRETTY_FUNCTION__, __LINE__);
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use SEM
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 SEMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // for multi-process safe, check if already other Instance exist
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_SEM, &pModule);

    if(UtopiaResourceObtain(pModule, SEM_POOL_ID_SEM0, &pResource) != 0)
    {
        //ULOGE(TAG_SEM, "UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pSEMResPri = NULL;    
    UtopiaResourceGetPrivate(pResource, &pSEMResPri); 

//    #if defined (__aarch64__)
//        ULOGD(TAG_SEM, "\033[35mFunction = %s, Line = %d, current resource pri_shm content is: %lu\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(MS_VIRT *)pSEMResPri);
//    #else
//        ULOGD(TAG_SEM, "\033[35mFunction = %s, Line = %d, current resource pri_shm content is: %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(MS_VIRT *)pSEMResPri);
//    #endif 

    if(*(MS_U32 *)pSEMResPri == 0)
    {
        *(MS_U32 *)pSEMResPri = 1;
        UtopiaResourceRelease(pResource);
    }
    else
    {
        //ULOGE(TAG_SEM, "\033[35mFunction = %s, Line = %d, [SEM INFO] can not open an INSTANCE\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        UtopiaResourceRelease(pResource);
        return 1;
    }
#endif
    //ULOGE(TAG_SEM, "\033[35mFunction = %s, Line = %d, [SEM INFO] OPEN INSTANCE...\033[m\n", __PRETTY_FUNCTION__, __LINE__);
    SEM_INSTANT_PRIVATE *pSEMPri = NULL;

    // instance is allocated here, also can allocate private for internal use
    UtopiaInstanceCreate(sizeof(SEM_INSTANT_PRIVATE), ppInstance);
    // set the pSEMPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pSEMPri);
    
    // setup func in private and assign the calling func in func ptr in instance private
    pSEMPri->fpSEMGetResource = (IOCTL_SEM_GETRESOURCE)MDrv_SEM_Get_Resource_U2K;
    pSEMPri->fpSEMFreeResource = (IOCTL_SEM_FREERESOURCE)MDrv_SEM_Free_Resource_U2K;
    pSEMPri->fpSEMResetResource = (IOCTL_SEM_RESETRESOURCE)MDrv_SEM_Reset_Resource_U2K;
    pSEMPri->fpSEMGetResourceID = (IOCTL_SEM_GETRESOURCEID)MDrv_SEM_Get_ResourceID_U2K;
    pSEMPri->fpSEMLock = (IOCTL_SEM_LOCK)MDrv_SEM_Lock_U2K;
    pSEMPri->fpSEMUnlock = (IOCTL_SEM_UNLOCK)MDrv_SEM_Unlock_U2K;
    pSEMPri->fpSEMDelete = (IOCTL_SEM_DELETE)MDrv_SEM_Delete_U2K;

    return 0;
}

MS_U32 SEMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    SEM_INSTANT_PRIVATE* psSEMInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psSEMInstPri);

    PSEM_GETRESOURCE_PARAM pGetResource = NULL;
    PSEM_FREERESOURCE_PARAM pFreeResource = NULL;
    PSEM_RESETRESOURCE_PARAM pResetResource = NULL;
    PSEM_GETRESOURCEID_PARAM pGetResourceID = NULL;
    PSEM_GETLIBVER_PARAM pGetLibVer = NULL;
    PSEM_LOCK_PARAM pLock = NULL;
    PSEM_UNLOCK_PARAM pUnlock = NULL;
    PSEM_DELETE_PARAM pDelete = NULL;

    MS_U32 u32Ret;

    switch(u32Cmd)
    {
        case MDrv_CMD_SEM_Get_Resource:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Get_Resource\n");
            pGetResource = (PSEM_GETRESOURCE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMGetResource(pGetResource->u8SemID, pGetResource->u16ResId);
            return u32Ret;
        case MDrv_CMD_SEM_Free_Resource:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Free_Resource\n");
            pFreeResource = (PSEM_FREERESOURCE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMFreeResource(pFreeResource->u8SemID, pFreeResource->u16ResId);
            return u32Ret;
        case MDrv_CMD_SEM_Reset_Resource:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Reset_Resource\n");
            pResetResource = (PSEM_RESETRESOURCE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMResetResource(pResetResource->u8SemID);
            return u32Ret;
        case MDrv_CMD_SEM_Get_ResourceID:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Get_ResourceID\n");
            pGetResourceID = (PSEM_GETRESOURCEID_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMGetResourceID(pGetResourceID->u8SemID, pGetResourceID->pu16ResId);
            return u32Ret;
        case MDrv_CMD_SEM_Get_Num:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Get_Num\n");
            *((MS_U32 *)(pArgs)) = psSEMInstPri->fpSEMGetNum(); 
            return 0;
        case MDrv_CMD_SEM_GetLibVer:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_GetLibVer\n");
            pGetLibVer = (PSEM_GETLIBVER_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMGetLibVer(pGetLibVer->ppVersion);
            return u32Ret;
        case MDrv_CMD_SEM_Lock:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Lock\n");
            pLock = (PSEM_LOCK_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMLock(pLock->u32SemID, pLock->u32WaitMs);
            return u32Ret;
        case MDrv_CMD_SEM_Unlock:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Unlock\n");
            pUnlock = (PSEM_UNLOCK_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMUnlock(pUnlock->u32SemID);
            return u32Ret;
        case MDrv_CMD_SEM_Delete:
            //ULOGD(TAG_SEM, "SEMIoctl - MDrv_CMD_SEM_Delete\n");
            pDelete = (PSEM_DELETE_PARAM)pArgs;
            u32Ret = psSEMInstPri->fpSEMDelete(pDelete->u32SemID);
            return u32Ret;
        default:
            break;
    };

    return 1; // FIXME: error code
}

MS_U32 SEMClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_NUTTX)
    // Restore resource pri_shm content
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_SEM, &pModule);
    
    if(UtopiaResourceObtain(pModule, SEM_POOL_ID_SEM0, &pResource) != 0)
    {
        //ULOGE(TAG_SEM, "UtopiaResourceObtainToInstant fail\n");
        return 1;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pSEMResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pSEMResPri);
    
    if(*(MS_U32 *)pSEMResPri == 0)
    {
        //ULOGE(TAG_SEM, "\033[35mFunction = %s, Line = %d, [SEMClose] Strange resource pri_shm content!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        UtopiaResourceRelease(pResource);
        return 1;
    }
    else
    {
        //ULOGD(TAG_SEM, "\033[35mFunction = %s, Line = %d, [Multi-process Safe] Release an Instance!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        *(MS_U32 *)pSEMResPri = 0;
        UtopiaResourceRelease(pResource);
        return 0;
    }
#endif
    return 0;
}
