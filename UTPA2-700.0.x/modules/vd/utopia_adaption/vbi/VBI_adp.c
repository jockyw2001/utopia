#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvVBI.h"
#include "drvVBI_v2.h"
#include "VBI_adp.h"


#include "utopia_adp.h"

//VBI CC/TTX

UADP_SPT_0NXT_DEF(VBI_BOOL);
UADP_SPT_0NXT_DEF(VBI_GET_DATA);
UADP_SPT_0NXT_DEF(VBI_GETINFO_PARAM);

UADP_SPT_1NXT_DEF(VBI_GETSTATUS_PARAM);
UADP_SPT_0NXT_DEF(VBI_DrvStatus);      // member belong to VBI_GETSTATUS_PARAM

UADP_SPT_0NXT_DEF(VBI_SETDBGLEVEL_PARAM);
UADP_SPT_0NXT_DEF(VBI_CMD);
UADP_SPT_0NXT_DEF(VBI_INIT_TYPE_PARAM);
UADP_SPT_0NXT_DEF(DMX_TTX_CMD);
UADP_SPT_0NXT_DEF(VBI_REGISTER_CB);
UADP_SPT_0NXT_DEF(VBI_INITIALIZER_TTX_SLICER);
UADP_SPT_0NXT_DEF(VBI_ENABLE_TTX_SLICER);
UADP_SPT_0NXT_DEF(VBI_GET_RAW_VPS_DATA);

UADP_SPT_2NXT_DEF(VBI_GET_VPS_DATA);

UADP_SPT_1NXT_DEF(VBI_TTX_GET_PACKETS);
UADP_SPT_1NXT_DEF(VBI_TTX_GET_PACKET);
UADP_SPT_0NXT_DEF(TYPE_U32);            // member belong to VBI_TTX_GET_PACKETS
UADP_SPT_0NXT_DEF(MS_PHYADDR);          // member belong to VBI_TTX_GET_PACKET


UADP_SPT_1NXT_DEF(VBI_CC_SETDATARATE);
UADP_SPT_0NXT_DEF(VBI_CC_TABLE);

UADP_SPT_0NXT_DEF(VBI_SET_VIDEO_STANDARD);
UADP_SPT_0NXT_DEF(VBI_TTX_ENABLE_LINE);
UADP_SPT_0NXT_DEF(VBI_SYNC_MEMORY);
UADP_SPT_0NXT_DEF(VBI_CC_INIT_SLICER);
UADP_SPT_0NXT_DEF(VBI_CC_INIT_YPBYR);

UADP_SPT_0NXT_DEF(VBI_CC_GETINFO);
UADP_SPT_0NXT_DEF(VBI_CC_SET_FRAMECNT);
UADP_SPT_0NXT_DEF(VBI_CC_ENABLE_LINE);

//DMX TTX
UADP_SPT_0NXT_DEF(DMX_TTX_SET_FILTERID);

extern UADP_STRUCT_POINTER_TABLE spt_MS_U8[];
extern UADP_STRUCT_POINTER_TABLE spt_MS_U32[];
extern UADP_STRUCT_POINTER_TABLE spt_MS_NULL[];

MS_U32 VBI_adp_Init(FUtopiaIOctl *pIoctl)
{
//member of struct
    UADP_SPT_0NXT(VBI_DrvStatus);
    UADP_SPT_0NXT(MS_PHYADDR);
    UADP_SPT_0NXT(VBI_CC_TABLE);

//set table
    UADP_SPT_0NXT(VBI_BOOL);
    UADP_SPT_0NXT(VBI_GET_DATA);
    UADP_SPT_0NXT(VBI_GETINFO_PARAM);
    UADP_SPT_0NXT(VBI_SETDBGLEVEL_PARAM);
    UADP_SPT_0NXT(VBI_CMD);
    UADP_SPT_0NXT(VBI_INIT_TYPE_PARAM);
    UADP_SPT_0NXT(DMX_TTX_CMD);

    UADP_SPT_0NXT(VBI_ENABLE_TTX_SLICER);
    UADP_SPT_0NXT(VBI_SET_VIDEO_STANDARD);
    UADP_SPT_0NXT(VBI_TTX_ENABLE_LINE);
    UADP_SPT_0NXT(VBI_SYNC_MEMORY);
    UADP_SPT_0NXT(VBI_CC_INIT_YPBYR);
    UADP_SPT_0NXT(VBI_CC_GETINFO);
    UADP_SPT_0NXT(VBI_CC_SET_FRAMECNT);
    UADP_SPT_0NXT(VBI_CC_ENABLE_LINE);
    UADP_SPT_0NXT(VBI_GET_RAW_VPS_DATA);
    //UADP_SDT_1(VBI_REGISTER_CB, UADP_SDT_AT, bufferAddr, MS_NULL);
    //UADP_SDT_1(VBI_INITIALIZER_TTX_SLICER, UADP_SDT_AT, bufferAddr, MS_NULL);
    //UADP_SDT_2(VBI_CC_INIT_SLICER, UADP_SDT_AT, u32RiuAddr, MS_NULL, UADP_SDT_AT, bufferAddr, MS_NULL);
    UADP_SDT_1(VBI_CC_INIT_SLICER, UADP_SDT_AT, u32RiuAddr, MS_NULL);

    UADP_SDT_1(VBI_GETSTATUS_PARAM, UADP_SDT_P2N, pDrvStatus, VBI_DrvStatus);
    //UADP_SDT_2(VBI_TTX_GET_PACKETS, UADP_SDT_AT, dataAddr, MS_NULL, UADP_SDT_P2N, length, MS_U32);
    UADP_SDT_1(VBI_TTX_GET_PACKETS, UADP_SDT_P2N, length, MS_U32);
    //UADP_SDT_1(VBI_TTX_GET_PACKET, UADP_SDT_P2N, packetAddress, MS_NULL);
    UADP_SDT_1(VBI_CC_SETDATARATE, UADP_SDT_P2N, ptable, MS_U8);
    UADP_SDT_2(VBI_GET_VPS_DATA, UADP_SDT_P2N, lowerWord, MS_U8, UADP_SDT_P2N, higherWord, MS_U8);
    *pIoctl= (FUtopiaIOctl)VBI_adp_Ioctl;
    return 0;

}

/*
MS_U32 VBI_DDI_Open(void* pInstant,const void* const pAttribute)
{
    return UtopiaOpen(MODULE_AESDMA,pInstant,0,pAttribute);
}
*/
MS_U32 VBI_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[1024];

    //printk("%s, u32Cmd = 0x%x\n", __FUNCTION__, (unsigned int)u32Cmd);
    switch(u32Cmd)
    {

        case MDrv_CMD_VBI_GetLibVer :
            break;
        case MDrv_CMD_VBI_GetInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GETINFO_PARAM,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GETSTATUS_PARAM,spt_VBI_GETSTATUS_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_SetDbgLevel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_SETDBGLEVEL_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_INIT_TYPE_PARAM, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_RegisterCB:
            //u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_REGISTER_CB,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_RingBuffer_Reset:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_VBI_InitializeTTXSlicer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_INITIALIZER_TTX_SLICER,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_EnableTTXSlicer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_ENABLE_TTX_SLICER,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_IsVPS_Ready:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_IsTTX_Ready:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_IsWSS_Ready:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_GetWSS_Data:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GET_DATA,spt_VBI_GET_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_GetVPS_Data:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GET_VPS_DATA,spt_VBI_GET_VPS_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_GetRawVPS_Data:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GET_RAW_VPS_DATA,spt_VBI_GET_RAW_VPS_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_SetVideoStandard:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_SET_VIDEO_STANDARD,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferIsEmpty:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_CheckCircuitReady:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_GetPacketCount:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GET_DATA,spt_VBI_GET_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_GetPackets:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_TTX_GET_PACKETS,spt_VBI_TTX_GET_PACKETS,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_GetPacket:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_TTX_GET_PACKET,spt_VBI_TTX_GET_PACKET,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferIsOverflow:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GET_DATA,spt_VBI_GET_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_TTX_EnableLine:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_TTX_ENABLE_LINE,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_SyncMemory:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_SYNC_MEMORY,NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case MDrv_CMD_VBI_CC_InitSlicer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_INIT_SLICER,NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case MDrv_CMD_VBI_CC_InitYPbYr:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_INIT_YPBYR,NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case MDrv_CMD_VBI_CC_SetDataRate:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_SETDATARATE,NULL,buffer_arg,sizeof(buffer_arg));
            break;

        case MDrv_CMD_VBI_CC_GetInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_GETINFO,spt_VBI_CC_GETINFO,buffer_arg,sizeof(buffer_arg));
            break;

        case MDrv_CMD_VBI_CC_SetFrameCnt:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_SET_FRAMECNT,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_CC_EnableSlicer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_ENABLE_TTX_SLICER,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_CC_EnableLine:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_ENABLE_LINE,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_WSS_SetVpsByteNum:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CC_SET_FRAMECNT,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        //=============================================
        // DTV - TTX
        //=============================================

        case MDrv_CMD_DMX_TTX_RingBuffer_Reset:
            u32Ret= UtopiaIoctl(pInstanceTmp,u32Cmd,pArgs);
            break;
        case MDrv_CMD_DMX_TTX_Init:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_INITIALIZER_TTX_SLICER,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_SetFilterID:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DMX_TTX_SET_FILTERID,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_SetCB:
            // no function callback from kernel to user
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferIsEmpty:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_GetPackets:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_TTX_GET_PACKETS,spt_VBI_TTX_GET_PACKETS,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_GetPacket:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_TTX_GET_PACKET,spt_VBI_TTX_GET_PACKET,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferIsOverflow:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_BOOL,spt_VBI_BOOL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_GET_DATA,spt_VBI_GET_DATA,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_VBI_Exit:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_VBI_CMD,NULL,buffer_arg,sizeof(buffer_arg));
            break;
        case MDrv_CMD_DMX_TTX_Exit:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_DMX_TTX_CMD,NULL,buffer_arg,sizeof(buffer_arg));
            break;

        default:
            break;

    }


    return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}




