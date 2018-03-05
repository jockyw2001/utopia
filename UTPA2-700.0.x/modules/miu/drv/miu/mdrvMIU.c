#include "MsTypes.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>
#else
#include <stdio.h>
#endif
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvMIU.h"
#include "drvMIU_private.h"
#include "MsOS.h"
#include "ULog.h"

#define MIUDBG(x)
#define TAG_MIU "MIU"

enum
{
    MIU_POOL_ID_MIU0=0
} eMIUPoolID;

// this func will be call to init by utopia20 framework
void MIURegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_MIU, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)MIUOpen, (FUtopiaClose)MIUClose, (FUtopiaIOctl)MIUIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource.
    UtopiaModuleAddResourceStart(pUtopiaModule, MIU_POOL_ID_MIU0);
    // resource can alloc private for internal use, ex, MIU_RESOURCE_PRIVATE
    UtopiaResourceCreate("miu0", sizeof(MIU_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, MIU_POOL_ID_MIU0);

    // end function to add res
    UtopiaModuleAddResourceEnd(pUtopiaModule, MIU_POOL_ID_MIU0);
    // MIURegisterToUtopia only call once, so driver init code can put here,
    //MDrv_MIU_Init();
}

MS_U32 MIUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MIU_INSTANT_PRIVATE *pMIUPri = NULL;
    void *pMIUPriVoid = NULL;
    ULOGD(TAG_MIU, "\n[MIU INFO] miu open");
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, MIU_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(MIU_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pMIUPriVoid);
    pMIUPri = (MIU_INSTANT_PRIVATE*)pMIUPriVoid;

    pMIUPri->fpMIU_Init = (IOCTL_MIU_INIT)_MDrv_MIU_Init;
    pMIUPri->fpMIU_Mask_Req_OPM_R = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_OPM_R;
    pMIUPri->fpMIU_Mask_Req_DNRB_R = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_DNRB_R;
    pMIUPri->fpMIU_Mask_Req_DNRB_W = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_DNRB_W;
    pMIUPri->fpMIU_Mask_Req_DNRB_RW = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_DNRB_RW;
    pMIUPri->fpMIU_Mask_Req_SC_RW = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_SC_RW;
    pMIUPri->fpMIU_Mask_Req_MVOP_R = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_MVOP_R;
    pMIUPri->fpMIU_Mask_Req_MVD_R = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_MVD_R;
    pMIUPri->fpMIU_Mask_Req_MVD_W = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_MVD_W;
    pMIUPri->fpMIU_Mask_Req_MVD_RW = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_MVD_RW;
    pMIUPri->fpMIU_Mask_Req_AUDIO_RW = (IOCTL_MIU_MASK_IP)_MDrv_MIU_Mask_Req_AUDIO_RW;
    pMIUPri->fpMIU_Mask_Req = (IOCTL_MIU_MASK_REQ)_MDrv_MIU_MaskReq;
    pMIUPri->fpMIU_UnMask_Req = (IOCTL_MIU_MASK_REQ)_MDrv_MIU_UnMaskReq;
    pMIUPri->fpMIU_SetSsc = (IOCTL_MIU_SET_SSC)_MDrv_MIU_SetSsc;
    pMIUPri->fpMIU_SetSscValue = (IOCTL_MIU_SET_SSCVALUE)_MDrv_MIU_SetSscValue;
    pMIUPri->fpMIU_Protect = (IOCTL_MIU_PROTECT)_MDrv_MIU_Protect;
    pMIUPri->fpMIU_ProtectEx = (IOCTL_MIU_PROTECTEx)_MDrv_MIU_ProtectEx;
    pMIUPri->fpMIU_SelMIU = (IOCTL_MIU_SELMIU)_MDrv_MIU_SelMIU;
    pMIUPri->fpMIU_GetProtectInfo = (IOCTL_MIU_GETPROTECTINFO)_MDrv_MIU_GetProtectInfo;
    pMIUPri->fpMIU_SetGroupPriority = (IOCTL_MIU_SETGROUPPRIORITY)_MDrv_MIU_SetGroupPriority;
    pMIUPri->fpMIU_SetHighPriority = (IOCTL_MIU_SETHIGHPRIORITY)_MDrv_MIU_SetHPriorityMask;
    pMIUPri->fpMIU_PrintProtectInfo = (IOCTL_MIU_PRINTPROTECTINFO)_MDrv_MIU_PrintProtectInfo;
    pMIUPri->fpMIU_Dram_Size = (IOCTL_MIU_DRAM_SIZE)_MDrv_MIU_Dram_Size;
    pMIUPri->fpMIU_ProtectAlign = (IOCTL_MIU_PROTECTALIGN)_MDrv_MIU_ProtectAlign;
    pMIUPri->fpMIU_GetDramType = (IOCTL_MIU_GETDRAMTYPE)_MDrv_MIU_GetDramType;

	return UTOPIA_STATUS_SUCCESS;
}

// FIXME: why static?
MS_U32 MIUIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;
    PMIU_MIU_MASK pMIUMask;
    PMIU_MIUREQ_MASK pMIUMaskReq;
    PMIU_SET_SSC pMIUSetSsc;
    PMIU_SET_SSCVALUE pMIUSetSscValue;
    PMIU_PROTECT pMIUProtect;
    PMIU_PROTECTEx pMIUProtectEx;
    PMIU_SELMIU pMIUSelMIU;
    PMIU_GETPROTECTINFO pMIUGetProtectInfo;
    PMIU_SETGROUPPRIORITY pMIUSetGroupPri;
    PMIU_SETHIGHPRIORITY pMIUSetHighPri;
    PMIU_DRAM_SIZE pMIUDramSizePri;
    PMIU_PROTECT_ALIGN pMIUProtectAlignPri;    
    PMIU_GETDRAMTYPE pMIUGetDramTypePri;
    MS_BOOL bRet = FALSE;
    //MS_U32 u32Ret;

    //utopia_secure_check(pInstant->pPrivate); // FIXME: check what?

    //if (NULL == pArgs)
    //    return UTOPIA_STATUS_PARAMETER_ERROR;

    MIU_INSTANT_PRIVATE* psMIUInstPri = NULL;
    void* psMIUInstPriVoid = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psMIUInstPriVoid);
    psMIUInstPri = (MIU_INSTANT_PRIVATE*)psMIUInstPriVoid;

    switch(u32Cmd)
    {
        case MDrv_CMD_MIU_Init:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Init\n");
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
	            return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_Init();
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_Mask_Req_OPM_R:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_OPM_R\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
	            return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_OPM_R(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_DNRB_R:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_DNRB_R\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_DNRB_R(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_DNRB_W:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_DNRB_W\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_DNRB_W(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_DNRB_RW:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_DNRB_RW\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_DNRB_RW(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_SC_RW:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_SC_RW\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_SC_RW(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_MVOP_R:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_MVOP_R\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_MVOP_R(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_MVD_R:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_MVD_R\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_MVD_R(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_MVD_W:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_MVD_W\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_MVD_W(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_MVD_RW:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_MVD_RW\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_MVD_RW(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req_AUDIO_RW:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req_AUDIO_RW\n");
            pMIUMask = (PMIU_MIU_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req_AUDIO_RW(pMIUMask->u8Mask,pMIUMask->u8Miu);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Mask_Req:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Mask_Req\n");
            pMIUMaskReq = (PMIU_MIUREQ_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_Mask_Req(pMIUMaskReq->u8Miu,pMIUMaskReq->eClientID);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_UnMask_Req:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_UnMask_Req\n");
            pMIUMaskReq = (PMIU_MIUREQ_MASK)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_UnMask_Req(pMIUMaskReq->u8Miu,pMIUMaskReq->eClientID);
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_SetSsc:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_SetSsc\n");
            pMIUSetSsc = (PMIU_SET_SSC)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_SetSsc(pMIUSetSsc->u16Fmodulation,pMIUSetSsc->u16FDeviation,pMIUSetSsc->bEnable);
            pMIUSetSsc->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_SetSscValue:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_SetSscValue\n");
            pMIUSetSscValue = (PMIU_SET_SSCVALUE)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_SetSscValue(pMIUSetSscValue->u8MiuDev,pMIUSetSscValue->u16Fmodulation,pMIUSetSscValue->u16FDeviation,pMIUSetSscValue->bEnable);
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_Protect:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Protect\n");
            pMIUProtect = (PMIU_PROTECT)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_Protect(pMIUProtect->u8Blockx, pMIUProtect->pu8ProtectId, pMIUProtect->phy64Start, pMIUProtect->phy64End, pMIUProtect->bSetFlag);
            pMIUProtect->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_ProtectEx:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_ProtectEx\n");
            pMIUProtectEx = (PMIU_PROTECTEx)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_ProtectEx(pMIUProtectEx->u8Blockx, pMIUProtectEx->pu32ProtectId, pMIUProtectEx->phy64Start, pMIUProtectEx->phy64End, pMIUProtectEx->bSetFlag);
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_SelMIU:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_SelMIU\n");
            pMIUSelMIU = (PMIU_SELMIU)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_SelMIU(pMIUSelMIU->eClientID, pMIUSelMIU->eType);
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_GetProtectInfo:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_GetProtectInfo\n");
            pMIUGetProtectInfo = (PMIU_GETPROTECTINFO)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_GetProtectInfo(pMIUGetProtectInfo->u8MiuDev, pMIUGetProtectInfo->pInfo);
            pMIUGetProtectInfo->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_SetGroupPriority:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_SetGroupPriority\n");
            pMIUSetGroupPri = (PMIU_SETGROUPPRIORITY)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_SetGroupPriority(pMIUSetGroupPri->u8MiuDev, pMIUSetGroupPri->sPriority);
            pMIUSetGroupPri->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_SetHighPriority:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_SetHighPriority\n");
            pMIUSetHighPri = (PMIU_SETHIGHPRIORITY)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_SetHighPriority(pMIUSetHighPri->u8MiuDev,pMIUSetHighPri->eClientID,pMIUSetHighPri->bMask);
            pMIUSetHighPri->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;
        
        case MDrv_CMD_MIU_PrintProtectInfo:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_PrintProtectInfo\n");
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "Ioctl UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            psMIUInstPri->fpMIU_PrintProtectInfo();
            UtopiaResourceRelease(pResource);
            return UTOPIA_STATUS_SUCCESS;

        case MDrv_CMD_MIU_Dram_Size:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_Dram_Size\n");
            pMIUDramSizePri = (PMIU_DRAM_SIZE)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                ULOGE(TAG_MIU, "Ioctl UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_Dram_Size(pMIUDramSizePri->u8MiuDev, pMIUDramSizePri->DramSize);
            pMIUDramSizePri->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_GetDramType:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_GetDramType\n");
            pMIUGetDramTypePri = (PMIU_GETDRAMTYPE)pArgs;
            bRet = psMIUInstPri->fpMIU_GetDramType(pMIUGetDramTypePri->eMiu, pMIUGetDramTypePri->pType);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        case MDrv_CMD_MIU_ProtectAlign:
            MIUDBG("MIUIoctl - MDrv_CMD_MIU_ProtectAlign\n");
            pMIUProtectAlignPri = (PMIU_PROTECT_ALIGN)pArgs;
            // when calling func occpuy h/w, need to call UtopiaResourceObtainToInstant to obtain res to lock other process
            if(UtopiaResourceObtain(pModule, MIU_POOL_ID_MIU0, &pResource) != 0)
            {
                printf("UtopiaResourceObtainToInstant fail\n");
                return UTOPIA_STATUS_ERR_RESOURCE;
            }
            bRet = psMIUInstPri->fpMIU_ProtectAlign(pMIUProtectAlignPri->u32PageShift);
            pMIUProtectAlignPri->bRet = bRet;
            UtopiaResourceRelease(pResource);
            if (TRUE == bRet)
                return UTOPIA_STATUS_SUCCESS;
            else
                return UTOPIA_STATUS_FAIL;

        default:
            break;
    };

	return UTOPIA_STATUS_SUCCESS; // FIXME: error code
}

MS_U32 MIUClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}



