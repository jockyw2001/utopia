#include "MsTypes.h"
#if defined(MSOS_TYPE_LINUX_KERNEL)
    #include <linux/slab.h>
    #include <linux/string.h>
#else
#include <stdio.h>
#include <string.h>
#endif
#include "drvVBI_v2.h"
#include "drvVBI_private.h"
#include "utopia_dapi.h"
#include "drvVBI.h"
#include "drvDMX_TTX.h"
#include "MsOS.h"
#include "utopia.h"
#include "ULog.h"
#define VBI_DEBUG 0

#if (VBI_DEBUG == 1)
  #define VBI_DEBUG_MSG(x)       x
#else
  #define VBI_DEBUG_MSG(x)
#endif


void VBIRegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pUtopiaModule = NULL;

    VBI_DEBUG_MSG(ULOGD("VBI", "\n[VBI INFO] vbi register start\n"));

    // 1. deal with module
    UtopiaModuleCreate(MODULE_VBI, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)VBIOpen, (FUtopiaClose)VBIClose, (FUtopiaIOctl)VBIIoctl);
    #ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("vbi", (FUtopiaMdbIoctl)VBIMdbIoctl);
    #endif
#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)VBIStr);
    UtopiaModuleSetSTRPrivate(pUtopiaModule, sizeof(VBI_INSTANT_PRIVATE));
#endif

    // 2. deal with resource (2 resources for DTV & ATV)
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, VBI_POOL_ID_VBI0);
    UtopiaResourceCreate("VBI0", sizeof(VBI_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, VBI_POOL_ID_VBI0);
    UtopiaModuleAddResourceEnd(pUtopiaModule, VBI_POOL_ID_VBI0);

    psResource = NULL;
    UtopiaModuleAddResourceStart(pUtopiaModule, VBI_POOL_ID_VBI1);
    UtopiaResourceCreate("VBI1", sizeof(VBI_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, VBI_POOL_ID_VBI1);
    UtopiaModuleAddResourceEnd(pUtopiaModule, VBI_POOL_ID_VBI1);

}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 VBIMdbIoctl(MS_U32 cmd, const void* const pArgs)
{
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;
    VBI_DrvStatus _vbiStatus;
    switch(cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;
            break;
        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            MdbPrint(paraGetInfo->u64ReqHdl,"---------MStar vbi state---------\n");
            if(FALSE == MDrv_VBI_GetStatus(&_vbiStatus))
            {
                MdbPrint(paraGetInfo->u64ReqHdl,"Not Initialized\n", _vbiStatus.eInitType);
            }
            else
            {
                switch(_vbiStatus.eInitType)
                {
                    case E_VBI_TELETEXT:
                        MdbPrint(paraGetInfo->u64ReqHdl,"state : TTX\n");
                        MdbPrint(paraGetInfo->u64ReqHdl,"pktcnt : %d\n", MDrv_VBI_TTX_GetPacketCount());
                        break;
                    case E_VBI_CC:
                        MdbPrint(paraGetInfo->u64ReqHdl,"state : CC\n");
                        MdbPrint(paraGetInfo->u64ReqHdl,"pktcnt : %d\n", MDrv_VBI_CC_GetInfo(VBI_CC_PACKET_COUNT));
                        break;
                    case E_VBI_WSS:
                        MdbPrint(paraGetInfo->u64ReqHdl,"state : WSS\n");
                        break;
                    default:
                        break;
                }
            }
            paraGetInfo->result = MDBRESULT_SUCCESS_FIN;
            break;
        default:
            break;
    }
    return 0;
}
#endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
MS_U32 VBIStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if ((u32PowerState == E_POWER_SUSPEND) || (u32PowerState == E_POWER_RESUME))
    {
        u32Return = _MDrv_VBI_SetPowerState(u32PowerState);
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
    }
    return u32Return;// for success
}
#endif

MS_U32 VBIOpen(void** ppInstance, const void* const pAttribute)
{
    VBI_DEBUG_MSG(ULOGD("VBI", "\n[VBI INFO] vbi open start\n"));

    VBI_INSTANT_PRIVATE *pVbiPri = NULL;
    void *pVbiPriVoid = NULL;
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,ULOGD("VBI", "enter %s %d\n",__FUNCTION__,__LINE__));

    UtopiaInstanceCreate(sizeof(VBI_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, &pVbiPriVoid);

    pVbiPri = (VBI_INSTANT_PRIVATE*)pVbiPriVoid;
    // For ATV
    pVbiPri->fpVBIGetLibVer                       = (IOCTL_VBI_GETLIBVER                       )MDrv_VBI_GetLibVer;
    pVbiPri->fpVBIGetInfo                         = (IOCTL_VBI_GETINFO                         )MDrv_VBI_GetInfo;
    pVbiPri->fpVBIGetStatus                       = (IOCTL_VBI_GETSTATUS                       )_MDrv_VBI_GetStatus;
    pVbiPri->fpVBISetDbgLevel                     = (IOCTL_VBI_SET_DBGLEVEL                    )MDrv_VBI_SetDbgLevel;
    pVbiPri->fpVBIInit                            = (IOCTL_VBI_INIT                            )_MDrv_VBI_Init;
    pVbiPri->fpVBIExit                            = (IOCTL_VBI_EXIT                            )_MDrv_VBI_Exit;
    pVbiPri->fpVBIRegisterCB                      = (IOCTL_VBI_REGISTERCB                      )_MDrv_VBI_RegisterCB;
    pVbiPri->fpVBIRingBufferReset                 = (IOCTL_VBI_RINGBUFFER_RESET                )_MDrv_VBI_RingBuffer_Reset;
    pVbiPri->fpVBIInitializeTTXSlicer             = (IOCTL_VBI_INITIALIZETTXSLICER             )_MDrv_VBI_InitializeTTXSlicer;
    pVbiPri->fpVBIEnableTTXSlicer                 = (IOCTL_VBI_ENABLETTXSLICER                 )_MDrv_VBI_EnableTTXSlicer;
    pVbiPri->fpVBIIsVPSReady                      = (IOCTL_VBI_ISVPS_READY                     )_MDrv_VBI_IsVPS_Ready;
    pVbiPri->fpVBIIsTTXReady                      = (IOCTL_VBI_ISTTX_READY                     )_MDrv_VBI_IsTTX_Ready;
    pVbiPri->fpVBIIsWSSReady                      = (IOCTL_VBI_ISWSS_READY                     )_MDrv_VBI_IsWSS_Ready;
    pVbiPri->fpVBIGetWSSData                      = (IOCTL_VBI_GETWSS_DATA                     )_MDrv_VBI_GetWSS_Data;
    pVbiPri->fpVBIGetVPSData                      = (IOCTL_VBI_GETVPS_DATA                     )_MDrv_VBI_GetVPS_Data;
    pVbiPri->fpVBISetVideoStandard                = (IOCTL_VBI_SETVIDEOSTANDARD                )_MDrv_VBI_SetVideoStandard;
    pVbiPri->fpVBITTXPacketBufferIsEmpty          = (IOCTL_VBI_TTX_PACKETBUFFERISEMPTY         )_MDrv_VBI_TTX_PacketBufferIsEmpty;
    pVbiPri->fpVBITTXCheckCircuitReady            = (IOCTL_VBI_TTX_CHECKCIRCUITREADY           )_MDrv_VBI_TTX_CheckCircuitReady;
    pVbiPri->fpVBITTXGetPacketCount               = (IOCTL_VBI_TTX_GETPACKETCOUNT              )_MDrv_VBI_TTX_GetPacketCount;
    pVbiPri->fpVBITTXGetPackets                   = (IOCTL_VBI_TTX_GETPACKETS                  )_MDrv_VBI_TTX_GetPackets;
    pVbiPri->fpVBITTXGetPacket                    = (IOCTL_VBI_TTX_GETPACKET                   )_MDrv_VBI_TTX_GetPacket;
    pVbiPri->fpVBITTXPacktetBufferIsOverflow      = (IOCTL_VBI_TTX_PACKETBUFFERISOVERFLOW      )_MDrv_VBI_TTX_PacketBufferIsOverflow;
    pVbiPri->fpVBITTXPacketBufferGetNoOfOverflows = (IOCTL_VBI_TTX_PACKETBUFFERGETNOOFOVERFLOWS)_MDrv_VBI_TTX_PacketBufferGetNoOfOverflows;
    pVbiPri->fpVBITTXEnableLine                   = (IOCTL_VBI_TTX_ENABLELINE                  )_MDrv_VBI_TTX_EnableLine;
    pVbiPri->fpVBISyncMemory                      = (IOCTL_VBI_SYNCMEMORY                      )_MDrv_VBI_SyncMemory;
    pVbiPri->fpVBICCInitSlicer                    = (IOCTL_VBI_CC_INITSLICER                   )_MDrv_VBI_CC_InitSlicer;
    pVbiPri->fpVBICCInitYPbYr                     = (IOCTL_VBI_CC_INITYPBYR                    )_MDrv_VBI_CC_InitYPbYr;
    pVbiPri->fpVBICCSetDataRate                   = (IOCTL_VBI_CC_SETDATARATE                  )_MDrv_VBI_CC_SetDataRate;
    pVbiPri->fpVBICCGetInfo                       = (IOCTL_VBI_CC_GETINFO                      )_MDrv_VBI_CC_GetInfo;
    pVbiPri->fpVBICCSetFrameCnt                   = (IOCTL_VBI_CC_SETFRAMECNT                  )_MDrv_VBI_CC_SetFrameCnt;
    pVbiPri->fpVBICCEnableSlicer                  = (IOCTL_VBI_CC_ENABLESLICER                 )_MDrv_VBI_CC_EnableSlicer;
    pVbiPri->fpVBICCEnableLine                    = (IOCTL_VBI_CC_ENABLELINE                   )_MDrv_VBI_CC_EnableLine;
    pVbiPri->fpVBICCSetSCWindowLen                = (IOCTL_VBI_CC_SETSCWINDOWLEN               )_MDrv_VBI_CC_SetSCWindowLen;
    pVbiPri->fpVBICCSetVideoStandard              = (IOCTL_VBI_CC_SETVIDEOSTANDARD             )_MDrv_VBI_CC_SetStandard;
    pVbiPri->fpVBIWSSSetVpsByteNum                = (IOCTL_VBI_WSS_SETVPSBYTENUM               )_MDrv_VBI_WSS_SetVpsByteNum;
    pVbiPri->fpVBISuspend                         = (IOCTL_VBI_SUSPEND                         )_MDrv_VBI_Suspend;
    pVbiPri->fpVBIResume                          = (IOCTL_VBI_RESUME                          )_MDrv_VBI_Resume;
    pVbiPri->fpVBISetPowerState                   = (IOCTL_VBI_SETPOWERSTATE                   )_MDrv_VBI_SetPowerState;

    // For DTV
    pVbiPri->fpDMXTTXRingBufferReset              = (IOCTL_DMX_TTX_RINGBUFFER_RESET            )_MDrv_DMX_TTX_RingBuffer_Reset;
    pVbiPri->fpDMXTTXInit                         = (IOCTL_DMX_TTX_INIT                        )_MDrv_DMX_TTX_Init;
    pVbiPri->fpDMXTTXExit                         = (IOCTL_DMX_TTX_EXIT                        )_MDrv_DMX_TTX_Exit;
    pVbiPri->fpDMXTTXSetFilterID                  = (IOCTL_DMX_TTX_SETFILTERID                 )_MDrv_DMX_TTX_SetFilterID;
    pVbiPri->fpDMXTTXSetCB                        = (IOCTL_DMX_TTX_SETCB                       )_MDrv_DMX_TTX_SetCB;
    pVbiPri->fpDMXTTXPacketBufferIsEmpty          = (IOCTL_DMX_TTX_PACKETBUFFERISEMPTY         )_MDrv_DMX_TTX_PacketBufferIsEmpty;
    pVbiPri->fpDMXTTXGetPackets                   = (IOCTL_DMX_TTX_GETPACKETS                  )_MDrv_DMX_TTX_GetPackets;
    pVbiPri->fpDMXTTXGetPacket                    = (IOCTL_DMX_TTX_GETPACKET                   )_MDrv_DMX_TTX_GetPacket;
    pVbiPri->fpDMXTTXPacketBufferIsOverflow       = (IOCTL_DMX_TTX_PACKETBUFFERISOVERFLOW      )_MDrv_DMX_TTX_PacketBufferIsOverflow;
    pVbiPri->fpDMXTTXPacketBufferGetNoOfOverflows = (IOCTL_DMX_TTX_PACKETBUFFERGETNOOFOVERFLOWS)_MDrv_DMX_TTX_PacketBufferGetNoOfOverflows;
    pVbiPri->fpDMXTTXSuspend                      = (IOCTL_DMX_TTX_SUSPEND                     )_MDrv_DMX_TTX_Suspend;
    pVbiPri->fpDMXTTXResume                       = (IOCTL_DMX_TTX_RESUME                      )_MDrv_DMX_TTX_Resume;
    pVbiPri->fpDMXTTXSetPowerState                = (IOCTL_DMX_TTX_SETPOWERSTATE               )_MDrv_DMX_TTX_SetPowerState;

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 VBIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,ULOGD("VBI", "enter %s %d\n",__FUNCTION__,__LINE__));
    VBI_DEBUG_MSG(ULOGD("VBI", "\n[VBI INFO] vbi ioctl cmd = %ld\n",u32Cmd));
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    VBI_INSTANT_PRIVATE*  psVbiInstPri = NULL;
    void* pModule       = NULL;
    void* pResource_atv = NULL;
    void* pResource_dtv = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    UtopiaInstanceGetPrivate(pInstance, (void**)&psVbiInstPri); // Get instance

    PVBI_GETLIBVER              pGetLibVer            = NULL;
    PVBI_GETINFO_PARAM          pGetInfoParam         = NULL;
    PVBI_GETSTATUS              pGetStatus            = NULL;
    PVBI_SETDBGLEVEL            pSetDbgLevel          = NULL;
    PVBI_INIT_TYPE              pInitType             = NULL;
    PVBI_REGISTER_CB            pRegisterCB           = NULL;
    PVBI_INITIALIZER_TTX_SLICER pInitializerTTXSlicer = NULL;
    PVBI_ENABLE_TTX_SLICER      pEnableTTXSlicer      = NULL;
    PVBI_GET_VPS_DATA           pGetVPSData           = NULL;
    PVBI_SET_VIDEO_STANDARD     pSetVideoStandard     = NULL;
    PVBI_TTX_GET_PACKETS        pGetPackets           = NULL;
    PVBI_TTX_GET_PACKET         pGetPacket            = NULL;
    PVBI_TTX_ENABLE_LINE        pEnableLine           = NULL;
    PVBI_SYNC_MEMORY            pSyncMemory           = NULL;
    PVBI_CC_INIT_SLICER         pCCInitSlicer         = NULL;
    PVBI_CC_INIT_YPBYR          pCCInitYPbYr          = NULL;
    PVBI_CC_SETDATARATE         pCCSetDataRate        = NULL;
    PVBI_CC_GETINFO             pGetInfo              = NULL;
    PVBI_CC_SET_FRAMECNT        pCCSetFrameCnt        = NULL;
    PVBI_CC_ENABLE_SLICER       pCCEnableSlicer       = NULL;
    PVBI_CC_ENABLE_LINE         pCCEnableLine         = NULL;
    PVBI_CC_SET_SC_WND_LEN      pCCSetWndLen          = NULL;
    PVBI_VBI_WSS_VPSBYTENUM     pWSSSetVpsByteCnt     = NULL;
    PVBI_GET_RAW_VPS_DATA       pGetRawVPSData        = NULL;
    PDMX_TTX_SET_FILTERID       pDMXSetFilterID       = NULL;
    PDMX_TTX_SET_CB             pDMXSetCB             = NULL;

    PVBI_CMD                    pVbiCmd               = NULL;
    PDMX_TTX_CMD                pDmxTtxCmd            = NULL;
    
    PVBI_GET_DATA pGetData        = NULL;
    PVBI_BOOL     pCheckTureFalse = NULL;
    PVBI_GET_RESULT             pGetResult            = NULL;
    PVBI_PROTECT_MEMORY         pProtectMemory        = NULL;

    VBI_DrvInfo *pVBIInfo;

    if(u32Cmd <= MDrv_CMD_VBI_WSS_SetVpsByteNum ||  u32Cmd == MDrv_CMD_VBI_Exit)
    {
        if(UtopiaResourceObtain(pModule, VBI_POOL_ID_VBI0, &pResource_atv) == UTOPIA_STATUS_FAIL)
        {
            return UTOPIA_STATUS_ERR_RESOURCE;
        }
    }
    else
    {
        if(UtopiaResourceObtain(pModule, VBI_POOL_ID_VBI1, &pResource_dtv) == UTOPIA_STATUS_FAIL)
        {
            return UTOPIA_STATUS_ERR_RESOURCE;
        }
    }

    switch(u32Cmd)
    {
        //=============================================
        // ATV - TTX, CC
        //=============================================
        case MDrv_CMD_VBI_GetLibVer:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_GetLibVer\n");
            pGetLibVer = (PVBI_GETLIBVER)pArgs;
            if(psVbiInstPri->fpVBIGetLibVer(pGetLibVer->ppVersion) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetInfo:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_GetInfo\n");
            pGetInfoParam = (PVBI_GETINFO_PARAM)pArgs;
            pVBIInfo = psVbiInstPri->fpVBIGetInfo();
            memcpy (pGetInfoParam,pVBIInfo,sizeof(VBI_DrvInfo));
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetStatus:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_GetStatus\n");
            pGetStatus = (PVBI_GETSTATUS)pArgs;
            if(psVbiInstPri->fpVBIGetStatus(pGetStatus->pDrvStatus) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_SetDbgLevel:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_SetDbgLevel\n");
            pSetDbgLevel = (PVBI_SETDBGLEVEL)pArgs;
            if(psVbiInstPri->fpVBISetDbgLevel(pSetDbgLevel->u16DbgSwitch) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_Init:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_Init\n");
            pInitType = (PVBI_INIT_TYPE)pArgs;
            if(psVbiInstPri->fpVBIInit(pInitType->cmd, pInitType->type) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_RegisterCB:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_RegisterCB\n");
            pRegisterCB = (PVBI_REGISTER_CB)pArgs;
            psVbiInstPri->fpVBIRegisterCB(pRegisterCB->pFN,
                                          pRegisterCB->bufferAddr,
                                          pRegisterCB->length);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_RingBuffer_Reset:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_RingBuffer_Reset\n");
            psVbiInstPri->fpVBIRingBufferReset();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_InitializeTTXSlicer:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_InitializeTTXSlicer\n");
            pInitializerTTXSlicer = (PVBI_INITIALIZER_TTX_SLICER)pArgs;
            psVbiInstPri->fpVBIInitializeTTXSlicer(pInitializerTTXSlicer->bufferAddr,
                                                   pInitializerTTXSlicer->packetCount);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_EnableTTXSlicer:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_EnableTTXSlicer\n");
            pEnableTTXSlicer = (PVBI_ENABLE_TTX_SLICER)pArgs;
            psVbiInstPri->fpVBIEnableTTXSlicer(pEnableTTXSlicer->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_IsVPS_Ready:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_IsVPS_Ready\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIIsVPSReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_IsTTX_Ready:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_IsTTX_Ready\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIIsTTXReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_IsWSS_Ready:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_IsWSS_Ready\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIIsWSSReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetWSS_Data:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_GetWSS_Data\n");
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->u16data = psVbiInstPri->fpVBIGetWSSData();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_GetVPS_Data:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_GetVPS_Data\n");
            pGetVPSData = (PVBI_GET_VPS_DATA)pArgs;
            psVbiInstPri->fpVBIGetVPSData(pGetVPSData->lowerWord, pGetVPSData->higherWord);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
		case MDrv_CMD_VBI_GetRawVPS_Data:
            //printf("VBIIoctl - MDrv_CMD_VBI_GetVPS_Data\n");            
            pGetRawVPSData = (PVBI_GET_RAW_VPS_DATA)pArgs;
            psVbiInstPri->fpVBIGetRawVPSData(&pGetRawVPSData->byte0, &pGetRawVPSData->byte1, &pGetRawVPSData->byte2, &pGetRawVPSData->byte3);            
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;            
        case MDrv_CMD_VBI_SetVideoStandard:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_SetVideoStandard\n");
            pSetVideoStandard = (PVBI_SET_VIDEO_STANDARD)pArgs;
            psVbiInstPri->fpVBISetVideoStandard(pSetVideoStandard->eStandard);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferIsEmpty:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_PacketBufferIsEmpty\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck= psVbiInstPri->fpVBITTXPacketBufferIsEmpty();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_CheckCircuitReady:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_CheckCircuitReady\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBITTXCheckCircuitReady();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_GetPacketCount:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_GetPacketCount\n");
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->u16data = psVbiInstPri->fpVBITTXGetPacketCount();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_GetPackets:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_GetPackets\n");
            pGetPackets = (PVBI_TTX_GET_PACKETS)pArgs;
            if(psVbiInstPri->fpVBITTXGetPackets(pGetPackets->dataAddr, pGetPackets->length) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_GetPacket:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_GetPacket\n");
            pGetPacket = (PVBI_TTX_GET_PACKET)pArgs;
            if(psVbiInstPri->fpVBITTXGetPacket(pGetPacket->packetAddress) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferIsOverflow:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_PacketBufferIsOverflow\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBITTXPacktetBufferIsOverflow();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows\n");
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->u16data = psVbiInstPri->fpVBITTXPacketBufferGetNoOfOverflows();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_TTX_EnableLine:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_TTX_EnableLine\n");
            pEnableLine = (PVBI_TTX_ENABLE_LINE)pArgs;
            psVbiInstPri->fpVBITTXEnableLine(pEnableLine->StartLine,
                                             pEnableLine->EndLine);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_SyncMemory:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_SyncMemory\n");
            pSyncMemory = (PVBI_SYNC_MEMORY)pArgs;
            if(psVbiInstPri->fpVBISyncMemory(pSyncMemory->u32Start, pSyncMemory->u32Size) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_InitSlicer:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_InitSlicer\n");
            pCCInitSlicer = (PVBI_CC_INIT_SLICER)pArgs;
            psVbiInstPri->fpVBICCInitSlicer(pCCInitSlicer->u32RiuAddr,
                                            pCCInitSlicer->bufferAddr,
                                            pCCInitSlicer->packetCount);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_InitYPbYr:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_InitYPbYr\n");
            pCCInitYPbYr = (PVBI_CC_INIT_YPBYR)pArgs;
            psVbiInstPri->fpVBICCInitYPbYr(pCCInitYPbYr->cvbs_no);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_SetDataRate:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_SetDataRate\n");
            pCCSetDataRate = (PVBI_CC_SETDATARATE)pArgs;
            if(psVbiInstPri->fpVBICCSetDataRate(pCCSetDataRate->ptable) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_GetInfo:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_GetInfo\n");
            pGetInfo = (PVBI_CC_GETINFO)pArgs;
            pGetInfo->info = psVbiInstPri->fpVBICCGetInfo(pGetInfo->selector);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_SetFrameCnt:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_SetFrameCnt\n");
            pCCSetFrameCnt = (PVBI_CC_SET_FRAMECNT)pArgs;
            psVbiInstPri->fpVBICCSetFrameCnt(pCCSetFrameCnt->cnt);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_EnableSlicer:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_EnableSlicer\n");
            pCCEnableSlicer = (PVBI_CC_ENABLE_SLICER)pArgs;
            psVbiInstPri->fpVBICCEnableSlicer(pCCEnableSlicer->bEnable);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_CC_EnableLine:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_EnableLine\n");
            pCCEnableLine = (PVBI_CC_ENABLE_LINE)pArgs;
            psVbiInstPri->fpVBICCEnableLine(pCCEnableLine->StartLine,
                                            pCCEnableLine->EndLine,
                                            pCCEnableLine->mode);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;

        case MDrv_CMD_VBI_CC_SetSCWindowLen:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_SetSCWindowLen\n");
            pCCSetWndLen = (PVBI_CC_SET_SC_WND_LEN)pArgs;
            pCCSetWndLen->bRet = psVbiInstPri->fpVBICCSetSCWindowLen(pCCSetWndLen->u8Len);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;            
        
        case MDrv_CMD_VBI_CC_SetVideoStandard:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_CC_SetVideoStandard\n");
            pSetVideoStandard = (PVBI_SET_VIDEO_STANDARD)pArgs;
            pSetVideoStandard->bRet = psVbiInstPri->fpVBICCSetVideoStandard(pSetVideoStandard->eStandard);
            u32Ret = UTOPIA_STATUS_SUCCESS;            
            break;
        case MDrv_CMD_VBI_WSS_SetVpsByteNum:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_WSS_SetVpsByteNum\n");
            pWSSSetVpsByteCnt = (PVBI_VBI_WSS_VPSBYTENUM)pArgs;
            psVbiInstPri->fpVBIWSSSetVpsByteNum(pWSSSetVpsByteCnt->cnt);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;

        case MDrv_CMD_VBI_Suspend:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_Suspend\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBISuspend();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_Resume:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_Resume\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpVBIResume();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_SetPowerState:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_SetPowerState\n");
            pGetResult = (PVBI_GET_RESULT)pArgs;
            pGetResult->u32result= psVbiInstPri->fpVBISetPowerState(pGetResult->u16PowerState);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;

        case MDrv_CMD_VBI_ProtectMemory:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_ProtectMemory\n");
            pProtectMemory = (PVBI_PROTECT_MEMORY)pArgs;
            pProtectMemory->bRet = psVbiInstPri->fpVBIProtectMemory(pProtectMemory->bEnable,
                                                                    pProtectMemory->phyAddr,
                                                                    pProtectMemory->u32Size);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        //=============================================
        // DTV - TTX
        //=============================================
        case MDrv_CMD_DMX_TTX_RingBuffer_Reset:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_RingBuffer_Reset\n");
            psVbiInstPri->fpDMXTTXRingBufferReset();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_Init:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_Init\n");
            pInitializerTTXSlicer = (PVBI_INITIALIZER_TTX_SLICER)pArgs;
            if(psVbiInstPri->fpDMXTTXInit(pInitializerTTXSlicer->cmd, pInitializerTTXSlicer->bufferAddr, pInitializerTTXSlicer->packetCount) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_SetFilterID:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_SetFilterID\n");
            pDMXSetFilterID = (PDMX_TTX_SET_FILTERID)pArgs;
            psVbiInstPri->fpDMXTTXSetFilterID(pDMXSetFilterID->fid);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_SetCB:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_SetCB\n");
            pDMXSetCB = (PDMX_TTX_SET_CB)pArgs;
            psVbiInstPri->fpDMXTTXSetCB(pDMXSetCB->fn);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferIsEmpty:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_PacketBufferIsEmpty\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpDMXTTXPacketBufferIsEmpty();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_GetPackets:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_GetPackets\n");
            pGetPackets = (PVBI_TTX_GET_PACKETS)pArgs;
            if(psVbiInstPri->fpDMXTTXGetPackets(pGetPackets->dataAddr, pGetPackets->length) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_GetPacket:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_GetPacket\n");
            pGetPacket = (PVBI_TTX_GET_PACKET)pArgs;
            if(psVbiInstPri->fpDMXTTXGetPacket(pGetPacket->packetAddress) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferIsOverflow:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_PacketBufferIsOverflow\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpDMXTTXPacketBufferIsOverflow();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows\n");
            pGetData = (PVBI_GET_DATA)pArgs;
            pGetData->u16data = psVbiInstPri->fpDMXTTXPacketBufferGetNoOfOverflows();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_VBI_Exit:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_VBI_Exit\n");
            pVbiCmd = (PVBI_CMD)pArgs;
            if(psVbiInstPri->fpVBIExit(pVbiCmd->cmd) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_Exit:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_Exit\n");
            pDmxTtxCmd = (PDMX_TTX_CMD)pArgs;
            if(psVbiInstPri->fpDMXTTXExit(pDmxTtxCmd->cmd) == TRUE)
                u32Ret = UTOPIA_STATUS_SUCCESS;
            break;

        case MDrv_CMD_DMX_TTX_Suspend:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_Suspend\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpDMXTTXSuspend();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_Resume:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_Resume\n");
            pCheckTureFalse = (PVBI_BOOL)pArgs;
            pCheckTureFalse->bCheck = psVbiInstPri->fpDMXTTXResume();
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        case MDrv_CMD_DMX_TTX_SetPowerState:
            //ULOGD("VBI", "VBIIoctl - MDrv_CMD_DMX_TTX_SetPowerState\n");
            pGetResult = (PVBI_GET_RESULT)pArgs;
            pGetResult->u32result= psVbiInstPri->fpDMXTTXSetPowerState(pGetResult->u16PowerState);
            u32Ret = UTOPIA_STATUS_SUCCESS;
            break;
        default:
            break;
    };

    if(u32Cmd <= MDrv_CMD_VBI_WSS_SetVpsByteNum ||  u32Cmd == MDrv_CMD_VBI_Exit )
    {
        UtopiaResourceRelease(pResource_atv);
    }
    else
    {
        UtopiaResourceRelease(pResource_dtv);
    }

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,ULOGD("VBI", "leave %s %d\n",__FUNCTION__,__LINE__));
    return u32Ret; // FIXME: error code
}

MS_U32 VBIClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);
    return UTOPIA_STATUS_SUCCESS;
}

