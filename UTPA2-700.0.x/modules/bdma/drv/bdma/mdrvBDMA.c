#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#include <stdio.h>
#else
#include <linux/string.h>
#include <linux/slab.h>
#endif
#include "MsTypes.h"
#include "utopia_dapi.h"
#include "drvBDMA.h"
#include "drvBDMA_private.h"
#include "MsOS.h"
#if (BDMA_UTOPIA20)
#include "utopia.h"
#endif

#include "ULog.h"

#define TAG_BDMA "BDMA"

enum
{
    BDMA_POOL_ID_BDMA0=0
} eBdmaPoolID;

// this func will be call to init by utopia20 framework
void BDMARegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_BDMA, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)BDMAOpen, (FUtopiaClose)BDMAClose, (FUtopiaIOctl)BDMAIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)BDMAStr);
    //UtopiaModuleSetSTRPrivate(pUtopiaModule, STRPrivateDataSize);
#endif

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, BDMA_POOL_ID_BDMA0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("bdma0", sizeof(BDMA_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, BDMA_POOL_ID_BDMA0);

    // when there are 2 ch bdma, call UtopiaResourceCreate for BDMA_POOL_ID_BDMA0 again, then we have 2 resource.
#if 0
    psResource = (UTOPIA_RESOURCE*)UtopiaResourceCreate("bdma1", sizeof(BDMA_RESOURCE_PRIVATE));
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, BDMA_POOL_ID_BDMA0);
#endif
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, BDMA_POOL_ID_BDMA0);
}

MS_U32 BDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    BDMA_INSTANT_PRIVATE *pBdmaPri = NULL;
    void *pBdmaPriVoid = NULL;
    //printf("\n[BDMA INFO] bdma open");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(BDMA_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pBdmaPriVoid);
    pBdmaPri = (BDMA_INSTANT_PRIVATE*)pBdmaPriVoid;
    pBdmaPri->fpBDMAMemCpy = (IOCTL_BDMA_MEMCPY)_MDrv_BDMA_MemCopy;
    pBdmaPri->fpBDMASearch = (IOCTL_BDMA_SEARCH)_MDrv_BDMA_Search;
    pBdmaPri->fpBDMACRC32 = (IOCTL_BDMA_CRC32)_MDrv_BDMA_CRC32;
    pBdmaPri->fpBDMAPatternFill = (IOCTL_BDMA_PATTERN_FILL)_MDrv_BDMA_PatternFill;
    pBdmaPri->fpBDMAFlashCopy2Dram = (IOCTL_BDMA_FLASHCOPY2DRAM)_MDrv_BDMA_FlashCopy2Dram;
#ifdef MOBF_ENABLE
    //if (u32ModuleVersion == 0x00010001)
    {
        pBdmaPri->fpBDMAMOBFSearch = (IOCTL_BDMA_MOBFSEARCH)_MDrv_BDMA_MOBFSearch;
    }
    //else (u32ModuleVersion == 0x00010002)
    {
        //pBdmaPri->fpBDMAMOBFSearch = (IOCTL_BDMA_MOBFSEARCH)MDrv_BDMA_MOBFSearch_V2;
    }
#endif
    pBdmaPri->fpBDMAGetStatus = (IOCTL_BDMA_GETSTATUS)_MDrv_BDMA_GetStatus;
    pBdmaPri->fpBDMAGetInfo = (IOCTL_BDMA_GETINFO)MDrv_BDMA_GetInfo;
    pBdmaPri->fpBDMAGetLibVer = (IOCTL_BDMA_GETLIBVER)MDrv_BDMA_GetLibVer;
    pBdmaPri->fpBDMAGetMinSize = (IOCTL_BDMA_GETMINSIZE)MDrv_BDMA_GetMinSize;
    pBdmaPri->fpBDMASetDbgLevel = (IOCTL_BDMA_SETDBGLEVEL)MDrv_BDMA_SetDbgLevel;
    pBdmaPri->fpBDMASetSPIOffsetForMCU = (IOCTL_BDMA_SETSPIOFFSETFORMCU)_MDrv_BDMA_SetSPIOffsetForMCU;
    pBdmaPri->fpBDMAWaitFlashDone = (IOCTL_BDMA_WAITFLASHDONE)_MDrv_BDMA_WaitFlashDone;
    pBdmaPri->fpBDMAInit = (IOCTL_BDMA_INIT)_MDrv_BDMA_Init;
    pBdmaPri->fpBDMACopyHnd = (IOCTL_BDMA_COPYHND)_MDrv_BDMA_CopyHnd;

	return UTOPIA_STATUS_SUCCESS;
}

// FIXME: why static?
MS_U32 BDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
	void* pResource = NULL;
    PBDMA_MEMCOPY_PARAM pMemCpyParam = NULL;
    PBDMA_SEARCH_PARAM pSearchParam = NULL;
    PBDMA_CRC32_PARAM pCRC32Param = NULL;
    PBDMA_PATTERN_FILL_PARAM pPattFillParam = NULL;
    PBDMA_FLASHCOPY2DRAM_PARAM pFlash2DramParam = NULL;
#ifdef MOBF_ENABLE
    PBDMA_MOBFSEARCH_PARAM pMOBFSearchParam = NULL;
#endif
    PBDMA_GETSTATUS_PARAM pGetStatusParam = NULL;
    PBDMA_GETINFO_PARAM pGetInfoParam = NULL;
    PBDMA_GETLIBVER_PARAM pGetLibVerParam = NULL;
    PBDMA_GETMINSIZE_PARAM pGetMinSize = NULL;
    PBDMA_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;
    BDMA_Info *pBDMAInfo;
    MS_U32 u32Ret;
    MS_PHY phy64SearchAddr;
    MS_U32 u32CRC32;

    //utopia_secure_check(pInstant->pPrivate); // FIXME: check what?


    BDMA_INSTANT_PRIVATE* psBDMAInstPri = NULL;
    void* psBDMAInstPriVoid = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psBDMAInstPriVoid);
    psBDMAInstPri = (BDMA_INSTANT_PRIVATE*)psBDMAInstPriVoid;

    switch(u32Cmd)
    {
        case MDrv_CMD_BDMA_Init:
        {
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;

            PBDMA_INIT_PARAM pParam = (PBDMA_INIT_PARAM)pArgs;
            u32Ret = psBDMAInstPri->fpBDMAInit(pParam->u64Miu1Base);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        }
        case MDrv_CMD_BDMA_CopyHnd:
        {
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;

            PBDMA_COPYHND_PARAM pParam = (PBDMA_COPYHND_PARAM)pArgs;
            u32Ret = psBDMAInstPri->fpBDMACopyHnd(pParam->phy64SrcAddr, pParam->phy64DstAddr, pParam->u32Len, pParam->eCpyType, pParam->u8OpCfg);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        }
        case MDrv_CMD_BDMA_MemCopy:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_MemCopy\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pMemCpyParam = (PBDMA_MEMCOPY_PARAM)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
	            return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret = psBDMAInstPri->fpBDMAMemCpy(pMemCpyParam->phy64SrcAddr,pMemCpyParam->phy64DstAddr, pMemCpyParam->u32Len);
            UtopiaResourceRelease(pResource);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        case MDrv_CMD_BDMA_Search:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_Search\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pSearchParam = (PBDMA_SEARCH_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            phy64SearchAddr = psBDMAInstPri->fpBDMASearch(pSearchParam->u32Addr, pSearchParam->u32Len, pSearchParam->u32Pattern, pSearchParam->u32ExcluBit, pSearchParam->eDev);
            pSearchParam->pSearchAddr = phy64SearchAddr;
            UtopiaResourceRelease(pResource);
            if (0xFFFFFFFF != phy64SearchAddr)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        case MDrv_CMD_BDMA_CRC32:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_CRC32\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pCRC32Param = (PBDMA_CRC32_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32CRC32 = psBDMAInstPri->fpBDMACRC32(pCRC32Param->phy64Addr,pCRC32Param->u32Len,pCRC32Param->u32Poly,pCRC32Param->u32Seed,pCRC32Param->eDev,pCRC32Param->bReflect);
            UtopiaResourceRelease(pResource);
            pCRC32Param->u32CRC32 = u32CRC32;
            if (0xFFFFFFFF != u32CRC32)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        case MDrv_CMD_BDMA_Pattern_Fill:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_Pattern_Fill\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pPattFillParam = (PBDMA_PATTERN_FILL_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret = psBDMAInstPri->fpBDMAPatternFill(pPattFillParam->u32Addr,pPattFillParam->u32Len,pPattFillParam->u32Pattern,pPattFillParam->eDev);
            UtopiaResourceRelease(pResource);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        case MDrv_CMD_BDMA_FlashCopy2Dram:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_FlashCopy2Dram\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pFlash2DramParam = (PBDMA_FLASHCOPY2DRAM_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret = psBDMAInstPri->fpBDMAFlashCopy2Dram(pFlash2DramParam->u32FlashAddr,pFlash2DramParam->u32DramAddr,pFlash2DramParam->u32Len);
            UtopiaResourceRelease(pResource);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        case MDrv_CMD_BDMA_MOBFSearch:
#ifdef MOBF_ENABLE
            printf("BDMAIoctl - MDrv_CMD_BDMA_MOBFSearch\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pMOBFSearchParam = (PBDMA_MOBFSEARCH_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            phy64SearchAddr = psBDMAInstPri->fpBDMAMOBFSearch(pMOBFSearchParam->phy64Addr,pMOBFSearchParam->u32Len,pMOBFSearchParam->pMobfPsCfg,pMOBFSearchParam->eDev);
            UtopiaResourceRelease(pResource);
            pMOBFSearchParam->phy64SearchAddr = phy64SearchAddr;
            if (0xFFFFFFFF != phy64SearchAddr)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
#else
            return UTOPIA_STATUS_FAIL;
#endif
        case MDrv_CMD_BDMA_GetStatus:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_GetStatus\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pGetStatusParam = (PBDMA_GETSTATUS_PARAM)pArgs;
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psBDMAInstPri->fpBDMAGetStatus(pGetStatusParam->pStatus);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;
        case MDrv_CMD_BDMA_GetInfo:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_GetInfo\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            // this function won't occupy h/w, so don't need to call UtopiaResourceObtainToInstant
            pGetInfoParam = (PBDMA_GETINFO_PARAM)pArgs;
            pBDMAInfo = psBDMAInstPri->fpBDMAGetInfo();
            memcpy (pGetInfoParam,pBDMAInfo,sizeof(BDMA_Info));
            return UTOPIA_STATUS_SUCCESS;
        case MDrv_CMD_BDMA_GetLibVer:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_GetLibVer\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pGetLibVerParam = (PBDMA_GETLIBVER_PARAM)pArgs;
            u32Ret = psBDMAInstPri->fpBDMAGetLibVer(pGetLibVerParam->ppVersion);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        case MDrv_CMD_BDMA_GetMinSize:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_GetMinSize\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pGetMinSize = (PBDMA_GETMINSIZE_PARAM)pArgs;
            pGetMinSize->u32MinSize = (MS_U32)psBDMAInstPri->fpBDMAGetMinSize();
            return UTOPIA_STATUS_SUCCESS;
        case MDrv_CMD_BDMA_SetDbgLevel:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_SetDbgLevel\n");
            if (NULL == pArgs)
                return UTOPIA_STATUS_PARAMETER_ERROR;
            pSetDbgLevelParam = (PBDMA_SETDBGLEVEL_PARAM)pArgs;
            psBDMAInstPri->fpBDMASetDbgLevel(pSetDbgLevelParam->eLevel);
            return UTOPIA_STATUS_SUCCESS;
        case MDrv_CMD_BDMA_SetSPIOffsetForMCU:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_SetSPIOffsetForMCU\n");
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psBDMAInstPri->fpBDMASetSPIOffsetForMCU();
            UtopiaResourceRelease(pResource);
            break;
        case MDrv_CMD_BDMA_WaitFlashDone:
            //printf("BDMAIoctl - MDrv_CMD_BDMA_WaitFlashDone\n");
            if(UtopiaResourceObtain(pModule, BDMA_POOL_ID_BDMA0, &pResource) != 0)
            {
                ULOGE(TAG_BDMA, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            u32Ret = (MS_U32)psBDMAInstPri->fpBDMAWaitFlashDone();
            UtopiaResourceRelease(pResource);
            if (E_BDMA_OK == u32Ret)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        default:
            break;
    };

	return UTOPIA_STATUS_SUCCESS; // FIXME: error code
}

MS_U32 BDMAClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 BDMAStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
    }
    return u32Return;// for success
}
