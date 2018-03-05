#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvGPIO.h"
#include "drvGPIO_private.h"
#include "GPIO_adp.h"
#include "utopia_adp.h"


UADP_SPT_0NXT_DEF(GPIO_Status);
UADP_SPT_0NXT_DEF(GPIO_SET_HIGH_PARAM);
UADP_SPT_0NXT_DEF(GPIO_SET_LOW_PARAM);
UADP_SPT_0NXT_DEF(GPIO_SET_INPUT_PARAM);
UADP_SPT_0NXT_DEF(GPIO_GET_INOUT_PARAM);
UADP_SPT_0NXT_DEF(GPIO_GET_LEVEL_PARAM);
UADP_SPT_0NXT_DEF(GPIO_DETACH_INTERRUPT_PARAM);
UADP_SPT_0NXT_DEF(GPIO_ENABLE_INTERRUPT_PARAM);
UADP_SPT_0NXT_DEF(GPIO_DISABLE_INTERRUPT_PARAM);

UADP_SPT_0NXT_DEF(void);
//UADP_SPT_1NXT_DEF(GPIO_ATTACH_INTERRUPT_PARAM);

UADP_SDT_0_DEF(GPIO_STS_READ);
UADP_SDT_1_DEF(GPIO_PRIVATE_STS);

UADP_SDT_0_DEF(GPIO_CALL_BACK);
UADP_SDT_1_DEF(GPIO_ATTACH_PARAM);

MS_U32 GPIO_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct
//set table
	UADP_SPT_0NXT(GPIO_Status);
	UADP_SPT_0NXT(GPIO_SET_HIGH_PARAM);
	UADP_SPT_0NXT(GPIO_SET_LOW_PARAM);
	UADP_SPT_0NXT(GPIO_SET_INPUT_PARAM);
	UADP_SPT_0NXT(GPIO_GET_INOUT_PARAM);
	UADP_SPT_0NXT(GPIO_GET_LEVEL_PARAM);
	UADP_SPT_0NXT(GPIO_DETACH_INTERRUPT_PARAM);
	UADP_SPT_0NXT(GPIO_ENABLE_INTERRUPT_PARAM);
	UADP_SPT_0NXT(GPIO_DISABLE_INTERRUPT_PARAM);
	//UADP_SPT_1NXT(GPIO_ATTACH_INTERRUPT_PARAM, GPIO_Callback, void);
	UADP_SPT_0NXT(GPIO_DISABLE_INTERRUPT_PARAM);
	UADP_SDT_NAME0(GPIO_STS_READ,GPIO_Status);
	UADP_SDT_NAME1(GPIO_PRIVATE_STS,GPIO_GET_STS_PARAM,UADP_SDT_P2N,pSts,GPIO_STS_READ);
	UADP_SDT_NAME0(GPIO_CALL_BACK,void);
	UADP_SDT_NAME1(GPIO_ATTACH_PARAM,GPIO_ATTACH_INTERRUPT_PARAM,UADP_SDT_P2N,GPIO_Callback,GPIO_CALL_BACK);
	*pIoctl= (FUtopiaIOctl)GPIO_adp_Ioctl;
	return 0;
}

MS_U32 GPIO_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[2048];
    switch(u32Cmd)
    {
        case MDrv_CMD_GPIO_Init:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_GPIO_GetStauts:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_PRIVATE_STS, spt_GPIO_PRIVATE_STS,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Set_High:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_SET_HIGH_PARAM, spt_GPIO_SET_HIGH_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Set_Low:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_SET_LOW_PARAM, spt_GPIO_SET_LOW_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case  MDrv_CMD_GPIO_Set_Input:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_SET_INPUT_PARAM, spt_GPIO_SET_INPUT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Get_Inout:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_GET_INOUT_PARAM, spt_GPIO_GET_INOUT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Get_Level:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_GET_LEVEL_PARAM, spt_GPIO_GET_LEVEL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case  MDrv_CMD_GPIO_Attach_Interrupt:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_ATTACH_PARAM, spt_GPIO_ATTACH_PARAM,buffer_arg,sizeof(buffer_arg));        
            break;;
        case MDrv_CMD_GPIO_Detach_Interrupt:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_DETACH_INTERRUPT_PARAM, spt_GPIO_DETACH_INTERRUPT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_En_Interrupt:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_ENABLE_INTERRUPT_PARAM, spt_GPIO_ENABLE_INTERRUPT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Dis_Interrupt:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_GPIO_DISABLE_INTERRUPT_PARAM, spt_GPIO_DISABLE_INTERRUPT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_GPIO_Dis_Interrupt_All:
		u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_GPIO_En_Interrupt_All:
		u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
	     break;
        case MDrv_CMD_GPIO_Interrupt_Action:
		u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
	     break;
        default:
            break;
    }
	return u32Ret;
}

