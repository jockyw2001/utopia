#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvVIF.h"
#include "drvVIF_v2.h"

#include "VIF_adp.h"

#include "utopia_adp.h"
//#include "MsOS.h"

//#include <linux/kernel.h>

//Top
UADP_STRUCT_POINTER_TABLE bEnable[10];
UADP_STRUCT_POINTER_TABLE u8Value[10];
UADP_STRUCT_POINTER_TABLE u16Value[10];
UADP_STRUCT_POINTER_TABLE u32Value[10];

UADP_STRUCT_POINTER_TABLE spt_VIF_stParPtr[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_INIT[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_SETREGVALUE[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_SETPARA[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_VIFSoundSystem[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_IfFrequencyType[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_FrequencyBand[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_CopyToUser[10];
UADP_STRUCT_POINTER_TABLE spt_VIF_MSIF_Version[10];

//Bottom
UADP_STRUCT_POINTER_TABLE spt_DrvVIF_VIF_INITDATA[10];
UADP_STRUCT_POINTER_TABLE spt_DrvVIF_VIF_SETPARA[10];
UADP_STRUCT_POINTER_TABLE spt_DrvVIF_VIF_Version[10];


MS_U32 VIF_adp_Init(FUtopiaIOctl* pIoctl)
{
    //set table
    UADP_SPT_BGN(&spt_VIF_MSIF_Version[0], sizeof(MSIF_Version));
    UADP_SPT_FIN(&spt_VIF_MSIF_Version[1]);  

    UADP_SPT_BGN(&spt_VIF_stParPtr[0], sizeof(VIFUserFilter));    
    UADP_SPT_FIN(&spt_VIF_stParPtr[1]);
    
    UADP_SPT_BGN(&u8Value[0],sizeof(BYTE));
    UADP_SPT_FIN(&u8Value[1]);  
    UADP_SPT_BGN(&u16Value[0],sizeof(WORD));
    UADP_SPT_FIN(&u16Value[1]); 
    UADP_SPT_BGN(&u32Value[0],sizeof(DWORD));
    UADP_SPT_FIN(&u32Value[1]); 
    UADP_SPT_BGN(&bEnable[0],sizeof(BOOL));
    UADP_SPT_FIN(&bEnable[1]); 
    
    UADP_SPT_BGN(&spt_VIF_INIT[0], sizeof(VIFInitialIn));
    UADP_SPT_FIN(&spt_VIF_INIT[1]);

    UADP_SPT_BGN(&spt_VIF_SETREGVALUE[0], sizeof(VIF_SETREGVALUE));
    UADP_SPT_FIN(&spt_VIF_SETREGVALUE[1]);

    UADP_SPT_BGN(&spt_VIF_CopyToUser[0], sizeof(VIF_COPYTOUSER));
    UADP_SPT_FIN(&spt_VIF_CopyToUser[1]);

    UADP_SPT_BGN(&spt_VIF_VIFSoundSystem[0], sizeof(VIFSoundSystem));
    UADP_SPT_FIN(&spt_VIF_VIFSoundSystem[1]);

    UADP_SPT_BGN(&spt_VIF_IfFrequencyType[0], sizeof(IfFrequencyType));
    UADP_SPT_FIN(&spt_VIF_IfFrequencyType[1]);

    UADP_SPT_BGN(&spt_VIF_FrequencyBand[0], sizeof(FrequencyBand));
    UADP_SPT_FIN(&spt_VIF_FrequencyBand[1]);

    //bottom
    UADP_SPT_BGN(&spt_DrvVIF_VIF_INITDATA[0], sizeof(eVIF_INIT));
    UADP_SPT_NXT(&spt_DrvVIF_VIF_INITDATA[1], eVIF_INIT, pVIF_InitData, spt_VIF_INIT);
    UADP_SPT_FIN(&spt_DrvVIF_VIF_INITDATA[2]);

    UADP_SDT_BGN(&spt_DrvVIF_VIF_SETPARA[0], sizeof(VIF_SETPARA));
    UADP_SDT_KIND(&spt_DrvVIF_VIF_SETPARA[1], VIF_SETPARA, UADP_SDT_P2N, pVIF_Para, spt_VIF_stParPtr);
    UADP_SDT_FIN(&spt_DrvVIF_VIF_SETPARA[2]);
    
    *pIoctl= (FUtopiaIOctl)VIF_adp_Ioctl;

    return 0;
}

MS_U32 VIF_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[2048];
    
    switch(u32Cmd)
    {
        case Drv_CMD_VIF_Version:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        break;    
        case Drv_CMD_VIF_SetClock:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, bEnable, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_Init :
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DrvVIF_VIF_INITDATA, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_Exit:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        break;
        case Drv_CMD_VIF_Reset:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
        break;
        case Drv_CMD_VIF_Handler:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, bEnable, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_SetSoundSystem:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_VIF_VIFSoundSystem, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_SetIfFreq:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_VIF_IfFrequencyType, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_Read_CR_FOE:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_VIF_CopyToUser, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_Read_CR_LOCK_STATUS:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_VIF_CopyToUser, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_BypassDBBAudioFilter:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, bEnable, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_SetFreqBand :
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_VIF_FrequencyBand, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_GetInputLevelIndicator :
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_VIF_CopyToUser, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_SetParameter:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_DrvVIF_VIF_SETPARA, spt_DrvVIF_VIF_SETPARA, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_ShiftClk:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, u8Value, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_SetPowerState :
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_VIF_CopyToUser, spt_VIF_CopyToUser, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_WriteByte:
             u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_VIF_SETREGVALUE, NULL, buffer_arg, sizeof(buffer_arg));
        break;
        case Drv_CMD_VIF_ReadByte :
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_VIF_SETREGVALUE, spt_VIF_SETREGVALUE, buffer_arg, sizeof(buffer_arg));
        break;
        
        default:
        break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




