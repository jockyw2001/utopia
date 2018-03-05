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
/// file   drvOTV.h
/// @brief  OTV Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVOTV_H_
#define _DRVOTV_H_

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------
        
#define DRVOTV_PID_NULL             0x1FFF                                                 // Transport stream null PID

#define MSIF_OTV_LIB_CODE           {'O','T','V'}                                          // Lib code
#define MSIF_OTV_LIBVER             {'0','2'}                                              // LIB version
#define MSIF_OTV_BUILDNUM           {'0','0'}                                              // Build Number
#define MSIF_OTV_CHANGELIST         {'0','0','5','1','1','4','7','0'}                      // P4 ChangeList Number

#define OTV_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_OTV_LIB_CODE,                  /* IP__                                             */  \
    MSIF_OTV_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_OTV_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_OTV_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------

/// OTV DDI return value
/// @name TSP_Result
/// @ref TSP_Result
/// return value
/// @{

typedef enum
{
    DRVOTV_FAIL                   = 0,
    DRVOTV_OK,
    DRVOTV_INVALID_PARAM,
    DRVOTV_FUNC_ERROR,
} DrvOTV_Result;


typedef enum
{
    E_DRVOTV_TIME_WATERMARK_50MS  = 0,
    E_DRVOTV_TIME_WATERMARK_200MS = 1,
    E_DRVOTV_TIME_WATERMARK_500MS = 2,

}DrvOTV_Time_Watermark;


// DRV OTV HW Interrupt Status
#define DRVOTV_HWINT_TYPE                MS_U32

#define E_DRVOTV_HWINT_MASK              0x0000FFFF
#define E_DRVOTV_HWINT_EVENT             0x00000001
#define E_DRVOTV_HWINT_TIMER             0x00000002
#define E_DRVOTV_HWINT_TASKEND           0x00008000    // This is not OTV HWINT, the bit is used to inform OTV ISR task end   
#define E_DRVOTV_HWINT_MASK_SHIFT        0


#define E_DRVOTV_EVENT_NUM_MASK          0x00FF0000
#define E_DRVOTV_EVENT_NUM_MASK_SHIFT    16

#define E_DRVOTV_ENG_MASK                0xFF000000
#define E_DRVOTV_ENG_MASK_SHIFT          24


// DRV OTV Event Type
#define DRVOTV_EVENT_TYPE                MS_U32

// Payload/Event Mask flag
#define E_DRVOTV_EVENT_MASK            0xffffffff
#define E_DRVOTV_EVENT_AFE             0x00000001      // Adaptation Field Extension flag
#define E_DRVOTV_EVENT_TPD             0x00000002      // Transport Private Data flag
#define E_DRVOTV_EVENT_SP              0x00000004      // Splicing Point flag
#define E_DRVOTV_EVENT_OPCR            0x00000008      // OPCR flag
#define E_DRVOTV_EVENT_PCR             0x00000010      // PCR flag
#define E_DRVOTV_EVENT_ESPI            0x00000020      // Elementary Stream Priority Indicator
#define E_DRVOTV_EVENT_RAI             0x00000040      // Random Access Indicator
#define E_DRVOTV_EVENT_DI              0x00000080      // Discontinuity Indicator
#define E_DRVOTV_EVENT_CESOS           0x00000100      // Change of Elementary Stream to Odd  Scrambled
#define E_DRVOTV_EVENT_CESES           0x00000200      // Change of Elementary Stream to Even Scrambled
#define E_DRVOTV_EVENT_CESNS           0x00000400      // Change of Elementary Stream to Not  Scrambled
#define E_DRVOTV_EVENT_PUSI            0x00000800      // Payload Unit Start Indicator
#define E_DRVOTV_EVENT_FPR             0x00001000      // First Packet Recorded


typedef enum
{
    E_DRVOTV_CAP_FILTER_NUM = 0,                   // Get filter number
    E_DRVOTV_CAP_FILTER_PVR_NUM,                   // Get pvr filter number
    E_DRVOTV_CAP_PVR_ALIGN,                        // Get pvr buffer minimal alignment
    E_DRVOTV_CAP_RESOURCE_SIZE,                    // Get the data structure size of private resource (share resource)
    E_DRVOTV_CAP_OTV_NUM,                          // Get OTV num

    E_DRVOTV_CAP_NULL,
} DrvOTV_Cap;

/// OTV channel state bit flags
typedef enum //_DrvTSP_FltState
{
    E_DRVOTV_FLT_STATE_FREE         = 0x00000000,                                                   ///<\n
    E_DRVOTV_FLT_STATE_ALLOC        = 0x00000001,                                                   ///<\n
    E_DRVOTV_FLT_STATE_ENABLE       = 0x00000002,                                                   ///<\n
    E_DRVOTV_FLT_STATE_OVERFLOW     = 0x00010000,                                                   //[Reserved]
    E_DRVOTV_FLT_STATE_NA           = 0xFFFFFFFF,
} DrvOTV_FltState;

typedef enum
{
    E_DRV_OTV_DBG_LEVEL_NONE,   // no debug message shown
    E_DRV_OTV_DBG_LEVEL_ERR,    // only shows error message that can't be recover
    E_DRV_OTV_DBG_LEVEL_WARN,   // error case can be recover, like retry
    E_DRV_OTV_DBG_LEVEL_EVENT,  // event that is okay but better known, ex: timestamp ring, file circular, etc.
    E_DRV_OTV_DBG_LEVEL_INFO,   // information for internal parameter
    E_DRV_OTV_DBG_LEVEL_FUNC,   // Function trace and input parameter trace
    E_DRV_OTV_DBG_LEVEL_TRACE,  // debug trace
} DRV_OTV_DBGMSG_LEVEL;

typedef enum
{
    E_DRV_OTV_DBG_MODEL_NONE,   // @temporarily , need to refine
    E_DRV_OTV_DBG_MODEL_ALL,
} DRV_OTV_DBGMSG_MODEL;

typedef struct //_DrvOTV_Msg
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
} DrvOTV_Msg;


typedef struct
{
    MS_U32        Event_Descriptor;
    MS_U16        Pid;
    MS_U32        PacketNum;
    MS_U32        Timer;
    MS_U32        PCR;

}DrvOTV_EventArray;


/// TSP notification function
typedef void (*DrvOTV_NotifyCb)(MS_U32 u32OTV_Eng, DRVOTV_HWINT_TYPE OTV_HWINT, MS_U32 u32EventNum, DrvOTV_EventArray* pstEventArray /*event callback struct*/, void * );


//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------

// Initialization API
DrvOTV_Result MDrv_OTV_InitLibResource(void* pResMemAddr);
DrvOTV_Result MDrv_OTV_Init(void);
DrvOTV_Result MDrv_OTV_Exit(void);
DrvOTV_Result MDrv_OTV_ISR(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE);

//DrvOTV_Result MDrv_OTV_Reset(void);
//DrvOTV_Result MDrv_OTV_GetCap(DrvOTV_Cap eCap, void* pOutput);

// OTV API
DrvOTV_Result MDrv_OTV_AllocFlt(MS_U32 u32OTVEng, MS_U32 *pu32FltId);
DrvOTV_Result MDrv_OTV_ReleaseFlt(MS_U32 u32OTVEng, MS_U32 u32FltId);
DrvOTV_Result MDrv_OTV_SetFltPid(MS_U32 u32OTVEng, MS_U32 u32Fltid, MS_U16 u16Pid);
DrvOTV_Result MDrv_OTV_GetFltPid(MS_U32 u32OTVEng, MS_U32 u32Fltid, MS_U16 *pu16Pid);
DrvOTV_Result MDrv_OTV_SetFltEvent(MS_U32 u32OTVEng, MS_U32 u32FltId, DRVOTV_EVENT_TYPE  eEvent);
DrvOTV_Result MDrv_OTV_GetFltEvent(MS_U32 u32OTVEng, MS_U32 u32FltId, DRVOTV_EVENT_TYPE* pEvent);
DrvOTV_Result MDrv_OTV_GetCurPktStatus(MS_U32 u32OTVEng, MS_U32* u32PktStamp, MS_U32* u32PktNumber);
DrvOTV_Result MDrv_OTV_SetNotifyCb(MS_U32 u32OTVEng, DrvOTV_NotifyCb pfCallback);
DrvOTV_Result MDrv_OTV_HWINT_Enable(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE, MS_BOOL bEnable);
DrvOTV_Result MDrv_OTV_HWINT_Get(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE, MS_BOOL* bInterrupted);
DrvOTV_Result MDrv_OTV_HWINT_Clear(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE);
DrvOTV_Result MDrv_OTV_HWINT_SetEvent_Threshold(MS_U32 u32OTVEng, MS_U32 u32Threshold /*Maximum value is 31*/);
DrvOTV_Result MDrv_OTV_HWINT_SetTimer_Timeout(MS_U32 u32OTVEng, DrvOTV_Time_Watermark Timeout);
DrvOTV_Result MDrv_OTV_SetDbgLevel(DRV_OTV_DBGMSG_LEVEL level, DRV_OTV_DBGMSG_MODEL model);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif // _DRVOTV_H_
