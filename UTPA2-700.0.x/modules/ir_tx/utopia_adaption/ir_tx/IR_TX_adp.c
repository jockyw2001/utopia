#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drv_IR_TX.h"
#include "drvIR_TX_private.h"
#include "IR_TX_adp.h"
#include "utopia_adp.h"



UADP_SPT_0NXT_DEF(IRTX_SETSTATUS_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETMEMSTATUS_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETCLKDIV_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETDELAYCLKTIME_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETMEMADDR_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETMEMDATA_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETUNITVALUE_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETSHOTCOUNT_PARAM);
UADP_SPT_0NXT_DEF(IRTX_SETCARRIERCOUNT_PARAM);
UADP_SPT_0NXT_DEF(IRTX_INIT_PARAM);

MS_U32 IR_TX_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct
//set table
        UADP_SPT_0NXT(IRTX_SETSTATUS_PARAM);
        UADP_SPT_0NXT(IRTX_SETMEMSTATUS_PARAM);
        UADP_SPT_0NXT(IRTX_SETCLKDIV_PARAM);
        UADP_SPT_0NXT(IRTX_SETDELAYCLKTIME_PARAM);
        UADP_SPT_0NXT(IRTX_SETMEMADDR_PARAM);
        UADP_SPT_0NXT(IRTX_SETMEMDATA_PARAM);
        UADP_SPT_0NXT(IRTX_SETUNITVALUE_PARAM);
        UADP_SPT_0NXT(IRTX_SETSHOTCOUNT_PARAM);
        UADP_SPT_0NXT(IRTX_SETCARRIERCOUNT_PARAM);
        UADP_SPT_0NXT(IRTX_INIT_PARAM);

        *pIoctl= (FUtopiaIOctl)IR_TX_adp_Ioctl;
        return 0;
}


MS_U32 IR_TX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[2048];
    switch(u32Cmd)
	{
		case MDrv_CMD_IR_TX_SetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETSTATUS_PARAM, spt_IRTX_SETSTATUS_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetMemStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETMEMSTATUS_PARAM, spt_IRTX_SETMEMSTATUS_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetClkDiv:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETCLKDIV_PARAM, spt_IRTX_SETCLKDIV_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetDelayCycleTime:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETDELAYCLKTIME_PARAM, spt_IRTX_SETDELAYCLKTIME_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetMemAddr:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETMEMADDR_PARAM, spt_IRTX_SETMEMADDR_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetMemData:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETMEMDATA_PARAM, spt_IRTX_SETMEMDATA_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetUnitValue:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETUNITVALUE_PARAM, spt_IRTX_SETUNITVALUE_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_SetShotCount:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETSHOTCOUNT_PARAM, spt_IRTX_SETSHOTCOUNT_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_INIT_PARAM, spt_IRTX_INIT_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		case MDrv_CMD_IR_TX_Trigger:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
		    break;

		case MDrv_CMD_IR_TX_SetCarrierCount:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_IRTX_SETCARRIERCOUNT_PARAM, spt_IRTX_SETCARRIERCOUNT_PARAM,buffer_arg,sizeof(buffer_arg));
		    break;

		default:
             break;
	}

        return u32Ret;
}


