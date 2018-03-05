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
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvTSP.c
/// @brief  Transport Stream Processer (TSP) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/string.h>
#else
#include <string.h>
#endif

// Common Definition
#include "MsVersion.h"
#include "drvMMIO.h"
#include "ULog.h"
#include "drvSYS.h"

// Internal Definition
#include "asmCPU.h"
#include "osalTSP.h"
#include "regTSP.h"
#include "halTSP.h"
#include "drvTSP.h"

#if DMX_DEBUG
#include "../../api/dmx/Internal_DMX_debug.h"
#endif

#ifdef MSOS_TYPE_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> // O_RDWR
#include "mdrv_firmware_io.h"
#endif

#ifdef SECURE_PVR_ENABLE
#include "halCHIP.h"
#include "../../drv/mbx/apiMBX_St.h"
#include "../../drv/mbx/apiMBX.h"
#include "../drv/sys/tee_client_api.h"
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
static MS_U8 u8TSPFirmware[] = {
    #include "fwTSP.dat"
};
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define _TSP_SetEvent(eventid, flag)                        OSAL_TSP_EventSet(eventid, (flag))
#define _TSP_GetEvent(eventid, events, flag, opt, timeout)  OSAL_TSP_EventGet(eventid, (flag), &(events), (opt), (timeout))

#define _TSP_SetSecRdyId(fltid)                             OSAL_TSP_EventSet(_s32TspSecRdyId[fltid>>5], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetSecRdyId(fltid, events, opt)                OSAL_TSP_EventGet(_s32TspSecRdyId[fltid>>5], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeSecRdy(idx, mask,events, opt)           OSAL_TSP_EventGet(_s32TspSecRdyId[idx], mask, (events), (opt), 0)

#define _TSP_SetSecOvfId(fltid)                             OSAL_TSP_EventSet(_s32TspSecOvfId[fltid>>5], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetSecOvfId(fltid, events, opt)                OSAL_TSP_EventGet(_s32TspSecOvfId[fltid>>5], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeSecOvf(idx, mask,events, opt)           OSAL_TSP_EventGet(_s32TspSecOvfId[idx], mask, (events), (opt), 0)

#define _TSP_SetSecCrcId(fltid)                             OSAL_TSP_EventSet(_s32TspSecCrcId[fltid>>5], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetSecCrcId(fltid, events, opt)                OSAL_TSP_EventGet(_s32TspSecCrcId[fltid>>5], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeSecCrc(idx, mask,events, opt)           OSAL_TSP_EventGet(_s32TspSecCrcId[idx], mask, (events), (opt), 0)

#define _TSP_SetFltDisableId(fltid)                         OSAL_TSP_EventSet(_s32TspFltDisableId[fltid>>5], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetFltDisableId(fltid, events, opt)            OSAL_TSP_EventGet(_s32TspFltDisableId[fltid>>5], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeFltDisable(idx, mask,events, opt)       OSAL_TSP_EventGet(_s32TspFltDisableId[idx], mask, (events), (opt), 0)

#if 0
#define _TSP_FLAG_FREE_TIMEOUT                              4
#define _TSP_SetEventFree0Id(fltid)                         OSAL_TSP_EventSet(_s32EventFree0[fltid>>5], (1<<(fltid&0x1F)))
#define _TSP_GetEventFree0Id(fltid, events, opt)            OSAL_TSP_EventGet(_s32EventFree0[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define _TSP_GetModeEventFree0(idx, mask,events, opt)       OSAL_TSP_EventGet(_s32EventFree0[idx], mask, (events), (opt), 0)
#define _TSP_FLAG_SetDone(fltid)                            OSAL_TSP_EventSet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)))
#define _TSP_FLAG_GetDone(fltid, events, opt)               OSAL_TSP_EventGet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), _TSP_FLAG_FREE_TIMEOUT)
#endif

#if 0
#define _TSP_SetEventFreeDefer0Id(fltid)                    OSAL_TSP_EventSet(_s32EventFreeDefer0[fltid>>5], (1<<(fltid&0x1F)))
#define _TSP_GetEventFreeDefer0Id(fltid, events, opt)       OSAL_TSP_EventGet(_s32EventFreeDefer0[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define _TSP_GetEventFreeDefer0(idx, mask,events, opt)      OSAL_TSP_EventGet(_s32EventFreeDefer0[idx], mask, (events), (opt), 0)
#endif

// for mutiple process using, record which filter is enabled by this process
#define _TSP_SetSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5)] |= (1UL << (fltid&0x1FUL)))
#define _TSP_RemoveSecNotifyId(fltid)                       (_u32SecNotifyId[(fltid>>5)] &= ~(1UL << (fltid&0x1FUL)))
#define _TSP_ChkSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5)] & (1UL << (fltid&0x1FUL)))

// for mutiple process using, record which pvr engine is enabled by this process
#ifndef NO_PVR
#define _TSP_SetPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5)] |= (1UL << (fltid&0x1FUL)))
#define _TSP_RemovePvrNotifyId(fltid)                       (_u32PvrNotifyId[(fltid>>5)] &= ~(1UL << (fltid&0x1FUL)))
#define _TSP_ChkPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5)] & (1UL << (fltid&0x1FUL)))
#endif

// for mutiple process using, record which hw pcr is enabled by this process
#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
#define _TSP_SetHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5)] |= (1UL << (fltid&0x1FUL)))
#define _TSP_RemoveHwPcrNotifyId(fltid)                     (_u32HwPcrNotifyId[(fltid>>5)] &= ~(1UL << (fltid&0x1FUL)))
#define _TSP_ChkHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5)] & (1UL << (fltid&0x1FUL)))
#endif

#define _TSP_MUTEX_TIMEOUT                                  10000UL //OSAL_TSP_MUTEX_TIMEOUT

// For kernel STR callback function, should not use mutex
#define _TSP_ENTRY()                                        do{ \
                                                                while (!OSAL_TSP_MutexObtain(_ptsp_res->_s32TspMutexId, _TSP_MUTEX_TIMEOUT))      \
                                                                {                                                                       \
                                                                    _ptsp_res->_u32LastErr = DRVTSP_FUNC_ERROR;                                    \
                                                                    printf("TSP FATAL ERR: [%s][%d] TSP mutex get error\n", __FUNCTION__, __LINE__);   \
                                                                    MsOS_DelayTask(1);  \
                                                                }                                                                       \
                                                                while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IndMutexId, _TSP_MUTEX_TIMEOUT))    \
                                                                {      \
                                                                    _ptsp_res->_u32LastErr = DRVTSP_FUNC_ERROR;        \
                                                                    printf("TSP FATAL ERR: [%s][%d] IND mutex get error\n", __FUNCTION__, __LINE__);   \
                                                                    MsOS_DelayTask(1);  \
                                                                }  \
                                                            }while(0);

#define _TSP_RETURN(_ret)                                   do { \
                                                                if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IndMutexId))  \
                                                                {  \
                                                                    printf("TSP FATAL ERR: [%s][%d] IND mutex free error\n", __FUNCTION__, __LINE__);   \
                                                                }  \
                                                                if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32TspMutexId))   \
                                                                {  \
                                                                    printf("TSP FATAL ERR: [%s][%d] TSP mutex free error\n", __FUNCTION__, __LINE__);   \
                                                                }  \
                                                                if (_ret != DRVTSP_OK) { _ptsp_res->_u32LastErr = _ret; }                          \
                                                                return _ret;                                                         \
                                                            }while(0);   // Should be optimized by compiler

#define _TSP_IND_LOCK()                                     while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IndMutexId, _TSP_MUTEX_TIMEOUT))  \
                                                            {   \
                                                                printf("TSP FATAL ERR: [%s][%d] IND mutex get error\n", __FUNCTION__, __LINE__);   \
                                                                MsOS_DelayTask(1); \
                                                            }

#define _TSP_IND_UNLOCK()                                   if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IndMutexId)) { printf("TSP FATAL ERR: [%s][%d] IND mutex free error\n", __FUNCTION__, __LINE__); }

#define _TSP_INT_LOCK()                                     while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IntRegMutexId, _TSP_MUTEX_TIMEOUT)) \
                                                            {   \
                                                                printf("TSP FATAL ERR: [%s][%d] INT mutex get error\n", __FUNCTION__, __LINE__);   \
                                                                MsOS_DelayTask(1); \
                                                            }

#define _TSP_INT_UNLOCK()                                   if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IntRegMutexId)) { printf("TSP FATAL ERR: [%s][%d] INT mutex free error\n", __FUNCTION__, __LINE__); }

#define _TSP_EXT_LOCK()                                     while((_s32ExtMutexId != -1) && (!OSAL_TSP_MutexObtain(_s32ExtMutexId, _TSP_MUTEX_TIMEOUT)))  \
                                                            {   \
                                                                printf("TSP FATAL ERR: [%s][%d] EXT mutex get error %x %x %d\n", __FUNCTION__, __LINE__, (int)_s32ExtMutexId, (int)_s32TspTaskId, (int)_db_flag);   \
                                                                MsOS_DelayTask(3);  \
                                                            }

#define _TSP_EXT_UNLOCK()                                   if((_s32ExtMutexId != -1) && (!OSAL_TSP_MutexRelease(_s32ExtMutexId))){ printf("TSP FATAL ERR: [%s][%d] EXT mutex free error %x %x %d\n", __FUNCTION__, __LINE__, (int)_s32ExtMutexId, (int)_s32TspTaskId, (int)_db_flag); }

// For kernel STR callback function, should not use mutex
#ifdef MSOS_TYPE_LINUX_KERNEL

#define _TSP_STR_ENTRY()                                    if(_ptsp_res->_bKerStrSuspend == FALSE){ \
                                                                while (!OSAL_TSP_MutexObtain(_ptsp_res->_s32TspMutexId, _TSP_MUTEX_TIMEOUT))      \
                                                                {                                                                       \
                                                                    if(_u32TSPDGBLevel >= E_DRVTSP_DBG_L1) { printf("[%s][%d] TSP mutex get error\n", __FUNCTION__, __LINE__); }  \
                                                                _ptsp_res->_u32LastErr = DRVTSP_FUNC_ERROR;              \
                                                                    MsOS_DelayTask(3); \
                                                                }                                                                       \
                                                                while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IndMutexId, _TSP_MUTEX_TIMEOUT))    \
                                                                {      \
                                                                    if(_u32TSPDGBLevel >= E_DRVTSP_DBG_L1) { printf("[%s][%d] IND mutex get error\n", __FUNCTION__, __LINE__); }  \
                                                                    _ptsp_res->_u32LastErr = DRVTSP_FUNC_ERROR;        \
                                                                    MsOS_DelayTask(3);  \
                                                                }  \
                                                            }

#define _TSP_STR_RETURN(_ret)                               do{   \
                                                                if(_ptsp_res->_bKerStrSuspend == TRUE){ return _ret;    } \
                                                                if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IndMutexId))  \
                                                                {  \
                                                                    if(_u32TSPDGBLevel >= E_DRVTSP_DBG_L1) { printf("[%s][%d] IND mutex free error\n", __FUNCTION__, __LINE__); }   \
                                                                }  \
                                                                if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32TspMutexId))   \
                                                                {  \
                                                                    if(_u32TSPDGBLevel >= E_DRVTSP_DBG_L1) { printf("[%s][%d] TSP mutex free error\n", __FUNCTION__, __LINE__); }  \
                                                                }  \
                                                                if (_ret != DRVTSP_OK) { _ptsp_res->_u32LastErr = _ret; }                          \
                                                                return _ret;                                                         \
                                                            }while(0);   // Should be optimized by compiler

#else  //else MSOS_TYPE_LINUX_KERNEL

#define _TSP_STR_ENTRY                                  _TSP_ENTRY
#define _TSP_STR_RETURN(_ret)                           _TSP_RETURN(_ret)

#endif //MSOS_TYPE_LINUX_KERNEL


#define TSP_ASSERT(_bool, _f)                           if(!(_bool)){   \
                                                            if (_u32TSPDGBLevel == E_DRVTSP_DBG_L1) { (_f); return DRVTSP_FAIL;} \
                                                            else if (_u32TSPDGBLevel == E_DRVTSP_DBG_L2) { (_f); MsOS_DelayTask(MSOS_WAIT_FOREVER); return DRVTSP_FAIL;} \
                                                            else {return DRVTSP_FAIL;}             \
                                                        }

#define TSP_ASSERT2(_bool, _f)                          if(!(_bool)){   \
                                                            if (_u32TSPDGBLevel == E_DRVTSP_DBG_L1) { (_f); _TSP_STR_RETURN(DRVTSP_FAIL);} \
                                                            else if (_u32TSPDGBLevel == E_DRVTSP_DBG_L2) { (_f); MsOS_DelayTask(MSOS_WAIT_FOREVER); _TSP_STR_RETURN(DRVTSP_FAIL);} \
                                                            else {_TSP_STR_RETURN(DRVTSP_FAIL);}                                                                                                                  \
                                                        }while(0);

#define TSP_DBG(_f, _a...)                              { if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L1)||(_u32TSPDGBLevel == E_DRVTSP_DBG_L2)) { OSAL_TSP_Print("[TSP_DBG]"); OSAL_TSP_Print(_f, ##_a); } }


#define DRVTSP_MAP_NULL                                 0xFFFFFFFFUL

#define DRVTSP_MAX_PROCESS_NUM                          (10)

#ifdef MSOS_TYPE_LINUX_KERNEL
#define current                                         get_current()
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//----------------
//For Tee
//----------------
#ifdef SECURE_PVR_ENABLE
typedef enum
{
    E_TSP_TEE_TO_REE_MBX_MSG_NULL,
    E_TSP_TEE_TO_REE_MBX_ACK_MSG_INVALID,
    E_TSP_TEE_TO_REE_MBX_ACK_MSG_NO_TEE,
    E_TSP_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS,
    E_TSP_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL
} TSP_TEE_TO_REE_MBX_ACK_TYPE;
#endif

//--------------------------------------------------------
// Total 128 PID filters
// First 0~63 for section
//--------------------------------------------------------
typedef struct _DrvTSP_State
{
    DrvTSP_SW_Status                TSPStatus;
    DrvTSP_FltState                 FltState[TSP_PIDFLT_NUM_ALL];
    DrvTSP_FltType                  FltType[TSP_PIDFLT_NUM_ALL];
    MS_U32                          FltSource[TSP_PIDFLT_NUM_ALL];
    MS_U32                          FltMap[TSP_PIDFLT_NUM_ALL];
    DrvTSP_FltState                 SecFltState[TSP_SECFLT_NUM];
    DrvTSP_Event                    SecFltEvtNotify[TSP_SECFLT_NUM];
    P_DrvTSP_EvtCallback            SecFltCallback[TSP_SECFLT_NUM];
    DrvTSP_FltState                 SecBufState[TSP_SECFLT_NUM];
    DrvTSP_FltMode                  SecFltMode[TSP_SECFLT_NUM];

    MS_S32                          s32TspEvtId[TSP_SECFLT_NUM];

#ifdef MERGE_STR_SUPPORT
    MS_U32                          FltSrcId[TSP_PIDFLT_NUM_ALL];
#endif

#ifdef DUP_PIDFLT
    MS_U32                          DupFlt[TSP_PIDFLT_NUM];
#endif
} DrvTSP_State;

typedef struct
{
    MS_PHY                    phyBufAddr;
    MS_U32                    u32Size;
    MS_PHY                    phyReadPos;
    MS_PHY                    phyWritePos;
    MS_BOOL                   bEnable;
} DrvTSP_FW_DBG_INFO;

typedef struct _DrvTSP_DscmbFlt_State
{
    MS_U32 u32Src;
    MS_U32 u32Start;
    MS_U32 u32End;
    MS_U32 u32ShareStart;
    MS_U32 u32ShareEnd;
}DrvTSP_DscmbFlt_State;

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
typedef struct _DrvTSP_HWPcrParams
{
    MS_U8                   _u8ResetPcr[TSP_PCRFLT_NUM];
    MS_U32                  _u32PrevStcBase[TSP_PCRFLT_NUM];
    MS_U32                  _u32PrevPllControl[TSP_PCRFLT_NUM];
}DrvTSP_HWPcrParams;
#endif

//UFO_DMX_RES_ALLOC_EN - FOR STC
typedef struct _DrvTSP_Stc_State
{
    MS_U32  u32PcrFltId;
    MS_U32  u32SrcType;
    MS_BOOL bUsed;
}DrvTSP_Stc_State;

#ifdef DMX_UTOPIA_2K
typedef struct _DrvTSP_KMode_ProcEvtIdTbl
{
    #ifdef MSOS_TYPE_LINUX_KERNEL
    pid_t                   tgid;
    #endif

    MS_S32                  s32TspEvtId;
}DrvTSP_KMode_ProcEvtIdTbl;
#endif

typedef struct _TSP_RESOURCE_PRIVATE
{
    MS_S32                  _s32TspMutexId;
    MS_S32                  _s32IndMutexId;
    MS_S32                  _s32IntRegMutexId;

    MS_U32                  _u32LastErr;
    MS_BOOL                 _bKerStrSuspend;
    MS_U32                  _u32ProcessNum;
    DrvTSP_State            _TspState[TSP_ENGINE_NUM];

    MS_BOOL                 _bSWInitReady;
    MS_PHY                  _phyFwAddr;
    MS_U32                  _u32FwSize;
    MS_U32                  _u32DbgIntrCount;
    DrvTSP_If_Set           _ifset_save;

    DrvTSP_Event            _PvrEvtNotify[TSP_PVR_IF_NUM];
    P_DrvTSP_EvtCallback    _PvrCallback[TSP_PVR_IF_NUM];
    MS_S32                  _s32PvrEvtId[TSP_PVR_IF_NUM];

    #if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    DrvTSP_HWPcrParams      TSPHwPcrParams;
    #endif

    DrvTSP_DscmbFlt_State   _DscmbFlt_State[TSP_CA_ENGINE_NUM];

    MS_PHY                  _phyTspFWDramBuf;
    MS_U32                  _u32TspFwDramBufSize;
    DrvTSP_Stc_State        _TspStcState[TSP_STC_NUM];
    MS_BOOL                 _bStcAllocDisable;
    MS_U32                  _u32StcAdjustUnit;

#ifdef SECURE_PVR_ENABLE
    SYS_TEEINFO             _teemode;
    DrvTSP_SecureBuf        _stSecBuf;
#endif

}TSP_RESOURCE_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_tsp_version = {
    .DDI = { TSP_DRV_VERSION, },
};

#ifndef TSP_UTOPIA_20
static TSP_RESOURCE_PRIVATE         _tsp_res = {
    ._s32TspMutexId                     = -1,
    ._s32IndMutexId                     = -1,
    ._s32IntRegMutexId                  = -1,

    ._u32LastErr                        = DRVTSP_OK,
    ._bKerStrSuspend                    = FALSE,
    ._u32ProcessNum                     = 0,
    ._bSWInitReady                      = FALSE,
    ._phyFwAddr                         = 0,
    ._u32FwSize                         = 0,
    ._u32DbgIntrCount                   = 0,
    ._ifset_save                        = {E_DRVTSP_PAD_EXT_INPUT0, FALSE, TRUE, TRUE},

    ._PvrEvtNotify                      = {[0 ... (TSP_PVR_IF_NUM-1)] = E_DRVTSP_EVENT_DATA_INIT},
    ._PvrCallback                       = {[0 ... (TSP_PVR_IF_NUM-1)] = 0},
    ._s32PvrEvtId                       = {[0 ... (TSP_PVR_IF_NUM-1)] = -1},

    #if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    .TSPHwPcrParams = {
        ._u8ResetPcr                    = {[0 ... (TSP_PCRFLT_NUM-1)] = 3},
        ._u32PrevStcBase                = {[0 ... (TSP_PCRFLT_NUM-1)] = 0},
        ._u32PrevPllControl             = {[0 ... (TSP_PCRFLT_NUM-1)] = 0},
    },
    #endif

    ._phyTspFWDramBuf                   = 0,
    ._u32TspFwDramBufSize               = 0,
    ._sTspStcState                      = {[0 ... (TSP_STC_NUM-1)] = {0xFFFFFFFF, FALSE}},
    ._bStcAllocDisable                  = FALSE,
    _u32StcAdjustUnit                  = 100,


    #ifdef SECURE_PVR_ENABLE
    ._teemode                           = {.OsType = SYS_TEEINFO_OSTYPE_NOTEE},
    ._stSecBuf                          = {0, 0, 0, 0},
    #endif
};

static TSP_RESOURCE_PRIVATE*        _ptsp_res = &_tsp_res;
#else
static TSP_RESOURCE_PRIVATE*        _ptsp_res = NULL;
#endif

static MS_U8                        _db_flag = 0;
static MS_S32                       _s32ExtMutexId = -1; //For exiting ISR process task
static MS_S32                       _s32TspEventId = -1;
static MS_S32                       _s32TspSecRdyId[(TSP_SECFLT_NUM+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM+31UL)>>5UL)-1UL)] = -1};
static MS_S32                       _s32TspSecOvfId[(TSP_SECFLT_NUM+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM+31UL)>>5UL)-1UL)] = -1};
static MS_S32                       _s32TspSecCrcId[(TSP_SECFLT_NUM+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM+31UL)>>5UL)-1UL)] = -1};
static MS_S32                       _s32TspFltDisableId[(TSP_SECFLT_NUM+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM+31UL)>>5)-1UL)] = -1};
//static MS_S32                     _s32EventFree0[(TSP_PIDFLT_NUM_ALL+31)>>5] = {[0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1};
//static MS_S32                     _s32DoneFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1};
//static MS_S32                     _s32EventFreeDefer0[(TSP_PIDFLT_NUM_ALL+31)>>5] = {[0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1};
static MS_U32                       _u32SecNotifyId[(TSP_SECFLT_NUM+31UL)>>5UL] = {0UL}; //for ISR processing

#ifndef NO_PVR
static MS_U32                       _u32PvrNotifyId[(TSP_PVR_IF_NUM+31UL)>>5UL] = {0UL};
#endif

static MS_S32                       _s32TspTaskId = -1;
MS_U32                              _u32IntNum = E_INT_IRQ_TSP2HK;
MS_U32                              _u32TaskLine = 0;
static DrvTSP_DbgLevel              _u32TSPDGBLevel = E_DRVTSP_DBG_L1;

static MS_BOOL                      _bBankInit = FALSE;

#ifdef MSOS_TYPE_LINUX_KERNEL
wait_queue_head_t                   tsp_wq_head[TSP_SECFLT_NUM];
#endif // MSOS_TYPE_LINUX_KERNEL

#ifdef DMX_UTOPIA_2K
static DrvTSP_KMode_ProcEvtIdTbl    _stKModePrcEvtTbl[DRVTSP_MAX_PROCESS_NUM];
static MS_U32                       _u32KPrcEvtTblUseFlag = 0;
#endif

static DrvTSP_FW_DBG_INFO           _FwDbgInfo;
static MS_U32                       _u32LibMode = DRVTSP_CMD_LIB_MODE_USER;

//-----      For optee   ------------------
#ifdef SECURE_PVR_ENABLE
#define MSTAR_UPA_DMXTA_UUID {0x7a7c8b61, 0x5d74, 0x4b6c, {0xb2, 0xa9, 0x92, 0xe4, 0x9c, 0xe8, 0x66, 0xbd} }
#define SYS_TEEC_OPERATION_INITIALIZER { 0 }

static const char optee_dmx[] = "opteearmtz00";
char *_device = (char *)optee_dmx;
TEEC_Context mstar_teec_ctx;
TEEC_Session session = { 0 };
TEEC_UUID uuid = MSTAR_UPA_DMXTA_UUID;
TEEC_Operation op = SYS_TEEC_OPERATION_INITIALIZER;
#endif //SECURE_PVR_ENABLE

static void _DumpFwMsg(void);
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if 0
static MS_U32 _TSP_GetTrailOne64(MS_U64 u64Flags)
{
    MS_U32 u32Temp = (MS_U32)(u64Flags & 0xFFFFFFFF);

    if (u32Temp)
    {
        return MAsm_CPU_GetTrailOne(u32Temp);
    }
    return (32 + MAsm_CPU_GetTrailOne((MS_U32)(u64Flags>> 32)));
}
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
extern MS_PHY MDrv_FW_TSP_Addr(void);
extern MS_U32 MDrv_FW_TSP_Size(void);
#endif

//For Optee
#ifdef SECURE_PVR_ENABLE
extern MS_U32 MDrv_SYS_TEEC_InitializeContext(const char *name, TEEC_Context *context);
extern MS_U32 MDrv_SYS_TEEC_Open(TEEC_Context *context, TEEC_Session *session, const TEEC_UUID *destination, MS_U32 connection_method, const void *connection_data, TEEC_Operation *operation, MS_U32 *error_origin);
extern MS_U32 MDrv_SYS_TEEC_InvokeCmd(TEEC_Session *session, MS_U32 cmd_id, TEEC_Operation *operation, MS_U32 *error_origin);
extern void   MDrv_SYS_TEEC_Close(TEEC_Session *session);
extern void   MDrv_SYS_TEEC_FinalizeContext(TEEC_Context *context);
#endif

static MS_BOOL _TSP_GetFirmware(MS_PHY* pphyAddr, MS_U32* pu32Size)
{
#ifdef MSOS_TYPE_LINUX
    MS_S32 s32Fd;
#endif

    if (_ptsp_res->_u32FwSize)
    {
        *pphyAddr = _ptsp_res->_phyFwAddr;
        *pu32Size = _ptsp_res->_u32FwSize;
        return TRUE;
    }

#ifdef MSOS_TYPE_LINUX
    if (0> (s32Fd = open("/dev/fw", O_RDWR)))
    {
        TSP_DBG("[%s][%d] open fail /dev/fw\n", __FUNCTION__, __LINE__);
        MS_ASSERT(0);
        return FALSE;
    }
    if (ioctl(s32Fd, FW_IOC_TSP_ADDR, pphyAddr))
    {
        TSP_DBG("[%s][%d] FW_IOC_TSP_ADDR\n", __FUNCTION__, __LINE__);
        MS_ASSERT(0);
        close(s32Fd);
        return FALSE;
    }
    if (ioctl(s32Fd, FW_IOC_TSP_SIZE, pu32Size))
    {
        TSP_DBG("[%s][%d] FW_IOC_TSP_SIZE\n", __FUNCTION__, __LINE__);
        MS_ASSERT(0);
        close(s32Fd);
        return FALSE;
    }
    close(s32Fd);
    return TRUE;
#elif defined (MSOS_TYPE_LINUX_KERNEL)
    //*pphyAddr = MDrv_FW_TSP_Addr();
    //*pu32Size = MDrv_FW_TSP_Size();
    return TRUE;
#else
    return FALSE;
#endif
}

static void _TSP_CmdQ_Init(void)
{
    while (HAL_TSP_CmdQ_CmdCount()); // wait command finish
}

static MS_BOOL _TSP_InitResource(TSP_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii = 0;

    if(presource == NULL)
    {
        return FALSE;
    }

    presource->_s32TspMutexId = -1;
    presource->_s32IndMutexId = -1;
    presource->_s32IntRegMutexId = -1;

    presource->_u32LastErr = 1;
    presource->_bKerStrSuspend = FALSE;

    presource->_u32ProcessNum = 0;

    presource->_bSWInitReady = FALSE;
    presource->_phyFwAddr = 0;
    presource->_u32FwSize = 0;
    presource->_u32DbgIntrCount = 0;

    if(_ptsp_res->_TspState[0].TSPStatus != E_DRVTSP_SW_SUSPEND)
    {
        presource->_ifset_save.ePad = E_DRVTSP_PAD_EXT_INPUT0;
        presource->_ifset_save.bClkInv = FALSE;
        presource->_ifset_save.bExtSync = TRUE;
        presource->_ifset_save.bParallel = TRUE;
    }

    for(u32ii = 0; u32ii < TSP_PVR_IF_NUM; u32ii++)
    {
        presource->_PvrEvtNotify[u32ii] = E_DRVTSP_EVENT_DATA_INIT;
        presource->_PvrCallback[u32ii] = 0;
    }

    #if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    for(u32ii = 0; u32ii < TSP_PCRFLT_NUM; u32ii++)
    {
        presource->TSPHwPcrParams._u8ResetPcr[u32ii] = 3;
        presource->TSPHwPcrParams._u32PrevStcBase[u32ii] = 0;
        presource->TSPHwPcrParams._u32PrevPllControl[u32ii] = 0;
    }
    #endif

    presource->_phyTspFWDramBuf = 0;
    presource->_u32TspFwDramBufSize = 0;
    presource->_u32StcAdjustUnit = 100;

    #ifdef SECURE_PVR_ENABLE
    presource->_teemode.OsType = SYS_TEEINFO_OSTYPE_NOTEE;
    presource->_stSecBuf.u32BufId = 0;
    presource->_stSecBuf.u32BufOpt = 0;
    presource->_stSecBuf.u32BufSize = 0;
    presource->_stSecBuf.phyBufAddr = 0;
    #endif

    return TRUE;

}

#ifdef SECURE_PVR_ENABLE
//For REE, HK cowork with Tee by mailbox

MS_BOOL _bTSPMbxInitFlag = 0;
MS_U8   _u8TSPMbxMsgClass = 0;
MBX_Msg _TSPReeToTeeMbxMsg;
MBX_Msg _TSPTeeToReeMbxMsg;

MS_BOOL _TSP_REE_MbxParamToBufParam(MBX_Msg *pMsg, MS_U32* pu32BufAddr, MS_U32* pu32Size)
{
    MS_U32 u32addr = 0;

    if(pMsg->u8ParameterCount < 9)
        return FALSE;

    *pu32Size = ((MS_U32)pMsg->u8Parameters[0]) & 0xFFUL;
    *pu32Size += ((((MS_U32)pMsg->u8Parameters[1]) << 8UL) & 0xFF00UL);
    *pu32Size += ((((MS_U32)pMsg->u8Parameters[2]) << 16UL) & 0xFF0000UL);
    *pu32Size += ((((MS_U32)pMsg->u8Parameters[3]) << 24UL) & 0xFF000000UL);
    u32addr = ((MS_U32)pMsg->u8Parameters[4]) & 0xFFUL;
    u32addr += ((((MS_U32)pMsg->u8Parameters[5]) << 8UL) & 0xFF00UL);
    u32addr += (((MS_U32)pMsg->u8Parameters[6] << 16UL) & 0xFF0000UL);
    u32addr += (((MS_U32)pMsg->u8Parameters[7] << 24UL) & 0xFF000000UL);
    _miu_offset_to_phy(pMsg->u8Parameters[8], u32addr, *pu32BufAddr);

    return TRUE;
}

MS_BOOL _TSP_REE_RegisterMBX(void)
{
    MS_U8 ClassNum = 0;
    MBX_Result result = E_MBX_ERR_NOT_INITIALIZED;
    MS_U32 u32StartTime = 0;
    const MS_U32 u32RetryTimeOutMs = 3000;

    result = MApi_MBX_Init(E_MBX_CPU_MIPS,E_MBX_ROLE_HK,1000);
    if (( E_MBX_SUCCESS != result) && ( E_MBX_ERR_SLOT_AREADY_OPENNED != result ))
    {
        TSP_DBG("DMX_REE MApi_MBX_Init fail\n");
        return FALSE;
    }
    else
    {
        MApi_MBX_Enable(TRUE);
    }

    u32StartTime = MsOS_GetSystemTime();
    while((MsOS_GetSystemTime() - u32StartTime) < u32RetryTimeOutMs)
    {
        result = MApi_MBX_QueryDynamicClass(E_MBX_CPU_MIPS_VPE1, "DMX_TEE", (MS_U8 *)&ClassNum);
        if(result == E_MBX_SUCCESS)
            break;
        MsOS_DelayTask(100);
    };

    if(result != E_MBX_SUCCESS)
    {
        TSP_DBG("DMX_REE MApi_MBX_QueryDynamicClass fail\n");
        return FALSE;
    }

    result = MApi_MBX_RegisterMSG(ClassNum, 10);
    if (( E_MBX_SUCCESS != result) && ( E_MBX_ERR_SLOT_AREADY_OPENNED != result ))
    {
        TSP_DBG("%s fail\n",__FUNCTION__);
        return FALSE;
    }
    else
    {
        _bTSPMbxInitFlag = TRUE;
        _u8TSPMbxMsgClass = ClassNum;
        return TRUE;
    }
}

MS_BOOL _TSP_Ree_SendMBXMsg(TSP_REE_TO_TEE_CMD_TYPE msg_type, void* param)
{
    MBX_Result result = E_MBX_UNKNOW_ERROR;
    DrvTSP_SecureBuf *pstSecBuf = (DrvTSP_SecureBuf*)param;
    MS_U8 u8Id = 0;
    MS_U32 u32Addr = 0, u32Size = 0;
    MS_U32 u32Offset = 0;
    MS_U8 u8MiuSel = 0;
    MS_U8 u8Opt = 0;
    MS_U32 u32StartTime = MsOS_GetSystemTime();
    MS_U32 u32Timeout = 2000;
    const MS_U32 u32TotTimeout = 6000;
    const MS_U32 u32RevMsgTimeout = 2000;

    if (_bTSPMbxInitFlag == FALSE)
    {
        return FALSE;
    }

    //retry mbx msg sending, if sending fail
    u32StartTime = MsOS_GetSystemTime();
    while((MsOS_GetSystemTime() - u32StartTime) < u32TotTimeout)
    {
        _TSPReeToTeeMbxMsg.eRoleID = E_MBX_CPU_MIPS_VPE1;
        _TSPReeToTeeMbxMsg.u8Ctrl = 0;
        _TSPReeToTeeMbxMsg.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        _TSPReeToTeeMbxMsg.u8MsgClass = _u8TSPMbxMsgClass;
        _TSPReeToTeeMbxMsg.u8Index = msg_type;

        switch(msg_type)
        {
            case E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode:
            case E_DRVTSP_REE_TO_TEE_CMD_SET_VqBuf:
                break;
            case E_DRVTSP_REE_TO_TEE_CMD_SET_PvrBuf:
                if(pstSecBuf == 0)
                {
                    MS_DEBUG_MSG(TSP_DBG("[%s][%d] Parameter pointer of PVR buffer is 0!!!!\n",__FUNCTION__, __LINE__));
                    return FALSE;
                }
                u8Id = (MS_U8)(pstSecBuf->u32BufId);
                u8Opt = (MS_U8)(pstSecBuf->u32BufOpt);
                _TSPReeToTeeMbxMsg.u8Parameters[0] = u8Id;
                _TSPReeToTeeMbxMsg.u8Parameters[1] = u8Opt;
                _TSPReeToTeeMbxMsg.u8ParameterCount = 2;
                break;
            case E_DRVTSP_REE_TO_TEE_CMD_SET_FileinBuf:
                if(pstSecBuf == 0)
                {
                    MS_DEBUG_MSG(TSP_DBG("[%s][%d] Parameter pointer of Filein buffer is 0!!!!\n",__FUNCTION__, __LINE__));
                    return FALSE;
                }
                u8Id = (MS_U8)(pstSecBuf->u32BufId);
                u32Addr = (MS_U32)(pstSecBuf->phyBufAddr);
                u32Size = pstSecBuf->u32BufSize;
                _phy_to_miu_offset(u8MiuSel, u32Offset, u32Addr);
                _TSPReeToTeeMbxMsg.u8Parameters[0] = (MS_U8)(u32Size & 0xFFUL);
                _TSPReeToTeeMbxMsg.u8Parameters[1] = (MS_U8)((u32Size & 0xFF00UL) >> 8UL);
                _TSPReeToTeeMbxMsg.u8Parameters[2] = (MS_U8)((u32Size & 0xFF0000UL) >> 16UL);
                _TSPReeToTeeMbxMsg.u8Parameters[3] = (MS_U8)((u32Size & 0xFF000000UL) >> 24UL);
                _TSPReeToTeeMbxMsg.u8Parameters[4] = (MS_U8)(u32Offset & 0xFFUL);
                _TSPReeToTeeMbxMsg.u8Parameters[5] = (MS_U8)((u32Offset & 0xFF00UL) >> 8UL);
                _TSPReeToTeeMbxMsg.u8Parameters[6] = (MS_U8)((u32Offset & 0xFF0000UL) >> 16UL);
                _TSPReeToTeeMbxMsg.u8Parameters[7] = (MS_U8)((u32Offset & 0xFF000000UL) >> 24UL);
                _TSPReeToTeeMbxMsg.u8Parameters[8] = u8MiuSel;
                _TSPReeToTeeMbxMsg.u8Parameters[9] = u8Id;
                _TSPReeToTeeMbxMsg.u8ParameterCount = 10;
                break;
            case E_DRVTSP_REE_TO_TEE_CMD_GET_PvrWPtr:
                _TSPReeToTeeMbxMsg.u8Parameters[0] = (MS_U8)(pstSecBuf->u32BufId);
                _TSPReeToTeeMbxMsg.u8ParameterCount = 1;
                break;

            default:
                MS_DEBUG_MSG(TSP_DBG("[%s] Tee cmd not support\n",__FUNCTION__));
                return FALSE;
        }

        result = MApi_MBX_SendMsg(&_TSPReeToTeeMbxMsg);
        if (E_MBX_SUCCESS != result)
        {
            MsOS_DelayTask(100);
            continue;
        }

        // Receive Reply ACK from TEE side.
        memset(&_TSPTeeToReeMbxMsg, 0, sizeof(MBX_Msg));

        _TSPTeeToReeMbxMsg.u8MsgClass = _u8TSPMbxMsgClass;

        #if 0 // marked temperarily, wait kernel team to fix MApi_MBX_RecvMsg.
        if(E_MBX_SUCCESS != MApi_MBX_RecvMsg(TEE_MBX_MSG_CLASS, &(TEE_TO_REE_MBX_MSG), 20, MBX_CHECK_INSTANT_MSG))
        {
            VPU_MSG_ERR("VDEC get Secure world ACK fail\n");
            return E_TSP_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
        }
        else
        #else
        do
        {
            result = MApi_MBX_RecvMsg(_u8TSPMbxMsgClass, &_TSPTeeToReeMbxMsg, u32RevMsgTimeout, MBX_CHECK_INSTANT_MSG);
            u32Timeout -= u32RevMsgTimeout;
        } while((E_MBX_SUCCESS != result) && (u32Timeout > 0));
        #endif

        if((result == E_MBX_SUCCESS) && (_TSPTeeToReeMbxMsg.u8Index == E_TSP_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS))
        {
            break;
        }

        MsOS_DelayTask(100);

    }

    //parsing return data
    if((result == E_MBX_SUCCESS) && (_TSPTeeToReeMbxMsg.u8Index == E_TSP_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS))
    {
        switch(msg_type)
        {
            case E_DRVTSP_REE_TO_TEE_CMD_SET_PvrBuf:
                pstSecBuf->phyBufAddr = 0;
                pstSecBuf->u32BufSize = 0;
                _TSP_REE_MbxParamToBufParam(&_TSPTeeToReeMbxMsg, &u32Addr, &u32Size);
                pstSecBuf->phyBufAddr = ((MS_PHY)u32Addr) & 0xFFFFFFFFUL;
                pstSecBuf->u32BufSize = u32Size;
                break;
            case E_DRVTSP_REE_TO_TEE_CMD_GET_PvrWPtr:
                pstSecBuf->phyBufAddr = 0;
                if(_TSPTeeToReeMbxMsg.u8ParameterCount < 5)
                {
                    return FALSE;
                }
                u32Addr = ((MS_U32)_TSPTeeToReeMbxMsg.u8Parameters[1]) & 0xFFUL;
                u32Addr += ((((MS_U32)_TSPTeeToReeMbxMsg.u8Parameters[2]) << 8UL) & 0xFF00UL);
                u32Addr += ((((MS_U32)_TSPTeeToReeMbxMsg.u8Parameters[3]) << 16UL) & 0xFF0000UL);
                u32Addr += ((((MS_U32)_TSPTeeToReeMbxMsg.u8Parameters[4]) << 24UL) & 0xFF000000UL);
                _miu_offset_to_phy(_TSPTeeToReeMbxMsg.u8Parameters[0], u32Offset, u32Addr);
                pstSecBuf->phyBufAddr = ((MS_PHY)u32Addr) & 0xFFFFFFFFUL;
                break;
            default:
                break;
        }

        return TRUE;
    }

    //printf("[%s][%d] timeout!\n",__FUNCTION__,__LINE__);

    return FALSE;
}

#endif  //SECURE_PVR_ENABLE

static void _TSP_SwStateReset(MS_BOOL bInited)
{
    MS_U32 u32EngId, u32FilterId;

    if(bInited == TRUE)
        return;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (u32FilterId = 0; u32FilterId < TSP_SECFLT_NUM; u32FilterId++)
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32FilterId]        = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]         = E_DRVTSP_FLT_TYPE_SECTION; // for section filter
            _ptsp_res->_TspState[u32EngId].SecFltState[u32FilterId]     = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecBufState[u32FilterId]     = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecFltMode[u32FilterId]      = E_DRVTSP_FLT_MODE_CONTI;
            _ptsp_res->_TspState[u32EngId].SecBufState[u32FilterId]     = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecBufState[u32FilterId]     = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32FilterId] = E_DRVTSP_EVENT_DATA_INIT;
            _ptsp_res->_TspState[u32EngId].SecFltCallback[u32FilterId]  = NULL;
            _ptsp_res->_TspState[u32EngId].s32TspEvtId[u32FilterId]     = -1;
            #ifdef DUP_PIDFLT
            _ptsp_res->_TspState[u32EngId].DupFlt[u32FilterId]          = 0xFFFFUL;
            #endif

            #ifdef MERGE_STR_SUPPORT
            _ptsp_res->_TspState[u32EngId].FltSrcId[u32FilterId]        = 0UL;
            #endif
        }

        for (u32FilterId = TSP_SECFLT_NUM; u32FilterId < TSP_PIDFLT_NUM_ALL; u32FilterId++)
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32FilterId] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]  = E_DRVTSP_FLT_TYPE_PVR;
            #ifdef DUP_PIDFLT
            _ptsp_res->_TspState[u32EngId].DupFlt[u32FilterId]   = 0xFFFFUL;
            #endif

            #ifdef MERGE_STR_SUPPORT
            _ptsp_res->_TspState[u32EngId].FltSrcId[u32FilterId]        = 0UL;
            #endif
        }

#ifdef HWPCR_ENABLE
        for (u32FilterId = TSP_PIDFLT_NUM; u32FilterId < TSP_PIDFLT_NUM_ALL; u32FilterId++)
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32FilterId] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]  = E_DRVTSP_FLT_TYPE_PCR;
        }
#endif
    }

    //---------------------------------------------------------------------------------
    //Select source of every range of DSCMB filters
    // if TSP_CA_ENGINE_NUM is 3, ENG0 for TS0_Live & TS0_File, Eng1 for TS1, Eng2 for TS2
    // if TSP_CA_ENGINE_NUM is 4, ENG0 for TS0_Live, ENG1 for TS0_File, Eng2 for TS1, Eng3 for TS2
    //---------------------------------------------------------------------------------
    _ptsp_res->_DscmbFlt_State[0].u32Src                = E_DRVTSP_FLT_SOURCE_TYPE_LIVE|E_DRVTSP_FLT_SOURCE_TYPE_FILE; //eng0 src
    _ptsp_res->_DscmbFlt_State[0].u32Start              = DSCMB_FLT_START_ID;  //# eng0 filter range
    _ptsp_res->_DscmbFlt_State[0].u32End                = DSCMB_FLT_END_ID;
    _ptsp_res->_DscmbFlt_State[0].u32ShareStart         = DSCMB_FLT_SHAREKEY_START_ID;
    _ptsp_res->_DscmbFlt_State[0].u32ShareEnd           = DSCMB_FLT_SHAREKEY_END_ID;

    #if (TSP_CA_ENGINE_NUM >= 2)
    _ptsp_res->_DscmbFlt_State[1].u32Start              = DSCMB1_FLT_START_ID;  //# eng1 filter range
    _ptsp_res->_DscmbFlt_State[1].u32End                = DSCMB1_FLT_END_ID;
    _ptsp_res->_DscmbFlt_State[1].u32ShareStart         = DSCMB_FLT_SHAREKEY1_START_ID;
    _ptsp_res->_DscmbFlt_State[1].u32ShareEnd           = DSCMB_FLT_SHAREKEY1_END_ID;
    _ptsp_res->_DscmbFlt_State[1].u32Src                = E_DRVTSP_FLT_SOURCE_TYPE_TS1|E_DRVTSP_FLT_SOURCE_TYPE_FILE1; //eng1 src
    #endif

    #if (TSP_CA_ENGINE_NUM >= 3)
    _ptsp_res->_DscmbFlt_State[2] .u32Src               = E_DRVTSP_FLT_SOURCE_TYPE_TS2|E_DRVTSP_FLT_SOURCE_TYPE_FILE2; //eng2 src
    _ptsp_res->_DscmbFlt_State[2].u32Start              = DSCMB2_FLT_START_ID;  //# eng2 filter range
    _ptsp_res->_DscmbFlt_State[2].u32End                = DSCMB2_FLT_END_ID;
    _ptsp_res->_DscmbFlt_State[2].u32ShareStart         = DSCMB_FLT_SHAREKEY2_START_ID;
    _ptsp_res->_DscmbFlt_State[2].u32ShareEnd           = DSCMB_FLT_SHAREKEY2_END_ID;
    #endif

    #if (TSP_CA_ENGINE_NUM == 4)
    _ptsp_res->_DscmbFlt_State[3].u32Src                = 0; //eng3 src
    _ptsp_res->_DscmbFlt_State[3].u32Start              = DSCMB3_FLT_START_ID;
    _ptsp_res->_DscmbFlt_State[3].u32End                = DSCMB3_FLT_END_ID;
    _ptsp_res->_DscmbFlt_State[3].u32ShareStart         = DSCMB_FLT_SHAREKEY_START_ID;
    _ptsp_res->_DscmbFlt_State[3].u32ShareEnd           = DSCMB_FLT_SHAREKEY_END_ID;
    #endif

}

static MS_U8 _TSP_Get_DscmbEngId_ByFltSrc(DrvTSP_FltType FltSrc)
{
    MS_U8 u8ii = 0xFF;

    for(u8ii = 0; u8ii < ((MS_U8)TSP_CA_ENGINE_NUM); u8ii++)
    {
        if(FltSrc & _ptsp_res->_DscmbFlt_State[u8ii].u32Src)
            break;
    }

    if(u8ii == TSP_CA_ENGINE_NUM)
    {
        u8ii = 0xFF;
    }

    return u8ii;
}

static void _TSP_SW_Exit(void)
{
    MS_U8 u8ii, u8tmp;
    MS_U32 u32Timer, u32CurTime = 0;

    //printf("[%s][%d] _u32LibMode %d %d\n", __FUNCTION__, __LINE__, (int)_u32LibMode, (int)_db_flag);

    if(_u32LibMode != DRVTSP_CMD_LIB_MODE_KRN)
    {
        _TSP_EXT_LOCK();
        _db_flag = 1;
        _TSP_EXT_UNLOCK();

        _TSP_IND_UNLOCK();
        u32Timer = MsOS_GetSystemTime();
        // check _s32TspTaskId for NOS case
        while(_s32TspTaskId > 0)
        {
            _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_FLT_FREE);
            _TSP_EXT_LOCK();
            if (_db_flag == 2)
            {
                _db_flag = 0;
                //_TSP_EXT_UNLOCK();
                break;
            }
            _TSP_EXT_UNLOCK();

            u32CurTime = MsOS_GetSystemTime();
            if(u32CurTime < u32Timer)
            {
                printf("[%s][%d] Systemtime Change %d %d %x\n", __FUNCTION__, __LINE__, (int)u32CurTime, (int)u32Timer, (int)_s32TspTaskId);
                u32Timer = u32CurTime;
            }
            if ((u32CurTime-u32Timer) > 10000UL)
            {
                printf("[%s][%d] TSP task exit timeout %d %d %x !!!!!!\n", __FUNCTION__, __LINE__, (int)u32CurTime, (int)u32Timer, (int)_s32TspTaskId);
                break;
            }
            MsOS_DelayTask(3);
        }

        _TSP_IND_LOCK();

        /*if(_s32TspTaskId > 0)
            {
             MsOS_DeleteTask(_s32TspTaskId);
            _s32TspTaskId = -1;
            }*/
    }
    else
    {
        if(_ptsp_res->_TspState[0].TSPStatus == E_DRVTSP_SW_SUSPEND)
        {
            #ifndef MSOS_TYPE_LINUX_KERNEL
            _ptsp_res->_bSWInitReady = FALSE;
            #endif

            return;
        }

        #if 0
        //to free CB process task, so send one event
        _db_flag = 1;
        _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_FLT_FREE);
        MsOS_DelayTask(3);

        while(_db_flag != 2)
        {
            _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_FLT_FREE);
            MsOS_DelayTask(3);
        }

        while(OSAL_TSP_EventDelete(_s32TspEventId) == FALSE)
        {
            _TSP_SetEvent(TSP_TASK_EVENT_FLT_FREE);
            printf("Waiting for evet deleting... \n");
            MsOS_DelayTask(3);
        }
        _s32TspEventId = -1;
        #endif

        _TSP_EXT_LOCK();
    }

    #if 0
    u8tmp = (MS_U8)((TSP_PIDFLT_NUM_ALL+31UL)>>5UL);
    for (u8ii = 0; u8ii < u8tmp; u8ii++)
    {
        if(_s32EventFree0[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32EventFree0[u8ii]);
            _s32EventFree0[u8ii] = -1;
        }
        if(_s32DoneFlags[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32DoneFlags[u8ii]);
            _s32DoneFlags[u8ii] = -1;
        }

        /*if(_s32EventFreeDefer0[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32EventFreeDefer0[u8ii]);
            _s32EventFreeDefer0[u8ii] = -1;
        }*/
    }
    #endif

    if (_s32ExtMutexId != -1)
    {
        //_TSP_EXT_LOCK();
        _TSP_EXT_UNLOCK();
        MsOS_DeleteMutex(_s32ExtMutexId);
        _s32ExtMutexId = -1;
    }

    if (_s32TspEventId != -1)
    {
        if(_u32LibMode != DRVTSP_CMD_LIB_MODE_KRN)
        {
            OSAL_TSP_EventDelete(_s32TspEventId);
        }
        _s32TspEventId = -1;
    }

    u8tmp = (MS_U8)((TSP_SECFLT_NUM+31UL)>>5UL);
    for (u8ii = 0; u8ii < u8tmp; u8ii++)
    {
        if(_s32TspSecOvfId[u8ii] != -1)
        {
            //printf("[%s][%d] %d %x\n", __FUNCTION__, __LINE__, (int)u8ii, (int)_s32TspSecOvfId[u8ii]);
            OSAL_TSP_EventDelete(_s32TspSecOvfId[u8ii]);
            _s32TspSecOvfId[u8ii] = -1;
        }
        if(_s32TspSecCrcId[u8ii] != -1)
        {
            //printf("[%s][%d] %d %x\n", __FUNCTION__, __LINE__, (int)u8ii, (int)_s32TspSecCrcId[u8ii]);
            OSAL_TSP_EventDelete(_s32TspSecCrcId[u8ii]);
            _s32TspSecCrcId[u8ii] = -1;
        }
        if(_s32TspFltDisableId[u8ii] != -1)
        {
            //printf("[%s][%d] %d %x\n", __FUNCTION__, __LINE__, (int)u8ii, (int)_s32TspFltDisableId[u8ii]);
            OSAL_TSP_EventDelete(_s32TspFltDisableId[u8ii]);
            _s32TspFltDisableId[u8ii] = -1;
        }
        if(_s32TspSecRdyId[u8ii] != -1)
        {
            //printf("[%s][%d] %d %x\n", __FUNCTION__, __LINE__, (int)u8ii, (int)_s32TspSecRdyId[u8ii]);
            OSAL_TSP_EventDelete(_s32TspSecRdyId[u8ii]);
            _s32TspSecRdyId[u8ii] = -1;
        }
    }

    //checking if stiil there are other processes exist
    if(--_ptsp_res->_u32ProcessNum > 0)
        return;

    if (_ptsp_res->_s32IntRegMutexId != -1)
    {
        _TSP_INT_LOCK();
        _TSP_INT_UNLOCK();
        MsOS_DeleteMutex(_ptsp_res->_s32IntRegMutexId);
        _ptsp_res->_s32IntRegMutexId = -1;
    }

    if (_ptsp_res->_s32IndMutexId != -1)
    {
        MsOS_ReleaseMutex(_ptsp_res->_s32IndMutexId);
        MsOS_DeleteMutex(_ptsp_res->_s32IndMutexId);
        _ptsp_res->_s32IndMutexId = -1;
    }

    if (_ptsp_res->_s32TspMutexId != -1)
    {
        MsOS_ReleaseMutex(_ptsp_res->_s32TspMutexId);
        MsOS_DeleteMutex(_ptsp_res->_s32TspMutexId);
        _ptsp_res->_s32TspMutexId = -1;
    }

    if(_ptsp_res->_TspState[0].TSPStatus != E_DRVTSP_SW_SUSPEND)
    {
        _ptsp_res->_TspState[0].TSPStatus  = E_DRVTSP_SW_EXIT;
    }

#ifdef SECURE_PVR_ENABLE
    MDrv_TSP_Ree_TeeCmdSystem_Exit();
#endif

    _ptsp_res->_bSWInitReady           = FALSE;

}

static void _TSP_Close(void)
{
    HAL_TSP_Int_Disable(TSP_HWINT_ALL);
    HAL_TSP_Int2_Disable(TSP_HWINT2_ALL);

    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();
    HAL_TSP_WbDmaEnable(FALSE);
    HAL_TSP_SetCtrlMode(0, 0, TSP_IF_NUM);
    _TSP_SW_Exit();
    HAL_TSP_PowerCtrl(DISABLE);
    HAL_TSP_HW_Lock_Release();
}

//------------------------------------------------
//Default STC mapping:
//STC0: live0 & file
//STC1: TS1 & TS2
//------------------------------------------------
//If FltSrc = 0, goto auto arranging directly
//------------------------------------------------
static TSP_Result _TSP_STC_Alloc(DrvTSP_FltType FltSrc, MS_U32 *pu32EngId)
{
    MS_U32 i;
    *pu32EngId = 0xFFFFFFFF;

    if((_ptsp_res->_bStcAllocDisable))
        return DRVTSP_FAIL;

    if((FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_LIVE) || (FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE))
    {
        if(_ptsp_res->_TspStcState[0].bUsed == FALSE)
        {
            i = 0;
            goto FOUND;
        }
    }
    else if((FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TS1) || (FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TS2)
        || (FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE1) || (FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE2))
    {
        if(_ptsp_res->_TspStcState[1].bUsed == FALSE)
        {
            i = 1;
            goto FOUND;
        }
    }

    for(i = 0; i < TSP_STC_NUM; i++)
    {
        if(_ptsp_res->_TspStcState[i].bUsed == FALSE)
        {
            goto FOUND;
        }
    }

    if(*pu32EngId == 0xFFFFFFFF)
    {
        return DRVTSP_FAIL;
    }

FOUND:

    _ptsp_res->_TspStcState[i].u32SrcType = FltSrc;
    _ptsp_res->_TspStcState[i].bUsed = TRUE;
    *pu32EngId = i;

    return DRVTSP_OK;
}

static TSP_Result _TSP_STC_Free(MS_U32 u32EngId)
{
    if(_ptsp_res->_TspStcState[u32EngId].bUsed == FALSE)
        return DRVTSP_OK;

    _ptsp_res->_TspStcState[u32EngId].bUsed = FALSE;
    _ptsp_res->_TspStcState[u32EngId].u32PcrFltId = 0xFFFFFFFF;
    _ptsp_res->_TspStcState[u32EngId].u32SrcType = 0xFFFFFFFF;
    return DRVTSP_OK;
}

static TSP_Result _TSP_STC_Select(DrvTSP_FltType eFltSrc, MS_U32 u32StcEng)
{
    MS_U32 u32Src = 0;

    switch(eFltSrc & E_DRVTSP_FLT_SOURCE_TYPE_MASK)
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            u32Src = TSP_PIDFLT_IN_PIDFLT0;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            u32Src = TSP_PIDFLT_IN_PIDFLT_FILE;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
            u32Src = TSP_PIDFLT_IN_PIDFLT1;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
            u32Src = TSP_PIDFLT_IN_PIDFLT2;
            break;
        default:
            return DRVTSP_INVALID_PARAM;
    }
    if (HAL_TSP_SelectSTCEng(u32Src, u32StcEng))
    {
        return DRVTSP_OK;
    }
    else
    {
        return DRVTSP_FAIL;
    }
}

static TSP_Result _TSP_STC_SetPcrFlt(MS_U32 u32EngId, MS_U32 u32PcrFltId)
{

    _ptsp_res->_TspStcState[u32EngId].u32PcrFltId = u32PcrFltId;

    return DRVTSP_OK;
}

static TSP_Result _TSP_PcrId_To_StcId(MS_U32 u32PcrFltId,MS_U32 *pu32EngId)
{
    MS_U32 i;

    *pu32EngId = 0xFFFFFFFF;

    for(i = 0; i < TSP_STC_NUM; i++)
    {
        if(_ptsp_res->_TspStcState[i].u32PcrFltId == u32PcrFltId)
        {
            *pu32EngId = i;
            return DRVTSP_OK;
        }
    }

    return DRVTSP_FAIL;
}

static void _TSP_PidFlt_Init(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    if(u32PidFltId >= TSP_PIDFLT_NUM_ALL)
    {
        return;
    }

    HAL_TSP_PidFlt_SetPid(u32EngId, u32PidFltId, DRVTSP_PID_NULL);
    HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_NONE);
    HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_NONE);
    HAL_TSP_PidFlt_SelSecFlt(u32EngId, u32PidFltId, TSP_PIDFLT_SECFLT_NULL);

    #ifdef MERGE_STR_SUPPORT
    HAL_TSP_PidFlt_SetFltSrcStreamID(u32EngId, u32PidFltId, 0);
    #endif

    _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] = DRVTSP_MAP_NULL;
}

static void _TSP_PidFlt_Disable(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_PIDFLT_NUM)
    {
        MS_U32 u32Idx = u32PidFltId - TSP_PIDFLT_NUM;

        HAL_TSP_PidFlt_HWPcrFlt_Enable(u32Idx, FALSE);
        HAL_TSP_PidFlt_SetHWPcrPid(u32Idx, DRVTSP_PID_NULL);
        #if (TSP_HWPCR_BY_HK == 1)
        HAL_TSP_HWPcr_Int_Enable(u32Idx, FALSE);
        _TSP_RemoveHwPcrNotifyId(u32Idx);
        #endif
    }
    else
    {
#endif
        #ifdef DUP_PIDFLT
        if(_ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId] != 0xFFFFUL)
        {
            MS_U32 u32Idx = _ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId];
            HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32Idx, TSP_PIDFLT_OUT_NONE);
            _ptsp_res->_TspState[u32EngId].FltState[u32Idx] &= (DrvTSP_FltState)(~E_DRVTSP_FLT_STATE_ENABLE);
            //ULOGD("TSP", "[%s][%d] \n", __FUNCTION__, __LINE__);
        }
        #endif

        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_NONE);

#ifdef HWPCR_ENABLE
    }
#endif

    _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] &= (DrvTSP_FltState)(~E_DRVTSP_FLT_STATE_ENABLE);
}

// Note: This is only for PID filter & Section filter to be one by one mapping, and section filter number is equal to pid filter number.
// DSCMB0 filter        :    16~31  --> Source from live0 and file
// DSCMB1 filter        :    32~47  --> Source from TS1 and TS2
// DSCMB2 filter        :    48~63  --> Source from TS2
// SubDSCMB0 filter  :     80~95
// SubDSCMB1 filter  :     96~111
// SubDSCMB2 filter  :     112~127
// Section filter          :     0 ~63 / 0~127
// Total 128 filters
//
//  -----------------  0
//  |                           |
//  -----------------  16
//  |   DSCM0             |
//  -----------------  32
//  |   DSCM1             |
//  -----------------  48
//  |   DSCM2             |
//  -----------------  64
//  |                           |
//  -----------------  79
//  |   SubDSCMB0      |
//  -----------------  96
//  |   SubDSCMB1      |
//  -----------------  111
//  |   SubDSCMB2      |
//  -----------------  127
//  |   HWPCR             |
//  -----------------  129
static MS_BOOL _TSP_PidFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32PidFltId)
{
    MS_S32  i = 0, j = 0;
    MS_U32  pid_idx_start = 0;
    MS_U32  pid_idx_end = 0;
    MS_BOOL bFound = FALSE;
    MS_U8   u8CAEng = 0;
    MS_U32  u32StcEng = 0xFFFFFFFF;

    #ifdef DUP_PIDFLT_API
    MS_U32  u32DupFlag = eFilterType & E_DRVTSP_FLT_TYPE_DUPSEC;
    #endif

    *pu32PidFltId = 0xFFFFFFFF;

    if((E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK)) && !(_ptsp_res->_bStcAllocDisable))
    {
        if(_TSP_STC_Alloc(eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_MASK, &u32StcEng) == FALSE)
        {
            goto FIND_END;
        }
    }

#ifdef HWPCR_ENABLE
    if(E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        for(i = TSP_PIDFLT_NUM; i < TSP_PIDFLT_NUM_ALL; i++)
        {
            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                goto FIND_END;
            }
        }
    }
    else
#endif
    if((eFilterType & E_DRVTSP_FLT_SCMB_MASK) ||
#ifndef HWPCR_ENABLE
        (E_DRVTSP_FLT_TYPE_PCR    == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
#endif
        (E_DRVTSP_FLT_TYPE_VIDEO  == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
        (E_DRVTSP_FLT_TYPE_AUDIO  == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
        (E_DRVTSP_FLT_TYPE_AUDIO2 == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
        (E_DRVTSP_FLT_TYPE_AUDIO3 == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
        (E_DRVTSP_FLT_TYPE_AUDIO4 == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
        (E_DRVTSP_FLT_TYPE_VIDEO3D == (eFilterType&~E_DRVTSP_FLT_MASK)))
    {
        MS_U32 u321stShareIdx = 0;

        u8CAEng = _TSP_Get_DscmbEngId_ByFltSrc(eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_MASK);

        if(u8CAEng < TSP_CA_ENGINE_NUM)
        {
            if((eFilterType & E_DRVTSP_FLT_SCMB_MASK) == E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY)
            {
                pid_idx_start = _ptsp_res->_DscmbFlt_State[u8CAEng].u32ShareStart;
                pid_idx_end = _ptsp_res->_DscmbFlt_State[u8CAEng].u32ShareEnd;
            }
            else
            {
                pid_idx_start = _ptsp_res->_DscmbFlt_State[u8CAEng].u32Start + 1; //reserved first filter for sharekey filter
                pid_idx_end = _ptsp_res->_DscmbFlt_State[u8CAEng].u32End;
            }
            u321stShareIdx = _ptsp_res->_DscmbFlt_State[u8CAEng].u32Start;
        }

        // For PCR & PVRCA, allocate filter form bottom to top
        if((E_DRVTSP_FLT_TYPE_PVR  == (eFilterType&~E_DRVTSP_FLT_MASK))
#ifndef HWPCR_ENABLE
            || (E_DRVTSP_FLT_TYPE_PCR    == (eFilterType&~E_DRVTSP_FLT_MASK))
#endif
            || (E_DRVTSP_FLT_TYPE_PVR1  == (eFilterType&~E_DRVTSP_FLT_MASK)))
        {
            for(i = (int)pid_idx_end; i >= (int)pid_idx_start; i--)
            {
                if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
                {
                    bFound = TRUE;
                    goto FIND_END;
                }
            }
            if((eFilterType & E_DRVTSP_FLT_SCMB_MASK) == E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY)
            {
                i = u321stShareIdx;
                if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
                {
                    bFound = TRUE;
                    goto FIND_END;
                }
            }
        }
        else  //For Live, allocate filter from top to bottom
        {
            if((eFilterType & E_DRVTSP_FLT_SCMB_MASK) == E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY)
            {
                i = u321stShareIdx;
                if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
                {
                    bFound = TRUE;
                    goto FIND_END;
                }
            }

            for(i = (int)pid_idx_start; i <= (int)pid_idx_end; i++)
            {
                //Reserved filter #17 for NMatch temp writting, can not be used for section data
                if((i == TSP_NMATCH_FLTID) && (eFilterType & E_DRVTSP_FLT_TYPE_SECTION_MASK))
                {
                    continue;
                }

                if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
                {
                    bFound = TRUE;
                    goto FIND_END;
                }
            }
        }

        #ifndef GENERAL_DSCMB
        if((eFilterType & E_DRVTSP_FLT_SCMB_MASK) != 0)
        {
            goto FIND_END;
        }
        #endif
    }

    if(eFilterType & E_DRVTSP_FLT_TYPE_SECTION_MASK)
    {
        pid_idx_start = TSP_SECFLT_NUM - 1;
        pid_idx_end = pid_idx_start;
    }
    else
    {
        pid_idx_start = TSP_PIDFLT_NUM_ALL - 1;
        pid_idx_end = DSCMB_FLT_SHAREKEY_END_ID;
    }

    // #1 Using Non-descramble range first, and from top to buttom; at last using descramble range form bottom to top
    for(i = 0; ((i < DSCMB_FLT_START_ID) && (bFound == FALSE)); i++)
    {
        if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
        {
            bFound = TRUE;
            goto FIND_END;
        }
    }

    // # 2 Using Non-descramble range first, and from top to buttom; at last using descramble range form bottom to top
    #if((!defined(DUP_PIDFLT_API)) && (TSP_CA_ENGINE_NUM > 2))
    for(i = DSCMB2_FLT_END_ID + 1; ((i < DSCMB_FLT_SHAREKEY_START_ID) && (bFound == FALSE)); i++)
    {
        if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
        {
            bFound = TRUE;
            goto FIND_END;
        }
    }
    #else
    for(i = DSCMB_FLT_SHAREKEY_START_ID; ((i <= pid_idx_end) && (bFound == FALSE)); i++)
    {
        //Reserved filter #17 for NMatch temp writting, can not be used for section data
        if((i == TSP_NMATCH_FLTID) && (eFilterType & E_DRVTSP_FLT_TYPE_SECTION_MASK)
            && (E_DRVTSP_FLT_TYPE_PCR != (eFilterType&~E_DRVTSP_FLT_MASK)))
            continue;

        if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
        {
            bFound = TRUE;
            goto FIND_END;
        }
    }
    #endif

    // # 3 Using Non-descramble range first, and from top to buttom; at last using descramble range form bottom to top
    for(i = (int)pid_idx_start; ((i >= 0) && (bFound == FALSE)); i--)
    {
        //Reserved filter #17 for NMatch temp writting, can not be used for section data
        if((i == TSP_NMATCH_FLTID) && (eFilterType & E_DRVTSP_FLT_TYPE_SECTION_MASK)
            && (E_DRVTSP_FLT_TYPE_PCR != (eFilterType&~E_DRVTSP_FLT_MASK)))
            continue;

        if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
        {
            bFound = TRUE;
            break;
        }
    }

FIND_END:

    if (bFound == FALSE)
    {
        _TSP_STC_Free(u32StcEng);
        TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] No free fitler found 0x%02x\n", __LINE__, (int)i));
        return FALSE;
    }

    if(E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        if(_ptsp_res->_bStcAllocDisable)
        {
            for(j = 0; j< TSP_STC_NUM; j++)
            {
                if(_ptsp_res->_TspStcState[j].u32SrcType == (eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_MASK))
                {
                    _TSP_STC_SetPcrFlt(j, (MS_U32)i);
                }
            }
        }
        else
        {
            _TSP_STC_SetPcrFlt(u32StcEng, (MS_U32)i);
            _TSP_STC_Select(eFilterType, u32StcEng);
        }
    }
    _ptsp_res->_TspState[u32EngId].FltState[i] = E_DRVTSP_FLT_STATE_ALLOC;
    _ptsp_res->_TspState[u32EngId].FltType[i] = (eFilterType&(DrvTSP_FltType)(~E_DRVTSP_FLT_MASK));
    _ptsp_res->_TspState[u32EngId].FltSource[i] = eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_MASK;
    #ifdef MERGE_STR_SUPPORT
    _ptsp_res->_TspState[u32EngId].FltSrcId[i] =  (eFilterType & E_DRVTSP_FLT_SOURCEID_MASK) >> E_DRVTSP_FLT_SOURCEID_SHIFT;
    #endif

    _TSP_PidFlt_Init(u32EngId, i);
    *pu32PidFltId = i;

    #ifdef DUP_PIDFLT_API
    if(u32DupFlag == E_DRVTSP_FLT_TYPE_DUPSEC)
    {
        MS_BOOL bDupFound = FALSE;

        for(i = (DSCMB2_FLT_END_ID + 1); i < DSCMB_FLT_SHAREKEY_START_ID; i++)
        {
            if(_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_NA)
            {
                bDupFound = TRUE;
                break;
            }
        }
        if(bDupFound == FALSE)
        {
            for(i = (DSCMB_FLT_START_ID - 1); i >= 0; i--)
            {
                if(_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_NA)
                {
                    bDupFound = TRUE;
                    break;
                }
            }
        }

        if(bDupFound == TRUE)
        {
            _TSP_PidFlt_Init(u32EngId, i);
            _ptsp_res->_TspState[u32EngId].DupFlt[*pu32PidFltId] = i;
            _ptsp_res->_TspState[u32EngId].FltState[i] = E_DRVTSP_FLT_STATE_ALLOC;
            _ptsp_res->_TspState[u32EngId].FltType[i] = _ptsp_res->_TspState[u32EngId].FltType[*pu32PidFltId];
            _ptsp_res->_TspState[u32EngId].FltSource[i] = _ptsp_res->_TspState[u32EngId].FltSource[*pu32PidFltId];
            //ULOGD("TSP", "[%s] duplcate id %ld %d\n", __FUNCTION__, *pu32PidFltId, i);
        }
    }
    #endif

    //ULOGD("TSP", "[%s] %d, time %d, u32DupFlag %x\n", __FUNCTION__, i, MsOS_GetSystemTime(), u32DupFlag);

    return TRUE;
}

static TSP_Result _TSP_PidFlt_Free(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bResetState)
{
    MS_U32 u32SecFltId = 0;
    MS_U32 u32STCEng = 0xFFFFFFFF;

    if ( (u32EngId    >= TSP_ENGINE_NUM) ||
         (u32PidFltId >= TSP_PIDFLT_NUM_ALL) ||
         (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] == E_DRVTSP_FLT_STATE_FREE) )
    {
        //TSP_DBG("[TSP_ERROR][%06d] (EngId, PidFltId, FilterState)= (%ld %ld %d)\n", __LINE__, u32EngId, u32PidFltId, _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId]);
        MS_CRITICAL_MSG(TSP_DBG("[TSP_ERROR][%06d] Bad free parmeter\n",__LINE__));;
        return (TSP_Result)FALSE;
    }

    u32SecFltId = _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId];

    _TSP_PidFlt_Disable(u32EngId, u32PidFltId);

    if((_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId] == E_DRVTSP_FLT_TYPE_PCR)
       && (_ptsp_res->_bStcAllocDisable == FALSE))
    {
        if(_TSP_PcrId_To_StcId(u32PidFltId, &u32STCEng) == DRVTSP_OK)
        {
            _TSP_STC_Free(u32STCEng);
        }
    }

    // Release Filter
    _TSP_PidFlt_Init(u32EngId, u32PidFltId);

    #ifdef DUP_PIDFLT
    if(_ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId] != 0xFFFFUL)
    {
        MS_U32 u32DupId = _ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId];

        _TSP_PidFlt_Init(u32EngId, u32DupId);
        _ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId] = 0xFFFFUL;
        _ptsp_res->_TspState[u32EngId].FltState[u32DupId] = E_DRVTSP_FLT_STATE_NA;
        //ULOGD("TSP", "[%s][%d] dup id %ld\n", __FUNCTION__, __LINE__, u32DupId);
    }
    #endif

    // Reset call back function without PVR
    if (u32SecFltId < TSP_SECFLT_NUM)
    {
        _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecFltId] = NULL;
        _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId] = E_DRVTSP_FLT_MODE_CONTI;
    }

    // clear all event while filter free
    if(u32SecFltId < TSP_SECFLT_NUM)
    {
        MS_U32 u32Dummy = 0;

        _TSP_GetSecOvfId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecCrcId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecRdyId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetFltDisableId(u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
    }

    if (bResetState)
    {
        _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] = E_DRVTSP_FLT_STATE_FREE;
    }

    return (TSP_Result)TRUE;
}

static MS_BOOL _TSP_PidFlt_StateGet(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltState *peState)
{
    MS_U32 u32SecFltId = 0;
    MS_U8 u8ScmbSrc = 0;

    // don't need to check PVR pid filter
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT(TSP_PIDFLT_NUM_ALL > u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFlt Id %u\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT(peState, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    *peState=   _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId];

    switch(_ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
    {
        default:
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            u8ScmbSrc = 0;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            u8ScmbSrc = 1;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
            u8ScmbSrc = 2;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
            u8ScmbSrc = 3;
            break;
    }

#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_PIDFLT_NUM_ALL)
    {
        return TRUE;
    }
#endif

    if(HAL_TSP_Scmb_Status(u8ScmbSrc, (MS_U8)(u32PidFltId >> 5UL), u32PidFltId))
    {
        *peState |= E_DRVTSP_FLT_STATE_SCRAMBLED;
    }

    switch (_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        case E_DRVTSP_FLT_TYPE_SECTION:
        case E_DRVTSP_FLT_TYPE_PES:
        case E_DRVTSP_FLT_TYPE_PACKET:
        case E_DRVTSP_FLT_TYPE_TELETEXT:
        case E_DRVTSP_FLT_TYPE_SECTION_VER:
        case E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI:
            u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(u32EngId, u32PidFltId);
            if (HAL_TSP_SecBuf_GetBufRead(u32EngId, u32SecFltId) != HAL_TSP_SecBuf_GetBufWrite(u32EngId, u32SecFltId))
            {
                *peState |= E_DRVTSP_FLT_STATE_STREAM_AVAIL;
            }
            break;
        case E_DRVTSP_FLT_TYPE_VIDEO:
        case E_DRVTSP_FLT_TYPE_AUDIO:
        case E_DRVTSP_FLT_TYPE_AUDIO2:
        case E_DRVTSP_FLT_TYPE_VIDEO3D:
        case E_DRVTSP_FLT_TYPE_PCR:
        default:
            break;
    }
    return TRUE;
}

static void _TSP_SecFlt_SetType(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltType eSecFltType)
{
    MS_U32 u32FltType = TSP_SECFLT_TYPE_SEC;

    switch (eSecFltType)
    {
        case E_DRVTSP_FLT_TYPE_TELETEXT:
            u32FltType = TSP_SECFLT_TYPE_TTX;
            break;
        case E_DRVTSP_FLT_TYPE_PCR:
            u32FltType = TSP_SECFLT_TYPE_PCR;
            break;
        case E_DRVTSP_FLT_TYPE_PES:
            u32FltType = TSP_SECFLT_TYPE_PES;
            break;
        case E_DRVTSP_FLT_TYPE_SECTION:
            u32FltType = TSP_SECFLT_TYPE_SEC;
            break;
        case E_DRVTSP_FLT_TYPE_PACKET:
            u32FltType = TSP_SECFLT_TYPE_PKT;
            break;
        case E_DRVTSP_FLT_TYPE_ECM:
            u32FltType = TSP_SECFLT_TYPE_ECM;
            break;
        case E_DRVTSP_FLT_TYPE_EMM:
            u32FltType = TSP_SECFLT_TYPE_EMM;
            break;
        case E_DRVTSP_FLT_TYPE_SECTION_VER:
            u32FltType = TSP_SECFLT_TYPE_VER;
            break;
        case E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI:
            u32FltType = TSP_SECFLT_TYPE_SEC_NO_PUSI;
            break;
        case E_DRVTSP_FLT_TYPE_PVR:
        case E_DRVTSP_FLT_TYPE_CB:
        default:
            break;
    }
    HAL_TSP_SecFlt_SetType(u32EngId, u32SecFltId, u32FltType);
}

static MS_BOOL _TSP_SecFlt_StateGet(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltState *peState)
{
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT(peState, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    *peState =  _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId];
    if (HAL_TSP_SecFlt_GetState(0, u32SecFltId) & TSP_SECFLT_STATE_OVERFLOW)
    {
        *peState |= E_DRVTSP_FLT_STATE_OVERFLOW;
    }
    return TRUE;
}

static void _TSP_SecFlt_Init(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    static MS_U8    u8Mask[TSP_FILTER_DEPTH]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    static MS_U8    u8NMask[TSP_FILTER_DEPTH] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    HAL_TSP_SecFlt_ClrCtrl(u32EngId, u32SecFltId);

    HAL_TSP_SecFlt_SelSecBuf(u32EngId, u32SecFltId, TSP_SECFLT_BUF_NULL, FALSE);
    HAL_TSP_SecFlt_SetMatch(u32EngId, u32SecFltId, u8Mask);
    HAL_TSP_SecFlt_SetMask(u32EngId, u32SecFltId, u8NMask);
    //HAL_TSP_SecFlt_SetNMask(HAL_TSP_SECFLT(u32EngId, u32SecFltId)), u8Mask);
    _TSP_SecFlt_SetType(u32EngId, u32SecFltId, E_DRVTSP_FLT_TYPE_SECTION);
    HAL_TSP_SecFlt_SetReqCount(u32EngId, u32SecFltId, 0);
    HAL_TSP_SecFlt_ResetState(u32EngId, u32SecFltId);
    _ptsp_res->_TspState[0].SecFltEvtNotify[u32SecFltId] = E_DRVTSP_EVENT_DATA_INIT;
    _ptsp_res->_TspState[0].SecFltCallback[u32SecFltId]  = NULL;
}

// Section filter ID == PID filter ID, Set *pu32SecFltId == PIDFlterID
static MS_BOOL _TSP_SecFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32SecFltId)
{
    MS_U32 u32ii = 0;

    u32ii = *pu32SecFltId;

    if (u32ii < TSP_SECFLT_NUM && _ptsp_res->_TspState[u32EngId].SecFltState[u32ii] == E_DRVTSP_FLT_STATE_FREE)
    {
        _ptsp_res->_TspState[u32EngId].SecFltState[u32ii] = E_DRVTSP_FLT_STATE_ALLOC;
        _TSP_SecFlt_Init(u32EngId, u32ii);
        *pu32SecFltId = u32ii;
        _TSP_SetSecNotifyId(*pu32SecFltId); //for isr processing checking
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _TSP_SecFlt_Free(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_BOOL bResetState)
{
    HAL_TSP_SecFlt_ResetEmmIdx(u32EngId, u32SecFltId);
    HAL_TSP_SecFlt_SelSecBuf(0, u32SecFltId, TSP_SECFLT_BUF_NULL, FALSE);
    if (bResetState)
    {
        // clear all event while filter free
        MS_U32 u32Dummy = 0;
        _TSP_GetSecOvfId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecCrcId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecRdyId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetFltDisableId(u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);

        _TSP_RemoveSecNotifyId(u32SecFltId);
        _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId] = E_DRVTSP_FLT_STATE_FREE;
    }
    _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] = E_DRVTSP_EVENT_DATA_INIT;
    _ptsp_res->_TspState[u32EngId].s32TspEvtId[u32SecFltId] = -1;

    return TRUE;
}

static void _TSP_SecBuf_Init(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    HAL_TSP_SecBuf_SetBuffer(u32EngId, u32SecBufId, 0, 0);
    HAL_TSP_SecBuf_ResetBuffer(u32EngId, u32SecBufId);
}

// Section buffer ID == Section filter ID, Set *pu32SecBufId == SecFlterID
static MS_BOOL _TSP_SecBuf_Alloc(MS_U32 u32EngId, MS_U32 *pu32SecBufId)
{
    MS_U32     u32ii = *pu32SecBufId;

    if (u32ii < TSP_SECBUF_NUM && _ptsp_res->_TspState[u32EngId].SecBufState[u32ii] == E_DRVTSP_FLT_STATE_FREE)
    {
        goto SECBUF_ALC_END;
    }
    else
    {
        return FALSE;
    }

    *pu32SecBufId = 0xFFFFFFFFUL;

    for (u32ii = 0; u32ii < TSP_SECBUF_NUM; u32ii++)
    {
        if (_ptsp_res->_TspState[u32EngId].SecBufState[u32ii] == E_DRVTSP_FLT_STATE_FREE)
        {
            break;
        }
    }
    if (u32ii == TSP_SECBUF_NUM)
    {
        return FALSE;
    }

SECBUF_ALC_END:
    _ptsp_res->_TspState[u32EngId].SecBufState[u32ii] = E_DRVTSP_FLT_STATE_ALLOC;
    *pu32SecBufId = u32ii;
    return TRUE;
}

static void _TSP_RegStateStore(void)
{
    HAL_TSP_Int_Disable(TSP_HWINT_ALL);
    HAL_TSP_Int2_Disable(TSP_HWINT2_ALL);

    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();
    HAL_TSP_SaveFltState();
}

static void _TSP_RegStateRestore(void)
{
    HAL_TSP_RestoreFltState();
}

static MS_BOOL _TSP_SecBuf_Free(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    _ptsp_res->_TspState[u32EngId].SecBufState[u32SecBufId] = E_DRVTSP_FLT_STATE_FREE;
    return TRUE;
}

static void _TSP_RegStateReset(void)
{
    MS_U32 u32EngId;
    MS_U32 u32FilterId;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (u32FilterId = 0; u32FilterId < TSP_PIDFLT_NUM_ALL; u32FilterId++)
        {
            _TSP_PidFlt_Init(u32EngId, u32FilterId);
        }
        for (u32FilterId = 0; u32FilterId < TSP_SECFLT_NUM; u32FilterId++)
        {
            _TSP_SecBuf_Init(u32EngId, u32FilterId);
            //HAL_TSP_SecBuf_SetBuffer(u32EngId, u32FilterId, 0, 0);
            _TSP_SecFlt_Init(u32EngId, u32FilterId);
        }
    }
}

static TSP_Result _TSP_SelPad(MS_U32 u32EngId, DrvTSP_If eIf, DrvTSP_If_Set* pIfSet)
{
    if (u32EngId>= TSP_ENGINE_NUM)
    {
        return DRVTSP_FAIL;
    }

    if((eIf >= E_DRVTSP_IF_NUM) && (eIf != E_DRVTSP_IF_FI))
    {
        return DRVTSP_FAIL;
    }

    HAL_TSP_SelPad(u32EngId, (MS_U32)eIf, (MS_U32)pIfSet->ePad, pIfSet->bParallel);
    HAL_TSP_SelPad_ClkInv(u32EngId, (MS_U32)eIf, pIfSet->bClkInv);
    HAL_TSP_SelPad_ExtSync(u32EngId, pIfSet->bExtSync, (MS_U32)eIf);
    HAL_TSP_SelPad_Parl(u32EngId, pIfSet->bParallel, (MS_U32)eIf);

    #ifdef TS3_IF_SUPPORT
    if(pIfSet->bParallel == FALSE && pIfSet->ePad == E_DRVTSP_PAD_EXT_INPUT3)
    {
        HAL_TSP_TSOut_En(TRUE);
    }
    else
    {
        HAL_TSP_TSOut_En(FALSE);
    }
    #endif

    return DRVTSP_OK;
}

#if 0
static MS_BOOL _TSP_IsPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_U32 u32Flag = 0;

    _TSP_GetEventFreeDefer0Id(u32PidFltId, &u32Flag, E_AND);
    return ((u32Flag & (1 << (u32PidFltId & 0x1F))) ? TRUE: FALSE);
}

static void _TSP_SetPidFltFreeDefer(MS_U32 u32PidFltId)
{
    _TSP_SetEventFreeDefer0Id(u32PidFltId);
}

static void _TSP_ResetPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_U32 u32Flag = 0;

    _TSP_GetEventFreeDefer0Id(u32PidFltId, &u32Flag, E_OR_CLEAR);
}
#endif

#if 0
static MS_BOOL _TSP_IsAskPidFltFree(MS_U32 u32PidFltId)
{
    MS_U32 u32Flag = 0;

    _TSP_GetEventFree0Id(u32PidFltId, &u32Flag, E_AND);
    return ((u32Flag & (1 << (u32PidFltId & 0x1F))) ? TRUE: FALSE);
}

static void _TSP_AskPidFltFree(MS_U32 u32PidFltId)
{
    _TSP_SetEventFree0Id(u32PidFltId);
}

//Free Section PID filters, 32 filters for one group
//For 64 pid filters, there are 2 gropus
//For 128 pid filters, there are 4 groups
static MS_U32 _TSP_GetAskPidFltFree0List(MS_U32 u32Group)
{
    MS_U32 u32Flag = 0;

    _TSP_GetModeEventFree0(u32Group, 0xFFFFFFFF, &u32Flag, E_OR);

    return u32Flag;
}

//Free Section PID filters
//For 64 pid filters, there are 2 gropus
//For 128 pid filters, there are 4 groups
static void _TSP_ClrAskPidFltFree0List(MS_U32 u32Group, MS_U32 u32Mask)
{
    MS_U32 u32Flag;

    _TSP_GetModeEventFree0(u32Group, u32Mask, &u32Flag, E_OR_CLEAR);
}
#endif

//For pid filter and section filter are one by one mappig
static MS_BOOL _TSP_Flt_StateGet(MS_U32 u32EngId, MS_U32 u32FltId, DrvTSP_FltState *peState)
{
    DrvTSP_FltState     PidFltState;
    DrvTSP_FltState     SecFltState;

    if (!_TSP_PidFlt_StateGet(u32EngId, u32FltId, &PidFltState))
    {
         return FALSE;
    }
    *peState=   PidFltState;
    if (TSP_SECFLT_NUM<= u32FltId)
    {
         return TRUE;
    }
    if (!_TSP_SecFlt_StateGet(u32EngId, u32FltId, &SecFltState))
    {
         return FALSE;
    }
    *peState|=  SecFltState;
    return TRUE;
}

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
static MS_BOOL _TSP_PCR_Adjust(MS_U32 u32EngId)
{
    MS_U32 u32Pcr = 0, u32Pcr_H = 0, u32Diff = 0, u32STC = 0;

    HAL_TSP_HWPcr_Read(u32EngId, &u32Pcr, &u32Pcr_H);

#if 1
    if(_ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] >= 2)
    {
        if(_ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] == 3)
        {
            HAL_TSP_Stc_ctrl(u32EngId, STC_SYNTH_DEFAULT);
        }
        HAL_TSP_SetSTC(u32EngId, u32Pcr, u32Pcr_H);
        _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] = 0;
        _ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32EngId] = u32Pcr;
        _ptsp_res->TSPHwPcrParams._u32PrevPllControl[u32EngId] = HAL_TSP_GetSTCSynth(u32EngId);
    }

    HAL_TSP_STC_Update_Disable(u32EngId, TRUE);
    u32STC = HAL_TSP_GetSTC(u32EngId);
    HAL_TSP_STC_Update_Disable(u32EngId, FALSE);

    //ULOGD("TSP", "Pcr %08lx, STC %08lx \n", u32Pcr, u32STC);
    if((u32STC - _ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32EngId]) > 0x4000UL)
    {
        //ULOGD("TSP", "STC_DIFF = %08lx ==== \n\n",u32STC - _u32PrevStcBase[u32EngId]);
        _ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32EngId] = u32STC;

        if(_ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32EngId] > u32Pcr)
        {
            u32Diff = _ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32EngId] - u32Pcr;
            if( u32Diff  <= 0x02UL)
            {
                _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] = 0;
            }
            else if( u32Diff  < 0x1000UL)
            {
                _ptsp_res->TSPHwPcrParams._u32PrevPllControl[u32EngId] += ((u32Diff) >> 3UL);                      //now = error x alpha + prev
                HAL_TSP_Stc_ctrl(u32EngId, _ptsp_res->TSPHwPcrParams._u32PrevPllControl[u32EngId] + ((u32Diff)>> 6UL));        //out = now + error x beta
                _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] = 0;
                //ULOGD("TSP", "======== Set STC CONTROL <++> %08lx  PCR/Diff = %08lx/%08lx \n",
                //    _u32PrevPllControl[u32EngId] + ((_u32PrevStcBase[u32EngId] - u32Pcr)>>6),u32Pcr,_u32PrevStcBase[u32EngId]-u32Pcr);
            }
            else
            {
                _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId]++;
            }
        }
        else
        {
            u32Diff = u32Pcr - _ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32EngId] ;
            if(( u32Diff) <= 0x02UL)
            {
                _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] = 0;
            }
            else if((u32Diff) < 0x1000UL)
            {
                _ptsp_res->TSPHwPcrParams._u32PrevPllControl[u32EngId] -= ((u32Diff)>> 3UL);                //now = error x alpha + prev
                HAL_TSP_Stc_ctrl(u32EngId, _ptsp_res->TSPHwPcrParams._u32PrevPllControl[u32EngId] - ((u32Diff)>> 6UL));        //out = now + error x beta
                _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId] = 0;
                //ULOGD("TSP", "======== Set STC CONTROL <--> %08lx  PCR/Diff = %08lx/%08lx \n",
                //    _u32PrevPllControl[u32EngId] - ((u32Pcr - _u32PrevStcBase[u32EngId] )>>6),u32Pcr,u32Pcr-_u32PrevStcBase[u32EngId] );
            }
            else
            {
                _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32EngId]++;
            }
        }
    }
#else
    HAL_TSP_SetSTC(u32EngId, u32Pcr, u32Pcr_H);
#endif

    return TRUE;
}

#endif //HWPCR_ENABLE

//--------------------------------------------------------------------------------------------------
// Interrupt service routine of TSP
// Arguments:   None
// Return:      None
//--------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
//static irqreturn_t _TSP_Isr(int irq,void *dev_id)
static void _TSP_Isr(void)
#else
static void _TSP_Isr(void)
#endif
{
    MS_U32                 u32HwInt;
    MS_U32                 u32SwInt;
    MS_U32                 u32HwInt2;

    _ptsp_res->_u32DbgIntrCount++;
    //HAL_TSP_ISR_SAVE_ALL();

    _TSP_INT_LOCK();
    u32HwInt = HAL_TSP_HW_INT_STATUS();
    u32SwInt = HAL_TSP_SW_INT_STATUS();
    u32HwInt2 = HAL_TSP_HW_INT2_STATUS();
    _TSP_INT_UNLOCK();

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    if (u32HwInt2 & TSP_HWINT2_PCR0_UPDATE_END)
    {
        if(_TSP_ChkHwPcrNotifyId(0))
        {
            _TSP_PCR_Adjust(0);
        }
        else
        {
            u32HwInt2 &= ~TSP_HWINT2_PCR0_UPDATE_END;
        }
    }
    if (u32HwInt2 & TSP_HWINT2_PCR1_UPDATE_END)
    {
        if(_TSP_ChkHwPcrNotifyId(1))
        {
            _TSP_PCR_Adjust(1);
        }
        else
        {
            u32HwInt2 &= ~TSP_HWINT2_PCR1_UPDATE_END;
        }
    }
#endif

    //PVR using polling mode, so ignore PVR interrupts
    if (HAS_FLAG(u32HwInt, TSP_HWINT_HW_PVR1_MASK))
    {
        if(_TSP_ChkPvrNotifyId(0))
        {
            _TSP_SetEvent(_ptsp_res->_s32PvrEvtId[0], TSP_TASK_EVENT_PVR0_RDY_SELF|TSP_TASK_EVENT_PVR0_RDY_POLL|TSP_TASK_EVENT_PVR1_RDY_SELF|TSP_TASK_EVENT_PVR1_RDY_POLL);
        }
        else
        {
            u32HwInt &= ~TSP_HWINT_HW_PVR1_MASK;
        }
    }
    if (HAS_FLAG(u32HwInt2, (TSP_HWINT_HW_PVR2_MASK)))
    {
        if(_TSP_ChkPvrNotifyId(1))
        {
            _TSP_SetEvent(_ptsp_res->_s32PvrEvtId[1], TSP_TASK_EVENT_PVR2PVR0_RDY_SELF|TSP_TASK_EVENT_PVR2PVR0_RDY_POLL|TSP_TASK_EVENT_PVR2PVR1_RDY_SELF|TSP_TASK_EVENT_PVR2PVR1_RDY_POLL);
        }
        else
        {
            u32HwInt2 &= ~TSP_HWINT_HW_PVR2_MASK;
        }
    }

    #ifdef TS3_IF_SUPPORT
    if (HAS_FLAG(u32HwInt2, (TSP_HWINT_HW_PVRCB_MASK)))
    {
        if(_TSP_ChkPvrNotifyId(2))
        {
            _TSP_SetEvent(_ptsp_res->_s32PvrEvtId[2], TSP_TASK_EVENT_CBPVR0_RDY_SELF|TSP_TASK_EVENT_CBPVR0_RDY_POLL);
        }
        else
        {
            u32HwInt2 &= ~TSP_HWINT_HW_PVRCB_MASK;
        }
    }
    #endif

    if (u32SwInt & TSP_SWINT_CTRL_FIRE)
    {
        MS_U32          u32EngId;
        MS_U32          u32SecFltId;
        MS_U32          u32IntStat;
        //REG_SecFlt*     pSecFilter;

        u32EngId    = (u32SwInt & TSP_SWINT_INFO_ENG_MASK) >> TSP_SWINT_INFO_ENG_SHFT;
        u32IntStat  = (u32SwInt & TSP_SWINT_STATUS_CMD_MASK) >> TSP_SWINT_STATUS_CMD_SHFT;
        u32SecFltId = (u32SwInt & TSP_SWINT_INFO_SEC_MASK) >> TSP_SWINT_INFO_SEC_SHFT;
        //pSecFilter  = HAL_TSP_SECFLT(u32EngID, u32SecFltId);

#if DMX_DEBUG
        extern DMX_FLT_DBG_INFO    stDmxDbgInfo[];

        if(u32IntStat == TSP_SWINT_STATUS_SEC_RDY ||
           u32IntStat == TSP_SWINT_STATUS_REQ_RDY )
        {
            stDmxDbgInfo[u32SecFltId].u32Int_rdy ++;
        }
        else if(u32IntStat == TSP_SWINT_STATUS_BUF_OVFLOW)
        {
            stDmxDbgInfo[u32SecFltId].u32Int_ovf ++;
        }
        else if(u32IntStat == TSP_SWINT_STATUS_SEC_CRCERR)
        {
            stDmxDbgInfo[u32SecFltId].u32Int_crc ++;
        }
#endif

        if(_ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId] == E_DRVTSP_FLT_STATE_FREE)
            goto CLEAR_INT;

        if(_TSP_ChkSecNotifyId(u32SecFltId) == FALSE)
        {
            u32SwInt = 0;
            u32IntStat = 0;
            u32HwInt &= ~TSP_HWINT_TSP_SW_INT_STATUS;
        }
        else if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
        {
            _s32TspEventId = _ptsp_res->_TspState[u32EngId].s32TspEvtId[u32SecFltId];
        }

        switch (u32IntStat)
        {
            case TSP_SWINT_CMD_DMA_PAUSE:
                HAL_TSP_TsDma_Pause();
                break;

            case TSP_SWINT_CMD_DMA_RESUME:
                HAL_TSP_TsDma_Resume();
                break;
            case TSP_SWINT_STATUS_SEC_RDY:
            case TSP_SWINT_STATUS_REQ_RDY:
            case TSP_SWINT_STATUS_SEC_CRCERR:
                //printf("SEC %d\n", (int)u32SecFltId);
                if (E_DRVTSP_FLT_MODE_ONESHOT & _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId])
                {
                    _TSP_SetFltDisableId(u32SecFltId);
                }
                if ((TSP_SWINT_STATUS_SEC_CRCERR == u32IntStat) && (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId] & E_DRVTSP_FLT_MODE_CRCCHK))
                {
                    //printf("ERR %d\n", (int)u32SecFltId);
                    if (_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_SEC_CRCERR)
                    {
                        _TSP_SetSecCrcId(u32SecFltId);             // Set section event flag
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    //printf("SEC %d\n", (int)u32SecFltId);
                    _TSP_SetSecRdyId(u32SecFltId);             // Set section event flag
                }

                if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_POLL)
                {
                    _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_SECTION_POLL);                      // Trigger task
                }
                else if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_SELF)
                {
                    _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_SECTION_SELF);                      // Trigger task
#ifdef MSOS_TYPE_LINUX_KERNEL
                    wake_up_interruptible(&(tsp_wq_head[u32SecFltId]));
#endif
                }
                break;

            case TSP_SWINT_STATUS_BUF_OVFLOW:
                //printf("OVF %d\n", (int)u32SecFltId);
                _TSP_SetFltDisableId(u32SecFltId);
                _TSP_SetSecOvfId(u32SecFltId);             // Set section event flag
                if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_POLL)
                {
                    _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_SECTION_POLL);                      // Trigger task
                }
                else if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_SELF)
                {
                    _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_SECTION_SELF);                      // Trigger task
#ifdef MSOS_TYPE_LINUX_KERNEL
                wake_up_interruptible(&(tsp_wq_head[u32SecFltId]));
#endif

                }
                break;

            case TSP_SWINT_STATUS_PKT_OVRUN:
                // Enable debug message when PCR filter can be handle by hardware.
                // TSP_DBG("Packet Overrun %d\n", u32EngId);
                break;

            case TSP_SWINT_STATUS_SEC_ERROR:
                // [ISR print]
                // TSP_DBG("SEC error %d-0x%02X\n", u32EngId, u32SecFltId);
                break;

            case TSP_SWINT_STATUS_DEBUG:
                // [ISR print]
                // TSP_DBG("DEBUG %d-0x%02X\n", u32EngId, u32SecFltId);
                if(_FwDbgInfo.bEnable)
                {
                    _TSP_SetEvent(_s32TspEventId, TSP_TASK_EVENT_FWMSG);
#ifdef MSOS_TYPE_LINUX_KERNEL
                wake_up_interruptible(&(tsp_wq_head[u32SecFltId]));
#endif

                }
                break;

            default:
                // [ISR print]
                // DBG_TSP(TSP_DBG("Unknown TSP interrupt %d\n", u32EngId));
                break;
        }
    }

CLEAR_INT:

    _TSP_INT_LOCK();
    //If it still has interrupt status, it should be processed by another process again, do not clear interrupt status
    if(u32SwInt)
    {
        HAL_TSP_Int_ClearSw();
    }
    if(u32HwInt)
    {
        HAL_TSP_Int_ClearHw(u32HwInt);
    }
    if(u32HwInt2)
    {
        HAL_TSP_Int_ClearHw2(u32HwInt2);
    }
    _TSP_INT_UNLOCK();

#ifdef MSOS_TYPE_LINUX_KERNEL
    //return IRQ_HANDLED;
#endif

    OSAL_TSP_IntEnable();
}

void MDrv_TSP_SecFlt_Update(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    _TSP_SetSecRdyId(u32SecFltId);
}

static MS_BOOL _TSP_Isr_Proc_SecRdyCrc(MS_U32 u32EngId, MS_U32 u32FltId, MS_U32 u32CbOpt, MS_U32* pu32Evt)
{
    MS_BOOL bData = 0;
    MS_U32 u32Event;
    MS_U32 u32Events;
    MS_U32 u32EvtNotify;
    static DrvTSP_Msg msg;
    MS_PHY phyRead;
    MS_U32 u32PIDFlt;
    P_DrvTSP_EvtCallback pfEvtCallback = NULL;

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        _TSP_GetEvent(_s32TspEventId, u32Events, TSP_TASK_EVENT_SECTION_POLL, OSAL_EVENT_OR_CLEAR, 0);
    }

    _TSP_IND_LOCK();

    //using section filter ID to find the mapping pid filter id
    for(u32PIDFlt = 0; u32PIDFlt < TSP_SECFLT_NUM; u32PIDFlt++)
    {
        if(_ptsp_res->_TspState[u32EngId].FltMap[u32PIDFlt] == u32FltId)
            break;
    }

    if(u32PIDFlt >= TSP_SECFLT_NUM)
    {
        _TSP_IND_UNLOCK();
        return FALSE;
    }

    if ((E_DRVTSP_FLT_MODE_ONESHOT & _ptsp_res->_TspState[u32EngId].SecFltMode[u32FltId]) == 0)
    {
        if (!(_ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] & E_DRVTSP_FLT_STATE_ENABLE))
        {
            // TSP_DBG("[%s][%d] %d 0x%08x\n", __FUNCTION__, __LINE__,
            //(int)u32FltId, (int)_ptsp_res->_TspState[u32EngId].FltState[u32FltId]);
            _TSP_IND_UNLOCK();
            return FALSE;
        }
    }

    if (_TSP_GetFltDisableId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR))
    {
        //No need to disable filter here or Overflow callback will miss.
        _TSP_PidFlt_Disable(u32EngId, u32PIDFlt);  // critical section
        //_TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
        //_TSP_GetSecOvfId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
    }

    phyRead = HAL_TSP_SecBuf_GetBufRead(u32EngId, u32FltId);
    bData = (phyRead != HAL_TSP_SecBuf_GetBufWrite(u32EngId, u32FltId))? TRUE: FALSE; // critical section

    if (bData)
    {
        if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
        {
            *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_READY;
        }
        else
        {
            u32EvtNotify  = _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32FltId];
            pfEvtCallback = _ptsp_res->_TspState[u32EngId].SecFltCallback[u32FltId];
            if ((u32EvtNotify & E_DRVTSP_EVENT_DATA_READY) && pfEvtCallback)
            {
                if(_ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] & E_DRVTSP_FLT_STATE_FREEING)
                {
                    //ULOGD("TSP", "[%s][%d]Warning!! Filter %u is closing %x\n", __FUNCTION__, __LINE__, (unsigned int)u32PIDFlt, (unsigned int)stDrvState);
                    _TSP_IND_UNLOCK();
                    MsOS_DelayTask(1);
                    return FALSE;
                }
                else
                {
                    _ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] |= E_DRVTSP_FLT_STATE_CBRUN;
                }

                msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32FltId <<MSG_FLTINFO_SEC_ID_SHFT);
                _u32TaskLine = __LINE__;
                _TSP_IND_UNLOCK();
                pfEvtCallback(E_DRVTSP_EVENT_DATA_READY, &msg);
                _TSP_IND_LOCK();
                _u32TaskLine = __LINE__;

                _ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] &= ~E_DRVTSP_FLT_STATE_CBRUN;
                if (phyRead == HAL_TSP_SecBuf_GetBufRead(u32EngId, u32FltId))
                {
                    _TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
                }
            }
        }
    }
    /*else
    {
        _TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
    }*/

    //Clear evets
    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        _TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
    }

    _TSP_IND_UNLOCK();

    if (bData)
    {
        return TRUE;
    }

//jump_bad_state:
    return FALSE;
}

static MS_BOOL _TSP_Isr_Proc_SecCrcErr(MS_U32 u32EngId, MS_U32 u32FltId, MS_U32 u32CbOpt, MS_U32* pu32Evt)
{
    MS_U32 u32Events;
    MS_U32 u32EvtNotify;
    P_DrvTSP_EvtCallback pfEvtCallback = NULL;
    static DrvTSP_Msg msg;
    MS_U32 u32PIDFlt;

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        _TSP_GetEvent(_s32TspEventId, u32Events, TSP_TASK_EVENT_SECTION_POLL, OSAL_EVENT_OR_CLEAR, 0);
    }

    _TSP_IND_LOCK();

    //using section filter ID to find the mapping pid filter id
    for(u32PIDFlt = 0; u32PIDFlt < TSP_SECFLT_NUM; u32PIDFlt++)
    {
        if(_ptsp_res->_TspState[u32EngId].FltMap[u32PIDFlt] == u32FltId)
            break;
    }
    if(u32PIDFlt == TSP_SECFLT_NUM)
    {
        _TSP_IND_UNLOCK();
        return FALSE;
    }

    if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32FltId] & E_DRVTSP_EVENT_SEC_CRCERR) == 0)
    {
        _TSP_IND_UNLOCK();
        return FALSE;
    }

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        *pu32Evt = (MS_U32)E_DRVTSP_EVENT_SEC_CRCERR;
    }
    else
    {
        u32EvtNotify  = _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32FltId];
        pfEvtCallback = _ptsp_res->_TspState[u32EngId].SecFltCallback[u32FltId];
        if ((u32EvtNotify & E_DRVTSP_EVENT_SEC_CRCERR) && pfEvtCallback)
        {
            msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32FltId <<MSG_FLTINFO_SEC_ID_SHFT);
            //_TSP_SECFLT_UNLOCK(u32EngId, u32SecFltId);
            _u32TaskLine = __LINE__;
            _TSP_IND_UNLOCK();
            pfEvtCallback(E_DRVTSP_EVENT_SEC_CRCERR, &msg);
            _u32TaskLine = __LINE__;
            return TRUE;
        }
    }

    _TSP_IND_UNLOCK();

    return TRUE;
}

static void _TSP_Isr_Proc_Pvr(MS_U32 u32CbOpt)
{
    MS_U32 u32Events;
    static DrvTSP_Msg   msg;
    MS_U8 u8ii;

    for(u8ii = 0; u8ii < TSP_PVR_IF_NUM; u8ii++)
    {
        if ((_ptsp_res->_PvrEvtNotify[u8ii] & E_DRVTSP_EVENT_CB_MASK) == u32CbOpt)
        {
            break;
        }
    }

    if(u8ii == TSP_PVR_IF_NUM)
    {
        return;
    }

    if (u32CbOpt == E_DRVTSP_EVENT_CB_POLL)
    {
        _TSP_GetEvent(_s32TspEventId, u32Events, TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL|
                                 TSP_TASK_EVENT_CBPVR0_RDY_POLL |
                                 TSP_TASK_EVENT_PVR2PVR0_RDY_POLL | TSP_TASK_EVENT_PVR2PVR1_RDY_POLL,
                                 OSAL_EVENT_OR_CLEAR, 0);
    }
    else
    {
        _TSP_GetEvent(_s32TspEventId, u32Events, TSP_TASK_EVENT_PVR_SELF |
                                 TSP_TASK_EVENT_CBPVR0_RDY_SELF |
                                 TSP_TASK_EVENT_PVR2PVR0_RDY_SELF | TSP_TASK_EVENT_PVR2PVR1_RDY_SELF
                                 , OSAL_EVENT_OR_CLEAR, 0);
    }

    if(_ptsp_res->_PvrEvtNotify[0] && _ptsp_res->_PvrCallback[0])
    {
        switch (u32Events & (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL | TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF))
        {
            case TSP_TASK_EVENT_PVR0_RDY_SELF:
            case TSP_TASK_EVENT_PVR0_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x0 << MSG_PVRBUF_ID_SHFT) | (0 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[0](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                break;
            case TSP_TASK_EVENT_PVR1_RDY_SELF:
            case TSP_TASK_EVENT_PVR1_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x1 << MSG_PVRBUF_ID_SHFT) | (0 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[0](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                break;
            case (TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF):
            case (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL):
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                {
                    msg.PvrBufId = (MSG_PVRBUF_ID_NULL << MSG_PVRBUF_ID_SHFT) | (0 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[0](E_DRVTSP_EVENT_PVRBUF_OVERFLOW, &msg);
                }
                break;
            default:
                break;
        }
    }

    #if (TSP_PVR_IF_NUM > 1)
    if(_ptsp_res->_PvrEvtNotify[1] && _ptsp_res->_PvrCallback[1])
    {
        switch (u32Events & (TSP_TASK_EVENT_PVR2PVR0_RDY_POLL | TSP_TASK_EVENT_PVR2PVR1_RDY_POLL | TSP_TASK_EVENT_PVR2PVR0_RDY_SELF | TSP_TASK_EVENT_PVR2PVR1_RDY_SELF))
        {
            case TSP_TASK_EVENT_PVR2PVR0_RDY_SELF:
            case TSP_TASK_EVENT_PVR2PVR0_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x0 << MSG_PVRBUF_ID_SHFT) | (1 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[1](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                break;
            case TSP_TASK_EVENT_PVR2PVR1_RDY_SELF:
            case TSP_TASK_EVENT_PVR2PVR1_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x1 << MSG_PVRBUF_ID_SHFT) | (1 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[1](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                break;
            case (TSP_TASK_EVENT_PVR2PVR0_RDY_SELF | TSP_TASK_EVENT_PVR2PVR1_RDY_SELF):
            case (TSP_TASK_EVENT_PVR2PVR0_RDY_POLL | TSP_TASK_EVENT_PVR2PVR1_RDY_POLL):
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                {
                    msg.PvrBufId = (MSG_PVRBUF_ID_NULL << MSG_PVRBUF_ID_SHFT) | (1 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[1](E_DRVTSP_EVENT_PVRBUF_OVERFLOW, &msg);
                }
                break;
            default:
                break;
        }
    }
    #endif

    #ifdef TS3_IF_SUPPORT
    if(_ptsp_res->_PvrEvtNotify[2] && _ptsp_res->_PvrCallback[2])
    {
        switch (u32Events & (TSP_TASK_EVENT_CBPVR0_RDY_SELF | TSP_TASK_EVENT_CBPVR0_RDY_POLL))
        {
            case TSP_TASK_EVENT_CBPVR0_RDY_SELF:
            case TSP_TASK_EVENT_CBPVR0_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[2], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x0 << MSG_PVRBUF_ID_SHFT) | (2 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[2](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[2], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                {
                    msg.PvrBufId = (MSG_PVRBUF_ID_NULL << MSG_PVRBUF_ID_SHFT) | (2 << MSG_PVR_ID_SHFT);
                    _ptsp_res->_PvrCallback[2](E_DRVTSP_EVENT_PVRBUF_OVERFLOW, &msg);
                }
                break;
            default:
                break;
        }
    }
    #endif
}

static MS_BOOL _TSP_Isr_Proc_SecOvf(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32CbOpt, MS_U32* pu32Evt)
{
    static DrvTSP_Msg       msg;
    MS_U32                  u32PidFltId;
    MS_U32                  u32EvtNotify, u32Events = 0;
    P_DrvTSP_EvtCallback    pfEvtCallback = NULL;

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

    // disable overflow filter
    _TSP_IND_LOCK();

    for (u32PidFltId = 0; u32PidFltId < TSP_SECFLT_NUM; u32PidFltId++)
    {
        if (u32SecFltId == _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId])
        {
            break;
        }
    }

    if(u32PidFltId >= TSP_SECFLT_NUM)
    {
        _TSP_IND_UNLOCK();
        return FALSE;
    }

    if (_TSP_GetFltDisableId(u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR))
    {
        if (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] & E_DRVTSP_FLT_STATE_ENABLE)
        {
            _TSP_PidFlt_Disable(u32EngId, u32PidFltId);  // critical section
        }
    }

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        *pu32Evt = (MS_U32)E_DRVTSP_EVENT_BUF_OVERFLOW;

        //----  if overflow, clear evets to wait restarting ------------
        _TSP_GetSecOvfId    (u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecCrcId    (u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecRdyId    (u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
        //-------------------------------------------------------------------------
    }
    else
    {
        if (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] == E_DRVTSP_FLT_STATE_FREE)
        {
            goto PROC_END;
        }

        u32EvtNotify  = _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId];
        pfEvtCallback = _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecFltId];
        if ((u32EvtNotify & E_DRVTSP_EVENT_BUF_OVERFLOW) && pfEvtCallback)
        {
            if(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] & E_DRVTSP_FLT_STATE_FREEING)
            {
                //ULOGD("TSP", "[%s][%d]Warning!! Filter %u is closing %x\n", __FUNCTION__, __LINE__, (unsigned int)u32PidFltId, (unsigned int)stDrvState);
                _TSP_IND_UNLOCK();
                MsOS_DelayTask(1);
                return TRUE;
            }
            else
            {
                _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] |= E_DRVTSP_FLT_STATE_CBRUN;
            }

            msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32SecFltId <<MSG_FLTINFO_SEC_ID_SHFT);
            _u32TaskLine = __LINE__;
            _TSP_IND_UNLOCK();
            pfEvtCallback(E_DRVTSP_EVENT_BUF_OVERFLOW, &msg);
            _TSP_IND_LOCK();
            _u32TaskLine = __LINE__;
            _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] &= ~E_DRVTSP_FLT_STATE_CBRUN;

        }
    }

PROC_END:

    _TSP_IND_UNLOCK();

    return TRUE;
}

static void _TSP_Isr_ProcSelf(OSAL_TSP_ProcTypeParam* pData)
{
    TSP_Result*         pRet = (TSP_Result*)pData->pRet;
    MS_U32              u32Events = 0, u32SecRdy = 0, u32SecOvf = 0, u32SecCrc = 0;//, u32SecFree = 0;
    MS_U32              i = 0, j = 0;
    MS_U32              u32Evt = 0;
    //MS_U32              u32PidFltId;
    MS_U32              u32GroupNum = (TSP_SECFLT_NUM+31)>>5;
    //MS_U32              u32Mask;
    //MS_U32              u32AskFreeFltList;

#if 0
    MS_U32              u32TempEvent = 0;

    _TSP_GetEvent(u32TempEvent, TSP_TASK_EVENT_FWMSG, OSAL_EVENT_OR_CLEAR, 0);
    if(u32TempEvent & (TSP_TASK_EVENT_FWMSG))
    {
        _DumpFwMsg();
    }
#endif

    _u32TaskLine = __LINE__;

    if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
    {
#ifdef DMX_UTOPIA_2K
        u32Events = pData->u32PrcEvt;
#endif //DMX_UTOPIA_2K
    }
    else
    {
        _TSP_GetEvent(_s32TspEventId, u32Events, TSP_TASK_EVENT_GROUP_SELF|TSP_TASK_EVENT_FLT_FREE, OSAL_EVENT_OR_CLEAR, MSOS_WAIT_FOREVER);
        *pRet = DRVTSP_FAIL;
    }

    if(u32Events & (TSP_TASK_EVENT_PVR_SELF|TSP_TASK_EVENT_PVR_POLL))
    {
        _TSP_Isr_Proc_Pvr(E_DRVTSP_EVENT_CB_SELF);
    }

    if (u32Events & TSP_TASK_EVENT_SECTION_SELF)
    {
        for (i = 0; i < TSP_ENGINE_NUM; i++)
        {
            MS_U32                  u32SecFltId;

            for (j = 0; j < u32GroupNum; j++)
            {
                _TSP_GetModeSecOvf(j, 0xFFFFFFFFUL, &u32SecOvf, OSAL_EVENT_OR_CLEAR);
                _TSP_GetModeSecRdy(j, 0xFFFFFFFFUL, &u32SecRdy, OSAL_EVENT_OR_CLEAR);
                _TSP_GetModeSecCrc(j, 0xFFFFFFFFUL, &u32SecCrc, OSAL_EVENT_OR_CLEAR);

                //printf("u32SecOvf %x, u32SecRdy %x, u32SecCrc %x\n", (int)u32SecOvf, (int)u32SecRdy, (int)u32SecCrc);

                while (u32SecRdy)
                {
                    u32SecFltId = MAsm_CPU_GetTrailOne(u32SecRdy) + (j << 5UL);  //[TODO] balance every filter priority
                    //[NOTE] it may be already balance for long term running
                    *pRet = (_TSP_Isr_Proc_SecRdyCrc(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                    u32SecRdy &= ~(1UL << (u32SecFltId & 0x1FUL));
                }

                while (u32SecCrc)
                {
                    u32SecFltId = MAsm_CPU_GetTrailOne(u32SecCrc) + (j << 5);  //[TODO] balance every filter priority
                                                                                //[NOTE] it may be already balance for long term running
                    *pRet = (_TSP_Isr_Proc_SecCrcErr(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                    u32SecCrc &= ~(1UL << (u32SecFltId & 0x1FUL));
                }

                while(u32SecOvf)
                {
                    u32SecFltId = MAsm_CPU_GetTrailOne(u32SecOvf) + (j << 5UL);  //[TODO] balance every filter priority
                    //[NOTE] it may be already balance for long term running
                    *pRet = (_TSP_Isr_Proc_SecOvf(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                    u32SecOvf &= ~(1UL << (u32SecFltId & 0x1FUL));
                }
            }
        }
    }

    #if 0
    if (u32Events & TSP_TASK_EVENT_FLT_FREE)
    {
        for (i = 0; i < TSP_ENGINE_NUM; i++)
        {
            for (j = 0; j < u32GroupNum; j++)
            {
                u32AskFreeFltList = _TSP_GetAskPidFltFree0List(j);
                u32Mask = u32AskFreeFltList;

                if(u32Mask)
                {
                    _TSP_IND_LOCK();
                }

                while(u32AskFreeFltList)
                {
                    u32PidFltId = MAsm_CPU_GetTrailOne(u32AskFreeFltList) + (j << 5UL);

                    if (_ptsp_res->_TspState[0].FltState[u32PidFltId] != E_DRVTSP_FLT_STATE_FREE)
                    {
                        //section filter id is the same as pid filter id
                        MS_U32 u32SecFltId;

                        //u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(0, u32PidFltId);
                        u32SecFltId = _ptsp_res->_TspState[0].FltMap[u32PidFltId];
                        _TSP_GetSecRdyId(u32SecFltId, &u32SecRdy, OSAL_EVENT_OR_CLEAR);
                        _TSP_GetSecOvfId(u32SecFltId, &u32SecOvf, OSAL_EVENT_OR_CLEAR);
                        _TSP_GetSecCrcId(u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
                        _TSP_SecBuf_Free(0, u32SecFltId);
                        _TSP_SecFlt_Free(0, u32SecFltId, TRUE);
                        _TSP_PidFlt_Free(0, u32PidFltId, FALSE);
                        //ULOGD("TSP", "_TSP_PidFlt_Free %ld , time %ld\n", u32PidFltId, MsOS_GetSystemTime());
                    }
                    _TSP_FLAG_SetDone(u32PidFltId);
                    u32AskFreeFltList &= (~(1UL<<(u32PidFltId & 0x1FUL)));
                }

                if(u32Mask)
                {
                    _TSP_ClrAskPidFltFree0List(j, u32Mask);
                    _TSP_IND_UNLOCK();
                }

            }
        }
    }
    #endif

}

static void _TSP_Isr_ProcPoll(OSAL_TSP_ProcTypeParam* pData, MS_U32* pu32Evt)
{
    MS_U32              u32Events = 0, u32SecOvf = 0, u32SecCrc = 0;
    MS_U32              i = 0;
    MS_U32              u32FltId = pData->u32FltId;
    TSP_Result*         pRet = (TSP_Result*)pData->pRet;
    MS_BOOL             bRes;
    MS_U32              u32FwEvent = 0;

    _TSP_GetEvent(_s32TspEventId, u32FwEvent, TSP_TASK_EVENT_FWMSG, OSAL_EVENT_OR_CLEAR, 0);
    if(u32FwEvent & (TSP_TASK_EVENT_FWMSG))
    {
        _DumpFwMsg();
    }
    _TSP_GetEvent(_s32TspEventId, u32Events, TSP_TASK_EVENT_GROUP_POLL, OSAL_EVENT_OR, 0);

    *pRet = DRVTSP_FAIL;

    if(u32Events & (TSP_TASK_EVENT_PVR_SELF|TSP_TASK_EVENT_PVR_POLL))
    {
        _TSP_Isr_Proc_Pvr(E_DRVTSP_EVENT_CB_POLL);
        return;
    }

    for (i = 0; i < TSP_ENGINE_NUM; i++)
    {
        MS_U32 u32SecFltId;

        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(0, u32FltId);

        *pRet = (_TSP_Isr_Proc_SecRdyCrc(i, u32SecFltId, E_DRVTSP_EVENT_CB_POLL, pu32Evt))? DRVTSP_OK: DRVTSP_FAIL;
        if (DRVTSP_FAIL == *pRet)
        {
            bRes = _TSP_GetSecCrcId(u32SecFltId, &u32SecCrc, OSAL_EVENT_OR);

            if (bRes)
            {
                *pRet = (_TSP_Isr_Proc_SecCrcErr(i, u32SecFltId, E_DRVTSP_EVENT_CB_POLL, pu32Evt))? DRVTSP_OK: DRVTSP_FAIL;
            }
        }
        if (DRVTSP_FAIL == *pRet)
        {
            bRes = _TSP_GetSecOvfId(u32SecFltId, &u32SecOvf, OSAL_EVENT_OR);

            if (bRes)
            {
                *pRet = (_TSP_Isr_Proc_SecOvf(i, u32SecFltId, E_DRVTSP_EVENT_CB_POLL, pu32Evt))? DRVTSP_OK: DRVTSP_FAIL;
            }
        }
    }
}

static void _TSP_Isr_Proc_Task(OSAL_TSP_ProcTypeParam* pDummy, MS_U32* pu32Dummy)
{
    TSP_Result             Ret;
    OSAL_TSP_ProcTypeParam Data;

    Data.u32FltId = 0xFFFFFFFFUL;
    Data.pRet = (void*)&Ret;
    Data.bInfiniteWait = TRUE;
    Data.bFromTask = TRUE;
#ifdef DMX_UTOPIA_2K
    Data.u32PrcEvt = 0;
#endif

    while (1)
    {
        _TSP_EXT_LOCK();
        if (_db_flag == 1)
        {
            _db_flag = 2;
            //_TSP_EXT_UNLOCK();
            break;
        }
        _TSP_EXT_UNLOCK();
        _TSP_Isr_ProcSelf(&Data);
    } // Task while loop

    _s32TspTaskId = -1;
    _TSP_EXT_UNLOCK();
    //printf("TSP ISR Proc End!!!\n");
}

static char tsp_mutex[] = "TSP_Mutex";
static char tsp_ind_mutex[] = "TSP_Ind_Mutex";
static char tsp_int_mutex[] = "TSP_Int_Mutex";
static char tsp_ext_mutex[] = "TSP_Ext_Mutex";
static char tsp_event[] = "TSP_Event";

static char tsp_secRdy[] = "TSP_SecRdy";
static char tsp_crcerr[] = "TSP_CrcErr";
static char tsp_secovf[] = "TSP_SecOvf";
static char tsp_fltdis[] = "TSP_FltDis";
//static char freelist0[] = "TSP_FreeList0";
//static char tsp_done[] = "TSP_Done";

//static char freelistdefer0[] = "TSP_FreeListDefer0";
static char tsp_isr_task[] = "TSP_ISR_Task";

static TSP_Result _TSP_Init(MS_BOOL bResetSwState, MS_BOOL bRestoreRegState)
{
    MS_VIRT virtBank = 0, virtBank1 = 0;
    MS_PHY  u32BankSize = 0;
    MS_PHY  u32BankSize1 = 0;
    MS_BOOL bAlive;
    MS_U32  u32FwSize = 0;
    MS_PHY  phyFwAddr = 0;
    MS_U8   u8ii = 0, u8temp = 0;

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_TSP))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
        MS_ASSERT(0);
    }
    if (FALSE == MDrv_MMIO_GetBASE(&virtBank1, &u32BankSize1, MS_MODULE_PM))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
        MS_ASSERT(0);
    }

    HAL_TSP_SetBank(virtBank, virtBank1);

    for(u8ii = 0; u8ii < ((MS_U8)TSP_PVR_IF_NUM); u8ii++)
    {
        _ptsp_res->_PvrEvtNotify[u8ii] = E_DRVTSP_EVENT_DATA_INIT;
        _ptsp_res->_PvrCallback[u8ii] = NULL;
    }

    HAL_TSP_STC64_Mode_En(TRUE);

    bAlive = HAL_TSP_Alive();
    if (!bAlive)
    {
        HAL_TSP_PowerCtrl(ENABLE);
        HAL_TSP_Reset(0);
        // Diable interrupt
        HAL_TSP_Int_Disable(TSP_HWINT_ALL);
        HAL_TSP_Int2_Disable(TSP_HWINT2_ALL);
    }
    else if(bResetSwState == FALSE)
    {
        HAL_ResetAll();
        MsOS_DelayTask(5);
    }

    // IntDisable
    // IntDetach
    OSAL_TSP_IntDisable();
//#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntDetach();
//#endif

    if (!bAlive)
    {
        // Disable CPU
        HAL_TSP_SetCtrlMode(0, 0x0, TSP_IF_NUM);  // Disable TSP0(Reset CPU), Software_Reset
        HAL_TSP_HW_Lock_Init();

        // Richard: Enable indirect access
        HAL_TSP_Ind_Enable();
        _TSP_CmdQ_Init();
        HAL_TSP_WbDmaEnable(TRUE);

        // The below is setting hw_config0, (ts interface 0), how about hw_config3, (ts_interface 1)
        // Mark : Byte enable setting, fix HW bug. (Important)
        HAL_TSP_HwPatch();
    }

    if (bResetSwState)
    {
        _TSP_SwStateReset(FALSE);
    }

    if (!bAlive)
    {
        if (bRestoreRegState == 0)
        {
            _TSP_RegStateReset();
        }

        if (_TSP_GetFirmware(&phyFwAddr, &u32FwSize) == FALSE)
        {
            MS_CRITICAL_MSG(TSP_DBG ("TSP init fail: Please set FW address and size\n"));
            return DRVTSP_FAIL;
        }

        //TSP_DBG("[%s][%d]Reset CMDQ\n",__FUNCTION__,__LINE__);
        if(!HAL_TSP_CmdQ_Reset())
        {
            return DRVTSP_FAIL;
        }

        #ifdef SECURE_PVR_ENABLE   //load fwtsp.dat from Tee
        _ptsp_res->_stSecBuf.u32BufId = 0;
        _ptsp_res->_stSecBuf.u32BufOpt = 0;
        _ptsp_res->_stSecBuf.u32BufSize = u32FwSize;
        _ptsp_res->_stSecBuf.phyBufAddr = phyFwAddr;
        if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
        {
            return DRVTSP_FAIL;
        }
        #endif

        HAL_TSP_CPU_SetBase(phyFwAddr, u32FwSize);
        // Synthesizer Tuning
        // For difference : 100 - 4000 STC
        // Step: 10 STC
        // HAL_TSP_SetFwMsg(0x03A4); // Catch up n*10 STC in 1/20 sec

        HAL_TSP_SetCtrlMode(0, TSP_CTRL_SW_RST, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU

        // TSP_DBG ("**************************DDD TSP protect miu00000000000000000\n");
        // TSP write protect to prevent TSP write memory
        HAL_TSP_OrzWriteProtect_Enable(TRUE, 0, 0);

        HAL_Reset_WB();

        HAL_TSP_SetCtrlMode(0, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU

        HAL_TSP_Int_ClearSw();
        HAL_TSP_Int_ClearHw(TSP_HWINT_ALL);
        HAL_TSP_Int_ClearHw2(TSP_HWINT2_ALL);

        while (FALSE == HAL_TSP_Alive())
        {
            MS_CRITICAL_MSG(TSP_DBG("Reset TSP CPU fail\n"));
            HAL_TSP_SetCtrlMode(0, 0, TSP_IF_NUM);

            #ifdef SECURE_PVR_ENABLE   //load fwtsp.dat from Tee
            if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
            {
                return DRVTSP_FAIL;
            }
            #endif

            HAL_TSP_CPU_SetBase(phyFwAddr, u32FwSize);
            HAL_TSP_SetCtrlMode(0, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU
        }

        while(FALSE == HAL_TSP_Check_FW_VER());

        #ifdef SECURE_PVR_ENABLE
        _ptsp_res->_stSecBuf.u32BufId = 0xFF;
        _ptsp_res->_stSecBuf.u32BufOpt = 0;
        _ptsp_res->_stSecBuf.u32BufSize = 0;
        _ptsp_res->_stSecBuf.phyBufAddr = 0;
        if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
        {
            return DRVTSP_FAIL;
        }
        #endif

        HAL_TSP_Int_ClearSw();
        HAL_TSP_Int_ClearHw(TSP_HWINT_ALL);
        HAL_TSP_Int_ClearHw2(TSP_HWINT2_ALL);

        // Implicity MDrv_TSP_Reset()
        HAL_TSP_TsDma_SetDelay(0x0AUL);                                          // Set TsDma delay, //jyliu.tsp, how come the value

        _FwDbgInfo.bEnable         = FALSE;
        _FwDbgInfo.phyBufAddr      = 0;
        _FwDbgInfo.u32Size         = 0x0UL;
        _FwDbgInfo.phyReadPos      = 0x0UL;
        _FwDbgInfo.phyWritePos     = 0x0UL;
    }

    // STC control by TSP need indirect R/W enable
    for (u8ii = 0; u8ii < ((MS_U8)TSP_PCRFLT_NUM); u8ii++)
    {
        HAL_TSP_Stc_ctrl(u8ii, STC_SYNTH_DEFAULT);
    }

    // Enable SYNC_RISING_DETECT and VALID_FALLING_DETECT for Serial mode
    HAL_TSP_Enable_ValidSync_Dectect();

    // Disable TSIF1
    HAL_TSP_PVR_Enable(0, FALSE);
    HAL_TSP_PVR_Enable(1, FALSE);
    HAL_TSP_PVR_Enable(2, FALSE);

    OSAL_TSP_IntAttach(_TSP_Isr);
//#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntEnable();
//#endif

    if (!bAlive)
    {
        HAL_TSP_Int_Enable(TSP_HWINT_TSP_SW_INT_STATUS);                                   // Enable TSP hardware interrupt
        //HAL_TSP_Int2_Enable(TSP_HWINT2_ALL);
        #if 0 //(defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 0))
        for (u8ii = 0; u8ii < TSP_PCRFLT_NUM; u8ii++)
        {
            HAL_TSP_HWPcr_Int_Enable(u8ii, TRUE);
        }
        #endif
    }

    if(bResetSwState)
    {
        for (u8ii = 0; u8ii < ((MS_U8)TSP_PVR_IF_NUM); u8ii++)
        {
            _ptsp_res->_PvrEvtNotify[u8ii] = E_DRVTSP_EVENT_DATA_INIT;
            _ptsp_res->_PvrCallback[u8ii] = NULL;
        }
        u8temp = (MS_U8)((TSP_PVR_IF_NUM+31UL)>>5UL);
        for (u8ii = 0; u8ii < u8temp; u8ii++)
        {
            _u32PvrNotifyId[u8ii] = 0;
        }
    }

    if(((_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN) && (_ptsp_res->_TspState[0].TSPStatus == E_DRVTSP_SW_RESUME)) == FALSE)
    {
        _ptsp_res->_s32TspMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_mutex);
        _ptsp_res->_s32IndMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_ind_mutex);
        _ptsp_res->_s32IntRegMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_int_mutex);
        _s32ExtMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_ext_mutex);
        //printf("[%s][%d] tsp_ext_mutex %x \n", __FUNCTION__, __LINE__, (int)tsp_ext_mutex);

        if(_u32LibMode != DRVTSP_CMD_LIB_MODE_KRN)
        {
            _s32TspEventId = OSAL_TSP_EventCreate((MS_U8*)tsp_event);
        }

        u8temp = (MS_U8)((TSP_SECFLT_NUM+31UL)>>5UL);
        for (u8ii = 0; u8ii < u8temp; u8ii++)
        {
            _s32TspSecRdyId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_secRdy);
            _s32TspSecOvfId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_crcerr);
            _s32TspSecCrcId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_secovf);
            _s32TspFltDisableId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_fltdis);

            if(bResetSwState)
            {
                _u32SecNotifyId[u8ii] = 0;
            }
        }
    }

    #if 0
    u8temp = ((TSP_PIDFLT_NUM_ALL+31UL)>>5UL);
    for (u8ii = 0; u8ii < u8temp; u8ii++)
    {
        _s32EventFree0[u8ii] = OSAL_TSP_EventCreate((MS_U8*)freelist0);
        _s32DoneFlags[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_done);
        //_s32EventFreeDefer0[u8ii] = OSAL_TSP_EventCreate((MS_U8*)freelistdefer1);
    }
    #endif

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    if(bResetSwState)
    {
        u8temp = (MS_U8)((TSP_PCRFLT_NUM+31UL)>>5UL);
        for (u8ii = 0; u8ii < u8temp; u8ii++)
        {
            _u32HwPcrNotifyId[u8ii] = 0;
        }
    }
#endif

    if(_u32LibMode != DRVTSP_CMD_LIB_MODE_KRN)
    {
        _s32TspTaskId = OSAL_TSP_TaskCreate(_TSP_Isr_Proc_Task, (MS_U8*)tsp_isr_task);
        //printf("[%s][%d] _u32LibMode %d , _db_flag %d _s32TspTaskId %x \n", __FUNCTION__, __LINE__, (int)_u32LibMode, (int)_db_flag, (int)_s32TspTaskId);
    }

    if(bResetSwState)
    {
        _ptsp_res->_bSWInitReady = TRUE;
        _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_INIT;
        _ptsp_res->_u32LastErr = DRVTSP_OK;
        _ptsp_res->_u32ProcessNum = 1UL;
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32  u32ii;

    for (u32ii= 0; u32ii< TSP_SECFLT_NUM; u32ii++)
    {
        init_waitqueue_head(&tsp_wq_head[u32ii]);
    }
    for (u32ii= 0; u32ii< DRVTSP_MAX_PROCESS_NUM; u32ii++)
    {
        _stKModePrcEvtTbl[u32ii].tgid = 0;
        _stKModePrcEvtTbl[u32ii].s32TspEvtId = -1;
    }
#endif

    if (bRestoreRegState)
    {
        if((_ptsp_res->_phyTspFWDramBuf > 0) && (_ptsp_res->_u32TspFwDramBufSize > 0))
        {
            HAL_TSP_OrzWriteProtect_Enable(TRUE, _ptsp_res->_phyTspFWDramBuf, _ptsp_res->_u32TspFwDramBufSize);
            HAL_TSP_SecFlt_SetDataAddr(_ptsp_res->_phyTspFWDramBuf);
        }
        _TSP_RegStateRestore();
    }

    return (TSP_Result)(_ptsp_res->_u32LastErr);
}

//-------------------------------------------------------------------------------------------------
/// Poweroff TSP driver and TSP engine
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PowerOff(void)
{
    _TSP_ENTRY();
    _TSP_Close();
    _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_POWEROFF;
    // this is not a bug. Intend to lock to mutex to prevent alive task accessing TSP after power off
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Relsase TSP indirect register read/write semaphone
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_HW_Lock_Release(void)
{
    HAL_TSP_HW_Lock_Release();

    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Close TSP driver and reset software/register state
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Exit(void)
{
    MS_U8 i;

   _TSP_ENTRY();

    //checking if stiil other process exist, only close sw thread
    if(_ptsp_res->_u32ProcessNum > 1)
    {
        _TSP_SW_Exit();
        _TSP_RETURN(DRVTSP_OK);
    }

    for (i = 0; i < ((MS_U8)TSP_ENGINE_NUM); i++)
    {
        _ptsp_res->_TspState[i].TSPStatus = E_DRVTSP_SW_EXIT;
    }

    if(HAL_TSP_Alive())
    {
        _TSP_SwStateReset(TRUE);
    }
    else
    {
        _TSP_SwStateReset(FALSE);
    }
    _TSP_RegStateReset();
    _TSP_Close();

    // this is not a bug. Intend to lock to mutex to prevent alive task accessing TSP after power off
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Suspend TSP driver
/// @return TSP_Result
/// @note
/// Save TSP driver states to DRAM
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Suspend(void)
{
    MS_U32 u32EngId, u32FilterId, u32SecFltId, u32Flag;
    MS_U16 u16Clk;

    if(!((_ptsp_res->_TspState[0].TSPStatus == E_DRVTSP_SW_INIT) || (_ptsp_res->_TspState[0].TSPStatus == E_DRVTSP_SW_RESUME)))
        return DRVTSP_FAIL;

    _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_SUSPEND;

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
    {
        _ptsp_res->_bKerStrSuspend = TRUE;
        HAL_TSP_CMD_Run(HAL_CMD_SET_KRSTR_MODE, 1, 0, 0);
        HAL_TSP_SaveRegs();
    }
#endif

    MDrv_TSP_GetTSIFStatus(E_DRVTSP_IF_TS0, &_ptsp_res->_ifset_save, &u16Clk);

    _TSP_STR_ENTRY();

    //Stop filter before storing filter
    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (u32FilterId = 0; u32FilterId < TSP_PIDFLT_NUM_ALL; u32FilterId++)
        {
            if((_ptsp_res->_TspState[u32EngId].FltState[u32FilterId] != E_DRVTSP_FLT_STATE_NA) &&
                (_ptsp_res->_TspState[u32EngId].FltState[u32FilterId] & E_DRVTSP_FLT_STATE_ENABLE))
            {
                HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32FilterId, TSP_PIDFLT_OUT_NONE);
                //printf("[%s][%d] disable flt %d , sec %x, %x \n", __FUNCTION__, __LINE__, (int)u32FilterId,
                //    (int)_ptsp_res->_TspState[u32EngId].FltMap[u32FilterId], (int)_ptsp_res->_TspState[u32EngId].FltType[u32FilterId]);

                RESET_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32FilterId], E_DRVTSP_FLT_STATE_ENABLE);
                SET_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32FilterId], E_DRVTSP_FLT_STATE_SUSPEND);

                //clear event
                u32SecFltId = _ptsp_res->_TspState[u32EngId].FltMap[u32FilterId];
                if(u32SecFltId < TSP_SECFLT_NUM)
                {
                    _TSP_GetSecOvfId    (u32SecFltId, &u32Flag, OSAL_EVENT_OR_CLEAR);
                    _TSP_GetSecCrcId    (u32SecFltId, &u32Flag, OSAL_EVENT_OR_CLEAR);
                    _TSP_GetSecRdyId    (u32SecFltId, &u32Flag, OSAL_EVENT_OR_CLEAR);
                    _TSP_GetFltDisableId(u32SecFltId, &u32Flag, OSAL_EVENT_OR_CLEAR);
                }
            }
        }
    }

    _TSP_RegStateStore();
    _TSP_Close();

    // this is not a bug. Intend to lock to mutex to prevent alive task accessing TSP after power off
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Resume TSP driver
/// @return TSP_Result
/// @note
/// Restore TSP driver states from DRAM
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
    {
        HAL_TSP_CMD_Run(HAL_CMD_SET_KRSTR_MODE, 2, 0, 0);
    }
#endif

    // Reset TSP register
    //HAL_ResetAll();
    //MsOS_DelayTask(5);

    _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_RESUME;

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
    {
        MS_U8 *pu8tr = 0;

        pu8tr = (MS_U8*)MS_PA2KSEG1(phyFWAddr);
        memcpy(pu8tr, u8TSPFirmware, sizeof(u8TSPFirmware));
    }
#endif

    _ptsp_res->_phyFwAddr = phyFWAddr;
    _ptsp_res->_u32FwSize = u32FWSize;
    //printf("[%s][%d] TSPStatus %x \n", __FUNCTION__, __LINE__, (int)_ptsp_res->_TspState[0].TSPStatus);

    if(_TSP_Init(FALSE, TRUE) != DRVTSP_OK)
    {
        return DRVTSP_FAIL;
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
    {
        HAL_TSP_RestoreRegs();
    }
#endif

    if(MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_TS0, 0) != DRVTSP_OK)
    {
        return DRVTSP_FAIL;
    }

    _TSP_STR_ENTRY();

    if(_TSP_SelPad(0, E_DRVTSP_IF_TS0, &_ptsp_res->_ifset_save) != DRVTSP_OK)
    {
        _TSP_STR_RETURN(DRVTSP_FAIL);
    }

    //_ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_INIT;

    _TSP_STR_RETURN(DRVTSP_OK);

}

TSP_Result MDrv_TSP_Resume_Filter(void)
{
    MS_U32 u32EngId, u32FilterId;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        for (u32FilterId = 0; u32FilterId < TSP_PIDFLT_NUM_ALL; u32FilterId++)
        {
            if((_ptsp_res->_TspState[u32EngId].FltState[u32FilterId] != E_DRVTSP_FLT_STATE_NA) &&
                (_ptsp_res->_TspState[u32EngId].FltState[u32FilterId] & E_DRVTSP_FLT_STATE_SUSPEND))
            {
                RESET_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32FilterId], E_DRVTSP_FLT_STATE_SUSPEND);
                //printf("[%s][%d] enable flt %d, sec %x,  %x\n", __FUNCTION__, __LINE__, (int)u32FilterId,
                //    (int)_ptsp_res->_TspState[u32EngId].FltMap[u32FilterId], (int)_ptsp_res->_TspState[u32EngId].FltType[u32FilterId]);
                MDrv_TSP_PidFlt_Enable(u32EngId, u32FilterId, TRUE);
            }
        }
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_u32LibMode == DRVTSP_CMD_LIB_MODE_KRN)
    {
        HAL_TSP_CMD_Run(HAL_CMD_SET_KRSTR_MODE, 0, 0, 0);
        _ptsp_res->_bKerStrSuspend = FALSE;
    }
#endif

    return DRVTSP_OK;
}

// Misc API
//-------------------------------------------------------------------------------------------------
/// Set TSP engine operating mode
/// @param  u32EngId                     \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eCtrlMode                    \b IN: TSP engine operating mode
/// @param  u8DscmbEngSel            \b IN: DSCMB Engine ID. IF 0xFF, not use DSCMB engine
/// @return TSP_Result
/// @note
/// Only one engine can be set to use memory stream input mode, and all kinds
/// of input are mutually exclusive for each engine.\n
/// Only one engine can output to MAD. The TSP engine which is not selected to
/// MAD can not filter any audio stream, it will block the stream.
/// @sa MDrv_TSP_PidFlt_Alloc, MDrv_TSP_PidFlt_Enable, E_DRVTSP_FLT_TYPE_AUDIO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetOperateMode(MS_U32 u32EngId, DrvTSP_CtrlMode eCtrlMode, MS_U8 u8DscmbEngSel)      // Set TSP internal operate
{
    MS_U8 u8ScmSrcTsif = 0, u8OrgCaEng = 0;
    MS_U32 u32ScmDest = 0;
    MS_U32 u32OrgDest = 0;
    MS_BOOL bClearOrgPath = FALSE;
    MS_U8 u8OrgSrc = 0;
    DrvTSP_FltType eFltType = 0;

    _TSP_STR_ENTRY();

    if (u32EngId >= TSP_ENGINE_NUM)
    {
        _TSP_STR_RETURN(DRVTSP_FAIL);
    }

    // reset audio BD mode
    HAL_TSP_AU_BD_Mode_Enable(FALSE);

    switch (eCtrlMode)
    {
        case E_DRVTSP_CTRL_MODE_TS0:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case E_DRVTSP_CTRL_MODE_TS1:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS1 | E_DRVTSP_FLT_SOURCE_TYPE_FILE1;
            break;
        case E_DRVTSP_CTRL_MODE_TS2:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS2 | E_DRVTSP_FLT_SOURCE_TYPE_FILE2;
            break;
        #ifdef MERGE_STR_SUPPORT
        case E_DRVTSP_CTRL_MODE_TSFI_LIVE:
        #endif
        case E_DRVTSP_CTRL_MODE_MEM:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        default:
            break;
    }

    u8OrgCaEng = _TSP_Get_DscmbEngId_ByFltSrc(eFltType);
    if(u8OrgCaEng != u8DscmbEngSel)
    {
        if(eFltType != 0)
        {
            if(u8DscmbEngSel < TSP_CA_ENGINE_NUM)
            {
                _ptsp_res->_DscmbFlt_State[u8DscmbEngSel].u32Src |= eFltType;
            }
            if(u8OrgCaEng < TSP_CA_ENGINE_NUM)
            {
                _ptsp_res->_DscmbFlt_State[u8OrgCaEng].u32Src &= ~eFltType;
                HAL_TSP_CSA_Get_CACtrl(u8OrgCaEng,&u8OrgSrc,&u32OrgDest);

                //do not clear ca control, if the ca control is not set for original source
                if((eCtrlMode == E_DRVTSP_CTRL_MODE_TS0) && (u8OrgSrc == TSP_SRC_FROM_TSIF0_LIVE) && (u32OrgDest & TSP_PKTDMX0_LIVE))
                    bClearOrgPath = TRUE;
                else if((eCtrlMode == E_DRVTSP_CTRL_MODE_MEM) && (u8OrgSrc == TSP_SRC_FROM_TSIF0_FILE) && (u32OrgDest & TSP_PKTDMX0_FILE))
                    bClearOrgPath = TRUE;
                else if((eCtrlMode == E_DRVTSP_CTRL_MODE_TS1) && (u8OrgSrc == TSP_SRC_FROM_TSIF1) && (u32OrgDest & TSP_PKTDMX1))
                    bClearOrgPath = TRUE;
                else if((eCtrlMode == E_DRVTSP_CTRL_MODE_TS2) && (u8OrgSrc == TSP_SRC_FROM_TSIF2) && (u32OrgDest & TSP_PKTDMX2))
                    bClearOrgPath = TRUE;

                if(bClearOrgPath)
                {
                    HAL_TSP_CSA_Set_ScrmPath(u8OrgCaEng, 0);
                }
            }
        }
    }

    //printf("[%s] eCtrlMode %d , u8DscmbEngSel %d\n", __FUNCTION__, (int)eCtrlMode, (int)u8DscmbEngSel);

    switch (eCtrlMode)
    {
        case E_DRVTSP_CTRL_MODE_TS0:
            HAL_TSP_PS_Path_Disable();
            HAL_TSP_CSA_Set_CACtrl(u8DscmbEngSel, TSP_SRC_FROM_TSIF0_LIVE, TSP_PKTDMX0_LIVE);
            HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);
            break;

#if (TSP_IF_NUM >= 1)
        case E_DRVTSP_CTRL_MODE_TS1:
            HAL_TSP_PS_Path_Disable();
            HAL_TSP_CSA_Set_CACtrl(u8DscmbEngSel, TSP_SRC_FROM_TSIF1, TSP_PKTDMX1);
            HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 1);
            break;

        case E_DRVTSP_CTRL_MODE_TS2:
            HAL_TSP_PS_Path_Disable();
            HAL_TSP_CSA_Set_CACtrl(u8DscmbEngSel, TSP_SRC_FROM_TSIF2, TSP_PKTDMX2);
            HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 2);
            break;

        case E_DRVTSP_CTRL_MODE_TS3:
            HAL_TSP_PS_Path_Disable();
            HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 3);
            break;
        #ifdef MERGE_STR_SUPPORT
        case E_DRVTSP_CTRL_MODE_TSFI_LIVE:
            HAL_TSP_PS_Path_Disable();
            HAL_TSP_CSA_Set_CACtrl(u8DscmbEngSel, TSP_SRC_FROM_TSIF0_FILE, TSP_PKTDMX0_FILE);
            HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 3);
            break;
        #endif
#endif

        case E_DRVTSP_CTRL_MODE_MEM:
            HAL_TSP_PS_Path_Disable();
            HAL_TSP_CSA_Get_CACtrl(u8DscmbEngSel, &u8ScmSrcTsif, &u32ScmDest);
            if(HAL_TSP_PVR_StartingEngs_Get(u32ScmDest) == 0)
            {
                HAL_TSP_CSA_Set_CACtrl(u8DscmbEngSel, TSP_SRC_FROM_TSIF0_FILE, TSP_PKTDMX0_FILE);
            }
            HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST| TSP_CTRL_TSFILE_EN, TSP_IF_NUM);
            break;

        case E_DRVTSP_CTRL_MODE_PVR_CA:
        case E_DRVTSP_CTRL_MODE_PVR1_CA:
            break;

        default:
            _TSP_STR_RETURN(DRVTSP_FAIL);
            break;
    }

    _TSP_STR_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Select ChipTop PAD for ts0 and ts1
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eIf                        \b IN: TSP TS interface
/// @param  pIfSet                   \b IN: TS interface setting
/// @return TSP_Result
/// @note
/// Uranus: only can select pad0/pad1 to ts0
/// Oberon: can select pad0/pad1 to ts0, pad0/pad1 to ts1.
///         Default setting was pad0->ts0, pad1->ts1.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SelPad(MS_U32 u32EngId, DrvTSP_If eIf, DrvTSP_If_Set* pIfSet)
{
    TSP_Result bRet;

    _TSP_ENTRY();
    bRet = _TSP_SelPad(u32EngId, eIf, pIfSet);

    _TSP_RETURN(bRet);
}

//-------------------------------------------------------------------------------------------------
/// Configure output pad
/// @param  eOutputPad                        \b IN: Output pad & Output pad mode
/// @param  eInputPad                          \b IN: Source of Input pad
/// @param  bInParallel                          \b IN: TRUE: Input pad mode is parallel; FALSE: Input pad mode is serial
/// @return TSP_Result
/// @note
/// Only EXT_PAD1 and EXT_PAD3 has output mode
///
/// If output from S2P mode, input source should be external input pad only
/// If output from TSO mode, eInputPad should be E_DRVTSP_PAD_TSO
/// If input source is internal demod, output mode can be demod out or TSO out
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_OutputPadCfg(DrvTSP_PadOut eOutputPad, DrvTSP_PadIn eInputPad, MS_BOOL bInParallel, MS_BOOL bEnable)
{
#if (TSP_TS_PAD_NUM > 1UL)
    MS_U32 u32InPad;
    MS_U32 u32OutPad = (eOutputPad & ~E_DRVTSP_OUTPAD_MODE_MASK);
    MS_U32 u32OutPadMode = eOutputPad & E_DRVTSP_OUTPAD_MODE_MASK;

    //ULOGD("TSP", "[%s][%d] eOutputPad %lx, eInputPad %x, bInParallel %d, bEnable %d\n", __FUNCTION__, __LINE__, eOutputPad, (int)eInputPad, (int)bInParallel, (int)bEnable);

    if(!((u32OutPad == E_DRVTSP_OUTPAD_PAD1) || (u32OutPad == E_DRVTSP_OUTPAD_PAD3)))
    {
        return DRVTSP_FAIL;
    }

    if(bEnable == FALSE)
    {
        u32InPad = TSP_MUX_NONE;
    }
    else
    {
        switch(eInputPad)
        {
            case E_DRVTSP_PAD_EXT_INPUT0:
                u32InPad = TSP_MUX_TS0;
                break;
            case E_DRVTSP_PAD_EXT_INPUT1:
                u32InPad = TSP_MUX_TS1;
                break;
            case E_DRVTSP_PAD_EXT_INPUT2:
                u32InPad = TSP_MUX_TS2;
                break;
            #if defined(TSP_MUX_TS3)
            case E_DRVTSP_PAD_EXT_INPUT3:
                u32InPad = TSP_MUX_TS3;
                break;
            #endif
            #if defined(TSP_MUX_TS4)
            case E_DRVTSP_PAD_EXT_INPUT4:
                u32InPad = TSP_MUX_TS4;
                break;
            #endif
            #if defined(TSP_MUX_TS5)
            case E_DRVTSP_PAD_EXT_INPUT5:
                u32InPad = TSP_MUX_TS5;
                break;
            #endif
            case E_DRVTSP_PAD_DEMOD:
                u32InPad = TSP_MUX_INDEMOD;
                break;
            case E_DRVTSP_PAD_TSO:
                u32InPad = TSP_MUX_TSO;
                break;
            default:
                u32InPad = TSP_MUX_NONE;
                break;
        }
    }

    _TSP_ENTRY();

    if(HAL_TSP_TsOutPadCfg(u32OutPad, u32OutPadMode, u32InPad, bInParallel) == FALSE)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    else
    {
        _TSP_RETURN(DRVTSP_OK);
    }

#else
    return DRVTSP_FAIL;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set TS out pad clock phase
/// @param  u16OutPad                          \b IN: Reserved for future using
/// @param  u16Val                               \b IN: Clock phase tuning value
/// @param  bEnable                              \b IN: TRUE:Enable Clock phase tuning; FALSE: Disable Clock phase tuning
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_OutputClkPhase(MS_U16 u16OutPad, MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt)
{

#if (defined(TSO_20_ENABLE) && (TSP_TS_PAD_NUM > 1))

    _TSP_ENTRY();

    if(HAL_TSP_TsOuOutClockPhase(u16OutPad, u16Val, bEnable, u32S2pOpt) == TRUE)
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
#else
        return DRVTSP_FAIL;
#endif

}

//-------------------------------------------------------------------------------------------------
// Inverting bit order of TS parallel input
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  eIf                     \b IN: TSP TS interface
// @param  bInvert                 \b IN: bit order invert [1:invert 0:not invert]
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Parl_BitOrderSwap(MS_U32 u32EngId, DrvTSP_If eIf, MS_BOOL bInvert)
{
    MS_U32 u32Flow = 0;
    _TSP_ENTRY();
    if (u32EngId>= TSP_ENGINE_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    switch (eIf)
    {
        case E_DRVTSP_IF_TS0:
            u32Flow = 0;
            break;
        case E_DRVTSP_IF_TS1:
            u32Flow = 1;
            break;
        case E_DRVTSP_IF_TS2:
            u32Flow = 2;
            break;
        case E_DRVTSP_IF_TS3:
            u32Flow = 3;
            break;
        default:
            MS_ASSERT(0);
            _TSP_RETURN(DRVTSP_FAIL);
    }
    HAL_TSP_Parl_BitOrderSwap(u32EngId, u32Flow, bInvert);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset audio/video fifo or not
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_AVFifo_Reset(DrvTSP_FltType eFilterType, MS_BOOL bFlush)
{
    _TSP_ENTRY();
    switch (eFilterType)
    {
        case E_DRVTSP_FLT_TYPE_VIDEO:
            HAL_TSP_Flush_AV_FIFO(0, bFlush);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO:
            HAL_TSP_Flush_AV_FIFO(1, bFlush);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO2:
            HAL_TSP_Flush_AV_FIFO(2, bFlush);
            break;
        case E_DRVTSP_FLT_TYPE_VIDEO3D:
            HAL_TSP_Flush_AV_FIFO(3, bFlush);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO3:
            HAL_TSP_Flush_AV_FIFO(4, bFlush);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO4:
            HAL_TSP_Flush_AV_FIFO(5, bFlush);
            break;
        default:
            _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_AVFifo_Status(DrvTSP_FltType eFilterType, MS_U32 *u32FifoLevel)
{
    *u32FifoLevel = 0xFFFFFFFFUL;

    _TSP_ENTRY();
    switch (eFilterType)
    {
        case E_DRVTSP_FLT_TYPE_VIDEO:
            *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(0);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO:
            *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(1);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO2:
            *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(2);
            break;
        case E_DRVTSP_FLT_TYPE_VIDEO3D:
            *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(3);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO3:
            *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(4);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO4:
            *u32FifoLevel = HAL_TSP_Get_AVFifoLevel(5);
            break;
        default:
            _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_AVFifo_BlockEnable(DrvTSP_FltType eFilterType, MS_BOOL bEnable)
{
    MS_BOOL bAVFIFO_BLK_Disable = !bEnable;
    MS_U32  u32Fifo = 0;

    _TSP_ENTRY();
    switch (eFilterType)
    {
        case E_DRVTSP_FLT_TYPE_VIDEO:
            u32Fifo = TSP_FIFO_VD;
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO:
            u32Fifo = TSP_FIFO_AU;
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO2:
            u32Fifo = TSP_FIFO_AUB;
            break;
        case E_DRVTSP_FLT_TYPE_VIDEO3D:
            u32Fifo = TSP_FIFO_V3D;
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO3:
            u32Fifo = TSP_FIFO_AUC;
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO4:
            u32Fifo = TSP_FIFO_AUD;
            break;
        default:
            _TSP_RETURN(DRVTSP_FAIL);
    }

    HAL_TSP_AVFIFO_Block_Disable(u32Fifo, bAVFIFO_BLK_Disable);

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_AVFifo_SourceSelect(DrvTSP_PKTDMXSrcType ePKTSrc, MS_U32 u32FifoType)
{
    _TSP_ENTRY();
    HAL_TSP_AVFIFO_Src_Select(u32FifoType, ePKTSrc);

    if((ePKTSrc == E_DRVTSP_PKTSRC_DEMUXFILE) || (ePKTSrc == E_DRVTSP_PKTSRC_DEMUXMMFI0) ||
        (ePKTSrc == E_DRVTSP_PKTSRC_DEMUXMMFI1))
    {
        HAL_TSP_AVFIFO_Block_Disable(u32FifoType, FALSE);
    }
    else
    {
        HAL_TSP_AVFIFO_Block_Disable(u32FifoType, TRUE);
    }
    _TSP_RETURN(DRVTSP_OK);
}
//-------------------------------------------------------------------------------------------------
/// Get Last General Error Return Code
/// @return TSP_Result
/// @note
/// Last error return code is reset after calling this function.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetLastErr(void)
{
    TSP_Result ret;
    ret = (TSP_Result)(_ptsp_res->_u32LastErr);
    _ptsp_res->_u32LastErr = DRVTSP_OK;
    return ret;
}

//-------------------------------------------------------------------------------------------------
/// Get TSP Flt states
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  peState                 \b OUT: Flter status
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Flt_GetState(MS_U32 u32EngId, MS_U32 u32FltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_SECFLT_NUM> u32FltId, TSP_DBG("[TSP_ERROR][%06d] Bad FltId %d\n", __LINE__, (unsigned int)u32FltId));
    if (_TSP_Flt_StateGet(0, u32FltId, peState))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
/// Get TSP Flt states
/// @param  u32EngId                   \b IN: index of CA engine [ 0 .. (@ref DSCMB_ENGINE_NUM-1) ]
/// @param  pu32StartId               \b OUT: pointer to store dscmb filter start ID
/// @param  pu32EndId                 \b OUT: pointer to store dscmb filter end ID
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetDscmbFltId_Range(MS_U32 u32EngId, MS_U32 *pu32StartId, MS_U32 *pu32EndId)
{
    MS_U8 u8ii, u8ScmSrcTsif;
    MS_U32 u32ScmDest, u32FltSrc;

    *pu32StartId = 0;
    *pu32EndId = 0;

    _TSP_ENTRY();

    HAL_TSP_CSA_Get_CACtrl(u32EngId, &u8ScmSrcTsif, &u32ScmDest);

    switch(u8ScmSrcTsif)
    {
        case TSP_SRC_FROM_PKTDMX0:
            u32FltSrc = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case TSP_SRC_FROM_PKTDMXFL:
            u32FltSrc = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case TSP_SRC_FROM_PKTDMX1:
            u32FltSrc = E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case TSP_SRC_FROM_PKTDMX2:
            u32FltSrc = E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        default:
            _TSP_RETURN(DRVTSP_FAIL);
    }

    for(u8ii = 0; u8ii < TSP_CA_ENGINE_NUM; u8ii++)
    {
        if(_ptsp_res->_DscmbFlt_State[u8ii].u32Src & u32FltSrc)
        {

            *pu32StartId = _ptsp_res->_DscmbFlt_State[u8ii].u32Start;
            *pu32EndId = _ptsp_res->_DscmbFlt_State[u8ii].u32End;
            _TSP_RETURN(DRVTSP_OK);
        }
    }

    _TSP_RETURN(DRVTSP_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Get Dscmbler engine id depended on input source
/// @param  eInputSrc                  \b IN: Input source
/// @param  pu32EngId                 \b OUT: pointer to store dscmb engine ID
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetDscmbEngIdx_BySource(DrvTSP_PKTDMXSrcType eInputSrc, MS_U32* pu32EngId)
{
    DrvTSP_FltType eFltType = 0;
    *pu32EngId = 0xFF;

    switch (eInputSrc)
    {
        case E_DRVTSP_PKTSRC_DEMUX0:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case E_DRVTSP_PKTSRC_DEMUX1:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS1 | E_DRVTSP_FLT_SOURCE_TYPE_FILE1;
            break;
        case E_DRVTSP_PKTSRC_DEMUX2:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS2 | E_DRVTSP_FLT_SOURCE_TYPE_FILE2;
            break;
        case E_DRVTSP_PKTSRC_DEMUXFILE:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        default:
            return DRVTSP_FAIL;
    }

    _TSP_ENTRY();
    *pu32EngId = ((MS_U32)_TSP_Get_DscmbEngId_ByFltSrc(eFltType)) & 0xFF;
    _TSP_RETURN(DRVTSP_OK);

}

//-------------------------------------------------------------------------------------------------
/// Set/Get Dscmb engine control path
/// @param  u32EngId                   \b IN: index of CA engine [ 0 .. (@ref DSCMB_ENGINE_NUM-1) ]
/// @param  peInputSrc                 \b IN/OUT: Pointer to store Dscmb Engine input source
/// @param  bSet                         \b IN: Get or set dscmb eingine path
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Dscmb_Source(MS_U32 u32EngId, DrvTSP_PKTDMXSrcType* peInputSrc, MS_BOOL bSet)
{
    TSP_ASSERT(u32EngId < TSP_CA_ENGINE_NUM, TSP_DBG("[TSP_ERROR][%06d] Bad Dscmb Id %d\n", __LINE__, (int)u32EngId));

    _TSP_ENTRY();

    if(bSet == FALSE)
    {
        *peInputSrc = 0;
        if(_ptsp_res->_DscmbFlt_State[u32EngId].u32Src & E_DRVTSP_FLT_SOURCE_TYPE_LIVE)
        {
            *peInputSrc = E_DRVTSP_PKTSRC_DEMUX0;
        }
        else if(_ptsp_res->_DscmbFlt_State[u32EngId].u32Src & E_DRVTSP_FLT_SOURCE_TYPE_TS1)
        {
            *peInputSrc = E_DRVTSP_PKTSRC_DEMUX1;
        }
        else if(_ptsp_res->_DscmbFlt_State[u32EngId].u32Src & E_DRVTSP_FLT_SOURCE_TYPE_TS2)
        {
            *peInputSrc = E_DRVTSP_PKTSRC_DEMUX2;
        }
        else if(_ptsp_res->_DscmbFlt_State[u32EngId].u32Src & E_DRVTSP_FLT_SOURCE_TYPE_FILE)
        {
            *peInputSrc = E_DRVTSP_PKTSRC_DEMUXFILE;
        }
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        MS_U8 u8OrgCaEng = 0xFF, u8CaSrc = 0;
        DrvTSP_FltType eFltType = 0;
        MS_U32 u32CaDes = 0;

        switch (*peInputSrc)
        {
            case E_DRVTSP_PKTSRC_DEMUX0:
                eFltType = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
                u8CaSrc = TSP_SRC_FROM_TSIF0_LIVE;
                u32CaDes = TSP_PKTDMX0_LIVE;
                break;
            case E_DRVTSP_PKTSRC_DEMUX1:
                eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS1 | E_DRVTSP_FLT_SOURCE_TYPE_FILE1;
                u8CaSrc = TSP_SRC_FROM_TSIF1;
                u32CaDes = TSP_PKTDMX1;
                break;
            case E_DRVTSP_PKTSRC_DEMUX2:
                eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS2 | E_DRVTSP_FLT_SOURCE_TYPE_FILE2;
                u8CaSrc = TSP_SRC_FROM_TSIF2;
                u32CaDes = TSP_PKTDMX2;
                break;
            case E_DRVTSP_PKTSRC_DEMUXFILE:
                eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
                u8CaSrc = TSP_SRC_FROM_TSIF0_FILE;
                u32CaDes = TSP_PKTDMX0_FILE;
                break;
            default:
                _TSP_RETURN(DRVTSP_FAIL);
        }

        _ptsp_res->_DscmbFlt_State[u32EngId].u32Src = eFltType;
        HAL_TSP_CSA_Set_CACtrl((MS_U8)u32EngId, u8CaSrc, u32CaDes);

        u8OrgCaEng = _TSP_Get_DscmbEngId_ByFltSrc(eFltType);
        if((u8OrgCaEng < TSP_CA_ENGINE_NUM) && (u8OrgCaEng != (MS_U8)u32EngId))
        {
            _ptsp_res->_DscmbFlt_State[u8OrgCaEng].u32Src &= ~eFltType;
            HAL_TSP_CSA_Set_ScrmPath(u8OrgCaEng, 0);
        }

        _TSP_RETURN(DRVTSP_OK);
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_RemoveDupAVPkt(MS_BOOL bEnable)
{
    _TSP_ENTRY();
    HAL_TSP_RemoveDupAVPkt(bEnable);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  flttype                 \b IN: FIFO type
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_RemoveDupAVFifoPkt(DrvTSP_FltType flttype, MS_BOOL bEnable)
{
    MS_U32 u32FifoIdx = 0;

    _TSP_ENTRY();

    if(flttype == E_DRVTSP_FLT_TYPE_VIDEO)
    {
        u32FifoIdx = 0;
    }
    else if(flttype == E_DRVTSP_FLT_TYPE_AUDIO)
    {
        u32FifoIdx = 1;
    }
    else if(flttype == E_DRVTSP_FLT_TYPE_AUDIO2)
    {
        u32FifoIdx = 2;
    }
    else if(flttype == E_DRVTSP_FLT_TYPE_VIDEO3D)
    {
        u32FifoIdx = 3;
    }
#if (TSP_AUDIO3_AUDIO4_SUPPORT == 1)
    else if(flttype == E_DRVTSP_FLT_TYPE_AUDIO3)
    {
        u32FifoIdx = 4;
    }
    else if(flttype == E_DRVTSP_FLT_TYPE_AUDIO4)
    {
        u32FifoIdx = 5;
    }
#endif
    else
    {
        _TSP_RETURN(DRVTSP_INVALID_PARAM);
    }

    HAL_TSP_RemoveDupAVFifoPkt(u32FifoIdx, bEnable);

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable or Disable to remove audio,or video error packets.
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_TEI_RemoveErrorPkt(TSP_DRV_TEI_RmPktType eDrvPktType, MS_BOOL bEnable)
{
    MS_U32 u32PktType = TSP_PKTDMX_NONE;

    switch(eDrvPktType)
    {
        case E_TSP_DRV_TEI_REMOVE_PKT0_LIVE:
            u32PktType = TSP_PKTDMX0_LIVE;
            break;
        case E_TSP_DRV_TEI_REMOVE_PKT0_FILE:
            u32PktType = TSP_PKTDMX0_FILE;
            break;
        case E_TSP_DRV_TEI_REMOVE_PKT1:
            u32PktType = TSP_PKTDMX1;
            break;
        case E_TSP_DRV_TEI_REMOVE_PKT2:
            u32PktType = TSP_PKTDMX2;
            break;
        default:
            return DRVTSP_FAIL;
    }

    _TSP_ENTRY();

    HAL_TSP_TEI_RemoveErrorPkt(u32PktType, bEnable);

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Enable audio BD stream mode
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
    if(HAL_TSP_AU_BD_Mode_Enable(bEnable))
    {
        return DRVTSP_OK;
    }
    else
    {
        return DRVTSP_NOT_SUPPORTED;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable or Disable STC64 bit mode. Defalut is STC33 mode.
/// @param  bEnable                 \b IN: STC64 system time mode enable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC64_Mode_Enable(MS_BOOL bEnable)
{
    _TSP_ENTRY();
    HAL_TSP_STC64_Mode_En(bEnable);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// STC Clock Rate Adjust (27MHz clock)
/// @param  u32EngId                        \b IN: STC engine ID
/// @param bUpClk                            \b IN: If TRUE, Clk will be faster, and if FALSE, Clk will be slower
/// @param u32Percentage                 \b IN: The percentage of STC clock ratio
/// @return TSP_Result
/// @note
/// StcClk = (InClk * (2^27) /Stc_CW) * 3.2
/// Stc_CW = Stc_CW_Def / P
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STCClk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage)
{
    MS_U64 u64CW_New = 0;

    _TSP_ENTRY();

    u64CW_New = (((MS_U64)STC_SYNTH_DEFAULT) & 0xFFFFFFFFUL) * (((MS_U64)_ptsp_res->_u32StcAdjustUnit) & 0xFFFFFFFFUL) / (((MS_U64)u32Percentage) & 0xFFFFFFFFUL);

    //printf("[%s](%d, %d, %d) CW %d\n", __FUNCTION__, (unsigned int)u32EngId, (unsigned int)bUpClk, (unsigned int)u32Percentage, (unsigned int)u64CW_New);

    HAL_TSP_Stc_ctrl(u32EngId, (MS_U32)u64CW_New);

    _TSP_RETURN(DRVTSP_OK);
}

// Hidden interface. For Mstar use only
//-------------------------------------------------------------------------------------------------
// Set/Get TSP filter information
// @param  u32EngId                 \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  pu32StartFlt             \b IN/OUT: starting filter id which is availabe
// @param  pu32EndFlt               \b IN/OUT: starting filter id which is availabe
// @param  bSet                       \b IN: TRUE set, and FALSE for get
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Flt_SetOwner(MS_U32 u32EngId, MS_U32 u32FltIdStart, MS_U32 u32FltIdEnd, MS_BOOL bOwner)
{
    MS_U32 i = 0;

    _TSP_ENTRY();
    TSP_ASSERT2(0UL <= u32FltIdStart, TSP_DBG("[TSP_ERROR][%06d] Bad FltIdStart\n", __LINE__));
    TSP_ASSERT2(0UL <= u32FltIdEnd, TSP_DBG("[TSP_ERROR][%06d] Bad FltIdEnd\n", __LINE__));
    TSP_ASSERT2(u32FltIdEnd >= u32FltIdStart, TSP_DBG("[TSP_ERROR][%06d] u32FltIdEnd < u32FltIdStart\n", __LINE__));
    TSP_ASSERT2(TSP_SECFLT_NUM > u32FltIdStart, TSP_DBG("[TSP_ERROR][%06d] Bad FltIdStart\n", __LINE__));
    TSP_ASSERT2(TSP_SECFLT_NUM >= u32FltIdEnd, TSP_DBG("[TSP_ERROR][%06d] Bad FltIdEnd\n", __LINE__));

    for (i = u32FltIdStart; (i <= u32FltIdEnd) && (i < TSP_SECFLT_NUM); i++)
    {
        if ((_ptsp_res->_TspState[u32EngId].FltState[i] != E_DRVTSP_FLT_STATE_FREE) &&
            (_ptsp_res->_TspState[u32EngId].FltState[i] != E_DRVTSP_FLT_STATE_NA))
        {
            MS_ASSERT(0);
        }
        if ((_ptsp_res->_TspState[u32EngId].SecFltState[i] != E_DRVTSP_FLT_STATE_FREE) &&
            (_ptsp_res->_TspState[u32EngId].SecFltState[i] != E_DRVTSP_FLT_STATE_NA))
        {
            MS_ASSERT(0);
        }
        if ((_ptsp_res->_TspState[u32EngId].SecBufState[i] != E_DRVTSP_FLT_STATE_FREE) &&
            (_ptsp_res->_TspState[u32EngId].SecBufState[i] != E_DRVTSP_FLT_STATE_NA))
        {
            MS_ASSERT(0);
        }

        if (bOwner)
        {
            //reserverd 32 non-scramble filter for duplicate filter
            #ifdef DUP_PIDFLT_API
            if((i <= DSCMB2_FLT_END_ID) || (i >= DSCMB_FLT_SHAREKEY_END_ID))
            #endif
            #ifdef DUP_PIDFLT
            //reserverd 16 non-scramble filter for duplicate filter
            if((i <= DSCMB2_FLT_END_ID) || (i >= DSCMB_FLT_SHAREKEY_START_ID))
            #endif
            _ptsp_res->_TspState[u32EngId].FltState[i] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].SecFltState[i] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].SecBufState[i] = E_DRVTSP_FLT_STATE_FREE;
            HAL_TSP_SetOwner(u32EngId, i, TRUE);
        }
        else
        {
            _ptsp_res->_TspState[u32EngId].FltState[i] = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecFltState[i] = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecBufState[i] = E_DRVTSP_FLT_STATE_NA;
            HAL_TSP_SetOwner(u32EngId, i, FALSE);
        }
    }
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
// Ask TSP to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @param  u32EngId                  \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32FltId                   \b IN: Which filter id to process. 0xFFFFFFFF means process all filters (DMXID)
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Proc(MS_U32 u32EngId, MS_U32 u32FltId, TSP_Result* pRet, DrvTSP_Event* pEvt)                        // for non-OS TSP scheduling
{
    OSAL_TSP_ProcTypeParam Data;
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_SECFLT_NUM > u32FltId, TSP_DBG("[TSP_ERROR][%06d] Bad FltId %u\n", __LINE__, (unsigned int)u32FltId));
    if (_ptsp_res->_TspState[u32EngId].FltState[u32FltId] == E_DRVTSP_FLT_STATE_NA)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    Data.u32FltId = u32FltId;
    Data.pRet = (void*)pRet;
    Data.bInfiniteWait = FALSE;
    Data.bFromTask = FALSE;

    _TSP_IND_UNLOCK();
    OSAL_TSP_Proc(_TSP_Isr_ProcPoll, &Data, (MS_U32*)pEvt);
    _TSP_IND_LOCK();

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
// Allocate STC engine.
// @param  pu32EngId                  \b OUT: STC engine id. If no free STC engine, it will be 0xFFFFFFFF and return FAIL.
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_Alloc(MS_U32 *pu32EngId)
{
    TSP_Result ret;

    _TSP_ENTRY();

    ret = _TSP_STC_Alloc(0, pu32EngId);

    _TSP_RETURN(ret);
}


//-------------------------------------------------------------------------------------------------
// Free STC engine
// @param  u32EngId                  \b IN: index of STC engine to be free
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_Free(MS_U32 u32EngId)
{
    TSP_Result ret = DRVTSP_FAIL;

    _TSP_ENTRY();

    ret = _TSP_STC_Free(u32EngId);

    _TSP_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
// Free STC engine
// @param  u32PcrFltId                  \b IN: index of PCR PID filter
// @param  pu32EngId                    \b Out: index of STC engine which u32PcrFltId mapping to
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PcrId_To_StcId(MS_U32 u32PcrFltId,MS_U32 *pu32EngId)
{
    TSP_Result ret = DRVTSP_FAIL;

    _TSP_ENTRY();

    ret = _TSP_PcrId_To_StcId(u32PcrFltId, pu32EngId);

    _TSP_RETURN(ret);
}


//-------------------------------------------------------------------------------------------------
/// Get current system time clock (STC) of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32STC_32              \b OUT: pointer of STC[32] return
/// @param  pu32STC                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC)
{
    _TSP_ENTRY();
    HAL_TSP_STC_Update_Disable(u32EngId, TRUE);
    *pu32STC = HAL_TSP_GetSTC(u32EngId);
    *pu32STC_32 = HAL_TSP_GetSTC_32(u32EngId);
    HAL_TSP_STC_Update_Disable(u32EngId, FALSE);

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set STC control
/// @param u8Eng \b         IN: STC Engine ID
/// @param u8Opt \b         IN: STC update control options
///                                         0x00 -> Update STC by TSP FW
///                                         0x01 -> Update STC by HK
///                                         0x02 -> Update STC Once when PCR reset
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Mode)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_STC_NUM> u8Eng, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u8Eng));

    if (HAL_TSP_STC_UpdateCtrl(u8Eng, u8Mode))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
/// Select STC Engine (Only support in driver version 3.0)
/// @param eFltSrc                                 \b IN: Fliter Source type
/// @param u32StcEng                           \b IN: STC engine ID (0, 1)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_Select(DrvTSP_FltType eFltSrc, MS_U32 u32StcEng)
{
    TSP_Result ret;
    MS_U32 i;
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_STC_NUM> u32StcEng, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32StcEng));

    ret = _TSP_STC_Select(eFltSrc,u32StcEng);

    for(i=0;i<TSP_STC_NUM;i++)
    {
        if(_ptsp_res->_TspStcState[i].bUsed)
            TSP_DBG("[TSP_WARNING][%06d] STC Allocate before STC Select!\n", __LINE__);

        if(_ptsp_res->_TspStcState[i].u32SrcType == (eFltSrc & E_DRVTSP_FLT_SOURCE_TYPE_MASK))
            _ptsp_res->_TspStcState[u32StcEng].u32PcrFltId = _ptsp_res->_TspStcState[i].u32PcrFltId;
    }

    _ptsp_res->_TspStcState[u32StcEng].u32SrcType = eFltSrc & E_DRVTSP_FLT_SOURCE_TYPE_MASK;
    _ptsp_res->_bStcAllocDisable = TRUE;

    _TSP_RETURN(ret);

}

//-------------------------------------------------------------------------------------------------
/// Add or subtract STC Offset
// @param  u32EngId                  \b IN:  index of STC engine
// @param  u32Offset                  \b IN: STC offset value
// @param  bAdd                        \b IN:  If TRUE, add the offset value; otherwise subtract the offset value
// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd)
{
    _TSP_ENTRY();
    HAL_TSP_SetSTCOffset(u32EngId, u32Offset, bAdd);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get PRC of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32Pcr_32              \b OUT: pointer of STC[32] return
/// @param  pu32Pcr                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32EngId, MS_U32 *pu32Pcr_32, MS_U32 *pu32Pcr)
{
    _TSP_ENTRY();
    if (u32EngId >= TSP_PCRFLT_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    if (HAL_TSP_GetPcr(u32EngId, pu32Pcr_32, pu32Pcr))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    _TSP_RETURN(DRVTSP_FAIL);
}

// PIDFlt API
//-------------------------------------------------------------------------------------------------
/// Allocate a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eFilterType             \b IN: type of PID filter to be allocated
/// @param  pu32PidFltId            \b OUT: pointer of PID filter id return
/// @return TSP_Result
/// @note
/// These filter types have to select a section filter\n
/// @ref E_DRVTSP_FLT_TYPE_SECTION\n
/// @ref E_DRVTSP_FLT_TYPE_PCR\n
/// @ref E_DRVTSP_FLT_TYPE_PES\n
/// @note
/// These filter types also have to setup section buffer for data output\n
/// @ref E_DRVTSP_FLT_TYPE_SECTION\n
/// @ref E_DRVTSP_FLT_TYPE_PES\n
/// @sa MDrv_TSP_PidFlt_SelSecFlt, MDrv_TSP_SecFlt_SetBuffer
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32PidFltId)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));

#if (TSP_AUDIO3_AUDIO4_SUPPORT == 0) // if not support audio C & audio D FIFO
    TSP_ASSERT2((E_DRVTSP_FLT_TYPE_AUDIO3 != (eFilterType&~E_DRVTSP_FLT_MASK)) &&
                (E_DRVTSP_FLT_TYPE_AUDIO4 != (eFilterType&~E_DRVTSP_FLT_MASK)),
                TSP_DBG("[TSP_ERROR][%06d] Bad Filter Type %u\n", __LINE__, (unsigned int)eFilterType));
#endif

    *pu32PidFltId = 0xFFFFFFFFUL;
    if (!_TSP_PidFlt_Alloc(u32EngId, eFilterType, pu32PidFltId))
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate Pid Filter fail\n", __LINE__));
        _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_PidFlt_Init(u32EngId, *pu32PidFltId);

#ifndef HWPCR_ENABLE
    if (E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        MS_U32 u32SecFltId = *pu32PidFltId;
        if (!_TSP_SecFlt_Alloc(u32EngId, eFilterType, &u32SecFltId))
        {
            TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate Section Filter fail\n", __LINE__));
        }
        HAL_TSP_PidFlt_SelSecFlt(u32EngId, *pu32PidFltId, u32SecFltId);
        _ptsp_res->_TspState[u32EngId].FltMap[*pu32PidFltId] = u32SecFltId;
        _TSP_SecFlt_SetType(u32EngId, u32SecFltId, _ptsp_res->_TspState[u32EngId].FltType[*pu32PidFltId]);

        #ifdef DUP_PIDFLT_API
        if(_ptsp_res->_TspState[u32EngId].DupFlt[*pu32PidFltId] != 0xFFFFUL)
        {
            MS_U32 u32DupFlt = _ptsp_res->_TspState[u32EngId].DupFlt[*pu32PidFltId];

            HAL_TSP_PidFlt_SelSecFlt(u32EngId, u32DupFlt, u32SecFltId);
            _ptsp_res->_TspState[u32EngId].FltMap[u32DupFlt] = u32SecFltId;
        }
        #endif
    }
#endif

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Free a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_Free(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    MS_U32 u32SecFltId, u32Flag = 0;
    TSP_Result tRes = DRVTSP_FAIL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    // this API only free pidflt which had section
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));

    //_TSP_IND_UNLOCK();
    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId])
    {
        _TSP_RETURN(DRVTSP_OK);
    }

    // PVR pid filters and AV pid filter do not have SecFltEvtNotify, so call free function directly.
    u32SecFltId = _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId];
    if((u32SecFltId >= TSP_SECFLT_NUM) ||
         ((_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId] & E_DRVTSP_FLT_TYPE_SECTION_MASK) == 0))
    {
        //_TSP_IND_LOCK();
        _TSP_PidFlt_Free(u32EngId, u32PidFltId, TRUE);
        _TSP_RETURN(DRVTSP_OK);
    }

    if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_POLL)
    {
        //_TSP_IND_LOCK();
        _TSP_SecBuf_Free(0, u32SecFltId);
        _TSP_SecFlt_Free(0, u32SecFltId, TRUE);
        _TSP_PidFlt_Free(u32EngId, u32PidFltId, TRUE);
        _TSP_RETURN(DRVTSP_OK);
    }
    else if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_SELF)
    {
        if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId])
        {
            _TSP_RETURN(DRVTSP_OK);
        }

        #if 1
        if((_s32TspTaskId != -1) && (_s32TspTaskId == MsOS_GetOSThreadID()))
        {
            TSP_DBG("[TSP_ERROR][%06d] Don't call %s in Callback function!!!", __LINE__, __FUNCTION__);
            _TSP_RETURN(DRVTSP_FAIL);
        }
        #endif

        if(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] & E_DRVTSP_FLT_STATE_CBRUN)
        {
            //ULOGD("TSP", "[%s][%d]Warning !! Filter %d callback is running %x! Try Again!\n", __FUNCTION__, __LINE__, (unsigned int)u32PidFltId, (unsigned int)stDrvState);
            _TSP_RETURN(DRVTSP_FAIL);
        }

        _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] |= E_DRVTSP_FLT_STATE_FREEING;
        _TSP_GetSecRdyId(u32SecFltId, &u32Flag, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecOvfId(u32SecFltId, &u32Flag, OSAL_EVENT_OR_CLEAR);
        _TSP_SecBuf_Free(u32EngId, u32SecFltId);
        _TSP_SecFlt_Free(u32EngId, u32SecFltId, TRUE);
        _TSP_PidFlt_Free(u32EngId, u32PidFltId, TRUE);

        _TSP_RETURN(DRVTSP_OK);

    }

    //_TSP_IND_LOCK();
    _TSP_RETURN(tRes);
}


//-------------------------------------------------------------------------------------------------
/// Set PID to a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be set
/// @param  u32PID                  \b IN: PID value
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_SetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U16 u16PID)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_PIDFLT_NUM)
    {
        HAL_TSP_PidFlt_SetHWPcrPid(u32PidFltId - TSP_PIDFLT_NUM, ((MS_U32)u16PID) & 0xFFFFUL);
    }
    else
    {
#endif
        HAL_TSP_PidFlt_SetPid(u32EngId, u32PidFltId, ((MS_U32)u16PID) & 0xFFFFUL);

        #ifdef DUP_PIDFLT
        if(_ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId] != 0xFFFFUL)
        {
            MS_U32 u32DupFlt = _ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId];
            HAL_TSP_PidFlt_SetPid(u32EngId, u32DupFlt, ((MS_U32)u16PID & 0xFFFFUL));
            //ULOGD("TSP", "[%s][%d] dup id %ld, pid %x\n", __FUNCTION__, __LINE__, u32DupFlt, u16PID);
        }
        #endif

#ifdef HWPCR_ENABLE
    }
#endif

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be set
/// @param  pu32PID                 \b OUT: the output PID
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_GetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32* pu32PID)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));
    //TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_PIDFLT_NUM)
    {
        *pu32PID = HAL_TSP_PidFlt_GetHWPcrPid(u32PidFltId - TSP_PIDFLT_NUM);
    }
    else
    {
#endif
        *pu32PID = HAL_TSP_PidFlt_GetPid(u32EngId, u32PidFltId);

#ifdef HWPCR_ENABLE
    }
#endif
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Select section filter of PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter\n
/// @param  u32SecFltId             \b IN: index of section filter
/// @return TSP_Result
/// @note
/// The PID filter and section filter pair is one-to-one mapping. User has to
/// allocate other PID filters if user have more than one section filter for same
/// PID packet.\n
/// @sa MDrv_TSP_PidFlt_Alloc
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_SelSecFlt(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32SecFltId)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] SecFltId= %u\n", __LINE__, (unsigned int)u32SecFltId));

    HAL_TSP_PidFlt_SelSecFlt(u32EngId, u32PidFltId, u32SecFltId);
    _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] = u32SecFltId;
    _TSP_SecFlt_SetType(u32EngId, u32SecFltId, _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId]);

    #ifdef DUP_PIDFLT_API
    if(_ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId] != 0xFFFFUL)
    {
        MS_U32 u32DupFlt = _ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId];

        HAL_TSP_PidFlt_SelSecFlt(u32EngId, u32DupFlt, u32SecFltId);
        _ptsp_res->_TspState[u32EngId].FltMap[u32DupFlt] = u32SecFltId;
    }
    #endif

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID filter input source directly
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId              \b IN: index of filter to be enable
/// @param  eFltSrc                    \b IN: Filter input source
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_SetInputSrc(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltType eFltSrc)
{
    _TSP_ENTRY();

    switch(eFltSrc & E_DRVTSP_FLT_SOURCE_TYPE_MASK)
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT0);
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT_FILE);
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT1);
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT2);
            break;

        #ifdef TS3_IF_SUPPORT
        case E_DRVTSP_FLT_SOURCE_TYPE_TSCB:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT_CB);
            break;
        #endif

        default:
            _TSP_RETURN(DRVTSP_INVALID_PARAM);

    }

    #ifdef MERGE_STR_SUPPORT
    HAL_TSP_PidFlt_SetFltSrcStreamID(u32EngId, u32PidFltId, (eFltSrc & E_DRVTSP_FLT_SOURCEID_MASK) >> E_DRVTSP_FLT_SOURCEID_SHIFT);
    _ptsp_res->_TspState[u32EngId].FltSrcId[u32PidFltId] =  (eFltSrc & 0x00000F00UL) >> E_DRVTSP_FLT_SOURCEID_SHIFT;
    #endif

    _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = eFltSrc;

    _TSP_RETURN(DRVTSP_OK);

}


TSP_Result MDrv_TSP_PidFlt_SetFltRushPass(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U8 u8Enable)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

    HAL_TSP_PidFlt_SetFltRushPass(u32EngId, u32PidFltId, u8Enable);

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of filter to be enable
/// @param  bEnable                 \b IN: TRUE(enable), FALSE(disable)
/// @return TSP_Result
/// @note
/// When PID filter enable, the section buffer pointer will be reset to buffer start address,
/// overflow condition will be resolved if exist.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_Enable(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bEnable)
{
    MS_U32              u32AVSrc = 0;

#ifdef TSP_LIVE_AV_BLOCK_EN
    MS_BOOL             bAVFIFO_BLK_Disable = FALSE;
#else
    MS_BOOL             bAVFIFO_BLK_Disable = TRUE;
#endif

    _TSP_STR_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

    if (!bEnable)
    {
        if (!HAS_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], E_DRVTSP_FLT_STATE_ENABLE))
        {
            // TSP_DBG("[TSP_WARNNING][%06d] disable an inactive PidFlt %d\n", __LINE__, u32PidFltId);
            _TSP_STR_RETURN(DRVTSP_OK);
        }

        _TSP_PidFlt_Disable(u32EngId, u32PidFltId);
        _TSP_STR_RETURN(DRVTSP_OK);
    }

    if (HAS_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], E_DRVTSP_FLT_STATE_ENABLE))
    {
        // TSP_DBG("[TSP_WARNNING][%06d] Enable an active PidFlt %d\n", __LINE__, u32PidFltId);
        _TSP_STR_RETURN(DRVTSP_OK);
    }

    switch(_ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            u32AVSrc = E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            u32AVSrc = E_DRVTSP_PKTSRC_DEMUXFILE;
            bAVFIFO_BLK_Disable = FALSE;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
            u32AVSrc = E_DRVTSP_PKTSRC_DEMUX1;
            bAVFIFO_BLK_Disable = FALSE;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
            u32AVSrc = E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
            u32AVSrc = E_DRVTSP_PKTSRC_DEMUX2;
            bAVFIFO_BLK_Disable = FALSE;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
            u32AVSrc = E_DRVTSP_PKTSRC_DEMUX2;
            break;
        default:
            break;
    }

    if (E_DRVTSP_FLT_TYPE_PVR == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_PVR1);
    }
    #if (TSP_PVR_IF_NUM >= 2)
    else if (E_DRVTSP_FLT_TYPE_PVR1 == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_PVR2);
    }
    #endif
    #ifdef TS3_IF_SUPPORT
    else if (E_DRVTSP_FLT_TYPE_CB == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_CB);
    }
    #endif
    else if (E_DRVTSP_FLT_TYPE_VIDEO == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_AVFIFO_Src_Select(TSP_FIFO_VD, u32AVSrc);
        HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_VD, bAVFIFO_BLK_Disable);
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_VFIFO);
    }
    else if (E_DRVTSP_FLT_TYPE_AUDIO == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_AVFIFO_Src_Select(TSP_FIFO_AU, u32AVSrc);
        HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_AU, bAVFIFO_BLK_Disable);
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_AFIFO);
    }
    else if (E_DRVTSP_FLT_TYPE_AUDIO2 == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_AVFIFO_Src_Select(TSP_FIFO_AUB, u32AVSrc);
        HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_AUB, bAVFIFO_BLK_Disable);
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_AFIFO2);
    }
#if (TSP_AUDIO3_AUDIO4_SUPPORT == 1)
    else if (E_DRVTSP_FLT_TYPE_AUDIO3 == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_AVFIFO_Src_Select(TSP_FIFO_AUC, u32AVSrc);
        HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_AUC, bAVFIFO_BLK_Disable);
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_AFIFO3);
    }
    else if (E_DRVTSP_FLT_TYPE_AUDIO4 == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_AVFIFO_Src_Select(TSP_FIFO_AUD, u32AVSrc);
        HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_AUD, bAVFIFO_BLK_Disable);
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_AFIFO4);
    }
#endif
    else if (E_DRVTSP_FLT_TYPE_VIDEO3D == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        HAL_TSP_AVFIFO_Src_Select(TSP_FIFO_V3D, u32AVSrc);
        HAL_TSP_AVFIFO_Block_Disable(TSP_FIFO_V3D, bAVFIFO_BLK_Disable);
        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_VFIFO3D);
    }
#ifdef HWPCR_ENABLE
    else if (E_DRVTSP_FLT_TYPE_PCR == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        MS_U32 u32Idx = u32PidFltId - TSP_PIDFLT_NUM;

        HAL_TSP_HWPcr_SelSrc(u32Idx, u32AVSrc);

        #ifdef MERGE_STR_SUPPORT
        HAL_TSP_HWPcr_SetSrcId(u32Idx, _ptsp_res->_TspState[u32EngId].FltSrcId[u32PidFltId]);
        #endif

#if (TSP_HWPCR_BY_HK == 1)
        _ptsp_res->TSPHwPcrParams._u8ResetPcr[u32Idx] = 3;
        _ptsp_res->TSPHwPcrParams._u32PrevStcBase[u32Idx] = 0;
        _ptsp_res->TSPHwPcrParams._u32PrevPllControl[u32Idx] = 0;
#endif

        HAL_TSP_HWPcr_Reset(u32Idx, TRUE);
#if (TSP_HWPCR_BY_HK == 1)
        HAL_TSP_HWPcr_Reset(u32Idx, FALSE);
        HAL_TSP_HWPcr_Int_Enable(u32Idx, TRUE);
        _TSP_SetHwPcrNotifyId(u32Idx);
#endif

        HAL_TSP_PidFlt_HWPcrFlt_Enable(u32Idx, TRUE);
        _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId]|= E_DRVTSP_FLT_STATE_ENABLE;

        _TSP_STR_RETURN(DRVTSP_OK);
    }
#endif
    else
    {
        MS_U32          u32SecFltId;

        TSP_ASSERT2(TSP_SECFLT_NUM> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));

        //u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(u32EngId, u32PidFltId);
        u32SecFltId = _ptsp_res->_TspState[0].FltMap[u32PidFltId];
        TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] SecFltId= %u\n", __LINE__, (unsigned int)u32SecFltId));

        // reset ver/crc check
        #ifndef DUP_PIDFLT
        if (_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId] == E_DRVTSP_FLT_TYPE_SECTION_VER)
        #else
        if (_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId] & E_DRVTSP_FLT_TYPE_SECTION_MASK)
        #endif
        {
            HAL_TSP_SecFlt_VerReset(u32SecFltId);
        }

        switch (_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
        {
            case E_DRVTSP_FLT_TYPE_SECTION:
            case E_DRVTSP_FLT_TYPE_PES:
            case E_DRVTSP_FLT_TYPE_PACKET:
            case E_DRVTSP_FLT_TYPE_TELETEXT:
            case E_DRVTSP_FLT_TYPE_SECTION_VER:
            case E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI:
                //TSP_ASSERT2(TSP_SECBUF_NUM> u32SecBufId, TSP_DBG("[TSP_ERROR][%06d] SecBufId= %ld\n", __LINE__, u32SecBufId));
                HAL_TSP_SecBuf_ResetBuffer(u32EngId, u32SecFltId);
                HAL_TSP_SecFlt_ResetState(u32EngId, u32SecFltId);
                HAL_TSP_SecFlt_SetRmnCount(u32EngId, u32SecFltId, 0);
                //HAL_TSP_Reset_SecDisCont_PktCount(u32PidFltId);
                HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_SECFLT);
                break;
            case E_DRVTSP_FLT_TYPE_EMM:
            case E_DRVTSP_FLT_TYPE_ECM:
                HAL_TSP_SecFlt_ResetState(u32EngId, u32SecFltId);
                HAL_TSP_SecFlt_SetRmnCount(u32EngId, u32SecFltId, 0);
                HAL_TSP_SecBuf_ResetBuffer(u32EngId, u32SecFltId);
                //HAL_TSP_Reset_SecDisCont_PktCount(u32PidFltId);
                HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_SECFLT);
                break;
            case E_DRVTSP_FLT_TYPE_PCR:
                HAL_TSP_SecFlt_PcrReset(u32EngId, u32SecFltId);
                HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32PidFltId, TSP_PIDFLT_OUT_SECFLT_AF);
                break;
            default:
                break;
        }
    }


    #ifdef MERGE_STR_SUPPORT
        HAL_TSP_PidFlt_SetFltSrcStreamID(u32EngId, u32PidFltId, _ptsp_res->_TspState[u32EngId].FltSrcId[u32PidFltId]);
    #endif

    switch(_ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT0);
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT_FILE);
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT1);
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT2);
            break;
        #ifdef TS3_IF_SUPPORT
        case E_DRVTSP_FLT_SOURCE_TYPE_TSCB:
            HAL_TSP_PidFlt_SelFltSource(u32EngId, u32PidFltId, TSP_PIDFLT_IN_PIDFLT_CB);
            break;
        #endif
        default:
            break;
    }

    _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId]|= E_DRVTSP_FLT_STATE_ENABLE;

#ifdef DUP_PIDFLT
    if(_ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId] != 0xFFFFUL)
    {
        MS_U32 u32DupFlt = _ptsp_res->_TspState[u32EngId].DupFlt[u32PidFltId];

        HAL_TSP_PidFlt_SelFltOutput(u32EngId, u32DupFlt, TSP_PIDFLT_OUT_SECFLT);
        switch(_ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
        {
            case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
                HAL_TSP_PidFlt_SelFltSource(u32EngId, u32DupFlt, TSP_PIDFLT_IN_PIDFLT0);
                break;
            case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
                HAL_TSP_PidFlt_SelFltSource(u32EngId, u32DupFlt, TSP_PIDFLT_IN_PIDFLT_FILE);
                break;
            case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
            case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
                HAL_TSP_PidFlt_SelFltSource(u32EngId, u32DupFlt, TSP_PIDFLT_IN_PIDFLT1);
                break;
            case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
            case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
                HAL_TSP_PidFlt_SelFltSource(u32EngId, u32DupFlt, TSP_PIDFLT_IN_PIDFLT2);
                break;
            case E_DRVTSP_FLT_SOURCE_TYPE_TSCB:
                HAL_TSP_PidFlt_SelFltSource(u32EngId, u32DupFlt, TSP_PIDFLT_IN_PIDFLT_CB);
                break;
            default:
                break;
        }
        _ptsp_res->_TspState[u32EngId].FltState[u32DupFlt]|= E_DRVTSP_FLT_STATE_ENABLE;

        //ULOGD("TSP", "[%s][%d] dup id %u\n", __FUNCTION__, __LINE__, u32DupFlt);
    }
#endif

    _TSP_STR_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current PID filter status
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter
/// @param  peState                 \b OUT: current ORed state flag of PID filter\n
///                                         E_DRVTSP_FLT_STATE_ALLOC\n
///                                         E_DRVTSP_FLT_STATE_ENABLE\n
///                                         E_DRVTSP_FLT_STATE_SCRAMBLED ([VENUS]: TS level scramble status)
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_GetState(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %u\n", __LINE__, (unsigned int)u32PidFltId));
    if (_TSP_PidFlt_StateGet(u32EngId, u32PidFltId, peState))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
/// Check current PID filter owner
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_ChkOwner(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
    _TSP_ENTRY();

    if (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] == E_DRVTSP_FLT_STATE_NA)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Change PID filter source
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eFilterType             \b IN: index of filter to be enable
/// @param  bEnable                  \b IN: New filter source type & filter source Id to change
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_ChangeSource(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltType eFilterType)
{
    DrvTSP_FltType eFltSrc = eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK;

    _TSP_ENTRY();

    if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_LIVE)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
    }
    else if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
    }
    else if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TS1)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_TS1;
    }
    else if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TS2)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_TS2;
    }
    else if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TSCB)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_TSCB;
    }
    else if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE1)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_FILE1;
    }
    else if (eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE2)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_FILE2;
    }

    #ifdef MERGE_STR_SUPPORT
    _ptsp_res->_TspState[u32EngId].FltSrcId[u32PidFltId] = (eFilterType & E_DRVTSP_FLT_SOURCEID_MASK) >> E_DRVTSP_FLT_SOURCEID_SHIFT;
    #endif

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get PID filter scramble status
/// @param  FltSrc                         \b IN     : Filter source
/// @param  u32FltGroupId             \b IN     : Pid filter group id, every 32 filter for 1 group. 0: filter 0~31, 1: 32~63, 2: 64~95, 3: 96~127
/// @param  PidFltId                       \b IN     : Pid filter id for getting. If 0xFFFFFFFF, means getting the status of all filters of the same group
/// @param  pu32ScmbSts              \b OUT  : The scrmabling status of pid filter. If getting all group status, every bit means one status of every filter
///                                                               If only checking one filter,  1 means scrambled status and 0 means non-scrambled status
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_GetScmbSts(DrvTSP_FltType FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts)
{
    MS_U32 u32Src = 0;

    _TSP_ENTRY();

    *pu32ScmbSts = 0;

    switch(FltSrc & E_DRVTSP_FLT_SOURCE_TYPE_MASK)
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            u32Src = 0;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            u32Src = 1;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE1:
            u32Src = 2;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE2:
            u32Src = 3;
            break;
        default:
            _TSP_RETURN(DRVTSP_INVALID_PARAM);
    }

    *pu32ScmbSts = HAL_TSP_Scmb_Status(u32Src, u32FltGroupId, PidFltId);

    _TSP_RETURN(DRVTSP_OK);
}

// SecFlt API
//-------------------------------------------------------------------------------------------------
/// Allocate a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32SecFltId            \b OUT: pointer of section filter id return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_Alloc(MS_U32 u32EngId, MS_U32 *pu32SecFltId)
{
    MS_U32         u32SecBufId;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM > u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (FALSE== _TSP_SecFlt_Alloc(u32EngId, E_DRVTSP_FLT_TYPE_SECTION, pu32SecFltId))
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate section filter fail\n", __LINE__));
    }
    TSP_ASSERT2(TSP_SECFLT_NUM > *pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)(*pu32SecFltId)));
    u32SecBufId = *pu32SecFltId;

#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 u32ii;

    for(u32ii = 0; u32ii < DRVTSP_MAX_PROCESS_NUM; u32ii++)
    {
        if(_u32KPrcEvtTblUseFlag & (1 << u32ii))
        {
            if(_stKModePrcEvtTbl[u32ii].tgid == current->tgid)
            {
                _ptsp_res->_TspState[u32EngId].s32TspEvtId[u32SecBufId] = _stKModePrcEvtTbl[u32ii].s32TspEvtId;
                break;
            }
        }
    }
#else
    _ptsp_res->_TspState[u32EngId].s32TspEvtId[u32SecBufId] = _s32TspEventId;
#endif

    //printf("[%s][%d] u32SecBufId %d , s32TspEvtId %x\n", __FUNCTION__, __LINE__, (int)u32SecBufId, (int) _ptsp_res->_TspState[u32EngId].s32TspEvtId[u32SecBufId]);

    if (FALSE == _TSP_SecBuf_Alloc(u32EngId, &u32SecBufId))
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate section buffer fail\n", __LINE__));
    }
    TSP_ASSERT2(TSP_SECBUF_NUM > u32SecBufId, TSP_DBG("[TSP_ERROR][%06d] Bad SecBufId %u\n", __LINE__, (unsigned int)u32SecBufId));
    TSP_ASSERT2(u32SecBufId == *pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId/SecFltId alignment\n", __LINE__));
    _TSP_SecBuf_Init(u32EngId, u32SecBufId);
    HAL_TSP_SecFlt_SelSecBuf(0, *pu32SecFltId, u32SecBufId, TRUE);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Free a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: section filter of TSP to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_Free(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId],
    TSP_DBG("[TSP_ERROR][%06d] Bad SecFlt state\n", __LINE__));

    _TSP_SecBuf_Free(u32EngId, u32SecFltId);
    _TSP_SecFlt_Free(u32EngId, u32SecFltId, TRUE);

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Set section filtering mode
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32SecFltId             \b IN: section filter of TSP to be free
// @param  eSecFltMode             \b IN: continue/one-shot mode of section filter
// @return TSP_Result
// @attention
// One-shot filter has the disadvantage of interrupt lost becuase it stops filter, a timeout
// to check filter status is better for usage.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetMode(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltMode eSecFltMode)
{
    MS_U32         uMode = 0;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));

    if (eSecFltMode & E_DRVTSP_FLT_MODE_ONESHOT)
    {
        uMode |= TSP_SECFLT_MODE_ONESHOT;
    }

    if (eSecFltMode & E_DRVTSP_FLT_MODE_CRCCHK)
    {
        uMode |= TSP_SECFLT_MODE_CRCCHK;
    }

    _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId] = eSecFltMode;

    HAL_TSP_SecFlt_SetMode(u32EngId, u32SecFltId, uMode);

#ifdef DUP_PIDFLT //only duplicate one-shot mode section filter
    if((eSecFltMode & E_DRVTSP_FLT_MODE_ONESHOT) == 0)
    {
        MS_U32 u32ii, u32OrgPidFlt = u32SecFltId;

        //Suggest original pid filter id = section filter id
        if(_ptsp_res->_TspState[u32EngId].FltMap[u32SecFltId] != u32SecFltId)
        {
            for(u32OrgPidFlt = 0; u32OrgPidFlt < TSP_PIDFLT_NUM; u32OrgPidFlt++)
            {
                if(_ptsp_res->_TspState[u32EngId].FltMap[u32OrgPidFlt] == u32SecFltId)
                {
                    break;
                }
            }
            if(u32OrgPidFlt == TSP_PIDFLT_NUM)
            {
                ULOGD("TSP", "[%s][%d] No mapping Pid filter\n", __FUNCTION__, __LINE__);
                _TSP_RETURN(DRVTSP_OK);
            }
        }

        for(u32ii = (DSCMB2_FLT_END_ID + 1); u32ii < DSCMB_FLT_SHAREKEY_START_ID; u32ii++)
        {
            if(_ptsp_res->_TspState[u32EngId].FltState[u32ii] == E_DRVTSP_FLT_STATE_NA)
            {
                MS_U32 u32OrgPid = HAL_TSP_PidFlt_GetPid(u32EngId, u32OrgPidFlt);

                _TSP_PidFlt_Init(u32EngId, u32ii);
                HAL_TSP_PidFlt_SelSecFlt(u32EngId, u32ii, u32SecFltId);
                _ptsp_res->_TspState[u32EngId].FltMap[u32ii] = u32SecFltId;
                if(u32OrgPid != DRVTSP_PID_NULL)
                {
                    HAL_TSP_PidFlt_SetPid(u32EngId, u32ii, u32OrgPid);
                }
                _ptsp_res->_TspState[u32EngId].DupFlt[u32OrgPidFlt] = u32ii;
                _ptsp_res->_TspState[u32EngId].FltState[u32ii] = E_DRVTSP_FLT_STATE_ALLOC;
                _ptsp_res->_TspState[u32EngId].FltType[u32ii] = _ptsp_res->_TspState[u32EngId].FltType[u32OrgPidFlt];
                _ptsp_res->_TspState[u32EngId].FltSource[u32ii] = _ptsp_res->_TspState[u32EngId].FltSource[u32OrgPidFlt];
                _ptsp_res->_TspState[u32EngId].FltMap[u32ii] = u32SecFltId;
                //ULOGD("TSP", "[%s] duplcate id %ld %ld, pid %lx\n", __FUNCTION__, u32OrgPidFlt, u32ii, u32OrgPid);
                break;
            }
        }
    }
#endif

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set Match/Mask filter pattern of section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of filter to be set pattern
/// @param  pu8Match                \b IN: pointer of filter pattern (in @ref DRVTSP_FILTER_DEPTH bytes)
/// @param  pu8Mask                 \b IN: pointer of pattern bitmask (in @ref DRVTSP_FILTER_DEPTH bytes)
/// @param  bNotMatch               \b IN: negative the result of comparion of filter
/// @return TSP_Result
/// note: match mask -- must set 0 to be compare (customer request)
///       not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetPattern(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NotMask)
{
    if(pu8Match == NULL || pu8Mask == NULL || pu8NotMask == NULL)
    {
        return (DRVTSP_INVALID_PARAM);
    }

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));

    MS_U8 i = 0;
    for (i = 0 ; i < 16 ; i++)
    {
        *(pu8Mask+i) = 0xFF^(*(pu8Mask+i));
    }

    HAL_TSP_SecFlt_SetMask(u32EngId, u32SecFltId, pu8Mask);
    HAL_TSP_SecFlt_SetMatch(u32EngId, u32SecFltId, pu8Match);
    HAL_TSP_SecFlt_SetNMask(u32EngId, u32SecFltId, pu8NotMask);
    _TSP_RETURN(DRVTSP_OK);
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
//-------------------------------------------------------------------------------------------------
/// Get Match/Mask filter pattern of section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of filter to be set pattern
/// @param  pu8Match                \b IN: pointer of filter pattern (in @ref DRVTSP_FILTER_DEPTH bytes)
/// @param  pu8Mask                 \b IN: pointer of pattern bitmask (in @ref DRVTSP_FILTER_DEPTH bytes)
/// @param  bNotMatch               \b IN: negative the result of comparion of filter
/// @return TSP_Result
/// note: match mask -- must set 0 to be compare (customer request)
///       not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetPattern(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NotMask)
{
    if(pu8Match == NULL || pu8Mask == NULL || pu8NotMask == NULL)
    {
        return (DRVTSP_INVALID_PARAM);
    }

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));

    HAL_TSP_SecFlt_GetMask(u32EngId, u32SecFltId, pu8Mask);
    HAL_TSP_SecFlt_GetMatch(u32EngId, u32SecFltId, pu8Match);

    MS_U8 i = 0;
    for (i = 0 ; i < 16 ; i++)
    {
        *(pu8Mask+i) = 0xFF^(*(pu8Mask+i));
    }

    _TSP_RETURN(DRVTSP_OK);
}
#endif

//-------------------------------------------------------------------------------------------------
/// Reset the section buffer read/write pointer to start address and resolve overflow condition
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be reset
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_ResetBuffer(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));

    // Richard: Be careful since it works based the assumption that SecFltId is equal to SecBufId
    HAL_TSP_SecBuf_ResetBuffer(u32EngId, u32SecFltId);
    HAL_TSP_SecFlt_ResetState(u32EngId, u32SecFltId);
    HAL_TSP_SecFlt_ResetEmmIdx(u32EngId, u32SecFltId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set buffer start address and buffer size to section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be set
/// @param  u32StartAddr            \b IN: start address of section buffer
/// @param  u32BufSize              \b IN: size of section buffer
/// @return TSP_Result
/// @note
/// Buffer start address and buffer size should be 128-bit (16-byte) aligned.\n
/// @sa MDrv_TSP_PidFlt_Alloc
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetBuffer(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY phyStartAddr, MS_U32 u32BufSize)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    HAL_TSP_SecBuf_SetBuffer(u32EngId, u32SecFltId, phyStartAddr, u32BufSize);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set request data size to trigger interrupt
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be set
/// @param  u32ReqCount             \b IN: received data size to trigger interrupt
/// @return TSP_Result
/// @note
/// TSP always calls section callback function when a completed section data is
/// ready at section buffer.
/// It can set a request value other than 0 and TSP will also notify user when
/// request size of data is ready at buffer. Only support @ref E_DRVTSP_FLT_TYPE_PES.
/// @sa MDrv_TSP_SecFlt_Notify
/// @attention
/// The maximum request count is 0xFFFF
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetReqCount(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32ReqCount)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    HAL_TSP_SecFlt_SetReqCount(u32EngId, u32SecFltId, u32ReqCount);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set ECM index
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  u32EcmIdx               \b IN: Index of ECM
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetEcmIdx(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32EcmIdx)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    HAL_TSP_SecFlt_SetEcmIdx(u32EngId, u32SecFltId, u32EcmIdx);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get buffer start address of setction buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32BufStart            \b OUT:  pointer of buffer start address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetBufStart(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyBufStart)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pphyBufStart, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    *pphyBufStart = (MS_PHY)HAL_TSP_SecBuf_GetBufStart(u32EngId, u32SecFltId) & 0xFFFFFFFFUL;
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get buffer size of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32BufSize             \b OUT: pointer of buffer size return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetBufSize(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 *pu32BufSize)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pu32BufSize, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    *pu32BufSize = 0;
    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    *pu32BufSize=  (MS_U32)(HAL_TSP_SecBuf_GetBufEnd(u32EngId, u32SecFltId)-
                   HAL_TSP_SecBuf_GetBufStart(u32EngId, u32SecFltId));
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pphyReadAddr            \b OUT: pointer of address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetReadAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyReadAddr)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pphyReadAddr, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    // move write point to read point while E_DRVTSP_FLT_MODE_AUTO_ADDR mode
    if (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId]&E_DRVTSP_FLT_MODE_AUTO_ADDR)
    {
        HAL_TSP_SecBuf_SetBufRead(u32EngId, u32SecFltId, HAL_TSP_SecBuf_GetBufWrite(u32EngId, u32SecFltId));
    }
    *pphyReadAddr= HAL_TSP_SecBuf_GetBufRead(u32EngId, u32SecFltId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current section data write address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pphyWriteAddr           \b OUT: pointer of address return
/// @return TSP_Result
/// @note
/// User can get current write address to know where is the end of section data
/// received in the section buffer.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyWriteAddr)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pphyWriteAddr, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    // move write point to read point while E_DRVTSP_FLT_MODE_AUTO_ADDR mode
    if (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId]&E_DRVTSP_FLT_MODE_AUTO_ADDR)
    {
        HAL_TSP_SecBuf_SetBufRead(u32EngId, u32SecFltId, HAL_TSP_SecBuf_GetBufWrite(u32EngId, u32SecFltId));
    }
    *pphyWriteAddr= HAL_TSP_SecBuf_GetBufWrite(u32EngId, u32SecFltId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  phyReadAddr             \b IN: address of read pointer
/// @return TSP_Result
/// @note
/// User can update the read address to notify TSP where is the end of section
/// data already read back by user.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetReadAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY phyReadAddr)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    HAL_TSP_SecBuf_SetBufRead(u32EngId, u32SecFltId, phyReadAddr);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Subscribe event notification callback function for specified section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  eEvents                 \b IN: events need to be subscribed\n
///                                        @ref E_DRVTSP_EVENT_DATA_READY\n
///                                        @ref E_DRVTSP_EVENT_BUF_OVERFLOW
/// @param  pfCallback              \b IN: callback function (NULL to disable)
/// @return TSP_Result
/// @note
/// This function register a callback function for a section filter to TSP.
/// TSP calls callback function each time when data is ready in section buffer.\n
/// Data ready of section filter:\n
/// @ref E_DRVTSP_FLT_TYPE_SECTION : a section ready\n
/// @ref E_DRVTSP_FLT_TYPE_PES : PES packet ready or received data over than request size.
/// @sa MDrv_TSP_SecFlt_SetReqCount
/// @attention
/// Callback function resides in OS TSP interrupt context, it recommends
/// that callback function should not take too much time to block the system.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_Notify(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));

    if ((eEvents & (E_DRVTSP_EVENT_DATA_READY | E_DRVTSP_EVENT_BUF_OVERFLOW | E_DRVTSP_EVENT_SEC_CRCERR)) == 0)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] = eEvents;
    _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecFltId]  = pfCallback;

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Get current section filter status
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32SecFltId             \b IN: index of section filter
// @param  peState                 \b OUT: current ORed state flag of section filter
//                                         E_DRVTSP_FLT_STATE_ALLOC\n
//                                         E_DRVTSP_FLT_STATE_ENABLE\n
//                                         E_DRVTSP_FLT_STATE_OVERFLOW
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetState(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(peState, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (_TSP_SecFlt_StateGet(u32EngId, u32SecFltId, peState))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

TSP_Result MDrv_TSP_TTX_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyWriteAddr)
{
    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %u\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %u\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT(pphyWriteAddr, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));
    // move write point to read point while E_DRVTSP_FLT_MODE_AUTO_ADDR mode

    if (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId]&E_DRVTSP_FLT_MODE_AUTO_ADDR)
    {
        HAL_TSP_SecBuf_SetBufRead_tmp(u32EngId, u32SecFltId, HAL_TSP_SecBuf_GetBufWrite_tmp(u32EngId, u32SecFltId));
    }
    *pphyWriteAddr= HAL_TSP_SecBuf_GetBufWrite_tmp(u32EngId, u32SecFltId);
    return DRVTSP_OK;
}

// PVR API
// Channel Browser API
//-------------------------------------------------------------------------------------------------
/// Set Channel Browser record buffer start addresses and buffer size
/// @param  u8PVRId                      \b IN:PVR ID
/// @param  phyBufStart0            \b IN: start address of PVR buffer 0
/// @param  phyBufStart1            \b IN: start address of PVR buffer 1
/// @param  u32BufSize0                \b IN: size of PVR buffer 0
/// @param  u32BufSize1                \b IN: size of PVR buffer 1
/// @return TSP_Result
/// @note: PVR1 only has single buffer
/// Buffer start address and size should be 128-bit (16-byte) aligned\n
/// The maximum support size is 0xFFFF0 (1048560 bytes).
/// @sa MDrv_TSP_PVR_Notify
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SetBuffer(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
    TSP_ASSERT(u8PVRId < ((MS_U8)TSP_PVR_IF_NUM), TSP_DBG("[TSP_ERROR][%06d] Bad PVR Engine ID\n", __LINE__));

    _TSP_ENTRY();
    HAL_TSP_PVR_SetBuffer(u8PVRId, phyBufStart0, phyBufStart1, u32BufSize0, u32BufSize1);
    _TSP_RETURN(DRVTSP_OK);

}

//-------------------------------------------------------------------------------------------------
/// Set Channel Browser record mode and START/STOP recording.
/// @param  u8PVRId                   \b IN:PVR ID
/// @param  eRecMode                \b IN: mode of recording
/// @param  bStart                     \b IN: TRUE(START), FALSE(STOP)
/// @return TSP_Result
/// @note
/// Parameter eRecMode will be ignored when bStart is FALSE(STOP)\n
/// @note
/// It's a synchronous function. When STOP, it flushs internal record fifo
/// and update current PVR record buffer write address before function return.\n
/// User can call MDrv_TSP_PVR1_GetWriteAddr to get the final valid data address
/// after recording.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_Start(MS_U8 u8PVRId, DrvTSP_RecMode eRecMode, MS_BOOL bStart)
{
    TSP_ASSERT(u8PVRId < ((MS_U8)TSP_PVR_IF_NUM), TSP_DBG("[TSP_ERROR][%06d] Bad PVR Engine ID\n", __LINE__));
    _TSP_ENTRY();

    if (bStart)
    {
        HAL_TSP_PVR_All(u8PVRId, (HAS_FLAG(eRecMode, 0x02UL))? TRUE: FALSE, (HAS_FLAG(eRecMode, 0x02UL))? TRUE: FALSE, TRUE);

        HAL_TSP_PVR_Enable(u8PVRId, TRUE);
    }
    else
    {
        HAL_TSP_PVR_Enable(u8PVRId, FALSE);
        HAL_TSP_PVR_WaitFlush(u8PVRId);
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get final write address of Channel Browser record buffer
/// @param  u8PVRId                         \b IN:PVR ID
/// @param  pphy2WriteAddr           \b OUT: pointer of address return
/// @return TSP_Result
/// @note
/// User can get current write address to know where is the end of section data
/// received in the section buffer.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_GetWriteAddr(MS_U8 u8PVRId, MS_PHY *pphy2WriteAddr)
{
    TSP_ASSERT(u8PVRId < ((MS_U8)TSP_PVR_IF_NUM), TSP_DBG("[TSP_ERROR][%06d] Bad PVR Engine ID\n", __LINE__));
    _TSP_ENTRY();
    *pphy2WriteAddr = HAL_TSP_PVR_GetBufWrite(u8PVRId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Subscribe event notification callback function for Channel Browser record buffer
/// @param  u8PVRId                 \b IN:PVR ID
/// @param  eEvents                 \b IN: events need to be subscribed\n
///                                        @ref E_DRVTSP_EVENT_CBBUF_FULL
/// @param  pfCallback              \b IN: callback function (NULL to disable)
/// @return TSP_Result
/// @note
/// TSP Channel Browser recording uses single buffer mechanism. This function register a callback
/// function for recording. TSP calls callback function each time when one of
/// single buffer is full and ring to buffer start to record.\n
/// @ref E_DRVTSP_FLT_TYPE_CB
/// @attention
/// Callback function resides in OS TSP interrupt context, it recommends
/// that callback function should not take too much time to block the system.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_Notify(MS_U8 u8PVRId, DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback)
{
    if(u8PVRId >= TSP_PVR_IF_NUM)
        return DRVTSP_FAIL;

    _TSP_ENTRY();

    if (pfCallback)
    {
        switch(u8PVRId)
        {
            case 0:
                HAL_TSP_Int_Enable(TSP_HWINT_HW_PVR1_MASK);
                break;
            case 1:
                HAL_TSP_Int2_Enable(TSP_HWINT_HW_PVR2_MASK);
                break;
            #if (TSP_PVR_IF_NUM > 2)
            case 2:
                HAL_TSP_Int2_Enable(TSP_HWINT_HW_PVRCB_MASK);
                break;
            #endif
            default:
                _TSP_RETURN(DRVTSP_FAIL);
        }
        _TSP_SetPvrNotifyId(u8PVRId);
    }
    else
    {
        switch(u8PVRId)
        {
            case 0:
                HAL_TSP_Int_Disable(TSP_HWINT_HW_PVR1_MASK);
                break;
            case 1:
                HAL_TSP_Int2_Disable(TSP_HWINT_HW_PVR2_MASK);
                break;
            #if (TSP_PVR_IF_NUM > 2)
            case 2:
                HAL_TSP_Int2_Disable(TSP_HWINT_HW_PVRCB_MASK);
                break;
            #endif
            default:
                _TSP_RETURN(DRVTSP_FAIL);
        }
        _TSP_RemovePvrNotifyId(u8PVRId);
    }

    _ptsp_res->_PvrEvtNotify[u8PVRId] = eEvents;
    _ptsp_res->_PvrCallback[u8PVRId] = pfCallback;

#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 u32ii;

    for(u32ii = 0; u32ii < DRVTSP_MAX_PROCESS_NUM; u32ii++)
    {
        if(_u32KPrcEvtTblUseFlag & (1 << u32ii))
        {
            if(_stKModePrcEvtTbl[u32ii].tgid == current->tgid)
            {
                _ptsp_res->_s32PvrEvtId[u8PVRId] = _stKModePrcEvtTbl[u32ii].s32TspEvtId;
                break;
            }
        }
    }
#else
    _ptsp_res->_s32PvrEvtId[u8PVRId] = _s32TspEventId;
#endif

    _TSP_RETURN(DRVTSP_OK);

}

TSP_Result MDrv_TSP_PVR_Eng_SetPacketMode(MS_U8 u8PVRId, MS_BOOL bSet)
{
    TSP_ASSERT(u8PVRId < TSP_PVR_IF_NUM, TSP_DBG("[TSP_ERROR][%06d] Bad PVR Engine ID\n", __LINE__));
    _TSP_ENTRY();
    HAL_TSP_PVR_PacketMode(u8PVRId, bSet);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Select PVR source
/// @param  u8PVRId              \b IN:PVR ID
/// @param  ePktSrc              \b IN: PVR packet demux source type
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SelSrc(MS_U8 u8PVRId, DrvTSP_PKTDMXSrcType ePktSrc)
{
    _TSP_ENTRY();
    HAL_TSP_PVR_Src_Select(u8PVRId, (MS_U32)ePktSrc);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable PVR FIFO block mode
/// @param  u8PVRId              \b IN:PVR ID
/// @param  ePktSrc              \b IN: PVR packet demux source type
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_BlockEnable(MS_U8 u8PVRId, MS_BOOL bEnable)
{
    _TSP_ENTRY();
    HAL_TSP_PVR_Fifo_Block_Disable(u8PVRId, !bEnable);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Channel Browser time stamp set
/// @param  u8PVRId              \b IN:PVR ID
/// @param  u32Stamp           \b IN: Channel Browser time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(MS_U8 u8PVRId, MS_U32 u32Stamp)
{
    _TSP_ENTRY();
    HAL_TSP_SetPVRTimeStamp(u8PVRId, u32Stamp);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Channel Browser time stamp get
/// @param  u8PVRId               \b IN:PVR ID
/// @param  pu32Stamp           \b OUT: pointer of Channel Browser time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(MS_U8 u8PVRId, MS_U32* u32Stamp)
{
    _TSP_ENTRY();
    *u32Stamp = HAL_TSP_GetPVRTimeStamp(u8PVRId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Playback time stamp Set
/// @param  pu32Stamp           \b IN: Playback time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp)
{
    _TSP_ENTRY();
    HAL_TSP_SetPlayBackTimeStamp(u32Stamp);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Playback time stamp get
/// @param  pu32Stamp           \b OUT: pointer of Playback time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp)
{
    _TSP_ENTRY();
    * u32Stamp = HAL_TSP_GetPlayBackTimeStamp();
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// PVR time stamp Enable/Disable
/// @param  pu32Stamp           \b IN: Enable/Disable time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStamp(MS_BOOL bEnable)
{
    _TSP_ENTRY();
    if (bEnable)
    {
        HAL_TSP_FileIn_Set(TRUE);
        //HAL_TSP_ResetTimeStamp();
    }
    else
    {
        HAL_TSP_FileIn_Set(FALSE);
    }
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Playback time stamp clock set
/// @param  u8EngId               \b IN:playback ID
/// @param  eClk           \b IN: clock source type
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SetPlaybackStampClk(MS_U32 u32EngId, MS_U32 u32Clk)
{
    HAL_TSP_SetPlayBackTimeStampClk(u32EngId, u32Clk);
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// PVR time stamp clock set
/// @param  u8PvrId               \b IN:PVR ID
/// @param  eClk           \b IN: clock source type
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SetRecordStampClk(MS_U32 u32PvrId, MS_U32 u32Clk)
{
    HAL_TSP_SetPVRTimeStampClk(u32PvrId, u32Clk);
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Check if PVR Engine is started
/// @param  u8PvrId                       \b IN:PVR ID
/// @param  pbIsStart                    \b OUT: Pointer to store started status
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_IsStart(MS_U32 u32PvrId, MS_BOOL *pbIsStart)
{
    if(pbIsStart == NULL)
        return DRVTSP_FAIL;

    *pbIsStart = HAL_TSP_PVR_IsEnabled(u32PvrId);

    return DRVTSP_OK;
}

//--- PVRCA API
// if the same PID is allocated, set record flag directly
TSP_Result MDrv_TSP_PVRCA_Close(MS_U8 u8PVRId)
{
    return (DRVTSP_OK);
}

// File API
//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream start address
/// @param  u32StreamAddr           \b IN: pointer of transport stream in memory
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetAddr(MS_PHY phyStreamAddr)
{
    _TSP_ENTRY();
    HAL_TSP_CmdQ_TsDma_SetAddr(phyStreamAddr);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream data size
/// @param  u32StreamSize           \b IN: size of transport stream data to be copied
/// @return TSP_Result
//  @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetSize(MS_U32 u32StreamSize)
{
    if(u32StreamSize <= 16)
    {
        return DRVTSP_FAIL;
    }

    _TSP_ENTRY();
    if (HAL_TSP_CmdQ_TsDma_SetSize(u32StreamSize))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Memory to TSP stream command : Update Stream STC
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32STC_32               \b IN: STC[32] for next input stream
// @param  u32STC                  \b IN: STC[31:0] for next input stream
// @return TSP_Result
// @note M2T Command Size: 3
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetSTC(MS_U32 u32EngId, MS_U32 u32STC_32, MS_U32 u32STC)
{
    _TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // STC register mapping is different between MCU and CPU.
    HAL_TSP_SetSTC(u32EngId, u32STC, u32STC_32);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Start stream input
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
/// @note Filein Command Size: 1
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Start(DrvTSP_FileinMode eFileinMode)
{
    _TSP_ENTRY();
    // @FIXME: Good to assume enum and register definition identical?
    //if (E_DRVTSP_FILEIN_MODE_ENG0_TS != eFileinMode)
    {
        HAL_TSP_PS_Path_Enable((MS_U32)eFileinMode);
    }
    HAL_TSP_CmdQ_TsDma_Start((MS_U32)eFileinMode);

    HAL_TSP_filein_enable(TRUE); //enable cmdQ after cmds are set

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stop stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Stop(void)
{
    MS_U8 u8ii;
    MS_U8 u8ScmSrcTsif;
    MS_U32 u32ScmDest;

    _TSP_ENTRY();

    HAL_TSP_filein_enable(FALSE);

    HAL_TSP_PS_Path_Disable();

    for(u8ii = 0; u8ii < ((MS_U8)TSP_CA_ENGINE_NUM); u8ii++)
    {
        HAL_TSP_CSA_Get_CACtrl(u8ii, &u8ScmSrcTsif, &u32ScmDest);

        if(u32ScmDest & TSP_PKTDMX0_FILE)
        {
            HAL_TSP_CSA_Set_CACtrl(u8ii, TSP_SRC_FROM_PKTDMX0, TSP_PKTDMX0_LIVE);
            break;
        }
        if(u8ScmSrcTsif & TSP_SRC_FROM_PKTDMX0)
        {
            HAL_TSP_CSA_Set_CACtrl(u8ii, u8ScmSrcTsif, u32ScmDest|TSP_PKTDMX0_LIVE);
            break;
        }
    }

    HAL_TSP_filein_enable(FALSE);

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : pause stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Pause(void)
{
    _TSP_ENTRY();
    HAL_TSP_TsDma_Pause();
    _TSP_RETURN(DRVTSP_OK);

}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : resume stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Resume(void)
{

    _TSP_ENTRY();
    HAL_TSP_TsDma_Resume();
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current file in state
/// @return DrvTSP_M2tStatus
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_GetState(DrvTSP_FileinState* peFileinState)
{
    MS_U32 state, cmdcnt;

    _TSP_ENTRY();

    state = HAL_TSP_CmdQ_TsDma_GetState();
    cmdcnt = HAL_TSP_CmdQ_CmdCount();

    if (state & TSP_CTRL1_FILEIN_PAUSE)
    {
        *peFileinState = E_DRVTSP_FILEIN_STATE_PAUSE;
    }
    else if ((state & TSP_TSDMA_CTRL_START) || cmdcnt)
    {
        *peFileinState = E_DRVTSP_FILEIN_STATE_BUSY;
    }
    else
    {
        *peFileinState = E_DRVTSP_FILEIN_STATE_IDLE;
    }
    _TSP_RETURN(DRVTSP_OK);

}

//-------------------------------------------------------------------------------------------------
/// Set Memory to TSP Stream Input Rate
/// @param  u32Div2                 \b IN: Divider of M2T stream input rate ([1 .. 31], default 10)
/// @return TSP_Result
/// @note
/// <b>input_rate = stream_rate / (u32Div2 * 2)</b>\n
/// @note
/// It's not recommend to change input rate at run-time, because it conflict with
/// the internal stream synchornization mechanism.
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetRate(MS_U32 u32Div2)
{
    _TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // TsDma pause can be access by TSP CPU
    // TsDma pause it's hard to control because read/write in different register
    // When setting TsDma it should be disable interrupt to prevent ISR access
    // but it still can't prevent TSP_cpu access at the same time.
    //[SW PATCH] Add a special firmware command to lock TSP_cpu DMA pause/resume.

    OSAL_TSP_IntDisable();
    HAL_TSP_TsDma_SetDelay(u32Div2);
    OSAL_TSP_IntEnable();

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set packet size to TSP file
/// @param  PacketMode                 \b IN: Mode of TSP file packet mode (192, 204, 188)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetPacketMode(DrvTSP_PacketMode PKT_Mode)
{
    _TSP_ENTRY();

    HAL_TSP_filein_enable(FALSE); //disable cmdQ before cmds are set

    switch (PKT_Mode)
    {
        case E_DRVTSP_PKTMODE_188:
            HAL_TSP_SetPKTSize(0xBBUL);
            break;
        case E_DRVTSP_PKTMODE_192:
            HAL_TSP_SetPKTSize(0xBFUL);
            break;
        case E_DRVTSP_PKTMODE_204:
            HAL_TSP_SetPKTSize(0xCBUL);
            break;
        case E_DRVTSP_PKTMODE_130:
            HAL_TSP_SetPKTSize(0x82UL);
            break;
        case E_DRVTSP_PKTMODE_134:
            HAL_TSP_SetPKTSize(0x86UL);
            break;
        default:
            break;
    }

    HAL_TSP_PS_Path_Disable();

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get the number of empty slot of M2T command
/// @param  pu32EmptySlot           \b OUT: pointer of empty slot count return
/// @return TSP_Result
/// @note
/// M2T is a command queue command, it can be queued by TSP when another M2T command
/// is executing by TSP. The queued commands will be executed by order (FIFO)
/// when previous M2T command execution is finished.
/// @note
/// User should call GetM2TSlot to make sure there is enough empty M2T command slot
/// before sending any M2T command. (Each command has different command size)
/// @sa MDrv_TSP_M2T_SetAddr, MDrv_TSP_M2T_SetSize, MDrv_TSP_M2T_Start,
//[Reserved]    MDrv_TSP_M2T_SetSTC
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_CMDQ_GetSlot(MS_U32 *pu32EmptySlot)
{
    _TSP_ENTRY();
    *pu32EmptySlot = HAL_TSP_CmdQ_EmptyCount();
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset command queue
/// @return TSP_Result
/// @note
/// SW patch--wait command queue empty
/// wait HW ECO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_CMDQ_Reset(void)
{
    MS_BOOL bRet;

    _TSP_ENTRY();
    bRet = HAL_TSP_CmdQ_Reset();
    if (TRUE == bRet)
    {
         _TSP_RETURN(DRVTSP_OK);
    }
    _TSP_RETURN(DRVTSP_FAIL);
}

//--------------------------------------------------------------------------------------------------
/// Get command queue fifo level
/// @param  pu8FifoLevel      \b OUT: fifo level, 0~3
/// @return TSP_Result
/// @note
//--------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel)
{
    _TSP_ENTRY();

    *pu8FifoLevel = HAL_TSP_Get_CmdQFifoLevel();

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable timestamp block scheme
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable)
{
    _TSP_ENTRY();
    HAL_TSP_FileIn_192BlockScheme_En(bEnable);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable PS path
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_PS_Path_Enable(DrvTSP_FileinMode eFileinMode)
{
    _TSP_ENTRY();
    HAL_TSP_PS_Path_Enable((MS_U32)eFileinMode);
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_File_TSIFSrcSel(DrvTSP_FltType eFltSrc, MS_BOOL bFileMode)
{
    MS_U32 u32Src = 0;

    _TSP_ENTRY();

    #ifdef MERGE_STR_SUPPORT
    if(eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        HAL_TSP_TSIFFI_SrcSelect(bFileMode);
        _TSP_RETURN(DRVTSP_OK);
    }
    #endif

    #if (TSP_IF_NUM > 1)
    if(eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TS1)
    {
        if(bFileMode == TRUE)
        {
            u32Src = TSP_PIDFLT1_USE_TSIF_MMFI0;
        }
        else
        {
            u32Src = TSP_PIDFLT1_USE_TSIF1;
        }
    }
    else if(eFltSrc == E_DRVTSP_FLT_SOURCE_TYPE_TS2)
    {
        if(bFileMode == TRUE)
        {
            u32Src = TSP_PIDFLT2_USE_TSIF_MMFI1;
        }
        else
        {
            u32Src = TSP_PIDFLT2_USE_TSIF2;
        }
    }
    #endif

    HAL_TSP_PidFlt_Src_Select(u32Src);
    _TSP_RETURN(DRVTSP_OK);
}

// Legacy
//-------------------------------------------------------------------------------------------------
/// Get Scmb states
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pScmbLevel             \b IN: TS interface setting
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Scmb_Status(MS_U32 u32EngId, DrvTSP_Scmb_Level* pScmbLevel)
{
    _TSP_ENTRY();
    *pScmbLevel=        E_DRVTSP_SCMB_NONE;

    if(HAL_TSP_Scmb_Status(0UL, 0UL, 0xFFFFFFFFUL))
    {
        *pScmbLevel=    E_DRVTSP_SCMB_TS;
        _TSP_RETURN(DRVTSP_OK);
    }
    if(HAL_TSP_Scmb_Status(0UL, 1UL, 0xFFFFFFFFUL))
    {
        *pScmbLevel=    E_DRVTSP_SCMB_TS;
        _TSP_RETURN(DRVTSP_OK);
    }
    if(HAL_TSP_Scmb_Status(0UL, 2UL, 0xFFFFFFFFUL))
    {
        *pScmbLevel=    E_DRVTSP_SCMB_TS;
        _TSP_RETURN(DRVTSP_OK);
    }
    if(HAL_TSP_Scmb_Status(0UL, 3UL, 0xFFFFFFFFUL))
    {
        *pScmbLevel=    E_DRVTSP_SCMB_TS;
        _TSP_RETURN(DRVTSP_OK);
    }
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_GetCap(DrvTSP_Cap eCap, void* pOutput)
{
    *((MS_U32*)pOutput) = 0UL;

    if (eCap >= E_DRVTSP_CAP_NULL)
        return DRVTSP_FAIL;

    if(eCap == E_DRVTSP_CAP_RESOURCE_SIZE)
    {
        *((MS_U32*)pOutput) = sizeof(TSP_RESOURCE_PRIVATE);
        return DRVTSP_OK;
    }

    if(HAL_TSP_GetCap((MS_U32)eCap, pOutput))
    {
        return DRVTSP_OK;
    }
    else
    {
        return DRVTSP_FAIL;
    }
}

TSP_Result MDrv_TSP_GetStatus(DrvTSP_HW_Status *HW_Status, DrvTSP_SW_Status *SW_Status)
{
    _TSP_ENTRY();
    if (HAL_TSP_Alive())
    {
        *HW_Status = E_DRVTSP_HW_ALIVE;
    }
    else
    {
        *HW_Status = E_DRVTSP_HW_DEALIVE;
    }
    *SW_Status = _ptsp_res->_TspState[0].TSPStatus;

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_SetDbgLevel(DrvTSP_DbgLevel DbgLevel)
{
    //TSP_DBG("[%s][%d] DbgLevel %u \n", __FUNCTION__, __LINE__, DbgLevel);
    _u32TSPDGBLevel = DbgLevel;

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_GetTSIFStatus(DrvTSP_If eIf, DrvTSP_If_Set* pIfSet, MS_U16* pu16Clk)
{
    TSP_Result Res = DRVTSP_OK;
    MS_U16 u16pad = 0, u16clk = 0;

    _TSP_STR_ENTRY();

    switch(eIf)
    {
        case E_DRVTSP_IF_TS0:
            HAL_TSP_GetTSIF_Status(0, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
        case E_DRVTSP_IF_TS1:
            HAL_TSP_GetTSIF_Status(1, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
        case E_DRVTSP_IF_TS2:
            HAL_TSP_GetTSIF_Status(2, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
        case E_DRVTSP_IF_TS3:
            HAL_TSP_GetTSIF_Status(3, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
        #ifdef MERGE_STR_SUPPORT
        case E_DRVTSP_IF_FI:
            HAL_TSP_GetTSIF_Status(0x80, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
        #endif
        default:
            _TSP_STR_RETURN(DRVTSP_NOT_SUPPORTED);
    }

    *pu16Clk = u16clk;

    switch(u16pad)
    {
        case TSP_MUX_INDEMOD:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_INDEMOD)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_DEMOD;
            break;
        case TSP_MUX_TS0:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS0)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT0;
            break;
        #if (TSP_TS_PAD_NUM >= 2)
        case TSP_MUX_TS1:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS1)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        #endif
        #if (TSP_TS_PAD_NUM >= 3)
        case TSP_MUX_TS2:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS2)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
        case TSP_MUX_TSCB:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TSCB)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        #endif
        #if defined(TSP_MUX_TS3)
        case TSP_MUX_TS3:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS3)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        #endif
        #if defined(TSP_MUX_TS4)
        case TSP_MUX_TS4:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS4)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT4;
            break;
        #endif
        #if defined(TSP_MUX_TS5)
        case TSP_MUX_TS5:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS5)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT5;
            break;
        #endif
        #if defined(TSP_MUX_TSO)
        case TSP_MUX_TSO:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TSOOUT)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_TSO;
            break;
        #endif
        default:
            break;
    }

    pIfSet->bClkInv = ((u16clk & TSP_CLK_INVERSE) == TSP_CLK_INVERSE);

    _TSP_STR_RETURN(Res);
}

//-------------------------------------------------------------------------------------------------
/// Get TSP driver version
/// @param <IN>        \b const MS_U8 **pVerString :
/// @param <RET>       \b   : when get ok, return the pointer to the driver version
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetLibVer(const MSIF_Version **ppVersion)
{
    // No mutex check, it can be called before Init
    if (!ppVersion)
    {
        return DRVTSP_FAIL;
    }

    *ppVersion = &_drv_tsp_version;

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId)
{
    _TSP_ENTRY();
    if (HAL_TSP_Alive())
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

inline TSP_Result MDrv_TSP_IsAccess(MS_U32 u32Try)
{
    return (HAL_TSP_TTX_IsAccess(u32Try))? DRVTSP_OK: DRVTSP_FAIL;
}

inline TSP_Result MDrv_TSP_UnlockAccess(void)
{
    HAL_TSP_TTX_UnlockAccess();
    return DRVTSP_OK;
}


// Following are 2 private function used to solve annoying interrupt thread disappearing problem when fork.
// Should not be exposed to general users.
MS_BOOL MDrv_TSP_CloseInterrupt(void)
{
    // IntDisable
    // IntDetach
    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pu32ResMemAddr                \b IN: Pointer to store resource memory address
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_InitLibResource(void *pResMemAddr)
{
    TSP_ASSERT((pResMemAddr != 0),
        TSP_DBG("[TSP_ERROR][%s][%06d] pointer is NULL!\n", __FUNCTION__, __LINE__));

    MS_VIRT virtBank, virtBank1;
    MS_PHY u32BankSize;
    MS_PHY u32BankSize1;

    _ptsp_res = (TSP_RESOURCE_PRIVATE*)pResMemAddr;

    if(_ptsp_res->_bSWInitReady != TRUE)
    {
        if(_TSP_InitResource(_ptsp_res) == FALSE)
            return DRVTSP_FAIL;
    }

    // For multi-process use case. (different process should set the value of bank again)
    if(_bBankInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_TSP))
        {
            MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
            MS_ASSERT(0);
        }
        if (FALSE == MDrv_MMIO_GetBASE(&virtBank1, &u32BankSize1, MS_MODULE_PM))
        {
            MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
            MS_ASSERT(0);
        }

        HAL_TSP_SetBank(virtBank, virtBank1);
        _bBankInit = TRUE;
    }

    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize TSP driver and TSP engine
/// @return TSP_Result
/// @note
/// It should be called before calling any other TSP DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Init(MS_PHY phyFWAddr, MS_U32 u32FWSize)
{
    MS_DEBUG_MSG(TSP_DBG ("******************start TSP init*********************************\n"));
    TSP_Result bRet;

#ifdef SECURE_PVR_ENABLE
    if(MDrv_TSP_Ree_TeeCmdSystem_Init() == DRVTSP_FAIL)
    {
        MS_DEBUG_MSG(TSP_DBG ("[%s] Register Mailbox failed!\n", __FUNCTION__));
        return DRVTSP_FAIL;
    }
#endif //SECURE_PVR_ENABLE

    //if already init, set the same HK parameter & interrupt num to this process, and attached Isr for this new process
#ifndef MSOS_TYPE_LINUX_KERNEL
    if((_ptsp_res->_bSWInitReady == TRUE) && (_s32TspTaskId != -1))
    {
        return MDrv_TSP_Reinit(_bIsHK);
    }
#endif //MSOS_TYPE_LINUX_KERNEL

    _ptsp_res->_phyFwAddr = phyFWAddr;
    _ptsp_res->_u32FwSize = u32FWSize;
    if (DRVTSP_OK != (bRet = _TSP_Init(TRUE, FALSE)))
    {
        return bRet;
    }
    MS_DEBUG_MSG(TSP_DBG ("**********************End TSP init*****************************\n"));

    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize TSP driver for hal parameters init and attaching Isr
/// @return TSP_Result
/// @note
/// Only for the second process to init
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Reinit(MS_BOOL bHK)
{
    MS_VIRT virtTspBank, virtTspBank1;
    MS_PHY u32TspBankSize;
    MS_PHY u32TspBankSize1;
    MS_U8  u8ii = 0, u8tmp;

    _bIsHK = bHK;
    _u32IntNum = (bHK) ? E_INT_IRQ_TSP2HK : E_INT_FIQ_TSP2AEON; // use this once MHEG5 using utopia msos

    if (FALSE == MDrv_MMIO_GetBASE(&virtTspBank, &u32TspBankSize, MS_MODULE_TSP))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
        MS_ASSERT(0);
    }
    if (FALSE == MDrv_MMIO_GetBASE(&virtTspBank1, &u32TspBankSize1, MS_MODULE_PM))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
        MS_ASSERT(0);
    }

    HAL_TSP_SetBank(virtTspBank, virtTspBank1);

    HAL_TSP_STC64_Mode_En(TRUE);

    _s32ExtMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_ext_mutex);

    if(_u32LibMode != DRVTSP_CMD_LIB_MODE_KRN)
    {
        _s32TspEventId = OSAL_TSP_EventCreate((MS_U8*)tsp_event);
    }

    u8tmp = (MS_U8)((TSP_SECFLT_NUM+31UL)>>5UL);
    for (u8ii = 0; u8ii < u8tmp; u8ii++)
    {
        _s32TspSecRdyId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_secRdy);
        _s32TspSecOvfId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_crcerr);
        _s32TspSecCrcId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_secovf);
        _s32TspFltDisableId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_fltdis);
        _u32SecNotifyId[u8ii] = 0;
    }

    u8tmp = (MS_U8)((TSP_PVR_IF_NUM+31UL)>>5UL);
    for (u8ii = 0; u8ii < u8tmp; u8ii++)
    {
        _u32PvrNotifyId[u8ii] = 0;
    }

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    u8tmp = (MS_U8)((TSP_PCRFLT_NUM+31UL)>>5UL);
    for (u8ii = 0; u8ii < u8tmp; u8ii++)
    {
        _u32HwPcrNotifyId[u8ii] = 0;
    }
#endif

    if(_u32LibMode != DRVTSP_CMD_LIB_MODE_KRN)
    {
        _s32TspTaskId = OSAL_TSP_TaskCreate(_TSP_Isr_Proc_Task, (MS_U8*)tsp_isr_task);
    }

     _ptsp_res->_u32ProcessNum++;


    // IntDisable
    // IntDetach
    OSAL_TSP_IntDisable();
//#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntDetach();
//#endif

    OSAL_TSP_IntAttach(_TSP_Isr);
//#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntEnable();
//#endif

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Reset(void)
{
    _TSP_RegStateReset();
    HAL_TSP_Reset(0);
    return DRVTSP_OK;
}

MS_BOOL MDrv_TSP_OpenInterrupt(void)
{
    OSAL_TSP_IntAttach(_TSP_Isr);
    OSAL_TSP_IntEnable();
    return TRUE;
}

TSP_Result MDrv_TSP_BurstLen(MS_U32 u32Len)
{
    _TSP_ENTRY();
    HAL_TSP_SetDMABurstLen(u32Len);
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_GetFileInTimeStamp(MS_U32* u32TSLen)
{
    _TSP_ENTRY();
    *u32TSLen = HAL_TSP_GetFileInTimeStamp();
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo)
{
    _TSP_ENTRY();
    *u32DbgInfo = HAL_TSP_GetDBGPortInfo(u32DbgSel);
    _TSP_RETURN(DRVTSP_OK);
}

//[LEGACY] //[OBSOLETE]
TSP_Result MDrv_TSP_SetHK(MS_BOOL bIsHK)
{
    //_TSP_ENTRY();
    _bIsHK = bIsHK;
    _u32IntNum = ((bIsHK) ? E_INT_IRQ_TSP2HK : E_INT_FIQ_TSP2AEON); // use this once MHEG5 using utopia msos

    return DRVTSP_OK;
    //_TSP_RETURN(DRVTSP_OK);
}
//[LEGACY] //[OBSOLETE]

TSP_Result MDrv_TSP_Get_FW_VER(MS_U32* u32FWVer)
{
    MS_U32 _u32fw;
    _u32fw = HAL_TSP_Get_FW_VER();
    *u32FWVer = _u32fw;
    return DRVTSP_OK;
}

void MDrv_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr)
{
    HAL_TSP_WriteProtect_Enable(bEnable, pphyStartAddr, pphyEndAddr);
}

void MDrv_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr)
{
    HAL_TSP_OrzWriteProtect_Enable(bEnable, phyStartAddr, phyEndAddr);
}

TSP_Result MDrv_TSP_TSIF_Enable(DrvTSP_If tsif, MS_BOOL bEnable)
{
    _TSP_ENTRY();
    switch (tsif)
    {
        case E_DRVTSP_IF_TS0:
            HAL_TSP_TSIF0_Enable(bEnable);
            break;
        case E_DRVTSP_IF_TS1:
            HAL_TSP_TSIF1_Enable(bEnable);
            break;
        default:
            ULOGE("TSP", "[%s][%d] IF not support\n",__FUNCTION__, __LINE__);
            break;
    }
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_ReadDropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt)
{
    _TSP_ENTRY();

    if(HAL_TSP_Read_DropPktCnt(pu16ADropCnt, pu16VDropCnt) == TRUE)
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

TSP_Result MDrv_TSP_Get_Intr_Count(MS_U32* pu32Count)
{
    *pu32Count = _ptsp_res->_u32DbgIntrCount;
    return DRVTSP_OK;
}

static void _DumpFwMsg(void)
{
    MS_S32 tmp = 0;
    MS_S32 len = 0;
    MS_VIRT vptr = 0;
    char p8buf[256];
    p8buf[255] = '\0';
    char* p = 0;

    MS_PHY phyWritePos = *(volatile MS_PHY*)MS_PA2KSEG1(_FwDbgInfo.phyBufAddr);
    vptr = MS_PA2KSEG1(_FwDbgInfo.phyBufAddr) + _FwDbgInfo.phyReadPos;
    p = (char*)vptr;
    //ULOGD("TSP", "_pFwDbgInfo->phyBufAddr = 0x%x\n", (unsigned int)_FwDbgInfo.phyBufAddr);
    //ULOGD("TSP", "_pFwDbgInfo->phyReadPos = 0x%x\n", (unsigned int)_FwDbgInfo.phyReadPos);
    //ULOGD("TSP", "_pFwDbgInfo->phyWritePos = 0x%x\n", (unsigned int)phyWritePos);
    if(phyWritePos > _FwDbgInfo.u32Size)
    {
        TSP_DBG("[%s], WARN : write pointer incorrect!!!\n", __FUNCTION__);
        return;
    }

    if(phyWritePos > _FwDbgInfo.phyReadPos)
    {
        while(1)
        {
            len = (phyWritePos - _FwDbgInfo.phyReadPos) > 255 ? 255 : (phyWritePos - _FwDbgInfo.phyReadPos);
            if((tmp = snprintf(p8buf, len, "%s", p)) == 0)
                break;
            tmp++;
            p+=tmp;
            _FwDbgInfo.phyReadPos += tmp;
            ULOGD("TSP", "[TSP] %s", p8buf);
        }
    }
    else if(phyWritePos < _FwDbgInfo.phyReadPos)
    {
        while(1)
        {
            len = (_FwDbgInfo.u32Size - _FwDbgInfo.phyReadPos) > 255? 255 : (_FwDbgInfo.u32Size - _FwDbgInfo.phyReadPos);
            if((tmp = snprintf(p8buf, len, "%s", p)) == 0)
                break;
            tmp++;
            p+=tmp;
            _FwDbgInfo.phyReadPos += tmp;
            ULOGD("TSP", "[TSP] %s", p8buf);
        }

        _FwDbgInfo.phyReadPos = (MS_PHY)sizeof(MS_U32);
        vptr = MS_PA2KSEG1(_FwDbgInfo.phyBufAddr) + _FwDbgInfo.phyReadPos;
        p = (char*)vptr;
        while(1)
        {
            len = (phyWritePos - _FwDbgInfo.phyReadPos) > 255 ? 255 : (phyWritePos - _FwDbgInfo.phyReadPos);
            if((tmp = snprintf(p8buf, len, "%s", p)) == 0)
                break;
            tmp++;
            p+=tmp;
            _FwDbgInfo.phyReadPos += tmp;
            ULOGD("TSP", "[TSP] %s", p8buf);
        }
    }
}

TSP_Result MDrv_TSP_SetFwDBGParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord)
{
    TSP_ASSERT((phyAddr != 0) && (u32Size != 0),
    TSP_DBG("[TSP_ERROR][%06d] memory information for debug buffer incorrect address : %u, size : %u\n", __LINE__, (unsigned int)phyAddr, (unsigned int)u32Size));

    _TSP_ENTRY();

    HAL_TSP_OrzWriteProtect_Enable(FALSE, 0, 0);
    if(HAL_TSP_SetFwDbgMem(phyAddr, u32Size) && HAL_TSP_SetFwDbgWord(u32DbgWord))
    {
        _FwDbgInfo.bEnable = TRUE;
        _FwDbgInfo.phyBufAddr         = phyAddr;
        _FwDbgInfo.u32Size            = u32Size;
        _FwDbgInfo.phyReadPos         = (MS_PHY)sizeof(MS_U32);
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        TSP_DBG("********HAL_TSP_SetFwDbgMem fail**************\n");
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

TSP_Result MDrv_TSP_ResetFileinTimestamp(void)
{
    HAL_TSP_ResetTimeStamp();
    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Enable PS path
/// @param  pphyReadAddr                \b OUT: Pointer to store file-in current reand pointer
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_GetReadAddr(MS_PHY *pphyReadAddr)
{
    _TSP_ENTRY();
    if(!HAL_TSP_GetFilinReadAddr(pphyReadAddr))
    {
        _TSP_RETURN(DRVTSP_NOT_SUPPORTED);
    }
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Virtual Queue buffer
/// @param  phyVQBaseAddr                \b IN: Virtual Queue memorry address
/// @param  u32VQBufLen_192PKT        \b IN: Number of packets for Virtual Queue (192 bytes per packet)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetVQueBuf(MS_PHY phyVQBaseAddr, MS_U32 u32VQBufLen)
{
    MS_U8 u8ii = 0;
#ifndef SECURE_PVR_ENABLE
    MS_U32 u32VQSize = 0;
    MS_PHY phyVQBufStart = phyVQBaseAddr;
    MS_U32 u32MiuAlign = 0;

    _TSP_STR_ENTRY();

    if(HAL_TSP_GetCap(HAL_TSP_CAP_VQ_ALIGN, (void*)&u32MiuAlign) == FALSE)
    {
        _TSP_STR_RETURN(DRVTSP_FAIL);
    }

    u32VQSize = ((u32VQBufLen / u32MiuAlign) / TSP_VQ_NUM) * u32MiuAlign ; //miu alignment

    for(u8ii = 0; u8ii < ((MS_U8)TSP_VQ_NUM); u8ii++)
    {
        HAL_TSP_SetVQBuffer(u8ii, phyVQBufStart, u32VQSize);
        HAL_TSP_VQueue_Reset(u8ii);
        phyVQBufStart += u32VQSize;
    }

#else
    _ptsp_res->_stSecBuf.u32BufId = 0;
    _ptsp_res->_stSecBuf.u32BufOpt = 0;
    _ptsp_res->_stSecBuf.u32BufSize = u32VQBufLen;
    _ptsp_res->_stSecBuf.phyBufAddr = phyVQBaseAddr;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_VqBuf, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        TSP_DBG("[MDRV TSP][%06d] Set VQ buf fail!\n", __LINE__);
        _TSP_STR_RETURN(DRVTSP_FAIL);
    }
    for(u8ii = 0; u8ii < ((MS_U8)TSP_VQ_NUM); u8ii++)
    {
        HAL_TSP_VQueue_Reset(u8ii);
    }

#endif  //SECURE_PVR_ENABLE

    _TSP_STR_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Virtual Quere
/// @param  bEnable                \b IN: Virtual Queue memorry Enable flag
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_VQueEnable(MS_BOOL bEnable)
{
    _TSP_STR_ENTRY();
    HAL_TSP_VQueue_Enable(bEnable);
    _TSP_STR_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset Virtual Quere
/// @param  u8VQID                           \b IN: Virtual Queue Index. 0: TS0, 1: File, 2: TS1, 3: TS3
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_VQueReset(MS_U8 u8VQID)
{
    _TSP_ENTRY();
    HAL_TSP_VQueue_Reset(u8VQID);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable Virtual Quere Overflow interrupt
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_VQueue_OverflowInt_En(MS_BOOL bEnable)
{
    MS_U8 u8ii = 0;

    _TSP_ENTRY();

    for(u8ii = 0; u8ii < ((MS_U8)TSP_VQ_NUM); u8ii++)
    {
        HAL_TSP_VQueue_OverflowInt_En(u8ii, bEnable);
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Virtual Quere request threshold
/// @param  u8req_len                        \b IN: Request length
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Set_VQ_Threshold(MS_U8 u8req_len)
{
    _TSP_ENTRY();
    HAL_TSP_Set_Req_VQ_RX_Threshold(u8req_len);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Virtual Quere Status
/// @param  pu32Status                     \b IN: Pointer to store Virtual Queue status.
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Get_VQStatus(MS_U32* pu32Status)
{
    _TSP_ENTRY();
    *pu32Status = HAL_TSP_Get_VQStatus();
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Select MOBF key
/// @param  u8Model                \b IN: MOBF path select. 0: File, 1: PVR1, 2: PVR2
/// @param  u8MobfIndex0        \b IN: Select key index of ring buffer 0
/// @param  u8MobfIndex1        \b IN: Select key index of ring buffer 1
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
// Model : 0 -> File, 1 -> PVR1, 2 -> PVR2
// u8MobfIndex0: 0 -> Disable, 1~31
// u8MobfIndex1: 0 -> Disable, 1~31
TSP_Result MDrv_TSP_Set_MOBF_Set(MS_U8 u8Model, MS_U8 u8MobfIndex0, MS_U8 u8MobfIndex1)
{
    _TSP_ENTRY();
    HAL_TSP_MOBF_Select(u8Model, u8MobfIndex0, u8MobfIndex1);
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_FLT_LiveSrcSwitch(DrvTSP_FltType eFltSrcType)
{
    return DRVTSP_FAIL;
}

//-------------------------------------------------------------------------------------------------
/// Hardware general config for driver interface
/// @param u32Cmd                   \b IN: commend
/// @param u32Config0               \b IN: configure value
/// @param u32Config1               \b IN: configure value
/// @param pData                      \b IN: pointer to store input data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData)
{
    if(u32Cmd == DRVTSP_CMD_STC_ADJUST_UNIT)
    {
        _ptsp_res->_u32StcAdjustUnit = u32Config0;
        return DRVTSP_OK;
    }
    else if(u32Cmd == DRVTSP_CMD_SET_LIB_MODE)
    {
        _u32LibMode = u32Config0;
        HAL_TSP_CMD_Run(u32Cmd, u32Config0, u32Config1,pData);
        return DRVTSP_OK;
    }
    else
    {
        _TSP_ENTRY();
        if(HAL_TSP_CMD_Run(u32Cmd, u32Config0, u32Config1,pData))
        {
            _TSP_RETURN(DRVTSP_OK);
        }
        else
        {
            _TSP_RETURN(DRVTSP_FAIL);
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the PES scrambling control status
/// @param  u32FltId                     \b IN: filter id
/// @param  pu8status                 \b OUT: pointer to store PES scrambled statis. Every bit indicates one status of one packet.
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPesScmbSts(MS_U32 u32FltId, MS_U8 *pu8status)
{
    _TSP_ENTRY();
    *pu8status = HAL_TSP_Get_PesScmb_Sts(u32FltId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get the TS header scrambling control status
/// @param  u8FltId                     \b IN: filter id
/// @param  pu8status                 \b OUT: pointer to store TS scrambled statis. Every bit indicates one status of one packet.
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetTsScmbSts(MS_U32 u32FltId, MS_U8 *pu8status)
{
    _TSP_ENTRY();
    *pu8status = HAL_TSP_Get_TsScmb_Sts(u32FltId);
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32Size)
{
    TSP_ASSERT((phyDataAddr & _TSP_QMEM_D_MASK),
        TSP_DBG("[TSP_ERROR][%06d] setFwDataAddr 0x%x <= 0x%x\n", __LINE__,(unsigned int)phyDataAddr, (unsigned int)(~_TSP_QMEM_D_MASK)));

    _TSP_ENTRY();

    HAL_TSP_OrzWriteProtect_Enable(TRUE, phyDataAddr, phyDataAddr+u32Size);
    HAL_TSP_SecFlt_SetDataAddr(phyDataAddr);

    _ptsp_res->_phyTspFWDramBuf = phyDataAddr;
    _ptsp_res->_u32TspFwDramBufSize = u32Size;

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_FQ_FltNullPkt(MS_U32 u32FQEng, MS_BOOL bFltNull)
{
    // not Implement
    return DRVTSP_OK;
}

//FQ Src Switch
TSP_Result MDrv_TSP_FQ_SetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf eTsSrc)
{
    MS_U32 u32FQSrc = 0;
    MS_U32 u32Ret = DRVTSP_OK;
    MS_U32 u32MapingFQEng = u32FQEng;

    TSP_ASSERT((u32FQEng < TSP_FIQ_NUM),
           TSP_DBG("[TSP_ERROR][%06d] Bad engin id(%d)\n", __LINE__, (int)u32FQEng));


    _TSP_ENTRY();

    switch(eTsSrc)
    {
        case E_DRVFQ_SRC_TSIF0:
            u32FQSrc = 0;
            break;
        case E_DRVFQ_SRC_TSIF1:
            u32FQSrc = 1;
            break;
        case E_DRVFQ_SRC_TSIF2:
            u32FQSrc = 2;
            break;
        case E_DRVFQ_SRC_TSIF3:
            u32FQSrc = 3;
            break;
        case E_DRVFQ_SRC_TSIFFI:
            u32FQSrc = 7;
            break;
        default:
            _TSP_RETURN(DRVTSP_FAIL);
    }

    if(!HAL_FQ_SetMuxSwitch(u32MapingFQEng,u32FQSrc))
        u32Ret = DRVTSP_FAIL;

    _TSP_RETURN(u32Ret);
}

TSP_Result MDrv_TSP_FQ_GetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf* peTsSrc)
{
    MS_U32 u32FQSrc = 0xFF;
    MS_U32 u32Ret = DRVTSP_OK;
    MS_U32 u32MapingFQEng = u32FQEng;

    TSP_ASSERT((u32FQEng < TSP_FIQ_NUM),
           TSP_DBG("[TSP_ERROR][%06d] Bad engin id(%d)\n", __LINE__, (int)u32FQEng));

    _TSP_ENTRY();

    u32FQSrc = HAL_FQ_GetMuxSwitch(u32MapingFQEng);

    switch(u32FQSrc)
    {
        case 0:
            *peTsSrc = E_DRVFQ_SRC_TSIF0;
            break;
        case 1:
            *peTsSrc = E_DRVFQ_SRC_TSIF1;
            break;
        case 2:
            *peTsSrc = E_DRVFQ_SRC_TSIF2;
            break;
        case 3:
            *peTsSrc = E_DRVFQ_SRC_TSIF3;
            break;
        case 7:
            *peTsSrc = E_DRVFQ_SRC_TSIFFI;
            break;
        default:
            *peTsSrc = 0xFF;
            _TSP_RETURN(DRVTSP_FAIL);
    }

    _TSP_RETURN(u32Ret);
}

// -------------------------------------------------------------
// Debug table
// -------------------------------------------------------------
#ifdef DEBUG_TABLE_SUPPORT
TSP_Result MDrv_TSP_Get_DisContiCnt(DrvTSP_DisContiCnt_info* tspInfo, MS_U32* pu32Cnt)
{
    TSP_DisconPktCnt_Info TspDisContiCntInfo;

    _TSP_ENTRY();

    memset(&TspDisContiCntInfo, 0, sizeof(TSP_DisconPktCnt_Info));
    TspDisContiCntInfo.TspCmd  = tspInfo->TspCmd;
    TspDisContiCntInfo.TspFifo = tspInfo->TspFifo;
    TspDisContiCntInfo.TspSrc  = tspInfo->TspSrc;

    *pu32Cnt = HAL_TSP_Get_DisContiCnt(&TspDisContiCntInfo);

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_Get_DropPktCnt(DrvTSP_DropPktCnt_info* tspInfo, MS_U32* pu32Cnt)
{
    TSP_DropPktCnt_Info TspDropPktCntInfo;

    _TSP_ENTRY();

    memset(&TspDropPktCntInfo, 0, sizeof(TSP_DisconPktCnt_Info));
    TspDropPktCntInfo.TspCmd  = tspInfo->TspCmd;
    TspDropPktCntInfo.TspFifo = tspInfo->TspFifo;
    TspDropPktCntInfo.TspSrc  = tspInfo->TspSrc;

    *pu32Cnt = HAL_TSP_Get_DropPktCnt(&TspDropPktCntInfo);

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_Get_LockPktCnt(DrvTSP_LockPktCnt_info* tspInfo, MS_U32* pu32Cnt)
{
    TSP_LockPktCnt_info TspLockCntInfo;

    _TSP_ENTRY();

    memset(&TspLockCntInfo, 0, sizeof(TSP_LockPktCnt_info));
    TspLockCntInfo.TspCmd  = tspInfo->TspCmd;
    TspLockCntInfo.TspTsif = tspInfo->TspTsif;

    *pu32Cnt = HAL_TSP_Get_LockPktCnt(&TspLockCntInfo);

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_Get_AVPktCnt(DrvTSP_AVPktCnt_info* tspInfo, MS_U32* pu32Cnt)
{
    TSP_AVPktCnt_info TspAVCntInfo;

    _TSP_ENTRY();

    memset(&TspAVCntInfo, 0, sizeof(TSP_AVPktCnt_info));
    TspAVCntInfo.TspCmd     = tspInfo->TspCmd;
    TspAVCntInfo.TspFifo    = tspInfo->TspFifo;
    TspAVCntInfo.TspFifoSrc = tspInfo->TspFifoSrc;

    *pu32Cnt = HAL_TSP_GetAVPktCnt(&TspAVCntInfo);

    _TSP_RETURN(DRVTSP_OK);
}
#endif

//-------------------------------------------------------------------------------------------------
/// Get TEI Section error Packet count
/// @param  FltSrc                          \b IN: Filter source of packet
/// @param  pu32PktCnt                 \b OUT: pointer to store TEI packet count value
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Get_SecTEI_PktCount(DrvTSP_FltType FltSrc, MS_U32* pu32PktCnt)
{
    _TSP_ENTRY();
    if(FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        *pu32PktCnt = HAL_TSP_Get_SecTEI_PktCount(1);
    }
    else
    {
        *pu32PktCnt = HAL_TSP_Get_SecTEI_PktCount(0);
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset Section TEI error Packet count
/// @param  FltSrc                          \b IN: Filter source of packet
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Reset_SecTEI_PktCount(DrvTSP_FltType FltSrc)
{
    _TSP_ENTRY();
    if(FltSrc == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        HAL_TSP_Reset_SecTEI_PktCount(1);
    }
    else
    {
        HAL_TSP_Reset_SecTEI_PktCount(0);
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Section Dis-continuity Packet count
/// @param  u32FltID                          \b IN: Filter source of packet
/// @param  pu32PktCnt                     \b OUT: pointer to store Dis-continuity packet count value
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Get_SecDisCont_PktCount(MS_U32 u32FltID, MS_U32* pu32PktCnt)
{
    _TSP_ENTRY();
    *pu32PktCnt = HAL_TSP_Get_SecDisCont_PktCount(u32FltID);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset Section Dis-continuity Packet count
/// @param  u32FltID                          \b IN: Filter source of packet
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Reset_SecDisCont_PktCount(MS_U32 u32FltID)
{
    _TSP_ENTRY();
    HAL_TSP_Reset_SecDisCont_PktCount(u32FltID);
    _TSP_RETURN(DRVTSP_OK);
}



//-------------------------------------------------------------------------------------------------
/// Scramble mask- enable mask of fifo/pvr for scramble stream
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_DropScmbPkt(DrvTSP_FltType eFilterType, MS_BOOL bEnable)
{
    _TSP_ENTRY();
    switch (eFilterType)
    {
        case E_DRVTSP_FLT_TYPE_VIDEO:
            HAL_TSP_DropScmbPkt(0, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO:
            HAL_TSP_DropScmbPkt(1, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO2:
            HAL_TSP_DropScmbPkt(2, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_VIDEO3D:
            HAL_TSP_DropScmbPkt(3, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO3:
            HAL_TSP_DropScmbPkt(4, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_AUDIO4:
            HAL_TSP_DropScmbPkt(5, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_PVR:
            HAL_TSP_DropScmbPkt(6, bEnable);
            break;
        case E_DRVTSP_FLT_TYPE_PVR1:
            HAL_TSP_DropScmbPkt(7, bEnable);
            break;
        default:
            _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
///Set or Get merge stream sync byte
/// @param eIf                                 \b IN:  Eunm value of TSP TSIF selection
/// @param u8StrId                           \b IN:  Stream index
/// @param pu8SyncByte                   \b IN:  Pointer to store sync byte of merege streams
/// @param bSet                              \b IN:  TRUE to setting data or FALSE to getting table
/// @return DRVTSP_OK   - Success
/// @return DRVTSP_FAIL - Failure
/// @note: Currently, maxmum number is 8, and don't call this API when stream processing is started
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_MStr_SyncByte(DrvTSP_If eIf, MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet)
{
#ifdef MERGE_STR_SUPPORT
    MS_U8 u8Path = 0;

    switch(eIf)
    {
        case E_DRVTSP_IF_TS0:
            u8Path = TSP_SRC_FROM_TSIF0_LIVE;
            break;
        case E_DRVTSP_IF_TS1:
            u8Path = TSP_SRC_FROM_TSIF1;
            break;
        case E_DRVTSP_IF_TS2:
            u8Path = TSP_SRC_FROM_TSIF2;
            break;
        case E_DRVTSP_IF_FI:
            u8Path = TSP_SRC_FROM_TSIF0_FILE;
            break;
        default:
            return DRVTSP_FAIL;
    }

    _TSP_ENTRY();
    if(HAL_TSP_Set_Sync_Byte(u8Path, u8StrId, pu8SyncByte, bSet) == FALSE)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);

#else

    return DRVTSP_FAIL;
#endif
}

//for kernel mode multiple process support, create eventid of callback thread of user mode in fitst time
#ifdef DMX_UTOPIA_2K
static char tspevt_name[] = "tspevt";
#endif
TSP_Result MDrv_TSP_Create_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef DMX_UTOPIA_2K
    MS_U32  u32ii = 0;

    for(u32ii = 0; u32ii < TSP_MAX_PVRSIGFLAG_NUM; u32ii++)
    {
        pstIoSignal->u32PvrBufFullFlag[u32ii] = 0;
    }
    for(u32ii = 0; u32ii < TSP_MAX_SIGFLAG_NUM; u32ii++)
    {
        pstIoSignal->u32SecRdyFlag[u32ii] = 0;
        pstIoSignal->u32SecOvfFlag[u32ii] = 0;
    }

    if(pstIoSignal->s32KerModeTspEvtId == -1)
    {
        if((pstIoSignal->s32KerModeTspEvtId = OSAL_TSP_EventCreate((MS_U8*)tspevt_name)) != -1)
        {
            for(u32ii = 0; u32ii < DRVTSP_MAX_PROCESS_NUM; u32ii++)
            {
                if((_u32KPrcEvtTblUseFlag & (1 << u32ii)) == 0)
                {
                    pstIoSignal->u32TblIndex = u32ii;
                    #ifdef MSOS_TYPE_LINUX_KERNEL
                    _stKModePrcEvtTbl[u32ii].tgid = current->tgid;
                    #endif
                    _stKModePrcEvtTbl[u32ii].s32TspEvtId = pstIoSignal->s32KerModeTspEvtId;
                    _u32KPrcEvtTblUseFlag |= (1 << u32ii);
                    break;
                }
            }

            //printf("[%s][%d] _u32KPrcEvtTblUseFlag %x , s32TspEvtId %x \n", __FUNCTION__, __LINE__, (int)_u32KPrcEvtTblUseFlag, (int)pstIoSignal->s32KerModeTspEvtId);

            return DRVTSP_OK;
        }
        else
        {
            return DRVTSP_FAIL;
        }
    }

    return DRVTSP_OK;
#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

// Send free event to end user mode callback thread, and delete event id
TSP_Result MDrv_TSP_Close_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef DMX_UTOPIA_2K

    #ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 u32ii = 0;

    pstIoSignal->s32KerModeTspEvtId = -1;
    for(u32ii = 0; u32ii < DRVTSP_MAX_PROCESS_NUM; u32ii++)
    {
        if((_u32KPrcEvtTblUseFlag & (1 << u32ii)) != 0)
        {
            if(_stKModePrcEvtTbl[u32ii].tgid == current->tgid)
            {
                pstIoSignal->s32KerModeTspEvtId = _stKModePrcEvtTbl[u32ii].s32TspEvtId;
                break;
            }
        }
    }
    if(pstIoSignal->s32KerModeTspEvtId == -1)
        return DRVTSP_FAIL;
    #else
    pstIoSignal->s32KerModeTspEvtId = _s32TspEventId;
    #endif

    //printf("%s][%d] s32KerModeTspEvtId %x\n", __FUNCTION__, __LINE__, (int)pstIoSignal->s32KerModeTspEvtId);

    //to free CB process task, so send one event
    _db_flag = 1;
    _TSP_SetEvent(pstIoSignal->s32KerModeTspEvtId, TSP_TASK_EVENT_FLT_FREE);
    MsOS_DelayTask(3);

    while(_db_flag != 2)
    {
        _TSP_SetEvent(pstIoSignal->s32KerModeTspEvtId, TSP_TASK_EVENT_FLT_FREE);
        MsOS_DelayTask(3);
    }

    while(OSAL_TSP_EventDelete(pstIoSignal->s32KerModeTspEvtId) == FALSE)
    {
        _TSP_SetEvent(pstIoSignal->s32KerModeTspEvtId, TSP_TASK_EVENT_FLT_FREE);
        printf("Waiting for evet deleting... \n");
        MsOS_DelayTask(3);
    }
    pstIoSignal->s32KerModeTspEvtId = -1;

    #ifdef MSOS_TYPE_LINUX_KERNEL
    _u32KPrcEvtTblUseFlag &= ~(1 << u32ii);
    _stKModePrcEvtTbl[u32ii].s32TspEvtId = -1;
    _stKModePrcEvtTbl[u32ii].tgid = 0;
    #endif

#endif

    return DRVTSP_OK;
}

//-------------------------------------------------------------------------------------------------------
// Refer to _TSP_Isr_ProcSelf, this is for user mode to getting events when using kernel dmx driver
// Plese update both  _TSP_Isr_ProcSelf & MDrv_TSP_Get_IoSignal at the same time for synchrony ISR evets processing flow
//-------------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Wait_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef DMX_UTOPIA_2K
    MS_U32  u32ii, u32Evt = 0;
    MS_S32  s32Res = 0;

    for(u32ii = 0; u32ii < TSP_MAX_PVRSIGFLAG_NUM; u32ii++)
    {
        pstIoSignal->u32PvrBufFullFlag[u32ii] = 0;
    }
    for(u32ii = 0; u32ii < TSP_MAX_SIGFLAG_NUM; u32ii++)
    {
        pstIoSignal->u32SecRdyFlag[u32ii] = 0;
        pstIoSignal->u32SecOvfFlag[u32ii] = 0;
    }

    if((s32Res = MsOS_WaitEvent_Interrupt(pstIoSignal->s32KerModeTspEvtId, TSP_TASK_EVENT_GROUP_SELF|TSP_TASK_EVENT_FLT_FREE, &u32Evt, E_OR_CLEAR, MSOS_WAIT_FOREVER)) <= 0)
    {
        pstIoSignal->u32TspEvt = 0;

        if(s32Res < 0)
            return DRVTSP_FUNC_ERROR;
        else
            return DRVTSP_FAIL;
    }

    if((_db_flag == 1) && (u32Evt & TSP_TASK_EVENT_FLT_FREE))
        _db_flag = 2;

    pstIoSignal->u32TspEvt = u32Evt;
    //printf("TSP", "u32TspEvt %lx\n", pstIoSignal->u32TspEvt);

#endif  //end of MSOS_TYPE_LINUX_KERNEL

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Proc_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{

#ifdef DMX_UTOPIA_2K
    TSP_Result Ret = DRVTSP_OK;
    MS_U32 u32ii;
    MS_U32 u32GroupNum = (TSP_SECFLT_NUM + 31UL) >> 5UL;
    OSAL_TSP_ProcTypeParam Data;

    Data.u32FltId = 0xFFFFFFFFUL;
    Data.pRet = (void*)&Ret;
    Data.bInfiniteWait = TRUE;
    Data.bFromTask = TRUE;
    Data.u32PrcEvt = TSP_TASK_EVENT_FLT_FREE;

    if(pstIoSignal->u32TspEvt & TSP_TASK_EVENT_FLT_FREE)
    {
        _TSP_Isr_ProcSelf(&Data);
        pstIoSignal->u32TspEvt &= ~TSP_TASK_EVENT_FLT_FREE;
    }

    #ifndef NO_PVR
    // set PVR buffer events
    if (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_PVR0_RDY_POLL|TSP_TASK_EVENT_PVR1_RDY_POLL|TSP_TASK_EVENT_PVR0_RDY_SELF|TSP_TASK_EVENT_PVR1_RDY_SELF))
    {
        if (_ptsp_res->_PvrEvtNotify[0] && _ptsp_res->_PvrCallback[0])
        {
            switch(pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL | TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF))
            {
                case TSP_TASK_EVENT_PVR0_RDY_SELF:
                case TSP_TASK_EVENT_PVR0_RDY_POLL:
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                    {
                        pstIoSignal->u32PvrBufFullFlag[0] = 1UL;
                    }
                    break;
                case TSP_TASK_EVENT_PVR1_RDY_SELF:
                case TSP_TASK_EVENT_PVR1_RDY_POLL:
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                    {
                        pstIoSignal->u32PvrBufFullFlag[0] |= 2UL;
                    }
                    break;
                case (TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF):
                case (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL):
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                    {
                        pstIoSignal->u32PvrBufFullFlag[0] = 0xFFUL;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    #if (TSP_PVR_IF_NUM > 1)
    if (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_PVR2PVR0_RDY_POLL|TSP_TASK_EVENT_PVR2PVR1_RDY_POLL|TSP_TASK_EVENT_PVR2PVR0_RDY_SELF|TSP_TASK_EVENT_PVR2PVR1_RDY_SELF))
    {
        if (_ptsp_res->_PvrEvtNotify[1] && _ptsp_res->_PvrCallback[1])
        {
            switch (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_PVR2PVR0_RDY_POLL|TSP_TASK_EVENT_PVR2PVR1_RDY_POLL|TSP_TASK_EVENT_PVR2PVR0_RDY_SELF|TSP_TASK_EVENT_PVR2PVR1_RDY_SELF))
            {
                case TSP_TASK_EVENT_PVR2PVR0_RDY_SELF:
                case TSP_TASK_EVENT_PVR2PVR0_RDY_POLL:
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_PVRBUF_FULL))
                    {
                        pstIoSignal->u32PvrBufFullFlag[1] = 1UL;
                    }
                    break;
                case TSP_TASK_EVENT_PVR2PVR1_RDY_SELF:
                case TSP_TASK_EVENT_PVR2PVR1_RDY_POLL:
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_PVRBUF_FULL))
                    {
                        pstIoSignal->u32PvrBufFullFlag[1] = 2UL;
                    }
                    break;
                case (TSP_TASK_EVENT_PVR2PVR0_RDY_SELF | TSP_TASK_EVENT_PVR2PVR1_RDY_SELF):
                case (TSP_TASK_EVENT_PVR2PVR0_RDY_POLL | TSP_TASK_EVENT_PVR2PVR1_RDY_POLL):
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                    {
                        pstIoSignal->u32PvrBufFullFlag[1] = 0xFFUL;
                    }
                    break;
            }
        }
    }
    #endif //endof TS2_IF_SUPPORT

    #ifdef TS3_IF_SUPPORT
    if (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_CBPVR0_RDY_SELF|TSP_TASK_EVENT_CBPVR0_RDY_POLL))
    {
        if (_ptsp_res->_PvrEvtNotify[2] && _ptsp_res->_PvrCallback[2])
        {
            switch (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_CBPVR0_RDY_SELF | TSP_TASK_EVENT_CBPVR0_RDY_POLL))
            {
            case TSP_TASK_EVENT_CBPVR0_RDY_SELF:
            case TSP_TASK_EVENT_CBPVR0_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[2], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    pstIoSignal->u32PvrBufFullFlag[2] = 1UL;
                }
                break;
        }
    }
    #endif

#endif //end of NO_PVR

    if (pstIoSignal->u32TspEvt & TSP_TASK_EVENT_SECTION_SELF)
    {
        for(u32ii = 0; u32ii < u32GroupNum; u32ii++)
        {
            MDrv_TSP_Get_SecEvents(u32ii, pstIoSignal);
        }
    }

#endif //end of MSOS_TYPE_LINUX_KERN

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Get_SecEvents(MS_U32 u32Group, DrvTSP_IoSignal *pstIoSignal)
{
    MS_U32 u32PidFltGroup = 0, u32SecFlt = 0, u32PidFlt = 0;
    MS_U32 u32RdyMask = 0, u32OvfMask = 0;

    _TSP_GetModeSecRdy(u32Group, 0xFFFFFFFFUL, &u32RdyMask, OSAL_EVENT_OR_CLEAR);
    _TSP_GetModeSecOvf(u32Group, 0xFFFFFFFFUL, &u32OvfMask, OSAL_EVENT_OR_CLEAR);
    //pstIoSignal->u32SecRdyFlag[u32Group] = u32RdyMask;
    //pstIoSignal->u32SecOvfFlag[u32Group] = u32OvfMask;

    //ULOGD("TSP", "[DMX] %ld , %lx, %lx \n", u32Group, u32RdyMask, u32OvfMask);

    u32SecFlt = u32Group << 5UL;
    while(u32RdyMask)
    {
        if(u32RdyMask & 0x01UL)
        {
            if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltMap[u32SecFlt] != u32SecFlt)
            {
                for(u32PidFlt = 0UL; u32PidFlt < TSP_PIDFLT_NUM_ALL; u32PidFlt++)
                {
                    if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltMap[u32PidFlt] == u32SecFlt)
                    {
                        break;
                    }
                }
            }
            else
            {
                u32PidFlt = u32SecFlt;
            }
            if(u32PidFlt < TSP_PIDFLT_NUM_ALL)
            {
                if((_ptsp_res->_TspState[pstIoSignal->u32Eng].FltState[u32PidFlt] & E_DRVTSP_FLT_STATE_ENABLE)&&
                     (_ptsp_res->_TspState[pstIoSignal->u32Eng].SecFltEvtNotify[u32SecFlt] & E_DRVTSP_EVENT_DATA_READY) &&
                     (_ptsp_res->_TspState[pstIoSignal->u32Eng].SecFltCallback[u32SecFlt]))
                {
                    u32PidFltGroup = u32PidFlt >> 5UL;
                    pstIoSignal->u32SecRdyFlag[u32PidFltGroup] |= (1UL << (u32PidFlt & 0x1FUL));
                }
            }
        }
        u32RdyMask >>= 1UL;
        u32SecFlt++;
    }

    u32SecFlt = u32Group << 5UL;
    while(u32OvfMask)
    {
        if(u32OvfMask & 0x01UL)
        {
            if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltMap[u32SecFlt] != u32SecFlt)
            {
                for(u32PidFlt = 0UL; u32PidFlt < TSP_PIDFLT_NUM_ALL; u32PidFlt++)
                {
                    if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltMap[u32PidFlt] == u32SecFlt)
                    {
                        break;
                    }
                }
            }
            else
            {
                u32PidFlt = u32SecFlt;
            }
            if(u32PidFlt < TSP_PIDFLT_NUM_ALL)
            {
                if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltState[u32PidFlt] & E_DRVTSP_FLT_STATE_ENABLE)
                {
                    _TSP_IND_LOCK();    // disable overflow filter
                    _TSP_PidFlt_Disable(pstIoSignal->u32Eng, u32PidFlt);  // ???? move out --> OK  // // critical section
                    _TSP_IND_UNLOCK();
                }
                if ((_ptsp_res->_TspState[pstIoSignal->u32Eng].FltState[u32PidFlt] != E_DRVTSP_FLT_STATE_FREE) &&
                    (_ptsp_res->_TspState[pstIoSignal->u32Eng].SecFltEvtNotify[u32SecFlt] & E_DRVTSP_EVENT_BUF_OVERFLOW) &&
                    (_ptsp_res->_TspState[pstIoSignal->u32Eng].SecFltCallback[u32SecFlt]))
                {
                    u32PidFltGroup = u32PidFlt >> 5UL;
                    pstIoSignal->u32SecOvfFlag[u32PidFltGroup] |= (1UL << (u32PidFlt & 0x1FUL));
                }
            }
        }
        u32OvfMask >>= 1UL;
        u32SecFlt++;
    }

    return DRVTSP_OK;
}

#ifdef SECURE_PVR_ENABLE

TSP_Result MDrv_TSP_Ree_TeeCmdSystem_Init()
{
    MDrv_SYS_ReadKernelCmdLine();
    MDrv_SYS_GetTEEInfo(&_ptsp_res->_teemode);
    //printf("[%s][%d] _teemode %x \n", __FUNCTION__, __LINE__, (int)_ptsp_res->_teemode.OsType);

    //_ptsp_res->_teemode.OsType = SYS_TEEINFO_OSTYPE_OPTEE;
    if(_ptsp_res->_teemode.OsType == SYS_TEEINFO_OSTYPE_NUTTX)
    {
        if(_TSP_REE_RegisterMBX() == FALSE)
        {
            MS_DEBUG_MSG(TSP_DBG ("[%s] Register Mailbox failed!\n", __FUNCTION__));
            return DRVTSP_FAIL;
        }
    }

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Ree_TeeCmdSystem_Exit()
{
    _bTSPMbxInitFlag = FALSE;
    _u8TSPMbxMsgClass = 0;

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Ree_SendTeeCmd(TSP_REE_TO_TEE_CMD_TYPE cmd_type, void* param, size_t datasize)
{
    //printf("[%s][%d] OsType %x , cmd_type %x\n", __FUNCTION__, __LINE__, _ptsp_res->_teemode.OsType, (int)cmd_type);
    //printf("sizeof PHY %d \n", sizeof(MS_PHY));

    if(_ptsp_res->_teemode.OsType == SYS_TEEINFO_OSTYPE_NUTTX)
    {
        if(_TSP_Ree_SendMBXMsg(cmd_type, param) == TRUE)
            return DRVTSP_OK;
        else
            return DRVTSP_FAIL;
    }
    else if(_ptsp_res->_teemode.OsType == SYS_TEEINFO_OSTYPE_OPTEE)
    {
        unsigned int ret, ret_orig= 0;

        if ((ret = MDrv_SYS_TEEC_InitializeContext(_device, &mstar_teec_ctx)) != TEEC_SUCCESS)
        {
            printf("%s %d: Init Context failed, err code: %x\n",__FUNCTION__,__LINE__, (int)ret);
            return DRVTSP_FAIL;
        }
        if ((ret = MDrv_SYS_TEEC_Open(&mstar_teec_ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &ret_orig)) != TEEC_SUCCESS)
        {
            printf("%s %d: TEEC Open session failed, err code: %x\n",__FUNCTION__,__LINE__, (int)ret);
            return DRVTSP_FAIL;
        }
        if(datasize == 0)
        {
            op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,TEEC_NONE, TEEC_NONE);
        }
        else
        {
            op.params[0].tmpref.buffer = param;
            op.params[0].tmpref.size = datasize;
            op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
        }

        if ((ret = MDrv_SYS_TEEC_InvokeCmd(&session, cmd_type, &op, &ret_orig)) != TEEC_SUCCESS)
        {
            printf("%s %d: TEEC Invoke command failed, err code: %x\n",__FUNCTION__,__LINE__, (int)ret);
        }
        MDrv_SYS_TEEC_Close(&session);
        MDrv_SYS_TEEC_FinalizeContext(&mstar_teec_ctx);
        if(ret == TEEC_SUCCESS)
        {
            return DRVTSP_OK;
        }
    }

    return DRVTSP_FAIL;
}

#endif //SECURE_PVR_ENABLE
