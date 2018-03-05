#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>


#include "MsTypes.h"
#include "utopia.h"
#include "drvBDMA.h"
#include "drvBDMA_v2.h"
#include "BDMA_adp.h"
#include "utopia_adp.h"


extern void* spt_MS_U32;
UADP_SPT_0NXT_DEF(BDMA_Status);
UADP_SPT_0NXT_DEF(BDMA_MOBF_PS);
UADP_SPT_0NXT_DEF(BDMA_CRC32_PARAM);
UADP_SPT_0NXT_DEF(BDMA_MEMCOPY_PARAM);
UADP_STRUCT_POINTER_TABLE spt_BDMA_INIT_PARAM[10];
UADP_STRUCT_POINTER_TABLE spt_BDMA_COPYHND_PARAM[10];

#ifdef MOBF_ENABLE
//UADP_SDT_0_DEF(MS_PHY_phy64Addr);
UADP_SDT_0_DEF(BDMA_MOBF_PS_pMobfPsCfg);
//UADP_SDT_0_DEF(MS_PHY_phy64SearchAddr);
UADP_SDT_1_DEF(BDMA_MOBFSEARCH_PARAM_PRIVATE);
#endif

UADP_SDT_0_DEF(MS_PHYADDR_u32Addr);
UADP_SDT_1_DEF(BDMA_SEARCH_PARAM_PRIVATE);

UADP_SDT_0_DEF(BDMA_Status_pStatus);
UADP_SDT_1_DEF(BDMA_GETSTATUS_PARAM_PRIVATE);

UADP_SDT_0_DEF(MS_PHYADDR_u32FlashAddr);
UADP_SDT_0_DEF(MS_PHYADDR_u32DramAddr);
UADP_SDT_2_DEF(BDMA_FLASHCOPY2DRAM_PARAM_PRIVATE);

//UADP_SDT_0_DEF(MS_PHY_phy64SrcAddr);
//UADP_SDT_0_DEF(MS_PHY_phy64DstAddr);
//UADP_SDT_2_DEF(BDMA_MEMCOPY_PARAM_PRIVATE);

UADP_SDT_0_DEF(MS_PHYADDR_u32Addr);
UADP_SDT_1_DEF(BDMA_PATTERN_FILL_PARAM_PRIVATE);

//UADP_SDT_0_DEF(MS_PHY_phy64Addr);
//UADP_SDT_1_DEF(BDMA_CRC32_PARAM_PRIVATE);



MS_U32 BDMA_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct 
//set table
    UADP_SPT_0NXT(BDMA_CRC32_PARAM);
    UADP_SPT_0NXT(BDMA_MEMCOPY_PARAM);
    
    UADP_SDT_NAME0(MS_PHYADDR_u32Addr,MS_PHYADDR);
    UADP_SDT_NAME1(BDMA_SEARCH_PARAM_PRIVATE,BDMA_SEARCH_PARAM,UADP_SDT_AT ,u32Addr,MS_PHYADDR_u32Addr);	
	
    UADP_SDT_NAME0(BDMA_Status_pStatus,BDMA_Status);
    UADP_SDT_NAME1(BDMA_GETSTATUS_PARAM_PRIVATE,BDMA_GETSTATUS_PARAM,UADP_SDT_P2N,pStatus,BDMA_Status_pStatus);	

    UADP_SDT_NAME0(MS_PHYADDR_u32FlashAddr,MS_PHYADDR);
    UADP_SDT_NAME0(MS_PHYADDR_u32DramAddr,MS_PHYADDR);
    UADP_SDT_NAME2(BDMA_FLASHCOPY2DRAM_PARAM_PRIVATE,BDMA_FLASHCOPY2DRAM_PARAM,UADP_SDT_AT ,u32FlashAddr,MS_PHYADDR_u32FlashAddr,UADP_SDT_AT,u32DramAddr,MS_PHYADDR_u32DramAddr);	    

    //UADP_SDT_NAME0(MS_PHY_phy64SrcAddr,MS_PHY);
    //UADP_SDT_NAME0(MS_PHY_phy64DstAddr,MS_PHY);
    //UADP_SDT_NAME2(BDMA_MEMCOPY_PARAM_PRIVATE,BDMA_MEMCOPY_PARAM,UADP_SDT_AT ,phy64SrcAddr,MS_PHY_phy64SrcAddr,UADP_SDT_AT,phy64DstAddr,MS_PHY_phy64DstAddr);	    

    UADP_SDT_NAME0(MS_PHYADDR_u32Addr,MS_PHYADDR);
    UADP_SDT_NAME1(BDMA_PATTERN_FILL_PARAM_PRIVATE,BDMA_PATTERN_FILL_PARAM,UADP_SDT_AT,u32Addr,MS_PHYADDR_u32Addr);	

    //UADP_SDT_NAME0(MS_PHY_phy64Addr,MS_PHY);
    //UADP_SDT_NAME1(BDMA_CRC32_PARAM_PRIVATE,BDMA_CRC32_PARAM,UADP_SDT_AT,phy64Addr,MS_PHY_phy64Addr);	

    #ifdef MOBF_ENABLE
    //UADP_SDT_NAME0(MS_PHY_phy64Addr,MS_PHY);
    UADP_SDT_NAME0(BDMA_MOBF_PS_pMobfPsCfg,BDMA_MOBF_PS);
    //UADP_SDT_NAME0(MS_PHY_phy64SearchAddr,MS_PHY);
    UADP_SDT_NAME1(BDMA_MOBFSEARCH_PARAM_PRIVATE,BDMA_MOBFSEARCH_PARAM,UADP_SDT_P2N,pMobfPsCfg,BDMA_MOBF_PS_pMobfPsCfg);	
    #endif

    UADPBypassSetSPT(&spt_BDMA_INIT_PARAM[0],UADP_SPT_SELF_SIZE,0, sizeof(BDMA_INIT_PARAM));
    UADPBypassSetSPT(&spt_BDMA_INIT_PARAM[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_BDMA_COPYHND_PARAM[0],UADP_SPT_SELF_SIZE,0, sizeof(BDMA_COPYHND_PARAM));
    UADPBypassSetSPT(&spt_BDMA_COPYHND_PARAM[1],UADP_SPT_END , 0, 0);
 
    *pIoctl= (FUtopiaIOctl)BDMA_adp_Ioctl;
    return 0;	
}

MS_U32 BDMA_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret=0;
    char buffer_arg[2048];
    switch(u32Cmd)
    {
        case MDrv_CMD_BDMA_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_INIT_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_CopyHnd:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_COPYHND_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_MemCopy:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_MEMCOPY_PARAM, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_Search:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_SEARCH_PARAM_PRIVATE, spt_BDMA_SEARCH_PARAM_PRIVATE,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_CRC32:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_CRC32_PARAM, spt_BDMA_CRC32_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_Pattern_Fill:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_PATTERN_FILL_PARAM_PRIVATE, spt_BDMA_PATTERN_FILL_PARAM_PRIVATE,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_FlashCopy2Dram:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_FLASHCOPY2DRAM_PARAM_PRIVATE, NULL, buffer_arg,sizeof(buffer_arg));
            break;
		#ifdef MOBF_ENABLE
        case MDrv_CMD_BDMA_MOBFSearch:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_BDMA_MOBFSEARCH_PARAM_PRIVATE, spt_BDMA_MOBFSEARCH_PARAM_PRIVATE, buffer_arg,sizeof(buffer_arg));
            break;
		#endif
        case MDrv_CMD_BDMA_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL, spt_BDMA_GETSTATUS_PARAM_PRIVATE, buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_BDMA_GetInfo:
            break;
        case MDrv_CMD_BDMA_GetLibVer:
            break;
        case MDrv_CMD_BDMA_GetMinSize:
            break;
        case MDrv_CMD_BDMA_SetDbgLevel:
            break;
        case MDrv_CMD_BDMA_SetSPIOffsetForMCU:
        case MDrv_CMD_BDMA_WaitFlashDone:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        default:
            break;
    }
	return u32Ret;
}

