#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "NJPEG_EX_adp.h"
#include "utopia.h"
#include "utopia_adp.h"

#include "apiJPEG.h"
#include "apiJPEG_v2.h"

#include "MsDevice.h"


#ifdef MSOS_TYPE_LINUX_KERNEL

#ifndef CPY_FROM_USER
#define CPY_FROM_USER                   copy_from_user
#endif
#ifndef CPY_to_USER
#define CPY_to_USER                     copy_to_user
#endif

#else
#ifndef CPY_FROM_USER
#define CPY_FROM_USER                   memcpy
#endif
#ifndef CPY_to_USER
#define CPY_to_USER                     memcpy
#endif

#endif



UADP_SDT_0_DEF(NJPEG_MS_PHY);
UADP_SDT_0_DEF(NJPEG_MS_VIRT);
UADP_SDT_0_DEF(NJPEG_POINTER);



UADP_SPT_NAMEnNXT_DEF(NJPEG_BOOL,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_U8,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_U16,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_U32,0);



UADP_SPT_NAMEnNXT_DEF(NJPEG_BOOL_PARAM,0);


#ifdef BIFROST_32BIT_MODE
UADP_SDT_4_DEF(NJPEG_InitParam);
#else
UADP_SDT_1_DEF(NJPEG_InitParam);
#endif
UADP_SDT_1_DEF(NJPEG_INIT_PARAM);


UADP_SDT_3_DEF(NJPEG_GETAPP0INFO_PARAM);

UADP_SPT_NAMEnNXT_DEF(NJPEG_RESULT_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_GETERRORCODE_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_JPEGEVENT_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_SETMBUFFERVALID_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_UPDATEREADINFO_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_PROCESSEOF_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_SETERRORCODE_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_RESOLUTION_PARAM,0);


UADP_SDT_0_DEF(NJPEG_MSIF_Version);
UADP_SDT_3_DEF(NJPEG_Info);
UADP_SDT_1_DEF(NJPEG_GETINFO_PARAM);


UADP_SPT_NAMEnNXT_DEF(NJPEG_Status,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_GETSTATUS_PARAM,1);

typedef struct
{
    MSIF_Version* pMSIF_Version;
}PMSIF_Version;
UADP_SDT_1_DEF(NJPEG_PMSIF_Version);
UADP_SDT_1_DEF(NJPEG_GETLIBVER_PARAM);


UADP_SPT_NAMEnNXT_DEF(NJPEG_BuffLoadType,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_GETBUFFLOADTYPE_PARAM,1);

UADP_SPT_NAMEnNXT_DEF(NJPEG_WAITDONE_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_EXIF_DateTime,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_GETEXIFDATETIME_PARAM,1);

UADP_SDT_0_DEF(NJPEG_EXIF_Orientation);
UADP_SDT_1_DEF(NJPEG_GETEXIFORIENTATION_PARAM);

UADP_SDT_0_DEF(NJPEG_EXIF_STRING);
UADP_SDT_1_DEF(NJPEG_GETEXIFSTRING_PARAM);

UADP_SDT_1_DEF(NJPEG_GETEXIFU16_PARAM);

UADP_SDT_1_DEF(NJPEG_GETEXIFU32_PARAM);

UADP_SDT_0_DEF(NJPEG_RATIONAL);
UADP_SDT_1_DEF(NJPEG_GETEXIFRATIONAL_PARAM);

#ifdef BIFROST_32BIT_MODE
UADP_SDT_1_DEF(NJPEG_GETFREEMEMORY_PARAM);
#else
UADP_SDT_0_DEF(NJPEG_GETFREEMEMORY_PARAM);
#endif

UADP_SPT_NAMEnNXT_DEF(NJPEG_SUPPORTCMYK_PARAM,0);


typedef struct DLL_PACKED
{
    JPEG_MPO_INDEX_INFO* pMPOIndex;
}PJPEG_MPO_INDEX_INFO;

UADP_SDT_0_DEF(NJPEG_MPO_INDEX_INFO);
//UADP_SDT_1_DEF(NJPEG_PJPEG_MPO_INDEX_INFO);
UADP_SDT_1_DEF(NJPEG_GETMPOINDEX_PARAM);

typedef struct DLL_PACKED
{
    JPEG_MPO_ATTRIBUTE_INFO* pMPOAttr;
}PJPEG_MPO_ATTRIBUTE_INFO;

UADP_SDT_0_DEF(NJPEG_MPO_ATTRIBUTE_INFO);
//UADP_SDT_1_DEF(NJPEG_PJPEG_MPO_ATTRIBUTE_INFO);
UADP_SDT_1_DEF(NJPEG_GETMPOATTR_PARAM);

UADP_SPT_NAMEnNXT_DEF(NJPEG_SETMPOBUFFER_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_VERIFICATION_PARAM,0);

UADP_SPT_NAMEnNXT_DEF(NJPEG_GET_CTRL_PARAM,1);

typedef struct DLL_PACKED
{
    char info[128];
}JPEG_INFO_128BYTE;
UADP_SPT_NAMEnNXT_DEF(NJPEG_EXIF_MANUFACTURER,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_GET_EXIF_MANUFACTURER,1);

typedef struct DLL_PACKED
{
    char info[8];
}JPEG_INFO_8BYTE;
UADP_SPT_NAMEnNXT_DEF(NJPEG_EXIF_SHUTTER_SPEED_VALUE,0);
UADP_SPT_NAMEnNXT_DEF(NJPEG_GET_EXIF_SHUTTER_SPEED_VALUE,1);


#ifdef BIFROST_32BIT_MODE
UADP_SDT_1_DEF(NJPEG_CB_CTRL_PARAM);
#else
UADP_SDT_0_DEF(NJPEG_CB_CTRL_PARAM);
#endif

UADP_SDT_1_DEF(NJPEG_ENABLE_ISR);






MS_U32 NJPEG_EX_adp_Init(FUtopiaIOctl* pIoctl)
{

    UADP_SDT_NAME0(NJPEG_MS_PHY,MS_PHY);
    UADP_SDT_NAME0(NJPEG_MS_VIRT,MS_VIRT);
    UADP_SDT_NAME0(NJPEG_POINTER,unsigned char);

    UADP_SPT_NAME0NXT(NJPEG_BOOL,MS_BOOL);
    UADP_SPT_NAME0NXT(NJPEG_U8,MS_U8);
    UADP_SPT_NAME0NXT(NJPEG_U16,MS_U16);
    UADP_SPT_NAME0NXT(NJPEG_U32,MS_U32);

    UADP_SPT_NAME0NXT(NJPEG_BOOL_PARAM,JPEG_BOOL_PARAM);

#ifdef BIFROST_32BIT_MODE
    UADP_SDT_NAME4(NJPEG_InitParam,JPEG_InitParam,
        UADP_SDT_AT, u32MRCBufAddr, NJPEG_MS_PHY,
        UADP_SDT_AT, u32MWCBufAddr, NJPEG_MS_PHY,
        UADP_SDT_AT, u32InternalBufAddr, NJPEG_MS_PHY,
        UADP_SDT_AT, pFillHdrFunc, NJPEG_POINTER);
#else
    UADP_SDT_NAME1(NJPEG_InitParam,JPEG_InitParam,
        UADP_SDT_AT, pFillHdrFunc, NJPEG_POINTER);
#endif
    UADP_SDT_NAME1(NJPEG_INIT_PARAM, JPEG_INIT_PARAM,
        UADP_SDT_P2N, pInitParam, NJPEG_InitParam);

    UADP_SDT_NAME3(NJPEG_GETAPP0INFO_PARAM,JPEG_GETAPP0INFO_PARAM,
        UADP_SDT_P2N, unit, NJPEG_U8,
        UADP_SDT_P2N, x, NJPEG_U16,
        UADP_SDT_P2N, y, NJPEG_U16);

    UADP_SPT_NAME0NXT(NJPEG_RESULT_PARAM,JPEG_RESULT_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_GETERRORCODE_PARAM,JPEG_GETERRORCODE_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_JPEGEVENT_PARAM,JPEG_JPEGEVENT_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_SETMBUFFERVALID_PARAM,JPEG_SETMBUFFERVALID_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_UPDATEREADINFO_PARAM,JPEG_UPDATEREADINFO_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_PROCESSEOF_PARAM,JPEG_PROCESSEOF_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_SETERRORCODE_PARAM,JPEG_SETERRORCODE_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_RESOLUTION_PARAM,JPEG_RESOLUTION_PARAM);

    UADP_SDT_NAME0(NJPEG_MSIF_Version, MSIF_Version);
    UADP_SDT_NAME3(NJPEG_Info,JPEG_Info,
        UADP_SDT_AT, pu8DrvVer, NJPEG_MSIF_Version,
        UADP_SDT_AT, pu8HalVer, NJPEG_U8,
        UADP_SDT_AT, pu8FwVer, NJPEG_U8);
    UADP_SDT_NAME1(NJPEG_GETINFO_PARAM, JPEG_GETINFO_PARAM,
        UADP_SDT_P2N, pJPEG_Info, NJPEG_Info);

    UADP_SDT_NAME0(NJPEG_Status, JPEG_Status);
    UADP_SDT_NAME1(NJPEG_GETSTATUS_PARAM, JPEG_GETSTATUS_PARAM,
        UADP_SDT_P2N, pJPEG_Status, NJPEG_Status);

    UADP_SDT_NAME1(NJPEG_PMSIF_Version, PMSIF_Version,
        UADP_SDT_P2N, pMSIF_Version, NJPEG_MSIF_Version);
    UADP_SDT_NAME1(NJPEG_GETLIBVER_PARAM, JPEG_GETLIBVER_PARAM,
        UADP_SDT_AT, ppVersion, NJPEG_PMSIF_Version);

    UADP_SDT_NAME0(NJPEG_BuffLoadType, JPEG_BuffLoadType);
    UADP_SDT_NAME1(NJPEG_GETBUFFLOADTYPE_PARAM, JPEG_GETBUFFLOADTYPE_PARAM,
        UADP_SDT_P2N, pBuffLoadType, NJPEG_BuffLoadType);

    UADP_SPT_NAME0NXT(NJPEG_WAITDONE_PARAM,JPEG_WAITDONE_PARAM);

    UADP_SDT_NAME0(NJPEG_EXIF_DateTime, JPEG_EXIF_DateTime);
    UADP_SDT_NAME1(NJPEG_GETEXIFDATETIME_PARAM, JPEG_GETEXIFDATETIME_PARAM,
        UADP_SDT_P2N, DateTime, NJPEG_EXIF_DateTime);

    UADP_SDT_NAME0(NJPEG_EXIF_Orientation, JPEG_EXIF_Orientation);
    UADP_SDT_NAME1(NJPEG_GETEXIFORIENTATION_PARAM, JPEG_GETEXIFORIENTATION_PARAM,
        UADP_SDT_P2N, eOrientation, NJPEG_EXIF_Orientation);

    UADP_SDT_NAME0(NJPEG_EXIF_STRING, JPEG_INFO_128BYTE);
    UADP_SDT_NAME1(NJPEG_GETEXIFSTRING_PARAM, JPEG_GETEXIFSTRING_PARAM,
        UADP_SDT_P2N, pu8String, NJPEG_EXIF_STRING);

    UADP_SDT_NAME1(NJPEG_GETEXIFU16_PARAM, JPEG_GETEXIFU16_PARAM,
        UADP_SDT_P2N, pu16Value, NJPEG_U16);

    UADP_SDT_NAME1(NJPEG_GETEXIFU32_PARAM, JPEG_GETEXIFU32_PARAM,
        UADP_SDT_P2N, pu32Value, NJPEG_U32);

    UADP_SDT_NAME0(NJPEG_RATIONAL, JPEG_RATIONAL);
    UADP_SDT_NAME1(NJPEG_GETEXIFRATIONAL_PARAM, JPEG_GETEXIFRATIONAL_PARAM,
        UADP_SDT_P2N, pRational, NJPEG_RATIONAL);

#ifdef BIFROST_32BIT_MODE
    UADP_SDT_NAME1(NJPEG_GETFREEMEMORY_PARAM, JPEG_GETFREEMEMORY_PARAM,
        UADP_SDT_AT, addr, NJPEG_MS_PHY);
#else
    UADP_SDT_NAME0(NJPEG_GETFREEMEMORY_PARAM, JPEG_GETFREEMEMORY_PARAM);
#endif

    UADP_SPT_NAME0NXT(NJPEG_SUPPORTCMYK_PARAM,JPEG_SUPPORTCMYK_PARAM);

    UADP_SDT_NAME0(NJPEG_MPO_INDEX_INFO, JPEG_MPO_INDEX_INFO);
//    UADP_SDT_NAME1(NJPEG_PJPEG_MPO_INDEX_INFO, PJPEG_MPO_INDEX_INFO,
//        UADP_SDT_P2N, pMPOIndex, NJPEG_MPO_INDEX_INFO);
    UADP_SDT_NAME1(NJPEG_GETMPOINDEX_PARAM, JPEG_GETMPOINDEX_PARAM,
        UADP_SDT_AT, pMPOIndex, NJPEG_MPO_INDEX_INFO);

    UADP_SDT_NAME0(NJPEG_MPO_ATTRIBUTE_INFO, JPEG_MPO_ATTRIBUTE_INFO);
//    UADP_SDT_NAME1(NJPEG_PJPEG_MPO_ATTRIBUTE_INFO, PJPEG_MPO_ATTRIBUTE_INFO,
//        UADP_SDT_P2N, pMPOAttr, NJPEG_MPO_ATTRIBUTE_INFO);
    UADP_SDT_NAME1(NJPEG_GETMPOATTR_PARAM, JPEG_GETMPOATTR_PARAM,
        UADP_SDT_AT, pMPOAttr, NJPEG_MPO_ATTRIBUTE_INFO);

    UADP_SPT_NAME0NXT(NJPEG_SETMPOBUFFER_PARAM,JPEG_SETMPOBUFFER_PARAM);

    UADP_SPT_NAME0NXT(NJPEG_VERIFICATION_PARAM,JPEG_VERIFICATION_PARAM);

    UADP_SDT_NAME1(NJPEG_GET_CTRL_PARAM, JPEG_GET_CTRL_PARAM,
        UADP_SDT_AT, param, NJPEG_POINTER);

    UADP_SDT_NAME0(NJPEG_EXIF_MANUFACTURER, JPEG_INFO_128BYTE);
    UADP_SDT_NAME1(NJPEG_GET_EXIF_MANUFACTURER, JPEG_GET_CTRL_PARAM,
        UADP_SDT_P2N, param,NJPEG_EXIF_MANUFACTURER);

    UADP_SDT_NAME0(NJPEG_EXIF_SHUTTER_SPEED_VALUE, JPEG_INFO_128BYTE);
    UADP_SDT_NAME1(NJPEG_GET_EXIF_SHUTTER_SPEED_VALUE, JPEG_GET_CTRL_PARAM,
        UADP_SDT_P2N, param,NJPEG_EXIF_SHUTTER_SPEED_VALUE);

#ifdef BIFROST_32BIT_MODE
    UADP_SDT_NAME1(NJPEG_CB_CTRL_PARAM, JPEG_CB_EVENT_PARAM,
        UADP_SDT_AT, BufAddr,NJPEG_MS_PHY);
#else
    UADP_SDT_NAME0(NJPEG_CB_CTRL_PARAM, JPEG_CB_EVENT_PARAM);
#endif

    UADP_SDT_NAME1(NJPEG_ENABLE_ISR, JPEG_ENABLEISR_PARAM,
        UADP_SDT_AT, IsrCb,NJPEG_POINTER);



	*pIoctl= (FUtopiaIOctl)NJPEG_EX_adp_Ioctl;

    return 0;
}


MS_U32 NJPEG_EX_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2048];

    switch(u32Cmd)
    {
        case MApi_CMD_JPEG_EnableOJPD:
        case MApi_CMD_JPEG_IsProgressive:
        case MApi_CMD_JPEG_ThumbnailFound:
        case MApi_CMD_JPEG_SetMHEG5:
        case MApi_CMD_JPEG_IsMPOFormat:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_BOOL_PARAM, spt_NJPEG_BOOL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_Init_UsingOJPD:
        case MApi_CMD_JPEG_Init:// TODO: there is callback inside
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_INIT_PARAM, spt_NJPEG_INIT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_get_APP0_info:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETAPP0INFO_PARAM, spt_NJPEG_GETAPP0INFO_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_DecodeHdr:
        case MApi_CMD_JPEG_Decode:
        case MApi_CMD_JPEG_HdlVidxChk:
        case MApi_CMD_JPEG_DisableISR:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_RESULT_PARAM, spt_NJPEG_RESULT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetMaxDecodeResolution:
        case MApi_CMD_JPEG_SetProMaxDecodeResolution:
        case MApi_CMD_JPEG_SetMPOMaxDecodeResolution:
        case MApi_CMD_JPEG_SetMPOProMaxDecodeResolution:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_RESOLUTION_PARAM, spt_NJPEG_RESOLUTION_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_Exit:
        case MApi_CMD_JPEG_Rst:
        case MApi_CMD_JPEG_PowerOn:
        case MApi_CMD_JPEG_PowerOff:
        case MApi_CMD_JPEG_DumpMPO:
        case MApi_CMD_JPEG_Debug:
            u32Ret=UtopiaIoctl(pInstanceTmp,u32Cmd, NULL);
            break;
        case MApi_CMD_JPEG_GetErrorCode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETERRORCODE_PARAM, spt_NJPEG_GETERRORCODE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetJPDEventFlag:
        case MApi_CMD_JPEG_SetJPDEventFlag:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_JPEGEVENT_PARAM, spt_NJPEG_JPEGEVENT_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetCurVidx:
        case MApi_CMD_JPEG_GetWidth:
        case MApi_CMD_JPEG_GetHeight:
        case MApi_CMD_JPEG_GetOriginalWidth:
        case MApi_CMD_JPEG_GetOriginalHeight:
        case MApi_CMD_JPEG_GetNonAlignmentWidth:
        case MApi_CMD_JPEG_GetNonAlignmentHeight:
        case MApi_CMD_JPEG_GetAlignedPitch:
        case MApi_CMD_JPEG_GetAlignedPitch_H:
        case MApi_CMD_JPEG_GetAlignedWidth:
        case MApi_CMD_JPEG_GetAlignedHeight:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_U16, spt_NJPEG_U16,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetScaleDownFactor:
        case MApi_CMD_JPEG_SetDbgLevel:
        case MApi_CMD_JPEG_GetDbgLevel:
        case MApi_CMD_JPEG_SetNJPDInstance:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_U8, spt_NJPEG_U8,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetMRBufferValid:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_SETMBUFFERVALID_PARAM, spt_NJPEG_SETMBUFFERVALID_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_UpdateReadInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_UPDATEREADINFO_PARAM, spt_NJPEG_UPDATEREADINFO_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_ProcessEOF:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_PROCESSEOF_PARAM, spt_NJPEG_PROCESSEOF_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetErrCode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_SETERRORCODE_PARAM, spt_NJPEG_SETERRORCODE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetInfo:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETINFO_PARAM, spt_NJPEG_GETINFO_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetStatus:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETSTATUS_PARAM, spt_NJPEG_GETSTATUS_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetLibVer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETLIBVER_PARAM, spt_NJPEG_GETLIBVER_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetBuffLoadType:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETBUFFLOADTYPE_PARAM, spt_NJPEG_GETBUFFLOADTYPE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_EnableISR:// TODO: callback
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_ENABLE_ISR, spt_NJPEG_ENABLE_ISR,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_WaitDone:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_WAITDONE_PARAM, spt_NJPEG_WAITDONE_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFDateTime:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETEXIFDATETIME_PARAM, spt_NJPEG_GETEXIFDATETIME_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFOrientation:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETEXIFORIENTATION_PARAM, spt_NJPEG_GETEXIFORIENTATION_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFManufacturer:
        case MApi_CMD_JPEG_GetEXIFModel:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETEXIFSTRING_PARAM, spt_NJPEG_GETEXIFSTRING_PARAM,buffer_arg,sizeof(buffer_arg));
//            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GET_EXIF_MANUFACTURER, spt_NJPEG_GET_EXIF_MANUFACTURER,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFFlash:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETEXIFU16_PARAM, spt_NJPEG_GETEXIFU16_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFISOSpeedRatings:
        case MApi_CMD_JPEG_GetEXIFImageWidth:
        case MApi_CMD_JPEG_GetEXIFImageHeight:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETEXIFU32_PARAM, spt_NJPEG_GETEXIFU32_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetEXIFShutterSpeedValue:
        case MApi_CMD_JPEG_GetEXIFApertureValue:
        case MApi_CMD_JPEG_GetEXIFExposureBiasValue:
        case MApi_CMD_JPEG_GetEXIFFocalLength:
        case MApi_CMD_JPEG_GetEXIFExposureTime:
        case MApi_CMD_JPEG_GetEXIFFNumber:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETEXIFRATIONAL_PARAM, spt_NJPEG_GETEXIFRATIONAL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetFreeMemory:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETFREEMEMORY_PARAM, spt_NJPEG_GETFREEMEMORY_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetDataOffset:
        case MApi_CMD_JPEG_GetSOFOffset:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_U32, spt_NJPEG_U32,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SupportCMYK:
        case MApi_CMD_JPEG_SupportRGB:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_SUPPORTCMYK_PARAM, spt_NJPEG_SUPPORTCMYK_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetMPOIndex:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETMPOINDEX_PARAM, spt_NJPEG_GETMPOINDEX_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetMPOAttr:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GETMPOATTR_PARAM, spt_NJPEG_GETMPOATTR_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetMPOBuffer:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_SETMPOBUFFER_PARAM, spt_NJPEG_SETMPOBUFFER_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_SetVerificationMode:
        case MApi_CMD_JPEG_GetVerificationMode:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_VERIFICATION_PARAM, spt_NJPEG_VERIFICATION_PARAM,buffer_arg,sizeof(buffer_arg));
            break;
        case MApi_CMD_JPEG_GetControl:
            {
            JPEG_GET_CTRL_PARAM jpegGetCtrlParam;
            CPY_FROM_USER((void*)(&jpegGetCtrlParam), (JPEG_GET_CTRL_PARAM*)pArgs, sizeof(JPEG_GET_CTRL_PARAM));
            switch(jpegGetCtrlParam.eGetID)
            {
                case E_JPEG_GET_EXIF_MANUFACTURER://96
                case E_JPEG_GET_EXIF_MODEL:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GET_EXIF_MANUFACTURER, spt_NJPEG_GET_EXIF_MANUFACTURER,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_JPEG_GET_EXIF_FLASH://32
                case E_JPEG_GET_EXIF_ISO_SPEED_RATINGS:
                case E_JPEG_GET_EXIF_IMAGE_WIDTH:
                case E_JPEG_GET_EXIF_IMAGE_HEIGHT:
                case E_JPEG_GET_EXIF_EXPOSURE_PROGRAM:
                case E_JPEG_GET_Y_SAMPLING_FACTOR_HORIZONTAL:
                case E_JPEG_GET_Y_SAMPLING_FACTOR_VERTICAL:
                case E_JPEG_GET_COMPONENTS_NUM:
                case E_JPEG_GET_IS_PROGRESSIVE:
                case E_JPEG_GET_COLOR_FORMAT:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GET_CTRL_PARAM, spt_NJPEG_GET_CTRL_PARAM,buffer_arg,sizeof(buffer_arg));
                    break;
                case E_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE://64
                case E_JPEG_GET_EXIF_APERTURE_VALUE:
                case E_JPEG_GET_EXIF_EXPOSURE_BIAS_VALUE:
                case E_JPEG_GET_EXIF_FOCAL_LENGTH:
                case E_JPEG_GET_EXIF_EXPOSURE_TIME:
                case E_JPEG_GET_EXIF_F_NUMBER:
                    u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_GET_EXIF_SHUTTER_SPEED_VALUE, spt_NJPEG_GET_EXIF_SHUTTER_SPEED_VALUE,buffer_arg,sizeof(buffer_arg));
                    break;
            }
            }
            break;
        case MApi_CMD_JPEG_GetCbEvent:
        case MApi_CMD_JPEG_SetCbEvent:
            u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_NJPEG_CB_CTRL_PARAM, spt_NJPEG_CB_CTRL_PARAM,buffer_arg,sizeof(buffer_arg));
            break;

        default:
            break;
    }

    return u32Ret;
}




