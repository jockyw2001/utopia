#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvSAR.h"
#include "drvSAR_v2.h"
#include "SAR_adp.h"
#include "utopia_adp.h"


#include <linux/uaccess.h>

#ifdef CONFIG_MSTAR_PORTING_KERNEL_4_4_3
extern void *spt_MS_U8;
#else
extern spt_MS_U8;
#endif /* CONFIG_MSTAR_PORTING_KERNEL_4_4_3 */

// no pointer member
UADP_SPT_0NXT_DEF(SAR_PRIVATE_PARAM_Kpd_GetKeyCode);
UADP_SPT_0NXT_DEF(SAR_KpdRegCfg);
// one pointer member
UADP_SPT_1NXT_DEF(SAR_PRIVATE_PARAM_KeyConfig);
// two pointer member

#ifdef CONFIG_MSTAR_UTPA2K_SAR_ENABLE
MS_U32 SAR_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct
//set table
    UADP_SPT_0NXT(SAR_PRIVATE_PARAM_Kpd_GetKeyCode);
    UADP_SPT_0NXT(SAR_KpdRegCfg);
    UADP_SPT_1NXT(SAR_PRIVATE_PARAM_KeyConfig, pKeyRegCfg, SAR_KpdRegCfg);
    *pIoctl= (FUtopiaIOctl)SAR_adp_Ioctl;
    return 0;
}

MS_U32 SAR_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[1024];
    switch(u32Cmd)
    {

        case MDrv_CMD_Kpd_GetKeyCode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SAR_PRIVATE_PARAM_Kpd_GetKeyCode, spt_SAR_PRIVATE_PARAM_Kpd_GetKeyCode,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_SAR_Config:
        	  u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_SAR_PRIVATE_PARAM_KeyConfig, spt_SAR_PRIVATE_PARAM_KeyConfig,buffer_arg,sizeof(buffer_arg));
            break;
        default:
            break;
    }
	return u32Ret;
}
#endif



