#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvSEM.h"
#include "drvSEM_priv.h"
#include "SEM_adp.h"
#include "utopia_adp.h"


UADP_SPT_0NXT_DEF(SEM_GETRESOURCE_PARAM);
UADP_SPT_0NXT_DEF(SEM_FREERESOURCE_PARAM);
UADP_SPT_0NXT_DEF(SEM_RESETRESOURCE_PARAM);
UADP_SPT_0NXT_DEF(SEM_GETRESOURCEID_PARAM);
UADP_SPT_0NXT_DEF(SEM_GETLIBVER_PARAM);
UADP_SPT_0NXT_DEF(SEM_LOCK_PARAM);
UADP_SPT_0NXT_DEF(SEM_UNLOCK_PARAM);
UADP_SPT_0NXT_DEF(SEM_DELETE_PARAM);




MS_U32 SEM_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct 
//set table
    UADP_SPT_0NXT(SEM_GETRESOURCE_PARAM);
    UADP_SPT_0NXT(SEM_FREERESOURCE_PARAM);
    UADP_SPT_0NXT(SEM_RESETRESOURCE_PARAM);
    UADP_SPT_0NXT(SEM_GETRESOURCEID_PARAM);
    UADP_SPT_0NXT(SEM_GETLIBVER_PARAM);
    UADP_SPT_0NXT(SEM_LOCK_PARAM);
    UADP_SPT_0NXT(SEM_UNLOCK_PARAM);
    UADP_SPT_0NXT(SEM_DELETE_PARAM);
	*pIoctl= (FUtopiaIOctl)SEM_adp_Ioctl;
	return 0;	
}



typedef void (*IOCTL_TIMER_INIT) (void);
typedef void (*IOCTL_TIMER_EXIT) (void);
MS_U32 SEM_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[2048];
    switch(u32Cmd)
    {
        case MDrv_CMD_SEM_Get_Resource:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_GETRESOURCE_PARAM, spt_SEM_GETRESOURCE_PARAM ,buffer_arg,sizeof(buffer_arg));
        case MDrv_CMD_SEM_Free_Resource:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_FREERESOURCE_PARAM, spt_SEM_FREERESOURCE_PARAM ,buffer_arg,sizeof(buffer_arg));
        case MDrv_CMD_SEM_Reset_Resource:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_RESETRESOURCE_PARAM, spt_SEM_RESETRESOURCE_PARAM ,buffer_arg,sizeof(buffer_arg));
        case MDrv_CMD_SEM_Get_ResourceID:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_GETRESOURCEID_PARAM, spt_SEM_GETRESOURCEID_PARAM,buffer_arg,sizeof(buffer_arg)); 
        case MDrv_CMD_SEM_GetLibVer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_GETLIBVER_PARAM,spt_SEM_GETLIBVER_PARAM,buffer_arg,sizeof(buffer_arg)); 
        case MDrv_CMD_SEM_Lock:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_LOCK_PARAM, spt_SEM_LOCK_PARAM ,buffer_arg,sizeof(buffer_arg));
        case MDrv_CMD_SEM_Unlock:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_UNLOCK_PARAM, spt_SEM_UNLOCK_PARAM ,buffer_arg,sizeof(buffer_arg));
        case MDrv_CMD_SEM_Delete:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SEM_DELETE_PARAM, spt_SEM_DELETE_PARAM ,buffer_arg,sizeof(buffer_arg));
		default:
            break;
    }
	return u32Ret;
}