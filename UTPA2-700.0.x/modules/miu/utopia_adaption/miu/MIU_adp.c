#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include <linux/uaccess.h>
#include "MsTypes.h"
#include "utopia.h"
#include "drvMIU.h"
#include "drvMIU_v2.h"
#include "MIU_adp.h"
#include "utopia_adp.h"

// no pointer member
//UADP_SPT_0NXT_DEF(MS_U32);
extern void* spt_MS_U32;
UADP_SPT_0NXT_DEF(MIU_RET);
UADP_SPT_0NXT_DEF(MIU_MIU_MASK);
UADP_SPT_0NXT_DEF(MIU_MIUREQ_MASK);
UADP_SPT_0NXT_DEF(MIU_SET_SSC);
UADP_SPT_0NXT_DEF(MIU_SET_SSCVALUE);
UADP_SPT_0NXT_DEF(MIU_PROYECT_ID);
UADP_SPT_0NXT_DEF(MIU_SELMIU);
UADP_SPT_0NXT_DEF(MIU_PortectInfo);
UADP_SPT_0NXT_DEF(MIU_GroupPriority);
UADP_SPT_0NXT_DEF(MIU_DRAM_SIZE);
UADP_SPT_0NXT_DEF(MIU_SETGROUPPRIORITY);
UADP_SPT_0NXT_DEF(MIU_SETHIGHPRIORITY);

// one pointer member

UADP_SDT_0_DEF(U8_pu8ProtectId);
UADP_SDT_1_DEF(MIU_PROTECT_PRIVATE);

UADP_SDT_0_DEF(U32_pu32ProtectId);
UADP_SDT_1_DEF(MIU_PROTECTEx_PRIVATE);

UADP_SDT_0_DEF(MIU_PortectInfo_pInfo);
UADP_SDT_1_DEF(MIU_GETPROTECTINFO_PRIVATE);

UADP_SDT_0_DEF(MS_U32_u32PageShift);
UADP_SDT_1_DEF(MIU_PROTECT_ALIGN_PRIVATE);

UADP_SDT_0_DEF(MIU_DDR_TYPE_pType);
UADP_SDT_1_DEF(MIU_GETDRAMTYPE_PRIVATE);

MS_U32 MIU_adp_Init(FUtopiaIOctl* pIoctl)
{
//member of struct
//set table
    UADP_SPT_0NXT(MIU_RET);
    UADP_SPT_0NXT(MIU_MIU_MASK);
    UADP_SPT_0NXT(MIU_MIUREQ_MASK);
    UADP_SPT_0NXT(MIU_SET_SSC);
    UADP_SPT_0NXT(MIU_SET_SSCVALUE);
    UADP_SPT_0NXT(MIU_PROYECT_ID);
    UADP_SPT_0NXT(MIU_SELMIU);
    UADP_SPT_0NXT(MIU_PortectInfo);
    UADP_SPT_0NXT(MIU_GroupPriority);
    UADP_SPT_0NXT(MIU_DRAM_SIZE);
    //UADP_SPT_0NXT(MIU_PROTECT_ALIGN);
    UADP_SPT_0NXT(MIU_SETGROUPPRIORITY);
    UADP_SPT_0NXT(MIU_SETHIGHPRIORITY);

    //UADP_SPT_1NXT(MIU_PROTECT, pu8ProtectId, MIU_PROYECT_ID);
    //UADP_SPT_1NXT(MIU_GETPROTECTINFO, pInfo, MIU_PortectInfo);
    //UADP_SPT_1NXT(MIU_PROTECT_ALIGN, u32PageShift, MS_U32);

    UADP_SDT_NAME0(U8_pu8ProtectId,MS_U8);
    UADP_SDT_NAME1(MIU_PROTECT_PRIVATE,MIU_PROTECT,UADP_SDT_P2N,pu8ProtectId,U8_pu8ProtectId);

    UADP_SDT_NAME0(U32_pu32ProtectId,MS_U32);
    UADP_SDT_NAME1(MIU_PROTECTEx_PRIVATE,MIU_PROTECTEx,UADP_SDT_P2N,pu32ProtectId,U32_pu32ProtectId);

    UADP_SDT_NAME0(MIU_DDR_TYPE_pType,MIU_DDR_TYPE);
    UADP_SDT_NAME1(MIU_GETDRAMTYPE_PRIVATE,MIU_GETDRAMTYPE,UADP_SDT_P2N,pType,MIU_DDR_TYPE_pType);

    UADP_SDT_NAME0(MIU_PortectInfo_pInfo,MIU_PortectInfo);
    UADP_SDT_NAME1(MIU_GETPROTECTINFO_PRIVATE,MIU_GETPROTECTINFO,UADP_SDT_P2N,pInfo,MIU_PortectInfo_pInfo);

    UADP_SDT_NAME0(MS_U32_u32PageShift,MS_U32);
    UADP_SDT_NAME1(MIU_PROTECT_ALIGN_PRIVATE,MIU_PROTECT_ALIGN,UADP_SDT_P2N,u32PageShift,MS_U32_u32PageShift);

    *pIoctl= (FUtopiaIOctl)MIU_adp_Ioctl;
    return 0;
}

MS_U32 MIU_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{

    MS_U32 u32Ret=0;
    char buffer_arg[1024];

    switch(u32Cmd)
    {
        case MDrv_CMD_MIU_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, spt_MIU_RET, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_OPM_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_DNRB_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_DNRB_W:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_DNRB_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_SC_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVOP_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVD_R:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVD_W:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_MVD_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req_AUDIO_RW:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIU_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Mask_Req:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIUREQ_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_UnMask_Req:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_MIUREQ_MASK, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetSsc:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SET_SSC, spt_MIU_SET_SSC, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetSscValue:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SET_SSCVALUE, spt_MIU_SET_SSCVALUE, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Protect:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECT_PRIVATE, spt_MIU_PROTECT_PRIVATE, buffer_arg, sizeof(buffer_arg));
            //u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECT, spt_MIU_PROTECT, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_ProtectEx:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECTEx_PRIVATE, spt_MIU_PROTECTEx_PRIVATE, buffer_arg, sizeof(buffer_arg));
            //u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECT, spt_MIU_PROTECT, buffer_arg, sizeof(buffer_arg));
            break;

        case MDrv_CMD_MIU_GetDramType:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_GETPROTECTINFO_PRIVATE, spt_MIU_GETPROTECTINFO_PRIVATE, buffer_arg, sizeof(buffer_arg));
            break;

        case MDrv_CMD_MIU_SelMIU:
        {
            PMIU_SELMIU	paramSts;
            copy_from_user(&paramSts,pArgs,sizeof(PMIU_SELMIU));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&paramSts);
            copy_to_user(pArgs, &paramSts, sizeof(PMIU_SELMIU));
	        break;
        }

        case MDrv_CMD_MIU_GetProtectInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_GETPROTECTINFO_PRIVATE, spt_MIU_GETPROTECTINFO_PRIVATE, buffer_arg, sizeof(buffer_arg));
            //u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_GETPROTECTINFO, spt_MIU_GETPROTECTINFO, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetGroupPriority:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SETGROUPPRIORITY, spt_MIU_SETGROUPPRIORITY, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_SetHighPriority:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_SETHIGHPRIORITY, spt_MIU_SETHIGHPRIORITY, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_PrintProtectInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, NULL, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_MIU_Dram_Size:
        {
            PMIU_DRAM_SIZE PVar;
            copy_from_user(&PVar,pArgs,sizeof(PMIU_DRAM_SIZE));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,&PVar);
            copy_to_user(pArgs, &PVar, sizeof(PMIU_DRAM_SIZE));
            break;
        }
        case MDrv_CMD_MIU_ProtectAlign:
        {
            u32Ret=UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_MIU_PROTECT_ALIGN_PRIVATE, spt_MIU_PROTECT_ALIGN_PRIVATE, buffer_arg, sizeof(buffer_arg));
            break;
        }

        default:
            break;
    }
	return u32Ret;
}
