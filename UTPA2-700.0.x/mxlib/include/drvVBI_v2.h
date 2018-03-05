//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************

#ifndef __DRV_VBI_V2_H__
#define __DRV_VBI_V2_H__

#include "drvVBI.h"
#include "drvDMX_TTX.h"
#include "UFO.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    VBI_POOL_ID_VBI0 = 0,
    VBI_POOL_ID_VBI1 = 1,
} eVBIPoolID;

typedef enum {
    // VBI Teletext
    MDrv_CMD_VBI_RingBuffer_Reset = 0,
    MDrv_CMD_VBI_RegisterCB,
    MDrv_CMD_VBI_InitializeTTXSlicer,
    MDrv_CMD_VBI_EnableTTXSlicer,
    MDrv_CMD_VBI_IsVPS_Ready,
    MDrv_CMD_VBI_IsTTX_Ready,
    MDrv_CMD_VBI_IsWSS_Ready,
    MDrv_CMD_VBI_GetWSS_Data,
    MDrv_CMD_VBI_GetVPS_Data,
    MDrv_CMD_VBI_SetVideoStandard,
    MDrv_CMD_VBI_TTX_PacketBufferIsEmpty,
    MDrv_CMD_VBI_TTX_CheckCircuitReady,
    MDrv_CMD_VBI_TTX_GetPacketCount,
    MDrv_CMD_VBI_TTX_GetPackets,
    MDrv_CMD_VBI_TTX_GetPacket,
    MDrv_CMD_VBI_TTX_PacketBufferIsOverflow,
    MDrv_CMD_VBI_TTX_PacketBufferGetNoOfOverflows,
    MDrv_CMD_VBI_TTX_EnableLine,
    MDrv_CMD_VBI_WSS_SetVpsByteNum,
    MDrv_CMD_VBI_GetCompleteVPS_Data,
    MDrv_CMD_VBI_GetRawVPS_Data,
    
    // VBI CC
    MDrv_CMD_VBI_CC_InitSlicer = 100,
    MDrv_CMD_VBI_CC_InitYPbYr,
    MDrv_CMD_VBI_CC_SetDataRate,
    MDrv_CMD_VBI_CC_GetInfo,
    MDrv_CMD_VBI_CC_SetFrameCnt,
    MDrv_CMD_VBI_CC_EnableSlicer,
    MDrv_CMD_VBI_CC_EnableLine,
    MDrv_CMD_VBI_CC_SetSCWindowLen,
    MDrv_CMD_VBI_CC_SetVideoStandard,
    
    // DMX Teletext
    MDrv_CMD_DMX_TTX_RingBuffer_Reset = 200,
    MDrv_CMD_DMX_TTX_Init,
    MDrv_CMD_DMX_TTX_SetFilterID,
    MDrv_CMD_DMX_TTX_SetCB,
    MDrv_CMD_DMX_TTX_PacketBufferIsEmpty,
    MDrv_CMD_DMX_TTX_GetPackets,
    MDrv_CMD_DMX_TTX_GetPacket,
    MDrv_CMD_DMX_TTX_PacketBufferIsOverflow,
    MDrv_CMD_DMX_TTX_PacketBufferGetNoOfOverflows,
    MDrv_CMD_DMX_TTX_Exit,
    
    // MISC
    MDrv_CMD_VBI_ObtainEng = 300,
    MDrv_CMD_VBI_ReleaseEng,
    MDrv_CMD_VBI_QueryCap,
    MDrv_CMD_VBI_GetLibVer,
    MDrv_CMD_VBI_GetInfo,
    MDrv_CMD_VBI_GetStatus,
    MDrv_CMD_VBI_SetDbgLevel,
    MDrv_CMD_VBI_Init,
    MDrv_CMD_VBI_SyncMemory,
    MDrv_CMD_VBI_Exit,    
    MDrv_CMD_VBI_ProtectMemory,

    MDrv_CMD_VBI_Suspend,
    MDrv_CMD_VBI_Resume,
    MDrv_CMD_VBI_SetPowerState,

    MDrv_CMD_DMX_TTX_Suspend,
    MDrv_CMD_DMX_TTX_Resume,
    MDrv_CMD_DMX_TTX_SetPowerState,
} eVBIIoctlOpt;

typedef struct DLL_PACKED _VBI_BOOL
{
    MS_BOOL bCheck;
} VBI_BOOL, *PVBI_BOOL;

typedef struct DLL_PACKED _VBI_GET_DATA
{
    MS_U16 u16data;
} VBI_GET_DATA, *PVBI_GET_DATA;

typedef struct _VBI_GET_RESULT
{
    EN_POWER_MODE u16PowerState;
    MS_U32 u32result;
} VBI_GET_RESULT, *PVBI_GET_RESULT;

//-------------------------------------------------------------

typedef struct _VBI_GETLIBVER
{
    const MSIF_Version **ppVersion;
} VBI_GETLIBVER_PARAM, *PVBI_GETLIBVER;

typedef struct DLL_PACKED _VBI_GETINFO
{
    MS_U8 u8NoInfo;
} VBI_GETINFO_PARAM, *PVBI_GETINFO_PARAM;

typedef struct DLL_PACKED _GETSTATUS
{
    VBI_DrvStatus *pDrvStatus;
} VBI_GETSTATUS_PARAM, *PVBI_GETSTATUS;

typedef struct DLL_PACKED _VBI_SETDBGLEVEL
{
    MS_U16 u16DbgSwitch;
} VBI_SETDBGLEVEL_PARAM, *PVBI_SETDBGLEVEL;

typedef struct DLL_PACKED _VBI_CMD
{
    EN_VBI_CMD cmd;
} VBI_CMD, *PVBI_CMD;

typedef struct DLL_PACKED _VBI_INIT_TYPE
{
    EN_VBI_CMD cmd;
    VBI_INIT_TYPE type;
} VBI_INIT_TYPE_PARAM, *PVBI_INIT_TYPE;

typedef struct DLL_PACKED _DMX_TTX_CMD
{
    EN_VBI_CMD cmd;
} DMX_TTX_CMD, *PDMX_TTX_CMD;

typedef struct DLL_PACKED _VBI_REGISTER_CB
{
    VBI_CB_FN pFN;
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR bufferAddr;
#else
    MS_PHY     bufferAddr;
#endif    
    MS_U32 length;
} VBI_REGISTER_CB, *PVBI_REGISTER_CB;

typedef struct DLL_PACKED _VBI_INITIALIZER_TTX_SLICER
{
    EN_VBI_CMD cmd;
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR bufferAddr;
#else
    MS_PHY     bufferAddr;
#endif    
    MS_U16 packetCount;
} VBI_INITIALIZER_TTX_SLICER, *PVBI_INITIALIZER_TTX_SLICER;

typedef struct DLL_PACKED _VBI_ENABLE_TTX_SLICER
{
    MS_BOOL bEnable;
} VBI_ENABLE_TTX_SLICER, *PVBI_ENABLE_TTX_SLICER, VBI_CC_ENABLE_SLICER, *PVBI_CC_ENABLE_SLICER;

typedef struct DLL_PACKED _VBI_GET_VPS_DATA
{
    MS_U8 *lowerWord;
    MS_U8 *higherWord;
} VBI_GET_VPS_DATA, *PVBI_GET_VPS_DATA;

typedef struct DLL_PACKED _VBI_GET_COMPLETE_VPS_DATA
{
    MS_U8  **pBuffer;
    MS_U32 *length;
} VBI_GET_COMPLETE_VPS_DATA, *PVBI_GET_COMPLETE_VPS_DATA;

typedef struct DLL_PACKED _VBI_GET_RAW_VPS_DATA
{
    MS_U8 byte0;
    MS_U8 byte1;
    MS_U8 byte2;
    MS_U8 byte3;
} VBI_GET_RAW_VPS_DATA, *PVBI_GET_RAW_VPS_DATA;

typedef struct DLL_PACKED _VBI_SET_VIDEO_STANDARD
{
    VBI_VIDEO_STANDARD eStandard;
    MS_BOOL bRet;
} VBI_SET_VIDEO_STANDARD, *PVBI_SET_VIDEO_STANDARD;


typedef struct DLL_PACKED _VBI_TTX_GET_PACKETS
{
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR dataAddr;
#else
    MS_PHY     dataAddr;
#endif    
    MS_U32 *length;
} VBI_TTX_GET_PACKETS, *PVBI_TTX_GET_PACKETS;

typedef struct DLL_PACKED _VBI_TTX_GET_PACKET
{
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_PHYADDR *packetAddress;
#else
    MS_PHY     *packetAddress;
#endif    
} VBI_TTX_GET_PACKET, *PVBI_TTX_GET_PACKET;

typedef struct DLL_PACKED _VBI_TTX_ENABLE_LINE
{
    MS_U16 StartLine;
    MS_U16 EndLine;
} VBI_TTX_ENABLE_LINE, *PVBI_TTX_ENABLE_LINE;

typedef struct DLL_PACKED _VBI_SYNC_MEMORY
{
    MS_U32 u32Start;
    MS_U32 u32Size;
} VBI_SYNC_MEMORY, *PVBI_SYNC_MEMORY;

typedef struct DLL_PACKED _VBI_CC_INIT_SLICER
{
#if defined(UFO_PUBLIC_HEADER_300)    
    MS_U32     u32RiuAddr;
    MS_PHYADDR bufferAddr;
#else
    MS_VIRT    u32RiuAddr;
    MS_PHY     bufferAddr;
#endif    
    MS_U16 packetCount;
} VBI_CC_INIT_SLICER, *PVBI_CC_INIT_SLICER;

typedef struct DLL_PACKED _VBI_CC_INIT_YPBYR
{
    MS_U8 cvbs_no;
} VBI_CC_INIT_YPBYR, *PVBI_CC_INIT_YPBYR;

typedef struct DLL_PACKED _VBI_CC_SETDATARATE
{
    MS_U8 *ptable;
} VBI_CC_SETDATARATE, *PVBI_CC_SETDATARATE;

typedef struct DLL_PACKED _VBI_CC_GETINFO
{
    MS_U32 selector;
    MS_U32 info;
} VBI_CC_GETINFO, *PVBI_CC_GETINFO;

typedef struct DLL_PACKED _VBI_CC_SET_FRAMECNT
{
    MS_U8 cnt;
} VBI_CC_SET_FRAMECNT, *PVBI_CC_SET_FRAMECNT, VBI_VBI_WSS_VPSBYTENUM, *PVBI_VBI_WSS_VPSBYTENUM;

typedef struct DLL_PACKED _VBI_CC_ENABLE_LINE
{
    MS_U16 StartLine;
    MS_U16 EndLine;
    MS_U8 mode;
} VBI_CC_ENABLE_LINE, *PVBI_CC_ENABLE_LINE;

typedef struct DLL_PACKED _VBI_CC_SET_SC_WND_LEN
{
    MS_U8 u8Len;
    MS_BOOL bRet;
} VBI_CC_SET_SC_WND_LEN, *PVBI_CC_SET_SC_WND_LEN;

typedef struct DLL_PACKED _VBI_PROTECT_MEMORY
{
    MS_BOOL bEnable;
    MS_PHY phyAddr;
    MS_U32 u32Size;
    MS_BOOL bRet;
} VBI_PROTECT_MEMORY, *PVBI_PROTECT_MEMORY;

typedef struct DLL_PACKED _DMX_TTX_SET_FILTERID
{
    MS_U8 fid;
} DMX_TTX_SET_FILTERID, *PDMX_TTX_SET_FILTERID;

typedef struct DLL_PACKED _DMX_TTX_SET_CB
{
    DMX_CB_FN fn;
} DMX_TTX_SET_CB, *PDMX_TTX_SET_CB;

#ifdef __cplusplus
}
#endif

#endif //__DRV_VBI_V2_H__

