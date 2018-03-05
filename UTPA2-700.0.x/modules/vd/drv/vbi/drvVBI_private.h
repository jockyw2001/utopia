#include "drvVBI.h"
#include "drvDMX_TTX.h"

#include "MsTypes.h"

#ifndef __VBI_PRIVATE_H__
#define __VBI_PRIVATE_H__

typedef enum E_VBI_RES_ID {
    E_VBI_RESOURCE,
} eVbiResourceId;

typedef struct
{
    MS_S32 ReadPacketNumber;
    MS_S32 WritePacketNumber;
    MS_U16 NoOfPacketBufferOverflows;

    MS_BOOL pkt_received;
    MS_BOOL PacketBufferOverflow;
} VBI_RING_BUFFER;

typedef struct _VBI_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
} VBI_RESOURCE_PRIVATE;

typedef MS_BOOL (*IOCTL_VBI_GETLIBVER)(const MSIF_Version **);
typedef VBI_DrvInfo * (*IOCTL_VBI_GETINFO)(void);
typedef MS_BOOL (*IOCTL_VBI_GETSTATUS)(VBI_DrvStatus*);
typedef MS_BOOL (*IOCTL_VBI_SET_DBGLEVEL)(MS_U16);
typedef MS_BOOL (*IOCTL_VBI_INIT)(EN_VBI_CMD, VBI_INIT_TYPE);
typedef MS_BOOL (*IOCTL_VBI_EXIT)(EN_VBI_CMD);
typedef void    (*IOCTL_VBI_REGISTERCB)(VBI_CB_FN, MS_PHY, MS_U32);
typedef void    (*IOCTL_VBI_RINGBUFFER_RESET)(void);
typedef void    (*IOCTL_VBI_INITIALIZETTXSLICER)(MS_PHY, MS_U16);
typedef void    (*IOCTL_VBI_ENABLETTXSLICER)(MS_BOOL);
typedef MS_BOOL (*IOCTL_VBI_ISVPS_READY)(void);
typedef MS_BOOL (*IOCTL_VBI_ISTTX_READY)(void);
typedef MS_BOOL (*IOCTL_VBI_ISWSS_READY)(void);
typedef MS_U16  (*IOCTL_VBI_GETWSS_DATA)(void);
typedef void    (*IOCTL_VBI_GETVPS_DATA)(MS_U8*, MS_U8*);
typedef void    (*IOCTL_VBI_SETVIDEOSTANDARD)(VBI_VIDEO_STANDARD);
typedef MS_BOOL (*IOCTL_VBI_TTX_PACKETBUFFERISEMPTY)(void);
typedef MS_BOOL (*IOCTL_VBI_TTX_CHECKCIRCUITREADY)(void);
typedef MS_U16  (*IOCTL_VBI_TTX_GETPACKETCOUNT)(void);
typedef MS_BOOL (*IOCTL_VBI_TTX_GETPACKETS)(MS_PHY, MS_U32*);
typedef MS_BOOL (*IOCTL_VBI_TTX_GETPACKET)(MS_PHY*);
typedef MS_BOOL (*IOCTL_VBI_TTX_PACKETBUFFERISOVERFLOW)( void );
typedef MS_U16  (*IOCTL_VBI_TTX_PACKETBUFFERGETNOOFOVERFLOWS)( void );
typedef void    (*IOCTL_VBI_TTX_ENABLELINE)(MS_U16, MS_U16);
typedef MS_BOOL (*IOCTL_VBI_SYNCMEMORY)( MS_U32, MS_U32);
typedef void    (*IOCTL_VBI_CC_INITSLICER)(MS_U32, MS_PHY, MS_U16);
typedef void    (*IOCTL_VBI_CC_INITYPBYR)(MS_U8);
typedef MS_U8   (*IOCTL_VBI_CC_SETDATARATE)(MS_U8*);
typedef MS_U32  (*IOCTL_VBI_CC_GETINFO)(MS_U32);
typedef void    (*IOCTL_VBI_CC_SETFRAMECNT)(MS_U8);
typedef void    (*IOCTL_VBI_CC_ENABLESLICER)(MS_BOOL);
typedef void    (*IOCTL_VBI_CC_ENABLELINE)(MS_U16, MS_U16, MS_U8);
typedef MS_BOOL (*IOCTL_VBI_CC_SETSCWINDOWLEN)(MS_U8);
typedef MS_BOOL (*IOCTL_VBI_CC_SETVIDEOSTANDARD)(VBI_VIDEO_STANDARD);
typedef void    (*IOCTL_VBI_WSS_SETVPSBYTENUM)(MS_U8);
typedef MS_BOOL (*IOCTL_VBI_GETRAWVPS_DATA)(MS_U8*, MS_U8*, MS_U8*, MS_U8*);
typedef MS_BOOL (*IOCTL_VBI_SUSPEND)( void );
typedef MS_BOOL (*IOCTL_VBI_RESUME)( void );
typedef MS_U32  (*IOCTL_VBI_SETPOWERSTATE)( EN_POWER_MODE );
typedef MS_U32  (*IOCTL_VBI_PROTECTMEMORY)( MS_BOOL, MS_PHY, MS_U32 );

typedef void    (*IOCTL_DMX_TTX_RINGBUFFER_RESET)(void);
typedef MS_BOOL (*IOCTL_DMX_TTX_INIT)(EN_DMX_TTX_CMD, MS_PHY, MS_U16);
typedef MS_BOOL (*IOCTL_DMX_TTX_EXIT)(EN_DMX_TTX_CMD);
typedef void    (*IOCTL_DMX_TTX_SETFILTERID)(MS_U8);
typedef void    (*IOCTL_DMX_TTX_SETCB)(DMX_CB_FN);
typedef MS_BOOL (*IOCTL_DMX_TTX_PACKETBUFFERISEMPTY)(void);
typedef MS_BOOL (*IOCTL_DMX_TTX_GETPACKETS)(MS_PHY, MS_U32*);
typedef MS_BOOL (*IOCTL_DMX_TTX_GETPACKET)(MS_PHY*);
typedef MS_BOOL (*IOCTL_DMX_TTX_PACKETBUFFERISOVERFLOW)( void );
typedef MS_U16  (*IOCTL_DMX_TTX_PACKETBUFFERGETNOOFOVERFLOWS)( void );
typedef MS_BOOL (*IOCTL_DMX_TTX_SUSPEND)( void );
typedef MS_BOOL (*IOCTL_DMX_TTX_RESUME)( void );
typedef MS_U32  (*IOCTL_DMX_TTX_SETPOWERSTATE)( EN_POWER_MODE );

typedef struct _VBI_INSTANT_PRIVATE
{
    IOCTL_VBI_GETLIBVER                        fpVBIGetLibVer;
    IOCTL_VBI_GETINFO                          fpVBIGetInfo;
    IOCTL_VBI_GETSTATUS                        fpVBIGetStatus;
    IOCTL_VBI_SET_DBGLEVEL                     fpVBISetDbgLevel;
    IOCTL_VBI_INIT                             fpVBIInit;
    IOCTL_VBI_EXIT                             fpVBIExit;
    IOCTL_VBI_REGISTERCB                       fpVBIRegisterCB;
    IOCTL_VBI_RINGBUFFER_RESET                 fpVBIRingBufferReset;
    IOCTL_VBI_INITIALIZETTXSLICER              fpVBIInitializeTTXSlicer;
    IOCTL_VBI_ENABLETTXSLICER                  fpVBIEnableTTXSlicer;
    IOCTL_VBI_ISVPS_READY                      fpVBIIsVPSReady;
    IOCTL_VBI_ISTTX_READY                      fpVBIIsTTXReady;
    IOCTL_VBI_ISWSS_READY                      fpVBIIsWSSReady;
    IOCTL_VBI_GETWSS_DATA                      fpVBIGetWSSData;
    IOCTL_VBI_GETVPS_DATA                      fpVBIGetVPSData;
    IOCTL_VBI_SETVIDEOSTANDARD                 fpVBISetVideoStandard;
    IOCTL_VBI_TTX_PACKETBUFFERISEMPTY          fpVBITTXPacketBufferIsEmpty;
    IOCTL_VBI_TTX_CHECKCIRCUITREADY            fpVBITTXCheckCircuitReady;
    IOCTL_VBI_TTX_GETPACKETCOUNT               fpVBITTXGetPacketCount;
    IOCTL_VBI_TTX_GETPACKETS                   fpVBITTXGetPackets;
    IOCTL_VBI_TTX_GETPACKET                    fpVBITTXGetPacket;
    IOCTL_VBI_TTX_PACKETBUFFERISOVERFLOW       fpVBITTXPacktetBufferIsOverflow;
    IOCTL_VBI_TTX_PACKETBUFFERGETNOOFOVERFLOWS fpVBITTXPacketBufferGetNoOfOverflows;
    IOCTL_VBI_TTX_ENABLELINE                   fpVBITTXEnableLine;
    IOCTL_VBI_SYNCMEMORY                       fpVBISyncMemory;
    IOCTL_VBI_CC_INITSLICER                    fpVBICCInitSlicer;
    IOCTL_VBI_CC_INITYPBYR                     fpVBICCInitYPbYr;
    IOCTL_VBI_CC_SETDATARATE                   fpVBICCSetDataRate;
    IOCTL_VBI_CC_GETINFO                       fpVBICCGetInfo;
    IOCTL_VBI_CC_SETFRAMECNT                   fpVBICCSetFrameCnt;
    IOCTL_VBI_CC_ENABLESLICER                  fpVBICCEnableSlicer;
    IOCTL_VBI_CC_ENABLELINE                    fpVBICCEnableLine;
    IOCTL_VBI_CC_SETSCWINDOWLEN                fpVBICCSetSCWindowLen;
    IOCTL_VBI_CC_SETVIDEOSTANDARD              fpVBICCSetVideoStandard;
    IOCTL_VBI_GETRAWVPS_DATA                   fpVBIGetRawVPSData;
    IOCTL_VBI_WSS_SETVPSBYTENUM                fpVBIWSSSetVpsByteNum;
    IOCTL_VBI_SUSPEND                          fpVBISuspend;
    IOCTL_VBI_RESUME                           fpVBIResume;
    IOCTL_VBI_SETPOWERSTATE                    fpVBISetPowerState;
    IOCTL_VBI_PROTECTMEMORY                    fpVBIProtectMemory;

    IOCTL_DMX_TTX_RINGBUFFER_RESET             fpDMXTTXRingBufferReset;
    IOCTL_DMX_TTX_INIT                         fpDMXTTXInit;
    IOCTL_DMX_TTX_EXIT                         fpDMXTTXExit;
    IOCTL_DMX_TTX_SETFILTERID                  fpDMXTTXSetFilterID;
    IOCTL_DMX_TTX_SETCB                        fpDMXTTXSetCB;
    IOCTL_DMX_TTX_PACKETBUFFERISEMPTY          fpDMXTTXPacketBufferIsEmpty;
    IOCTL_DMX_TTX_GETPACKETS                   fpDMXTTXGetPackets;
    IOCTL_DMX_TTX_GETPACKET                    fpDMXTTXGetPacket;
    IOCTL_DMX_TTX_PACKETBUFFERISOVERFLOW       fpDMXTTXPacketBufferIsOverflow;
    IOCTL_DMX_TTX_PACKETBUFFERGETNOOFOVERFLOWS fpDMXTTXPacketBufferGetNoOfOverflows;
    IOCTL_DMX_TTX_SUSPEND                      fpDMXTTXSuspend;
    IOCTL_DMX_TTX_RESUME                       fpDMXTTXResume;
    IOCTL_DMX_TTX_SETPOWERSTATE                fpDMXTTXSetPowerState;

}VBI_INSTANT_PRIVATE;

//---------------------------------------------------------------------------------------
MS_BOOL _MDrv_VBI_GetStatus(VBI_DrvStatus *pDrvStatus);
MS_BOOL _MDrv_VBI_Init(EN_VBI_CMD cmd, VBI_INIT_TYPE type);
MS_BOOL _MDrv_VBI_Exit(EN_VBI_CMD cmd);
void    _MDrv_VBI_RegisterCB(VBI_CB_FN pFN, MS_PHY bufferAddr, MS_U32 length);
void    _MDrv_VBI_RingBuffer_Reset(void);
void    _MDrv_VBI_InitializeTTXSlicer(MS_PHY bufferAddr, MS_U16 packetCount);
void    _MDrv_VBI_EnableTTXSlicer(MS_BOOL bEnable);
MS_BOOL _MDrv_VBI_IsVPS_Ready(void);
MS_BOOL _MDrv_VBI_IsTTX_Ready(void);
MS_BOOL _MDrv_VBI_IsWSS_Ready(void);
MS_U16  _MDrv_VBI_GetWSS_Data(void);
void    _MDrv_VBI_GetVPS_Data(MS_U8 *lowerWord, MS_U8 *higherWord);
void    _MDrv_VBI_SetVideoStandard(VBI_VIDEO_STANDARD eStandard);
MS_BOOL _MDrv_VBI_TTX_PacketBufferIsEmpty(void);
MS_BOOL _MDrv_VBI_TTX_CheckCircuitReady(void);
MS_U16  _MDrv_VBI_TTX_GetPacketCount(void);
MS_BOOL _MDrv_VBI_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length);
MS_BOOL _MDrv_VBI_TTX_GetPacket(MS_PHY *packetAddress);
MS_BOOL _MDrv_VBI_TTX_PacketBufferIsOverflow( void );
MS_U16  _MDrv_VBI_TTX_PacketBufferGetNoOfOverflows( void );
void    _MDrv_VBI_TTX_EnableLine(MS_U16 StartLine, MS_U16 EndLine);
MS_BOOL _MDrv_VBI_SyncMemory( MS_U32 u32Start, MS_U32 u32Size );
void    _MDrv_VBI_CC_InitSlicer(MS_VIRT u32RiuAddr, MS_PHY bufferAddr, MS_U16 packetCount);
void    _MDrv_VBI_CC_InitYPbYr(MS_U8 cvbs_no);
MS_U8   _MDrv_VBI_CC_SetDataRate(MS_U8 *ptable);
MS_U32  _MDrv_VBI_CC_GetInfo(MS_U32 selector);
void    _MDrv_VBI_CC_SetFrameCnt(MS_U8 cnt);
void    _MDrv_VBI_CC_EnableSlicer(MS_BOOL bEnable);
void    _MDrv_VBI_CC_EnableLine(MS_U16 StartLine, MS_U16 EndLine, MS_U8 mode);
MS_BOOL _MDrv_VBI_CC_SetSCWindowLen(MS_U8 u8Len);
MS_BOOL _MDrv_VBI_CC_SetStandard(VBI_VIDEO_STANDARD eStandard);
void    _MDrv_VBI_WSS_SetVpsByteNum(MS_U8 cnt);
void    _MDrv_VBI_GetRawVPS_Data(MS_U8 *byte0, MS_U8 *byte1, MS_U8 *byte2, MS_U8 *byte3);
MS_BOOL _MDrv_VBI_Suspend(void);
MS_BOOL _MDrv_VBI_Resume(void);
MS_U32  _MDrv_VBI_SetPowerState(EN_POWER_MODE u16PowerState);

void    _MDrv_DMX_TTX_RingBuffer_Reset(void);
MS_BOOL _MDrv_DMX_TTX_Init(EN_DMX_TTX_CMD cmd, MS_PHY bufferAddr, MS_U16 packetCount);
MS_BOOL _MDrv_DMX_TTX_Exit(EN_DMX_TTX_CMD cmd);
void    _MDrv_DMX_TTX_SetFilterID(MS_U8 fid);
void    _MDrv_DMX_TTX_SetCB(DMX_CB_FN fn);
MS_BOOL _MDrv_DMX_TTX_PacketBufferIsEmpty(void);
MS_BOOL _MDrv_DMX_TTX_GetPackets(MS_PHY dataAddr, MS_U32 *length);
MS_BOOL _MDrv_DMX_TTX_GetPacket(MS_PHY *packetAddress);
MS_BOOL _MDrv_DMX_TTX_PacketBufferIsOverflow( void );
MS_U16  _MDrv_DMX_TTX_PacketBufferGetNoOfOverflows( void );
MS_BOOL _MDrv_DMX_TTX_Suspend(void);
MS_BOOL _MDrv_DMX_TTX_Resume(void);
MS_U32  _MDrv_DMX_TTX_SetPowerState(EN_POWER_MODE u16PowerState);

//---------------------------------------------------------------------------------------

void   VBIRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 VBIOpen(void** ppInstance, const void* const pAttribute);
MS_U32 VBIClose(void* pInstance);
MS_U32 VBIIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 VBIStr(MS_U32 u32PowerState, void* pModule);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 VBIMdbIoctl(MS_U32 cmd, const void* const pArgs);
#endif
#endif //__VBI_PRIVATE_H__
