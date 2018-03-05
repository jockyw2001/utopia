#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiACP.h"
#include "apiACP_private.h"
#include "apiACP_v2.h"

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
// for semaphore POOL


//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// this func will be call to init by utopia20 framework
void ACPRegisterToUtopia(void)
{
    MS_U32 u32ResourceStatusCheck[E_ACP_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_ACP, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)ACPOpen, (FUtopiaClose)ACPClose, (FUtopiaIOctl)ACPIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, E_ACP_POOL_ID_INTERNAL_VARIABLE);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    u32ResourceStatusCheck[E_ACP_POOL_ID_INTERNAL_VARIABLE] = UtopiaResourceCreate("acp", sizeof(ACP_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, E_ACP_POOL_ID_INTERNAL_VARIABLE);

	UtopiaModuleAddResourceEnd(pUtopiaModule, E_ACP_POOL_ID_INTERNAL_VARIABLE);

    //4. init resource private members here   (aka, global variable)
    ACP_RESOURCE_PRIVATE* pACPResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pACPResourcePrivate));

    //pXCResourcePrivate->bResourceRegistered will automatically cleared by UtopiaResourceRegister
    if (u32ResourceStatusCheck[E_ACP_POOL_ID_INTERNAL_VARIABLE] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pACPResourcePrivate->bResourceRegistered = TRUE;
    }
}

MS_U32 ACPOpen(void** ppInstance, const void* const pAttribute)
{
    ACP_INSTANT_PRIVATE *pACPPri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(ACP_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pACPPri);

    pACPPri->fpACPSetProtection = MApi_ACP_SetProtection_U2;
    pACPPri->fpACPSetMVBitControl = MApi_ACP_SetMV_BitControl_U2;
    pACPPri->fpACPDCSProtection = MApi_DCS_SetProtection_U2;
    pACPPri->fpACPDCSSetActivationKey = MApi_DCS_SetActivationKey_U2;
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 ACPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    if(pInstance == NULL)
    {
        printf("[%s] pInstance is NULL\n",__FUNCTION__);
        return UTOPIA_STATUS_FAIL;
    }
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    ACP_INSTANT_PRIVATE* psACPInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psACPInstPri);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    //printf("[%s] cmd:%lx\n",__FUNCTION__,u32Cmd);
    switch(u32Cmd)
    {
        case E_ACP_SET_PROTECTION:
        {
            pstACP_SET_PROTECTION ptr = (pstACP_SET_PROTECTION)pArgs;
            ptr->eReturnValue = psACPInstPri->fpACPSetProtection(ptr->bEnable,ptr->bIsYPbPr,ptr->u8Type);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_ACP_SET_MV_BITCONTROL:
        {
            pstACP_SET_MV_BITCONTROL ptr = (pstACP_SET_MV_BITCONTROL)pArgs;
            ptr->eReturnValue = psACPInstPri->fpACPSetMVBitControl(ptr->bEnable,ptr->MV_BitControl_Data);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_ACP_DCS_PROTECTION:
        {
            pstACP_DCS_PROTECTION ptr = (pstACP_DCS_PROTECTION)pArgs;
            ptr->eReturnValue = psACPInstPri->fpACPDCSProtection(ptr->bEnable,ptr->u8Type);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_ACP_DCS_SETACTIVATIONKEY:
        {
            pstACP_DCS_SETACTIVATIONKEY ptr = (pstACP_DCS_SETACTIVATIONKEY)pArgs;
            ptr->eReturnValue = psACPInstPri->fpACPDCSSetActivationKey(ptr->pu8ActivationKeyTbl,ptr->u8ActivationKeyTblSize);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        default:
            printf("[%s] out of ACP cmd:%d\n",__FUNCTION__,u32Cmd);
            break;
    }

	return u32Return;
}

MS_U32 ACPClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


