//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   mdrvPVR_IframeLUT.c
/// @brief  PVR IframeLUT API Interface
/// @author MStar Semiconductor,Inc.
/// @attention
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/errno.h>
#else
#include <stdio.h>
#include <string.h>
#endif

#include "MsTypes.h"
#include "drvPVR_IframeLUT_v2.h"
#include "drvPVR_IframeLUT_private.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

#define PVR_IFRAMELUT_DBGERR(_f,_a...)                    {if(_iframelutdbglevel >= E_PVR_IFRAMELUT_DBG_ERR) {printf(_f, ##_a);}}
#define PVR_IFRAMELUT_DBGINFO(_f,_a...)                   {if(_iframelutdbglevel >= E_PVR_IFRAMELUT_DBG_INFO){printf(_f, ##_a);}}

static EN_PVR_IFRAMELUT_DBGLEVEL _iframelutdbglevel = E_PVR_IFRAMELUT_DBG_INFO;

// This function will be called by Utopia2.0 framework
void PVR_IFRAMELUTRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // UtopiaModuleCreate and UtopiaResourceCreate would deal with share memory.
    //
    // Using mstar driver semutex in linux kernel to allocate and map share memory.
    // When create share memory, semutex allocate pages and add them to a linked-list in its fd's private data,
    // and then, mmap is called to use vm_insert_page to map these pages to static _pu8ShareMem.
    //
    // All utopia drivers' resources are on the same memory space _pu8ShareMem.
    // static _ShmHdr records information about _pu8ShareMem usage (current client number, current offset ...)
    //
    // client id != module, every resource has a client id.
    //
    // MsOS_SHM_Context[] is a table in _ShmHdr, when a resource is created (by using shm_malloc() -> MsOS_SHM_GetId(MSOS_SHM_CREATE)),
    // client number++, and a new entry is inserted into the MsOS_SHM_Context[].
    //
    // The created resource addr returned by MsOS_SHM_GetId(MSOS_SHM_CREATE) is _pu8ShareMem + u32Offset.
    // The created resource id returned by MsOS_SHM_GetId(MSOS_SHM_CREATE) is the table id (client id) of MsOS_SHM_Context[]
    // These 2 information would be stored in shm_info_table.
    //
    // When we want to get resource addr (pointer), use id to look-up shm_info_table,
    // just like shmid2va does.
    //
    PVR_IFRAMELUT_DBGINFO("[%s][%d] start\n", __FUNCTION__, __LINE__);
    // 1. create module and register it.
    void *pUtopiaModule = NULL;
    // malloc UTOPIA_MODULE,
    // and shm_malloc UTOPIA_MODULE_SHM and module private
    //
    // It seems that module private is unused.
    //
    // There could be lots of resource pools belong to a specific module, these resource pools consist of a linked-list.
    // UTOPIA_MODULE_SHM records the head of this linked-list.
    //
    // After UtopiaModuleCreate called, there is no resource pool in the module,
    // resource pool will be created when UtopiaResourceRegister called.
    UtopiaModuleCreate(MODULE_PVR_IFRAMELUT, 0, &pUtopiaModule);
    // psUtopiaPrivate is a linked-list stores all modules,
    // add the module we created to the head of this linked-list.
    UtopiaModuleRegister(pUtopiaModule);

    // 2. register functions to the created module, after registered here,
    // the UtopiaOpen/UtopiaIoctl/UtopiaClose function calls will be re-directed to these registered functions.
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)PVR_IFRAMELUTOpen, (FUtopiaClose)PVR_IFRAMELUTClose, (FUtopiaIOctl)PVR_IFRAMELUTIoctl);

    // 3. create resource and register it to a specific resource pool.
    void* psResource = NULL;
    // This function directly return SUCCESS
    UtopiaModuleAddResourceStart(pUtopiaModule, E_PVR_IFRAMELUT_RESOURCE_POOL_0);
    MS_U32 u32ResSize = 0;
    _MDrv_PVR_IframeLUT_GetCap(PVR_IFRAMELUT_CAP_RESOURCE_SIZE, (void*)(&u32ResSize));
    // Create a resource called "pvr_iframelut".
    // shm_malloc UTOPIA_RESOURCE and resource private.
    // resource private is where we store static PVR_IFRAMELUT_RESOURCE_PRIVATE
    UtopiaResourceCreate("pvr_iframelut", u32ResSize, &psResource);
    // Register "pvr_iframelut" resource to a resource pool called "E_PVR_IFRAMELUT_RESOURCE_POOL_0".
    // Apparently, there is no resource pool called "E_PVR_IFRAMELUT_RESOURCE_POOL_0",
    // so this resource pool will be created first.
    //
    // There could be lots of resource belong to a specific resource pool, resources are a linked-list in a resource pool.
    //
    // If cannot find "pvr_iframelut" resource in resource pool "E_PVR_IFRAMELUT_RESOURCE_POOL_0",
    // add "pvr_iframelut" resource to the head of this resource pool.
    //
    // We can create more resources and add it to the same resource pool.
    UtopiaResourceRegister(pUtopiaModule, psResource, E_PVR_IFRAMELUT_RESOURCE_POOL_0);

    // Create semaphore for a specific resource pool,
    // the value of this semaphore would be initialized to # of resources in this resource pool.
    UtopiaModuleAddResourceEnd(pUtopiaModule, E_PVR_IFRAMELUT_RESOURCE_POOL_0);
    PVR_IFRAMELUT_DBGINFO("[%s][%d] end\n", __FUNCTION__, __LINE__);
}

// u32ModuleVersion and pAttribute are for polymorphism.
// Using u32ModuleVersion or pAttribute to let function pointers point to different functions.
// It would result in same symbol, different version, different behavior.
MS_U32 PVR_IFRAMELUTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%d] start\n", __FUNCTION__, __LINE__);
    if(ppInstance == 0)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTOpen] ppInstance is NULL!\n");
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

	PVR_IFRAMELUT_INSTANT_PRIVATE *pInstancePri = NULL;

	// malloc UTOPIA_INSTANCE and instance private.
	// instance pricate is where stores all function pointers.
	UtopiaInstanceCreate(sizeof(PVR_IFRAMELUT_INSTANT_PRIVATE), ppInstance);

    // There is no where to register instance.
    // We store instance in a static variable by ourselves.

    // When UtopiaOpen is called, module would be linked to instance,
    // (store module in instance->psModule)
    // so we can get module from instance later.

    // setup functions in instance private, assign functions to function ptrs in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pInstancePri);
    pInstancePri->fpPVRIframeLUTInit = (IOCTL_PVR_IframeLUT_Init)_MDrv_PVR_IframeLUT_Init;
    pInstancePri->fpPVRIframeLUTOpen = (IOCTL_PVR_IframeLUT_Open)_MDrv_PVR_IframeLUT_Open;
    pInstancePri->fpPVRIframeLUTSetVdecType = (IOCTL_PVR_IframeLUT_SetVdecType)_MDrv_PVR_IframeLUT_SetVdecType;
    pInstancePri->fpPVRIframeLUTGetWritePtr = (IOCTL_PVR_IframeLUT_GetWritePtr)_MDrv_PVR_IframeLUT_GetWritePtr;
    pInstancePri->fpPVRIframeLUTClose = (IOCTL_PVR_IframeLUT_Close)_MDrv_PVR_IframeLUT_Close;
    pInstancePri->fpPVRIframeLUTExit = (IOCTL_PVR_IframeLUT_Exit)_MDrv_PVR_IframeLUT_Exit;

    PVR_IFRAMELUT_DBGINFO("[%s][%d] end\n", __FUNCTION__, __LINE__);
	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 PVR_IFRAMELUTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%d]\n", __FUNCTION__, __LINE__);
    if(pInstance == NULL)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] pInstance is NULL!\n");
        return UTOPIA_STATUS_FAIL;
    }

    MS_U32 u32Res = UTOPIA_STATUS_SUCCESS;
    PVR_IFRAMELUT_INSTANT_PRIVATE *pInstancePrivate = NULL;
	void *pModule = NULL;
    void *pResource = NULL;
    void *pResourcePrivate = NULL;

    PVR_IFRAMELUT_DBGINFO("[%s][%d] UtopiaInstanceGetPrivate\n", __FUNCTION__, __LINE__);
    // Get instance private from instance
    if(UtopiaInstanceGetPrivate(pInstance, (void*)&pInstancePrivate) != 0)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] UtopiaInstanceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    PVR_IFRAMELUT_DBGINFO("[%s][%d] UtopiaInstanceGetModule\n", __FUNCTION__, __LINE__);
    // Get module from instance
    if(UtopiaInstanceGetModule(pInstance, &pModule) != 0)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] UtopiaInstanceGetModule fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    PVR_IFRAMELUT_DBGINFO("[%s][%d] UtopiaResourceObtain\n", __FUNCTION__, __LINE__);
    // Get resource from module, semaphore--
    if(UtopiaResourceObtain(pModule, E_PVR_IFRAMELUT_RESOURCE_POOL_0, &pResource) != 0)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    PVR_IFRAMELUT_DBGINFO("[%s][%d] UtopiaResourceGetPrivate\n", __FUNCTION__, __LINE__);
    // Get resource private from resource
    if(UtopiaResourceGetPrivate(pResource, &pResourcePrivate) != 0)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] UtopiaResourceGetPrivatefail\n");
        UtopiaResourceRelease(pResource);
        return UTOPIA_STATUS_FAIL;
    }

    // Initialize resource private, and assign resource private pointer to static variable _lut_res
    if(_MDrv_PVR_IframeLUT_InitResource(pResourcePrivate) != E_PVR_IFRAMELUT_RESULT_OK)
    {
        PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Init share memory failed \n");
        UtopiaResourceRelease(pResource);
        return UTOPIA_STATUS_FAIL;
    }

	switch(u32Cmd)
    {
        // General API
        case E_MDrv_CMD_PVR_IframeLUT_Init:
            PVR_IFRAMELUT_DBGINFO("[%s][%d] CMD Init\n", __FUNCTION__, __LINE__);
            if(pInstancePrivate->fpPVRIframeLUTInit() != E_PVR_IFRAMELUT_RESULT_OK)
            {
                PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Init fail\n");
                u32Res = UTOPIA_STATUS_FAIL;
            }
            break;
        case E_MDrv_CMD_PVR_IframeLUT_Open:
            PVR_IFRAMELUT_DBGINFO("[%s][%d] CMD Open\n", __FUNCTION__, __LINE__);
            PVR_IFRAMELUT_OPEN_PARAM *pOpenParam = (PVR_IFRAMELUT_OPEN_PARAM *)pArgs;
            if(pInstancePrivate->fpPVRIframeLUTOpen(pOpenParam) != E_PVR_IFRAMELUT_RESULT_OK)
            {
                PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Open fail\n");
                u32Res = UTOPIA_STATUS_FAIL;
            }
            break;
        case E_MDrv_CMD_PVR_IframeLUT_SetVdecType:
            PVR_IFRAMELUT_DBGINFO("[%s][%d] CMD Set Vdec Type\n", __FUNCTION__, __LINE__);
            PVR_IFRAMELUT_SETVDECTYPE_PARAM *pSetVdecTypeParam = (PVR_IFRAMELUT_SETVDECTYPE_PARAM *)pArgs;
            if(pInstancePrivate->fpPVRIframeLUTSetVdecType(pSetVdecTypeParam->u32PvrEng, pSetVdecTypeParam->eVdecType) != E_PVR_IFRAMELUT_RESULT_OK)
            {
                PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Set Vdec Type fail\n");
                u32Res = UTOPIA_STATUS_FAIL;
            }
            break;
        case E_MDrv_CMD_PVR_IframeLUT_GetWritePtr:
            PVR_IFRAMELUT_DBGINFO("[%s][%d] CMD Get Write Ptr\n", __FUNCTION__, __LINE__);
            PVR_IFRAMELUT_GETWRITEPTR_PARAM *pGetWritePtrParam = (PVR_IFRAMELUT_GETWRITEPTR_PARAM *)pArgs;
            if(pInstancePrivate->fpPVRIframeLUTGetWritePtr(pGetWritePtrParam->u32PvrEng, pGetWritePtrParam->pu32WritePtr) != E_PVR_IFRAMELUT_RESULT_OK)
            {
                PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Get Write Ptr fail\n");
                u32Res = UTOPIA_STATUS_FAIL;
            }
            break;
        case E_MDrv_CMD_PVR_IframeLUT_Close:
            PVR_IFRAMELUT_DBGINFO("[%s][%d] CMD Close\n", __FUNCTION__, __LINE__);
            PVR_IFRAMELUT_CLOSE_PARAM *pCloseParam = (PVR_IFRAMELUT_CLOSE_PARAM *)pArgs;
            if(pInstancePrivate->fpPVRIframeLUTClose(*pCloseParam) != E_PVR_IFRAMELUT_RESULT_OK)
            {
                PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Close fail\n");
                u32Res = UTOPIA_STATUS_FAIL;
            }
            break;
        case E_MDrv_CMD_PVR_IframeLUT_Exit:
            PVR_IFRAMELUT_DBGINFO("[%s][%d] CMD Exit\n", __FUNCTION__, __LINE__);
            if(pInstancePrivate->fpPVRIframeLUTExit() != E_PVR_IFRAMELUT_RESULT_OK)
            {
                PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Exit fail\n");
                u32Res = UTOPIA_STATUS_FAIL;
            }
            break;
        default:
            PVR_IFRAMELUT_DBGERR("[PVR_IframeLUTIoctl] Unknown command!\n");
            u32Res = UTOPIA_STATUS_NOT_SUPPORTED;
            break;
    }
    // semaphore++
    UtopiaResourceRelease(pResource);
	return u32Res;
}

MS_U32 PVR_IFRAMELUTClose(void* pInstance)
{
    PVR_IFRAMELUT_DBGINFO("[%s][%d]\n", __FUNCTION__, __LINE__);
	UtopiaInstanceDelete(pInstance);
	return UTOPIA_STATUS_SUCCESS;
}
