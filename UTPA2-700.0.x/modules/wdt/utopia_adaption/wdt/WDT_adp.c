#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvWDT.h"
#include "drvWDT_private.h"
#include "WDT_adp.h"
#include "utopia_adp.h"


UADP_SPT_0NXT_DEF(WDT_INIT_PARAM);
UADP_SPT_0NXT_DEF(WDT_STOP_PARAM);
UADP_SPT_0NXT_DEF(WDT_SET_S_PARAM);
UADP_SPT_0NXT_DEF(WDT_SET_MS_PARAM);
UADP_SPT_0NXT_DEF(WDT_SET_US_PARAM);
UADP_SPT_0NXT_DEF(WDT_SETTIMER_PARAM);
UADP_SPT_0NXT_DEF(TIMER_COUNT_PARAM);
UADP_SPT_0NXT_DEF(TIMER_INT_PARAM);
UADP_SPT_0NXT_DEF(TIMER_HIT_MAX_MATCH_PARAM);
UADP_SPT_0NXT_DEF(TIMER_RST_PARAM);
UADP_SPT_0NXT_DEF(TIMER_SET_MAX_MATCH_PARAM);
UADP_SPT_0NXT_DEF(TIMER_GET_MAX_MATCH_PARAM);
UADP_SPT_0NXT_DEF(TIMER_GET_COUNTER_PARAM);
UADP_SPT_0NXT_DEF(TIMER_GET_SEC_PARAM);
UADP_SPT_0NXT_DEF(TIMER_GET_MS_PARAM);
UADP_SPT_0NXT_DEF(TIMER_GET_US_PARAM);
UADP_SPT_0NXT_DEF(TIMER_DELAY_PARAM);
UADP_SPT_0NXT_DEF(TIMER_DELAY_MS_PARAM);
UADP_SPT_0NXT_DEF(TIMER_DELAY_US_PARAM);



MS_U32 WDT_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct 
//set table
	UADP_SPT_0NXT(WDT_INIT_PARAM);
	UADP_SPT_0NXT(WDT_STOP_PARAM);
	UADP_SPT_0NXT(WDT_SET_S_PARAM);
	UADP_SPT_0NXT(WDT_SET_MS_PARAM);
	UADP_SPT_0NXT(WDT_SET_US_PARAM);
	UADP_SPT_0NXT(WDT_SETTIMER_PARAM);
	UADP_SPT_0NXT(TIMER_COUNT_PARAM);
	UADP_SPT_0NXT(TIMER_INT_PARAM);
	UADP_SPT_0NXT(TIMER_HIT_MAX_MATCH_PARAM);
	UADP_SPT_0NXT(TIMER_RST_PARAM);
	UADP_SPT_0NXT(TIMER_SET_MAX_MATCH_PARAM);
	UADP_SPT_0NXT(TIMER_GET_MAX_MATCH_PARAM);
	UADP_SPT_0NXT(TIMER_GET_COUNTER_PARAM);
	UADP_SPT_0NXT(TIMER_GET_SEC_PARAM);
	UADP_SPT_0NXT(TIMER_GET_MS_PARAM);
	UADP_SPT_0NXT(TIMER_GET_US_PARAM);
	UADP_SPT_0NXT(TIMER_DELAY_PARAM);
	UADP_SPT_0NXT(TIMER_DELAY_MS_PARAM);
	UADP_SPT_0NXT(TIMER_DELAY_US_PARAM);
	*pIoctl= (FUtopiaIOctl)WDT_adp_Ioctl;
	return 0;	
}



typedef void (*IOCTL_TIMER_INIT) (void);
typedef void (*IOCTL_TIMER_EXIT) (void);
MS_U32 WDT_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[2048];
    switch(u32Cmd)
    {
        case MDrv_CMD_WDT_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_INIT_PARAM, spt_WDT_INIT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_Stop:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_STOP_PARAM, spt_WDT_STOP_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_Clear:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_ClearRstFlag:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case  MDrv_CMD_WDT_IsReset:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_IsEnable:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_WDT_SetTimer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_SET_S_PARAM, spt_WDT_SET_S_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_SetTimer_ms:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_SET_MS_PARAM, spt_WDT_SET_MS_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_SetTimer_us:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_SET_US_PARAM, spt_WDT_SET_US_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_WDT_SetIntTimer:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_WDT_SETTIMER_PARAM, spt_WDT_SETTIMER_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_Init:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_INT_PARAM, spt_TIMER_INT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_Count:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_COUNT_PARAM, spt_TIMER_COUNT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_Rst:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_RST_PARAM, spt_TIMER_RST_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_SetMaxMatch:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_SET_MAX_MATCH_PARAM, spt_TIMER_SET_MAX_MATCH_PARAM,buffer_arg,sizeof(buffer_arg));
	     break;
        case MDrv_CMD_TIMER_HitMaxMatch:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_HIT_MAX_MATCH_PARAM, spt_TIMER_HIT_MAX_MATCH_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GetMaxMatch:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_GET_MAX_MATCH_PARAM, spt_TIMER_GET_MAX_MATCH_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GetCounter:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_GET_COUNTER_PARAM, spt_TIMER_GET_COUNTER_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GetSecond:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_GET_SEC_PARAM, spt_TIMER_GET_SEC_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GetMs:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_DELAY_MS_PARAM, spt_TIMER_DELAY_MS_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_GetUs:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_DELAY_US_PARAM, spt_TIMER_DELAY_US_PARAM,buffer_arg,sizeof(buffer_arg));
	     break;
        case MDrv_CMD_TIMER_Delay:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_DELAY_PARAM, spt_TIMER_DELAY_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
       case MDrv_CMD_TIMER_DelayMs:
	   	u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_DELAY_MS_PARAM, spt_TIMER_DELAY_MS_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_TIMER_DelayUs:
		u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_TIMER_DELAY_US_PARAM, spt_TIMER_DELAY_US_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
	 case MDrv_CMD_TIMER_Initial:
	 	u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
	     break;
	 case MDrv_CMD_TIMER_Exit:
	 	u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
	     break;
        default:
            break;
    }
	return u32Ret;
}

