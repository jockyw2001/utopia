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
// comply with all applicable laws and regulations:
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
//    or otherwise:
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
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   drvRASP.h
/// @brief  RASP Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVRASP_H_
#define _DRVRASP_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "UFO.h"
//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
#define RASP_PIDFLT_START_NUM        0
#define RASP_PIDFLT_END_NUM          24

#define RASP_PIDFLT_NUM_ALL          24

#define RASP_ECMFLT_START_NUM        0
#define RASP_ECMFLT_END_NUM          6

#define RASP_ECMFLT_NUM_ALL          6

#ifdef HW_PVR_ENABLE
    #define PVR_PIDFLT_START_NUM        0
    #define PVR_PIDFLT_END_NUM          16

    #define PVR_PIDFLT_NUM_ALL          16
#endif //#endif HW_PVR_ENABLE

#define CALLBACK_SIZE_MIN               16//due to current MIU alignment's up to 16 bytes

#define DRVRASP_PID_NULL             0x1FFF                                                 // Transport stream null PID
#define MSIF_RASP_LIB_CODE           {'R','A','S','P'}                                      // Lib code
#define MSIF_RASP_LIBVER             {'0','2'}                                              // LIB version
#define MSIF_RASP_BUILDNUM           {'0','0'}                                              // Build Number
#define MSIF_RASP_CHANGELIST         {'0','0','5','1','1','4','7','0'}                      // P4 ChangeList Number

#define RASP_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                            /* 'MSIF'                                           */  \
    MSIF_CLASS,                          /* '00'                                             */  \
    MSIF_CUS,                            /* 0x0000                                           */  \
    MSIF_MOD,                            /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                   \
    MSIF_CPU,                                                                                    \
    MSIF_RASP_LIB_CODE,                  /* IP__                                             */  \
    MSIF_RASP_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_RASP_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_RASP_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------

/// RASP DDI return value
/// @name TSP_Result
/// @ref TSP_Result
/// return value
/// @{
typedef enum
{
    DRVRASP_FAIL                   = 0,
    DRVRASP_OK,
    DRVRASP_INVALID_PARAM,
    DRVRASP_FUNC_ERROR,
} RASP_RESULT;

typedef enum
{
    E_NDS_OK,
    E_NDS_FAIL,

} NDS_Result;

typedef struct _NDS_RASP_Param
{
    MS_PHYADDR                      ecm_addr;                       // NDS_CAP_BUF_SIZE * NDS_CAP_ECM_NUM
    MS_U8                          *pu8ecm_buf;                     // virtual address of ecm address
    MS_U32                          ecm_size;                       // total buffer size from emm_ecm_addr

    MS_PHYADDR                      payload_addr;                   // NDS_CAP_BUF_SIZE * NDS_CAP_ECM_NUM
    MS_U8                          *pu8playload_buf;                // virtual address of ecm address
    MS_U32                          payload_size;                   // total buffer size from emm_ecm_addr

} NDS_RASP_Param;

#define DrvRASP_Event                    MS_U32
#define E_DRVRASP_EVENT_DATA_INIT        0x00000000
//PVR buffer callback size met
#define E_DRVRASP_EVENT_CALLBACK_SIZE_MET 0x00000040

// Payload/Event Mask flag
//#define RASP_BYPASS_MASK            0xffffffff
#define E_DRVRASP_BYPASS_AFE             0x00000001      // adaptation field extension
#define E_DRVRASP_BYPASS_TPD             0x00000002      // transport private data
#define E_DRVRASP_BYPASS_SP              0x00000004      // splicing point
#define E_DRVRASP_BYPASS_OPCR            0x00000008      // OPCR
#define E_DRVRASP_BYPASS_PCR             0x00000010      // PCR
#define E_DRVRASP_BYPASS_ESPI            0x00000020      // elementary stream priority indicator
#define E_DRVRASP_BYPASS_RAI             0x00000040      // random access indicator
#define E_DRVRASP_BYPASS_DI              0x00000080      // discontinue indicator
#define E_DRVRASP_BYPASS_ESNS            0x00000100      // elementary stream not scrambled
#define E_DRVRASP_BYPASS_ESES            0x00000200      // elementary stream even scrambled
#define E_DRVRASP_BYPASS_ESOS            0x00000400      // elementary stream odd scrambled
#define E_DRVRASP_BYPASS_PUSI            0x00000800      // payload unit start indicator
#define E_DRVRASP_BYPASS_FPR             0x00001000      // first packet recorded
#define E_DRVRASP_BYPASS_RASP_Tick       0x80000000      // rasp tick

typedef enum
{
    E_NDSRASP_CAP_FILTER_NUM = 0,                   // Get filter number
    E_NDSRASP_CAP_FILTER_PVR_NUM,                   // Get pvr filter number
    E_NDSRASP_CAP_PVR_ALIGN,                        // Get pvr buffer minimal alignment
    E_NDSRASP_CAP_RESOURCE_SIZE,                    // Get the data structure size of private resource (share resource)
    E_NDSRASP_CAP_RASP_NUM,                         // Get RASP num
    E_NDSRASP_CAP_ECMFLT_NUM,
    E_NDSRASP_CAP_EVENT_FIFO_DEPTH,
    E_NDSRASP_CAP_EVENT_NUM,
    E_NDSRASP_CAP_NULL,
} NDSRASP_Cap;

/// RASP channel state bit flags
typedef enum //_DrvTSP_FltState
{
    E_DRVRASP_FLT_STATE_FREE         = 0x00000000,                                                   ///<\n
    E_DRVRASP_FLT_STATE_ALLOC        = 0x00000001,                                                   ///<\n
    E_DRVRASP_FLT_STATE_ENABLE       = 0x00000002,                                                   ///<\n
    E_DRVRASP_FLT_STATE_OVERFLOW     = 0x00010000,                                                   //[Reserved]
    E_DRVRASP_FLT_STATE_NA           = 0xFFFFFFFF,
} DrvRASP_FltState;

/// TSP record mode
typedef enum //_DrvRASP_RecMode
{
    // Record PID
    E_DRVRASP_REC_MODE_PID  = 0x00000000,
    // Record ALL
    E_DRVRASP_REC_MODE_ALL  = 0x00000001,
} DrvRASP_RecMode;

//
typedef enum
{
    E_DRVRASP_MODE_PAYLOAD  = 0x00000000,
    // Record ECM
    E_DRVRASP_MODE_ECM  = 0x00000001,
} DrvRASP_CtrlMode;


/// TSP interface
typedef enum
{
    E_DRVRASP_IF_PLAYBACK            = 0x0, // TS interface 0
    E_DRVRASP_IF_PVR0                = 0x1, // TS interface 1, mainly for PVR
} DrvRASP_If;

typedef struct //_DrvRASP_Msg
{/*//why union? by teddy.chen
    /// Union data type of message
    union
    {
        /// FltInfo message
        ///   - Byte[0] : Section filter id
        ///   - Byte[1] : TSP id
        MS_U32                          FltInfo;
        /// PvrBufId
        ///   - Byte[0] : PVR buffer id
        MS_U32                          PvrBufId;
    };
*/
    MS_U8 u8PVREngNum;
} DrvRASP_Msg;

typedef enum //_DrvRASP_RecType
{
    STR2MIU          = 0x0,
    PAYLOAD2MIU      = 0x1,
    ECM2MIU          = 0x2,
} NDSRASP_RecType;

typedef enum //RASP Output Packet Size
{
    RASP_OUT_188 = 0,
    RASP_OUT_192
} RASP_OUTSIZE_e;

typedef enum //RASP Output Packet Size
{
    RASP_IN_188,
    RASP_IN_192
} RASP_INPUTSIZE_e;

typedef enum{
    EVENT_WATERMARK,
    TIMER_WATERMARK,
}WATERMARK_TYPE;

typedef enum
{
    WATERMARK_50MS = 0,
    WATERMARK_200MS = 1,
    WATERMARK_500MS = 2,
}TIME_WATERMARK_e;


typedef enum
{
    NDSRASP_CB_STR2RAM = 0,
    NDSRASP_CB_PAYLOAD2RAM,
    NDSRASP_CB_ECM2RAM,
    NDSRASP_CB_ECMREADY,
    NDSRASP_CB_EVENTMARK,
    NDSRASP_CB_TIMEMARK,
    NDSRASP_CB_EVENTWRITE_OVF,
    NDSRASP_CB_EVENTREAD_OVF,
}NDSRASP_Event_e;

typedef enum
{
    CMDQ_WR_LEVEL_EMPTY=0,
	CMDQ_WR_LEVEL_25_FULL,
	CMDQ_WR_LEVEL_50_FULL,
	CMDQ_WR_LEVEL_75_FULL,
}FILEIN_CMDQ_LEVEL;


typedef struct
{
    NDSRASP_Event_e  eEvent;
    MS_U16           length;
    MS_U16           index;
    MS_U32           write_ptr;
    MS_U8            *pRetBuf;
} NDSRaspCallBack_t;

typedef struct
{
    MS_U32        Event_Descriptor;
    MS_U32        Pid;
    MS_U32        PacketNum;
    MS_U32        Timer;
    MS_U32        PCR;

}NDS_RASP_EVENT;

#if defined(UFO_PUBLIC_HEADER_500_3)
typedef enum
{
    RASP_FLOW_INPUT_TS0,               ///< DMX input from TS0
    RASP_FLOW_INPUT_TS1,               ///< DMX input from TS1
    RASP_FLOW_INPUT_TS2,               ///< DMX input from TS2
    RASP_FLOW_INPUT_TS3,               ///< DMX input from TS3
    RASP_FLOW_INPUT_TS4,               ///< DMX input from TS4
    RASP_FLOW_INPUT_TS5,               ///< DMX input from TS5
    RASP_FLOW_INPUT_TS6,               ///< DMX input from TS6
    RASP_FLOW_INPUT_DEMOD0,            ///< DMX input from internal demod 0
    RASP_FLOW_INPUT_DEMOD1,            ///< DMX input from internal demod 1
    RASP_FLOW_INPUT_MEM,                 ///< DMX input from memory
} RASP_FLOW_INPUT;
#else
typedef enum
{
    RASP_FLOW_INPUT_DEMOD,               ///< DMX input from internal demod
    RASP_FLOW_INPUT_TS0,               ///< DMX input from internal demod
    RASP_FLOW_INPUT_TS1,               ///< DMX input from internal demod
    RASP_FLOW_INPUT_MEM,                 ///< DMX input from memory
} RASP_FLOW_INPUT;
#endif

typedef enum
{
    RASP_FLOW_OUTPUT_PVR,
    RASP_FLOW_OUTPUT_PLAYBACK,
} RASP_FLOW_OUTPUT;

/// TSP notification function
typedef void (*P_DrvRASP_EvtCallback)(DrvRASP_Event eEvent, DrvRASP_Msg *pMsg);
typedef void (*P_DrvRASP_EventCB)(MS_U32 RaspID, NDS_RASP_EVENT* event_ptr /*event callback struct*/, MS_U16 event_size, MS_U16 event_index);

typedef void (*P_NDSRASP_Callback)(MS_U8 RaspID, NDSRaspCallBack_t *pMsg);

//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------

// Initialization API
MS_U32 NDS_RASP_InitLibResource(void* pResMemAddr);
MS_U32 NDS_RASP_Init(void);
MS_U32 NDS_RASP_Exit(void);
MS_U32 NDS_RASP_Reset(void);
//MS_U32 NDS_RASP_SetTSIF(MS_U32 u32RASPEng, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP);
MS_U32 NDS_RASP_GetTSIFStatus(MS_U32 u32RASPEng, MS_BOOL* pbExtSyc, MS_BOOL* pbParl, MS_BOOL *pbDataSWP);
MS_U32 NDS_RASP_FileinEnable(MS_U32 u32RASPEng, MS_BOOL bEn);
MS_U32 NDS_RASP_GetFileinEnable(MS_U32 u32RASPEng, MS_BOOL* bEn);

// Capacity query
MS_U32 NDS_RASP_GetCap(NDSRASP_Cap eCap, void* pOutput);
MS_U32 NDS_RASP_GetLibVer(const MSIF_Version **ppVersion);
MS_U32 NDS_RASP_GetTimerAndPacketNum(const MSIF_Version **ppVersion);
MS_U32 NDS_RASP_GetTsPayload(const MSIF_Version **ppVersion);
MS_U32 NDS_RASP_GetEventMask(const MSIF_Version **ppVersion);

// PVR API
#if 0
MS_U32 NDS_RASP_SetBuffer(MS_U32 u32RASPEng, MS_PHYADDR u32BufStart0, MS_PHYADDR u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
#endif
MS_U32 NDS_RASP_CtrlConfig(MS_U32 u32RASPEng,DrvRASP_CtrlMode eCtrlMode,MS_BOOL bEnable);
MS_U32 NDS_RASP_PvrEngStart(MS_U32 u32RASPEng,DrvRASP_RecMode eRecMode,MS_BOOL bEnable);
MS_U32 NDS_RASP_Pause(MS_U32 u32RASPEng, MS_BOOL bPause);
MS_U32 NDS_RASP_GetWriteAddr(MS_U32 u32RASPEng, MS_PHYADDR *pu32WriteAddr);
MS_U32 NDS_RASP_SetNotify(MS_U32 u32RASPEng, DrvRASP_Event eEvents, P_DrvRASP_EvtCallback pfCallback);
MS_U32 NDS_RASP_SetPacketMode(MS_U32 u32RASPEng, NDSRASP_RecType eRecType, RASP_OUTSIZE_e eOutSize);
MS_U32 NDS_RASP_SetRecordTimeStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp);
MS_U32 NDS_RASP_GetRecordTimeStamp(MS_U32 u32RASPEng, MS_U32* u32Stamp);
MS_U32 NDS_RASP_GetCurrentPktStatus(MS_U32 u32RASPEng, MS_U32* u32PktStamp, MS_U32* u32PktNumber);
MS_U32 NDS_RASP_TimeStampSelRecordStampSrc(MS_U32 u32RASPEng, MS_BOOL bLocal);
MS_U32 NDS_RASP_AllocFlt(MS_U32 u32RASPEng, MS_U32 *pu16PidFltId);
MS_U32 NDS_RASP_FreeFlt(MS_U32 u32RASPEng, MS_U32 u32PidFltId);
MS_U32 NDS_RASP_AllocECMFlt(MS_U32 u32RASPEng,MS_U16 * pu16ECMFltId);
MS_U32 NDS_RASP_FreeECMFlt(MS_U32 u32RASPEng,MS_U16 u16ECMFltId);
MS_U32 NDS_RASP_SetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid);
MS_U32 NDS_RASP_GetPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 *pu16Pid);
MS_U32 NDS_RASP_AttachInterrupt(InterruptCb pIntCb);
MS_U32 NDS_RASP_EnableInterrupt(void);
MS_U32 NDS_RASP_CallbackSize(MS_U32 u32RASPEng, MS_U32* pu32CallbackSize, MS_BOOL bSet);
//MS_U32 NDS_RASP_TimeStampSetPlaybackStamp(MS_U32 u32Stamp);
//MS_U32 NDS_RASP_TimeStampGetPlaybackStamp(MS_U32* u32Stamp);
//MS_U32 NDS_RASP_TimeStamp(MS_BOOL bEnable);

// RASP API
MS_U32 NDS_RASP_SetEventMask(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Event);
MS_U32 NDS_RASP_SetWatermark(MS_U32 u32RASPEng, WATERMARK_TYPE WType, MS_BOOL bEnable);
MS_U32 NDS_RASP_AdvEnable(MS_U32 u32RASPEng, DrvRASP_RecMode eRecMode, MS_BOOL bEn);
MS_U32 NDS_RASP_Rec_PID(MS_U32 u32RASPEng, MS_BOOL bEnRecordPid);

MS_U32 NDS_RASP_SetPayloadMask(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Payload);
MS_U32 NDS_RASP_SetDataSwap(MS_U32 u32RASPEng, MS_BOOL bEn);

MS_U32 NDS_PROC_RASP_PVR_SizeMet(MS_U32 u32RASPEng);
MS_U32 NDS_RASP_CallbackIntCheck(MS_U32 u32RASPEng, MS_BOOL* bInterrupted);
MS_U32 NDS_RASP_CallbackIntClr(MS_U32 u32RASPEng);

NDS_Result NDS_RASP_Init2(NDS_RASP_Param *Param);
NDS_Result NDS_RASP_Exit2(void);

MS_U32 NDS_RASP_SetFileIn_Config(MS_U32 RaspEng, MS_U32 StartAddr, MS_U32 FileInSize);
MS_U32 NDS_RASP_FileIn_Start(MS_U32 RaspEng);
MS_U32 NDS_RASP_192FileIn_Start(MS_U32 RaspEng);
MS_U32 NDS_RASP_SetFileIn_Timer(MS_U32 RaspEng, MS_U16 u16Timer);
MS_U32 NDS_RASP_SetFileIn_PktSize(MS_U32 RaspEng, MS_U16 PktSize);
MS_BOOL NDS_RASP_IsFileIn_Done(MS_U32 RaspEng);
MS_U32 NDS_RASP_FileIn_Flush(MS_U32 RaspEng);
MS_U32 NDS_RASP_SetBufInfo(MS_U32 u32RASPEng, NDSRASP_RecType eRecType, MS_PHYADDR u32BufStart0, MS_PHYADDR u32BufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
MS_U32 NDS_RASP_FileinInit(MS_U32 u32RASPEng);

MS_U32 NDS_RASP_FileIn_BypassTimeStamp(MS_U32 u32RASPEng,MS_BOOL bypass);
MS_U32 NDS_RASP_FileIn_SetPlaybackTimeStamp(MS_U32 u32RASPEng,MS_U32 u32Stamp);
MS_U32 NDS_RASP_FileIn_GetPlaybackTimeStamp(MS_U32 u32RASPEng);
MS_BOOL NDS_RASP_FileIn_IsCMDQ_Full(MS_U32 RaspEng);
MS_BOOL NDS_RASP_FileIn_IsCMDQ_Empty(MS_U32 RaspEng);
MS_BOOL NDS_RASP_FileIn_GetCmdQueueLevel(MS_U32 RaspEng,FILEIN_CMDQ_LEVEL * peCMDQLvl);
MS_BOOL NDS_RASP_FileIn_GetEmptyNum(MS_U32 RaspEng,MS_U8 * peCMDQCnt);
MS_U32 NDS_RASP_FileIn_Timer(MS_U32 RaspEng,MS_BOOL bEnFileInTimer,MS_U16 u16Timer);
#ifdef UFO_PUBLIC_HEADER_500_3
MS_BOOL NDS_RASP_FileIn_Init_TimeStamp(MS_U32 RaspEng,MS_U32 u32SetPacketTimeStamp);
#else
MS_BOOL NDS_RASP_FileIn_Init_TimeStamp(MS_U32 RaspEng,MS_BOOL u32SetPacketTimeStamp);
#endif
MS_BOOL NDS_RASP_Reset_EventPktCounter(MS_U32 RaspEng);
MS_BOOL NDS_RASP_Reset_EventPktTimer(MS_U32 RaspEng);
MS_BOOL NDS_RASP_GetEventDescriptor(MS_U32 RaspEng,NDS_RASP_EVENT * pEventDesc,MS_U32 u32ArraySize);
MS_U16 NDS_RASP_GetEventNumber(MS_U32 RaspEng);

MS_BOOL NDS_RASP_FlowSet(MS_U32 rasp_eng, RASP_FLOW_INPUT eSource, RASP_FLOW_OUTPUT eDest, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP, RASP_INPUTSIZE_e eMode );
MS_U32 NDS_RASP_RaspEngStart(MS_U32 u32RASPEng, DrvRASP_CtrlMode eCtrlMode, MS_BOOL bEnable);
MS_BOOL NDS_RASP_Livein_Config(MS_U32 rasp_eng);

void NDS_RASP_SetDbgLevel(MS_U32 level);
MS_U32 NDS_RASP_SetPayloadTimeStamp(MS_U32 u32RASPEng , MS_U32 u32TimeStamp);
MS_U32 NDS_RASP_GetPayloadTimeStamp(MS_U32 u32RASPEng , MS_U32 *pu32TimeStamp);
MS_U32 NDS_RASP_GetPayloadWriteAddr(MS_U32 u32RASPEng, MS_PHYADDR *pu32WriteAddr);

MS_U32 NDS_RASP_SetECMPid(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid);
MS_U32 NDS_RASP_SetCallBack(MS_U32 u32RASPEng, P_NDSRASP_Callback pfCallback);
MS_U32 NDS_RASP_Set_EventNotify(MS_U32 u32RASPEng,P_DrvRASP_EventCB CallBackFun);
MS_BOOL NDS_RASP_SetEvent_Threshold(MS_U32 u32RASPEng,MS_U8 u8Threshold  /*!!! Maximum value is 31 !!!*/);
MS_BOOL NDS_RASP_SetTime_Timeout(MS_U32 u32RASPEng,TIME_WATERMARK_e timeout);

MS_U32 NDS_RASP_SetECMTimeStamp(MS_U32 u32RASPEng,MS_U32 u32Stamp);
MS_U32 NDS_RASP_GetECMTimeStamp(MS_U32 u32RASPEng,MS_U32 * u32Stamp);

MS_BOOL NDS_RASP_SetEventCounter(MS_U32 RaspEng,MS_U32 ValueInitial);
MS_BOOL NDS_RASP_SetEventTimer(MS_U32 RaspEng,MS_U32 ValueInitial);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif
#endif // _DRVRASP_H_
