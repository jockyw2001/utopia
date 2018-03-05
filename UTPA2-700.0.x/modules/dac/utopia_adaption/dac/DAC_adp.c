#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/compat.h>

#include "MsTypes.h"
//#include <stdio.h>
//#include "utopia_DDI.h"
#include "utopia.h"
#include "apiDAC.h"
#include "apiDAC_v2.h"

#include "utopia_adp.h"
#include "DAC_adp.h"

//Function parameter
UADP_STRUCT_POINTER_TABLE spt_DAC_GET_LIBVER[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_GET_INFO[5];
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
UADP_STRUCT_POINTER_TABLE spt_DAC_GET_OUTPUTINTERLACETIMING[5];
#endif
UADP_STRUCT_POINTER_TABLE spt_DAC_GET_STATUS[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_DBG_LEVEL[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_INIT[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_ENABLE[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_CLKINV[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_YPBPR_OUTPUTTIMING[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_OUTPUT_SOURCE[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_OUTPUT_LEVEL[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_OUTPUT_SWAPSEL[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_ONOFF_SD[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_ONOFF_HD[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_CLKSEL[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_DUMP_TABLE[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_IHALF_OUTPUT[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_QUART_OUTPUT[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_DAC_STATE[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_HOTPLUG_DETECT[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_POWER_MODE[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_WSSONOFF[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_RESET_WSSDATA[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SET_WSSOUTPUT[5];

UADP_STRUCT_POINTER_TABLE spt_DAC_ApiStatus[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_ApiInfo[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_SETWSS_INFO[5];

UADP_STRUCT_POINTER_TABLE spt_DAC_MS_U8[5];
UADP_STRUCT_POINTER_TABLE spt_DAC_MS_BOOL[5];

extern UADP_STRUCT_POINTER_TABLE spt_MSIF_Version;

MS_U32 DAC_adp_Init(FUtopiaIOctl* pIoctl)
{
//set table
    MS_U32 temp = 0;

    //Function parameter
    pstDAC_GET_LIBVER pDAC_GET_LIBVER = (pstDAC_GET_LIBVER)temp;
    pstDAC_GET_INFO pDAC_GET_INFO = (pstDAC_GET_INFO)temp;
    //pstDAC_GET_OUTPUTINTERLACETIMING pDAC_GET_OUTPUTINTERLACETIMING = (pstDAC_GET_OUTPUTINTERLACETIMING*)temp;
    pstDAC_GET_STATUS pDAC_GET_STATUS = (pstDAC_GET_STATUS)temp;
    //pstDAC_SET_DBG_LEVEL pDAC_SET_DBG_LEVEL= (pstDAC_SET_DBG_LEVEL*)temp;
    //pstDAC_INIT pDAC_INIT = (pstDAC_INIT*)temp;
    //pstDAC_ENABLE pDAC_ENABLE = (pstDAC_ENABLE*)temp;
    //pstDAC_SET_CLKINV pDAC_SET_CLKINV = (pstDAC_SET_CLKINV*)temp;
    //pstDAC_SET_YPBPR_OUTPUTTIMING pDAC_SET_YPBPR_OUTPUTTIMING = (pstDAC_SET_YPBPR_OUTPUTTIMING*)temp;
    //pstDAC_SET_OUTPUT_SOURCE pDAC_SET_OUTPUT_SOURCE = (pstDAC_SET_OUTPUT_SOURCE*)temp;
    //pstDAC_SET_OUTPUT_LEVEL pXC_SET_OUTPUT_LEVEL = (pstDAC_SET_OUTPUT_LEVEL*)temp;
    //pstDAC_SET_OUTPUT_SWAPSEL pDAC_SET_OUTPUT_SWAPSEL = (pstDAC_SET_OUTPUT_SWAPSEL*)temp;
    //pstDAC_ONOFF_SD pDAC_ONOFF_SD = (pstDAC_ONOFF_SD*)temp;
    //pstDAC_ONOFF_HD pDAC_ONOFF_HD = (pstDAC_ONOFF_HD*)temp;
    //pstDAC_CLKSEL pDAC_CLKSEL = (pstDAC_CLKSEL*)temp;
    pstDAC_DUMP_TABLE pDAC_DUMP_TABLE = (pstDAC_DUMP_TABLE)temp;
    //pstDAC_SET_IHALF_OUTPUT pDAC_SET_IHALF_OUTPUT = (pstDAC_SET_IHALF_OUTPUT*)temp;
    //pstDAC_SET_QUART_OUTPUT pDAC_SET_QUART_OUTPUT = (pstDAC_SET_QUART_OUTPUT*)temp;
    //pstDAC_SET_DAC_STATE pDAC_SET_DAC_STATE = (pstDAC_SET_DAC_STATE*)temp;
    pstDAC_HOTPLUG_DETECT pDAC_HOTPLUG_DETECT = (pstDAC_HOTPLUG_DETECT)temp;
    //pstDAC_POWER_MODE pDAC_POWER_MODE = (pstDAC_POWER_MODE*)temp;
    pstDAC_SET_WSSOUTPUT pDAC_SET_WSSOUTPUT = (pstDAC_SET_WSSOUTPUT)temp;

    //Function parameter
    UADPBypassSetSPT(&spt_DAC_GET_LIBVER[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_GET_LIBVER));
    UADPBypassSetSPT(&spt_DAC_GET_LIBVER[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDAC_GET_LIBVER->ppVersion)) - (unsigned long)pDAC_GET_LIBVER), (unsigned long)&spt_MSIF_Version);
    UADPBypassSetSPT(&spt_DAC_GET_LIBVER[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_ApiInfo[0],UADP_SPT_SELF_SIZE,0, sizeof(DAC_ApiInfo));
    UADPBypassSetSPT(&spt_DAC_ApiInfo[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_GET_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_GET_INFO));
    UADPBypassSetSPT(&spt_DAC_GET_INFO[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDAC_GET_INFO->stReturnValue)) - (unsigned long)pDAC_GET_INFO), (unsigned long)&spt_DAC_ApiInfo);
    UADPBypassSetSPT(&spt_DAC_GET_INFO[2],UADP_SPT_END , 0, 0);

#ifdef UFO_XC_GETOUTPUTINTELACETIMING
    UADPBypassSetSPT(&spt_DAC_GET_OUTPUTINTERLACETIMING[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_GET_OUTPUTINTERLACETIMING));
    UADPBypassSetSPT(&spt_DAC_GET_OUTPUTINTERLACETIMING[1],UADP_SPT_END , 0, 0);
#endif

    UADPBypassSetSPT(&spt_DAC_ApiStatus[0],UADP_SPT_SELF_SIZE,0, sizeof(DAC_ApiStatus));
    UADPBypassSetSPT(&spt_DAC_ApiStatus[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_GET_STATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_GET_STATUS));
    UADPBypassSetSPT(&spt_DAC_GET_STATUS[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDAC_GET_STATUS->pDacStatus)) - (unsigned long)pDAC_GET_STATUS), (unsigned long)&spt_DAC_ApiStatus);
    UADPBypassSetSPT(&spt_DAC_GET_STATUS[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_DBG_LEVEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_DBG_LEVEL));
    UADPBypassSetSPT(&spt_DAC_SET_DBG_LEVEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_INIT));
    UADPBypassSetSPT(&spt_DAC_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_ENABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_ENABLE));
    UADPBypassSetSPT(&spt_DAC_ENABLE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_CLKINV[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_CLKINV));
    UADPBypassSetSPT(&spt_DAC_SET_CLKINV[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_YPBPR_OUTPUTTIMING[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_YPBPR_OUTPUTTIMING));
    UADPBypassSetSPT(&spt_DAC_SET_YPBPR_OUTPUTTIMING[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_OUTPUT_SOURCE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_OUTPUT_SOURCE));
    UADPBypassSetSPT(&spt_DAC_SET_OUTPUT_SOURCE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_OUTPUT_LEVEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_OUTPUT_LEVEL));
    UADPBypassSetSPT(&spt_DAC_SET_OUTPUT_LEVEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_OUTPUT_SWAPSEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_OUTPUT_SWAPSEL));
    UADPBypassSetSPT(&spt_DAC_SET_OUTPUT_SWAPSEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_ONOFF_SD[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_ONOFF_SD));
    UADPBypassSetSPT(&spt_DAC_ONOFF_SD[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_ONOFF_HD[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_ONOFF_HD));
    UADPBypassSetSPT(&spt_DAC_ONOFF_HD[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_CLKSEL[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_CLKSEL));
    UADPBypassSetSPT(&spt_DAC_CLKSEL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_MS_U8[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_U8));
    UADPBypassSetSPT(&spt_DAC_MS_U8[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_DUMP_TABLE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_DUMP_TABLE));
    UADPBypassSetSPT(&spt_DAC_DUMP_TABLE[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDAC_DUMP_TABLE->pDACTable)) - (unsigned long)pDAC_DUMP_TABLE), (unsigned long)&spt_DAC_MS_U8);
    UADPBypassSetSPT(&spt_DAC_DUMP_TABLE[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_IHALF_OUTPUT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_IHALF_OUTPUT));
    UADPBypassSetSPT(&spt_DAC_SET_IHALF_OUTPUT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_QUART_OUTPUT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_QUART_OUTPUT));
    UADPBypassSetSPT(&spt_DAC_SET_QUART_OUTPUT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_DAC_STATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_DAC_STATE));
    UADPBypassSetSPT(&spt_DAC_SET_DAC_STATE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_MS_BOOL[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_BOOL));
    UADPBypassSetSPT(&spt_DAC_MS_BOOL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_HOTPLUG_DETECT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_HOTPLUG_DETECT));
    UADPBypassSetSPT(&spt_DAC_HOTPLUG_DETECT[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDAC_HOTPLUG_DETECT->State)) - (unsigned long)pDAC_HOTPLUG_DETECT), (unsigned long)&spt_DAC_MS_BOOL);
    UADPBypassSetSPT(&spt_DAC_HOTPLUG_DETECT[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_POWER_MODE[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_POWER_MODE));
    UADPBypassSetSPT(&spt_DAC_POWER_MODE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_WSSONOFF[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_WSSONOFF));
    UADPBypassSetSPT(&spt_DAC_SET_WSSONOFF[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_RESET_WSSDATA[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_RESET_WSSDATA));
    UADPBypassSetSPT(&spt_DAC_RESET_WSSDATA[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SET_WSSOUTPUT[0],UADP_SPT_SELF_SIZE,0, sizeof(stDAC_SET_WSSOUTPUT));
    UADPBypassSetSPT(&spt_DAC_SET_WSSOUTPUT[1],UADP_SPT_POINTER_TO_NEXT, ((unsigned long)(&(pDAC_SET_WSSOUTPUT->SetWSS_Data)) - (unsigned long)pDAC_SET_WSSOUTPUT), (unsigned long)&spt_DAC_SETWSS_INFO);
    UADPBypassSetSPT(&spt_DAC_SET_WSSOUTPUT[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_DAC_SETWSS_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(DAC_SETWSS_INFO));
    UADPBypassSetSPT(&spt_DAC_SETWSS_INFO[1],UADP_SPT_END , 0, 0);

    *pIoctl= (FUtopiaIOctl)DAC_adp_Ioctl;

    return 0;

}

MS_U32 DAC_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0xFF;
    char buffer_arg[2048];
    switch(u32Cmd)
    {
        case E_DAC_GET_LIBVER:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_GET_LIBVER, spt_DAC_GET_LIBVER, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_GET_INFO:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_DAC_GET_INFO, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_GET_STATUS:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_GET_STATUS, spt_DAC_GET_STATUS, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_DBG_LEVEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_DBG_LEVEL, spt_DAC_SET_DBG_LEVEL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_INIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_DAC_INIT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_ENABLE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_ENABLE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_CLKINV:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_CLKINV, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_YPBPR_OUTPUTTIMING:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_YPBPR_OUTPUTTIMING, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_OUTPUT_SOURCE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_OUTPUT_SOURCE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_OUTPUT_LEVEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_OUTPUT_LEVEL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_OUTPUT_SWAPSEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_OUTPUT_SWAPSEL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_ONOFF_SD:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_ONOFF_SD, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_ONOFF_HD:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_ONOFF_HD, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_CLKSEL:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_CLKSEL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_DUMP_TABLE:
            //Attention! Dump table is not use extern table now, so we do not care copy table anymore
            //(or need calc size of table here?)
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_DUMP_TABLE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_EXIT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_IHALF_OUTPUT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_IHALF_OUTPUT, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_QUART_OUTPUT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_QUART_OUTPUT, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_DAC_STATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_DAC_STATE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_HOTPLUG_DETECT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_HOTPLUG_DETECT, spt_DAC_HOTPLUG_DETECT, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_POWER_STATE:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_POWER_MODE, spt_DAC_POWER_MODE, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_GET_GETOUTPUTINTERLACETIMING:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, NULL, spt_DAC_GET_OUTPUTINTERLACETIMING, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_WSSONOFF:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_WSSONOFF, spt_DAC_SET_WSSONOFF, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_RESET_WSSDATA:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_RESET_WSSDATA, spt_DAC_RESET_WSSDATA, buffer_arg,sizeof(buffer_arg));
            break;
        case E_DAC_SET_WSSOUTPUT:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs, spt_DAC_SET_WSSOUTPUT, spt_DAC_SET_WSSOUTPUT, buffer_arg,sizeof(buffer_arg));
            break;
        default:
            break;

    }

    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




