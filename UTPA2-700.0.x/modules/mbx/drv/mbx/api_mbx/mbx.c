#include "MsTypes.h"
#include "utopia.h"
#include <stdio.h>
#include <string.h>
#include "drvMBX.h"
//#include "apiMBX_St.h"
#include "mbx.h"
#include "utopia_dapi.h"
#include "apiMBX.h"
#include "MsOS.h"
#include "ULog.h"

#define TAG_MBX "MBX"

enum
{
    MBX_POOL_ID_MIU0=0
} eMBXPoolID;

void MBXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_MBX, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
	UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)MBXOpen, (FUtopiaClose)MBXClose, (FUtopiaIOctl)MBXIoctl);
	
    // 2. deal with resource

	// 2. deal with resource
    void* psResource = NULL;
	UtopiaModuleAddResourceStart(pUtopiaModule, MBX_POOL_ID_MIU0);
    UtopiaResourceCreate("MBX", sizeof(MBX_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, MBX_POOL_ID_MIU0);
	UtopiaModuleAddResourceEnd(pUtopiaModule, MBX_POOL_ID_MIU0);
}




MS_U32 MBXOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MBX_INSTANT_PRIVATE *pMbxPri = NULL;
	UtopiaInstanceCreate(sizeof(MBX_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pMbxPri);

	pMbxPri->fpRegisterMSGWithCallBack = MApi_MBX_RegisterMSGWithCallBack;
	pMbxPri->fpRegisterMSG = MApi_MBX_RegisterMSG;
	pMbxPri->fpSendMsg = MApi_MBX_SendMsg;
	pMbxPri->fpRecvMsg = MApi_MBX_RecvMsg;
	pMbxPri->fpEnable = MApi_MBX_Enable;
	pMbxPri->fpQueryDynamicClass = MApi_MBX_QueryDynamicClass;


	MBX_INSTANT_ATTRIBUTE* pMbxAttribue = (MBX_INSTANT_ATTRIBUTE*) pAttribute;

	MApi_MBX_Init(pMbxAttribue->eHKCPU, pMbxAttribue->eHostRole, pMbxAttribue->u32TimeoutMillSecs);

	return TRUE;
}



// FIXME: why static?
MS_U32 MBXIoctl(void** pInstance, MS_U32 u32Cmd, MS_U32* pu32Args)
{

    MBX_INSTANT_PRIVATE* psMbxInstPri ;
	void* pResource = NULL;
	void* pModule = NULL;
	
	UtopiaInstanceGetPrivate(pInstance, (void*)&psMbxInstPri);
	UtopiaInstanceGetModule(pInstance, &pModule);
	MBX_RESOURCE_PRIVATE *pMbxResPri = (MBX_RESOURCE_PRIVATE *) pu32Args;

	
    switch(u32Cmd)
    {
		case MDrv_CMD_MBX_RegisterMSG:
			 if(UtopiaResourceObtain(pModule, MBX_POOL_ID_MIU0, &pResource) != 0)
             {
                    ULOGE(TAG_MBX, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
             }

			psMbxInstPri->fpRegisterMSG(pMbxResPri->privateRegisterMSG.eMsgClass,
								pMbxResPri->privateRegisterMSG.u16MsgQueueSize);
			UtopiaResourceRelease(pResource);
			break;
		case MDrv_CMD_MBX_RegisterMSGWithCallBack:
			if(UtopiaResourceObtain(pModule, MBX_POOL_ID_MIU0, &pResource) != 0)
             {
                    ULOGE(TAG_MBX, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
             }

			psMbxInstPri->fpRegisterMSGWithCallBack(pMbxResPri->privateRegisterMSGWithCallBack.eMsgClass,
								pMbxResPri->privateRegisterMSGWithCallBack.u16MsgQueueSize,
								pMbxResPri->privateRegisterMSGWithCallBack.notifier);
			UtopiaResourceRelease(pResource);
			break;
		case MDrv_CMD_MBX_SendMsg:
			if(UtopiaResourceObtain(pModule, MBX_POOL_ID_MIU0, &pResource) != 0)
             {
                    ULOGE(TAG_MBX, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
             }
            	
			psMbxInstPri->fpSendMsg(pMbxResPri->privateSendMsg.pMsg);
			UtopiaResourceRelease(pResource);
			break;
		case MDrv_CMD_MBX_RecvMsg:
			if(UtopiaResourceObtain(pModule, MBX_POOL_ID_MIU0, &pResource) != 0)
             {
                    ULOGE(TAG_MBX, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
             }
            		
			psMbxInstPri->fpRecvMsg(pMbxResPri->privateRecvMsg.eTargetClass,
									pMbxResPri->privateRecvMsg.pMsg,
									pMbxResPri->privateRecvMsg.u32WaitMillSecs,
									pMbxResPri->privateRecvMsg.u32Flag);
			UtopiaResourceRelease(pResource);
			break;
		case MDrv_CMD_MBX_Enable:
			if(UtopiaResourceObtain(pModule, MBX_POOL_ID_MIU0, &pResource) != 0)
             {
                    ULOGE(TAG_MBX, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
             }
			
			ULOGD(TAG_MBX, "==Doyle== %s, bEnable=0x%x\n", __FUNCTION__, pMbxResPri->privateEnable.bEnable);
			psMbxInstPri->fpEnable(pMbxResPri->privateEnable.bEnable);

			UtopiaResourceRelease(pResource);
			break;
		case MDrv_CMD_MBX_QueryClass:
			if(UtopiaResourceObtain(pModule, MBX_POOL_ID_MIU0, &pResource) != 0)
             {
                    ULOGE(TAG_MBX, "UtopiaResourceObtainToInstant fail\n");
		            return UTOPIA_STATUS_ERR_NOT_AVAIL;
             }
            		
			psMbxInstPri->fpQueryDynamicClass(pMbxResPri->privateQClass.eRoleID,
											  pMbxResPri->privateQClass.name,
											  &pMbxResPri->privateQClass.mbx_class);

			UtopiaResourceRelease(pResource);
			break;
        default:
            break;
    };
	
	return 1; // FIXME: error code
}


MS_U32 MBXClose(void** pInstance)
{
	UtopiaInstanceDelete(pInstance);
	return TRUE;
}

